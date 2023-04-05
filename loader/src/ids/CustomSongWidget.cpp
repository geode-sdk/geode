#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

$register_ids(CustomSongWidget) {
    setIDSafe<CCScale9Sprite>(this, 0, "bg");
    setIDSafe<CCSprite>(this, 0, "loading-bar");
    setIDSafe<CCLabelBMFont>(this, 0, "song-name-label");
    setIDSafe<CCLabelBMFont>(this, 1, "author-name-label");
    setIDSafe<CCLabelBMFont>(this, 2, "id-and-size-label");
    setIDSafe<CCLabelBMFont>(this, 3, "error-label");
    setIDSafe<CCMenu>(this, 0, "buttons-menu");
    auto customSongWidgetMenu = this->getChildByID("buttons-menu");
    setIDSafe<CCMenuItemSpriteExtra>(customSongWidgetMenu, 0, "download-button");
    setIDSafe<CCMenuItemSpriteExtra>(customSongWidgetMenu, 1, "cancel-button");
    setIDSafe<CCMenuItemSpriteExtra>(customSongWidgetMenu, 2, "use-button");
    setIDSafe<CCMenuItemSpriteExtra>(customSongWidgetMenu, 3, "refresh-button");
    setIDSafe<CCMenuItemSpriteExtra>(customSongWidgetMenu, 4, "play-song-button");
    setIDSafe<CCMenuItemSpriteExtra>(customSongWidgetMenu, 5, "more-button");
};

struct CustomSongWidgetIDs : Modify<CustomSongWidgetIDs, CustomSongWidget> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("CustomSongWidget::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set CustomSongWidget::init hook priority, node IDs may not work properly");
        }
    }

    bool init(SongInfoObject* s, LevelSettingsObject* l, bool b1, bool b2, bool b3, bool b4, bool hideBackground) {
        if (!CustomSongWidget::init(s, l, b1, b2, b3, b4, hideBackground)) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};