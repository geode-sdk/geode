#pragma once

#include <Geode/DefaultInclude.hpp>
#include <optional>
#include <unordered_set>
#include "../utils/json.hpp"
#include "../utils/Result.hpp"
#include "../utils/JsonValidation.hpp"

namespace geode {
    class SettingNode;
    class BoolSetting;
    class IntSetting;
    class FloatSetting;
    class StringSetting;

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

    class GEODE_DLL Setting {
    protected:
        std::string m_key;

        static Result<std::shared_ptr<Setting>> parse(
            std::string const& type,
            std::string const& key,
            JsonMaybeObject& obj
        );

    public:
        virtual ~Setting() = default;
        
        // Load from mod.json
        static Result<std::shared_ptr<Setting>> parse(
            std::string const& key,
            nlohmann::json const& json
        );
        // Load value from saved settings
        virtual bool load(nlohmann::json const& json) = 0;
        // Save setting value
        virtual bool save(nlohmann::json& json) const = 0;

        virtual SettingNode* createNode(float width) = 0;

        std::string getKey() const;
        virtual SettingType getType() const = 0;
    };

    namespace {
        #define GEODE_INT_PARSE_SETTING_IMPL(obj, func) \
            if constexpr (requires(JsonMaybeObject& obj) {\
                {res->func(obj)} -> std::same_as<Result<>>;\
            }) {\
                auto r = res->func(obj);\
                if (!r) return Err(r.error());\
            }

        #define GEODE_INT_CONSTRAIN_SETTING_IMPL(func) \
            if constexpr (requires(ValueType& value) {\
                this->func(value);\
            }) {\
                this->func(m_value);\
            }

        template<class Class, class ValueType, SettingType Type>
        class GeodeSetting : public Setting {
        protected:
            ValueType m_default;
            ValueType m_value;
            std::string m_name;
            std::string m_description;
            bool m_canResetToDefault = true;

            friend class Setting;

            static Result<std::shared_ptr<Class>> parse(
                std::string const& key,
                JsonMaybeObject& obj
            ) {
                auto res = std::make_shared<Class>();
                
                res->m_key = key;
                obj.needs("default").into(res->m_default);
                obj.has("name").into(res->m_name);
                obj.has("description").into(res->m_description);
                GEODE_INT_PARSE_SETTING_IMPL(obj, Class::parseMinMax);
                GEODE_INT_PARSE_SETTING_IMPL(obj, Class::parseOneOf);
                res->setValue(res->m_default);

                if (auto controls = obj.has("control").obj()) {
                    // every built-in setting type has a reset button 
                    // by default
                    controls.has("can-reset").into(res->m_canResetToDefault);
                    GEODE_INT_PARSE_SETTING_IMPL(controls, Class::parseArrows);
                    GEODE_INT_PARSE_SETTING_IMPL(controls, Class::parseSlider);
                    GEODE_INT_PARSE_SETTING_IMPL(controls, Class::parseInput);
                }

                return Ok(res);
            }
        
        public:
            using value_t = ValueType;

            std::string getName() const {
                return m_name;
            }

            std::string getDescription() const {
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
                GEODE_INT_CONSTRAIN_SETTING_IMPL(Class::constrainMinMax);
                GEODE_INT_CONSTRAIN_SETTING_IMPL(Class::constrainOneOf);
            }

