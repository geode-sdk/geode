#include <Geode/DefaultInclude.hpp>

#include "../load.hpp"
#include <jni.h>
#include <Geode/cocos/platform/android/jni/JniHelper.h>
#include "internalString.hpp"
#include <cocos2d.h>
#include <Geode/loader/Log.hpp>
#include <filesystem>
#include <Geode/loader/Dirs.hpp>

using namespace geode::prelude;

// idk where to put this
#include <EGL/egl.h>
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT = 0;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT = 0;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT = 0;

namespace {
    bool reportPlatformCapability(std::string id) {
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "reportPlatformCapability", "(Ljava/lang/String;)Z")) {
            jstring stringArg1 = t.env->NewStringUTF(id.c_str());

            auto r = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);

            t.env->DeleteLocalRef(stringArg1);
            t.env->DeleteLocalRef(t.classID);

            return r;
        } else {
            auto vm = JniHelper::getJavaVM();

            JNIEnv* env;
            if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) {
                env->ExceptionClear();
            }
        }

        return false;
    }
}

extern "C" [[gnu::visibility("default")]] jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    glGenVertexArraysOESEXT = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
    glBindVertexArrayOESEXT = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
    glDeleteVertexArraysOESEXT = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");

    auto updatePath = geode::dirs::getGameDir() / "update";
    std::error_code ec;
    std::filesystem::remove_all(updatePath, ec);
    if (ec) {
        geode::log::warn("Failed to remove update directory: {}", ec.message());
    }

    {
        // Epic hack: get the empty internal string from CCString
        // avoid ::create as to not call autorelease
        auto* cc = new CCString();
        setEmptyInternalString(&cc->m_sString);
        delete cc;
    }

    reportPlatformCapability("extended_input");

    geodeEntry(nullptr);
    return JNI_VERSION_1_6;
}

extern "C" [[gnu::visibility("default")]] void emptyFunction(void*) {
    // empty
}
