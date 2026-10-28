#include <Geode/ui/TextArea.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/terminate.hpp>
#include <Geode/cocos/robtop/sprite_nodes/CCFontSprite.h>
#include <deque>

#include <stack>
#include <memory>
#include <optional>
#include <regex>

#include <fmt/chrono.h>
#include <asp/iter.hpp>

class geode::SimpleTextAreaImpl {
public:
    bool m_shouldUpdate = false;
    bool m_artificialWidth = false;
    cocos2d::CCMenu* m_container = nullptr;
    std::string m_font;
    std::string m_text;
    std::vector<geode::Label*> m_lines;
    cocos2d::ccColor4B m_color = { 0xFF, 0xFF, 0xFF, 0xFF };
    cocos2d::CCTextAlignment m_alignment = cocos2d::kCCTextAlignmentLeft;
    geode::WrappingMode m_wrappingMode = geode::WrappingMode::WORD_WRAP;
    size_t m_maxLines = 0;
    float m_scale = 1.f;
    float m_lineHeight = 0.f;
    float m_linePadding = 0.f;

    geode::SimpleTextArea* m_self = nullptr;

    SimpleTextAreaImpl(geode::SimpleTextArea* self) : m_self(self) {}

    virtual geode::Label* createLabel(std::string text, float top);

    float calculateOffset(geode::Label* label);

    virtual void charIteration(geode::FunctionRef<geode::Label*(geode::Label* line, char c, float top)> overflowHandling);

    void updateLinesNoWrap();

    void updateLinesWordWrap(bool spaceWrap);

    void updateLinesCutoffWrap();

    void updateContainer();

    virtual void setTextImpl(std::string text);
};

geode::Label* geode::SimpleTextAreaImpl::createLabel(std::string text, float top) {
    if (m_maxLines && m_lines.size() >= m_maxLines) {
        geode::Label* last = m_lines.at(m_maxLines - 1);
        std::string_view textv = last->getString();

        last->setText(fmt::format("{}...", textv.substr(0, textv.size() - 3)));

        return nullptr;
    } else {
        geode::Label* label = geode::Label::createRich(std::move(text), m_font.c_str());

        label->setScale(m_scale);
        label->setPosition({ 0, top });
        label->setColor({ m_color.r, m_color.g, m_color.b });
        label->setOpacity(m_color.a);

        return label;
    }
}

float geode::SimpleTextAreaImpl::calculateOffset(geode::Label* label) {
    return m_linePadding + label->getContentSize().height * m_scale;
}

void geode::SimpleTextAreaImpl::charIteration(geode::FunctionRef<geode::Label*(geode::Label* line, char c, float top)> overflowHandling) {
    float top = 0;
    m_lines.clear();
    geode::Label* line = createLabel("", top);
    m_lines = { line };

    for (const char c : m_text) {
        if (c == '\n') {
            line = createLabel("", top -= calculateOffset(line));

            if (line == nullptr) {
                break;
            } else {
                m_lines.push_back(line);
            }
        } else if (m_artificialWidth && line->getContentWidth() * m_scale >= m_self->getWidth()) {
            line = overflowHandling(line, c, top -= calculateOffset(line));

            if (line == nullptr) {
                break;
            } else {
                m_lines.push_back(line);
            }
        } else {
            line->setText((std::string(line->getString()) + c));
        }
    }
}

void geode::SimpleTextAreaImpl::updateLinesNoWrap() {
    std::stringstream stream(m_text);
    std::string part;
    float top = 0;
    m_lines.clear();

    while (std::getline(stream, part)) {
        Label* line = createLabel(std::move(part), top);
        part.clear();

        if (line == nullptr) {
            break;
        } else {
            top -= calculateOffset(line);

            m_lines.push_back(line);
        }
    }
}

void geode::SimpleTextAreaImpl::updateLinesWordWrap(bool spaceWrap) {
    charIteration([this, spaceWrap](geode::Label* line, char c, float top) {
        const std::string_view delimiters(spaceWrap ? " " : " `~!@#$%^&*()-_=+[{}];:'\",<.>/?\\|");

        if (!delimiters.contains(c)) {
            const std::string& text = line->getString();
            const size_t position = text.find_last_of(delimiters) + 1;
            geode::Label* newLine = createLabel(text.substr(position) + c, top);

            if (newLine != nullptr) {
                line->setText(text.substr(0, position));
            }

            return newLine;
        } else {
            return createLabel(std::string(c != ' ', c), top);
        }
    });
}

