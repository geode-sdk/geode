#include <Geode/ui/Label.hpp>
#include <Geode/ui/ProcessPopup.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class ProcessPopup::Impl final {
public:
    Label* m_label = nullptr;

    State m_state = State::Ongoing;
    CloseCallback m_closeCallback = nullptr;

    LoadingSpinner* m_processSpinner = nullptr;

    CCSprite* m_successIcon = nullptr;
    CCSprite* m_failureIcon = nullptr;

    void setState(State state, std::string msg) {
        m_state = state;
        m_label->setRichText(std::move(msg));

        m_processSpinner->setVisible(false);
    };
};

ProcessPopup::ProcessPopup() : m_impl(std::make_unique<Impl>()) {}
ProcessPopup::~ProcessPopup() {}

bool ProcessPopup::init(std::string text, ZStringView font, ZStringView bg, CircleBaseColor btnColor, float width) {
    if (!Popup::init({width, 105.f}, bg.c_str())) return false;

    this->setCloseButtonSpr(CircleButtonSprite::createWithSpriteFrameName("close.png"_spr, .875f, btnColor), .825f);

    m_impl->m_processSpinner = LoadingSpinner::create(50.f);
    m_mainLayer->addChildAtPosition(m_impl->m_processSpinner, Anchor::Center, {.0f, 12.5f});

    m_impl->m_label = Label::createRich(std::move(text), font);
    m_impl->m_label->setAnchorPoint({.5f, 1.f});
    m_impl->m_label->setLimitLabelWidth(width - 5.f, 15.f / m_impl->m_label->getScaledContentHeight());

    m_mainLayer->addChildAtPosition(m_impl->m_label, Anchor::Center, {.0f, -22.5f});

    m_impl->m_successIcon = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
    m_impl->m_successIcon->setID("success-icon");
    m_impl->m_successIcon->setScale(2.f);
    m_impl->m_successIcon->setVisible(false);

    m_impl->m_failureIcon = CCSprite::createWithSpriteFrameName("exMark_001.png");
    m_impl->m_failureIcon->setID("failure-icon");
    m_impl->m_failureIcon->setVisible(false);

    m_mainLayer->addChildAtPosition(m_impl->m_successIcon, Anchor::Center, {.0f, 12.5f});
    m_mainLayer->addChildAtPosition(m_impl->m_failureIcon, Anchor::Center, {.0f, 12.5f});

    return true;
}

void ProcessPopup::onClose(CCObject* sender) {
    m_impl->m_closeCallback ? m_impl->m_closeCallback(this, m_impl->m_state) : Popup::onClose(sender);
}

void ProcessPopup::setCloseCallback(CloseCallback cb) {
    m_impl->m_closeCallback = std::move(cb);
}

void ProcessPopup::resetCloseCallback() {
    m_impl->m_closeCallback = nullptr;
}

void ProcessPopup::updateMessage(std::string text) {
    if (m_impl->m_state == State::Ongoing) m_impl->m_label->setRichText(std::move(text));
}

void ProcessPopup::setSuccess(std::string text) {
    if (m_impl->m_state != State::Ongoing) return;

    m_impl->m_successIcon->setVisible(true);
    m_impl->setState(State::Success, std::move(text));
}

void ProcessPopup::setFailure(std::string text) {
    if (m_impl->m_state != State::Ongoing) return;

    m_impl->m_failureIcon->setVisible(true);
    m_impl->setState(State::Failure, std::move(text));
}

ZStringView ProcessPopup::getCurrentMessage() const noexcept {
    return m_impl->m_label->getText();
}

ProcessPopup::State ProcessPopup::getProcessState() const noexcept {
    return m_impl->m_state;
}

ProcessPopup* ProcessPopup::create(std::string text, ZStringView font, float width) {
    return create(std::move(text), font, "GJ_square02.png", CircleBaseColor::Green, width);
}

ProcessPopup* ProcessPopup::create(std::string text, ZStringView font, ZStringView bg, CircleBaseColor btnColor, float width) {
    auto ret = new ProcessPopup();
    if (ret->init(std::move(text), font, bg, btnColor, width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}