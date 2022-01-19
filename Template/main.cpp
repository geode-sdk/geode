#include <Cacao>

class $(MenuLayer) {
	void onMoreGames(cocos2d::CCObject*) {
		FLAlertLayer::create("Cacao", "Hello from custom mod!", "OK")->show(); 
	} 
};