void geode::SimpleTextAreaImpl::updateLinesCutoffWrap() {
    charIteration([this](geode::Label* line, char c, float top) {
        const std::string& text = line->getString();
        const char back = text.back();
        const bool lastIsSpace = back == ' ';
        geode::Label* newLine = createLabel(std::string(!lastIsSpace, back).append(std::string(c != ' ', c)), top);

        if (newLine == nullptr && !lastIsSpace) {
            if (text[text.size() - 2] == ' ') {
                line->setText(text.substr(0, text.size() - 1));
            } else {
                line->setText(text.substr(0, text.size() - 1) + '-');
            }
        }

        return newLine;
});

}

void geode::SimpleTextAreaImpl::updateContainer() {
    switch (m_wrappingMode) {
        case geode::NO_WRAP: {
            updateLinesNoWrap();
        } break;
        case geode::WORD_WRAP: {
            updateLinesWordWrap(false);
        } break;
        case geode::SPACE_WRAP: {
            updateLinesWordWrap(true);
        } break;
        case geode::CUTOFF_WRAP: {
            updateLinesCutoffWrap();
        } break;
    }

    const size_t lineCount = m_lines.size();
    const float width = m_self->getWidth();

    if (lineCount > 0) {
        m_lineHeight = m_lines.back()->getContentSize().height * m_scale;
    } else {
        m_lineHeight = 0;
    }

    const float height = m_lineHeight * lineCount + m_linePadding * (lineCount - 1);

    m_self->setContentSize({ width, height });
    m_container->setContentSize(m_self->getContentSize());
    m_container->removeAllChildren();

    for (geode::Label* line : m_lines) {
        const float y = height + line->getPositionY();

        switch (m_alignment) {
            case cocos2d::kCCTextAlignmentLeft: {
                line->setAnchorPoint({ 0, 1 });
                line->setPosition({ 0, y });
            } break;
            case cocos2d::kCCTextAlignmentCenter: {
                line->setAnchorPoint({ 0.5f, 1 });
                line->setPosition({ width / 2, y });
            } break;
            case cocos2d::kCCTextAlignmentRight: {
                line->setAnchorPoint({ 1, 1 });
                line->setPosition({ width, y });
            } break;
        }

        m_container->addChild(line);
    }
}

void geode::SimpleTextAreaImpl::setTextImpl(std::string text){
    m_text = std::move(text);
    updateContainer();
}

geode::SimpleTextArea::SimpleTextArea() = default;
geode::SimpleTextArea::~SimpleTextArea() = default;

geode::SimpleTextArea* geode::SimpleTextArea::create(std::string text, std::string font, float scale) {
    return SimpleTextArea::create(
        std::move(font), std::move(text), scale,
        cocos2d::CCDirector::sharedDirector()->getWinSize().width / 2,
        false
    );
}

geode::SimpleTextArea* geode::SimpleTextArea::create(std::string text, std::string font, float scale, float width) {
    return SimpleTextArea::create(
        std::move(font), std::move(text), scale, width, true
    );
}

geode::SimpleTextArea* geode::SimpleTextArea::create(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    SimpleTextArea* instance = new SimpleTextArea();
    instance->m_impl = instance->createImpl();

    if (instance->init(std::move(font), std::move(text), scale, width, artificialWidth)) {
        instance->autorelease();
        return instance;
    }

    delete instance;
    return nullptr;
}

bool geode::SimpleTextArea::init(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    m_impl->m_font = std::move(font);
    m_impl->m_text = std::move(text);
    m_impl->m_scale = scale;
    m_impl->m_artificialWidth = artificialWidth;
    m_impl->m_container = cocos2d::CCMenu::create();

    this->setAnchorPoint({ 0.5f, 0.5f });
    m_impl->m_container->setPosition({ 0, 0 });
    m_impl->m_container->setAnchorPoint({ 0, 1 });
    m_impl->m_container->setContentSize({ width, 0 });
    this->addChild(m_impl->m_container);
    m_impl->updateContainer();

    return true;
}

void geode::SimpleTextArea::setFont(std::string font) {
    m_impl->m_font = std::move(font);
    m_impl->updateContainer();
}

std::string geode::SimpleTextArea::getFont() {
    return m_impl->m_font;
}

void geode::SimpleTextArea::setColor(const cocos2d::ccColor4B& color) {
    m_impl->m_color = color;
    m_impl->updateContainer();
}

cocos2d::ccColor4B geode::SimpleTextArea::getColor() {
    return m_impl->m_color;
}

void geode::SimpleTextArea::setAlignment(cocos2d::CCTextAlignment alignment) {
    m_impl->m_alignment = alignment;
    m_impl->updateContainer();
}

