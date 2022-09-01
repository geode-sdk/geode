#include "SearchFilterPopup.hpp"
#include "ModListLayer.hpp"
#include "ModListView.hpp"

bool SearchFilterPopup::setup(ModListLayer* layer) {
    // todo: clean this shitty ass popup up
    m_noElasticity = true;
    m_modLayer = layer;

    this->setTitle("Match Fields");

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto pos = CCPoint { winSize.width / 2 - 55.f, winSize.height / 2 + 50.f };

    this->addToggle("Name",        ModListView::SearchFlags::Name,        pos);
    this->addToggle("ID",          ModListView::SearchFlags::ID,          pos);
    this->addToggle("Credits",     ModListView::SearchFlags::Credits,     pos);
    this->addToggle("Description", ModListView::SearchFlags::Description, pos);
    this->addToggle("Details",     ModListView::SearchFlags::Details,     pos);
    this->addToggle("Developer",   ModListView::SearchFlags::Developer,   pos);

    return true;
}

void SearchFilterPopup::addToggle(const char* title, int flag, CCPoint& pos) {
    GameToolbox::createToggleButton(
        title, menu_selector(SearchFilterPopup::onToggle),
        m_modLayer->m_searchFlags & flag,
        m_buttonMenu, pos, this,
        m_buttonMenu, .5f, .5f, 100.f, 
        { 10.f, .0f }, nullptr, false, flag, nullptr
    )->setTag(flag);
    pos.y -= 25.f;
}

void SearchFilterPopup::onToggle(cocos2d::CCObject* pSender) {
    if (as<CCMenuItemToggler*>(pSender)->isToggled()) {
        m_modLayer->m_searchFlags &= ~pSender->getTag();
    } else {
        m_modLayer->m_searchFlags |= pSender->getTag();
    }
}

SearchFilterPopup* SearchFilterPopup::create(ModListLayer* layer) {
    auto ret = new SearchFilterPopup();
    if (ret && ret->init(200.f, 200.f, layer, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
