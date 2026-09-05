#include <Geode/ui/Toggler.hpp>

using namespace geode::prelude;

class Toggler::Impl final {
public:
    TogglerCallback m_toggleCallback = nullptr;
    bool m_isToggled = false;

    CCNode* m_offNode = nullptr;
    CCNode* m_onNode = nullptr;
};

Toggler::Toggler() : m_impl(std::make_unique<Impl>()) {}

Toggler::~Toggler() = default;

Toggler* Toggler::create(
    CCNode* offNode,
    CCNode* onNode,
    TogglerCallback toggleCallback
) {
    auto ret = new Toggler();
    if (ret->init(offNode, onNode, std::move(toggleCallback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Toggler* Toggler::createWithStandardSprites(
    TogglerCallback toggleCallback
) {
    return createWithStandardSprites(1.0f, std::move(toggleCallback));
}

Toggler* Toggler::createWithStandardSprites(
    float scale,
    TogglerCallback toggleCallback
) {
    auto offNode = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    auto onNode = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

    if (!offNode || !onNode) return nullptr;

    offNode->setScale(scale);
    onNode->setScale(scale);

    return create(offNode, onNode, std::move(toggleCallback));
}

Toggler* Toggler::createWithSprites(
    ZStringView offFileName,
    ZStringView onFileName,
    TogglerCallback toggleCallback
) {
    return createWithSprites(offFileName, onFileName, 1.0f, std::move(toggleCallback));
}

Toggler* Toggler::createWithSprites(
    ZStringView offFileName,
    ZStringView onFileName,
    float scale,
    TogglerCallback toggleCallback
) {
    auto offNode = CCSprite::create(offFileName.c_str());
    auto onNode = CCSprite::create(onFileName.c_str());

    if (!offNode || !onNode) return nullptr;

    offNode->setScale(scale);
    onNode->setScale(scale);

    return create(offNode, onNode, std::move(toggleCallback));
}

Toggler* Toggler::createWithSpriteFrameNames(
    ZStringView offFrameName,
    ZStringView onFrameName,
    TogglerCallback toggleCallback
) {
    return createWithSpriteFrameNames(
        offFrameName, onFrameName, 1.0f, std::move(toggleCallback)
    );
}

Toggler* Toggler::createWithSpriteFrameNames(
    ZStringView offFrameName,
    ZStringView onFrameName,
    float scale,
    TogglerCallback toggleCallback
) {
    auto offNode = CCSprite::createWithSpriteFrameName(offFrameName.c_str());
    auto onNode = CCSprite::createWithSpriteFrameName(onFrameName.c_str());

    if (!offNode || !onNode) return nullptr;

    offNode->setScale(scale);
    onNode->setScale(scale);

    return create(offNode, onNode, std::move(toggleCallback));
}

bool Toggler::init(
    CCNode* offNode,
    CCNode* onNode,
    TogglerCallback toggleCallback
) {
    if (!offNode || !onNode) return false;

    if (!Button::init(nullptr)) return false;

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
    if (!node || node == m_impl->m_offNode) return;

    m_impl->m_offNode->removeFromParent();

    m_impl->m_offNode = node;
    addChild(node);

    updateDisplay();
}

CCNode* Toggler::getOnNode() const {
    return m_impl->m_onNode;
}

void Toggler::setOnNode(CCNode* node) {
    if (!node || node == m_impl->m_onNode) return;

    m_impl->m_onNode->removeFromParent();

    m_impl->m_onNode = node;
    addChild(node);

    updateDisplay();
}

void Toggler::setToggleCallback(TogglerCallback callback) {
    m_impl->m_toggleCallback = std::move(callback);
}

void Toggler::activate() {
    if (!isEnabled()) return;

    Button::activate();
    toggle(true);
}

void Toggler::updateDisplay() {
    auto activeNode = m_impl->m_isToggled
        ? m_impl->m_onNode
        : m_impl->m_offNode;

    auto size = activeNode->getScaledContentSize();
    auto center = size * 0.5f;

    m_impl->m_offNode->setVisible(!m_impl->m_isToggled);
    m_impl->m_onNode->setVisible(m_impl->m_isToggled);

    m_impl->m_offNode->setPosition(center);
    m_impl->m_onNode->setPosition(center);

    setContentSize(size);
}
