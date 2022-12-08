#pragma once

#include <Geode/binding/TextInputDelegate.hpp>
#include <Geode/loader/Index.hpp>

USE_GEODE_NAMESPACE();

class SearchFilterPopup;
class ModListCell;

enum class ModListType {
    Installed,
    Download,
    Featured,
};

enum class ModListDisplay {
    Concise,
    Expanded,
};

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
    CCMenuItemSpriteExtra* m_filterBtn;
    ModListDisplay m_display = ModListDisplay::Concise;
    EventListener<IndexUpdateFilter> m_indexListener;

    virtual ~ModListLayer();

    bool init() override;

    void onExit(CCObject*);
    void onReload(CCObject*);
    void onCheckForUpdates(CCObject*);
    void onOpenFolder(CCObject*);
    void onResetSearch(CCObject*);
    void onExpand(CCObject*);
    void onTab(CCObject*);
    void onFilters(CCObject*);
    void keyDown(enumKeyCodes) override;
    void textChanged(CCTextInputNode*) override;
    void createSearchControl();
    void onIndexUpdate(IndexUpdateEvent* event);

    CCArray* createModCells(ModListType type);
    CCSize getCellSize() const;
    CCSize getListSize() const;

    friend class SearchFilterPopup;

public:
    static ModListLayer* create();
    static ModListLayer* scene();
    void updateAllStates(ModListCell* except = nullptr);

    ModListDisplay getDisplay() const;

    void reloadList();
};
