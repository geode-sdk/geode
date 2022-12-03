#include <../support/zip_support/ZipUtils.h>
#include <../support/zip_support/ioapi.h>
#include <../support/zip_support/unzip.h>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/string.hpp>
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

Result<byte_array> utils::file::readBinary(ghc::filesystem::path const& path) {
#if _WIN32
    std::ifstream in(path.wstring(), std::ios::in | std::ios::binary);
#else
    std::ifstream in(path.string(), std::ios::in | std::ios::binary);
#endif
    if (in) {
        return Ok(byte_array(std::istreambuf_iterator<char>(in), {}));
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

Result<> utils::file::writeBinary(ghc::filesystem::path const& path, byte_array const& data) {
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

Result<bool> utils::file::createDirectory(ghc::filesystem::path const& path) {
    try {
        return Ok(ghc::filesystem::create_directory(path));
    }
    catch (...) {
        return Err("Unable to create directory");
    }
}

Result<bool> utils::file::createDirectoryAll(ghc::filesystem::path const& path) {
    try {
        return Ok(ghc::filesystem::create_directories(path));
    }
    catch (...) {
        return Err("Unable to create directories");
    }
}

Result<std::vector<std::string>> utils::file::listFiles(std::string const& path) {
    if (!ghc::filesystem::exists(path)) return Err("Directory does not exist");

    std::vector<std::string> res;
    for (auto const& file : ghc::filesystem::directory_iterator(path)) {
        res.push_back(file.path().string());
    }
    return Ok(res);
}

Result<std::vector<std::string>> utils::file::listFilesRecursively(std::string const& path) {
    if (!ghc::filesystem::exists(path)) return Err("Directory does not exist");

    std::vector<std::string> res;
    for (auto const& file : ghc::filesystem::recursive_directory_iterator(path)) {
        res.push_back(file.path().string());
    }
    return Ok(res);
}

Result<> utils::file::unzipTo(ghc::filesystem::path const& from, ghc::filesystem::path const& to) {
    GEODE_UNWRAP_INTO(auto unzip, Unzip::create(from));
    return unzip.extractAllTo(to);
}

static constexpr auto MAX_ENTRY_PATH_LEN = 256;

struct ZipEntry {
    unz_file_pos m_pos;
    ZPOS64_T m_compressedSize;
    ZPOS64_T m_uncompressedSize;
};

class file::UnzipImpl final {
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
                m_entries.insert({ fileName,
                                   ZipEntry {
                                       .m_pos = pos,
                                       .m_compressedSize = fileInfo.compressed_size,
                                       .m_uncompressedSize = fileInfo.uncompressed_size,
                                   } });
            }
            // Read next file, or break on error
            if (unzGoToNextFile64(m_zip, &fileInfo, fileName, sizeof(fileName) - 1) != UNZ_OK) {
                break;
            }
        }
        return true;
    }

    Result<byte_array> extract(Path const& name) {
        if (!m_entries.count(name)) {
            return Err("Entry not found");
        }

        auto entry = m_entries.at(name);

        if (unzGoToFilePos(m_zip, &entry.m_pos) != UNZ_OK) {
            return Err("Unable to navigate to entry");
        }
        if (unzOpenCurrentFile(m_zip) != UNZ_OK) {
            return Err("Unable to open entry");
        }
        byte_array res;
        res.resize(entry.m_uncompressedSize);
        auto size = unzReadCurrentFile(m_zip, res.data(), entry.m_uncompressedSize);
        if (size < 0 || size != entry.m_uncompressedSize) {
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

    UnzipImpl(unzFile zip, Path const& path) : m_zip(zip), m_zipPath(path) {}

    ~UnzipImpl() {
        unzClose(m_zip);
    }
};

Unzip::Unzip(UnzipImpl* impl) : m_impl(impl) {}

Unzip::~Unzip() {
    if (m_impl) {
        delete m_impl;
    }
}

Unzip::Unzip(Unzip&& other) : m_impl(other.m_impl) {
    other.m_impl = nullptr;
}

Result<Unzip> Unzip::create(Path const& file) {
    // todo: make sure unicode paths work
    auto zip = unzOpen(file.generic_string().c_str());
    if (!zip) {
        return Err("Unable to open zip file");
    }
    auto impl = new UnzipImpl(zip, file);
    if (!impl->loadEntries()) {
        delete impl;
        return Err("Unable to read zip file");
    }
    return Ok(Unzip(impl));
}

ghc::filesystem::path Unzip::getPath() const {
    return m_impl->path();
}

std::vector<ghc::filesystem::path> Unzip::getEntries() const {
    return map::getKeys(m_impl->entries());
}

bool Unzip::hasEntry(Path const& name) {
    return m_impl->entries().count(name);
}

Result<byte_array> Unzip::extract(Path const& name) {
    return m_impl->extract(name);
}

Result<> Unzip::extractTo(Path const& name, Path const& path) {
    GEODE_UNWRAP_INTO(auto bytes, m_impl->extract(name));
    GEODE_UNWRAP(file::writeBinary(path, bytes));
    return Ok();
}

Result<> Unzip::extractAllTo(Path const& dir) {
    GEODE_UNWRAP(file::createDirectoryAll(dir));
    for (auto& [entry, _] : m_impl->entries()) {
        GEODE_UNWRAP(this->extractTo(entry, dir / entry));
    }
    return Ok();
}
