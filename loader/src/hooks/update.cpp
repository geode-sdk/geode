#include <InternalLoader.hpp>

USE_GEODE_NAMESPACE();
// clang-format off
#include <Geode/modify/CCScheduler.hpp>
class $modify(CCScheduler) {
    void update(float dt) {
        InternalLoader::get()->executeGDThreadQueue();
        return CCScheduler::update(dt);
    }
};
// clang-format on