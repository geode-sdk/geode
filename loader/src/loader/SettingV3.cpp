#include <Geode/loader/SettingV3.hpp>
#include <Geode/utils/JsonValidation.hpp>

using namespace geode::prelude;

class SettingV3::GeodeImpl {
public:
    std::string modID;
    std::string key;
};

SettingV3::~SettingV3() = default;

SettingV3::SettingV3(std::string const& key, std::string const& modID)
  : m_impl(std::make_shared<GeodeImpl>())
{
    m_impl->key = key;
    m_impl->modID = modID;
}

std::string SettingV3::getKey() const {
    return m_impl->key;
}
std::string SettingV3::getModID() const {
    return m_impl->modID;
}
Mod* SettingV3::getMod() const {
    return Loader::get()->getInstalledMod(m_impl->modID);
}

Result<std::shared_ptr<SettingV3>> SettingV3::parseBuiltin(std::string const& modID, matjson::Value const& json) {
    auto root = checkJson(json, "SettingV3");
    std::string type;
    root.needs("type").into(type);
    std::shared_ptr<SettingV3> ret;
    switch (hash(type)) {
        case hash("bool"): ret = std::make_shared<BoolSettingV3>(); break;
        case hash("int"): ret = std::make_shared<IntSettingV3>(); break;
        case hash("float"): ret = std::make_shared<FloatSettingV3>(); break;
        case hash("string"): ret = std::make_shared<StringSettingV3>(); break;
        case hash("rgb"): case hash("color"): ret = std::make_shared<Color3BSettingV3>(); break;
        case hash("rgba"): ret = std::make_shared<Color4BSettingV3>(); break;
        case hash("path"): case hash("file"): ret = std::make_shared<FileSettingV3>(); break;
        case hash("custom"): ret = std::make_shared<UnresolvedCustomSettingV3>(); break;
        case hash("title"): ret = std::make_shared<TitleSettingV3>(); break;
    }
    GEODE_UNWRAP(ret->parse(modID, json));
    return root.ok(ret);
}

std::optional<Setting> SettingV3::convertToLegacy() const {
    return std::nullopt;
}
std::optional<std::unique_ptr<SettingValue>> SettingV3::convertToLegacyValue() const {
    return std::nullopt;
}

class geode::detail::GeodeSettingBaseV3::Impl final {
public:
    std::string name;
    std::optional<std::string> description;
    std::optional<std::string> enableIf;
};

std::string geode::detail::GeodeSettingBaseV3::getName() const {
    return m_impl->name;
}
std::optional<std::string> geode::detail::GeodeSettingBaseV3::getDescription() const {
    return m_impl->description;
}
std::optional<std::string> geode::detail::GeodeSettingBaseV3::getEnableIf() const {
    return m_impl->enableIf;
}

Result<> geode::detail::GeodeSettingBaseV3::parseShared(JsonExpectedValue& json) {
    json.needs("name").into(m_impl->name);
    json.needs("description").into(m_impl->description);
    json.needs("enable-if").into(m_impl->enableIf);
    return Ok();
}
Result<> geode::detail::GeodeSettingBaseV3::isValidShared() const {
    // In the future if something like `enable-if` preventing 
    // programmatic modification of settings it should be added here
    return Ok();
}

class TitleSettingV3::Impl final {
public:
    std::string title;
};

std::string TitleSettingV3::getTitle() const {
    return m_impl->title;
}

Result<> TitleSettingV3::parse(std::string const& modID, matjson::Value const& json) {
    auto root = checkJson(json, "TitleSettingV3");
    root.needs("title").into(m_impl->title);
    root.checkUnknownKeys();
    return root.ok();
}
bool TitleSettingV3::load(matjson::Value const& json) {
    return true;
}
bool TitleSettingV3::save(matjson::Value&) const {
    return true;
}
SettingNodeV3* TitleSettingV3::createNode(float width) {
    // todo
}
bool TitleSettingV3::isDefaultValue() const {
    return true;
}
void TitleSettingV3::reset() {}

class UnresolvedCustomSettingV3::Impl final {
public:
    matjson::Value json;
};

Result<> UnresolvedCustomSettingV3::parse(std::string const& modID, matjson::Value const& json) {
    m_impl->json = json;
    return Ok();
}
bool UnresolvedCustomSettingV3::load(matjson::Value const& json) {
    return true;
}
bool UnresolvedCustomSettingV3::save(matjson::Value& json) const {
    return true;
}
SettingNodeV3* UnresolvedCustomSettingV3::createNode(float width) {
    // todo
}

