#include "SearchFilterPopup.hpp"
#include "ModListLayer.hpp"
#include "ModListView.hpp"
#include <Geode/ui/SelectList.hpp>
#include "../info/CategoryNode.hpp"
#include <Geode/binding/GameToolbox.hpp>

bool SearchFilterPopup::setup(ModListLayer* layer, ModListType type) {
    m_noElasticity = true;
    m_modLayer = layer;

    this->setTitle("Search Filters");

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto pos = CCPoint { winSize.width / 2 - 140.f, winSize.height / 2 + 45.f };

    // platforms

    auto platformTitle = CCLabelBMFont::create("Platforms", "goldFont.fnt");
    platformTitle->setPosition(winSize.width / 2 - 85.f, winSize.height / 2 + 75.f);
    platformTitle->setScale(.5f);
    m_mainLayer->addChild(platformTitle);

    auto platformBG = CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    platformBG->setColor({ 0, 0, 0 });
    platformBG->setOpacity(90);
    platformBG->setContentSize({ 290.f, 205.f });
    platformBG->setPosition(winSize.width / 2 - 85.f, winSize.height / 2 + 11.f);
    platformBG->setScale(.5f);
    m_mainLayer->addChild(platformBG);

    this->enable(this->addPlatformToggle("Windows", PlatformID::Windows, pos), type);
    this->enable(this->addPlatformToggle("MacOS",   PlatformID::MacOS,   pos), type);
    this->enable(this->addPlatformToggle("iOS",     PlatformID::iOS,     pos), type);
    this->enable(this->addPlatformToggle("Android", PlatformID::Android, pos), type);

    // show installed

    auto installedTitle = CCLabelBMFont::create("Other", "goldFont.fnt");
    installedTitle->setPosition(winSize.width / 2 - 85.f, winSize.height / 2 - 57.f);
    installedTitle->setScale(.5f);
    m_mainLayer->addChild(installedTitle);

	auto installedBG = CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    installedBG->setColor({ 0, 0, 0 });
    installedBG->setOpacity(90);
	installedBG->setContentSize({ 290.f, 65.f });
	installedBG->setPosition(winSize.width / 2 - 85.f, winSize.height / 2 - 85.f);
	installedBG->setScale(.5f);
	m_mainLayer->addChild(installedBG);

    pos = CCPoint { winSize.width / 2 - 140.f, winSize.height / 2 - 85.f };

    this->addToggle(
        "Show Installed",
        menu_selector(SearchFilterPopup::onShowInstalled),
        layer->m_query.m_showInstalled,
        0,
        pos
    );

    // categories

    auto categoriesTitle = CCLabelBMFont::create("Categories", "goldFont.fnt");
    categoriesTitle->setPosition(winSize.width / 2 + 85.f, winSize.height / 2 + 75.f);
    categoriesTitle->setScale(.5f);
    m_mainLayer->addChild(categoriesTitle);

    auto categoriesBG = CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    categoriesBG->setColor({ 0, 0, 0 });
    categoriesBG->setOpacity(90);
    categoriesBG->setContentSize({ 290.f, 328.f });
    categoriesBG->setPosition(winSize.width / 2 + 85.f, winSize.height / 2 - 19.5f);
    categoriesBG->setScale(.5f);
    m_mainLayer->addChild(categoriesBG);

    pos = CCPoint { winSize.width / 2 + 30.f, winSize.height / 2 + 45.f };

    for (auto& category : Index::get()->getCategories()) {
        auto toggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(SearchFilterPopup::onCategory), .5f
        );
        toggle->toggle(m_modLayer->m_query.m_categories.count(category));
        toggle->setPosition(pos - winSize / 2);
        toggle->setUserObject(CCString::create(category));
        m_buttonMenu->addChild(toggle);

        auto label = CategoryNode::create(category, CategoryNodeStyle::Dot);
        label->setScale(.4f);
        label->setAnchorPoint({ .0f, .5f });
        label->setPosition(pos.x + 10.f, pos.y);
        m_mainLayer->addChild(label);

        pos.y -= 22.5f;
    }

    return true;
}

void SearchFilterPopup::onCategory(CCObject* sender) {
    try {
        auto toggle = static_cast<CCMenuItemToggler*>(sender);
        auto category = static_cast<CCString*>(toggle->getUserObject())->getCString();
        if (!toggle->isToggled()) {
            m_modLayer->m_query.m_categories.insert(category);
        } else {
            m_modLayer->m_query.m_categories.erase(category);
        }
    } catch(...) {}
}

void SearchFilterPopup::onShowInstalled(CCObject* sender) {
    auto toggle = static_cast<CCMenuItemToggler*>(sender);
    m_modLayer->m_query.m_showInstalled = !toggle->isToggled();
}

void SearchFilterPopup::enable(CCMenuItemToggler* toggle, ModListType type) {
    if (type == ModListType::Installed) {
        toggle->setEnabled(false);
        toggle->m_onButton->setColor(cc3x(0x50));
        toggle->m_offButton->setColor(cc3x(0x50));
    }
}

CCMenuItemToggler* SearchFilterPopup::addToggle(
    const char* title, SEL_MenuHandler selector,
    bool toggled, int tag, CCPoint& pos
) {
    auto toggle = GameToolbox::createToggleButton(
        title, selector,
        toggled,
        m_buttonMenu, pos, this,
        m_buttonMenu, .5f, .5f, 100.f, 
        { 10.f, .0f }, nullptr, false, tag, nullptr
    );
    toggle->setTag(tag);
    pos.y -= 22.5f;
    return toggle;
}

CCMenuItemToggler* SearchFilterPopup::addSearchMatch(const char* title, int flag, CCPoint& pos) {
    return this->addToggle(
        title,
        menu_selector(SearchFilterPopup::onSearchToggle),
        m_modLayer->m_query.m_searchFlags & flag,
        flag,
        pos
    );
}

CCMenuItemToggler* SearchFilterPopup::addPlatformToggle(
    const char* title,
    PlatformID id,
    CCPoint& pos
) {
    return this->addToggle(
        title,
        menu_selector(SearchFilterPopup::onPlatformToggle),
        m_modLayer->m_query.m_platforms.count(id),
        id.to<int>(),
        pos
    );
}

void SearchFilterPopup::onSearchToggle(CCObject* sender) {
    if (static_cast<CCMenuItemToggler*>(sender)->isToggled()) {
        m_modLayer->m_query.m_searchFlags &= ~sender->getTag();
    } else {
        m_modLayer->m_query.m_searchFlags |= sender->getTag();
    }
}

void SearchFilterPopup::onPlatformToggle(CCObject* sender) {
    if (static_cast<CCMenuItemToggler*>(sender)->isToggled()) {
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
    if (ret && ret->init(350.f, 240.f, layer, type)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
