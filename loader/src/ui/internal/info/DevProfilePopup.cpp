#include "DevProfilePopup.hpp"
#include <Geode/ui/ListView.hpp>
#include <Geode/loader/Index.hpp>
#include <Geode/ui/General.hpp>
#include "../list/ModListCell.hpp"
#include "../list/ModListLayer.hpp"

bool DevProfilePopup::setup(std::string const& developer, ModListLayer* list) {
    m_noElasticity = true;
    m_layer = list;

    this->setTitle("Mods by " + developer);

    auto winSize = CCDirector::get()->getWinSize();

    auto items = CCArray::create();

    // installed mods
    for (auto& mod : Loader::get()->getAllMods()) {
        if (mod->getDeveloper() == developer) {
            auto cell = ModCell::create(
                mod, m_layer, ModListDisplay::Concise, { 358.f, 40.f }
            );
            cell->disableDeveloperButton();
            items->addObject(cell);
        }
    }

    // index mods
    for (auto& item : Index::get()->getItemsByDeveloper(developer)) {
        if (Loader::get()->isModInstalled(item->getMetadata().getID())) {
            continue;
        }
        auto cell = IndexItemCell::create(
            item, m_layer, ModListDisplay::Concise, { 358.f, 40.f }
        );
        cell->disableDeveloperButton();
        items->addObject(cell);
    }

    // mods list
    auto listSize = CCSize { 358.f, 160.f };
    auto cellList = ListView::create(items, 40.f, listSize.width, listSize.height);
    cellList->setPosition(winSize / 2 - listSize / 2);
    m_mainLayer->addChild(cellList);

    addListBorders(m_mainLayer, winSize / 2, listSize);

    return true;
}

DevProfilePopup* DevProfilePopup::create(std::string const& developer, ModListLayer* list) {
    auto ret = new DevProfilePopup();
    if (ret && ret->init(420.f, 260.f, developer, list)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
