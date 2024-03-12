#include "ModImpl.hpp"
#include "LoaderImpl.hpp"
#include "ModMetadataImpl.hpp"
#include "HookImpl.hpp"
#include "PatchImpl.hpp"
#include "about.hpp"
#include "console.hpp"

#include <hash/hash.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <optional>
#include <string>
#include <vector>
#include <unordered_set>

using namespace geode::prelude;

Mod::Impl* ModImpl::get() {
    return Mod::get()->m_impl.get();
}

Mod::Impl* ModImpl::getImpl(Mod* mod)  {
    return mod->m_impl.get();
}

Mod::Impl::Impl(Mod* self, ModMetadata const& metadata) : m_self(self), m_metadata(metadata), m_enabled(false), m_isCurrentlyLoading(false), m_loggingEnabled(false) {}

Mod::Impl::~Impl() = default;

Result<> Mod::Impl::setup() {
    m_saveDirPath = dirs::getModsSaveDir() / m_metadata.getID();
    (void) utils::file::createDirectoryAll(m_saveDirPath);

    GEODE_UNWRAP(this->createTempDir().expect("Unable to create temp dir: {error}"));

    this->setupSettings();
    auto loadRes = this->loadData();
    if (!loadRes) {
        log::warn("Unable to load data for \"{}\": {}", m_metadata.getID(), loadRes.unwrapErr());
    }
    if (!m_resourcesLoaded) {
        auto searchPathRoot = dirs::getModRuntimeDir() / m_metadata.getID() / "resources";
        const auto binariesDir = searchPathRoot / m_metadata.getID() / "binaries" / PlatformID::toShortString(GEODE_PLATFORM_TARGET);
        if (ghc::filesystem::exists(binariesDir))
            LoaderImpl::get()->addNativeBinariesPath(binariesDir);

        m_resourcesLoaded = true;
    }

    return Ok();
}

ghc::filesystem::path Mod::Impl::getSaveDir() const {
    return m_saveDirPath;
}

std::string Mod::Impl::getID() const {
    return m_metadata.getID();
}

std::string Mod::Impl::getName() const {
    return m_metadata.getName();
}

std::vector<std::string> Mod::Impl::getDevelopers() const {
    return m_metadata.getDevelopers();
}

std::optional<std::string> Mod::Impl::getDescription() const {
    return m_metadata.getDescription();
}

std::optional<std::string> Mod::Impl::getDetails() const {
    return m_metadata.getDetails();
}

ModMetadata Mod::Impl::getMetadata() const {
    return m_metadata;
}

ghc::filesystem::path Mod::Impl::getTempDir() const {
    return m_tempDirName;
}

ghc::filesystem::path Mod::Impl::getBinaryPath() const {
    return m_tempDirName / m_metadata.getBinaryName();
}

ghc::filesystem::path Mod::Impl::getPackagePath() const {
    return m_metadata.getPath();
}

VersionInfo Mod::Impl::getVersion() const {
    return m_metadata.getVersion();
}

matjson::Value& Mod::Impl::getSaveContainer() {
    return m_saved;
}

bool Mod::Impl::isEnabled() const {
    return m_enabled;
}

bool Mod::Impl::isInternal() const {
    return m_metadata.getID() == "geode.loader";
}

bool Mod::Impl::needsEarlyLoad() const {
    auto deps = m_dependants;
    return getMetadata().needsEarlyLoad() ||
        !deps.empty() && std::any_of(deps.begin(), deps.end(), [&](auto& item) {
             return item->needsEarlyLoad();
         });
}

std::vector<Hook*> Mod::Impl::getHooks() const {
    std::vector<Hook*> ret;
    for (auto& hook : m_hooks) {
        ret.push_back(hook.get());
    }
    return ret;
}

std::vector<Patch*> Mod::Impl::getPatches() const {
    std::vector<Patch*> ret;
    for (auto& patch : m_patches) {
        ret.push_back(patch.get());
    }
    return ret;
}

