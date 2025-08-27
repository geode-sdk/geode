#include <Geode/loader/Loader.hpp> // a third great circular dependency fix
#include <Geode/loader/Log.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/string.hpp>
#include <matjson.hpp>
#include <fstream>
#include <mz.h>
#include <mz_os.h>
#include <mz_strm.h>
#include <mz_strm_os.h>
#include <mz_strm_mem.h>
#include <mz_zip.h>
#include <internal/FileWatcher.hpp>
#include <Geode/utils/ranges.hpp>

#ifdef GEODE_IS_WINDOWS
#include <filesystem>
#endif

#if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS)
struct path_hash_t {
    std::size_t operator()(std::filesystem::path const& path) const noexcept {
        return std::filesystem::hash_value(path);
    }
};
#else
using path_hash_t = std::hash<std::filesystem::path>;
#endif

using namespace geode::prelude;
using namespace geode::utils::file;

Result<std::string> utils::file::readString(std::filesystem::path const& path) {
    std::error_code ec;

    if (!std::filesystem::exists(path, ec) || ec)
        return Err("File does not exist");

    std::ifstream in(path, std::ios::in | std::ios::binary);

    if (!in)
        return Err("Unable to open file");

    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize((const size_t)in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return Ok(contents);
}

Result<matjson::Value> utils::file::readJson(std::filesystem::path const& path) {
    auto str = GEODE_UNWRAP(utils::file::readString(path));
    return matjson::parse(str).mapErr([&](auto const& err) {
        return fmt::format("Unable to parse JSON: {}", err);
    });
}

Result<ByteVector> utils::file::readBinary(std::filesystem::path const& path) {
    std::error_code ec;

    if (!std::filesystem::exists(path, ec) || ec)
        return Err("File does not exist");

    std::ifstream in(path, std::ios::in | std::ios::binary);

    if (!in)
        return Err("Unable to open file");

    return Ok(ByteVector(std::istreambuf_iterator<char>(in), {}));
}

Result<> utils::file::writeString(std::filesystem::path const& path, std::string const& data) {
    std::ofstream file(path);

    if (!file.is_open()) {
        file.close();
        return Err("Unable to open file");
    }

    file << data;
    if (file.fail()) {
        file.close();
        return Err("Failed to write to file");
    }

    file.close();

    return Ok();
}

Result<> utils::file::writeStringSafe(std::filesystem::path const& path, std::string const& data) {
    GEODE_ANDROID(
        return utils::file::writeString(path, data); // safe approach causes significant performance issues on Android
    )

    std::error_code ec;

    auto tmpPath = path;
    tmpPath += ".tmp";

    auto res = utils::file::writeString(tmpPath, data);
    if (!res) {
        if (std::filesystem::exists(tmpPath, ec)) {
            std::filesystem::remove(tmpPath, ec);
        }
        return res;
    }

    std::filesystem::rename(tmpPath, path, ec);
    if (ec) {
        return Err("Unable to rename temporary file: " + ec.message());
    }

    return Ok();
}

Result<> utils::file::writeBinary(std::filesystem::path const& path, ByteVector const& data) {
    std::ofstream file(path, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        file.close();
        return Err("Unable to open file");
    }

    file.write(reinterpret_cast<char const*>(data.data()), data.size());
    if (file.fail()) {
        file.close();
        return Err("Failed to write to file");
    }

    file.close();
    return Ok();
}

Result<> utils::file::writeBinarySafe(std::filesystem::path const& path, ByteVector const& data) {
    GEODE_ANDROID(
        return utils::file::writeBinary(path, data); // safe approach causes significant performance issues on Android
    )

    std::error_code ec;

    auto tmpPath = path;
    tmpPath += ".tmp";

    auto res = utils::file::writeBinary(tmpPath, data);
    if (!res) {
        if (std::filesystem::exists(tmpPath, ec)) {
            std::filesystem::remove(tmpPath, ec);
        }
        return res;
    }

    std::filesystem::rename(tmpPath, path, ec);
    if (ec) {
        return Err("Unable to rename temporary file: " + ec.message());
    }

    return Ok();
}

Result<> utils::file::createDirectory(std::filesystem::path const& path) {
    std::error_code ec;
    std::filesystem::create_directory(path, ec);

    if (ec) {
        return Err("Unable to create directory: {}", ec.message());
    }
    return Ok();
}

Result<> utils::file::createDirectoryAll(std::filesystem::path const& path) {
    std::error_code ec;
    std::filesystem::create_directories(path, ec);

    if (ec) {
        return Err("Unable to create directory: {}", ec.message());
    }
    return Ok();
}

Result<std::vector<std::filesystem::path>> utils::file::readDirectory(
    std::filesystem::path const& path, bool recursive
) {
    std::error_code ec;

    if (!std::filesystem::exists(path, ec) || ec) {
        return Err("Directory does not exist");
    }

    if (!std::filesystem::is_directory(path, ec) || ec) {
        return Err("Path is not a directory");
    }
    std::vector<std::filesystem::path> res;
    if (recursive) {
        for (auto const& file : std::filesystem::recursive_directory_iterator(path)) {
            res.push_back(file.path());
        }
    } else {
        for (auto const& file : std::filesystem::directory_iterator(path)) {
            res.push_back(file.path());
        }
    }
    return Ok(res);
}

// Unzip

static constexpr auto MAX_ENTRY_PATH_LEN = 256;

struct ZipEntry {
    bool isDirectory;
    int64_t compressedSize;
    int64_t uncompressedSize;
};

class Zip::Impl final {
public:
    using Path = Zip::Path;

private:
    void* m_handle = nullptr;
    void* m_stream = nullptr;
    int32_t m_mode;
    std::variant<Path, ByteVector> m_srcDest;
    std::unordered_map<Path, ZipEntry, path_hash_t> m_entries;
    std::function<void(uint32_t, uint32_t)> m_progressCallback;

    Result<> init() {
        // open stream from file
        if (std::holds_alternative<Path>(m_srcDest)) {
            auto& path = std::get<Path>(m_srcDest);
            // open file
            m_stream = mz_stream_os_create();
            if (!m_stream) {
                return Err("Unable to open file");
            }

            auto pathstr = utils::string::pathToString(path);

            if (mz_stream_os_open(
                m_stream,
                pathstr.c_str(),
                m_mode
            ) != MZ_OK) {
                return Err("Unable to read file");
            }
        }
        // open stream from memory stream
        else {
            auto& src = std::get<ByteVector>(m_srcDest);
            m_stream = mz_stream_mem_create();
            if (!m_stream) {
                return Err("Unable to create memory stream");
            }
            // mz_stream_mem_set_buffer doesn't memcpy so we gotta store the data
            // elsewhere
            if (m_mode == MZ_OPEN_MODE_READ) {
                mz_stream_mem_set_buffer(m_stream, src.data(), src.size());
            }
            else {
                mz_stream_mem_set_grow_size(m_stream, 128 * 1024);
            }
            if (mz_stream_open(m_stream, nullptr, m_mode) != MZ_OK) {
                return Err("Unable to read memory stream");
            }
        }

        // open zip
        m_handle = mz_zip_create();
        if (!m_handle) {
            return Err("Unable to create zip handler");
        }
        if (mz_zip_open(m_handle, m_stream, m_mode) != MZ_OK) {
            return Err("Unable to open zip");
        }

        // get list of entries
        if (!this->loadEntries()) {
            return Err("Unable to read zip");
        }

        return Ok();
    }

    bool loadEntries() {
        uint64_t entryCount;
        if (mz_zip_get_number_entry(m_handle, &entryCount) != MZ_OK) {
            return false;
        }
        auto err = mz_zip_goto_first_entry(m_handle) != MZ_OK;
        while (err == MZ_OK) {
            mz_zip_file* info = nullptr;
            if (mz_zip_entry_get_info(m_handle, &info) != MZ_OK) {
                return false;
            }

            Path filePath;
            filePath.assign(info->filename, info->filename + info->filename_size);
            m_entries.insert({ filePath, ZipEntry {
                .isDirectory = mz_zip_entry_is_dir(m_handle) == MZ_OK,
                .compressedSize = info->compressed_size,
                .uncompressedSize = info->uncompressed_size,
            } });

            err = mz_zip_goto_next_entry(m_handle);
        }
        return true;
    }

    static Result<> mzTry(int32_t code) {
        if (code == MZ_OK) {
            return Ok();
        }
        else {
            return Err("{}", code);
        }
    }

public:
    static Result<std::unique_ptr<Impl>> inFile(Path const& path, int32_t mode) {
        auto ret = std::make_unique<Impl>();
        ret->m_mode = mode;
        ret->m_srcDest = path;
        GEODE_UNWRAP(ret->init());
        return Ok(std::move(ret));
    }

    static Result<std::unique_ptr<Impl>> fromMemory(ByteVector const& raw) {
        auto ret = std::make_unique<Impl>();
        ret->m_mode = MZ_OPEN_MODE_READ;
        ret->m_srcDest = raw;
        GEODE_UNWRAP(ret->init());
        return Ok(std::move(ret));
    }

    static Result<std::unique_ptr<Impl>> intoMemory() {
        auto ret = std::make_unique<Impl>();
        ret->m_mode = MZ_OPEN_MODE_CREATE;
        ret->m_srcDest = ByteVector();
        GEODE_UNWRAP(ret->init());
        return Ok(std::move(ret));
    }

    void setProgressCallback(std::function<void(uint32_t, uint32_t)> callback) {
        m_progressCallback = callback;
    }

    Result<> extractAt(Path const& dir, Path const& name) {
        auto entry = m_entries.at(name);

        GEODE_UNWRAP(
            mzTry(mz_zip_entry_read_open(m_handle, 0, nullptr))
            .mapErr([&](auto error) {
                return fmt::format("Unable to open entry (code {})", error);
            })
        );

        // if the file is empty, its data is empty (duh)
        if (!entry.uncompressedSize) {
            return Ok();
        }

        ByteVector res;
        res.resize(entry.uncompressedSize);
        auto read = mz_zip_entry_read(m_handle, res.data(), entry.uncompressedSize);
        if (read < 0) {
            mz_zip_entry_close(m_handle);
            return Err("Unable to read entry (code {})", read);
        }

        mz_zip_entry_close(m_handle);

        GEODE_UNWRAP(file::createDirectoryAll((dir / name).parent_path()));
        GEODE_UNWRAP(file::writeBinary(dir / name, res).mapErr([&](auto error) {
            return fmt::format("Unable to write to {}: {}", dir / name, error);
        }));

        return Ok();
    }

    Result<> extractAllTo(Path const& dir) {
        GEODE_UNWRAP(file::createDirectoryAll(dir));

        GEODE_UNWRAP(
            mzTry(mz_zip_goto_first_entry(m_handle))
            .mapErr([&](auto error) {
                return fmt::format("Unable to navigate to first entry (code {})", error);
            })
        );

        uint64_t numEntries;

        GEODE_UNWRAP(
            mzTry(mz_zip_get_number_entry(m_handle, &numEntries))
            .mapErr([&](auto error) {
                return fmt::format("Unable to get number of entries (code {})", error);
            })
        );

        uint32_t currentEntry = 0;
        // while not at MZ_END_OF_LIST
        do {
            mz_zip_file* info = nullptr;
            if (mz_zip_entry_get_info(m_handle, &info) != MZ_OK) {
                return Err("Unable to get entry info");
            }
            currentEntry++;

            Path filePath;
            filePath.assign(info->filename, info->filename + info->filename_size);

            // make sure zip files like root/../../file.txt don't get extracted to
            // avoid zip attacks
            std::error_code ec;
#ifdef GEODE_IS_WINDOWS
            if (!std::filesystem::relative((dir / filePath).wstring(), dir.wstring(), ec).empty()) {
#else
            if (!std::filesystem::relative(dir / filePath, dir, ec).empty()) {
#endif
                if (m_entries.at(filePath).isDirectory) {
                    GEODE_UNWRAP(file::createDirectoryAll(dir / filePath));
                }
                else {
                    GEODE_UNWRAP(this->extractAt(dir, filePath));
                }
                if (m_progressCallback) {
                    m_progressCallback(currentEntry, numEntries);
                }
            }
            else {
                log::error(
                    "Zip entry '{}' is not contained within zip bounds",
                    dir / filePath
                );

                if (ec) {
                    return Err(fmt::format("Unable to check relative: {}", ec.message()));
                }
            }
        } while (mz_zip_goto_next_entry(m_handle) == MZ_OK);

        return Ok();
    }

    Result<ByteVector> extract(Path const& name) {
        if (!m_entries.count(name)) {
            return Err("Entry not found");
        }

        auto entry = m_entries.at(name);
        if (entry.isDirectory) {
            return Err("Entry is directory");
        }

        GEODE_UNWRAP(
            mzTry(mz_zip_goto_first_entry(m_handle))
            .mapErr([&](auto error) {
                return fmt::format("Unable to navigate to first entry (code {})", error);
            })
        );

        auto namestr = utils::string::pathToString(name);

        GEODE_UNWRAP(
            mzTry(mz_zip_locate_entry(
                m_handle,
                namestr.c_str(),
                1
            ))
            .mapErr([&](auto error) {
                return fmt::format("Unable to locate entry (code {})", error);
            })
        );

        GEODE_UNWRAP(
            mzTry(mz_zip_entry_read_open(m_handle, 0, nullptr))
            .mapErr([&](auto error) {
                return fmt::format("Unable to open entry (code {})", error);
            })
        );

        // if the file is empty, its data is empty (duh)
        if (!entry.uncompressedSize) {
            return Ok(ByteVector());
        }

        ByteVector res;
        res.resize(entry.uncompressedSize);
        auto read = mz_zip_entry_read(m_handle, res.data(), entry.uncompressedSize);
        if (read < 0) {
            mz_zip_entry_close(m_handle);
            return Err("Unable to read entry (code {})", read);
        }
        mz_zip_entry_close(m_handle);

        return Ok(res);
    }

    Result<> addFolder(Path const& path) {
        auto strPath = path.u8string();
        if (!strPath.ends_with(u8"/") && !strPath.ends_with(u8"\\")) {
            strPath += u8"/";
        }

        mz_zip_file info = { 0 };
        info.version_madeby = MZ_VERSION_MADEBY;
        info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;
        info.filename = reinterpret_cast<const char*>(strPath.c_str());
        info.uncompressed_size = 0;
        info.flag = MZ_ZIP_FLAG_UTF8;
    #ifdef GEODE_IS_WINDOWS
        info.external_fa = FILE_ATTRIBUTE_DIRECTORY;
    #endif
        info.aes_version = MZ_AES_VERSION;


        GEODE_UNWRAP(
            mzTry(mz_zip_entry_write_open(m_handle, &info, MZ_COMPRESS_LEVEL_DEFAULT, 0, nullptr))
            .mapErr([&](auto error) {
                return fmt::format("Unable to open entry for writing (code {})", error);
            })
        );
        mz_zip_entry_close(m_handle);

        return Ok();
    }

    Result<> add(Path const& path, ByteVector const& data) {
        auto namestr = utils::string::pathToString(path);

        mz_zip_file info = { 0 };
        info.version_madeby = MZ_VERSION_MADEBY;
        info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;
        info.filename = namestr.c_str();
        info.uncompressed_size = data.size();
        info.aes_version = MZ_AES_VERSION;

        GEODE_UNWRAP(
            mzTry(mz_zip_entry_write_open(m_handle, &info, MZ_COMPRESS_LEVEL_DEFAULT, 0, nullptr))
            .mapErr([&](auto error) {
                return fmt::format("Unable to open entry for writing (code {})", error);
            })
        );
        auto written = mz_zip_entry_write(m_handle, data.data(), data.size());
        if (written < 0) {
            mz_zip_entry_close(m_handle);
            return Err("Unable to write entry data (code {})", written);
        }
        mz_zip_entry_close(m_handle);

        return Ok();
    }

    ByteVector compressedData() const {
        if (!std::holds_alternative<ByteVector>(m_srcDest)) {
            return ByteVector();
        }
        const uint8_t* buf = nullptr;
        mz_stream_mem_get_buffer(m_stream, reinterpret_cast<const void**>(&buf));
        mz_stream_mem_seek(m_stream, 0, MZ_SEEK_END);
        auto size = mz_stream_mem_tell(m_stream);
        return ByteVector(buf, buf + size);
    }

    Path getPath() const {
        if (std::holds_alternative<Path>(m_srcDest)) {
            return std::get<Path>(m_srcDest);
        }
        return Path();
    }

    std::unordered_map<Path, ZipEntry, path_hash_t> getEntries() const {
        return m_entries;
    }

    ~Impl() {
        if (m_handle) {
            mz_zip_close(m_handle);
            mz_zip_delete(&m_handle);
        }
        if (m_stream) {
            mz_stream_close(m_stream);
            mz_stream_delete(&m_stream);
        }
    }
};

Unzip::Unzip() : m_impl(nullptr) {}

Unzip::~Unzip() {}

Unzip::Unzip(std::unique_ptr<Unzip::Impl>&& impl) : m_impl(std::move(impl)) {}

Unzip::Unzip(Unzip&& other) : m_impl(std::move(other.m_impl)) {
    other.m_impl = nullptr;
}

Result<Unzip> Unzip::create(Path const& file) {
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::inFile(file, MZ_OPEN_MODE_READ));
    return Ok(Unzip(std::move(impl)));
}

Result<Unzip> Unzip::create(ByteVector const& data) {
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::fromMemory(data));
    return Ok(Unzip(std::move(impl)));
}

