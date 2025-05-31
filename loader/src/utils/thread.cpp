#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#include <Geode/loader/Loader.hpp> // i don't think i have to label these anymore
#include <Geode/Utils.hpp>
#include "thread.hpp"

static thread_local std::string s_threadName;

std::string geode::utils::thread::getName() {
    // only use the thread-local variable here, no need for platform get methods
    if (s_threadName.empty())
        return getDefaultName();
    return s_threadName;
}

void geode::utils::thread::setName(std::string const& name) {
    s_threadName = name;
    platformSetName(name);
}
