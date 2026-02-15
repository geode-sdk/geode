#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextArea.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/binding/SetTextPopupDelegate.hpp>
#include <Geode/binding/SetIDPopupDelegate.hpp>
#include <Geode/cocos/cocoa/CCObject.h>
#include "list/ModItem.hpp"
#include "list/ModList.hpp"
#include "sources/ModListSource.hpp"
#include "UpdateModListState.hpp"
#include <server/DownloadManager.hpp>
#include <Geode/loader/Setting.hpp>

using namespace geode::prelude;

class ModsStatusNode : public CCNode {
protected:
    enum class DownloadState {
        None,
        SomeCancelled,
        AllDone,
        SomeErrored,
        SomeToBeConfirmed,
        SomeFetching,
        SomeDownloading,
    };

    NineSlice* m_statusBG;
    CCLabelBMFont* m_status;
    CCLabelBMFont* m_statusPercentage;
    Slider* m_progressBar;
    CCNode* m_loadingCircle;
    CCMenu* m_btnMenu;
    CCMenuItemSpriteExtra* m_viewBtn;
    CCMenuItemSpriteExtra* m_cancelBtn;
    CCMenuItemSpriteExtra* m_restartBtn;
    ListenerHandle m_updateStateHandle;
    ListenerHandle m_downloadHandle;
    DownloadState m_lastState = DownloadState::None;
    ListenerHandle m_settingNodeHandle;

    bool init();
    void updateState();

    void onRestart(CCObject*);
    void onViewErrors(CCObject*);
    void onConfirm(CCObject*);
    void onCancel(CCObject*);

public:
    static ModsStatusNode* create();
};

class ModsLayer : public CCLayer, public SetIDPopupDelegate {
protected:
    CCNode* m_frame;
    std::vector<CCMenuItemSpriteExtra*> m_tabs;
    ModListSource* m_currentSource = nullptr;
    std::unordered_map<ModListSource*, Ref<ModList>> m_lists;
    CCMenu* m_pageMenu;
    CCLabelBMFont* m_pageLabel;
    CCMenuItemSpriteExtra* m_goToPageBtn;
    ModsStatusNode* m_statusNode;
    ListenerHandle m_updateStateHandle;
    bool m_showSearch = true;
    std::vector<CCMenuItemSpriteExtra*> m_displayBtns;
    ModListDisplay m_modListDisplay;

    bool init();

    void keyDown(enumKeyCodes key, double) override;
    void keyBackClicked() override;
    void setIDPopupClosed(SetIDPopup*, int value) override;

    void onTab(CCObject* sender);
    void onOpenModsFolder(CCObject*);
    void onAddModFromFile(CCObject*);
    void onRestartGD(CCObject*);
    void onDisplay(CCObject*);
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

    static server::ServerFuture<InstalledModsUpdateCheck> checkInstalledModsForUpdates();

    void gotoTab(ModListSource* src, bool searchingDev = false);
};
