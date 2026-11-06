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
    bool m_touchDown = false;
    Scrollbar::Style m_style = Scrollbar::Style::Geode;
    NineSlice::Insets m_insets = {};
    bool m_shrinkWithOverscroll = false;
    bool m_onlyTouchInThumb = false;
    float m_touchOffsetLocalY = 0;
    cocos2d::ccColor4B m_selectedColor = ccc4(255, 255, 255, 255);
    cocos2d::ccColor4B m_normalColor = ccc4(255, 255, 255, 255);

    Impl(Scrollbar* self) : m_self(self) {}

    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void ccTouchEnded(CCTouch*, CCEvent*);
    void ccTouchCancelled(CCTouch*, CCEvent*);
    void ccTouchMoved(CCTouch* touch, CCEvent*);
    bool scrollWheel(float x, float y);
    void registerWithTouchDispatcher();
    void draw();
    bool init(CCScrollLayerExt* target, Style style);
    void setTarget(CCScrollLayerExt* target) { m_target = target; }
    void setStyle(Scrollbar::Style style);
    float calcThumbHeightP();
    float calcTargetScrollP();
};

bool Scrollbar::Impl::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    // hitbox
    auto const size = m_self->getContentSize();
    auto const pos = m_self->convertToNodeSpace(touch->getLocation());
    auto const rect = m_onlyTouchInThumb ? CCRect{m_thumb->getPosition() - m_thumb->getScaledContentSize() * m_thumb->getAnchorPoint(), m_thumb->getScaledContentSize()} : CCRect{0, 0, size.width, size.height};

    if (!m_target || !rect.containsPoint(pos)) return false;

    if (m_onlyTouchInThumb)
        m_touchOffsetLocalY = pos.y - m_thumb->getPositionY();
    else
        m_touchOffsetLocalY = 0;

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

    pos.y -= m_touchOffsetLocalY;
    auto posY = h *
        ((-pos.y + thumbHeight / 4 - 5) / (targetHeight - thumbHeight / 2 + 10));

    if (posY > 0.0f) posY = 0.0f;
    if (posY < -h) posY = -h;

    m_target->m_contentLayer->setPositionY(posY);
}

bool Scrollbar::Impl::scrollWheel(float x, float y) {
    if (!m_target) return false;
    return m_target->scrollWheel(x, y);
}

void Scrollbar::Impl::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(m_self, 0, true);
}

float Scrollbar::Impl::calcThumbHeightP() {
    float percent = m_target->getScaledContentHeight() / m_target->m_contentLayer->getScaledContentHeight();

    if (m_shrinkWithOverscroll) {
        float pos = m_target->m_contentLayer->getPositionY();
        float min = m_target->getMinY();
        float max = m_target->getMaxY();

        float overScroll = 0;

        if (pos < min) {
            overScroll = min - pos;
        } else if (pos > max) {
            overScroll = pos - max;
        }

        if (overScroll > 0)
            percent *= (1.0f / (1.0f + (overScroll / m_target->getScaledContentHeight())));    
    }

    if (m_target->m_contentLayer->getScaledContentHeight() <= m_target->getScaledContentHeight())
        percent = 1.0f;

    return percent;
}

float Scrollbar::Impl::calcTargetScrollP() {
    auto fTo01 = [&](float value, float min, float max) -> float {
        if (min == max)
            return 0;
        return (value - min) / (max - min);
    };

    return std::clamp<float>(fTo01(m_target->m_contentLayer->getPositionY(), m_target->getMaxY(), m_target->getMinY()), 0, 1);
}

void Scrollbar::Impl::draw() {
    m_self->CCLayer::draw();

    if (!m_target) return;

    auto contentHeight = m_target->m_contentLayer->getScaledContentHeight();
    auto targetHeight = m_target->getScaledContentHeight();

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

    float scrollP = calcTargetScrollP();
    float min = m_insets.bottom;
    float max = m_self->getContentHeight() - m_insets.top;
    float thumbH = m_resizeThumb ? calcThumbHeightP() * (max - min) : m_thumb->getScaledContentHeight();

    if (m_touchDown) {
        m_thumb->setColor(ccc3(m_selectedColor.r, m_selectedColor.g, m_selectedColor.b));
        m_thumb->setOpacity(m_selectedColor.a);
    } else {
        m_thumb->setColor(ccc3(m_normalColor.r, m_normalColor.g, m_normalColor.b));
        m_thumb->setOpacity(m_normalColor.a);
    }


    m_thumb->setPosition(ccp(std::lerp<double>(m_insets.left, m_self->m_obContentSize.width - m_insets.right, 0.5f), std::lerp<double>(min + thumbH / 2, max - thumbH / 2, scrollP)));
    if (m_resizeThumb) {
        m_thumb->setContentSize(ccp(m_width - m_insets.left - m_insets.right, thumbH));
    }
}

