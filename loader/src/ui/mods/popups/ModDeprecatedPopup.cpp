#include "ModDeprecatedPopup.hpp"

bool ModDeprecatedPopup::init(Mod* mod, server::ServerModDeprecation dep) {
    if (!GeodePopup::init(350.f, 250.f))
        return false;

    this->setTitle("Deprecated");

    m_deprecation = std::move(dep);

    return true;
}

ModDeprecatedPopup* ModDeprecatedPopup::create(Mod* mod, server::ServerModDeprecation dep) {
    auto ret = new ModDeprecatedPopup();
    if (ret->init(mod, std::move(dep))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
