#include "ModListView.hpp"

#include "../info/CategoryNode.hpp"
#include "../info/ModInfoLayer.hpp"
#include "ModListLayer.hpp"

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/StatsCell.hpp>
#include <Geode/binding/TableView.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/string.hpp>
#include <Index.hpp>
#include <InternalLoader.hpp>

template <class T>
static bool tryOrAlert(Result<T> const& res, char const* title) {
    if (!res) {
        FLAlertLayer::create(title, res.unwrapErr(), "OK")->show();
    }
    return res.isOk();
}

ModCell::ModCell(char const* name, CCSize size) : TableViewCell(name, size.width, size.height) {}

void ModCell::draw() {
    reinterpret_cast<StatsCell*>(this)->StatsCell::draw();
}

void ModCell::onFailedInfo(CCObject*) {
    FLAlertLayer::create(
        this, "Error Info",
        m_obj->m_info.m_reason.size() ? 
            m_obj->m_info.m_reason : 
            "Unable to load mod",
        "OK", "Remove file", 360.f
    )->show();
}

void ModCell::FLAlert_Clicked(FLAlertLayer*, bool btn2) {
    if (btn2) {
        try {
            if (ghc::filesystem::remove(m_obj->m_info.m_path)) {
                FLAlertLayer::create(
                    "File removed", "Removed <cy>" + m_obj->m_info.m_path.string() + "</c>", "OK"
                )->show();
            }
            else {
                FLAlertLayer::create(
                    "Unable to remove file",
                    "Unable to remove <cy>" + m_obj->m_info.m_path.string() + "</c>", "OK"
                )->show();
            }
        }
        catch (std::exception& e) {
            FLAlertLayer::create(
                "Unable to remove file",
                "Unable to remove <cy>" + m_obj->m_info.m_path.string() + "</c>: <cr>" +
                    std::string(e.what()) + "</c>",
                "OK"
            )->show();
        }
        (void)Loader::get()->refreshModsList();
        m_list->refreshList();
    }
}

void ModCell::setupUnloaded() {
    m_mainLayer->setVisible(true);

    auto menu = CCMenu::create();
    menu->setPosition(m_width - m_height, m_height / 2);
    m_mainLayer->addChild(menu);

    auto titleLabel = CCLabelBMFont::create("Failed to Load", "bigFont.fnt");
    titleLabel->setAnchorPoint({ .0f, .5f });
    titleLabel->setScale(.5f);
    titleLabel->setPosition(m_height / 2, m_height / 2 + 7.f);
    m_mainLayer->addChild(titleLabel);

    auto pathLabel = CCLabelBMFont::create(
        m_obj->m_info.m_path.string().c_str(),
        "chatFont.fnt"
    );
    pathLabel->setAnchorPoint({ .0f, .5f });
    pathLabel->setScale(.43f);
    pathLabel->setPosition(m_height / 2, m_height / 2 - 7.f);
    pathLabel->setColor({ 255, 255, 0 });
    m_mainLayer->addChild(pathLabel);

    auto whySpr = ButtonSprite::create("Info", 0, 0, "bigFont.fnt", "GJ_button_01.png", 0, .8f);
    whySpr->setScale(.65f);

    auto viewBtn =
        CCMenuItemSpriteExtra::create(whySpr, this, menu_selector(ModCell::onFailedInfo));
    menu->addChild(viewBtn);
}

