#pragma once

#include <Geode/utils/VersionInfo.hpp>

namespace about {
    geode::VersionInfo GEODE_DLL getLoaderVersion();
    const char* GEODE_DLL getLoaderVersionStr();
    const char* GEODE_DLL getLoaderCommitHash();
    const char* GEODE_DLL getBindingsCommitHash();
    const char* GEODE_DLL getLoaderModJson();
};
