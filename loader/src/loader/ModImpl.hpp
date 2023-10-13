#pragma once

#include <json.hpp>

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
        std::vector<Hook*> m_hooks;
        /**
         * Patches owned by this mod
         */
        std::vector<Patch*> m_patches;
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
        json::Value m_saved = json::Object();
        /**
         * Setting values
         */
        std::unordered_map<std::string, std::unique_ptr<SettingValue>> m_settings;
        /**
         * Settings save data. Stored for efficient loading of custom settings
         */
        json::Value m_savedSettingsData = json::Object();
        /**
         * Whether the mod resources are loaded or not
         */
        bool m_resourcesLoaded = false;
        /**
         * Whether logging is enabled for this mod
         */
        bool m_loggingEnabled = true;

        std::unordered_map<std::string, char const*> m_expandedSprites;


        ModRequestedAction m_requestedAction = ModRequestedAction::None;

        Impl(Mod* self, ModMetadata const& metadata);
        ~Impl();

        Result<> setup();

        Result<> loadPlatformBinary();
        Result<> unloadPlatformBinary();
        Result<> createTempDir();

        void setupSettings();

        std::string getID() const;
        std::string getName() const;
        std::string getDeveloper() const;
        std::optional<std::string> getDescription() const;
        std::optional<std::string> getDetails() const;
        ghc::filesystem::path getPackagePath() const;
        VersionInfo getVersion() const;
        bool isEnabled() const;
        bool supportsDisabling() const;
        bool needsEarlyLoad() const;
        ModMetadata getMetadata() const;
        ghc::filesystem::path getTempDir() const;
        ghc::filesystem::path getBinaryPath() const;

        json::Value& getSaveContainer();

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
        bool hasSetting(std::string const& key) const;
        std::optional<Setting> getSettingDefinition(std::string const& key) const;
        SettingValue* getSetting(std::string const& key) const;
        void registerCustomSetting(std::string const& key, std::unique_ptr<SettingValue> value);

        std::vector<Hook*> getHooks() const;
        Result<Hook*> addHook(Hook* hook);
        Result<> enableHook(Hook* hook);
        Result<> disableHook(Hook* hook);
        Result<> removeHook(Hook* hook);
        Result<Patch*> patch(void* address, ByteVector const& data);
        Result<> unpatch(Patch* patch);
        Result<> enable();
        Result<> disable();
        Result<> uninstall(bool deleteSaveData);
        bool isUninstalled() const;

        // 1.3.0 additions
        ModRequestedAction getRequestedAction() const;

        bool depends(std::string const& id) const;
        Result<> updateDependencies();
        bool hasUnresolvedDependencies() const;
        bool hasUnresolvedIncompatibilities() const;
        [[deprecated]] std::vector<Dependency> getUnresolvedDependencies();

        Result<> loadBinary();

        char const* expandSpriteName(char const* name);
        ModJson getRuntimeInfo() const;

        bool isLoggingEnabled() const;
        void setLoggingEnabled(bool enabled);

        bool shouldLoad() const;
    };

    class ModImpl : public Mod::Impl {
    public:
        static Mod::Impl* get();

        static Mod::Impl* getImpl(Mod* mod);
    };
}
