#include <Geode/ui/LoadingSpinner.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

bool LoadingSpinner::init(float sideLength) {
    if (!CCNode::init())
        return false;

    this->setID("loading-spinner");
    this->setContentSize({ sideLength, sideLength });
    this->setAnchorPoint({ .5f, .5f });

    m_spinner = CCSprite::create("loadingCircle.png");
    m_spinner->setBlendFunc({ GL_ONE, GL_ONE });
    limitNodeSize(m_spinner, m_obContentSize, 1.f, .1f);
    this->addChildAtPosition(m_spinner, Anchor::Center);

    this->spin();

    return true;
}

void LoadingSpinner::spin() {
    m_spinner->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));
}

LoadingSpinner* LoadingSpinner::create(float sideLength) {
    auto ret = new LoadingSpinner();
    if (ret->init(sideLength)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void LoadingSpinner::setVisible(bool visible) {
    CCNode::setVisible(visible);
    if (visible) {
        this->spin();
    }
}
