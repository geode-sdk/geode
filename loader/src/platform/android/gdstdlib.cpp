#include <Geode/c++stl/gdstdlib.hpp>
#include "../../c++stl/string-impl.hpp"

#ifdef GEODE_IS_ANDROID32

// 2.2 addition
// zmx please fix this

namespace geode::base {
    uintptr_t get() {
        static uintptr_t base = (reinterpret_cast<uintptr_t>(&MenuLayer::scene) - (0x309068 - 0x10000)) & (~0x1);
        // static uintptr_t base = reinterpret_cast<uintptr_t>(dlopen("libcocos2dcpp.so", RTLD_NOW));
        return base;
    }
}

namespace geode::stl {
    static inline auto emptyInternalString() {
        return reinterpret_cast<StringData::Internal*>(
            geode::base::get() + (0xaa1c3c - 0x10000) + sizeof(StringData::Internal)
        );
    }

    void StringImpl::setEmpty() {
        data.m_data = emptyInternalString();
    }

    void StringImpl::free() {
        if (data.m_data == nullptr || data.m_data == emptyInternalString()) return;
        // TODO: reimplement this
        reinterpret_cast<void (*)(StringData*)>(geode::base::get() + (0x7514c8 - 0x10000) + 1)(&data);

        
    }

    char* StringImpl::getStorage() {
        return reinterpret_cast<char*>(data.m_data);
    }
    // TODO: add a copyFrom(string const&) to take advantage
    // of gnustl refcounted strings
    void StringImpl::setStorage(const std::string_view str) {
        this->free();

        if (str.size() == 0) {
            this->setEmpty();
            return;
        }

        // TODO: should be using (char*, size_t) at the very least, or yknow, just reimplement it :-)
        reinterpret_cast<void (*)(StringData*, char const*)>(geode::base::get() + (0x753a44 - 0x10000) + 1)(&data, str.data());
        return;

        // TODO: this crashes because we need to use gd's operator new...
#if 0
        StringData::Internal internal;
        internal.m_size = str.size();
        internal.m_capacity = str.size();
        internal.m_refcount = 0;

        auto* buffer = static_cast<char*>(operator new(str.size() + 1 + sizeof(internal)));
        std::memcpy(buffer, &internal, sizeof(internal));
        std::memcpy(buffer + sizeof(internal), str.data(), str.size());
        buffer[sizeof(internal) + str.size()] = 0;

        data.m_data = reinterpret_cast<StringData::Internal*>(buffer + sizeof(internal));
#endif
    }

    size_t StringImpl::getSize() {
        return data.m_data[-1].m_size;
    }
    void StringImpl::setSize(size_t size) {
        // TODO: implement this, remember its copy-on-write...
    }

    size_t StringImpl::getCapacity() {
        return data.m_data[-1].m_capacity;
    }
    void StringImpl::setCapacity(size_t cap) {
        // TODO: implement this, remember its copy-on-write...
    }
}

#endif