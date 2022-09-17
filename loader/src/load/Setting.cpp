#include <Geode/loader/Setting.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/loader/SettingNode.hpp>

USE_GEODE_NAMESPACE();

#define PROPAGATE(err) \
    { auto err__ = err; if (!err__) return Err(err__.error()); }

std::string Setting::getKey() const {
    return m_key;
}

Result<std::shared_ptr<Setting>> Setting::parse(
    std::string const& type,
    std::string const& key,
    JsonMaybeObject& obj
) {
    switch (hash(type.c_str())) {
        case hash("bool"):   return BoolSetting::parse(key, obj);
        case hash("int"):    return IntSetting::parse(key, obj);
        case hash("float"):  return FloatSetting::parse(key, obj);
        case hash("string"): return StringSetting::parse(key, obj);
        default: return Err(
            "Setting \"" + key + "\" has unknown type \"" + type + "\""
        );
    }
}

Result<std::shared_ptr<Setting>> Setting::parse(
    std::string const& key,
    nlohmann::json const& rawJson
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
