#include "mods/ModsLayer.hpp"
#include <Geode/loader/Dirs.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/web.hpp>
#include <server/Server.hpp>
#include "mods/GeodeStyle.hpp"
#include "mods/settings/ModSettingsPopup.hpp"
#include "mods/popups/ModPopup.hpp"

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
    // TODO
    // LocalModInfoPopup::create(mod, nullptr)->show();
}

void geode::openIndexPopup(Mod* mod) {
    ModPopup::create(mod)->show();
}

void geode::openSettingsPopup(Mod* mod) {
    if (mod->hasSettings()) {
        ModSettingsPopup::create(mod)->show();
    }
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

        m_modID = id;
        m_listener.bind(this, &ModLogoSprite::onFetch);

        // Load from Resources
        if (!fetch) {
            this->setSprite(id == "geode.loader" ? 
                CCSprite::createWithSpriteFrameName("geode-logo.png"_spr) : 
                CCSprite::create(fmt::format("{}/logo.png", id).c_str())
            );
        }
        // Asynchronously fetch from server
        else {
            this->setSprite(createLoadingCircle(25));
            m_listener.setFilter(server::getModLogo(id));
        }

        return true;
    }

    void setSprite(CCNode* sprite) {
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
        limitNodeSize(m_sprite, m_obContentSize, 99.f, .05f);
        this->addChildAtPosition(m_sprite, Anchor::Center);
    }

    void onFetch(server::ServerRequest<ByteVector>::Event* event) {
        if (auto result = event->getValue()) {
            // Set default sprite on error
            if (result->isErr()) {
                this->setSprite(nullptr);
            }
            // Otherwise load downloaded sprite to memory
            else {
                auto data = result->unwrap();
                auto image = Ref(new CCImage());
                image->initWithImageData(const_cast<uint8_t*>(data.data()), data.size());

                auto texture = CCTextureCache::get()->addUIImage(image, m_modID.c_str());
                this->setSprite(CCSprite::createWithTexture(texture));
            }
        }
        else if (event->isCancelled()) {
            this->setSprite(nullptr);
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
