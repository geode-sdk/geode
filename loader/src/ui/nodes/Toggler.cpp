#include <Geode/ui/Toggler.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class Toggler::Impl final {
public:
    TogglerCallback m_toggleCallback = nullptr;
    bool m_isToggled = false;

    CCNode* m_offNode = nullptr;
    CCNode* m_onNode = nullptr;
    CCNode* m_containerNode = nullptr;
};

Toggler::Toggler() : m_impl(std::make_unique<Impl>()) {}

Toggler::~Toggler() = default;

Toggler* Toggler::create(TogglerCallback toggleCallback) {
    auto ret = new Toggler();
    if (ret->init(std::move(toggleCallback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Toggler* Toggler::createWithNodes(
    CCNode* offNode,
    CCNode* onNode,
    TogglerCallback toggleCallback
) {
    auto ret = new Toggler();
    if (ret->initWithNodes(offNode, onNode, std::move(toggleCallback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Toggler* Toggler::createWithStandardSprites(
    TogglerCallback toggleCallback,
    float scale
) {
    auto offSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    auto onSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

    if (offSprite) offSprite->setScale(scale);
    if (onSprite) onSprite->setScale(scale);

    return Toggler::createWithNodes(offSprite, onSprite, std::move(toggleCallback));
}

Toggler* Toggler::createWithSprites(
    ZStringView offFileName,
    ZStringView onFileName,
    TogglerCallback toggleCallback,
    float scale
) {
    auto ret = new Toggler();
    if (ret->initWithSprites(offFileName, onFileName, std::move(toggleCallback), scale)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Toggler* Toggler::createWithSpriteFrameNames(
    ZStringView offFrameName,
    ZStringView onFrameName,
    TogglerCallback toggleCallback,
    float scale
) {
    auto ret = new Toggler();
    if (ret->initWithSpriteFrameNames(offFrameName, onFrameName, std::move(toggleCallback), scale)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Toggler::init(TogglerCallback toggleCallback) {
    if (!Button::init(nullptr)) return false;

    m_impl->m_toggleCallback = std::move(toggleCallback);
    m_impl->m_containerNode = CCNode::create();

    setDisplayNode(m_impl->m_containerNode);
    return true;
}

bool Toggler::initWithNodes(
    CCNode* offNode,
    CCNode* onNode,
    TogglerCallback toggleCallback
) {
    if (!offNode || !onNode) return false;
    if (!Button::init(nullptr)) return false;

    m_impl->m_toggleCallback = std::move(toggleCallback);
    m_impl->m_containerNode = CCNode::create();

    m_impl->m_offNode = offNode;
    m_impl->m_onNode = onNode;
    m_impl->m_containerNode->addChild(m_impl->m_offNode);
    m_impl->m_containerNode->addChild(m_impl->m_onNode);

    setDisplayNode(m_impl->m_containerNode);
    updateVisuals();

    return true;
}

bool Toggler::initWithSprites(
    ZStringView offFileName,
    ZStringView onFileName,
    TogglerCallback toggleCallback,
    float scale
) {
    auto offSprite = CCSprite::create(offFileName.c_str());
    auto onSprite = CCSprite::create(onFileName.c_str());

    if (offSprite) offSprite->setScale(scale);
    if (onSprite) onSprite->setScale(scale);

    return initWithNodes(offSprite, onSprite, std::move(toggleCallback));
}

bool Toggler::initWithSpriteFrameNames(
    ZStringView offFrameName,
    ZStringView onFrameName,
    TogglerCallback toggleCallback,
    float scale
) {
    auto offSprite = CCSprite::createWithSpriteFrameName(offFrameName.c_str());
    auto onSprite = CCSprite::createWithSpriteFrameName(onFrameName.c_str());

    if (offSprite) offSprite->setScale(scale);
    if (onSprite) onSprite->setScale(scale);

    return initWithNodes(offSprite, onSprite, std::move(toggleCallback));
}

bool Toggler::isToggled() const {
    return m_impl->m_isToggled;
}

void Toggler::setToggled(bool toggled, bool triggerCallback) {
    if (m_impl->m_isToggled == toggled) return;

    m_impl->m_isToggled = toggled;
    updateVisuals();

    if (triggerCallback && m_impl->m_toggleCallback) {
        m_impl->m_toggleCallback(this, m_impl->m_isToggled);
    }
}

void Toggler::toggle(bool triggerCallback) {
    setToggled(!m_impl->m_isToggled, triggerCallback);
}

CCNode* Toggler::getOffNode() const {
    return m_impl->m_offNode;
}

void Toggler::setOffNode(CCNode* node) {
    if (m_impl->m_offNode) {
        m_impl->m_offNode->removeFromParent();
    }

    m_impl->m_offNode = node;
    if (m_impl->m_offNode) {
        m_impl->m_containerNode->addChild(m_impl->m_offNode);
    }

    updateVisuals();
}

CCNode* Toggler::getOnNode() const {
    return m_impl->m_onNode;
}

void Toggler::setOnNode(CCNode* node) {
    if (m_impl->m_onNode) {
        m_impl->m_onNode->removeFromParent();
    }

    m_impl->m_onNode = node;
    if (m_impl->m_onNode) {
        m_impl->m_containerNode->addChild(m_impl->m_onNode);
    }

    updateVisuals();
}


void Toggler::setToggleCallback(TogglerCallback callback) {
    m_impl->m_toggleCallback = std::move(callback);
}

void Toggler::activate() {
    if (!isEnabled()) return;

    Button::activate();

    toggle(true);
}

void Toggler::updateVisuals() {
    if (!m_impl->m_containerNode) return;

    auto offNode = m_impl->m_offNode;
    auto onNode = m_impl->m_onNode;

    if (offNode) {
        offNode->setVisible(!m_impl->m_isToggled);
    }

    if (onNode) {
        onNode->setVisible(m_impl->m_isToggled);
    }

    auto activeNode = m_impl->m_isToggled ? onNode : offNode;
    auto size = activeNode ? activeNode->getScaledContentSize() : CCSize{};

    m_impl->m_containerNode->setContentSize(size);
    m_impl->m_containerNode->setAnchorPoint({0.5f, 0.5f});

    auto center = size / 2.f;

    if (offNode) {
        offNode->setAnchorPoint({0.5f, 0.5f});
        offNode->setPosition(center);
    }

    if (onNode) {
        onNode->setAnchorPoint({0.5f, 0.5f});
        onNode->setPosition(center);
    }

    setContentSize(size);
}