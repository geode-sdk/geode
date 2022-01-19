#pragma once

#include <platform/platform.hpp>

// Because C++ doesn't like using a
// namespace that doesn't exist

namespace lilac {}
namespace lilac::cast {}
namespace lilac::cocos {}
namespace lilac::utils {}
namespace lilac::node {}
namespace lilac::op {}
namespace lilac::stream {}
namespace cocos2d {}
namespace cocos2d::extension {}

#define USE_LILAC_NAMESPACE()           \
    using namespace lilac;              \
    using namespace lilac::cast;        \
    using namespace lilac::cocos;       \
    using namespace lilac::utils;       \
    using namespace lilac::node;        \
    using namespace lilac::op;          \
    using namespace lilac::stream;      \
    using namespace cocos2d;            \
    using namespace cocos2d::extension; \


