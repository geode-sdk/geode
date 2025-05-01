#include <Geode/DefaultInclude.hpp>
#include <loader/IPC.hpp>
#include <Geode/loader/Log.hpp>

using namespace geode::prelude;

void ipc::setup() {
    log::debug("IPC is not supported on this platform!");
}
