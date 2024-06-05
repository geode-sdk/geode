#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include "../sources/ModSource.hpp"
#include "../GeodeStyle.hpp"
#include "../UpdateModListState.hpp"
#include <server/DownloadManager.hpp>

using namespace geode::prelude;

class ModPopup : public GeodePopup<ModSource&&> {
protected:
    enum class Tab {
        Details,
        Changelog,
        Versions,
    };

    ModSource m_source;
    CCNode* m_stats;
    CCNode* m_tags;
    CCMenu* m_installMenu;
    CCMenuItemToggler* m_enableBtn;
    CCMenuItemToggler* m_reenableBtn;
    CCMenuItemSpriteExtra* m_uninstallBtn;
    CCMenuItemSpriteExtra* m_installBtn;
    CCMenuItemSpriteExtra* m_updateBtn;
    CCMenuItemSpriteExtra* m_cancelBtn;
    CCLabelBMFont* m_installStatusLabel;
    CCScale9Sprite* m_installBG;
    CCLabelBMFont* m_enabledStatusLabel;
    ButtonSprite* m_restartRequiredLabel;
    CCNode* m_rightColumn;
    CCNode* m_currentTabPage = nullptr;
    std::unordered_map<Tab, std::pair<GeodeTabSprite*, Ref<CCNode>>> m_tabs;
    EventListener<server::ServerRequest<server::ServerModMetadata>> m_statsListener;
    EventListener<server::ServerRequest<std::unordered_set<std::string>>> m_tagsListener;
    EventListener<server::ServerRequest<std::optional<server::ServerModUpdate>>> m_checkUpdateListener;
    EventListener<UpdateModListStateFilter> m_updateStateListener;
    EventListener<server::ModDownloadFilter> m_downloadListener;

    bool setup(ModSource&& src) override;
    void updateState();

    void setStatIcon(CCNode* stat, const char* spr);
    void setStatLabel(CCNode* stat, std::string const& value, bool noValue = false, ccColor3B color = ccWHITE);
    void setStatValue(CCNode* stat, std::optional<std::string> const& value);

    void onLoadServerInfo(typename server::ServerRequest<server::ServerModMetadata>::Event* event);
    void onLoadTags(typename server::ServerRequest<std::unordered_set<std::string>>::Event* event);
    void onCheckUpdates(typename server::ServerRequest<std::optional<server::ServerModUpdate>>::Event* event);
    
    void loadTab(Tab tab);
    void onTab(CCObject* sender);
    void onEnable(CCObject*);
    void onInstall(CCObject*);
    void onUninstall(CCObject*);
    void onCancelDownload(CCObject*);

    void onSettings(CCObject*);
    void onLink(CCObject*);
    void onSupport(CCObject*);

public:
    static ModPopup* create(ModSource&& src);
};
