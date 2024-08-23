#include <Geode/loader/SettingV3.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <regex>
#include "SettingNodeV3.hpp"

using namespace geode::prelude;

class SettingV3::GeodeImpl {
public:
    std::string modID;
    std::string key;
    std::optional<std::string> name;
    std::optional<std::string> description;
    std::optional<std::string> enableIf;
    bool requiresRestart = false;
};

SettingV3::SettingV3() : m_impl(std::make_shared<GeodeImpl>()) {}
SettingV3::~SettingV3() = default;

Result<> SettingV3::parseSharedProperties(std::string const& key, std::string const& modID, matjson::Value const& value, bool onlyNameAndDesc) {
    auto json = checkJson(value, "SettingV3");
    this->parseSharedProperties(key, modID, json, onlyNameAndDesc);
    return json.ok();
}
void SettingV3::parseSharedProperties(std::string const& key, std::string const& modID, JsonExpectedValue& value, bool onlyNameAndDesc) {
    this->init(key, modID);
    value.needs("type");
    value.has("platforms");
    value.has("name").into(m_impl->name);
    value.has("description").into(m_impl->description);
    if (!onlyNameAndDesc) {
        value.has("enable-if").into(m_impl->enableIf);
        value.has("requires-restart").into(m_impl->requiresRestart);
    }
}
void SettingV3::init(std::string const& key, std::string const& modID) {
    m_impl->key = key;
    m_impl->modID = modID;
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
bool SettingV3::requiresRestart() const {
    return m_impl->requiresRestart;
}
Mod* SettingV3::getMod() const {
    return Loader::get()->getInstalledMod(m_impl->modID);
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
    ret->parseSharedProperties(key, modID, root, true);
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
    return true;
}
bool LegacyCustomSettingV3::save(matjson::Value& json) const {
    return true;
}
SettingNodeV3* LegacyCustomSettingV3::createNode(float width) {
    if (m_impl->legacyValue) {
        return LegacyCustomSettingToV3Node::create(
            std::static_pointer_cast<LegacyCustomSettingV3>(shared_from_this()), width
        );
    }
    return UnresolvedCustomSettingNodeV3::create(
        std::static_pointer_cast<LegacyCustomSettingV3>(shared_from_this()), width
    );
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
    bool value;
    bool defaultValue;
};

BoolSettingV3::BoolSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<BoolSettingV3>> BoolSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<BoolSettingV3>(PrivateMarker());

    auto root = checkJson(json, "BoolSettingV3");
    ret->parseSharedProperties(key, modID, root);
    ret->parseDefaultValue(root, ret->m_impl->defaultValue);
    ret->m_impl->value = ret->m_impl->defaultValue;

    root.checkUnknownKeys();
    return root.ok(ret);
}

bool& BoolSettingV3::getValueMut() const {
    return m_impl->value;
}
bool BoolSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}
Result<> BoolSettingV3::isValid(bool value) const {
    return Ok();
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
    int64_t value;
    int64_t defaultValue;
    std::optional<int64_t> minValue;
    std::optional<int64_t> maxValue;

    struct {
        // 0 means not enabled
        size_t arrowStepSize = 1;
        size_t bigArrowStepSize = 5;
        bool sliderEnabled = true;
        std::optional<int64_t> sliderSnap;
        bool textInputEnabled = true;
    } controls;
};

Result<std::shared_ptr<IntSettingV3>> IntSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<IntSettingV3>(PrivateMarker());
    
    auto root = checkJson(json, "IntSettingV3");
    ret->parseSharedProperties(key, modID, root);
    ret->parseDefaultValue(root, ret->m_impl->defaultValue);
    ret->m_impl->value = ret->m_impl->defaultValue;

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

int64_t& IntSettingV3::getValueMut() const {
    return m_impl->value;
}
int64_t IntSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}
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
std::optional<int64_t> IntSettingV3::getSliderSnap() const {
    return m_impl->controls.sliderSnap;
}
bool IntSettingV3::isInputEnabled() const {
    return m_impl->controls.textInputEnabled;
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
    double value;
    double defaultValue;
    std::optional<double> minValue;
    std::optional<double> maxValue;

    struct {
        // 0 means not enabled
        double arrowStepSize = 1;
        double bigArrowStepSize = 5;
        bool sliderEnabled = true;
        std::optional<double> sliderSnap;
        bool textInputEnabled = true;
    } controls;
};

FloatSettingV3::FloatSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<FloatSettingV3>> FloatSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<FloatSettingV3>(PrivateMarker());

    auto root = checkJson(json, "FloatSettingV3");
    ret->parseSharedProperties(key, modID, root);
    ret->parseDefaultValue(root, ret->m_impl->defaultValue);
    ret->m_impl->value = ret->m_impl->defaultValue;

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

double& FloatSettingV3::getValueMut() const {
    return m_impl->value;
}
double FloatSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
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
std::optional<double> FloatSettingV3::getSliderSnap() const {
    return m_impl->controls.sliderSnap;
}
bool FloatSettingV3::isInputEnabled() const {
    return m_impl->controls.textInputEnabled;
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
    std::string value;
    std::string defaultValue;
    std::optional<std::string> match;
    std::optional<std::string> filter;
    std::optional<std::vector<std::string>> oneOf;
};

