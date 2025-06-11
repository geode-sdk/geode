# PlatformDetect.cmake

# Clear previous platform variable
unset(GEODE_TARGET_PLATFORM CACHE)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # Detect MacOS vs iOS
    if(IOS)
        set(GEODE_TARGET_PLATFORM "iOS")
    else()
        # Use CMAKE_OSX_SYSROOT to distinguish iOS SDK from MacOS
        if(CMAKE_OSX_SYSROOT MATCHES "iphoneos")
            set(GEODE_TARGET_PLATFORM "iOS")
        else()
            set(GEODE_TARGET_PLATFORM "MacOS")
        endif()
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(GEODE_TARGET_PLATFORM "Win64")
    else()
        message(FATAL_ERROR "32-bit Windows is not supported")
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Support both 32-bit and 64-bit, but default to Linux platform
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(GEODE_TARGET_PLATFORM "Linux")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(GEODE_TARGET_PLATFORM "Linux")
    else()
        message(FATAL_ERROR "Unknown Linux architecture pointer size: ${CMAKE_SIZEOF_VOID_P}")
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    # Android can be 32-bit or 64-bit
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(GEODE_TARGET_PLATFORM "Android64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(GEODE_TARGET_PLATFORM "Android32")
    else()
        message(FATAL_ERROR "Unknown Android architecture pointer size: ${CMAKE_SIZEOF_VOID_P}")
    endif()

else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

message(STATUS "Detected platform: ${GEODE_TARGET_PLATFORM}")
