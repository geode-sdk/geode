#pragma once
#include <string>
#include <string_view>
#include <filesystem>
#include <Geode/c++stl/string.hpp>

namespace geode {
    class ZStringView {
    public:
        ZStringView() : m_str("") {}
        ZStringView(char const* str) : m_str(str ? str : "") {}
        ZStringView(std::string const& str) : m_str(str.c_str()), m_length(str.size()) {}

#ifdef GEODE_IS_ANDROID
        ZStringView(gd::string const& str) : m_str(str.c_str()), m_length(str.size()) {}
#endif

        ZStringView(ZStringView const& other) = default;
        ZStringView& operator=(ZStringView const& other) = default;

        explicit operator char const*() const {
            return m_str;
        }

        operator std::string_view() const {
            return this->view();
        }

        operator std::string() const {
            return std::string(m_str, _size());
        }

        operator std::filesystem::path() const {
            return std::filesystem::path(this->view());
        }

        char const* c_str() const {
            return m_str;
        }

        char const* data() const {
            return m_str;
        }

        std::string_view view() const {
            return std::string_view(m_str, _size());
        }

        size_t size() const {
            return _size();
        }
        
        size_t length() const {
            return _size();
        }

        bool empty() const {
            return m_str[0] == '\0';
        }

        bool operator==(ZStringView const& other) const {
            return std::string_view(*this) == std::string_view(other);
        }

        bool operator==(char const* other) const {
            return std::string_view(*this) == std::string_view(other);
        }

    private:
        char const* m_str;
        mutable size_t m_length = (size_t)-1;

        size_t _size() const {
            if (m_length == (size_t)-1) {
                m_length = std::char_traits<char>::length(m_str);
            }

            return m_length;
        }
    };

    inline std::string_view format_as(ZStringView zsv) {
        return std::string_view(zsv);
    }

    template <typename T>
    requires std::same_as<std::remove_cvref_t<T>, ZStringView>
    std::filesystem::path operator/(std::filesystem::path const& lhs, T&& rhs) {
        return lhs / std::string_view{rhs};
    }
    
    template <typename T>
    requires std::same_as<std::remove_cvref_t<T>, ZStringView>
    std::string operator+(std::string const& lhs, T&& rhs) {
        std::string out;
        out.reserve(lhs.size() + std::string_view{rhs}.size());
        out += lhs;
        out += std::string_view{rhs};
        return out;
    }
    
    template <typename T>
    requires std::same_as<std::remove_cvref_t<T>, ZStringView>
    std::string operator+(T&& lhs, std::string const& rhs) {
        std::string out;
        out.reserve(std::string_view{lhs}.size() + rhs.size());
        out += std::string_view{lhs};
        out += rhs;
        return out;
    }
}