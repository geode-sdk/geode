#include <simdutf.h>
#include <Geode/utils/base64.hpp>
#include <Geode/Prelude.hpp>

using namespace geode::prelude;
using simdutf::base64_options;

std::string base64::encode(std::span<std::uint8_t const> data, Base64Variant var) {
    base64_options opt;
    switch (var) {
        case Base64Variant::Normal: opt = base64_options::base64_default; break;
        case Base64Variant::NormalNoPad: opt = base64_options::base64_default_no_padding; break;
        case Base64Variant::Url: opt = base64_options::base64_url; break;
        case Base64Variant::UrlWithPad: opt = base64_options::base64_url_with_padding; break;
    }

    std::string buffer;
    buffer.resize(simdutf::base64_length_from_binary(data.size(), opt));
    // returned size is the same as above
    std::ignore = simdutf::binary_to_base64(data, buffer, opt);
    return buffer;
}

std::string base64::encode(std::string_view str, Base64Variant var) {
    return base64::encode(
        std::span(
            reinterpret_cast<std::uint8_t const*>(str.data()),
            reinterpret_cast<std::uint8_t const*>(str.data() + str.size())
        ),
        var
    );
}

template <class Container>
Result<Container> decodeImpl(std::string_view str, base64::Base64Variant var) {
    using namespace geode::utils::base64;

    if (auto i = str.find('\0'); i != -1) [[unlikely]] {
        str = str.substr(0, i);
    }
    // this may seem stupid but its what ziputils does
    if (auto i = str.find('='); i != -1) [[unlikely]] {
        str = str.substr(0, i);
    }

    base64_options opt;
    switch (var) {
        case Base64Variant::NormalNoPad:
        case Base64Variant::Normal:
            opt = base64_options::base64_default;
            break;
        case Base64Variant::UrlWithPad:
        case Base64Variant::Url:
            opt = base64_options::base64_url;
            break;
    }

    Container buffer;
    buffer.resize(simdutf::maximal_binary_length_from_base64(str.data(), str.size()));
    simdutf::result r = simdutf::base64_to_binary(str, buffer, opt);
    if (r.error) {
        if (r.error == simdutf::INVALID_BASE64_CHARACTER) {
            return Err("Invalid base64 character");
        }
        return Err("Error on decoding");
    } else {
        buffer.resize(r.count);
    }
    return Ok(std::move(buffer));
}

Result<std::vector<std::uint8_t>> base64::decode(std::string_view str, Base64Variant var) {
    return decodeImpl<std::vector<std::uint8_t>>(str, var);
}

Result<std::string> base64::decodeString(std::string_view str, Base64Variant var) {
    return decodeImpl<std::string>(str, var);
}
