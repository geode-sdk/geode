#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

$register_ids(LevelBrowserLayer) {
    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("go-back-menu");
        setIDSafe(menu, 0, "back-button");
    }

    if (m_searchObject->m_searchType == SearchType::MyLevels) {
        if (auto menu = getChildOfType<CCMenu>(this, 2)) {
            menu->setID("new-level-menu");
            setIDSafe(menu, 0, "new-level-button");

            if (auto myLevelsBtn = setIDSafe(menu, 1, "my-levels-button")) {
                detachAndCreateMenu(
                    this,
                    "my-levels-menu",
                    ColumnLayout::create()
                        ->setFitInside(false)
                        ->setAxisAlignment(Alignment::End),
                    myLevelsBtn
                )->setAnchorPoint({ .5f, 1.f });
            }

            menu->setLayout(
                ColumnLayout::create()
                    ->setFitInside(false)
                    ->setAxisAlignment(Alignment::End)
            );
            menu->setAnchorPoint({ .5f, 1.f });
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
