/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CCGEMETRY_H__
#define __CCGEMETRY_H__

#include "platform/CCPlatformMacros.h"
#include "CCObject.h"
#include <math.h>

// why
#if !defined(FLT_EPSILON)
#define FLT_EPSILON 1.19209290E-07F // decimal constant 
#endif

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

// for CCPoint assignement operator and copy constructor
class CCSize;

/**
 * @js NA
 */
class CC_DLL CCPoint
{
    GEODE_FRIEND_MODIFY
public:
    float x;
    float y;
    // alk modifications: all defined

    inline CCPoint() : x(0), y(0) {}
    inline CCPoint(float x, float y) : x(x), y(y) {}
    /**
     * @lua NA
     */
    inline CCPoint(const CCPoint& other) : x(other.x), y(other.y) {}
    /**
     * @lua NA
     */
    inline CCPoint(const CCSize& size);
    /**
     * @lua NA
     */
    inline CCPoint& operator= (const CCPoint& other) {
        setPoint(other.x, other.y);
        return *this;
    }
    /**
     * @lua NA
     */
    inline CCPoint& operator= (const CCSize& size);
    /**
     * @lua NA
     */
    inline CCPoint operator+(const CCPoint& right) const {
        return CCPoint(this->x + right.x, this->y + right.y);
    }
    /**
     * @lua NA
     */
    inline CCPoint operator-(const CCPoint& right) const {
        return CCPoint(this->x - right.x, this->y - right.y);
    }
    /**
     * @lua NA
     */
    inline CCPoint operator-() const {
        return CCPoint(-x, -y);
    }
    /**
     * @lua NA
     */
    inline CCPoint operator*(float a) const {
        return CCPoint(this->x * a, this->y * a);
    }
    /**
     * @lua NA
     */
    inline CCPoint operator/(float a) const {
        return CCPoint(this->x / a, this->y / a);
    }


    // camila modification
    inline CCPoint operator*(const CCPoint& right) const {
        return CCPoint(x * right.x, y * right.y);
    }
    inline CCPoint operator/(const CCPoint& right) const {
        return CCPoint(x / right.x, y / right.y);
    }
    //314

    /**
     * @lua NA
     */
    inline void setPoint(float x, float y) {
        this->x = x;
        this->y = y;
    }

    inline bool equals(const CCPoint& target) const {
        return (fabs(this->x - target.x) < FLT_EPSILON) && (fabs(this->y - target.y) < FLT_EPSILON);
    }
    
    /** @returns if points have fuzzy equality which means equal with some degree of variance.
     * @since v2.1.4
     * @lua NA
     */
    inline bool fuzzyEquals(const CCPoint& b, float var) const {
        if(x - var <= b.x && b.x <= x + var)
            if(y - var <= b.y && b.y <= y + var)
                return true;
        return false;
    }

    /** Calculates distance between point an origin
     * @return float
     * @since v2.1.4
     * @lua NA
     */
    inline float getLength() const {
        return sqrtf(x*x + y*y);
    };

    /** Calculates the square length of a CCPoint (not calling sqrt() )
     * @return float
     * @since v2.1.4
     * @lua NA
     */
    inline float getLengthSq() const {
        return dot(*this); //x*x + y*y;
    };

    /** Calculates the square distance between two points (not calling sqrt() )
     @return float
     @since v2.1.4
    */
    inline float getDistanceSq(const CCPoint& other) const {
        return (*this - other).getLengthSq();
    };

    /** Calculates the distance between two points
     @return float
     @since v2.1.4
     */
    inline float getDistance(const CCPoint& other) const {
        return (*this - other).getLength();
    };

    /** @returns the angle in radians between this vector and the x axis
     @since v2.1.4
    */
    inline float getAngle() const {
        return atan2f(y, x);
    };

    /** @returns the angle in radians between two vector directions
     @since v2.1.4
    */
    inline float getAngle(const CCPoint& other) const {
        CCPoint a2 = normalize();
        CCPoint b2 = other.normalize();
        float angle = atan2f(a2.cross(b2), a2.dot(b2));
        if(fabs(angle) < FLT_EPSILON) return 0.f;
        return angle;
    }

