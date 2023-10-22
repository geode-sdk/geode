using namespace geode::prelude;

#include <Geode/modify/CCTouchDispatcher.hpp>

struct ForcePrioRevert : Modify<ForcePrioRevert, CCTouchDispatcher> {
    void addTargetedDelegate(CCTouchDelegate* delegate, int priority, bool swallowsTouches) {
        m_bForcePrio = false;
        if (m_pTargetedHandlers->count() > 0) {
            auto handler = static_cast<CCTouchHandler*>(m_pTargetedHandlers->objectAtIndex(0));
            priority = handler->getPriority() - 2;
        }

        CCTouchDispatcher::addTargetedDelegate(delegate, priority, swallowsTouches);
    }

    void incrementForcePrio(int num) {
        m_bForcePrio = false;
    }

    void decrementForcePrio(int num) {
        m_bForcePrio = false;
    }
};
