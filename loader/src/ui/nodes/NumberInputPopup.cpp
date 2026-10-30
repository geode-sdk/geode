#include <Geode/ui/NumberInputPopup.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/general.hpp>

using namespace geode::prelude;

class NumberInputPopup::Impl final {
public:
    Callback m_callback = nullptr;

    std::string offsetPageStr(std::string_view current, int64_t by, size_t max) {
        auto currentRes = numFromString<size_t>(current);
        if (currentRes.isErr()) return "1";

        auto cur = std::move(currentRes).unwrap();
        auto to = cur + by;

        if (to < 1) return std::string{current};
        if (to > max) return numToString(max);

        return numToString(to);
    }
};

NumberInputPopup::NumberInputPopup() : m_impl(std::make_unique<Impl>()) {}
NumberInputPopup::~NumberInputPopup() {}

bool NumberInputPopup::init(int64_t current, int64_t min, int64_t max, std::string title, ZStringView btnText, ZStringView btnBg, ZStringView bg, CircleBaseColor closeBtnColor, float inputWidth) {
    if (!Popup::init({210.f, 125.f}, bg.c_str())) return false;

    this->setTitle(std::move(title));
    this->setCloseButtonSpr(CircleButtonSprite::createWithSpriteFrameName("close.png"_spr, .875f, closeBtnColor), .825f);

    auto maxWidth = m_mainLayer->getScaledContentWidth() - 12.5f;

    m_title->setLimitLabelWidth(maxWidth - 50.f, .7f);

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

    auto input = TextInput::create((inputWidth <= maxWidth) ? inputWidth : maxWidth, "Num");
    input->setID("page-input");
    input->setCommonFilter(CommonFilter::Int);
    input->setString(numToString(current));

    auto prev = Button::createWithSpriteFrameName(
        "edit_leftBtn_001.png",
        [this, input, min, max](auto) {
            input->setString(m_impl->offsetPageStr(input->getString(), -1, max));
        }
    );
    prev->setID("decrement-button");
    prev->setScale(.95f);

    auto next = Button::createWithSpriteFrameName(
        "edit_rightBtn_001.png",
        [this, input, min, max](auto) {
            input->setString(m_impl->offsetPageStr(input->getString(), 1, max));
        }
    );
    next->setID("increment-button");
    next->setScale(.95f);

    container->addChild(prev);
    container->addChild(input);
    container->addChild(next);

    container->updateLayout();

    auto reset = Button::createWithSpriteFrameName(
        "GJ_resetBtn_001.png",
        [input, current](auto) {
            input->setString(numToString(current));
        }
    );
    reset->setScale(.875f);
    m_mainLayer->addChildAtPosition(reset, Anchor::TopRight, {-20.f, -20.f});

    auto submit = Button::createWithNode(
        ButtonSprite::create(btnText.c_str(), "goldFont.fnt", btnBg.c_str(), .8f),
        [this, input, max](auto) {
            auto res = numFromString<size_t>(input->getString());
            if (res.isErr()) {
                Notification::create("Invalid number", NotificationIcon::Error)->show();
                return;
            }

            size_t num = std::move(res).unwrap();
            if (num > max) num = max;
            if (num <= 0) num = 1;

            m_impl->m_callback ? m_impl->m_callback(this, num) : this->onClose(nullptr);
        }
    );
    submit->setID("submit-button");
    submit->setScale(.825f);
    m_mainLayer->addChildAtPosition(submit, Anchor::Bottom, {.0f, 22.5f});

    return true;
}

void NumberInputPopup::setCallback(Callback cb) {
    m_impl->m_callback = std::move(cb);
}

void NumberInputPopup::resetCallback() {
    m_impl->m_callback = nullptr;
}

NumberInputPopup* NumberInputPopup::create(
    int64_t current, int64_t min, int64_t max,
    std::string title, ZStringView btnText,
    float inputWidth
) {
    return create(current, min, max, std::move(title), btnText, "GJ_button_01.png", "GJ_square01.png", CircleBaseColor::Green, inputWidth);
}

NumberInputPopup* NumberInputPopup::create(
    int64_t current, int64_t min, int64_t max,
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