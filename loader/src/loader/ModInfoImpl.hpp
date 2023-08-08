#pragma once

#include "ModMetadataImpl.hpp"

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/VersionInfo.hpp>

using namespace geode::prelude;

namespace geode {
    class ModInfo::Impl {
    public:
        ModMetadata::Impl m_metadata;
        std::optional<IssuesInfo> m_issues;
        std::vector<Dependency> m_dependencies;
        bool m_supportsDisabling = true;
        bool m_supportsUnloading = false;

        static Result<ModInfo> createFromGeodeZip(utils::file::Unzip& zip);
        static Result<ModInfo> createFromGeodeFile(ghc::filesystem::path const& path);
        static Result<ModInfo> createFromFile(ghc::filesystem::path const& path);
        static Result<ModInfo> create(ModJson const& json);

        ModJson toJSON() const;
        ModJson getRawJSON() const;

        bool operator==(ModInfo::Impl const& other) const;

        static bool validateID(std::string const& id);

        static Result<ModInfo> createFromSchemaV010(ModJson const& json);

        Result<> addSpecialFiles(ghc::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();
    };

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    class ModInfoImpl {
    public:
        static ModInfo::Impl& getImpl(ModInfo& info);
    };
#pragma clang diagnostic pop
}
