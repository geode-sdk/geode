#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>

using namespace geode::prelude;

class GenericContentLayer::Impl {
public:
    GenericContentLayer* m_self = nullptr;
    Impl(GenericContentLayer* self);
    void setPosition(CCPoint const& pos);
};

class ScrollLayer::Impl {
public:
    ScrollLayer* m_self = nullptr;
    bool m_scrollWheelEnabled = false;

    Impl(ScrollLayer* self);

    void visit();
    void scrollWheel(float y, float x);
    void enableScrollWheel(bool enable);
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void scrollToTop();
    void setupScrollLayer(CCRect const& rect, bool scrollWheelEnabled, bool vertical);
};

GenericContentLayer::Impl::Impl(GenericContentLayer* self) : m_self(self) {}

void GenericContentLayer::Impl::setPosition(CCPoint const& pos) {
    auto parent = m_self->getParent();

    if (parent && parent->getContentHeight() > m_self->getScaledContentHeight()) {
        auto y = parent->getContentHeight() - m_self->getScaledContentHeight();
        m_self->CCLayerColor::setPosition({pos.x, y});
    }
    else {
        m_self->CCLayerColor::setPosition(pos);
    }

    // CCContentLayer expect its children to
    // all be TableViewCells
    CCSize scrollLayerSize{};
    if (parent) {
        scrollLayerSize = parent->getContentSize();
    }

    for (auto child : CCArrayExt<CCNode*>(m_self->getChildren())) {
        float childY = m_self->getPositionY() + child->getPositionY();
        auto anchor = child->isIgnoreAnchorPointForPosition() ? CCPoint{ 0, 0 } : child->getAnchorPoint();
        float scaleY = child->getScaleY();
        float childTop = scaleY * (childY + (1.f - anchor.y) * child->getScaledContentSize().height);
        float childBottom = scaleY * (childY - child->getAnchorPoint().y * child->getScaledContentSize().height);
        bool visible = childTop > 0 && childBottom < scrollLayerSize.height;

        child->setVisible(visible);
    }
}

ScrollLayer::Impl::Impl(ScrollLayer* self) : m_self(self) {}

void ScrollLayer::Impl::visit() {
    if (m_self->m_cutContent && m_self->isVisible()) {
        glEnable(GL_SCISSOR_TEST);

        if (m_self->getParent()) {
            auto const bottomLeft = m_self->convertToWorldSpace(ccp(0, 0));
            auto const topRight = m_self->convertToWorldSpace(m_self->getContentSize());
            CCSize const size = topRight - bottomLeft;

            CCEGLView::get()->setScissorInPoints(bottomLeft.x, bottomLeft.y, size.width, size.height);
        }
    }

    m_self->CCNode::visit();

    if (m_self->m_cutContent && m_self->isVisible()) {
        glDisable(GL_SCISSOR_TEST);
    }
}

void ScrollLayer::Impl::scrollWheel(float y, float x) {
    if (m_scrollWheelEnabled) {
        m_self->scrollLayer(y);
    }
}

void ScrollLayer::Impl::enableScrollWheel(bool enable) { m_scrollWheelEnabled = enable; }

bool ScrollLayer::Impl::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (nodeIsVisible(m_self)) {
        return m_self->CCScrollLayerExt::ccTouchBegan(touch, event);
    }
    return false;
}

void ScrollLayer::Impl::scrollToTop() {
    auto listTopScrollPos = -m_self->m_contentLayer->getContentHeight() + m_self->getContentHeight();
    m_self->m_contentLayer->setPositionY(listTopScrollPos);
}

void ScrollLayer::Impl::setupScrollLayer(CCRect const& rect, bool scrollWheelEnabled, bool vertical) {
    m_scrollWheelEnabled = scrollWheelEnabled;

    m_self->m_disableVertical = !vertical;
    m_self->m_disableHorizontal = vertical;
    m_self->m_cutContent = true;

    m_self->m_contentLayer->removeFromParent();
    m_self->m_contentLayer = GenericContentLayer::create(rect.size.width, rect.size.height);
    m_self->m_contentLayer->setID("content-layer");
    m_self->m_contentLayer->setAnchorPoint({ 0, 0 });
    m_self->addChild(m_self->m_contentLayer);

    m_self->setID("ScrollLayer");

    m_self->setMouseEnabled(true);
    m_self->setTouchEnabled(true);
}

GenericContentLayer* GenericContentLayer::create(float width, float height) {
    auto ret = new GenericContentLayer();
    if (ret->initWithColor({ 0, 0, 0, 0 }, width, height)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

GenericContentLayer::GenericContentLayer() : m_impl(std::make_unique<GenericContentLayer::Impl>(this)) {}
GenericContentLayer::~GenericContentLayer() = default;

void GenericContentLayer::setPosition(CCPoint const& pos) { 
    m_impl->setPosition(pos); 
}


void ScrollLayer::visit() { 
    m_impl->visit(); 
}

void ScrollLayer::scrollWheel(float y, float x) { 
    m_impl->scrollWheel(y, x); 
}

void ScrollLayer::enableScrollWheel(bool enable) { 
    m_impl->enableScrollWheel(enable); 
}

bool ScrollLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) { 
    return m_impl->ccTouchBegan(touch, event); 
}

void ScrollLayer::scrollToTop() { 
    m_impl->scrollToTop(); 
}

ScrollLayer::ScrollLayer(CCRect const& rect, bool scrollWheelEnabled, bool vertical) :
    CCScrollLayerExt(rect) {
    this->m_impl = std::make_unique<ScrollLayer::Impl>(this);
    this->m_impl->setupScrollLayer(rect, scrollWheelEnabled, vertical);
}

ScrollLayer::~ScrollLayer() = default;

ScrollLayer* ScrollLayer::create(CCRect const& rect, bool scroll, bool vertical) {
    auto ret = new ScrollLayer(rect, scroll, vertical);
    ret->autorelease();
    return ret;
}

ScrollLayer* ScrollLayer::create(CCSize const& size, bool scroll, bool vertical) {
    return ScrollLayer::create({ 0, 0, size.width, size.height }, scroll, vertical);
}

Layout* ScrollLayer::createDefaultListLayout(float gap) {
    return SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::End)
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setGap(gap)
        ->ignoreInvisibleChildren(false);
}
