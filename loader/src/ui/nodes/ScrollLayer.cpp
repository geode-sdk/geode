#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/platform/CCEGLView.h>
#include <Geode/cocos/touch_dispatcher/CCTouchHandler.h>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/utils/cocos.hpp>
#include <algorithm>

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
    bool m_cancellingTouches = false;
    // robtop doesnt implement this simple check but lets add it anyway
    bool m_alreadyCancelled = false;
    bool m_stealingTouches = false;
    bool m_touchClipping = true;
    bool m_scrollClipping = true;
    float m_cancelTouchLimit = 10.f;
    geode::Ref<CCNode> m_defaultStencil = nullptr;
    geode::Ref<CCNode> m_stencil = nullptr;
    geode::Ref<CCClippingNode> m_clippingNode = nullptr;

    Impl(ScrollLayer* self);

    void visit();
    bool scrollWheel(float y, float x);
    void enableScrollWheel(bool enable);
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void ccTouchMoved(CCTouch* touch, CCEvent* event);
    void ccTouchEnded(CCTouch* touch, CCEvent* event);
    void ccTouchCancelled(CCTouch* touch, CCEvent* event);
    void setupScrollLayer(CCRect const& rect, bool scrollWheelEnabled, bool vertical);
    bool pointInsideRect(CCPoint const& point);
    CCNode* getRealStencil();
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

CCNode* ScrollLayer::Impl::getRealStencil() {
    if (!m_defaultStencil) {
        // i would use a CCLayerColor, but turns out CCClippingNode doesnt support them!
        m_defaultStencil = NineSlice::create("cc_2x2_white_image");
    }

    return m_stencil ? m_stencil : m_defaultStencil;
}

bool ScrollLayer::Impl::pointInsideRect(CCPoint const& point) {
    auto local = m_self->convertToNodeSpace(point);
    return CCRect{CCPointZero, m_self->getContentSize()}.containsPoint(local);
}

void ScrollLayer::Impl::visit() {
    if (!m_self->isVisible())
        return;

    if (!m_self->m_cutContent)
        return m_clippingNode->CCNode::visit();

    m_clippingNode->setContentSize(m_self->getContentSize());
    auto stencil = getRealStencil();
    stencil->setContentSize(m_self->getContentSize());
    stencil->setAnchorPoint(CCPointZero);
    stencil->setPosition(CCPointZero);

    m_self->CCNode::visit();
}

bool ScrollLayer::Impl::scrollWheel(float y, float x) {
    if (m_scrollWheelEnabled) {
        if (!geode::cocos::nodeIsVisible(m_self))
            return false;

        if (m_scrollClipping) {
            #ifdef GEODE_IS_DESKTOP
            if (!pointInsideRect(geode::cocos::getMousePos()))
                return false;
            #endif
        }

        m_self->scrollLayer(y);
        return true;
    }

    return false;
}

void ScrollLayer::Impl::enableScrollWheel(bool enable) { m_scrollWheelEnabled = enable; }

bool ScrollLayer::Impl::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    m_alreadyCancelled = false;
    if (nodeIsVisible(m_self)) {
        return m_self->CCScrollLayerExt::ccTouchBegan(touch, event);
    }
    return false;
}

void ScrollLayer::Impl::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    m_self->CCScrollLayerExt::ccTouchMoved(touch, event);

    // Don't shoot the messenger, leave your complaints to robtop
    // this is the implementation of TableView::cancelAndStoleTouch
    if (m_stealingTouches && !m_alreadyCancelled && std::abs(touch->getLocation().y - touch->getStartLocation().y) > m_cancelTouchLimit) {
        auto dispatcher = CCDirector::get()->m_pTouchDispatcher;

        auto touches = CCSet::create();
        touches->addObject(touch);

        m_cancellingTouches = true;
        dispatcher->touchesCancelled(touches, event);
        m_cancellingTouches = false;

        if (auto handler = static_cast<CCTargetedTouchHandler*>(dispatcher->findHandler(m_self))) {
            if (auto claimedTouches = handler->m_pClaimedTouches) {
                if (!claimedTouches->containsObject(touch)) {
                    claimedTouches->addObject(touch);
                }
            }
        }

        m_alreadyCancelled = true;
    }
}

