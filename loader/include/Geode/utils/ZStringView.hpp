#pragma once
#include <string>
#include <string_view>
#include <filesystem>
#include <matjson.hpp>
#include <Geode/c++stl/string.hpp>
#include <fmt/format.h>

namespace geode {
    template <typename C>
    class BasicZStringView {
    public:
        using value_type = C;

        BasicZStringView() : m_str("") {}
        BasicZStringView(C const* str) : m_str(str ? str : "") {}
        BasicZStringView(std::basic_string<C> const& str) : m_str(str.c_str()), m_length(str.size()) {}

#ifdef GEODE_IS_ANDROID
        BasicZStringView(gd::string const& str) requires std::is_same_v<C, char> : m_str(str.c_str()), m_length(str.size()) {}
#endif

        BasicZStringView(BasicZStringView const& other) = default;
        BasicZStringView& operator=(BasicZStringView const& other) = default;

        explicit operator C const*() const {
            return m_str;
        }

        operator std::basic_string_view<C>() const {
            return this->view();
        }

        operator std::basic_string<C>() const {
            return std::basic_string<C>(m_str, _size());
        }

#ifdef GEODE_IS_ANDROID
        operator gd::string() const requires std::is_same_v<C, char> {
            return gd::string(m_str, _size());
        }
#endif

        operator std::filesystem::path() const {
            return std::filesystem::path(this->view());
        }

        C const* c_str() const {
            return m_str;
        }

        C const* data() const {
            return m_str;
        }

        std::basic_string_view<C> view() const {
            return std::basic_string_view<C>(m_str, _size());
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

        auto begin() const {
            return m_str;
        }

        auto end() const {
            return m_str + _size();
        }

        auto operator<=>(BasicZStringView const& other) const {
            return std::basic_string_view<C>(*this) <=> std::basic_string_view<C>(other);
        }

        bool operator==(BasicZStringView const& other) const {
            return std::basic_string_view<C>(*this) == std::basic_string_view<C>(other);
        }

        bool operator==(C const* other) const {
            return std::basic_string_view<C>(*this) == std::basic_string_view<C>(other);
        }

    private:
        C const* m_str;
        mutable size_t m_length = (size_t)-1;

        size_t _size() const {
            if (m_length == (size_t)-1) {
                m_length = std::char_traits<C>::length(m_str);
            }

            return m_length;
        }
        
        friend std::basic_ostream<C>& operator<<(std::basic_ostream<C>& os, BasicZStringView const& zsv) {
            os << zsv.view();
            return os;
        }
    };

    using ZStringView = BasicZStringView<char>;
    using WZStringView = BasicZStringView<wchar_t>;
    using U8ZStringView = BasicZStringView<char8_t>;
    using U16ZStringView = BasicZStringView<char16_t>;
    using U32ZStringView = BasicZStringView<char32_t>;

    template <typename T, typename C>
    requires std::same_as<std::remove_cvref_t<T>, BasicZStringView<C>>
    std::filesystem::path operator/(std::filesystem::path const& lhs, T&& rhs) {
        return lhs / rhs.view();
    }
    
    template <typename T, typename C>
    requires std::same_as<std::remove_cvref_t<T>, BasicZStringView<C>>
    std::basic_string<C> operator+(std::basic_string<C> const& lhs, T&& rhs) {
        std::basic_string<C> out;
        out.reserve(lhs.size() + rhs.size());
        out += lhs;
        out += rhs.view();
        return out;
    }
    
    template <typename T, typename C>
    requires std::same_as<std::remove_cvref_t<T>, BasicZStringView<C>>
    std::basic_string<C> operator+(T&& lhs, std::basic_string<C> const& rhs) {
        std::basic_string<C> out;
        out.reserve(lhs.size() + rhs.size());
        out += lhs.view();
        out += rhs;
        return out;
    }
}

template <>
struct matjson::Serialize<geode::ZStringView> {
    static matjson::Value toJson(geode::ZStringView const& value) {
        return matjson::Value(value.view());
    }
};

// inline std::string_view format_as(geode::ZStringView zsv) {
//     return std::string_view(zsv);
// }

template <>
struct fmt::formatter<geode::ZStringView> : formatter<string_view> {
    auto format(geode::ZStringView const& str, fmt::format_context& ctx) const noexcept {
        return formatter<string_view>::format(
            str.view(), ctx
        );
    }
};
