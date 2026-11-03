#include <Geode/ui/TextArea.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/cocos/robtop/sprite_nodes/CCFontSprite.h>
#include <deque>
#include <memory>
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
    std::deque<std::u32string> m_replacements;

    struct TagInstance {
        RichTag* tag;
        std::string id;
        std::string arg;
        int idx;

        bool verifyArgs() {
            if (arg.empty() && tag->requireArgs) return false;
            if (tag->validate) {
                return tag->validate(arg);
            } else {
                return true;
            }
        }

        inline std::string makeReplacement(std::string const& text = "") const {
            return fmt::format(
                "!!![{}{}{}]",
                id, idx,
                text.empty() ? "" : "|" + text
            );
        }
    };

    void parseRichText();
    void registerTag(std::string const& str, TagInstance const& instance);
    Label* getErrorText();

    void addRichTag(
        std::string id,
        ValidateArgs validate,
        CreateCallback,
        CharIterator iterate,
        TextAddition addText,
        bool updateText,
        bool requireArgs
    );
    void updateRichText();

    Label* createLabel(std::string text, float top) override;
    void setTextImpl(std::string text) override;

    RichTextArea* m_self = nullptr;
    RichImpl(RichTextArea* self) : m_self(self), SimpleTextAreaImpl(self) {}
};

void RichTextArea::RichImpl::parseRichText() {
    m_rawText = m_text;
    m_emojiRegistry = EmojiRegistry::vanillaEmojis().clone();

    std::string newText;
    std::deque<TagInstance> stack;
    std::string tagText;

    auto replaceTag = [&](TagInstance const& instance) {
        auto repl = instance.makeReplacement(tagText);
        newText += repl;
        this->registerTag(repl, instance);
        tagText.clear();
    };

    for (size_t i = 0; i < m_text.size(); i++) {
        char c = m_text[i];
        if (c == '<') {
            auto closing = m_text.find('>', i);
            if (closing == std::string::npos) {
                newText += c;
                continue;
            }

            auto tag = m_text.substr(i + 1, closing - i - 1);
            if (tag.empty()) {
                newText += c;
                continue;
            }

            if (tag[0] == '/') {
                if (stack.empty()) {
                    newText += c;
                    continue;
                }
                auto t = std::ranges::find(stack, tag.substr(1), &TagInstance::id);
                if (t == stack.end()) {
                    newText += c;
                    continue;
                }

                replaceTag(*t);
                i += closing - i;
                stack.pop_front();
                continue;
            }
            if (string::endsWith(tag, "/")) {
                tag = tag.substr(0, tag.size() - 1);
                string::trimRightIP(tag);
            }

            std::string id;
            std::string arg;
            size_t sep = tag.find_first_of("-=");
            if (sep != std::string::npos) {
                arg = tag.substr(sep + 1);
                id = tag.substr(0, sep);
            } else {
                id = tag;
            }

            auto it = std::ranges::find(m_tags, id, &RichTag::id);
            if (it == std::ranges::end(m_tags)) {
                newText += c;
                continue;
            }
            auto instance = TagInstance{
                .tag = &*it,
                .id = std::move(id),
                .arg = std::move(arg),
                .idx = static_cast<int>(i)
            };

            if (!instance.verifyArgs()) {
                newText += c;
                continue;
            }

            if (!stack.empty()) {
                replaceTag(stack.at(0));
            }

            if (instance.tag->addText) {
                replaceTag(instance);
            } else {
                stack.push_front(std::move(instance));
            }

            i += closing - i;
        } else if (c == '\n') {
            if (!stack.empty()) {
                replaceTag(stack.at(0));
            }
            newText += c;
        } else {
            if (!stack.empty()) {
                tagText += c;
            } else {
                newText += c;
            }
        }
    }

    if (!stack.empty()) {
        replaceTag(stack.at(0));
    }

    m_text = newText;
}

void RichTextArea::RichImpl::registerTag(std::string const& str, TagInstance const& instance) {
    m_replacements.emplace_back(std::u32string(str.begin(), str.end()));
    m_emojiRegistry.insert(m_replacements.back(), [this, instance](std::u32string_view str32, size_t& idx) -> CCNode* {
        auto str8 = string::utf32ToUtf8(str32);
        if (str8.isErr()) {
            log::error("Could not parse UTF-32: {}", str8.unwrapErr());
            return this->getErrorText();
        }
        auto str = str8.unwrap();

        auto start = str.rfind("[", idx - 1);
        if (start == std::string::npos) {
            log::error("Could not find opening '['");
            log::info("THIS SHOULD BE UNREACHABLE. REPORT THIS BUG TO THE GEODE DEVELOPERS");
            return this->getErrorText();
        }
        auto repl = str.substr(start + 1, idx - start - 1);

        std::string text;
        size_t sep = repl.find("|");
        if (sep != std::string::npos) {
            text = repl.substr(sep + 1);
        }

        auto tag = instance.tag;
        if (tag->create) {
            return tag->create(std::move(text), instance.arg);
        } else if (tag->iterate) {
            // geode::Label doesn't have an easy method to add per-character
            // effects (ty quads) so we have to resort to this
            auto ret = CCLabelBMFont::create(std::move(text).c_str(), m_font.c_str());
            ret->setColor(to3B(m_color));
            ret->setOpacity(m_color.a);

            auto children = ret->getChildren();
            for (int i = 0; i < children->count(); i++) {
                auto child = static_cast<CCFontSprite*>(children->objectAtIndex(i));
                tag->iterate(child, instance.arg, i);
            }
            return ret;
        } else if (tag->addText) {
            auto ret = Label::create(tag->addText(instance.arg), m_font);
            ret->setColor(to3B(m_color));
            ret->setOpacity(m_color.a);
            return ret;
        } else {
            return this->getErrorText();
        }
    });
}

