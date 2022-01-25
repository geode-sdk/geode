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
		static RecursiveMenu* createWithArray(CCArray* arrayOfItems);

	protected:
		virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
		virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override;

		CCMenuItem* itemForTouch(CCTouch* touch);
		CCMenuItem* itemForTouch(CCTouch* touch, CCNode* child);
	};
}
