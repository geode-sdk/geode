#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>

using namespace geode::prelude;

/// ~CCDirector first destroys the touch dispatcher, and only pops the autorelease pool afterwards.
/// This means that if there's any node currently in the autorelease pool that has touch enabled,
/// it will try to unregister itself from the destroyed touch dispatcher and likely crash.
///
/// This fix simply retains the touch dispatcher until it's safe to destroy it.
struct CCDirectorTouchUBFix : Modify<CCDirectorTouchUBFix, CCDirector> {
    void destructor() {
        Ref _{m_pTouchDispatcher};
        CCDirector::~CCDirector();
    }
};
