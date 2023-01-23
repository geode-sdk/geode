#include <Geode/loader/Loader.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/GameManager.hpp>

USE_GEODE_NAMESPACE();

struct ResourcesUpdate : Modify<ResourcesUpdate, LoadingLayer> {
    void loadAssets() {
        LoadingLayer::loadAssets();
        // this is in case the user refreshes texture quality at runtime
        if (m_loadStep == 10) {
            Loader::get()->updateResources();
        }
    }
};
