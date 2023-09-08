#pragma once

#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/ui/Scrollbar.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/loader/Index.hpp>

using namespace geode::prelude;

class ModListLayer;
class ModObject;

class DownloadStatusNode : public CCNode {
protected:
    Slider* m_bar;
    CCLabelBMFont* m_label;

    bool init();

public:
    static DownloadStatusNode* create();

    void setProgress(uint8_t progress);
    void setStatus(std::string const& text);
};

class ModInfoPopup : public FLAlertLayer {
protected:
    ModListLayer* m_layer = nullptr;
    DownloadStatusNode* m_installStatus = nullptr;
    IconButtonSprite* m_installBtnSpr;
    CCMenuItemSpriteExtra* m_installBtn;
    CCMenuItemSpriteExtra* m_infoBtn;
    CCLabelBMFont* m_latestVersionLabel = nullptr;
    CCLabelBMFont* m_minorVersionLabel = nullptr;
    MDTextArea* m_detailsArea;
    MDTextArea* m_changelogArea = nullptr;
    Scrollbar* m_scrollbar;
    IndexItemHandle m_item;

    void onChangelog(CCObject*);
    void onRepository(CCObject*);
    void onSupport(CCObject*);
    void onInfo(CCObject*);

    bool init(ModMetadata const& metadata, ModListLayer* list);

    void keyDown(cocos2d::enumKeyCodes) override;
    void onClose(cocos2d::CCObject*);

    void setInstallStatus(std::optional<UpdateProgress> const& progress);

    void popupInstallItem(IndexItemHandle item);
    void preInstall();
    void onCancelInstall(CCObject*);

    virtual CCNode* createLogo(CCSize const& size) = 0;
    virtual ModMetadata getMetadata() const = 0;
};

class LocalModInfoPopup : public ModInfoPopup, public FLAlertLayerProtocol {
protected:
    EventListener<ModInstallFilter> m_installListener;
    Mod* m_mod;

    bool init(Mod* mod, ModListLayer* list);

    void onIssues(CCObject*);
    void onSettings(CCObject*);
    void onNoSettings(CCObject*);
    void onDisablingNotSupported(CCObject*);
    void onEnableMod(CCObject*);
    void onUninstall(CCObject*);
    void onOpenConfigDir(CCObject*);
    void doUninstall();

    void onUpdateProgress(ModInstallEvent* event);
    void onUpdate(CCObject*);


    void FLAlert_Clicked(FLAlertLayer*, bool) override;

    CCNode* createLogo(CCSize const& size) override;
    ModMetadata getMetadata() const override;

    LocalModInfoPopup();

public:
    static LocalModInfoPopup* create(Mod* mod, ModListLayer* list);
};

class IndexItemInfoPopup : public ModInfoPopup {
protected:
    EventListener<ModInstallFilter> m_installListener;

    bool init(IndexItemHandle item, ModListLayer* list);

    void onInstallProgress(ModInstallEvent* event);
    void onInstall(CCObject*);

    CCNode* createLogo(CCSize const& size) override;
    ModMetadata getMetadata() const override;

    IndexItemInfoPopup();

public:
    static IndexItemInfoPopup* create(IndexItemHandle item, ModListLayer* list);
};
