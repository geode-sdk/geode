#pragma once

#include "Result.hpp"
#include "general.hpp"

#include <json.hpp>
#include <Geode/DefaultInclude.hpp>
#include <ghc/fs_fwd.hpp>
#include <string>
#include <unordered_set>

template <>
struct json::Serialize<ghc::filesystem::path> {
    static json::Value to_json(ghc::filesystem::path const& path) {
        return path.string();
    }
    static ghc::filesystem::path from_json(json::Value const& value) {
        return value.as_string();
    }
};

namespace geode::utils::file {
    GEODE_DLL Result<std::string> readString(ghc::filesystem::path const& path);
    GEODE_DLL Result<json::Value> readJson(ghc::filesystem::path const& path);
    GEODE_DLL Result<ByteVector> readBinary(ghc::filesystem::path const& path);

    GEODE_DLL Result<> writeString(ghc::filesystem::path const& path, std::string const& data);
    GEODE_DLL Result<> writeBinary(ghc::filesystem::path const& path, ByteVector const& data);

    GEODE_DLL Result<> createDirectory(ghc::filesystem::path const& path);
    GEODE_DLL Result<> createDirectoryAll(ghc::filesystem::path const& path);
    [[deprecated("Use file::readDirectory")]]
    GEODE_DLL Result<std::vector<ghc::filesystem::path>> listFiles(
        ghc::filesystem::path const& path, bool recursive = false
    );
    GEODE_DLL Result<std::vector<ghc::filesystem::path>> readDirectory(
        ghc::filesystem::path const& path, bool recursive = false
    );

    class Unzip;

    class GEODE_DLL Zip final {
    public:
        using Path = ghc::filesystem::path;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
        
        Zip();
        Zip(std::unique_ptr<Impl>&& impl);

        Result<> addAllFromRecurse(
            Path const& dir, Path const& entry
        );

        // for sharing Impl
        friend class Unzip;
    
    public:
        Zip(Zip const&) = delete;
        Zip(Zip&& other);
        ~Zip();

        /**
         * Create zipper for file
         */
        static Result<Zip> create(Path const& file);

        /**
         * Create zipper for in-memory data
         */
        static Result<Zip> create();

        /**
         * Path to the created zip
         * @returns The path to the zip that is being created, or an empty path 
         * if the zip was opened in memory
         */
        Path getPath() const;

        /**
         * Get the zipped data
         */
        ByteVector getData() const;

        /**
         * Add an entry to the zip with data
         */
        Result<> add(Path const& entry, ByteVector const& data);
        /**
         * Add an entry to the zip with string data
         */
        Result<> add(Path const& entry, std::string const& data);
        /**
         * Add an entry to the zip from a file on disk. If you want to add the 
         * file with a different name, read it into memory first and add it 
         * with Zip::add
         * @param file File on disk
         * @param entryDir Folder to place the file in in the zip
         */
        Result<> addFrom(Path const& file, Path const& entryDir = Path());
        /**
         * Add an entry to the zip from a directory on disk
         * @param entry Path in the zip
         * @param dir Directory on disk
         */
        Result<> addAllFrom(Path const& dir);
        /**
         * Add a folder entry to the zip. If you want to add a folder from disk, 
         * use Zip::addAllFrom
         * @param entry Folder path in zip
         */
        Result<> addFolder(Path const& entry);
    };

    class GEODE_DLL Unzip final {
    private:
        using Impl = Zip::Impl;
        std::unique_ptr<Impl> m_impl;

        Unzip();
        Unzip(std::unique_ptr<Impl>&& impl);

    public:
        Unzip(Unzip const&) = delete;
        Unzip(Unzip&& other);
        ~Unzip();

        using Path = ghc::filesystem::path;

        /**
         * Create unzipper for file
         */
        static Result<Unzip> create(Path const& file);

        /**
         * Create unzipper for data in-memory
         */
        static Result<Unzip> create(ByteVector const& data);

        /**
         * Path to the opened zip
         * @returns The path to the zip that is being read, or an empty path 
         * if the zip was opened in memory
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
        Result<ByteVector> extract(Path const& name);
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

    /**
     * Open a folder / file in the system's file explorer
     * @param path Folder / file to open
     */
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

        /**
         * On PickMode::SaveFile and PickMode::OpenFile, last item is assumed 
         * to be a filename, unless it points to an extant directory.
         * On PickMode::OpenFolder, path is treated as leading up to a directory
         */
        std::optional<ghc::filesystem::path> defaultPath;
        /**
         * File extension filters to show on the file picker
         */
        std::vector<Filter> filters;
    };

    /**
     * Prompt the user to pick a file using the system's file system picker
     * @param mode Type of file selection prompt to show
     * @param options Picker options
     */
    GEODE_DLL Result<ghc::filesystem::path> pickFile(PickMode mode, FilePickOptions const& options);
    /**
     * Prompt the user to pick a bunch of files for opening using the system's file system picker
     * @param options Picker options
     */
    GEODE_DLL Result<std::vector<ghc::filesystem::path>> pickFiles(FilePickOptions const& options);
}
