#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_ANDROID

using namespace geode::prelude;

#include <Geode/utils/cocos.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/web.hpp>
#include <ghc/filesystem.hpp>

#include <jni.h>
#include <Geode/cocos/platform/android/jni/JniHelper.h>

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

ghc::filesystem::path dirs::getGameDir() {
    return ghc::filesystem::path(
        "/storage/emulated/0/Android/data/com.geode.launcher/files/game"
        // "/data/user/0/com.geode.launcher/files/"
        /*CCFileUtils::sharedFileUtils()->getWritablePath().c_str()*/
    );
}

ghc::filesystem::path dirs::getSaveDir() {
    return ghc::filesystem::path(
        "/storage/emulated/0/Android/data/com.geode.launcher/files/save"
        // "/data/user/0/com.geode.launcher/files/"
        /*CCFileUtils::sharedFileUtils()->getWritablePath().c_str()*/
    );
}

ghc::filesystem::path dirs::getModRuntimeDir() {
    return ghc::filesystem::path(
        "/data/user/0/com.geode.launcher/files/geode/unzipped"
    );
    // return dirs::getGeodeDir() / "unzipped";
}

void utils::web::openLinkInBrowser(std::string const& url) {
    CCApplication::sharedApplication()->openURL(url.c_str());
}

bool utils::file::openFolder(ghc::filesystem::path const& path) {
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


static utils::MiniFunction<void(ghc::filesystem::path)> s_fileCallback;
static utils::MiniFunction<void(std::vector<ghc::filesystem::path>)> s_filesCallback;
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

    s_fileCallback(dataStr);
}

extern "C"
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_selectFilesCallback(
        JNIEnv *env,
        jobject,
        jobjectArray datas
) {
    auto isCopy = jboolean();
    auto count = env->GetArrayLength(datas);
    auto result = std::vector<ghc::filesystem::path>();
    for (int i = 0; i < count; i++) {
        auto data = (jstring)env->GetObjectArrayElement(datas, i);
        auto dataStr = env->GetStringUTFChars(data, &isCopy);
        result.push_back(dataStr);

        log::debug("Selected file {}: {}", i, dataStr);
    }

    s_filesCallback(result);
}

extern "C"
JNIEXPORT void JNICALL Java_com_geode_launcher_utils_GeodeUtils_failedCallback(
        JNIEnv *env,
        jobject
) {
    if (s_failedCallback) s_failedCallback();
}

Result<ghc::filesystem::path> file::pickFile(file::PickMode mode, file::FilePickOptions const& options) {
    return Err("Use the callback version");
}

void file::pickFile(
    PickMode mode, FilePickOptions const& options,
    MiniFunction<void(ghc::filesystem::path)> callback,
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
        jstring stringArg1 = t.env->NewStringUTF(options.defaultPath.value_or(ghc::filesystem::path()).string().c_str());

        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        if (result) {
            return;
        }
    }
    if (s_failedCallback) s_failedCallback();
}

Result<std::vector<ghc::filesystem::path>> file::pickFiles(file::FilePickOptions const& options) {
    return Err("Use the callback version");
}

void file::pickFiles(
    FilePickOptions const& options,
    MiniFunction<void(std::vector<ghc::filesystem::path>)> callback,
    MiniFunction<void()> failed
) {
    s_filesCallback = callback;
    s_failedCallback = failed;

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "selectFiles", "(Ljava/lang/String;)Z")) {
        jstring stringArg1 = t.env->NewStringUTF(options.defaultPath.value_or(ghc::filesystem::path()).string().c_str());

        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        if (result) {
            return;
        }
    }
    if (s_failedCallback) s_failedCallback();
}

void geode::utils::game::launchLoaderUninstaller(bool deleteSaveData) {
    log::error("Launching Geode uninstaller is not supported on android");
}

void geode::utils::game::exit() {
    if (CCApplication::sharedApplication() &&
        (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
        log::error("Cannot exit in PlayLayer or LevelEditorLayer!");
        return;
    }
    AppDelegate::get()->trySaveGame();
    // AppDelegate::get()->showLoadingCircle(false, true);

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(MenuLayer::endGame)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

void geode::utils::game::restart() {
    if (CCApplication::sharedApplication() &&
        (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
        log::error("Cannot restart in PlayLayer or LevelEditorLayer!");
        return;
    }

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

    AppDelegate::get()->trySaveGame();
    // AppDelegate::get()->showLoadingCircle(false, true);

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(Exit::restart)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

#endif
