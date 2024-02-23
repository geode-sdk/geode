#include "ModItem.hpp"
#include <Geode/ui/GeodeUI.hpp>

bool BaseModItem::init() {
    if (!CCNode::init())
        return false;
    
    auto meta = this->getMetadata();

    m_logo = this->createModLogo();
    this->addChild(m_logo);

    m_title = CCLabelBMFont::create(meta.getName().c_str(), "bigFont.fnt");
    m_title->setAnchorPoint({ .0f, .5f });
    this->addChild(m_title);

    auto by = "By " + ModMetadata::formatDeveloperDisplayString(meta.getDevelopers());
    auto developersBtn = CCMenuItemSpriteExtra::create(
        CCLabelBMFont::create(by.c_str(), "goldFont.fnt"),
        this, nullptr
    );
    m_developers = CCMenu::create();
    m_developers->ignoreAnchorPointForPosition(false);
    m_developers->setContentSize(developersBtn->getScaledContentSize());
    m_developers->addChildAtPosition(developersBtn, Anchor::Center);
    m_developers->setAnchorPoint({ .0f, .5f });
    this->addChild(m_developers);

    m_viewMenu = CCMenu::create();
    m_viewMenu->setAnchorPoint({ 1.f, .5f });
    m_viewMenu->setLayout(
        RowLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
    );
    this->addChildAtPosition(m_viewMenu, Anchor::Right, ccp(-10, 0));

    this->updateState();

    return true;
}

void BaseModItem::updateState() {
    m_viewMenu->removeAllChildren();
    if (this->wantsRestart()) {
        auto restartSpr = ButtonSprite::create("Restart", "bigFont.fnt", "GE_button_02.png"_spr, .8f);
        restartSpr->setScale(.55f);
        auto restartBtn = CCMenuItemSpriteExtra::create(
            restartSpr, this, nullptr
        );
        m_viewMenu->addChild(restartBtn);
    }
    else {
        auto viewSpr = ButtonSprite::create("View", "bigFont.fnt", "GE_button_05.png"_spr, .8f);
        viewSpr->setScale(.55f);
        auto viewBtn = CCMenuItemSpriteExtra::create(
            viewSpr, this, nullptr
        );
        m_viewMenu->addChild(viewBtn);
    }
    m_viewMenu->updateLayout();
}

void BaseModItem::updateSize(float width, bool big) {
    this->setContentSize({ width, big ? 40.f : 25.f });

    auto logoSize = m_obContentSize.height - 5;
    limitNodeSize(m_logo, { logoSize, logoSize }, 999, .1f);
    m_logo->setPosition(m_obContentSize.height / 2 + 5, m_obContentSize.height / 2);

    CCSize titleSpace {
        m_obContentSize.width / 2 - m_obContentSize.height,
        m_obContentSize.height / 2
    };
    m_title->setPosition(m_obContentSize.height + 10, m_obContentSize.height * .7f);
    limitNodeSize(m_title, titleSpace, 1.f, .1f);
    m_developers->setPosition(m_obContentSize.height + 10, m_obContentSize.height * .3f);
    limitNodeSize(m_developers, titleSpace, .4f, .1f);

    m_viewMenu->setContentWidth(m_obContentSize.width / 2 - 20);
    m_viewMenu->updateLayout();

    this->updateLayout();
}

bool InstalledModItem::init(Mod* mod) {
    m_mod = mod;

    if (!BaseModItem::init())
        return false;
    
    return true;
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
