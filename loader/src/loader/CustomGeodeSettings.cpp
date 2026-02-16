#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <ui/mods/GeodeStyle.hpp>
#include <ui/mods/ModsLayer.hpp>

using namespace geode::prelude;

class GeodeSettingsInfoSetting : public SettingV3 {
public:
    static Result<std::shared_ptr<SettingV3>> parse(std::string key, std::string modID, matjson::Value const& json) {
        auto res = std::make_shared<GeodeSettingsInfoSetting>();
        auto root = checkJson(json, "GeodeSettingsInfoSetting");
        res->init(std::move(key), std::move(modID), root);
        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }

    bool load(matjson::Value const& json) override {
        return true;
    }
    bool save(matjson::Value& json) const override {
        return true;
    }

    bool isDefaultValue() const override {
        return true;
    }
    void reset() override {}

    SettingNodeV3* createNode(float width) override;
};

class GeodeSettingsInfoSettingNode : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<GeodeSettingsInfoSetting> setting, float width) {
        if (!SettingNodeV3::init(setting, width))
            return false;

        this->setContentHeight(60);
        this->getNameLabel()->setVisible(false);

        auto bg = NineSlice::create("white-square.png"_spr);
        bg->setColor(ccc3(255, 135, 88));
        bg->setOpacity(90);
        bg->setContentSize(m_obContentSize - ccp(40, 10));
        this->addChildAtPosition(bg, Anchor::Center);

        auto infoLabel = TextArea::create(
            "These are settings for the <co>Geode Mod Loader</c>, NOT for mods. "
            "To edit settings for <cj>mods</c>, press <cy>View</c> on the mod "
            "and then the settings button in the <cg>bottom left</c> of the popup.",
            "chatFont.fnt", .65f, bg->getContentWidth() - 40,
            ccp(.4999f, .4999f), 12, false
        );
        bg->addChildAtPosition(infoLabel, Anchor::Center);

        this->updateState(nullptr);

        return true;
    }

    void onCommit() override {}
    void onResetToDefault() override {}

public:
    static GeodeSettingsInfoSettingNode* create(std::shared_ptr<GeodeSettingsInfoSetting> setting, float width) {
        auto ret = new GeodeSettingsInfoSettingNode();
        if (ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool hasUncommittedChanges() const override {
        return false;
    }
    bool hasNonDefaultValue() const override {
        return false;
    }
};

SettingNodeV3* GeodeSettingsInfoSetting::createNode(float width) {
    return GeodeSettingsInfoSettingNode::create(
        std::static_pointer_cast<GeodeSettingsInfoSetting>(shared_from_this()),
        width
    );
}

class CopyButtonSetting : public SettingV3 {
public:
    static Result<std::shared_ptr<SettingV3>> parse(std::string key, std::string modID, matjson::Value const& json) {
        auto res = std::make_shared<CopyButtonSetting>();
        auto root = checkJson(json, "CopyButtonSetting");
        res->init(std::move(key), std::move(modID), root);
        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }

    bool load(matjson::Value const& json) override {
        return true;
    }
    bool save(matjson::Value& json) const override {
        return true;
    }

    bool isDefaultValue() const override {
        return true;
    }
    void reset() override {}

    SettingNodeV3* createNode(float width) override;
};

class CopyButtonSettingNode : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<CopyButtonSetting> setting, float width) {
        if (!SettingNodeV3::init(setting, width))
            return false;
        
        this->getNameLabel()->setVisible(false);

        auto folderSpr = createGeodeButton("Open Mods Folder");
        folderSpr->setScale(.5f);
        auto folderBtn = CCMenuItemSpriteExtra::create(
            folderSpr, this, menu_selector(CopyButtonSettingNode::onOpenFolder)
        );
        this->getButtonMenu()->addChild(folderBtn);

        auto copySpr = createGeodeButton("Copy Mods List");
        copySpr->setScale(.5f);
        auto copyBtn = CCMenuItemSpriteExtra::create(
            copySpr, this, menu_selector(CopyButtonSettingNode::onCopy)
        );
        this->getButtonMenu()->addChild(copyBtn);

        auto installFromFileSpr = createGeodeButton("Install From File");
        installFromFileSpr->setScale(.5f);
        auto installFromFileBtn = CCMenuItemSpriteExtra::create(
            installFromFileSpr, this, menu_selector(CopyButtonSettingNode::onInstallFromFile)
        );
        this->getButtonMenu()->addChild(installFromFileBtn);

        this->getButtonMenu()->setPosition(getContentSize() / 2);
        this->getButtonMenu()->setAnchorPoint({ .5f, .5f });
        this->getButtonMenu()->setContentWidth(width - 20);
        this->getButtonMenu()->setLayout(RowLayout::create());

        this->updateState(nullptr);

        return true;
    }

    void onOpenFolder(CCObject*) {
        file::openFolder(dirs::getModsDir());
    }
    void onCopy(CCObject*) {
        auto mods = Loader::get()->getAllMods();
        if (mods.empty()) {
            Notification::create("No mods installed", NotificationIcon::Info, 0.5f)->show();
            return;
        }

        std::sort(mods.begin(), mods.end(), [](Mod* a, Mod* b) {
            auto const s1 = a->getID();
            auto const s2 = b->getID();
            return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), [](auto a, auto b) {
                return std::tolower(a) < std::tolower(b);
            });
        });

        std::string modsList;
        using namespace std::string_view_literals;
        for (int i = 0; i < mods.size(); i++) {
            auto& mod = mods[i];
            modsList += fmt::format("{} | [{}] {}{}",
                mod->isLoaded() ? "x"sv :
                mod->targetsOutdatedVersion() ? "*"sv :
                mod->failedToLoad() ? "!"sv :
                " "sv,
                mod->getVersion().toVString(), mod->getID(),
                i < mods.size() ? "\n" : ""
            );
        }
        clipboard::write(modsList);

        Notification::create("Mods list copied to clipboard", NotificationIcon::Info, 0.5f)->show();
    }
    void onInstallFromFile(CCObject*) {
        ModsLayer::installModFromFile();
    }

    void onCommit() override {}
    void onResetToDefault() override {}

public:
    static CopyButtonSettingNode* create(std::shared_ptr<CopyButtonSetting> setting, float width) {
        auto ret = new CopyButtonSettingNode();
        if (ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool hasUncommittedChanges() const override {
        return false;
    }
    bool hasNonDefaultValue() const override {
        return false;
    }
};

SettingNodeV3* CopyButtonSetting::createNode(float width) {
    return CopyButtonSettingNode::create(
        std::static_pointer_cast<CopyButtonSetting>(shared_from_this()),
        width
    );
}

$on_mod(Loaded) {
    (void)Mod::get()->registerCustomSettingType("copy-mods", &CopyButtonSetting::parse);
    (void)Mod::get()->registerCustomSettingType("settings-info", &GeodeSettingsInfoSetting::parse);
}
