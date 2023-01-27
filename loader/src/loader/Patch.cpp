#include <Geode/loader/Hook.hpp>
#include <json.hpp>

USE_GEODE_NAMESPACE();

bool Patch::apply() {
    return bool(tulip::hook::writeMemory(m_address, m_patch.data(), m_patch.size()));
}

bool Patch::restore() {
    return bool(tulip::hook::writeMemory(m_address, m_original.data(), m_original.size()));
}

template <>
struct json::Serialize<ByteVector> {
    static json::Value to_json(ByteVector const& bytes) {
        return json::Array(bytes.begin(), bytes.end());
    }
};

json::Value Patch::getRuntimeInfo() const {
    auto json = json::Object();
    json["address"] = std::to_string(reinterpret_cast<uintptr_t>(m_address));
    json["original"] = m_original;
    json["patch"] = m_patch;
    json["applied"] = m_applied;
    return json;
}
