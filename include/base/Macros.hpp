#pragma once

#include "platform/platform.hpp"

// Because C++ doesn't like using a
// namespace that doesn't exist

namespace geode {}
namespace geode::addresser {}
namespace geode::cast {}
namespace geode::cocos {}
namespace geode::utils {}
namespace geode::helper {}
namespace geode::node {}
namespace geode::op {}
namespace geode::stream {}
namespace geode::view {}
namespace cocos2d {}
namespace cocos2d::extension {}

#define USE_GEODE_NAMESPACE()           \
    using namespace geode;              \
    using namespace geode::addresser;   \
    using namespace geode::cast;        \
    using namespace geode::cocos;       \
    using namespace geode::helper;      \
    using namespace geode::utils;       \
    using namespace geode::node;        \
    using namespace geode::op;          \
    using namespace geode::stream;      \
    using namespace geode::view;        \
    using namespace cocos2d;            \
    using namespace cocos2d::extension; \

#define GEODE_STATIC_PTR(type, name)        \
static type* s_##name;                      \
inline type* name() {                       \
	if (!s_##name) s_##name = new type();   \
	return s_##name;                        \
}

#define GEODE_STATIC_VAR(type, name)        \
inline type& name() {                       \
	static type s_##name;                   \
	return s_##name;                        \
}

#define GEODE_WRAPPER_CONCAT(x, y) x##y
#define GEODE_CONCAT(x, y) GEODE_WRAPPER_CONCAT(x, y)

#define GEODE_WRAPPER_STR(...) #__VA_ARGS__
#define GEODE_STR(...) GEODE_WRAPPER_STR(__VA_ARGS__)

#define GEODE_PAD(size) uint8_t GEODE_CONCAT(__pad, __LINE__)[size]
#define GEODE_UNIMPLEMENTED_PAD private:

#define GEODE_NONINHERITED_MEMBERS private:

#define GEODE_EXPAND(x) x
#define GEODE_INVOKE(macro, ...) GEODE_EXPAND(macro(__VA_ARGS__))

#define GEODE_NUMBER_OF_ARGS(...) GEODE_EXPAND(GEODE_NUMBER_OF_ARGS_(__VA_ARGS__, GEODE_NUMBER_SEQUENCE(),))
#define GEODE_NUMBER_OF_ARGS_(...) GEODE_EXPAND(GEODE_NUMBER_OF_ARGS_N(__VA_ARGS__))
#define GEODE_NUMBER_OF_ARGS_N( 			\
_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, 	\
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, 	\
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, 	\
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, 	\
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, 	\
_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, 	\
_61,_62,_63,N,...) N
#define GEODE_NUMBER_SEQUENCE() 			\
63, 62, 61, 60,                    			\
59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 	\
49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 	\
39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 	\
29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 	\
19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 	\
9,  8,  7,  6,  5,  4,  3,  2,  1,  0

