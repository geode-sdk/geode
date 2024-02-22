#pragma once

#include <Geode/ui/General.hpp>

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
