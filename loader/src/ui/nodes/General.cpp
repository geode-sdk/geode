#include "Geode/utils/ColorProvider.hpp"
#include <Geode/ui/General.hpp>
#include <cocos-ext.h>

using namespace geode::prelude;

CCSprite* geode::createLayerBG() {
    auto winSize = CCDirector::get()->getWinSize();

    auto bg = CCSprite::create("GJ_gradientBG.png");
    bg->setID("bg");
    auto bgSize = bg->getTextureRect().size;

    bg->setAnchorPoint({ 0.0f, 0.0f });
    bg->setScaleX((winSize.width + 10.0f) / bgSize.width);
    bg->setScaleY((winSize.height + 10.0f) / bgSize.height);
    bg->setPosition({ -5.0f, -5.0f });
    bg->setColor(ColorProvider::get()->color3b("mods-layer-gd-bg"_spr)); // todo: let mods customize this

    return bg;
}

void geode::addSideArt(CCNode* to, SideArt sides, SideArtStyle style, bool useAnchorLayout) {
    const char* sprite;
    float offset;
    switch (style) {
        default:
        case SideArtStyle::Layer:     sprite = "GJ_sideArt_001.png";       offset = 35; break;
        case SideArtStyle::LayerGray: sprite = "gauntletCorner_001.png";   offset = 35; break;
        case SideArtStyle::PopupBlue: sprite = "rewardCorner_001.png";     offset = 24.75f; break;
        case SideArtStyle::PopupGold: sprite = "dailyLevelCorner_001.png"; offset = 24.75f; break;
    }
    if (sides & SideArt::BottomLeft) {
        auto spr = CCSprite::createWithSpriteFrameName(sprite);
        spr->setID("side-art-bottom-left");
        to->addChildAtPosition(spr, Anchor::BottomLeft, ccp(offset, offset), useAnchorLayout);
    }
    if (sides & SideArt::BottomRight) {
        auto spr = CCSprite::createWithSpriteFrameName(sprite);
        spr->setID("side-art-bottom-right");
        spr->setFlipX(true);
        to->addChildAtPosition(spr, Anchor::BottomRight, ccp(-offset, offset), useAnchorLayout);
    }
    if (sides & SideArt::TopLeft) {
        auto spr = CCSprite::createWithSpriteFrameName(sprite);
        spr->setID("side-art-top-left");
        spr->setFlipY(true);
        to->addChildAtPosition(spr, Anchor::TopLeft, ccp(offset, -offset), useAnchorLayout);
    }
    if (sides & SideArt::TopRight) {
        auto spr = CCSprite::createWithSpriteFrameName(sprite);
        spr->setID("side-art-top-right");
        spr->setFlipX(true);
        spr->setFlipY(true);
        to->addChildAtPosition(spr, Anchor::TopRight, ccp(-offset, -offset), useAnchorLayout);
    }
}
void geode::addSideArt(CCNode* to, SideArt sides, bool useAnchorLayout) {
    return addSideArt(to, sides, SideArtStyle::Layer, useAnchorLayout);
}

