#pragma once
#include <string>
#include <string_view>
#include <asp/fs/Path.hpp>
#include <matjson.hpp>
#include <fmt/format.h>

namespace geode {
    template <typename C>
    requires std::same_as<C, char> || std::same_as<C, wchar_t> || std::same_as<C, char8_t> || std::same_as<C, char16_t> || std::same_as<C, char32_t>
    class BasicZStringView {
    public:
        using value_type = C;
        static constexpr size_t npos = std::basic_string_view<C>::npos;

        constexpr BasicZStringView() : BasicZStringView(nullptr) {}
        constexpr BasicZStringView(C const* str) {
            if (str) {
                m_str = str;
            } else {
                if constexpr (std::is_same_v<C, char>) {
                    m_str = "";
                } else if constexpr (std::is_same_v<C, wchar_t>) {
                    m_str = L"";
                } else if constexpr (std::is_same_v<C, char8_t>) {
                    m_str = u8"";
                } else if constexpr (std::is_same_v<C, char16_t>) {
                    m_str = u"";
                } else if constexpr (std::is_same_v<C, char32_t>) {
                    m_str = U"";
                }
            }
        }

        constexpr BasicZStringView(std::basic_string<C> const& str) : m_str(str.c_str()) {}

        constexpr BasicZStringView(BasicZStringView const& other) = default;
        constexpr BasicZStringView& operator=(BasicZStringView const& other) = default;

        constexpr explicit operator C const*() const {
            return m_str;
        }

        constexpr operator std::basic_string_view<C>() const {
            return this->view();
        }

        constexpr operator std::basic_string<C>() const {
            return std::basic_string<C>(m_str, this->size());
        }

        operator asp::fs::Path() const {
            return asp::fs::Path(this->view());
        }

        constexpr C const* c_str() const {
            return m_str;
        }

        constexpr C const* data() const {
            return m_str;
        }

        constexpr std::basic_string_view<C> view() const {
            return std::basic_string_view<C>(m_str, this->size());
        }

        constexpr size_t size() const {
            return std::char_traits<C>::length(m_str);
        }

        constexpr size_t length() const {
            return size();
        }

        constexpr bool empty() const {
            return m_str[0] == C(0);
        }

        constexpr auto begin() const {
            return m_str;
        }

        constexpr auto end() const {
            return m_str + this->size();
        }

        constexpr auto operator<=>(BasicZStringView const& other) const {
            return std::basic_string_view<C>(*this) <=> std::basic_string_view<C>(other);
        }

        constexpr bool operator==(BasicZStringView const& other) const {
            return std::basic_string_view<C>(*this) == std::basic_string_view<C>(other);
        }

        constexpr bool operator==(C const* other) const {
            return std::basic_string_view<C>(*this) == std::basic_string_view<C>(other);
        }

        // Methods from string_view
        // Note that we simply forward those to std::basic_string_view<C>, so it's a little inefficient,
        // as every call is an additional strlen(). But this is fine for most cases, users who care can call .view() beforehand.

        // Returns a reference to the character at the given position. Undefined behavior if pos >= size().
        constexpr C const& operator[](size_t pos) const {
            return m_str[pos];
        }

        // Returns a reference to the character at the given position. Throws std::out_of_range if pos >= size().
        constexpr C const& at(size_t pos) const {
            return this->view().at(pos);
        }

        // Returns a reference to the first character. Undefined behavior if empty().
        constexpr C const& front() const {
            return m_str[0];
        }

        // Returns a reference to the last character. Undefined behavior if empty().
        constexpr C const& back() const {
            return this->view().back();
        }

        // Returns a substring, note that this can only return `string_view` and not ZStringView due to lack of null termination
        constexpr std::basic_string_view<C> substr(size_t pos = 0, size_t count = std::basic_string_view<C>::npos) const {
            return this->view().substr(pos, count);
        }

        // Returns whether the string starts with the given prefix
        constexpr bool starts_with(std::basic_string_view<C> pfx) const {
            return this->view().starts_with(pfx);
        }

        // Returns whether the string ends with the given suffix
        constexpr bool ends_with(std::basic_string_view<C> sfx) const {
            return this->view().ends_with(sfx);
        }

        // Returns whether the string contains the given substring
        constexpr bool contains(std::basic_string_view<C> str) const {
            return this->view().contains(str);
        }

        // Returns the index of the first occurrence of the given substring, or npos if not found
        constexpr size_t find(std::basic_string_view<C> str, size_t pos = 0) const {
            return this->view().find(str, pos);
        }

        // Returns the index of the first occurrence of the given character, or npos if not found
        constexpr size_t find(C ch, size_t pos = 0) const {
            return this->view().find(ch, pos);
        }

        // Returns the index of the last occurrence of the given substring, or npos if not found
        constexpr size_t rfind(std::basic_string_view<C> str, size_t pos = std::basic_string_view<C>::npos) const {
            return this->view().rfind(str, pos);
        }

        // Returns the index of the last occurrence of the given character, or npos if not found
        constexpr size_t rfind(C ch, size_t pos = std::basic_string_view<C>::npos) const {
            return this->view().rfind(ch, pos);
        }

        // Returns the index of the first occurrence of any character in the given set, or npos if not found
        constexpr size_t find_first_of(std::basic_string_view<C> str, size_t pos = 0) const {
            return this->view().find_first_of(str, pos);
        }

        // Returns the index of the last occurrence of any character in the given set, or npos if not found
        constexpr size_t find_last_of(std::basic_string_view<C> str, size_t pos = std::basic_string_view<C>::npos) const {
            return this->view().find_last_of(str, pos);
        }

        // Returns the index of the first occurrence of any character not in the given set, or npos if not found
        constexpr size_t find_first_not_of(std::basic_string_view<C> str, size_t pos = 0) const {
            return this->view().find_first_not_of(str, pos);
        }

        // Returns the index of the first occurrence of any character not equal to the given character, or npos if not found
        constexpr size_t find_first_not_of(C ch, size_t pos = 0) const {
            return this->view().find_first_not_of(ch, pos);
        }

        // Returns the index of the last occurrence of any character not in the given set, or npos if not found
        constexpr size_t find_last_not_of(std::basic_string_view<C> str, size_t pos = std::basic_string_view<C>::npos) const {
            return this->view().find_last_not_of(str, pos);
        }

        // Returns the index of the last occurrence of any character not equal to the given character, or npos if not found
        constexpr size_t find_last_not_of(C ch, size_t pos = std::basic_string_view<C>::npos) const {
            return this->view().find_last_not_of(ch, pos);
        }

    private:
        C const* m_str;

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
    asp::fs::Path operator/(asp::fs::Path const& lhs, T&& rhs) {
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
    std::basic_string<C> operator+(C const* lhs, T&& rhs) {
        std::basic_string<C> out;
        out.reserve(std::char_traits<C>::length(lhs) + rhs.size());
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

    template <typename T, typename C>
    requires std::same_as<std::remove_cvref_t<T>, BasicZStringView<C>>
    std::basic_string<C> operator+(T&& lhs, C const* rhs) {
        std::basic_string<C> out;
        out.reserve(lhs.size() + std::char_traits<C>::length(rhs));
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

template <>
struct fmt::formatter<geode::ZStringView> : formatter<string_view> {
    auto format(geode::ZStringView const& str, fmt::format_context& ctx) const noexcept {
        return formatter<string_view>::format(
            str.view(), ctx
        );
    }
};
