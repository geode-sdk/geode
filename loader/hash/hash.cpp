#include "hash.hpp"

#include <string>
#include <fstream>
#include <ciso646>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/terminate.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;

static std::string hexEncode(const uint8_t* data, size_t size) {
    std::string str;
    str.reserve(size * 2);

    constexpr char hexChars[] = "0123456789abcdef";

    for (size_t i = 0; i < size; ++i) {
        str.push_back(hexChars[(data[i] >> 4) & 0x0F]);
        str.push_back(hexChars[data[i] & 0x0F]);
    }

    return str;
}

std::string calculateHash(std::span<const uint8_t> data) {
    uint8_t hash[SHA256_DIGEST_LENGTH];
    ::SHA256(data.data(), data.size(), hash);
    return hexEncode(hash, sizeof(hash));
}

std::string calculateHash(std::string_view data) {
    return calculateHash(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(data.data()), data.size()));
}

static Result<std::string> computeWithReader(auto&& fn) {
    auto context = std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>(EVP_MD_CTX_new(), &EVP_MD_CTX_free);
    if (!context) {
        return Err("EVP_MD_CTX_new failed");
    }

    if (EVP_DigestInit_ex(context.get(), EVP_sha256(), nullptr) != 1) {
        return Err("EVP_DigestInit_ex failed");
    }

    uint8_t buffer[4096];
    while (true) {
        GEODE_UNWRAP_INTO(auto read, fn(buffer, sizeof(buffer)));
        if (read == 0) {
            break;
        }

        if (EVP_DigestUpdate(context.get(), buffer, read) != 1) {
            return Err("EVP_DigestUpdate failed");
        }
    }

    uint8_t hash[SHA256_DIGEST_LENGTH];
    unsigned int hashLen = 0;
    if (EVP_DigestFinal_ex(context.get(), hash, &hashLen) != 1) {
        return Err("EVP_DigestFinal_ex failed");
    }

    return Ok(hexEncode(hash, hashLen));
}

std::string calculateSHA256(std::filesystem::path const& path) {
    std::ifstream file(path, std::ios::binary);

    auto result = computeWithReader([&](auto* buffer, size_t bufSize) -> Result<int64_t> {
        if (file.eof()) {
            return Ok(0);
        } else if (!file.good()) {
            return Err("failed to read from file", path);
        }

        file.read(reinterpret_cast<char*>(buffer), bufSize);
        return Ok(file.gcount());
    });

    if (!result) {
        log::error("Failed to compute SHA256 for file '{}': {}", path, result.unwrapErr());
        return "";
    }

    return std::move(result).unwrap();
}

std::string calculateSHA256Text(std::filesystem::path const& path) {
    // remove all newlines
    std::ifstream file(path);
    std::string text;
    std::string line;
    while (std::getline(file, line)) {
        text += line;
    }

    return calculateHash(text);
}
