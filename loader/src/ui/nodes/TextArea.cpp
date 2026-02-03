#include <Geode/ui/TextArea.hpp>

using namespace geode::prelude;

CCLabelBMFont* SimpleTextArea::Impl::createLabel(char const* text, float top) {
    if (m_maxLines && m_lines.size() >= m_maxLines) {
        CCLabelBMFont* last = m_lines.at(m_maxLines - 1);
        std::string_view textv = last->getString();

        last->setString(fmt::format("{}...", textv.substr(0, textv.size() - 3)).c_str());

        return nullptr;
    } else {
        CCLabelBMFont* label = CCLabelBMFont::create(text, m_font.c_str());

        label->setScale(m_scale);
        label->setPosition({ 0, top });
        label->setColor({ m_color.r, m_color.g, m_color.b });
        label->setOpacity(m_color.a);

        return label;
    }
}

float SimpleTextArea::Impl::calculateOffset(CCLabelBMFont* label) {
    return m_linePadding + label->getContentSize().height * m_scale;
}

void SimpleTextArea::Impl::charIteration(geode::FunctionRef<CCLabelBMFont*(CCLabelBMFont* line, char c, float top)> overflowHandling) {
    float top = 0;
    m_lines.clear();
    CCLabelBMFont* line = createLabel("", top);
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
            line->setString((std::string(line->getString()) + c).c_str());
        }
    }
}

void SimpleTextArea::Impl::updateLinesNoWrap() {
    std::stringstream stream(m_text);
    std::string part;
    float top = 0;
    m_lines.clear();

    while (std::getline(stream, part)) {
        CCLabelBMFont* line = createLabel(part.c_str(), top);

        if (line == nullptr) {
            break;
        } else {
            top -= calculateOffset(line);

            m_lines.push_back(line);
        }
    }
}

void SimpleTextArea::Impl::updateLinesWordWrap(bool spaceWrap) {
    charIteration([this, spaceWrap](CCLabelBMFont* line, char c, float top) {
        const std::string_view delimiters(spaceWrap ? " " : " `~!@#$%^&*()-_=+[{}];:'\",<.>/?\\|");

        if (delimiters.find(c) == std::string_view::npos) {
            const std::string& text = line->getString();
            const size_t position = text.find_last_of(delimiters) + 1;
            CCLabelBMFont* newLine = createLabel((text.substr(position) + c).c_str(), top);

            if (newLine != nullptr) {
                line->setString(text.substr(0, position).c_str());
            }

            return newLine;
        } else {
            return createLabel(std::string(c, c != ' ').c_str(), top);
        }
    });
}

void SimpleTextArea::Impl::updateLinesCutoffWrap() {
    charIteration([this](CCLabelBMFont* line, char c, float top) {
        const std::string& text = line->getString();
        const char back = text.back();
        const bool lastIsSpace = back == ' ';
        CCLabelBMFont* newLine = createLabel(std::string(!lastIsSpace, back).append(std::string(c != ' ', c)).c_str(), top);

        if (newLine == nullptr && !lastIsSpace) {
            if (text[text.size() - 2] == ' ') {
                line->setString(text.substr(0, text.size() - 1).c_str());
            } else {
                line->setString((text.substr(0, text.size() - 1) + '-').c_str());
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

    for (CCLabelBMFont* line : m_lines) {
        const float y = height + line->getPositionY();

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

SimpleTextArea::SimpleTextArea() : m_impl(std::make_unique<SimpleTextArea::Impl>(this)) {}
SimpleTextArea::~SimpleTextArea() = default;

SimpleTextArea* SimpleTextArea::create(std::string text, std::string font, float scale) {
    return SimpleTextArea::create(
        std::move(font), std::move(text), scale,
        CCDirector::sharedDirector()->getWinSize().width / 2,
        false
    );
}

SimpleTextArea* SimpleTextArea::create(std::string text, std::string font, float scale, float width) {
    return SimpleTextArea::create(
        std::move(font), std::move(text), scale, width, true
    );
}

SimpleTextArea* SimpleTextArea::create(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    SimpleTextArea* instance = new SimpleTextArea();
    instance->m_impl = std::make_unique<SimpleTextArea::Impl>(instance);

    if (instance->init(std::move(font), std::move(text), scale, width, artificialWidth)) {
        instance->autorelease();
        return instance;
    }

    delete instance;
    return nullptr;
}

bool SimpleTextArea::init(std::string font, std::string text, float scale, float width, bool artificialWidth) {
    m_impl->m_font = std::move(font);
    m_impl->m_text = std::move(text);
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

void SimpleTextArea::setFont(std::string font) {
    m_impl->m_font = std::move(font);
    m_impl->updateContainer();
}

std::string SimpleTextArea::getFont() {
    return m_impl->m_font;
}

void SimpleTextArea::setColor(const ccColor4B& color) {
    m_impl->m_color = color;
    m_impl->updateContainer();
}

ccColor4B SimpleTextArea::getColor() {
    return m_impl->m_color;
}

void SimpleTextArea::setAlignment(CCTextAlignment alignment) {
    m_impl->m_alignment = alignment;
    m_impl->updateContainer();
}

CCTextAlignment SimpleTextArea::getAlignment() {
    return m_impl->m_alignment;
}

void SimpleTextArea::setWrappingMode(WrappingMode mode) {
    m_impl->m_wrappingMode = mode;
    m_impl->updateContainer();
}

WrappingMode SimpleTextArea::getWrappingMode() {
    return m_impl->m_wrappingMode;
}

void SimpleTextArea::setText(std::string text) {
    m_impl->m_text = std::move(text);
    m_impl->updateContainer();
}

std::string SimpleTextArea::getText() {
    return m_impl->m_text;
}

void SimpleTextArea::setMaxLines(size_t maxLines) {
    m_impl->m_maxLines = maxLines;
    m_impl->updateContainer();
}

size_t SimpleTextArea::getMaxLines() {
    return m_impl->m_maxLines;
}

void SimpleTextArea::setWidth(float width) {
    m_impl->m_artificialWidth = true;
    m_impl->updateContainer();

    this->setContentSize({ width, this->getContentSize().height });
    m_impl->m_container->setContentSize(this->getContentSize());
}

float SimpleTextArea::getWidth() {
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

float SimpleTextArea::getLinePadding() {
    return m_impl->m_linePadding;
}

std::vector<CCLabelBMFont*> SimpleTextArea::getLines() {
    return m_impl->m_lines;
}

float SimpleTextArea::getHeight() {
    return m_impl->m_container->getContentSize().height;
}

float SimpleTextArea::getLineHeight() {
    return m_impl->m_lineHeight;
}