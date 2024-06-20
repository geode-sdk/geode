#pragma once

#include "Result.hpp"
#include "general.hpp"
#include "../loader/Event.hpp"
#include "Task.hpp"

#include <matjson.hpp>
#include <Geode/DefaultInclude.hpp>
#include <filesystem>
#include <string>
#include <unordered_set>

template <>
struct matjson::Serialize<std::filesystem::path> {
    static matjson::Value to_json(std::filesystem::path const& path) {
        return path.string();
    }
    static std::filesystem::path from_json(matjson::Value const& value) {
        return value.as_string();
    }
    static bool is_json(matjson::Value const& value) {
        return value.is_string();
    }
};

namespace geode::utils::file {
    GEODE_DLL Result<std::string> readString(std::filesystem::path const& path);
    GEODE_DLL Result<matjson::Value> readJson(std::filesystem::path const& path);
    GEODE_DLL Result<ByteVector> readBinary(std::filesystem::path const& path);

    template <class T>
    Result<T> readFromJson(std::filesystem::path const& file) {
        GEODE_UNWRAP_INTO(auto json, readJson(file));
        if (!json.template is<T>()) {
            return Err("JSON is not of type {}", typeid(T).name());
        }
        return Ok(json.template as<T>());
    }

    GEODE_DLL Result<> writeString(std::filesystem::path const& path, std::string const& data);
    GEODE_DLL Result<> writeBinary(std::filesystem::path const& path, ByteVector const& data);

    template <class T>
    Result<> writeToJson(std::filesystem::path const& path, T const& data) {
        GEODE_UNWRAP(writeString(path, matjson::Value(data).dump()));
        return Ok();
    }

    GEODE_DLL Result<> createDirectory(std::filesystem::path const& path);
    GEODE_DLL Result<> createDirectoryAll(std::filesystem::path const& path);
    GEODE_DLL Result<std::vector<std::filesystem::path>> readDirectory(
        std::filesystem::path const& path, bool recursive = false
    );

    class Unzip;

    class GEODE_DLL Zip final {
    public:
        using Path = std::filesystem::path;

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

        using Path = std::filesystem::path;

        /**
         * Create unzipper for file
         */
        static Result<Unzip> create(Path const& file);

        /**
         * Create unzipper for data in-memory
         */
        static Result<Unzip> create(ByteVector const& data);

        /**
         * Set a callback to be called with the progress of the unzip operation, first
         * argument is the current entry, second argument is the total entries
         * @note This is not thread-safe
         * @param callback Callback to call with the progress of the unzip operation
         */
        void setProgressCallback(
            utils::MiniFunction<void(uint32_t, uint32_t)> callback
        );

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

        static Result<> intoDir(
            utils::MiniFunction<void(uint32_t, uint32_t)> progressCallback,
            Path const& from,
            Path const& to,
            bool deleteZipAfter = false
        );
    };

    /**
     * Open a folder / file in the system's file explorer
     * @param path Folder / file to open
     */
    GEODE_DLL bool openFolder(std::filesystem::path const& path);

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
        std::optional<std::filesystem::path> defaultPath;
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
    GEODE_DLL Task<Result<std::filesystem::path>> pick(PickMode mode, FilePickOptions const& options);

    /**
     * Prompt the user to pick a bunch of files for opening using the system's file system picker
     * @param options Picker options
     */
    GEODE_DLL Task<Result<std::vector<std::filesystem::path>>> pickMany(FilePickOptions const& options);

    class GEODE_DLL FileWatchEvent final : public Event {
    protected:
        std::filesystem::path m_path;
    
    public:
        FileWatchEvent(std::filesystem::path const& path);
        std::filesystem::path getPath() const;
    };

    class GEODE_DLL FileWatchFilter final : public EventFilter<FileWatchEvent> {
    protected:
        std::filesystem::path m_path;
    
    public:
        using Callback = void(FileWatchEvent*);

        ListenerResult handle(utils::MiniFunction<Callback> callback, FileWatchEvent* event);
        FileWatchFilter(std::filesystem::path const& path);
    };

    /**
     * Watch a file for changes. Whenever the file is modified on disk, a 
     * FileWatchEvent is emitted. Add an EventListener with FileWatchFilter 
     * to catch these events
     * @param file The file to watch
     * @note Watching uses file system equivalence instead of path equivalence, 
     * so different paths that point to the same file will be considered the 
     * same
     */
    GEODE_DLL Result<> watchFile(std::filesystem::path const& file);
    /**
     * Stop watching a file for changes
     * @param file The file to unwatch
     */
    GEODE_DLL void unwatchFile(std::filesystem::path const& file);
}
