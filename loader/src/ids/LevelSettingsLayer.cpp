#include <Geode/Modify.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/utils/cocos.hpp>
#include "AddIDs.hpp"

USE_GEODE_NAMESPACE();

$register_ids(LevelSettingsLayer) {
	bool startPos = m_mainLayer->getChildrenCount() < 10;

	if (startPos) {
		setIDSafe(m_mainLayer, 0, "back-button");
	}

	if (auto menu = getChildOfType<CCMenu>(m_mainLayer, 0)) {
		menu->setID("song-select-menu");

		if (startPos) {
			setIDs(menu, 0,
				"cube-button",
				"ship-button",
				"ball-button",
				"ufo-button",
				"wave-button",
				"robot-button",
				"spider-button",
				"mini-toggle",
				"dual-toggle",
				"ok-button",
				"flip-gravity-toggle",
				"half-speed-button",
				"normal-speed-button",
				"2x-speed-button",
				"3x-speed-button",
				"4x-speed-button"
			);
		} else {
			setIDs(menu, 0,
				"bg-color-button",
				"g-color-button",
				"g2-color-button",
				"line-color-button",
				"obj-color-button",
				"more-color-button",
				"3dl-color-button",
				"bg-quick-edit-button",
				"g-quick-edit-button",
				"g2-quick-edit-button",
				"line-quick-edit-button",
				"cube-button",
				"ship-button",
				"ball-button",
				"ufo-button",
				"wave-button",
				"robot-button",
				"spider-button",
				"background-select-button",
				"ground-select-button",
				"mini-toggle",
				"dual-toggle",
				"font-button",
				"ok-button",
				"2-player-toggle",
				"2-player-help-button",
				"prev-song-button",
				"next-song-button",
				"normal-song-button",
				"custom-song-button",
				"select-custom-song-button",
				"new-song-button",
				"half-speed-button",
				"normal-speed-button",
				"2x-speed-button",
				"3x-speed-button",
				"4x-speed-button"
			);
		}

		detachAndCreateMenu(
			this,
			"vehicle-selection-menu",
			RowLayout::create(),
			menu->getChildByID("cube-button"),
			menu->getChildByID("ship-button"),
			menu->getChildByID("ball-button"),
			menu->getChildByID("ufo-button"),
			menu->getChildByID("wave-button"),
			menu->getChildByID("robot-button"),
			menu->getChildByID("spider-button")
		);

		detachAndCreateMenu(
			this,
			"right-toggle-menu",
			ColumnLayout::create(),
			menu->getChildByID("mini-toggle"),
			menu->getChildByID("dual-toggle")
		);

		detachAndCreateMenu(
			this,
			"speed-selection-menu",
			ColumnLayout::create(),
			menu->getChildByID("half-speed-button"),
			menu->getChildByID("normal-song-button"),
			menu->getChildByID("2x-speed-button"),
			menu->getChildByID("3x-speed-button"),
			menu->getChildByID("4x-speed-button")
		);

		if (startPos) {
			detachAndCreateMenu(
				this,
				"flip-gravity-menu",
				ColumnLayout::create(),
				menu->getChildByID("flip-gravity-toggle")
			);
		} else {
			detachAndCreateMenu(
				this,
				"color-button-menu",
				RowLayout::create(),
				menu->getChildByID("bg-color-button"),
				menu->getChildByID("g-color-button"),
				menu->getChildByID("g2-color-button"),
				menu->getChildByID("line-color-button"),
				menu->getChildByID("obj-color-button"),
				menu->getChildByID("3dl-color-button"),
				menu->getChildByID("more-color-button")
			);

			detachAndCreateMenu(
				this,
				"color-quick-edit-menu",
				RowLayout::create(),
				menu->getChildByID("bg-quick-edit-button"),
				menu->getChildByID("g-quick-edit-button"),
				menu->getChildByID("g2-quick-edit-button"),
				menu->getChildByID("line-quick-edit-button")
			);

			detachAndCreateMenu(
				this,
				"scenery-selection-menu",
				ColumnLayout::create(),
				menu->getChildByID("background-select-button"),
				menu->getChildByID("ground-select-button")
			);

			detachAndCreateMenu(
				this,
				"2-player-menu",
				ColumnLayout::create(),
				menu->getChildByID("2-player-help-button"),
				menu->getChildByID("2-player-toggle")
			);

			detachAndCreateMenu(
				this,
				"font-button-menu",
				RowLayout::create()->setAlignment(Alignment::End),
				menu->getChildByID("font-button")
			);
		}
	}

	setIDs(m_mainLayer, 2,
		"select-color-label",
		"bg-color-label",
		"g-color-label",
		"g2-color-label",
		"3dl-color-label",
		"line-color-label",
		"obj-color-label",
		"more-color-label",
		"select-mode-label",
		"bg-selection-label",
		"g-selection-label",
		"mini-label",
		"dual-label",
		"2-player-label-1",
		"2-player-label-2",
		"select-song-label",
		"default-song-label",
		"custom-song-widget",
		"speed-label"
	);
}

class $modify(LevelSettingsLayer) {
    bool init(LevelSettingsObject* levelSettings, LevelEditorLayer* editor) {
        if (!LevelSettingsLayer::init(levelSettings, editor))
            return false;
        
        //NodeIDs::get()->provide(this);

        return true;
    }
};
