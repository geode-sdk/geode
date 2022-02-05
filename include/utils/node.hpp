#pragma once

#include <Geode.hpp>

namespace geode::cocos {
    /**
     * Get child at index. Checks bounds; a negative 
     * index will get the child starting from the end
     * @returns Child at index cast to the given type, 
     * or nullptr if index exceeds bounds
     */
    template<class T = cocos2d::CCNode*>
    static T getChild(cocos2d::CCNode* x, int i) {
        if (i < 0)
            i = x->getChildrenCount() + i;
        if (static_cast<int>(x->getChildrenCount()) <= i)
            return nullptr;
        return reinterpret_cast<T>(x->getChildren()->objectAtIndex(i));
    }
}
