#pragma once

#include <Geode/ui/General.hpp>
#include <server/Server.hpp>

using namespace geode::prelude;

class BaseModItem : public CCNode {
protected:
    CCNode* m_logo;
    CCNode* m_infoContainer;
    CCNode* m_titleContainer;
    CCNode* m_developers;
    ButtonSprite* m_restartRequiredLabel = nullptr;
    CCMenu* m_viewMenu;

    /**
     * @warning Make sure `getMetadata` and `createModLogo` are callable 
     * before calling `init`!
    */
    bool init();

public:
    virtual ModMetadata getMetadata() const = 0;
    virtual CCNode* createModLogo() const = 0;
    virtual bool wantsRestart() const = 0;

    virtual void updateSize(float width, bool big);
    virtual void updateState();
};

class InstalledModItem : public BaseModItem {
protected:
    Mod* m_mod;
    CCMenuItemToggler* m_enableToggle = nullptr;

    bool init(Mod* mod);

    void onEnable(CCObject*);

public:
    /**
     * @note Make sure to call `updateSize` afterwards
     */
    static InstalledModItem* create(Mod* mod);

    ModMetadata getMetadata() const override;
    CCNode* createModLogo() const override;
    bool wantsRestart() const override;

    void updateState() override;
};

class ServerModItem : public BaseModItem {
protected:
    server::ServerModMetadata m_metadata;
    CCScale9Sprite* m_checkmark = nullptr;

    bool init(server::ServerModMetadata const& metadata);

public:
    /**
     * @note Make sure to call `updateSize` afterwards
     */
    static ServerModItem* create(server::ServerModMetadata const& metadata);

    ModMetadata getMetadata() const override;
    CCNode* createModLogo() const override;
    bool wantsRestart() const override;
};
