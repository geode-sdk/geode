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
    bool m_menuDisabled;
    CCLabelBMFont* m_smallLabel = nullptr;
    CCLabelBMFont* m_smallLabel2 = nullptr;
    int m_geodeLoadStep = 0;
    int m_totalMods = 0;
    int m_totalSteps = 18;

    static void onModify(auto& self) {
        if (!self.setHookPriority("LoadingLayer::init", geode::node_ids::GEODE_ID_PRIORITY)) {
            log::warn("Failed to set LoadingLayer::init hook priority, node IDs may not work properly");
        }
        GEODE_FORWARD_COMPAT_DISABLE_HOOKS_INNER("Switching to fallback custom loading layer")
    }

    void initLabels(const std::string& font) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        m_smallLabel = CCLabelBMFont::create("", font.c_str());
        m_smallLabel->setPosition(winSize.width / 2, 30.f);
        m_smallLabel->setScale(0.45f);
        addChild(m_smallLabel);

        m_smallLabel2 = CCLabelBMFont::create("", font.c_str());
        m_smallLabel2->setPosition(winSize.width / 2, 15.f);
        m_smallLabel2->setScale(0.45f);
        addChild(m_smallLabel2);
    }

    void updateLoadedModsLabel() {
        auto count = std::count_if(Loader::get()->getAllMods().begin(), Loader::get()->getAllMods().end(), [](auto& item) {
            return item->isEnabled();
        });
        auto str = fmt::format("Geode: Loaded {}/{} mods", count, m_totalMods);
        setSmallText(str, m_smallLabel);
        auto currentMod = LoaderImpl::get()->m_currentlyLoadingMod;
        auto modName = currentMod ? currentMod->getName() : "Unknown";
        setSmallText(modName, m_smallLabel2);
    }

    void setSmallText(const std::string& text, CCLabelBMFont* label) {
        if (!m_menuDisabled && label) {
            label->setString(text.c_str());
        }
    }

    bool init(bool fromReload) {
        CCFileUtils::get()->updatePaths();

        if (!LoadingLayer::init(fromReload)) return false;

        NodeIDs::provideFor(this);

        m_totalMods = Loader::get()->getAllMods().size();
        m_menuDisabled = Loader::get()->getLaunchFlag("disable-custom-menu");
        if (m_menuDisabled) {
            return true;
        }

        initLabels("goldFont.fnt");

        return true;
    }

    void setupLoadingMods() {
        if (Loader::get()->getLoadingState() != Loader::LoadingState::Done) {
            updateLoadedModsLabel();
            waitLoadAssets();
        }
        else {
            continueLoadAssets();
            setSmallText("", m_smallLabel2);
        }
    }

    void setupLoaderResources() {
        log::debug("Verifying Loader Resources");
        setSmallText("Verifying Loader Resources", m_smallLabel);
        Loader::get()->queueInMainThread([this]() {
            if (!updater::verifyLoaderResources()) {
                log::debug("Downloading Loader Resources");
                setSmallText("Downloading Loader Resources", m_smallLabel);
                addChild(EventListenerNode<updater::ResourceDownloadFilter>::create(
                    this, &CustomLoadingLayer::updateResourcesProgress
                ));
            }
            else {
                log::debug("Loading Loader Resources");
                setSmallText("Loading Loader Resources", m_smallLabel);
                updater::updateSpecialFiles();
                continueLoadAssets();
            }
        });
    }

    void updateResourcesProgress(updater::ResourceDownloadEvent* event) {
        std::visit(makeVisitor {
            [&](UpdateProgress const& progress) {
                setSmallText(fmt::format("Downloading Loader Resources: {}%", progress.first), m_smallLabel);
            },
            [&](UpdateFinished) {
                log::debug("Downloaded Loader Resources");
                setSmallText("Downloaded Loader Resources", m_smallLabel);
                continueLoadAssets();
            },
            [&](UpdateFailed const& error) {
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
                setSmallText("Failed Loader Resources", m_smallLabel);
                continueLoadAssets();
            }
        }, event->status);
    }

    void setupModResources() {
        log::debug("Loading mod resources");
        setSmallText("Loading mod resources", m_smallLabel);
        LoaderImpl::get()->updateResources(true);
        continueLoadAssets();
    }
    
    int getCurrentStep() {
        return m_geodeLoadStep + m_loadStep + 1 + LoaderImpl::get()->m_refreshedModCount;
    }

    int getTotalStep() {
        return m_totalSteps + m_totalMods;
    }

    void updateLoadingBar() {
        auto length = m_sliderGrooveXPos * getCurrentStep() / getTotalStep();
        m_sliderBar->setTextureRect({0, 0, length, m_sliderGrooveHeight});
    }

    void waitLoadAssets() {
        Loader::get()->queueInMainThread([this]() {
            loadAssets();
        });
    }

    void continueLoadAssets() {
        ++m_geodeLoadStep;
        Loader::get()->queueInMainThread([this]() {
            loadAssets();
        });
    }

    bool skipOnRefresh() {
        if (m_fromRefresh) {
            continueLoadAssets();
        }
        return !m_fromRefresh;
    }

    void loadAssets() {
        switch (m_geodeLoadStep) {
        case 0:
            if (skipOnRefresh()) setupLoadingMods();
            break;
        case 1:
            if (skipOnRefresh()) setupLoaderResources();
            break;
        case 2:
            setupModResources();
            break;
        case 3:
        default:
            setSmallText("Loading game resources", m_smallLabel);
            LoadingLayer::loadAssets();
            break;
        }
        updateLoadingBar();
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
        label->CCNode::setPosition(winSize.width / 2, 30.f);
        label->CCLabelBMFont::setScale(.45f);
        label->CCNode::setZOrder(99);
        this->CCNode::addChild(label);

        LoaderImpl::get()->updateResources(true);

        return true;
    }
};
