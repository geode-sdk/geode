#include "HotReloadLayer.hpp"

bool HotReloadLayer::init(std::string const& name) {
    if (!CCLayer::init()) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto bg = CCSprite::create("GJ_gradientBG.png");
    auto bgSize = bg->getTextureRect().size;

    bg->setAnchorPoint({ 0.0f, 0.0f });
    bg->setScaleX((winSize.width + 10.0f) / bgSize.width);
    bg->setScaleY((winSize.height + 10.0f) / bgSize.height);
    bg->setPosition({ -5.0f, -5.0f });
    bg->setColor({ 0, 102, 255 });

    this->addChild(bg);

    std::string text = "Reloading " + name + "...";
    auto label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
    label->setPosition(this->getContentSize() / 2);
    label->setZOrder(1);
    label->setScale(.5f);
    this->addChild(label);

    return true;
}

HotReloadLayer* HotReloadLayer::create(std::string const& name) {
    auto ret = new HotReloadLayer;
    if (ret && ret->init(name)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

HotReloadLayer* HotReloadLayer::scene(std::string const& name) {
    auto scene = CCScene::create();
    auto layer = HotReloadLayer::create(name);
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(scene);
    return layer;
}
