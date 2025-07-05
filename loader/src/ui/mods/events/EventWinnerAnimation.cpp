#include "EventWinnerAnimation.hpp"
#include <Geode/binding/CCLightFlash.hpp>
#include <Geode/binding/GameToolbox.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/binding/CCCircleWave.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/cocos.hpp>

static float shakeyNoise(float x) {
    // i wrote some stuff in geogebra until i got a shakey looking function
    return sin(31.7f * x) + cos(17.2f * x) + sin(24.f * x) + sin(4.2f * x) + cos(18.7f * x) + sin(47.f * x);
}

class ShakeyWakey : public CCActionInterval {
protected:
    CCPoint m_startAnchor;
    float m_startRotation;

    bool initWithDuration(float duration) {
        if (!CCActionInterval::initWithDuration(duration))
            return false;

        return true;
    }

    CCActionInterval* reverse() override {
        return ShakeyWakey::create(m_fDuration);
    }
    void startWithTarget(CCNode* target) override {
        CCActionInterval::startWithTarget(target);
        m_startAnchor = target->getAnchorPoint();
        m_startRotation = target->getRotation();
    }

    void update(float time) override {
        if (m_pTarget) {
            time *= 15.f;
            m_pTarget->setAnchorPoint(m_startAnchor + ccp(shakeyNoise(time), shakeyNoise(time - 2)) / 45.f);
            m_pTarget->setRotation(m_startRotation + shakeyNoise(time + .2f));
        }
    }

public:
    static ShakeyWakey* create(float duration) {
        auto ret = new ShakeyWakey();
        if (ret && ret->initWithDuration(duration)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

bool EventWinnerAnimation::init() {
    if (!CCMenu::init())
        return false;

    // much taken from https://github.com/undefined06855/Icon-Ninja/blob/main/src/NinjaSwipeLayer.cpp
    // which is pretty messed up given it didn't win :(

    this->setContentSize(CCDirector::get()->getWinSize());
    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint(ccp(.5f, .5f));

    m_bg = CCLayerColor::create(ccc4(0, 0, 0, 0));
    m_bg->ignoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint(ccp(.5f, .5f));
    m_bg->runAction(CCFadeTo::create(1.f, 205));
    this->addChildAtPosition(m_bg, Anchor::Center);

    // Hopefully this loads before it is shows in the UI otherwise there will
    // be an awkward loading circle lol
    auto logo = createServerModLogo("rainixgd.geome3dash");
    logo->setVisible(false);
    logo->setZOrder(1);
    this->addChildAtPosition(logo, Anchor::Center);

    auto flash = CCLightFlash::create();
    auto size = cocos2d::CCDirector::sharedDirector()->getWinSize();
    float screenDiagonalSize = sqrtf(size.width * size.width + size.height * size.height) + 30.f;
    flash->playEffect(
        /* pos */ m_obContentSize / 2,
        /* col */ { 255, 255, 255 },
        /* bW */ 1.f,
        /* bwVar */ 0.f,
        /* tW */ 30.f,
        /* tWVar */ 25.f,
        /* tH */ screenDiagonalSize,
        /* dur */ .9f,
        /* durVar */ .24f,
        /* stripInterval */ .08f,
        /* stripDelay */ .3f,
        /* stripDelayVar */ .1f,
        /* rotation */ 0.f, // (doesnt matter)
        /* rotationVar */ 180.f,
        /* opacity */ 155.f,
        /* opacityVar */ 100.f,
        /* lightStripCount */ 14.f,
        /* circleRotation */ false,
        /* fadeIn */ false,
        /* delay */ 1.1f
    );
    this->addChild(flash);

    auto particles = GameToolbox::particleFromString(
        "30a-1a1.09a0.3a30a-1a180a29a0a0a0a0a0a0a0a0a0a2a1a0a27a1a0a1a0a1a0a1a0a1a1a0a27a1a0a1a0a1a0a1a0a0a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
        nullptr, false
    );
    particles->setScale(2.f);
    this->addChildAtPosition(particles, Anchor::Center);

    auto cloud = CCSprite::createWithSpriteFrameName("d_artCloud_03_001.png");
    cloud->setColor(ccBLACK);
    cloud->runAction(CCSequence::create(
        CCSpawn::createWithTwoActions(
            CCSpawn::createWithTwoActions(
                ShakeyWakey::create(2.f),
                CCScaleTo::create(2.f, 2.f)
            ),
            CCTintTo::create(2.f, 255, 255, 255)
        ),
        nullptr
    ));
    this->addChildAtPosition(cloud, Anchor::Center);

    this->runAction(CCSequence::create(
        CCDelayTime::create(2.f),
        CallFuncExt::create([=, this]() {
            flash->removeFromParent();
            cloud->removeFromParent();

            particles->setStartColor(to4F(ccc4(255, 156, 181, 255)));
            particles->setEndColor(to4F(ccc4(111, 160, 242, 0)));

            auto screenFlash = CCLayerColor::create(ccc4(255, 255, 255, 0));
            screenFlash->ignoreAnchorPointForPosition(false);
            screenFlash->setAnchorPoint(ccp(.5f, .5f));
            screenFlash->setZOrder(100);
            screenFlash->runAction(CCSequence::create(
                CCFadeTo::create(.1f, 255),
                CCFadeTo::create(1.f, 0),
                CCRemoveSelf::create(),
                nullptr
            ));
            this->addChildAtPosition(screenFlash, Anchor::Center);

            auto explosion = CCParticleSystemQuad::create("explodeEffect.plist", false);
            explosion->setPositionType(kCCPositionTypeGrouped);
            explosion->setAutoRemoveOnFinish(true);
            explosion->setStartColor(to4F(ccc4(255, 255, 255, 255)));
            explosion->resetSystem();
            this->addChildAtPosition(explosion, Anchor::Center);

            auto circleWave = CCCircleWave::create(10, 90, .5f, false);
            circleWave->m_color = ccc3(255, 255, 255);
            this->addChildAtPosition(circleWave, Anchor::Center);

            auto modtoberWinnerLabel = CCLabelBMFont::create("Modtober Winner", "goldFont.fnt");
            float offset = 0.f;
            for (auto ch : CCArrayExt<CCNode*>(modtoberWinnerLabel->getChildren())) {
                ch->setScale(0.f);
                ch->runAction(CCSequence::create(
                    CCDelayTime::create(offset),
                    CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f)),
                    nullptr
                ));
                offset += .03f;
            }
            this->addChildAtPosition(modtoberWinnerLabel, Anchor::Center, ccp(0, 75));

            auto modtoberWinnerSpr = CCSprite::createWithSpriteFrameName("tag-modtober-winner-long.png"_spr);
            modtoberWinnerSpr->setScale(0);
            modtoberWinnerSpr->runAction(CCEaseInOut::create(CCScaleTo::create(.5f, .4f), 2.f));
            this->addChildAtPosition(modtoberWinnerSpr, Anchor::Center, ccp(0, 45));

            auto winnerNameLabel = CCLabelBMFont::create("Geome3Dash", "bigFont.fnt");
            winnerNameLabel->setScale(.7f);
            offset = 0.f;
            for (auto ch : CCArrayExt<CCNode*>(winnerNameLabel->getChildren())) {
                ch->setScale(0.f);
                ch->runAction(CCSequence::create(
                    CCDelayTime::create(offset + .5f),
                    CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f)),
                    nullptr
                ));
                offset += .03f;
            }
            this->addChildAtPosition(winnerNameLabel, Anchor::Center, ccp(0, -50));

            auto winnerDevLabel = CCLabelBMFont::create("Rainix & Adaf", "goldFont.fnt");
            winnerDevLabel->setScale(.5f);
            offset = 0.f;
            for (auto ch : CCArrayExt<CCNode*>(winnerDevLabel->getChildren())) {
                ch->setScale(0.f);
                ch->runAction(CCSequence::create(
                    CCDelayTime::create(offset + 1.25f),
                    CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f)),
                    nullptr
                ));
                offset += .03f;
            }
            this->addChildAtPosition(winnerDevLabel, Anchor::Center, ccp(0, -75));

            logo->setVisible(true);
            logo->setScale(1.5f);
            logo->runAction(CCSequence::create(
                CCEaseElasticInOut::create(CCScaleTo::create(1.f, 1.f)),
                CCRepeat::create(
                    CCSequence::create(
                        CCEaseInOut::create(CCScaleTo::create(2.f, 1.2f), 2.f),
                        CCEaseInOut::create(CCScaleTo::create(2.f, 1.f), 2.f),
                        nullptr
                    ),
                    1000
                ),
                nullptr
            ));

            auto okSpr = ButtonSprite::create("OK", "goldFont.fnt", "GJ_button_01.png", .8f);
            auto okBtn = CCMenuItemSpriteExtra::create(okSpr, this, menu_selector(EventWinnerAnimation::onClose));
            okBtn->setScale(0.f);
            okBtn->runAction(CCSequence::create(
                CCDelayTime::create(2.f),
                CCEaseInOut::create(CCScaleTo::create(.5f, 1.f), 2.f),
                nullptr
            ));
            this->addChildAtPosition(okBtn, Anchor::Center, ccp(0, -110));
        }),
        CCDelayTime::create(10.f),
        CallFuncExt::create([this]() {
            this->onClose(nullptr);
        }),
        nullptr
    ));

    FMODAudioEngine::get()->playEffect("endStart_02.ogg");

    return true;
}

void EventWinnerAnimation::onClose(CCObject*) {
    this->stopAllActions();
    m_bg->runAction(CCFadeTo::create(1.f, 0));
    float offset = 0.f;
    for (auto child : CCArrayExt<CCNode*>(this->getChildren())) {
        if (child == m_bg) continue;
        child->stopAllActions();
        child->runAction(CCSequence::create(
            CCDelayTime::create(offset),
            CCEaseIn::create(CCScaleTo::create(.5f, 0.f), 2.f),
            nullptr
        ));
        offset += .02f;
    }
    this->runAction(CCSequence::create(
        CCDelayTime::create(1.f),
        CCRemoveSelf::create(),
        nullptr
    ));
}

EventWinnerAnimation* EventWinnerAnimation::create() {
    auto ret = new EventWinnerAnimation();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
