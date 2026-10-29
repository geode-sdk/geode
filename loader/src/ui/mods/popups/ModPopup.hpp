#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/Label.hpp>
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
    Button* m_uninstallBtn;
    Button* m_installBtn;
    Button* m_unavailableBtn;
    Button* m_updateBtn;
    Button* m_cancelBtn;
    Label* m_installStatusLabel;
    NineSlice* m_installBG;
    NineSlice* m_settingsBG;
    Label* m_enabledStatusLabel;
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
    async::TaskHolder<server::ServerResult<server::ServerModUpdateOneCheck>> m_checkUpdateListener;
    ListenerHandle m_downloadListener;

    enum class RepoPlatform {
        Unknown,
        GitHub,
        GitLab,
        Codeberg,
        GitGay,
    };

    bool init(ModSource&& src);
    CCNode* createContainer(CCSize const& size);
    RepoPlatform getRepoPlatform(ZStringView url);
    ZStringView getPlatformSprite(std::optional<std::string> const& url);
    void updateState();

    void setStatIcon(CCNode* stat, ZStringView spr);
    void setStatLabel(CCNode* stat, std::string value, bool noValue = false, ccColor3B color = ccWHITE);
    void setStatValue(CCNode* stat, std::optional<std::string> value);

    void onLoadServerInfo(server::ServerResult<server::ServerModMetadata> result);
    void onLoadTags(server::ServerResult<std::vector<server::ServerTag>> result);
    void onCheckUpdates(server::ServerResult<server::ServerModUpdateOneCheck> result);

    void onEnable(CCObject*);
    void onInstall();

    bool availableForInstall() const;

public:
    void loadTab(Tab tab);
    static ModPopup* create(ModSource&& src);

    ModSource& getSource() &;
};