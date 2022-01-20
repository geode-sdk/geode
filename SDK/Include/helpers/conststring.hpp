#pragma once

#include <array>
#include <string_view>

using namespace std::string_view_literals;

namespace geode::utils {
    static constexpr const std::string_view sv_null = "\0";
    static constexpr const std::string_view sv_none = "";
    static constexpr const std::string_view sv_path = "/";

    // straight off https://stackoverflow.com/questions/38955940/how-to-concatenate-static-strings-at-compile-time
    template <std::string_view const& Separator, std::string_view const&... Strs>
    struct ConstStringJoin {
        static constexpr auto impl() noexcept {
            constexpr std::size_t len = (Strs.size() + ... + 0) + (sizeof...(Strs) - 1) * Separator.size();
            std::array<char, len + 1> arr{};
            auto append = [len, i = 0, &arr](auto const& s) mutable {
                for (auto c : s) arr[i++] = c;
                if (i < static_cast<int>(len - 1)) {
                    for (auto c : Separator) arr[i++] = c;
                }
            };
            (append(Strs), ...);
            arr[len] = 0;
            return arr;
        }
        static constexpr auto arr = impl();
        static constexpr std::string_view value {arr.data(), arr.size() - 1};
    };
    template <std::string_view const& Separator, std::string_view const&... Strs>
    static constexpr auto const_join_sep = ConstStringJoin<Separator, Strs...>::value;

    template <std::string_view const&... Strs>
    static constexpr auto const_join = ConstStringJoin<sv_none, Strs...>::value;

    template <std::string_view const&... Strs>
    static constexpr auto const_join_path = ConstStringJoin<
        sv_path, Strs...
    >::value;
    
    template <std::string_view const& Separator, std::string_view const&... Strs>
    static constexpr const char* const_join_sep_c_str = ConstStringJoin<
        Separator, Strs...
    >::value.data();

    template <std::string_view const&... Strs>
    static constexpr const char* const_join_c_str = ConstStringJoin<
        sv_none, Strs...
    >::value.data();

    template <std::string_view const&... Strs>
    static constexpr const char* const_join_path_c_str = ConstStringJoin<
        sv_path, Strs...
    >::value.data();
}
