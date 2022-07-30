#include <Geode/utils/file.hpp>
#include <fstream>

USE_GEODE_NAMESPACE();

Result<std::string> file_utils::readString(std::string const& path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
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

#if _WIN32
Result<std::string> file_utils::readString(std::wstring const& path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
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
#endif

Result<std::string> file_utils::readString(ghc::filesystem::path const& path) {
    std::ifstream in(path.string(), std::ios::in | std::ios::binary);
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

Result<byte_array> file_utils::readBinary(std::string const& path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in) {
        return Ok(byte_array (std::istreambuf_iterator<char>(in), {}));
    }
    return Err("Unable to open file");
}

#if _WIN32
Result<byte_array> file_utils::readBinary(std::wstring const& path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in) {
        return Ok(byte_array (std::istreambuf_iterator<char>(in), {}));
    }
    return Err("Unable to open file");
}
#endif

Result<byte_array> file_utils::readBinary(ghc::filesystem::path const& path) {
    std::ifstream in(path.string(), std::ios::in | std::ios::binary);
    if (in) {
        return Ok(byte_array (std::istreambuf_iterator<char>(in), {}));
    }
    return Err("Unable to open file");
}

Result<> file_utils::writeString(std::string const& path, std::string const& data) {
    std::ofstream file;
    file.open(path);
    if (file.is_open()) {
        file << data;
        file.close();

        return Ok<>();
    }
    file.close();
    return Err<>("Unable to open file");
}

#if _WIN32
Result<> file_utils::writeString(std::wstring const& path, std::string const& data) {
    std::ofstream file;
    file.open(path);
    if (file.is_open()) {
        file << data;
        file.close();

        return Ok<>();
    }
    file.close();
    return Err<>("Unable to open file");
}
#endif

Result<> file_utils::writeString(ghc::filesystem::path const& path, std::string const& data) {
    std::ofstream file;
    file.open(path.string());
    if (file.is_open()) {
        file << data;
        file.close();

        return Ok<>();
    }
    file.close();
    return Err<>("Unable to open file");
}

Result<> file_utils::writeBinary(std::string const& path, byte_array const& data) {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();

        return Ok<>();
    }
    file.close();
    return Err<>("Unable to open file");
}

#if _WIN32
Result<> file_utils::writeBinary(std::wstring const& path, byte_array const& data) {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();

        return Ok<>();
    }
    file.close();
    return Err<>("Unable to open file");
}
#endif

Result<> file_utils::writeBinary(ghc::filesystem::path const& path, byte_array const& data) {
    std::ofstream file;
    file.open(path.string(), std::ios::out | std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();

        return Ok<>();
    }
    file.close();
    return Err<>("Unable to open file");
}

Result<> file_utils::createDirectory(std::string const& path) {
    if (ghc::filesystem::create_directory(path))
        return Ok<>();
    return Err<>("Unable to create directory");
}

Result<> file_utils::createDirectoryAll(std::string const& path) {
    if (ghc::filesystem::create_directories(path))
        return Ok<>();
    return Err<>("Unable to create directories");
}

Result<std::vector<std::string>> file_utils::listFiles(std::string const& path) {
    if (!ghc::filesystem::exists(path))
        return Err<>("Directory does not exist");

    std::vector<std::string> res;
    for (auto const& file : ghc::filesystem::directory_iterator(path)) {
        res.push_back(file.path().string());
    }
    return Ok<>(res);
}

Result<std::vector<std::string>> file_utils::listFilesRecursively(std::string const& path) {
    if (!ghc::filesystem::exists(path))
        return Err<>("Directory does not exist");
    
    std::vector<std::string> res;
    for (auto const& file : ghc::filesystem::recursive_directory_iterator(path)) {
        res.push_back(file.path().string());
    }
    return Ok<>(res);
}
