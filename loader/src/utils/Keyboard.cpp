#include <Geode/utils/Keyboard.hpp>

using namespace geode::prelude;

Keyboard::Keyboard(enumKeyCodes key, bool down, bool repeat) {
    m_key = key;
    m_down = down;
    m_repeat = repeat;
}

enumKeyCodes Keyboard::getKey() const {
    return m_key;
}

bool Keyboard::isDown() const {
    return m_down;
}

bool Keyboard::isRepeat() const {
    return m_repeat;
}

 ModifierKeyEvent::ModifierKeyEvent(bool shift, bool ctrl, bool alt, bool cmd) {
    m_shift = shift;
    m_ctrl = ctrl;
    m_alt = alt;
    m_cmd = cmd;
}

bool ModifierKeyEvent::shiftPressed() const {
    return m_shift;
}

bool ModifierKeyEvent::controlPressed() const {
    return m_ctrl;
}

bool ModifierKeyEvent::altPressed() const {
    return m_alt;
}

bool ModifierKeyEvent::commandPressed() const {
    return m_cmd;
}

#ifndef GEODE_IS_IOS

#include <Geode/modify/CCKeyboardDispatcher.hpp>
class $modify(EventKeyboardDispatcher, cocos2d::CCKeyboardDispatcher) {
    static void onModify(auto& self) {
        if (auto err = self.setHookPriority("cocos2d::CCKeyboardDispatcher::dispatchKeyboardMSG", Priority::First).err()) {
            log::error("Failed to hook CCKeyboardDispatcher::dispatchKeyboardMSG: {}", err);
        }
        if (auto err = self.setHookPriority("cocos2d::CCKeyboardDispatcher::updateModifierKeys", Priority::First).err()) {
            log::error("Failed to hook CCKeyboardDispatcher::updateModifierKeys: {}", err);
        }
    }
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool repeat) {
        if (Keyboard(key, down, repeat).post() == ListenerResult::Stop) return true;
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
    }
    void updateModifierKeys(bool shift, bool ctrl, bool alt, bool cmd) {
        ModifierKeyEvent(shift, ctrl, alt, cmd).post();
        CCKeyboardDispatcher::updateModifierKeys(shift, ctrl, alt, cmd);
    }
};

#endif