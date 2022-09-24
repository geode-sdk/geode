#include <Geode/loader/Setting.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/loader/SettingNode.hpp>
#include "../ui/internal/settings/GeodeSettingNode.hpp"

USE_GEODE_NAMESPACE();

#define PROPAGATE(err) \
    { auto err__ = err; if (!err__) return Err(err__.error()); }

std::string Setting::getKey() const {
    return m_key;
}

Result<std::shared_ptr<Setting>> Setting::parse(
    std::string const& type,
    std::string const& key,
    JsonMaybeObject<ModJson>& obj
) {
    switch (hash(type.c_str())) {
        case hash("bool"):   return BoolSetting::parse(key, obj);
        case hash("int"):    return IntSetting::parse(key, obj);
        case hash("float"):  return FloatSetting::parse(key, obj);
        case hash("string"): return StringSetting::parse(key, obj);
        case hash("rgb"):
        case hash("color"):  return ColorSetting::parse(key, obj);
        case hash("rgba"):   return ColorAlphaSetting::parse(key, obj);
        case hash("path"):
        case hash("file"):   return FileSetting::parse(key, obj);
        default: return Err(
            "Setting \"" + key + "\" has unknown type \"" + type + "\""
        );
    }
}

Result<std::shared_ptr<Setting>> Setting::parse(
    std::string const& key,
    ModJson const& rawJson
) {
    if (rawJson.is_object()) {
        auto json = rawJson;
        JsonChecker checker(json);
        auto root = checker.root("[setting \"" + key + "\"]").obj();

        auto res = Setting::parse(
            root.needs("type").get<std::string>(), key, root
        );
        root.checkUnknownKeys();
        if (checker.isError()) {
            return Err(checker.getError());
        }
        return res;
    }
    return Err("Setting value is not an object");
}

SettingNode* BoolSetting::createNode(float width) {
    return BoolSettingNode::create(shared_from_this(), width);
}

SettingNode* IntSetting::createNode(float width) {
    return IntSettingNode::create(shared_from_this(), width);
}

SettingNode* FloatSetting::createNode(float width) {
    return FloatSettingNode::create(shared_from_this(), width);
}

SettingNode* StringSetting::createNode(float width) {
    return StringSettingNode::create(shared_from_this(), width);
}

SettingNode* FileSetting::createNode(float width) {
    return FileSettingNode::create(shared_from_this(), width);
}

SettingNode* ColorSetting::createNode(float width) {
    return ColorSettingNode::create(shared_from_this(), width);
}

SettingNode* ColorAlphaSetting::createNode(float width) {
    return ColorAlphaSettingNode::create(shared_from_this(), width);
}
