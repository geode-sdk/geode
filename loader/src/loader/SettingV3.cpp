#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/loader/ModSettingsManager.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <regex>
#include "SettingNodeV3.hpp"

using namespace geode::prelude;

namespace enable_if_parsing {
    struct Component {
        virtual ~Component() = default;
        virtual Result<> check() const = 0;
        virtual Result<> eval(std::string const& defaultModID) const = 0;
    };
    struct RequireModLoaded final : public Component {
        std::string modID;
        RequireModLoaded(std::string const& modID)
          : modID(modID) {}
        
        Result<> check() const override {
            return Ok();
        }
        Result<> eval(std::string const& defaultModID) const override {
            if (Loader::get()->getLoadedMod(modID)) {
                return Ok();
            }
            auto modName = modID;
            if (auto mod = Loader::get()->getInstalledMod(modID)) {
                modName = mod->getName();
            }
            return Err("Enable the mod {}", modName);
        }
    };
    struct RequireSettingEnabled final : public Component {
        std::string modID;
        std::string settingID;
        RequireSettingEnabled(std::string const& modID, std::string const& settingID)
          : modID(modID), settingID(settingID) {}
        
        Result<> check() const override {
            if (auto mod = Loader::get()->getInstalledMod(modID)) {
                if (!mod->hasSetting(settingID)) {
                    return Err("Mod '{}' does not have setting '{}'", mod->getName(), settingID);
                }
                if (!typeinfo_pointer_cast<BoolSettingV3>(mod->getSettingV3(settingID))) {
                    return Err("Setting '{}' in mod '{}' is not a boolean setting", settingID, mod->getName());
                }
            }
            return Ok();
        }
        Result<> eval(std::string const& defaultModID) const override {
            if (auto mod = Loader::get()->getLoadedMod(modID)) {
                if (mod->template getSettingValue<bool>(settingID)) {
                    return Ok();
                }
                // This is an if-check just in case, even though check() should already 
                // make sure that getSettingV3 is guaranteed to return true
                auto name = settingID;
                if (auto sett = mod->getSettingV3(settingID)) {
                    name = sett->getDisplayName();
                }
                if (modID == defaultModID) {
                    return Err("Enable the setting '{}'", name);
                }
                return Err("Enable the setting '{}' from the mod {}", name, mod->getName());
            }
            auto modName = modID;
            if (auto mod = Loader::get()->getInstalledMod(modID)) {
                modName = mod->getName();
            }
            return Err("Enable the mod {}", modName);
        }
    };
    struct RequireSavedValueEnabled final : public Component {
        std::string modID;
        std::string savedValue;
        RequireSavedValueEnabled(std::string const& modID, std::string const& savedValue)
          : modID(modID), savedValue(savedValue) {}
        
        Result<> check() const override {
            return Ok();
        }
        Result<> eval(std::string const& defaultModID) const override {
            if (auto mod = Loader::get()->getLoadedMod(modID)) {
                if (mod->template getSavedValue<bool>(savedValue)) {
                    return Ok();
                }
                if (modID == defaultModID) {
                    return Err("Enable the value '{}'", savedValue);
                }
                return Err("Enable the value '{}' from the mod {}", savedValue, mod->getName());
            }
            auto modName = modID;
            if (auto mod = Loader::get()->getInstalledMod(modID)) {
                modName = mod->getName();
            }
            return Err("Enable the mod {}", modName);
        }
    };
    struct RequireNot final : public Component {
        std::unique_ptr<Component> component;
        RequireNot(std::unique_ptr<Component>&& component)
          : component(std::move(component)) {}
        
        Result<> check() const override {
            return component->check();
        }
        Result<> eval(std::string const& defaultModID) const override {
            if (auto res = component->eval(defaultModID)) {
                // Surely this will never break!
                auto str = res.unwrapErr();
                string::replaceIP(str, "Enable", "___TEMP");
                string::replaceIP(str, "Disable", "Enable");
                string::replaceIP(str, "___TEMP", "Disable");
                return Err(str);
            }
            return Ok();
        }
    };
    struct RequireAll final : public Component {
        std::vector<std::unique_ptr<Component>> components;
        RequireAll(std::vector<std::unique_ptr<Component>>&& components)
          : components(std::move(components)) {}

        Result<> check() const override {
            for (auto& comp : components) {
                GEODE_UNWRAP(comp->check());
            }
            return Ok();
        }
        Result<> eval(std::string const& defaultModID) const override {
            // Only print out whatever the first erroring condition is to not shit out 
            // "Please enable X and Y and Z and Ö and Å and"
            for (auto& comp : components) {
                GEODE_UNWRAP(comp->eval(defaultModID));
            }
            return Ok();
        }
    };
    struct RequireSome final : public Component {
        std::vector<std::unique_ptr<Component>> components;
        RequireSome(std::vector<std::unique_ptr<Component>>&& components)
          : components(std::move(components)) {}

        Result<> check() const override {
            for (auto& comp : components) {
                GEODE_UNWRAP(comp->check());
            }
            return Ok();
        }
        Result<> eval(std::string const& defaultModID) const override {
            Result<> err = Ok();
            for (auto& comp : components) {
                auto res = comp->eval(defaultModID);
                if (res) {
                    return Ok();
                }
                // Only show first condition that isn't met
                if (err.isOk()) {
                    err = Err(res.unwrapErr());
                }
            }
            return err;
        }
    };

