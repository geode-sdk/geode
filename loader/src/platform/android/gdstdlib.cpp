#include <Geode/c++stl/gdstdlib.hpp>
#include "../../c++stl/string-adapter.hpp"

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

namespace geode::stl {
    static inline auto emptyInternalString() {
        return reinterpret_cast<StringImpl::Internal*>(
            geode::base::get() + (0xaa1c3c - 0x10000) + sizeof(StringImpl::Internal)
        );
    }

    void StringImplAdapter::setEmpty() {
        impl.m_data = emptyInternalString();
    }

    void StringImplAdapter::free() {
        if (impl.m_data == nullptr) return;
        // TODO: reimplement this
        reinterpret_cast<void (*)(StringImpl*)>(geode::base::get() + (0x7514c8 - 0x10000) + 1)(&impl.m_data);
    }

    char* StringImplAdapter::getStorage() {
        return reinterpret_cast<char*>(impl.m_data);
    }
    void StringImplAdapter::setStorage(const std::string_view str) {
        this->free();
        // TODO: should be using char*, size_t at the very least, or yknow, just reimplement it :-)
        reinterpret_cast<void (*)(StringImpl*, char const*)>(geode::base::get() + (0x753a44 - 0x10000) + 1)(&impl.m_data, str.data());
    }

    size_t StringImplAdapter::getSize() {
        return impl.m_data[-1].m_size;
    }
    void StringImplAdapter::setSize(size_t size) {
        // TODO: implement this, remember its copy-on-write...
    }

    size_t StringImplAdapter::getCapacity() {
        return impl.m_data[-1].m_capacity;
    }
    void StringImplAdapter::setCapacity(size_t cap) {
        // TODO: implement this, remember its copy-on-write...
    }
}

#endif