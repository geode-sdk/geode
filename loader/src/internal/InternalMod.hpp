#pragma once

class InternalMod;

#include <Geode/loader/Mod.hpp>

USE_GEODE_NAMESPACE();

class InternalMod : public Mod {
    protected:
        InternalMod();
        virtual ~InternalMod();

    public:
        static InternalMod* get();
};