    static bool isComponentStartChar(char c) {
        return
            ('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            c == '_';
    }
    static bool isComponentContinueChar(char c) {
        return
            ('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            ('0' <= c && c <= '9') ||
            c == '_' || c == '-' || c == '/' ||
            c == '.' || c == ':';
    }

    class Parser final {
    private:
        std::string_view m_src;
        size_t m_index = 0;
        std::string m_defaultModID;

        static bool isUnOpWord(std::string_view op) {
            return op == "!";
        }
        static bool isBiOpWord(std::string_view op) {
            return op == "&&" || op == "||";
        }
        
        Result<std::optional<std::string_view>> nextWord() {
            // Skip whitespace
            while (m_index < m_src.size() && std::isspace(m_src[m_index])) {
                m_index += 1;
            }
            if (m_index == m_src.size()) {
                return Ok(std::nullopt);
            }
            // Parentheses & single operators
            if (m_src[m_index] == '(' || m_src[m_index] == ')' || m_src[m_index] == '!') {
                m_index += 1;
                return Ok(m_src.substr(m_index - 1, 1));
            }
            // Double-character operators
            if (m_src[m_index] == '&' || m_src[m_index] == '|') {
                // Consume first character
                m_index += 1;
                // Next character must be the same
                if (m_index == m_src.size() || m_src[m_index - 1] != m_src[m_index]) {
                    return Err("Expected '{}' at index {}", m_src[m_index - 1], m_index - 1);
                }
                // Consume second character
                m_index += 1;
                return Ok(m_src.substr(m_index - 2, 2));
            }
            // Components
            if (isComponentStartChar(m_src[m_index])) {
                auto start = m_index;
                m_index += 1;
                while (m_index < m_src.size() && isComponentContinueChar(m_src[m_index])) {
                    m_index += 1;
                }
                return Ok(m_src.substr(start, m_index - start));
            }
            return Err("Unexpected character '{}' at index {}", m_src[m_index], m_index);
        }
        std::optional<std::string_view> peekWord() {
            auto original = m_index;
            auto ret = this->nextWord();
            m_index = original;
            return ret ? *ret : std::nullopt;
        }
        Result<std::unique_ptr<Component>> nextComponent() {
            GEODE_UNWRAP_INTO(auto maybeWord, this->nextWord());
            if (!maybeWord) {
                return Err("Expected component, got end-of-enable-if-string");
            }
            const auto word = *maybeWord;
            if (isUnOpWord(word) || isBiOpWord(word)) {
                return Err("Expected component, got operator \"{}\" at index {}", word, m_index - word.size());
            }
            if (word == ")") {
                return Err("Unexpected closing parenthesis at index {}", m_index - 1);
            }
            if (word == "(") {
                GEODE_UNWRAP_INTO(auto op, this->next());
                GEODE_UNWRAP_INTO(auto maybeClosing, this->nextWord());
                if (!maybeClosing) {
                    return Err("Expected closing parenthesis, got end-of-enable-if-string");
                }
                if (maybeClosing != ")") {
                    return Err(
                        "Expected closing parenthesis, got \"{}\" at index {}",
                        *maybeClosing, m_index - maybeClosing->size()
                    );
                }
                return Ok(std::move(op));
            }
            std::string_view ty = "setting";
            std::string_view value = word;
            if (word.find(':') != std::string::npos) {
                ty = word.substr(0, word.find(':'));
                value = word.substr(word.find(':') + 1);
            }
            switch (hash(ty)) {
                case hash("setting"): {
                    std::string modID = m_defaultModID;
                    std::string settingID = std::string(value);
                    // mod.id/setting-id
                    if (value.find('/') != std::string::npos) {
                        modID = value.substr(0, value.find('/'));
                        settingID = value.substr(value.find('/') + 1);
                    }
                    if (!ModMetadata::validateID(std::string(modID))) {
                        return Err("Invalid mod ID '{}'", modID);
                    }
                    return Ok(std::make_unique<RequireSettingEnabled>(modID, settingID));
                } break;

                case hash("saved"): {
                    std::string modID = m_defaultModID;
                    std::string savedValue = std::string(value);
                    // mod.id/setting-id
                    if (value.find('/') != std::string::npos) {
                        modID = value.substr(0, value.find('/'));
                        savedValue = value.substr(value.find('/') + 1);
                    }
                    if (!ModMetadata::validateID(std::string(modID))) {
                        return Err("Invalid mod ID '{}'", modID);
                    }
                    return Ok(std::make_unique<RequireSavedValueEnabled>(modID, savedValue));
                } break;

                case hash("loaded"): {
                    if (!ModMetadata::validateID(std::string(value))) {
                        return Err("Invalid mod ID '{}'", value);
                    }
                    return Ok(std::make_unique<RequireModLoaded>(std::string(value)));
                } break;

                default: {
                    return Err("Invalid designator '{}' at index {}", ty, m_index - word.size());
                } break;
            }
        }
        Result<std::unique_ptr<Component>> nextUnOp() {
            std::string op;
            if (auto peek = this->peekWord()) {
                if (isUnOpWord(*peek)) {
                    op = *peek;
                }
            }
            GEODE_UNWRAP_INTO(auto comp, this->nextComponent());
            if (op.empty()) {
                return Ok(std::move(comp));
            }
            switch (hash(op)) {
                case hash("!"): {
                    return Ok(std::make_unique<RequireNot>(std::move(comp)));
                } break;
                default: {
                    return Err(
                        "THIS SHOULD BE UNREACHABLE!! \"{}\" was an unhandled "
                        "unary operator despite isUnOpWord claiming it's valid! "
                        "REPORT THIS BUG TO GEODE DEVELOPERS",
                        op
                    );
                } break;
            }
        }
        Result<std::unique_ptr<Component>> nextBiOp() {
            GEODE_UNWRAP_INTO(auto first, this->nextUnOp());
            std::string firstOp;
            std::vector<std::unique_ptr<Component>> components;
            while (auto peek = this->peekWord()) {
                if (!isBiOpWord(*peek)) {
                    break;
                }
                GEODE_UNWRAP_INTO(auto word, this->nextWord());
                auto op = *word;
                if (firstOp.empty()) {
                    firstOp = op;
                }
                if (op != firstOp) {
                    return Err(
                        "Expected operator \"{}\", got operator \"{}\" - "
                        "parentheses are required to disambiguate operator chains",
                        firstOp, op
                    );
                }
                GEODE_UNWRAP_INTO(auto comp, this->nextUnOp());
                components.emplace_back(std::move(comp));
            }
            if (components.size()) {
                components.emplace(components.begin(), std::move(first));
                switch (hash(firstOp)) {
                    case hash("&&"): {
                        return Ok(std::make_unique<RequireAll>(std::move(components)));
                    } break;
                    case hash("||"): {
                        return Ok(std::make_unique<RequireSome>(std::move(components)));
                    } break;
                    default: {
                        return Err(
                            "THIS SHOULD BE UNREACHABLE!! \"{}\" was an unhandled "
                            "binary operator despite isBiOpWord claiming it's valid! "
                            "REPORT THIS BUG TO GEODE DEVELOPERS",
                            firstOp
                        );
                    } break;
                }
            }
            return Ok(std::move(first));
        }
        Result<std::unique_ptr<Component>> next() {
            return this->nextBiOp();
        }
    
    public:
        static Result<std::unique_ptr<Component>> parse(std::string_view str, std::string const& defaultModID) {
            auto ret = Parser();
            ret.m_src = str;
            ret.m_defaultModID = defaultModID;
            GEODE_UNWRAP_INTO(auto comp, ret.next());
            GEODE_UNWRAP_INTO(auto shouldBeEOF, ret.nextWord());
            if (shouldBeEOF) {
                return Err(
                    "Expected end-of-enable-if-string, got \"{}\" at index {}",
                    *shouldBeEOF, ret.m_index - shouldBeEOF->size()
                );
            }
            return Ok(std::move(comp));
        }
    };
}

class SettingChangedEventV3::Impl final {
public:
    std::shared_ptr<SettingV3> setting;
};

SettingChangedEventV3::SettingChangedEventV3(std::shared_ptr<SettingV3> setting)
  : m_impl(std::make_shared<Impl>()) 
{
    m_impl->setting = setting;
}

std::shared_ptr<SettingV3> SettingChangedEventV3::getSetting() const {
    return m_impl->setting;
}

class SettingChangedFilterV3::Impl final {
public:
    std::string modID;
    std::optional<std::string> settingKey;
};

ListenerResult SettingChangedFilterV3::handle(utils::MiniFunction<Callback> fn, SettingChangedEventV3* event) {
    if (
        event->getSetting()->getModID() == m_impl->modID &&
        !m_impl->settingKey || event->getSetting()->getKey() == m_impl->settingKey
    ) {
        fn(event->getSetting());
    }
    return ListenerResult::Propagate;
}

SettingChangedFilterV3::SettingChangedFilterV3(
    std::string const& modID,
    std::optional<std::string> const& settingKey
) : m_impl(std::make_shared<Impl>())
{
    m_impl->modID = modID;
    m_impl->settingKey = settingKey;
}

SettingChangedFilterV3::SettingChangedFilterV3(Mod* mod, std::optional<std::string> const& settingKey)
  : SettingChangedFilterV3(mod->getID(), settingKey) {}

SettingChangedFilterV3::SettingChangedFilterV3(SettingChangedFilterV3 const&) = default;

EventListener<SettingChangedFilterV3>* geode::listenForAllSettingChanges(
    std::function<void(std::shared_ptr<SettingV3>)> const& callback,
    Mod* mod
) {
    return new EventListener(
        [callback](std::shared_ptr<SettingV3> setting) {
            callback(setting);
        },
        SettingChangedFilterV3(mod->getID(), std::nullopt)
    );
}

class SettingV3::GeodeImpl {
public:
    std::string modID;
    std::string key;
    std::vector<PlatformID> platforms;
    std::optional<std::string> name;
    std::optional<std::string> description;
    std::optional<std::string> enableIf;
    std::unique_ptr<enable_if_parsing::Component> enableIfTree;
    std::optional<std::string> enableIfDescription;
    bool requiresRestart = false;
};

SettingV3::SettingV3() : m_impl(std::make_shared<GeodeImpl>()) {}
SettingV3::~SettingV3() = default;

void SettingV3::init(std::string const& key, std::string const& modID) {
    m_impl->key = key;
    m_impl->modID = modID;
}
void SettingV3::init(std::string const& key, std::string const& modID, JsonExpectedValue& json) {
    this->init(key, modID);

    // Keys every setting must have
    json.needs("type");
    for (auto& plat : json.has("platforms").items()) {
        ranges::push(m_impl->platforms, PlatformID::getCovered(plat.template get<std::string>()));
    }
}

void SettingV3::parseNameAndDescription(JsonExpectedValue& json) {
    json.has("name").into(m_impl->name);
    json.has("description").into(m_impl->description);
}
void SettingV3::parseEnableIf(JsonExpectedValue& json) {
    json.has("enable-if")
        .template mustBe<std::string>("a valid \"enable-if\" scheme", [this](std::string const& str) -> Result<> {
            GEODE_UNWRAP_INTO(auto tree, enable_if_parsing::Parser::parse(str, m_impl->modID));
            GEODE_UNWRAP(tree->check());
            m_impl->enableIfTree = std::move(tree);
            return Ok();
        })
        .into(m_impl->enableIf);
    json.has("enable-if-description").into(m_impl->enableIfDescription);
}
void SettingV3::parseValueProperties(JsonExpectedValue& json) {
    json.has("requires-restart").into(m_impl->requiresRestart);
}

Result<> SettingV3::parseBaseProperties(std::string const& key, std::string const& modID, matjson::Value const& value) {
    auto json = checkJson(value, "SettingV3");
    this->parseBaseProperties(key, modID, json);
    return json.ok();
}
void SettingV3::parseBaseProperties(std::string const& key, std::string const& modID, JsonExpectedValue& json) {
    this->init(key, modID, json);
    this->parseNameAndDescription(json);
    this->parseValueProperties(json);
    this->parseEnableIf(json);
}

std::string SettingV3::getKey() const {
    return m_impl->key;
}
std::string SettingV3::getModID() const {
    return m_impl->modID;
}
std::optional<std::string> SettingV3::getName() const {
    return m_impl->name;
}
std::string SettingV3::getDisplayName() const {
    return m_impl->name.value_or(m_impl->key);
}
std::optional<std::string> SettingV3::getDescription() const {
    return m_impl->description;
}
std::optional<std::string> SettingV3::getEnableIf() const {
    return m_impl->enableIf;
}
bool SettingV3::shouldEnable() const {
    if (m_impl->enableIfTree) {
        return m_impl->enableIfTree->eval(m_impl->modID).isOk();
    }
    return true;
}
std::optional<std::string> SettingV3::getEnableIfDescription() const {
    if (m_impl->enableIfDescription) {
        return *m_impl->enableIfDescription;
    }
    if (!m_impl->enableIfTree) {
        return std::nullopt;
    }
    auto res = m_impl->enableIfTree->eval(m_impl->modID);
    if (res) {
        return std::nullopt;
    }
    return res.unwrapErr();
}
bool SettingV3::requiresRestart() const {
    return m_impl->requiresRestart;
}
std::vector<PlatformID> SettingV3::getPlatforms() const {
    return m_impl->platforms;
}
Mod* SettingV3::getMod() const {
    return Loader::get()->getInstalledMod(m_impl->modID);
}

void SettingV3::markChanged() {
    auto manager = ModSettingsManager::from(this->getMod());
    if (m_impl->requiresRestart) {
        manager->markRestartRequired();
    }
    SettingChangedEventV3(shared_from_this()).post();
    if (manager) {
        // Use ModSettingsManager rather than convertToLegacyValue since it 
        // caches the result and we want to have that for performance
        SettingChangedEvent(this->getMod(), manager->getLegacy(this->getKey()).get()).post();
    }
}

std::optional<Setting> SettingV3::convertToLegacy() const {
    return std::nullopt;
}
std::optional<std::shared_ptr<SettingValue>> SettingV3::convertToLegacyValue() const {
    return std::nullopt;
}

class TitleSettingV3::Impl final {
public:
};

TitleSettingV3::TitleSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<TitleSettingV3>> TitleSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<TitleSettingV3>(PrivateMarker());
    auto root = checkJson(json, "TitleSettingV3");
    ret->init(key, modID, root);
    ret->parseNameAndDescription(root);
    root.checkUnknownKeys();
    return root.ok(ret);
}

bool TitleSettingV3::load(matjson::Value const& json) {
    return true;
}
bool TitleSettingV3::save(matjson::Value&) const {
    return true;
}
SettingNodeV3* TitleSettingV3::createNode(float width) {
    return TitleSettingNodeV3::create(
        std::static_pointer_cast<TitleSettingV3>(shared_from_this()), width
    );
}
bool TitleSettingV3::isDefaultValue() const {
    return true;
}
void TitleSettingV3::reset() {}

class LegacyCustomSettingV3::Impl final {
public:
    matjson::Value json;
    std::shared_ptr<SettingValue> legacyValue = nullptr;
};

LegacyCustomSettingV3::LegacyCustomSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<LegacyCustomSettingV3>> LegacyCustomSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<LegacyCustomSettingV3>(PrivateMarker());
    ret->init(key, modID);
    ret->m_impl->json = json;
    return Ok(ret);
}

