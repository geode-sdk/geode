
#include "mods/ModsLayer.hpp"
#include <Geode/loader/Dirs.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/web.hpp>
#include <server/Server.hpp>

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
        )->show();
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
    #pragma message("todo")
    // LocalModInfoPopup::create(mod, nullptr)->show();
}

void geode::openIndexPopup(Mod* mod) {
    #pragma message("todo")
    // if (auto item = Index::get()->getItem(mod)) {
    //     IndexItemInfoPopup::create(item, nullptr)->show();
    // }
}

void geode::openSettingsPopup(Mod* mod) {
    if (mod->hasSettings()) {
        #pragma message("todo")
        // ModSettingsPopup::create(mod)->show();
    }
}

class ModLogoSprite : public CCNode {
protected:
    std::string m_modID;
    CCNode* m_sprite = nullptr;
    CCNode* m_featuredSprite = nullptr;
    EventListener<PromiseEventFilter<ByteVector, server::ServerError>> m_listener;

    bool init(std::string const& id, bool fetch, bool featured) {
        if (!CCNode::init())
            return false;
        
        this->setAnchorPoint({ .5f, .5f });
        this->setContentSize({ 50, 50 });

        m_modID = id;
        m_listener.bind(this, &ModLogoSprite::onFetch);

        if (featured) {
            m_featuredSprite = CCSprite::createWithSpriteFrameName("logo-glow.png"_spr);
            limitNodeSize(m_featuredSprite, m_obContentSize * 1.15f, 99.f, .1f);
            m_featuredSprite->setZOrder(-1);
            this->addChildAtPosition(m_featuredSprite, Anchor::Center);
        }

        // Load from Resources
        if (!fetch) {
            this->setSprite(id == "geode.loader" ? 
                CCSprite::createWithSpriteFrameName("geode-logo.png"_spr) : 
                CCSprite::create(fmt::format("{}/logo.png", id).c_str())
            );
        }
        // Asynchronously fetch from server
        else {
            this->setSprite(CCSprite::create("loadingCircle.png"));
            static_cast<CCSprite*>(m_sprite)->setBlendFunc({ GL_ONE, GL_ONE });
            m_sprite->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));
            m_listener.setFilter(server::getModLogo(id).listen());
        }

        if (m_featuredSprite) {
            m_featuredSprite->setVisible(!fetch);
        }
        
        return true;
    }

    void setSprite(CCNode* sprite) {
        // Remove any existing sprite
        if (m_sprite) {
            m_sprite->removeFromParent();
        }
        // Fallback to default logo if the sprite is null
        if (!sprite) {
            sprite = CCSprite::createWithSpriteFrameName("no-logo.png"_spr);
        }
        // Fallback to lobotomy if Geode sprites are missing
        if (!sprite) {
            sprite = CCSprite::createWithSpriteFrameName("difficulty_02_btn_001.png");
        }
        // Set sprite and scale it to node size
        m_sprite = sprite;
        limitNodeSize(m_sprite, m_obContentSize, 99.f, .05f);
        this->addChildAtPosition(m_sprite, Anchor::Center);

        // Featured sprite is initially invisible if fetched from server
        if (m_featuredSprite) {
            m_featuredSprite->setVisible(true);
        }
    }

    void onFetch(PromiseEvent<ByteVector, server::ServerError>* event) {
        // Set default sprite on error
        if (event->getReject()) {
            this->setSprite(nullptr);
        }
        // Otherwise load downloaded sprite to memory
        else if (auto data = event->getResolve()) {
            auto image = Ref(new CCImage());
            image->initWithImageData(const_cast<uint8_t*>(data->data()), data->size());

            auto texture = CCTextureCache::get()->addUIImage(image, m_modID.c_str());
            this->setSprite(CCSprite::createWithTexture(texture));
        }
    }

public:
    static ModLogoSprite* create(std::string const& id, bool fetch = false, bool featured = false) {
        auto ret = new ModLogoSprite();
        if (ret && ret->init(id, fetch, featured)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

CCNode* geode::createDefaultLogo() {
    return ModLogoSprite::create("");
}

CCNode* geode::createModLogo(Mod* mod) {
    return ModLogoSprite::create(mod->getID());
}

CCNode* geode::createServerModLogo(std::string const& id, bool featured) {
    return ModLogoSprite::create(id, true, featured);
}

// CCNode* geode::createIndexItemLogo(IndexItemHandle item) {
//     auto logoPath = ghc::filesystem::absolute(item->getRootPath() / "logo.png");
//     CCNode* spr = CCSprite::create(logoPath.string().c_str());
//     if (!spr) {
//         spr = createDefaultLogo();
//     }
//     if (item->isFeatured()) {
//         auto logoGlow = CCSprite::createWithSpriteFrameName("logo-glow.png"_spr);
//         spr->setScaleX(logoGlow->getContentWidth() / spr->getContentWidth());
//         spr->setScaleY(logoGlow->getContentHeight() / spr->getContentHeight());
//         logoGlow->addChildAtPosition(spr, Anchor::Center);
//         spr = logoGlow;
//     }
//     return spr;
// }
