
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <array>
#include <fmt/format.h>
#include <loader/LoaderImpl.hpp>

using namespace geode::prelude;

struct CustomLoadingLayer : Modify<CustomLoadingLayer, LoadingLayer> {
    CCLabelBMFont* m_loadedModsLabel;
    bool m_updatingResources;

    CustomLoadingLayer() : m_loadedModsLabel(nullptr), m_updatingResources(false) {}

    void updateLoadedModsLabel() {
        auto allMods = Loader::get()->getAllMods();
        auto count = std::count_if(allMods.begin(), allMods.end(), [&](auto& item) {
            return item->isLoaded();
        });
        auto str = fmt::format("Geode: Loaded {}/{} mods", count, allMods.size());
        m_fields->m_loadedModsLabel->setCString(str.c_str());
    }

    bool init(bool fromReload) {
        CCFileUtils::get()->updatePaths();

        if (!LoadingLayer::init(fromReload)) return false;

        if (fromReload) return true;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        m_fields->m_loadedModsLabel = CCLabelBMFont::create("Geode: Loaded 0/0 mods", "goldFont.fnt");
        m_fields->m_loadedModsLabel->setPosition(winSize.width / 2, 30.f);
        m_fields->m_loadedModsLabel->setScale(.45f);
        m_fields->m_loadedModsLabel->setID("geode-loaded-info");
        this->addChild(m_fields->m_loadedModsLabel);
        this->updateLoadedModsLabel();

        // fields have unpredictable destructors
        this->addChild(EventListenerNode<ResourceDownloadFilter>::create(
            this, &CustomLoadingLayer::updateResourcesProgress
        ));

        // verify loader resources
        if (!LoaderImpl::get()->verifyLoaderResources()) {
            m_fields->m_updatingResources = true;
            this->setUpdateText("Downloading Resources");
        }
        else {
            LoaderImpl::get()->updateSpecialFiles();
        }

        return true;
    }

    void setUpdateText(std::string const& text) {
        m_textArea->setString(text.c_str());
    }

    void updateResourcesProgress(ResourceDownloadEvent* event) {
        std::visit(makeVisitor {
            [&](UpdateProgress const& progress) {
                this->setUpdateText(fmt::format(
                    "Downloading Resources: {}%", progress.first
                ));
            },
            [&](UpdateFinished) {
                this->setUpdateText("Resources Downloaded");
                m_fields->m_updatingResources = false;
                this->loadAssets();
            },
            [&](UpdateFailed const& error) {
                LoaderImpl::get()->platformMessageBox(
                    "Error updating resources",
                    error + ".\n"
                    "You will have to install resources manually by downloading resources.zip "
                    "from the latest release on GitHub: "
                    "https://github.com/geode-sdk/geode/releases/latest.\n"
                    "The game will be loaded as normal, but please be aware "
                    "that it is very likely to crash. "
                );
                this->setUpdateText("Resource Download Failed");
                m_fields->m_updatingResources = false;
                this->loadAssets();
            }
        }, event->status);
    }

    void loadAssets() {
        if (Loader::get()->getLoadingState() != Loader::LoadingState::Done) {
            this->updateLoadedModsLabel();
            Loader::get()->queueInGDThread([this]() {
                this->loadAssets();
            });
            return;
        }
        if (m_fields->m_updatingResources) {
            return;
        }
        LoadingLayer::loadAssets();
    }
};
