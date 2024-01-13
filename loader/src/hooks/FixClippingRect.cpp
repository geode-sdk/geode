#include <Geode/modify/GameToolbox.hpp>

using namespace geode::prelude;

struct MyGameToolbox : Modify<MyGameToolbox, GameToolbox> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("FixClippingRect disabled")
    static void preVisitWithClippingRect(CCNode* node, CCRect rect) {
        if (node->isVisible()) {
            glEnable(0xc11);
            if (node->getParent()) {
                // rob messed this up somehow causing nested ScrollLayers to be not clipped properly
                rect.origin = node->getParent()->convertToWorldSpace(rect.origin);
            }
            CCEGLView::get()->setScissorInPoints(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
        } 
    }
};
