#include <Geode/ui/TextArea.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/terminate.hpp>
#include <Geode/cocos/robtop/sprite_nodes/CCFontSprite.h>
#include <deque>
#include <memory>
#include <optional>
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

std::string geode::SimpleTextArea::getText() const {
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

class RichTextArea::RichImpl : public SimpleTextAreaImpl {
public:
    std::vector<RichTag> m_tags;
    EmojiRegistry m_emojiRegistry;
    std::string m_rawText;
    std::deque<std::u32string> m_tagIRs;

    size_t m_idx = 0;

    void parseRichText();
    void registerTag(const std::string& ir);
    CCLabelBMFont* getErrorText();

    char current();
    std::optional<char> peek();
    void advance(int offset = 1);

    void addRichTag(std::string id, bool requireArgs, ValidateArgs validate, CreateCallback, CharIterator iterator, bool updateText);
    void updateRichText();

    Label* createLabel(std::string text, float top) override;
    void setTextImpl(std::string text) override;

    RichTextArea* m_self = nullptr;
    RichImpl(RichTextArea* self) : m_self(self), SimpleTextAreaImpl(self) {}
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

void RichTextArea::RichImpl::parseRichText() {
    m_idx = 0;
    m_rawText = m_text;
    std::string newText;
    ParseState state = ParseState::Normal;

    RichTag* tag;
    std::string id;
    std::string args;
    std::string text;
    size_t tagPos;

    auto handleError = [&] {
        newText += m_text.substr(tagPos, m_idx - tagPos + 1);
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
                    tagPos = m_idx;
                    state = ParseState::ParseKey;
                } else {
                    newText += c;
                }
                break;
            case ParseState::ParseKey:
                id += c;
                if (auto p = this->peek(); p && (*p == '>' || *p == '=' || *p == '-')) {
                    auto match = std::ranges::find(m_tags, id, &RichTag::id);
                    if (match == m_tags.end()) {
                        handleError();
                        break;
                    }
                    tag = &*match;
                    if (*p == '>') {
                        if (tag->requireArgs) {
                            handleError();
                            break;
                        }
                        state = ParseState::ParseText;
                    } else {
                        state = ParseState::ParseArgs;
                    }
                    this->advance();
                }
                break;
            case ParseState::ParseArgs:
                if (c == '>') {
                    if (args.empty() && tag->requireArgs) {
                        handleError();
                        break;
                    }
                    if (!tag->validate(args)) {
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
                    }
                    // TODO: Handle nested tags
                    break;
                }
                if (c == '\n') {
                    auto ir = makeIR();
                    newText += ir + "\n";
                    this->registerTag(ir);
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
                this->registerTag(ir);
                state = ParseState::Normal;
                id.clear();
                args.clear();
                text.clear();
                break;
            }
        }
        this->advance();
    }

    // If the parser ever stopped abruptly
    if (state != ParseState::Normal) {
        newText += m_text.substr(tagPos);
    }
    m_text = newText;
}

void RichTextArea::RichImpl::registerTag(const std::string& ir) {
    m_tagIRs.emplace_back(std::u32string(ir.begin(), ir.end()));
    m_emojiRegistry.insert(m_tagIRs.back(), [this](std::u32string_view str32, size_t& idx) -> CCNode* {
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
            log::error("Missing ending ':' on intermediate (RichTextArea)");
            return this->getErrorText();
        }
        auto inter = str.substr(start + 1, idx - start - 1);

        // Split text and id
        auto split = string::split(inter, "|");

        std::string idStr;
        std::string arg;
        if (string::contains(split[0], "-")) {
            auto k = string::split(split[0], "-");
            idStr = k[0];
            arg = k[1];
        } else {
            idStr = split[0];
        }

        auto tag = std::ranges::find(m_tags, idStr, &RichTag::id);
        if (tag == m_tags.end()) {
            log::error("Could not find tag '{}'", idStr);
            return this->getErrorText();
        }

        if (tag->create) {
            return tag->create(std::move(split[1]), std::move(arg));
        } else {
            // geode::Label doesn't have an easy method to add per-character
            // effects (ty quads) so we have to resort to this
            auto ret = CCLabelBMFont::create(split[1].c_str(), m_font.c_str());
            ret->setColor(to3B(m_color));
            ret->setOpacity(m_color.a);

            if (tag->iterator) {
                for (auto& c : CCArrayExt(ret->getChildren())) {
                    tag->iterator(static_cast<CCFontSprite*>(c), std::move(arg));
                }
            }

            return ret;
        }
    });
}

CCLabelBMFont* RichTextArea::RichImpl::getErrorText() {
    auto ret = CCLabelBMFont::create("ERROR", "bigFont.fnt");
    ret->setColor({ 255, 0, 0 });
    return ret;
}

char RichTextArea::RichImpl::current() {
    return m_text[m_idx];
}

std::optional<char> RichTextArea::RichImpl::peek() {
    auto i = m_idx + 1;
    if (i >= m_text.size()) {
        return std::nullopt;
    }
    return m_text[i];
}

void RichTextArea::RichImpl::advance(int offset) {
    m_idx += offset;
}

void RichTextArea::RichImpl::addRichTag(
    std::string id,
    bool requireArgs,
    ValidateArgs validate,
    CreateCallback create,
    CharIterator iterator,
    bool updateText
) {
    m_tags.emplace_back(std::move(id), requireArgs, std::move(validate), std::move(create), std::move(iterator));
    if (updateText) {
        m_text = m_rawText;
        this->updateRichText();
    }
}

