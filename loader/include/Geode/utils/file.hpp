#pragma once

#include "Result.hpp"
#include "general.hpp"

#include "../external/json/json.hpp"
#include <Geode/DefaultInclude.hpp>
#include <fs/filesystem.hpp>
#include <string>
#include <unordered_set>

namespace geode::utils::file {
    GEODE_DLL Result<std::string> readString(ghc::filesystem::path const& path);
    GEODE_DLL Result<nlohmann::json> readJson(ghc::filesystem::path const& path);
    GEODE_DLL Result<byte_array> readBinary(ghc::filesystem::path const& path);

    GEODE_DLL Result<> writeString(ghc::filesystem::path const& path, std::string const& data);
    GEODE_DLL Result<> writeBinary(ghc::filesystem::path const& path, byte_array const& data);

    GEODE_DLL Result<> createDirectory(ghc::filesystem::path const& path);
    GEODE_DLL Result<> createDirectoryAll(ghc::filesystem::path const& path);
    GEODE_DLL Result<std::vector<ghc::filesystem::path>> listFiles(
        ghc::filesystem::path const& path, bool recursive = false
    );

    class UnzipImpl;

    class GEODE_DLL Unzip final {
    private:
        UnzipImpl* m_impl;

    public:
        Unzip() = delete;
        Unzip(Unzip const&) = delete;
        Unzip(Unzip&& other);
        Unzip(UnzipImpl* impl);
        ~Unzip();

        using Path = ghc::filesystem::path;

        /**
         * Create unzipper for file
         */
        static Result<Unzip> create(Path const& file);

        /**
         * Path to the opened zip
         */
        Path getPath() const;

        /**
         * Get all entries in zip
         */
        std::vector<Path> getEntries() const;
        /**
         * Check if zip has entry
         * @param name Entry path in zip
         */
        bool hasEntry(Path const& name);

        /**
         * Extract entry to memory
         * @param name Entry path in zip
         */
        Result<byte_array> extract(Path const& name);
        /**
         * Extract entry to file
         * @param name Entry path in zip
         * @param path Target file path
         */
        Result<> extractTo(Path const& name, Path const& path);
        /**
         * Extract all entries to directory
         * @param dir Directory to unzip the contents to
         */
        Result<> extractAllTo(Path const& dir);

        /**
         * Helper method for quickly unzipping a file
         * @param from ZIP file to unzip
         * @param to Directory to unzip to
         * @param deleteZipAfter Whether to delete the zip after unzipping
         * @returns Succesful result on success, errorful result on error
         */
        static Result<> intoDir(
            Path const& from,
            Path const& to,
            bool deleteZipAfter = false
        );
    };

    GEODE_DLL bool openFolder(ghc::filesystem::path const& path);

    enum class PickMode {
        OpenFile,
        SaveFile,
        OpenFolder,
    };

    struct FilePickOptions {
        struct Filter {
            // Name of the filter
            std::string description;
            // Extensions (*.txt, *.doc, *.mp3, etc.)
            std::unordered_set<std::string> files;
        };

        ghc::filesystem::path defaultPath;
        std::vector<Filter> filters;
    };

    GEODE_DLL Result<ghc::filesystem::path> pickFile(PickMode mode, FilePickOptions const& options);
    GEODE_DLL Result<std::vector<ghc::filesystem::path>> pickFiles(FilePickOptions const& options);
}
