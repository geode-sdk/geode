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

class TextInput::Impl final {
public:
    NineSlice* bgSprite = nullptr;
    CCTextInputNode* input = nullptr;
    geode::Function<void(std::string const&)> onInput = nullptr;
    cocos2d::CCLabelBMFont* label = nullptr;
    bool callbackEnabled = true;
};

TextInput::TextInput() : m_impl(std::make_unique<Impl>()) { }

bool TextInput::init(float width, ZStringView placeholder, ZStringView font) {
    if (!CCNode::init())
        return false;

    constexpr float HEIGHT = 30.f;

    this->setContentSize({ width, HEIGHT });
    this->setAnchorPoint({ .5f, .5f });

    m_impl->bgSprite = NineSlice::create("square02b_001.png", { 0, 0, 80, 80 });
    m_impl->bgSprite->setScale(.5f);
    m_impl->bgSprite->setColor({ 0, 0, 0 });
    m_impl->bgSprite->setOpacity(90);
    m_impl->bgSprite->setContentSize({ width * 2, HEIGHT * 2 });
    this->addChildAtPosition(m_impl->bgSprite, Anchor::Center);

    m_impl->input = CCTextInputNode::create(width - 10.f, HEIGHT, placeholder.c_str(), 24, font.c_str());
    m_impl->input->setLabelPlaceholderColor({ 150, 150, 150 });
    m_impl->input->setLabelPlaceholderScale(.5f);
    m_impl->input->setMaxLabelScale(.6f);
    m_impl->input->setUserObject("fix-text-input", CCBool::create(true));
    this->addChildAtPosition(m_impl->input, Anchor::Center);

    return true;
}

TextInput* TextInput::create(float width, ZStringView placeholder, ZStringView font) {
    auto ret = new TextInput();
    if (ret->init(width, placeholder, font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void TextInput::textChanged(CCTextInputNode* input) {
    if (m_impl->onInput && m_impl->callbackEnabled) {
        m_impl->onInput(input->getString());
    }
}

void TextInput::setPlaceholder(gd::string placeholder) {
    m_impl->input->m_caption = std::move(placeholder);
    m_impl->input->refreshLabel();
}

void TextInput::setLabel(ZStringView label) {
    if (label.size()) {
        if (m_impl->label) {
            m_impl->label->setString(label.c_str());
        }
        else {
            m_impl->label = CCLabelBMFont::create(label.c_str(), "goldFont.fnt");
            this->addChildAtPosition(m_impl->label, Anchor::TopLeft, ccp(3, 2), ccp(0, 0));
        }
        m_impl->label->limitLabelWidth(m_impl->bgSprite->getScaledContentWidth() - 6, .4f, .1f);
    }
    else {
        if (m_impl->label) {
            m_impl->label->removeFromParent();
            m_impl->label = nullptr;
        }
    }
}
void TextInput::setFilter(gd::string allowedChars) {
    m_impl->input->m_allowedChars = std::move(allowedChars);
}
void TextInput::setCommonFilter(CommonFilter filter) {
    this->setFilter(getCommonFilterAllowedChars(filter));
}
void TextInput::setMaxCharCount(size_t length) {
    m_impl->input->m_maxLabelLength = length == 0 ? 9999999 : length;
}
void TextInput::setPasswordMode(bool enable) {
    m_impl->input->m_usePasswordChar = enable;
    m_impl->input->refreshLabel();
}
void TextInput::setWidth(float width) {
    this->setContentWidth(width);
    m_impl->input->m_maxLabelWidth = width - 10.f;
    m_impl->input->setContentWidth(width);
    m_impl->bgSprite->setContentWidth(width * 2);
    m_impl->input->setPositionX(width / 2.f);
    m_impl->bgSprite->setPositionX(width / 2.f);
}
void TextInput::setDelegate(TextInputDelegate* delegate, std::optional<int> tag) {
    m_impl->input->m_delegate = delegate;
    m_impl->onInput = nullptr;
    if (tag.has_value()) {
        m_impl->input->setTag(tag.value());
    }
}
void TextInput::setCallback(geode::Function<void(std::string const&)> onInput) {
    this->setDelegate(this);
    m_impl->onInput = std::move(onInput);
}
void TextInput::setCallbackEnabled(bool enabled) {
    m_impl->callbackEnabled = enabled;
}
void TextInput::setEnabled(bool enabled) {
    m_impl->input->setTouchEnabled(enabled);
    m_impl->input->m_textLabel->setOpacity(enabled ? 255 : 150);
}
void TextInput::setTextAlign(TextInputAlign align) {
    switch (align) {
        default:
        case TextInputAlign::Center: {
            m_impl->input->m_textField->setAnchorPoint({ .5f, .5f });
            m_impl->input->m_textLabel->setAnchorPoint({ .5f, .5f });
            m_impl->input->updateAnchoredPosition(Anchor::Center);
        } break;

        case TextInputAlign::Left: {
            m_impl->input->m_textField->setAnchorPoint({ .0f, .5f });
            m_impl->input->m_textLabel->setAnchorPoint({ .0f, .5f });
            m_impl->input->updateAnchoredPosition(Anchor::Left, ccp(5, 0));
        } break;
    }
}

void TextInput::hideBG() {
    m_impl->bgSprite->setVisible(false);
}

void TextInput::setString(gd::string str, bool triggerCallback) {
    auto oldDelegate = m_impl->input->m_delegate;
    // Avoid triggering the callback
    m_impl->input->m_delegate = nullptr;
    m_impl->input->setString(std::move(str));
    m_impl->input->m_delegate = oldDelegate;
    if (triggerCallback && m_impl->input->m_delegate) {
        m_impl->input->m_delegate->textChanged(m_impl->input);
    }
}

gd::string TextInput::getString() const {
    return m_impl->input->getString();
}
bool TextInput::isCallbackEnabled() const {
    return m_impl->callbackEnabled;
}

void TextInput::focus() {
    m_impl->input->onClickTrackNode(true);
}
void TextInput::defocus() {
    m_impl->input->detachWithIME();
}

CCTextInputNode* TextInput::getInputNode() const {
    return m_impl->input;
}
NineSlice* TextInput::getBGSprite() const {
    return m_impl->bgSprite;
}

TextInput::~TextInput() {
    if (m_impl->input) {
        m_impl->input->onClickTrackNode(false);
        m_impl->input->m_delegate = nullptr;
    }
}
