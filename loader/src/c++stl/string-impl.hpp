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
        void setStorage(std::string_view str);
        void setStorage(StringData& other);
        void swapStorage(StringData& other);

        size_t getSize();
        void setSize(size_t size, char fill);

        size_t getCapacity();
        void setCapacity(size_t capacity);

        void append(std::string_view str);
        void append(size_t count, char c);
        void insert(size_t pos, std::string_view str);
        void insert(size_t pos, size_t count, char c);
        void erase(size_t pos, size_t count);
        void replace(size_t pos, size_t count, std::string_view str);
        void replace(size_t pos, size_t count, size_t count2, char c);
    };
}