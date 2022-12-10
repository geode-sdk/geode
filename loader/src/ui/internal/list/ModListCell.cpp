#include "ModListCell.hpp"

#include "../info/ModInfoPopup.hpp"
#include "ModListView.hpp"

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/StatsCell.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <loader/LoaderImpl.hpp>

template <class T>
static bool tryOrAlert(Result<T> const& res, char const* title) {
    if (!res) {
        FLAlertLayer::create(title, res.unwrapErr(), "OK")->show();
    }
    return res.isOk();
}

ModListCell::ModListCell(char const* name, CCSize const& size) :
    TableViewCell(name, size.width, size.height) {}

void ModListCell::draw() {
    reinterpret_cast<StatsCell*>(this)->StatsCell::draw();
}

void ModListCell::setupInfo(ModInfo const& info, bool spaceForCategories) {
    m_mainLayer->setVisible(true);
    m_backgroundLayer->setOpacity(255);

    m_menu = CCMenu::create();
    m_menu->setPosition(m_width - 40.f, m_height / 2);
    m_mainLayer->addChild(m_menu);

    auto logoSize = m_height / 1.5f;

    auto logoSpr = this->createLogo({logoSize, logoSize});
    logoSpr->setPosition({logoSize / 2 + 12.f, m_height / 2});
    m_mainLayer->addChild(logoSpr);

    bool hasDesc = m_display == ModListDisplay::Expanded && info.m_description.has_value();

    auto titleLabel = CCLabelBMFont::create(info.m_name.c_str(), "bigFont.fnt");
    titleLabel->setAnchorPoint({.0f, .5f});
    titleLabel->setPositionX(m_height / 2 + logoSize / 2 + 13.f);
    if (hasDesc && spaceForCategories) {
        titleLabel->setPositionY(m_height / 2 + 20.f);
    }
    else if (hasDesc || spaceForCategories) {
        titleLabel->setPositionY(m_height / 2 + 15.f);
    }
    else {
        titleLabel->setPositionY(m_height / 2 + 7.f);
    }
    titleLabel->limitLabelWidth(m_width / 2 - 40.f, .5f, .1f);
    m_mainLayer->addChild(titleLabel);

    auto versionLabel = CCLabelBMFont::create(info.m_version.toString().c_str(), "bigFont.fnt");
    versionLabel->setAnchorPoint({.0f, .5f});
    versionLabel->setScale(.3f);
    versionLabel->setPosition(
        titleLabel->getPositionX() + titleLabel->getScaledContentSize().width + 5.f,
        titleLabel->getPositionY() - 1.f
    );
    versionLabel->setColor({0, 255, 0});
    m_mainLayer->addChild(versionLabel);

    auto creatorStr = "by " + info.m_developer;
    auto creatorLabel = CCLabelBMFont::create(creatorStr.c_str(), "goldFont.fnt");
    creatorLabel->setAnchorPoint({.0f, .5f});
    creatorLabel->setScale(.43f);
    creatorLabel->setPositionX(m_height / 2 + logoSize / 2 + 13.f);
    if (hasDesc && spaceForCategories) {
        creatorLabel->setPositionY(m_height / 2 + 7.5f);
    }
    else if (hasDesc || spaceForCategories) {
        creatorLabel->setPositionY(m_height / 2);
    }
    else {
        creatorLabel->setPositionY(m_height / 2 - 7.f);
    }
    m_mainLayer->addChild(creatorLabel);

    if (hasDesc) {
        auto descBG = CCScale9Sprite::create("square02b_001.png", {0.0f, 0.0f, 80.0f, 80.0f});
        descBG->setColor({0, 0, 0});
        descBG->setOpacity(90);
        descBG->setContentSize({m_width * 2, 60.f});
        descBG->setAnchorPoint({.0f, .5f});
        descBG->setPositionX(m_height / 2 + logoSize / 2 + 13.f);
        if (spaceForCategories) {
            descBG->setPositionY(m_height / 2 - 7.5f);
        }
        else {
            descBG->setPositionY(m_height / 2 - 17.f);
        }
        descBG->setScale(.25f);
        m_mainLayer->addChild(descBG);

        auto descText = CCLabelBMFont::create(info.m_description.value().c_str(), "chatFont.fnt");
        descText->setAnchorPoint({.0f, .5f});
        descText->setPosition(m_height / 2 + logoSize / 2 + 18.f, descBG->getPositionY());
        descText->limitLabelWidth(m_width / 2 - 10.f, .5f, .1f);
        m_mainLayer->addChild(descText);
    }
}

void ModListCell::updateBGColor(int index) {
    if (index % 2) {
        m_backgroundLayer->setColor(ccc3(0xc2, 0x72, 0x3e));
    }
    else m_backgroundLayer->setColor(ccc3(0xa1, 0x58, 0x2c));
    m_backgroundLayer->setOpacity(0xff);
}

