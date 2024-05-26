#pragma once

#ifdef _MSC_VER
    #pragma warning(disable : 4251) // dll-interface
    #pragma warning(disable : 4099) // type first seen as 'class'
    #pragma warning(disable : 4584) // CCKeyboardDelegate included twice in UILayer
#endif

#include <Geode/Enums.hpp>

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/addresser.hpp>
#include <cocos-ext.h>
#include <cocos2d.h>
#include <fmod.hpp>
#include <stdint.h>
#include <type_traits>
#include <unordered_map>

#define CLASSPARAM(...)
#define STRUCTPARAM(...)

#include <Geode/GeneratedBinding.hpp>
