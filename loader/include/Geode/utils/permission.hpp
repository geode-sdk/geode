#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/function.hpp>
#include <string_view>

namespace geode::utils::permission {
    enum class Permission {
        ReadAllFiles = 0x3, // maintain compatibility with some older mods
        RecordAudio
    };

    /**
     * Request whether the given permission is granted to Geode by the operating system.
     * @param permission The permission
    */
    bool GEODE_DLL getPermissionStatus(Permission permission);

    /**
     * Request a permission to be granted by the operating system.
     * @param permission The permission
     * @param callback The callback, passed value is 'true' if permission was granted and 'false' otherwise.
     */
    void GEODE_DLL requestPermission(Permission permission, geode::Function<void(bool)> callback);
}