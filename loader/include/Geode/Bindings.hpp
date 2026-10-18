#pragma once

#ifdef _MSC_VER
    #pragma warning(disable : 4251) // dll-interface
    #pragma warning(disable : 4099) // type first seen as 'class'
    #pragma warning(disable : 4584) // CCKeyboardDelegate included twice in UILayer
#endif

#include <Geode/Enums.hpp>

#include <Geode/DefaultInclude.hpp>
#include <fmod.hpp>
#include <stdint.h>
#include <type_traits>
#include <array>
#include <unordered_map>
#include <algorithm>

#define CLASSPARAM(...)
#define STRUCTPARAM(...)

#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/include/ccTypes.h>

#include <Geode/GeneratedBinding.hpp>
