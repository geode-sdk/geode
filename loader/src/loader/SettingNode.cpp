#include <Geode/loader/SettingNode.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

void SettingNodeDelegate::settingValueChanged(SettingNode* node) {}

void SettingNodeDelegate::settingValueCommitted(SettingNode* node) {}

bool SettingNode::init(std::shared_ptr<Setting> setting) {
    m_setting = setting;
    return true;
}

void SettingNode::setDelegate(SettingNodeDelegate* delegate) {
    m_delegate = delegate;
}
