#pragma once

#include "ModMetadataImpl.hpp"

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/VersionInfo.hpp>

#pragma warning(disable : 4996) // deprecation

using namespace geode::prelude;

namespace geode {
    class [[deprecated]] ModInfo::Impl {
    public:
        ModMetadataImpl m_metadata;
        std::optional<IssuesInfo> m_issues;
        std::vector<Dependency> m_dependencies;
        bool m_supportsDisabling = true;
        bool m_supportsUnloading = false;

        bool operator==(ModInfo::Impl const& other) const;
    };

    class [[deprecated]] ModInfoImpl : public ModInfo::Impl {
    public:
        static ModInfo::Impl& getImpl(ModInfo& info);
    };
}
