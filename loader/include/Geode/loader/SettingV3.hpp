#pragma once

#include "../DefaultInclude.hpp"
#include <optional>
#include <cocos2d.h>
// todo: remove this header in 4.0.0
#include "Setting.hpp"
#include "../utils/cocos.hpp"

// todo in v4: these can be removed as well as the friend decl in UnresolvedCustomSettingV3
class ModSettingsManager;
class LegacyCustomSettingToV3Node;

namespace geode {
    class SettingNodeV3;
    class JsonExpectedValue;

    class GEODE_DLL SettingV3 : public std::enable_shared_from_this<SettingV3> {
    private:
        class GeodeImpl;
        std::shared_ptr<GeodeImpl> m_impl;

    protected:
        virtual Result<> onParse(
            std::string const& key, std::string const& modID, matjson::Value const& json
        ) = 0;

    public:
        SettingV3();
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

        Result<> parse(std::string const& key, std::string const& modID, matjson::Value const& json);
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
        virtual std::optional<std::shared_ptr<SettingValue>> convertToLegacyValue() const;

        static Result<std::shared_ptr<SettingV3>> parseBuiltin(
            std::string const& key, std::string const& modID, matjson::Value const& json
        );
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

        template <class T, class V = T>
        class GeodeSettingBaseValueV3 : public GeodeSettingBaseV3 {
        protected:
            virtual T& getValueMut() const = 0;
        
        public:
            using ValueType = T;

            virtual T getDefaultValue() const = 0;

            T getValue() const {
                return this->getValueMut();
            }
            void setValue(V value) {
                this->getValueMut() = this->isValid(value) ? value : this->getDefaultValue();
            }
            virtual Result<> isValid(V value) const = 0;
            
            bool isDefaultValue() const override {
                return this->getValue() == this->getDefaultValue();
            }
            void reset() {
                this->setValue(this->getDefaultValue());
            }
        };
    }

    class GEODE_DLL TitleSettingV3 final : public SettingV3 {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
    
    protected:
        Result<> onParse(std::string const& key, std::string const& modID, matjson::Value const& json) override;

    public:
        std::string getTitle() const;

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

        friend class ModSettingsManager;
        friend class LegacyCustomSettingToV3Node;
    
    protected:
        Result<> onParse(std::string const& key, std::string const& modID, matjson::Value const& json) override;

    public:
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        bool isDefaultValue() const override;
        void reset() override;
        
        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::shared_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL BoolSettingV3 final : public detail::GeodeSettingBaseValueV3<bool> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    protected:
        bool& getValueMut() const override;
        Result<> onParse(std::string const& key, std::string const& modID, matjson::Value const& json) override;

    public:
        bool getDefaultValue() const override;
        Result<> isValid(bool value) const override;
        
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::shared_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL IntSettingV3 final : public detail::GeodeSettingBaseValueV3<int64_t> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    protected:
        int64_t& getValueMut() const override;
        Result<> onParse(std::string const& key, std::string const& modID, matjson::Value const& json) override;

    public:
        int64_t getDefaultValue() const override;
        Result<> isValid(int64_t value) const override;

        std::optional<int64_t> getMinValue() const;
        std::optional<int64_t> getMaxValue() const;

        bool isArrowsEnabled() const;
        bool isBigArrowsEnabled() const;
        size_t getArrowStepSize() const;
        size_t getBigArrowStepSize() const;
        bool isSliderEnabled() const;
        std::optional<int64_t> getSliderSnap() const;
        bool isInputEnabled() const;
    
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::shared_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL FloatSettingV3 final : public detail::GeodeSettingBaseValueV3<double> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    protected:
        double& getValueMut() const override;
        Result<> onParse(std::string const& key, std::string const& modID, matjson::Value const& json) override;

    public:
        double getDefaultValue() const override;
        Result<> isValid(double value) const override;

        std::optional<double> getMinValue() const;
        std::optional<double> getMaxValue() const;

        bool isArrowsEnabled() const;
        bool isBigArrowsEnabled() const;
        size_t getArrowStepSize() const;
        size_t getBigArrowStepSize() const;
        bool isSliderEnabled() const;
        std::optional<double> getSliderSnap() const;
        bool isInputEnabled() const;
        
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::shared_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL StringSettingV3 final : public detail::GeodeSettingBaseValueV3<std::string, std::string_view> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    protected:
        std::string& getValueMut() const override;
        Result<> onParse(std::string const& key, std::string const& modID, matjson::Value const& json) override;