void ScrollLayer::Impl::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    m_self->CCScrollLayerExt::ccTouchEnded(touch, event);
}

void ScrollLayer::Impl::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
    if (!m_cancellingTouches) {
        m_self->CCScrollLayerExt::ccTouchCancelled(touch, event);
    }
}

void ScrollLayer::Impl::setupScrollLayer(CCRect const& rect, bool scrollWheelEnabled, bool vertical) {
    m_self->ignoreAnchorPointForPosition(false);
    m_self->setAnchorPoint(ccp(0.5f, 0.5f));

    m_scrollWheelEnabled = scrollWheelEnabled;

    m_self->m_disableVertical = !vertical;
    m_self->m_disableHorizontal = vertical;
    m_self->m_cutContent = true;

    m_clippingNode = CCClippingNode::create(getRealStencil());
    m_clippingNode->setAlphaThreshold(0.03f);
    m_clippingNode->setContentSize(rect.size); 
    m_self->addChild(m_clippingNode);

    m_self->m_contentLayer->removeFromParent();
    m_self->m_contentLayer = GenericContentLayer::create(rect.size.width, rect.size.height);
    m_self->m_contentLayer->setID("content-layer");
    m_self->m_contentLayer->setAnchorPoint({ 0, 0 });
    m_clippingNode->addChild(m_self->m_contentLayer);

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

bool ScrollLayer::scrollWheel(float y, float x) {
    return m_impl->scrollWheel(y, x);
}

void ScrollLayer::enableScrollWheel(bool enable) {
    m_impl->enableScrollWheel(enable);
}

bool ScrollLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    return m_impl->ccTouchBegan(touch, event);
}

void ScrollLayer::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    m_impl->ccTouchMoved(touch, event);
}

void ScrollLayer::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    m_impl->ccTouchEnded(touch, event);
}

void ScrollLayer::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
    m_impl->ccTouchCancelled(touch, event);
}

bool ScrollLayer::touchInsideClip(cocos2d::CCPoint const& worldPoint) {
    if (!m_cutContent || !m_impl->m_touchClipping) {
        return true;
    }

    return m_impl->pointInsideRect(worldPoint);
}

float ScrollLayer::getScrollFromTop() const {
    float viewHeight = this->getContentHeight();
    float contentHeight = this->m_contentLayer->getContentHeight();
    return this->m_contentLayer->getPositionY() - (viewHeight - contentHeight);
}

void ScrollLayer::scrollToFromTop(float offset) {
    float viewHeight = this->getContentHeight();
    float contentHeight = this->m_contentLayer->getContentHeight();
    float maxOffset = std::max(0.f, contentHeight - viewHeight);
    this->m_contentLayer->setPositionY(
        (viewHeight - contentHeight) + std::clamp(offset, 0.f, maxOffset)
    );
}

void ScrollLayer::scrollToTop() {
    this->scrollToFromTop(0.f);
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

void ScrollLayer::setCancelTouchLimit(float limit) {
    m_impl->m_cancelTouchLimit = limit;
}
float ScrollLayer::getCancelTouchLimit() const {
    return m_impl->m_cancelTouchLimit;
}

void ScrollLayer::setStealingTouches(bool steal) {
    m_impl->m_stealingTouches = steal;
}

bool ScrollLayer::isStealingTouches() const {
    return m_impl->m_stealingTouches;
}

void ScrollLayer::enableTouchClipping(bool clip) {
    m_impl->m_touchClipping = clip;
}

bool ScrollLayer::hasTouchClipping() const {
    return m_impl->m_touchClipping;
}

void ScrollLayer::enableScrollClipping(bool clip) {
    m_impl->m_scrollClipping = clip;
}

bool ScrollLayer::hasScrollClipping() const {
    return m_impl->m_scrollClipping;
}

void ScrollLayer::setStencil(cocos2d::CCNode* stencil) {
    m_impl->m_stencil = stencil;
    m_impl->m_clippingNode->setStencil(m_impl->getRealStencil());
}

cocos2d::CCNode* ScrollLayer::getStencil() const {
    return m_impl->m_stencil;
}

cocos2d::CCClippingNode* ScrollLayer::getClippingNode() const {
    return m_impl->m_clippingNode;
}