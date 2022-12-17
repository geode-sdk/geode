#include <loader/LoaderImpl.hpp>

USE_GEODE_NAMESPACE();

#include <Geode/modify/CCScheduler.hpp>

struct FunctionQueue : Modify<FunctionQueue, CCScheduler> {
    void update(float dt) {
        LoaderImpl::get()->executeGDThreadQueue();
        return CCScheduler::update(dt);
    }
};
