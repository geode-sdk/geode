#include <Geode/utils/file.hpp>
#include <Geode/utils/string.hpp>
#include <fstream>

USE_GEODE_NAMESPACE();

Result<std::string> utils::file::readString(std::string const& path) {
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
Result<std::string> utils::file::readString(std::wstring const& path) {
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

Result<byte_array> utils::file::readBinary(std::string const& path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in) {
        return Ok(byte_array (std::istreambuf_iterator<char>(in), {}));
    }
    return Err("Unable to open file");
}

#if _WIN32
Result<byte_array> utils::file::readBinary(std::wstring const& path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in) {
        return Ok(byte_array (std::istreambuf_iterator<char>(in), {}));
    }
    return Err("Unable to open file");
}
#endif

Result<byte_array> utils::file::readBinary(ghc::filesystem::path const& path) {
    #if _WIN32
    std::ifstream in(path.wstring(), std::ios::in | std::ios::binary);
    #else
    std::ifstream in(path.string(), std::ios::in | std::ios::binary);
    #endif
    if (in) {
        return Ok(byte_array (std::istreambuf_iterator<char>(in), {}));
    }
    return Err("Unable to open file");
}

Result<> utils::file::writeString(std::string const& path, std::string const& data) {
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
Result<> utils::file::writeString(std::wstring const& path, std::string const& data) {
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

        return Ok<>();
    }
    file.close();
    return Err<>("Unable to open file");
}

Result<> utils::file::writeBinary(std::string const& path, byte_array const& data) {
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
Result<> utils::file::writeBinary(std::wstring const& path, byte_array const& data) {
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

Result<> utils::file::writeBinary(ghc::filesystem::path const& path, byte_array const& data) {
    std::ofstream file;
    #if _WIN32
    file.open(path.wstring(), std::ios::out | std::ios::binary);
    #else
    file.open(path.string(), std::ios::out | std::ios::binary);
    #endif
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();

        return Ok<>();
    }
    file.close();
    return Err<>("Unable to open file");
}

Result<> utils::file::createDirectory(std::string const& path) {
    try {
        if (ghc::filesystem::create_directory(path)) {
            return Ok<>();
        }
    } catch(...) {}
    return Err<>("Unable to create directory");
}

Result<> utils::file::createDirectory(ghc::filesystem::path const& path) {
    try {
        if (ghc::filesystem::create_directory(path)) {
            return Ok<>();
        }
    } catch(...) {}
    return Err<>("Unable to create directory");
}

Result<> utils::file::createDirectoryAll(std::string const& path) {
    try {
        if (ghc::filesystem::create_directories(path)) {
            return Ok<>();
        }
    } catch(...) {}
    return Err<>("Unable to create directories");
}

Result<> utils::file::createDirectoryAll(ghc::filesystem::path const& path) {
    try {
        if (ghc::filesystem::create_directories(path)) {
            return Ok<>();
        }
    } catch(...) {}
    return Err<>("Unable to create directories");
}

Result<std::vector<std::string>> utils::file::listFiles(std::string const& path) {
    if (!ghc::filesystem::exists(path))
        return Err<>("Directory does not exist");

    std::vector<std::string> res;
    for (auto const& file : ghc::filesystem::directory_iterator(path)) {
        res.push_back(file.path().string());
    }
    return Ok<>(res);
}

Result<std::vector<std::string>> utils::file::listFilesRecursively(std::string const& path) {
    if (!ghc::filesystem::exists(path))
        return Err<>("Directory does not exist");
    
    std::vector<std::string> res;
    for (auto const& file : ghc::filesystem::recursive_directory_iterator(path)) {
        res.push_back(file.path().string());
    }
    return Ok<>(res);
}

Result<> utils::file::unzipTo(
    ghc::filesystem::path const& from,
    ghc::filesystem::path const& to
) {
    // unzip downloaded
    auto unzip = ZipFile(from.string());
    if (!unzip.isLoaded()) {
        return Err("Unable to unzip index.zip");
    }

    if (
        !ghc::filesystem::exists(to) &&
        !ghc::filesystem::create_directories(to)
    ) {
        return Err(
            "Unable to create directories \"" + 
            to.string() + "\""
        );
    }

    for (auto file : unzip.getAllFiles()) {
        // this is a very bad check for seeing 
        // if file is a directory. it seems to 
        // work on windows at least. idk why 
        // getAllFiles returns the directories 
        // aswell now
        if (
            utils::string::endsWith(file, "\\") ||
            utils::string::endsWith(file, "/")
        ) continue;

        auto zipPath = file;

        // dont include the github repo folder
        file = file.substr(file.find_first_of("/") + 1);

        auto path = ghc::filesystem::path(file);
        if (path.has_parent_path()) {
            auto dir = to / path.parent_path();
            if (
                !ghc::filesystem::exists(dir) &&
                !ghc::filesystem::create_directories(dir)
            ) {
                return Err(
                    "Unable to create directories \"" + 
                    dir.string() + "\""
                );
            }
        }
        unsigned long size;
        auto data = unzip.getFileData(zipPath, &size);
        if (!data || !size) {
            return Err("Unable to read \"" + std::string(zipPath) + "\"");
        }
        auto wrt = utils::file::writeBinary(
            to / file,
            byte_array(data, data + size)
        );
        if (!wrt) {
            return Err("Unable to write \"" + (to / file).string() + "\": " + wrt.error());
        }
    }

    return Ok();
}
