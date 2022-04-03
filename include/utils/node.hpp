#pragma once

#include <Geode.hpp>

namespace geode::cocos {
    /**
     * Get child at index. Checks bounds. A negative 
     * index will get the child starting from the end
     * @returns Child at index cast to the given type, 
     * or nullptr if index exceeds bounds
     */
    template<class T = cocos2d::CCNode*>
    static T getChild(cocos2d::CCNode* x, int i) {
        // start from end for negative index
        if (i < 0) i = x->getChildrenCount() + i;
        // check if backwards index is out of bounds
        if (i < 0) return nullptr;
        // check if forwards index is out of bounds
        if (static_cast<int>(x->getChildrenCount()) <= i) return nullptr;
        return reinterpret_cast<T>(x->getChildren()->objectAtIndex(i));
    }

    /**
     * Get bounds for a set of nodes. Based on content 
     * size
     * @param nodes Nodes to calculate coverage of
     * @returns Rectangle fitting all nodes. Origin 
     * will be <= 0 and size will be >= 0
     */
    cocos2d::CCRect calculateNodeCoverage(std::vector<cocos2d::CCNode*> const& nodes);
    /**
     * Get bounds for a set of nodes. Based on content 
     * size
     * @param nodes Nodes to calculate coverage of
     * @returns Rectangle fitting all nodes. Origin 
     * will be <= 0 and size will be >= 0
     */
    cocos2d::CCRect calculateNodeCoverage(cocos2d::CCArray* nodes);
    /**
     * Get bounds for a set of nodes. Based on content 
     * size
     * @param parent Parent whose children to calculate
     * coverage of
     * @returns Rectangle fitting all the parent's children.
     * Origin will be <= 0 and size will be >= 0
     */
    cocos2d::CCRect calculateChildCoverage(cocos2d::CCNode* parent);
}
