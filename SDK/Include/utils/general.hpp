#pragma once

#include <string>
#include <chrono>

#include <Macros.hpp>
#include "Result.hpp"

namespace geode::utils {
    constexpr unsigned int hash(const char* str, int h = 0) {
        return !str[h] ? 5381 : (hash(str, h+1) * 33) ^ str[h];
    }
    
    constexpr unsigned int hash(const wchar_t* str, int h = 0) {
        return !str[h] ? 5381 : (hash(str, h+1) * 33) ^ str[h];
    }

    constexpr size_t operator"" _h (const char* txt, size_t) {
        return geode::utils::hash(txt);
    }

    constexpr size_t operator"" _h (const wchar_t* txt, size_t) {
        return geode::utils::hash(txt);
    }

    template<typename T>
    constexpr const T& clamp(const T& value, const T& minValue, const T& maxValue) {
        return value < minValue ? minValue : maxValue < value ? maxValue : value;
    }
    
    // from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
    template<typename ... Args>
    std::string strfmt(std::string const& format, Args ... args) {
        int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
        if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
        auto size = static_cast<size_t>( size_s );
        auto buf = std::make_unique<char[]>( size );
        std::snprintf( buf.get(), size, format.c_str(), args ... );
        return std::string( buf.get(), buf.get() + size - 1 );
    }

    /**
     * Format a string
     * @returns Pointer to char array. MAKE SURE TO CALL DELETE[]!
     */
    template<typename ... Args>
    const char* cstrfmt(const char* fmt, Args ... args) {
        auto str = strfmt(fmt, args...);
        char* ptr = new char[str.size() + 1];
        strcpy_s(ptr, str.c_str());
        return ptr;
    }

    GEODE_DLL std::string timePointAsString(const std::chrono::system_clock::time_point& tp);
}