std::shared_ptr<SettingValue> LegacyCustomSettingV3::getValue() const {
    return m_impl->legacyValue;
}
void LegacyCustomSettingV3::setValue(std::shared_ptr<SettingValue> value) {
    m_impl->legacyValue = value;
}

bool LegacyCustomSettingV3::load(matjson::Value const& json) {
    if (m_impl->legacyValue) {
        return m_impl->legacyValue->load(json);
    }
    return true;
}
bool LegacyCustomSettingV3::save(matjson::Value& json) const {
    if (m_impl->legacyValue) {
        return m_impl->legacyValue->save(json);
    }
    return true;
}
SettingNodeV3* LegacyCustomSettingV3::createNode(float width) {
    if (m_impl->legacyValue) {
        return LegacyCustomSettingToV3Node::create(
            std::static_pointer_cast<LegacyCustomSettingV3>(shared_from_this()), width
        );
    }
    return UnresolvedCustomSettingNodeV3::create(this->getKey(), this->getMod(), width);
}

bool LegacyCustomSettingV3::isDefaultValue() const {
    return true;
}
void LegacyCustomSettingV3::reset() {}

std::optional<Setting> LegacyCustomSettingV3::convertToLegacy() const {
    return Setting(this->getKey(), this->getModID(), SettingKind(CustomSetting {
        .json = std::make_shared<ModJson>(m_impl->json)
    }));
}
std::optional<std::shared_ptr<SettingValue>> LegacyCustomSettingV3::convertToLegacyValue() const {
    return m_impl->legacyValue ? std::optional(m_impl->legacyValue) : std::nullopt;
}

