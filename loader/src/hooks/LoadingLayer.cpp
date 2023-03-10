
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <array>
#include <fmt/format.h>
#include <loader/LoaderImpl.hpp>

using namespace geode::prelude;

struct CustomLoadingLayer : Modify<CustomLoadingLayer, LoadingLayer> {
    bool m_updatingResources;

    CustomLoadingLayer() : m_updatingResources(false) {}

    bool init(bool fromReload) {
        if (!fromReload) {
            Loader::get()->waitForModsToBeLoaded();
        }

        CCFileUtils::get()->updatePaths();

        if (!LoadingLayer::init(fromReload)) return false;
        
        if (!fromReload) {
            auto winSize = CCDirector::sharedDirector()->getWinSize();

            auto count = Loader::get()->getAllMods().size();

            auto label = CCLabelBMFont::create(
                fmt::format("Geode: Loaded {} mods", count).c_str(),
                "goldFont.fnt"
            );
            label->setPosition(winSize.width / 2, 30.f);
            label->setScale(.45f);
            label->setID("geode-loaded-info");
            this->addChild(label);

            // fields have unpredictable destructors
            this->addChild(EventListenerNode<ResourceDownloadFilter>::create(
                this, &CustomLoadingLayer::updateResourcesProgress
            ));

            // verify loader resources
            if (!LoaderImpl::get()->verifyLoaderResources()) {
                m_fields->m_updatingResources = true;
                this->setUpdateText("Downloading Resources");
            }
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
        if (m_fields->m_updatingResources) {
            return;
        }
        LoadingLayer::loadAssets();
    }
};
