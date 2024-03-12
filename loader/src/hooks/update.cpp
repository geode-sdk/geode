#include <lodaer/LoaderImpl.hpp>
#include <Geode/modify/CCScheduler.hpp>

using geode::prelude::Modify;
using geode::prelude::CCScheduler;

struct FunctionQueue : Modify<FunctionQueue, CCScheduler> {
    void update(float dt) {
        LoaderImpl::get()->executeMainThreadQueue();
        return CCScheduler::update(dt);
    }
};
