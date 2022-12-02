#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/ranges.hpp>
#include <InternalLoader.hpp>
#include <lilac/include/geode/core/hook/hook.hpp>
#include <vector>

USE_GEODE_NAMESPACE();

byte_array readMemory(void* address, size_t amount) {
    byte_array ret;
    for (size_t i = 0; i < amount; i++) {
        ret.push_back(*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(address) + i));
    }
    return ret;
}

Result<Patch*> Mod::patch(void* address, byte_array data) {
    auto p = new Patch;
    p->m_address = address;
    p->m_original = readMemory(address, data.size());
    p->m_owner = this;
    p->m_patch = data;
    if (!p->apply()) {
        delete p;
        return Err("Unable to enable patch at " + std::to_string(p->getAddress()));
    }
    m_patches.push_back(p);
    return Ok(p);
}

Result<> Mod::unpatch(Patch* patch) {
    if (patch->restore()) {
        ranges::remove(m_patches, patch);
        delete patch;
        return Ok();
    }
    return Err("Unable to restore patch!");
}

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
