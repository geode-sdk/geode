#include <Geode/ui/Scrollbar.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/binding/CCContentLayer.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

class Scrollbar::Impl {
public:
    Scrollbar* m_self = nullptr;
    CCScrollLayerExt* m_target = nullptr;
    NineSlice* m_track = nullptr;
    NineSlice* m_thumb = nullptr;
    cocos2d::CCPoint m_clickOffset{};
    float m_width = 0.f;
    bool m_resizeThumb = false;
    bool m_trackIsRotated = false;
    bool m_hoverHighlight = false;
    bool m_touchDown = false;

    Impl(Scrollbar* self) : m_self(self) {}

    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void ccTouchEnded(CCTouch*, CCEvent*);
    void ccTouchCancelled(CCTouch*, CCEvent*);
    void ccTouchMoved(CCTouch* touch, CCEvent*);
    void scrollWheel(float x, float y);
    void registerWithTouchDispatcher();
    void draw();
    bool init(CCScrollLayerExt* target);
    void setTarget(CCScrollLayerExt* target) { m_target = target; }
};

bool Scrollbar::Impl::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    // hitbox
    auto const size = m_self->getContentSize();
    auto const pos = m_self->convertToNodeSpace(touch->getLocation());
    auto const rect = CCRect{0, 0, size.width, size.height};

    if (!m_target || !rect.containsPoint(pos)) return false;

    // trigger scrollbar thumb move
    m_self->ccTouchMoved(touch, event);

    m_touchDown = true;

    return true;
}

void Scrollbar::Impl::ccTouchEnded(CCTouch*, CCEvent*) { m_touchDown = false; }
void Scrollbar::Impl::ccTouchCancelled(CCTouch*, CCEvent*) { m_touchDown = false; }

void Scrollbar::Impl::ccTouchMoved(CCTouch* touch, CCEvent*) {
    if (!m_target) return;

    auto pos = m_self->convertToNodeSpace(touch->getLocation());

    auto contentHeight = m_target->m_contentLayer->getScaledContentSize().height;
    auto targetHeight = m_target->getScaledContentSize().height;

    auto h = contentHeight - targetHeight + m_target->m_scrollLimitTop;
    auto p = targetHeight / contentHeight;

    auto thumbHeight = m_resizeThumb ? std::min(p, 1.f) * targetHeight / .4f : 0;

    auto posY = h *
        ((-pos.y + thumbHeight / 4 - 5) / (targetHeight - thumbHeight / 2 + 10));

    if (posY > 0.0f) posY = 0.0f;
    if (posY < -h) posY = -h;

    m_target->m_contentLayer->setPositionY(posY);
}

void Scrollbar::Impl::scrollWheel(float x, float y) {
    if (!m_target) return;
    m_target->scrollWheel(x, y);
}

void Scrollbar::Impl::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(m_self, 0, true);
}

void Scrollbar::Impl::draw() {
    m_self->CCLayer::draw();

    if (!m_target) return;

    auto contentHeight = m_target->m_contentLayer->getScaledContentSize().height;
    auto targetHeight = m_target->getScaledContentSize().height;

    if (m_trackIsRotated) {
        m_track->setContentSize({ targetHeight / m_track->getScale(),
                                  m_width / m_track->getScale() });
    }
    else {
        m_track->setContentSize({ m_width / m_track->getScale(),
                                  targetHeight / m_track->getScale() });
    }
    m_track->setPosition(m_self->m_obContentSize / 2);

    m_self->setContentSize({ m_width, targetHeight });

    auto h = contentHeight - targetHeight + m_target->m_scrollLimitTop;
    auto p = targetHeight / contentHeight;

    GLubyte o;
    if (m_hoverHighlight) {
        o = 100;
        // if (m_extMouseHovered) {
        // o = 160;
        // }
        if (m_touchDown) {
            o = 255;
        }
    }
    else {
        o = 255;
        if (m_touchDown) {
            o = 125;
        }
    }
    m_thumb->setColor({ o, o, o });

    auto y = m_target->m_contentLayer->getPositionY();

    auto thumbHeight = m_resizeThumb ? std::min(p, 1.f) * targetHeight / .4f : 0;
    if (thumbHeight < 15.f) {
        thumbHeight = 15.f;
    }

    auto thumbPosY = -targetHeight / 2 + thumbHeight / 4 - 5.0f +
        (h ? (-y) / h : 1.f) * (targetHeight - thumbHeight / 2 + 10.0f);

    auto fHeightTop = [&]() -> float {
        return thumbPosY - targetHeight / 2 + thumbHeight * .4f / 2 + 3.0f;
    };
    auto fHeightBottom = [&]() -> float {
        return thumbPosY + targetHeight / 2 - thumbHeight * .4f / 2 - 3.0f;
    };

    if (fHeightTop() > 0.0f) {
        thumbHeight -= fHeightTop();
        if (thumbHeight < 15.f) {
            thumbHeight = 15.f;
        }
        thumbPosY -= fHeightTop();
    }

    if (fHeightBottom() < 0.f) {
        thumbHeight += fHeightBottom();
        if (thumbHeight < 15.f) {
            thumbHeight = 15.f;
        }
        thumbPosY -= fHeightBottom();
    }

    m_thumb->setPosition(m_self->m_obContentSize / 2 + ccp(0.f, thumbPosY));
    if (m_resizeThumb) {
        m_thumb->setContentSize({ m_width, thumbHeight });
    }
}

