#pragma once

#include "Types.hpp"
#include "../DefaultInclude.hpp"
#include "../utils/Result.hpp"
#include "../utils/file.hpp"
#include <json.hpp>
#include <optional>
#include <unordered_set>
#include <cocos2d.h>

#pragma warning(push)
#pragma warning(disable : 4275)

namespace geode {
    class SettingNode;
    class SettingValue;

    struct JsonMaybeObject;
    struct JsonMaybeValue;

    /**
     * A Setting for a boolean value. Represented in-game as a simple toggle
     */
    struct GEODE_DLL BoolSetting final {
        using ValueType = bool;

        std::optional<std::string> name;
        std::optional<std::string> description;
        bool defaultValue;

        static Result<BoolSetting> parse(JsonMaybeObject& obj);
    };

    /**
     * A Setting for an integer value. The value can be limited using the min 
     * and max options
     */
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

        static Result<IntSetting> parse(JsonMaybeObject& obj);
    };

    /**
     * A Setting for a float value. The value can be limited using the min 
     * and max options
     */
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

        static Result<FloatSetting> parse(JsonMaybeObject& obj);
    };

    /**
     * A Setting for a string value
     */
    struct GEODE_DLL StringSetting final {
        using ValueType = std::string;

        std::optional<std::string> name;
        std::optional<std::string> description;
        ValueType defaultValue;
        /**
         * A regex the string must succesfully match against
         */
        std::optional<std::string> match;

        static Result<StringSetting> parse(JsonMaybeObject& obj);
    };

    /**
     * A Setting for a file input. Lets the user select a file from their 
     * local device
     */
    struct GEODE_DLL FileSetting final {
        using ValueType = ghc::filesystem::path;
        using Filter = utils::file::FilePickOptions::Filter;

        std::optional<std::string> name;
        std::optional<std::string> description;
        ValueType defaultValue;
        struct {
            std::vector<Filter> filters;
        } controls;

        static Result<FileSetting> parse(JsonMaybeObject& obj);
    };

    /**
     * A Setting for an RGB color. See ColorAlphaSetting for a setting that 
     * also allows customizing alpha
     */
    struct GEODE_DLL ColorSetting final {
        using ValueType = cocos2d::ccColor3B;

        std::optional<std::string> name;
        std::optional<std::string> description;
        ValueType defaultValue;

        static Result<ColorSetting> parse(JsonMaybeObject& obj);
    };

    /**
     * A Setting for an RGBA color. See ColorSetting for a setting that doesn't 
     * have alpha
     */
    struct GEODE_DLL ColorAlphaSetting final {
        using ValueType = cocos2d::ccColor4B;

        std::optional<std::string> name;
        std::optional<std::string> description;
        ValueType defaultValue;

        static Result<ColorAlphaSetting> parse(JsonMaybeObject& obj);
    };

    /**
     * A custom setting, defined by the mod. See 
     * [the tutorial page](https://docs.geode-sdk.org/mods/settings) for more 
     * information about how to create custom settings
     */
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

    /**
     * Represents a saved value for a mod that can be customized by the user 
     * through an in-game UI. This class is for modeling the setting's 
     * definition - what values are accepted, its name etc.
     * See [the tutorial page](https://docs.geode-sdk.org/mods/settings) 
     * for more information about how settings work
     * @see SettingValue
     * @see SettingNode
     */
    struct GEODE_DLL Setting final {
    private:
        std::string m_key;
        std::string m_modID;
        SettingKind m_kind;

        Setting() = default;

    public:
        static Result<Setting> parse(
            std::string const& key,
            std::string const& mod,
            JsonMaybeValue& obj
        );
        Setting(
            std::string const& key,
            std::string const& mod,
            SettingKind const& kind
        );

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
        std::string getModID() const;
    };

    /**
     * Stores the actual, current value of a Setting. See 
     * [the tutorial page](https://docs.geode-sdk.org/mods/settings) for more 
     * information, and how to create custom settings
     */
    class GEODE_DLL SettingValue {
    protected:
        std::string m_key;
        std::string m_modID;

        SettingValue(std::string const& key, std::string const& mod);

        void valueChanged();
    
    public:
        virtual ~SettingValue() = default;
        virtual bool load(json::Value const& json) = 0;
        virtual bool save(json::Value& json) const = 0;
        virtual SettingNode* createNode(float width) = 0;

        std::string getKey() const;
        std::string getModID() const;
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
        GeodeSettingValue(std::string const& key, std::string const& modID, T const& definition)
          : SettingValue(key, modID),
            m_definition(definition),
            m_value(definition.defaultValue) {}

        bool load(json::Value const& json) override;
        bool save(json::Value& json) const;

        GEODE_DLL SettingNode* createNode(float width) override;
        T castDefinition() const {
            return m_definition;
        }
        Setting getDefinition() const {
            return Setting(m_key, m_modID, m_definition);
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
    struct GEODE_DLL SettingValueSetter {
        static T get(SettingValue* setting);
        static void set(SettingValue* setting, T const& value);
    };
}

#pragma warning(pop)
