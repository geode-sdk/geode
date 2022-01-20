#include <hook.hpp>
#include "impl.hpp"

using namespace geode::core;
using namespace geode::core::hook;

hook::Handle GEODE_CALL geode::core::hook::add(const void* address, const void* detour) {
    return HookManager::add_hook(address, detour);
}

bool GEODE_CALL geode::core::hook::remove(hook::Handle handle) {
    return HookManager::remove_hook(handle);
}

bool GEODE_CALL geode::core::hook::write_memory(void* address, void* data, size_t size) {
    return TargetPlatform::write_memory(address, data, size);
}

bool GEODE_CALL geode::core::hook::read_memory(void* address, void* receive, size_t size) {
    return TargetPlatform::read_memory(address, receive, size);
}
