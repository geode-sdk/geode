#include <Geode/DefaultInclude.hpp>

#if defined(GEODE_IS_ANDROID)

#include "../load.hpp"
#include <jni.h>

extern "C" [[gnu::visibility("default")]] jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    geodeEntry(nullptr);
    return JNI_VERSION_1_1;
}

#endif