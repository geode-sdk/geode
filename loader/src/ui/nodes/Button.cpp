#include <Geode/ui/Button.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class Button::Impl final {
public:
    ButtonCallback m_activateCallback = nullptr;
    ButtonCallback m_selectCallback = nullptr;
    ButtonCallback m_unselectCallback = nullptr;

    bool m_enabled = true;
    bool m_selected = false;
    bool m_defaultsSet = false;

    int m_touchPriority = cocos2d::kCCMenuHandlerPriority;

    float m_scaleMultiplier = 1.26;
    float m_touchMultiplier = 1.f;

    CCPoint m_offset = {0, -15};
    
    float m_selectedDuration = 0.3f;
    float m_unselectedDuration = 0.4f;

    AnimationType m_animationType = AnimationType::Scale;

    geode::Ref<cocos2d::CCActionInterval> m_clickAction = nullptr;
    geode::Ref<cocos2d::CCActionInterval> m_releaseAction = nullptr;

    geode::Ref<cocos2d::CCActionInterval> m_activeClickAction = nullptr;
    geode::Ref<cocos2d::CCActionInterval> m_activeReleaseAction = nullptr;

    cocos2d::CCNode* m_displayNode = nullptr;

    geode::Ref<cocos2d::CCNodeRGBA> m_defaults = nullptr;
};

Button::Button() : m_impl(std::make_unique<Impl>()) {}

Button::~Button() {}

