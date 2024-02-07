#include <Geode/DefaultInclude.hpp>

#include "../load.hpp"
#include <jni.h>
#include "internalString.hpp"
#include <cocos2d.h>
#include <Geode/loader/Log.hpp>
#include <ghc/fs_fwd.hpp>
#include <Geode/loader/Dirs.hpp>

using namespace geode::prelude;

// idk where to put this
#include <EGL/egl.h>
PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOESEXT = 0;
PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOESEXT = 0;
PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOESEXT = 0;

extern "C" [[gnu::visibility("default")]] jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    glGenVertexArraysOESEXT = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress("glGenVertexArraysOES");
    glBindVertexArrayOESEXT = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress("glBindVertexArrayOES");
    glDeleteVertexArraysOESEXT = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress("glDeleteVertexArraysOES");

    auto updatePath = geode::dirs::getGameDir() / "update";
    std::error_code ec;
    ghc::filesystem::remove_all(updatePath, ec);
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

    geodeEntry(nullptr);
    return JNI_VERSION_1_6;
}

extern "C" [[gnu::visibility("default")]] void emptyFunction(void*) {
    // empty
}
