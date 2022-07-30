#pragma once

class InternalMod;

#include <Geode/loader/Mod.hpp>

USE_GEODE_NAMESPACE();

class InternalLoader;

class InternalMod : public Mod {
    protected:
        friend class InternalLoade;

        InternalMod();
        virtual ~InternalMod();

    public:
        static InternalMod* get();
};