Unzip::Path Unzip::getPath() const {
    return m_impl->getPath();
}

void Unzip::setProgressCallback(
    std::function<void(uint32_t, uint32_t)> callback
) {
    return m_impl->setProgressCallback(callback);
}

std::vector<Unzip::Path> Unzip::getEntries() const {
    return map::keys(m_impl->getEntries());
}

bool Unzip::hasEntry(Path const& name) {
    return m_impl->getEntries().count(name);
}

Result<ByteVector> Unzip::extract(Path const& name) {
    return m_impl->extract(name).mapErr([&](auto error) {
        return fmt::format("Unable to extract entry {}: {}", name, error);
    });
}

Result<> Unzip::extractTo(Path const& name, Path const& path) {
    GEODE_UNWRAP_INTO(auto bytes, m_impl->extract(name).mapErr([&](auto error) {
        return fmt::format("Unable to extract entry {}: {}", name, error);
    }));
    // create containing directories for target path
    if (path.has_parent_path()) {
        GEODE_UNWRAP(file::createDirectoryAll(path.parent_path()));
    }
    GEODE_UNWRAP(file::writeBinary(path, bytes).mapErr([&](auto error) {
        return fmt::format("Unable to write file {}: {}", path, error);
    }));
    return Ok();
}

Result<> Unzip::extractAllTo(Path const& dir) {
    return m_impl->extractAllTo(dir);
}