class BoolSettingV3::Impl final {
public:
};

BoolSettingV3::BoolSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<BoolSettingV3>> BoolSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<BoolSettingV3>(PrivateMarker());
    auto root = checkJson(json, "BoolSettingV3");
    ret->parseBaseProperties(key, modID, root);
    root.checkUnknownKeys();
    return root.ok(ret);
}

Result<> BoolSettingV3::isValid(bool value) const {
    return Ok();
}

SettingNodeV3* BoolSettingV3::createNode(float width) {
    return BoolSettingNodeV3::create(
        std::static_pointer_cast<BoolSettingV3>(shared_from_this()), width
    );
}

std::optional<Setting> BoolSettingV3::convertToLegacy() const {
    return Setting(this->getKey(), this->getModID(), SettingKind(BoolSetting {
        .name = this->getName(),
        .description = this->getDescription(),
        .defaultValue = this->getDefaultValue(),
    }));
}
std::optional<std::shared_ptr<SettingValue>> BoolSettingV3::convertToLegacyValue() const {
    return this->convertToLegacy()->createDefaultValue();
}

class IntSettingV3::Impl final {
public:
    std::optional<int64_t> minValue;
    std::optional<int64_t> maxValue;

    struct {
        // 0 means not enabled
        size_t arrowStepSize = 1;
        size_t bigArrowStepSize = 5;
        bool sliderEnabled = true;
        int64_t sliderSnap = 1;
        bool textInputEnabled = true;
    } controls;
};

