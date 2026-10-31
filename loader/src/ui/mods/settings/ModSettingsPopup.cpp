#include "ModSettingsPopup.hpp"
#include <Geode/loader/ModSettingsManager.hpp>

bool ModSettingsPopup::init(Mod* mod, bool forceDisableTheme) {
    if (!BaseSettingsPopup::init(forceDisableTheme))
        return false;

    this->setTitle(fmt::format("Settings for {}", mod->getName()));

    m_mod = mod;
    for (auto& key : mod->getSettingKeys()) {
        if (auto sett = mod->getSetting(key)) {
            m_settings.push_back(sett->createNode(m_listSize.width));
        }
        else {
            m_settings.push_back(UnresolvedCustomSettingNode::create(key, mod, m_listSize.width));
        }
    }

    auto foldersMenu = CCNode::create();
    foldersMenu->setID("folders-menu");
    foldersMenu->setContentSize({ .0f, 30.f });
    foldersMenu->setAnchorPoint({1.f, .0f});
    foldersMenu->setLayout(RowLayout::create()
        ->setGap(5.f)
        ->setDefaultScaleLimits(.1f, .6f)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAxisReverse(true)
        ->setAutoGrowAxis(30.f)
        ->setGrowCrossAxis(true)
    );
    m_mainLayer->addChildAtPosition(foldersMenu, Anchor::BottomRight, {-9.f, 10.f});

    auto createFolderButton = [this, foldersMenu](ZStringView subSprName, Button::ButtonCallback callback) {
        auto folderSpr = CCSprite::createWithSpriteFrameName("folderIcon_001.png");
        auto folderSprSub = CCSprite::createWithSpriteFrameName(subSprName.c_str());
        folderSprSub->setColor(ccBLACK);
        folderSprSub->setOpacity(155);
        folderSprSub->setScale(.55f);
        folderSpr->addChildAtPosition(folderSprSub, Anchor::Center, {.0f, -3.f});

        auto buttonSpr = createGeodeButton(folderSpr, "", GeodeButtonSprite::Default, m_forceDisableTheme);
        buttonSpr->getIcon()->setScale(buttonSpr->getIcon()->getScale() * 1.4f);

        auto folderBtn = Button::createWithNode(
            buttonSpr, std::move(callback)
        );
        foldersMenu->addChild(folderBtn);

        return folderBtn;
    };

    createFolderButton("save.png"_spr, [this](auto) { file::openFolder(m_mod->getSaveDir()); });
    createFolderButton("settings.png"_spr, [this](auto) { file::openFolder(m_mod->getConfigDir()); });
    auto persistentBtn = createFolderButton("GJ_lockGray_001.png", [this](auto) { file::openFolder(m_mod->getPersistentDir()); });
    if (mod->getPersistentDir(false).exists()) {
        persistentBtn->setEnabled(false);
        auto spr = static_cast<CCSprite*>(persistentBtn->getDisplayNode());
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
    BaseSettingsPopup::updateState();

    auto manager = ModSettingsManager::from(m_mod);
    m_restartBtn->setVisible(manager->restartRequired());

    // frame delay for debounce (avoids repeating save for every changed setting)
    Loader::get()->queueInMainThread([mod = m_mod, manager] {
        if (manager->shouldSave()) {
            (void) mod->saveData();
        }
    });
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
