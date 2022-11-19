#include "../ui/internal/settings/GeodeSettingNode.hpp"

#include <Geode/loader/Setting.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/utils/general.hpp>

USE_GEODE_NAMESPACE();

#define PROPAGATE(err)                         \
    {                                          \
        auto err__ = err;                      \
        if (!err__) return Err(err__.error()); \
    }

std::string Setting::getKey() const {
    return m_key;
}

Result<std::shared_ptr<Setting>> Setting::parse(
    std::string const& type, std::string const& key, JsonMaybeObject<ModJson>& obj
) {
    switch (hash(type.c_str())) {
        case hash("bool"): return BoolSetting::parse(key, obj);
        case hash("int"): return IntSetting::parse(key, obj);
        case hash("float"): return FloatSetting::parse(key, obj);
        case hash("string"): return StringSetting::parse(key, obj);
        case hash("rgb"):
        case hash("color"): return ColorSetting::parse(key, obj);
        case hash("rgba"): return ColorAlphaSetting::parse(key, obj);
        case hash("path"):
        case hash("file"): return FileSetting::parse(key, obj);
        default: return Err("Setting \"" + key + "\" has unknown type \"" + type + "\"");
    }
}

Result<std::shared_ptr<Setting>> Setting::parse(std::string const& key, ModJson const& rawJson) {
    if (rawJson.is_object()) {
        auto json = rawJson;
        JsonChecker checker(json);
        auto root = checker.root("[setting \"" + key + "\"]").obj();

        auto res = Setting::parse(root.needs("type").get<std::string>(), key, root);
        root.checkUnknownKeys();
        if (checker.isError()) {
            return Err(checker.getError());
        }
        return res;
    }
    return Err("Setting value is not an object");
}

void Setting::valueChanged() {
    SettingChangedEvent(m_modID, this).post();
}

SettingNode* BoolSetting::createNode(float width) {
    return BoolSettingNode::create(
        std::static_pointer_cast<BoolSetting>(shared_from_this()), width
    );
}

SettingNode* IntSetting::createNode(float width) {
    return IntSettingNode::create(std::static_pointer_cast<IntSetting>(shared_from_this()), width);
}

SettingNode* FloatSetting::createNode(float width) {
    return FloatSettingNode::create(
        std::static_pointer_cast<FloatSetting>(shared_from_this()), width
    );
}

SettingNode* StringSetting::createNode(float width) {
    return StringSettingNode::create(
        std::static_pointer_cast<StringSetting>(shared_from_this()), width
    );
}

SettingNode* FileSetting::createNode(float width) {
    return FileSettingNode::create(
        std::static_pointer_cast<FileSetting>(shared_from_this()), width
    );
}

SettingNode* ColorSetting::createNode(float width) {
    return ColorSettingNode::create(
        std::static_pointer_cast<ColorSetting>(shared_from_this()), width
    );
}

SettingNode* ColorAlphaSetting::createNode(float width) {
    return ColorAlphaSettingNode::create(
        std::static_pointer_cast<ColorAlphaSetting>(shared_from_this()), width
    );
}

SettingChangedEvent::SettingChangedEvent(
    std::string const& modID, Setting* setting
) :
    m_modID(modID),
    m_setting(setting) {}

std::string SettingChangedEvent::getModID() const {
    return m_modID;
}

Setting* SettingChangedEvent::getSetting() const {
    return m_setting;
}
