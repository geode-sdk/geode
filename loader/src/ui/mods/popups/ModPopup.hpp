#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/ui/NineSlice.hpp>
#include "../sources/ModSource.hpp"
#include "../GeodeStyle.hpp"
#include "../UpdateModListState.hpp"
#include <server/DownloadManager.hpp>

using namespace geode::prelude;

class ModPopup : public GeodePopup {
public:
    enum class Tab {
        Details,
        Changelog,
        Versions,
    };

protected:
    ModSource m_source;
    CCNode* m_stats;
    CCNode* m_tags;
    CCMenu* m_installMenu;
    CCMenuItemToggler* m_enableBtn;
    CCMenuItemToggler* m_reenableBtn;
    CCMenuItemSpriteExtra* m_uninstallBtn;
    CCMenuItemSpriteExtra* m_installBtn;
    CCMenuItemSpriteExtra* m_unavailableBtn;
    CCMenuItemSpriteExtra* m_updateBtn;
    CCMenuItemSpriteExtra* m_cancelBtn;
    CCLabelBMFont* m_installStatusLabel;
    NineSlice* m_installBG;
    NineSlice* m_settingsBG;
    CCLabelBMFont* m_enabledStatusLabel;
    ButtonSprite* m_restartRequiredLabel;
    CCNode* m_rightColumn;
    CCNode* m_currentTabPage = nullptr;
    CCNode* m_modtoberBanner = nullptr;
    CCMenu* m_titleContainer = nullptr;
    std::unordered_map<Tab, std::pair<GeodeTabSprite*, Ref<CCNode>>> m_tabs;
    ListenerHandle m_statsHandle;
    ListenerHandle m_tagsHandle;
    ListenerHandle m_checkUpdateHandle;
    ListenerHandle m_updateStateHandle;
    ListenerHandle m_downloadHandle;
    ListenerHandle m_settingNodeHandle;
    async::TaskHolder<server::ServerResult<server::ServerModMetadata>> m_statsListener;
    async::TaskHolder<server::ServerResult<std::vector<server::ServerTag>>> m_tagsListener;
    async::TaskHolder<server::ServerResult<std::optional<VersionInfo>>> m_checkUpdateListener;
    ListenerHandle m_downloadListener;

    bool init(ModSource&& src);
    void updateState();

    void setStatIcon(CCNode* stat, const char* spr);
    void setStatLabel(CCNode* stat, ZStringView value, bool noValue = false, ccColor3B color = ccWHITE);
    void setStatValue(CCNode* stat, std::optional<std::string> const& value);

    void onLoadServerInfo(server::ServerResult<server::ServerModMetadata> result);
    void onLoadTags(server::ServerResult<std::vector<server::ServerTag>> result);
    void onCheckUpdates(server::ServerResult<std::optional<VersionInfo>> result);

    void onTab(CCObject* sender);
    void onEnable(CCObject*);
    void onInstall(CCObject*);
    void onUninstall(CCObject*);
    void onCancelDownload(CCObject*);

    void onSettings(CCObject*);
    void onLink(CCObject*);
    void onSupport(CCObject*);
    void onModtober24Info(CCObject*);
    void onModtober25Info(CCObject*);

    bool availableForInstall() const;

public:
    void loadTab(Tab tab);
    static ModPopup* create(ModSource&& src);

    ModSource& getSource() &;
};
