#pragma once

#include <Geode/utils/VersionInfo.hpp>

namespace about {
    geode::VersionInfo getLoaderVersion();
    const char* getLoaderVersionStr();
    const char* getLoaderCommitHash();
    const char* getBindingsCommitHash();
    const char* getLoaderModJson();
};
