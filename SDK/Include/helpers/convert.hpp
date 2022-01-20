#pragma once

#include <Geode>

namespace geode::cocos {
    void ccDrawColor4B(cocos2d::ccColor4B const& color);
    cocos2d::ccColor4B invert4B(cocos2d::ccColor4B const& color);
    cocos2d::ccColor3B invert3B(cocos2d::ccColor3B const& color);
    cocos2d::ccColor3B to3B(cocos2d::ccColor4B const& color);
    cocos2d::ccColor4B to4B(cocos2d::ccColor3B const& color, GLubyte alpha = 255);
    cocos2d::ccColor4F to4F(cocos2d::ccColor4B const& color);
    constexpr cocos2d::ccColor3B cc3x(int hexValue) {
        if (hexValue <= 0xf)
            return cocos2d::ccColor3B {
                static_cast<GLubyte>(hexValue * 17),
                static_cast<GLubyte>(hexValue * 17),
                static_cast<GLubyte>(hexValue * 17)
            };
        if (hexValue <= 0xff)
            return cocos2d::ccColor3B {
                static_cast<GLubyte>(hexValue),
                static_cast<GLubyte>(hexValue),
                static_cast<GLubyte>(hexValue)
            };
        if (hexValue <= 0xfff)
            return cocos2d::ccColor3B {
                static_cast<GLubyte>((hexValue >> 8 & 0xf) * 17),
                static_cast<GLubyte>((hexValue >> 4 & 0xf) * 17),
                static_cast<GLubyte>((hexValue >> 0 & 0xf) * 17)
            };
        else
            return cocos2d::ccColor3B {
                static_cast<GLubyte>(hexValue >> 16 & 0xff),
                static_cast<GLubyte>(hexValue >> 8  & 0xff),
                static_cast<GLubyte>(hexValue >> 0  & 0xff)
            };
    }

    template<typename T>
    static cocos2d::CCArray* vectorToCCArray(std::vector<T> const& vec) {
        auto res = cocos2d::CCArray::create();
        for (auto const& item : vec)
            res->addObject(item);
        return res;
    }

    template<typename T, class T2>
    static cocos2d::CCArray* vectorToCCArray(std::vector<T> const& vec, std::function<T2(T)> convFunc) {
        auto res = cocos2d::CCArray::create();
        for (auto const& item : vec)
            res->addObject(convFunc(item));
        return res;
    }
}
