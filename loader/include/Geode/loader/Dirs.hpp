#pragma once

#include <asp/fs/Path.hpp>
#include "../DefaultInclude.hpp"

namespace geode::dirs {
    /**
     * Directory where Geometry Dash is
     *
     * Windows: {GD installation folder}/ (traditionally Geometry Dash)
     * macOS: {GD installation folder}/ (traditionally GeometryDash.app)
     * Android: {Android Launcher's media directory}/game
     * iOS: Documents/game (within the launcher's sandbox)
     */
    GEODE_DLL asp::fs::Path getGameDir();
    /**
     * Directory where GD saves its files
     *
     * Windows: %LOCALAPPDATA%/{executable name}
     * macOS: ~/Library/Application Support/GeometryDash
     * Android: {Android Launcher's media directory}/save
     * iOS: Documents/save (within the launcher's sandbox)
     */
    GEODE_DLL asp::fs::Path getSaveDir();
    /**
     * Directory where Geode is
     * `getGameDir()`/geode
     */
    GEODE_DLL asp::fs::Path getGeodeDir();
    /**
     * Directory where Geode saves its files
     * `getSaveDir()`/geode
     */
    GEODE_DLL asp::fs::Path getGeodeSaveDir();
    /**
     * Directory where Geode's resources are stored
     * `getGeodeDir()`/resources
     */
    GEODE_DLL asp::fs::Path getGeodeResourcesDir();
    /**
     * Directory where Geode's logs are stored
     * `getGeodeDir()`/logs
     */
    GEODE_DLL asp::fs::Path getGeodeLogDir();
    /**
     * Directory to store temporary files
     * `getGeodeDir()`/temp
     */
    GEODE_DLL asp::fs::Path getTempDir();
    /**
     * Directory where mods are stored by default
     * `getGeodeDir()`/mods
     */
    GEODE_DLL asp::fs::Path getModsDir();
    /**
     * Directory where mods' save data is stored
     * `getGeodeSaveDir()`/mods
     */
    GEODE_DLL asp::fs::Path getModsSaveDir();
    /**
     * Directory where mods' unzipped packages are stored at runtime
     *
     * Windows/macOS/iOS: `getGeodeDir()`
     * Android: {Android Launcher's internal files directory}/geode
     */
    GEODE_DLL asp::fs::Path getGeodeInternalDir();
    /**
     * Directory where mods' unzipped packages are stored at runtime
     *
     * Windows/macOS/iOS: `getGeodeDir()`/unzipped
     * Android: {Android Launcher's internal files directory}/geode/unzipped
     */
    GEODE_DLL asp::fs::Path getModRuntimeDir();
    /**
     * Directory where mods' unzipped binary files are stored
     * This is used in order to override existing unzip binaries,
     * such as on iOS where signing the binary is required
     *
     * TODO: enable on ~~4.7.0~~ nice job
     */
    // GEODE_DLL asp::fs::Path getModBinariesDir();
    /**
     * Directory where mods' config files lie
     * `getGeodeDir()`/config
     */
    GEODE_DLL asp::fs::Path getModConfigDir();
    /**
     * Directory where Geode stores the cached index
     * `getGeodeDir()`/index
     */
    GEODE_DLL asp::fs::Path getIndexDir();
    /**
     * Directory where crashlogs are stored
     * `getGeodeDir()`/crashlogs
     */
    GEODE_DLL asp::fs::Path getCrashlogsDir();
    /**
     * Directory where mods' persistent files lie
     * This directory is not deleted even when Geode is uninstalled
     * `getSaveDir()`/geode-persistent
     */
    GEODE_DLL asp::fs::Path getModPersistentDir();
    /**
     * Directory where Geometry Dash's resources are stored
     *
     * Windows/macOS: `getGameDir()`/Resources
     * Android: assets
     * iOS: {GD bundle path}/Resources
     */
    GEODE_DLL asp::fs::Path getResourcesDir();
}
