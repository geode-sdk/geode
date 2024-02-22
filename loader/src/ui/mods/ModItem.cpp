#include "ModItem.hpp"
#include <Geode/ui/GeodeUI.hpp>

bool BaseModItem::init() {
    if (!CCNode::init())
        return false;
    
    return true;
}

void BaseModItem::setupCommonInfo() {
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
}

void BaseModItem::updateSize(float width, bool big) {
    this->setContentSize({ width, big ? 40.f : 25.f });

    if (m_logo) {
        auto logoSize = m_obContentSize.height - 5;
        limitNodeSize(m_logo, { logoSize, logoSize }, 999, .1f);
        m_logo->setPosition(m_obContentSize.height / 2 + 5, m_obContentSize.height / 2);
    }
    CCSize titleSpace {
        m_obContentSize.width / 2 - m_obContentSize.height,
        m_obContentSize.height / 2
    };
    if (m_title) {
        m_title->setPosition(m_obContentSize.height + 10, m_obContentSize.height * .7f);
        limitNodeSize(m_title, titleSpace, 1.f, .1f);
    }
    if (m_developers) {
        m_developers->setPosition(m_obContentSize.height + 10, m_obContentSize.height * .3f);
        limitNodeSize(m_developers, titleSpace, .6f, .1f);
    }
}

bool InstalledModItem::init(Mod* mod) {
    if (!BaseModItem::init())
        return false;
    
    m_mod = mod;
    
    this->setupCommonInfo();
    
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
