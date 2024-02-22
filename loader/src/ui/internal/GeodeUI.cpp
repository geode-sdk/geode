
#include "info/ModInfoPopup.hpp"
#include "list/ModListLayer.hpp"
#include "settings/ModSettingsPopup.hpp"

#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Index.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/web.hpp>

void geode::openModsList() {
    ModListLayer::scene();
}

void geode::openIssueReportPopup(Mod* mod) {
    if (mod->getMetadata().getIssues()) {
        MDPopup::create(
            "Issue Report",
                "Please report the issue to the mod that caused the crash.\n"
                "If your issue relates to a <cr>game crash</c>, <cb>please include</c> the "
                "latest crash log(s) from `" +
                dirs::getCrashlogsDir().string() + "`",
            "OK", "Open Folder",
            [mod](bool btn2) {
                file::openFolder(dirs::getCrashlogsDir());
            }
        )->show();
    }
    else {
        MDPopup::create(
            "Issue Report",
            "Please report your issue on the "
            "[#support](https://discord.com/channels/911701438269386882/979352389985390603) "
            "channnel in the [Geode Discord Server](https://discord.gg/9e43WMKzhp)\n\n"
            "If your issue relates to a <cr>game crash</c>, <cb>please include</c> the "
            "latest crash log(s) from `" +
                dirs::getCrashlogsDir().string() + "`",
            "OK"
        )
            ->show();
    }
}

void geode::openSupportPopup(Mod* mod) {
    MDPopup::create(
        "Support " + mod->getMetadata().getName(),
        mod->getMetadata().getSupportInfo().value(),
        "OK"
    )->show();
}

void geode::openInfoPopup(Mod* mod) {
    LocalModInfoPopup::create(mod, nullptr)->show();
}

void geode::openIndexPopup(Mod* mod) {
    if (auto item = Index::get()->getItem(mod)) {
        IndexItemInfoPopup::create(item, nullptr)->show();
    }
}

void geode::openSettingsPopup(Mod* mod) {
    if (mod->hasSettings()) {
        ModSettingsPopup::create(mod)->show();
    }
}

CCNode* geode::createDefaultLogo() {
    CCNode* spr = CCSprite::createWithSpriteFrameName("no-logo.png"_spr);
    if (!spr) {
        spr = CCLabelBMFont::create("OwO", "goldFont.fnt");
    }
    return spr;
}

CCNode* geode::createModLogo(Mod* mod) {
    CCNode* ret = nullptr;
    if (mod == Mod::get()) {
        ret = CCSprite::createWithSpriteFrameName("geode-logo.png"_spr);
    }
    else {
        ret = CCSprite::create(fmt::format("{}/logo.png", mod->getID()).c_str());
    }
    if (!ret) {
        ret = createDefaultLogo();
    }
    return ret;
}

CCNode* geode::createIndexItemLogo(IndexItemHandle item) {
    auto logoPath = ghc::filesystem::absolute(item->getRootPath() / "logo.png");
    CCNode* spr = CCSprite::create(logoPath.string().c_str());
    if (!spr) {
        spr = createDefaultLogo();
    }
    if (item->isFeatured()) {
        auto logoGlow = CCSprite::createWithSpriteFrameName("logo-glow.png"_spr);
        spr->setScaleX(logoGlow->getContentWidth() / spr->getContentWidth());
        spr->setScaleY(logoGlow->getContentHeight() / spr->getContentHeight());
        logoGlow->addChildAtPosition(spr, Anchor::Center);
        spr = logoGlow;
    }
    return spr;
}
