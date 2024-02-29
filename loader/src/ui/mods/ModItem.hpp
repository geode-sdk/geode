#pragma once

#include <Geode/ui/General.hpp>
#include <server/Server.hpp>

using namespace geode::prelude;

class BaseModItem : public CCNode {
protected:
    CCScale9Sprite* m_bg;
    ccColor4B m_defaultBG = { 255, 255, 255, 25 };
    CCNode* m_logo;
    CCNode* m_infoContainer;
    CCNode* m_titleContainer;
    CCLabelBMFont* m_titleLabel;
    CCNode* m_developers;
    CCLabelBMFont* m_developerLabel;
    ButtonSprite* m_restartRequiredLabel = nullptr;
    CCMenu* m_viewMenu;
    MiniFunction<void()> m_updateParentState = nullptr;

    /**
     * @warning Make sure `getMetadata` and `createModLogo` are callable 
     * before calling `init`!
    */
    bool init();

    // This should never be exposed outside, so the parent can't call this and 
    // cause an infinite loop during state updating
    virtual void updateState();

public:
    virtual ModMetadata getMetadata() const = 0;
    virtual CCNode* createModLogo() const = 0;
    virtual bool wantsRestart() const = 0;

    virtual void updateSize(float width, bool big);

    void onUpdateParentState(MiniFunction<void()> listener);
};

class InstalledModItem : public BaseModItem {
protected:
    Mod* m_mod;
    CCMenuItemToggler* m_enableToggle = nullptr;

    bool init(Mod* mod);

    void onEnable(CCObject*);

    void updateState() override;

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

    void updateState() override;

public:
    /**
     * @note Make sure to call `updateSize` afterwards
     */
    static ServerModItem* create(server::ServerModMetadata const& metadata);

    ModMetadata getMetadata() const override;
    CCNode* createModLogo() const override;
    bool wantsRestart() const override;
};
