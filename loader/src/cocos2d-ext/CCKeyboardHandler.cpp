#include <cocos2d.h>

using namespace cocos2d;

#ifdef GEODE_IS_IOS

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
    //dynamic_cast<CCObject*>(pDelegate)->retain();

    return true;
}

void CCKeyboardHandler::setDelegate(CCKeyboardDelegate* pDelegate)
{
    /*if (pDelegate)
    {
        dynamic_cast<CCObject*>(pDelegate)->retain();
    }

    if (m_pDelegate)
    {
        dynamic_cast<CCObject*>(m_pDelegate)->release();
    }*/

    m_pDelegate = pDelegate;
}

#endif
