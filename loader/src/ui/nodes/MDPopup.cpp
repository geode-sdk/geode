#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/string.hpp>

using namespace geode::prelude;

bool MDPopup::setup(
    std::string const& title, std::string const& info, char const* btn1Text, char const* btn2Text,
    utils::MiniFunction<void(bool)> onClick
) {
    this->setTitle(title.c_str(), "goldFont.fnt", .9f, 33.f);

    m_onClick = onClick;

    auto contentSize = CCSize {
        m_size.width - 70.f,
        m_size.height - 120.f,
    };
    auto content = MDTextArea::create(info, contentSize);
    m_mainLayer->addChildAtPosition(content, Anchor::Center, ccp(0, 0));

    auto btnSpr = ButtonSprite::create(btn1Text);

    auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(MDPopup::onBtn));
    btn->setTag(0);

    auto menu = CCMenu::create();
    menu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Center)
            ->setGap(10.f)
    );
    menu->addChild(btn);

    if (btn2Text) {
        auto btn2Spr = ButtonSprite::create(btn2Text);

        auto btn2 = CCMenuItemSpriteExtra::create(btn2Spr, this, menu_selector(MDPopup::onBtn));
        btn2->setTag(1);

        menu->addChild(btn2);
    }

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

float MDPopup::estimateHeight(std::string const& content) {
    return clamp(string::count(content, '\n') * 30.f, 200.f, 280.f);
}

MDPopup* MDPopup::create(
    std::string const& title, std::string const& content, char const* btn1, char const* btn2,
    utils::MiniFunction<void(bool)> onClick
) {
    auto ret = new MDPopup();
    if (ret->initAnchored(
            320.f, MDPopup::estimateHeight(content), title, content, btn1, btn2, onClick,
            "square01_001.png", { 0, 0, 94, 94 }
        )) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
