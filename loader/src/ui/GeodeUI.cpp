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

class LoadServerModLayer : public Popup {
protected:
    enum class MetadataLoaded {
        NotLoaded = 0,
        Loaded = 1,
        Error = 2,
    };

    std::string m_id;
    ListenerHandle m_listenerHandle;
    ListenerHandle m_versionListenerHandle;
    arc::Notify m_metadataLoaded;
    std::atomic<MetadataLoaded> m_metadataLoadedState{MetadataLoaded::NotLoaded};
    async::TaskHolder<Result<server::ServerModMetadata, server::ServerError>> m_listener;
    async::TaskHolder<Result<server::ServerModVersion, server::ServerError>> m_versionListener;

    std::optional<server::ServerModMetadata> m_loadedMod{};

    bool init(std::string id) {
        if (!Popup::init(180.f, 100.f, "square01_001.png"))
            return false;

        m_closeBtn->setVisible(false);

        this->setTitle("Loading mod...");

        auto spinner = LoadingSpinner::create(40);
        m_mainLayer->addChildAtPosition(spinner, Anchor::Center, ccp(0, -10));

        m_id = std::move(id);

        m_listener.spawn(
            server::getMod(m_id),
            [this](auto result) {
                this->onModRequest(std::move(result));
            }
        );

        return true;
    }

    void onModRequest(Result<server::ServerModMetadata, server::ServerError> result) {
        if (result.isOk()) {
            // Copy info first as onClose may free the listener which will free the event
            auto info = std::move(result).unwrap();
            m_loadedMod = std::move(info);

            m_metadataLoadedState.store(MetadataLoaded::Loaded);

            m_versionListener.spawn(
                server::getModVersion(m_id),
                [this](auto result) {
                    this->onVersionRequest(std::move(result));
                }
            );
        }
        else {
            this->onClose(nullptr);
            FLAlertLayer::create(
                "Error Loading Mod",
                fmt::format("Unable to find mod with the ID <cr>{}</c>!", m_id),
                "OK"
            )->show();

            m_metadataLoadedState.store(MetadataLoaded::Error);
        }
        m_metadataLoaded.notifyOne(true);
    }

    void onVersionRequest(Result<server::ServerModVersion, server::ServerError> result) {
        // this is promised non optional by this point
        auto info = std::move(*m_loadedMod);

        if (result.isOk()) {
            // i don't actually think there's a better way to do this
            // sorry guys

            info.versions = {std::move(result).unwrap()};
        }

        // if there's an error, just load whatever the last fetched version was
        // (this can happen for mods not on current gd version)

        this->onClose(nullptr);
        // Run this on next frame because otherwise the popup is unable to call server::getMod for some reason
        Loader::get()->queueInMainThread([info = std::move(info)]() mutable {
            ModPopup::create(ModSource(std::move(info)))->show();
        });
    }

public:
    arc::Future<bool> listen() const {
        co_await m_metadataLoaded.notified();
        m_metadataLoaded.notifyOne(true);
        co_return m_metadataLoadedState.load() == MetadataLoaded::Loaded;
    }

    static LoadServerModLayer* create(std::string id) {
        auto ret = new LoadServerModLayer();
        if (ret->init(std::move(id))) {
            ret->autorelease();
            return ret;
        }
        delete ret;
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

                auto issues = mod->getMetadata().getIssues();
                if (issues && issues->getURL()) {
                    auto& url = *issues->getURL();
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
    openSupportPopup(mod->getMetadata());
}

void geode::openSupportPopup(ModMetadata const& metadata) {
    MDPopup::create(
        fmt::format("Support {}", metadata.getName()),
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
std::optional<arc::TaskHandle<bool>> geode::openInfoPopup(std::string modID) {
    if (auto mod = Loader::get()->getInstalledMod(modID)) {
        openInfoPopup(mod);
        return std::nullopt;
    }

    auto popup = LoadServerModLayer::create(std::move(modID));
    return async::runtime().spawn([popup = Ref(popup)](this auto self) -> arc::Future<bool> {
        auto ret = co_await popup->listen();
        if (ret) {
            geode::queueInMainThread([popup] {
                popup->show();
            });
        }
        
        co_return ret;
    });
}

void geode::openChangelogPopup(Mod* mod) {
    auto popup = ModPopup::create(mod);
    popup->loadTab(ModPopup::Tab::Changelog);
    popup->show();
}

void geode::openSettingsPopup(Mod* mod) {
    openSettingsPopup(mod, true);
}
Popup* geode::openSettingsPopup(Mod* mod, bool disableGeodeTheme) {
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
    async::TaskHolder<Result<ByteVector, server::ServerError>> m_listener;

    bool init(ModLogoSrc&& src) {
        if (!CCNode::init())
            return false;

        this->setAnchorPoint({ .5f, .5f });
        this->setContentSize({ 50, 50 });

        m_sprite = LazySprite::create(this->getContentSize());
        this->addChildAtPosition(m_sprite, Anchor::Center);

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
                m_listener.spawn(
                    server::getModLogo(id),
                    [this](auto result) {
                        this->onFetch(std::move(result));
                    }
                );
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
        this->setID(Mod::get()->expandSpriteName(fmt::format("sprite-{}", m_modID)));

        ModLogoUIEvent().send(this, m_modID, std::nullopt);

        return true;
    }

    void doPostEvent() {
        ModLogoUIEvent().send(this, m_modID, std::nullopt);
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

    void onFetch(Result<ByteVector, server::ServerError> result) {
        // Set default sprite on error
        if (result.isErr()) {
            this->onLoadFailed(true);
        }
        // Otherwise load downloaded sprite to memory
        else {
            m_sprite->setLoadCallback([this](Result<> res) {
                this->onLoaded(std::move(res));
            });
            m_sprite->loadFromData(std::move(result).unwrap());
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

CCNode* geode::createServerModLogo(std::string id) {
    return ModLogoSprite::create(ModLogoSrc(std::move(id)));
}
