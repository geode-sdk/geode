#include "InstallListPopup.hpp"
#include "InstallListCell.hpp"

#include <utility>
#include <queue>

bool InstallListPopup::setup(IndexItemHandle item, MiniFunction<void(IndexInstallList const&)> callback) {
    m_noElasticity = true;

    m_item = item;
    m_callback = callback;

    this->setTitle("Select Mods to Install");

    this->createList();

    auto installBtnSpr = IconButtonSprite::create(
        "GE_button_01.png"_spr,
        CCSprite::createWithSpriteFrameName("install.png"_spr),
        "Install",
        "bigFont.fnt"
    );
    installBtnSpr->setScale(.6f);

    auto installBtn = CCMenuItemSpriteExtra::create(
        installBtnSpr,
        this,
        menu_selector(InstallListPopup::onInstall)
    );
    installBtn->setPositionY(-m_bgSprite->getScaledContentSize().height / 2 + 22.f);
    m_buttonMenu->addChild(installBtn);

    return true;
}

void InstallListPopup::createList() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    std::unordered_map<std::string, InstallListCell*> oldCells;
    bool oldScrollAtBottom;
    std::optional<float> oldScroll;
    if (m_list) {
        CCArray* oldEntries = m_list->m_entries;
        for (size_t i = 0; i < oldEntries->count(); i++) {
            auto* itemCell = typeinfo_cast<InstallListCell*>(oldEntries->objectAtIndex(i));
            oldCells[itemCell->getID()] = itemCell;
        }
        auto content = m_list->m_tableView->m_contentLayer;
        oldScroll = content->getPositionY();
        oldScrollAtBottom = oldScroll >= 0.f;
        if (!oldScrollAtBottom)
            *oldScroll += content->getScaledContentSize().height;
        m_list->removeFromParent();
    }
    if (m_listParent) {
        m_listParent->removeFromParent();
    }

    m_listParent = CCNode::create();
    m_mainLayer->addChild(m_listParent);

    auto items = this->createCells(oldCells);
    m_list = ListView::create(
        items,
        this->getCellSize().height,
        this->getListSize().width,
        this->getListSize().height
    );
    m_list->setPosition(winSize / 2 - m_list->getScaledContentSize() / 2);
    m_listParent->addChild(m_list);

    // restore scroll on list recreation
    // it's stored from the top unless was scrolled all the way to the bottom
    if (oldScroll) {
        auto content = m_list->m_tableView->m_contentLayer;
        if (oldScrollAtBottom)
            content->setPositionY(*oldScroll);
        else
            content->setPositionY(*oldScroll - content->getScaledContentSize().height);
    }

    addListBorders(m_listParent, winSize / 2, m_list->getScaledContentSize());
}

CCArray* InstallListPopup::createCells(std::unordered_map<std::string, InstallListCell*> const& oldCells) {
    std::vector<InstallListCell*> top;
    std::vector<InstallListCell*> middle;
    std::vector<InstallListCell*> bottom;

    std::queue<ModMetadata::Dependency> queue;
    std::unordered_set<std::string> queued;

    auto id = m_item->getMetadata().getID();
    middle.push_back(IndexItemInstallListCell::create(
        m_item,
        ModMetadata::Dependency::Importance::Required,
        this,
        this->getCellSize(),
        oldCells.contains(id) ? std::make_optional(oldCells.at(id)->isIncluded()) : std::nullopt
    ));
    for (auto const& dep : m_item->getMetadata().getDependencies()) {
        queue.push(dep);
    }

    auto index = Index::get();
    while (!queue.empty()) {
        auto const& item = queue.front();
        if (queued.contains(item.id)) {
            queue.pop();
            continue;
        }
        queued.insert(item.id);

        // installed
        // TODO: we should be able to select a different version even if its installed
        if (item.mod && /*!item.mod->isUninstalled()*/item.mod->getMetadata().getID() == "geode.loader") {
            bottom.push_back(ModInstallListCell::create(item.mod, this, this->getCellSize()));
            for (auto const& dep : item.mod->getMetadata().getDependencies()) {
                queue.push(dep);
            }
            queue.pop();
            continue;
        }

        // on index
        if (auto depItem = index->getItem(item.id, item.version)) {
            auto cell = IndexItemInstallListCell::create(
                depItem,
                item.importance,
                this,
                this->getCellSize(),
                oldCells.contains(item.id) ?
                    std::make_optional(oldCells.at(item.id)->isIncluded()) :
                    std::nullopt
            );

            // put missing dependencies at the top
            if (depItem->getAvailablePlatforms().count(GEODE_PLATFORM_TARGET) == 0)
                top.push_back(cell);
            // put installed dependencies at the bottom
            else if (depItem->isInstalled())
                bottom.push_back(cell);
            else
                middle.push_back(cell);

            if (!cell->isIncluded()) {
                queue.pop();
                continue;
            }

            for (auto const& dep : depItem->getMetadata().getDependencies()) {
                queue.push(dep);
            }
            queue.pop();
            continue;
        }

        // unknown (aka not installed and missing from index)
        auto unknownCell = UnknownInstallListCell::create(item, this, this->getCellSize());
        top.push_back(unknownCell);
        queue.pop();
    }

    auto mods = CCArray::create();
    for (auto const& item : top) {
        mods->addObject(item);
    }
    for (auto const& item : middle) {
        mods->addObject(item);
    }
    for (auto const& item : bottom) {
        mods->addObject(item);
    }

    return mods;
}

