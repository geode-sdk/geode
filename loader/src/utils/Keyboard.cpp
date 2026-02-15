#include <Geode/loader/Mod.hpp>
#include <Geode/utils/StringMap.hpp>

using namespace geode::prelude;

StringMap<enumKeyCodes> keyNameToCode = {
    { "Backspace", KEY_Backspace },
    { "Tab", KEY_Tab },
    { "Clear", KEY_Clear },
    { "Enter", KEY_Enter },
    { "Shift", KEY_Shift },
    { "Control", KEY_Control },
    { "Alt", KEY_Alt },
    { "Pause", KEY_Pause },
    { "CapsLock", KEY_CapsLock },
    { "Escape", KEY_Escape },
    { "Space", KEY_Space },
    { "PageUp", KEY_PageUp },
    { "PageDown", KEY_PageDown },
    { "End", KEY_End },
    { "Home", KEY_Home },
    { "Left", KEY_Left },
    { "Up", KEY_Up },
    { "Right", KEY_Right },
    { "Down", KEY_Down },
    { "Select", KEY_Select },
    { "Print", KEY_Print },
    { "Execute", KEY_Execute },
    { "PrintScreen", KEY_PrintScreen },
    { "Insert", KEY_Insert },
    { "Delete", KEY_Delete },
    { "Help", KEY_Help },
    { "Zero", KEY_Zero },
    { "One", KEY_One },
    { "Two", KEY_Two },
    { "Three", KEY_Three },
    { "Four", KEY_Four },
    { "Five", KEY_Five },
    { "Six", KEY_Six },
    { "Seven", KEY_Seven },
    { "Eight", KEY_Eight },
    { "Nine", KEY_Nine },
    { "A", KEY_A },
    { "B", KEY_B },
    { "C", KEY_C },
    { "D", KEY_D },
    { "E", KEY_E },
    { "F", KEY_F },
    { "G", KEY_G },
    { "H", KEY_H },
    { "I", KEY_I },
    { "J", KEY_J },
    { "K", KEY_K },
    { "L", KEY_L },
    { "M", KEY_M },
    { "N", KEY_N },
    { "O", KEY_O },
    { "P", KEY_P },
    { "Q", KEY_Q },
    { "R", KEY_R },
    { "S", KEY_S },
    { "T", KEY_T },
    { "U", KEY_U },
    { "V", KEY_V },
    { "W", KEY_W },
    { "X", KEY_X },
    { "Y", KEY_Y },
    { "Z", KEY_Z },
    { "LeftWindowsKey", KEY_LeftWindowsKey },
    { "RightWindowsKey", KEY_RightWindowsKey },
    { "ApplicationsKey", KEY_ApplicationsKey },
    { "Sleep", KEY_Sleep },
    { "NumPad0", KEY_NumPad0 },
    { "NumPad1", KEY_NumPad1 },
    { "NumPad2", KEY_NumPad2 },
    { "NumPad3", KEY_NumPad3 },
    { "NumPad4", KEY_NumPad4 },
    { "NumPad5", KEY_NumPad5 },
    { "NumPad6", KEY_NumPad6 },
    { "NumPad7", KEY_NumPad7 },
    { "NumPad8", KEY_NumPad8 },
    { "NumPad9", KEY_NumPad9 },
    { "Multiply", KEY_Multiply },
    { "Add", KEY_Add },
    { "Separator", KEY_Seperator },
    { "Subtract", KEY_Subtract },
    { "Decimal", KEY_Decimal },
    { "Divide", KEY_Divide },
    { "F1", KEY_F1 },
    { "F2", KEY_F2 },
    { "F3", KEY_F3 },
    { "F4", KEY_F4 },
    { "F5", KEY_F5 },
    { "F6", KEY_F6 },
    { "F7", KEY_F7 },
    { "F8", KEY_F8 },
    { "F9", KEY_F9 },
    { "F10", KEY_F10 },
    { "F11", KEY_F11 },
    { "F12", KEY_F12 },
    { "F13", KEY_F13 },
    { "F14", KEY_F14 },
    { "F15", KEY_F15 },
    { "F16", KEY_F16 },
    { "F17", KEY_F17 },
    { "F18", KEY_F18 },
    { "F19", KEY_F19 },
    { "F20", KEY_F20 },
    { "F21", KEY_F21 },
    { "F22", KEY_F22 },
    { "F23", KEY_F23 },
    { "F24", KEY_F24 },
    { "Numlock", KEY_Numlock },
    { "ScrollLock", KEY_ScrollLock },
    { "LeftShift", KEY_LeftShift },
    { "RightShift", KEY_RightShift },
    { "LeftControl", KEY_LeftControl },
    { "RightControl", KEY_RightControl },
    { "LeftMenu", KEY_LeftMenu },
    { "RightMenu", KEY_RightMenu },
    { "BrowserBack", KEY_BrowserBack },
    { "BrowserForward", KEY_BrowserForward },
    { "BrowserRefresh", KEY_BrowserRefresh },
    { "BrowserStop", KEY_BrowserStop },
    { "BrowserSearch", KEY_BrowserSearch },
    { "BrowserFavorites", KEY_BrowserFavorites },
    { "BrowserHome", KEY_BrowserHome },
    { "VolumeMute", KEY_VolumeMute },
    { "VolumeDown", KEY_VolumeDown },
    { "VolumeUp", KEY_VolumeUp },
    { "NextTrack", KEY_NextTrack },
    { "PreviousTrack", KEY_PreviousTrack },
    { "StopMedia", KEY_StopMedia },
    { "PlayPause", KEY_PlayPause },
    { "LaunchMail", KEY_LaunchMail },
    { "SelectMedia", KEY_SelectMedia },
    { "LaunchApp1", KEY_LaunchApp1 },
    { "LaunchApp2", KEY_LaunchApp2 },
    { "OEM1", KEY_OEM1 },
    { "OEMPlus", KEY_OEMPlus },
    { "OEMComma", KEY_OEMComma },
    { "OEMMinus", KEY_OEMMinus },
    { "OEMPeriod", KEY_OEMPeriod },
    { "OEM2", KEY_OEM2 },
    { "OEM3", KEY_OEM3 },
    { "OEM4", KEY_OEM4 },
    { "OEM5", KEY_OEM5 },
    { "OEM6", KEY_OEM6 },
    { "OEM7", KEY_OEM7 },
    { "OEM8", KEY_OEM8 },
    { "OEM102", KEY_OEM102 },
    { "Process", KEY_Process },
    { "Packet", KEY_Packet },
    { "Attn", KEY_Attn },
    { "CrSel", KEY_CrSel },
    { "ExSel", KEY_ExSel },
    { "EraseEOF", KEY_EraseEOF },
    { "Play", KEY_Play },
    { "Zoom", KEY_Zoom },
    { "PA1", KEY_PA1 },
    { "OEMClear", KEY_OEMClear },
    { "ArrowUp", KEY_ArrowUp },
    { "ArrowDown", KEY_ArrowDown },
    { "ArrowLeft", KEY_ArrowLeft },
    { "ArrowRight", KEY_ArrowRight },
    { "Controller_A", CONTROLLER_A },
    { "Controller_B", CONTROLLER_B },
    { "Controller_Y", CONTROLLER_Y },
    { "Controller_X", CONTROLLER_X },
    { "Controller_Start", CONTROLLER_Start },
    { "Controller_Back", CONTROLLER_Back },
    { "Controller_RB", CONTROLLER_RB },
    { "Controller_LB", CONTROLLER_LB },
    { "Controller_RT", CONTROLLER_RT },
    { "Controller_LT", CONTROLLER_LT },
    { "Controller_Up", CONTROLLER_Up },
    { "Controller_Down", CONTROLLER_Down },
    { "Controller_Left", CONTROLLER_Left },
    { "Controller_Right", CONTROLLER_Right },
    { "Controller_LTHUMBSTICK_UP", CONTROLLER_LTHUMBSTICK_UP },
    { "Controller_LTHUMBSTICK_DOWN", CONTROLLER_LTHUMBSTICK_DOWN },
    { "Controller_LTHUMBSTICK_LEFT", CONTROLLER_LTHUMBSTICK_LEFT },
    { "Controller_LTHUMBSTICK_RIGHT", CONTROLLER_LTHUMBSTICK_RIGHT },
    { "Controller_RTHUMBSTICK_UP", CONTROLLER_RTHUMBSTICK_UP },
    { "Controller_RTHUMBSTICK_DOWN", CONTROLLER_RTHUMBSTICK_DOWN },
    { "Controller_RTHUMBSTICK_LEFT", CONTROLLER_RTHUMBSTICK_LEFT },
    { "Controller_RTHUMBSTICK_RIGHT", CONTROLLER_RTHUMBSTICK_RIGHT },
    { "`", KEY_GraveAccent },
    { "=", KEY_OEMEqual },
    { "[", KEY_LeftBracket },
    { "]", KEY_RightBracket },
    { "\\", KEY_Backslash },
    { ";", KEY_Semicolon },
    { "'", KEY_Apostrophe },
    { "/", KEY_Slash },
    { "=", KEY_NumEnter },
    { "INTL-1", KEY_World1 },
    { "INTL-2", KEY_World2 },
    { "Mouse 4", MOUSE_4 },
    { "Mouse 5", MOUSE_5 },
    { "Mouse 6", MOUSE_6 },
    { "Mouse 7", MOUSE_7 },
    { "Mouse 8", MOUSE_8 },
    { "None", KEY_None },
    { "Unknown", KEY_Unknown }
};

