#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/CCLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <array>
#include <fmt/format.h>
#include <loader/LoaderImpl.hpp>
#include <loader/console.hpp>
#include <loader/updater.hpp>
#include <Geode/utils/NodeIDs.hpp>

using namespace geode::prelude;

struct CustomLoadingLayer : Modify<CustomLoadingLayer, LoadingLayer> {
    struct Fields {
        bool m_menuDisabled = false;
        CCLabelBMFont* m_smallLabel = nullptr;
        CCLabelBMFont* m_smallLabel2 = nullptr;
        int m_geodeLoadStep = 0;
        int m_totalMods = 0;
    };

    static void onModify(auto& self) {
        if (!self.setHookPriority("LoadingLayer::init", geode::node_ids::GEODE_ID_PRIORITY)) {
            log::warn("Failed to set LoadingLayer::init hook priority, node IDs may not work properly");
        }
        GEODE_FORWARD_COMPAT_DISABLE_HOOKS_INNER("Switching to fallback custom loading layer")
    }

    void updateLoadedModsLabel() {
        auto allMods = Loader::get()->getAllMods();
        auto count = std::count_if(allMods.begin(), allMods.end(), [&](auto& item) {
            return item->isEnabled();
        });
        auto str = fmt::format("Geode: Loaded {}/{} mods", count, m_fields->m_totalMods);
        this->setSmallText(str);
        auto currentMod = LoaderImpl::get()->m_currentlyLoadingMod;
        auto modName = currentMod ? currentMod->getName() : "Unknown";
        this->setSmallText2(modName);
    }

    void setSmallText(std::string const& text) {
        if (!m_fields->m_menuDisabled) {
            m_fields->m_smallLabel->setString(text.c_str());
        }
    }

    void setSmallText2(std::string const& text) {
        if (!m_fields->m_menuDisabled) {
            m_fields->m_smallLabel2->setString(text.c_str());
        }
    }

