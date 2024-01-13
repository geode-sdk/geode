#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Log.hpp>
#include <loader/ModImpl.hpp>
#include <iostream>
#include <loader/LoaderImpl.hpp>
#include <Geode/utils/string.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_ANDROID

#include <Geode/cocos/platform/android/jni/JniHelper.h>
#include <android/log.h>

namespace {
    android_LogPriority getLogSeverityForSeverity(Severity severity) {
        switch (severity) {
            case Severity::Debug: return ANDROID_LOG_DEBUG;
            case Severity::Info: return ANDROID_LOG_INFO;
            case Severity::Warning: return ANDROID_LOG_WARN;
            case Severity::Error: return ANDROID_LOG_ERROR;
            default: return ANDROID_LOG_DEFAULT;
        }
    }
}

std::string Loader::Impl::getGameVersion() {
    if (m_gdVersion.empty()) {
        /*
        // before uncommenting please note:
        // getGameVersion can only run after JNI_OnLoad is called. otherwise it crashes

        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "getGameVersion", "()Ljava/lang/String;")) {
            jstring str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
            t.env->DeleteLocalRef(t.classID);
            m_gdVersion = JniHelper::jstring2string(str);
            t.env->DeleteLocalRef(str);
        } else {
            auto vm = JniHelper::getJavaVM();

            JNIEnv* env;
            if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) {
                env->ExceptionClear();
            }
        }
        */
    }
    return m_gdVersion;
}

void Loader::Impl::platformMessageBox(char const* title, std::string const& info, Severity severity) {
    cocos2d::CCMessageBox(info.c_str(), title);
}

void Loader::Impl::logConsoleMessageWithSeverity(std::string const& msg, Severity severity) {
    __android_log_print(
        getLogSeverityForSeverity(severity),
        "Geode",
        "%s",
        msg.c_str()
    );
}

void Loader::Impl::openPlatformConsole() {
    return;
}

void Loader::Impl::closePlatformConsole() {
    return;
}

void Loader::Impl::setupIPC() {
    log::warn("IPC is not supported on this platform!");
}

bool Loader::Impl::userTriedToLoadDLLs() const {
    return false;
}

#endif
