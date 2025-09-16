#include <Geode/Geode.hpp>
#ifndef GEODE_IS_IOS
#include <Geode/utils/VMTHookManager.hpp>
#include <Geode/modify/Traits.hpp>
#include <Geode/modify/CCMenu.hpp>

using namespace geode::prelude;

struct CCMenuSelectedFix : Modify<CCMenuSelectedFix, CCMenu> {
    bool initWithArray(CCArray* arr) {
        if (!CCMenu::initWithArray(arr)) return false;

        if (!exact_cast<CCMenu*>(this)) {
            return true;
        }

        auto hook = VMTHookManager::get().addHook<
            geode::modifier::ResolveC<CCMenuSelectedFix>::func(&CCMenuSelectedFix::removeAllChildren)
        >(this, "cocos2d::CCMenu::removeAllChildren");

        if (!hook) {
            log::error("Failed to hook CCMenu::removeAllChildren: {}", hook.unwrapErr());
        }

        return true;
    }

    $override
    void removeAllChildren() {
        // CCMenu itself overrides `removeChild` to reset `m_pSelectedItem` and prevent UB.
        // `removeAllChildren` is not overriden and never calls `removeChild`, thus not resetting this member.
        // This fixes that, eliminating UB that can happen when spam clicking a button that removes itself with this method.
        m_pSelectedItem = nullptr;
        CCNode::removeAllChildren();
    }
};
#endif
