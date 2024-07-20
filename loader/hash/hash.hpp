#pragma once

#include <string>
#include <filesystem>
#include <span>

std::string calculateSHA3_256(std::filesystem::path const& path);

std::string calculateSHA256(std::filesystem::path const& path);

std::string calculateSHA256Text(std::filesystem::path const& path);

/**
 * Calculates the SHA256 hash of the given data,
 * used for verifying mods.
 */
std::string calculateHash(std::span<const uint8_t> data);
