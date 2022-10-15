#pragma once

#include <cocos2d.h>

USE_GEODE_NAMESPACE();

class HotReloadLayer : public CCLayer {
protected:
	bool init(std::string const& name);

public:
	static HotReloadLayer* create(std::string const& name);
	static HotReloadLayer* scene(std::string const& name);
};
