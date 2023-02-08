#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/string.hpp>

USE_GEODE_NAMESPACE();

bool MDPopup::setup(
    std::string const& title, std::string const& info, char const* btn1Text, char const* btn2Text,
    utils::MiniFunction<void(bool)> onClick
) {
    this->setTitle(title.c_str(), "goldFont.fnt", .9f, 33.f);

    m_onClick = onClick;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto contentSize = CCSize {
        m_size.width - 70.f,
        m_size.height - 120.f,
    };
    auto content = MDTextArea::create(info, contentSize);
    content->setPosition(winSize / 2 - contentSize / 2);
    m_mainLayer->addChild(content);

    auto btnSpr = ButtonSprite::create(btn1Text);

    auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(MDPopup::onBtn));
    btn->setTag(0);
    m_buttonMenu->addChild(btn);

    if (btn2Text) {
        auto btn2Spr = ButtonSprite::create(btn2Text);

        auto btn2 = CCMenuItemSpriteExtra::create(btn2Spr, this, menu_selector(MDPopup::onBtn));
        btn2->setTag(1);
        m_buttonMenu->addChild(btn2);

        auto fullBtnWidth = btnSpr->getContentSize().width + 10.f + btn2Spr->getContentSize().width;

        btn->setPosition(
            -fullBtnWidth / 2 + btnSpr->getContentSize().width / 2, -m_size.height / 2 + 35.f
        );
        btn2->setPosition(
            fullBtnWidth / 2 - btn2Spr->getContentSize().width / 2, -m_size.height / 2 + 35.f
        );
    }
    // position button in the middle
    else {
        btn->setPosition(.0f, -m_size.height / 2 + 35.f);
    }

    return true;
}

void MDPopup::onBtn(CCObject* sender) {
    if (m_onClick) {
        m_onClick(sender->getTag());
    }
    this->onClose(nullptr);
}

float MDPopup::estimateHeight(std::string const& content) {
    return clamp(string::count(content, '\n') * 30.f, 200.f, 360.f);
}

MDPopup* MDPopup::create(
    std::string const& title, std::string const& content, char const* btn1, char const* btn2,
    utils::MiniFunction<void(bool)> onClick
) {
    auto ret = new MDPopup();
    if (ret &&
        ret->init(
            320.f, MDPopup::estimateHeight(content), title, content, btn1, btn2, onClick,
            "square01_001.png", { 0, 0, 94, 94 }
        )) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