bool Scrollbar::Impl::init(CCScrollLayerExt* target) {
    if (!m_self->CCLayer::init()) return false;

    m_self->ignoreAnchorPointForPosition(false);

    m_target = target;

    if (cocos::fileExistsInSearchPaths("scrollbar.png"_spr)) {
        m_track = NineSlice::create("scrollbar.png"_spr);
        m_track->setColor({ 0, 0, 0 });
        m_track->setOpacity(150);
        m_track->setScale(.8f);

        m_thumb = NineSlice::create("scrollbar.png"_spr);
        m_thumb->setScale(.4f);

        m_width = 8.f;
        m_resizeThumb = true;
        m_trackIsRotated = false;
        m_hoverHighlight = true;
    }
    else {
        m_track = NineSlice::create("slidergroove.png");
        m_track->setRotation(90);
        m_track->setScale(.8f);

        m_thumb = NineSlice::create("sliderthumb.png");
        m_thumb->setScale(.6f);

        m_width = 12.f;
        m_resizeThumb = false;
        m_trackIsRotated = true;
        m_hoverHighlight = false;
    }

    m_self->addChild(m_track);
    m_self->addChild(m_thumb);

    m_self->setTouchEnabled(true);

    return true;
}

// Scrollbar public forwards / wiring

Scrollbar::Scrollbar() : m_impl(std::make_unique<Scrollbar::Impl>(this)) {}
Scrollbar::~Scrollbar() = default;

bool Scrollbar::ccTouchBegan(CCTouch* touch, CCEvent* event) { 
    return m_impl->ccTouchBegan(touch, event); 
}
void Scrollbar::ccTouchEnded(CCTouch* t, CCEvent* e) { 
    m_impl->ccTouchEnded(t, e); 
}
void Scrollbar::ccTouchCancelled(CCTouch* t, CCEvent* e) { 
    m_impl->ccTouchCancelled(t, e); 
}
void Scrollbar::ccTouchMoved(CCTouch* t, CCEvent* e) { 
    m_impl->ccTouchMoved(t, e); 
}
void Scrollbar::scrollWheel(float x, float y) { 
    m_impl->scrollWheel(x, y); 
}
void Scrollbar::registerWithTouchDispatcher() { 
    m_impl->registerWithTouchDispatcher(); 
}
void Scrollbar::draw() { 
    m_impl->draw(); 
}
bool Scrollbar::init(CCScrollLayerExt* target) {
    return m_impl->init(target);
}

bool Scrollbar::isTouching() {
    return m_impl->m_touchDown;
}

CCScrollLayerExt* Scrollbar::getTarget() {
    return m_impl->m_target;
}

NineSlice* Scrollbar::getTrack() {
    return m_impl->m_track;
}

NineSlice* Scrollbar::getThumb() {
    return m_impl->m_thumb;
}

void Scrollbar::setTarget(CCScrollLayerExt* list) {
    m_impl->setTarget(list);
}

void Scrollbar::setTrack(NineSlice* track) {
    m_impl->m_track = track;
}

void Scrollbar::setThumb(NineSlice* thumb) {
    m_impl->m_thumb = thumb;
}

Scrollbar* Scrollbar::create(CCScrollLayerExt* target) {
    auto ret = new Scrollbar;

    if (ret->init(target)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
