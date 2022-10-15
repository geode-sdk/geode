#include <cocos2d.h>

using namespace cocos2d;

#pragma warning(push)
#pragma warning(disable: 4273)

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

#pragma warning(pop)