void ModCell::setupLoadedButtons() {
    auto viewSpr = m_obj->m_mod->wasSuccesfullyLoaded()
        ? ButtonSprite::create("View", "bigFont.fnt", "GJ_button_01.png", .8f)
        : ButtonSprite::create("Why", "bigFont.fnt", "GJ_button_06.png", .8f);
    viewSpr->setScale(.65f);

    auto viewBtn = CCMenuItemSpriteExtra::create(
        viewSpr, this,
        m_obj->m_mod->wasSuccesfullyLoaded() ? menu_selector(ModCell::onInfo)
                                             : menu_selector(ModCell::onFailedInfo)
    );
    m_menu->addChild(viewBtn);

    if (m_obj->m_mod->wasSuccesfullyLoaded() && m_obj->m_mod->supportsDisabling()) {
        m_enableToggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(ModCell::onEnable), .7f
        );
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

    if (m_obj->m_mod->wasSuccesfullyLoaded()) {
        if (Index::get()->isUpdateAvailableForItem(m_obj->m_mod->getID())) {
            viewSpr->updateBGImage("GE_button_01.png"_spr);

            auto updateIcon = CCSprite::createWithSpriteFrameName("updates-available.png"_spr);
            updateIcon->setPosition(viewSpr->getContentSize() - CCSize { 2.f, 2.f });
            updateIcon->setZOrder(99);
            updateIcon->setScale(.5f);
            viewSpr->addChild(updateIcon);
        }
    }
}

void ModCell::setupIndexButtons() {
    auto viewSpr = ButtonSprite::create("View", "bigFont.fnt", "GJ_button_01.png", .8f);
    viewSpr->setScale(.65f);

    auto viewBtn = CCMenuItemSpriteExtra::create(viewSpr, this, menu_selector(ModCell::onInfo));
    m_menu->addChild(viewBtn);
}

void ModCell::loadFromObject(ModObject* modobj) {
    m_obj = modobj;

    if (modobj->m_type == ModObjectType::Unloaded) {
        return this->setupUnloaded();
    }

    m_mainLayer->setVisible(true);
    m_backgroundLayer->setOpacity(255);

    m_menu = CCMenu::create();
    m_menu->setPosition(m_width - 40.f, m_height / 2);
    m_mainLayer->addChild(m_menu);

    auto logoSize = m_height / 1.5f;

    auto logoSpr = ModInfoLayer::createLogoSpr(modobj, { logoSize, logoSize });
    logoSpr->setPosition({ logoSize / 2 + 12.f, m_height / 2 });
    m_mainLayer->addChild(logoSpr);

    bool hasCategories = false;

    ModInfo info;
    switch (modobj->m_type) {
        case ModObjectType::Mod: info = modobj->m_mod->getModInfo(); break;

        case ModObjectType::Index:
            info = modobj->m_index.m_info;
            hasCategories = m_expanded && modobj->m_index.m_categories.size();
            break;

        default: return;
    }

    bool hasDesc = m_expanded && info.m_description.has_value();

    auto titleLabel = CCLabelBMFont::create(info.m_name.c_str(), "bigFont.fnt");
    titleLabel->setAnchorPoint({ .0f, .5f });
    titleLabel->setPositionX(m_height / 2 + logoSize / 2 + 13.f);
    if (hasDesc && hasCategories) {
        titleLabel->setPositionY(m_height / 2 + 20.f);
    }
    else if (hasDesc || hasCategories) {
        titleLabel->setPositionY(m_height / 2 + 15.f);
    }
    else {
        titleLabel->setPositionY(m_height / 2 + 7.f);
    }
    titleLabel->limitLabelWidth(m_width / 2 - 40.f, .5f, .1f);
    m_mainLayer->addChild(titleLabel);

    auto versionLabel = CCLabelBMFont::create(info.m_version.toString().c_str(), "bigFont.fnt");
    versionLabel->setAnchorPoint({ .0f, .5f });
    versionLabel->setScale(.3f);
    versionLabel->setPosition(
        titleLabel->getPositionX() + titleLabel->getScaledContentSize().width + 5.f,
        titleLabel->getPositionY() - 1.f
    );
    versionLabel->setColor({ 0, 255, 0 });
    m_mainLayer->addChild(versionLabel);

    auto creatorStr = "by " + info.m_developer;
    auto creatorLabel = CCLabelBMFont::create(creatorStr.c_str(), "goldFont.fnt");
    creatorLabel->setAnchorPoint({ .0f, .5f });
    creatorLabel->setScale(.43f);
    creatorLabel->setPositionX(m_height / 2 + logoSize / 2 + 13.f);
    if (hasDesc && hasCategories) {
        creatorLabel->setPositionY(m_height / 2 + 7.5f);
    }
    else if (hasDesc || hasCategories) {
        creatorLabel->setPositionY(m_height / 2);
    }
    else {
        creatorLabel->setPositionY(m_height / 2 - 7.f);
    }
    m_mainLayer->addChild(creatorLabel);

    if (hasDesc) {
        auto descBG = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
        descBG->setColor({ 0, 0, 0 });
        descBG->setOpacity(90);
        descBG->setContentSize({ m_width * 2, 60.f });
        descBG->setAnchorPoint({ .0f, .5f });
        descBG->setPositionX(m_height / 2 + logoSize / 2 + 13.f);
        if (hasCategories) {
            descBG->setPositionY(m_height / 2 - 7.5f);
        }
        else {
            descBG->setPositionY(m_height / 2 - 17.f);
        }
        descBG->setScale(.25f);
        m_mainLayer->addChild(descBG);

        auto descText = CCLabelBMFont::create(info.m_description.value().c_str(), "chatFont.fnt");
        descText->setAnchorPoint({ .0f, .5f });
        descText->setPosition(m_height / 2 + logoSize / 2 + 18.f, descBG->getPositionY());
        descText->limitLabelWidth(m_width / 2 - 10.f, .5f, .1f);
        m_mainLayer->addChild(descText);
    }

    if (hasCategories) {
        float x = m_height / 2 + logoSize / 2 + 13.f;
        for (auto& category : modobj->m_index.m_categories) {
            auto node = CategoryNode::create(category);
            node->setAnchorPoint({ .0f, .5f });
            node->setPositionX(x);
            node->setScale(.3f);
            if (hasDesc) {
                node->setPositionY(m_height / 2 - 23.f);
            }
            else {
                node->setPositionY(m_height / 2 - 17.f);
            }
            m_mainLayer->addChild(node);

            x += node->getScaledContentSize().width + 5.f;
        }
    }

    switch (modobj->m_type) {
        case ModObjectType::Mod: this->setupLoadedButtons(); break;

        case ModObjectType::Index: this->setupIndexButtons(); break;

        default: break;
    }
    this->updateState();
}

