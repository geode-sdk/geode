#include <Geode/loader/Loader.hpp> // a third great circular dependency fix
#include <Geode/loader/Log.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/string.hpp>

#include <asp/fs/fs.hpp>
#include <asp/iter.hpp>
#include <matjson.hpp>
#include <mz.h>
#include <mz_os.h>
#include <mz_strm.h>
#include <mz_strm_mem.h>
#include <mz_strm_os.h>
#include <mz_zip.h>

#ifdef GEODE_IS_WINDOWS
#include <Windows.h>
#endif

using namespace geode::prelude;
using namespace geode::utils::file;
using asp::fs::Path;
using asp::fs::PathView;

Result<std::string> utils::file::readString(PathView path) {
    return asp::fs::readToString(path);
}

Result<matjson::Value> utils::file::readJson(PathView path) {
    auto str = GEODE_UNWRAP(utils::file::readString(path));
    return matjson::parse(str).mapErr([&](auto const& err) {
        return fmt::format("Unable to parse JSON: {}", err);
    });
}

Result<ByteVector> utils::file::readBinary(PathView path) {
    return asp::fs::read(path);
}

Result<> utils::file::writeString(PathView path, std::string_view data) {
    return asp::fs::write(path, data);
}

Result<> utils::file::writeStringSafe(PathView path, std::string_view data) {
    GEODE_ANDROID(
        return utils::file::writeString(path, data); // safe approach causes significant performance issues on Android
    )

    Path tmpPath = path;
    tmpPath += ".tmp";

    auto res = utils::file::writeString(tmpPath, data);
    if (!res) {
        // remove temp path if we anything was created
        (void) asp::fs::remove(tmpPath);
        return res;
    }

    res = asp::fs::rename(tmpPath, path);
    if (!res) {
        return Err("Unable to rename temporary file: {}", res.unwrapErr());
    }

    return Ok();
}

Result<> utils::file::writeBinary(PathView path, ByteSpan data) {
    return asp::fs::write(path, data);
}

Result<> utils::file::writeBinarySafe(PathView path, ByteSpan data) {
    GEODE_ANDROID(
        return utils::file::writeBinary(path, data); // safe approach causes significant performance issues on Android
    )

    Path tmpPath = path;
    tmpPath += ".tmp";

    auto res = utils::file::writeBinary(tmpPath, data);
    if (!res) {
        // remove temp path if we anything was created
        (void) asp::fs::remove(tmpPath);
        return res;
    }

    res = asp::fs::rename(tmpPath, path);
    if (!res) {
        return Err("Unable to rename temporary file: {}", res.unwrapErr());
    }

    return Ok();
}

Result<> utils::file::createDirectory(PathView path) {
    return asp::fs::createDir(path);
}

Result<> utils::file::createDirectoryAll(PathView path) {
    return asp::fs::createDirAll(path);
}

Result<std::vector<Path>> utils::file::readDirectory(
    PathView path, bool recursive
) {
    if (recursive) {
        return GEODE_UNWRAP(asp::fs::iterDirRecursive(path))
            .map([](auto const& entry) {
                return entry.map([](auto e) { return e.path; });
            })
            .collect<Result<std::vector<Path>>>();
    } else {
        return GEODE_UNWRAP(asp::fs::iterDir(path))
            .map([](auto const& entry) {
                return entry.map([](auto e) { return e.path; });
            })
            .collect<Result<std::vector<Path>>>();
    }
}

// Unzip

static constexpr auto MAX_ENTRY_PATH_LEN = 256;

static Path safePathJoin(const Path& path, const Path& suffix) {
    auto joined = path / suffix;
    auto lrel = joined.lexicallyRelative(path);
    if (!lrel || lrel->empty() || lrel->components().next() == "..") return {};

    return joined;
}

struct ZipEntry {
    bool isDirectory;
    int64_t compressedSize;
    int64_t uncompressedSize;
};

class Zip::Impl final {
private:
    void* m_handle = nullptr;
    void* m_stream = nullptr;
    int32_t m_mode;
    std::variant<Path, ByteVector> m_srcDest;
    std::unordered_map<Path, ZipEntry> m_entries;
    geode::Function<void(uint32_t, uint32_t)> m_progressCallback;

