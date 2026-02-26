#pragma once
#include <Geode/c++stl/gdstdlib.hpp>

namespace geode::stl {
    struct StringImpl {
        StringData& data;

        // clear but assumes the existing data is uninit,
        // so basically a default ctor
        void setEmpty();

        // frees the existing string
        void free();

        char* getStorage();
        void setStorage(std::string_view);
        void setStorage(StringData&);
        void swapStorage(StringData&);

        size_t getSize();
        void setSize(size_t, char);

        size_t getCapacity();
        void setCapacity(size_t);

        void append(std::string_view);
        void append(size_t, char);
        void insert(size_t, std::string_view);
        void insert(size_t, size_t, char);
        void erase(size_t, size_t);
        void replace(size_t, size_t, std::string_view);
        void replace(size_t, size_t, size_t, char);
    };
}