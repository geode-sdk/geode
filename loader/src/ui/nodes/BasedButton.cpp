#include <Geode/ui/BasedButton.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

using namespace geode::prelude;

TabButton* TabButton::create(
    TabBaseColor unselected, TabBaseColor selected, char const* text, cocos2d::CCObject* target,
    cocos2d::SEL_MenuHandler callback
) {
    auto ret = new TabButton();
    auto sprOff = TabButtonSprite::create(text, unselected);
    auto sprOn = TabButtonSprite::create(text, selected);
    if (ret->init(sprOff, sprOn, target, callback)) {
        ret->m_offButton->m_colorDip = .3f;
        ret->m_offButton->m_colorEnabled = true;
        ret->m_offButton->m_scaleMultiplier = 1.f;
        ret->m_onButton->setEnabled(false);
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

TabButton* TabButton::create(char const* text, CCObject* target, SEL_MenuHandler callback) {
    return TabButton::create(
        TabBaseColor::Unselected, TabBaseColor::Selected, text, target, callback
    );
}
