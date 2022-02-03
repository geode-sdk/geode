#pragma once

#include "../utils/general.hpp"
#include "../utils/casts.hpp"
#include <cocos2d.h>
#include <string>

namespace cocos2d {
    class CCNode;
}

struct PlayLayer;
struct EditorUI;
struct LevelEditorLayer;

namespace geode::helper {
	
	/**
	 * A recursive version of CCMenu that can traverse 
	 * and detect all of it's children's touches
	 */
	class RecursiveMenu : cocos2d::CCMenu {
	public:
		static RecursiveMenu* create();
		static RecursiveMenu* createWithArray(cocos2d::CCArray* arrayOfItems);

	protected:
		virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
		virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

		cocos2d::CCMenuItem* itemForTouch(cocos2d::CCTouch* touch);
		cocos2d::CCMenuItem* itemForTouch(cocos2d::CCTouch* touch, cocos2d::CCNode* child);
	};
}
