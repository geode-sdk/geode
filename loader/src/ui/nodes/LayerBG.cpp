#include <Geode/ui/LayerBG.hpp>

USE_GEODE_NAMESPACE();

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
