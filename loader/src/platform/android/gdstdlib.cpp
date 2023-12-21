#include <Geode/c++stl/gdstdlib.hpp>

#ifdef GEODE_IS_ANDROID

// 2.2 addition
// zmx please fix this

namespace geode::base {
    uintptr_t get() {
        static uintptr_t base = (reinterpret_cast<uintptr_t>(&MenuLayer::scene) - (0x309068 - 0x10000)) & (~0x1);
        // static uintptr_t base = reinterpret_cast<uintptr_t>(dlopen("libcocos2dcpp.so", RTLD_NOW));
        return base;
    }
}

namespace gd {
    namespace {
        static inline auto emptyInternalString() {
            return reinterpret_cast<_internal_string*>(
                geode::base::get() + (0xaa1c3c - 0x10000) + sizeof(_internal_string)
            );
        }
    }

    string::string() : m_data(nullptr) {
        m_data = emptyInternalString();
    }

    string::string(char const* ok) : m_data(nullptr) {
        reinterpret_cast<void (*)(string*, char const*)>(geode::base::get() + (0x753a44 - 0x10000) + 1)(this, ok);
    }

    string::string(string const& ok) : m_data(nullptr) {
        if (*(string**)(&ok) == nullptr) return;
        reinterpret_cast<void (*)(string*, string const&)>(geode::base::get() + (0x7530e0 - 0x10000) + 1)(this, ok);
    }

    string& string::operator=(char const* ok) {
        this->~string();
        new (this) string(ok);
        return *this;
    }

    string& string::operator=(string const& ok) {
        this->~string();
        new (this) string(ok);
        return *this;
    }

    string::~string() {
        if (m_data == nullptr) return;

        reinterpret_cast<void (*)(string*)>(geode::base::get() + (0x7514c8 - 0x10000) + 1)(this);
    }

    bool string::operator<(string const& other) const {
        return std::string(*this) < std::string(other);
    }

    bool string::operator==(string const& other) const {
        return std::string(*this) == std::string(other);
    }
}

#endif