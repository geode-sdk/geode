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

    class GEODE_DLL ColorCode {
    public:
        /// @returns The current alpha value
        operator float() const;
        operator cocos2d::ccColor3B() const;
        operator cocos2d::ccColor4B() const;

        GLubyte m_r;
        GLubyte m_g;
        GLubyte m_b;
        GLubyte m_a;

        /// @returns The equivalent of #000000{a}
        ColorCode(GLubyte a = 0);
        /// Parses the given hex string using one of the following formats:
        /// #R/G/B, #RR/GG/BB, #RGB, #RGBA, #RRGGBB or #RRGGBBAA
        /// Where the # prefix is optional and / means it's 1 byte/nibble representing all channels
        /// @returns The parsed hex string
        ColorCode(std::string_view hex);
        /// @returns The equivalent of #{r}{g}{b}{a}
        ColorCode(GLubyte r, const GLubyte g, const GLubyte b, const GLubyte a = 255);

        GLubyte getR() const;
        GLubyte getRed() const;
        GLubyte getG() const;
        GLubyte getGreen() const;
        GLubyte getB() const;
        GLubyte getBlue() const;
        GLubyte getA() const;
        GLubyte getAlpha() const;
        cocos2d::ccColor3B to3B() const;
        cocos2d::ccColor4B to4B() const;
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
    private:
        void extractChannels(std::string_view hex);
        GLubyte extractChannel(std::string_view hexString, size_t index, bool isShort) const;
    };
}