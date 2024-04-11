#pragma once

#include <Geode/utils/VersionInfo.hpp>

namespace about {
    GEODE_DLL geode::VersionInfo getLoaderVersion();
    GEODE_DLL const char* getLoaderVersionStr();
    GEODE_DLL const char* getLoaderCommitHash();
    GEODE_DLL const char* getBindingsCommitHash();
    GEODE_DLL const char* getLoaderModJson();
};
