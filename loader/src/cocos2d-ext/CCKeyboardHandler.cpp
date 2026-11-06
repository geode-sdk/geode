#include <Geode/platform/cplatform.h>

#ifdef GEODE_IS_IOS

#include <Geode/mapping/CCKeyboardHandler.hpp>

using namespace cocos2d;

CCKeyboardHandler::~CCKeyboardHandler() {}

CCKeyboardDelegate* CCKeyboardHandler::getDelegate()
{
    return m_pDelegate;
}

CCKeyboardHandler* CCKeyboardHandler::handlerWithDelegate(CCKeyboardDelegate* pDelegate)
{
    CCKeyboardHandler* handler = new CCKeyboardHandler();

    if (handler->initWithDelegate(pDelegate))
    {
        handler->autorelease();
        return handler;
    }

    handler->release();
    return nullptr;
}

bool CCKeyboardHandler::initWithDelegate(CCKeyboardDelegate* pDelegate)
{
    m_pDelegate = pDelegate;
    return true;
}

void CCKeyboardHandler::setDelegate(CCKeyboardDelegate* pDelegate)
{
    m_pDelegate = pDelegate;
}

#endif
