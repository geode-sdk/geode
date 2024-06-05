#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#include <Geode/utils/cocos.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/web.hpp>
#include <filesystem>
#include <Geode/utils/file.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/utils/MiniFunction.hpp>
#include <Geode/utils/permission.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/binding/AppDelegate.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/binding/MenuLayer.hpp>

#include <jni.h>
#include <Geode/cocos/platform/android/jni/JniHelper.h>

using geode::utils::permission::Permission;

bool utils::clipboard::write(std::string const& data) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "writeClipboard", "(Ljava/lang/String;)V")) {
        jstring stringArg1 = t.env->NewStringUTF(data.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        return true;
    }
    return false;
}

std::string utils::clipboard::read() {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "readClipboard", "()Ljava/lang/String;")) {
        jstring stringResult = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);

        std::string result = JniHelper::jstring2string(stringResult);

        t.env->DeleteLocalRef(stringResult);
        t.env->DeleteLocalRef(t.classID);
        return result;
    }
    return "";
}

CCPoint cocos::getMousePos() {
    return CCPoint(0, 0);
}

namespace {
    void clearJNIException() {
        // this is a silly workaround to not crash when the method is not found.
        // cocos figured this out half a year later...
        auto vm = JniHelper::getJavaVM();

        JNIEnv* env;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) {
            env->ExceptionClear();
        }
    }

    // jni breaks over multithreading, so the value is stored to avoid more jni calls
    std::string s_savedBaseDir = "";

    std::filesystem::path getBaseDir() {
        std::string path = "/storage/emulated/0/Android/data/com.geode.launcher/files";

        if (!s_savedBaseDir.empty()) {
            return std::filesystem::path(s_savedBaseDir);
        }

        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "getBaseDirectory", "()Ljava/lang/String;")) {
            jstring str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
            t.env->DeleteLocalRef(t.classID);
            path = JniHelper::jstring2string(str);
            t.env->DeleteLocalRef(str);
        } else {
            clearJNIException();
        }

        s_savedBaseDir = path;
        return std::filesystem::path(path);
    }
}

std::filesystem::path dirs::getGameDir() {
    return getBaseDir() / "game";
}

std::filesystem::path dirs::getSaveDir() {
    return getBaseDir() / "save";
}

std::filesystem::path dirs::getModRuntimeDir() {
    static std::string cachedResult = [] {
        // incase the jni fails, default to this
        std::string path = "/data/user/0/com.geode.launcher/files/";

        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "getInternalDirectory", "()Ljava/lang/String;")) {
            jstring str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
            t.env->DeleteLocalRef(t.classID);
            path = JniHelper::jstring2string(str);
            t.env->DeleteLocalRef(str);
        } else {
            clearJNIException();
        }

        return path;
    }();
    return std::filesystem::path(cachedResult) / "geode" / "unzipped";
}

void utils::web::openLinkInBrowser(std::string const& url) {
    CCApplication::sharedApplication()->openURL(url.c_str());
}

bool utils::file::openFolder(std::filesystem::path const& path) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "openFolder", "(Ljava/lang/String;)Z")) {
        jstring stringArg1 = t.env->NewStringUTF(path.string().c_str());

        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        return result;
    }
    return false;
}


static utils::MiniFunction<void(std::filesystem::path)> s_fileCallback;
static utils::MiniFunction<void(std::vector<std::filesystem::path>)> s_filesCallback;
static utils::MiniFunction<void()> s_failedCallback;

extern "C"
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_selectFileCallback(
        JNIEnv *env,
        jobject,
        jstring data
) {
    auto isCopy = jboolean();
    auto dataStr = env->GetStringUTFChars(data, &isCopy);

    log::debug("Selected file: {}", dataStr);

    Loader::get()->queueInMainThread([dataStr]() {
        s_fileCallback(dataStr);
    });
}

extern "C"
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_selectFilesCallback(
        JNIEnv *env,
        jobject,
        jobjectArray datas
) {
    auto isCopy = jboolean();
    auto count = env->GetArrayLength(datas);
    auto result = std::vector<std::filesystem::path>();
    for (int i = 0; i < count; i++) {
        auto data = (jstring)env->GetObjectArrayElement(datas, i);
        auto dataStr = env->GetStringUTFChars(data, &isCopy);
        result.push_back(dataStr);

        log::debug("Selected file {}: {}", i, dataStr);
    }

    Loader::get()->queueInMainThread([result]() {
        s_filesCallback(result);
    });
}

extern "C"
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_failedCallback(
        JNIEnv *env,
        jobject
) {
    if (s_failedCallback) {
        Loader::get()->queueInMainThread([]() {
            s_failedCallback();
        });
    }
}

Result<std::filesystem::path> file::pickFile(file::PickMode mode, file::FilePickOptions const& options) {
    return Err("Use the callback version");
}

