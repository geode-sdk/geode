#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Log.hpp>
#include <loader/ModImpl.hpp>
#include <iostream>
#include <loader/LoaderImpl.hpp>
#include <Geode/utils/string.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_ANDROID

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

void Loader::Impl::platformMessageBox(char const* title, std::string const& info) {
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
