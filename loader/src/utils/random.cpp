#include <Geode/utils/random.hpp>
#include <random>

namespace geode::utils::random {
    
std::array<uint64_t, 3>& _getState() {
    static thread_local std::array<uint64_t, 3> state = []{
        std::array<uint64_t, 3> seed;
        std::random_device rd;

        for (auto& s : seed) {
            auto a = rd(), b = rd();
            s = (uint64_t(a) << 32) ^ uint64_t(b);
        }
        return seed;
    }();

    return state;
}

static char genHexChar() {
    return "0123456789abcdef"[generate<size_t>(0, 16)];
}

std::string generateString(size_t length, std::string_view alphabet) {
    std::string out;
    out.reserve(length);
    for (size_t i = 0; i < length; i++) {
        out += alphabet[generate<size_t>(0, alphabet.size())];
    }
    return out;
}

std::string generateHexString(size_t length) {
    return generateString(length, "0123456789abcdef");
}

std::string generateAlphanumericString(size_t length) {
    return generateString(length, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
}

std::string generateUUID() {
    std::string out;
    out.reserve(36);

    auto pushChar = [&] { out += genHexChar(); };
    auto pushChars = [&](size_t n) {
        for (size_t i = 0; i < n; i++) {
            pushChar();
        }
    };

    pushChars(8);
    out += '-';
    pushChars(4);
    out += '-';
    out += '4';
    pushChars(3);
    out += '-';
    out += "89ab"[generate<size_t>(0, 4)];
    pushChars(3);
    out += '-';
    pushChars(12);

    return out;
}

void fillBytes(void* buffer, size_t size) {
    uint8_t* buf = (uint8_t*)buffer;

    // fill 8 bytes at a time for speed
    while (size >= 8) {
        uint64_t val = nextU64();
        std::memcpy(buf, &val, 8);
        buf += 8;
        size -= 8;
    }
    
    if (size > 0) {
        uint64_t val = nextU64();
        std::memcpy(buf, &val, size);
    }
}

}