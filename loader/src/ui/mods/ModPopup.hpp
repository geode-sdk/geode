#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include "ModSource.hpp"
#include "GeodeStyle.hpp"

using namespace geode::prelude;

class ModPopup : public Popup<ModSource&&> {
protected:
    enum class Tab {
        Details,
        Changelog,
        Versions,
    };

    ModSource m_source;
    CCNode* m_stats;
    CCNode* m_rightColumn;
    CCNode* m_currentTabPage = nullptr;
    std::unordered_map<Tab, std::pair<GeodeTabSprite*, Ref<CCNode>>> m_tabs;
    EventListener<PromiseEventFilter<server::ServerModMetadata, server::ServerError>> m_statsListener;

    bool setup(ModSource&& src) override;

    void setStatIcon(CCNode* stat, const char* spr);
    void setStatLabel(CCNode* stat, std::string const& value, bool noValue = false, ccColor3B color = ccWHITE);
    void setStatValue(CCNode* stat, std::optional<std::string> const& value);

    void onLoadServerInfo(PromiseEvent<server::ServerModMetadata, server::ServerError>* event);
    void loadTab(Tab tab);
    void onTab(CCObject* sender);

public:
    static ModPopup* create(ModSource&& src);
};
