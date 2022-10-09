#pragma once

#include <Geode/Geode.hpp>
#include "ModListView.hpp"
#include <Index.hpp>

USE_GEODE_NAMESPACE();

class SearchFilterPopup;

class ModListLayer : public CCLayer, public TextInputDelegate {
protected:
	GJListLayer* m_list = nullptr;
	CCLabelBMFont* m_listLabel;
	CCLabelBMFont* m_indexUpdateLabel;
	CCMenu* m_menu;
	CCMenu* m_topMenu;
	CCMenuItemToggler* m_installedTabBtn;
	CCMenuItemToggler* m_downloadTabBtn;
	CCMenuItemToggler* m_featuredTabBtn;
	CCMenuItemSpriteExtra* m_searchBtn;
	CCMenuItemSpriteExtra* m_searchClearBtn;
	CCMenuItemSpriteExtra* m_checkForUpdatesBtn = nullptr;
	CCNode* m_searchBG = nullptr;
	CCTextInputNode* m_searchInput = nullptr;
	LoadingCircle* m_loadingCircle = nullptr;
	ModListQuery m_query;
	CCMenuItemSpriteExtra* m_filterBtn;
	bool m_expandedList = false;

	virtual ~ModListLayer();

	bool init() override;

	void onExit(CCObject*);
	void onReload(CCObject*);
	void onCheckForUpdates(CCObject*);
	void onOpenFolder(CCObject*);
	void onResetSearch(CCObject*);
	void keyDown(enumKeyCodes) override;
	void onTab(CCObject*);
	void textChanged(CCTextInputNode*) override;
	void indexUpdateProgress(
        UpdateStatus status,
        std::string const& info,
        uint8_t percentage
	);
	void createSearchControl();

	friend class SearchFilterPopup;

public:
	static ModListLayer* create();
	static ModListLayer* scene();

	void reloadList();
};
