#pragma once 

/**
 * 2 levels of indirection for common operations
 */
#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)

#define STR_(...) #__VA_ARGS__
#define STR(...) STR_(__VA_ARGS__)

#if defined(GEODE_DEBUG)
	#define GeodeDebug(format, ...) GeodeLog(format, ##__VA_ARGS__)
#else
    #define GeodeDebug(format, ...)
#endif

#define EXPAND(x) x
#define NUMBER_OF_ARGS(...) \
    EXPAND(_NARGS_IMPL(__VA_ARGS__,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))
#define _NARGS_IMPL(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,N,...) N

#define NUMBER_AFTER(num) CONCAT(NUMBER_AFTER_, num)()
#define NUMBER_AFTER_0() 1
#define NUMBER_AFTER_1() 2
#define NUMBER_AFTER_2() 3
#define NUMBER_AFTER_3() 4
#define NUMBER_AFTER_4() 5
#define NUMBER_AFTER_5() 6
#define NUMBER_AFTER_6() 7
#define NUMBER_AFTER_7() 8
#define NUMBER_AFTER_8() 9
#define NUMBER_AFTER_9() 10
#define NUMBER_AFTER_10() 11
#define NUMBER_AFTER_11() 12
#define NUMBER_AFTER_12() 13
#define NUMBER_AFTER_13() 14
#define NUMBER_AFTER_14() 15
#define NUMBER_AFTER_15() 16
#define NUMBER_AFTER_16() 17
#define NUMBER_AFTER_17() 18
#define NUMBER_AFTER_18() 19
#define NUMBER_AFTER_19() 20

#define TYPE(arg, i) decltype(CONCAT(p, i))
#define PARAMETER(arg, i) CONCAT(p, i)
#define EVAL(arg, i) arg PARAMETER(arg, i)
#define DECLVAL(arg, i) std::declval<arg>()

#define GETREPEAT(...) CONCAT(REPEAT, NUMBER_OF_ARGS(__VA_ARGS__))
#define REPEAT0( macro, i, ...)
#define REPEAT1( macro, i, arg, ...) macro(arg, i)
#define REPEAT2( macro, i, arg, ...) macro(arg, i), REPEAT1( macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT3( macro, i, arg, ...) macro(arg, i), REPEAT2( macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT4( macro, i, arg, ...) macro(arg, i), REPEAT3( macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT5( macro, i, arg, ...) macro(arg, i), REPEAT4( macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT6( macro, i, arg, ...) macro(arg, i), REPEAT5( macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT7( macro, i, arg, ...) macro(arg, i), REPEAT6( macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT8( macro, i, arg, ...) macro(arg, i), REPEAT7( macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT9( macro, i, arg, ...) macro(arg, i), REPEAT8( macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT10(macro, i, arg, ...) macro(arg, i), REPEAT9( macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT11(macro, i, arg, ...) macro(arg, i), REPEAT10(macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT12(macro, i, arg, ...) macro(arg, i), REPEAT11(macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT13(macro, i, arg, ...) macro(arg, i), REPEAT12(macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT14(macro, i, arg, ...) macro(arg, i), REPEAT13(macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT15(macro, i, arg, ...) macro(arg, i), REPEAT14(macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT16(macro, i, arg, ...) macro(arg, i), REPEAT15(macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT17(macro, i, arg, ...) macro(arg, i), REPEAT16(macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT18(macro, i, arg, ...) macro(arg, i), REPEAT17(macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT19(macro, i, arg, ...) macro(arg, i), REPEAT18(macro, NUMBER_AFTER(i), __VA_ARGS__)
#define REPEAT20(macro, i, arg, ...) macro(arg, i), REPEAT19(macro, NUMBER_AFTER(i), __VA_ARGS__)