Result<std::shared_ptr<IntSettingV3>> IntSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<IntSettingV3>(PrivateMarker());
    
    auto root = checkJson(json, "IntSettingV3");
    ret->parseBaseProperties(key, modID, root);

    root.has("min").into(ret->m_impl->minValue);
    root.has("max").into(ret->m_impl->maxValue);
    if (auto controls = root.has("control")) {
        controls.has("arrows");
        controls.has("big-arrows");
        controls.has("arrow-step").into(ret->m_impl->controls.arrowStepSize);
        controls.has("big-arrow-step").into(ret->m_impl->controls.bigArrowStepSize);
        controls.has("slider").into(ret->m_impl->controls.sliderEnabled);
        controls.has("slider-step").into(ret->m_impl->controls.sliderSnap);
        controls.has("input").into(ret->m_impl->controls.textInputEnabled);
        controls.checkUnknownKeys();
    }
    
    // Disable arrows if they aren't enabled
    // This silly code is because step size being 0 is what defines if they are enabled

    // Small arrows are enabled by default
    if (!root.has("control").has("arrows").template get<bool>(true)) {
        ret->m_impl->controls.arrowStepSize = 0;
    }
    if (!root.has("control").has("big-arrows").template get<bool>()) {
        ret->m_impl->controls.bigArrowStepSize = 0;
    }

    // Without "min" or "max" slider makes no sense
    if (!ret->m_impl->minValue || !ret->m_impl->maxValue) {
        if (ret->m_impl->controls.sliderEnabled && root.has("control").has("slider")) {
            log::warn(
                "Setting '{}' has \"controls.slider\" enabled but doesn't "
                "have both \"min\" and \"max\" defined - the slider has "
                "been force-disabled!",
                key
            );
        }
        ret->m_impl->controls.sliderEnabled = false;
    }

    root.checkUnknownKeys();
    return root.ok(ret);
}

IntSettingV3::IntSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<> IntSettingV3::isValid(int64_t value) const {
    if (m_impl->minValue && value < *m_impl->minValue) {
        return Err("Value must be at least {}", *m_impl->minValue);
    }
    if (m_impl->maxValue && value > *m_impl->maxValue) {
        return Err("Value must be at most {}", *m_impl->maxValue);
    }
    return Ok();
}

