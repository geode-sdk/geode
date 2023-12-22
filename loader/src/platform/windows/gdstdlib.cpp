#include "../../c++stl/string-adapter.hpp"

#ifdef GEODE_IS_WINDOWS

namespace geode::stl {
    void StringImplAdapter::setEmpty() {
        impl.m_size = 0;
        impl.m_capacity = 15;
        impl.m_smallStorage[0] = 0;
    }

    void StringImplAdapter::free() {
        if (impl.m_capacity > 15) {
            delete impl.m_bigStorage;
        }
    }

    char* StringImplAdapter::getStorage() {
        return impl.m_capacity <= 15 ? impl.m_smallStorage.data() : impl.m_bigStorage;
    }
    void StringImplAdapter::setStorage(const std::string_view str) {
        impl.m_size = impl.m_capacity = str.size();
        if (str.size() <= 15) {
            impl.m_capacity = 15;
        } else {
            impl.m_bigStorage = static_cast<char*>(operator new(str.size() + 1));
        }
        std::memcpy(getStorage(), str.data(), str.size());
        getStorage()[str.size()] = 0;
    }

    size_t StringImplAdapter::getSize() {
        return impl.m_size;
    }
    void StringImplAdapter::setSize(size_t size) {
        impl.m_size = size;
    }

    size_t StringImplAdapter::getCapacity() {
        return impl.m_capacity;
    }
    void StringImplAdapter::setCapacity(size_t cap) {
        impl.m_capacity = cap;
    }
}

#endif