void RichTextArea::RichImpl::updateRichText() {
    this->parseRichText();
    this->updateContainer();
}

Label* RichTextArea::RichImpl::createLabel(std::string text, float top) {
    auto label = SimpleTextAreaImpl::createLabel(text, top);
    if (!label) return nullptr;
    label->setEmojiRegistry(m_emojiRegistry);
    return label;
}

void RichTextArea::RichImpl::setTextImpl(std::string text) {
    m_text = std::move(text);
    this->updateRichText();
}

RichTextArea* RichTextArea::create(std::string text, std::string font, float scale) {
    return create(
        std::move(font), std::move(text), scale,
        CCDirector::sharedDirector()->getWinSize().width / 2, false
    );
}

RichTextArea* RichTextArea::create(std::string text, std::string font, float scale, float width) {
    return create(std::move(font), std::move(text), scale, width, true);
}

RichTextArea* RichTextArea::create(std::string font, std::string text, float scale, float width, const bool artificialWidth) {
    auto ret = new RichTextArea();
    ret->m_impl = ret->createImpl();
    if (ret->init(std::move(font), std::move(text), scale, width, artificialWidth)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool RichTextArea::init(std::string font, std::string text, float scale, float width, const bool artificialWidth) {
    if (!SimpleTextArea::init(font, text, scale, width, artificialWidth)) return false;
    m_richImpl->m_rawText = m_impl->m_text;

    // Default tags
    this->addRichTag(
        "flip",
        [](CCFontSprite* spr, std::string) {
            spr->setFlipY(true);
        },
        false
    );

    this->addRichTag(
        "size",
        true,
        [](const std::string& arg) {
            auto num = utils::numFromString<float>(arg);
            return num.isOk();
        },
        [](CCFontSprite* ch, std::string arg) {
            auto num = utils::numFromString<float>(arg);
            if (num.isErr()) {
                // This should not happen but just in case
                log::error("Could not convert to float: {}", num.unwrapErr());
                return;
            }
            ch->setScale(num.unwrap());
        }, false
    );

    // !!!
    this->addRichTag(
        "font",
        true,
        [](const std::string& arg) {
            auto font = BitmapFont::load(std::string(arg).c_str());
            return font == nullptr;
        },
        [this](std::string text, std::string arg) {
            auto label = Label::create(text, arg);
            label->setColor(to3B(m_impl->m_color));
            label->setOpacity(m_impl->m_color.a);
            return label;
        }, false
    );

    this->addRichTag(
        "link",
        true,
        [](const std::string& arg) {
            if (arg.contains("#:")) {
                return false;
            }
            auto lower = string::toLower(std::string(arg));
            return lower.starts_with("https://") || lower.starts_with("http://") || lower.starts_with("mailto://");
        },
        [this](std::string text, std::string arg) {
            auto link = Button::createWithLabel(std::string(text), m_impl->m_font, [arg](auto) {
                web::openLinkInBrowser(std::string(arg));
            });
            link->setColor({ 150, 255, 255 });
            link->setOpacity(m_impl->m_color.a);
            return link;
        }, false
    );

    m_richImpl->updateRichText();
    return true;
}

void RichTextArea::addRichTag(std::string id, CreateCallback create, bool updateText) {
    addRichTag(std::move(id), false, std::move(create), updateText);
}

void RichTextArea::addRichTag(std::string id, bool requireArgs, CreateCallback create, bool updateText) {
    m_richImpl->addRichTag(std::move(id), false, [](auto) { return true; }, std::move(create), nullptr, updateText);
}

void RichTextArea::addRichTag(std::string id, ValidateArgs validate, CreateCallback create, bool updateText) {
    addRichTag(std::move(id), false, std::move(validate), std::move(create), updateText);
}

void RichTextArea::addRichTag(std::string id, bool requireArgs, ValidateArgs validate, CreateCallback create, bool updateText) {
    m_richImpl->addRichTag(std::move(id), requireArgs, std::move(validate), std::move(create), nullptr, updateText);
}

void RichTextArea::addRichTag(std::string id, CharIterator iterator, bool updateText) {
    addRichTag(std::move(id), false, std::move(iterator), updateText);
}

void RichTextArea::addRichTag(std::string id, bool requireArgs, CharIterator iterator, bool updateText) {
    m_richImpl->addRichTag(std::move(id), requireArgs, [](auto) { return true; }, nullptr, std::move(iterator), updateText);
}

void RichTextArea::addRichTag(std::string id, ValidateArgs validate, CharIterator iterator, bool updateText) {
    addRichTag(std::move(id), false, std::move(validate), std::move(iterator), updateText);
}

void RichTextArea::addRichTag(std::string id, bool requireArgs, ValidateArgs validate, CharIterator iterator, bool updateText) {
    m_richImpl->addRichTag(std::move(id), requireArgs, std::move(validate), nullptr, std::move(iterator), updateText);
}

std::string RichTextArea::getText() const {
    return m_richImpl->m_rawText;
};

void RichTextArea::updateRichText() {
    m_richImpl->m_text = m_richImpl->m_rawText;
    m_richImpl->updateRichText();
}

std::unique_ptr<SimpleTextAreaImpl> RichTextArea::createImpl() {
    auto ret = std::make_unique<RichImpl>(this);
    m_richImpl = ret.get();
    return ret;
}
