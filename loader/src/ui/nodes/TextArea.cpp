#include <Geode/ui/TextArea.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

struct SimpleTextArea::Impl {
    SimpleTextArea* m_self = nullptr;
    Impl(SimpleTextArea* self) : m_self(self) {}

    bool m_shouldUpdate = false;
    bool m_artificialWidth = false;
    CCMenu* m_container = nullptr;
    std::string m_text;
    BitmapFont* m_font;
    std::vector<Label*> m_lines;
    ccColor4B m_color = { 0xFF, 0xFF, 0xFF, 0xFF };
    CCTextAlignment m_alignment = kCCTextAlignmentLeft;
    WrappingMode m_wrappingMode = WORD_WRAP;
    size_t m_maxLines = 0;
    float m_scale = 1.f;
    float m_lineHeight = 0.f;
    float m_linePadding = 0.f;

    Label* createLabel(std::string text, float top);
    float calculateOffset(Label* label);
    void charIteration(FunctionRef<Label*(Label* line, char c, float top)> overflowHandling);
    void updateLinesNoWrap();
    void updateLinesWordWrap(bool spaceWrap);
    void updateLinesCutoffWrap();
    void updateContainer();
};

Label* SimpleTextArea::Impl::createLabel(std::string text, float top) {
    if (m_maxLines && m_lines.size() >= m_maxLines) {
        Label* last = m_lines.at(m_maxLines - 1);
        std::string_view textv = last->getString();

        last->setText(fmt::format("{}...", textv.substr(0, textv.size() - 3)));

        return nullptr;
    } else {
        Label* label = Label::createRich(std::move(text), m_font);

        label->setScale(m_scale);
        label->setPosition({ 0, top });
        label->setColor(to3B(m_color));
        label->setOpacity(m_color.a);

        return label;
    }
}

float SimpleTextArea::Impl::calculateOffset(Label* label) {
    return m_linePadding + label->getContentSize().height * m_scale;
}