bool UnresolvedCustomSettingV3::isDefaultValue() const {
    return true;
}
void UnresolvedCustomSettingV3::reset() {}

std::optional<Setting> UnresolvedCustomSettingV3::convertToLegacy() const {
    return Setting(this->getKey(), this->getModID(), SettingKind(CustomSetting {
        .json = std::make_shared<ModJson>(m_impl->json)
    }));
}
std::optional<std::unique_ptr<SettingValue>> UnresolvedCustomSettingV3::convertToLegacyValue() const {
    return std::nullopt;
}

class BoolSettingV3::Impl final {
public:
    bool value;
    bool defaultValue;
};

bool BoolSettingV3::getValue() const {
    return m_impl->value;
}
void BoolSettingV3::setValue(bool value) {
    m_impl->value = value;
}
Result<> BoolSettingV3::isValid(bool value) const {
    GEODE_UNWRAP(this->isValidShared());
    return Ok();
}

bool BoolSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}

Result<> BoolSettingV3::parse(std::string const& modID, matjson::Value const& json) {
    auto root = checkJson(json, "BoolSettingV3");

    GEODE_UNWRAP(this->parseShared(root));
    root.needs("default").into(m_impl->defaultValue);
    m_impl->value = m_impl->defaultValue;

    root.checkUnknownKeys();
    return root.ok();
}
bool BoolSettingV3::load(matjson::Value const& json) {
    if (json.is_bool()) {
        m_impl->value = json.as_bool();
        return true;
    }
    return false;
}
bool BoolSettingV3::save(matjson::Value& json) const {
    json = m_impl->value;
    return true;
}
SettingNodeV3* BoolSettingV3::createNode(float width) {
    // todo
}
bool BoolSettingV3::isDefaultValue() const {
    return m_impl->value == m_impl->defaultValue;
}
void BoolSettingV3::reset() {
    m_impl->value = m_impl->defaultValue;
}

std::optional<Setting> BoolSettingV3::convertToLegacy() const {
    return Setting(this->getKey(), this->getModID(), SettingKind(BoolSetting {
        .name = this->getName(),
        .description = this->getDescription(),
        .defaultValue = this->getDefaultValue(),
    }));
}
std::optional<std::unique_ptr<SettingValue>> BoolSettingV3::convertToLegacyValue() const {
    return std::make_unique<BoolSettingValue>(this->getKey(), this->getModID(), *this->convertToLegacy());
}

class IntSettingV3::Impl final {
public:
    int64_t value;
    int64_t defaultValue;
    std::optional<int64_t> minValue;
    std::optional<int64_t> maxValue;

    struct {
        // 0 means not enabled
        size_t arrowStepSize;
        size_t bigArrowStepSize;
        bool sliderEnabled;
        std::optional<int64_t> sliderSnap;
        bool textInputEnabled;
    } controls;
};

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
std::optional<int64_t> IntSettingV3::getSliderSnap() const {
    return m_impl->controls.sliderSnap;
}
bool IntSettingV3::isInputEnabled() const {
    return m_impl->controls.textInputEnabled;
}

int64_t IntSettingV3::getValue() const {
    return m_impl->value;
}
void IntSettingV3::setValue(int64_t value) {
    m_impl->value = clamp(
        value,
        m_impl->minValue.value_or(std::numeric_limits<int64_t>::min()),
        m_impl->maxValue.value_or(std::numeric_limits<int64_t>::max())
    );
}
int64_t IntSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}
std::optional<int64_t> IntSettingV3::getMinValue() const {
    return m_impl->minValue;
}
std::optional<int64_t> IntSettingV3::getMaxValue() const {
    return m_impl->maxValue;
}

Result<> IntSettingV3::parse(std::string const& modID, matjson::Value const& json) {
    auto root = checkJson(json, "IntSettingV3");

    GEODE_UNWRAP(this->parseShared(root));
    root.needs("default").into(m_impl->defaultValue);
    m_impl->value = m_impl->defaultValue;
    
    root.has("min").into(m_impl->minValue);
    root.has("max").into(m_impl->maxValue);
    if (auto controls = root.has("control")) {
        controls.has("arrow-step").into(m_impl->controls.arrowStepSize);
        if (!controls.has("arrows").template get<bool>()) {
            m_impl->controls.arrowStepSize = 0;
        }
        controls.has("big-arrow-step").into(m_impl->controls.bigArrowStepSize);
        if (!controls.has("big-arrows").template get<bool>()) {
            m_impl->controls.bigArrowStepSize = 0;
        }
        controls.has("slider").into(m_impl->controls.sliderEnabled);
        controls.has("slider-step").into(m_impl->controls.sliderSnap);
        controls.has("input").into(m_impl->controls.textInputEnabled);
        controls.checkUnknownKeys();
    }

    root.checkUnknownKeys();
    return root.ok();
}
bool IntSettingV3::load(matjson::Value const& json) {
    if (json.is_number()) {
        m_impl->value = json.as_int();
        return true;
    }
    return false;
}
bool IntSettingV3::save(matjson::Value& json) const {
    json = m_impl->value;
    return true;
}
SettingNodeV3* IntSettingV3::createNode(float width) {
    // todo
}

