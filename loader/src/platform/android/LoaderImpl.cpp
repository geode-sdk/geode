#include <Geode/loader/IPC.hpp>
#include <loader/LoaderImpl.hpp>
#include <Geode/cocos/platform/android/jni/JniHelper.h>

using namespace geode::prelude;

std::string Loader::Impl::getGameVersion() {
    if (m_gdVersion.empty()) {
        std::ifstream version_file("/data/data/com.geode.launcher/files/game_version.txt");
        if (!version_file) {
            // probably on an older launcher
            return m_gdVersion;
        }

        int version_code = 0;
        version_file >> version_code;

        version_file.close();

        switch (version_code) {
            case 37: m_gdVersion = "2.200"; break;
            case 38: m_gdVersion = "2.205"; break;
            case 39: m_gdVersion = "2.206"; break;
            default: m_gdVersion = std::to_string(version_code);
        }
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
