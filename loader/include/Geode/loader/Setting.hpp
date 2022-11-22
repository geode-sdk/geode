#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/Result.hpp>
#include <Geode/utils/container.hpp>
#include <Geode/utils/convert.hpp>
#include <Geode/utils/json.hpp>
#include <Geode/utils/platform.hpp>
#include <Geode/utils/ranges.hpp>
#include <optional>
#include <regex>
#include <unordered_set>

#pragma warning(push)
#pragma warning(disable : 4275)

namespace geode {
    using ModJson = nlohmann::ordered_json;

    class Setting;
    class SettingNode;
    class BoolSetting;
    class IntSetting;
    class FloatSetting;
    class StringSetting;

    struct ModInfo;

    enum class SettingType {
        Bool,
        Int,
        Float,
        String,
        File,
        Color,
        ColorAlpha,
        User,
    };

    /**
     * Base class for all settings in Geode mods. Note that for most purposes
     * you should use the built-in setting types. If you need a custom setting
     * type however, inherit from this class. Do note that you are responsible
     * for things like storing the default value, broadcasting value change
     * events, making the setting node etc.
     */
    class Setting : public std::enable_shared_from_this<Setting> {
    protected:
        std::string m_key;
        std::string m_modID;

        friend struct ModInfo;

        static Result<std::shared_ptr<Setting>> parse(
            std::string const& type, std::string const& key, JsonMaybeObject<ModJson>& obj
        );

    public:
        GEODE_DLL virtual ~Setting() = default;

        // Load from mod.json
        GEODE_DLL static Result<std::shared_ptr<Setting>> parse(
            std::string const& key, ModJson const& json
        );
        // Load value from saved settings
        virtual bool load(nlohmann::json const& json) = 0;
        // Save setting value
        virtual bool save(nlohmann::json& json) const = 0;

        virtual SettingNode* createNode(float width) = 0;

        GEODE_DLL void valueChanged();

        GEODE_DLL std::string getKey() const;
        virtual SettingType getType() const = 0;
    };

    // built-in settings' implementation details
    namespace {
#define GEODE_INT_PARSE_SETTING_IMPL(obj, func, ...)              \
    if constexpr (std::is_base_of_v<__VA_ARGS__, Class>) {        \
        auto r = std::static_pointer_cast<Class>(res)->func(obj); \
        if (!r) return Err(r.error());                            \
    }

#define GEODE_INT_CONSTRAIN_SETTING_CAN_IMPL(func, ...)    \
    if constexpr (std::is_base_of_v<__VA_ARGS__, Class>) { \
        auto res = static_cast<Class*>(this)->func(value); \
        if (!res) {                                        \
            return res;                                    \
        }                                                  \
    }

        template <class ValueType>
        class IMinMax;
        template <class Class, class ValueType>
        class IOneOf;
        template <class Class, class ValueType>
        class IMatch;

        class ICArrows;
        template <class ValueType>
        class ICSlider;
        class ICInput;
        class ICFileFilters;

        template <class Class, class ValueType, SettingType Type>
        class GeodeSetting : public Setting {
        protected:
            ValueType m_default;
            ValueType m_value;
            std::optional<std::string> m_name;
            std::optional<std::string> m_description;
            bool m_canResetToDefault = true;

            friend class Setting;

            static Result<std::shared_ptr<Class>> parse(
                std::string const& key, JsonMaybeObject<ModJson>& obj
            ) {
                auto res = std::make_shared<Class>();

                res->m_key = key;
                obj.needs("default").into(res->m_default);
                obj.has("name").intoAs<std::string>(res->m_name);
                obj.has("description").intoAs<std::string>(res->m_description);
                GEODE_INT_PARSE_SETTING_IMPL(obj, parseMinMax, IMinMax<ValueType>);
                GEODE_INT_PARSE_SETTING_IMPL(obj, parseOneOf, IOneOf<Class, ValueType>);
                GEODE_INT_PARSE_SETTING_IMPL(obj, parseMatch, IMatch<Class, ValueType>);
                res->setValue(res->m_default);

                if (auto controls = obj.has("control").obj()) {
                    // every built-in setting type has a reset button
                    // by default
                    controls.has("can-reset").into(res->m_canResetToDefault);
                    GEODE_INT_PARSE_SETTING_IMPL(controls, parseArrows, ICArrows);
                    GEODE_INT_PARSE_SETTING_IMPL(controls, parseSlider, ICSlider<ValueType>);
                    GEODE_INT_PARSE_SETTING_IMPL(controls, parseInput, ICInput);
                    GEODE_INT_PARSE_SETTING_IMPL(controls, parseFileFilters, ICFileFilters);
                }

                return Ok(res);
            }

