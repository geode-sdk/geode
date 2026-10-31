#include <Geode/ui/Toggler.hpp>

using namespace geode::prelude;

class Toggler::Impl final {
public:
    ToggleCallback m_toggleCallback = nullptr;
    bool m_isToggled = false;

    CCNode* m_offNode = nullptr;
    CCNode* m_onNode = nullptr;
};

Toggler::Toggler() : m_impl(std::make_unique<Impl>()) {}

Toggler::~Toggler() = default;

Toggler* Toggler::create(
    CCNode* offNode,
    CCNode* onNode,
    ToggleCallback toggleCallback
) {
    auto ret = new Toggler();
    if (ret->init(offNode, onNode, std::move(toggleCallback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

static Toggler* createScaled(
    CCNode* offNode,
    CCNode* onNode,
    float scale,
    Toggler::ToggleCallback toggleCallback
) {
    auto ret = Toggler::create(offNode, onNode, std::move(toggleCallback));
    ret->setScale(scale);

    return ret;
}

Toggler* Toggler::createWithStandardSprites(
    ToggleCallback toggleCallback
) {
    return createWithStandardSprites(1.0f, std::move(toggleCallback));
}

Toggler* Toggler::createWithStandardSprites(
    float scale,
    ToggleCallback toggleCallback
) {
    return createScaled(
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        scale,
        std::move(toggleCallback)
    );
}

Toggler* Toggler::createWithSprites(
    ZStringView offFileName,
    ZStringView onFileName,
    ToggleCallback toggleCallback
) {
    return createWithSprites(offFileName, onFileName, 1.0f, std::move(toggleCallback));
}

Toggler* Toggler::createWithSprites(
    ZStringView offFileName,
    ZStringView onFileName,
    float scale,
    ToggleCallback toggleCallback
) {
    return createScaled(
        CCSprite::create(offFileName.c_str()),
        CCSprite::create(onFileName.c_str()),
        scale,
        std::move(toggleCallback)
    );
}

Toggler* Toggler::createWithSpriteFrameNames(
    ZStringView offFrameName,
    ZStringView onFrameName,
    ToggleCallback toggleCallback
) {
    return createWithSpriteFrameNames(
        offFrameName, onFrameName, 1.0f, std::move(toggleCallback)
    );
}

Toggler* Toggler::createWithSpriteFrameNames(
    ZStringView offFrameName,
    ZStringView onFrameName,
    float scale,
    ToggleCallback toggleCallback
) {
    return createScaled(
        CCSprite::createWithSpriteFrameName(offFrameName.c_str()),
        CCSprite::createWithSpriteFrameName(onFrameName.c_str()),
        scale,
        std::move(toggleCallback)
    );
}

bool Toggler::init(
    CCNode* offNode,
    CCNode* onNode,
    ToggleCallback toggleCallback
) {
    if (!Button::init(nullptr)) return false;

    this->setAnchorPoint({0.5f, 0.5f});

    offNode->setAnchorPoint({0.5f, 0.5f});
    onNode->setAnchorPoint({0.5f, 0.5f});

    m_impl->m_offNode = offNode;
    m_impl->m_onNode = onNode;
    m_impl->m_toggleCallback = std::move(toggleCallback);

    addChild(offNode);
    addChild(onNode);

    updateDisplay();

    return true;
}

bool Toggler::isToggled() const {
    return m_impl->m_isToggled;
}

void Toggler::setToggled(bool toggled, bool triggerCallback) {
    if (m_impl->m_isToggled == toggled) return;

    m_impl->m_isToggled = toggled;
    updateDisplay();

    if (triggerCallback && m_impl->m_toggleCallback) {
        m_impl->m_toggleCallback(this, toggled);
    }
}

void Toggler::toggle(bool triggerCallback) {
    setToggled(!m_impl->m_isToggled, triggerCallback);
}

CCNode* Toggler::getOffNode() const {
    return m_impl->m_offNode;
}

void Toggler::setOffNode(CCNode* node) {
    node->setAnchorPoint({0.5f, 0.5f});

    m_impl->m_offNode->removeFromParent();
    m_impl->m_offNode = node;

    addChild(node);
    updateDisplay();
}

CCNode* Toggler::getOnNode() const {
    return m_impl->m_onNode;
}

void Toggler::setOnNode(CCNode* node) {
    node->setAnchorPoint({0.5f, 0.5f});

    m_impl->m_onNode->removeFromParent();
    m_impl->m_onNode = node;

    addChild(node);
    updateDisplay();
}

void Toggler::setToggleCallback(ToggleCallback callback) {
    m_impl->m_toggleCallback = std::move(callback);
}

void Toggler::activate() {
    if (!isEnabled()) return;

    Button::activate();
    toggle(true);
}

void Toggler::updateDisplay() {
    auto offSize = m_impl->m_offNode->getScaledContentSize();
    auto onSize = m_impl->m_onNode->getScaledContentSize();

    CCSize size(
        std::max(std::abs(offSize.width), std::abs(onSize.width)),
        std::max(std::abs(offSize.height), std::abs(onSize.height))
    );

    auto center = size * 0.5f;

    m_impl->m_offNode->setPosition(center);
    m_impl->m_onNode->setPosition(center);

    m_impl->m_offNode->setVisible(!m_impl->m_isToggled);
    m_impl->m_onNode->setVisible(m_impl->m_isToggled);

    setContentSize(size);
}
