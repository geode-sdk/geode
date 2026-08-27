#include <Geode/utils/general.hpp>
#include <fast_float/fast_float.h>

using namespace geode::prelude;

#ifndef GEODE_IS_MACOS
// feel free to properly implement this for other platforms
float geode::utils::getDisplayFactor() {
    return 1.0f;
}
#endif

template <typename T>
static Result<T> toParseResult(T* value, fast_float::from_chars_result res, std::string_view str) {
    auto [ptr, ec] = res;
    if (ec == std::errc()) return Ok(*value);
    else if (ptr != str.data() + str.size()) return Err("String contains trailing extra data");
    else if (ec == std::errc::invalid_argument) return Err("String is not a number");
    else if (ec == std::errc::result_out_of_range) return Err("Number is out of range for target type");
    else return Err("Unknown error");
}

template <typename T>
static Result<T> parseFloat(std::string_view str) {
    T result;
    auto res = fast_float::from_chars(str.data(), str.data() + str.size(), result);
    return toParseResult(&result, res, str);
}

template <typename T>
static Result<T> parseInt(std::string_view str, int base = 10) {
    T result;
    auto res = fast_float::from_chars(str.data(), str.data() + str.size(), result, base);
    return toParseResult(&result, res, str);
}

Result<float> geode::utils::_detail::floatFromString(std::string_view str) {
    return parseFloat<float>(str);
}

Result<double> geode::utils::_detail::doubleFromString(std::string_view str) {
    return parseFloat<double>(str);
}

Result<long double> geode::utils::_detail::longDoubleFromString(std::string_view str) {
    // ld not supported by fast float
    return parseFloat<double>(str).map([](double val) {
        return static_cast<long double>(val);
    });
}

Result<uint64_t> geode::utils::_detail::uint64FromString(std::string_view str, int base) {
    return parseInt<uint64_t>(str, base);
}

Result<int64_t> geode::utils::_detail::int64FromString(std::string_view str, int base) {
    return parseInt<int64_t>(str, base);
}
