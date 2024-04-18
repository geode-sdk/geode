#include "PatchImpl.hpp"

#include <utility>
#include "LoaderImpl.hpp"

Patch::Impl::Impl(void* address, ByteVector original, ByteVector patch) :
    m_address(address),
    m_original(std::move(original)),
    m_patch(std::move(patch)) {}
Patch::Impl::~Impl() {
    if (m_enabled) {
        auto res = this->disable();
        if (!res) {
            log::error("Failed to disable patch: {}", res.unwrapErr());
        }
    }
    if (m_owner) {
        auto res = m_owner->disownPatch(m_self);
        if (!res) {
            log::error("Failed to disown patch: {}", res.unwrapErr());
        }
    }
}

// TODO: replace this with a safe one
static ByteVector readMemory(void* address, size_t amount) {
    ByteVector ret;
    for (size_t i = 0; i < amount; i++) {
        ret.push_back(*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(address) + i));
    }
    return ret;
}

std::shared_ptr<Patch> Patch::Impl::create(void* address, const geode::ByteVector& patch) {
    auto impl = std::make_shared<Impl>(
        address, readMemory(address, patch.size()), patch
    );
    return std::shared_ptr<Patch>(new Patch(std::move(impl)), [](Patch* patch) {
        delete patch;
    });
}

std::vector<Patch::Impl*>& Patch::Impl::allEnabled() {
    static std::vector<Patch::Impl*> vec;
    return vec;
}

Result<> Patch::Impl::enable() {
    auto const thisMin = this->getAddress();
    auto const thisMax = this->getAddress() + this->m_patch.size() - 1;
    // TODO: this feels slow. can be faster
    for (const auto& other : allEnabled()) {
        auto const otherMin = other->getAddress();
        auto const otherMax = other->getAddress() + other->m_patch.size() - 1;
        bool intersects = (thisMin >= otherMin && thisMin <= otherMax) || (thisMax >= otherMin && thisMax <= otherMax);
        if (!intersects)
            continue;
        return Err(
            "Failed to enable patch: overlaps patch at {} from {}",
            other->m_address, other->getOwner()->getID()
        );
    }
    auto res = tulip::hook::writeMemory(m_address, m_patch.data(), m_patch.size());
    if (!res) return Err("Failed to enable patch: {}", res.unwrapErr());
    m_enabled = true;
    allEnabled().push_back(this);
    return Ok();
}

Result<> Patch::Impl::disable() {
    auto res = tulip::hook::writeMemory(m_address, m_original.data(), m_original.size());
    if (!res) return Err("Failed to disable patch: {}", res.unwrapErr());

    m_enabled = false;
    auto it = std::find(allEnabled().begin(), allEnabled().end(), this);

    if (it == allEnabled().end()) {
        return Err("Failed to disable patch: patch is already disabled");
    }

    allEnabled().erase(it);
    return Ok();
}

ByteVector const& Patch::Impl::getBytes() const {
    return m_patch;
}

Result<> Patch::Impl::updateBytes(const ByteVector& bytes) {
    m_patch = bytes;

    if (m_enabled) {
        auto res = this->disable();
        if (!res) return Err("Failed to update patch: {}", res.unwrapErr());
        res = this->enable();
        if (!res) return Err("Failed to update patch: {}", res.unwrapErr());
    }

    return Ok();
}

uintptr_t Patch::Impl::getAddress() const {
    return reinterpret_cast<uintptr_t>(m_address);
}

matjson::Value Patch::Impl::getRuntimeInfo() const {
    auto json = matjson::Object();
    json["address"] = std::to_string(reinterpret_cast<uintptr_t>(m_address));
    json["original"] = m_original;
    json["patch"] = m_patch;
    json["enabled"] = m_enabled;
    return json;
}
