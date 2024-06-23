#include <Geode/ui/Scrollbar.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/binding/CCContentLayer.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

bool Scrollbar::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    // hitbox
    auto const size = this->getContentSize();
    auto const pos = this->convertToNodeSpace(touch->getLocation());
    auto const rect = CCRect{0, 0, size.width, size.height};

    if (!m_target || !rect.containsPoint(pos)) return false;

    // trigger scrollbar thumb move
    this->ccTouchMoved(touch, event);

    m_touchDown = true;

    return true;
}

void Scrollbar::ccTouchEnded(CCTouch*, CCEvent*) {
    m_touchDown = false;
}

void Scrollbar::ccTouchCancelled(CCTouch*, CCEvent*) {
    m_touchDown = false;
}

void Scrollbar::ccTouchMoved(CCTouch* touch, CCEvent*) {
    if (!m_target) return;

    auto pos = this->convertToNodeSpace(touch->getLocation());

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

void Scrollbar::scrollWheel(float x, float y) {
    if (!m_target) return;
    m_target->scrollWheel(x, y);
}

void Scrollbar::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void Scrollbar::draw() {
    CCLayer::draw();

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
    m_track->setPosition(m_obContentSize / 2);

    this->setContentSize({ m_width, targetHeight });

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

    m_thumb->setPosition(m_obContentSize / 2 + ccp(0.f, thumbPosY));
    if (m_resizeThumb) {
        m_thumb->setContentSize({ m_width, thumbHeight });
    }
}

void Scrollbar::setTarget(CCScrollLayerExt* target) {
    m_target = target;
}

bool Scrollbar::init(CCScrollLayerExt* target) {
    if (!this->CCLayer::init()) return false;

    this->ignoreAnchorPointForPosition(false);

    m_target = target;

    if (cocos::fileExistsInSearchPaths("scrollbar.png"_spr)) {
        m_track = CCScale9Sprite::create("scrollbar.png"_spr);
        m_track->setColor({ 0, 0, 0 });
        m_track->setOpacity(150);
        m_track->setScale(.8f);

        m_thumb = CCScale9Sprite::create("scrollbar.png"_spr);
        m_thumb->setScale(.4f);

        m_width = 8.f;
        m_resizeThumb = true;
        m_trackIsRotated = false;
        m_hoverHighlight = true;
    }
    else {
        m_track = CCScale9Sprite::create("slidergroove.png");
        m_track->setRotation(90);
        m_track->setScale(.8f);

        m_thumb = CCScale9Sprite::create("sliderthumb.png");
        m_thumb->setScale(.6f);

        m_width = 12.f;
        m_resizeThumb = false;
        m_trackIsRotated = true;
        m_hoverHighlight = false;
    }

    this->addChild(m_track);
    this->addChild(m_thumb);

    this->setTouchEnabled(true);

    return true;
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
