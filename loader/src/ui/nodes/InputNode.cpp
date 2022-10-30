#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/ui/InputNode.hpp>

USE_GEODE_NAMESPACE();

char const* InputNode::getString() {
    return m_input->getString();
}

void InputNode::setString(std::string const& str) {
    m_input->setString(str);
}

CCTextInputNode* InputNode::getInput() const {
    return m_input;
}

CCScale9Sprite* InputNode::getBG() const {
    return m_bgSprite;
}

void InputNode::activate() {
    m_input->onClickTrackNode(true);
}

void InputNode::setEnabled(bool enabled) {
    m_input->setMouseEnabled(enabled);
    m_input->setTouchEnabled(enabled);
}

bool InputNode::init(
    float width, float height, char const* placeholder, char const* font, std::string const& filter,
    int maxCharCount
) {
    if (!CCMenuItem::initWithTarget(this, nullptr)) return false;

    m_bgSprite = cocos2d::extension::CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    m_bgSprite->setScale(.5f);
    m_bgSprite->setColor({ 0, 0, 0 });
    m_bgSprite->setOpacity(75);
    m_bgSprite->setContentSize({ width * 2, height * 2 });
    m_bgSprite->setPosition(width / 2, height / 2);
    this->addChild(m_bgSprite);

    m_input = CCTextInputNode::create(width - 10.0f, height, placeholder, font);
    m_input->setLabelPlaceholderColor({ 150, 150, 150 });
    m_input->setLabelPlaceholderScale(.75f);
    m_input->setMaxLabelScale(.85f);
    m_input->setMaxLabelWidth(maxCharCount);
    m_input->setPosition(width / 2, height / 2);
    if (filter.length()) {
        m_input->setAllowedChars(filter);
    }
    this->addChild(m_input);

    this->setContentSize({ width, height });
    this->setAnchorPoint({ .5f, .5f });

    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);

    return true;
}

bool InputNode::init(
    float width, char const* placeholder, char const* font, std::string const& filter,
    int maxCharCount
) {
    return init(width, 30.0f, placeholder, font, filter, maxCharCount);
}

InputNode* InputNode::create(
    float width, char const* placeholder, char const* font, std::string const& filter,
    int maxCharCount
) {
    auto pRet = new InputNode();

    if (pRet && pRet->init(width, placeholder, font, filter, maxCharCount)) {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

InputNode* InputNode::create(float width, char const* placeholder, std::string const& filter) {
    return create(width, placeholder, "bigFont.fnt", filter, 69);
}

InputNode* InputNode::create(
    float width, char const* placeholder, std::string const& filter, int maxCharCount
) {
    return create(width, placeholder, "bigFont.fnt", filter, maxCharCount);
}

InputNode* InputNode::create(float width, char const* placeholder, char const* font) {
    return create(width, placeholder, font, "", 69);
}

InputNode* InputNode::create(float width, char const* placeholder) {
    return create(width, placeholder, "bigFont.fnt");
}