cocos2d::CCTextAlignment geode::SimpleTextArea::getAlignment() {
    return m_impl->m_alignment;
}

void geode::SimpleTextArea::setWrappingMode(WrappingMode mode) {
    m_impl->m_wrappingMode = mode;
    m_impl->updateContainer();
}

geode::WrappingMode geode::SimpleTextArea::getWrappingMode() {
    return m_impl->m_wrappingMode;
}

void geode::SimpleTextArea::setText(std::string text) {
    m_impl->setTextImpl(std::move(text));
}

std::string geode::SimpleTextArea::getText() {
    return m_impl->m_text;
}

void geode::SimpleTextArea::setMaxLines(size_t maxLines) {
    m_impl->m_maxLines = maxLines;
    m_impl->updateContainer();
}

size_t geode::SimpleTextArea::getMaxLines() {
    return m_impl->m_maxLines;
}

void geode::SimpleTextArea::setWidth(float width) {
    m_impl->m_artificialWidth = true;
    m_impl->updateContainer();

    this->setContentSize({ width, this->getContentSize().height });
    m_impl->m_container->setContentSize(this->getContentSize());
}

float geode::SimpleTextArea::getWidth() {
    return m_impl->m_container->getContentSize().width;
}

void geode::SimpleTextArea::setScale(float scale) {
    m_impl->m_scale = scale;
    m_impl->updateContainer();
}

float geode::SimpleTextArea::getScale() {
    return m_impl->m_scale;
}

void geode::SimpleTextArea::setLinePadding(float padding) {
    m_impl->m_linePadding = padding;
    m_impl->updateContainer();
}

float geode::SimpleTextArea::getLinePadding() {
    return m_impl->m_linePadding;
}

std::vector<geode::Label*> geode::SimpleTextArea::getLines() {
    return m_impl->m_lines;
}

float geode::SimpleTextArea::getHeight() {
    return m_impl->m_container->getContentSize().height;
}

float geode::SimpleTextArea::getLineHeight() {
    return m_impl->m_lineHeight;
}

inline std::unique_ptr<geode::SimpleTextAreaImpl> geode::SimpleTextArea::createImpl() {
    return std::make_unique<geode::SimpleTextAreaImpl>(this);
}

using namespace geode::prelude;

class NewRichTextArea::RichImpl : public SimpleTextAreaImpl {
public:
    std::vector<RichKey> m_keys;
    EmojiRegistry m_emojiRegistry;
    std::string m_rawText;
    std::deque<std::u32string> m_keyIRs;

    size_t m_idx = 0;

    void parseRichText();
    void registerKey(const std::string& ir);
    CCLabelBMFont* getErrorText();

    char current();
    std::optional<char> peek();
    void advance(int offset = 1);

    void addRichKey(std::string id, ValidateArgs validate, CreateCallback, CharIterator iterator, bool updateText);
    void updateRichText();

    Label* createLabel(std::string text, float top) override;

    NewRichTextArea* m_self = nullptr;
    RichImpl(NewRichTextArea* self) : m_self(self), SimpleTextAreaImpl(self) {}
};

enum class ParseState {
    /// Normal state, don't parse anything
    Normal,
    /// Parse the opening tag
    ParseKey,
    /// Parse the opening tag's arguments
    ParseArgs,
    /// Parse the text in between the tags
    ParseText,
    /// Parse the closing tag
    ParseClosing
};

