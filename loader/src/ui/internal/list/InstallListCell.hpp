#pragma once

#include <Geode/binding/TableViewCell.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/ModMetadata.hpp>
#include <Geode/loader/Index.hpp>

#include "../info/TagNode.hpp"

using namespace geode::prelude;

class InstallListPopup;

/**
 * Base class for install list items
 */
class InstallListCell : public CCLayer {
protected:
    float m_width;
    float m_height;
    InstallListPopup* m_layer = nullptr;
    CCMenu* m_menu = nullptr;
    CCLabelBMFont* m_creatorLabel = nullptr;
    CCLabelBMFont* m_titleLabel = nullptr;
    CCLabelBMFont* m_versionLabel = nullptr;
    TagNode* m_tagLabel = nullptr;
    CCMenuItemToggler* m_toggle = nullptr;
    bool m_inactive = false;

    void setupInfo(
        std::string name,
        std::optional<std::string> developer,
        std::variant<VersionInfo, ComparableVersionInfo> version,
        bool inactive
    );

    void setupVersion(std::variant<VersionInfo, ComparableVersionInfo> version);

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
    void setVersionFromItem(IndexItemHandle item);

    void onSelectVersion(CCObject*);
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

class SelectVersionPopup;
/**
 * Select version list item
 */
class SelectVersionCell : public InstallListCell {
protected:
    IndexItemHandle m_item;
    SelectVersionPopup* m_versionPopup;

    bool init(IndexItemHandle item, SelectVersionPopup* versionPopup, CCSize const& size);

    void onSelected(CCObject*);
public:
    static SelectVersionCell* create(IndexItemHandle item, SelectVersionPopup* versionPopup, CCSize const& size);

    CCNode* createLogo(CCSize const& size) override;
    [[nodiscard]] std::string getID() const override;
    [[nodiscard]] std::string getDeveloper() const override;
};