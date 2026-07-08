#pragma once

#include <ccTypes.h>

namespace geode {
    template<typename T>
    concept has_4b = requires(T object) { object.setColor(cocos2d::ccColor4B()); };
    template<typename T>
    concept has_3b = requires(T object) {
        object.setColor(cocos2d::ccColor3B());
        object.setOpacity(GLubyte());
    };

    struct GEODE_DLL Color final {
        /// Parses the given hex string using one of the following formats:
        /// #R/G/B, #RR/GG/BB, #RGB, #RGBA, #RRGGBB or #RRGGBBAA
        /// Where the # prefix is optional and / means it's 1 byte/nibble representing all channels
        /// @returns The parsed result
        static Result<Color> parse(std::string_view hex);
    private:
        inline static GLubyte getByte(uint32_t colorNum, size_t index, bool isShort);
    public:
        /// @returns The current alpha value
        operator float() const;
        operator cocos2d::ccColor3B() const;
        operator cocos2d::ccColor4B() const;
        operator cocos2d::extension::HSV() const;
        Color& operator=(Color&& other) = default;
        Color& operator=(const Color& other) = default;

        GLubyte r;
        GLubyte g;
        GLubyte b;
        GLubyte a;

        Color(Color&& other) = default;
        Color(const Color& other) = default;
        /// @returns The equivalent of #000000{a}
        Color(GLubyte alpha = 0);
        Color(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha = 0xFF);
        Color(const cocos2d::ccColor3B& color, GLubyte alpha = 0xFF);
        Color(const cocos2d::ccColor4B& color);
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
        void setR(GLubyte red);
        GLubyte getR() const;
        void setRed(GLubyte red);
        GLubyte getRed() const;
        void setG(GLubyte green);
        GLubyte getG() const;
        void setGreen(GLubyte green);
        GLubyte getGreen() const;
        void setB(GLubyte blue);
        GLubyte getB() const;
        void setBlue(GLubyte blue);
        GLubyte getBlue() const;
        void setA(GLubyte alpha);
        GLubyte getA() const;
        void setAlpha(GLubyte alpha);
        GLubyte getAlpha() const;
        cocos2d::ccColor3B to3B() const;
        cocos2d::ccColor4B to4B() const;
        cocos2d::extension::HSV toHSV() const;
        /// @returns True if alpha is 0
        bool isInvisible() const;
        /// @returns True if alpha is bigger than 0 and smaller than 255
        bool isTransparent() const;
        /// @returns True if alpha is 255
        bool isOpaque() const;
        template<typename T> requires (has_4b<T> || has_3b<T>)
        inline void applyTo(T* node) const {
            if constexpr (has_4b<T>) {
                node->setColor(this->to4B());
            } else {
                node->setColor(this->to3B());
                node->setOpacity(this->getA());
            }
        }
    };
}