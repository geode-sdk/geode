#pragma once

#include "Result.hpp"
#include "general.hpp"

#include <Geode/DefaultInclude.hpp>
#include <fs/filesystem.hpp>
#include <string>
#include <unordered_set>

namespace geode::utils::file {
    GEODE_DLL Result<std::string> readString(ghc::filesystem::path const& path);
    GEODE_DLL Result<byte_array> readBinary(ghc::filesystem::path const& path);

    GEODE_DLL Result<> writeString(ghc::filesystem::path const& path, std::string const& data);
    GEODE_DLL Result<> writeBinary(ghc::filesystem::path const& path, byte_array const& data);

    GEODE_DLL Result<bool> createDirectory(ghc::filesystem::path const& path);
    GEODE_DLL Result<bool> createDirectoryAll(ghc::filesystem::path const& path);
    GEODE_DLL Result<std::vector<std::string>> listFiles(std::string const& path);
    GEODE_DLL Result<std::vector<std::string>> listFilesRecursively(std::string const& path);

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
    };

    /**
     * Unzip file to directory
     * @param from File to unzip
     * @param to Directory to unzip to
     * @returns Ok on success, Error on error
     */
    GEODE_DLL Result<> unzipTo(ghc::filesystem::path const& from, ghc::filesystem::path const& to);

    GEODE_DLL ghc::filesystem::path geodeRoot();
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
