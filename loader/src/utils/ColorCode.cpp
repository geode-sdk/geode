#include <Geode/utils/ColorCode.hpp>

using namespace geode::prelude;

Result<Color> Color::parse(std::string_view hex) {
    const bool isPrefixed = hex.starts_with("#");

    if (hex.size() == isPrefixed) {
        return Err("Empty color string");
    }

    const std::string_view hexStr = hex.substr(isPrefixed);
    const size_t size = hexStr.size();

    // If the string is not hex and the size is not an exact size of 1, 2, 3, 4, 6 or 8
    if (hexStr.find_first_not_of("0123456789ABCDEFabcdef") != std::string_view::npos) {
        return Err("Non base16 characters");
    } else if (size > 4 && size != 6 && size != 8) {
        return Err("Unsupported size");
    }

    GEODE_UNWRAP_INTO(const uint32_t colorNum, utils::numFromString<uint32_t>(hexStr, 16));
    ccColor4B color;

    if (size <= 2) {
        color.r = color.g = color.b = Color::getByte(colorNum, 0, size == 1);
        color.a = 0xFF;
    } else if (size % 3 == 0) {
        const bool isShort = size == 3;

        color.r = Color::getByte(colorNum, 2, isShort);
        color.g = Color::getByte(colorNum, 1, isShort);
        color.b = Color::getByte(colorNum, 0, isShort);
        color.a = 0xFF;
    } else {
        const bool isShort = size == 4;

        color.r = Color::getByte(colorNum, 3, isShort);
        color.g = Color::getByte(colorNum, 2, isShort);
        color.b = Color::getByte(colorNum, 1, isShort);
        color.a = Color::getByte(colorNum, 0, isShort);
    }

    return Ok(Color(color));
}

GLubyte Color::getByte(uint32_t colorNum, size_t index, bool isShort) {
    const GLubyte byte = colorNum >> (isShort ? 4 : 8) * index & 0xFF;

    // If it's short, mirror the nibble
    return isShort ? (byte << 4) | byte : byte;
}


Color::operator float() const {
    return a;
}

Color::operator ccColor3B() const {
    return { r, g, b };
}

Color::operator ccColor4B() const {
    return { r, g, b, a };
}

Color::operator ccColor4F() const {
    return { static_cast<GLfloat>(r / 255.0), static_cast<GLfloat>(g / 255.0), static_cast<GLfloat>(b / 255.0), static_cast<GLfloat>(a / 255.0) };
}

Color::operator HSV() const {
    const double red = r / 255.0;
    const double green = g / 255.0;
    const double blue = b / 255.0;
    const double max = std::max({ red, green, blue });
    const double delta = max - std::min({ red, green, blue });
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

Color::Color(GLubyte alpha): r(0x00), g(0x00), b(0x00), a(alpha) { }

Color::Color(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha): r(red), g(green), b(blue), a(alpha) { }

Color::Color(const ccColor3B& color, GLubyte alpha): r(color.r), g(color.g), b(color.b), a(alpha) { }

Color::Color(const ccColor4B& color): r(color.r), g(color.g), b(color.b), a(color.a) { }

Color::Color(const ccColor4F& color): r(std::round(color.r * 255)), g(std::round(color.g * 255)), b(std::round(color.b * 255)), a(std::round(color.a * 255)) { }

Color::Color(const HSV& hsv, GLubyte alpha): a(alpha) {
    double correctedHue = std::fmod(hsv.h, 360);

    if (correctedHue < 0) {
        correctedHue += 360;
    }

    const double c = hsv.v * hsv.s;
    const double x = c * (1 - std::abs(std::fmod(correctedHue / 60, 2) - 1));
    const double m = hsv.v - c;
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

void Color::applyH(double hue) {
    this->applyHSV({ hue, 1, 1 });
}

void Color::applyHue(double hue) {
    this->applyHSV({ hue, 1, 1 });
}

void Color::applyS(double saturation) {
    this->applyHSV({ 0, saturation, 1 });
}

void Color::applySaturation(double saturation) {
    this->applyHSV({ 0, saturation, 1 });
}

void Color::applyV(double value) {
    this->applyHSV({ 0, 1, value });
}

void Color::applyValue(double value) {
    this->applyHSV({ 0, 1, value });
}

void Color::applyB(double brightness) {
    this->applyHSV({ 0, 1, brightness });
}

void Color::applyBrightness(double brightness) {
    this->applyHSV({ 0, 1, brightness });
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

void Color::setR(GLubyte red) {
    r = red;
}

GLubyte Color::getR() const {
    return r;
}

void Color::setRed(GLubyte red) {
    r = red;
}

GLubyte Color::getRed() const {
    return r;
}

void Color::setG(GLubyte green) {
    g = green;
}

GLubyte Color::getG() const {
    return g;
}

void Color::setGreen(GLubyte green) {
    g = green;
}

GLubyte Color::getGreen() const {
    return g;
}

void Color::setB(GLubyte blue) {
    b = blue;
}

GLubyte Color::getB() const {
    return b;
}

void Color::setBlue(GLubyte blue) {
    b = blue;
}

GLubyte Color::getBlue() const {
    return b;
}

void Color::setA(GLubyte alpha) {
    a = alpha;
}

GLubyte Color::getA() const {
    return a;
}

void Color::setAlpha(GLubyte alpha) {
    a = alpha;
}

GLubyte Color::getAlpha() const {
    return a;
}

ccColor3B Color::to3B() const {
    return *this;
}

ccColor4B Color::to4B() const {
    return *this;
}

ccColor4F Color::to4F() const {
    return *this;
}

HSV Color::toHSV() const {
    return *this;
}

bool Color::isInvisible() const {
    return a == 0x00;
}

bool Color::isTransparent() const {
    return a > 0x00 && a < 0xFF;
}

bool Color::isOpaque() const {
    return a == 0xFF;
}