#include "SwelvyBG.hpp"
#include <Geode/loader/Mod.hpp>
#include <random>

bool SwelvyBG::init() {
    if (!CCNode::init())
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
    int idx = 0;
    for (auto layer : std::initializer_list<const char*> {
        "swelve-layer3.png"_spr,
        "swelve-layer0.png"_spr,
        "swelve-layer1.png"_spr,
        "swelve-layer2.png"_spr,
        "swelve-layer1.png"_spr,
        "swelve-layer0.png"_spr
    }) {
        float speed = dis(gen);
        if (sign(gen) == 0) {
            speed = -speed;
        }
        ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};

        auto sprite = CCSprite::create(layer);
        auto rect = sprite->getTextureRect();
        sprite->setUserObject("width", CCFloat::create(rect.size.width));
        rect.size = CCSize{winSize.width, rect.size.height};

        std::string layerID = fmt::format("layer-{}", idx);
        auto colorID = fmt::format("swelvy-bg-{}"_spr, idx);
        sprite->setID(layerID);
        sprite->getTexture()->setTexParameters(&params);
        sprite->setTextureRect(rect);
        sprite->setAnchorPoint({ 0, 1 });
        sprite->setContentSize({winSize.width, sprite->getContentSize().height});
        sprite->setColor(ColorProvider::get()->color3b(colorID));
        sprite->setPosition({0, y});
        sprite->schedule(schedule_selector(SwelvyBG::updateSpritePosition));
        sprite->setUserObject("speed", CCFloat::create(speed));
        this->addChild(sprite);

        m_colorListeners.emplace_back([=](ColorProvidedEvent* event) {
            sprite->setColor(to3B(event->color));
        }, ColorProvidedFilter(colorID));

        y -= m_obContentSize.height / 6;
        idx += 1;
    }
    return true;
}

void SwelvyBG::updateSpritePosition(float dt) {
    auto speed = static_cast<CCFloat*>(this->getUserObject("speed"))->getValue();
    auto width = static_cast<CCFloat*>(this->getUserObject("width"))->getValue();

    auto sprite = typeinfo_cast<CCSprite*>(this);
    auto rect = sprite->getTextureRect();

    float dX = rect.origin.x - speed * dt;
    if(dX >= std::abs(width)) {
        dX = 0;
    }

    rect.origin = CCPoint{dX, 0};
    sprite->setTextureRect(rect);
}

SwelvyBG* SwelvyBG::create() {
    auto ret = new SwelvyBG();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