Result<Keybind> Keybind::fromString(std::string_view str) {
    Keybind::Modifiers mods = Keybind::Mods_None;
    size_t pos = str.find('+');
    while (pos != std::string::npos) {
        auto token = str.substr(0, pos);
        if (token == "Ctrl" || token == "Control") {
            mods |= Keybind::Mods_Control;
        }
        else if (token == "Shift") {
            mods |= Keybind::Mods_Shift;
        }
        else if (token == "Alt" || token == "Opt" || token == "Option") {
            mods |= Keybind::Mods_Alt;
        }
        else if (token == "Super" || token == "Cmd" || token == "Command" || token == "Win" || token == "Windows") {
            mods |= Keybind::Mods_Super;
        }
        else {
            return Err(fmt::format("Invalid modifier '{}'", token));
        }
        str.remove_prefix(pos + 1);
        pos = str.find('+');
    }

    auto it = keyNameToCode.find(str);
    if (it == keyNameToCode.end()) {
        return Err(fmt::format("Invalid key '{}'", str));
    }
    return Ok(Keybind(it->second, mods));
}

std::string Keybind::toString() const {
    StringBuffer buf;
    if (modifiers & Mods_Control) {
        buf.append("Ctrl+");
    }
    if (modifiers & Mods_Super) {
        #if defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS)
        buf.append("Cmd+");
        #elif defined(GEODE_IS_WINDOWS)
        buf.append("Win+");
        #else
        buf.append("Super+");
        #endif
    }
    if (modifiers & Mods_Shift) {
        buf.append("Shift+");
    }
    if (modifiers & Mods_Alt) {
        #if defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS)
        buf.append("Opt+");
        #else
        buf.append("Alt+");
        #endif
    }
    auto str = CCKeyboardDispatcher::get()->keyToString(key);
    buf.append(str ? str : "Unknown");
    return buf.str();
}

