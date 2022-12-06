#include "ModListView.hpp"

#include "../info/CategoryNode.hpp"
#include "ModListLayer.hpp"
#include "ModListCell.hpp"

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/TableView.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/loader/Index.hpp>
#include <InternalLoader.hpp>

void ModListView::updateAllStates(ModListCell* toggled) {
    for (auto cell : CCArrayExt<ModListCell>(m_tableView->m_cellArray)) {
        if (toggled != cell) {
            cell->updateState();
        }
    }
}

void ModListView::setupList() {
    m_itemSeparation = m_display == ModListDisplay::Expanded ? 60.f : 40.0f;

    if (!m_entries->count()) return;

    m_tableView->reloadData();

    // fix content layer content size so the
    // list is properly aligned to the top
    auto coverage = calculateChildCoverage(m_tableView->m_contentLayer);
    m_tableView->m_contentLayer->setContentSize({
        -coverage.origin.x + coverage.size.width,
        -coverage.origin.y + coverage.size.height
    });

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
    return ModCell::create(this, m_display, key, { m_width, m_itemSeparation });
}

void ModListView::loadCell(TableViewCell* cell, unsigned int index) {
    auto obj = m_entries->objectAtIndex(index);
    if (auto mod = typeinfo_cast<ModObject*>(obj)) {
        as<ModCell*>(cell)->loadFromMod(mod->mod);
    }
    if (auto mod = typeinfo_cast<IndexItemObject*>(obj)) {
        // as<IndexItemCell*>(cell)->loadFromItem(mod->item);
    }
    if (auto failed = typeinfo_cast<InvalidGeodeFileObject*>(obj)) {
        as<InvalidGeodeFileCell*>(cell)->loadFromInfo(failed->info);
    }
    as<ModListCell*>(cell)->updateBGColor(index);
}

static void sortInstalledMods(std::vector<Mod*>& mods) {
    if (!mods.size()) return;
    // keep track of first object
    size_t frontIndex = 0;
    auto front = mods.front();
    for (auto mod = mods.begin(); mod != mods.end(); mod++) {
        // move mods with updates to front
        if (auto item = Index::get()->getItem(*mod)) {
            if (Index::get()->updateAvailable(item)) {
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
}

static std::vector<Mod*> sortedInstalledMods() {
    auto mods = Loader::get()->getAllMods();
    sortInstalledMods(mods);
    return std::move(mods);
}

bool ModListView::init(CCArray* mods, ModListDisplay display) {
    m_display = display;
    return CustomListView::init(mods, BoomListType::Default, 358.f, 190.f);
}

CCArray* ModListView::getModsForType(ModListType type) {
    auto mods = CCArray::create();
    switch (type) {
        default:
        case ModListType::Installed: {
            // failed mods first
            for (auto const& mod : Loader::get()->getFailedMods()) {
                mods->addObject(new InvalidGeodeFileObject(mod));
            }
            // internal geode representation always at the top
            auto imod = Loader::getInternalMod();
            mods->addObject(new ModObject(imod));

            // then other mods
            for (auto const& mod : sortedInstalledMods()) {
                // if the mod is no longer installed nor
                // loaded, it's as good as not existing
                // (because it doesn't)
                if (mod->isUninstalled() && !mod->isLoaded()) continue;
                mods->addObject(new ModObject(mod));
            }
        } break;

        case ModListType::Download: {
            for (auto const& item : Index::get()->getItems()) {
                mods->addObject(new IndexItemObject(item));
            }
        } break;

        case ModListType::Featured: {
        } break;
    }
    return mods;
}

ModListView* ModListView::create(CCArray* mods, ModListDisplay display) {
    auto pRet = new ModListView;
    if (pRet) {
        if (pRet->init(mods, display)) {
            pRet->autorelease();
            return pRet;
        }
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

ModListView* ModListView::create(ModListType type, ModListDisplay display) {
    return ModListView::create(getModsForType(type), display);
}

void ModListView::setLayer(ModListLayer* layer) {
    m_layer = layer;
}

void ModListView::refreshList() {
    if (m_layer) {
        m_layer->reloadList();
    }
}
