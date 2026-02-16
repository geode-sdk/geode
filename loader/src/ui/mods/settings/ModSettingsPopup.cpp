#include "ModSettingsPopup.hpp"
#include <Geode/loader/ModSettingsManager.hpp>

bool ModSettingsPopup::init(Mod* mod, bool forceDisableTheme) {
    if (!BaseSettingsPopup::init(forceDisableTheme))
        return false;
    
    this->setTitle(fmt::format("Settings for {}", mod->getName()));

    m_mod = mod;
    for (auto key : mod->getSettingKeys()) {
        if (auto sett = mod->getSetting(key)) {
            m_settings.push_back(sett->createNode(m_layerSize.width));
        }
        else {
            m_settings.push_back(UnresolvedCustomSettingNode::create(key, mod, m_layerSize.width));
        }
    }

    auto foldersMenu = CCMenu::create();
    foldersMenu->setContentSize({ 0, 30 });
    foldersMenu->setAnchorPoint(ccp(1, 0));
    foldersMenu->setLayout(RowLayout::create()
        ->setAxisReverse(true)
        ->setGap(5)
        ->setAutoGrowAxis(30)
        ->setGrowCrossAxis(true));
    m_buttonMenu->addChildAtPosition(foldersMenu, Anchor::BottomRight, ccp(-5, 10));

    auto createFolderButton = [&, this](auto subSprName, auto callback) {
        auto folderSpr = CCSprite::createWithSpriteFrameName("folderIcon_001.png");
        auto folderSprSub = CCSprite::createWithSpriteFrameName(subSprName);
        folderSprSub->setColor(ccBLACK);
        folderSprSub->setOpacity(155);
        folderSprSub->setScale(.55f);
        folderSpr->addChildAtPosition(folderSprSub, Anchor::Center, ccp(0, -3));
        auto buttonSpr = createGeodeButton(folderSpr, "", GeodeButtonSprite::Default, m_forceDisableTheme);
        buttonSpr->setScale(.6f);
        buttonSpr->getIcon()->setScale(buttonSpr->getIcon()->getScale() * 1.4f);
        auto folderBtn = CCMenuItemSpriteExtra::create(
            buttonSpr, this, static_cast<SEL_MenuHandler>(callback)
        );
        foldersMenu->addChild(folderBtn);
        return folderBtn;
    };

    createFolderButton("save.png"_spr, &ModSettingsPopup::onOpenSaveDirectory);
    createFolderButton("settings.png"_spr, &ModSettingsPopup::onOpenConfigDirectory);
    auto persistentBtn = createFolderButton("persistent.png"_spr, &ModSettingsPopup::onOpenPersistentDirectory);
    if (!std::filesystem::exists(mod->getPersistentDir(false))) {
        persistentBtn->setEnabled(false);
        auto spr = static_cast<CCSprite*>(persistentBtn->getNormalImage());
        spr->setCascadeColorEnabled(true);
        spr->setCascadeOpacityEnabled(true);
        spr->setColor(ccGRAY);
        spr->setOpacity(155);
    }

    foldersMenu->updateLayout();

    this->updateState();

    return true;
}

void ModSettingsPopup::updateState(SettingNode* invoker) {
    BaseSettingsPopup::updateState(invoker);
    m_restartBtn->setVisible(ModSettingsManager::from(m_mod)->restartRequired());
}

void ModSettingsPopup::onOpenSaveDirectory(CCObject*) {
    file::openFolder(m_mod->getSaveDir());
}
void ModSettingsPopup::onOpenConfigDirectory(CCObject*) {
    file::openFolder(m_mod->getConfigDir());
}
void ModSettingsPopup::onOpenPersistentDirectory(CCObject*) {
    file::openFolder(m_mod->getPersistentDir());
}

ModSettingsPopup* ModSettingsPopup::create(Mod* mod, bool forceDisableTheme) {
    auto ret = new ModSettingsPopup();
    if (ret->init(mod, forceDisableTheme)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
