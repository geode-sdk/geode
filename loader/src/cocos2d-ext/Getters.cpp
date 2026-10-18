#include <Geode/cocos/CCDirector.h>
#include <Geode/cocos/CCScheduler.h>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCScene.h>
#include <Geode/cocos/platform/CCFileUtils.h>
#include <Geode/cocos/platform/win32/CCApplication.h>
#include <Geode/cocos/platform/win32/CCEGLView.h>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#include <Geode/cocos/sprite_nodes/CCSpriteFrameCache.h>
#include <Geode/cocos/textures/CCTextureCache.h>
#include <Geode/cocos/touch_dispatcher/CCTouchDispatcher.h>

using namespace cocos2d;

#pragma warning(push)
#pragma warning(disable : 4273)

CCKeyboardDispatcher* CCKeyboardDispatcher::get() {
    return CCDirector::get()->getKeyboardDispatcher();
}

CCTouchDispatcher* CCTouchDispatcher::get() {
    return CCDirector::sharedDirector()->getTouchDispatcher();
}

CCApplication* CCApplication::get() {
    return CCApplication::sharedApplication();
}

CCDirector* CCDirector::get() {
    return CCDirector::sharedDirector();
}

CCEGLView* CCEGLView::get() {
    return CCDirector::sharedDirector()->getOpenGLView();
}

CCFileUtils* CCFileUtils::get() {
    return CCFileUtils::sharedFileUtils();
}

CCSpriteFrameCache* CCSpriteFrameCache::get() {
    return CCSpriteFrameCache::sharedSpriteFrameCache();
}

CCTextureCache* CCTextureCache::get() {
    return CCTextureCache::sharedTextureCache();
}

CCScene* CCScene::get() {
    return CCDirector::get()->getRunningScene();
}

CCScheduler* CCScheduler::get() {
    return CCDirector::get()->getScheduler();
}

#pragma warning(pop)
