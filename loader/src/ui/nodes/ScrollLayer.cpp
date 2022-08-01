#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/WackyGeodeMacros.hpp>

USE_GEODE_NAMESPACE();

GenericContentLayer* GenericContentLayer::create(float width, float height) {
    auto ret = new GenericContentLayer();
    if (ret && ret->initWithColor({ 0, 0, 0, 0 }, width, height)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void GenericContentLayer::setPosition(CCPoint const& pos) {
    // CCContentLayer expect its children to 
    // all be TableViewCells
    CCLayerColor::setPosition(pos);

    CCARRAY_FOREACH_B_TYPE(m_pChildren, child, CCNode) {
        auto y = this->getPositionY() + child->getPositionY();
        child->setVisible(!(
            (m_obContentSize.height < y) ||
            (y < -child->getContentSize().height)
        ));
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

ScrollLayer::ScrollLayer(
    CCRect const& rect,
    bool scrollWheelEnabled,
    bool vertical
) : CCScrollLayerExt(rect) {
    m_scrollWheelEnabled = scrollWheelEnabled;

    m_disableVertical = !vertical;
    m_disableHorizontal = vertical;
    m_cutContent = true;

    m_contentLayer->removeFromParent();
    m_contentLayer = GenericContentLayer::create(
        rect.size.width, rect.size.height
    );
    m_contentLayer->setAnchorPoint({ 0, 0 });
    this->addChild(m_contentLayer);

    this->setMouseEnabled(true);
    this->setTouchEnabled(true);
}

ScrollLayer* ScrollLayer::create(CCRect const& rect, bool scroll, bool vertical) {
    auto ret = new ScrollLayer(rect, scroll, vertical);
    if (ret) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScrollLayer* ScrollLayer::create(CCSize const& size, bool scroll, bool vertical) {
    return ScrollLayer::create({ 0, 0, size.width, size.height }, scroll, vertical);
}
