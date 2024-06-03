#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextArea.hpp>
#include "list/ModItem.hpp"
#include "list/ModList.hpp"
#include "sources/ModListSource.hpp"
#include "UpdateModListState.hpp"
#include <server/DownloadManager.hpp>

using namespace geode::prelude;

class ModsStatusNode : public CCNode {
protected:
    CCScale9Sprite* m_statusBG;
    CCLabelBMFont* m_status;
    CCLabelBMFont* m_statusPercentage;
    Slider* m_progressBar;
    CCNode* m_loadingCircle;
    CCMenu* m_btnMenu;
    CCMenuItemSpriteExtra* m_viewBtn;
    CCMenuItemSpriteExtra* m_cancelBtn;
    CCMenuItemSpriteExtra* m_restartBtn;
    EventListener<UpdateModListStateFilter> m_updateStateListener;
    EventListener<server::ModDownloadFilter> m_downloadListener;
    
    bool init();
    void updateState();

    void onRestart(CCObject*);
    void onViewErrors(CCObject*);
    void onConfirm(CCObject*);
    void onCancel(CCObject*);

public:
    static ModsStatusNode* create();
};

class ModsLayer : public CCLayer, public SetTextPopupDelegate {
protected:
    CCNode* m_frame;
    std::vector<CCMenuItemSpriteExtra*> m_tabs;
    ModListSource* m_currentSource = nullptr;
    std::unordered_map<ModListSource*, Ref<ModList>> m_lists;
    CCMenu* m_pageMenu;
    CCLabelBMFont* m_pageLabel;
    CCMenuItemSpriteExtra* m_goToPageBtn;
    ModsStatusNode* m_statusNode;
    EventListener<UpdateModListStateFilter> m_updateStateListener;
    bool m_showSearch = false;
    bool m_bigView = false;

    bool init();

    void keyBackClicked() override;
    void setTextPopupClosed(SetTextPopup*, gd::string value) override;
    
    void onTab(CCObject* sender);
    void onBigView(CCObject*);
    void onSearch(CCObject*);
    void onGoToPage(CCObject*);
    void onRefreshList(CCObject*);
    void onTheme(CCObject*);
    void onSettings(CCObject*);
    void onBack(CCObject*);

    void updateState();

public:
    static ModsLayer* create();
    static ModsLayer* scene();

    static server::ServerRequest<std::vector<std::string>> checkInstalledModsForUpdates();

    void gotoTab(ModListSource* src);
};
