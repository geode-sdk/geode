#pragma once

#include <string>
#include <ghc/filesystem.hpp>

std::string calculateSHA3_256(ghc::filesystem::path const& path);

std::string calculateSHA256(ghc::filesystem::path const& path);

std::string calculateHash(ghc::filesystem::path const& path);