// Getters

CCSize InstallListPopup::getListSize() const {
    return { 340.f, 170.f };
}

CCSize InstallListPopup::getCellSize() const {
    return { getListSize().width, 30.f };
}

// Callbacks

void InstallListPopup::onCellToggle(cocos2d::CCObject* obj) {
    auto* toggler = typeinfo_cast<CCMenuItemToggler*>(obj);
    if (toggler && !toggler->m_notClickable)
        toggler->toggle(!toggler->isOn());
    this->createList();
}

void InstallListPopup::onInstall(cocos2d::CCObject* obj) {
    this->onBtn2(obj);
    if (!m_callback)
        return;

    IndexInstallList list;
    list.target = m_item;

    CCArray* entries = m_list->m_entries;
    for (size_t i = entries->count(); i > 0; i--) {
        auto* itemCell = typeinfo_cast<IndexItemInstallListCell*>(entries->objectAtIndex(i - 1));
        if (!itemCell || !itemCell->isIncluded() || itemCell->getItem()->isInstalled())
            continue;
        IndexItemHandle item = itemCell->getItem();
        list.list.push_back(item);
    }

    m_callback(list);
}

// Static

InstallListPopup* InstallListPopup::create(
    IndexItemHandle item,
    MiniFunction<void(IndexInstallList const&)> onInstall
) {
    auto ret = new InstallListPopup();
    if (!ret->init(380.f, 250.f, std::move(item), std::move(onInstall))) {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    ret->autorelease();
    return ret;
}

// SelectVersionPopup

bool SelectVersionPopup::setup(std::string const& modID, IndexItemInstallListCell* installCell) {
    m_modID = modID;
    m_installCell = installCell;

    this->setTitle("Select Version");

    this->createList();

    return true;
}

void SelectVersionPopup::createList() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if (m_listParent) {
        m_listParent->removeFromParent();
    }

    m_listParent = CCNode::create();
    m_mainLayer->addChild(m_listParent);

    auto items = this->createCells();
    m_list = ListView::create(
        items,
        this->getCellSize().height,
        this->getListSize().width,
        this->getListSize().height
    );
    m_list->setPosition(winSize / 2 - m_list->getScaledContentSize() / 2);
    m_listParent->addChild(m_list);

    addListBorders(m_listParent, winSize / 2, m_list->getScaledContentSize());
}

CCArray* SelectVersionPopup::createCells() {
    auto cells = CCArray::create();
    for (auto& item : ranges::reverse(Index::get()->getItemsByModID(m_modID))) {
        cells->addObject(SelectVersionCell::create(item, this, this->getCellSize()));
    }
    return cells;
}

CCSize SelectVersionPopup::getCellSize() const {
    return { getListSize().width, 30.f };
}
CCSize SelectVersionPopup::getListSize() const {
    return { 340.f, 170.f };
}

void SelectVersionPopup::selectItem(IndexItemHandle item) {
    this->onBtn2(nullptr);

    m_installCell->setVersionFromItem(item);
}

SelectVersionPopup* SelectVersionPopup::create(std::string const& modID, IndexItemInstallListCell* installCell) {
    auto ret = new SelectVersionPopup();
    if (!ret->init(380.f, 250.f, modID, installCell)) {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    ret->autorelease();
    return ret;
}