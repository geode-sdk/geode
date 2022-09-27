#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/string.hpp>

USE_GEODE_NAMESPACE();

bool MDPopup::setup(
    std::string const& title,
    std::string const& info,
    std::string const& btnText
) {
    this->setTitle(title.c_str(), "goldFont.fnt", .9f, 33.f);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto contentSize = CCSize {
        m_size.width - 70.f,
        m_size.height - 120.f,
    };
    auto content = MDTextArea::create(info, contentSize);
    content->setPosition(winSize / 2 - contentSize / 2);
    m_mainLayer->addChild(content);

    auto btnSpr = ButtonSprite::create(btnText.c_str());
    btnSpr->setScale(1.f);

    auto btn = CCMenuItemSpriteExtra::create(
        btnSpr, this, menu_selector(MDPopup::onClose)
    );
    btn->setPosition(.0f, -m_size.height / 2 + 35.f);
    m_buttonMenu->addChild(btn);

    return true;
}

float MDPopup::estimateHeight(std::string const& content) {
    return clamp(string::count(content, '\n') * 30.f, 200.f, 360.f);
}

MDPopup* MDPopup::create(
    std::string const& title,
    std::string const& content,
    std::string const& button
) {
    auto ret = new MDPopup();
    if (ret && ret->init(
        320.f, MDPopup::estimateHeight(content),
        title, content, button,
        "square01_001.png", { 0, 0, 94, 94 }
    )) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
