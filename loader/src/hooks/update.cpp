#include <Geode/Modify.hpp>
#include <InternalLoader.hpp>

USE_GEODE_NAMESPACE();

class $modify(CCScheduler) {
    void update(float dt) {
        InternalLoader::get()->executeGDThreadQueue();
        return CCScheduler::update(dt);
    }
};
