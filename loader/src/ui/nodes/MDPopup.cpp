#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

bool MDPopup::init(
    bool compatibilityMode, ZStringView title, std::string info, ZStringView btn1Text,
    ZStringView btn2Text, geode::Function<void(bool)> onClick
) {
    if (!Popup::init(
        400.f,
        MDPopup::estimateHeight(info),
        "square01_001.png",
        { 0, 0, 94, 94 }
    )) {
        return false;
    }

    m_onClick = std::move(onClick);

    auto contentSize = CCSize {
        m_size.width - 50.f,
        m_size.height - 100.f,
    };

    auto content = MDTextArea::create(info, contentSize, compatibilityMode);
    m_mainLayer->addChildAtPosition(content, Anchor::Center, ccp(0, 0));

    this->setTitle(title, "goldFont.fnt", .9f, 28.f);
    m_title->limitLabelWidth(contentSize.width - 4.f, .9f, .1f);

    auto btnSpr = ButtonSprite::create(btn1Text.c_str());

    auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(MDPopup::onBtn));
    btn->setTag(0);

    auto menu = CCMenu::create();
    menu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Center)
            ->setGap(10.f)
    );
    menu->addChild(btn);

    if (!btn2Text.empty()) {
        auto btn2Spr = ButtonSprite::create(btn2Text.c_str());

        auto btn2 = CCMenuItemSpriteExtra::create(btn2Spr, this, menu_selector(MDPopup::onBtn));
        btn2->setTag(1);

        menu->addChild(btn2);
    }

    m_closeBtn->setVisible(false);

    m_buttonMenu->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 30));
    menu->updateLayout();

    return true;
}

void MDPopup::onBtn(CCObject* sender) {
    if (m_onClick) {
        m_onClick(sender->getTag());
    }
    this->onClose(nullptr);
}

float MDPopup::estimateHeight(std::string_view content) {
    return std::clamp(string::count(content, '\n') * 50.f, 180.f, 280.f);
}

MDPopup* MDPopup::create(
    ZStringView title, std::string content, ZStringView btn1, ZStringView btn2,
    geode::Function<void(bool)> onClick
) {
    return MDPopup::create(false, title, std::move(content), btn1, btn2, std::move(onClick));
}

MDPopup* MDPopup::create(
    bool compatibilityMode, ZStringView title, std::string content, ZStringView btn1,
    ZStringView btn2, geode::Function<void(bool)> onClick
) {
    auto ret = new MDPopup();

    if (ret->init(compatibilityMode, title, std::move(content), btn1, btn2, std::move(onClick))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
