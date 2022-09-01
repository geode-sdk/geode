#include "SearchFilterPopup.hpp"
#include "ModListLayer.hpp"
#include "ModListView.hpp"

bool SearchFilterPopup::setup(ModListLayer* layer, ModListType type) {
    // todo: clean this shitty ass popup up
    m_noElasticity = true;
    m_modLayer = layer;

    this->setTitle("Search Filters");

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto pos = CCPoint { winSize.width / 2 - 145.f, winSize.height / 2 + 35.f };

    auto matchTitle = CCLabelBMFont::create("Search fields", "goldFont.fnt");
    matchTitle->setPosition(winSize.width / 2 - 90.f, winSize.height / 2 + 65.f);
    matchTitle->setScale(.5f);
    m_mainLayer->addChild(matchTitle);

	auto matchBG = CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    matchBG->setColor({ 0, 0, 0 });
    matchBG->setOpacity(90);
	matchBG->setContentSize({ 290.f, 300.f });
	matchBG->setPosition(winSize.width / 2 - 90.f, winSize.height / 2 - 21.f);
	matchBG->setScale(.5f);
	m_mainLayer->addChild(matchBG);

    this->addSearchMatch("Name",        ModListView::SearchFlags::Name,        pos);
    this->addSearchMatch("ID",          ModListView::SearchFlags::ID,          pos);
    this->addSearchMatch("Credits",     ModListView::SearchFlags::Credits,     pos);
    this->addSearchMatch("Description", ModListView::SearchFlags::Description, pos);
    this->addSearchMatch("Details",     ModListView::SearchFlags::Details,     pos);
    this->addSearchMatch("Developer",   ModListView::SearchFlags::Developer,   pos);

    
    auto line = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
    line->setPosition({ winSize.width / 2, winSize.height / 2 - 20.f });
    line->setScaleY(1.65f);
    line->setOpacity(100);
    m_mainLayer->addChild(line);

    return true;
}

void SearchFilterPopup::addSearchMatch(const char* title, int flag, CCPoint& pos) {
    GameToolbox::createToggleButton(
        title, menu_selector(SearchFilterPopup::onToggle),
        m_modLayer->m_searchFlags & flag,
        m_buttonMenu, pos, this,
        m_buttonMenu, .5f, .5f, 100.f, 
        { 10.f, .0f }, nullptr, false, flag, nullptr
    )->setTag(flag);
    pos.y -= 22.5f;
}

void SearchFilterPopup::onToggle(cocos2d::CCObject* pSender) {
    if (as<CCMenuItemToggler*>(pSender)->isToggled()) {
        m_modLayer->m_searchFlags &= ~pSender->getTag();
    } else {
        m_modLayer->m_searchFlags |= pSender->getTag();
    }
}

SearchFilterPopup* SearchFilterPopup::create(ModListLayer* layer, ModListType type) {
    auto ret = new SearchFilterPopup();
    if (ret && ret->init(350.f, 220.f, layer, type)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
