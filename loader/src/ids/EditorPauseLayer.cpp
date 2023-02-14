#include "AddIDs.hpp"

#include <Geode/modify/EditorPauseLayer.hpp>

USE_GEODE_NAMESPACE();

$register_ids(EditorPauseLayer) {
    auto winSize = CCDirector::get()->getWinSize();

    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("resume-menu");

        setIDs(
            menu, 0,
            "resume-button",
            "save-and-play-button",
            "save-and-exit-button",
            "save-button",
            "exit-button"
        );

        menu->setContentSize({ 100.f, 220.f });
        menu->setLayout(
            ColumnLayout::create()
                ->setGap(10.f)
                ->setAxisReverse(true)
        );
    }

    setIDs(
        this, 2,
        "ignore-damage-label",
        "follow-player-label",
        "select-filter-label",
        "show-grid-label",
        "show-object-info-label",
        "show-ground-label",
        "preview-mode-label",

        "object-count-label",
        "length-label",
        "length-name-label"
    );

    if (auto menu = getChildOfType<CCMenu>(this, 1)) {
        menu->setID("bottom-menu");

        setIDs(
            menu, 0, 
            "guidelines-enable-button",
            "help-button",
            "guidelines-disable-button",

            "uncheck-portals-button",
            "reset-unused-button",
            "create-edges-button",
            "create-outlines-button",
            "create-base-button",
            "build-helper-button",

            "align-x-button",
            "align-y-button",
            "select-all-button",
            "select-all-left-button",
            "select-all-right-button",

            "ignore-damage-toggle",
            "follow-player-toggle",
            "select-filter-toggle",
            "show-grid-toggle",
            "show-object-info-toggle",
            "show-ground-toggle",
            "preview-mode-toggle",

            "keys-button",
            "settings-button"
        );

        auto smallActionsMenu = detachAndCreateMenu(
            this,
            "small-actions-menu",
            ColumnLayout::create(),
            menu->getChildByID("align-x-button"),
            menu->getChildByID("align-y-button"),
            menu->getChildByID("select-all-button"),
            menu->getChildByID("select-all-left-button"),
            menu->getChildByID("select-all-right-button")
        );

        auto actionsMenu = detachAndCreateMenu(
            this,
            "actions-menu",
            ColumnLayout::create(),
            menu->getChildByID("uncheck-portals-button"),
            menu->getChildByID("reset-unused-button"),
            menu->getChildByID("create-edges-button"),
            menu->getChildByID("create-outlines-button"),
            menu->getChildByID("create-base-button"),
            menu->getChildByID("build-helper-button"),
            menu->getChildByID("keys-button")
        );

        auto optionsMenu = detachAndCreateMenu(
            this,
            "options-menu",
            ColumnLayout::create()
                ->setGap(-1.f)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setGrowCrossAxis(true)
                ->setCrossAxisAlignment(AxisAlignment::Start)
                ->setCrossAxisOverflow(false),
            menu->getChildByID("ignore-damage-toggle"),
            this->getChildByID("ignore-damage-label"),
            menu->getChildByID("follow-player-toggle"),
            this->getChildByID("follow-player-label"),
            menu->getChildByID("select-filter-toggle"),
            this->getChildByID("select-filter-label"),
            menu->getChildByID("show-grid-toggle"),
            this->getChildByID("show-grid-label"),
            menu->getChildByID("show-object-info-toggle"),
            this->getChildByID("show-object-info-label"),
            menu->getChildByID("show-ground-toggle"),
            this->getChildByID("show-ground-label"),
            menu->getChildByID("preview-mode-toggle"),
            this->getChildByID("preview-mode-label")
        );
        for (auto node : CCArrayExt<CCNode>(optionsMenu->getChildren())) {
            if (auto label = typeinfo_cast<CCLabelBMFont*>(node)) {
                label->setLayoutOptions(
                    AxisLayoutOptions::create()
                        ->setBreakLine(true)
                );
            }
        }
        optionsMenu->setContentSize({ 120.f, winSize.height - 100.f });
        optionsMenu->setPosition(70.f, winSize.height / 2 - 50.f + 10.f);
        optionsMenu->updateLayout();

        auto settingsMenu = detachAndCreateMenu(
            this,
            "settings-menu",
            RowLayout::create()
                ->setAxisReverse(true),
            menu->getChildByID("settings-button")
        );
    }
}

struct EditorPauseLayerIDs : Modify<EditorPauseLayerIDs, EditorPauseLayer> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("EditorPauseLayer::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set EditorPauseLayer::init hook priority, node IDs may not work properly");
        }
    }

    bool init(LevelEditorLayer* lel) {
        if (!EditorPauseLayer::init(lel)) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};
