#pragma once

#include "Setting.hpp"

#include <cocos2d.h>

namespace geode {
    class SettingNode;

    struct SettingNodeDelegate {
        virtual void settingValueChanged(SettingNode* node) {}
        virtual void settingValueCommitted(SettingNode* node) {}
    };

    class GEODE_DLL SettingNode : public cocos2d::CCNode {
    protected:
        SettingValue* m_value;
        SettingNodeDelegate* m_delegate = nullptr;

        bool init(SettingValue* value);
        void dispatchChanged();
        void dispatchCommitted();

    public:
        void setDelegate(SettingNodeDelegate* delegate);

        virtual void commit() = 0;
        virtual bool hasUncommittedChanges() = 0;
        virtual bool hasNonDefaultValue() = 0;
        virtual void resetToDefault() = 0;
    };
}