void NewRichTextArea::RichImpl::parseRichText() {
    m_idx = 0;
    m_rawText = m_text;
    std::string newText;
    ParseState state = ParseState::Normal;

    RichKey* key;
    std::stack<std::tuple<RichKey*, std::string>> prevKeys;
    std::string id;
    std::string args;
    std::string text;
    size_t keyStart = 0;

    auto handleError = [&] {
        newText += m_text.substr(keyStart, m_idx - keyStart + 1);
        state = ParseState::Normal;
        id.clear();
        args.clear();
        text.clear();
    };

    // IR = Intermediate Representation
    auto makeIR = [&] -> std::string {
        return fmt::format(
            ":{}{}|{}:",
            id, args.empty() ? "" : "-" + args,
            text
        );
    };

    while (m_idx < m_text.size()) {
        char c = this->current();
        switch (state) {
            case ParseState::Normal:
                if (c == '<') {
                    auto p = this->peek();
                    if (p && *p == '/') {
                        newText += c;
                        break;
                    }
                    keyStart = m_idx;
                    state = ParseState::ParseKey;
                } else {
                    newText += c;
                }
                break;
            case ParseState::ParseKey:
                id += c;
                if (auto p = this->peek(); p && (*p == '>' || *p == '=' || *p == '-')) {
                    auto match = std::ranges::find(m_keys, id, &RichKey::id);
                    if (match == m_keys.end()) {
                        handleError();
                        break;
                    }
                    key = &*match;
                    state = *p == '>' ? ParseState::ParseText : ParseState::ParseArgs;
                    this->advance();
                }
                break;
            case ParseState::ParseArgs:
                if (c == '>') {
                    if (!key->validate(args)) {
                        handleError();
                        break;
                    }
                    state = ParseState::ParseText;
                    break;
                }
                args += c;
                break;
            case ParseState::ParseText:
                if (c == '<') {
                    if (auto p = this->peek(); p && *p == '/') {
                        state = ParseState::ParseClosing;
                    } else {
                        auto ir = makeIR();
                        newText += ir;
                        this->registerKey(ir);
                        prevKeys.emplace(key, args);
                        state = ParseState::ParseKey;
                        id.clear();
                        args.clear();
                        text.clear();
                    }
                    break;
                }
                if (c == '\n') {
                    auto ir = makeIR();
                    newText += ir + "\n";
                    this->registerKey(ir);
                    text.clear();
                    break;
                }
                text += c;
                break;
            case ParseState::ParseClosing: {
                if (c == '/') {
                    this->advance();
                }
                std::string closeID;
                while (m_idx < m_text.size() && this->current() != '>') {
                    char ch = this->current();
                    closeID += ch;
                    this->advance();
                }
                if (m_idx >= m_text.size()) {
                    handleError();
                    break;
                }
                if (closeID != id) {
                    handleError();
                    break;
                }
                auto ir = makeIR();
                newText += ir;
                this->registerKey(ir);
                if (!prevKeys.empty()) {
                    auto prev = prevKeys.top();
                    key = std::get<0>(prev);
                    id = key->id;
                    args = std::get<1>(prev);
                    state = ParseState::ParseText;
                    prevKeys.pop();
                    text.clear();
                } else {
                    state = ParseState::Normal;
                    id.clear();
                    args.clear();
                    text.clear();
                }
                break;
            }
        }
        this->advance();
    }

    // If the parser ever stopped abruptly
    if (state != ParseState::Normal) {
        newText += m_text.substr(keyStart);
    }
    m_text = newText;
}

void NewRichTextArea::RichImpl::registerKey(const std::string& ir) {
    m_keyIRs.emplace_back(std::u32string(ir.begin(), ir.end()));
    m_emojiRegistry.insert(m_keyIRs.back(), [this](std::u32string_view str32, size_t& idx) -> CCNode* {
        auto str8 = string::utf32ToUtf8(str32);
        if (str8.isErr()) {
            log::error("Could not parse UTF-32: {}", str8.unwrapErr());
            // This will likely be able to tell you
            // something went wrong
            return this->getErrorText();
        }
        auto str = str8.unwrap();

        auto start = str.rfind(":", idx - 1);
        if (start == std::string::npos) {
            log::error("Missing ending ':' on intermediate (RichTextArea)\n\nText: {}, Index: {}", str, idx);
            return this->getErrorText();
        }
        auto inter = str.substr(start + 1, idx - start - 1);

        // Split text and key
        auto split = string::split(inter, "|");

        std::string keyStr;
        std::string arg;
        if (string::contains(split[0], "-")) {
            auto k = string::split(split[0], "-");
            keyStr = k[0];
            arg = k[1];
        } else {
            keyStr = split[0];
        }

        auto key = std::ranges::find(m_keys, keyStr, &RichKey::id);
        if (key == m_keys.end()) {
            log::error("Could not find key '{}'", keyStr);
            return this->getErrorText();
        }

        if (key->create) {
            return key->create(split[1], arg);
        } else {
            // geode::Label doesn't have an easy method to add per-character
            // effects (ty quads) so we have to resort to this
            auto ret = CCLabelBMFont::create(split[1].c_str(), m_font.c_str());
            ret->setColor(to3B(m_color));
            ret->setOpacity(m_color.a);

            if (key->iterator) {
                for (auto& c : CCArrayExt(ret->getChildren())) {
                    key->iterator(static_cast<CCFontSprite*>(c), arg);
                }
            }

            return ret;
        }
    });
}

CCLabelBMFont* NewRichTextArea::RichImpl::getErrorText() {
    auto ret = CCLabelBMFont::create("ERROR", "bigFont.fnt");
    ret->setColor({ 255, 0, 0 });
    return ret;
}