bool IntSettingV3::isDefaultValue() const {
    return m_impl->value == m_impl->defaultValue;
}
void IntSettingV3::reset() {
    m_impl->value = m_impl->defaultValue;
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
std::optional<std::unique_ptr<SettingValue>> IntSettingV3::convertToLegacyValue() const {
    return std::make_unique<IntSettingValue>(this->getKey(), this->getModID(), *this->convertToLegacy());
}

class FloatSettingV3::Impl final {
public:
    double value;
    double defaultValue;
    std::optional<double> minValue;
    std::optional<double> maxValue;

    struct {
        // 0 means not enabled
        size_t arrowStepSize;
        size_t bigArrowStepSize;
        bool sliderEnabled;
        std::optional<double> sliderSnap;
        bool textInputEnabled;
    } controls;
};

bool FloatSettingV3::isArrowsEnabled() const {
    return m_impl->controls.arrowStepSize > 0;
}
bool FloatSettingV3::isBigArrowsEnabled() const {
    return m_impl->controls.bigArrowStepSize > 0;
}
size_t FloatSettingV3::getArrowStepSize() const {
    return m_impl->controls.arrowStepSize;
}
size_t FloatSettingV3::getBigArrowStepSize() const {
    return m_impl->controls.bigArrowStepSize;
}
bool FloatSettingV3::isSliderEnabled() const {
    return m_impl->controls.sliderEnabled;
}
std::optional<double> FloatSettingV3::getSliderSnap() const {
    return m_impl->controls.sliderSnap;
}
bool FloatSettingV3::isInputEnabled() const {
    return m_impl->controls.textInputEnabled;
}

double FloatSettingV3::getValue() const {
    return m_impl->value;
}
Result<> FloatSettingV3::setValue(double value) {
    if (m_impl->minValue && value < *m_impl->minValue) {
        return Err("Value must be under ");
    }
    m_impl->value = clamp(
        value,
        m_impl->minValue.value_or(std::numeric_limits<double>::min()),
        m_impl->maxValue.value_or(std::numeric_limits<double>::max())
    );
}
double FloatSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}
std::optional<double> FloatSettingV3::getMinValue() const {
    return m_impl->minValue;
}
std::optional<double> FloatSettingV3::getMaxValue() const {
    return m_impl->maxValue;
}

Result<> FloatSettingV3::parse(std::string const& modID, matjson::Value const& json) {
    auto root = checkJson(json, "FloatSettingV3");

    GEODE_UNWRAP(this->parseShared(root));
    root.needs("default").into(m_impl->defaultValue);
    m_impl->value = m_impl->defaultValue;
    
    root.has("min").into(m_impl->minValue);
    root.has("max").into(m_impl->maxValue);
    if (auto controls = root.has("control")) {
        controls.has("arrow-step").into(m_impl->controls.arrowStepSize);
        if (!controls.has("arrows").template get<bool>()) {
            m_impl->controls.arrowStepSize = 0;
        }
        controls.has("big-arrow-step").into(m_impl->controls.bigArrowStepSize);
        if (!controls.has("big-arrows").template get<bool>()) {
            m_impl->controls.bigArrowStepSize = 0;
        }
        controls.has("slider").into(m_impl->controls.sliderEnabled);
        controls.has("slider-step").into(m_impl->controls.sliderSnap);
        controls.has("input").into(m_impl->controls.textInputEnabled);
        controls.checkUnknownKeys();
    }

    root.checkUnknownKeys();
    return root.ok();
}
bool FloatSettingV3::load(matjson::Value const& json) {
    if (json.is_number()) {
        m_impl->value = json.as_double();
        return true;
    }
    return false;
}
bool FloatSettingV3::save(matjson::Value& json) const {
    json = m_impl->value;
    return true;
}
SettingNodeV3* FloatSettingV3::createNode(float width) {
    // todo
}

