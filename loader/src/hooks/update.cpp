#include <loader/LoaderImpl.hpp>
#include <cpp-reactive.hpp>
using namespace geode::prelude;

#include <Geode/modify/CCScheduler.hpp>

struct FunctionQueue : Modify<FunctionQueue, CCScheduler> {
    void update(float dt) {
        LoaderImpl::get()->executeMainThreadQueue();
        cppreactive::ObserverStack::shared()->update();

        return CCScheduler::update(dt);
    }
};