    // hook
    bool init(bool fromReload) {
        CCFileUtils::get()->updatePaths();

        if (!LoadingLayer::init(fromReload)) return false;

        NodeIDs::provideFor(this);

        m_fields->m_totalMods = Loader::get()->getAllMods().size();
        m_fields->m_menuDisabled = Loader::get()->getLaunchFlag("disable-custom-menu");
        if (m_fields->m_menuDisabled) {
            return true;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        m_fields->m_smallLabel = CCLabelBMFont::create("", "goldFont.fnt");
        m_fields->m_smallLabel->setPosition(winSize.width / 2, 30.f);
        m_fields->m_smallLabel->setScale(.45f);
        m_fields->m_smallLabel->setID("geode-small-label");
        this->addChild(m_fields->m_smallLabel);

        m_fields->m_smallLabel2 = CCLabelBMFont::create("", "goldFont.fnt");
        m_fields->m_smallLabel2->setPosition(winSize.width / 2, 15.f);
        m_fields->m_smallLabel2->setScale(.45f);
        m_fields->m_smallLabel2->setID("geode-small-label-2");
        this->addChild(m_fields->m_smallLabel2);

        return true;
    }

    void setupLoadingMods() {
        if (Loader::get()->getLoadingState() != Loader::LoadingState::Done) {
            this->updateLoadedModsLabel();
            this->waitLoadAssets();
        }
        else {
            this->continueLoadAssets();
            this->setSmallText2("");
        }
    }

    void setupLoaderResources() {
        log::debug("Verifying Loader Resources");
        this->setSmallText("Verifying Loader Resources");
        // verify loader resources
        Loader::get()->queueInMainThread([&]() {
            if (!updater::verifyLoaderResources()) {
                log::debug("Downloading Loader Resources");
                this->setSmallText("Downloading Loader Resources");
                this->addChild(EventListenerNode<updater::ResourceDownloadFilter>::create(
                    this, &CustomLoadingLayer::updateResourcesProgress
                ));
            }
            else {
                log::debug("Loading Loader Resources");
                this->setSmallText("Loading Loader Resources");
                updater::updateSpecialFiles();
                this->continueLoadAssets();
            }
        });
    }

    void updateResourcesProgress(updater::ResourceDownloadEvent* event) {
        std::visit(makeVisitor {
            [&](updater::UpdateProgress const& progress) {
                this->setSmallText(fmt::format(
                    "Downloading Loader Resources: {}%", progress.first
                ));
            },
            [&](updater::UpdateFinished) {
                log::debug("Downloaded Loader Resources");
                this->setSmallText("Downloaded Loader Resources");
                this->continueLoadAssets();
            },
            [&](updater::UpdateFailed const& error) {
                log::debug("Failed Loader Resources");
                console::messageBox(
                    "Error updating resources",
                    error + ".\n"
                    "You will have to install resources manually by downloading resources.zip "
                    "from the latest release on GitHub: "
                    "https://github.com/geode-sdk/geode/releases/latest.\n"
                    "The game will be loaded as normal, but please be aware "
                    "that it is very likely to crash. "
                );
                this->setSmallText("Failed Loader Resources");
                this->continueLoadAssets();
            }
        }, event->status);
    }

    void setupModResources() {
        log::debug("Loading mod resources");
        this->setSmallText("Loading mod resources");
        LoaderImpl::get()->updateResources(true);
        this->continueLoadAssets();
    }

    int getLoadedMods() {
        auto allMods = Loader::get()->getAllMods();
        return std::count_if(allMods.begin(), allMods.end(), [&](auto& item) {
            return item->isEnabled();
        });
    }

    int getEnabledMods() {
        auto allMods = Loader::get()->getAllMods();
        return std::count_if(allMods.begin(), allMods.end(), [&](auto& item) {
            return item->shouldLoad();
        });
    }
    
    int getCurrentStep() {
        return m_fields->m_geodeLoadStep + m_loadStep + getLoadedMods();
    }

    int getTotalStep() {
        return 3 + 14 + getEnabledMods();
    }

    void updateLoadingBar() {
        auto length = m_sliderGrooveXPos * this->getCurrentStep() / this->getTotalStep();
        m_sliderBar->setTextureRect({0, 0, length, m_sliderGrooveHeight});
    }

    void waitLoadAssets() {
        Loader::get()->queueInMainThread([this]() {
            this->loadAssets();
        });
    }

    void continueLoadAssets() {
        ++m_fields->m_geodeLoadStep;
        Loader::get()->queueInMainThread([this]() {
            this->loadAssets();
        });
    }

    bool skipOnRefresh() {
        if (m_fromRefresh) {
            this->continueLoadAssets();
        }
        return !m_fromRefresh;
    }

    // hook
    void loadAssets() {
        switch (m_fields->m_geodeLoadStep) {
        case 0:
            if (this->skipOnRefresh()) this->setupLoadingMods();
            break;
        case 1:
            if (this->skipOnRefresh()) this->setupLoaderResources();
            break;
        case 2:
            this->setupModResources();
            break;
        case 3:
        default:
            this->setSmallText("Loading game resources");
            LoadingLayer::loadAssets();
            break;
        }
        this->updateLoadingBar();
    }
};

struct FallbackCustomLoadingLayer : Modify<FallbackCustomLoadingLayer, CCLayer> {
    static void onModify(auto& self) {
        GEODE_FORWARD_COMPAT_ENABLE_HOOKS_INNER("")
        else if (!self.setHookPriority("CCLayer::init", geode::node_ids::GEODE_ID_PRIORITY)) {
            log::warn("Failed to set CCLayer::init hook priority, node IDs may not work properly");
        }
    }

    bool init() {
        if (!CCLayer::init())
            return false;
        auto* self = typeinfo_cast<LoadingLayer*>(this);
        if (!self)
            return true;

        NodeIDs::provideFor(self);

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto label = CCLabelBMFont::create(
            "Loading Geode without UI, see console for details.",
            "goldFont.fnt"
        );
        // this code is weird but its to avoid any virtual calls,
        // which can change between versions. so instead, we force
        // it to use the symbol, so it would only break if the function signature
        // were to change.
        label->CCNode::setPosition(winSize.width / 2, 30.f);
        label->CCLabelBMFont::setScale(.45f);
        label->CCNode::setZOrder(99);
        this->CCNode::addChild(label);
        // label->setID("geode-small-label");

        // TODO: verify loader resources on fallback?

        LoaderImpl::get()->updateResources(true);

        return true;
    }
};
