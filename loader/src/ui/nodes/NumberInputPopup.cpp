#include <Geode/ui/NumberInputPopup.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class NumberInputPopup::Impl final {
public:
};

NumberInputPopup::NumberInputPopup() : m_impl(std::make_unique<Impl>()) {}
NumberInputPopup::~NumberInputPopup() {}

bool NumberInputPopup::init(int64_t current, int64_t min, int64_t max, std::string title, ZStringView button, float inputWidth) {
    if (!Popup::init({210.f, 125.f})) return false;

    this->setTitle(std::move(title));

    return true;
}

NumberInputPopup* NumberInputPopup::create(int64_t current, int64_t min, int64_t max, std::string title, ZStringView button, float inputWidth) {
    auto ret = new NumberInputPopup();
    if (ret->init(current, min, max, std::move(title), button, inputWidth)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}