            bool load(nlohmann::json const& json) override {
                auto rawJson = json;
                JsonChecker(rawJson)
                    .root("[setting value]")
                    .into(m_value);
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

        template<class ValueType>
        class IMinMax {
        protected:
            std::optional<ValueType> m_min = std::nullopt;
            std::optional<ValueType> m_max = std::nullopt;

        public:
            void constrainMinMax(ValueType& value) {
                if (m_min && value < m_min.value()) {
                    value = m_min.value();
                }
                if (m_max && value > m_max.value()) {
                    value = m_max.value();
                }
            }

            Result<> parseMinMax(JsonMaybeObject& obj) {
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

        template<class ValueType>
        class IOneOf {
        protected:
            std::optional<std::unordered_set<ValueType>> m_oneOf = std::nullopt;

        public:
            void constrainOneOf(ValueType& value) {
                if (m_oneOf && !m_oneOf.value().count(value)) {
                    if (m_oneOf.value().size()) {
                        value = m_oneOf.value()[0];
                    } else {
                        value = ValueType();
                    }
                }
            }

            Result<> parseOneOf(JsonMaybeObject& obj) {
                std::unordered_set<ValueType> oneOf {};
                for (auto& item : obj.has("one-of").iterate()) {
                    oneOf.insert(item.get<ValueType>());
                }
                if (oneOf.size()) {
                    m_oneOf = oneOf;
                }
                return Ok();
            }

            std::optional<std::unordered_set<ValueType>> getOneOf() const {
                return m_oneOf;
            }
        };
    
        #define GEODE_INT_DECL_SETTING_CONTROL(Name, name, default, json) \
            class IC##Name {\
            protected:\
                bool m_##name = default;\
            public:\
                Result<> parse##Name(JsonMaybeObject& obj) {\
                    obj.has(json).into(m_##name);\
                    return Ok();\
                }\
                bool has##Name() const {\
                    return m_##name;\
                }\
            }

        GEODE_INT_DECL_SETTING_CONTROL(Arrows,   hasArrows, true, "arrows");
        GEODE_INT_DECL_SETTING_CONTROL(Slider,   hasSlider, true, "slider");
        GEODE_INT_DECL_SETTING_CONTROL(Input,    hasInput,  true, "input");
    }

    class GEODE_DLL BoolSetting :
        public GeodeSetting<BoolSetting, bool, SettingType::Bool>,
        public std::enable_shared_from_this<BoolSetting>
    {
    public:
        SettingNode* createNode(float width) override;
    };

    class GEODE_DLL IntSetting :
        public GeodeSetting<IntSetting, int64_t, SettingType::Int>,
        public IMinMax<int64_t>,
        public std::enable_shared_from_this<IntSetting>,
        public ICArrows, public ICSlider, public ICInput
    {
    public:
        SettingNode* createNode(float width) override;
    };

    class GEODE_DLL FloatSetting : 
        public GeodeSetting<FloatSetting, double, SettingType::Float>,
        public IMinMax<double>,
        public std::enable_shared_from_this<FloatSetting>,
        public ICArrows, public ICSlider, public ICInput
    {
    public:
        SettingNode* createNode(float width) override;
    };

    class GEODE_DLL StringSetting : 
        public GeodeSetting<StringSetting, std::string, SettingType::String>,
        public std::enable_shared_from_this<StringSetting>,
        public ICInput
    {
    public:
        SettingNode* createNode(float width) override;
    };
    
    // these can't be member functions because C++ is single-pass >:(

    #define GEODE_INT_BUILTIN_SETTING_IF(type, action, ...) \
        if constexpr (__VA_ARGS__) {\
            if (setting->getType() == SettingType::type) {\
                return std::static_pointer_cast<type##Setting>(setting)->action;\
            }\
        }

    template<class T>
    T getBuiltInSettingValue(const std::shared_ptr<Setting> setting) {
        GEODE_INT_BUILTIN_SETTING_IF(Bool, getValue(), std::is_same_v<T, bool>)
        else GEODE_INT_BUILTIN_SETTING_IF(Float, getValue(), std::is_floating_point_v<T>)
        else GEODE_INT_BUILTIN_SETTING_IF(Int, getValue(), std::is_integral_v<T>)
        else GEODE_INT_BUILTIN_SETTING_IF(String, getValue(), std::is_same_v<T, std::string>)
        else {
            static_assert(!std::is_same_v<T, T>, "todo: implement");
        }
        return T();
    }

    template<class T>
    void setBuiltInSettingValue(const std::shared_ptr<Setting> setting, T const& value) {
        GEODE_INT_BUILTIN_SETTING_IF(Bool, setValue(value), std::is_same_v<T, bool>)
        else GEODE_INT_BUILTIN_SETTING_IF(Float, setValue(value), std::is_floating_point_v<T>)
        else GEODE_INT_BUILTIN_SETTING_IF(Int, setValue(value), std::is_integral_v<T>)
        else GEODE_INT_BUILTIN_SETTING_IF(String, setValue(value), std::is_same_v<T, std::string>)
        else {
            static_assert(!std::is_same_v<T, T>, "todo: implement");
        }
    }
}
