#include <Geode/ui/Scrollbar.hpp>
#include <Geode/Utils.hpp>

// TODO: die
#undef min
#undef max

USE_GEODE_NAMESPACE();

// bool Scrollbar::mouseDownExt(MouseEvent, cocos2d::CCPoint const& mpos) {
//     if (!m_target) return false;

//     ExtMouseDispatcher::get()->attainCapture(this);

//     auto pos = this->convertToNodeSpace(mpos);

//     auto contentHeight = m_target->m_contentLayer->getScaledContentSize().height;
//     auto targetHeight = m_target->getScaledContentSize().height;

//     auto h = contentHeight - targetHeight + m_target->m_scrollLimitTop;
//     auto p = targetHeight / contentHeight;

//     auto thumbHeight = m_resizeThumb ? std::min(p, 1.f) * targetHeight / .4f : 0;

//     auto posY = h * (
//         (-pos.y - targetHeight / 2 + thumbHeight / 4 - 5) /
//         (targetHeight - thumbHeight / 2 + 10)
//     );

//     if (posY > 0.0f) posY = 0.0f;
//     if (posY < -h) posY = -h;
    
//     auto offsetY = m_target->m_contentLayer->getPositionY() - posY;

//     return true;
// }

// bool Scrollbar::mouseUpExt(MouseEvent, cocos2d::CCPoint const&) {
//     ExtMouseDispatcher::get()->releaseCapture(this);
//     return true;
// }

// void Scrollbar::mouseMoveExt(cocos2d::CCPoint const& mpos) {
//     if (!m_target || !ExtMouseDispatcher::get()->isCapturing(this)) return;

//     if (this->m_extMouseDown.size()) {
//         auto pos = this->convertToNodeSpace(mpos);

//         auto contentHeight = m_target->m_contentLayer->getScaledContentSize().height;
//         auto targetHeight = m_target->getScaledContentSize().height;
        
//         auto h = contentHeight - targetHeight + m_target->m_scrollLimitTop;
//         auto p = targetHeight / contentHeight;

//         auto thumbHeight = m_resizeThumb ? std::min(p, 1.f) * targetHeight / .4f : 0;

//         auto posY = h * (
//             (-pos.y - targetHeight / 2 + thumbHeight / 4 - 5) /
//             (targetHeight - thumbHeight / 2 + 10)
//         );

//         if (posY > 0.0f) posY = 0.0f;
//         if (posY < -h) posY = -h;

//         m_target->m_contentLayer->setPositionY(posY);
//     }
// }

void Scrollbar::scrollWheel(float x, float y) {
    if (!m_target) return;
    m_target->scrollWheel(x, y);
}

void Scrollbar::draw() {
    CCLayer::draw();

    if (!m_target) return;

    auto contentHeight = m_target->m_contentLayer->getScaledContentSize().height;
    auto targetHeight = m_target->getScaledContentSize().height;
    
    if (m_trackIsRotated) {
        m_track->setContentSize({
            targetHeight / m_track->getScale(),
            m_width / m_track->getScale()
        });
    } else {
        m_track->setContentSize({
            m_width / m_track->getScale(),
            targetHeight / m_track->getScale()
        });
    }
    m_track->setPosition(.0f, .0f);

    this->setContentSize({ m_width, targetHeight });

    auto h = contentHeight - targetHeight + m_target->m_scrollLimitTop;
    auto p = targetHeight / contentHeight;

    GLubyte o;
    if (m_hoverHighlight) {
        o = 100;
        // if (m_extMouseHovered) {
            // o = 160;
        // }
        // if (m_extMouseDown.size()) {
            // o = 255;
        // }
    } else {
        o = 255;
        // if (m_extMouseDown.size()) {
        //     o = 125;
        // }
    }
    m_thumb->setColor({ o, o, o });

    auto y = m_target->m_contentLayer->getPositionY();

    auto thumbHeight = m_resizeThumb ? std::min(p, 1.f) * targetHeight / .4f : 0;
    auto thumbPosY = - targetHeight / 2 + thumbHeight / 4 - 5.0f + 
        (h ? (-y) / h : 1.f) * (targetHeight - thumbHeight / 2 + 10.0f);

    auto fHeightTop = [&]() -> float {
        return thumbPosY - targetHeight / 2 + thumbHeight * .4f / 2 + 3.0f;
    };
    auto fHeightBottom = [&]() -> float {
        return thumbPosY + targetHeight / 2 - thumbHeight * .4f / 2 - 3.0f;
    };
    
    if (fHeightTop() > 0.0f) {
        thumbHeight -= fHeightTop();
        thumbPosY -= fHeightTop();
    }
    
    if (fHeightBottom() < 0.f) {
        thumbHeight += fHeightBottom();
        thumbPosY -= fHeightBottom();
    }

    m_thumb->setPosition(0.f, thumbPosY);
    if (m_resizeThumb) {
        m_thumb->setContentSize({ m_width, thumbHeight });
    }
}

void Scrollbar::setTarget(CCScrollLayerExt* target) {
    m_target = target;
}

bool Scrollbar::init(CCScrollLayerExt* target) {
    if (!this->CCLayer::init())
        return false;
    
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
    } else {
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

    // this->registerWithMouseDispatcher();

    return true;
}

Scrollbar* Scrollbar::create(CCScrollLayerExt* target) {
    auto ret = new Scrollbar;

    if (ret && ret->init(target)) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

