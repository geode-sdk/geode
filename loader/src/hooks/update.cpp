#include <loader/LoaderImpl.hpp>

using namespace geode::prelude;

#include <Geode/modify/CCScheduler.hpp>

struct FunctionQueue : Modify<FunctionQueue, CCScheduler> {
    void update(float dt) {
        LoaderImpl::get()->executeMainThreadQueue(false);
        CCScheduler::update(dt);
        LoaderImpl::get()->executeMainThreadQueue(true);
    }
};
