#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>

using namespace geode::prelude;

AutoSizeLayout* AutoSizeLayout::create() {
    auto ret = new AutoSizeLayout();
    ret->m_targets = CCArray::create();
    ret->m_targets->retain();
    ret->autorelease();
    return ret;
}

AutoSizeLayout::~AutoSizeLayout() {
    m_targets->release();
}

AutoSizeLayout* AutoSizeLayout::add(CCNode* target) {
    m_targets->addObject(target);
    return this;
}

AutoSizeLayout* AutoSizeLayout::remove(CCNode* target) {
    m_targets->removeObject(target);
    return this;
}

void AutoSizeLayout::apply(CCNode* in) {
    AnchorLayout::apply(in);
    for (auto& node : CCArrayExt<CCNode*>(m_targets)) {
        // Prevent accidental infinite loop
        if (node == in) continue;
        node->ignoreAnchorPointForPosition(false);
        node->setContentSize(in->getContentSize());
        node->setPosition(in->getContentSize() / 2);
        node->updateLayout();
    }
}

CCSize AutoSizeLayout::getSizeHint(CCNode* in) const {
    return in->getContentSize();
}
