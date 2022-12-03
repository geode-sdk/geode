#include <Geode/loader/Hook.hpp>
#include <lilac/include/geode/core/hook/hook.hpp>

USE_GEODE_NAMESPACE();

bool Patch::apply() {
    return lilac::hook::write_memory(m_address, m_patch.data(), m_patch.size());
}

bool Patch::restore() {
    return lilac::hook::write_memory(m_address, m_original.data(), m_original.size());
}

nlohmann::json Patch::getRuntimeInfo() const {
    auto json = nlohmann::json::object();
    json["address"] = reinterpret_cast<uintptr_t>(m_address);
    json["original"] = m_original;
    json["patch"] = m_patch;
    json["applied"] = m_applied;
    return json;
}