void ModCell::onInfo(CCObject*) {
    ModInfoLayer::create(m_obj, m_list)->show();
}

void ModCell::updateBGColor(int index) {
    if (index & 1) m_backgroundLayer->setColor(ccc3(0xc2, 0x72, 0x3e));
    else m_backgroundLayer->setColor(ccc3(0xa1, 0x58, 0x2c));
    m_backgroundLayer->setOpacity(0xff);
}

void ModCell::onEnable(CCObject* pSender) {
    if (!InternalLoader::get()->shownInfoAlert("mod-disable-vs-unload")) {
        FLAlertLayer::create(
            "Notice",
            "<cb>Disabling</c> a <cy>mod</c> removes its hooks & patches and "
            "calls its user-defined disable function if one exists. You may "
            "still see some effects of the mod left however, and you may "
            "need to <cg>restart</c> the game to have it fully unloaded.",
            "OK"
        )->show();
        m_list->updateAllStates(this);
        return;
    }
    if (!as<CCMenuItemToggler*>(pSender)->isToggled()) {
        tryOrAlert(m_obj->m_mod->enable(), "Error enabling mod");
    }
    else {
        tryOrAlert(m_obj->m_mod->disable(), "Error disabling mod");
    }
    m_list->updateAllStates(this);
}

void ModCell::onUnresolvedInfo(CCObject* pSender) {
    std::string info =
        "This mod has the following "
        "<cr>unresolved dependencies</c>: ";
    for (auto const& dep : m_obj->m_mod->getUnresolvedDependencies()) {
        info += "<cg>" + dep.m_id +
            "</c> "
            "(<cy>" +
            dep.m_version.toString() + "</c>), ";
    }
    info.pop_back();
    info.pop_back();
    FLAlertLayer::create(nullptr, "Unresolved Dependencies", info, "OK", nullptr, 400.f)->show();
}

bool ModCell::init(ModListView* list, bool expanded) {
    m_list = list;
    m_expanded = expanded;
    return true;
}