StringSettingV3::StringSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<StringSettingV3>> StringSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<StringSettingV3>(PrivateMarker());

    auto root = checkJson(json, "StringSettingV3");
    ret->parseSharedProperties(key, modID, root);
    ret->parseDefaultValue(root, ret->m_impl->defaultValue);
    ret->m_impl->value = ret->m_impl->defaultValue;

    root.has("match").into(ret->m_impl->match);
    root.has("filter").into(ret->m_impl->filter);
    root.has("one-of").into(ret->m_impl->oneOf);
    if (ret->m_impl->oneOf && ret->m_impl->oneOf->empty()) {
        return Err("Setting '{}' in mod {} - \"one-of\" may not be empty!", key, modID);
    }

    root.checkUnknownKeys();
    return root.ok(ret);
}

std::string& StringSettingV3::getValueMut() const {
    return m_impl->value;
}
std::string StringSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
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
    std::filesystem::path value;
    std::filesystem::path defaultValue;
    FileType fileType;
    std::optional<std::vector<utils::file::FilePickOptions::Filter>> filters;
};

FileSettingV3::FileSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<FileSettingV3>> FileSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<FileSettingV3>(PrivateMarker());

    auto root = checkJson(json, "FileSettingV3");
    ret->parseSharedProperties(key, modID, root);
    ret->parseDefaultValue(root, ret->m_impl->defaultValue);
    ret->m_impl->value = ret->m_impl->defaultValue;

    // Replace known paths like `{gd-save-dir}/`    
    try {
        ret->m_impl->defaultValue = fmt::format(
            fmt::runtime(ret->m_impl->defaultValue.string()),
            fmt::arg("gd_dir", dirs::getGameDir()),
            fmt::arg("gd_save_dir", dirs::getSaveDir()),
            fmt::arg("mod_config_dir", dirs::getModConfigDir() / modID),
            fmt::arg("mod_save_dir", dirs::getModsSaveDir() / modID),
            fmt::arg("temp_dir", dirs::getTempDir())
        );
    }
    catch(fmt::format_error const& e) {
        return Err("Invalid format string for file setting path: {}", e.what());
    }
    ret->m_impl->value = ret->m_impl->defaultValue;

    if (auto ty = root.has("filetype")) {
        ty.assertIsString();
        switch (hash(ty.template get<std::string>())) {
            case hash("any"): ret->m_impl->fileType = FileType::Any; break;
            case hash("file"): ret->m_impl->fileType = FileType::File; break;
            case hash("folder"): ret->m_impl->fileType = FileType::Folder; break;
            default: return Err(
                "Setting '{}' in mod {}: Invalid filetype \"{}\"",
                key, modID, ty.template get<std::string>()
            );
        }
    }

    if (auto controls = root.has("control")) {
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

    root.checkUnknownKeys();
    return root.ok(ret);
}

std::filesystem::path& FileSettingV3::getValueMut() const {
    return m_impl->value;
}
std::filesystem::path FileSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}
Result<> FileSettingV3::isValid(std::filesystem::path const& value) const {
    if (m_impl->fileType != FileType::Any) {
        if (!std::filesystem::exists(value)) {
            return Err("{} must exist", m_impl->fileType == FileType::File ? "File" : "Folder");
        }
        if (m_impl->fileType == FileType::File && !std::filesystem::is_regular_file(value)) {
            return Err("Value must be a file");
        }
        if (m_impl->fileType == FileType::Folder && !std::filesystem::is_directory(value)) {
            return Err("Value must be a folder");
        }
    }
    return Ok();
}

FileSettingV3::FileType FileSettingV3::getFileType() const {
    return m_impl->fileType;
}

std::optional<std::vector<utils::file::FilePickOptions::Filter>> FileSettingV3::getFilters() const {
    return m_impl->filters;
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
    ccColor3B value;
    ccColor3B defaultValue;
};

Color3BSettingV3::Color3BSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<Color3BSettingV3>> Color3BSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<Color3BSettingV3>(PrivateMarker());

    auto root = checkJson(json, "Color3BSettingV3");
    ret->parseSharedProperties(key, modID, root);
    ret->parseDefaultValue(root, ret->m_impl->defaultValue);
    ret->m_impl->value = ret->m_impl->defaultValue;

    root.checkUnknownKeys();
    return root.ok(ret);
}

ccColor3B& Color3BSettingV3::getValueMut() const {
    return m_impl->value;
}
ccColor3B Color3BSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}
Result<> Color3BSettingV3::isValid(ccColor3B value) const {
    return Ok();
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
    ccColor4B value;
    ccColor4B defaultValue;
};

Color4BSettingV3::Color4BSettingV3(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<Color4BSettingV3>> Color4BSettingV3::parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
    auto ret = std::make_shared<Color4BSettingV3>(PrivateMarker());

    auto root = checkJson(json, "Color4BSettingV3");
    ret->parseSharedProperties(key, modID, root);
    ret->parseDefaultValue(root, ret->m_impl->defaultValue);
    ret->m_impl->value = ret->m_impl->defaultValue;

    root.checkUnknownKeys();
    return root.ok(ret);
}

ccColor4B& Color4BSettingV3::getValueMut() const {
    return m_impl->value;
}
ccColor4B Color4BSettingV3::getDefaultValue() const {
    return m_impl->defaultValue;
}
Result<> Color4BSettingV3::isValid(ccColor4B value) const {
    return Ok();
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
