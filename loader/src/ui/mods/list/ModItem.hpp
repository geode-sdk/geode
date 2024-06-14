#pragma once

#include <Geode/ui/General.hpp>
#include <server/Server.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <server/DownloadManager.hpp>
#include "../sources/ModSource.hpp"
#include "../UpdateModListState.hpp"

using namespace geode::prelude;

class ModItem : public CCNode {
protected:
    ModSource m_source;
    CCScale9Sprite* m_bg;
    CCNode* m_logo;
    CCNode* m_infoContainer;
    CCNode* m_titleContainer;
    CCLabelBMFont* m_titleLabel;
    CCLabelBMFont* m_versionLabel;
    CCNode* m_developers;
    CCNode* m_recommendedBy;
    CCLabelBMFont* m_developerLabel;
    ButtonSprite* m_restartRequiredLabel;
    CCNode* m_downloadWaiting;
    CCNode* m_downloadBarContainer;
    Slider* m_downloadBar;
    CCMenu* m_viewMenu;
    CCMenuItemToggler* m_enableToggle = nullptr;
    CCMenuItemSpriteExtra* m_updateBtn = nullptr;
    EventListener<UpdateModListStateFilter> m_updateStateListener;
    EventListener<server::ServerRequest<std::optional<server::ServerModUpdate>>> m_checkUpdateListener;
    EventListener<server::ModDownloadFilter> m_downloadListener;
    std::optional<server::ServerModUpdate> m_availableUpdate;

    /**
     * @warning Make sure `getMetadata` and `createModLogo` are callable 
     * before calling `init`!
    */
    bool init(ModSource&& source);

    void updateState();
    
    void onCheckUpdates(typename server::ServerRequest<std::optional<server::ServerModUpdate>>::Event* event);

    void onEnable(CCObject*);
    void onView(CCObject*);
    void onViewError(CCObject*);
    void onInstall(CCObject*);
    void onDevelopers(CCObject*);

public:
    static ModItem* create(ModSource&& source);

    void updateSize(float width, bool big);
};
