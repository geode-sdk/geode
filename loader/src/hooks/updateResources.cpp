#include <Geode/loader/Loader.hpp>
#include <Geode/modify/LoadingLayer.hpp>

USE_GEODE_NAMESPACE();

class $modify(LoadingLayer) {
	void loadAssets() {
		LoadingLayer::loadAssets();
		// this is in case the user refreshes texture quality at runtime
		if (this->m_loadStep == 10) {
			Loader::get()->updateResources();
		}
	}
};
