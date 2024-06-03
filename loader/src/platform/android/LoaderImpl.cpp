#include <Geode/loader/IPC.hpp>
#include <loader/LoaderImpl.hpp>
#include <Geode/cocos/platform/android/jni/JniHelper.h>

using namespace geode::prelude;

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

bool Loader::Impl::userTriedToLoadDLLs() const {
    return false;
}

void Loader::Impl::addNativeBinariesPath(std::filesystem::path const& path) {
    log::warn("LoaderImpl::addNativeBinariesPath not implement on this platform, not adding path {}", path.string());
}

bool Loader::Impl::supportsLaunchArguments() const {
    return true;
}

std::string Loader::Impl::getLaunchCommand() const {
    std::string launchArgs = "";

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "getLaunchArguments", "()Ljava/lang/String;")) {
        jstring str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
        t.env->DeleteLocalRef(t.classID);
        launchArgs = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    } else {
        // this is also defined in utils, but this is a loader function and thus it can't access
        auto vm = JniHelper::getJavaVM();

        JNIEnv* env;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) {
            env->ExceptionClear();
        }
    }

    return launchArgs;
}
