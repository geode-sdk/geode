#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

$register_ids(EditLevelLayer) {
    setIDs(
        this,
        0,
        "background",
        "bottom-left-art",
        "bottom-right-art",
        "level-name-background",
        "level-name-input",
        "description-background",
        "description-input",
        "description-text-area",
        "level-action-menu",
        "level-length",
        "level-song",
        "level-verified",
        "version-label",
        "level-id-label",
        "right-side-menu",
        "back-button-menu",
        "info-button-menu"
    );

    if (auto menu = this->getChildByID("level-action-menu")) {
        setIDs(menu, 0, "edit-button", "play-button", "share-button");
    }

    if (auto menu = this->getChildByID("right-side-menu")) {
        setIDs(
            menu,
            0,
            "delete-button",
            "help-button",
            "duplicate-button",
            "move-to-top-button",
            "folder-button"
        );

        detachAndCreateMenu(
            menu, "folder-menu", ColumnLayout::create(), menu->getChildByID("folder-button")
        );
    }

    if (auto menu = this->getChildByID("back-button-menu")) setIDSafe(menu, 0, "back-button");

    if (auto menu = this->getChildByID("info-button-menu")) setIDSafe(menu, 0, "info-button");
}

struct EditLevelLayerIDs : Modify<EditLevelLayerIDs, EditLevelLayer> {
    bool init(GJGameLevel* l) {
        if (!EditLevelLayer::init(l)) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};
