#pragma once

#include <json.hpp>

namespace geode {
    class Mod::Impl {
    public:
        Mod* m_self;
        /**
         * Mod info
         */
        ModInfo m_info;
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
         * Whether the mod binary is loaded or not
         */
        bool m_binaryLoaded = false;
        /**
         * Mod temp directory name
         */
        ghc::filesystem::path m_tempDirName;
        /**
         * Mod save directory name
         */
        ghc::filesystem::path m_saveDirPath;
        /**
         * Pointers to mods that depend on
         * this Mod. Makes it possible to
         * enable / disable them automatically,
         * when their dependency is disabled.
         */
        std::vector<Mod*> m_parentDependencies;
        /**
         * Saved values
         */
        json::Value m_saved;
        /**
         * Setting values
         */
        std::unordered_map<std::string, std::unique_ptr<SettingValue>> m_settings;
        /**
         * Settings save data. Stored for efficient loading of custom settings
         */
        json::Value m_savedSettingsData;

        Impl(Mod* self, ModInfo const& info);
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
        bool isLoaded() const;
        bool supportsDisabling() const;
        bool supportsUnloading() const;
        bool wasSuccesfullyLoaded() const;
        ModInfo getModInfo() const;
        ghc::filesystem::path getTempDir() const;
        ghc::filesystem::path getBinaryPath() const;

        json::Value& getSaveContainer();

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
        Result<> loadBinary();
        Result<> unloadBinary();
        Result<> enable();
        Result<> disable();
        Result<> uninstall();
        bool isUninstalled() const;
        bool depends(std::string const& id) const;
        bool hasUnresolvedDependencies() const;
        Result<> updateDependencies();
        std::vector<Dependency> getUnresolvedDependencies();

        char const* expandSpriteName(char const* name);
        ModJson getRuntimeInfo() const;
    };

    class ModImpl : public Mod {
    public:
        static Mod* get();

        static Mod::Impl* getImpl(Mod* mod);
    };
}