        public:
            using value_t = ValueType;

            std::optional<std::string> getName() const {
                return m_name;
            }

            std::string getDisplayName() const {
                return m_name.value_or(m_key);
            }

            std::optional<std::string> getDescription() const {
                return m_description;
            }

            ValueType getDefault() const {
                return m_default;
            }

            ValueType getValue() const {
                return m_value;
            }

            void setValue(ValueType const& value) {
                m_value = value;
                if constexpr (std::is_base_of_v<IMinMax<ValueType>, Class>) {
                    (void)static_cast<Class*>(this)->constrainMinMax(m_value);
                }
                if constexpr (std::is_base_of_v<IOneOf<Class, ValueType>, Class>) {
                    (void)static_cast<Class*>(this)->constrainOneOf(m_value);
                }
                if constexpr (std::is_base_of_v<IMatch<Class, ValueType>, Class>) {
                    (void)static_cast<Class*>(this)->constrainMatch(m_value);
                }
                this->valueChanged();
            }

            Result<> isValidValue(ValueType value) {
                GEODE_INT_CONSTRAIN_SETTING_CAN_IMPL(constrainMinMax, IMinMax<ValueType>);
                GEODE_INT_CONSTRAIN_SETTING_CAN_IMPL(constrainOneOf, IOneOf<Class, ValueType>);
                GEODE_INT_CONSTRAIN_SETTING_CAN_IMPL(constrainMatch, IMatch<Class, ValueType>);
                return Ok();
            }

            bool load(nlohmann::json const& json) override {
                auto rawJson = json;
                JsonChecker(rawJson).root("[setting value]").into(m_value);
                return true;
            }

            bool save(nlohmann::json& json) const override {
                json = m_value;
                return true;
            }

            bool canResetToDefault() const {
                return m_canResetToDefault;
            }

            SettingType getType() const override {
                return Type;
            }
        };

        template <class ValueType>
        class IMinMax {
        protected:
            std::optional<ValueType> m_min = std::nullopt;
            std::optional<ValueType> m_max = std::nullopt;

        public:
            Result<> constrainMinMax(ValueType& value) {
                if (m_min && value < m_min.value()) {
                    value = m_min.value();
                    return Err(
                        "Value must be between " + std::to_string(m_min.value()) + " and " +
                        std::to_string(m_max.value())
                    );
                }
                if (m_max && value > m_max.value()) {
                    value = m_max.value();
                    return Err(
                        "Value must be between " + std::to_string(m_min.value()) + " and " +
                        std::to_string(m_max.value())
                    );
                }
                return Ok();
            }

            Result<> parseMinMax(JsonMaybeObject<ModJson>& obj) {
                obj.has("min").intoAs<ValueType>(m_min);
                obj.has("max").intoAs<ValueType>(m_max);
                return Ok();
            }

            std::optional<ValueType> getMin() const {
                return m_min;
            }

            std::optional<ValueType> getMax() const {
                return m_max;
            }
        };

        template <class Class, class ValueType>
        class IOneOf {
        protected:
            std::optional<std::unordered_set<ValueType>> m_oneOf = std::nullopt;

        public:
            Result<> constrainOneOf(ValueType& value) {
                if (m_oneOf && !m_oneOf.value().count(value)) {
                    value = static_cast<Class*>(this)->getDefault();
                    return Err(
                        "Value must be one of " + utils::ranges::join(m_oneOf.value(), ", ")
                    );
                }
                return Ok();
            }

