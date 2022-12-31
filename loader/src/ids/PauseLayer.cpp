#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

$register_ids(PauseLayer) {
    setIDs(
        this,
        0,
        "background",
        "level-name",

        "normal-progress-bar",
        "practice-progress-bar",
        "normal-progress-label",
        "practice-progress-label",
        "normal-mode-label",
        "practice-mode-label",

        "center-button-menu",

        "auto-retry-label",
        "auto-checkpoints-label",
        "show-progress-bar-label"
    );

    if (auto menu = this->getChildByID("center-button-menu")) {
        int start_idx = 0;

        if (menu->getChildrenCount() == 5) {
            setIDSafe(menu, 0, "edit-button");
            start_idx = 1;
        }

        setIDs(menu, start_idx, "practice-button", "play-button", "exit-button");

        if (menu->getChildrenCount() == 4) setIDSafe(menu, start_idx + 3, "retry-button");
    }

    // Record toggle on mobile
    if (auto label = typeinfo_cast<CCLabelBMFont*>(getChild(this, 12))) {
        setIDSafe(this, 12, "record-label");
    }

    if (auto menu = getChildOfType<CCMenu>(this, 1)) {
        menu->setID("toggle-menu");

        setIDs(menu, 0, "auto-retry-toggle", "auto-checkpoints-toggle", "show-progress-bar-toggle");

        if (menu->getChildrenCount() == 4) setIDSafe(menu, 3, "record-toggle");
    }

    setIDs(
        this, this->getChildrenCount() - 4, "music-slider", "sfx-slider", "music-label", "sfx-label"
    );
}

struct PauseLayerIDs : Modify<PauseLayerIDs, PauseLayer> {
    void customSetup() {
        PauseLayer::customSetup();

        NodeIDs::get()->provide(this);
    }
};
