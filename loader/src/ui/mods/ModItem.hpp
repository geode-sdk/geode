#pragma once

#include <Geode/ui/General.hpp>
#include <server/Server.hpp>

using namespace geode::prelude;

class BaseModItem : public CCNode {
protected:
    CCNode* m_logo = nullptr;
    CCNode* m_title = nullptr;
    CCNode* m_developers = nullptr;
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

    bool init(Mod* mod);

public:
    /**
     * @note Make sure to call `updateSize` afterwards
     */
    static InstalledModItem* create(Mod* mod);

    ModMetadata getMetadata() const override;
    CCNode* createModLogo() const override;
    bool wantsRestart() const override;
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

    void updateSize(float width, bool big) override;
};
