#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include "../sources/ModSource.hpp"
#include "../GeodeStyle.hpp"
#include "../UpdateModListState.hpp"
#include <server/DownloadManager.hpp>

using namespace geode::prelude;

class ModPopup : public GeodePopup<ModSource&&> {
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
    CCScale9Sprite* m_installBG;
    CCScale9Sprite* m_settingsBG;
    CCLabelBMFont* m_enabledStatusLabel;
    ButtonSprite* m_restartRequiredLabel;
    CCNode* m_rightColumn;
    CCNode* m_currentTabPage = nullptr;
    CCNode* m_modtoberBanner = nullptr;
    CCMenu* m_titleContainer = nullptr;
    std::unordered_map<Tab, std::pair<GeodeTabSprite*, Ref<CCNode>>> m_tabs;
    EventListener<server::ServerRequest<server::ServerModMetadata>> m_statsListener;
    EventListener<server::ServerRequest<std::vector<server::ServerTag>>> m_tagsListener;
    EventListener<server::ServerRequest<std::optional<server::ServerModUpdate>>> m_checkUpdateListener;
    EventListener<UpdateModListStateFilter> m_updateStateListener;
    EventListener<server::ModDownloadFilter> m_downloadListener;
    EventListener<EventFilter<SettingNodeValueChangeEvent>> m_settingNodeListener;

    bool setup(ModSource&& src) override;
    void updateState();

    void setStatIcon(CCNode* stat, const char* spr);
    void setStatLabel(CCNode* stat, std::string const& value, bool noValue = false, ccColor3B color = ccWHITE);
    void setStatValue(CCNode* stat, std::optional<std::string> const& value);

    void onLoadServerInfo(typename server::ServerRequest<server::ServerModMetadata>::Event* event);
    void onLoadTags(typename server::ServerRequest<std::vector<server::ServerTag>>::Event* event);
    void onCheckUpdates(typename server::ServerRequest<std::optional<server::ServerModUpdate>>::Event* event);

    void onTab(CCObject* sender);
    void onEnable(CCObject*);
    void onInstall(CCObject*);
    void onUninstall(CCObject*);
    void onCancelDownload(CCObject*);

    void onSettings(CCObject*);
    void onLink(CCObject*);
    void onSupport(CCObject*);
    void onModtoberInfo(CCObject*);

    bool availableForInstall() const;

public:
    void loadTab(Tab tab);
    static ModPopup* create(ModSource&& src);

    ModSource& getSource() &;
};
