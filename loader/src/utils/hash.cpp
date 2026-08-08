#include <Geode/utils/hash.hpp>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/terminate.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/utils/file.hpp>

namespace geode {

Sha256Hasher::Sha256Hasher() {
    // In theory, this should never fail unless out of memory, in which case there's little we can do anyway
    m_state = EVP_MD_CTX_new();
    if (!m_state) {
        utils::terminate("EVP_MD_CTX_new failed");
    }

    this->reset();
}

void Sha256Hasher::reset() {
    EVP_DigestInit_ex(static_cast<EVP_MD_CTX*>(m_state), EVP_sha256(), nullptr);
}

Sha256Hasher::~Sha256Hasher() {
    EVP_MD_CTX_free(static_cast<EVP_MD_CTX*>(m_state));
}

Sha256Hasher::Sha256Hasher(Sha256Hasher&& other) noexcept {
    *this = std::move(other);
}

Sha256Hasher& Sha256Hasher::operator=(Sha256Hasher&& other) noexcept {
    if (this != &other) {
        EVP_MD_CTX_free(static_cast<EVP_MD_CTX*>(m_state));
        m_state = other.m_state;
        other.m_state = nullptr;
    }
    return *this;
}

void Sha256Hasher::update(std::span<uint8_t const> data) {
    update(data.data(), data.size());
}

void Sha256Hasher::update(void const* data, size_t bytes) {
    EVP_DigestUpdate(static_cast<EVP_MD_CTX*>(m_state), data, bytes);
}

void Sha256Hasher::update(std::string_view data) {
    update(data.data(), data.size());
}

Sha256 Sha256Hasher::finish() const {
    Sha256 hash;
    EVP_DigestFinal_ex(static_cast<EVP_MD_CTX*>(m_state), hash.data.data(), nullptr);
    return hash;
}

Result<Sha256> sha256File(std::filesystem::path const& path) {
    Sha256Hasher hasher;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return Err("Failed to open file '{}' for reading", path);
    }

    uint8_t buf[4096];
    while (file.good()) {
        file.read(reinterpret_cast<char*>(buf), sizeof(buf));
        auto read = file.gcount();

        if (read > 0) {
            hasher.update(buf, read);
        }
    }

    return Ok(hasher.finish());
}

Sha256 sha256(std::span<uint8_t const> data) {
    Sha256Hasher hasher;
    hasher.update(data);
    return hasher.finish();
}

Sha256 sha256(std::string_view data) {
    return sha256(std::span{reinterpret_cast<const uint8_t*>(data.data()), data.size()});
}

}