Result<> Unzip::intoDir(
    Path const& from,
    Path const& to,
    bool deleteZipAfter
) {
    // scope to ensure the zip is closed after extracting so the zip can be
    // removed
    {
        GEODE_UNWRAP_INTO(auto unzip, Unzip::create(from));
        // TODO: this is quite slow lol, takes 30 seconds to extract index..
        GEODE_UNWRAP(unzip.extractAllTo(to));
    }
    if (deleteZipAfter) {
        std::error_code ec;
        std::filesystem::remove(from, ec);
    }
    return Ok();
}

Result<> Unzip::intoDir(
    std::function<void(uint32_t, uint32_t)> progressCallback,
    Path const& from,
    Path const& to,
    bool deleteZipAfter
) {
    GEODE_UNWRAP_INTO(auto unzip, Unzip::create(from));
    unzip.setProgressCallback(progressCallback);
    GEODE_UNWRAP(unzip.extractAllTo(to));
    if (deleteZipAfter) {
        std::error_code ec;
        std::filesystem::remove(from, ec);
    }
    return Ok();
}

// Zip

Zip::Zip() : m_impl(nullptr) {}

Zip::~Zip() {}

Zip::Zip(std::unique_ptr<Zip::Impl>&& impl) : m_impl(std::move(impl)) {}

