#include <Geode/ui/NumberInputPopup.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

class NumberInputPopup::Impl final {
public:
    SubmitCallback m_callback = nullptr;

    TextInput* m_input = nullptr;

    double m_max = 1.0;
    double m_min = .0;

    double m_step = 1.0;

    size_t m_precision = 0;

    std::string stepNumStr(std::string_view current, double step) {
        auto currentRes = numFromString<double>(current);
        if (currentRes.isErr()) return std::string{current};

        auto cur = std::move(currentRes).unwrap();
        auto to = cur + step;

        if (to < m_min) return numToString(m_min, m_precision);
        if (to > m_max) return numToString(m_max, m_precision);

        return numToString(to, m_precision);
    }
};

NumberInputPopup::NumberInputPopup() : m_impl(std::make_unique<Impl>()) {}
NumberInputPopup::~NumberInputPopup() {}

bool NumberInputPopup::init(double current, double min, double max, std::string title, ZStringView btnText, ZStringView btnBg, ZStringView bg, CircleBaseColor closeBtnColor, float inputWidth) {
    if (!Popup::init({210.f, 125.f}, bg.c_str())) return false;

    m_impl->m_max = max;
    m_impl->m_min = min;

    this->setTitle(std::move(title));
    this->setCloseButtonSpr(CircleButtonSprite::createWithSpriteFrameName("close.png"_spr, .875f, closeBtnColor), .825f);

    auto maxWidth = m_mainLayer->getScaledContentWidth() - 50.f;

    m_title->setLimitLabelWidth(maxWidth - 12.5f, .7f);

    auto container = CCNode::create();
    container->setID("input-container");
    container->setAnchorPoint({.5f, .5f});
    container->setLayout(
        RowLayout::create()
            ->setGap(5.f)
            ->setAutoScale(false)
            ->setAutoGrowAxis(.1f)
            ->setGrowCrossAxis(true)
    );
    m_mainLayer->addChildAtPosition(container, Anchor::Center);

    m_impl->m_input = TextInput::create((inputWidth <= maxWidth) ? inputWidth : maxWidth, "Num");
    m_impl->m_input->setID("page-input");
    m_impl->m_input->setCommonFilter(CommonFilter::Float);
    m_impl->m_input->setString(numToString(current, m_impl->m_precision));

    auto prev = Button::createWithSpriteFrameName(
        "edit_leftBtn_001.png",
        [this](auto) {
            m_impl->m_input->setString(m_impl->stepNumStr(this->getValueString(), -m_impl->m_step));
        }
    );
    prev->setID("decrement-button");
    prev->setScale(.95f);

    auto next = Button::createWithSpriteFrameName(
        "edit_rightBtn_001.png",
        [this](auto) {
            m_impl->m_input->setString(m_impl->stepNumStr(this->getValueString(), m_impl->m_step));
        }
    );
    next->setID("increment-button");
    next->setScale(.95f);

    container->addChild(prev);
    container->addChild(m_impl->m_input);
    container->addChild(next);

    container->updateLayout();

    auto reset = Button::createWithSpriteFrameName(
        "GJ_resetBtn_001.png",
        [this, current](auto) {
            m_impl->m_input->setString(numToString(current, m_impl->m_precision));
        }
    );
    reset->setScale(.875f);
    m_mainLayer->addChildAtPosition(reset, Anchor::TopRight, {-20.f, -20.f});

    auto submit = Button::createWithNode(
        ButtonSprite::create(btnText.c_str(), "goldFont.fnt", btnBg.c_str(), .8f), [this](auto) {
            auto res = numFromString<double>(std::string{this->getValueString()});
            if (res.isErr()) {
                Notification::create("Invalid number", NotificationIcon::Error)->show();
                return;
            }

            double num = std::move(res).unwrap();
            if (num > this->getMax()) num = this->getMax();
            if (num <= this->getMin()) num = this->getMin();

            m_impl->m_callback ? m_impl->m_callback(this, num) : this->onClose(nullptr);
        }
    );
    submit->setID("submit-button");
    submit->setScale(.825f);
    m_mainLayer->addChildAtPosition(submit, Anchor::Bottom, {.0f, 22.5f});

    return true;
}

void NumberInputPopup::setSubmitCallback(SubmitCallback cb) {
    m_impl->m_callback = std::move(cb);
}

void NumberInputPopup::setValue(double value) {
    m_impl->m_input->setString(numToString(value, m_impl->m_precision));
}

void NumberInputPopup::setArrowStep(double step) {
    if (step <= .0) step = step * -1.0;
    m_impl->m_step = step;
}

void NumberInputPopup::setPrecision(size_t precision) {
    m_impl->m_precision = precision;
    m_impl->m_input->setString(numToString(this->getValue(), precision));
}

void NumberInputPopup::resetSubmitCallback() {
    m_impl->m_callback = nullptr;
}

double NumberInputPopup::getValue() const {
    auto res = numFromString<double>(this->getValueString());
    if (res.isErr()) return .0;

    return std::move(res).unwrap();
}

double NumberInputPopup::getMin() const noexcept {
    return m_impl->m_min;
}

double NumberInputPopup::getMax() const noexcept {
    return m_impl->m_max;
}

ZStringView NumberInputPopup::getValueString() const {
    return m_impl->m_input->getString();
}

double NumberInputPopup::getArrowStep() const noexcept {
    return m_impl->m_step;
}

size_t NumberInputPopup::getPrecision() const noexcept {
    return m_impl->m_precision;
}

NumberInputPopup* NumberInputPopup::create(
    double current, double min, double max,
    std::string title, ZStringView btnText,
    float inputWidth
) {
    return create(current, min, max, std::move(title), btnText, "GJ_button_01.png", "GJ_square01.png", CircleBaseColor::Green, inputWidth);
}

NumberInputPopup* NumberInputPopup::create(
    double current, double min, double max,
    std::string title, ZStringView btnText, ZStringView btnBg,
    ZStringView bg, CircleBaseColor closeBtnColor, float inputWidth
) {
    auto ret = new NumberInputPopup();
    if (ret->init(current, min, max, std::move(title), btnText, btnBg, bg, closeBtnColor, inputWidth)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}