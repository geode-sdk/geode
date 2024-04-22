#include "SearchFilterPopup.hpp"

#include "../info/TagNode.hpp"
#include "ModListLayer.hpp"

#include <Geode/binding/GameToolbox.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>

// re-add when we actually add the platforms
const float iosAndAndroidSize = 25.f;

bool SearchFilterPopup::setup(ModListLayer* layer, ModListType type) {
    m_noElasticity = true;
    m_modLayer = layer;

    this->setTitle("Search Filters");

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // platforms

    auto platformTitle = CCLabelBMFont::create("Platforms", "goldFont.fnt");
    platformTitle->setAnchorPoint({ 0.5f, 1.f });
    platformTitle->setPosition(winSize.width / 2 - 85.f, winSize.height / 2 + 81.5f - iosAndAndroidSize * 0.25f);
    platformTitle->setScale(.5f);
    m_mainLayer->addChild(platformTitle);

    auto platformsContainerBG = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    platformsContainerBG->setColor({ 0, 0, 0 });
    platformsContainerBG->setOpacity(90);
    platformsContainerBG->setContentSize({ 290.f, 225.f - iosAndAndroidSize * 2.f });
    platformsContainerBG->setAnchorPoint({ 0.5f, 1.f });
    platformsContainerBG->setPosition(winSize.width / 2 - 85.f, winSize.height / 2 + 62.25f - iosAndAndroidSize * 0.25f);
    platformsContainerBG->setScale(.5f);
    m_mainLayer->addChild(platformsContainerBG);

    m_platformsContainer = CCNode::create();
    m_platformsContainer->setAnchorPoint({ 0.5f, 1.f });
    m_platformsContainer->setContentSize(platformsContainerBG->getScaledContentSize());
    m_platformsContainer->setPosition(platformsContainerBG->getPosition());
    m_platformsContainer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setCrossAxisOverflow(true)
            ->setGap(0)
    );
    m_mainLayer->addChild(m_platformsContainer);

    // TODO: add scrolllayer

    this->enable(this->addPlatformToggle("Windows", PlatformID::Windows), type);
    this->enable(this->addPlatformToggle("macOS", PlatformID::MacOS), type);
    this->enable(this->addPlatformToggle("iOS", PlatformID::iOS), type);
    this->enable(this->addPlatformToggle("Android (32-bit)", PlatformID::Android32), type);
    this->enable(this->addPlatformToggle("Android (64-bit)", PlatformID::Android64), type);

    m_platformsContainer->updateLayout();

    // show installed

    auto installedTitle = CCLabelBMFont::create("Other", "goldFont.fnt");
    installedTitle->setAnchorPoint({ 0.5f, 1.f });
    installedTitle->setPosition(winSize.width / 2 - 85.f, winSize.height / 2 - 52.5f + iosAndAndroidSize - iosAndAndroidSize * 0.25f);
    installedTitle->setScale(.5f);
    m_mainLayer->addChild(installedTitle);

    auto optionsContainerBG = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    optionsContainerBG->setColor({ 0, 0, 0 });
    optionsContainerBG->setOpacity(90);
    optionsContainerBG->setContentSize({ 290.f, 90.f });
    optionsContainerBG->setAnchorPoint({ 0.5f, 1.f });
    optionsContainerBG->setPosition(winSize.width / 2 - 85.f, winSize.height / 2 - 68.75f + iosAndAndroidSize - iosAndAndroidSize * 0.25f);
    optionsContainerBG->setScale(.5f);
    m_mainLayer->addChild(optionsContainerBG);

    m_optionsContainer = CCNode::create();
    m_optionsContainer->setAnchorPoint({ 0.5f, 1.f });
    m_optionsContainer->setContentSize(optionsContainerBG->getScaledContentSize());
    m_optionsContainer->setPosition(optionsContainerBG->getPosition());
    m_optionsContainer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setCrossAxisOverflow(true)
            ->setGap(0)
    );
    m_mainLayer->addChild(m_optionsContainer);

    this->addToggle(
        "Show Installed", menu_selector(SearchFilterPopup::onShowInstalled),
        m_modLayer->getQuery().forceVisibility, 0, m_optionsContainer
    );
    this->addToggle(
        "Show Invalid", menu_selector(SearchFilterPopup::onShowInvalid),
        m_modLayer->getQuery().forceInvalid, 1, m_optionsContainer
    );
    m_optionsContainer->updateLayout();

    // tags

    auto tagsTitle = CCLabelBMFont::create("Tags", "goldFont.fnt");
    tagsTitle->setAnchorPoint({ 0.5f, 1.f });
    tagsTitle->setPosition(winSize.width / 2 + 85.f, winSize.height / 2 + 81.5f - iosAndAndroidSize * 0.25f);
    tagsTitle->setScale(.5f);
    m_mainLayer->addChild(tagsTitle);

    auto tagsSize = CCSize { 290.f, 328.f - iosAndAndroidSize };
    auto tagsPos = CCPoint {winSize.width / 2 + 85.f, winSize.height / 2 + 62.5f - iosAndAndroidSize * 0.25f};

    auto tagsBG = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    tagsBG->setColor({ 0, 0, 0 });
    tagsBG->setOpacity(90);
    tagsBG->setContentSize(tagsSize);
    tagsBG->setAnchorPoint({ 0.5f, 1.f });
    tagsBG->setPosition(winSize.width / 2 + 85.f, winSize.height / 2 + 62.5f - iosAndAndroidSize * 0.25f);
    tagsBG->setScale(.5f);
    m_mainLayer->addChild(tagsBG);

    auto tagsWrap = CCNode::create();
    tagsWrap->setContentSize(tagsSize / 2.f);
    tagsWrap->setAnchorPoint({ 0.5f, 1.f });
    tagsWrap->setPosition(tagsPos);
    m_mainLayer->addChild(tagsWrap);

    m_tagLayer = ScrollLayer::create(tagsSize / 2.f);
    m_tagLayer->setTouchEnabled(true);
    m_tagLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setCrossAxisOverflow(true)
            ->setAutoGrowAxis(tagsSize.height / 2.f)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
    );
    tagsWrap->addChild(m_tagLayer);

    for (auto& tag : Index::get()->getTags()) {
        constexpr float HEIGHT = 20.f;
        
        auto menu = CCMenu::create();
        menu->setContentSize({ tagsSize.width / 2.f, HEIGHT });
        menu->ignoreAnchorPointForPosition(false);

        auto toggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(SearchFilterPopup::onTag), .5f
        );
        toggle->toggle(m_modLayer->getQuery().tags.count(tag));
        toggle->setPosition(12.f, HEIGHT / 2);
        toggle->setUserObject(CCString::create(tag));
        menu->addChild(toggle);

        auto label = TagNode::create(tag);
        label->setScale(.4f);
        label->setAnchorPoint({ .0f, .5f });
        label->setPosition(22.f, HEIGHT / 2);
        menu->addChild(label);

        m_tagLayer->m_contentLayer->addChild(menu);
    }
    m_tagLayer->m_contentLayer->updateLayout();

    return true;
}