Zip::Zip(Zip&& other) : m_impl(std::move(other.m_impl)) {
    other.m_impl = nullptr;
}

Result<Zip> Zip::create(Path const& file) {
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::inFile(file, MZ_OPEN_MODE_CREATE | MZ_OPEN_MODE_WRITE));
    return Ok(Zip(std::move(impl)));
}

Result<Zip> Zip::create() {
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::intoMemory());
    return Ok(Zip(std::move(impl)));
}

Zip::Path Zip::getPath() const {
    return m_impl->getPath();
}

ByteVector Zip::getData() const {
    return m_impl->compressedData();
}

Result<> Zip::add(Path const& path, ByteVector const& data) {
    return m_impl->add(path, data);
}

Result<> Zip::add(Path const& path, std::string const& data) {
    return this->add(path, ByteVector(data.begin(), data.end()));
}

Result<> Zip::addFrom(Path const& file, Path const& entryDir) {
    GEODE_UNWRAP_INTO(auto data, file::readBinary(file));
    return this->add(entryDir / file.filename(), data);
}

Result<> Zip::addAllFromRecurse(Path const& dir, Path const& entry) {
    GEODE_UNWRAP(this->addFolder(entry / dir.filename()));
    for (auto& file : std::filesystem::directory_iterator(dir)) {
        if (std::filesystem::is_directory(file)) {
            GEODE_UNWRAP(this->addAllFromRecurse(file, entry / dir.filename()));
        } else {
            GEODE_UNWRAP_INTO(auto data, file::readBinary(file));
            GEODE_UNWRAP(this->addFrom(file, entry / dir.filename()));
        }
    }
    return Ok();
}

