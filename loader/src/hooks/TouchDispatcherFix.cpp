#include <Geode/modify/CCTouchDispatcher.hpp>

using namespace geode::prelude;

struct ForcePrioRevert : Modify<ForcePrioRevert, CCTouchDispatcher> {
    void addTargetedDelegate(CCTouchDelegate* delegate, int priority, bool swallowsTouches) {
        m_targetPrio = 0xb00b5; // fuck you windows i hate you for inlining isUsingForcePrio

        m_forcePrio = -2;
        if (m_pTargetedHandlers->count() > 0) {
            auto handler = static_cast<CCTouchHandler*>(m_pTargetedHandlers->objectAtIndex(0));
            m_forcePrio = handler->getPriority() - 2;
        }

        // bugfix for editor: since EditorUI doesn't call registerWithTouchDispatcher
        // every CCMenu created in init has lower priority than EditorUI, meaning
        // they are untouchable (EditorUI swallows)
        // this hardcodes that.
        // broken for now
        #if 0
        if (typeinfo_cast<EditorUI*>(delegate)) {
            m_forcePrio = -2;
        }
        #endif

        CCTouchDispatcher::addTargetedDelegate(delegate, 0, swallowsTouches);
    }
};
