#include <Geode/utils/general.hpp>
#include <fast_float/fast_float.h>

using namespace geode::prelude;

#ifndef GEODE_IS_MACOS
// feel free to properly implement this for other platforms
float geode::utils::getDisplayFactor() {
    return 1.0f;
}
#endif

std::string geode::utils::NumParseError::toString() const {
    switch (m_value) {
        case Unknown:
            return "Unknown error";
        case TrailingData:
            return "String contains trailing extra data";
        case NotANumber:
            return "String is not a number";
        case OutOfRange:
            return "Number is out of range for target type";
    }
}

template <typename T>
static Result<T, NumParseError> toParseResult(T* value, fast_float::from_chars_result res, std::string_view str) {
    auto [ptr, ec] = res;
    if (ec == std::errc()) return Ok(*value);
    else if (ptr != str.data() + str.size()) return Err(NumParseError::TrailingData);
    else if (ec == std::errc::invalid_argument) return Err(NumParseError::NotANumber);
    else if (ec == std::errc::result_out_of_range) return Err(NumParseError::OutOfRange);
    else return Err(NumParseError::Unknown);
}

template <typename T>
static Result<T, NumParseError> parseFloat(std::string_view str) {
    T result;
    auto res = fast_float::from_chars(str.data(), str.data() + str.size(), result);
    return toParseResult(&result, res, str);
}

template <typename T>
static Result<T, NumParseError> parseInt(std::string_view str, int base = 10) {
    T result;
    auto res = fast_float::from_chars(str.data(), str.data() + str.size(), result, base);
    return toParseResult(&result, res, str);
}

Result<float, NumParseError> geode::utils::_detail::floatFromString(std::string_view str) {
    return parseFloat<float>(str);
}

Result<double, NumParseError> geode::utils::_detail::doubleFromString(std::string_view str) {
    return parseFloat<double>(str);
}

Result<long double, NumParseError> geode::utils::_detail::longDoubleFromString(std::string_view str) {
    // ld not supported by fast float
    return parseFloat<double>(str).map([](double val) {
        return static_cast<long double>(val);
    });
}

Result<uint64_t, NumParseError> geode::utils::_detail::uint64FromString(std::string_view str, int base) {
    return parseInt<uint64_t>(str, base);
}

Result<int64_t, NumParseError> geode::utils::_detail::int64FromString(std::string_view str, int base) {
    return parseInt<int64_t>(str, base);
}
