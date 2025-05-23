#include <Geode/ui/TextArea.hpp>

using namespace geode::prelude;

SimpleTextArea* SimpleTextArea::create(const std::string& text, const std::string& font, float scale) {
    return SimpleTextArea::create(font, text, scale, CCDirector::sharedDirector()->getWinSize().width / 2, false);
}

SimpleTextArea* SimpleTextArea::create(const std::string& text, const std::string& font, float scale, float width) {
    return SimpleTextArea::create(font, text, scale, width, true);
}

SimpleTextArea* SimpleTextArea::create(const std::string& font, const std::string& text, float scale, float width, bool artificialWidth) {
    SimpleTextArea* instance = new SimpleTextArea();

    if (instance->init(font, text, scale, width, artificialWidth)) {
        instance->autorelease();
        return instance;
    }

    delete instance;
    return nullptr;
}

bool SimpleTextArea::init(const std::string& font, const std::string& text, float scale, float width, bool artificialWidth) {
    m_font = font;
    m_text = text;
    m_scale = scale;
    m_artificialWidth = artificialWidth;
    m_container = CCMenu::create();

    this->setAnchorPoint({ 0.5f, 0.5f });
    m_container->setPosition({ 0, 0 });
    m_container->setAnchorPoint({ 0, 1 });
    m_container->setContentSize({ width, 0 });
    this->addChild(m_container);
    this->updateContainer();

    return true;
}

void SimpleTextArea::setFont(const std::string& font) {
    m_font = font;
    this->updateContainer();
}

std::string SimpleTextArea::getFont() {
    return m_font;
}

void SimpleTextArea::setColor(const ccColor4B& color) {
    m_color = color;
    this->updateContainer();
}

ccColor4B SimpleTextArea::getColor() {
    return m_color;
}

void SimpleTextArea::setAlignment(CCTextAlignment alignment) {
    m_alignment = alignment;
    this->updateContainer();
}

CCTextAlignment SimpleTextArea::getAlignment() {
    return m_alignment;
}

void SimpleTextArea::setWrappingMode(WrappingMode mode) {
    m_wrappingMode = mode;
    this->updateContainer();
}

WrappingMode SimpleTextArea::getWrappingMode() {
    return m_wrappingMode;
}

void SimpleTextArea::setText(const std::string& text) {
    m_text = text;
    this->updateContainer();
}

std::string SimpleTextArea::getText() {
    return m_text;
}

void SimpleTextArea::setMaxLines(size_t maxLines) {
    m_maxLines = maxLines;
    this->updateContainer();
}

size_t SimpleTextArea::getMaxLines() {
    return m_maxLines;
}

void SimpleTextArea::setWidth(float width) {
    m_artificialWidth = true;
    this->updateContainer();

    this->setContentSize({ width, this->getContentSize().height });
    m_container->setContentSize(this->getContentSize());
}

float SimpleTextArea::getWidth() {
    return m_container->getContentSize().width;
}

void SimpleTextArea::setScale(float scale) {
    m_scale = scale;
    this->updateContainer();
}

float SimpleTextArea::getScale() {
    return m_scale;
}

void SimpleTextArea::setLinePadding(float padding) {
    m_linePadding = padding;
    this->updateContainer();
}

float SimpleTextArea::getLinePadding() {
    return m_linePadding;
}

std::vector<CCLabelBMFont*> SimpleTextArea::getLines() {
    return m_lines;
}

float SimpleTextArea::getHeight() {
    return m_container->getContentSize().height;
}

float SimpleTextArea::getLineHeight() {
    return m_lineHeight;
}

CCLabelBMFont* SimpleTextArea::createLabel(const std::string& text, float top) {
    if (m_maxLines && m_lines.size() >= m_maxLines) {
        CCLabelBMFont* last = m_lines.at(m_maxLines - 1);
        const std::string& text = last->getString();

        last->setString(text.substr(0, text.size() - 3).append("...").c_str());

        return nullptr;
    } else {
        CCLabelBMFont* label = CCLabelBMFont::create(text.c_str(), m_font.c_str());

        label->setScale(m_scale);
        label->setPosition({ 0, top });
        label->setColor({ m_color.r, m_color.g, m_color.b });
        label->setOpacity(m_color.a);

        return label;
    }
}

float SimpleTextArea::calculateOffset(CCLabelBMFont* label) {
    return m_linePadding + label->getContentSize().height * m_scale;
}

void SimpleTextArea::charIteration(const std::function<CCLabelBMFont*(CCLabelBMFont* line, char c, float top)>& overflowHandling) {
    float top = 0;
    m_lines.clear();
    CCLabelBMFont* line = this->createLabel("", top);
    m_lines = { line };

    for (const char c : m_text) {
        if (c == '\n') {
            line = this->createLabel("", top -= this->calculateOffset(line));

            if (line == nullptr) {
                break;
            } else {
                m_lines.push_back(line);
            }
        } else if (m_artificialWidth && line->getContentWidth() * m_scale >= this->getWidth()) {
            line = overflowHandling(line, c, top -= this->calculateOffset(line));

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

void SimpleTextArea::updateLinesNoWrap() {
    std::stringstream stream(m_text);
    std::string part;
    float top = 0;
    m_lines.clear();

    while (std::getline(stream, part)) {
        CCLabelBMFont* line = this->createLabel(part, top);

        if (line == nullptr) {
            break;
        } else {
            top -= this->calculateOffset(line);

            m_lines.push_back(line);
        }
    }
}

void SimpleTextArea::updateLinesWordWrap(bool spaceWrap) {
    this->charIteration([this, spaceWrap](CCLabelBMFont* line, char c, float top) {
        const std::string_view delimiters(spaceWrap ? " " : " `~!@#$%^&*()-_=+[{}];:'\",<.>/?\\|");

        if (delimiters.find(c) == std::string_view::npos) {
            const std::string& text = line->getString();
            const size_t position = text.find_last_of(delimiters) + 1;
            CCLabelBMFont* newLine = this->createLabel(text.substr(position) + c, top);

            if (newLine != nullptr) {
                line->setString(text.substr(0, position).c_str());
            }

            return newLine;
        } else {
            return this->createLabel(std::string(c, c != ' '), top);
        }
    });
}

void SimpleTextArea::updateLinesCutoffWrap() {
    this->charIteration([this](CCLabelBMFont* line, char c, float top) {
        const std::string& text = line->getString();
        const char back = text.back();
        const bool lastIsSpace = back == ' ';
        CCLabelBMFont* newLine = this->createLabel(std::string(!lastIsSpace, back).append(std::string(c != ' ', c)), top);

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

void SimpleTextArea::updateContainer() {
    switch (m_wrappingMode) {
        case NO_WRAP: {
            this->updateLinesNoWrap();
        } break;
        case WORD_WRAP: {
            this->updateLinesWordWrap(false);
        } break;
        case SPACE_WRAP: {
            this->updateLinesWordWrap(true);
        } break;
        case CUTOFF_WRAP: {
            this->updateLinesCutoffWrap();
        } break;
    }

    const size_t lineCount = m_lines.size();
    const float width = this->getWidth();

    if (lineCount > 0) {
        m_lineHeight = m_lines.back()->getContentSize().height * m_scale;
    } else {
        m_lineHeight = 0;
    }

    const float height = m_lineHeight * lineCount + m_linePadding * (lineCount - 1);

    this->setContentSize({ width, height });
    m_container->setContentSize(this->getContentSize());
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
