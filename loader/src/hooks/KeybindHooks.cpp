#include <Geode/binding/LevelBindingsLayer.hpp>
#include <algorithm>
#include <vector>

#include "Geode/loader/Log.hpp"
#include "Geode/platform/windows.hpp"
#include "LoaderImpl.hpp"
#include <Geode/binding/GJKeybindManager.hpp>
#include <Geode/binding/KeybindSource.hpp>
#include <Geode/cocos/menu_nodes/CCMenu.h>
#include <Geode/binding/GameSettingsLayer.hpp>
#include <Geode/binding/KeybindingsLayer.hpp>
#include <Geode/modify/KeybindingsLayer.hpp>
#include <Geode/modify/GJKeybindManager.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/Keyboard.hpp>
#include <ui/mods/settings/KeybindsPopup.hpp>
#include <ui/mods/GeodeStyle.hpp>

using namespace geode::prelude;

#ifndef GEODE_IS_IOS

struct GeodeKeybindSource : public KeybindSource {
    virtual std::vector<int> chordsForAction(int action) override {
        auto setting = LoaderImpl::get()->keybindSettingForActionId(action);

        if (setting == nullptr) {
            return GJKeybindManager::sharedState()->chordsForAction(action);
        }

        std::vector<int> result;
        result.reserve(setting->getValue().size());

        for (auto& keybind : setting->getValue()) {
            if (auto chord = keybind.toVanillaChord(); chord.has_value()) {
                result.push_back(*chord);
            }
        }

        return result;
    }

    virtual std::vector<int> actionsForChord(int chord) override {
        auto result = GJKeybindManager::sharedState()->actionsForChord(chord);

        auto loaderImpl = LoaderImpl::get();

        for (auto& [id, setting] : loaderImpl->m_actionIdToKeybindSetting) {
            auto mod = setting->getMod();

            if (mod == nullptr || !mod->isLoaded()) {
                continue;
            }

            if (std::ranges::contains(setting->getValue(), chord, &Keybind::toVanillaChord)) {
                result.push_back(id);
            }
        }

        return result;
    }

    virtual void bindChord(int chord, int action) override {
        auto loaderImpl = LoaderImpl::get();

        if (auto setting = loaderImpl->keybindSettingForActionId(action); setting != nullptr) {
            auto keybind = Keybind::fromVanillaChord(chord);
            std::vector<Keybind> keybinds = setting->getValue();

            if (!std::ranges::contains(keybinds, chord, &Keybind::toVanillaChord)) {
                keybinds.push_back(keybind);
                setting->setValue(std::move(keybinds));
            }
        } else {
            GJKeybindManager::sharedState()->bindChord(chord, action);
        }
    }

    virtual void unbindChord(int chord, int action) override {
        auto loaderImpl = LoaderImpl::get();

        if (auto setting = loaderImpl->keybindSettingForActionId(action); setting != nullptr) {
            std::vector<Keybind> keybinds = setting->getValue();

            auto deleted =  std::erase_if(keybinds, [&](auto& e) { return e.toVanillaChord() == chord; });
            if (deleted > 0) {
                setting->setValue(std::move(keybinds));
            }
        } else {
            GJKeybindManager::sharedState()->unbindChord(chord, action);
        }
    }
};

class $modify(KeybindingsLayer) {
    bool init(GameSettingsLayer* settings) {
        bool isLevelBindings = typeinfo_cast<LevelBindingsLayer*>(this) != nullptr;

        if (!isLevelBindings) {
            m_keybindSource = new GeodeKeybindSource;
            m_ownsKeybindSource = true;
        }

        if (!KeybindingsLayer::init(settings)) {
            return false;
        }

        if (isLevelBindings) {
            return true;
        }

        auto keybindsSpr = createGeodeCircleButton(
            CCSprite::createWithSpriteFrameName("keybinds.png"_spr), 1.f,
            CircleBaseSize::Medium
        );
        keybindsSpr->setScale(.8f);
        keybindsSpr->setTopOffset(ccp(0.2f, 1));
        auto keybindsBtn = Button::createWithNode(keybindsSpr, [](Button*) {
            KeybindsPopup::create()->show();
        });

        keybindsBtn->setID("settings-button"_spr);
        m_mainLayer->addChildAtPosition(keybindsBtn, Anchor::Center, { -m_width / 2, -m_height / 2 }, false);

        handleTouchPriority(this);

        return true;
    }
};

static bool s_parsingBlob = false;

class $modify(GJKeybindManager) {
    static char const* actionToString(int action) {
        if (s_parsingBlob) {
            return GJKeybindManager::actionToString(action);
        }

        if (auto name = LoaderImpl::get()->keybindNameForActionId(action); name.has_value()) {
            return name->c_str();
        }

        return GJKeybindManager::actionToString(action);
    }

    void parseBindsBlob(std::string const& blob, bool apply, int* outApplied, int* outUnknown) {
        s_parsingBlob = true;
        GJKeybindManager::parseBindsBlob(blob, apply, outApplied, outUnknown);
        s_parsingBlob = false;
    }
};
#endif
