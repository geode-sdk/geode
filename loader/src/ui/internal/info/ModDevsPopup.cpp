#include "ModDevsPopup.hpp"
#include "DevProfilePopup.hpp"
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/loader/Index.hpp>
#include <Geode/ui/General.hpp>
#include "../list/ModListCell.hpp"
#include "../list/ModListLayer.hpp"

ModListLayer* m_layer;

bool ModDevsPopup::setup(ModMetadata const& mod, ModListLayer* list) {
    m_noElasticity = true;
    m_layer = list;

    this->setTitle("Developers for " + mod.getName());

    auto listSize = CCSize { 190.f, 150.f };
    auto devList = ScrollLayer::create(listSize);

    auto bg = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setScale(.5f);
    bg->setColor({ 0, 0, 0 });
    bg->setOpacity(115);
    bg->setContentSize(listSize * 2 + ccp(10, 10));
    m_mainLayer->addChildAtPosition(bg, Anchor::Center, ccp(0, -10));

    for (auto& dev : mod.getDevelopers()) {
        auto menu = CCMenu::create();
        
        auto label = CCLabelBMFont::create(dev.c_str(), "goldFont.fnt");
        label->setScale(.65f);
        auto btn = CCMenuItemSpriteExtra::create(label, this, menu_selector(ModDevsPopup::onViewDev));
        btn->setUserObject(CCString::create(dev));
        menu->addChild(btn);

        menu->setContentSize({ listSize.width, 25.f });
        menu->setLayout(RowLayout::create());
        devList->m_contentLayer->addChild(menu);
    }

    devList->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAutoGrowAxis(listSize.height)
            ->setAxisAlignment(AxisAlignment::End)
    );
    m_mainLayer->addChildAtPosition(devList, Anchor::Center, -listSize / 2 - ccp(0, 10));

    return true;
}

void ModDevsPopup::onViewDev(CCObject* sender) {
    auto str = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject());
    DevProfilePopup::create(str->getCString(), m_layer)->show();
}

ModDevsPopup* ModDevsPopup::create(ModMetadata const& mod, ModListLayer* list) {
    auto ret = new ModDevsPopup();
    if (ret && ret->initAnchored(280.f, 220.f, mod, list)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
