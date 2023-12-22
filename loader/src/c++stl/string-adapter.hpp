#pragma once
#include <Geode/c++stl/gdstdlib.hpp>

namespace geode::stl {
    struct StringImplAdapter {
        StringImpl& impl;

        // clear but assumes the existing impl is uninit,
        // so basically a default ctor
        void setEmpty();

        // frees the existing string
        void free();

        char* getStorage();
        void setStorage(const std::string_view);

        size_t getSize();
        void setSize(size_t);

        size_t getCapacity();
        void setCapacity(size_t);
    };
}