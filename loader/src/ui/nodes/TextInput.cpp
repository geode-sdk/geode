#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/TextInputDelegate.hpp>
#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

struct TextInputNodeFix : Modify<TextInputNodeFix, CCTextInputNode> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("TextInputNode fix")

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        if (!this->getUserObject("fix-text-input")) {
            return CCTextInputNode::ccTouchBegan(touch, event);
        }

        if (!nodeIsVisible(this)) {
            this->onClickTrackNode(false);
            return false;
        }

        auto touchPos = touch->getLocation();
        auto const size = this->getContentSize();
        auto const pos = this->convertToNodeSpace(touchPos) + m_textField->getAnchorPoint() * size;

        float parentScale = 1.f;
        CCNode* currentParent = this;

        while ((currentParent = currentParent->getParent())) {
            parentScale *= currentParent->getScale();
        }

        CCPoint nodeSpace = this->convertToNodeSpace(touchPos);
        nodeSpace = nodeSpace / parentScale;
        touchPos = this->convertToWorldSpace(nodeSpace);

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

const char* geode::getCommonFilterAllowedChars(CommonFilter filter) {
    switch (filter) {
        default:
        case CommonFilter::Uint:         return "0123456789";
        case CommonFilter::Int:          return "-0123456789";
        case CommonFilter::Float:        return "-.0123456789";
        case CommonFilter::ID:           return "abcdefghijklmnopqrstuvwxyz0123456789-_.";
        case CommonFilter::Name:         return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_ ";
        case CommonFilter::Any:          return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ#_-+/\\&$%^~*\'\"{}()[]<>=!?@,;.:|• ";
        case CommonFilter::Hex:          return "0123456789abcdefABCDEF";
        case CommonFilter::Base64Normal: return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/=";
        case CommonFilter::Base64URL:    return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_=";
        case CommonFilter::Alphanumeric: return "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        case CommonFilter::Alphabetic:   return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
}

bool TextInput::init(float width, std::string const& placeholder, std::string const& font) {
    if (!CCNode::init())
        return false;

    constexpr float HEIGHT = 30.f;

    this->setContentSize({ width, HEIGHT });
    this->setAnchorPoint({ .5f, .5f });

    m_bgSprite = extension::CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    m_bgSprite->setScale(.5f);
    m_bgSprite->setColor({ 0, 0, 0 });
    m_bgSprite->setOpacity(90);
    m_bgSprite->setContentSize({ width * 2, HEIGHT * 2 });
    this->addChildAtPosition(m_bgSprite, Anchor::Center);

    m_input = CCTextInputNode::create(width - 10.f, HEIGHT, placeholder.c_str(), 24, font.c_str());
    m_input->setLabelPlaceholderColor({ 150, 150, 150 });
    m_input->setLabelPlaceholderScale(.5f);
    m_input->setMaxLabelScale(.6f);
    m_input->setUserObject("fix-text-input", CCBool::create(true));
    this->addChildAtPosition(m_input, Anchor::Center);

    return true;
}

TextInput* TextInput::create(float width, std::string const& placeholder, std::string const& font) {
    auto ret = new TextInput();
    if (ret->init(width, placeholder, font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void TextInput::textChanged(CCTextInputNode* input) {
    if (m_onInput) {
        m_onInput(input->getString());
    }
}

void TextInput::setPlaceholder(std::string const& placeholder) {
    m_input->m_caption = placeholder;
    m_input->refreshLabel();
}
void TextInput::setLabel(std::string const& label) {
    if (label.size()) {
        if (m_label) {
            m_label->setString(label.c_str());
        }
        else {
            m_label = CCLabelBMFont::create(label.c_str(), "goldFont.fnt");
            this->addChildAtPosition(m_label, Anchor::TopLeft, ccp(3, 2), ccp(0, 0));
        }
        m_label->limitLabelWidth(m_bgSprite->getScaledContentWidth() - 6, .4f, .1f);
    }
    else {
        if (m_label) {
            m_label->removeFromParent();
            m_label = nullptr;
        }
    }
}
void TextInput::setFilter(std::string const& allowedChars) {
    m_input->m_allowedChars = allowedChars;
}
void TextInput::setCommonFilter(CommonFilter filter) {
    this->setFilter(getCommonFilterAllowedChars(filter));
}
void TextInput::setMaxCharCount(size_t length) {
    m_input->m_maxLabelLength = length == 0 ? 9999999 : length;
}
void TextInput::setPasswordMode(bool enable) {
    m_input->m_usePasswordChar = enable;
    m_input->refreshLabel();
}
void TextInput::setWidth(float width) {
    this->setContentWidth(width);
    m_input->m_maxLabelWidth = width - 10.f;
    m_input->setContentWidth(width);
    m_bgSprite->setContentWidth(width * 2);
    m_input->setPositionX(width / 2.f);
    m_bgSprite->setPositionX(width / 2.f);
}
void TextInput::setDelegate(TextInputDelegate* delegate, std::optional<int> tag) {
    m_input->m_delegate = delegate;
    m_onInput = nullptr;
    if (tag.has_value()) {
        m_input->setTag(tag.value());
    }
}
void TextInput::setCallback(std::function<void(std::string const&)> onInput) {
    this->setDelegate(this);
    m_onInput = onInput;
}
void TextInput::setEnabled(bool enabled) {
    m_input->setTouchEnabled(enabled);
    m_input->m_textLabel->setOpacity(enabled ? 255 : 150);
}
void TextInput::setTextAlign(TextInputAlign align) {
    switch (align) {
        default:
        case TextInputAlign::Center: {
            m_input->m_textField->setAnchorPoint({ .5f, .5f });
            m_input->m_textLabel->setAnchorPoint({ .5f, .5f });
            m_input->updateAnchoredPosition(Anchor::Center);
        } break;

        case TextInputAlign::Left: {
            m_input->m_textField->setAnchorPoint({ .0f, .5f });
            m_input->m_textLabel->setAnchorPoint({ .0f, .5f });
            m_input->updateAnchoredPosition(Anchor::Left, ccp(5, 0));
        } break;
    }
}

void TextInput::hideBG() {
    m_bgSprite->setVisible(false);
}

void TextInput::setString(std::string const& str, bool triggerCallback) {
    auto oldDelegate = m_input->m_delegate;
    // Avoid triggering the callback
    m_input->m_delegate = nullptr;
    m_input->setString(str);
    m_input->m_delegate = oldDelegate;
    if (triggerCallback && m_input->m_delegate) {
        m_input->m_delegate->textChanged(m_input);
    }
}

std::string TextInput::getString() const {
    return m_input->getString();
}

void TextInput::focus() {
    m_input->onClickTrackNode(true);
}
void TextInput::defocus() {
    m_input->detachWithIME();
}

CCTextInputNode* TextInput::getInputNode() const {
    return m_input;
}
CCScale9Sprite* TextInput::getBGSprite() const {
    return m_bgSprite;
}