std::optional<int64_t> IntSettingV3::getMinValue() const {
    return m_impl->minValue;
}
std::optional<int64_t> IntSettingV3::getMaxValue() const {
    return m_impl->maxValue;
}

bool IntSettingV3::isArrowsEnabled() const {
    return m_impl->controls.arrowStepSize > 0;
}
bool IntSettingV3::isBigArrowsEnabled() const {
    return m_impl->controls.bigArrowStepSize > 0;
}
size_t IntSettingV3::getArrowStepSize() const {
    return m_impl->controls.arrowStepSize;
}
size_t IntSettingV3::getBigArrowStepSize() const {
    return m_impl->controls.bigArrowStepSize;
}
bool IntSettingV3::isSliderEnabled() const {
    return m_impl->controls.sliderEnabled;
}
int64_t IntSettingV3::getSliderSnap() const {
    return m_impl->controls.sliderSnap;
}
bool IntSettingV3::isInputEnabled() const {
    return m_impl->controls.textInputEnabled;
}

SettingNodeV3* IntSettingV3::createNode(float width) {
    return IntSettingNodeV3::create(
        std::static_pointer_cast<IntSettingV3>(shared_from_this()), width
    );
}

std::optional<Setting> IntSettingV3::convertToLegacy() const {
    return Setting(this->getKey(), this->getModID(), SettingKind(IntSetting {
        .name = this->getName(),
        .description = this->getDescription(),
        .defaultValue = this->getDefaultValue(),
        .min = this->getMinValue(),
        .max = this->getMaxValue(),
        .controls = {
            .arrows = this->isArrowsEnabled(),
            .bigArrows = this->isBigArrowsEnabled(),
            .arrowStep = this->getArrowStepSize(),
            .bigArrowStep = this->getBigArrowStepSize(),
            .slider = this->isSliderEnabled(),
            .sliderStep = this->getSliderSnap(),
            .input = this->isInputEnabled(),
        },
    }));
}
std::optional<std::shared_ptr<SettingValue>> IntSettingV3::convertToLegacyValue() const {
    return this->convertToLegacy()->createDefaultValue();
}

class FloatSettingV3::Impl final {
public:
    std::optional<double> minValue;
    std::optional<double> maxValue;

    struct {
        // 0 means not enabled
        double arrowStepSize = 1;
        double bigArrowStepSize = 5;
        bool sliderEnabled = true;
        double sliderSnap = 0.1;
        bool textInputEnabled = true;
    } controls;
};

FloatSettingV3::FloatSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<FloatSettingV3>> FloatSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<FloatSettingV3>(PrivateMarker());

    auto root = checkJson(json, "FloatSettingV3");
    ret->parseBaseProperties(key, modID, root);

    root.has("min").into(ret->m_impl->minValue);
    root.has("max").into(ret->m_impl->maxValue);
    if (auto controls = root.has("control")) {
        controls.has("arrows");
        controls.has("big-arrows");
        controls.has("arrow-step").into(ret->m_impl->controls.arrowStepSize);
        controls.has("big-arrow-step").into(ret->m_impl->controls.bigArrowStepSize);
        controls.has("slider").into(ret->m_impl->controls.sliderEnabled);
        controls.has("slider-step").into(ret->m_impl->controls.sliderSnap);
        controls.has("input").into(ret->m_impl->controls.textInputEnabled);
        controls.checkUnknownKeys();
    }

    // Disable arrows if they aren't enabled
    // Small arrows are enabled by default
    if (!root.has("control").has("arrows").template get<bool>(true)) {
        ret->m_impl->controls.arrowStepSize = 0;
    }
    if (!root.has("control").has("big-arrows").template get<bool>()) {
        ret->m_impl->controls.bigArrowStepSize = 0;
    }

    // Without "min" or "max" slider makes no sense
    if (!ret->m_impl->minValue || !ret->m_impl->maxValue) {
        if (ret->m_impl->controls.sliderEnabled && root.has("control").has("slider")) {
            log::warn(
                "Setting '{}' has \"controls.slider\" enabled but doesn't "
                "have both \"min\" and \"max\" defined - the slider has "
                "been force-disabled!",
                key
            );
        }
        ret->m_impl->controls.sliderEnabled = false;
    }

    root.checkUnknownKeys();
    return root.ok(ret);
}

Result<> FloatSettingV3::isValid(double value) const {
    if (m_impl->minValue && value < *m_impl->minValue) {
        return Err("Value must be at least {}", *m_impl->minValue);
    }
    if (m_impl->maxValue && value > *m_impl->maxValue) {
        return Err("Value must be at most {}", *m_impl->maxValue);
    }
    return Ok();
}

std::optional<double> FloatSettingV3::getMinValue() const {
    return m_impl->minValue;
}
std::optional<double> FloatSettingV3::getMaxValue() const {
    return m_impl->maxValue;
}

bool FloatSettingV3::isArrowsEnabled() const {
    return m_impl->controls.arrowStepSize > 0;
}
bool FloatSettingV3::isBigArrowsEnabled() const {
    return m_impl->controls.bigArrowStepSize > 0;
}
double FloatSettingV3::getArrowStepSize() const {
    return m_impl->controls.arrowStepSize;
}
double FloatSettingV3::getBigArrowStepSize() const {
    return m_impl->controls.bigArrowStepSize;
}
bool FloatSettingV3::isSliderEnabled() const {
    return m_impl->controls.sliderEnabled;
}
double FloatSettingV3::getSliderSnap() const {
    return m_impl->controls.sliderSnap;
}
bool FloatSettingV3::isInputEnabled() const {
    return m_impl->controls.textInputEnabled;
}

