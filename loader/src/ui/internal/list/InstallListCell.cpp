#include "InstallListCell.hpp"
#include "InstallListPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/StatsCell.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <loader/LoaderImpl.hpp>
#include <utility>
#include "../info/DevProfilePopup.hpp"

// InstallListCell

void InstallListCell::draw() {
    reinterpret_cast<StatsCell*>(this)->StatsCell::draw();
}

float InstallListCell::getLogoSize() const {
    return m_height / 1.5f;
}

void InstallListCell::setupInfo(
    std::string name,
    std::optional<std::string> developer,
    std::variant<VersionInfo, ComparableVersionInfo> version,
    bool inactive
) {
    m_inactive = inactive;
    m_menu = CCMenu::create();
    m_menu->setPosition(0, 0);
    m_menu->setAnchorPoint({ .0f, .0f });
    m_menu->setContentSize({m_width, m_height});
    this->addChild(m_menu);

    auto logoSize = this->getLogoSize();

    auto logoSpr = this->createLogo({ logoSize, logoSize });
    logoSpr->setPosition({ logoSize / 2 + 12.f, m_height / 2 });
    auto logoSprColor = typeinfo_cast<CCRGBAProtocol*>(logoSpr);
    if (inactive && logoSprColor) {
        logoSprColor->setColor({ 163, 163, 163 });
    }
    this->addChild(logoSpr);

    m_titleLabel = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    m_titleLabel->setAnchorPoint({ .0f, .5f });
    m_titleLabel->setPositionX(m_height / 2 + logoSize / 2 + 13.f);
    m_titleLabel->setPositionY(m_height / 2);
    m_titleLabel->limitLabelWidth(m_width / 2 - 70.f, .4f, .1f);
    if (inactive) {
        m_titleLabel->setColor({ 163, 163, 163 });
    }
    this->addChild(m_titleLabel);

    m_creatorLabel = nullptr;
    if (developer) {
        auto creatorStr = "by " + *developer;
        m_creatorLabel = CCLabelBMFont::create(creatorStr.c_str(), "goldFont.fnt");
        m_creatorLabel->setScale(.34f);
        if (inactive) {
            m_creatorLabel->setColor({ 163, 163, 163 });
        }

        m_creatorLabel->setPosition(
            m_titleLabel->getPositionX() + m_titleLabel->getScaledContentSize().width + 3.f +
                m_creatorLabel->getScaledContentSize().width / 2,
            m_height / 2
        );
        m_menu->addChild(m_creatorLabel);
    }

    this->setupVersion(version);
}

void InstallListCell::setupVersion(std::variant<VersionInfo, ComparableVersionInfo> version) {
    if (m_versionLabel) {
        m_versionLabel->removeFromParent();
        m_versionLabel = nullptr;
    }
    if (m_tagLabel) {
        m_tagLabel->removeFromParent();
        m_tagLabel = nullptr;
    }

    m_versionLabel = CCLabelBMFont::create(
        std::holds_alternative<VersionInfo>(version) ?
            std::get<VersionInfo>(version).toString(false).c_str() :
            std::get<ComparableVersionInfo>(version).toString().c_str(),
        "bigFont.fnt"
    );
    m_versionLabel->setAnchorPoint({ .0f, .5f });
    m_versionLabel->setScale(.2f);
    m_versionLabel->setPosition(
        m_titleLabel->getPositionX() + m_titleLabel->getScaledContentSize().width + 3.f +
            (m_creatorLabel ? m_creatorLabel->getScaledContentSize().width + 3.f : 0.f),
        m_titleLabel->getPositionY() - 1.f
    );
    m_versionLabel->setColor({ 0, 255, 0 });
    if (m_inactive) {
        m_versionLabel->setColor({ 0, 163, 0 });
    }
    this->addChild(m_versionLabel);

    if (!std::holds_alternative<VersionInfo>(version)) return;
    if (auto tag = std::get<VersionInfo>(version).getTag()) {
        m_tagLabel = TagNode::create(tag->toString());
        m_tagLabel->setAnchorPoint({.0f, .5f});
        m_tagLabel->setScale(.2f);
        m_tagLabel->setPosition(
            m_versionLabel->getPositionX() + m_versionLabel->getScaledContentSize().width + 3.f,
            m_versionLabel->getPositionY()
        );
        this->addChild(m_tagLabel);
    }
}

void InstallListCell::setupInfo(ModMetadata const& metadata, bool inactive) {
    this->setupInfo(metadata.getName(), metadata.getDeveloper(), metadata.getVersion(), inactive);
}

