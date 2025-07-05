#include "hash.hpp"

// shh, its fine :-)
#include "sha3.cpp"

#include <string>
#include <fstream>
#include <ciso646>
#include "picosha2.h"
#include <vector>

template <class Func>
void readBuffered(std::ifstream& stream, Func func) {
    constexpr size_t BUF_SIZE = 4096;
    stream.exceptions(std::ios_base::badbit);

    std::vector<uint8_t> buffer(BUF_SIZE);
    while (true) {
        stream.read(reinterpret_cast<char*>(buffer.data()), BUF_SIZE);
        size_t amt = stream ? BUF_SIZE : stream.gcount();
        func(buffer.data(), amt);
        if (!stream) break;
    }
}

std::string calculateSHA3_256(std::filesystem::path const& path) {
    std::ifstream file(path, std::ios::binary);
    SHA3 sha;
    readBuffered(file, [&](const void* data, size_t amt) {
        sha.add(data, amt);
    });
    return sha.getHash();
}

std::string calculateSHA256(std::filesystem::path const& path) {
    std::vector<uint8_t> hash(picosha2::k_digest_size);
    std::ifstream file(path, std::ios::binary);
    picosha2::hash256(file, hash.begin(), hash.end());
    return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}

std::string calculateSHA256Text(std::filesystem::path const& path) {
    // remove all newlines
    std::vector<uint8_t> hash(picosha2::k_digest_size);
    std::ifstream file(path);
    std::string text;
    std::string line;
    while (std::getline(file, line)) {
        text += line;
    }
    picosha2::hash256(text.begin(), text.end(), hash.begin(), hash.end());
    return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}

std::string calculateHash(std::span<const uint8_t> data) {
    std::vector<uint8_t> hash(picosha2::k_digest_size);
    picosha2::hash256(data.begin(), data.end(), hash);
    return picosha2::bytes_to_hex_string(hash.begin(), hash.end());
}