bool ModListCell::init(ModListView* list, ModListDisplay display) {
    m_list = list;
    m_display = display;
    return true;
}

// ModCell

ModCell::ModCell(char const* name, CCSize const& size) : ModListCell(name, size) {}

ModCell* ModCell::create(ModListView* list, ModListDisplay display, char const* key, CCSize const& size) {
    auto ret = new ModCell(key, size);
    if (ret && ret->init(list, display)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void ModCell::onEnable(CCObject* sender) {
    if (!LoaderImpl::get()->shownInfoAlert("mod-disable-vs-unload")) {
        FLAlertLayer::create(
            "Notice",
            "<cb>Disabling</c> a <cy>mod</c> removes its hooks & patches and "
            "calls its user-defined disable function if one exists. You may "
            "still see some effects of the mod left however, and you may "
            "need to <cg>restart</c> the game to have it fully unloaded.",
            "OK"
        )
            ->show();
        m_list->updateAllStates(this);
        return;
    }
    if (!as<CCMenuItemToggler*>(sender)->isToggled()) {
        tryOrAlert(m_mod->enable(), "Error enabling mod");
    }
    else {
        tryOrAlert(m_mod->disable(), "Error disabling mod");
    }
    m_list->updateAllStates(this);
}

void ModCell::onUnresolvedInfo(CCObject*) {
    std::string info =
        "This mod has the following "
        "<cr>unresolved dependencies</c>: ";
    for (auto const& dep : m_mod->getUnresolvedDependencies()) {
        info += fmt::format("<cg>{}</c> (<cy>{}</c>), ", dep.m_id, dep.m_version.toString());
    }
    info.pop_back();
    info.pop_back();
    FLAlertLayer::create(nullptr, "Unresolved Dependencies", info, "OK", nullptr, 400.f)->show();
}

void ModCell::onInfo(CCObject*) {
    LocalModInfoPopup::create(m_mod, m_list)->show();
}

void ModCell::updateState() {
    bool unresolved = m_mod->hasUnresolvedDependencies();
    if (m_enableToggle) {
        m_enableToggle->toggle(m_mod->isEnabled());
        m_enableToggle->setEnabled(!unresolved);
        m_enableToggle->m_offButton->setOpacity(unresolved ? 100 : 255);
        m_enableToggle->m_offButton->setColor(unresolved ? cc3x(155) : cc3x(255));
        m_enableToggle->m_onButton->setOpacity(unresolved ? 100 : 255);
        m_enableToggle->m_onButton->setColor(unresolved ? cc3x(155) : cc3x(255));
    }
    m_unresolvedExMark->setVisible(unresolved);
}

void ModCell::loadFromMod(Mod* mod) {
    m_mod = mod;

    this->setupInfo(mod->getModInfo(), false);

    auto viewSpr = ButtonSprite::create("View", "bigFont.fnt", "GJ_button_01.png", .8f);
    viewSpr->setScale(.65f);

    auto viewBtn = CCMenuItemSpriteExtra::create(viewSpr, this, menu_selector(ModCell::onInfo));
    m_menu->addChild(viewBtn);

    if (m_mod->wasSuccesfullyLoaded() && m_mod->supportsDisabling()) {
        m_enableToggle =
            CCMenuItemToggler::createWithStandardSprites(this, menu_selector(ModCell::onEnable), .7f);
        m_enableToggle->setPosition(-45.f, 0.f);
        m_menu->addChild(m_enableToggle);
    }

    auto exMark = CCSprite::createWithSpriteFrameName("exMark_001.png");
    exMark->setScale(.5f);

    m_unresolvedExMark =
        CCMenuItemSpriteExtra::create(exMark, this, menu_selector(ModCell::onUnresolvedInfo));
    m_unresolvedExMark->setPosition(-80.f, 0.f);
    m_unresolvedExMark->setVisible(false);
    m_menu->addChild(m_unresolvedExMark);

    // if (m_mod->wasSuccesfullyLoaded()) {
    // if (Index::get()->isUpdateAvailableForItem(m_obj->m_mod->getID())) {
    //     viewSpr->updateBGImage("GE_button_01.png"_spr);

    //     auto updateIcon = CCSprite::createWithSpriteFrameName("updates-available.png"_spr);
    //     updateIcon->setPosition(viewSpr->getContentSize() - CCSize { 2.f, 2.f });
    //     updateIcon->setZOrder(99);
    //     updateIcon->setScale(.5f);
    //     viewSpr->addChild(updateIcon);
    // }
    // }

    this->updateState();
}

CCNode* ModCell::createLogo(CCSize const& size) {
    return geode::createModLogo(m_mod, size);
}

// IndexItemCell

IndexItemCell::IndexItemCell(char const* name, CCSize const& size) : ModListCell(name, size) {}

void IndexItemCell::onInfo(CCObject*) {
    IndexItemInfoPopup::create(m_item, m_list)->show();
}

IndexItemCell* IndexItemCell::create(
    ModListView* list, ModListDisplay display, char const* key, CCSize const& size
) {
    auto ret = new IndexItemCell(key, size);
    if (ret && ret->init(list, display)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void IndexItemCell::loadFromItem(IndexItemHandle item) {
    m_item = item;

    this->setupInfo(item->info, true);

    auto viewSpr = ButtonSprite::create("View", "bigFont.fnt", "GJ_button_01.png", .8f);
    viewSpr->setScale(.65f);

    auto viewBtn = CCMenuItemSpriteExtra::create(viewSpr, this, menu_selector(IndexItemCell::onInfo));
    m_menu->addChild(viewBtn);

    // if (hasCategories) {
    //     float x = m_height / 2 + logoSize / 2 + 13.f;
    //     for (auto& category : modobj->m_index.m_categories) {
    //         auto node = CategoryNode::create(category);
    //         node->setAnchorPoint({ .0f, .5f });
    //         node->setPositionX(x);
    //         node->setScale(.3f);
    //         if (hasDesc) {
    //             node->setPositionY(m_height / 2 - 23.f);
    //         }
    //         else {
    //             node->setPositionY(m_height / 2 - 17.f);
    //         }
    //         m_mainLayer->addChild(node);

    //         x += node->getScaledContentSize().width + 5.f;
    //     }
    // }

    this->updateState();
}

void IndexItemCell::updateState() {}

CCNode* IndexItemCell::createLogo(CCSize const& size) {
    return geode::createIndexItemLogo(m_item, size);
}

// InvalidGeodeFileCell

InvalidGeodeFileCell::InvalidGeodeFileCell(char const* name, CCSize const& size) :
    ModListCell(name, size) {}

void InvalidGeodeFileCell::onInfo(CCObject*) {
    FLAlertLayer::create(this, "Error Info", m_info.m_reason, "OK", "Remove file", 360.f)->show();
}

void InvalidGeodeFileCell::FLAlert_Clicked(FLAlertLayer*, bool btn2) {
    if (btn2) {
        try {
            if (ghc::filesystem::remove(m_info.m_path)) {
                FLAlertLayer::create(
                    "File removed", "Removed <cy>" + m_info.m_path.string() + "</c>", "OK"
                )
                    ->show();
            }
            else {
                FLAlertLayer::create(
                    "Unable to remove file",
                    "Unable to remove <cy>" + m_info.m_path.string() + "</c>",
                    "OK"
                )
                    ->show();
            }
        }
        catch (std::exception& e) {
            FLAlertLayer::create(
                "Unable to remove file",
                "Unable to remove <cy>" + m_info.m_path.string() + "</c>: <cr>" +
                    std::string(e.what()) + "</c>",
                "OK"
            )
                ->show();
        }
        (void)Loader::get()->refreshModsList();
        m_list->refreshList();
    }
}

InvalidGeodeFileCell* InvalidGeodeFileCell::create(
    ModListView* list, ModListDisplay display, char const* key, CCSize const& size
) {
    auto ret = new InvalidGeodeFileCell(key, size);
    if (ret && ret->init(list, display)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void InvalidGeodeFileCell::loadFromInfo(InvalidGeodeFile const& info) {
    m_info = info;

    m_mainLayer->setVisible(true);

    auto menu = CCMenu::create();
    menu->setPosition(m_width - m_height, m_height / 2);
    m_mainLayer->addChild(menu);

    auto titleLabel = CCLabelBMFont::create("Failed to Load", "bigFont.fnt");
    titleLabel->setAnchorPoint({.0f, .5f});
    titleLabel->setScale(.5f);
    titleLabel->setPosition(m_height / 2, m_height / 2 + 7.f);
    m_mainLayer->addChild(titleLabel);

    auto pathLabel = CCLabelBMFont::create(m_info.m_path.string().c_str(), "chatFont.fnt");
    pathLabel->setAnchorPoint({.0f, .5f});
    pathLabel->setScale(.43f);
    pathLabel->setPosition(m_height / 2, m_height / 2 - 7.f);
    pathLabel->setColor({255, 255, 0});
    m_mainLayer->addChild(pathLabel);

    auto whySpr = ButtonSprite::create("Info", 0, 0, "bigFont.fnt", "GJ_button_01.png", 0, .8f);
    whySpr->setScale(.65f);

    auto viewBtn =
        CCMenuItemSpriteExtra::create(whySpr, this, menu_selector(InvalidGeodeFileCell::onInfo));
    menu->addChild(viewBtn);
}

void InvalidGeodeFileCell::updateState() {}

CCNode* InvalidGeodeFileCell::createLogo(CCSize const& size) {
    return nullptr;
}
