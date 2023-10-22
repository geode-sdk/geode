#include <Geode/c++stl/gdstdlib.hpp>

#ifdef GEODE_IS_ANDROID

namespace geode::base {
    uintptr_t get() {
        static uintptr_t base = (reinterpret_cast<uintptr_t>(&UILayer::create) - 0x20f168) & (~0x1);
        // static uintptr_t base = reinterpret_cast<uintptr_t>(dlopen("libcocos2dcpp.so", RTLD_NOW));
        return base;
    }
}

namespace gd {
    namespace {
        static inline auto emptyInternalString() {
            return reinterpret_cast<_internal_string*>(
                geode::base::get() + 0x75fb24 + sizeof(_internal_string)
            );
        }
    }

    string::string() : m_data(nullptr) {
        m_data = emptyInternalString();
    }

    string::string(char const* ok) : m_data(nullptr) {
        reinterpret_cast<void (*)(string*, char const*)>(geode::base::get() + 0x506c08)(this, ok);
    }

    string::string(string const& ok) : m_data(nullptr) {
        if (*(string**)(&ok) == nullptr) return;
        reinterpret_cast<void (*)(string*, string const&)>(geode::base::get() + 0x506634)(this, ok);
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

        reinterpret_cast<void (*)(string*)>(geode::base::get() + 0x5054bc)(this);
    }

    bool string::operator<(string const& other) const {
        return std::string(*this) < std::string(other);
    }

    bool string::operator==(string const& other) const {
        return std::string(*this) == std::string(other);
    }

    // TODO: these need to stay for old mods linking against geode <1.2.0, remove in 2.0.0
    template class map<int, int>;
    template class map<gd::string, gd::string>;
    template class map<gd::string, bool>;
    template class map<short, bool>;
}

#endif