void file::pickFile(
    PickMode mode, FilePickOptions const& options,
    MiniFunction<void(std::filesystem::path)> callback,
    MiniFunction<void()> failed
) {
    s_fileCallback = callback;
    s_failedCallback = failed;

    std::string method;
    switch (mode) {
        case file::PickMode::OpenFile:
            method = "selectFile";
            break;
        case file::PickMode::SaveFile:
            method = "createFile";
            break;
        case file::PickMode::OpenFolder:
            method = "selectFolder";
            break;
    }

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", method.c_str(), "(Ljava/lang/String;)Z")) {
        jstring stringArg1 = t.env->NewStringUTF(options.defaultPath.value_or(std::filesystem::path()).filename().string().c_str());

        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        if (result) {
            return;
        }
    }
    if (s_failedCallback) {
        Loader::get()->queueInMainThread([]() {
            s_failedCallback();
        });
    }
}

Result<std::vector<std::filesystem::path>> file::pickFiles(file::FilePickOptions const& options) {
    return Err("Use the callback version");
}

void file::pickFiles(
    FilePickOptions const& options,
    MiniFunction<void(std::vector<std::filesystem::path>)> callback,
    MiniFunction<void()> failed
) {
    s_filesCallback = callback;
    s_failedCallback = failed;

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "selectFiles", "(Ljava/lang/String;)Z")) {
        jstring stringArg1 = t.env->NewStringUTF(options.defaultPath.value_or(std::filesystem::path()).string().c_str());

        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        if (result) {
            return;
        }
    }
    if (s_failedCallback) {
        Loader::get()->queueInMainThread([]() {
            s_failedCallback();
        });
    }
}

void geode::utils::game::launchLoaderUninstaller(bool deleteSaveData) {
    log::error("Launching Geode uninstaller is not supported on android");
}

void geode::utils::game::exit() {
    // TODO: yeah
    // if (CCApplication::sharedApplication() &&
    //     (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
    //     log::error("Cannot exit in PlayLayer or LevelEditorLayer!");
    //     return;
    // }
    AppDelegate::get()->trySaveGame(true);
    // AppDelegate::get()->showLoadingCircle(false, true);

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(MenuLayer::endGame)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

void geode::utils::game::restart() {
    // if (CCApplication::sharedApplication() &&
    //     (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
    //     log::error("Cannot restart in PlayLayer or LevelEditorLayer!");
    //     return;
    // }

    class Exit : public CCObject {
    public:
        void restart() {
            JniMethodInfo t;
            if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "restartGame", "()V")) {
                t.env->CallStaticVoidMethod(t.classID, t.methodID);

                t.env->DeleteLocalRef(t.classID);
            }
        }
    };
    // Not implemented
    // log::error("Restarting the game is not implemented on android");

    AppDelegate::get()->trySaveGame(true);
    // AppDelegate::get()->showLoadingCircle(false, true);

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(Exit::restart)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

static const char* permissionToName(Permission permission) {
#define PERM(x) "android.permission." x
#define INTERNAL_PERM(x) "geode.permission_internal." x
    switch (permission) {
    case Permission::RecordAudio: return PERM("RECORD_AUDIO");
    case Permission::ReadAllFiles: return INTERNAL_PERM("MANAGE_ALL_FILES");
    }
#undef PERM
#undef INTERNAL_PERM
}

bool geode::utils::permission::getPermissionStatus(Permission permission) {
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info, "com/geode/launcher/utils/GeodeUtils", "getPermissionStatus", "(Ljava/lang/String;)Z")) {
        jstring permString = info.env->NewStringUTF(permissionToName(permission));
        jboolean result = info.env->CallStaticBooleanMethod(info.classID, info.methodID, permString);
        info.env->DeleteLocalRef(info.classID);
        info.env->DeleteLocalRef(permString);

        return result == JNI_TRUE;
    } else {
        clearJNIException();
    }

    return false;
}

static MiniFunction<void(bool)> s_permissionCallback;

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

void geode::utils::permission::requestPermission(Permission permission, utils::MiniFunction<void(bool)> callback) {
    s_permissionCallback = callback;
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info, "com/geode/launcher/utils/GeodeUtils", "requestPermission", "(Ljava/lang/String;)V")) {
        jstring permString = info.env->NewStringUTF(permissionToName(permission));
        info.env->CallStaticVoidMethod(info.classID, info.methodID, permString);
        info.env->DeleteLocalRef(info.classID);
        info.env->DeleteLocalRef(permString);
    } else {
        clearJNIException();
    }
}

#include "../../utils/thread.hpp"
#include <unistd.h>

std::string geode::utils::thread::getDefaultName() {
    return fmt::format("Thread #{}", gettid());
}

void geode::utils::thread::platformSetName(std::string const& name) {
    pthread_setname_np(pthread_self(), name.c_str());
}
