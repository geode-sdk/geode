#include <Geode/loader/Loader.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/GameManager.hpp>

USE_GEODE_NAMESPACE();

class $modify(GameManager) {
	void reloadAllStep2() {
		GameManager::reloadAllStep2();
		Loader::get()->updateResourcePaths();
	}
};

class $modify(LoadingLayer) {
	bool init(bool fromReload) {
		// this is in case the user refreshes texture quality at runtime
		// resources are loaded first so things like texture packs override 
		// the GD textures
		Loader::get()->waitForModsToBeLoaded();
		Loader::get()->updateResources();
		return LoadingLayer::init(fromReload);
	}
};
