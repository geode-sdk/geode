#pragma once

#include "Types.hpp"
#include "../DefaultInclude.hpp"
#include "../utils/Result.hpp"
#include "../utils/file.hpp"
#include <json.hpp>
#include <optional>
#include <unordered_set>

#pragma warning(push)
#pragma warning(disable : 4275)

namespace geode {
    class SettingNode;
    class SettingValue;

    template <class Json>
    struct JsonMaybeObject;
    template <class Json>
    struct JsonMaybeValue;

    struct GEODE_DLL BoolSetting final {
        using ValueType = bool;

        std::optional<std::string> name;
        std::optional<std::string> description;
        bool defaultValue;

        static Result<BoolSetting> parse(JsonMaybeObject<ModJson>& obj);
    };

    struct GEODE_DLL IntSetting final {
        using ValueType = int64_t;

        std::optional<std::string> name;
        std::optional<std::string> description;
        ValueType defaultValue;
        std::optional<ValueType> min;
        std::optional<ValueType> max;
        struct {
            bool arrows = true;
            bool bigArrows = false;
            size_t arrowStep = 1;
            size_t bigArrowStep = 5;
            bool slider = true;
            std::optional<ValueType> sliderStep = std::nullopt;
            bool input = true;
        } controls;

        static Result<IntSetting> parse(JsonMaybeObject<ModJson>& obj);
    };

    struct GEODE_DLL FloatSetting final {
        using ValueType = double;

        std::optional<std::string> name;
        std::optional<std::string> description;
        ValueType defaultValue;
        std::optional<ValueType> min;
        std::optional<ValueType> max;
        struct {
            bool arrows = true;
            bool bigArrows = false;
            size_t arrowStep = 1;
            size_t bigArrowStep = 5;
            bool slider = true;
            std::optional<ValueType> sliderStep = std::nullopt;
            bool input = true;
        } controls;

        static Result<FloatSetting> parse(JsonMaybeObject<ModJson>& obj);
    };

    struct GEODE_DLL StringSetting final {
        using ValueType = std::string;

        std::optional<std::string> name;
        std::optional<std::string> description;
        ValueType defaultValue;
        std::optional<std::string> match;

        static Result<StringSetting> parse(JsonMaybeObject<ModJson>& obj);
    };

    struct GEODE_DLL FileSetting final {
        using ValueType = ghc::filesystem::path;
        using Filter = utils::file::FilePickOptions::Filter;

        std::optional<std::string> name;
        std::optional<std::string> description;
        ValueType defaultValue;
        struct {
            std::vector<Filter> filters;
        } controls;

        static Result<FileSetting> parse(JsonMaybeObject<ModJson>& obj);
    };

    struct GEODE_DLL ColorSetting final {
        using ValueType = cocos2d::ccColor3B;

        std::optional<std::string> name;
        std::optional<std::string> description;
        ValueType defaultValue;

        static Result<ColorSetting> parse(JsonMaybeObject<ModJson>& obj);
    };

    struct GEODE_DLL ColorAlphaSetting final {
        using ValueType = cocos2d::ccColor4B;

        std::optional<std::string> name;
        std::optional<std::string> description;
        ValueType defaultValue;

        static Result<ColorAlphaSetting> parse(JsonMaybeObject<ModJson>& obj);
    };

    struct GEODE_DLL CustomSetting final {
        std::shared_ptr<ModJson> json;
    };

    using SettingKind = std::variant<
        BoolSetting,
        IntSetting,
        FloatSetting,
        StringSetting,
        FileSetting,
        ColorSetting,
        ColorAlphaSetting,
        CustomSetting
    >;

    struct GEODE_DLL Setting final {
    private:
        std::string m_key;
        SettingKind m_kind;

        Setting() = default;

    public:
        static Result<Setting> parse(
            std::string const& key,
            JsonMaybeValue<ModJson>& obj
        );
        Setting(std::string const& key, SettingKind const& kind);

        template<class T>
        std::optional<T> get() {
            if (std::holds_alternative<T>(m_kind)) {
                return std::get<T>(m_kind);
            }
            return std::nullopt;
        }

        std::unique_ptr<SettingValue> createDefaultValue() const;
        bool isCustom() const;
        std::string getDisplayName() const;
        std::optional<std::string> getDescription() const;
    };

    class GEODE_DLL SettingValue {
    protected:
        std::string m_key;

        SettingValue(std::string const& key);
    
    public:
        virtual ~SettingValue() = default;
        virtual bool load(json::Value const& json) = 0;
        virtual bool save(json::Value& json) const = 0;
        virtual SettingNode* createNode(float width) = 0;

        std::string getKey() const;
    };

    template<class T>
    class GeodeSettingValue : public SettingValue {
    public:
        using ValueType = typename T::ValueType;

    protected:
        ValueType m_value;
        T m_definition;

        using Valid = std::pair<ValueType, std::optional<std::string>>;

        GEODE_DLL Valid toValid(ValueType const& value) const;

    public:
        GeodeSettingValue(std::string const& key, T const& definition)
          : SettingValue(key),
            m_definition(definition),
            m_value(definition.defaultValue) {}

        bool load(json::Value const& json) override;
        bool save(json::Value& json) const;

        GEODE_DLL SettingNode* createNode(float width) override;
        T castDefinition() const {
            return m_definition;
        }
        Setting getDefinition() const {
            return Setting(m_key, m_definition);
        }

        ValueType getValue() const {
            return m_value;
        }
        GEODE_DLL void setValue(ValueType const& value);
        GEODE_DLL Result<> validate(ValueType const& value) const;
    };

    using BoolSettingValue       = GeodeSettingValue<BoolSetting>;
    using IntSettingValue        = GeodeSettingValue<IntSetting>;
    using FloatSettingValue      = GeodeSettingValue<FloatSetting>;
    using StringSettingValue     = GeodeSettingValue<StringSetting>;
    using FileSettingValue       = GeodeSettingValue<FileSetting>;
    using ColorSettingValue      = GeodeSettingValue<ColorSetting>;
    using ColorAlphaSettingValue = GeodeSettingValue<ColorAlphaSetting>;

    template<class T>
    struct SettingValueSetter {
        static GEODE_DLL T get(SettingValue* setting);
        static GEODE_DLL void set(SettingValue* setting, T const& value);
    };
}

#pragma warning(pop)
