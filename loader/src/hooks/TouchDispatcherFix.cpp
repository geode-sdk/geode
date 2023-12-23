using namespace geode::prelude;

#include <Geode/modify/CCTouchDispatcher.hpp>

struct ForcePrioRevert : Modify<ForcePrioRevert, CCTouchDispatcher> {
    bool isUsingForcePrio() {
        return true;
    }

    void addTargetedDelegate(CCTouchDelegate* delegate, int priority, bool swallowsTouches) {
        m_forcePrio = -2;
        if (m_pTargetedHandlers->count() > 0) {
            auto handler = static_cast<CCTouchHandler*>(m_pTargetedHandlers->objectAtIndex(0));
            m_forcePrio = handler->getPriority() - 2;
        }

        CCTouchDispatcher::addTargetedDelegate(delegate, 0, swallowsTouches);
    }
};