bool Scrollbar::Impl::init(CCScrollLayerExt* target, Style style) {
    if (!m_self->CCLayer::init()) return false;

    m_self->ignoreAnchorPointForPosition(false);

    m_target = target;

    setStyle(style);

    m_self->setTouchEnabled(true);
    return true;
}

void Scrollbar::Impl::setStyle(Scrollbar::Style style) {
    m_style = style;

    if (m_track)
        m_track->removeFromParent();

    if (m_thumb)
        m_thumb->removeFromParent();

    switch (style)
    {
        case Style::GDScrollbar: {
            m_track = NineSlice::create("cc_2x2_white_image");
            m_track->setOpacity(0);

            m_thumb = NineSlice::create("cc_2x2_white_image");

            m_width = 3;
            m_resizeThumb = true;
            m_trackIsRotated = false;
            m_shrinkWithOverscroll = false;
            m_insets = { 1.0f, 0.0f, 1.0f, 0.0f };
            m_normalColor = m_selectedColor = ccc4(255, 255, 255, 125);
            m_onlyTouchInThumb = true;
            break;
        }

        case Style::GDSlider: {
            m_track = NineSlice::create("slidergroove.png");
            m_track->setRotation(90);
            m_track->setScaleMultiplier(0.8f);

            m_thumb = NineSlice::create("sliderthumb.png");
            m_thumb->setScale(0.6f);

            m_width = 12.f;
            m_resizeThumb = false;
            m_trackIsRotated = true;
            m_shrinkWithOverscroll = false;
            m_insets = { 0.0f, 0.0f, 0.0f, 0.0f };
            m_normalColor = ccc4(255, 255, 255, 255);
            m_selectedColor = ccc4(125, 125, 125, 255);
            m_onlyTouchInThumb = false;
            break;
        }

        default:
        case Style::Geode: {
            m_track = NineSlice::create("scrollbar.png"_spr);
            m_track->setColor({ 0, 0, 0 });
            m_track->setOpacity(150);
            m_track->setScaleMultiplier(0.8f);

            m_thumb = NineSlice::create("scrollbar.png"_spr);
            m_thumb->setScaleMultiplier(0.4f);

            m_width = 8.f;
            m_resizeThumb = true;
            m_trackIsRotated = false;
            m_shrinkWithOverscroll = true;
            m_insets = { 2.4f, 2.4f, 2.4f, 2.4f };
            m_normalColor = ccc4(100, 100, 100, 255);
            m_selectedColor = ccc4(255, 255, 255, 255);
            m_onlyTouchInThumb = false;
            break;
        }
    }

    m_self->addChild(m_track);
    m_self->addChild(m_thumb);

    m_self->draw();
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
// TODO: 2.209
bool Scrollbar::scrollWheel(float x, float y) {
    return m_impl->scrollWheel(x, y);
}
void Scrollbar::registerWithTouchDispatcher() { 
    m_impl->registerWithTouchDispatcher(); 
}
void Scrollbar::draw() { 
    m_impl->draw(); 
}
bool Scrollbar::init(CCScrollLayerExt* target, Style style) {
    return m_impl->init(target, style);
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

Scrollbar::Style Scrollbar::getStyle() const {
    return m_impl->m_style;
}

void Scrollbar::setStyle(Style style) {
    m_impl->setStyle(style);
}

void Scrollbar::setShrinkWithOverscroll(bool shrink) {
    m_impl->m_shrinkWithOverscroll = shrink;
}

bool Scrollbar::getShrinkWithOverscroll() const {
    return m_impl->m_shrinkWithOverscroll;
}

void Scrollbar::setTouchInThumb(bool onlyInThumb) {
    m_impl->m_onlyTouchInThumb = onlyInThumb;
}

bool Scrollbar::getTouchInThumb() const {
    return m_impl->m_onlyTouchInThumb;
}

void Scrollbar::setThumbColors(cocos2d::ccColor4B normal, cocos2d::ccColor4B selected) {
    m_impl->m_normalColor = normal;
    m_impl->m_selectedColor = selected;
}

Scrollbar* Scrollbar::create(CCScrollLayerExt* target, Style style) {
    auto ret = new Scrollbar;

    if (ret->init(target, style)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