    Result<> init() {
        // open stream from file
        if (std::holds_alternative<Path>(m_srcDest)) {
            auto& path = std::get<Path>(m_srcDest);
            // open file
            m_stream = mz_stream_os_create();
            if (!m_stream) {
                return Err("Unable to open file");
            }

            if (mz_stream_os_open(
                m_stream,
                path.c_str(),
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
    static Result<std::unique_ptr<Impl>> inFile(Path path, int32_t mode) {
        auto ret = std::make_unique<Impl>();
        ret->m_mode = mode;
        ret->m_srcDest = std::move(path);
        GEODE_UNWRAP(ret->init());
        return Ok(std::move(ret));
    }

    static Result<std::unique_ptr<Impl>> fromMemory(ByteSpan raw) {
        auto ret = std::make_unique<Impl>();
        ret->m_mode = MZ_OPEN_MODE_READ;
        ret->m_srcDest = ByteVector{raw.begin(), raw.end()};
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

    void setProgressCallback(geode::Function<void(uint32_t, uint32_t)> callback) {
        m_progressCallback = std::move(callback);
    }

    Result<> extractAt(PathView dir, PathView name) {
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

        GEODE_UNWRAP(file::createDirectoryAll((dir / name).parent().value()));
        GEODE_UNWRAP(file::writeBinary(dir / name, res).mapErr([&](auto error) {
            return fmt::format("Unable to write to {}: {}", dir / name, error);
        }));

        return Ok();
    }

    Result<> extractAllTo(PathView dir) {
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
            auto safePath = safePathJoin(dir, filePath);

            if (!safePath.empty()) {
                if (m_entries.at(filePath).isDirectory) {
                    GEODE_UNWRAP(file::createDirectoryAll(safePath));
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
                    filePath
                );
            }
        } while (mz_zip_goto_next_entry(m_handle) == MZ_OK);

        return Ok();
    }

    Result<ByteVector> extract(PathView name) {
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

        GEODE_UNWRAP(
            mzTry(mz_zip_locate_entry(
                m_handle,
                name.string().c_str(),
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

    Result<> addFolder(PathView path) {
        auto strPath = path.string();
        if (!strPath.ends_with("/") && !strPath.ends_with("\\")) {
            strPath += "/";
        }

        mz_zip_file info = { 0 };
        info.version_madeby = MZ_VERSION_MADEBY;
        info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;
        info.filename = reinterpret_cast<const char*>(strPath.c_str());
        info.uncompressed_size = 0;
        info.flag = MZ_ZIP_FLAG_UTF8;
#ifdef GEODE_IS_WINDOWS
        info.external_fa = FILE_ATTRIBUTE_DIRECTORY;
        info.aes_version = MZ_AES_VERSION;
#endif


        GEODE_UNWRAP(
            mzTry(mz_zip_entry_write_open(m_handle, &info, MZ_COMPRESS_LEVEL_DEFAULT, 0, nullptr))
            .mapErr([&](auto error) {
                return fmt::format("Unable to open entry for writing (code {})", error);
            })
        );
        mz_zip_entry_close(m_handle);

        return Ok();
    }

    Result<> add(Path path, ByteSpan data) {
        mz_zip_file info = { 0 };
        info.version_madeby = MZ_VERSION_MADEBY;
        info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;
        info.filename = path.c_str();
        info.uncompressed_size = data.size();
#ifdef GEODE_IS_WINDOWS
        info.aes_version = MZ_AES_VERSION;
#endif

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

Unzip::Unzip(Unzip&& other) noexcept = default;

Result<Unzip> Unzip::create(PathView file) {
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::inFile(file, MZ_OPEN_MODE_READ));
    return Ok(Unzip(std::move(impl)));
}

Result<Unzip> Unzip::create(ByteSpan data) {
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::fromMemory(data));
    return Ok(Unzip(std::move(impl)));
}

Path Unzip::getPath() const {
    return m_impl->getPath();
}

void Unzip::setProgressCallback(
    geode::Function<void(uint32_t, uint32_t)> callback
) {
    return m_impl->setProgressCallback(std::move(callback));
}

std::vector<Path> Unzip::getEntries() const {
    return map::keys(m_impl->getEntries());
}

bool Unzip::hasEntry(PathView name) {
    return m_impl->getEntries().contains(name);
}

Result<ByteVector> Unzip::extract(PathView name) {
    return m_impl->extract(name).mapErr([&](auto error) {
        return fmt::format("Unable to extract entry {}: {}", name, error);
    });
}

Result<> Unzip::extractTo(PathView name, PathView path) {
    GEODE_UNWRAP_INTO(auto bytes, m_impl->extract(name).mapErr([&](auto error) {
        return fmt::format("Unable to extract entry {}: {}", name, error);
    }));
    // create containing directories for target path
    if (auto parent = path.parent()) {
        GEODE_UNWRAP(file::createDirectoryAll(*parent));
    }
    GEODE_UNWRAP(file::writeBinary(path, bytes).mapErr([&](auto error) {
        return fmt::format("Unable to write file {}: {}", path, error);
    }));
    return Ok();
}

Result<> Unzip::extractAllTo(PathView dir) {
    return m_impl->extractAllTo(dir);
}

Result<> Unzip::intoDir(
    PathView from,
    PathView to,
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
        GEODE_UNWRAP(asp::fs::remove(from));
    }
    return Ok();
}

Result<> Unzip::intoDir(
    geode::Function<void(uint32_t, uint32_t)> progressCallback,
    PathView from,
    PathView to,
    bool deleteZipAfter
) {
    GEODE_UNWRAP_INTO(auto unzip, Unzip::create(from));
    unzip.setProgressCallback(std::move(progressCallback));
    GEODE_UNWRAP(unzip.extractAllTo(to));

    if (deleteZipAfter) {
        GEODE_UNWRAP(asp::fs::remove(from));
    }
    return Ok();
}

// Zip

Zip::Zip() : m_impl(nullptr) {}

Zip::~Zip() {}

Zip::Zip(std::unique_ptr<Zip::Impl>&& impl) : m_impl(std::move(impl)) {}

Zip::Zip(Zip&& other) noexcept = default;

Result<Zip> Zip::create(PathView file) {
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::inFile(file, MZ_OPEN_MODE_CREATE | MZ_OPEN_MODE_WRITE));
    return Ok(Zip(std::move(impl)));
}

Result<Zip> Zip::create() {
    GEODE_UNWRAP_INTO(auto impl, Zip::Impl::intoMemory());
    return Ok(Zip(std::move(impl)));
}

Path Zip::getPath() const {
    return m_impl->getPath();
}

ByteVector Zip::getData() const {
    return m_impl->compressedData();
}

Result<> Zip::add(PathView path, ByteSpan data) {
    return m_impl->add(path, data);
}

Result<> Zip::add(PathView path, std::string_view data) {
    auto vec = ByteVector{data.begin(), data.end()};
    return this->add(path, vec);
}

Result<> Zip::addFrom(PathView file, PathView entryDir) {
    GEODE_UNWRAP_INTO(auto data, file::readBinary(file));

    if (auto filename = file.filename()) {
        return this->add(entryDir / *filename, data);
    }
    return Err("no filename in path");
}

Result<> Zip::addAllFromRecurse(PathView dir, PathView toAdd) {
    auto filename = dir.filename();
    if (!filename) {
        return Err("No filename in path");
    }

    GEODE_UNWRAP(this->addFolder(toAdd / *filename));

    for (auto res : GEODE_UNWRAP(asp::fs::iterDir(dir))) {
        auto entry = GEODE_UNWRAP(res);

        if (entry.isDir()) {
            GEODE_UNWRAP(this->addAllFromRecurse(entry.path, toAdd / *filename));
        } else {
            GEODE_UNWRAP_INTO(auto data, file::readBinary(entry.path));
            GEODE_UNWRAP(this->addFrom(entry.path, toAdd / *filename));
        }
    }
    return Ok();
}

Result<> Zip::addAllFrom(PathView dir) {
    if (!dir.isDir()) {
        return Err("Path is not a directory");
    }
    return this->addAllFromRecurse(dir, Path());
}

Result<> Zip::addFolder(PathView entry) {
    return m_impl->addFolder(entry);
}
