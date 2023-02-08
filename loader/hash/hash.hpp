#pragma once

#include <string>
#include <fstream>
#include <ciso646>
#include "picosha3.h"
#include "picosha2.h"
#include <vector>
#include <ghc/filesystem.hpp>

static std::string calculateSHA3_256(ghc::filesystem::path const& path) {
    std::vector<uint8_t> s(picosha3::bits_to_bytes(256));
    auto sha3_256 = picosha3::get_sha3_generator<256>();
    std::ifstream file(path, std::ios::binary);
    return sha3_256.get_hex_string(file);
}

static std::string calculateSHA256(ghc::filesystem::path const& path) {
    std::vector<uint8_t> hash(picosha2::k_digest_size);
    std::ifstream file(path, std::ios::binary);
    picosha2::hash256(file, hash.begin(), hash.end());
    return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}

static std::string calculateHash(ghc::filesystem::path const& path) {
    return calculateSHA3_256(path);
}