void SimpleTextArea::Impl::charIteration(FunctionRef<Label*(Label* line, char c, float top)> overflowHandling) {
    float top = 0;
    m_lines.clear();
    Label* line = createLabel("", top);
    m_lines = { line };

    for (char const c : m_text) {
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

void SimpleTextArea::Impl::updateLinesNoWrap() {
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

void SimpleTextArea::Impl::updateLinesWordWrap(bool spaceWrap) {
    charIteration([this, spaceWrap](Label* line, char c, float top) {
        std::string_view const delimiters(spaceWrap ? " " : " `~!@#$%^&*()-_=+[{}];:'\",<.>/?\\|");

        if (!delimiters.contains(c)) {
            const std::string& text = line->getString();
            const size_t position = text.find_last_of(delimiters) + 1;
            Label* newLine = createLabel(text.substr(position) + c, top);

            if (newLine != nullptr) {
                line->setText(text.substr(0, position));
            }

            return newLine;
        } else {
            return createLabel(std::string(c != ' ', c), top);
        }
    });
}

void SimpleTextArea::Impl::updateLinesCutoffWrap() {
    charIteration([this](Label* line, char c, float top) {
        std::string const& text = line->getString();
        char const back = text.back();
        bool const lastIsSpace = back == ' ';
        Label* newLine = createLabel(std::string(!lastIsSpace, back).append(std::string(c != ' ', c)), top);

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

void SimpleTextArea::Impl::updateContainer() {
    switch (m_wrappingMode) {
        case NO_WRAP: {
            updateLinesNoWrap();
        } break;
        case WORD_WRAP: {
            updateLinesWordWrap(false);
        } break;
        case SPACE_WRAP: {
            updateLinesWordWrap(true);
        } break;
        case CUTOFF_WRAP: {
            updateLinesCutoffWrap();
        } break;
    }

    size_t const lineCount = m_lines.size();
    float const width = m_self->getWidth();

    if (lineCount > 0) {
        m_lineHeight = m_lines.back()->getContentSize().height * m_scale;
    } else {
        m_lineHeight = 0;
    }

    float const height = m_lineHeight * lineCount + m_linePadding * (lineCount - 1);

    m_self->setContentSize({ width, height });
    m_container->setContentSize(m_self->getContentSize());
    m_container->removeAllChildren();

    for (Label* line : m_lines) {
        float const y = height + line->getPositionY();

        switch (m_alignment) {
            case kCCTextAlignmentLeft: {
                line->setAnchorPoint({ 0, 1 });
                line->setPosition({ 0, y });
            } break;
            case kCCTextAlignmentCenter: {
                line->setAnchorPoint({ 0.5f, 1 });
                line->setPosition({ width / 2, y });
            } break;
            case kCCTextAlignmentRight: {
                line->setAnchorPoint({ 1, 1 });
                line->setPosition({ width, y });
            } break;
        }

        m_container->addChild(line);
    }
}

SimpleTextArea::SimpleTextArea() : m_impl(std::make_unique<Impl>(this)) {}
SimpleTextArea::~SimpleTextArea() = default;

SimpleTextArea* SimpleTextArea::create(std::string text, ZStringView font, float scale) {
    auto f = BitmapFont::load(font);
    if (!f) return nullptr;
    return SimpleTextArea::create(std::move(text), f, scale);
}

SimpleTextArea* SimpleTextArea::create(std::string text, ZStringView font, float scale, float width) {
    auto f = BitmapFont::load(font);
    if (!f) return nullptr;
    return SimpleTextArea::create(std::move(text), f, scale, width);
}

SimpleTextArea* SimpleTextArea::create(std::string text, BitmapFont* font, float scale) {
    auto ret = new SimpleTextArea();
    if (ret->init(
        std::move(text), font, scale,
        CCDirector::get()->getWinSize().width / 2,
        false
    )) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

SimpleTextArea* SimpleTextArea::create(std::string text, BitmapFont* font, float scale, float width) {
    auto ret = new SimpleTextArea();
    if (ret->init(std::move(text), font, scale, width, true)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool SimpleTextArea::init(std::string text, BitmapFont* font, float scale, float width, bool artificialWidth) {
    m_impl->m_text = std::move(text);
    m_impl->m_font = font;
    m_impl->m_scale = scale;
    m_impl->m_artificialWidth = artificialWidth;
    m_impl->m_container = CCMenu::create();

    this->setAnchorPoint({ 0.5f, 0.5f });
    m_impl->m_container->setPosition({ 0, 0 });
    m_impl->m_container->setAnchorPoint({ 0, 1 });
    m_impl->m_container->setContentSize({ width, 0 });
    this->addChild(m_impl->m_container);
    m_impl->updateContainer();

    return true;
}

void SimpleTextArea::setFont(ZStringView font) {
    auto f = BitmapFont::load(font);
    if (!f) return;
    this->setFont(f);
}

void SimpleTextArea::setFont(geode::BitmapFont* font) {
    if (!font) return;
    m_impl->m_font = font;
    m_impl->updateContainer();
}

BitmapFont* SimpleTextArea::getFont() const {
    return m_impl->m_font;
}

void SimpleTextArea::setColor(const ccColor4B& color) {
    m_impl->m_color = color;
    m_impl->updateContainer();
}

void SimpleTextArea::setColor(const ccColor3B& color) {
    this->setColor(to4B(color, m_impl->m_color.a));
}

ccColor4B SimpleTextArea::getColor() const {
    return m_impl->m_color;
}

void SimpleTextArea::setAlignment(CCTextAlignment alignment) {
    m_impl->m_alignment = alignment;
    m_impl->updateContainer();
}

CCTextAlignment SimpleTextArea::getAlignment() const {
    return m_impl->m_alignment;
}

void SimpleTextArea::setWrappingMode(WrappingMode mode) {
    m_impl->m_wrappingMode = mode;
    m_impl->updateContainer();
}

WrappingMode SimpleTextArea::getWrappingMode() const {
    return m_impl->m_wrappingMode;
}

void SimpleTextArea::setText(std::string text) {
    m_impl->m_text = std::move(text);
    m_impl->updateContainer();
}

std::string SimpleTextArea::getText() const {
    return m_impl->m_text;
}

void SimpleTextArea::setMaxLines(size_t maxLines) {
    m_impl->m_maxLines = maxLines;
    m_impl->updateContainer();
}

size_t SimpleTextArea::getMaxLines() const {
    return m_impl->m_maxLines;
}

void SimpleTextArea::setWidth(float width) {
    m_impl->m_artificialWidth = true;
    m_impl->updateContainer();

    this->setContentSize({ width, this->getContentSize().height });
    m_impl->m_container->setContentSize(this->getContentSize());
}

float SimpleTextArea::getWidth() const {
    return m_impl->m_container->getContentSize().width;
}

void SimpleTextArea::setScale(float scale) {
    m_impl->m_scale = scale;
    m_impl->updateContainer();
}

float SimpleTextArea::getScale() {
    return m_impl->m_scale;
}

void SimpleTextArea::setLinePadding(float padding) {
    m_impl->m_linePadding = padding;
    m_impl->updateContainer();
}

float SimpleTextArea::getLinePadding() const {
    return m_impl->m_linePadding;
}

std::vector<Label*> SimpleTextArea::getLines() const {
    return m_impl->m_lines;
}

float SimpleTextArea::getHeight() const {
    return m_impl->m_container->getContentSize().height;
}

float SimpleTextArea::getLineHeight() const {
    return m_impl->m_lineHeight;
}
