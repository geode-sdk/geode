#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/ranges.hpp>
#include <vector>
#include "ModImpl.hpp"

USE_GEODE_NAMESPACE();

class Hook::Impl {
public:
    Impl(
        void* address,
        void* detour,
        std::string const& displayName,
        tulip::hook::HandlerMetadata const& handlerMetadata,
        tulip::hook::HookMetadata const& hookMetadata,
        Mod* owner
    );
    ~Impl();

    
    void* m_address;
    void* m_detour;
    std::string m_displayName;
    tulip::hook::HandlerMetadata m_handlerMetadata;
    tulip::hook::HookMetadata m_hookMetadata;
    Mod* m_owner;
    tulip::hook::HookHandle m_handle;
    bool m_enabled = false;
    bool m_autoEnable = true;


    // Used by Mod
    Result<> enable();
    Result<> disable();
    Result<> updateMetadata();

    uintptr_t getAddress() const;
    std::string_view getDisplayName() const;
    bool isEnabled() const;
    Mod* getOwner() const;
    json::Value getRuntimeInfo() const;
    tulip::hook::HookMetadata getHookMetadata() const;
    void setHookMetadata(tulip::hook::HookMetadata const& metadata);
    int32_t getPriority() const;
    void setPriority(int32_t priority);
    bool getAutoEnable() const;
    void setAutoEnable(bool autoEnable);
};