SettingNodeV3* FloatSettingV3::createNode(float width) {
    return FloatSettingNodeV3::create(
        std::static_pointer_cast<FloatSettingV3>(shared_from_this()), width
    );
}

std::optional<Setting> FloatSettingV3::convertToLegacy() const {
    return Setting(this->getKey(), this->getModID(), SettingKind(FloatSetting {
        .name = this->getName(),
        .description = this->getDescription(),
        .defaultValue = this->getDefaultValue(),
        .min = this->getMinValue(),
        .max = this->getMaxValue(),
        .controls = {
            .arrows = this->isArrowsEnabled(),
            .bigArrows = this->isBigArrowsEnabled(),
            .arrowStep = static_cast<size_t>(this->getArrowStepSize()),
            .bigArrowStep = static_cast<size_t>(this->getBigArrowStepSize()),
            .slider = this->isSliderEnabled(),
            .sliderStep = this->getSliderSnap(),
            .input = this->isInputEnabled(),
        },
    }));
}
std::optional<std::shared_ptr<SettingValue>> FloatSettingV3::convertToLegacyValue() const {
    return this->convertToLegacy()->createDefaultValue();
}

class StringSettingV3::Impl final {
public:
    std::optional<std::string> match;
    std::optional<std::string> filter;
    std::optional<std::vector<std::string>> oneOf;
};

StringSettingV3::StringSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<StringSettingV3>> StringSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<StringSettingV3>(PrivateMarker());

    auto root = checkJson(json, "StringSettingV3");
    ret->parseBaseProperties(key, modID, root);

    root.has("match").into(ret->m_impl->match);
    root.has("filter").into(ret->m_impl->filter);
    root.has("one-of").into(ret->m_impl->oneOf);
    if (ret->m_impl->oneOf && ret->m_impl->oneOf->empty()) {
        return Err("Setting '{}' in mod {} - \"one-of\" may not be empty!", key, modID);
    }

    root.checkUnknownKeys();
    return root.ok(ret);
}

Result<> StringSettingV3::isValid(std::string_view value) const {
    if (m_impl->match) {
        if (!std::regex_match(std::string(value), std::regex(*m_impl->match))) {
            return Err("Value must match regex {}", *m_impl->match);
        }
    }
    else if (m_impl->oneOf) {
        if (!ranges::contains(*m_impl->oneOf, std::string(value))) {
            return Err("Value must be one of {}", fmt::join(*m_impl->oneOf, ", "));
        }
    }
    return Ok();
}

std::optional<std::string> StringSettingV3::getRegexValidator() const {
    return m_impl->match;
}
std::optional<std::string> StringSettingV3::getAllowedCharacters() const {
    return m_impl->filter;
}
std::optional<std::vector<std::string>> StringSettingV3::getEnumOptions() const {
    return m_impl->oneOf;
}

SettingNodeV3* StringSettingV3::createNode(float width) {
    return StringSettingNodeV3::create(
        std::static_pointer_cast<StringSettingV3>(shared_from_this()), width
    );
}

std::optional<Setting> StringSettingV3::convertToLegacy() const {
    auto setting = StringSetting();
    setting.name = this->getName();
    setting.description = this->getDescription();
    setting.defaultValue = this->getDefaultValue();
    setting.controls->filter = this->getAllowedCharacters();
    setting.controls->match = this->getRegexValidator();
    setting.controls->options = this->getEnumOptions();
    return Setting(this->getKey(), this->getModID(), SettingKind(setting));
}
std::optional<std::shared_ptr<SettingValue>> StringSettingV3::convertToLegacyValue() const {
    return this->convertToLegacy()->createDefaultValue();
}

class FileSettingV3::Impl final {
public:
    bool folder = false;
    bool useSaveDialog = false; // this option makes no sense if folder = true
    std::optional<std::vector<utils::file::FilePickOptions::Filter>> filters;
};

