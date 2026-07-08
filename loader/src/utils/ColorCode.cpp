#include <Geode/utils/ColorCode.hpp>

using namespace geode::prelude;

ColorCode::operator float() const {
    return m_a;
}

ColorCode::operator ccColor3B() const {
    return { m_r, m_g, m_b };
}

ColorCode::operator ccColor4B() const {
    return { m_r, m_g, m_b, m_a };
}

ColorCode::ColorCode(GLubyte a) : m_r(0x00), m_g(0x00), m_b(0x00), m_a(a) { }

ColorCode::ColorCode(std::string_view hex) : m_r(0x00), m_g(0x00), m_b(0x00), m_a(0x00) {
    this->extractChannels(hex);
}

ColorCode::ColorCode(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : m_r(r), m_g(g), m_b(b), m_a(a) { }

GLubyte ColorCode::getR() const {
    return m_r;
}

GLubyte ColorCode::getRed() const {
    return m_r;
}

GLubyte ColorCode::getG() const {
    return m_g;
}

GLubyte ColorCode::getGreen() const {
    return m_g;
}

GLubyte ColorCode::getB() const {
    return m_b;
}

GLubyte ColorCode::getBlue() const {
    return m_b;
}

GLubyte ColorCode::getA() const {
    return m_a;
}

GLubyte ColorCode::getAlpha() const {
    return m_a;
}

cocos2d::ccColor3B ColorCode::to3B() const {
    return *this;
}

cocos2d::ccColor4B ColorCode::to4B() const {
    return *this;
}


bool ColorCode::isInvisible() const {
    return m_a == 0x00;
}

bool ColorCode::isTransparent() const {
    return m_a > 0x00 && m_a < 0xFF;
}

bool ColorCode::isOpaque() const {
    return m_a == 0xFF;
}

void ColorCode::extractChannels(std::string_view hex) {
    const bool isPrefixed = hex.starts_with("#");

    if (hex.size() == isPrefixed) return;

    const std::string_view hexStr = hex.substr(isPrefixed);
    const size_t size = hexStr.size();

    // If the string is not hex and the size is not an exact size of 1, 2, 3, 4, 6 or 8
    if (hexStr.find_first_not_of("0123456789ABCDEFabcdef") != std::string_view::npos || (size > 4 && size != 6 && size != 8)) return;

    // If the size is 2 or 1 then all channels are equal and alpha is max
    if (size <= 2) {
        m_r = m_g = m_b = extractChannel(hexStr, 0, size == 1);
        m_a = 0xFF;
    } else {
        const bool isShort = size < 6;

        m_r = extractChannel(hexStr, 0, isShort);
        m_g = extractChannel(hexStr, 1, isShort);
        m_b = extractChannel(hexStr, 2, isShort);

        // If a 4th byte or nibble exists, extract the alpha channel, otherwise default to full alpha
        if (size % 4 == 0) {
            m_a = extractChannel(hexStr, 3, isShort);
        } else {
            m_a = 0xFF;
        }
    }
}

GLubyte ColorCode::extractChannel(std::string_view hexString, size_t index, bool isShort) const {
    const size_t channelSize = 1 + !isShort;
    const GLubyte byte = utils::numFromString<GLubyte>(hexString.substr(channelSize * index, channelSize), 16).unwrapOr(0);

    // If it's short, mirror the nibble
    return isShort ? (byte << 4) | byte : byte;
}
