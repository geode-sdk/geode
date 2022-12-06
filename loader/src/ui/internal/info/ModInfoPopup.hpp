#pragma once

#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/ui/Scrollbar.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/loader/Index.hpp>

USE_GEODE_NAMESPACE();

class ModListView;
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
    ModListView* m_list = nullptr;
    DownloadStatusNode* m_installStatus = nullptr;
    IconButtonSprite* m_installBtnSpr;
    CCMenuItemSpriteExtra* m_installBtn;
    CCMenuItemSpriteExtra* m_infoBtn;
    CCLabelBMFont* m_updateVersionLabel = nullptr;
    MDTextArea* m_detailsArea;
    MDTextArea* m_changelogArea;
    Scrollbar* m_scrollbar;

    void onChangelog(CCObject*);
    void onRepository(CCObject*);
    void onSupport(CCObject*);
    void onInfo(CCObject*);

    bool init(ModInfo const& info, ModListView* list);

    void keyDown(cocos2d::enumKeyCodes) override;
    void onClose(cocos2d::CCObject*);

    virtual CCNode* createLogo(CCSize const& size) = 0;
    virtual ModInfo getModInfo() const = 0;
};

class LocalModInfoPopup : public ModInfoPopup, public FLAlertLayerProtocol {
protected:
    Mod* m_mod;

    bool init(Mod* mod, ModListView* list);
    
    void onIssues(CCObject*);
    void onSettings(CCObject*);
    void onNoSettings(CCObject*);
    void onDisablingNotSupported(CCObject*);
    void onEnableMod(CCObject*);
    void onUninstall(CCObject*);
    void onOpenConfigDir(CCObject*);
    void onAdvancedSettings(CCObject*);
    void uninstall();

    void FLAlert_Clicked(FLAlertLayer*, bool) override;

    CCNode* createLogo(CCSize const& size) override;
    ModInfo getModInfo() const override;

public:
    static LocalModInfoPopup* create(Mod* mod, ModListView* list);
};

class IndexItemInfoPopup : public ModInfoPopup {
protected:
    IndexItemHandle m_item;

    bool init(IndexItemHandle item, ModListView* list);
    
    CCNode* createLogo(CCSize const& size) override;
    ModInfo getModInfo() const override;

public:
    static IndexItemInfoPopup* create(IndexItemHandle item, ModListView* list);
};
