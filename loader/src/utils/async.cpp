#include <Geode/utils/async.hpp>

namespace geode::async {

arc::Runtime& runtime() {
    static auto instance = arc::Runtime::create(4);
    return *instance;
}

}