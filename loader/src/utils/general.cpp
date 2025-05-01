#include <Geode/utils/general.hpp>

#ifndef GEODE_IS_MACOS
// feel free to properly implement this for other platforms
float geode::utils::getDisplayFactor() {
    return 1.0f;
}
#endif