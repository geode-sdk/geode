#pragma once

#include "platform.hpp"

// Set dllexport/dllimport to geode classes & functions
#ifdef GEODE_EXPORTING
    #define GEODE_DLL    GEODE_WINDOWS(__declspec(dllexport))
#else
    #define GEODE_DLL    GEODE_WINDOWS(__declspec(dllimport))
#endif
#define GEODE_API    GEODE_WINDOWS() // idk where this is

// Because C++ doesn't like using a
// namespace that doesn't exist

namespace geode {}
namespace geode::cast {}
namespace geode::cocos {}
namespace geode::utils {}
namespace geode::node {}
namespace geode::op {}
namespace geode::stream {}
namespace cocos2d {}
namespace cocos2d::extension {}

#define USE_GEODE_NAMESPACE()           \
    using namespace geode;              \
    using namespace geode::cast;        \
    using namespace geode::cocos;       \
    using namespace geode::utils;       \
    using namespace geode::node;        \
    using namespace geode::op;          \
    using namespace geode::stream;      \
    using namespace cocos2d;            \
    using namespace cocos2d::extension; \


