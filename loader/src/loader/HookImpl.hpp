#pragma once

#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/ranges.hpp>
#include <vector>
#include "ModImpl.hpp"
#include "ModPatch.hpp"

using namespace geode::prelude;

class Hook::Impl final : ModPatch {
public:
    Impl(
        void* address,
        void* detour,
        std::string displayName,
        tulip::hook::HandlerMetadata handlerMetadata,
        tulip::hook::HookMetadata const& hookMetadata
    );
    ~Impl();

    static std::shared_ptr<Hook> create(
        void* address,
        void* detour,
        std::string const& displayName,
        tulip::hook::HandlerMetadata const& handlerMetadata,
        tulip::hook::HookMetadata const& hookMetadata
    );

    template<class DetourType>
    static std::shared_ptr<Hook> create(
        void* address,
        DetourType detour,
        std::string const& displayName,
        tulip::hook::TulipConvention convention,
        tulip::hook::HookMetadata const& hookMetadata
    );

    Hook* m_self = nullptr;
    void* m_address;
    void* m_detour;
    std::string m_displayName;
    tulip::hook::HandlerMetadata m_handlerMetadata;
    tulip::hook::HookMetadata m_hookMetadata;
    tulip::hook::HookHandle m_handle = 0;

    Result<> enable();
    Result<> disable();
    Result<> toggle();
    Result<> toggle(bool enable);

    uintptr_t getAddress() const;
    std::string_view getDisplayName() const;
    matjson::Value getRuntimeInfo() const;
    tulip::hook::HookMetadata getHookMetadata() const;
    void setHookMetadata(tulip::hook::HookMetadata const& metadata);
    int32_t getPriority() const;
    void setPriority(int32_t priority);

    Result<> updateHookMetadata();

    friend class Hook;
    friend class Mod;
};