Result<> Mod::Impl::loadData() {
    Loader::get()->queueInMainThread([&]() {
        ModStateEvent(m_self, ModEventType::DataLoaded).post();
    });

    auto settingPath = m_saveDirPath / "settings.json";
    if (ghc::filesystem::exists(settingPath)) {
        GEODE_UNWRAP_INTO(auto settingData, utils::file::readString(settingPath));
        std::string error;
        auto res = matjson::parse(settingData, error);
        if (error.size() > 0) {
            return Err("Unable to parse settings.json: " + error);
        }
        auto json = res.value();

        JsonChecker checker(json);
        auto root = checker.root(fmt::format("[{}/settings.json]", this->getID()));

        m_savedSettingsData = json;

        for (auto& [key, value] : root.items()) {
            if (auto setting = this->getSetting(key)) {
                if (!setting->load(value.json())) {
                    log::logImpl(
                        Severity::Error,
                        m_self,
                        "{}: Unable to load value for setting \"{}\"",
                        m_metadata.getID(),
                        key
                    );
                }
            }
            else {
                log::logImpl(
                    Severity::Warning,
                    m_self,
                    "Encountered unknown setting \"{}\" while loading settings",
                    key
                );
            }
        }
    }

    auto savedPath = m_saveDirPath / "saved.json";
    if (ghc::filesystem::exists(savedPath)) {
        GEODE_UNWRAP_INTO(auto data, utils::file::readString(savedPath));
        std::string error;
        auto res = matjson::parse(data, error);
        if (error.size() > 0) {
            return Err("Unable to parse saved values: " + error);
        }
        m_saved = res.value();
        if (!m_saved.is_object()) {
            log::warn("saved.json was somehow not an object, forcing it to one.");
            m_saved = matjson::Object();
        }
    }

    return Ok();
}

Result<> Mod::Impl::saveData() {
    ModStateEvent(m_self, ModEventType::DataSaved).post();

    std::unordered_set<std::string> coveredSettings;

    matjson::Value json = matjson::Object();
    for (auto& [key, value] : m_settings) {
        coveredSettings.insert(key);
        if (!value->save(json[key])) {
            log::error("Unable to save setting {}", key);
        }
    }

    if (!m_savedSettingsData.is_object()) {
        log::error("m_savedSettingsData was somehow not an object.");
        m_savedSettingsData = matjson::Object();
    }

    for (auto& [key, value] : m_savedSettingsData.items()) {
        if (coveredSettings.find(key) == coveredSettings.end()) {
            log::warn("Removed unused setting key {}", key);
            continue;
        }
    }

    auto settingPath = m_saveDirPath / "settings.json";
    GEODE_UNWRAP(utils::file::writeString(settingPath, json.dump(4)));

    auto savedPath = m_saveDirPath / "saved.json";
    GEODE_UNWRAP(utils::file::writeString(savedPath, m_saved.dump(4)));

    return Ok();
}

Result<> Mod::Impl::createTempDir() {
    auto tempDir = ghc::filesystem::temp_directory_path();
    if (!tempDir.has_filename() || tempDir.filename() == "") {
        tempDir = ghc::filesystem::temp_directory_path() / ("geode" + hash::sha256(about::get().modDataFolder));
    }
    tempDir = tempDir / m_metadata.getID();
    m_tempDirName = tempDir;
    return utils::file::createDirectoryAll(m_tempDirName);
}

std::optional<Setting*> Mod::Impl::getSetting(std::string_view name) {
    if (auto iter = m_settings.find(name); iter != m_settings.end()) {
        return iter->second.get();
    }
    return std::nullopt;
}

void Mod::Impl::addSetting(std::string_view name, Setting* setting) {
    m_settings.emplace(name, setting);
}

void Mod::Impl::setIsCurrentlyLoading(bool isLoading) {
    m_isCurrentlyLoading = isLoading;
}

bool Mod::Impl::isCurrentlyLoading() const {
    return m_isCurrentlyLoading;
}

void Mod::Impl::setEnabled(bool enabled) {
    m_enabled = enabled;
}

void Mod::Impl::addDependant(Mod* mod) {
    m_dependants.emplace_back(mod);
}

void Mod::Impl::addHook(Hook* hook) {
    m_hooks.emplace_back(hook);
}

void Mod::Impl::addPatch(Patch* patch) {
    m_patches.emplace_back(patch);
}

void Mod::Impl::setLoggingEnabled(bool enabled) {
    m_loggingEnabled = enabled;
}

bool Mod::Impl::isLoggingEnabled() const {
    return m_loggingEnabled;
}
