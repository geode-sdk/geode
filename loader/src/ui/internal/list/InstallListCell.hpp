#pragma once

#include <Geode/binding/TableViewCell.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/ModMetadata.hpp>
#include <Geode/loader/Index.hpp>

using namespace geode::prelude;

class InstallListPopup;

/**
 * Base class for install list items
 */
class InstallListCell : public CCLayer {
protected:
    float m_width;
    float m_height;
    InstallListPopup* m_layer;
    CCMenu* m_menu;
    CCMenuItemSpriteExtra* m_developerBtn;
    CCMenuItemToggler* m_toggle = nullptr;

    void setupInfo(
        std::string name,
        std::optional<std::string> developer,
        std::variant<VersionInfo, ComparableVersionInfo> version,
        bool inactive
    );

    bool init(InstallListPopup* list, CCSize const& size);
    void setupInfo(ModMetadata const& metadata, bool inactive);
    void draw() override;

    float getLogoSize() const;
    void onViewDev(CCObject*);

public:
    bool isIncluded();

    virtual CCNode* createLogo(CCSize const& size) = 0;
    [[nodiscard]] virtual std::string getID() const = 0;
    [[nodiscard]] virtual std::string getDeveloper() const = 0;
};

/**
 * Install list item for a mod
 */
class ModInstallListCell : public InstallListCell {
protected:
    Mod* m_mod;

    bool init(Mod* mod, InstallListPopup* list, CCSize const& size);

public:
    static ModInstallListCell* create(Mod* mod, InstallListPopup* list, CCSize const& size);

    CCNode* createLogo(CCSize const& size) override;
    [[nodiscard]] std::string getID() const override;
    [[nodiscard]] std::string getDeveloper() const override;
};

/**
 * Install list item for an index item
 */
class IndexItemInstallListCell : public InstallListCell {
protected:
    IndexItemHandle m_item;

    bool init(
        IndexItemHandle item,
        ModMetadata::Dependency::Importance importance,
        InstallListPopup* list,
        CCSize const& size,
        std::optional<bool> selected
    );

public:
    static IndexItemInstallListCell* create(
        IndexItemHandle item,
        ModMetadata::Dependency::Importance importance,
        InstallListPopup* list,
        CCSize const& size,
        std::optional<bool> selected
    );

    CCNode* createLogo(CCSize const& size) override;
    [[nodiscard]] std::string getID() const override;
    [[nodiscard]] std::string getDeveloper() const override;

    IndexItemHandle getItem();
};

/**
 * Install list item for an unknown item
 */
class UnknownInstallListCell : public InstallListCell {
protected:
    ModMetadata::Dependency m_dependency;

    bool init(ModMetadata::Dependency const& dependency, InstallListPopup* list, CCSize const& size);

public:
    static UnknownInstallListCell* create(
        ModMetadata::Dependency const& dependency,
        InstallListPopup* list,
        CCSize const& size
    );

    CCNode* createLogo(CCSize const& size) override;
    [[nodiscard]] std::string getID() const override;
    [[nodiscard]] std::string getDeveloper() const override;
};
