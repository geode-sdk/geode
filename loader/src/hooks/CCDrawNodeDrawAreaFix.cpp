#include <Geode/Geode.hpp>
#include <Geode/modify/CCDrawNode.hpp>

using namespace geode::prelude;

class $modify(CCDrawNode) {
    // TODO: move to ctor when modify with it is fixed
    $override
    bool init() {
        // robtop always calls either disableDrawArea or enableDrawArea,
        // which set m_bUseArea,
        // some mods dont do that, leaving m_bUseArea uninitialized
        // while m_rDrawArea is also uninitialized,
        // then if m_bUseArea ends up not 0 (very often)
        // the draw area checks in drawnodes funcs wont ever pass,
        // causing drawnodes to outright not work sometimes
        m_bUseArea = false;
        return CCDrawNode::init();
    }
};
