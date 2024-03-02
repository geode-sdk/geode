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

// these use setContentSize and getContentSize because they're virtuals and 
// some node may override those for wacky behaviour

void CCNode::setScaledContentSize(CCSize const& size) {
    this->setContentSize({ size.width / m_fScaleX, size.height / m_fScaleY });
}

void CCNode::setContentWidth(float width) {
    this->setContentSize({ width, m_obContentSize.height });
}

void CCNode::setContentHeight(float height) {
    this->setContentSize({ m_obContentSize.width, height });
}

float CCNode::getContentWidth() const {
    return this->getContentSize().width;
}

float CCNode::getContentHeight() const {
    return this->getContentSize().height;
}

// getScaledContentSize is not const bruh

float CCNode::getScaledContentWidth() const {
    return this->getContentWidth() * m_fScaleX;
}

float CCNode::getScaledContentHeight() const {
    return this->getContentHeight() * m_fScaleY;
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
