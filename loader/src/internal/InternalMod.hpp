#pragma once

class InternalMod;

#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Interface.hpp>

USE_GEODE_NAMESPACE();

class InternalMod : public Mod {
    protected:
        InternalMod();
        virtual ~InternalMod();

    public:
        static InternalMod* get();
};

template <typename ...Args>
void internal_log(Severity sev, Args... args) { log::log(sev, InternalMod::get(), args...); }