void geode::addListBorders(CCNode* to, CCPoint const& center, CCSize const& size) {
    // if the size is 346.f, the top aligns perfectly by default :3
    if (size.width == 346.f) {
        auto layerTopSpr = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
        layerTopSpr->setPosition({
            center.x,
            center.y + size.height / 2 - 5.f
        });
        to->addChild(layerTopSpr);

        auto layerBottomSpr = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
        layerBottomSpr->setFlipY(true);
        layerBottomSpr->setPosition({
            center.x,
            center.y - size.height / 2 + 5.f
        });
        to->addChild(layerBottomSpr);
    }
    // otherwise stretch using CCScale9Sprite
    else {
        auto layerTopSpr = CCScale9Sprite::createWithSpriteFrameName(
            "GJ_commentTop_001.png",
            { 0, 0, 240, 20 }
        );
        layerTopSpr->setContentSize({
            size.width + 9.f,
            layerTopSpr->getContentSize().height,
        });
        layerTopSpr->setPosition({
            center.x,
            center.y + size.height / 2 - 5.f
        });
        to->addChild(layerTopSpr);

        auto layerBottomSpr = CCScale9Sprite::createWithSpriteFrameName(
            "GJ_commentTop_001.png",
            { 0, 0, 240, 20 }
        );
        layerBottomSpr->setScaleY(-1);
        layerBottomSpr->setContentSize({
            size.width + 9.f,
            layerBottomSpr->getContentSize().height,
        });
        layerBottomSpr->setPosition({
            center.x,
            center.y - size.height / 2 + 5.f
        });
        to->addChild(layerBottomSpr);
    }

    auto layerLeftSpr = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
    layerLeftSpr->setScaleY(
        (size.height - 30.f) / layerLeftSpr->getScaledContentSize().height
    );
    layerLeftSpr->setPosition({
        center.x - size.width / 2 - .5f,
        center.y
    });
    to->addChild(layerLeftSpr);

    auto layerRightSpr = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
    layerRightSpr->setScaleY(
        (size.height - 30.f) / layerRightSpr->getScaledContentSize().height
    );
    layerRightSpr->setFlipX(true);
    layerRightSpr->setPosition({
        center.x + size.width / 2 + .5f,
        center.y
    });
    to->addChild(layerRightSpr);
}

bool ListBorders::init() {
    if (!CCNode::init())
        return false;

    this->setAnchorPoint({ .5f, .5f });
    this->setSpriteFrames("GJ_commentTop_001.png", "GJ_commentSide_001.png");

    return true;
}

ListBorders* ListBorders::create() {
    auto ret = new ListBorders();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void ListBorders::setSpriteFrames(const char* topAndBottom, const char* side, float horizontalPadding) {
    this->setSprites(
        CCScale9Sprite::createWithSpriteFrameName(topAndBottom),
        CCScale9Sprite::createWithSpriteFrameName(topAndBottom),
        CCSprite::createWithSpriteFrameName(side),
        CCSprite::createWithSpriteFrameName(side),
        horizontalPadding,
        horizontalPadding
    );
    m_bottom->setScaleY(-1);
    m_right->setFlipX(true);
}
void ListBorders::setSprites(
    CCScale9Sprite* top, CCScale9Sprite* bottom,
    CCSprite* left, CCSprite* right,
    float topPadding, float bottomPadding
) {
    if (m_top) m_top->removeFromParent();
    if (m_bottom) m_bottom->removeFromParent();
    if (m_left) m_left->removeFromParent();
    if (m_right) m_right->removeFromParent();

    m_top = top;
    this->addChildAtPosition(m_top, Anchor::Top, ccp(0, -m_top->getContentHeight() / 3));

    m_bottom = bottom;
    this->addChildAtPosition(m_bottom, Anchor::Bottom, ccp(0, m_bottom->getContentHeight() / 3));

    m_left = left;
    this->addChildAtPosition(m_left, Anchor::Left, ccp(0, 0));

    m_right = right;
    this->addChildAtPosition(m_right, Anchor::Right, ccp(0, 0));

    m_topPadding = topPadding;
    m_bottomPadding = bottomPadding;

    this->setContentSize(m_obContentSize);
}
void ListBorders::setContentSize(CCSize const& size) {
    CCNode::setContentSize(size);
    this->updateLayout();

    m_top->setContentWidth(size.width + m_topPadding);
    m_bottom->setContentWidth(size.width + m_bottomPadding);
    auto height = m_top->getContentHeight() * 0.75 + m_bottom->getContentHeight() * 0.75;
    m_left->setScaleY((size.height - height) / m_left->getContentHeight());
    m_right->setScaleY((size.height - height) / m_right->getContentHeight());
}
