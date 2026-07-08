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
        operator cocos2d::ccColor3B() const;
        operator cocos2d::ccColor4B() const;
        operator cocos2d::ccColor4F() const;
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
        Color(GLubyte alpha = 0);
        Color(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha = 0xFF);
        Color(const cocos2d::ccColor3B& color, GLubyte alpha = 0xFF);
        Color(const cocos2d::ccColor4B& color);
        Color(const cocos2d::ccColor4F& color);
        Color(const cocos2d::extension::HSV& hsv, GLubyte alpha = 0xFF);

        void applyH(double hue);
        void applyHue(double hue);
        void applyS(double saturation);
        void applySaturation(double saturation);
        void applyV(double value);
        void applyValue(double value);
        void applyB(double brightness);
        void applyBrightness(double brightness);
        void applyHSV(const cocos2d::extension::HSV& hsv);
        cocos2d::ccColor3B to3B() const;
        cocos2d::ccColor4B to4B() const;
        cocos2d::ccColor4F to4F() const;
        cocos2d::extension::HSV toHSV() const;
        /// @returns True if alpha is 0
        bool isInvisible() const;
        /// @returns True if alpha is bigger than 0 and smaller than 255
        bool isTranslucent() const;
        /// @returns True if alpha is 255
        bool isOpaque() const;
        template<typename T> requires (CanSetColor3B<T> || CanSetColor4B<T> || CanSetColor4F<T>)
        inline void applyTo(T* node) const {
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