bool FloatSettingV3::isDefaultValue() const {
    return m_impl->value == m_impl->defaultValue;
}
void FloatSettingV3::reset() {
    m_impl->value = m_impl->defaultValue;
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
            .arrowStep = this->getArrowStepSize(),
            .bigArrowStep = this->getBigArrowStepSize(),
            .slider = this->isSliderEnabled(),
            .sliderStep = this->getSliderSnap(),
            .input = this->isInputEnabled(),
        },
    }));
}
std::optional<std::unique_ptr<SettingValue>> FloatSettingV3::convertToLegacyValue() const {
    return std::make_unique<FloatSettingValue>(this->getKey(), this->getModID(), *this->convertToLegacy());
}

class StringSettingV3::Impl final {
public:
    std::string value;
    std::string defaultValue;
    std::optional<std::string> match;
    std::optional<std::string> filter;
    std::optional<std::vector<std::string>> oneOf;
};

std::string StringSettingV3::getValue() const {
    return m_impl->value;
}
Result<> StringSettingV3::setValue(std::string_view value) {
    m_impl->value = value;
}
std::string StringSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
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

Result<> StringSettingV3::parse(std::string const& modID, matjson::Value const& json) {
    auto root = checkJson(json, "StringSettingV3");

    GEODE_UNWRAP(this->parseShared(root));
    root.needs("default").into(m_impl->defaultValue);
    m_impl->value = m_impl->defaultValue;

    root.has("match").into(m_impl->match);
    root.has("filter").into(m_impl->filter);
    root.has("one-of").into(m_impl->oneOf);

    root.checkUnknownKeys();
    return root.ok();
}
bool StringSettingV3::load(matjson::Value const& json) {
    if (json.is_string()) {
        m_impl->value = json.as_string();
        return true;
    }
    return false;
}
bool StringSettingV3::save(matjson::Value& json) const {
    json = m_impl->value;
    return true;
}
SettingNodeV3* StringSettingV3::createNode(float width) {
    // todo
}

bool StringSettingV3::isDefaultValue() const {
    return m_impl->value == m_impl->defaultValue;
}
void StringSettingV3::reset() {
    m_impl->value = m_impl->defaultValue;
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
std::optional<std::unique_ptr<SettingValue>> StringSettingV3::convertToLegacyValue() const {
    return std::make_unique<StringSettingValue>(this->getKey(), this->getModID(), *this->convertToLegacy());
}

class FileSettingV3::Impl final {
public:
    std::filesystem::path value;
    std::filesystem::path defaultValue;
    std::optional<std::vector<utils::file::FilePickOptions::Filter>> filters;
};

std::filesystem::path FileSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}
std::filesystem::path FileSettingV3::getValue() const {
    return m_impl->value;
}
std::optional<std::vector<utils::file::FilePickOptions::Filter>> FileSettingV3::getFilters() const {
    return m_impl->filters;
}

Result<> FileSettingV3::parse(std::string const& modID, matjson::Value const& json) {
    auto root = checkJson(json, "FileSettingV3");

    GEODE_UNWRAP(this->parseShared(root));

    root.needs("default").into(m_impl->defaultValue);

    // Replace known paths like `{gd-save-dir}/`    
    try {
        m_impl->defaultValue = fmt::format(
            fmt::runtime(m_impl->defaultValue.string()), 
            fmt::arg("gd-save-dir", dirs::getSaveDir()),
            fmt::arg("gd-game-dir", dirs::getGameDir()),
            fmt::arg("mod-config-dir", dirs::getModConfigDir() / modID),
            fmt::arg("mod-save-dir", dirs::getModsSaveDir() / modID),
            fmt::arg("temp-dir", dirs::getTempDir())
        );
    }
    catch(fmt::format_error const&) {
        return Err("Invalid format string for file setting path");
    }
    m_impl->value = m_impl->defaultValue;

    if (auto controls = root.has("control")) {
        auto filters = std::vector<file::FilePickOptions::Filter>();
        for (auto& item : controls.has("filters").items()) {
            utils::file::FilePickOptions::Filter filter;
            item.has("description").into(filter.description);
            item.has("files").into(filter.files);
            filters.push_back(filter);
        }
        if (!filters.empty()) {
            m_impl->filters.emplace(filters);
        }
    }

    root.checkUnknownKeys();
    return root.ok();
}
bool FileSettingV3::load(matjson::Value const& json) {
    if (json.is_string()) {
        m_impl->value = json.as_string();
        return true;
    }
    return false;
}
bool FileSettingV3::save(matjson::Value& json) const {
    json = m_impl->value;
    return true;
}
SettingNodeV3* createNode(float width) {
    // todo
}