char NewRichTextArea::RichImpl::current() {
    return m_text[m_idx];
}

std::optional<char> NewRichTextArea::RichImpl::peek() {
    auto i = m_idx + 1;
    if (i >= m_text.size()) {
        return std::nullopt;
    }
    return m_text[i];
}

void NewRichTextArea::RichImpl::advance(int offset) {
    m_idx += offset;
}

void NewRichTextArea::RichImpl::addRichKey(
    std::string id,
    ValidateArgs validate,
    CreateCallback create,
    CharIterator iterator,
    bool updateText
) {
    m_keys.emplace_back(std::move(id), std::move(validate), std::move(create), std::move(iterator));
    if (updateText) {
        m_text = m_rawText;
        this->updateRichText();
    }
}

void NewRichTextArea::RichImpl::updateRichText() {
    this->parseRichText();
    this->updateContainer();
}

Label* NewRichTextArea::RichImpl::createLabel(std::string text, float top) {
    auto label = SimpleTextAreaImpl::createLabel(text, top);
    if (!label) return nullptr;
    label->setEmojiRegistry(m_emojiRegistry);
    return label;
}

NewRichTextArea* NewRichTextArea::create(std::string text, std::string font, float scale) {
    return create(
        std::move(font), std::move(text), scale,
        CCDirector::sharedDirector()->getWinSize().width / 2, false
    );
}

NewRichTextArea* NewRichTextArea::create(std::string text, std::string font, float scale, float width) {
    return create(std::move(font), std::move(text), scale, width, true);
}