Result<> Zip::addAllFrom(Path const& dir) {
    if (!std::filesystem::is_directory(dir)) {
        return Err("Path is not a directory");
    }
    return this->addAllFromRecurse(dir, Path());
}

Result<> Zip::addFolder(Path const& entry) {
    return m_impl->addFolder(entry);
}

FileWatchEvent::FileWatchEvent(std::filesystem::path const& path)
  : m_path(path) {}

std::filesystem::path FileWatchEvent::getPath() const {
    return m_path;
}

ListenerResult FileWatchFilter::handle(
    std::function<Callback> callback,
    FileWatchEvent* event
) {
    std::error_code ec;
    if (std::filesystem::equivalent(event->getPath(), m_path, ec)) {
        callback(event);
    }
    return ListenerResult::Propagate;
}

FileWatchFilter::FileWatchFilter(std::filesystem::path const& path)
  : m_path(path) {}

// This is a vector because need to use std::filesystem::equivalent for
// comparisons and removal is not exactly performance-critical here
// (who's going to add and remove 500 file watchers every frame)
static std::vector<std::unique_ptr<FileWatcher>> FILE_WATCHERS {};

Result<> file::watchFile(std::filesystem::path const& file) {
    if (!std::filesystem::exists(file)) {
        return Err("File does not exist");
    }
    auto watcher = std::make_unique<FileWatcher>(
        file,
        [](auto const& path) {
            Loader::get()->queueInMainThread([=] {
                FileWatchEvent(path).post();
            });
        }
    );
    if (!watcher->watching()) {
        return Err("Unknown error watching file");
    }
    FILE_WATCHERS.emplace_back(std::move(watcher));
    return Ok();
}

void file::unwatchFile(std::filesystem::path const& file) {
    ranges::remove(FILE_WATCHERS, [=](std::unique_ptr<FileWatcher> const& watcher) {
        return std::filesystem::equivalent(file, watcher->path());
    });
}
