#include "SwelvyBG.hpp"
#include <random>

bool SwelvyBG::init() {
    if (!CCSpriteBatchNode::initWithTexture(CCTextureCache::get()->textureForKey("SwelveSheet.png"_spr), 20))
        return false;

    this->setID("SwelvyBG");
    
    auto winSize = CCDirector::get()->getWinSize();
    this->setContentSize(winSize);
    this->setAnchorPoint({ 0.f, 0.f });

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> sign(0, 1);
    std::uniform_real_distribution<float> dis(3.f, 9.f);

    float y = m_obContentSize.height + 5;
    for (auto layer : std::initializer_list<std::pair<ccColor3B, const char*>> {
        { ccc3(244, 212, 142), "layer3.png"_spr },
        { ccc3(245, 174, 125), "layer0.png"_spr },
        { ccc3(236, 137, 124), "layer1.png"_spr },
        { ccc3(213, 105, 133), "layer2.png"_spr },
        { ccc3(173, 84,  146), "layer1.png"_spr },
        { ccc3(113, 74,  154), "layer0.png"_spr },
    }) {
        float speed = dis(gen);
        if (sign(gen) == 0) {
            speed = -speed;
        }
        auto frame = CCSpriteFrameCache::get()->spriteFrameByName(layer.second);
        auto repeatCount = static_cast<int>(floor(winSize.width / frame->getRect().size.width)) + 2;
        for (int i = 0; i < repeatCount; i += 1) {
            auto sprite = CCSprite::createWithSpriteFrame(frame);
            sprite->setColor(layer.first);
            sprite->setAnchorPoint({ (speed < 0 ? 0.f : 1.f), 1 });
            sprite->setPosition({ (i + 1) * (sprite->getContentWidth() - 1), y });
            sprite->schedule(schedule_selector(SwelvyBG::updateSpritePosition));
            sprite->setUserObject("speed", CCFloat::create(speed));
            sprite->setUserObject("repeat-count", CCInteger::create(repeatCount));
            this->addChild(sprite);
        }
        y -= m_obContentSize.height / 6;
    }

    return true;
}

void SwelvyBG::updateSpritePosition(float dt) {
    auto speed = static_cast<CCFloat*>(this->getUserObject("speed"))->getValue();
    auto repeatCount = static_cast<CCInteger*>(this->getUserObject("repeat-count"))->getValue();
    this->setPositionX(this->getPositionX() - speed * dt);
    if (speed > 0 && this->getPositionX() < 0.f) {
        this->setPositionX(this->getPositionX() + (this->getContentWidth() - 1) * repeatCount);
    }
    else if (speed < 0 && this->getPositionX() > this->getParent()->getContentWidth()) {
        this->setPositionX(this->getPositionX() - (this->getContentWidth() - 1) * repeatCount);
    }
}

SwelvyBG* SwelvyBG::create() {
    auto ret = new SwelvyBG();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