FileSettingV3::FileSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<FileSettingV3>> FileSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<FileSettingV3>(PrivateMarker());

    auto root = checkJson(json, "FileSettingV3");
    ret->parseBaseProperties(key, modID, root);

    ret->setDefaultValue(ret->getDefaultValue().make_preferred());

    // Replace known paths like `{gd-save-dir}/`
    try {
        ret->setDefaultValue(fmt::format(
            fmt::runtime(ret->getDefaultValue().string()),
            fmt::arg("gd_dir", dirs::getGameDir()),
            fmt::arg("gd_save_dir", dirs::getSaveDir()),
            fmt::arg("mod_config_dir", dirs::getModConfigDir() / modID),
            fmt::arg("mod_save_dir", dirs::getModsSaveDir() / modID),
            fmt::arg("temp_dir", dirs::getTempDir())
        ));
    }
    catch(fmt::format_error const& e) {
        return Err("Invalid format string for file setting path: {}", e.what());
    }
    ret->setValue(ret->getDefaultValue());

    std::string type;
    root.needs("type").into(type);
    if (type == "folder") {
        ret->m_impl->folder = true;
        // folder-specific stuff if they ever exist
    }
    else if (type == "file" || type == "path") {
        if (type == "path") {
            log::warn(
                "Setting '{}' in mod {}: the \"path\" type has been "
                "deprecated, use \"type\": \"file\" or \"type\": \"folder\" instead",
                key, modID
            );
        }

        // Controls only make sense for files but not for folders
        if (auto controls = root.has("control")) {
            std::string dialogType;
            controls.has("dialog").into(dialogType);
            switch (hash(dialogType)) {
                case hash("save"): ret->m_impl->useSaveDialog = true; break;
                case hash("open"): ret->m_impl->useSaveDialog = false; break;
                case hash(""): break;
                default: return Err("Setting '{}' in mod {}: unknown \"dialog\" type \"{}\"", key, modID, dialogType);
            }
            
            auto filters = std::vector<file::FilePickOptions::Filter>();
            for (auto& item : controls.has("filters").items()) {
                utils::file::FilePickOptions::Filter filter;
                item.has("description").into(filter.description);
                item.has("files").into(filter.files);
                filters.push_back(filter);
            }
            if (!filters.empty()) {
                ret->m_impl->filters.emplace(filters);
            }
        }
    }

    root.checkUnknownKeys();
    return root.ok(ret);
}

Result<> FileSettingV3::isValid(std::filesystem::path const& value) const {
    // This is because people tend to put `"default": "Please pick a good file"` 
    // which is clever and good UX but also a hack so I also need to hack to support that
    if (value == this->getDefaultValue()) {
        return Ok();
    }
    std::error_code ec;
    if (m_impl->folder) {
        if (!std::filesystem::is_directory(value, ec)) {
            return Err("Value must be a folder");
        }
    }
    else {
        if (!std::filesystem::is_regular_file(value, ec)) {
            return Err("Value must be a file");
        }
    }
    return Ok();
}

bool FileSettingV3::isFolder() const {
    return m_impl->folder;
}
bool FileSettingV3::useSaveDialog() const {
    return m_impl->useSaveDialog;
}

std::optional<std::vector<utils::file::FilePickOptions::Filter>> FileSettingV3::getFilters() const {
    return m_impl->filters;
}

SettingNodeV3* FileSettingV3::createNode(float width) {
    return FileSettingNodeV3::create(
        std::static_pointer_cast<FileSettingV3>(shared_from_this()), width
    );
}

std::optional<Setting> FileSettingV3::convertToLegacy() const {
    auto setting = FileSetting();
    setting.name = this->getName();
    setting.description = this->getDescription();
    setting.defaultValue = this->getDefaultValue();
    setting.controls.filters = this->getFilters().value_or(std::vector<utils::file::FilePickOptions::Filter>());
    return Setting(this->getKey(), this->getModID(), SettingKind(setting));
}
std::optional<std::shared_ptr<SettingValue>> FileSettingV3::convertToLegacyValue() const {
    return this->convertToLegacy()->createDefaultValue();
}

class Color3BSettingV3::Impl final {
public:
};

Color3BSettingV3::Color3BSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<Color3BSettingV3>> Color3BSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<Color3BSettingV3>(PrivateMarker());
    auto root = checkJson(json, "Color3BSettingV3");
    ret->parseBaseProperties(key, modID, root);
    root.checkUnknownKeys();
    return root.ok(ret);
}

Result<> Color3BSettingV3::isValid(ccColor3B value) const {
    return Ok();
}

SettingNodeV3* Color3BSettingV3::createNode(float width) {
    return Color3BSettingNodeV3::create(
        std::static_pointer_cast<Color3BSettingV3>(shared_from_this()), width
    );
}

std::optional<Setting> Color3BSettingV3::convertToLegacy() const {
    auto setting = ColorSetting();
    setting.name = this->getName();
    setting.description = this->getDescription();
    setting.defaultValue = this->getDefaultValue();
    return Setting(this->getKey(), this->getModID(), SettingKind(setting));
}
std::optional<std::shared_ptr<SettingValue>> Color3BSettingV3::convertToLegacyValue() const {
    return this->convertToLegacy()->createDefaultValue();
}

class Color4BSettingV3::Impl final {
public:
};

Color4BSettingV3::Color4BSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<Color4BSettingV3>> Color4BSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<Color4BSettingV3>(PrivateMarker());
    auto root = checkJson(json, "Color4BSettingV3");
    ret->parseBaseProperties(key, modID, root);
    root.checkUnknownKeys();
    return root.ok(ret);
}

Result<> Color4BSettingV3::isValid(ccColor4B value) const {
    return Ok();
}

SettingNodeV3* Color4BSettingV3::createNode(float width) {
    return Color4BSettingNodeV3::create(
        std::static_pointer_cast<Color4BSettingV3>(shared_from_this()), width
    );
}

std::optional<Setting> Color4BSettingV3::convertToLegacy() const {
    auto setting = ColorAlphaSetting();
    setting.name = this->getName();
    setting.description = this->getDescription();
    setting.defaultValue = this->getDefaultValue();
    return Setting(this->getKey(), this->getModID(), SettingKind(setting));
}
std::optional<std::shared_ptr<SettingValue>> Color4BSettingV3::convertToLegacyValue() const {
    return this->convertToLegacy()->createDefaultValue();
}
