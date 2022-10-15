#pragma once

#include "Setting.hpp"
#include <cocos2d.h>

namespace geode {
    class SettingNode;

    struct GEODE_DLL SettingNodeDelegate {
        virtual void settingValueChanged(SettingNode* node);
        virtual void settingValueCommitted(SettingNode* node);
    };

    class GEODE_DLL SettingNode : public cocos2d::CCNode {
    protected:
        std::shared_ptr<Setting> m_setting;
        SettingNodeDelegate* m_delegate = nullptr;

        bool init(std::shared_ptr<Setting> setting);

    public:
        void setDelegate(SettingNodeDelegate* delegate);

        virtual void commit() = 0;
        virtual bool hasUncommittedChanges() = 0;
        virtual bool hasNonDefaultValue() = 0;
        virtual void resetToDefault() = 0;
    };
}
