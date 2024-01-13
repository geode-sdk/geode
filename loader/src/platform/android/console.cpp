﻿#include <loader/console.hpp>
#include <iostream>
#include <Geode/loader/Log.hpp>
#include <android/log.h>

using namespace geode::prelude;

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

void console::open() {
    return;
}

void console::close() {
    return;
}

void console::log(std::string const& msg, Severity severity) {
    __android_log_print(
        getLogSeverityForSeverity(severity),
        "Geode",
        "%s",
        msg.c_str()
    );
}

void console::messageBox(char const* title, std::string const& info, Severity severity) {
    cocos2d::CCMessageBox(info.c_str(), title);
}
