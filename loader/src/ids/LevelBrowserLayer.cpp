#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

$register_ids(LevelBrowserLayer) {
    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("back-menu");
        setIDSafe(menu, 0, "back-button");
    }

    if (m_searchObject->m_searchType == SearchType::MyLevels) {
        if (auto menu = getChildOfType<CCMenu>(this, 2)) {
            menu->setID("new-level-menu");
            auto newLvlBtn = setIDSafe(menu, 0, "new-level-button");

            if (auto myLevelsBtn = setIDSafe(menu, 1, "my-levels-button")) {
                auto menu = detachAndCreateMenu(
                    this,
                    "my-levels-menu",
                    ColumnLayout::create()
                        ->setAxisAlignment(AxisAlignment::Start),
                    myLevelsBtn
                );
                menu->setPositionY(
                    menu->getPositionY() + 100.f / 2 - 
                        myLevelsBtn->getScaledContentSize().height / 2
                );
                menu->setContentSize({ 50.f, 100.f });
                menu->updateLayout();
            }

            menu->setLayout(
                ColumnLayout::create()
                    ->setAxisAlignment(AxisAlignment::Start)
            );
            menu->setPositionY(
                menu->getPositionY() + 130.f / 2 - 
                    newLvlBtn->getScaledContentSize().height / 2
            );
            menu->setContentSize({ 50.f, 130.f });
            menu->updateLayout();
        }

        if (auto menu = getChildOfType<CCMenu>(this, 1)) {
            if (auto searchBtn = setIDSafe(menu, 5, "search-button")) {
                auto clearBtn = setIDSafe(menu, 6, "clear-search-button");
                // this is a hacky fix because for some reason adding children 
                // before the clear button is made visible is inconsistent
                if (clearBtn) {
                    searchBtn->setZOrder(-1);
                    clearBtn->setZOrder(-1);
                }
                auto searchMenu = detachAndCreateMenu(
                    this,
                    "search-menu",
                    ColumnLayout::create()
                        ->setAxisReverse(true)
                        ->setCrossAxisReverse(true)
                        ->setGrowCrossAxis(true)
                        ->setCrossAxisOverflow(false)
                        ->setCrossAxisAlignment(AxisAlignment::Start)
                        ->setAxisAlignment(AxisAlignment::End),
                    searchBtn,
                    clearBtn
                );
                searchMenu->setPositionX(
                    searchMenu->getPositionX() + 80.f / 2 - 
                        searchBtn->getScaledContentSize().width / 2
                );
                searchMenu->setPositionY(
                    searchMenu->getPositionY() - 80.f / 2 + 
                        searchBtn->getScaledContentSize().height / 2
                );
                searchMenu->setContentSize({ 80.f, 80.f });
                searchMenu->updateLayout();
            }

            if (auto pageBtn = setIDSafe(menu, 2, "page-button")) {
                auto folderBtn = setIDSafe(menu, 3, "folder-button");
                auto lastPageBtn = setIDSafe(menu, 4, "last-page-button");
                auto pageMenu = detachAndCreateMenu(
                    this,
                    "page-menu",
                    ColumnLayout::create()
                        ->setAxisReverse(true)
                        ->setGrowCrossAxis(true)
                        ->setAxisAlignment(AxisAlignment::End),
                    pageBtn,
                    folderBtn,
                    lastPageBtn
                );
                pageMenu->setContentSize({ 40.f, 110.f });
                pageMenu->setAnchorPoint({ 1.f, .5f });
                pageMenu->setPosition(
                    pageMenu->getPositionX() + 20.f,
                    pageMenu->getPositionY() - 110.f / 2 + 12.5f
                );
                pageMenu->updateLayout();
            }
        }
    }
}

struct LevelBrowserLayerIDs : Modify<LevelBrowserLayerIDs, LevelBrowserLayer> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("LevelBrowserLayer::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set LevelBrowserLayer::init hook priority, node IDs may not work properly");
        }
    }

    bool init(GJSearchObject* obj) {
        if (!LevelBrowserLayer::init(obj)) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};
