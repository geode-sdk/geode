#pragma once

#include <string>
#include <span>
#include <string_view>
#include <vector>
#include <Geode/Result.hpp>
#include <Geode/platform/platform.hpp>

namespace geode::utils::base64 {
    enum class Base64Variant {
        Normal,
        NormalNoPad,
        Url,
        UrlWithPad,
    };

    /// Encodes the given data into base64, with the given variant.
    /// @param data Binary data to encode
    /// @param var Which variant of base64 to use
    /// @return The encoded base64 string
    GEODE_DLL std::string encode(std::span<std::uint8_t const> data, Base64Variant var = Base64Variant::UrlWithPad);

    /// Encodes the given data into base64, with the given variant.
    /// @param str Binary data to encode
    /// @param var Which variant of base64 to use
    /// @return The encoded base64 string
    /// @note Same as `encode(span<uint8_t>)`, just treats str as raw bytes.
    GEODE_DLL std::string encode(std::string_view str, Base64Variant var = Base64Variant::UrlWithPad);

    /// Decodes the given base64 string. Whitespace characters are skipped, and the string stops
    /// at the first null terminator or '=', as to match ZipUtils behavior.
    /// Padding is ignored, i.e. `Base64Variant::Normal` behaves the same as `Base64Variant::NormalNoPad`
    /// @param str Base64 encoded string
    /// @return Decoded data, or an error if decoding failed
    GEODE_DLL Result<std::vector<std::uint8_t>> decode(std::string_view str, Base64Variant var = Base64Variant::Url);

    /// Same as decode() but returns decoded data as a std::string. No validation is made if the
    /// string is "valid".
    /// @param str Base64 encoded string
    /// @return Decoded data, or an error if decoding failed
    GEODE_DLL Result<std::string> decodeString(std::string_view str, Base64Variant var = Base64Variant::Url);
}
