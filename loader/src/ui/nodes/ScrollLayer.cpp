#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

GenericContentLayer* GenericContentLayer::create(float width, float height) {
    auto ret = new GenericContentLayer();
    if (ret->initWithColor({ 0, 0, 0, 0 }, width, height)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void GenericContentLayer::setPosition(CCPoint const& pos) {
    // CCContentLayer expect its children to
    // all be TableViewCells
    CCLayerColor::setPosition(pos);

    CCSize scrollLayerSize{};
    if (auto parent = this->getParent()) {
        scrollLayerSize = parent->getContentSize();
    }

    for (auto child : CCArrayExt<CCNode*>(m_pChildren)) {
        float childY = this->getPositionY() + child->getPositionY();
        auto anchor = child->isIgnoreAnchorPointForPosition() ? CCPoint{ 0, 0 } : child->getAnchorPoint();
        float scaleY = child->getScaleY();
        float childTop = scaleY * (childY + (1.f - anchor.y) * child->getScaledContentSize().height);
        float childBottom = scaleY * (childY - child->getAnchorPoint().y * child->getScaledContentSize().height);
        bool visible = childTop > 0 && childBottom < scrollLayerSize.height;

        child->setVisible(visible);
    }
}

void ScrollLayer::visit() {
    if (m_cutContent && this->isVisible()) {
        glEnable(GL_SCISSOR_TEST);

        if (this->getParent()) {
            // CCPoint const offset = this->isIgnoreAnchorPointForPosition()
            //     ? ccp(0, 0) : CCPoint(this->getContentSize() * -this->getAnchorPoint());

            auto const bottomLeft = this->convertToWorldSpace(ccp(0, 0));
            auto const topRight = this->convertToWorldSpace(this->getContentSize());
            CCSize const size = topRight - bottomLeft;

            CCEGLView::get()->setScissorInPoints(bottomLeft.x, bottomLeft.y, size.width, size.height);
        }
    }

    CCNode::visit();

    if (m_cutContent && this->isVisible()) {
        glDisable(GL_SCISSOR_TEST);
    }
}

void ScrollLayer::scrollWheel(float y, float) {
    if (m_scrollWheelEnabled) {
        this->scrollLayer(y);
    }
}

void ScrollLayer::enableScrollWheel(bool enable) {
    m_scrollWheelEnabled = enable;
}

bool ScrollLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (nodeIsVisible(this)) {
        return CCScrollLayerExt::ccTouchBegan(touch, event);
    }
    return false;
}

void ScrollLayer::scrollToTop() {
    auto listTopScrollPos = -m_contentLayer->getContentHeight() + this->getContentHeight();
    m_contentLayer->setPositionY(listTopScrollPos);
}

ScrollLayer::ScrollLayer(CCRect const& rect, bool scrollWheelEnabled, bool vertical) :
    CCScrollLayerExt(rect) {
    m_scrollWheelEnabled = scrollWheelEnabled;

    m_disableVertical = !vertical;
    m_disableHorizontal = vertical;
    m_cutContent = true;

    m_contentLayer->removeFromParent();
    m_contentLayer = GenericContentLayer::create(rect.size.width, rect.size.height);
    m_contentLayer->setID("content-layer");
    m_contentLayer->setAnchorPoint({ 0, 0 });
    this->addChild(m_contentLayer);

    this->setID("ScrollLayer");

    this->setMouseEnabled(true);
    this->setTouchEnabled(true);
}

ScrollLayer* ScrollLayer::create(CCRect const& rect, bool scroll, bool vertical) {
    auto ret = new ScrollLayer(rect, scroll, vertical);
    ret->autorelease();
    return ret;
}

ScrollLayer* ScrollLayer::create(CCSize const& size, bool scroll, bool vertical) {
    return ScrollLayer::create({ 0, 0, size.width, size.height }, scroll, vertical);
}
