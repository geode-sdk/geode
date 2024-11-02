#include "mods/ModsLayer.hpp"
#include <Geode/loader/Dirs.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/ui/LoadingSpinner.hpp>
#include <Geode/utils/web.hpp>
#include <server/Server.hpp>
#include "mods/GeodeStyle.hpp"
#include "mods/settings/ModSettingsPopup.hpp"
#include "mods/popups/ModPopup.hpp"
#include "GeodeUIEvent.hpp"

class LoadServerModLayer : public Popup<std::string const&> {
protected:
    std::string m_id;
    EventListener<server::ServerRequest<server::ServerModMetadata>> m_listener;

    bool setup(std::string const& id) override {
        m_closeBtn->setVisible(false);

        this->setTitle("Loading mod...");

        auto spinner = LoadingSpinner::create(40);
        m_mainLayer->addChildAtPosition(spinner, Anchor::Center, ccp(0, -10));

        m_id = id;
        m_listener.bind(this, &LoadServerModLayer::onRequest);
        m_listener.setFilter(server::getMod(id));

        return true;
    }

    void onRequest(server::ServerRequest<server::ServerModMetadata>::Event* event) {
        if (auto res = event->getValue()) {
            if (res->isOk()) {
                // Copy info first as onClose may free the listener which will free the event
                auto info = **res;
                this->onClose(nullptr);
                // Run this on next frame because otherwise the popup is unable to call server::getMod for some reason
                Loader::get()->queueInMainThread([info = std::move(info)]() mutable {
                    ModPopup::create(ModSource(std::move(info)))->show();
                });
            }
            else {
                auto id = m_id;
                this->onClose(nullptr);
                FLAlertLayer::create(
                    "Error Loading Mod",
                    fmt::format("Unable to find mod with the ID <cr>{}</c>!", id),
                    "OK"
                )->show();
            }
        }
        else if (event->isCancelled()) {
            this->onClose(nullptr);
        }
    }

public:
    Task<bool> listen() const {
        return m_listener.getFilter().map(
            [](auto* result) -> bool { return result->isOk(); },
            [](auto) -> std::monostate { return std::monostate(); }
        );
    }

    static LoadServerModLayer* create(std::string const& id) {
        auto ret = new LoadServerModLayer();
        if (ret && ret->initAnchored(180, 100, id, "square01_001.png", CCRectZero)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_RELEASE(ret);
        return nullptr;
    }
};

void geode::openModsList() {
    ModsLayer::scene();
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
        )->show();
    }
}

void geode::openSupportPopup(Mod* mod) {
    openSupportPopup(mod->getMetadata());
}

void geode::openSupportPopup(ModMetadata const& metadata) {
    MDPopup::create(
        "Support " + metadata.getName(),
        metadata.getSupportInfo().value_or(
            "Developing mods takes a lot of time and effort! "
            "Consider <cy>supporting the developers</c> of your favorite mods "
            "to show them thanks for all their hard work <3"
        ),
        "OK"
    )->show();
}

void geode::openInfoPopup(Mod* mod) {
    ModPopup::create(mod)->show();
}
Task<bool> geode::openInfoPopup(std::string const& modID) {
    if (auto mod = Loader::get()->getInstalledMod(modID)) {
        openInfoPopup(mod);
        return Task<bool>::immediate(true);
    }
    else {
        auto popup = LoadServerModLayer::create(modID);
        auto task = popup->listen();
        popup->show();
        return task;
    }
}
void geode::openIndexPopup(Mod* mod) {
    // deprecated func
    openInfoPopup(mod);
}

void geode::openChangelogPopup(Mod* mod) {
    auto popup = ModPopup::create(mod);
    popup->loadTab(ModPopup::Tab::Changelog);
    popup->show();
}

void geode::openSettingsPopup(Mod* mod) {
    openSettingsPopup(mod, true);
}
Popup<Mod*>* geode::openSettingsPopup(Mod* mod, bool disableGeodeTheme) {
    if (mod->hasSettings()) {
        auto popup = ModSettingsPopup::create(mod, disableGeodeTheme);
        popup->show();
        return popup;
    }
    return nullptr;
}

class ModLogoSprite : public CCNode {
protected:
    std::string m_modID;
    CCNode* m_sprite = nullptr;
    EventListener<server::ServerRequest<ByteVector>> m_listener;

    bool init(std::string const& id, bool fetch) {
        if (!CCNode::init())
            return false;
        
        this->setAnchorPoint({ .5f, .5f });
        this->setContentSize({ 50, 50 });

        // This is a default ID, nothing should ever rely on the ID of any ModLogoSprite being this
        this->setID(std::string(Mod::get()->expandSpriteName(fmt::format("sprite-{}", id))));

        m_modID = id;
        m_listener.bind(this, &ModLogoSprite::onFetch);

        // Load from Resources
        if (!fetch) {
            this->setSprite(id == "geode.loader" ? 
                CCSprite::createWithSpriteFrameName("geode-logo.png"_spr) : 
                CCSprite::create(fmt::format("{}/logo.png", id).c_str()),
                false
            );
        }
        // Asynchronously fetch from server
        else {
            this->setSprite(createLoadingCircle(25), false);
            m_listener.setFilter(server::getModLogo(id));
        }

        ModLogoUIEvent(std::make_unique<ModLogoUIEvent::Impl>(this, id)).post();

        return true;
    }

    void setSprite(CCNode* sprite, bool postEvent) {
        // Remove any existing sprite
        if (m_sprite) {
            m_sprite->removeFromParent();
        }
        // Fallback to default logo if the sprite is null
        if (!sprite || sprite->getUserObject("geode.texture-loader/fallback")) {
            sprite = CCLabelBMFont::create("N/A", "bigFont.fnt");
            static_cast<CCLabelBMFont*>(sprite)->setOpacity(90);
        }
        // Set sprite and scale it to node size
        m_sprite = sprite;
        m_sprite->setID("sprite");
        limitNodeSize(m_sprite, m_obContentSize, 99.f, 0.f);
        this->addChildAtPosition(m_sprite, Anchor::Center);

        if (postEvent) {
            ModLogoUIEvent(std::make_unique<ModLogoUIEvent::Impl>(this, m_modID)).post();
        }
    }

    void onFetch(server::ServerRequest<ByteVector>::Event* event) {
        if (auto result = event->getValue()) {
            // Set default sprite on error
            if (result->isErr()) {
                this->setSprite(nullptr, true);
            }
            // Otherwise load downloaded sprite to memory
            else {
                auto data = result->unwrap();
                auto image = Ref(new CCImage());
                image->initWithImageData(data.data(), data.size());

                auto texture = CCTextureCache::get()->addUIImage(image, m_modID.c_str());
                this->setSprite(CCSprite::createWithTexture(texture), true);
            }
        }
        else if (event->isCancelled()) {
            this->setSprite(nullptr, true);
        }
    }

public:
    static ModLogoSprite* create(std::string const& id, bool fetch = false) {
        auto ret = new ModLogoSprite();
        if (ret->init(id, fetch)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

CCNode* geode::createDefaultLogo() {
    return ModLogoSprite::create("");
}

CCNode* geode::createModLogo(Mod* mod) {
    return ModLogoSprite::create(mod->getID());
}

CCNode* geode::createServerModLogo(std::string const& id) {
    return ModLogoSprite::create(id, true);
}
