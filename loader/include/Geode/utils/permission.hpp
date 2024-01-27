#pragma once

#include <Geode/DefaultInclude.hpp>
#include "MiniFunction.hpp"
#include <string_view>

namespace geode::utils::permission {
    /**
     * Request whether the given permission is granted to Geode by the operating system.
     * On Android, the permission name should start with `android.permission`.
     * @param name The name of the permission
    */
    bool getPermissionStatus(std::string_view const name);

    /**
     * Request a permission to be granted by the operating system.
     * On Android, the permission name should start with `android.permission`.
     * @param name The name of the permission
     * @param callback The callback, passed value is 'true' if permission was granted and 'false' otherwise.
     */
    void requestPermission(std::string_view const name, utils::MiniFunction<void(bool)> callback);
}