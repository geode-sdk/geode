#ifndef __SUPPORT_ZIPUTILS_MACROS_H__
#define __SUPPORT_ZIPUTILS_MACROS_H__

/**
 * This macro is for being able to define the 
 * Zip Utils stuff while not screwing ourselves 
 * over with undefined symbols with CC_DLL
 */
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #ifdef GEODE_EXPORTING
        #define CC_ZIP_DLL __declspec(dllexport)
    #else
        #define CC_ZIP_DLL __declspec(dllimport)
    #endif
#else
    #define CC_ZIP_DLL __attribute__((visibility("default")))
#endif

#endif