            Result<> parseOneOf(JsonMaybeObject<ModJson>& obj) {
                std::unordered_set<ValueType> oneOf {};
                for (auto& item : obj.has("one-of").iterate()) {
                    oneOf.insert(item.get<ValueType>());
                }
                if (oneOf.size()) {
                    m_oneOf = oneOf;
                }
                return Ok();
            }

            auto getOneOf() const {
                return m_oneOf;
            }
        };

        template <class Class, class ValueType>
        class IMatch {
        protected:
            std::optional<ValueType> m_matchRegex = std::nullopt;

        public:
            Result<> constrainMatch(ValueType& value) {
                if (m_matchRegex) {
                    auto regex = std::regex(m_matchRegex.value());
                    if (!std::regex_match(value, regex)) {
                        value = static_cast<Class*>(this)->getDefault();
                        return Err("Value must match regex " + m_matchRegex.value());
                    }
                }
                return Ok();
            }

            Result<> parseMatch(JsonMaybeObject<ModJson>& obj) {
                obj.has("match").intoAs<ValueType>(m_matchRegex);
                return Ok();
            }

            std::optional<ValueType> getMatch() const {
                return m_matchRegex;
            }
        };

#define GEODE_INT_DECL_SETTING_CONTROL(Name, name, default, json) \
    class IC##Name {                                              \
    protected:                                                    \
        bool m_##name = default;                                  \
                                                                  \
    public:                                                       \
        Result<> parse##Name(JsonMaybeObject<ModJson>& obj) {     \
            obj.has(json).into(m_##name);                         \
            return Ok();                                          \
        }                                                         \
        bool has##Name() const { return m_##name; }               \
    }

        class ICArrows {
        protected:
            bool m_hasArrows = true;
            bool m_hasBigArrows = false;
            size_t m_arrowStep = 1;
            size_t m_bigArrowStep = 5;

        public:
            Result<> parseArrows(JsonMaybeObject<ModJson>& obj) {
                obj.has("arrows").into(m_hasArrows);
                obj.has("arrow-step").into(m_arrowStep);
                obj.has("big-arrows").into(m_hasBigArrows);
                obj.has("big-arrow-step").into(m_bigArrowStep);
                return Ok();
            }

            bool hasArrows() const {
                return m_hasArrows;
            }

            bool hasBigArrows() const {
                return m_hasBigArrows;
            }

            size_t getArrowStepSize() const {
                return m_arrowStep;
            }

            size_t getBigArrowStepSize() const {
                return m_bigArrowStep;
            }
        };

        template <class ValueType>
        class ICSlider {
        protected:
            bool m_hasSlider = true;
            std::optional<ValueType> m_sliderStep = std::nullopt;

        public:
            Result<> parseSlider(JsonMaybeObject<ModJson>& obj) {
                obj.has("slider").into(m_hasSlider);
                obj.has("slider-step").intoAs<ValueType>(m_sliderStep);
                return Ok();
            }

            bool hasSlider() const {
                return m_hasSlider;
            }

            std::optional<ValueType> getSliderStepSize() const {
                return m_sliderStep;
            }
        };

        class ICFileFilters {
        protected:
            using Filter = utils::file::FilePickOptions::Filter;

            std::optional<std::vector<Filter>> m_filters = std::nullopt;

        public:
            Result<> parseFileFilters(JsonMaybeObject<ModJson>& obj) {
                std::vector<Filter> filters {};
                for (auto& item : obj.has("filters").iterate()) {
                    if (auto iobj = item.obj()) {
                        Filter filter;
                        iobj.has("description").into(filter.description);
                        iobj.has("files").into(filter.files);
                        filters.push_back(filter);
                    }
                }
                if (filters.size()) {
                    m_filters = filters;
                }
                return Ok();
            }

            auto getFileFilters() const {
                return m_filters;
            }
        };