cocos2d::CCNode* Keybind::createNode() const {
    if (key < 1000 || key > 2000) {
        return CCLabelBMFont::create(this->toString().c_str(), "goldFont.fnt");
    }

    const char* sprite;
    switch (key) {
        case CONTROLLER_A: sprite = "controllerBtn_A_001.png"; break;
        case CONTROLLER_B: sprite = "controllerBtn_B_001.png"; break;
        case CONTROLLER_X: sprite = "controllerBtn_X_001.png"; break;
        case CONTROLLER_Y: sprite = "controllerBtn_Y_001.png"; break;
        case CONTROLLER_Back: sprite = "controllerBtn_Back_001.png"; break;
        case CONTROLLER_Start: sprite = "controllerBtn_Start_001.png"; break;
        case CONTROLLER_Down: sprite = "controllerBtn_DPad_Down_001.png"; break;
        case CONTROLLER_Left: sprite = "controllerBtn_DPad_Left_001.png"; break;
        case CONTROLLER_Up: sprite = "controllerBtn_DPad_Up_001.png"; break;
        case CONTROLLER_Right: sprite = "controllerBtn_DPad_Right_001.png"; break;
        case CONTROLLER_LT: sprite = "controllerBtn_LT_001.png"_spr; break;
        case CONTROLLER_RT: sprite = "controllerBtn_RT_001.png"_spr; break;
        case CONTROLLER_LB: sprite = "controllerBtn_LB_001.png"_spr; break;
        case CONTROLLER_RB: sprite = "controllerBtn_RB_001.png"_spr; break;
        case CONTROLLER_LTHUMBSTICK_DOWN: sprite = "controllerBtn_LThumb_001.png"; break;
        case CONTROLLER_LTHUMBSTICK_LEFT: sprite = "controllerBtn_LThumb_001.png"; break;
        case CONTROLLER_LTHUMBSTICK_RIGHT: sprite = "controllerBtn_LThumb_001.png"; break;
        case CONTROLLER_LTHUMBSTICK_UP: sprite = "controllerBtn_LThumb_001.png"; break;
        case CONTROLLER_RTHUMBSTICK_RIGHT: sprite = "controllerBtn_RThumb_001.png"; break;
        case CONTROLLER_RTHUMBSTICK_DOWN: sprite = "controllerBtn_RThumb_001.png"; break;
        case CONTROLLER_RTHUMBSTICK_LEFT: sprite = "controllerBtn_RThumb_001.png"; break;
        case CONTROLLER_RTHUMBSTICK_UP: sprite = "controllerBtn_RThumb_001.png"; break;
        default: sprite = nullptr;
    }
    if (!sprite) {
        return CCLabelBMFont::create("Unk", "goldFont.fnt");
    }

    auto spr = CCSprite::createWithSpriteFrameName(sprite);
    switch (key) {
        case CONTROLLER_LTHUMBSTICK_DOWN:
        case CONTROLLER_RTHUMBSTICK_DOWN: {
            auto arrow = CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png");
            arrow->setPosition(ccp(13.f, -5.5f));
            arrow->setScale(0.7f);
            spr->addChild(arrow);
            break;
        }
        case CONTROLLER_LTHUMBSTICK_LEFT:
        case CONTROLLER_RTHUMBSTICK_LEFT: {
            auto arrow = CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png");
            arrow->setPosition(ccp(-5.5f, 13.5f));
            arrow->setScale(0.7f);
            arrow->setRotation(90.f);
            spr->addChild(arrow);
            break;
        }
        case CONTROLLER_LTHUMBSTICK_RIGHT:
        case CONTROLLER_RTHUMBSTICK_RIGHT: {
            auto arrow = CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png");
            arrow->setPosition(ccp(31.5f, 13.f));
            arrow->setScale(0.7f);
            arrow->setRotation(270.f);
            spr->addChild(arrow);
            break;
        }
        case CONTROLLER_LTHUMBSTICK_UP:
        case CONTROLLER_RTHUMBSTICK_UP: {
            auto arrow = CCSprite::createWithSpriteFrameName("PBtn_Arrow_001.png");
            arrow->setPosition(ccp(13.f, 31.f));
            arrow->setScale(0.7f);
            arrow->setRotation(180.f);
            spr->addChild(arrow);
            break;
        }
        default: {
            break;
        }
    }
    return spr;
}

Result<Keybind> matjson::Serialize<geode::Keybind>::fromJson(matjson::Value const& json) {
    if (!json.isObject()) {
        return Err("Expected object for Keybind");
    }
    return Ok(Keybind(
        static_cast<enumKeyCodes>(json["key"].asInt().unwrapOr(0)),
        static_cast<Keybind::Modifiers>(json["modifiers"].asInt().unwrapOr(0))
    ));
}

matjson::Value matjson::Serialize<geode::Keybind>::toJson(Keybind const& value) {
    return matjson::makeObject({
        { "key", static_cast<int>(value.key) },
        { "modifiers", static_cast<int>(value.modifiers) }
    });
}
