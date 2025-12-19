#pragma once

#include <Geode/platform/platform.hpp>
#include <fmt/format.h>

namespace geode::utils {

template <typename C, size_t N = 1024> requires (N > 0)
class BasicStringBuffer {
public:
    using CharType = C;
    using StringType = std::basic_string<CharType>;
    using ViewType = std::basic_string_view<CharType>;
    using BufType = fmt::basic_memory_buffer<CharType, N>;

    BasicStringBuffer() = default;
    BasicStringBuffer(ViewType str) {
        this->append(str);
    }

    ViewType view() const {
        return { m_buffer.data(), this->size() };
    }

    StringType str() const {
        return { m_buffer.data(), this->size() };
    }

    CharType* c_str() {
        this->terminate();
        return m_buffer.data();
    }

    void append(CharType c) {
        m_buffer.push_back(c);
    }

    void append(ViewType fmtStr) {
        m_buffer.append(fmtStr.data(), fmtStr.data() + fmtStr.size());
    }

    template <typename T> requires (std::is_integral_v<T> && !std::is_same_v<T, CharType>)
    void append(T value) {
        this->append("{}" , value);
    }

    template <typename... Args>
    void append(fmt::format_string<Args...> fmtStr, Args&&... args) {
        fmt::format_to(
            std::back_inserter(m_buffer),
            fmtStr,
            std::forward<Args>(args)...
        );
    }

    /// Places a null terminator into the buffer. Don't use this if more data needs to be written.
    /// This is **not** required to be called, it's only necessary if you must have a null terminator.
    /// `c_str()` implicitly calls this function.
    void terminate() {
        if (!m_terminated) {
            m_buffer.push_back(CharType{0});
            m_terminated = true;
        }
    }

    /// Returns the total size of data written into the buffer,
    /// not including the null terminator (if one is present)
    size_t size() const {
        return m_buffer.size() - (size_t)m_terminated;
    }

private:
    BufType m_buffer;
    bool m_terminated = false;
};

template <size_t N = 1024>
using StringBuffer = BasicStringBuffer<char, N>;

}

template <typename C, size_t N>
struct fmt::formatter<geode::utils::BasicStringBuffer<C, N>> : formatter<string_view> {
    auto format(geode::utils::BasicStringBuffer<C, N> const& result, fmt::format_context& ctx) const noexcept {
        return formatter<string_view>::format(
            result.view(), ctx
        );
    }
};
