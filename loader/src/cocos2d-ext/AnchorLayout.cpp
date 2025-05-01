#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>

using namespace geode::prelude;

AnchorLayoutOptions* AnchorLayoutOptions::create() {
    auto ret = new AnchorLayoutOptions();
    ret->autorelease();
    return ret;
}

Anchor AnchorLayoutOptions::getAnchor() const {
    return m_anchor;
}

CCPoint AnchorLayoutOptions::getOffset() const {
    return m_offset;
}

AnchorLayoutOptions* AnchorLayoutOptions::setAnchor(Anchor anchor) {
    m_anchor = anchor;
    return this;
}

AnchorLayoutOptions* AnchorLayoutOptions::setOffset(CCPoint const& offset) {
    m_offset = offset;
    return this;
}

AnchorLayout* AnchorLayout::create() {
    auto ret = new AnchorLayout();
    ret->autorelease();
    return ret;
}

void AnchorLayout::apply(CCNode* on) {
    on->ignoreAnchorPointForPosition(false);
    for (auto node : CCArrayExt<CCNode*>(this->getNodesToPosition(on))) {
        if (auto opts = typeinfo_cast<AnchorLayoutOptions*>(node->getLayoutOptions())) {
            auto pos = opts->getOffset();
            auto size = on->getContentSize();
            switch (opts->getAnchor()) {
                default:
                case Anchor::Center: pos += size / 2; break;
                case Anchor::TopLeft: pos += ccp(0, size.height); break;
                case Anchor::Top: pos += ccp(size.width / 2, size.height); break;
                case Anchor::TopRight: pos += ccp(size.width, size.height); break;
                case Anchor::Right: pos += ccp(size.width, size.height / 2); break;
                case Anchor::BottomRight: pos += ccp(size.width, 0); break;
                case Anchor::Bottom: pos += ccp(size.width / 2, 0); break;
                case Anchor::BottomLeft: pos += ccp(0, 0); break;
                case Anchor::Left: pos += ccp(0, size.height / 2); break;
            }
            // node->ignoreAnchorPointForPosition(false);
            node->setPosition(pos);
        }
    }
}

CCSize AnchorLayout::getSizeHint(CCNode* on) const {
    return on->getContentSize();
}

CCPoint AnchorLayout::getAnchoredPosition(CCNode* in, Anchor anchor, CCPoint const& offset) {
    auto pos = offset;
    auto size = in->getContentSize();
    switch (anchor) {
        default:
        case Anchor::Center: pos += size / 2; break;
        case Anchor::TopLeft: pos += ccp(0, size.height); break;
        case Anchor::Top: pos += ccp(size.width / 2, size.height); break;
        case Anchor::TopRight: pos += ccp(size.width, size.height); break;
        case Anchor::Right: pos += ccp(size.width, size.height / 2); break;
        case Anchor::BottomRight: pos += ccp(size.width, 0); break;
        case Anchor::Bottom: pos += ccp(size.width / 2, 0); break;
        case Anchor::BottomLeft: pos += ccp(0, 0); break;
        case Anchor::Left: pos += ccp(0, size.height / 2); break;
    }
    return pos;
}
