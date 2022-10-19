#include <Geode/loader/Hook.hpp>
#include <vector>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/vector.hpp>
#include <Geode/utils/ranges.hpp>
#include <InternalLoader.hpp>

#include <lilac/include/geode/core/hook/hook.hpp>

USE_GEODE_NAMESPACE();

byte_array readMemory(void* address, size_t amount) {
    byte_array ret;
    for (size_t i = 0; i < amount; i++) {
        ret.push_back(*as<uint8_t*>(as<uintptr_t>(address) + i));
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
        return Err<>("Unable to enable patch at " + std::to_string(p->getAddress()));
    }
    m_patches.push_back(p);
    return Ok<Patch*>(p);
}

Result<> Mod::unpatch(Patch* patch) {
    if (patch->restore()) {
        ranges::remove(m_patches, patch);
        delete patch;
        return Ok<>();
    }
    return Err<>("Unable to restore patch!");
}

bool Patch::apply() {
    return lilac::hook::write_memory(
        m_address, m_patch.data(), m_patch.size()
    );
}

bool Patch::restore() {
    return lilac::hook::write_memory(
        m_address, m_original.data(), m_original.size()
    );
}