    /** Calculates dot product of two points.
     @return float
     @since v2.1.4
     */
    inline float dot(const CCPoint& other) const {
        return x*other.x + y*other.y;
    };

    /** Calculates cross product of two points.
     @return float
     @since v2.1.4
     */
    inline float cross(const CCPoint& other) const {
        return x*other.y - y*other.x;
    };

    /** Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
     @return CCPoint
     @since v2.1.4
     */
    inline CCPoint getPerp() const {
        return CCPoint(-y, x);
    };

    /** Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
     @return CCPoint
     @since v2.1.4
     */
    inline CCPoint getRPerp() const {
        return CCPoint(y, -x);
    };

    /** Calculates the projection of this over other.
     @return CCPoint
     @since v2.1.4
     */
    inline CCPoint project(const CCPoint& other) const {
        return other * (dot(other)/other.dot(other));
    };

    /** Complex multiplication of two points ("rotates" two points).
     @return CCPoint vector with an angle of this.getAngle() + other.getAngle(),
     and a length of this.getLength() * other.getLength().
     @since v2.1.4
     */
    inline CCPoint rotate(const CCPoint& other) const {
        return CCPoint(x*other.x - y*other.y, x*other.y + y*other.x);
    };

    /** Unrotates two points.
     @return CCPoint vector with an angle of this.getAngle() - other.getAngle(),
     and a length of this.getLength() * other.getLength().
     @since v2.1.4
     */
    inline CCPoint unrotate(const CCPoint& other) const {
        return CCPoint(x*other.x + y*other.y, y*other.x - x*other.y);
    };

    /** Returns point multiplied to a length of 1.
     * If the point is 0, it returns (1, 0)
     @return CCPoint
     @since v2.1.4
     */
    inline CCPoint normalize() const {
        float length = getLength();
        if(length == 0.) return CCPoint(1.f, 0);
        return *this / getLength();
    };

    /** Linear Interpolation between two points a and b
     @returns
        alpha == 0 ? a
        alpha == 1 ? b
        otherwise a value between a..b
     @since v2.1.4
     */
    inline CCPoint lerp(const CCPoint& other, float alpha) const {
        return *this * (1.f - alpha) + other * alpha;
    };

    /** Rotates a point counter clockwise by the angle around a pivot
     @param pivot is the pivot, naturally
     @param angle is the angle of rotation ccw in radians
     @returns the rotated point
     @since v2.1.4
     */
    inline CCPoint rotateByAngle(const CCPoint& pivot, float angle) const {
        return pivot + (*this - pivot).rotate(CCPoint::forAngle(angle));
    }

    static inline CCPoint forAngle(const float a)
    {
    	return CCPoint(cosf(a), sinf(a));
    }
};

/**
 * @js NA
 */
class CC_DLL CCSize
{
    GEODE_FRIEND_MODIFY
public:
    float width;
    float height;

public:
    inline CCSize() : width(0), height(0) {}
    inline CCSize(float width, float height) : width(width), height(height) {}
    /**
     * @lua NA
     */
    inline CCSize(const CCSize& other) : width(other.width), height(other.height) {}
    /**
     * @lua NA
     */
    inline CCSize(const CCPoint& point) : width(point.x), height(point.y) {}
    /**
     * @lua NA
     */
    inline CCSize& operator= (const CCSize& other) {
        setSize(other.width, other.height);
        return *this;
    }
    /**
     * @lua NA
     */
    inline CCSize& operator= (const CCPoint& point) {
        setSize(point.x, point.y);
        return *this;
    }
    /**
     * @lua NA
     */
    inline CCSize operator+(const CCSize& right) const {
        return CCSize(this->width + right.width, this->height + right.height);
    }
    /**
     * @lua NA
     */
    inline CCSize operator-(const CCSize& right) const {
        return CCSize(this->width - right.width, this->height - right.height);
    }
    /**
     * @lua NA
     */
    inline CCSize operator*(float a) const {
        return CCSize(this->width * a, this->height * a);
    }
    /**
     * @lua NA
     */
    inline CCSize operator/(float a) const {
        return CCSize(this->width / a, this->height / a);
    }
    /**
     * @lua NA
     */
    inline void setSize(float width, float height) {
        this->width = width;
        this->height = height;
    }
    /**
     * @lua NA
     */
    inline bool equals(const CCSize& target) const {
        return (fabs(this->width  - target.width)  < FLT_EPSILON) && (fabs(this->height - target.height) < FLT_EPSILON);
    }
};

