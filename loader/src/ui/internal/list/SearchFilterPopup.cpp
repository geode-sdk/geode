#include "SearchFilterPopup.hpp"
#include "ModListLayer.hpp"
#include "ModListView.hpp"
#include <Geode/ui/SelectList.hpp>

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
	matchBG->setContentSize({ 290.f, 295.f });
	matchBG->setPosition(winSize.width / 2 - 90.f, winSize.height / 2 - 21.5f);
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
	platformBG->setContentSize({ 290.f, 205.f });
	platformBG->setPosition(winSize.width / 2 + 90.f, winSize.height / 2 + 1.f);
	platformBG->setScale(.5f);
	m_mainLayer->addChild(platformBG);

    pos = CCPoint { winSize.width / 2 + 35.f, winSize.height / 2 + 35.f };

    this->enable(this->addPlatformToggle("Windows", PlatformID::Windows, pos), type);
    this->enable(this->addPlatformToggle("MacOS",   PlatformID::MacOS,   pos), type);
    this->enable(this->addPlatformToggle("iOS",     PlatformID::iOS,     pos), type);
    this->enable(this->addPlatformToggle("Android", PlatformID::Android, pos), type);

	auto installedBG = CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    installedBG->setColor({ 0, 0, 0 });
    installedBG->setOpacity(90);
	installedBG->setContentSize({ 290.f, 65.f });
	installedBG->setPosition(winSize.width / 2 + 90.f, winSize.height / 2 - 80.f);
	installedBG->setScale(.5f);
	m_mainLayer->addChild(installedBG);

    return true;
}

void SearchFilterPopup::enable(CCMenuItemToggler* toggle, ModListType type) {
    if (type == ModListType::Installed) {
        toggle->setEnabled(false);
        toggle->m_onButton->setColor(cc3x(0x50));
        toggle->m_offButton->setColor(cc3x(0x50));
    }
}

CCMenuItemToggler* SearchFilterPopup::addSearchMatch(const char* title, int flag, CCPoint& pos) {
    auto toggle = GameToolbox::createToggleButton(
        title, menu_selector(SearchFilterPopup::onSearchToggle),
        m_modLayer->m_query.m_searchFlags & flag,
        m_buttonMenu, pos, this,
        m_buttonMenu, .5f, .5f, 100.f, 
        { 10.f, .0f }, nullptr, false, flag, nullptr
    );
    toggle->setTag(flag);
    pos.y -= 22.5f;
    return toggle;
}

CCMenuItemToggler* SearchFilterPopup::addPlatformToggle(
    const char* title,
    PlatformID id,
    CCPoint& pos
) {
    auto toggle = GameToolbox::createToggleButton(
        title, menu_selector(SearchFilterPopup::onPlatformToggle),
        m_modLayer->m_query.m_platforms.count(id),
        m_buttonMenu, pos, this,
        m_buttonMenu, .5f, .5f, 100.f, 
        { 10.f, .0f }, nullptr, false, id.to<int>(), nullptr
    );
    toggle->setTag(id.to<int>());
    pos.y -= 22.5f;
    return toggle;
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
