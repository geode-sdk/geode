
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <array>
#include <fmt/format.h>
#include <loader/LoaderImpl.hpp>

using namespace geode::prelude;

struct CustomLoadingLayer : Modify<CustomLoadingLayer, LoadingLayer> {
    CCLabelBMFont* m_smallLabel = nullptr;
    int m_geodeLoadStep = 0;

    void updateLoadedModsLabel() {
        auto allMods = Loader::get()->getAllMods();
        auto count = std::count_if(allMods.begin(), allMods.end(), [&](auto& item) {
            return item->isEnabled();
        });
        auto totalCount = std::count_if(allMods.begin(), allMods.end(), [&](auto& item) {
            return item->shouldLoad();
        });
        auto str = fmt::format("Geode: Loaded {}/{} mods", count, totalCount);
        this->setSmallText(str);
    }

    void setSmallText(std::string const& text) {
        m_fields->m_smallLabel->setString(text.c_str());
    }

    // hook
    bool init(bool fromReload) {
        CCFileUtils::get()->updatePaths();

        if (!LoadingLayer::init(fromReload)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        m_fields->m_smallLabel = CCLabelBMFont::create("", "goldFont.fnt");
        m_fields->m_smallLabel->setPosition(winSize.width / 2, 30.f);
        m_fields->m_smallLabel->setScale(.45f);
        m_fields->m_smallLabel->setID("geode-small-label");
        this->addChild(m_fields->m_smallLabel);

        return true;
    }

    void setupLoadingMods() {
        if (Loader::get()->getLoadingState() != Loader::LoadingState::Done) {
            this->updateLoadedModsLabel();
            this->waitLoadAssets();
        }
        else {
            this->continueLoadAssets();
        }
    }

    void setupLoaderResources() {
        // verify loader resources
        if (!LoaderImpl::get()->verifyLoaderResources()) {
            log::debug("Downloading Loader Resources");
            this->setSmallText("Downloading Loader Resources");
            this->addChild(EventListenerNode<ResourceDownloadFilter>::create(
                this, &CustomLoadingLayer::updateResourcesProgress
            ));
        }
        else {
            log::debug("Loading Loader Resources");
            this->setSmallText("Loading Loader Resources");
            LoaderImpl::get()->updateSpecialFiles();
            this->continueLoadAssets();
        }
    }

    void updateResourcesProgress(ResourceDownloadEvent* event) {
        std::visit(makeVisitor {
            [&](UpdateProgress const& progress) {
                this->setSmallText(fmt::format(
                    "Downloading Loader Resources: {}%", progress.first
                ));
            },
            [&](UpdateFinished) {
                log::debug("Downloaded Loader Resources");
                this->setSmallText("Downloaded Loader Resources");
                this->continueLoadAssets();
            },
            [&](UpdateFailed const& error) {
                log::debug("Failed Loader Resources");
                LoaderImpl::get()->platformMessageBox(
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
        Loader::get()->updateResources(true);
        this->continueLoadAssets();
    }
    
    int getCurrentStep() {
        return m_fields->m_geodeLoadStep + m_loadStep + 1;
    }

    int getTotalStep() {
        return 18;
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
