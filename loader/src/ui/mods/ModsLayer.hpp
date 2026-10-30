#pragma once

#include "GeodeStyle.hpp"
#include <Geode/Result.hpp>
#include <Geode/ui/General.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/TextArea.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/cocos/cocoa/CCObject.h>
#include "list/ModItem.hpp"
#include "list/ModList.hpp"
#include "sources/ModListSource.hpp"
#include "UpdateModListState.hpp"
#include <server/DownloadManager.hpp>
#include <Geode/loader/Setting.hpp>

using namespace geode::prelude;

class ModsSetPagePopup : public GeodePopup {
protected:
    using Callback = Function<void(size_t)>;

    Callback m_callback = nullptr;

    std::string offsetPageStr(ZStringView current, int64_t by, size_t end);

    bool init(size_t current, size_t begin, size_t end);

public:
    static ModsSetPagePopup* create(size_t current, size_t begin, size_t end);

    void setCallback(Callback cb);
};

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
    Label* m_status;
    Label* m_statusPercentage;
    Slider* m_progressBar;
    CCNode* m_loadingCircle;
    CCNode* m_btnMenu;
    Button* m_viewBtn;
    Button* m_cancelBtn;
    Button* m_restartBtn;
    ListenerHandle m_updateStateHandle;
    ListenerHandle m_downloadHandle;
    DownloadState m_lastState = DownloadState::None;
    ListenerHandle m_settingNodeHandle;

    bool init();
    void updateState();

public:
    static ModsStatusNode* create();
};

class ModsLayer : public CCLayer {
protected:
    CCNode* m_frame;
    std::vector<Ref<Button>> m_tabs;
    ModListSource* m_currentSource = nullptr;
    std::unordered_map<ModListSource*, Ref<ModList>> m_lists;
    CCNode* m_pageMenu;
    Label* m_pageLabel;
    Button* m_goToPageBtn;
    ModsStatusNode* m_statusNode;
    ListenerHandle m_updateStateHandle;
    bool m_showSearch = true;
    std::vector<Ref<Button>> m_displayBtns;
    ModListDisplay m_modListDisplay;

    bool init();

    void keyDown(enumKeyCodes key, double) override;
    void keyBackClicked() override;

    void onDisplay(ModListDisplay display);
    void onBack();

    void updateState();

public:
    static ModsLayer* create();
    static ModsLayer* scene();

    static server::ServerFuture<InstalledModsUpdateCheck> checkInstalledModsForUpdates();
    static void installModFromFile();

    void gotoTab(ModListSource* src, bool searchingDev = false);
    void refreshList();
};
