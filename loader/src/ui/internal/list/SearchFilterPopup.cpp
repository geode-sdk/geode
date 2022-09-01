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

    auto matchTitle = CCLabelBMFont::create("Match fields", "goldFont.fnt");
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

    this->addSearchMatch("Name",        SearchFlag::Name,        pos);
    this->addSearchMatch("ID",          SearchFlag::ID,          pos);
    this->addSearchMatch("Credits",     SearchFlag::Credits,     pos);
    this->addSearchMatch("Description", SearchFlag::Description, pos);
    this->addSearchMatch("Details",     SearchFlag::Details,     pos);
    this->addSearchMatch("Developer",   SearchFlag::Developer,   pos);
    
    auto line = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");
    line->setPosition({ winSize.width / 2, winSize.height / 2 - 20.f });
    line->setScaleY(1.65f);
    line->setOpacity(100);
    m_mainLayer->addChild(line);

    auto platformTitle = CCLabelBMFont::create("Platforms", "goldFont.fnt");
    platformTitle->setPosition(winSize.width / 2 + 90.f, winSize.height / 2 + 65.f);
    platformTitle->setScale(.5f);
    m_mainLayer->addChild(platformTitle);

	auto platformBG = CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    platformBG->setColor({ 0, 0, 0 });
    platformBG->setOpacity(90);
	platformBG->setContentSize({ 290.f, 300.f });
	platformBG->setPosition(winSize.width / 2 + 90.f, winSize.height / 2 - 21.f);
	platformBG->setScale(.5f);
	m_mainLayer->addChild(platformBG);

    pos = CCPoint { winSize.width / 2 + 45.f, winSize.height / 2 + 35.f };

    this->addPlatformToggle("Windows", PlatformID::Windows, pos);
    this->addPlatformToggle("MacOS",   PlatformID::MacOS,   pos);
    this->addPlatformToggle("iOS",     PlatformID::iOS,     pos);
    this->addPlatformToggle("Android", PlatformID::Android, pos);

    return true;
}

void SearchFilterPopup::addSearchMatch(const char* title, int flag, CCPoint& pos) {
    GameToolbox::createToggleButton(
        title, menu_selector(SearchFilterPopup::onSearchToggle),
        m_modLayer->m_query.m_searchFlags & flag,
        m_buttonMenu, pos, this,
        m_buttonMenu, .5f, .5f, 100.f, 
        { 10.f, .0f }, nullptr, false, flag, nullptr
    )->setTag(flag);
    pos.y -= 22.5f;
}

void SearchFilterPopup::addPlatformToggle(
    const char* title,
    PlatformID id,
    CCPoint& pos
) {
    GameToolbox::createToggleButton(
        title, menu_selector(SearchFilterPopup::onPlatformToggle),
        m_modLayer->m_query.m_platforms.count(id),
        m_buttonMenu, pos, this,
        m_buttonMenu, .5f, .5f, 100.f, 
        { 10.f, .0f }, nullptr, false, id.to<int>(), nullptr
    )->setTag(id.to<int>());
    pos.y -= 22.5f;
}

void SearchFilterPopup::onSearchToggle(CCObject* sender) {
    if (as<CCMenuItemToggler*>(sender)->isToggled()) {
        m_modLayer->m_query.m_searchFlags &= ~sender->getTag();
    } else {
        m_modLayer->m_query.m_searchFlags |= sender->getTag();
    }
}

void SearchFilterPopup::onPlatformToggle(CCObject* sender) {
    if (as<CCMenuItemToggler*>(sender)->isToggled()) {
        m_modLayer->m_query.m_platforms.erase(
            PlatformID::from(sender->getTag())
        );
    } else {
        m_modLayer->m_query.m_platforms.insert(
            PlatformID::from(sender->getTag())
        );
    }
}

void SearchFilterPopup::onClose(CCObject* sender) {
    Popup::onClose(sender);
    m_modLayer->reloadList();
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
