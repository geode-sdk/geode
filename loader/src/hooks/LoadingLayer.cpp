
#include <InternalLoader.hpp>
#include <array>
#include <Geode/modify/LoadingLayer.hpp>
#include <fmt/format.h>

USE_GEODE_NAMESPACE();

struct CustomLoadingLayer : Modify<CustomLoadingLayer, LoadingLayer> {
    bool m_updatingResources;
    EventListener<ResourceDownloadFilter> m_resourceListener;

    CustomLoadingLayer() : m_updatingResources(false) {}

    bool init(bool fromReload) {
        if (!LoadingLayer::init(fromReload)) return false;

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

        m_fields->m_resourceListener.bind(std::bind(
            &CustomLoadingLayer::updateResourcesProgress,
            this, std::placeholders::_1
        ));

        // verify loader resources
        if (!InternalLoader::get()->verifyLoaderResources()) {
            m_fields->m_updatingResources = true;
            this->setUpdateText("Downloading Resources");
        }

        return true;
    }

    void setUpdateText(std::string const& text) {
        m_textArea->setString(text.c_str());
        // m_fields->m_updatingResources->setString(text.c_str());
        // m_fields->m_updatingResourcesBG->setContentSize({
        //     m_fields->m_updatingResources->getScaledContentSize().width + 30.f,
        //     50.f
        // });
        // m_fields->m_updatingResources->setPosition(
        //     m_fields->m_updatingResourcesBG->getContentSize() / 2
        // );
    }

    void updateResourcesProgress(ResourceDownloadEvent* event) {
        auto status = event->getStatus();
        if (std::holds_alternative<UpdateProgress>(status)) {
            auto prog = std::get<UpdateProgress>(status);
            this->setUpdateText("Downloading Resources: " + std::to_string(prog.first) + "%");
        }
        else if (std::holds_alternative<UpdateFinished>(status)) {
            this->setUpdateText("Resources Downloaded");
            m_fields->m_updatingResources = false;
            this->loadAssets();
        }
        else {
            InternalLoader::platformMessageBox(
                "Error updating resources",
                "Unable to update Geode resources: " + 
                std::get<UpdateError>(status) + ".\n"
                "The game will be loaded as normal, but please be aware "
                "that it may very likely crash."
            );
            this->setUpdateText("Resource Download Failed");
            m_fields->m_updatingResources = false;
            this->loadAssets();
        }
    }

    void loadAssets() {
        if (m_fields->m_updatingResources) {
            return;
        }
        LoadingLayer::loadAssets();
    }
};
