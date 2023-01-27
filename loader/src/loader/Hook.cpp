#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/ranges.hpp>
#include <vector>
#include "ModImpl.hpp"
#include "HookImpl.hpp"

USE_GEODE_NAMESPACE();

Hook::Hook(std::shared_ptr<Impl>&& impl) : m_impl(std::move(impl)) {}
Hook::~Hook() {}

Hook* Hook::create(
    Mod* owner,
    void* address,
    void* detour,
    std::string const& displayName,
    tulip::hook::HandlerMetadata const& handlerMetadata,
    tulip::hook::HookMetadata const& hookMetadata
) {
    auto impl = std::make_shared<Hook::Impl>(
        address, detour, displayName, handlerMetadata, hookMetadata, owner
    );
    return new Hook(std::move(impl));
}

uintptr_t Hook::getAddress() const {
    return m_impl->getAddress();
}

std::string_view Hook::getDisplayName() const {
    return m_impl->getDisplayName();
}

bool Hook::isEnabled() const {
    return m_impl->isEnabled();
}

Mod* Hook::getOwner() const {
    return m_impl->getOwner();
}

json::Value Hook::getRuntimeInfo() const {
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

bool Hook::getAutoEnable() const {
    return m_impl->getAutoEnable();
}

void Hook::setAutoEnable(bool autoEnable) {
    return m_impl->setAutoEnable(autoEnable);
}

Result<> Hook::enable() {
    return m_impl->enable();
}
Result<> Hook::disable() {
    return m_impl->disable();
}