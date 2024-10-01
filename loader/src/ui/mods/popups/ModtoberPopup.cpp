#include "ModtoberPopup.hpp"
#include <Geode/utils/web.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/binding/ButtonSprite.hpp>

bool ModtoberPopup::setup() {
    m_bgSprite->setVisible(false);

    auto bg = CCSprite::createWithSpriteFrameName("modtober24-popup.png"_spr);
    m_mainLayer->addChildAtPosition(bg, Anchor::Center);

    auto supportSpr = createGeodeButton("Join");
    supportSpr->setScale(.8f);
    auto supportBtn = CCMenuItemSpriteExtra::create(
        supportSpr, this, menu_selector(ModtoberPopup::onDiscord)
    );
    m_buttonMenu->addChildAtPosition(supportBtn, Anchor::BottomRight, ccp(-65, 50));

    return true;
}

void ModtoberPopup::onDiscord(CCObject*) {
    createQuickPopup(
        "Join Modtober",
        "<cf>Modtober</c> is being hosted on the <cg>GD Programming</c> <ca>Discord Server</c>.\n"
        "To participate, join GDP and read the rules for the contest in <co>#modtober-2024</c>",
        "Cancel", "Join Discord",
        [](auto, bool btn2) {
            if (btn2) {
                web::openLinkInBrowser("https://discord.gg/gd-programming-646101505417674758");
            }
        }
    );
}

ModtoberPopup* ModtoberPopup::create() {
    auto ret = new ModtoberPopup();
    if (ret && ret->init(410, 270)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
