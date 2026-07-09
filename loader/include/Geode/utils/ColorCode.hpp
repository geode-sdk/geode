#pragma once

#include <string_view>
#include <ccTypes.h>
#include <Geode/Result.hpp>

namespace geode {
    template<typename T>
    concept CanSetColor3B = requires(T object) {
        object.setColor(cocos2d::ccColor3B());
        object.setOpacity(GLubyte());
    };
    template<typename T>
    concept CanSetColor4B = requires(T object) { object.setColor(cocos2d::ccColor4B()); };
    template<typename T>
    concept CanSetColor4F = requires(T object) { object.setColor(cocos2d::ccColor4F()); };

    struct GEODE_DLL Color final {
        /// Parses the given hex string using one of the following formats:
        /// #R/G/B, #RR/GG/BB, #RGB, #RGBA, #RRGGBB or #RRGGBBAA
        /// Where the # prefix is optional and / means it's 1 byte/nibble representing all channels
        /// @returns The parsed result
        static Result<Color> parse(std::string_view hex);
    private:
        static GLubyte getByte(uint32_t colorNum, size_t index, bool isShort);
    public:
        constexpr operator cocos2d::ccColor3B() const {
            return { r, g, b };
        }

        constexpr operator cocos2d::ccColor4B() const {
            return { r, g, b, a };
        }

        constexpr operator cocos2d::ccColor4F() const {
            return { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
        }

        operator cocos2d::extension::HSV() const;
        Color& operator=(Color&& other) noexcept = default;
        Color& operator=(const Color& other) noexcept = default;

        GLubyte r;
        GLubyte g;
        GLubyte b;
        GLubyte a;

        Color(Color&& other) noexcept = default;
        Color(const Color& other) noexcept = default;
        /// @returns The equivalent of #000000{a}
        constexpr Color(GLubyte alpha = 0): r(0x00), g(0x00), b(0x00), a(alpha) { }

        constexpr Color(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha = 0xFF): r(red), g(green), b(blue), a(alpha) { }

        constexpr Color(const cocos2d::ccColor3B& color, GLubyte alpha = 0xFF): r(color.r), g(color.g), b(color.b), a(alpha) { }

        constexpr Color(const cocos2d::ccColor4B& color): r(color.r), g(color.g), b(color.b), a(color.a) { }

        Color(const cocos2d::ccColor4F& color):
            r(std::round(std::clamp(color.r, 0.0f, 1.0f) * 255)),
            g(std::round(std::clamp(color.g, 0.0f, 1.0f) * 255)),
            b(std::round(std::clamp(color.b, 0.0f, 1.0f) * 255)),
            a(std::round(std::clamp(color.a, 0.0f, 1.0f) * 255)) { }

        Color(const cocos2d::extension::HSV& hsv, GLubyte alpha = 0xFF);

        void applyH(double hue) {
            this->applyHSV({ hue, 1, 1 });
        }

        void applyHue(double hue) {
            this->applyHSV({ hue, 1, 1 });
        }

        void applyS(double saturation) {
            this->applyHSV({ 0, saturation, 1 });
        }

        void applySaturation(double saturation) {
            this->applyHSV({ 0, saturation, 1 });
        }

        void applyV(double value) {
            this->applyHSV({ 0, 1, value });
        }

        void applyValue(double value) {
            this->applyHSV({ 0, 1, value });
        }

        void applyB(double brightness) {
            this->applyHSV({ 0, 1, brightness });
        }

        void applyBrightness(double brightness) {
            this->applyHSV({ 0, 1, brightness });
        }

        void applyHSV(const cocos2d::extension::HSV& hsv);

        constexpr cocos2d::ccColor3B to3B() const {
            return *this;
        }

        constexpr cocos2d::ccColor4B to4B() const {
            return *this;
        }

        constexpr cocos2d::ccColor4F to4F() const {
            return *this;
        }

        cocos2d::extension::HSV toHSV() const {
            return *this;
        }

        /// @returns True if alpha is 0
        constexpr bool isInvisible() const {
            return a == 0x00;
        }

        /// @returns True if alpha is bigger than 0 and smaller than 255
        constexpr bool isTranslucent() const {
            return a > 0x00 && a < 0xFF;
        }

        /// @returns True if alpha is 255
        constexpr bool isOpaque() const {
            return a == 0xFF;
        }

        template<typename T> requires (CanSetColor3B<T> || CanSetColor4B<T> || CanSetColor4F<T>)
        void applyTo(T* node) const {
            if constexpr (CanSetColor3B<T>) {
                node->setColor(this->to3B());
                node->setOpacity(a);
            } else if constexpr (CanSetColor4B<T>) {
                node->setColor(this->to4B());
            } else {
                node->setColor(this->to4F());
            }
        }
    };
}