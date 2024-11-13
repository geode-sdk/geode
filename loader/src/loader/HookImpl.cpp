#include "HookImpl.hpp"

#include <utility>
#include "LoaderImpl.hpp"

Hook::Impl::Impl(
    void* address,
    void* detour,
    std::string displayName,
    tulip::hook::HandlerMetadata handlerMetadata,
    tulip::hook::HookMetadata const& hookMetadata) :
    m_address(address),
    m_detour(detour),
    m_displayName(std::move(displayName)),
    m_handlerMetadata(std::move(handlerMetadata)),
    m_hookMetadata(hookMetadata) {}
Hook::Impl::~Impl() {
    if (m_enabled) {
        auto res = this->disable();
        if (!res) {
            log::error("Failed to disable hook: {}", res.unwrapErr());
        }
    }
    if (m_owner) {
        auto res = m_owner->disownHook(m_self);
        if (!res) {
            log::error("Failed to disown hook: {}", res.unwrapErr());
        }
    }
}

std::shared_ptr<Hook> Hook::Impl::create(
    void* address,
    void* detour,
    std::string const& displayName,
    tulip::hook::HandlerMetadata const& handlerMetadata,
    tulip::hook::HookMetadata const& hookMetadata
) {
    auto impl = std::make_shared<Impl>(
        address, detour, displayName, handlerMetadata, hookMetadata
    );
    return std::shared_ptr<Hook>(new Hook(std::move(impl)), [](Hook* hook) {
        delete hook;
    });
}

Result<> Hook::Impl::enable() {
    if (m_enabled) {
        return Ok();
    }

    // During a transition between updates when it's important to get a
    // non-functional version that compiles, address 0x9999999 is used to mark
    // functions not yet RE'd but that would prevent compilation
    if ((uintptr_t)m_address == (geode::base::get() + 0x9999999)) {
        if (m_owner) {
            log::warn(
                "Hook {} for {} uses placeholder address, refusing to hook",
                m_displayName, m_owner->getID()
            );
        }
        else {
            log::warn("Hook {} uses placeholder address, refusing to hook", m_displayName);
        }
        return Ok();
    }

    GEODE_UNWRAP_INTO(auto handler, LoaderImpl::get()->getOrCreateHandler(m_address, m_handlerMetadata));
    m_handle = tulip::hook::createHook(handler, m_detour, m_hookMetadata);
    m_enabled = true;

    if (m_owner) {
        log::debug("Enabled {} hook at {} for {}", m_displayName, m_address, m_owner->getID());
    }
    else {
        log::debug("Enabled {} hook at {}", m_displayName, m_address);
    }

    return Ok();
}

Result<> Hook::Impl::disable() {
    if (!m_enabled)
        return Ok();
    GEODE_UNWRAP_INTO(auto handler, LoaderImpl::get()->getHandler(m_address));
    tulip::hook::removeHook(handler, m_handle);
    m_enabled = false;
    log::debug("Disabled {} hook", m_displayName);
    return Ok();
}

uintptr_t Hook::Impl::getAddress() const {
    return reinterpret_cast<uintptr_t>(m_address);
}

std::string_view Hook::Impl::getDisplayName() const {
    return m_displayName;
}

matjson::Value Hook::Impl::getRuntimeInfo() const {
    matjson::Value json;
    json["address"] = std::to_string(reinterpret_cast<uintptr_t>(m_address));
    json["detour"] = std::to_string(reinterpret_cast<uintptr_t>(m_detour));
    json["name"] = m_displayName;
    json["enabled"] = m_enabled;
    return json;
}

tulip::hook::HookMetadata Hook::Impl::getHookMetadata() const {
    return m_hookMetadata;
}

void Hook::Impl::setHookMetadata(tulip::hook::HookMetadata const& metadata) {
    m_hookMetadata = metadata;
    auto res = this->updateHookMetadata();
    if (!res) {
        log::error("Failed to update hook metadata: {}", res.unwrapErr());
    }
}

int32_t Hook::Impl::getPriority() const {
    return m_hookMetadata.m_priority;
}

void Hook::Impl::setPriority(int32_t priority) {
    m_hookMetadata.m_priority = priority;
    auto res = this->updateHookMetadata();
    if (!res) {
        log::error("Failed to update hook priority: {}", res.unwrapErr());
    }
}

Result<> Hook::Impl::updateHookMetadata() {
    if (!m_enabled) return Ok();
    GEODE_UNWRAP_INTO(auto handler, LoaderImpl::get()->getHandler(m_address));
    tulip::hook::updateHookMetadata(handler, m_handle, m_hookMetadata);
    return Ok();
}
