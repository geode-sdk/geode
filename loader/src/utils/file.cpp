
#include <Geode/loader/Log.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/string.hpp>
#include <json.hpp>
#include <fstream>
#include <mz.h>
#include <mz_os.h>
#include <mz_strm.h>
#include <mz_strm_os.h>
#include <mz_strm_mem.h>
#include <mz_zip.h>
#include <internal/FileWatcher.hpp>

#ifdef GEODE_IS_WINDOWS
#include <filesystem>
#endif

using namespace geode::prelude;
using namespace geode::utils::file;

Result<std::string> utils::file::readString(ghc::filesystem::path const& path) {
#if _WIN32
    std::ifstream in(path.wstring(), std::ios::in | std::ios::binary);
#else
    std::ifstream in(path.string(), std::ios::in | std::ios::binary);
#endif
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize((const size_t)in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return Ok(contents);
    }
    return Err("Unable to open file");
}

Result<json::Value> utils::file::readJson(ghc::filesystem::path const& path) {

    auto str = utils::file::readString(path);

    if (str) {
        try {
            return Ok(json::parse(str.value()));
        } catch(std::exception const& e) {
            return Err("Unable to parse JSON: " + std::string(e.what()));
        }
    } else {
        return Err("Unable to open file");
    }
}

Result<ByteVector> utils::file::readBinary(ghc::filesystem::path const& path) {
#if _WIN32
    std::ifstream in(path.wstring(), std::ios::in | std::ios::binary);
#else
    std::ifstream in(path.string(), std::ios::in | std::ios::binary);
#endif
    if (in) {
        return Ok(ByteVector(std::istreambuf_iterator<char>(in), {}));
    }
    return Err("Unable to open file");
}

Result<> utils::file::writeString(ghc::filesystem::path const& path, std::string const& data) {
    std::ofstream file;
#if _WIN32
    file.open(path.wstring());
#else
    file.open(path.string());
#endif
    if (file.is_open()) {
        file << data;
        file.close();

        return Ok();
    }
    file.close();
    return Err("Unable to open file");
}

Result<> utils::file::writeBinary(ghc::filesystem::path const& path, ByteVector const& data) {
    std::ofstream file;
#if _WIN32
    file.open(path.wstring(), std::ios::out | std::ios::binary);
#else
    file.open(path.string(), std::ios::out | std::ios::binary);
#endif
    if (file.is_open()) {
        file.write(reinterpret_cast<char const*>(data.data()), data.size());
        file.close();

        return Ok();
    }
    file.close();
    return Err("Unable to open file");
}

Result<> utils::file::createDirectory(ghc::filesystem::path const& path) {
    try {
#ifdef GEODE_IS_WINDOWS
        std::filesystem::create_directory(path.wstring());
#else
        ghc::filesystem::create_directory(path);
#endif
        return Ok();
    }
    catch (...) {
        return Err("Unable to create directory");
    }
}

Result<> utils::file::createDirectoryAll(ghc::filesystem::path const& path) {
    try {
#ifdef GEODE_IS_WINDOWS
        std::filesystem::create_directories(path.wstring());
#else
        ghc::filesystem::create_directories(path);
#endif
        return Ok();
    }
    catch (...) {
        return Err("Unable to create directories");
    }
}

Result<std::vector<ghc::filesystem::path>> utils::file::listFiles(
    ghc::filesystem::path const& path, bool recursive
) {
    return file::readDirectory(path, recursive);
}

