#include <Geode/loader/Hook.hpp>
#include "HookImpl.hpp"

using namespace geode::prelude;

Hook::Hook(std::shared_ptr<Impl>&& impl) : m_impl(std::move(impl)) { m_impl->m_self = this; }
Hook::~Hook() = default;

std::shared_ptr<Hook> Hook::create(
    void* address,
    void* detour,
    std::string const& displayName,
    tulip::hook::HandlerMetadata const& handlerMetadata,
    tulip::hook::HookMetadata const& hookMetadata
) {
    return Impl::create(address, detour, displayName, handlerMetadata, hookMetadata);
}

Mod* Hook::getOwner() const {
    return m_impl->getOwner();
}

bool Hook::isEnabled() const {
    return m_impl->isEnabled();
}

Result<> Hook::enable() {
    return m_impl->enable();
}

Result<> Hook::disable() {
    return m_impl->disable();
}

Result<> Hook::toggle() {
    return m_impl->toggle();
}

Result<> Hook::toggle(bool enable) {
    return m_impl->toggle(enable);
}

bool Hook::getAutoEnable() const {
    return m_impl->getAutoEnable();
}

void Hook::setAutoEnable(bool autoEnable) {
    return m_impl->setAutoEnable(autoEnable);
}

uintptr_t Hook::getAddress() const {
    return m_impl->getAddress();
}

std::string_view Hook::getDisplayName() const {
    return m_impl->getDisplayName();
}

matjson::Value Hook::getRuntimeInfo() const {
    return m_impl->getRuntimeInfo();
}

tulip::hook::HookMetadata Hook::getHookMetadata() const {
    return m_impl->getHookMetadata();
}

void Hook::setHookMetadata(tulip::hook::HookMetadata const& metadata) {
    return m_impl->setHookMetadata(metadata);
}

int32_t Hook::getPriority() const {
    return m_impl->getPriority();
}

void Hook::setPriority(int32_t priority) {
    return m_impl->setPriority(priority);
}
