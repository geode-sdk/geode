#pragma once

#include <matjson.hpp>
#include "ModPatch.hpp"
#include <Geode/loader/Loader.hpp>

namespace geode {
    class Mod::Impl {
    public:
        Mod* m_self;
        /**
         * Mod metadata
         */
        ModMetadata m_metadata;
        /**
         * Platform-specific info
         */
        PlatformInfo* m_platformInfo = nullptr;
        /**
         * Hooks owned by this mod
         */
        std::vector<std::shared_ptr<Hook>> m_hooks;
        /**
         * Patches owned by this mod
         */
        std::vector<std::shared_ptr<Patch>> m_patches;
        /**
         * Whether the mod is enabled or not
         */
        bool m_enabled = false;
        /**
         * Mod temp directory name
         */
        ghc::filesystem::path m_tempDirName;
        /**
         * Mod save directory name
         */
        ghc::filesystem::path m_saveDirPath;
        /**
         * Pointers to mods that depend on this Mod.
         * Makes it possible to enable / disable them automatically,
         * when their dependency is disabled.
         */
        std::vector<Mod*> m_dependants;
        /**
         * Saved values
         */
        matjson::Value m_saved = matjson::Object();
        /**
         * Setting values
         */
        std::unordered_map<std::string, std::unique_ptr<SettingValue>> m_settings;
        /**
         * Settings save data. Stored for efficient loading of custom settings
         */
        matjson::Value m_savedSettingsData = matjson::Object();
        /**
         * Whether the mod resources are loaded or not
         */
        bool m_resourcesLoaded = false;
        /**
         * Whether logging is enabled for this mod
         */
        bool m_loggingEnabled = true;

        std::unordered_map<std::string, char const*> m_expandedSprites;

        bool m_isCurrentlyLoading = false;

        ModRequestedAction m_requestedAction = ModRequestedAction::None;

        std::vector<LoadProblem> m_problems;

        Impl(Mod* self, ModMetadata const& metadata);
        ~Impl();

        Result<> setup();

        Result<> loadPlatformBinary();
        Result<> createTempDir();

        // called on a separate thread
        Result<> unzipGeodeFile(ModMetadata metadata);

        void setupSettings();

        std::string getID() const;
        std::string getName() const;
        std::vector<std::string> getDevelopers() const;
        std::optional<std::string> getDescription() const;
        std::optional<std::string> getDetails() const;
        ghc::filesystem::path getPackagePath() const;
        VersionInfo getVersion() const;
        bool isEnabled() const;
        bool isInternal() const;
        bool needsEarlyLoad() const;
        ModMetadata getMetadata() const;
        ghc::filesystem::path getTempDir() const;
        ghc::filesystem::path getBinaryPath() const;

        matjson::Value& getSaveContainer();
        matjson::Value& getSavedSettingsData();

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
        void setMetadata(ModMetadata const& metadata);
        std::vector<Mod*> getDependants() const;
#endif

        Result<> saveData();
        Result<> loadData();

        ghc::filesystem::path getSaveDir() const;
        ghc::filesystem::path getConfigDir(bool create = true) const;

        bool hasSettings() const;
        std::vector<std::string> getSettingKeys() const;
        bool hasSetting(std::string_view const key) const;
        std::optional<Setting> getSettingDefinition(std::string_view const key) const;
        SettingValue* getSetting(std::string_view const key) const;
        void registerCustomSetting(std::string_view const key, std::unique_ptr<SettingValue> value);

        std::string getLaunchArgumentName(std::string_view const name) const;
        std::vector<std::string> getLaunchArgumentNames() const;
        bool hasLaunchArgument(std::string_view const name) const;
        std::optional<std::string> getLaunchArgument(std::string_view const name) const;
        bool getLaunchFlag(std::string_view const name) const;

        Result<Hook*> claimHook(std::shared_ptr<Hook> hook);
        Result<> disownHook(Hook* hook);
        [[nodiscard]] std::vector<Hook*> getHooks() const;

        Result<Patch*> claimPatch(std::shared_ptr<Patch> patch);
        Result<> disownPatch(Patch* patch);
        [[nodiscard]] std::vector<Patch*> getPatches() const;

        Result<> enable();
        Result<> disable();
        Result<> uninstall(bool deleteSaveData = false);
        bool isUninstalled() const;

        // 1.3.0 additions
        ModRequestedAction getRequestedAction() const;

        bool depends(std::string_view const id) const;
        Result<> updateDependencies();
        bool hasUnresolvedDependencies() const;
        bool hasUnresolvedIncompatibilities() const;

        Result<> loadBinary();

        char const* expandSpriteName(char const* name);
        ModJson getRuntimeInfo() const;

        bool isLoggingEnabled() const;
        void setLoggingEnabled(bool enabled);

        std::vector<LoadProblem> getProblems() const;

        bool hasProblems() const;
        bool shouldLoad() const;
        bool isCurrentlyLoading() const;
    };

    class ModImpl : public Mod::Impl {
    public:
        static Mod::Impl* get();

        static Mod::Impl* getImpl(Mod* mod);
    };
}
