#pragma once

#include <Geode/Geode.hpp>
#include <Index.hpp>

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

class ModInfoLayer :
    public FLAlertLayer,
    public FLAlertLayerProtocol
{
protected:
    Mod* m_mod = nullptr;
    ModInfo m_info;
    bool m_isIndexMod = false;
    ModListView* m_list = nullptr;
    DownloadStatusNode* m_installStatus = nullptr;
    IconButtonSprite* m_installBtnSpr;
    CCMenuItemSpriteExtra* m_installBtn;
    CCLabelBMFont* m_updateVersionLabel = nullptr;
    InstallTicket* m_ticket = nullptr;

    void onHooks(CCObject*);
    void onSettings(CCObject*);
    void onNoSettings(CCObject*);
    void onInfo(CCObject*);
    void onEnableMod(CCObject*);
    void onInstallMod(CCObject*);
    void onCancelInstall(CCObject*);
    void onUninstall(CCObject*);
    void onDisablingNotSupported(CCObject*);
    void install();
    void uninstall();
    void updateInstallStatus(std::string const& status, uint8_t progress);

    void modInstallProgress(
        InstallTicket*,
        UpdateStatus status,
        std::string const& info,
        uint8_t percentage
    );
    void FLAlert_Clicked(FLAlertLayer*, bool) override;

    bool init(ModObject* obj, ModListView* list);

    void keyDown(cocos2d::enumKeyCodes) override;
    void onClose(cocos2d::CCObject*);
    
public:
    static ModInfoLayer* create(Mod* mod, ModListView* list);
    static ModInfoLayer* create(ModObject* obj, ModListView* list);

    static CCNode* createLogoSpr(ModObject* modObj);
    static CCNode* createLogoSpr(Mod* mod);
    static CCNode* createLogoSpr(IndexItem const& item);
};
