#pragma once

#include <Geode/utils/VersionInfo.hpp>

namespace about {
    extern geode::VersionInfo getLoaderVersion();
    extern const char* getLoaderVersionStr();
    extern const char* getLoaderCommitHash();
    extern const char* getBindingsCommitHash();
    extern const char* getLoaderModJson();
};
