#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/ranges.hpp>
#include <vector>
// #include <hook/hook.hpp>
#include "InternalLoader.hpp"
#include "InternalMod.hpp"

#include <Geode/hook-core/Hook.hpp>

USE_GEODE_NAMESPACE();

Result<> Hook::enable() {
    if (!m_enabled) {
        auto res = std::invoke(m_addFunction, m_address);
        if (res) {
            log::debug("Enabling hook at function {}", m_displayName);
            m_enabled = true;
            m_handle = res.unwrap();
            return Ok();
        }
        else {
            return Err(
                "Unable to create hook at " + std::to_string(reinterpret_cast<uintptr_t>(m_address))
            );
        }
        return Err("Hook already has a handle");
    }
    return Ok();
}

Result<> Hook::disable() {
    if (m_enabled) {
        if (!geode::core::hook::remove(m_handle)) return Err("Unable to remove hook");

        log::debug("Disabling hook at function {}", m_displayName);
        m_enabled = false;
    }
    return Ok();
}

nlohmann::json Hook::getRuntimeInfo() const {
    auto json = nlohmann::json::object();
    json["address"] = reinterpret_cast<uintptr_t>(m_address);
    json["detour"] = reinterpret_cast<uintptr_t>(m_detour);
    json["name"] = m_displayName;
    json["enabled"] = m_enabled;
    return json;
}
