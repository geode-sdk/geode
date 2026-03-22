#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#include <Geode/loader/Loader.hpp> // i don't think i have to label these anymore
#include <Geode/Utils.hpp>
#include "thread.hpp"

static thread_local std::string s_threadName;

ZStringView geode::utils::thread::getName() {
    // only use the thread-local variable here, no need for platform get methods
    if (s_threadName.empty()) {
        s_threadName = getDefaultName();
    }
    return s_threadName;
}

void geode::utils::thread::setName(std::string name) {
    s_threadName = std::move(name);
    platformSetName(s_threadName);
}
