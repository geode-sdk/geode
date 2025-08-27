#include "mods/ModsLayer.hpp"
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/loader/Dirs.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/ui/LoadingSpinner.hpp>
#include <Geode/ui/LazySprite.hpp>
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
    EventListener<server::ServerRequest<server::ServerModVersion>> m_versionListener;

    std::optional<server::ServerModMetadata> m_loadedMod{};

    bool setup(std::string const& id) override {
        m_closeBtn->setVisible(false);

        this->setTitle("Loading mod...");

        auto spinner = LoadingSpinner::create(40);
        m_mainLayer->addChildAtPosition(spinner, Anchor::Center, ccp(0, -10));

        m_id = id;
        m_listener.bind(this, &LoadServerModLayer::onModRequest);
        m_listener.setFilter(server::getMod(id));

        return true;
    }

    void onModRequest(server::ServerRequest<server::ServerModMetadata>::Event* event) {
        if (auto res = event->getValue()) {
            if (res->isOk()) {
                // Copy info first as onClose may free the listener which will free the event
                auto info = res->unwrap();
                m_loadedMod = std::move(info);

                m_versionListener.bind(this, &LoadServerModLayer::onVersionRequest);
                m_versionListener.setFilter(server::getModVersion(m_id));
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

    void onVersionRequest(server::ServerRequest<server::ServerModVersion>::Event* event) {
        if (auto res = event->getValue()) {
            // this is promised non optional by this point
            auto info = std::move(*m_loadedMod);

            if (res->isOk()) {
                // i don't actually think there's a better way to do this
                // sorry guys

                auto versionInfo = res->unwrap();
                info.versions = {versionInfo};
            }

            // if there's an error, just load whatever the last fetched version was
            // (this can happen for mods not on current gd version)

            this->onClose(nullptr);
            // Run this on next frame because otherwise the popup is unable to call server::getMod for some reason
            Loader::get()->queueInMainThread([info = std::move(info)]() mutable {
                ModPopup::create(ModSource(std::move(info)))->show();
            });
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
    if (mod->getMetadataRef().getIssues()) {
        MDPopup::create(
            "Issue Report",
                fmt::format(
                    "Please report the issue to the mod that caused the crash.\n"
                    "If your issue relates to a <cr>game crash</c>, <cb>please include</c> the "
                    "latest crash log(s) from `{}`",
                    dirs::getCrashlogsDir()
                ),
            "OK", "Open Folder",
            [mod](bool btn2) {
                if (btn2) {
                    file::openFolder(dirs::getCrashlogsDir());
                    return;
                }

                auto issues = mod->getMetadataRef().getIssues();
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
            fmt::format(
                "Please report your issue on the "
                "[#support](https://discord.com/channels/911701438269386882/979352389985390603) "
                "channnel in the [Geode Discord Server](https://discord.gg/9e43WMKzhp)\n\n"
                "If your issue relates to a <cr>game crash</c>, <cb>please include</c> the "
                "latest crash log(s) from `{}`",
                dirs::getCrashlogsDir()
            ),
            "OK"
        )->show();
    }
}

void geode::openSupportPopup(Mod* mod) {
    openSupportPopup(mod->getMetadataRef());
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

using ModLogoSrc = std::variant<Mod*, std::string, std::filesystem::path>;

class ModLogoSprite : public CCNodeRGBA {
protected:
    LazySprite* m_sprite;
    std::string m_modID;
    EventListener<server::ServerRequest<ByteVector>> m_listener;

    bool init(ModLogoSrc&& src) {
        if (!CCNode::init())
            return false;

        this->setAnchorPoint({ .5f, .5f });
        this->setContentSize({ 50, 50 });

        m_sprite = LazySprite::create(this->getContentSize());
        this->addChildAtPosition(m_sprite, Anchor::Center);

        m_listener.bind(this, &ModLogoSprite::onFetch);

        std::visit(makeVisitor {
            [this](Mod* mod) {
                m_modID = mod->getID();

                m_sprite->setLoadCallback([this](Result<> res) {
                    this->onLoaded(std::move(res));
                });

                // Load from Resources
                if (!mod->isInternal()) {
                    m_sprite->loadFromFile(dirs::getModRuntimeDir() / mod->getID() / "logo.png");
                } else {
                    if (Mod::get()->getSavedValue("alternate-geode-style", false)) {
                        m_sprite->initWithSpriteFrameName("geode-logo-alternate.png"_spr);
                    }
                    else {
                        m_sprite->initWithSpriteFrameName("geode-logo.png"_spr);
                    }
                }
            },
            [this](std::string const& id) {
                m_modID = id;

                // Asynchronously fetch from server
                m_listener.setFilter(server::getModLogo(id));
            },
            [this](std::filesystem::path const& path) {
                m_sprite->setLoadCallback([this](Result<> res) {
                    this->onLoaded(std::move(res));
                });

                if (auto unzip = file::Unzip::create(path)) {
                    if (auto logo = unzip.unwrap().extract("logo.png")) {
                        m_sprite->loadFromData(std::move(logo).unwrap());
                    }
                }

                // if the logo.png was not found then switch to fallback label
                if (!m_sprite->isLoading()) {
                    this->onLoadFailed(true);
                }
            },
        }, src);

        // This is a default ID, nothing should ever rely on the ID of any ModLogoSprite being this
        this->setID(std::string(Mod::get()->expandSpriteName(fmt::format("sprite-{}", m_modID))));

        ModLogoUIEvent(std::make_unique<ModLogoUIEvent::Impl>(this, m_modID)).post();

        return true;
    }

    void doPostEvent() {
        ModLogoUIEvent(std::make_unique<ModLogoUIEvent::Impl>(this, m_modID)).post();
    }

    void onLoaded(Result<> res) {
        if (!res) {
            log::debug("Failed to load image: {}", res.err().value_or(std::string{}));
            this->onLoadFailed(true);
            return;
        }

        limitNodeSize(m_sprite, m_obContentSize, 99.f, 0.f);
        this->doPostEvent();
    }

    void onLoadFailed(bool postEvent) {
        // Fallback to default logo if the image failed to load
        auto sprite = CCLabelBMFont::create("N/A", "bigFont.fnt");
        sprite->setPosition(this->getScaledContentSize() / 2.f + CCSize{1.f, 2.f});
        sprite->setOpacity(90);
        limitNodeSize(sprite, m_obContentSize, 99.f, 0.f);
        this->addChildAtPosition(sprite, Anchor::Center);

        this->doPostEvent();
    }

    void onFetch(server::ServerRequest<ByteVector>::Event* event) {
        if (auto result = event->getValue()) {
            // Set default sprite on error
            if (result->isErr()) {
                this->onLoadFailed(true);
            }
            // Otherwise load downloaded sprite to memory
            else {
                m_sprite->setLoadCallback([this](Result<> res) {
                    this->onLoaded(std::move(res));
                });
                m_sprite->loadFromData(result->unwrap());
            }
        }
        else if (event->isCancelled()) {
            this->onLoadFailed(true);
        }
    }

public:
    static ModLogoSprite* create(ModLogoSrc&& src) {
        auto ret = new ModLogoSprite();
        if (ret->init(std::move(src))) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

CCNode* geode::createDefaultLogo() {
    return ModLogoSprite::create(ModLogoSrc(nullptr));
}

CCNode* geode::createModLogo(Mod* mod) {
    return ModLogoSprite::create(ModLogoSrc(mod));
}

CCNode* geode::createModLogo(std::filesystem::path const& geodePackage) {
    return ModLogoSprite::create(ModLogoSrc(geodePackage));
}

CCNode* geode::createServerModLogo(std::string const& id) {
    return ModLogoSprite::create(ModLogoSrc(id));
}
