#pragma once

#include <Geode/Geode.hpp>

namespace geode {
    static cocos2d::CCPoint& operator*=(cocos2d::CCPoint & pos,  float mul) {
        pos.x *= mul;
        pos.y *= mul;
        return pos;
    }
    static cocos2d::CCSize&  operator*=(cocos2d::CCSize  & size, float mul) {
        size.width  *= mul;
        size.height *= mul;
        return size;
    }
    static cocos2d::CCSize  operator*(cocos2d::CCSize const& size, cocos2d::CCPoint const& point) {
        return {
            size.width * point.x,
            size.height * point.y,
        };
    }
    static cocos2d::CCRect  operator*=(cocos2d::CCRect  & rect, float mul) {
        rect.origin *= mul;
        rect.size   *= mul;
        return rect;
    }
    static cocos2d::CCPoint operator/=(cocos2d::CCPoint & pos,  float div) {
        pos.x /= div;
        pos.y /= div;
        return pos;
    }
    static cocos2d::CCSize  operator/=(cocos2d::CCSize  & size, float div) {
        size.width  /= div;
        size.height /= div;
        return size;
    }
    static cocos2d::CCRect  operator/=(cocos2d::CCRect  & rect, float div) {
        rect.origin /= div;
        rect.size   /= div;
        return rect;
    }
    static cocos2d::CCPoint operator+=(cocos2d::CCPoint & pos,  cocos2d::CCPoint const& add) {
        pos.x += add.x;
        pos.y += add.y;
        return pos;
    }
    static cocos2d::CCSize  operator+=(cocos2d::CCSize  & size, cocos2d::CCPoint const& add) {
        size.width  += add.x;
        size.height += add.y;
        return size;
    }
    static cocos2d::CCSize  operator+=(cocos2d::CCSize  & size, cocos2d::CCSize  const& add) {
        size.width  += add.width;
        size.height += add.height;
        return size;
    }
    static cocos2d::CCRect  operator+=(cocos2d::CCRect  & rect, cocos2d::CCPoint const& add) {
        rect.origin += add;
        return rect;
    }
    static cocos2d::CCRect  operator+=(cocos2d::CCRect  & rect, cocos2d::CCSize  const& add) {
        rect.size += add;
        return rect;
    }
    static cocos2d::CCRect  operator+=(cocos2d::CCRect  & rect, cocos2d::CCRect  const& add) {
        rect.origin += add.origin;
        rect.size   += add.size;
        return rect;
    }
    static cocos2d::CCSize  operator- (cocos2d::CCSize const& size, float f) {
        return { size.width - f, size.height - f };
    }
    static cocos2d::CCSize  operator- (cocos2d::CCSize const& size) {
        return { -size.width, -size.height };
    }
    static bool operator==(cocos2d::CCPoint const& p1,   cocos2d::CCPoint const& p2) {
        return  p1.x == p2.x &&
                p1.y == p2.y;
    }
    static bool operator!=(cocos2d::CCPoint const& p1,   cocos2d::CCPoint const& p2) {
        return  p1.x != p2.x ||
                p1.y != p2.y;
    }
    static bool operator==(cocos2d::CCSize  const& s1,   cocos2d::CCSize  const& s2) {
        return  s1.width  == s2.width &&
                s1.height == s2.height;
    }
    static bool operator!=(cocos2d::CCSize  const& s1,   cocos2d::CCSize  const& s2) {
        return  s1.width  != s2.width ||
                s1.height != s2.height;
    }
    static bool operator==(cocos2d::CCRect  const& r1,   cocos2d::CCRect  const& r2) {
        return  r1.origin == r2.origin &&
                r1.size   == r2.size;
    }
    static bool operator!=(cocos2d::CCRect  const& r1,   cocos2d::CCRect  const& r2) {
        return  r1.origin != r2.origin ||
                r1.size   != r2.size;
    }
    static bool operator==(cocos2d::ccColor4B const& c1, cocos2d::ccColor4B const& c2) {
        return c1.r == c2.r &&
               c1.g == c2.g &&
               c1.b == c2.b &&
               c1.a == c2.a;
    }
    static bool operator!=(cocos2d::ccColor4B const& c1, cocos2d::ccColor4B const& c2) {
        return c1.r != c2.r ||
               c1.g != c2.g ||
               c1.b != c2.b ||
               c1.a != c2.a;
    }
    static bool operator!=(cocos2d::ccColor3B const& c1, cocos2d::ccColor3B const& c2) {
        return c1.r != c2.r ||
               c1.g != c2.g ||
               c1.b != c2.b;
    }
}