void InstallListCell::onViewDev(CCObject*) {
    // DevProfilePopup::create(getDeveloper(), m_layer)->show();
}

bool InstallListCell::init(InstallListPopup* list, CCSize const& size) {
    m_width = size.width;
    m_height = size.height;
    m_layer = list;
    this->setContentSize(size);
    this->setID("install-list-cell");
    return true;
}

bool InstallListCell::isIncluded() {
    return m_toggle && m_toggle->isOn();
}

// ModInstallListCell

bool ModInstallListCell::init(Mod* mod, InstallListPopup* list, CCSize const& size) {
    if (!InstallListCell::init(list, size))
        return false;
    m_mod = mod;
    this->setupInfo(mod->getMetadata(), true);
    auto message = CCLabelBMFont::create("Installed", "bigFont.fnt");
    message->setAnchorPoint({ 1.f, .5f });
    message->setPositionX(m_width - 10.0f);
    message->setPositionY(16.f);
    message->setScale(0.4f);
    message->setColor({ 163, 163, 163 });
    this->addChild(message);
    return true;
}

ModInstallListCell* ModInstallListCell::create(Mod* mod, InstallListPopup* list, CCSize const& size) {
    auto ret = new ModInstallListCell();
    if (ret->init(mod, list, size)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCNode* ModInstallListCell::createLogo(CCSize const& size) {
    return geode::createModLogo(m_mod, size);
}
std::string ModInstallListCell::getID() const {
    return m_mod->getID();
}
std::string ModInstallListCell::getDeveloper() const {
    return m_mod->getDeveloper();
}

// IndexItemInstallListCell

bool IndexItemInstallListCell::init(
    IndexItemHandle item,
    ModMetadata::Dependency::Importance importance,
    InstallListPopup* list,
    CCSize const& size,
    std::optional<bool> selected
) {
    if (!InstallListCell::init(list, size))
        return false;
    m_item = item;
    this->setupInfo(item->getMetadata(), item->isInstalled());

    // TODO: show installed label properly
    // if (item->isInstalled()) {
    //     auto message = CCLabelBMFont::create("Installed", "bigFont.fnt");
    //     message->setAnchorPoint({ 1.f, .5f });
    //     message->setPositionX(m_width - 10.0f);
    //     message->setPositionY(16.f);
    //     message->setScale(0.4f);
    //     message->setColor({ 163, 163, 163 });
    //     this->addChild(message);
    //     return true;
    // }

    m_toggle = CCMenuItemToggler::createWithStandardSprites(
        m_layer,
        menu_selector(InstallListPopup::onCellToggle),
        .6f
    );
    m_toggle->setAnchorPoint({1.f, .5f});
    m_toggle->setPosition(m_width - 5, m_height / 2);

    // recycling sprites in my Geode?? noo never
    auto versionSelectSpr = EditorButtonSprite::createWithSpriteFrameName(
        "filters.png"_spr, 1.0f, EditorBaseColor::Gray
    );
    versionSelectSpr->setScale(.7f);

    auto versionSelectBtn =
        CCMenuItemSpriteExtra::create(versionSelectSpr, this, menu_selector(IndexItemInstallListCell::onSelectVersion));
    versionSelectBtn->setAnchorPoint({1.f, .5f});
    versionSelectBtn->setPosition({m_toggle->getPositionX() - m_toggle->getContentSize().width - 5, m_height / 2});
    m_menu->addChild(versionSelectBtn);

    switch (importance) {
        case ModMetadata::Dependency::Importance::Required:
            m_toggle->setClickable(false);
            m_toggle->toggle(true);
            break;
        case ModMetadata::Dependency::Importance::Recommended:
            m_toggle->setClickable(true);
            m_toggle->toggle(true);
            break;
        case ModMetadata::Dependency::Importance::Suggested:
            m_toggle->setClickable(true);
            m_toggle->toggle(false);
            break;
    }

    if (item->isInstalled()) {
        m_toggle->setClickable(false);
        m_toggle->toggle(true);
    }

    if (m_item->getAvailablePlatforms().count(GEODE_PLATFORM_TARGET) == 0) {
        m_toggle->setClickable(false);
        m_toggle->toggle(false);

        auto message = CCLabelBMFont::create("N/A", "bigFont.fnt");
        message->setAnchorPoint({ 1.f, .5f });
        message->setPositionX(m_width - 5.f);
        message->setPositionY(16.f);
        message->setScale(0.4f);
        message->setColor({ 240, 31, 31 });
        this->addChild(message);

        if (importance != ModMetadata::Dependency::Importance::Required) {
            message->setCString("N/A (Optional)");
            message->setColor({ 163, 24, 24 });
        }
    }

    if (m_toggle->m_notClickable) {
        m_toggle->m_offButton->setOpacity(100);
        m_toggle->m_offButton->setColor(cc3x(155));
        m_toggle->m_onButton->setOpacity(100);
        m_toggle->m_onButton->setColor(cc3x(155));
    }

    if (!m_toggle->m_notClickable && selected) {
        m_toggle->toggle(*selected);
    }

    m_menu->addChild(m_toggle);
    return true;
}

IndexItemInstallListCell* IndexItemInstallListCell::create(
    IndexItemHandle item,
    ModMetadata::Dependency::Importance importance,
    InstallListPopup* list,
    CCSize const& size,
    std::optional<bool> selected
) {
    auto ret = new IndexItemInstallListCell();
    if (ret->init(std::move(item), importance, list, size, selected)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCNode* IndexItemInstallListCell::createLogo(CCSize const& size) {
    return geode::createIndexItemLogo(m_item, size);
}
std::string IndexItemInstallListCell::getID() const {
    return m_item->getMetadata().getID();
}
std::string IndexItemInstallListCell::getDeveloper() const {
    return m_item->getMetadata().getDeveloper();
}

IndexItemHandle IndexItemInstallListCell::getItem() {
    return m_item;
}

void IndexItemInstallListCell::setVersionFromItem(IndexItemHandle item) {
    this->setupVersion(item->getMetadata().getVersion());
    m_item = item;
}

void IndexItemInstallListCell::onSelectVersion(CCObject*) {
    SelectVersionPopup::create(m_item->getMetadata().getID(), this)->show();
}

// UnknownInstallListCell

bool UnknownInstallListCell::init(
    ModMetadata::Dependency const& dependency,
    InstallListPopup* list,
    CCSize const& size
) {
    if (!InstallListCell::init(list, size))
        return false;
    m_dependency = dependency;
    bool optional = dependency.importance != ModMetadata::Dependency::Importance::Required;
    this->setupInfo(dependency.id, std::nullopt, dependency.version, optional);
    auto message = CCLabelBMFont::create("Missing", "bigFont.fnt");
    message->setAnchorPoint({ 1.f, .5f });
    message->setPositionX(m_menu->getPositionX());
    message->setPositionY(16.f);
    message->setScale(0.4f);
    message->setColor({ 240, 31, 31 });
    if (optional) {
        message->setCString("Missing (Optional)");
        message->setColor({ 163, 24, 24 });
    }
    this->addChild(message);
    return true;
}

UnknownInstallListCell* UnknownInstallListCell::create(
    ModMetadata::Dependency const& dependency,
    InstallListPopup* list,
    CCSize const& size
) {
    auto ret = new UnknownInstallListCell();
    if (ret->init(dependency, list, size)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCNode* UnknownInstallListCell::createLogo(CCSize const& size) {
    return geode::createDefaultLogo(size);
}
std::string UnknownInstallListCell::getID() const {
    return m_dependency.id;
}
std::string UnknownInstallListCell::getDeveloper() const {
    return "";
}

// SelectVersionCell

bool SelectVersionCell::init(IndexItemHandle item, SelectVersionPopup* versionPopup, CCSize const& size) {
    if (!InstallListCell::init(nullptr, size))
        return false;
    m_item = item;
    m_versionPopup = versionPopup;
    this->setupInfo(item->getMetadata(), item->isInstalled());

    auto selectSpr = ButtonSprite::create(
        "Select", 0, 0, "bigFont.fnt", "GJ_button_01.png", 0, .6f
    );
    selectSpr->setScale(.6f);

    auto selectBtn = CCMenuItemSpriteExtra::create(
        selectSpr, this, menu_selector(SelectVersionCell::onSelected)
    );
    selectBtn->setAnchorPoint({1.f, .5f});
    selectBtn->setPosition({m_width - 5, m_height / 2});
    m_menu->addChild(selectBtn);

    return true;
}

void SelectVersionCell::onSelected(CCObject*) {
    m_versionPopup->selectItem(m_item);
}

SelectVersionCell* SelectVersionCell::create(IndexItemHandle item, SelectVersionPopup* versionPopup, CCSize const& size) {
    auto ret = new SelectVersionCell();
    if (ret->init(item, versionPopup, size)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCNode* SelectVersionCell::createLogo(CCSize const& size) {
    return geode::createIndexItemLogo(m_item, size);
}
std::string SelectVersionCell::getID() const {
    return m_item->getMetadata().getID();
}
std::string SelectVersionCell::getDeveloper() const {
    return m_item->getMetadata().getDeveloper();
}