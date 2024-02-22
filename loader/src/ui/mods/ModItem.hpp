#pragma once

#include <Geode/ui/General.hpp>

using namespace geode::prelude;

class BaseModItem : public CCNode {
protected:
    CCNode* m_logo = nullptr;
    CCNode* m_title = nullptr;
    CCNode* m_developers = nullptr;

    bool init();

    void setupCommonInfo();

public:
    virtual ModMetadata getMetadata() const = 0;
    virtual CCNode* createModLogo() const = 0;

    virtual void updateSize(float width, bool big);
};

class InstalledModItem : public BaseModItem {
protected:
    Mod* m_mod;

    bool init(Mod* mod);

public:
    static InstalledModItem* create(Mod* mod);

    ModMetadata getMetadata() const override;
    CCNode* createModLogo() const override;
};
