#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

$register_ids(EditorUI) {
    setIDSafe(this, 0, "position-slider");
    setIDSafe(this, this->getChildrenCount() - 2, "layer-index-label");
    setIDSafe(this, this->getChildrenCount() - 1, "object-info-label");

    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("toolbar-categories-menu");

        setIDs(
            menu,
            0,
            "build-button",
            "edit-button",
            "delete-button",

            "swipe-button",
            "free-move-button",
            "snap-button",
            "rotate-button",

            "undo-button",
            "redo-button",
            "delete-button",

            "music-playback-button",

            "playtest-button",
            "stop-playtest-button",

            "zoom-in-button",
            "zoom-out-button",

            "link-button",
            "unlink-button"
        );

        auto toolbarTogglesMenu = detachAndCreateMenu(
            this,
            "toolbar-toggles-menu",
            ColumnLayout::create()
                ->setCrossAxisOverflow(false)
                ->setAxisAlignment(AxisAlignment::Even)
                ->setCrossAxisAlignment(AxisAlignment::Even),
            menu->getChildByID("swipe-button"),
            menu->getChildByID("free-move-button"),
            menu->getChildByID("snap-button"),
            menu->getChildByID("rotate-button")
        );
        toolbarTogglesMenu->setPosition(
            toolbarTogglesMenu->getPosition() - CCPoint { 50.f, 50.f }
        );
        toolbarTogglesMenu->setContentSize({ 100.f, 100.f });
        toolbarTogglesMenu->updateLayout();

        detachAndCreateMenu(
            this,
            "top-left-menu",
            RowLayout::create(),
            menu->getChildByID("undo-button"),
            menu->getChildByID("redo-button"),
            menu->getChildByID("delete-button")
        );

        detachAndCreateMenu(
            this, "playback-menu", RowLayout::create(), menu->getChildByID("music-playback-button")
        );

        detachAndCreateMenu(
            this,
            "playtest-menu",
            RowLayout::create(),
            menu->getChildByID("playtest-button"),
            menu->getChildByID("stop-playtest-button")
        );

        detachAndCreateMenu(
            this,
            "zoom-menu",
            ColumnLayout::create(),
            menu->getChildByID("zoom-in-button"),
            menu->getChildByID("zoom-out-button")
        );

        detachAndCreateMenu(
            this,
            "link-menu",
            ColumnLayout::create(),
            menu->getChildByID("link-button"),
            menu->getChildByID("unlink-button")
        );
    }

    if (auto menu = getChildOfType<CCMenu>(this, 1)) {
        menu->setID("delete-category-menu");

        setIDs(
            menu,
            0,
            "delete-button",
            "delete-startpos-button",
            "delete-all-of-button",

            "delete-filter-none",
            "delete-filter-static",
            "delete-filter-detail",
            "delete-filter-custom",

            "delete-help-icon"
        );

        auto deleteButtonMenu = detachAndCreateMenu(
            menu,
            "delete-button-menu",
            ColumnLayout::create()
                ->setCrossAxisOverflow(false)
                ->setAxisAlignment(AxisAlignment::Even)
                ->setCrossAxisAlignment(AxisAlignment::Even),
            menu->getChildByID("delete-button"),
            menu->getChildByID("delete-all-of-button"),
            menu->getChildByID("delete-startpos-button")
        );
        deleteButtonMenu->setPosition(
            deleteButtonMenu->getPosition() - CCPoint { 50.f, 50.f }
        );
        deleteButtonMenu->setContentSize({ 100.f, 100.f });
        deleteButtonMenu->updateLayout();

        auto deleteFilterMenu = detachAndCreateMenu(
            menu,
            "delete-filter-menu",
            ColumnLayout::create()
                ->setCrossAxisOverflow(false)
                ->setAxisAlignment(AxisAlignment::Even)
                ->setCrossAxisAlignment(AxisAlignment::Even),
            menu->getChildByID("delete-filter-none"),
            menu->getChildByID("delete-filter-static"),
            menu->getChildByID("delete-filter-detail"),
            menu->getChildByID("delete-filter-custom")
        );
        deleteFilterMenu->setPosition(
            deleteFilterMenu->getPosition() - CCPoint { 50.f, 50.f }
        );
        deleteFilterMenu->setContentSize({ 100.f, 100.f });
        deleteFilterMenu->updateLayout();
    }

    if (auto menu = getChildOfType<CCMenu>(this, 2)) {
        menu->setID("build-tabs-menu");

        setIDs(
            menu,
            0,
            "static-tab-1",
            "static-tab-2",
            "static-tab-3",
            "slope-tab",
            "hazard-tab",
            "3d-tab",
            "portal-tab",
            "deco-tab-1",
            "deco-tab-2",
            "pulse-deco-tab",
            "sawblade-tab",
            "trigger-tab",
            "custom-tab"
        );
    }

    if (auto menu = getChildOfType<CCMenu>(this, 3)) {
        setIDs(
            menu,
            0,
            "pause-button",
            "settings-button",
            "copy-paste-button",
            "copy-button",
            "paste-button",
            "hsv-button",
            "edit-special-button",
            "edit-object-button",
            "deselect-button",
            "edit-group-button",
            "portal-check",
            "copy-values-button",
            "paste-state-button",
            "paste-color-button",
            "go-to-layer-button",
            "next-layer-button",
            "prev-layer-button",
            "all-layers-button"
        );

        auto topRightMenu = detachAndCreateMenu(
            this,
            "top-right-menu",
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::End),
            menu->getChildByID("pause-button"),
            menu->getChildByID("settings-button")
        );
        topRightMenu->setContentSize({ 60.f, 125.f });
        topRightMenu->setPositionX(topRightMenu->getPositionX() - 125.f / 2);
        topRightMenu->updateLayout();

        auto rightMenu = detachAndCreateMenu(
            this,
            "editor-buttons-menu",
            ColumnLayout::create()
                ->setAxisAlignment(AxisAlignment::Even)
                ->setCrossAxisAlignment(AxisAlignment::End)
                ->setCrossAxisReverse(true),
            menu->getChildByID("copy-paste-button"),
            menu->getChildByID("edit-object-button"),
            menu->getChildByID("paste-color-button"),
            menu->getChildByID("deselect-button"),
            menu->getChildByID("paste-button"),
            menu->getChildByID("edit-group-button"),
            menu->getChildByID("paste-state-button"),
            menu->getChildByID("go-to-layer-button"),
            menu->getChildByID("copy-button"),
            menu->getChildByID("edit-special-button"),
            menu->getChildByID("copy-values-button"),
            menu->getChildByID("hsv-button")
        );
        rightMenu->setContentSize({ 125.f, 125.f });
        rightMenu->setPosition(rightMenu->getPosition() - CCPoint { 125.f, 125.f });
        rightMenu->updateLayout();

        detachAndCreateMenu(
            this,
            "layer-menu",
            RowLayout::create(),
            menu->getChildByID("all-layers-button"),
            menu->getChildByID("prev-layer-button"),
            this->getChildByID("layer-index-label"),
            menu->getChildByID("next-layer-button")
        );
    }
}

struct EditorUIIDs : Modify<EditorUIIDs, EditorUI> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("EditorUI::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set EditorUI::init hook priority, node IDs may not work properly");
        }
    }

    bool init(LevelEditorLayer* lel) {
        if (!EditorUI::init(lel)) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};