bool FileSettingV3::isDefaultValue() const {
    return m_impl->value == m_impl->defaultValue;
}
void FileSettingV3::reset() {
    m_impl->value = m_impl->defaultValue;
}

std::optional<Setting> FileSettingV3::convertToLegacy() const {
    auto setting = FileSetting();
    setting.name = this->getName();
    setting.description = this->getDescription();
    setting.defaultValue = this->getDefaultValue();
    setting.controls.filters = this->getFilters().value_or(std::vector<utils::file::FilePickOptions::Filter>());
    return Setting(this->getKey(), this->getModID(), SettingKind(setting));
}
std::optional<std::unique_ptr<SettingValue>> FileSettingV3::convertToLegacyValue() const {
    return std::make_unique<FileSettingValue>(this->getKey(), this->getModID(), *this->convertToLegacy());
}

class Color3BSettingV3::Impl final {
public:
    ccColor3B value;
    ccColor3B defaultValue;
};

ccColor3B Color3BSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}
ccColor3B Color3BSettingV3::getValue() const {
    return m_impl->value;
}

Result<> Color3BSettingV3::parse(std::string const& modID, matjson::Value const& json) {
    auto root = checkJson(json, "Color3BSettingV3");

    GEODE_UNWRAP(this->parseShared(root));
    root.needs("default").into(m_impl->defaultValue);
    m_impl->value = m_impl->defaultValue;

    root.checkUnknownKeys();
    return root.ok();
}
bool Color3BSettingV3::load(matjson::Value const& json) {
    if (json.template is<ccColor3B>()) {
        m_impl->value = json.template as<ccColor3B>();
        return true;
    }
    return false;
}
bool Color3BSettingV3::save(matjson::Value& json) const {
    json = m_impl->value;
    return true;
}
SettingNodeV3* Color3BSettingV3::createNode(float width) {
    // todo
}

bool Color3BSettingV3::isDefaultValue() const {
    return m_impl->value == m_impl->defaultValue;

}
void Color3BSettingV3::reset() {
    m_impl->value = m_impl->defaultValue;
}

std::optional<Setting> Color3BSettingV3::convertToLegacy() const {
    auto setting = ColorSetting();
    setting.name = this->getName();
    setting.description = this->getDescription();
    setting.defaultValue = this->getDefaultValue();
    return Setting(this->getKey(), this->getModID(), SettingKind(setting));
}
std::optional<std::unique_ptr<SettingValue>> Color3BSettingV3::convertToLegacyValue() const {
    return std::make_unique<ColorSettingValue>(this->getKey(), this->getModID(), *this->convertToLegacy());
}

class Color4BSettingV3::Impl final {
public:
    ccColor4B value;
    ccColor4B defaultValue;
};

ccColor4B Color4BSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}
ccColor4B Color4BSettingV3::getValue() const {
    return m_impl->value;
}

Result<> Color4BSettingV3::parse(std::string const& modID, matjson::Value const& json) {
    auto root = checkJson(json, "Color4BSettingV3");

    GEODE_UNWRAP(this->parseShared(root));
    root.needs("default").into(m_impl->defaultValue);
    m_impl->value = m_impl->defaultValue;

    root.checkUnknownKeys();
    return root.ok();
}
bool Color4BSettingV3::load(matjson::Value const& json) {
    if (json.template is<ccColor4B>()) {
        m_impl->value = json.template as<ccColor4B>();
        return true;
    }
    return false;
}
bool Color4BSettingV3::save(matjson::Value& json) const {
    json = m_impl->value;
    return true;
}
SettingNodeV3* Color4BSettingV3::createNode(float width) {
    // todo
}

bool Color4BSettingV3::isDefaultValue() const {
    return m_impl->value == m_impl->defaultValue;

}
void Color4BSettingV3::reset() {
    m_impl->value = m_impl->defaultValue;
}

std::optional<Setting> Color4BSettingV3::convertToLegacy() const {
    auto setting = ColorAlphaSetting();
    setting.name = this->getName();
    setting.description = this->getDescription();
    setting.defaultValue = this->getDefaultValue();
    return Setting(this->getKey(), this->getModID(), SettingKind(setting));
}
std::optional<std::unique_ptr<SettingValue>> Color4BSettingV3::convertToLegacyValue() const {
    return std::make_unique<ColorAlphaSettingValue>(this->getKey(), this->getModID(), *this->convertToLegacy());
}
