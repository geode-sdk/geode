#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>

using namespace geode::prelude;

CopySizeLayout* CopySizeLayout::create() {
    auto ret = new CopySizeLayout();
    ret->m_targets = CCArray::create();
    ret->m_targets->retain();
    ret->autorelease();
    return ret;
}

CopySizeLayout::~CopySizeLayout() {
    m_targets->release();
}

CopySizeLayout* CopySizeLayout::add(CCNode* target) {
    m_targets->addObject(target);
    return this;
}

CopySizeLayout* CopySizeLayout::remove(CCNode* target) {
    m_targets->removeObject(target);
    return this;
}

void CopySizeLayout::apply(CCNode* in) {
    AnchorLayout::apply(in);
    for (auto& node : CCArrayExt<CCNode*>(m_targets)) {
        // Prevent accidental infinite loop
        if (node == in) continue;
        node->ignoreAnchorPointForPosition(false);
        node->setContentSize(in->getContentSize() * ccp(1 / in->getScaleX(), 1 / in->getScaleY()));
        node->setPosition(in->getContentSize() / 2);
        node->updateLayout();
    }
}

CCSize CopySizeLayout::getSizeHint(CCNode* in) const {
    return in->getContentSize();
}
