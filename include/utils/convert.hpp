#pragma once

#include <cocos2d.h>
#include "general.hpp"

namespace geode::cocos {
    inline void ccDrawColor4B(cocos2d::ccColor4B const& color) {
        cocos2d::ccDrawColor4B(color.r, color.g, color.b, color.a);
    }

    inline cocos2d::ccColor4B invert4B(cocos2d::ccColor4B const& color) {
        return {
            static_cast<GLubyte>(255 - color.r),
            static_cast<GLubyte>(255 - color.g),
            static_cast<GLubyte>(255 - color.b),
            color.a
        };
    }
    
    inline cocos2d::ccColor3B invert3B(cocos2d::ccColor3B const& color) {
        return {
            static_cast<GLubyte>(255 - color.r),
            static_cast<GLubyte>(255 - color.g),
            static_cast<GLubyte>(255 - color.b)
        };
    }

    inline cocos2d::ccColor3B lighten3B(cocos2d::ccColor3B const& color, int amount) {
        return {
            static_cast<GLubyte>(utils::clamp(color.r + amount, 0, 255)),
            static_cast<GLubyte>(utils::clamp(color.g + amount, 0, 255)),
            static_cast<GLubyte>(utils::clamp(color.b + amount, 0, 255)),
        };
    }

    inline cocos2d::ccColor3B darken3B(cocos2d::ccColor3B const& color, int amount) {
        return lighten3B(color, -amount);
    }
    
    inline cocos2d::ccColor3B to3B(cocos2d::ccColor4B const& color) {
        return { color.r, color.g, color.b };
    }

    inline cocos2d::ccColor4B to4B(cocos2d::ccColor3B const& color, GLubyte alpha = 255) {
        return { color.r, color.g, color.b, alpha };
    }
    
    inline cocos2d::ccColor4F to4F(cocos2d::ccColor4B const& color) {
        return {
            color.r / 255.f,
            color.g / 255.f,
            color.b / 255.f,
            color.a / 255.f
        };
    }

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

    template<typename T,
    	typename = std::enable_if_t<std::is_pointer_v<T>> >
    static cocos2d::CCArray* vectorToCCArray(std::vector<T> const& vec) {
        auto res = cocos2d::CCArray::createWithCapacity(vec.size());
        for (auto const& item : vec)
            res->addObject(item);
        return res;
    }

    template<typename T, typename C,
    	typename = std::enable_if_t<std::is_pointer_v<C>> >
    static cocos2d::CCArray* vectorToCCArray(std::vector<T> const& vec, std::function<C(T)> convFunc) {
        auto res = cocos2d::CCArray::createWithCapacity(vec.size());
        for (auto const& item : vec)
            res->addObject(convFunc(item));
        return res;
    }

    template <typename T,
    	typename = std::enable_if_t<std::is_pointer_v<T>> >
	std::vector<T> ccArrayToVector(cocos2d::CCArray* arr) {
		return std::vector<T>(reinterpret_cast<T*>(arr->data->arr), reinterpret_cast<T*>(arr->data->arr) + arr->data->num);
	}

	template<typename K, typename V, 
		typename = std::enable_if_t<std::is_same_v<K, std::string> || std::is_same_v<K, intptr_t>> >
    static cocos2d::CCDictionary* mapToCCDict(std::map<K, V> const& map) {
        auto res = cocos2d::CCDictionary::create();
        for (auto const& [key, value] : map)
            res->setObject(value, key);
        return res;
    }

    template<typename K, typename V, typename C, 
    	typename = std::enable_if_t<std::is_same_v<C, std::string> || std::is_same_v<C, intptr_t>> >
    static cocos2d::CCDictionary* mapToCCDict(std::map<K, V> const& map, std::function<C(K)> convFunc) {
        auto res = cocos2d::CCDictionary::create();
        for (auto const& [key, value] : map)
            res->setObject(value, convFunc(key));
        return res;
    }

  //   template<typename K, typename V, 
		// typename = std::enable_if_t<std::is_same_v<K, std::string> || std::is_same_v<K, intptr_t>> >
  //   static std::map<K, V> ccDictToMap(cocos2d::CCDictionary* dict) {
  //       auto res = std::map<K, V>();
  //       cocos2d::CCDictElement* element = nullptr;
  //       CCDICT_FOREACH(dict, element) {
  //       	if constexpr (std::is_same_v<K, std::string>) 
  //       		res[element->getStrKey()] = element->getObject();
  //       	if constexpr (std::is_same_v<K, intptr_t>) 
  //       		res[element->getIntKey()] = element->getObject();
  //       }
  //       return res;
  //   }
}
