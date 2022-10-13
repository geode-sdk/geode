#include <Geode/loader/Loader.hpp>

USE_GEODE_NAMESPACE();

#include <Geode/modify/GameManager.hpp>
class $modify(GameManager) {
	void reloadAllStep2() {
		GameManager::reloadAllStep2();
		Loader::get()->updateResourcePaths();
	}
};

#include <Geode/modify/LoadingLayer.hpp>
class $modify(LoadingLayer) {
	void loadAssets() {
		LoadingLayer::loadAssets();
		if (this->m_loadStep == 5) {
			Loader::get()->updateResourcePaths();
			Loader::get()->updateResources();
		}
	}
};
