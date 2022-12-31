#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/Modify.hpp>
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
                    ColumnLayout::create(5.f, 0.f)->setAlignment(Alignment::End),
                    myLevelsBtn
                );
            }

            menu->setLayout(ColumnLayout::create(5.f, 0.f)->setAlignment(Alignment::End));
        }
    }
}

struct LevelBrowserLayerIDs : Modify<LevelBrowserLayerIDs, LevelBrowserLayer> {
    bool init(GJSearchObject* obj) {
        if (!LevelBrowserLayer::init(obj)) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};
