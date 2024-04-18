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
                if (btn2) {
                    file::openFolder(dirs::getCrashlogsDir());
                    return;
                } 

                auto issues = mod->getMetadata().getIssues();
                if (issues && issues.value().url) {
                    auto url = issues.value().url.value();
                    web::openLinkInBrowser(url);
                }
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

CCNode* geode::createDefaultLogo(CCSize const& size) {
    CCNode* spr = CCSprite::createWithSpriteFrameName("no-logo.png"_spr);
    if (!spr) {
        spr = CCLabelBMFont::create("OwO", "goldFont.fnt");
    }
    limitNodeSize(spr, size, 1.f, .01f);
    return spr;
}

CCNode* geode::createModLogo(Mod* mod, CCSize const& size) {
    CCNode* spr = mod == Mod::get() ?
        CCSprite::createWithSpriteFrameName("geode-logo.png"_spr) :
        CCSprite::create(fmt::format("{}/logo.png", mod->getID()).c_str());
    if (!spr) spr = CCSprite::createWithSpriteFrameName("no-logo.png"_spr);
    if (!spr) spr = CCLabelBMFont::create("N/A", "goldFont.fnt");
    limitNodeSize(spr, size, 1.f, .01f);
    spr->setPosition(size/2);
    spr->setAnchorPoint({.5f, .5f});

    auto node = CCNode::create();
    node->addChild(spr);
    node->setContentSize(size);
    return node;
}

CCNode* geode::createIndexItemLogo(IndexItemHandle item, CCSize const& size) {
    auto logoPath = ghc::filesystem::absolute(item->getRootPath() / "logo.png");
    CCNode* spr = CCSprite::create(logoPath.string().c_str());
    if (!spr) {
        spr = CCSprite::createWithSpriteFrameName("no-logo.png"_spr);
    }
    if (!spr) {
        spr = CCLabelBMFont::create("N/A", "goldFont.fnt");
    }
    if (item->isFeatured()) {
        auto glowSize = size + CCSize(4.f, 4.f);

        auto logoGlow = CCSprite::createWithSpriteFrameName("logo-glow.png"_spr);
        logoGlow->setScaleX(glowSize.width / logoGlow->getContentSize().width);
        logoGlow->setScaleY(glowSize.height / logoGlow->getContentSize().height);

        // i dont know why + 1 is needed and its too late for me to figure out why
        spr->setPosition(
            logoGlow->getContentSize().width / 2 + 1,
            logoGlow->getContentSize().height / 2 - 1
        );
        // scary mathematics
        spr->setScaleX(size.width / spr->getContentSize().width / logoGlow->getScaleX());
        spr->setScaleY(size.height / spr->getContentSize().height / logoGlow->getScaleY());
        logoGlow->addChild(spr);
        spr = logoGlow;
    }
    else {
        limitNodeSize(spr, size, 1.f, .01f);
    }
    spr->setPosition(size/2);
    spr->setAnchorPoint({.5f, .5f});

    auto node = CCNode::create();
    node->addChild(spr);
    node->setContentSize(size);
    return node;
}
