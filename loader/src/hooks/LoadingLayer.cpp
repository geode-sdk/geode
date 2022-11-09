#include <InternalLoader.hpp>
#include <array>

USE_GEODE_NAMESPACE();

#include <Geode/modify/LoadingLayer.hpp>

struct CustomLoadingLayer : Modify<CustomLoadingLayer, LoadingLayer> {
    bool m_updatingResources;

    CustomLoadingLayer() : m_updatingResources(false) {}

    bool init(bool fromReload) {
        if (!LoadingLayer::init(fromReload)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto count = Loader::get()->getAllMods().size();

        auto label = CCLabelBMFont::create(
            CCString::createWithFormat("Geode: Loaded %lu mods", static_cast<unsigned long>(count))
                ->getCString(),
            "goldFont.fnt"
        );
        label->setPosition(winSize.width / 2, 30.f);
        label->setScale(.45f);
        label->setID("geode-loaded-info");
        this->addChild(label);

        // verify loader resources
        if (!InternalLoader::get()->verifyLoaderResources(std::bind(
                &CustomLoadingLayer::updateResourcesProgress, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3
            ))) {
            // auto bg = CCScale9Sprite::create(
            //     "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
            // );
            // bg->setScale(.6f);
            // bg->setColor({ 0, 0, 0 });
            // bg->setOpacity(150);
            // bg->setPosition(winSize / 2);
            // this->addChild(bg);

            // m_fields->m_updatingResourcesBG = bg;

            // auto label = CCLabelBMFont::create("", "goldFont.fnt");
            // label->setScale(1.1f);
            // bg->addChild(label);

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

    void updateResourcesProgress(UpdateStatus status, std::string const& info, uint8_t progress) {
        switch (status) {
            case UpdateStatus::Progress: {
                this->setUpdateText("Downloading Resources: " + std::to_string(progress) + "%");
            } break;

            case UpdateStatus::Finished: {
                this->setUpdateText("Resources Downloaded");
                m_fields->m_updatingResources = false;
                this->loadAssets();
            } break;

            case UpdateStatus::Failed: {
                InternalLoader::platformMessageBox(
                    "Error updating resources",
                    "Unable to update Geode resources: " + info +
                        ".\n"
                        "The game will be loaded as normal, but please be aware "
                        "that it may very likely crash."
                );
                this->setUpdateText("Resource Download Failed");
                m_fields->m_updatingResources = false;
                this->loadAssets();
            } break;
        }
    }

    void loadAssets() {
        if (m_fields->m_updatingResources) {
            return;
        }
        LoadingLayer::loadAssets();
    }
};