Result<std::vector<ghc::filesystem::path>> utils::file::readDirectory(
    ghc::filesystem::path const& path, bool recursive
) {
    if (!ghc::filesystem::exists(path)) {
        return Err("Directory does not exist");
    }
    if (!ghc::filesystem::is_directory(path)) {
        return Err("Path is not a directory");
    }
    std::vector<ghc::filesystem::path> res;
    if (recursive) {
        for (auto const& file : ghc::filesystem::recursive_directory_iterator(path)) {
            res.push_back(file.path());
        }
    } else {
        for (auto const& file : ghc::filesystem::directory_iterator(path)) {
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
    std::unordered_map<Path, ZipEntry> m_entries;

    Result<> init() {
        // open stream from file
        if (std::holds_alternative<Path>(m_srcDest)) {
            auto& path = std::get<Path>(m_srcDest);
            // open file
            if (!mz_stream_os_create(&m_stream)) {
                return Err("Unable to open file");
            }
            if (mz_stream_os_open(
                m_stream,
                reinterpret_cast<const char*>(path.u8string().c_str()),
                m_mode
            ) != MZ_OK) {
                return Err("Unable to read file");
            }
        }
        // open stream from memory stream
        else {
            auto& src = std::get<ByteVector>(m_srcDest);
            if (!mz_stream_mem_create(&m_stream)) {
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
        if (!mz_zip_create(&m_handle)) {
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
            return Err(std::to_string(code));
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
            .expect("Unable to navigate to first entry (code {error})")
        );

        GEODE_UNWRAP(
            mzTry(mz_zip_locate_entry(
                m_handle,
                reinterpret_cast<const char*>(name.u8string().c_str()),
                1
            )).expect("Unable to navigate to entry (code {error})")
        );

        GEODE_UNWRAP(
            mzTry(mz_zip_entry_read_open(m_handle, 0, nullptr))
            .expect("Unable to open entry (code {error})")
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
            return Err("Unable to read entry (code " + std::to_string(read) + ")");
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
            .expect("Unable to open entry for writing (code {error})")
        );
        mz_zip_entry_close(m_handle);

        return Ok();
    }

    Result<> add(Path const& path, ByteVector const& data) {
        mz_zip_file info = { 0 };
        info.version_madeby = MZ_VERSION_MADEBY;
        info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;
        info.filename = reinterpret_cast<const char*>(path.u8string().c_str());
        info.uncompressed_size = data.size();
        info.aes_version = MZ_AES_VERSION;

        GEODE_UNWRAP(
            mzTry(mz_zip_entry_write_open(m_handle, &info, MZ_COMPRESS_LEVEL_DEFAULT, 0, nullptr))
            .expect("Unable to open entry for writing (code {error})")
        );
        auto written = mz_zip_entry_write(m_handle, data.data(), data.size());
        if (written < 0) {
            mz_zip_entry_close(m_handle);
            return Err("Unable to write entry data (code " + std::to_string(written) + ")");
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

    std::unordered_map<Path, ZipEntry> getEntries() const {
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

std::vector<Unzip::Path> Unzip::getEntries() const {
    return map::keys(m_impl->getEntries());
}

bool Unzip::hasEntry(Path const& name) {
    return m_impl->getEntries().count(name);
}

Result<ByteVector> Unzip::extract(Path const& name) {
    return m_impl->extract(name).expect("{error} (entry {})", name.string());
}

Result<> Unzip::extractTo(Path const& name, Path const& path) {
    GEODE_UNWRAP_INTO(auto bytes, m_impl->extract(name).expect("{error} (entry {})", name.string()));
    // create containing directories for target path
    if (path.has_parent_path()) {
        GEODE_UNWRAP(file::createDirectoryAll(path.parent_path()));
    }
    GEODE_UNWRAP(file::writeBinary(path, bytes).expect("Unable to write file {}: {error}", path.string()));
    return Ok();
}

Result<> Unzip::extractAllTo(Path const& dir) {
    GEODE_UNWRAP(file::createDirectoryAll(dir));
    for (auto& [entry, info] : m_impl->getEntries()) {
        // make sure zip files like root/../../file.txt don't get extracted to 
        // avoid zip attacks
#ifdef GEODE_IS_WINDOWS
        if (!std::filesystem::relative((dir / entry).wstring(), dir.wstring()).empty()) {
#else
        if (!ghc::filesystem::relative(dir / entry, dir).empty()) {
#endif
            if (info.isDirectory) {
                GEODE_UNWRAP(file::createDirectoryAll(dir / entry));
            } else {
                GEODE_UNWRAP(this->extractTo(entry, dir / entry));
            }
        } else {
            log::error(
                "Zip entry '{}' is not contained within zip bounds",
                dir / entry
            );
        }
    }
    return Ok();
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
        GEODE_UNWRAP(unzip.extractAllTo(to));
    }
    if (deleteZipAfter) {
        try { ghc::filesystem::remove(from); } catch(...) {}
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
    for (auto& file : ghc::filesystem::directory_iterator(dir)) {
        if (ghc::filesystem::is_directory(file)) {
            GEODE_UNWRAP(this->addAllFromRecurse(file, entry / dir.filename()));
        } else {
            GEODE_UNWRAP_INTO(auto data, file::readBinary(file));
            GEODE_UNWRAP(this->addFrom(file, entry / dir.filename()));
        }
    }
    return Ok();
}

Result<> Zip::addAllFrom(Path const& dir) {
    if (!ghc::filesystem::is_directory(dir)) {
        return Err("Path is not a directory");
    }
    return this->addAllFromRecurse(dir, Path());
}

Result<> Zip::addFolder(Path const& entry) {
    return m_impl->addFolder(entry);
}

FileWatchEvent::FileWatchEvent(ghc::filesystem::path const& path)
  : m_path(path) {}

ghc::filesystem::path FileWatchEvent::getPath() const {
    return m_path;
}

ListenerResult FileWatchFilter::handle(
    MiniFunction<Callback> callback,
    FileWatchEvent* event
) {
    std::error_code ec;
    if (ghc::filesystem::equivalent(event->getPath(), m_path, ec)) {
        callback(event);
    }
    return ListenerResult::Propagate;
}

FileWatchFilter::FileWatchFilter(ghc::filesystem::path const& path) 
  : m_path(path) {}

// This is a vector because need to use ghc::filesystem::equivalent for 
// comparisons and removal is not exactly performance-critical here
// (who's going to add and remove 500 file watchers every frame)
static std::vector<std::unique_ptr<FileWatcher>> FILE_WATCHERS {};

Result<> file::watchFile(ghc::filesystem::path const& file) {
    if (!ghc::filesystem::exists(file)) {
        return Err("File does not exist");
    }
    auto watcher = std::make_unique<FileWatcher>(
        file,
        [](auto const& path) {
            Loader::get()->queueInGDThread([=] {
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

void file::unwatchFile(ghc::filesystem::path const& file) {
    ranges::remove(FILE_WATCHERS, [=](std::unique_ptr<FileWatcher> const& watcher) {
        return ghc::filesystem::equivalent(file, watcher->path());
    });
}
