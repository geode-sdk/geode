#pragma once

#include <filesystem>
#include "../DefaultInclude.hpp"

namespace geode::dirs {
    /**
     * Directory where Geometry Dash is
     */
    GEODE_DLL std::filesystem::path getGameDir();
    /**
     * Directory where GD saves its files
     */
    GEODE_DLL std::filesystem::path getSaveDir();
    /**
     * Directory where Geode is
     */
    GEODE_DLL std::filesystem::path getGeodeDir();
    /**
     * Directory where Geode saves its files
     */
    GEODE_DLL std::filesystem::path getGeodeSaveDir();
    /**
     * Directory where Geode's resources are stored
     */
    GEODE_DLL std::filesystem::path getGeodeResourcesDir();
    /**
     * Directory where Geode's resources are stored
     */
    GEODE_DLL std::filesystem::path getGeodeLogDir();
    /**
     * Directory to store temporary files
     */
    GEODE_DLL std::filesystem::path getTempDir();
    /**
     * Directory where mods are stored by default
     */
    GEODE_DLL std::filesystem::path getModsDir();
    /**
     * Directory where mods' save data is stored
     */
    GEODE_DLL std::filesystem::path getModsSaveDir();
    /**
     * Directory where mods' unzipped packages are stored at runtime
     */
    GEODE_DLL std::filesystem::path getModRuntimeDir();
    /**
     * Directory where mods' config files lie
     */
    GEODE_DLL std::filesystem::path getModConfigDir();
    /**
     * Directory where Geode stores the cached index
     */
    GEODE_DLL std::filesystem::path getIndexDir();
    /**
     * Directory where crashlogs are stored
     */
    GEODE_DLL std::filesystem::path getCrashlogsDir();
    /**
     * Directory where mods' persistent files lie
     * This directory is not deleted even when Geode is uninstalled
     */
    GEODE_DLL std::filesystem::path getModPersistentDir();
}
