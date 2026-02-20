#pragma once

#include "../GeodeStyle.hpp"

using namespace geode::prelude;

CCNode* createKeybindButton(Keybind const& keybind);

// If these are ever exposed then they probably need to be rewritten to not 
// contain the KeybindSettingV3 directly and instead take a title, subtitle, 
// etc.

class KeybindEditPopup : public GeodePopup {
protected:
    std::shared_ptr<KeybindSettingV3> m_setting;
    CCNode* m_keybindNode = nullptr;
    CCNode* m_originalKeybindContainer = nullptr;
    Keybind m_currentKeybind;
    std::optional<Keybind> m_originalKeybind;
    Function<void(Keybind const&)> m_callback;

    bool init(
        std::shared_ptr<KeybindSettingV3> setting,
        Keybind const& keybind,
        Function<void(Keybind const&)> callback
    );
    void onSet(CCObject*);
    void onRemove(CCObject*);

    void updateLabel();

public:
    static KeybindEditPopup* create(
        std::shared_ptr<KeybindSettingV3> setting,
        Keybind const& keybind,
        Function<void(Keybind const&)> callback
    );
};

class KeybindListPopup : public GeodePopup {
protected:
    std::shared_ptr<KeybindSettingV3> m_setting;
    std::vector<Keybind> m_currentKeybinds;
    Function<void(std::vector<Keybind>)> m_callback;
    ScrollLayer* m_scrollLayer;
    bool m_hasChanged;

    bool init(
        std::shared_ptr<KeybindSettingV3> setting,
        std::vector<Keybind> const& keybinds,
        Function<void(std::vector<Keybind>)> callback
    );
    void updateKeybinds();
    void onAdd(CCObject*);
    void onSave(CCObject*);
    void onKeybind(CCObject*);
    void onClose(CCObject*) override;

public:
    static KeybindListPopup* create(
        std::shared_ptr<KeybindSettingV3> setting,
        std::vector<Keybind> const& keybinds,
        Function<void(std::vector<Keybind>)> callback
    );
};
