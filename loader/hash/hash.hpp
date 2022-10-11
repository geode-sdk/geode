#pragma once

#include <string>
#include <fstream>
#include <ciso646>
#include "picosha3.h"
#include <vector>

static std::string calculateHash(std::string const& path) {
    std::vector<uint8_t> s(picosha3::bits_to_bytes(256));
    auto sha3_256 = picosha3::get_sha3_generator<256>();
    std::ifstream file(path, std::ios::binary);
    return sha3_256.get_hex_string(file);
}
