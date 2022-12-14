#pragma once

class InternalMod;

#include <Geode/loader/Mod.hpp>

USE_GEODE_NAMESPACE();

class InternalMod : public Mod {
public:
    static Mod* get();
};