#include "HookImpl.hpp"
#include "LoaderImpl.hpp"

Hook::Impl::Impl(void* address, void* detour, std::string const& displayName, tulip::hook::HandlerMetadata const& handlerMetadata, tulip::hook::HookMetadata const& hookMetadata, Mod* owner) :
    m_address(address),
    m_detour(detour),
    m_displayName(displayName),
    m_handlerMetadata(handlerMetadata),
    m_hookMetadata(hookMetadata),
    m_owner(owner),
    m_enabled(false),
    m_autoEnable(true) {}
Hook::Impl::~Impl() {
    if (m_enabled) {
        auto res = this->disable();
        if (!res) {
            log::error("Failed to disable hook: {}", res.unwrapErr());
        }
    }
}

uintptr_t Hook::Impl::getAddress() const {
    return reinterpret_cast<uintptr_t>(m_address);
}
std::string_view Hook::Impl::getDisplayName() const {
    return m_displayName;
}
bool Hook::Impl::isEnabled() const {
    return m_enabled;
}
Mod* Hook::Impl::getOwner() const {
    return m_owner;
}
matjson::Value Hook::Impl::getRuntimeInfo() const {
    auto json = matjson::Object();
    json["address"] = std::to_string(reinterpret_cast<uintptr_t>(m_address));
    json["detour"] = std::to_string(reinterpret_cast<uintptr_t>(m_detour));
    json["name"] = m_displayName;
    json["enabled"] = m_enabled;
    return json;
}
tulip::hook::HookMetadata Hook::Impl::getHookMetadata() const {
    return m_hookMetadata;
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
            log::debug(
                "Hook {} for {} uses placeholder address, refusing to hook",
                m_displayName, m_owner->getID()
            );
        }
        else {
            log::debug("Hook {} uses placeholder address, refusing to hook", m_displayName);
        }
        return Ok();
    }

    if (!LoaderImpl::get()->hasHandler(m_address)) {
        GEODE_UNWRAP(LoaderImpl::get()->createHandler(m_address, m_handlerMetadata));
    }
    GEODE_UNWRAP_INTO(auto handler, LoaderImpl::get()->getHandler(m_address));

    m_handle = tulip::hook::createHook(handler, m_detour, m_hookMetadata);
    if (m_owner) {
        log::debug("Enabled {} hook at {} for {}", m_displayName, m_address, m_owner->getID());
    }
    else {
        log::debug("Enabled {} hook at {}", m_displayName, m_address);
    }

    m_enabled = true;
    return Ok();
}

Result<> Hook::Impl::disable() {
    if (!m_enabled)
        return Ok();

    GEODE_UNWRAP_INTO(auto handler, LoaderImpl::get()->getHandler(m_address));

    tulip::hook::removeHook(handler, m_handle);

    log::debug("Disabled {} hook", m_displayName);

    m_enabled = false;
    return Ok();
}

Result<> Hook::Impl::updateMetadata() {
    if (m_enabled) {
        GEODE_UNWRAP_INTO(auto handler, LoaderImpl::get()->getHandler(m_address));

        tulip::hook::updateHookMetadata(handler, m_handle, m_hookMetadata);
    }
    return Ok();
}
void Hook::Impl::setHookMetadata(tulip::hook::HookMetadata const& metadata) {
    m_hookMetadata = metadata;
    auto res = this->updateMetadata();
    if (!res) {
        log::error("Failed to update hook metadata: {}", res.unwrapErr());
    }
}
int32_t Hook::Impl::getPriority() const {
    return m_hookMetadata.m_priority;
}
void Hook::Impl::setPriority(int32_t priority) {
    m_hookMetadata.m_priority = priority;
    auto res = this->updateMetadata();
    if (!res) {
        log::error("Failed to update hook priority: {}", res.unwrapErr());
    }
}

bool Hook::Impl::getAutoEnable() const {
    return m_autoEnable;
}

void Hook::Impl::setAutoEnable(bool autoEnable) {
    m_autoEnable = autoEnable;
}
