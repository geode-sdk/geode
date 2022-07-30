#include <Geode/Modify.hpp>


// this is the fix for the dynamic_cast problems

using namespace cocos2d;
using namespace geode::modifier;

#if defined(GEODE_IS_IOS) || defined(GEODE_IS_MACOS)
namespace geode::fixes {
	using namespace geode::cast;

#define HandlerFixFor(CCUtility)                                                                        \
class $modify(CCUtility##HandlerTypeinfoFix, CCUtility##Handler) {                                      \
	void destructor() {                                                                                 \
		if (m_pDelegate) {                                                                              \
			cocos2d::CCObject* pObject = base_cast<cocos2d::CCObject*>(m_pDelegate);                    \
			if (pObject) {                                                                              \
				pObject->release();                                                                     \
			}                                                                                           \
			/*reinterpret_cast<CCObject*>(this)->~CCObject();*/                                             \
		}                                                                                               \
	}                                                                                                   \
																										\
	bool initWithDelegate(cocos2d::CCUtility##Delegate *pDelegate) {                                    \
		cocos2d::CCObject* pObject = base_cast<cocos2d::CCObject*>(pDelegate);                          \
		if (pObject) {                                                                                  \
			m_pDelegate = pDelegate;                                                                    \
			pObject->retain();                                                                          \
			return true;                                                                                \
		}                                                                                               \
		return false;                                                                                   \
	}                                                                                                   \
																										\
	static cocos2d::CCUtility##Handler* handlerWithDelegate(cocos2d::CCUtility##Delegate *pDelegate) {  \
		cocos2d::CCUtility##Handler* pHandler = new cocos2d::CCUtility##Handler();                      \
																										\
		if (pHandler) {                                                                                 \
			if (pHandler->initWithDelegate(pDelegate)) {                                                \
				pHandler->autorelease();                                                                \
			}                                                                                           \
			else {                                                                                      \
				CC_SAFE_RELEASE_NULL(pHandler);                                                         \
				pHandler = CCUtility##Handler::handlerWithDelegate(pDelegate);                          \
			}                                                                                           \
		}                                                                                               \
																										\
		return pHandler;                                                                                \
	}                                                                                                   \
} 

HandlerFixFor(CCKeypad);
HandlerFixFor(CCKeyboard);
HandlerFixFor(CCMouse);

class $modify(CCTargetedTouchHandlerTypeinfoFix, CCTargetedTouchHandler) {
	void destructor() {
		if (m_pDelegate) {
			cocos2d::CCObject* pObject = base_cast<cocos2d::CCObject*>(m_pDelegate);
			if (pObject) {
				// the entire destructor
				pObject->release();
				CC_SAFE_RELEASE(m_pClaimedTouches);
			}
			/*reinterpret_cast<CCObject*>(this)->~CCObject();*/
		}
	}

	bool initWithDelegate(cocos2d::CCTouchDelegate *pDelegate, int nPriority, bool bSwallow) {
		cocos2d::CCObject* pObject = base_cast<cocos2d::CCObject*>(pDelegate);
		if (pObject) {
			m_pDelegate = pDelegate;
			pObject->retain();

			m_nPriority = nPriority;
			m_nEnabledSelectors = 0;

			m_pClaimedTouches = new cocos2d::CCSet();
			m_bSwallowsTouches = bSwallow;
			return true;
		}
		return false;
	}

	static cocos2d::CCTargetedTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate *pDelegate, int nPriority, bool bSwallow) {
		cocos2d::CCTargetedTouchHandler* pHandler = new cocos2d::CCTargetedTouchHandler();

		if (pHandler) {
			if (pHandler->initWithDelegate(pDelegate, nPriority, bSwallow)) {
				pHandler->autorelease();
			}
			else {
				CC_SAFE_RELEASE_NULL(pHandler);
				pHandler = CCTargetedTouchHandler::handlerWithDelegate(pDelegate, nPriority, bSwallow);
			}
		}

		return pHandler;
	}
};

class $modify(CCStandardTouchHandlerTypeinfoFix, CCStandardTouchHandler) {
	void destructor() {
		if (m_pDelegate) {
			cocos2d::CCObject* pObject = base_cast<cocos2d::CCObject*>(m_pDelegate);
			if (pObject) {
				// the entire destructor
				pObject->release();
			}
			/*reinterpret_cast<CCObject*>(this)->~CCObject();*/
		}
	}

	bool initWithDelegate(cocos2d::CCTouchDelegate *pDelegate, int nPriority) {
		cocos2d::CCObject* pObject = base_cast<cocos2d::CCObject*>(pDelegate);
		if (pObject) {
			m_pDelegate = pDelegate;
			pObject->retain();

			m_nPriority = nPriority;
			m_nEnabledSelectors = 0;
			return true;
		}
		return false;
	}

	static cocos2d::CCStandardTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate *pDelegate, int nPriority) {
		cocos2d::CCStandardTouchHandler* pHandler = new cocos2d::CCStandardTouchHandler();
		if (pHandler) {
			if (pHandler->initWithDelegate(pDelegate, nPriority)) {
				pHandler->autorelease();
			}
			else {
				CC_SAFE_RELEASE_NULL(pHandler);
				pHandler = CCStandardTouchHandler::handlerWithDelegate(pDelegate, nPriority);
			}
		}

		return pHandler;
	}
};

} // geode::fixes

#endif
