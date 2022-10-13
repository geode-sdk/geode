#include <array>

USE_GEODE_NAMESPACE();

#include <Geode/modify/LoadingLayer.hpp>
class $modify(CustomLoadingLayer, LoadingLayer) {
    bool init(bool fromReload) {
        if (!LoadingLayer::init(fromReload))
            return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto count = Loader::get()->getAllMods().size();

        auto label = CCLabelBMFont::create(
            CCString::createWithFormat(
                "Geode: Loaded %lu mods",
                static_cast<unsigned long>(count)
            )->getCString(),
            "goldFont.fnt"
        );
        label->setPosition(winSize.width / 2, 30.f);
        label->setScale(.45f);
        label->setTag(5);
        this->addChild(label);

        return true;
    }
};