Label* RichTextArea::RichImpl::getErrorText() {
    auto ret = Label::create("ERROR", m_font.c_str());
    ret->setColor({ 255, 0, 0 });
    return ret;
}

void RichTextArea::RichImpl::addRichTag(
    std::string id,
    ValidateArgs validate,
    CreateCallback create,
    CharIterator iterate,
    TextAddition addText,
    bool updateText,
    bool requireArgs
) {
    m_tags.emplace_back(
        std::move(id),
        requireArgs,
        std::move(validate),
        std::move(create),
        std::move(iterate),
        std::move(addText)
    );
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

RichTextArea* RichTextArea::create(std::string font, std::string text, float scale, float width, bool const artificialWidth) {
    auto ret = new RichTextArea();
    ret->m_impl = ret->createImpl();
    if (ret->init(std::move(font), std::move(text), scale, width, artificialWidth)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool RichTextArea::init(std::string font, std::string text, float scale, float width, bool const artificialWidth) {
    if (!SimpleTextArea::init(font, text, scale, width, artificialWidth)) return false;
    m_richImpl->m_rawText = m_impl->m_text;
    m_richImpl->m_emojiRegistry = EmojiRegistry::vanillaEmojis().clone();

    // Default tags
    this->addRichTag(
        "font",
        [](std::string const& arg) {
            auto font = BitmapFont::load(arg.c_str());
            return font != nullptr;
        },
        [this](std::string text, std::string arg) {
            auto label = Label::create(text, arg);
            label->setColor(to3B(m_impl->m_color));
            label->setOpacity(m_impl->m_color.a);
            return label;
        }, false, true
    );

    this->addRichTag(
        "link",
        [](std::string const& arg) {
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
        }, false, true
    );

    this->addRichTag(
        "date",
        [](std::string const& arg) {
            auto t = utils::numFromString<time_t>(arg);
            return t.isOk();
        },
        [](std::string arg) {
            time_t timestamp;
            if (arg.empty()) {
                timestamp = std::time(nullptr);
            } else {
                // Since of the earlier validate check this should not fail
                // Worst case scenario default to the current time
                timestamp = utils::numFromString<time_t>(arg)
                    .unwrapOr(std::time(nullptr));
            }
            return fmt::format("{:%Y-%m-%d %H:%M:%S}", geode::localtime(timestamp));
        },
        false
    );

    m_richImpl->updateRichText();
    return true;
}

void RichTextArea::addRichTag(std::string id, CreateCallback create, bool updateText, bool requireArgs) {
    m_richImpl->addRichTag(
        std::move(id),
        [](auto) { return true; },
        std::move(create),
        nullptr,
        nullptr,
        updateText,
        requireArgs
    );
}

void RichTextArea::addRichTag(
    std::string id,
    ValidateArgs validate,
    CreateCallback create,
    bool updateText,
    bool requireArgs
) {
    m_richImpl->addRichTag(
        std::move(id),
        std::move(validate),
        std::move(create),
        nullptr,
        nullptr,
        updateText,
        requireArgs
    );
}

void RichTextArea::addRichTag(std::string id, CharIterator iterate, bool updateText, bool requireArgs) {
    m_richImpl->addRichTag(
        std::move(id),
        [](auto) { return true; },
        nullptr,
        std::move(iterate),
        nullptr,
        updateText,
        requireArgs
    );
}

void RichTextArea::addRichTag(
    std::string id,
    ValidateArgs validate,
    CharIterator iterate,
    bool updateText,
    bool requireArgs
) {
    m_richImpl->addRichTag(
        std::move(id),
        std::move(validate),
        nullptr,
        std::move(iterate),
        nullptr,
        updateText,
        requireArgs
    );
}

void RichTextArea::addRichTag(std::string id, TextAddition addText, bool updateText, bool requireArgs) {
    m_richImpl->addRichTag(
        std::move(id),
        [](auto) { return true; },
        nullptr,
        nullptr,
        std::move(addText),
        updateText,
        requireArgs
    );
}

void RichTextArea::addRichTag(
    std::string id,
    ValidateArgs validate,
    TextAddition addText,
    bool updateText,
    bool requireArgs
) {
    m_richImpl->addRichTag(
        std::move(id),
        std::move(validate),
        nullptr,
        nullptr,
        std::move(addText),
        updateText,
        requireArgs
    );
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
