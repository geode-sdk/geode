#include <Geode/c++stl/gdstdlib.hpp>
#include "../../c++stl/string-impl.hpp"

#ifdef GEODE_IS_ANDROID

#if defined(GEODE_IS_ANDROID32)

static auto constexpr NEW_SYM = "_Znwj";
static constexpr uintptr_t MENULAYER_SCENE = 0x309068 - 0x10000;
static constexpr uintptr_t STRING_EMPTY = 0xaa1c48 - 0x10000 - 0xc; // the internal struct size

#elif defined(GEODE_IS_ANDROID64)

static auto constexpr NEW_SYM = "_Znwm";
static constexpr uintptr_t MENULAYER_SCENE = 0x6a62ec - 0x100000;
static constexpr uintptr_t STRING_EMPTY = 0x12d8568 - 0x100000 - 0x18; // the internal struct size

#endif

static auto constexpr DELETE_SYM = "_ZdlPv";

// 2.2 addition
// zmx please fix this

namespace geode::base {
    uintptr_t get() {
        static uintptr_t base = (reinterpret_cast<uintptr_t>(&MenuLayer::scene) - MENULAYER_SCENE) & (~0x1);
        // static uintptr_t base = reinterpret_cast<uintptr_t>(dlopen("libcocos2dcpp.so", RTLD_NOW));
        return base;
    }
}

static void* getLibHandle() {
    static void* handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD);
    return handle;
}

static void* gdOperatorNew(size_t size) {
    static auto fnPtr = reinterpret_cast<void*(*)(size_t)>(dlsym(getLibHandle(), NEW_SYM));
    return fnPtr(size);
}

static void gdOperatorDelete(void* ptr) {
    static auto fnPtr = reinterpret_cast<void(*)(void*)>(dlsym(getLibHandle(), DELETE_SYM));
    return fnPtr(ptr);
}

namespace geode::stl {
    static inline auto emptyInternalString() {
        return reinterpret_cast<StringData::Internal*>(
            geode::base::get() + STRING_EMPTY + sizeof(StringData::Internal)
        );
    }

    void StringImpl::setEmpty() {
        data.m_data = emptyInternalString();
    }

    void StringImpl::free() {
        if (data.m_data == nullptr || data.m_data == emptyInternalString()) return;

        if (data.m_data[-1].m_refcount <= 0) {
            gdOperatorDelete(&data.m_data[-1]);
            data.m_data = nullptr; 
        } else {
            --data.m_data[-1].m_refcount;
        }
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

        StringData::Internal internal;
        internal.m_size = str.size();
        internal.m_capacity = str.size();
        internal.m_refcount = 0;

        // use char* so we can do easy pointer arithmetic with it
        auto* buffer = static_cast<char*>(gdOperatorNew(str.size() + 1 + sizeof(internal)));
        std::memcpy(buffer, &internal, sizeof(internal));
        std::memcpy(buffer + sizeof(internal), str.data(), str.size());
        data.m_data = reinterpret_cast<StringData::Internal*>(buffer + sizeof(internal));

        this->getStorage()[str.size()] = 0;
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