void ModCell::updateState(bool invert) {
    if (m_obj->m_type == ModObjectType::Mod) {
        bool unresolved = m_obj->m_mod->hasUnresolvedDependencies();
        if (m_enableToggle) {
            m_enableToggle->toggle(m_obj->m_mod->isEnabled() ^ invert);
            m_enableToggle->setEnabled(!unresolved);
            m_enableToggle->m_offButton->setOpacity(unresolved ? 100 : 255);
            m_enableToggle->m_offButton->setColor(unresolved ? cc3x(155) : cc3x(255));
            m_enableToggle->m_onButton->setOpacity(unresolved ? 100 : 255);
            m_enableToggle->m_onButton->setColor(unresolved ? cc3x(155) : cc3x(255));
        }
        m_unresolvedExMark->setVisible(unresolved);
    }
}

ModCell* ModCell::create(ModListView* list, bool expanded, char const* key, CCSize size) {
    auto pRet = new ModCell(key, size);
    if (pRet && pRet->init(list, expanded)) {
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void ModListView::updateAllStates(ModCell* toggled) {
    for (auto cell : CCArrayExt<ModCell>(m_tableView->m_cellArray)) {
        cell->updateState(toggled == cell);
    }
}

void ModListView::setupList() {
    m_itemSeparation = m_expandedList ? 60.f : 40.0f;

    if (!m_entries->count()) return;

    m_tableView->reloadData();

    // fix content layer content size so the
    // list is properly aligned to the top
    auto coverage = calculateChildCoverage(m_tableView->m_contentLayer);
    m_tableView->m_contentLayer->setContentSize({ -coverage.origin.x + coverage.size.width,
                                                  -coverage.origin.y + coverage.size.height });

    if (m_entries->count() == 1) {
        m_tableView->moveToTopWithOffset(m_itemSeparation * 2);
    }
    else if (m_entries->count() == 2) {
        m_tableView->moveToTopWithOffset(-m_itemSeparation);
    }
    else {
        m_tableView->moveToTop();
    }
}

TableViewCell* ModListView::getListCell(char const* key) {
    return ModCell::create(this, m_expandedList, key, { m_width, m_itemSeparation });
}

void ModListView::loadCell(TableViewCell* cell, unsigned int index) {
    auto obj = as<ModObject*>(m_entries->objectAtIndex(index));
    as<ModCell*>(cell)->loadFromObject(obj);
    if (obj->m_type == ModObjectType::Mod) {
        if (obj->m_mod->wasSuccesfullyLoaded()) {
            as<ModCell*>(cell)->updateBGColor(index);
        }
        else {
            cell->m_backgroundLayer->setOpacity(255);
            cell->m_backgroundLayer->setColor({ 153, 0, 0 });
        }
        if (obj->m_mod->isUninstalled()) {
            cell->m_backgroundLayer->setColor({ 50, 50, 50 });
        }
    }
    else {
        as<ModCell*>(cell)->updateBGColor(index);
    }
}

bool ModListView::filter(ModInfo const& info, ModListQuery const& query) {
    // the UI for this functionality has been removed, however
    // the code has been kept in case we want to add it back at
    // some point.

    if (!query.m_searchFilter) return true;
    auto check = [query](SearchFlags flag, std::string const& name) -> bool {
        if (!(query.m_searchFlags & flag)) return false;
        return utils::string::contains(
            utils::string::toLower(name), utils::string::toLower(query.m_searchFilter.value())
        );
    };
    if (check(SearchFlag::Name, info.m_name)) return true;
    if (check(SearchFlag::ID, info.m_id)) return true;
    if (check(SearchFlag::Developer, info.m_developer)) return true;
    if (check(SearchFlag::Description, info.m_description.value_or(""))) return true;
    if (check(SearchFlag::Details, info.m_details.value_or(""))) return true;
    return false;
}

bool ModListView::filter(IndexItem const& item, ModListQuery const& query) {
    if (!query.m_showInstalled) {
        if (Loader::get()->isModInstalled(item.m_info.m_id)) {
            return false;
        }
    }
    if (query.m_categories.size()) {
        bool found = false;
        for (auto& cat : query.m_categories) {
            if (item.m_categories.count(cat)) {
                found = true;
            }
        }
        if (!found) {
            return false;
        }
    }
    for (auto& plat : query.m_platforms) {
        if (item.m_download.m_platforms.count(plat)) {
            return filter(item.m_info, query);
        }
    }
    return false;
}

static void sortInstalledMods(std::vector<Mod*>& mods) {
    if (!mods.size()) return;
    // keep track of first object
    size_t frontIndex = 0;
    auto front = mods.front();
    for (auto mod = mods.begin(); mod != mods.end(); mod++) {
        // move mods with updates to front
        if (Index::get()->isUpdateAvailableForItem((*mod)->getID())) {
            // swap first object and updatable mod
            // if the updatable mod is the first object,
            // nothing changes
            std::rotate(mods.begin(), mod, mod + 1);

            // get next object at front for next mod
            // to sort
            frontIndex++;
            front = mods[frontIndex];
        }
    }
}

static std::vector<Mod*> sortedInstalledMods() {
    auto mods = Loader::get()->getAllMods();
    sortInstalledMods(mods);
    return std::move(mods);
}

bool ModListView::init(
    CCArray* mods, ModListType type, bool expanded, float width, float height, ModListQuery query
) {
    m_expandedList = expanded;
    if (!mods) {
        switch (type) {
            case ModListType::Installed:
                {
                    mods = CCArray::create();
                    // failed mods first
                    for (auto const& mod : Loader::get()->getFailedMods()) {
                        mods->addObject(new ModObject(mod));
                    }
                    // internal geode representation always at the top
                    auto imod = Loader::getInternalMod();
                    if (this->filter(imod->getModInfo(), query)) {
                        mods->addObject(new ModObject(imod));
                    }
                    // then other mods
                    for (auto const& mod : sortedInstalledMods()) {
                        // if the mod is no longer installed nor
                        // loaded, it's as good as not existing
                        // (because it doesn't)
                        if (mod->isUninstalled() && !mod->isLoaded()) continue;
                        if (this->filter(mod->getModInfo(), query)) {
                            mods->addObject(new ModObject(mod));
                        }
                    }
                    if (!mods->count()) {
                        m_status = Status::SearchEmpty;
                    }
                }
                break;

            case ModListType::Download:
                {
                    mods = CCArray::create();
                    for (auto const& item : Index::get()->getItems()) {
                        if (this->filter(item, query)) {
                            mods->addObject(new ModObject(item));
                        }
                    }
                    if (!mods->count()) {
                        m_status = Status::NoModsFound;
                    }
                }
                break;

            case ModListType::Featured:
                {
                    mods = CCArray::create();
                    for (auto const& item : Index::get()->getFeaturedItems()) {
                        if (this->filter(item, query)) {
                            mods->addObject(new ModObject(item));
                        }
                    }
                    if (!mods->count()) {
                        m_status = Status::NoModsFound;
                    }
                }
                break;

            default: return false;
        }
    }
    return CustomListView::init(mods, BoomListType::Default, width, height);
}

ModListView* ModListView::create(
    CCArray* mods, ModListType type, bool expanded, float width, float height,
    ModListQuery const& query
) {
    auto pRet = new ModListView;
    if (pRet) {
        if (pRet->init(mods, type, expanded, width, height, query)) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

ModListView* ModListView::create(
    ModListType type, bool expanded, float width, float height, ModListQuery const& query
) {
    return ModListView::create(nullptr, type, expanded, width, height, query);
}

ModListView::Status ModListView::getStatus() const {
    return m_status;
}

std::string ModListView::getStatusAsString() const {
    switch (m_status) {
        case Status::OK: return "";
        case Status::Unknown: return "Unknown Issue";
        case Status::NoModsFound: return "No Mods Found";
        case Status::SearchEmpty: return "No Mods Match Search Query";
    }
    return "Unrecorded Status";
}

void ModListView::setLayer(ModListLayer* layer) {
    m_layer = layer;
}

void ModListView::refreshList() {
    if (m_layer) {
        m_layer->reloadList();
    }
}
