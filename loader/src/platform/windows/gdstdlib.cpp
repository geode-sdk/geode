#include "../../c++stl/string-impl.hpp"

#ifdef GEODE_IS_WINDOWS

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

#endif