#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/CCTouchDispatcher.hpp>

struct ForcePrioRevert : Modify<ForcePrioRevert, CCTouchDispatcher> {
    void addTargetedDelegate(CCTouchDelegate* delegate, int priority, bool swallowsTouches) {
        m_targetPrio = 0xb00b5; // fuck you windows i hate you for inlining isUsingForcePrio

        m_forcePrio = -1;
        if (m_pHandlersToAdd->count() > 0) {
            auto handler = static_cast<CCTouchHandler*>(m_pHandlersToAdd->objectAtIndex(0));
            m_forcePrio = std::min(m_forcePrio, handler->getPriority() - 1);
        }
        if (m_pTargetedHandlers->count() > 0) {
            auto handler = static_cast<CCTouchHandler*>(m_pTargetedHandlers->objectAtIndex(0));
            m_forcePrio = std::min(m_forcePrio, handler->getPriority() - 1);
        }

        // bugfix for editor: since EditorUI doesn't call registerWithTouchDispatcher
        // every CCMenu created in init has lower priority than EditorUI, meaning
        // they are untouchable (EditorUI swallows)
        // this hardcodes that.
        if (typeinfo_cast<EditorUI*>(delegate) || typeinfo_cast<UILayer*>(delegate)) {
            m_forcePrio = -1;
        }

        CCTouchDispatcher::addTargetedDelegate(delegate, 0, swallowsTouches);
    }
};
