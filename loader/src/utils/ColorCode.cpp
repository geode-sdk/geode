#include <Geode/utils/ColorCode.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;

Result<Color> Color::parse(std::string_view hex) {
    if (hex.starts_with("#")) {
        hex.remove_prefix(1);
    }

    size_t size = hex.size();

    // If the string is an exact size of 1, 2, 3, 4, 6 or 8
    if (!size || (size > 4 && size != 6 && size != 8)) {
        return Err("Unsupported size");
    }

    GEODE_UNWRAP_INTO(uint32_t colorNum, utils::numFromString<uint32_t>(hex, 16));
    ccColor4B color;

    if (size <= 2) {
        color.r = color.g = color.b = Color::getByte(colorNum, 0, size == 1);
        color.a = 0xFF;
    } else if (size % 3 == 0) {
        bool isShort = size == 3;

        color.r = Color::getByte(colorNum, 2, isShort);
        color.g = Color::getByte(colorNum, 1, isShort);
        color.b = Color::getByte(colorNum, 0, isShort);
        color.a = 0xFF;
    } else {
        bool isShort = size == 4;

        color.r = Color::getByte(colorNum, 3, isShort);
        color.g = Color::getByte(colorNum, 2, isShort);
        color.b = Color::getByte(colorNum, 1, isShort);
        color.a = Color::getByte(colorNum, 0, isShort);
    }

    return Ok(Color(color));
}

GLubyte Color::getByte(uint32_t colorNum, size_t index, bool isShort) {
    uint32_t shiftSize = (isShort ? 4 : 8) * index;
    GLubyte byte = (colorNum >> shiftSize) & 0xFF;

    // If it's short, mirror the nibble
    return isShort ? (byte << 4) | byte : byte;
}


Color::operator HSV() const {
    double red = r / 255.0;
    double green = g / 255.0;
    double blue = b / 255.0;
    double max = std::max({ red, green, blue });
    double delta = max - std::min({ red, green, blue });
    HSV hsv;

    if (delta == 0) {
        hsv.h = 0;
    } else if (max == red) {
        hsv.h = 60 * std::fmod((green - blue) / delta, 6);
    } else if (max == green) {
        hsv.h = 60 * ((blue - red) / delta + 2);
    } else {
        hsv.h = 60 * ((red - green) / delta + 4);
    }

    if (hsv.h < 0) {
        hsv.h += 360;
    }

    hsv.s = max == 0 ? 0 : delta / max;
    hsv.v = max;

    return hsv;
}

Color::Color(const HSV& hsv, GLubyte alpha): a(alpha) {
    double correctedHue = std::fmod(hsv.h, 360);
    double correctedSaturation = std::clamp(hsv.s, 0.0, 1.0);
    double correctedValue = std::clamp(hsv.v, 0.0, 1.0);

    if (correctedHue < 0) {
        correctedHue += 360;
    }

    double c = correctedValue * correctedSaturation;
    double x = c * (1 - std::abs(std::fmod(correctedHue / 60, 2) - 1));
    double m = correctedValue - c;
    double rp, gp, bp;

    if (correctedHue < 60) {
        rp = c; gp = x; bp = 0;
    } else if (correctedHue < 120) {
        rp = x; gp = c; bp = 0;
    } else if (correctedHue < 180) {
        rp = 0; gp = c; bp = x;
    } else if (correctedHue < 240) {
        rp = 0; gp = x; bp = c;
    } else if (correctedHue < 300) {
        rp = x; gp = 0; bp = c;
    } else {
        rp = c; gp = 0; bp = x;
    }

    r = std::round((rp + m) * 255);
    g = std::round((gp + m) * 255);
    b = std::round((bp + m) * 255);
}

void Color::applyHSV(const HSV& hsv) {
    HSV currentHSV = *this;

    currentHSV.h = std::fmod(currentHSV.h + hsv.h, 360);

    if (currentHSV.h < 0) {
        currentHSV.h += 360;
    }

    currentHSV.s = std::clamp<double>(currentHSV.s * hsv.s, 0, 1);
    currentHSV.v = std::clamp<double>(currentHSV.v * hsv.v, 0, 1);

    *this = currentHSV;
}