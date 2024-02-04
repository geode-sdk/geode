#pragma once

#include <Geode/DefaultInclude.hpp>
#include "MiniFunction.hpp"
#include <string_view>

namespace geode::utils::permission {
    enum class Permission {
        ReadAudio [[deprecated("This permission is Android 13+ specific! Use ReadFiles instead.")]],
        ReadImages [[deprecated("This permission is Android 13+ specific! Use ReadFiles instead.")]],
        ReadVideo [[deprecated("This permission is Android 13+ specific! Use ReadFiles instead.")]],
        ReadAllFiles,
        RecordAudio,
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
    void GEODE_DLL requestPermission(Permission permission, utils::MiniFunction<void(bool)> callback);
}