void SearchFilterPopup::onTag(CCObject* sender) {
    auto toggle = static_cast<CCMenuItemToggler*>(sender);
    auto tag = static_cast<CCString*>(toggle->getUserObject())->getCString();
    if (!toggle->isToggled()) {
        m_modLayer->getQuery().tags.insert(tag);
    }
    else {
        if (m_modLayer->getQuery().tags.count(tag) > 0) {
            m_modLayer->getQuery().tags.erase(tag);
        }
    }
}

void SearchFilterPopup::onShowInstalled(CCObject* sender) {
    auto toggle = static_cast<CCMenuItemToggler*>(sender);
    m_modLayer->getQuery().forceVisibility = !toggle->isToggled();
}

void SearchFilterPopup::onShowInvalid(CCObject* sender) {
    auto toggle = static_cast<CCMenuItemToggler*>(sender);
    m_modLayer->getQuery().forceInvalid = !toggle->isToggled();
}

void SearchFilterPopup::enable(CCMenuItemToggler* toggle, ModListType type) {
    if (type == ModListType::Installed) {
        toggle->setEnabled(false);
        toggle->m_onButton->setColor(cc3x(0x50));
        toggle->m_offButton->setColor(cc3x(0x50));
    }
}

CCMenuItemToggler* SearchFilterPopup::addToggle(
    char const* title, SEL_MenuHandler selector,
    bool toggled, int tag, CCNode* node
) {
    constexpr float HEIGHT = 20.f;

    auto menu = CCMenu::create();
    menu->setContentSize({ node->getContentSize().width, HEIGHT });

    auto toggle = CCMenuItemToggler::createWithStandardSprites(this, selector, .5f);
    toggle->toggle(toggled);
    toggle->setPosition(12.f, HEIGHT / 2);
    toggle->setTag(tag);
    menu->addChild(toggle);

    auto label = CCLabelBMFont::create(title, "bigFont.fnt");
    label->limitLabelWidth(node->getContentSize().width - 30.f, .4f, .1f);
    label->setAnchorPoint({ .0f, .5f });
    label->setPosition(22.f, HEIGHT / 2);
    menu->addChild(label);

    node->addChild(menu);

    return toggle;
}

CCMenuItemToggler* SearchFilterPopup::addPlatformToggle(const char* title, PlatformID id) {
    return this->addToggle(
        title, menu_selector(SearchFilterPopup::onPlatformToggle),
        m_modLayer->getQuery().platforms.count(id),
        id.to<int>(), m_platformsContainer
    );
    return nullptr;
}

void SearchFilterPopup::onPlatformToggle(CCObject* sender) {
    if (static_cast<CCMenuItemToggler*>(sender)->isToggled()) {
        m_modLayer->getQuery().platforms.erase(PlatformID::from(sender->getTag()));
    }
    else {
        m_modLayer->getQuery().platforms.insert(PlatformID::from(sender->getTag()));
    }
}

void SearchFilterPopup::onClose(CCObject* sender) {
    Popup::onClose(sender);
    m_modLayer->reloadList(false);
}

SearchFilterPopup* SearchFilterPopup::create(ModListLayer* layer, ModListType type) {
    auto ret = new SearchFilterPopup();
    if (ret && ret->init(350.f, 240.f - iosAndAndroidSize * 0.5f, layer, type)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
