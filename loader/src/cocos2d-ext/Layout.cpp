#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>

using namespace geode::prelude;

#pragma warning(disable: 4273)

void CCNode::swapChildIndices(CCNode* first, CCNode* second) {
    m_pChildren->exchangeObject(first, second);
    std::swap(first->m_nZOrder, second->m_nZOrder);
    std::swap(first->m_uOrderOfArrival, second->m_uOrderOfArrival);
}

void CCNode::insertBefore(CCNode* child, CCNode* before) {
    this->addChild(child);
    if (
        (before && m_pChildren->containsObject(before)) ||
        (before = static_cast<CCNode*>(m_pChildren->firstObject()))
    ) {
        child->setZOrder(before->getZOrder());
        child->setOrderOfArrival(before->getOrderOfArrival() - 1);
    }
}

void CCNode::insertAfter(CCNode* child, CCNode* after) {
    this->addChild(child);
    if (m_pChildren->containsObject(after)) {
        child->setZOrder(after->getZOrder());
        child->setOrderOfArrival(after->getOrderOfArrival() + 1);
    }
}

bool CCNode::hasAncestor(CCNode* ancestor) {
    if (!ancestor) {
        ancestor = CCScene::get();
    }
    if (m_pParent == ancestor) {
        return true;
    }
    if (m_pParent) {
        return m_pParent->hasAncestor(ancestor);
    }
    return false;
}

CCArray* Layout::getNodesToPosition(CCNode* on) const {
    auto arr = CCArray::create();
    for (auto child : CCArrayExt<CCNode*>(on->getChildren())) {
        if (!m_ignoreInvisibleChildren || child->isVisible()) {
            arr->addObject(child);
        }
    }
    return arr;
}

void Layout::ignoreInvisibleChildren(bool ignore) {
    m_ignoreInvisibleChildren = ignore;
}

bool Layout::isIgnoreInvisibleChildren() const {
    return m_ignoreInvisibleChildren;
}
