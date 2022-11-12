#include "AddIDs.hpp"
#include <Geode/modify/LevelSearchLayer.hpp>

$register_ids(LevelSearchLayer) {
	// set the funny ids
	setIDSafe(this, 0, "background");
	getChildOfType<CCTextInputNode>(this, 0)->setID("search-bar");
	getChildOfType<CCScale9Sprite>(this, 0)->setID("level-search-bg");
	getChildOfType<CCScale9Sprite>(this, 1)->setID("level-search-bar-bg");
	getChildOfType<CCScale9Sprite>(this, 2)->setID("quick-search-bg");
	getChildOfType<CCScale9Sprite>(this, 3)->setID("difficulty-filters-bg");
	getChildOfType<CCScale9Sprite>(this, 4)->setID("length-filters-bg");
	getChildOfType<CCLabelBMFont>(this, 0)->setID("quick-search-title");
	getChildOfType<CCLabelBMFont>(this, 1)->setID("filters-title");
	getChildOfType<CCSprite>(this, 1)->setID("left-corner");
	getChildOfType<CCSprite>(this, 2)->setID("right-corner");
	
	if (auto filtermenu = getChildOfType<CCMenu>(this, 0)) {
		filtermenu->setID("other-filter-menu");
		setIDSafe(filtermenu, 0, "clear-filters-button");
		setIDSafe(filtermenu, 1, "advanced-filters-button");
	}
	if (auto searchmenu = getChildOfType<CCMenu>(this, 1)) {
		searchmenu->setID("search-button-menu");
		setIDSafe(searchmenu, 0, "search-level-button");
		setIDSafe(searchmenu, 1, "search-user-button");

	}
	if (auto quickmenu = getChildOfType<CCMenu>(this, 2)) {
		quickmenu->setID("quick-search-menu");
		setIDSafe(quickmenu, 0, "most-downloaded-button");
		setIDSafe(quickmenu, 1, "most-liked-button");
		setIDSafe(quickmenu, 2, "trending-button");
		setIDSafe(quickmenu, 3, "recent-button");
		setIDSafe(quickmenu, 4, "magic-button");
		setIDSafe(quickmenu, 5, "awarded-button");
		setIDSafe(quickmenu, 6, "followed-button");
		setIDSafe(quickmenu, 7, "friends-button");
	}
	if (auto filtersmenu = getChildOfType<CCMenu>(this, 3)) {
		filtersmenu->setID("difficulty-filter-menu");
		setIDSafe(filtersmenu, 0, "na-filter-button");
		setIDSafe(filtersmenu, 1, "easy-filter-button");
		setIDSafe(filtersmenu, 2, "normal-filter-button");
		setIDSafe(filtersmenu, 3, "hard-filter-button");
		setIDSafe(filtersmenu, 4, "harder-filter-button");
		setIDSafe(filtersmenu, 5, "insane-filter-button");
		setIDSafe(filtersmenu, 6, "demon-filter-button");
		setIDSafe(filtersmenu, 7, "auto-filter-button");
		setIDSafe(filtersmenu, 8, "demon-type-filter-button");
	}
	if (auto filtersmenu = getChildOfType<CCMenu>(this, 4)) {
		filtersmenu->setID("length-filter-menu");
		setIDSafe(filtersmenu, 0, "clock-icon");
		setIDSafe(filtersmenu, 1, "tiny-filter-button");
		setIDSafe(filtersmenu, 2, "short-filter-button");
		setIDSafe(filtersmenu, 3, "medium-filter-button");
		setIDSafe(filtersmenu, 4, "long-filter-button");
		setIDSafe(filtersmenu, 5, "xl-filter-button");
		setIDSafe(filtersmenu, 6, "star-filter-button");
	}
	if (auto backmenu = getChildOfType<CCMenu>(this, 5)) {
		backmenu->setID("exit-menu");
		setIDSafe(backmenu, 0, "exit-button");
	}
}

class $modify(LevelSearchLayer) {
    bool init() {
        if (!LevelSearchLayer::init())
            return false;
        
        NodeIDs::get()->provide(this);

        return true;
    }
};

