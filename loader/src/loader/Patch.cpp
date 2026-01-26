#include <Geode/loader/Hook.hpp>
#include "PatchImpl.hpp"

using namespace geode::prelude;

Patch::Patch(std::shared_ptr<Impl>&& impl) : m_impl(std::move(impl)) { m_impl->m_self = this; }
Patch::~Patch() = default;

std::shared_ptr<Patch> Patch::create(void* address, ByteSpan patch) {
    return Impl::create(address, patch);
}

Mod* Patch::getOwner() const {
    return m_impl->getOwner();
}

bool Patch::isEnabled() const {
    return m_impl->isEnabled();
}

Result<> Patch::enable() {
    return m_impl->enable();
}

Result<> Patch::disable() {
    return m_impl->disable();
}

Result<> Patch::toggle() {
    return m_impl->toggle();
}

Result<> Patch::toggle(bool enable) {
    return m_impl->toggle(enable);
}

bool Patch::getAutoEnable() const {
    return m_impl->getAutoEnable();
}

void Patch::setAutoEnable(bool autoEnable) {
    return m_impl->setAutoEnable(autoEnable);
}

ByteVector const& Patch::getBytes() const {
    return m_impl->getBytes();
}

Result<> Patch::updateBytes(ByteSpan bytes) {
    return m_impl->updateBytes(bytes);
}

uintptr_t Patch::getAddress() const {
    return m_impl->getAddress();
}

matjson::Value Patch::getRuntimeInfo() const {
    return m_impl->getRuntimeInfo();
}
