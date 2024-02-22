#include <Geode/ui/General.hpp>
#include <cocos-ext.h>

using namespace geode::prelude;

CCSprite* geode::createLayerBG() {
    auto winSize = CCDirector::get()->getWinSize();

    auto bg = CCSprite::create("GJ_gradientBG.png");
    auto bgSize = bg->getTextureRect().size;

    bg->setAnchorPoint({ 0.0f, 0.0f });
    bg->setScaleX((winSize.width + 10.0f) / bgSize.width);
    bg->setScaleY((winSize.height + 10.0f) / bgSize.height);
    bg->setPosition({ -5.0f, -5.0f });
    bg->setColor({ 0, 102, 255 }); // todo: let mods customize this

    return bg;
}

void geode::addSideArt(CCNode* to, SideArt sides, bool useAnchorLayout) {
    if (sides & SideArt::BottomLeft) {
        auto spr = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        to->addChildAtPosition(spr, Anchor::BottomLeft, ccp(35, 35), useAnchorLayout);
    }
    if (sides & SideArt::BottomRight) {
        auto spr = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        spr->setFlipX(true);
        to->addChildAtPosition(spr, Anchor::BottomRight, ccp(-35, 35), useAnchorLayout);
    }
    if (sides & SideArt::TopLeft) {
        auto spr = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        spr->setFlipY(true);
        to->addChildAtPosition(spr, Anchor::TopLeft, ccp(35, -35), useAnchorLayout);
    }
    if (sides & SideArt::TopRight) {
        auto spr = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        spr->setFlipX(true);
        spr->setFlipY(true);
        to->addChildAtPosition(spr, Anchor::TopRight, ccp(-35, -35), useAnchorLayout);
    }
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
