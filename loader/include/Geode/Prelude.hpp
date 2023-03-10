#pragma once

// Because C++ doesn't like using
// namespaces that don't exist
namespace geode {
    namespace addresser {}
    namespace cast {}
    namespace cocos {}
    namespace utils {}
    namespace helper {}
    namespace op {}
    namespace stream {}
    namespace view {}
}

namespace cocos2d {
    namespace extension {}
}

namespace geode {
    namespace prelude {
        using namespace ::geode;
        using namespace ::geode::addresser;
        using namespace ::geode::cast;
        using namespace ::geode::cocos;
        using namespace ::geode::helper;
        using namespace ::geode::utils;
        using namespace ::geode::op;
        using namespace ::geode::stream;
        using namespace ::geode::view;
        using namespace ::cocos2d;
        using namespace ::cocos2d::extension;
    }

    namespace [[deprecated("Use `using namespace geode::prelude` instead!")]] old_prelude {
        using namespace ::geode::prelude;
    }
}

#define USE_GEODE_NAMESPACE() using namespace geode::old_prelude;