    public:
        std::string getDefaultValue() const override;
        Result<> isValid(std::string_view value) const override;

        std::optional<std::string> getRegexValidator() const;
        std::optional<std::string> getAllowedCharacters() const;
        std::optional<std::vector<std::string>> getEnumOptions() const;
        
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::shared_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL FileSettingV3 final : public detail::GeodeSettingBaseValueV3<std::filesystem::path, std::filesystem::path const&> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    protected:
        std::filesystem::path& getValueMut() const override;
        Result<> onParse(std::string const& key, std::string const& modID, matjson::Value const& json) override;

    public:
        std::filesystem::path getDefaultValue() const override;
        Result<> isValid(std::filesystem::path const& value) const override;

        std::optional<std::vector<utils::file::FilePickOptions::Filter>> getFilters() const;
        
        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::shared_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL Color3BSettingV3 final : public detail::GeodeSettingBaseValueV3<cocos2d::ccColor3B> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    protected:
        cocos2d::ccColor3B& getValueMut() const override;
        Result<> onParse(std::string const& key, std::string const& modID, matjson::Value const& json) override;

    public:
        cocos2d::ccColor3B getDefaultValue() const override;
        Result<> isValid(cocos2d::ccColor3B value) const override;

        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::shared_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL Color4BSettingV3 final : public detail::GeodeSettingBaseValueV3<cocos2d::ccColor4B> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    protected:
        cocos2d::ccColor4B& getValueMut() const override;
        Result<> onParse(std::string const& key, std::string const& modID, matjson::Value const& json) override;

    public:
        cocos2d::ccColor4B getDefaultValue() const override;
        Result<> isValid(cocos2d::ccColor4B value) const override;

        bool load(matjson::Value const& json) override;
        bool save(matjson::Value& json) const override;
        SettingNodeV3* createNode(float width) override;

        std::optional<Setting> convertToLegacy() const override;
        std::optional<std::shared_ptr<SettingValue>> convertToLegacyValue() const override;
    };

    class GEODE_DLL SettingNodeV3 : public cocos2d::CCNode {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;

    protected:
        bool init(std::shared_ptr<SettingV3> setting, float width);

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

    public:
        void commit();
        virtual bool hasUncommittedChanges() const = 0;
        virtual bool hasNonDefaultValue() const = 0;
        virtual void resetToDefault() = 0;

        void setContentSize(cocos2d::CCSize const& size) override;

        std::shared_ptr<SettingV3> getSetting() const;
    };

    class GEODE_DLL SettingNodeSizeChangeEventV3 : public Event {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
    
    public:
        SettingNodeSizeChangeEventV3(SettingNodeV3* node);
        virtual ~SettingNodeSizeChangeEventV3();

        SettingNodeV3* getTargetNode() const;
    };
    class GEODE_DLL SettingNodeValueChangeEventV3 : public Event {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
    
    public:
        SettingNodeValueChangeEventV3(bool commit);
        virtual ~SettingNodeValueChangeEventV3();

        bool isCommit() const;
    };

    template <class T>
    struct SettingTypeForValueType {
        static_assert(
            !std::is_same_v<T, T>,
            "specialize the SettingTypeForValueType class to use Mod::getSettingValue for custom settings"
        );
    };

    template <>
    struct SettingTypeForValueType<bool> {
        using SettingType = BoolSettingV3;
    };
    template <>
    struct SettingTypeForValueType<int64_t> {
        using SettingType = IntSettingV3;
    };
    template <>
    struct SettingTypeForValueType<double> {
        using SettingType = FloatSettingV3;
    };
    template <>
    struct SettingTypeForValueType<std::string> {
        using SettingType = StringSettingV3;
    };
    template <>
    struct SettingTypeForValueType<std::filesystem::path> {
        using SettingType = FileSettingV3;
    };
    template <>
    struct SettingTypeForValueType<cocos2d::ccColor3B> {
        using SettingType = Color3BSettingV3;
    };
    template <>
    struct SettingTypeForValueType<cocos2d::ccColor4B> {
        using SettingType = Color4BSettingV3;
    };
}