Button* Button::create(ButtonCallback activateCallback) {
    auto ret = new Button();
    if (ret->init(std::move(activateCallback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Button* Button::createWithNode(CCNode* node, ButtonCallback activateCallback) {
    auto ret = new Button();
    if (ret->initWithNode(node, std::move(activateCallback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Button* Button::createWithSprite(ZStringView fileName, ButtonCallback activateCallback) {
    auto ret = new Button();
    if (ret->initWithSprite(std::move(fileName), std::move(activateCallback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Button* Button::createWithSpriteFrameName(ZStringView frameName, ButtonCallback activateCallback) {
    auto ret = new Button();
    if (ret->initWithSpriteFrameName(std::move(frameName),std::move(activateCallback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Button* Button::createWithLabel(ZStringView text, ZStringView font, ButtonCallback activateCallback) {
    auto ret = new Button();
    if (ret->initWithLabel(std::move(text), std::move(font), std::move(activateCallback))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Button::init(ButtonCallback activateCallback) {
    if (!CCNodeRGBA::init()) return false;
    m_impl->m_defaults = CCNodeRGBA::create();
    m_impl->m_activateCallback = std::move(activateCallback);

    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);
    setAnchorPoint({0.5f, 0.5f});

    return true;
}

bool Button::initWithNode(CCNode* node, ButtonCallback activateCallback) {
    if (!Button::init(std::move(activateCallback))) return false;

    m_impl->m_displayNode = node;

    setContentSize(m_impl->m_displayNode->getScaledContentSize());
    m_impl->m_displayNode->setPosition(getContentSize() * m_impl->m_displayNode->getAnchorPoint());

    addChild(m_impl->m_displayNode);
    return true;
}

bool Button::initWithSprite(ZStringView fileName, ButtonCallback activateCallback) {
    if (!Button::init(std::move(activateCallback))) return false;

    m_impl->m_displayNode = CCSprite::create(fileName.c_str());
    if (!m_impl->m_displayNode) return false;

    setContentSize(m_impl->m_displayNode->getScaledContentSize());
    m_impl->m_displayNode->setPosition(getContentSize() * m_impl->m_displayNode->getAnchorPoint());

    addChild(m_impl->m_displayNode);
    return true;
}

bool Button::initWithSpriteFrameName(ZStringView frameName, ButtonCallback activateCallback) {
    if (!Button::init(std::move(activateCallback))) return false;

    m_impl->m_displayNode = CCSprite::createWithSpriteFrameName(frameName.c_str());
    if (!m_impl->m_displayNode) return false;

    setContentSize(m_impl->m_displayNode->getScaledContentSize());
    m_impl->m_displayNode->setPosition(getContentSize() * m_impl->m_displayNode->getAnchorPoint());

    addChild(m_impl->m_displayNode);
    return true;
}

bool Button::initWithLabel(ZStringView text, ZStringView font, ButtonCallback activateCallback) {
    if (!Button::init(std::move(activateCallback))) return false;

    m_impl->m_displayNode = CCLabelBMFont::create(text.c_str(), font.c_str());
    if (!m_impl->m_displayNode) return false;

    setContentSize(m_impl->m_displayNode->getScaledContentSize());
    m_impl->m_displayNode->setPosition(getContentSize() * m_impl->m_displayNode->getAnchorPoint());

    addChild(m_impl->m_displayNode);
    return true;
}

CCNode* Button::getDisplayNode() {
    return m_impl->m_displayNode;
}

CCActionInterval* Button::clickActionForType() {
    switch (m_impl->m_animationType) {
        case AnimationType::None: {
            return nullptr;
        }
        case AnimationType::Custom: {
            return m_impl->m_clickAction;
        }
        case AnimationType::Scale: {
            auto scaleTo = CCScaleTo::create(m_impl->m_selectedDuration, m_impl->m_defaults->getScale() * m_impl->m_scaleMultiplier);
            return CCEaseBounceOut::create(scaleTo);
        }
        case AnimationType::Move: {
            auto moveTo = CCMoveTo::create(m_impl->m_selectedDuration, m_impl->m_defaults->getPosition() + m_impl->m_offset);
            return CCEaseInOut::create(moveTo, 1.5f);
        }
    }

    return nullptr;
}   

CCActionInterval* Button::releaseActionForType() {
    switch (m_impl->m_animationType) {
        case AnimationType::None: {
            return nullptr;
        }
        case AnimationType::Custom: {
            return m_impl->m_releaseAction;
        }
        case AnimationType::Scale: {
            auto scaleTo = CCScaleTo::create(m_impl->m_unselectedDuration, m_impl->m_defaults->getScale());
            return CCEaseBounceOut::create(scaleTo);
        }
        case AnimationType::Move: {
            auto moveTo = CCMoveTo::create(m_impl->m_selectedDuration, m_impl->m_defaults->getPosition());
            return CCEaseInOut::create(moveTo, 2.f);
        }
    }
    
    return nullptr;
}

void Button::setAnimationType(AnimationType type) {
    m_impl->m_animationType = type;
}

void Button::setClickAnimation(CCActionInterval* action) {
    m_impl->m_animationType = AnimationType::Custom;
    m_impl->m_clickAction = action;
}

void Button::setReleaseAnimation(CCActionInterval* action) {
    m_impl->m_animationType = AnimationType::Custom;
    m_impl->m_releaseAction = action;
}

void Button::setDefaults() {
    m_impl->m_defaults->setScaleX(getScaleX());
    m_impl->m_defaults->setScaleY(getScaleY());

    m_impl->m_defaults->setSkewX(getSkewX());
    m_impl->m_defaults->setSkewY(getSkewY());

    m_impl->m_defaults->setRotationX(getRotationX());
    m_impl->m_defaults->setRotationY(getRotationY());

    m_impl->m_defaults->setPositionX(getPositionX());
    m_impl->m_defaults->setPositionY(getPositionY());

    m_impl->m_defaults->setContentSize(getContentSize());

    m_impl->m_defaults->setZOrder(getZOrder());

    m_impl->m_defaults->setOpacity(getOpacity());
    m_impl->m_defaults->setColor(getColor());
}

void Button::resetDefaults() {
    m_impl->m_defaultsSet = false;

    stopAction(m_impl->m_activeClickAction);
    stopAction(m_impl->m_activeReleaseAction);

    m_impl->m_activeClickAction = nullptr;
    m_impl->m_activeReleaseAction = nullptr;

    setScaleX(m_impl->m_defaults->getScaleX());
    setScaleY(m_impl->m_defaults->getScaleY());

    setSkewX(m_impl->m_defaults->getSkewX());
    setSkewY(m_impl->m_defaults->getSkewY());

    setRotationX(m_impl->m_defaults->getRotationX());
    setRotationY(m_impl->m_defaults->getRotationY());

    setPositionX(m_impl->m_defaults->getPositionX());
    setPositionY(m_impl->m_defaults->getPositionY());

    setContentSize(m_impl->m_defaults->getContentSize());

    setZOrder(m_impl->m_defaults->getZOrder());

    setOpacity(m_impl->m_defaults->getOpacity());
    setColor(m_impl->m_defaults->getColor());
}

void Button::selected() {
    if (!m_impl->m_enabled || m_impl->m_selected) return;
    if (!m_impl->m_defaultsSet) {
        setDefaults();
        m_impl->m_defaultsSet = true;
    }

    stopAction(m_impl->m_activeClickAction);
    stopAction(m_impl->m_activeReleaseAction);

    m_impl->m_activeClickAction = clickActionForType();

    if (m_impl->m_activeClickAction) {
        runAction(m_impl->m_activeClickAction);
    }

    if (m_impl->m_selectCallback) m_impl->m_selectCallback(this);

    m_impl->m_selected = true;
}

void Button::unselected() {
    if (!m_impl->m_selected) return;
    
    stopAction(m_impl->m_activeClickAction);
    stopAction(m_impl->m_activeReleaseAction);

    m_impl->m_activeReleaseAction = releaseActionForType();

    if (m_impl->m_activeReleaseAction) {
        runAction(m_impl->m_activeReleaseAction);
    }

    if (m_impl->m_unselectCallback) m_impl->m_unselectCallback(this);

    m_impl->m_selected = false;
}

void Button::activate() {
    if (!m_impl->m_enabled) return;
    m_impl->m_selected = false;

    resetDefaults();

    if (m_impl->m_activateCallback) m_impl->m_activateCallback(this);
}

void Button::setScaleMultiplier(float multiplier) {
    m_impl->m_scaleMultiplier = multiplier;
}

float Button::getScaleMultiplier() {
    return m_impl->m_scaleMultiplier;
}

void Button::setMoveOffset(cocos2d::CCPoint const& offset) {
    m_impl->m_offset = std::move(offset);
}

cocos2d::CCPoint Button::getMoveOffset() {
    return m_impl->m_offset;
}

void Button::setSelectedDuration(float duration) {
    m_impl->m_selectedDuration = duration;
}

float Button::getSelectedDuration() {
    return m_impl->m_selectedDuration;
}

void Button::setUnselectedDuration(float duration) {
    m_impl->m_unselectedDuration = duration;
}

float Button::getUnselectedDuration() {
    return m_impl->m_unselectedDuration;
}

void Button::setEnabled(bool enabled) {
    m_impl->m_enabled = enabled;
    if (!m_impl->m_enabled) {
        unselected();
    }
}

bool Button::isEnabled() {
    return m_impl->m_enabled;
}

bool Button::isSelected() {
    return m_impl->m_selected;
}

void Button::onEnter() {
    CCNodeRGBA::onEnter();
    SharedButtonHandler::get()->registerButton(this);
    registerWithTouchDispatcher();
}

void Button::onExit() {
    CCNodeRGBA::onExit();
    SharedButtonHandler::get()->unregisterButton(this);
    CCTouchDispatcher::get()->removeDelegate(this);
}

bool Button::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (!nodeIsVisible(this)) return false;

    if (SharedButtonHandler::containsTouch(this, touch)) {
        SharedButtonHandler::get()->setActiveButton(this);
        return true;
    }
    return false;
}

void Button::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    SharedButtonHandler::get()->passMoveToButtons(this, touch);
}

void Button::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    SharedButtonHandler::get()->passActivateToButtons();
}

void Button::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
    SharedButtonHandler::get()->passCancelledToButtons();
}

void Button::setTouchPriority(int priority) {
    m_impl->m_touchPriority = priority;
}

int Button::getTouchPriority() {
    return m_impl->m_touchPriority;
}

void Button::setTouchMultiplier(float multipler) {
    m_impl->m_touchMultiplier = multipler;
}

float Button::getTouchMultiplier() {
    return m_impl->m_touchMultiplier;
}

void Button::registerWithTouchDispatcher() {
    CCTouchDispatcher::get()->addTargetedDelegate(this, m_impl->m_touchPriority, true);
}

void Button::setActivateCallback(ButtonCallback callback) {
    m_impl->m_activateCallback = std::move(callback);
}

void Button::setSelectCallback(ButtonCallback callback) {
    m_impl->m_selectCallback = std::move(callback);
}

void Button::setUnselectCallback(ButtonCallback callback) {
    m_impl->m_unselectCallback = std::move(callback);
}

class SharedButtonHandler::Impl final {
public:
    Button* m_activeNeighbor = nullptr;
    std::unordered_map<cocos2d::CCNode*, std::unordered_set<Button*>> m_buttons;
};

SharedButtonHandler::SharedButtonHandler() : m_impl(std::make_unique<Impl>()) {}

SharedButtonHandler::~SharedButtonHandler() {}

SharedButtonHandler* SharedButtonHandler::get() {
    static SharedButtonHandler handler;
    return &handler;
}

void SharedButtonHandler::registerButton(Button* button) {
    auto parent = button->getParent();
    if (!parent) return;

    m_impl->m_buttons[parent].insert(button);
}

void SharedButtonHandler::unregisterButton(Button* button) {
    auto parent = button->getParent();
    if (!parent) return;

    if (m_impl->m_activeNeighbor == button) m_impl->m_activeNeighbor = nullptr;

    auto& buttons = m_impl->m_buttons[parent];

    buttons.erase(button);
    if (buttons.empty()) {
        m_impl->m_buttons.erase(parent);
    }
}

void SharedButtonHandler::passMoveToButtons(Button* button, CCTouch* touch) {
    auto parent = button->getParent();
    if (!parent) return;

    auto& buttons = m_impl-> m_buttons[parent];

    bool hasNewNeighbor = false;

    for (auto button : buttons) {
        if (!hasNewNeighbor && !m_impl->m_activeNeighbor && !button->isSelected() && SharedButtonHandler::containsTouch(button, touch)) {
            m_impl->m_activeNeighbor = button;
            hasNewNeighbor = true;
            button->selected();
        }
        else if (button->isSelected() && !SharedButtonHandler::containsTouch(button, touch)) {
            if (m_impl->m_activeNeighbor == button) {
                m_impl->m_activeNeighbor = nullptr;
            }
            button->unselected();
        }
    }
}

void SharedButtonHandler::passActivateToButtons() {
    if (!m_impl->m_activeNeighbor || !m_impl->m_activeNeighbor->isSelected()) return;

    m_impl->m_activeNeighbor->unselected();
    m_impl->m_activeNeighbor->activate();
    m_impl->m_activeNeighbor = nullptr;
}

void SharedButtonHandler::passCancelledToButtons() {
    if (!m_impl->m_activeNeighbor) return;

    m_impl->m_activeNeighbor->unselected();
    m_impl->m_activeNeighbor = nullptr;
}

bool SharedButtonHandler::containsTouch(Button* button, CCTouch* touch) {
    float multiplier = button->getTouchMultiplier();

    auto local = button->convertToNodeSpace(touch->getLocation());

    float width = button->getContentWidth();
    float height = button->getContentHeight();

    float scaledWidth = width * multiplier;
    float scaledHeight = height * multiplier;

    float dx = (scaledWidth - width) * 0.5f;
    float dy = (scaledHeight - height) * 0.5f;

    auto rect = cocos2d::CCRect{-dx, -dy, scaledWidth, scaledHeight};

    return rect.containsPoint(local);
}

void SharedButtonHandler::setActiveButton(Button* button) {
    m_impl->m_activeNeighbor = button;
    m_impl->m_activeNeighbor->selected();
}
