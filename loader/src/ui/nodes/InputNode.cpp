#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/ui/InputNode.hpp>

using namespace geode::prelude;

// rob only uses `CCTextInputNode`s in mostly-flat hierarchies, which still
// happen to work with the weird vanilla code. this fix makes it work even in
// deep hierarchies, because the vanilla code uses `getParent` and manually
// calculates the child location in the world space based on that rather than
// using `convertToNodeSpace`.
static constexpr int INPUT_TAG = 0x80082;

#include <Geode/modify/CCTextInputNode.hpp>

struct TextInputNodeFix : Modify<TextInputNodeFix, CCTextInputNode> {
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
        if (this->getTag() != INPUT_TAG) return CCTextInputNode::ccTouchBegan(touch, event);

        if (!this->isVisible()) {
            this->onClickTrackNode(false);
            return false;
        }

        auto const touchPos = touch->getLocation();
        auto const size = this->getContentSize();
        auto const pos = this->convertToNodeSpace(touchPos) + m_textField->getAnchorPoint() * size;

        if (pos.x < 0 || pos.x > size.width || pos.y < 0 || pos.y > size.height) {
            this->onClickTrackNode(false);
            return false;
        }
        if (m_delegate && !m_delegate->allowTextInput(this)) {
            this->onClickTrackNode(false);
            return false;
        }

        this->onClickTrackNode(true);
        this->updateCursorPosition(touchPos, {{0, 0}, size});

        return true;
    }
};

std::string InputNode::getString() {
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

    m_input = CCTextInputNode::create(width - 10.0f, height, placeholder, "Thonburi", 24, font);
    m_input->setLabelPlaceholderColor({ 150, 150, 150 });
    m_input->setLabelPlaceholderScale(.75f);
    m_input->setMaxLabelScale(.85f);
    m_input->setMaxLabelLength(maxCharCount);
    m_input->setPosition(width / 2, height / 2);
    m_input->setTag(INPUT_TAG);
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
