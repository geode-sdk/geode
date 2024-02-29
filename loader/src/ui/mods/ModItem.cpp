#include "ModItem.hpp"
#include <Geode/ui/GeodeUI.hpp>

bool BaseModItem::init() {
    if (!CCNode::init())
        return false;
    
    auto meta = this->getMetadata();

    m_logo = this->createModLogo();
    this->addChild(m_logo);

    m_infoContainer = CCNode::create();
    m_infoContainer->setScale(.4f);
    m_infoContainer->setAnchorPoint({ .0f, .5f });
    m_infoContainer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Even)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(0)
    );
    m_infoContainer->getLayout()->ignoreInvisibleChildren(true);

    m_titleContainer = CCNode::create();
    m_titleContainer->setAnchorPoint({ .0f, .5f });
    m_titleContainer->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );

    auto title = CCLabelBMFont::create(meta.getName().c_str(), "bigFont.fnt");
    title->setAnchorPoint({ .0f, .5f });
    title->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setMinScale(.1f)
    );
    m_titleContainer->addChild(title);
    
    m_infoContainer->addChild(m_titleContainer);
    
    m_developers = CCMenu::create();
    m_developers->ignoreAnchorPointForPosition(false);
    m_developers->setAnchorPoint({ .0f, .5f });

    auto by = "By " + ModMetadata::formatDeveloperDisplayString(this->getMetadata().getDevelopers());
    auto developersBtn = CCMenuItemSpriteExtra::create(
        CCLabelBMFont::create(by.c_str(), "goldFont.fnt"),
        this, nullptr
    );
    m_developers->addChild(developersBtn);

    m_developers->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    m_infoContainer->addChild(m_developers);

    m_restartRequiredLabel = ButtonSprite::create("Restart Required", "bigFont.fnt", "black-square.png"_spr, .8f);
    m_restartRequiredLabel->m_label->setColor({ 55, 255, 155 });
    m_restartRequiredLabel->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setMaxScale(.75f)
    );
    m_infoContainer->addChild(m_restartRequiredLabel);

    this->addChild(m_infoContainer);

    m_viewMenu = CCMenu::create();
    m_viewMenu->setAnchorPoint({ 1.f, .5f });
    m_viewMenu->setScale(.55f);
    
    auto viewBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("View", "bigFont.fnt", "GE_button_05.png"_spr, .8f),
        this, nullptr
    );
    m_viewMenu->addChild(viewBtn);

    m_viewMenu->setLayout(
        RowLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setGap(10)
    );
    this->addChildAtPosition(m_viewMenu, Anchor::Right, ccp(-10, 0));

    return true;
}

void BaseModItem::updateState() {
    auto wantsRestart = this->wantsRestart();
    m_restartRequiredLabel->setVisible(wantsRestart);
    m_developers->setVisible(!wantsRestart);
    m_infoContainer->updateLayout();
}

void BaseModItem::updateSize(float width, bool big) {
    this->setContentSize({ width, big ? 40.f : 30.f });

    auto logoSize = m_obContentSize.height - 10;
    limitNodeSize(m_logo, { logoSize, logoSize }, 999, .1f);
    m_logo->setPosition(m_obContentSize.height / 2 + 5, m_obContentSize.height / 2);

    CCSize titleSpace {
        m_obContentSize.width / 2 - m_obContentSize.height,
        logoSize + 5
    };

    // Divide by scale of info container since that actually determines the size
    // (Since the scale of m_titleContainer and m_developers is managed by its layout)
    m_titleContainer->setContentWidth(titleSpace.width / m_infoContainer->getScale());
    m_titleContainer->updateLayout();

    m_developers->setContentWidth(titleSpace.width / m_infoContainer->getScale());
    m_developers->updateLayout();

    m_infoContainer->setPosition(m_obContentSize.height + 10, m_obContentSize.height / 2);
    m_infoContainer->setContentSize(ccp(titleSpace.width, titleSpace.height) / m_infoContainer->getScale());
    m_infoContainer->updateLayout();

    m_viewMenu->setContentWidth(m_obContentSize.width / 2 - 20);
    m_viewMenu->updateLayout();

    this->updateLayout();
}

bool InstalledModItem::init(Mod* mod) {
    m_mod = mod;

    if (!BaseModItem::init())
        return false;
    
    // Add an enable button if the mod is enablable
    if (!mod->isInternal()) {
        m_enableToggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(InstalledModItem::onEnable), 1.f
        );
        // Manually handle toggle state
        m_enableToggle->m_notClickable = true;
        m_viewMenu->addChild(m_enableToggle);
        m_viewMenu->updateLayout();
    }

    this->updateState();
    
    return true;
}

void InstalledModItem::updateState() {
    BaseModItem::updateState();

    // Update enable toggle state
    if (m_enableToggle) {
        m_enableToggle->toggle(m_mod->isOrWillBeEnabled());
    }
}

void InstalledModItem::onEnable(CCObject*) {
    // Toggle the mod state
    auto res = m_mod->isOrWillBeEnabled() ? m_mod->disable() : m_mod->enable();
    if (!res) {
        FLAlertLayer::create(
            "Error Toggling Mod",
            res.unwrapErr(),
            "OK"
        )->show();
    }

    // Update whole state of the mod item
    this->updateState();
}

InstalledModItem* InstalledModItem::create(Mod* mod) {
    auto ret = new InstalledModItem();
    if (ret && ret->init(mod)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ModMetadata InstalledModItem::getMetadata() const {
    return m_mod->getMetadata();
}

CCNode* InstalledModItem::createModLogo() const {
    return geode::createModLogo(m_mod);
}

bool InstalledModItem::wantsRestart() const {
    return m_mod->getRequestedAction() != ModRequestedAction::None;
}

bool ServerModItem::init(server::ServerModMetadata const& metadata) {
    m_metadata = metadata;

    if (!BaseModItem::init())
        return false;
    
    if (metadata.featured) {
        m_checkmark = CCScale9Sprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
        m_checkmark->setContentSize({ 50, 38 });
        m_checkmark->setColor({ 255, 255, 120 });
        m_checkmark->setOpacity(45);

        auto tick = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
        m_checkmark->addChildAtPosition(tick, Anchor::Center);

        m_titleContainer->addChild(m_checkmark);
    }

    this->updateState();
    
    return true;
}

ServerModItem* ServerModItem::create(server::ServerModMetadata const& metadata) {
    auto ret = new ServerModItem();
    if (ret && ret->init(metadata)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ModMetadata ServerModItem::getMetadata() const {
    return m_metadata.versions.front().metadata;
}

CCNode* ServerModItem::createModLogo() const {
    return createServerModLogo(m_metadata.id);
}

bool ServerModItem::wantsRestart() const {
    // todo: request restart after install
    return false;
}
