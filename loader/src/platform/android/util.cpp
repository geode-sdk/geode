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

geode::Result<ghc::filesystem::path> utils::file::
    pickFile(geode::utils::file::PickMode, geode::utils::file::FilePickOptions const&) {
    return geode::Err("This function is currently unimplemented");
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