// alk cont

CCPoint::CCPoint(const CCSize& size) : x(size.width), y(size.height) {}

CCPoint& CCPoint::operator= (const CCSize& size) {
    setPoint(size.width, size.height);
    return *this;
}


/**
 * @js NA
 */
class CC_DLL CCRect
{
    GEODE_FRIEND_MODIFY
public:
    CCPoint origin;
    CCSize  size;

public:
    inline CCRect() {
        setRect(0.0f, 0.0f, 0.0f, 0.0f);
    }
    inline CCRect(float x, float y, float width, float height) {
        setRect(x, y, width, height);
    }
    /**
     * @lua NA
     */
    inline CCRect(const CCRect& other) {
        setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    }
    /**
     * @lua NA
     */
    inline CCRect& operator= (const CCRect& other) {
        setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
        return *this;
    }
    inline void setRect(float x, float y, float width, float height) {
        // CGRect can support width<0 or height<0
        // CCAssert(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0.");

        origin.x = x;
        origin.y = y;

        size.width = width;
        size.height = height;
    }

    inline float getMinX() const {
        return origin.x;
    } /// return the leftmost x-value of current rect
    inline float getMidX() const {
        return (float)(origin.x + size.width / 2.0);
    } /// return the midpoint x-value of current rect
    inline float getMaxX() const {
        return (float)(origin.x + size.width);
    } /// return the rightmost x-value of current rect
    inline float getMinY() const {
        return origin.y;
    } /// return the bottommost y-value of current rect
    inline float getMidY() const {
        return (float)(origin.y + size.height / 2.0);
    } /// return the midpoint y-value of current rect
    inline float getMaxY() const {
        return origin.y + size.height;
    } /// return the topmost y-value of current rect
    inline bool equals(const CCRect& rect) const {
        return (origin.equals(rect.origin) && size.equals(rect.size));
    }
    inline bool containsPoint(const CCPoint& point) const {
        bool bRet = false;

        if (point.x >= getMinX() && point.x <= getMaxX()
            && point.y >= getMinY() && point.y <= getMaxY())
        {
            bRet = true;
        }

        return bRet;
    }

    inline bool intersectsRect(const CCRect& rect) const {
        // lmao
        return !(     getMaxX() < rect.getMinX() ||
                 rect.getMaxX() <      getMinX() ||
                      getMaxY() < rect.getMinY() ||
                 rect.getMaxY() <      getMinY());
    }
};


#define CCPointMake(x, y) CCPoint((float)(x), (float)(y))
#define CCSizeMake(width, height) CCSize((float)(width), (float)(height))
#define CCRectMake(x, y, width, height) CCRect((float)(x), (float)(y), (float)(width), (float)(height))


// const CCPoint CCPointZero = CCPointMake(0,0);
#define CCPointZero CCPointMake(0,0)

/* The "zero" size -- equivalent to CCSizeMake(0, 0). */ 
// const CCSize CCSizeZero = CCSizeMake(0,0);
#define CCSizeZero CCSizeMake(0,0)

/* The "zero" rectangle -- equivalent to CCRectMake(0, 0, 0, 0). */ 
// const CCRect CCRectZero = CCRectMake(0,0,0,0);
#define CCRectZero CCRectMake(0,0,0,0)

// end of data_structure group
/// @}

NS_CC_END

#endif // __CCGEMETRY_H__
