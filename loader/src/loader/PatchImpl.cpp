#include "PatchImpl.hpp"

#include <utility>
#include "LoaderImpl.hpp"

std::string HookError::toString() const {
    switch (m_value) {
        case Unknown:
            return "Unknown error";
        case TulipHook:
            return "Tulip hook error: " + m_error.toString();
        case Overlaps:
            if (m_other) return fmt::format("Overlaps patch {} at {}", m_other->getAddress(), m_other->getOwner()->getID());
            return "Overlaps patch";
        case AlreadyOwned:
            return "The mod patch is already owned";
    }
    return "";
}

Patch::Impl::Impl(void* address, ByteSpan original, ByteSpan patch) :
    m_address(address),
    m_original(original.begin(), original.end()),
    m_patch(patch.begin(), patch.end()) {}
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

std::shared_ptr<Patch> Patch::Impl::create(void* address, ByteSpan patch) {
    auto vec = readMemory(address, patch.size());
    auto impl = std::make_shared<Impl>(
        address, vec, patch
    );
    return std::shared_ptr<Patch>(new Patch(std::move(impl)), [](Patch* patch) {
        delete patch;
    });
}

std::vector<Patch::Impl*>& Patch::Impl::allEnabled() {
    static std::vector<Patch::Impl*> vec;
    return vec;
}

Result<void, HookError> Patch::Impl::enable() {
    if (m_enabled) {
        return Ok();
    }

    auto const thisMin = this->getAddress();
    auto const thisMax = this->getAddress() + this->m_patch.size() - 1;
    // TODO: this feels slow. can be faster
    for (const auto& other : allEnabled()) {
        auto const otherMin = other->getAddress();
        auto const otherMax = other->getAddress() + other->m_patch.size() - 1;
        bool intersects = (thisMin >= otherMin && thisMin <= otherMax) || (thisMax >= otherMin && thisMax <= otherMax);
        if (!intersects)
            continue;
        return Err(other->m_self);
    }
    GEODE_UNWRAP(tulip::hook::writeMemory(m_address, m_patch.data(), m_patch.size()));
    m_enabled = true;
    allEnabled().push_back(this);
    return Ok();
}

Result<void, HookError> Patch::Impl::disable() {
    if (!m_enabled) {
        return Ok();
    }

    GEODE_UNWRAP(tulip::hook::writeMemory(m_address, m_original.data(), m_original.size()));

    m_enabled = false;
    auto it = std::find(allEnabled().begin(), allEnabled().end(), this);

    if (it != allEnabled().end()) {
        allEnabled().erase(it);
    }
    return Ok();
}

Result<void, HookError> Patch::Impl::toggle() {
    return this->toggle(!m_enabled);
}

Result<void, HookError> Patch::Impl::toggle(bool enable) {
    if (enable) {
        return this->enable();
    }
    else {
        return this->disable();
    }
}

ByteVector const& Patch::Impl::getBytes() const {
    return m_patch;
}

Result<void, HookError> Patch::Impl::updateBytes(ByteSpan bytes) {
    m_patch = {bytes.begin(), bytes.end()};

    if (m_enabled) {
        GEODE_UNWRAP(this->disable());
        GEODE_UNWRAP(this->enable());
    }

    return Ok();
}

uintptr_t Patch::Impl::getAddress() const {
    return reinterpret_cast<uintptr_t>(m_address);
}

matjson::Value Patch::Impl::getRuntimeInfo() const {
    auto json = matjson::Value::object();
    json["address"] = std::to_string(reinterpret_cast<uintptr_t>(m_address));
    json["original"] = m_original;
    json["patch"] = m_patch;
    json["enabled"] = m_enabled;
    return json;
}
