#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include "../sources/ModSource.hpp"
#include "../GeodeStyle.hpp"
#include "../UpdateModListState.hpp"

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
    CCLabelBMFont* m_installStatusLabel;
    CCScale9Sprite* m_installBG;
    ButtonSprite* m_restartRequiredLabel;
    CCNode* m_rightColumn;
    CCNode* m_currentTabPage = nullptr;
    std::unordered_map<Tab, std::pair<GeodeTabSprite*, Ref<CCNode>>> m_tabs;
    EventListener<PromiseEventFilter<server::ServerModMetadata, server::ServerError>> m_statsListener;
    EventListener<PromiseEventFilter<std::unordered_set<std::string>, server::ServerError>> m_tagsListener;
    EventListener<PromiseEventFilter<std::optional<server::ServerModUpdate>, server::ServerError>> m_checkUpdateListener;
    EventListener<UpdateModListStateFilter> m_updateStateListener;

    bool setup(ModSource&& src) override;
    void updateState();

    void setStatIcon(CCNode* stat, const char* spr);
    void setStatLabel(CCNode* stat, std::string const& value, bool noValue = false, ccColor3B color = ccWHITE);
    void setStatValue(CCNode* stat, std::optional<std::string> const& value);

    void onCheckUpdates(PromiseEvent<std::optional<server::ServerModUpdate>, server::ServerError>* event);
    void onLoadServerInfo(PromiseEvent<server::ServerModMetadata, server::ServerError>* event);
    void onLoadTags(PromiseEvent<std::unordered_set<std::string>, server::ServerError>* event);
    
    void loadTab(Tab tab);
    void onTab(CCObject* sender);
    void onEnable(CCObject*);
    void onUninstall(CCObject*);

    void onLink(CCObject*);
    void onSupport(CCObject*);

public:
    static ModPopup* create(ModSource&& src);
};
