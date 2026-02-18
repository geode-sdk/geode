#pragma once

#include "../GeodeStyle.hpp"

using namespace geode::prelude;

CCNode* createKeybindButton(Keybind const& keybind);

class KeybindEditPopup : public GeodePopup {
protected:
    CCNode* m_keybindNode = nullptr;
    CCNode* m_originalKeybindContainer = nullptr;
    Keybind m_currentKeybind;
    std::optional<Keybind> m_originalKeybind;
    Function<void(Keybind const&)> m_callback;

    bool init(ZStringView name, Keybind const& keybind, Function<void(Keybind const&)> callback);
    void onSet(CCObject*);
    void onRemove(CCObject*);

    void updateLabel();

public:
    static KeybindEditPopup* create(ZStringView name, Keybind const& keybind, Function<void(Keybind const&)> callback);
};

class KeybindListPopup : public GeodePopup {
protected:
    std::vector<Keybind> m_currentKeybinds;
    Function<void(std::vector<Keybind>)> m_callback;
    ScrollLayer* m_scrollLayer;
    bool m_hasChanged;

    bool init(ZStringView name, std::vector<Keybind> const& keybinds, Function<void(std::vector<Keybind>)> callback);
    void updateKeybinds();
    void onAdd(CCObject*);
    void onSave(CCObject*);
    void onKeybind(CCObject*);
    void onClose(CCObject*) override;
public:
    static KeybindListPopup* create(ZStringView name, std::vector<Keybind> const& keybinds, Function<void(std::vector<Keybind>)> callback);
};
