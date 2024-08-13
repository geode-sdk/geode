#pragma once

#include "../DefaultInclude.hpp"
#include <optional>
#include <cocos2d.h>
// todo: remove this header in 4.0.0
#include "Setting.hpp"

namespace geode {
    class SettingNodeV3;
    class JsonExpectedValue;

    class GEODE_DLL SettingV3 {
    private:
        class GeodeImpl;
        std::shared_ptr<GeodeImpl> m_impl;

    public:
        SettingV3(std::string const& key, std::string const& modID);
        virtual ~SettingV3();

        /**
         * Get the key of this setting
         */
        std::string getKey() const;
        /**
         * Get the mod ID this setting is for
         */
        std::string getModID() const;
        /**
         * Get the mod this setting is for. Note that this may return null 
         * while the mod is still being initialized
         */
        Mod* getMod() const;

        virtual Result<> parse(std::string const& modID, matjson::Value const& json) = 0;
        virtual bool load(matjson::Value const& json) = 0;
        virtual bool save(matjson::Value& json) const = 0;
        virtual SettingNodeV3* createNode(float width) = 0;

        virtual bool isDefaultValue() const = 0;
        /**
         * Reset this setting's value back to its original value
         */
        virtual void reset() = 0;

        [[deprecated("This function will be removed alongside legacy settings in 4.0.0!")]]
        virtual std::optional<Setting> convertToLegacy() const;
        [[deprecated("This function will be removed alongside legacy settings in 4.0.0!")]]
        virtual std::optional<std::unique_ptr<SettingValue>> convertToLegacyValue() const;

        static Result<std::shared_ptr<SettingV3>> parseBuiltin(std::string const& modID, matjson::Value const& json);
    };

    namespace detail {
        class GEODE_DLL GeodeSettingBaseV3 : public SettingV3 {
        private:
            class Impl;
            std::shared_ptr<Impl> m_impl;
        
        protected:
            Result<> parseShared(JsonExpectedValue& json);
            Result<> isValidShared() const;

        public:
            std::string getName() const;
            std::optional<std::string> getDescription() const;
            std::optional<std::string> getEnableIf() const;
        };
    }

    class GEODE_DLL TitleSettingV3 final : public SettingV3 {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
    
    public:
        std::string getTitle() const;

        Result<> parse(std::string const& modID, matjson::Value const& json) override;
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        bool isDefaultValue() const override;
        void reset() override;
    };

    class GEODE_DLL UnresolvedCustomSettingV3 final : public SettingV3 {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
    
    public:
        Result<> parse(std::string const& modID, matjson::Value const& json) override;
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        bool isDefaultValue() const override;
        void reset() override;
        
        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::unique_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL BoolSettingV3 final : public detail::GeodeSettingBaseV3 {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    public:
        bool getValue() const;
        void setValue(bool value);
        Result<> isValid(bool value) const;

        bool getDefaultValue() const;
        
        Result<> parse(std::string const& modID, matjson::Value const& json) override;
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        bool isDefaultValue() const override;
        void reset() override;
        
        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::unique_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL IntSettingV3 final : public detail::GeodeSettingBaseV3 {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    public:
        int64_t getValue() const;
        void setValue(int64_t value);
        Result<> isValid(int64_t value) const;

        int64_t getDefaultValue() const;
        std::optional<int64_t> getMinValue() const;
        std::optional<int64_t> getMaxValue() const;

        bool isArrowsEnabled() const;
        bool isBigArrowsEnabled() const;
        size_t getArrowStepSize() const;
        size_t getBigArrowStepSize() const;
        bool isSliderEnabled() const;
        std::optional<int64_t> getSliderSnap() const;
        bool isInputEnabled() const;
    
        Result<> parse(std::string const& modID, matjson::Value const& json) override;
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        bool isDefaultValue() const override;
        void reset() override;
        
        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::unique_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL FloatSettingV3 final : public detail::GeodeSettingBaseV3 {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    public:
        double getValue() const;
        void setValue(double value);
        Result<> isValid(double value) const;

        double getDefaultValue() const;
        std::optional<double> getMinValue() const;
        std::optional<double> getMaxValue() const;

        bool isArrowsEnabled() const;
        bool isBigArrowsEnabled() const;
        size_t getArrowStepSize() const;
        size_t getBigArrowStepSize() const;
        bool isSliderEnabled() const;
        std::optional<double> getSliderSnap() const;
        bool isInputEnabled() const;
        
        Result<> parse(std::string const& modID, matjson::Value const& json) override;
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        bool isDefaultValue() const override;
        void reset() override;
        
        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::unique_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL StringSettingV3 final : public detail::GeodeSettingBaseV3 {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    public:
        std::string getValue() const;
        void setValue(std::string_view value);
        Result<> isValid(std::string_view value) const;

        std::string getDefaultValue() const;

        std::optional<std::string> getRegexValidator() const;
        std::optional<std::string> getAllowedCharacters() const;
        std::optional<std::vector<std::string>> getEnumOptions() const;
        
        Result<> parse(std::string const& modID, matjson::Value const& json) override;
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        bool isDefaultValue() const override;
        void reset() override;
        
        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::unique_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL FileSettingV3 final : public detail::GeodeSettingBaseV3 {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    public:
        std::filesystem::path getValue() const;
        void setValue(std::filesystem::path const& value);
        Result<> isValid(std::filesystem::path value) const;

        std::filesystem::path getDefaultValue() const;
        std::optional<std::vector<utils::file::FilePickOptions::Filter>> getFilters() const;
        
        Result<> parse(std::string const& modID, matjson::Value const& json) override;
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        bool isDefaultValue() const override;
        void reset() override;
        
        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::unique_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL Color3BSettingV3 final : public detail::GeodeSettingBaseV3 {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    public:
        cocos2d::ccColor3B getValue() const;
        void setValue(cocos2d::ccColor3B value);
        Result<> isValid(cocos2d::ccColor3B value) const;

        cocos2d::ccColor3B getDefaultValue() const;
        
        Result<> parse(std::string const& modID, matjson::Value const& json) override;
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        bool isDefaultValue() const override;
        void reset() override;
        
        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::unique_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL Color4BSettingV3 final : public detail::GeodeSettingBaseV3 {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    public:
        cocos2d::ccColor4B getValue() const;
        void setValue(cocos2d::ccColor4B value);
        Result<> isValid(cocos2d::ccColor4B value) const;

        cocos2d::ccColor4B getDefaultValue() const;
        
        Result<> parse(std::string const& modID, matjson::Value const& json) override;
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        bool isDefaultValue() const override;
        void reset() override;
        
        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::unique_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL SettingNodeV3 : public cocos2d::CCNode {
    protected:
        bool init();

        /**
         * Mark this setting as changed. This updates the UI for committing 
         * the value
         */
        void markChanged();

        /**
         * When the setting value is committed (aka can't be undone), this 
         * function will be called. This should take care of actually saving 
         * the value in some sort of global manager
         */
        virtual void onCommit() = 0;

        void dispatchChanged();
        void dispatchCommitted();

    public:
        virtual void commit() = 0;
        virtual bool hasUncommittedChanges() = 0;
        virtual bool hasNonDefaultValue() = 0;
        virtual void resetToDefault() = 0;
    };
}
