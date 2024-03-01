#include "ModPopup.hpp"

bool ModPopup::setup(ModSource&& src) {
    m_source = std::move(src);
    m_noElasticity = true;

    return true;
}

ModPopup* ModPopup::create(ModSource&& src) {
    auto ret = new ModPopup();
    if (ret && ret->init(358.f, 250.f, std::move(src))) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
