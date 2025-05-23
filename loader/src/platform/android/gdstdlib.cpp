#include <Geode/c++stl/gdstdlib.hpp>
#include "../../c++stl/string-impl.hpp"
#include "internalString.hpp"
#include <assert.h>

#if defined(GEODE_IS_ANDROID32)
static auto constexpr NEW_SYM = "_Znwj";
#elif defined(GEODE_IS_ANDROID64)
static auto constexpr NEW_SYM = "_Znwm";
#endif

static auto constexpr DELETE_SYM = "_ZdlPv";

static void* getLibHandle() {
    static void* handle = dlopen("libcocos2dcpp.so", RTLD_LAZY | RTLD_NOLOAD);
    return handle;
}

namespace geode::base {
    uintptr_t get() {
        static std::uintptr_t basePtr = 0u;
        if (basePtr == 0u) {
            auto handle = getLibHandle();

            // JNI_OnLoad is present on all versions of GD
            auto sym = dlsym(handle, "JNI_OnLoad");
            assert(sym != nullptr);

            Dl_info p;
            auto dlAddrRes = dladdr(sym, &p);
            assert(dlAddrRes != 0);

            basePtr = reinterpret_cast<std::uintptr_t>(p.dli_fbase);
        }

        return basePtr;
    }
}

void* gd::operatorNew(size_t size) {
    static auto fnPtr = reinterpret_cast<void*(*)(size_t)>(dlsym(getLibHandle(), NEW_SYM));
    return fnPtr(size);
}

void gd::operatorDelete(void* ptr) {
    static auto fnPtr = reinterpret_cast<void(*)(void*)>(dlsym(getLibHandle(), DELETE_SYM));
    return fnPtr(ptr);
}

using namespace geode::stl;

void* g_ourInternalString = nullptr;

static auto& emptyInternalString() {
    static StringData::Internal* ptr = [] {
        StringData::Internal internal;
        internal.m_size = 0;
        internal.m_capacity = 0;
        // make our empty internal string different from gd's
        internal.m_refcount = 1'000'000'000;

        // use char* so we can do easy pointer arithmetic with it
        auto* buffer = static_cast<char*>(gd::operatorNew(sizeof(internal) + 1));
        std::memcpy(buffer, &internal, sizeof(internal));
        buffer[sizeof(internal)] = 0;
        g_ourInternalString = reinterpret_cast<void*>(buffer);
        return reinterpret_cast<StringData::Internal*>(buffer + sizeof(internal));
    }();
    return ptr;
}

void setEmptyInternalString(gd::string* str) {
    auto* internal = *reinterpret_cast<StringData::Internal**>(str);
    // make sure its empty
    if (internal[-1].m_size == 0 && internal[-1].m_capacity == 0 && internal[-1].m_refcount == 0) {
        emptyInternalString() = internal;
        g_ourInternalString = nullptr;
        // leak our internal string because we dont know if someone still has a pointer to it
        // its only like 20 bytes so who cares anyways
    }
}

namespace geode::stl {
    void StringImpl::setEmpty() {
        data.m_data = emptyInternalString();
    }

    void StringImpl::free() {
        if (data.m_data == nullptr || data.m_data == emptyInternalString()) return;

        if (data.m_data[-1].m_refcount <= 0) {
            gd::operatorDelete(&data.m_data[-1]);
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
    void StringImpl::setStorage(std::string_view str) {
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
        auto* buffer = static_cast<char*>(gd::operatorNew(str.size() + 1 + sizeof(internal)));
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
