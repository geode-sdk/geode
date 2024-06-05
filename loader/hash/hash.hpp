#pragma once

#include <string>
#include <filesystem>

std::string calculateSHA3_256(std::filesystem::path const& path);

std::string calculateSHA256(std::filesystem::path const& path);

std::string calculateSHA256Text(std::filesystem::path const& path);

std::string calculateHash(std::filesystem::path const& path);
