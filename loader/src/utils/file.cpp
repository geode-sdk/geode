#include <../support/zip_support/ZipUtils.h>
#include <../support/zip_support/ioapi.h>
#include <../support/zip_support/unzip.h>
#include <../support/zip_support/zip.h>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/string.hpp>
#include <json.hpp>
#include <fstream>

USE_GEODE_NAMESPACE();
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
        ghc::filesystem::create_directory(path);
        return Ok();
    }
    catch (...) {
        return Err("Unable to create directory");
    }
}

Result<> utils::file::createDirectoryAll(ghc::filesystem::path const& path) {
    try {
        ghc::filesystem::create_directories(path);
        return Ok();
    }
    catch (...) {
        return Err("Unable to create directories");
    }
}

Result<std::vector<ghc::filesystem::path>> utils::file::listFiles(
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
    unz_file_pos pos;
    ZPOS64_T compressedSize;
    ZPOS64_T uncompressedSize;
};

class file::Unzip::Impl final {
public:
    using Path = Unzip::Path;

private:
    unzFile m_zip;
    Path m_zipPath;
    std::unordered_map<Path, ZipEntry> m_entries;

public:
    bool loadEntries() {
        // Clear old entries
        m_entries.clear();

        char fileName[MAX_ENTRY_PATH_LEN + 1];
        unz_file_info64 fileInfo;

        // Read first file
        if (unzGoToFirstFile64(m_zip, &fileInfo, fileName, sizeof(fileName) - 1)) {
            return false;
        }
        // Loop over all files
        while (true) {
            // Read file and add to entries
            unz_file_pos pos;
            if (unzGetFilePos(m_zip, &pos) == UNZ_OK) {
                auto len = strlen(fileName);
                m_entries.insert({
                    fileName,
                    ZipEntry {
                        .isDirectory = 
                            fileInfo.uncompressed_size == 0 &&
                            len > 0 &&
                            (fileName[len - 1] == '/' || fileName[len - 1] == '\\'),
                        .pos = pos,
                        .compressedSize = fileInfo.compressed_size,
                        .uncompressedSize = fileInfo.uncompressed_size,
                    },
                });
            }
            // Read next file, or break on error
            if (unzGoToNextFile64(m_zip, &fileInfo, fileName, sizeof(fileName) - 1) != UNZ_OK) {
                break;
            }
        }
        return true;
    }

    Result<ByteVector> extract(Path const& name) {
        if (!m_entries.count(name)) {
            return Err("Entry not found");
        }

        auto entry = m_entries.at(name);

        if (entry.isDirectory) {
            return Err("Entry is directory");
        }

        if (unzGoToFilePos(m_zip, &entry.pos) != UNZ_OK) {
            return Err("Unable to navigate to entry");
        }
        if (unzOpenCurrentFile(m_zip) != UNZ_OK) {
            return Err("Unable to open entry");
        }
        ByteVector res;
        res.resize(entry.uncompressedSize);
        auto size = unzReadCurrentFile(m_zip, res.data(), entry.uncompressedSize);
        if (size < 0 || size != entry.uncompressedSize) {
            unzCloseCurrentFile(m_zip);
            return Err("Unable to extract entry");
        }
        unzCloseCurrentFile(m_zip);

        return Ok(res);
    }

    std::unordered_map<Path, ZipEntry>& entries() {
        return m_entries;
    }

    Path& path() {
        return m_zipPath;
    }

    Impl(unzFile zip, Path const& path) : m_zip(zip), m_zipPath(path) {}

    ~Impl() {
        unzClose(m_zip);
    }
};

Unzip::Unzip() : m_impl(nullptr) {}

Unzip::~Unzip() {}

Unzip::Unzip(std::unique_ptr<Unzip::Impl>&& impl) : m_impl(std::move(impl)) {}

Unzip::Unzip(Unzip&& other) : m_impl(std::move(other.m_impl)) {
    other.m_impl = nullptr;
}