NewRichTextArea* NewRichTextArea::create(std::string font, std::string text, float scale, float width, const bool artificialWidth) {
    auto ret = new NewRichTextArea();
    ret->m_impl = ret->createImpl();
    if (ret->init(std::move(font), std::move(text), scale, width, artificialWidth)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool NewRichTextArea::init(std::string font, std::string text, float scale, float width, const bool artificialWidth) {
    if (!SimpleTextArea::init(font, text, scale, width, artificialWidth)) return false;
    m_richImpl->m_rawText = m_impl->m_text;

    this->addRichKey("red", [](CCFontSprite* ch, std::string_view args) {
        ch->setColor({ 255, 0, 0 });
    }, false);

    m_richImpl->updateRichText();
    return true;
}

void NewRichTextArea::addRichKey(std::string id, CreateCallback create, bool updateText) {
    m_richImpl->addRichKey(std::move(id), [](auto) { return true; }, std::move(create), nullptr, updateText);
}

void NewRichTextArea::addRichKey(std::string id, ValidateArgs validate, CreateCallback create, bool updateText) {
    m_richImpl->addRichKey(std::move(id), std::move(validate), std::move(create), nullptr, updateText);
}

void NewRichTextArea::addRichKey(std::string id, CharIterator iterator, bool updateText) {
    m_richImpl->addRichKey(std::move(id), [](auto) { return true; }, nullptr, std::move(iterator), updateText);
}

void NewRichTextArea::addRichKey(std::string id, ValidateArgs validate, CharIterator iterator, bool updateText) {
    m_richImpl->addRichKey(std::move(id), std::move(validate), nullptr, std::move(iterator), updateText);
}

void NewRichTextArea::updateRichText() {
    m_richImpl->m_text = m_richImpl->m_rawText;
    m_richImpl->updateRichText();
}

std::unique_ptr<SimpleTextAreaImpl> NewRichTextArea::createImpl() {
    auto ret = std::make_unique<RichImpl>(this);
    m_richImpl = ret.get();
    return ret;
}

class RichTextArea::RichImpl : public SimpleTextAreaImpl {
public:
    RichTextArea* m_self = nullptr;
    RichImpl(RichTextArea* self)  : m_self(self), SimpleTextAreaImpl(self) {}

    std::map<std::string, std::shared_ptr<RichTextKeyBase>> m_richTextKeys;
    std::map<int, std::vector<std::shared_ptr<RichTextKeyInstanceBase>>> m_richTextInstances;
    std::map<std::shared_ptr<RichTextKeyInstanceBase>, std::set<CCFontSprite*>> m_charactersForButton{};
    std::shared_ptr<RichTextKeyInstanceBase> m_currentlyHeldButton = nullptr;

    std::string m_rawText;

    std::map<CCFontSprite*, ccColor3B> m_ogColorForLink{};

    void charIteration(geode::FunctionRef<geode::Label*(geode::Label* line, char c, float top)> overflowHandling) override;
    void parseRichText();

    void processLinkClick(
        std::string const& link,
        bool keyDown,
        cocos2d::CCFontSprite* specificSpriteClicked,
        std::set<cocos2d::CCFontSprite*> const& wordClicked
    );

    void setTextImpl(std::string text) override;
};

RichTextArea* RichTextArea::create(std::string text, std::string font, float scale) {
    return RichTextArea::create(std::move(font), std::move(text), scale, CCDirector::sharedDirector()->getWinSize().width / 2, false);
}

RichTextArea* RichTextArea::create(std::string text, std::string font, float scale, float width) {
    return RichTextArea::create(std::move(font), std::move(text), scale, width, true);
}

RichTextArea* RichTextArea::create(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    RichTextArea* instance = new RichTextArea();
    instance->m_impl = instance->createImpl();

    if (instance->init(std::move(font), std::move(text), scale, width, artificialWidth)) {
        instance->autorelease();
        return instance;
    }

    delete instance;
    return nullptr;
}

bool RichTextArea::init(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    if (!SimpleTextArea::init(font, text, scale, width, artificialWidth)) return false;

    addRichKey(std::make_shared<RichTextKey<ccColor3B>>(
        "color",
        [](std::string value) -> Result<ccColor3B> {
            auto colorRes = cc3bFromHexString(value);
            if (colorRes.isErr()) return Err(colorRes.unwrapErr());

            return Ok(colorRes.unwrap());
        },
        [](ccColor3B const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex) {
            sprite->setColor({ value.r, value.g, value.b });
        }
    ));

    addRichKey(std::make_shared<RichTextKey<bool>>(
        "flip",
        [](std::string value) -> Result<bool> {
            if (value == "") return Ok(true);

            if (value != "true" && value != "false") {
                return Err("Value must be 'true' or 'false'");
            }

            return Ok(value == "true");
        },
        [](bool const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex) {
            sprite->setFlipY(value);
        }
    ));

    addRichKey(std::make_shared<RichTextKey<bool>>(
        "mirror",
        [](std::string value) -> Result<bool> {
            if (value == "") return Ok(true);

            if (value != "true" && value != "false") {
                return Err("Value must be 'true' or 'false'");
            }

            return Ok(value == "true");
        },
        [](bool const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex) {
            sprite->setFlipX(value);
        }
    ));

    addRichKey(std::make_shared<RichTextKey<std::time_t>>(
        "workingTime",
        [](std::string value) -> Result<std::time_t> {
            auto timeRes = geode::utils::numFromString<time_t>(value);
            if (timeRes.isErr()) return Ok(std::time(nullptr));

            return Ok(timeRes.unwrap());
        },
        [](std::time_t const& value) -> std::string {
            return fmt::format("{:%Y-%m-%d %H:%M:%S}", geode::localtime(value));
        }
    ));

    addRichKey(std::make_shared<RichTextKey<float>>(
        "size",
        [](std::string value) -> Result<float> {
            auto numRes = geode::utils::numFromString<float>(value);
            if (numRes.isErr()) return Err("size invalid!");

            return Ok(numRes.unwrap());
        },
        [](float const& value, cocos2d::CCFontSprite* sprite, int localIndex, int charIndex) {
            sprite->setScale(value);
        }
    ));

    // maybe one day someone will make this work but its fine :(
    // registerRichTextKey(std::make_shared<RichTextKey<std::string>>(
    //     "font",
    //     [](std::string value) -> Result<std::string> {
    //         auto temp = CCLabelBMFont::create("Test", value.c_str());
    //         if (temp == nullptr) return Err("Font file isn't valid!");

    //         return Ok(value);
    //     },
    //     [&](std::string const& value, CCFontSprite* const applyToSpr) {
    //         applyToSpr->retain();
    //         applyToSpr->removeFromParent();
    //         this->addChild(applyToSpr);
    //         applyToSpr->release();
    //     }
    // ));

    addRichKey(std::make_shared<RichTextKey<std::string>>(
        "link",
        [](std::string value) -> Result<std::string> {
            return Ok(value);
        },
        [&](std::string const& value, bool keyDown, cocos2d::CCFontSprite* specificSpriteClicked, std::set<cocos2d::CCFontSprite*> const& wordClicked) {
            this->castedImpl()->processLinkClick(value, keyDown, specificSpriteClicked, wordClicked);
        }
    ));

    castedImpl()->m_rawText = m_impl->m_text;

    castedImpl()->parseRichText();
    m_impl->updateContainer();

    return true;
}

std::string RichTextArea::getRawText(){
    return castedImpl()->m_rawText;
}

void RichTextArea::RichImpl::charIteration(geode::FunctionRef<geode::Label*(geode::Label* line, char c, float top)> overflowHandling) {
    float top = 0;
    m_lines.clear();
    geode::Label* line = this->createLabel("", top);
    m_lines = { line };

    std::map<std::string, std::shared_ptr<RichTextKeyInstanceBase>> appliedRichTextInstances{};

    std::map<std::shared_ptr<RichTextKeyInstanceBase>, int> m_charIndexForInstance{};

    m_charactersForButton.clear();

    int index = 0;
    for (const char& c : m_text) {
        if (m_richTextInstances.contains(index)){
            for (const auto& instancePtr : m_richTextInstances[index]) {
                if (appliedRichTextInstances.contains(instancePtr->getKey())) {
                    if (instancePtr->isCancellation()){
                        appliedRichTextInstances.erase(instancePtr->getKey());
                        m_charIndexForInstance.erase(instancePtr);
                    }
                    else{
                        appliedRichTextInstances[instancePtr->getKey()] = instancePtr;
                        m_charIndexForInstance[instancePtr] = 0;
                    }
                } else {
                    appliedRichTextInstances.insert({instancePtr->getKey(), instancePtr});
                    m_charIndexForInstance[instancePtr] = 0;
                }
            }
        }

        index++;

        if (c == '\n') {
            line = this->createLabel("", top -= this->calculateOffset(line));

            if (line == nullptr) {
                break;
            } else {
                m_lines.push_back(line);
            }
        } else if (m_artificialWidth && line->getContentWidth() * m_scale >= m_self->getWidth()) {
            line = overflowHandling(line, c, top -= this->calculateOffset(line));

            if (line == nullptr) {
                break;
            } else {
                m_lines.push_back(line);
            }
        } else {
            line->setText((std::string(line->getString()) + c));
        }

        if (line->getChildren()->lastObject() != nullptr){
            for (const auto& [key, instancePtr] : appliedRichTextInstances) {
                auto lastChar = static_cast<cocos2d::CCFontSprite*>(line->getChildren()->lastObject());

                instancePtr->applyChangesToSprite(lastChar, m_charIndexForInstance[instancePtr], index);

                if (instancePtr->isButton()){
                    if (!instancePtr->isCancellation()){
                        if (m_charactersForButton.contains(instancePtr)){
                            m_charactersForButton[instancePtr].insert(lastChar);
                        }
                        else{
                            m_charactersForButton.insert({instancePtr, {lastChar}});
                        }
                    }
                }

            }

            for (const auto& [instancePtr, _] : appliedRichTextInstances) {
                m_charIndexForInstance[_]++;
            }
        }
    }
}

void RichTextArea::RichImpl::parseRichText() {
    std::regex pattern(R"(<(\/)?([^=<>]+)(?:\s*=\s*([^<>]+))?>)");
    std::smatch match;

    m_richTextInstances.clear();

    // theres probably a better way to do this lol idk aa
    struct MatchInfo {
        int position;
        ptrdiff_t length;
        std::string key;
        std::string value;
        int overallOffset;
        bool cancellation;
    };
    std::vector<MatchInfo> matches;

    auto begin = m_text.cbegin();
    auto end = m_text.cend();

    int offset = 0;

    while (std::regex_search(begin, end, match, pattern)) {
        int matchStartPos = std::distance(m_text.cbegin(), match[0].first);

        std::string value;
        if (match.size() >= 4 && match[3].matched) value = match[3];

        matches.push_back({matchStartPos, match[0].length(), match[2], value, offset, match[1] == '/'});
        offset += match[0].length();

        begin = match.suffix().first;
    }

    std::map<int, std::vector<std::shared_ptr<RichTextKeyInstanceBase>>> richTextInstancesBeforeExtraOffset{};

    for (auto& m : asp::iter::fromReverse(matches)) {
        if (!m_richTextKeys.contains(m.key)) continue;

        auto result = m_richTextKeys[m.key]->createInstance(m.value, m.cancellation);

        if (result.isErr()) continue;

        int effectIndex = m.position - m.overallOffset;

        auto& keyRef = result.unwrap();

        if (richTextInstancesBeforeExtraOffset.contains(effectIndex)) {
            richTextInstancesBeforeExtraOffset[effectIndex].push_back(keyRef);
        } else {
            richTextInstancesBeforeExtraOffset[effectIndex] = {keyRef};
        }

        m_text.erase(m.position, m.length);
    }

    int textAdditionOverallOffset = 0;
    int prevExtraOffset = 0;

    for (auto& [index, keys] : richTextInstancesBeforeExtraOffset) {
        for (const auto& keyRef : keys)
        {
            auto currentAddition = keyRef->runStrAddition();
            if (currentAddition == "") continue;
            m_text.insert(index + textAdditionOverallOffset, currentAddition);
            textAdditionOverallOffset += currentAddition.length();
        }

        m_richTextInstances[index + prevExtraOffset] = std::move(keys);

        prevExtraOffset = textAdditionOverallOffset;
    }
}

void RichTextArea::RichImpl::processLinkClick(
    std::string const& link,
    bool keyDown,
    cocos2d::CCFontSprite* specificSpriteClicked,
    std::set<cocos2d::CCFontSprite*> const& wordClicked
){
    if (keyDown){
        for (const auto& linkCharacter : wordClicked)
        {
            this->m_ogColorForLink.insert({
                linkCharacter,
                linkCharacter->getColor() == ccColor3B{255, 255, 255} ?
                    ccColor3B{m_self->getColor().r, m_self->getColor().g, m_self->getColor().b} :
                    linkCharacter->getColor()
                }
            );

            linkCharacter->setColor({ 78, 78, 255 });
        }
    }
    else {
        for (const auto& linkCharacter : wordClicked) {
            if (this->m_ogColorForLink.contains(linkCharacter)) {
                linkCharacter->setColor(this->m_ogColorForLink[linkCharacter]);
            }
        }

        if (specificSpriteClicked != nullptr)
            web::openLinkInBrowser(link);
    }
}

void RichTextArea::RichImpl::setTextImpl(std::string text) {
    m_text = std::move(text);
    m_rawText = m_text;
    parseRichText();
    updateContainer();
}

void RichTextArea::addRichKey(std::shared_ptr<RichTextKeyBase> key){
    if (this->castedImpl()->m_richTextKeys.contains(key->getKey())) return;

    this->castedImpl()->m_richTextKeys.insert({key->getKey(), key});
}

bool RichTextArea::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    for (const auto& [btnKey, textForBtn] : this->castedImpl()->m_charactersForButton)
    {
        for (const auto& fontSpr : textForBtn)
        {
            if (fontSpr == nullptr) continue;

            auto touchInSpace = fontSpr->convertTouchToNodeSpace(pTouch);
            if (touchInSpace.x > 0 && touchInSpace.y > 0 && touchInSpace.x <= fontSpr->getContentWidth() && touchInSpace.y <= fontSpr->getContentHeight()){
                btnKey->callButton(true, fontSpr, textForBtn);
                this->castedImpl()->m_currentlyHeldButton = btnKey;
                return true;
            }
        }
    }

    return false;
}

void RichTextArea::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (this->castedImpl()->m_charactersForButton.contains(this->castedImpl()->m_currentlyHeldButton)){
        auto textForBtn = this->castedImpl()->m_charactersForButton[this->castedImpl()->m_currentlyHeldButton];

        for (const auto& fontSpr : textForBtn)
        {
            if (fontSpr == nullptr) continue;

            auto touchInSpace = fontSpr->convertTouchToNodeSpace(pTouch);
            if (touchInSpace.x > 0 && touchInSpace.y > 0 && touchInSpace.x <= fontSpr->getContentWidth() && touchInSpace.y <= fontSpr->getContentHeight()){
                this->castedImpl()->m_currentlyHeldButton->callButton(false, fontSpr, textForBtn);
                this->castedImpl()->m_currentlyHeldButton = nullptr;
                return;
            }
        }

        this->castedImpl()->m_currentlyHeldButton->callButton(false, nullptr, textForBtn);
    }

    this->castedImpl()->m_currentlyHeldButton = nullptr;
}
void RichTextArea::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    RichTextArea::ccTouchEnded(pTouch, pEvent);
}

RichTextArea::RichTextArea() : SimpleTextArea() {}

RichTextArea::~RichTextArea() = default;


inline std::unique_ptr<SimpleTextAreaImpl> RichTextArea::createImpl() {
    return std::make_unique<RichTextArea::RichImpl>(this);
}

RichTextArea::RichImpl* RichTextArea::castedImpl() {
    return static_cast<RichTextArea::RichImpl*>(m_impl.get());
}

void RichTextArea::onEnter(){
    CCNode::onEnter();
    CCTouchDispatcher::get()->addTargetedDelegate(this, 0, true);
}

void RichTextArea::onExit(){
    CCTouchDispatcher::get()->removeDelegate(this);
    CCNode::onExit();
}
