#include <Geode/loader/Hook.hpp>
#include <json.hpp>

using namespace geode::prelude;

bool Patch::apply() {
    return bool(tulip::hook::writeMemory(m_address, m_patch.data(), m_patch.size()));
}

bool Patch::restore() {
    return bool(tulip::hook::writeMemory(m_address, m_original.data(), m_original.size()));
}

Patch::Patch() : m_owner(nullptr), m_address(nullptr), m_applied(false), m_autoEnable(true) {}

void Patch::setAutoEnable(bool autoEnable) {
    m_autoEnable = autoEnable;
}

bool Patch::getAutoEnable() const {
    return m_autoEnable;
}

uintptr_t Patch::getAddress() const {
    return reinterpret_cast<uintptr_t>(m_address);
}

bool Patch::isApplied() const {
    return m_applied;
}

Mod* Patch::getOwner() const {
    return m_owner;
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