Result<Unzip> Unzip::create(Path const& file) {
    // todo: make sure unicode paths work
    auto zip = unzOpen(file.generic_string().c_str());
    if (!zip) {
        return Err("Unable to open zip file");
    }
    auto impl = std::make_unique<Unzip::Impl>(zip, file);
    if (!impl->loadEntries()) {
        return Err("Unable to read zip file");
    }
    return Ok(Unzip(std::move(impl)));
}

Unzip::Path Unzip::getPath() const {
    return m_impl->path();
}

std::vector<Unzip::Path> Unzip::getEntries() const {
    return map::keys(m_impl->entries());
}

bool Unzip::hasEntry(Path const& name) {
    return m_impl->entries().count(name);
}

Result<ByteVector> Unzip::extract(Path const& name) {
    return m_impl->extract(name);
}

Result<> Unzip::extractTo(Path const& name, Path const& path) {
    GEODE_UNWRAP_INTO(auto bytes, m_impl->extract(name));
    // create containing directories for target path
    if (path.has_parent_path()) {
        GEODE_UNWRAP(file::createDirectoryAll(path.parent_path()));
    }
    GEODE_UNWRAP(file::writeBinary(path, bytes).expect("Unable to write file {}: {error}", path.string()));
    return Ok();
}

Result<> Unzip::extractAllTo(Path const& dir) {
    GEODE_UNWRAP(file::createDirectoryAll(dir));
    for (auto& [entry, info] : m_impl->entries()) {
        // make sure zip files like root/../../file.txt don't get extracted to 
        // avoid zip attacks
        if (!ghc::filesystem::relative(dir / entry, dir).empty()) {
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

class file::Zip::Impl final {
public:
    using Path = Unzip::Path;

private:
    zipFile m_zip;
    Path m_zipPath;

public:
    Path& path() {
        return m_zipPath;
    }

    Result<> addFolder(Path const& path) {
        // a directory in a zip is just a file with no data and which ends in 
        // a slash
        auto strPath = path.generic_string();
        if (!strPath.ends_with("/") && !strPath.ends_with("\\")) {
            strPath += "/";
        }
        if (zipOpenNewFileInZip(
            m_zip, strPath.c_str(),
            nullptr, nullptr, 0, nullptr, 0, nullptr,
            Z_DEFLATED, Z_DEFAULT_COMPRESSION
        ) != ZIP_OK) {
            return Err("Unable to create directory " + path.string());
        }
        zipCloseFileInZip(m_zip);
        return Ok();
    }

    Result<> add(Path const& path, ByteVector const& data) {
        // open entry
        zip_fileinfo info = { 0 };
        if (zipOpenNewFileInZip(
            m_zip, path.generic_string().c_str(),
            &info, nullptr, 0, nullptr, 0, nullptr,
            Z_DEFLATED, Z_DEFAULT_COMPRESSION
        ) != ZIP_OK) {
            return Err("Unable to create entry " + path.string());
        }

        // write data
        if (zipWriteInFileInZip(m_zip, data.data(), data.size()) != ZIP_OK) {
            zipCloseFileInZip(m_zip);
            return Err("Unable to write entry " + path.string());
        }

        // make sure to close!
        zipCloseFileInZip(m_zip);

        return Ok();
    }

    Impl(zipFile zip, Path const& path) : m_zip(zip), m_zipPath(path) {}

    ~Impl() {
        zipClose(m_zip, nullptr);
    }
};

Zip::Zip() : m_impl(nullptr) {}

Zip::~Zip() {}

Zip::Zip(std::unique_ptr<Zip::Impl>&& impl) : m_impl(std::move(impl)) {}

Zip::Zip(Zip&& other) : m_impl(std::move(other.m_impl)) {
    other.m_impl = nullptr;
}

Result<Zip> Zip::create(Path const& file) {
    // todo: make sure unicode paths work
    auto zip = zipOpen(file.generic_string().c_str(), APPEND_STATUS_CREATE);
    if (!zip) {
        return Err("Unable to open zip file");
    }
    auto impl = std::make_unique<Zip::Impl>(zip, file);
    return Ok(Zip(std::move(impl)));
}

Zip::Path Zip::getPath() const {
    return m_impl->path();
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
