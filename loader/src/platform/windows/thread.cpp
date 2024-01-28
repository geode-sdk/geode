#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#include <Geode/Utils.hpp>
#include "thread.hpp"

static thread_local std::string s_threadName;

std::string geode::utils::thread::getName() {
    // only use the thread-local variable here, no need for platform get methods
    if (s_threadName.empty())
        return fmt::format("Thread #{}", GetThreadId(GetCurrentThread()));
    return s_threadName;
}

void geode::utils::thread::setName(std::string const& name) {
    std::wstring wName(name.size(), L' ');
    size_t numConv;
    auto res = mbstowcs_s(&numConv, wName.data(), sizeof(wchar_t) * wName.size(), name.data(), wName.size());
    if (res == EINVAL) {
        log::warn(
            "Native method to set thread name failed (EINVAL), using only fallback methods.");
    }
    else if (res == ERANGE) {
        log::warn(
            "Native method to set thread name failed (ERANGE), using only fallback methods.");
    }
    else {
        platformSetNameWide(wName);
    }

    platformSetNameOrdinary(name);
    s_threadName = name;
}

void geode::utils::thread::setName(std::wstring const& wName) {
    platformSetNameWide(wName);

    std::string name(wName.size(), ' ');
    size_t numConv;
    auto res = wcstombs_s(&numConv, name.data(), name.size(), wName.data(), name.size());
    if (res == EINVAL) {
        log::warn("Fallback method to set thread name failed (EINVAL).");
        return;
    }
    if (res == ERANGE) {
        log::warn("Fallback method to set thread name failed (ERANGE).");
        return;
    }

    platformSetNameOrdinary(name);
    s_threadName = name;
}
