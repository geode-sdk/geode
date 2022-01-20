#pragma once

#include <Macros.hpp>
#include <helpers/Result.hpp>
#include <string>
#include <helpers/types.hpp>
#include <fs/filesystem.hpp>

namespace geode::file_utils {
    GEODE_DLL Result<std::string> readString(std::string            const& path);
    GEODE_DLL Result<std::string> readString(std::wstring           const& path);
    GEODE_DLL Result<std::string> readString(ghc::filesystem::path  const& path);
    GEODE_DLL Result<byte_array>  readBinary(std::string            const& path);
    GEODE_DLL Result<byte_array>  readBinary(std::wstring           const& path);
    GEODE_DLL Result<byte_array>  readBinary(ghc::filesystem::path  const& path);

    GEODE_DLL Result<> writeString(std::string           const& path, std::string const& data);
    GEODE_DLL Result<> writeString(std::wstring          const& path, std::string const& data);
    GEODE_DLL Result<> writeString(ghc::filesystem::path const& path, std::string const& data);
    GEODE_DLL Result<> writeBinary(std::string           const& path, byte_array  const& data);
    GEODE_DLL Result<> writeBinary(std::wstring          const& path, byte_array  const& data);
    GEODE_DLL Result<> writeBinary(ghc::filesystem::path const& path, byte_array  const& data);

    GEODE_DLL Result<> createDirectory(std::string const& path);
    GEODE_DLL Result<> createDirectoryAll(std::string const& path);
    GEODE_DLL Result<std::vector<std::string>> listFiles(std::string const& path);
    GEODE_DLL Result<std::vector<std::string>> listFilesRecursively(std::string const& path);
}
