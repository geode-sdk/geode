#include "../../c++stl/msvc/allocator.hpp"
#include "../../c++stl/string-impl.hpp"

static auto constexpr NEW_SYM = "??2@YAPAXI@Z";
static auto constexpr DELETE_SYM = "??3@YAXPAX@Z";

static void* getFn(const char* sym) {
    auto msvcr = GetModuleHandleW(L"MSVCR120.dll");
    if (msvcr != NULL)
        return reinterpret_cast<void*>(GetProcAddress(msvcr, sym));

    return nullptr;
}

void* geode::stl::operatorNew(size_t size) {
    static auto fnPtr = reinterpret_cast<void*(*)(size_t)>(getFn(NEW_SYM));
    return fnPtr(size);
}

void geode::stl::operatorDelete(void* ptr) {
    static auto fnPtr = reinterpret_cast<void(*)(void*)>(getFn(DELETE_SYM));
    return fnPtr(ptr);
}

namespace geode::stl {
    void StringImpl::setEmpty() {
        data.m_size = 0;
        data.m_capacity = 15;
        data.m_smallStorage[0] = 0;
    }

    void StringImpl::free() {
        if (data.m_capacity > 15) {
            delete data.m_bigStorage;
        }
    }

    char* StringImpl::getStorage() {
        return data.m_capacity <= 15 ? data.m_smallStorage.data() : data.m_bigStorage;
    }
    void StringImpl::setStorage(const std::string_view str) {
        data.m_size = data.m_capacity = str.size();
        if (str.size() <= 15) {
            data.m_capacity = 15;
        } else {
            data.m_bigStorage = static_cast<char*>(operator new(str.size() + 1));
        }
        std::memcpy(getStorage(), str.data(), str.size());
        getStorage()[str.size()] = 0;
    }

    size_t StringImpl::getSize() {
        return data.m_size;
    }
    void StringImpl::setSize(size_t size) {
        data.m_size = size;
    }

    size_t StringImpl::getCapacity() {
        return data.m_capacity;
    }
    void StringImpl::setCapacity(size_t cap) {
        data.m_capacity = cap;
    }
}