        GEODE_INT_DECL_SETTING_CONTROL(Input, hasInput, true, "input");
    }

    class BoolSetting : public GeodeSetting<BoolSetting, bool, SettingType::Bool> {
    public:
        GEODE_DLL SettingNode* createNode(float width) override;
    };

    class IntSetting :
        public GeodeSetting<IntSetting, int64_t, SettingType::Int>,
        public IOneOf<IntSetting, int64_t>,
        public IMinMax<int64_t>,
        public ICArrows,
        public ICSlider<int64_t>,
        public ICInput {
    public:
        GEODE_DLL SettingNode* createNode(float width) override;
    };

    class FloatSetting :
        public GeodeSetting<FloatSetting, double, SettingType::Float>,
        public IOneOf<FloatSetting, double>,
        public IMinMax<double>,
        public ICArrows,
        public ICSlider<double>,
        public ICInput {
    public:
        GEODE_DLL SettingNode* createNode(float width) override;
    };

    class StringSetting :
        public GeodeSetting<StringSetting, std::string, SettingType::String>,
        public IOneOf<StringSetting, std::string>,
        public IMatch<StringSetting, std::string> {
    public:
        GEODE_DLL SettingNode* createNode(float width) override;
    };

    class FileSetting :
        public GeodeSetting<FileSetting, ghc::filesystem::path, SettingType::File>,
        public ICFileFilters {
    public:
        GEODE_DLL SettingNode* createNode(float width) override;
    };

    class ColorSetting : public GeodeSetting<ColorSetting, cocos2d::ccColor3B, SettingType::Color> {
    public:
        GEODE_DLL SettingNode* createNode(float width) override;
    };

    class ColorAlphaSetting :
        public GeodeSetting<ColorAlphaSetting, cocos2d::ccColor4B, SettingType::ColorAlpha> {
    public:
        GEODE_DLL SettingNode* createNode(float width) override;
    };

    // these can't be member functions because C++ is single-pass >:(

#define GEODE_INT_BUILTIN_SETTING_IF(type, action, ...)                      \
    if constexpr (__VA_ARGS__) {                                             \
        if (setting->getType() == SettingType::type) {                       \
            return std::static_pointer_cast<type##Setting>(setting)->action; \
        }                                                                    \
    }

    // clang-format off

    template <class T>
    T getBuiltInSettingValue(const std::shared_ptr<Setting> setting) {
        GEODE_INT_BUILTIN_SETTING_IF(Bool, getValue(), std::is_same_v<T, bool>)
        else GEODE_INT_BUILTIN_SETTING_IF(Float, getValue(), std::is_floating_point_v<T>)
        else GEODE_INT_BUILTIN_SETTING_IF(Int, getValue(), std::is_integral_v<T>)
        else GEODE_INT_BUILTIN_SETTING_IF(String, getValue(), std::is_same_v<T, std::string>)
        else GEODE_INT_BUILTIN_SETTING_IF(File, getValue(), std::is_same_v<T, ghc::filesystem::path>)
        else GEODE_INT_BUILTIN_SETTING_IF(Color, getValue(), std::is_same_v<T, cocos2d::ccColor3B>)
        else GEODE_INT_BUILTIN_SETTING_IF(ColorAlpha, getValue(), std::is_same_v<T, cocos2d::ccColor4B>)
        else {
            static_assert(!std::is_same_v<T, T>, "Unsupported type for getting setting value!");
        }
        return T();
    }

    template <class T>
    void setBuiltInSettingValue(const std::shared_ptr<Setting> setting, T const& value) {
        GEODE_INT_BUILTIN_SETTING_IF(Bool, setValue(value), std::is_same_v<T, bool>)
        else GEODE_INT_BUILTIN_SETTING_IF(Float, setValue(value), std::is_floating_point_v<T>)
        else GEODE_INT_BUILTIN_SETTING_IF(Int, setValue(value), std::is_integral_v<T>)
        else GEODE_INT_BUILTIN_SETTING_IF(String, setValue(value), std::is_same_v<T, std::string>)
        else GEODE_INT_BUILTIN_SETTING_IF(File, setValue(value), std::is_same_v<T, ghc::filesystem::path>)
        else GEODE_INT_BUILTIN_SETTING_IF(Color, setValue(value), std::is_same_v<T, cocos2d::ccColor3B>)
        else GEODE_INT_BUILTIN_SETTING_IF(ColorAlpha, setValue(value), std::is_same_v<T, cocos2d::ccColor4B>)
        else {
            static_assert(!std::is_same_v<T, T>, "Unsupported type for getting setting value!");
        }
    }

    // clang-format on
}

#pragma warning(pop)
