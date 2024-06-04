#include <Geode/binding/ButtonSprite.hpp>
#include "DevPopup.hpp"
#include "../UpdateModListState.hpp"

bool DevListPopup::setup(ModMetadata const& meta) {
    this->setTitle(fmt::format("Developers for {}", meta.getName()));

    auto container = CCNode::create();
    container->setAnchorPoint({ .5f, .5f });
    container->setContentHeight(150);

    for (auto dev : meta.getDevelopers()) {
        auto menu = CCMenu::create();
        menu->setContentWidth(m_size.width - 30);

        auto label = CCLabelBMFont::create(dev.c_str(), "bigFont.fnt");
        label->setLayoutOptions(AxisLayoutOptions::create()->setScalePriority(1));
        menu->addChild(label);

        auto plus = CCSprite::createWithSpriteFrameName("GJ_plus2Btn_001.png");
        plus->setScale(1.f);
        auto btn = CCMenuItemSpriteExtra::create(
            plus, this, menu_selector(DevListPopup::onMoreByThisDev)
        );
        btn->setUserObject(CCString::create(dev));
        menu->addChild(btn);

        menu->setLayout(
            RowLayout::create()
                ->setDefaultScaleLimits(.1f, .5f)
        );
        container->addChild(menu);
    }

    container->setLayout(
        ColumnLayout::create()
            ->setDefaultScaleLimits(.1f, 1.f)
    );
    m_mainLayer->addChildAtPosition(container, Anchor::Center, ccp(0, 0), ccp(.5f, .5f));

    auto okSpr = createGeodeButton("OK");
    okSpr->setScale(.7f);
    auto okBtn = CCMenuItemSpriteExtra::create(
        okSpr, this, menu_selector(DevListPopup::onClose)
    );
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, ccp(0, 20));

    return true;
}

void DevListPopup::onMoreByThisDev(CCObject* sender) {
    auto str = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject());
    UpdateModListStateEvent(UpdateWholeState(str->getCString())).post();
    this->onClose(nullptr);
}

DevListPopup* DevListPopup::create(ModMetadata const& meta) {
    auto ret = new DevListPopup();
    if (ret && ret->init(220, 220, meta)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
