#pragma once

#include <Geode/binding/TableViewCell.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/ModInfo.hpp>
#include <Geode/loader/Index.hpp>

USE_GEODE_NAMESPACE();

class ModListView;
enum class ModListDisplay;

class ModListCell : public TableViewCell {
protected:
    ModListView* m_list;
    CCMenu* m_menu;
    CCMenuItemToggler* m_enableToggle = nullptr;
    CCMenuItemSpriteExtra* m_unresolvedExMark;
    ModListDisplay m_display;

    ModListCell(char const* name, CCSize const& size);
    bool init(ModListView* list, ModListDisplay display);
    void setupInfo(ModInfo const& info, bool spaceForCategories);
    void draw() override;

public:
    void updateBGColor(int index);
    virtual void updateState() = 0;
    virtual CCNode* createLogo(CCSize const& size) = 0;
};

class ModCell : public ModListCell {
protected:
    Mod* m_mod;

    ModCell(char const* name, CCSize const& size);

    void onInfo(CCObject*);
    void onEnable(CCObject*);
    void onUnresolvedInfo(CCObject*);

public:
    static ModCell* create(
        ModListView* list, ModListDisplay display,
        const char* key, CCSize const& size
    );

    void loadFromMod(Mod* mod);
    void updateState() override;
    CCNode* createLogo(CCSize const& size) override;
};

class IndexItemCell : public ModListCell {
protected:
    IndexItemHandle m_item;

    IndexItemCell(char const* name, CCSize const& size);

    void onInfo(CCObject*);

public:
    static IndexItemCell* create(
        ModListView* list, ModListDisplay display,
        const char* key, CCSize const& size
    );

    void loadFromItem(IndexItemHandle item);
    void updateState() override;
    CCNode* createLogo(CCSize const& size) override;
};

class InvalidGeodeFileCell : public ModListCell, public FLAlertLayerProtocol {
protected:
    InvalidGeodeFile m_info;

    InvalidGeodeFileCell(char const* name, CCSize const& size);

    void onInfo(CCObject*);
    void FLAlert_Clicked(FLAlertLayer*, bool btn2) override;

public:
    static InvalidGeodeFileCell* create(
        ModListView* list, ModListDisplay display,
        const char* key, CCSize const& size
    );

    void loadFromInfo(InvalidGeodeFile const& file);
    void updateState() override;
    CCNode* createLogo(CCSize const& size) override;
};
