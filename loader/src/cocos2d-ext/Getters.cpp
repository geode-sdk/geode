#include <cocos2d.h>

using namespace cocos2d;

#pragma warning(push)
#pragma warning(disable: 4273)

CCKeyboardDispatcher* CCKeyboardDispatcher::get() {
    return CCDirector::get()->getKeyboardDispatcher();
}

#pragma warning(pop)
