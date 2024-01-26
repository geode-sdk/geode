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

void Loader::Impl::addNativeBinariesPath(ghc::filesystem::path const& path) {
    log::warn("LoaderImpl::addNativeBinariesPath not implement on this platform, not adding path {}", path.string());
}

bool Loader::Impl::supportsLaunchArguments() const {
    return false;
}

std::string Loader::Impl::getLaunchCommand() const {
    return std::string(); // Empty
}

bool Loader::Impl::getPermissionStatus(std::string_view const name) const {
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info, "com/geode/launcher/utils/GeodeUtils", "getPermissionStatus", "(Ljava/lang/String;)Z")) {
        jstring permString = info.env->NewStringUTF(std::string(name).c_str());
        jboolean result = info.env->CallStaticBooleanMethod(info.classID, info.methodID, permString);
        info.env->DeleteLocalRef(info.classID);
        info.env->DeleteLocalRef(permString);

        return result == JNI_TRUE;
    } else {
        auto vm = JniHelper::getJavaVM();

        JNIEnv* env;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) {
            env->ExceptionClear();
        }
    }

    return false;
}

static utils::MiniFunction<void(bool)> s_permissionCallback;

extern "C"
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_permissionCallback(
        JNIEnv* env,
        jobject,
        jboolean granted
) {
    if (s_permissionCallback) {
        Loader::get()->queueInMainThread([granted] {
            s_permissionCallback(granted == JNI_TRUE);
        });
    }
}

void Loader::Impl::requestPermission(std::string_view const name, utils::MiniFunction<void(bool)> callback) const {
    s_permissionCallback = callback;
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info, "com/geode/launcher/utils/GeodeUtils", "requestPermission", "(Ljava/lang/String;)V")) {
        jstring permString = info.env->NewStringUTF(std::string(name).c_str());
        info.env->CallStaticVoidMethod(info.classID, info.methodID, permString);
        info.env->DeleteLocalRef(info.classID);
        info.env->DeleteLocalRef(permString);
    } else {
        auto vm = JniHelper::getJavaVM();

        JNIEnv* env;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) {
            env->ExceptionClear();
        }
    }
}
