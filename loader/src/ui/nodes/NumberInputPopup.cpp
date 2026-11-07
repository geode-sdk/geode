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
    CCNode* m_inputContainer = nullptr;
    Button* m_submitBtn = nullptr;
    Button* m_resetBtn = nullptr;

    double m_current = .0;
    bool m_currentDirty = false;

    double m_max = std::numeric_limits<double>::max();
    double m_min = std::numeric_limits<double>::lowest();

    double m_step = 1.0;

    size_t m_precision = 0;

    std::string stepNumStr(double step = .0) {
        auto to = validateValue() + step;

        to = std::clamp(to, m_min, m_max);

        m_current = to;
        m_currentDirty = false;

        return formatNum(to);
    }

    double validateValue() {
        if (m_currentDirty) {
            auto res = numFromString<double>(m_input->getString());
            if (res.isErr()) return .0;

            m_current = std::move(res).unwrap();
            m_currentDirty = false;
        }

        return m_current;
    }

    void updateInput(double step = .0) {
        m_input->setString(stepNumStr(step));
    }

    std::string formatNum(double num) {
        return fmt::format("{:.{}f}", num, m_precision);
    }
};

NumberInputPopup::NumberInputPopup() : m_impl(std::make_unique<Impl>()) {}
NumberInputPopup::~NumberInputPopup() {}

bool NumberInputPopup::init(double current, std::string title, ZStringView bg, std::optional<double> min, std::optional<double> max) {
    if (!Popup::init({210.0f, 125.0f}, bg.c_str())) return false;

    m_impl->m_current = current;

    if (min.has_value()) m_impl->m_min = std::move(*min);
    if (max.has_value()) m_impl->m_max = std::move(*max);

    this->setTitle(std::move(title));
    this->setCloseBtnColor(CircleBaseColor::Green);

    m_title->setLimitLabelWidth(m_mainLayer->getScaledContentWidth() - 62.5f, .7f);

    m_impl->m_inputContainer = CCNode::create();
    m_impl->m_inputContainer->setID("input-container");
    m_impl->m_inputContainer->setAnchorPoint({.5f, .5f});
    m_impl->m_inputContainer->setLayout(
        RowLayout::create()
            ->setGap(5.0f)
            ->setAutoScale(false)
            ->setAutoGrowAxis(.1f)
            ->setGrowCrossAxis(true)
    );
    m_mainLayer->addChildAtPosition(m_impl->m_inputContainer, Anchor::Center);

    m_impl->m_input = TextInput::create(55.0f, "Num");
    m_impl->m_input->setID("page-input");
    m_impl->m_input->setCommonFilter(CommonFilter::Float);

    m_impl->m_input->setCallback([this](auto) {
        m_impl->m_currentDirty = true;
    });

    auto prev = Button::createWithSpriteFrameName(
        "edit_leftBtn_001.png",
        [this](auto) {
            m_impl->updateInput(-m_impl->m_step);
        }
    );
    prev->setID("decrement-button");
    prev->setScale(.95f);

    auto next = Button::createWithSpriteFrameName(
        "edit_rightBtn_001.png",
        [this](auto) {
            m_impl->updateInput(m_impl->m_step);
        }
    );
    next->setID("increment-button");
    next->setScale(.95f);

    m_impl->m_inputContainer->addChild(prev);
    m_impl->m_inputContainer->addChild(m_impl->m_input);
    m_impl->m_inputContainer->addChild(next);

    m_impl->m_inputContainer->updateLayout();

    m_impl->m_resetBtn = Button::createWithSpriteFrameName(
        "GJ_resetBtn_001.png",
        [this, current](auto) {
            this->setValue(current);
        }
    );
    m_impl->m_resetBtn->setScale(.875f);
    m_mainLayer->addChildAtPosition(m_impl->m_resetBtn, Anchor::TopRight, {-20.0f, -20.0f});

    this->setupSubmitBtn("Submit", "GJ_button_01.png");

    m_impl->updateInput();

    return true;
}

void NumberInputPopup::setupSubmitBtn(ZStringView text, ZStringView bg) {
    if (m_impl->m_submitBtn) m_impl->m_submitBtn->removeFromParent();

    m_impl->m_submitBtn = Button::createWithNode(
        ButtonSprite::create(text.c_str(), "goldFont.fnt", bg.c_str(), .8f), [this](auto) {
            double num = std::clamp(this->getValue(), this->getMin(), this->getMax());
            m_impl->m_callback ? m_impl->m_callback(this, num) : this->onClose(nullptr);
        }
    );
    m_impl->m_submitBtn->setID("submit-button");
    m_impl->m_submitBtn->setScale(.825f);
    m_mainLayer->addChildAtPosition(m_impl->m_submitBtn, Anchor::Bottom, {0.0f, 22.5f});
}

void NumberInputPopup::setSubmitCallback(SubmitCallback cb) {
    m_impl->m_callback = std::move(cb);
}

void NumberInputPopup::setValue(double value) {
    m_impl->m_current = value;
    m_impl->m_currentDirty = false;

    m_impl->updateInput();
}

void NumberInputPopup::setArrowStep(double step) {
    m_impl->m_step = std::abs(step);
}

void NumberInputPopup::setPrecision(size_t precision) {
    m_impl->m_precision = precision;
    m_impl->updateInput();
}

void NumberInputPopup::setInputWidth(float width) {
    m_impl->m_input->setWidth(std::clamp(width, 50.0f, m_mainLayer->getScaledContentWidth() - 62.5f));
    m_impl->m_inputContainer->updateLayout();
}

void NumberInputPopup::setCloseBtnColor(CircleBaseColor color) {
    this->setCloseButtonSpr(CircleButtonSprite::createWithSpriteFrameName("close.png"_spr, .875f, color), .825f);
}

void NumberInputPopup::showResetButton(bool show) {
    m_impl->m_resetBtn->setVisible(show);
}

void NumberInputPopup::resetSubmitCallback() {
    m_impl->m_callback = nullptr;
}

double NumberInputPopup::getValue() const {
    return m_impl->validateValue();
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

NumberInputPopup* NumberInputPopup::create(double current, std::string title, std::optional<double> min, std::optional<double> max) {
    return create(current, std::move(title), "GJ_square02.png", std::move(min), std::move(max));
}

NumberInputPopup* NumberInputPopup::create(double current, std::string title, ZStringView bg, std::optional<double> min, std::optional<double> max) {
    auto ret = new NumberInputPopup();
    if (ret->init(current, std::move(title), bg, std::move(min), std::move(max))) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}