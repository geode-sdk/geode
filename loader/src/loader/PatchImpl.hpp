#pragma once

#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include "ModImpl.hpp"
#include "ModPatch.hpp"

using namespace geode::prelude;

class Patch::Impl final : ModPatch {
public:
    Impl(void* address, ByteSpan original, ByteSpan patch);
    ~Impl();

    static std::shared_ptr<Patch> create(void* address, ByteSpan patch);
    static std::vector<Patch::Impl*>& allEnabled();

    Patch* m_self = nullptr;
    void* m_address;
    ByteVector m_original;
    ByteVector m_patch;

    Result<void, HookError> enable();
    Result<void, HookError> disable();
    Result<void, HookError> toggle();
    Result<void, HookError> toggle(bool enable);

    ByteVector const& getBytes() const;
    Result<void, HookError> updateBytes(ByteSpan bytes);

    uintptr_t getAddress() const;
    matjson::Value getRuntimeInfo() const;

    friend class Patch;
    friend class Mod;
};
