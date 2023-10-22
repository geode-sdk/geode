#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

$register_ids(LevelBrowserLayer) {
    auto winSize = CCDirector::get()->getWinSize();

    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("back-menu");
        auto btn = setIDSafe(menu, 0, "back-button");
        menu->setContentSize({ 100.f, 50.f });
        menu->setPositionX(
            menu->getPositionX() + 100.f / 2 - 
                getSizeSafe(btn).width / 2
        );
        menu->setLayout(
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::Start)
        );
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
                    menu->getPositionY() + 125.f / 2 - 
                        myLevelsBtn->getScaledContentSize().height / 2
                );
                menu->setContentSize({ 50.f, 125.f });
                menu->updateLayout();
            }

            menu->setLayout(
                ColumnLayout::create()
                    ->setAxisAlignment(AxisAlignment::Start)
            );
            menu->setPositionY(
                menu->getPositionY() + 130.f / 2 - 
                    getSizeSafe(newLvlBtn).height / 2
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
                auto width = 45.f * winSize.aspect();
                searchMenu->setPosition(
                    searchMenu->getPositionX() + width / 2 - 
                        searchBtn->getScaledContentSize().width / 2,
                    searchMenu->getPositionY() - 80.f / 2 + 
                        searchBtn->getScaledContentSize().height / 2
                );
                searchMenu->setContentSize({ width, 80.f });
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

            auto navMenuWidth = 50.f * winSize.aspect();

            if (auto prevPageBtn = setIDSafe(menu, 0, "prev-page-button")) {
                auto navMenu = detachAndCreateMenu(
                    this,
                    "prev-page-menu",
                    RowLayout::create()
                        ->setAxisAlignment(AxisAlignment::Start),
                    prevPageBtn
                );
                prevPageBtn->setZOrder(-1);
                navMenu->setContentSize({ navMenuWidth, 40.f });
                navMenu->setPositionX(
                    navMenu->getPositionX() + navMenuWidth / 2 - 
                        prevPageBtn->getScaledContentSize().width / 2
                );
                navMenu->updateLayout();
            }

            auto nextPageBtn = setIDSafe(menu, 0, "next-page-button");

            menu->setID("next-page-menu");
            menu->setLayout(
                RowLayout::create()
                    ->setAxisReverse(true)
                    ->setAxisAlignment(AxisAlignment::End)
            );
            menu->setContentSize({ navMenuWidth, 40.f });
            menu->setPositionX(
                winSize.width - navMenuWidth / 2 - 5.f
            );
            menu->updateLayout();
        }
    }

    auto bottomMenu = CCMenu::create();
    bottomMenu->setID("bottom-menu");
    bottomMenu->setContentSize({ 325.f + 20.f * winSize.aspect(), 50.f });
    bottomMenu->setPosition(winSize.width / 2, 28.f);
    bottomMenu->setZOrder(15);
    bottomMenu->setLayout(RowLayout::create());
    this->addChild(bottomMenu);
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