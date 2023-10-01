#include <Geode/ui/TextArea.hpp>

using namespace geode::prelude;

SimpleTextArea* SimpleTextArea::create(const std::string& font, const std::string& text, const float scale = 1) {
    return SimpleTextArea::create(font, text, scale, 500, false);
}

SimpleTextArea* SimpleTextArea::create(const std::string& font, const std::string& text, const float scale, const float width) {
    return SimpleTextArea::create(font, text, scale, width, true);
}

SimpleTextArea* SimpleTextArea::create(const std::string& font, const std::string& text, const float scale, const float width, const bool artificialWidth) {
    SimpleTextArea* instance = new SimpleTextArea(font, text, scale, width, artificialWidth);

    if (instance && instance->init()) {
        instance->autorelease();

        return instance;
    } else {
        CC_SAFE_DELETE(instance);
        
        return nullptr;
    }
}

SimpleTextArea::SimpleTextArea(const std::string& font, const std::string& text, const float scale, const float width, const bool artificialWidth) {
    m_font = font;
    m_text = text;
    m_maxLines = 0;
    m_scale = scale;
    m_linePadding = 0;
    m_artificialWidth = artificialWidth;
    m_container = CCMenu::create();

    this->setAnchorPoint({ 0.5f, 0.5f });
    m_container->setPosition({ 0, 0 });
    m_container->setAnchorPoint({ 0, 1 });
    m_container->setContentSize({ width, 0 });

    this->addChild(m_container);
    this->updateContents();
}

void SimpleTextArea::setFont(const std::string& font) {
    m_font = font;

    this->updateContents();
}

std::string SimpleTextArea::getFont() {
    return m_font;
}

void SimpleTextArea::setAlignment(const CCTextAlignment alignment) {
    m_alignment = alignment;

    this->updateContents();
}

CCTextAlignment SimpleTextArea::getAlignment() {
    return m_alignment;
}

void SimpleTextArea::setText(const std::string& text) {
    m_text = text;

    this->updateContents();
}

std::string SimpleTextArea::getText() {
    return m_text;
}

void SimpleTextArea::setMaxLines(const size_t maxLines) {
    m_maxLines = maxLines;

    this->updateContents();
}

size_t SimpleTextArea::getMaxLines() {
    return m_maxLines;
}

void SimpleTextArea::setWidth(const float width) {
    m_artificialWidth = true;

    this->setContentSize({ width, this->getContentSize().height });
    m_container->setContentSize(this->getContentSize());
}

float SimpleTextArea::getWidth() {
    return m_container->getContentSize().width;
}

void SimpleTextArea::setScale(const float scale) {
    m_scale = scale;

    this->updateContents();
}

float SimpleTextArea::getScale() {
    return m_scale;
}

void SimpleTextArea::setLinePadding(const float padding) {
    m_linePadding = padding;

    this->updateContents();
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

CCLabelBMFont* SimpleTextArea::createLabel(const std::string& text, const float top) {
    CCLabelBMFont* label = CCLabelBMFont::create(text.c_str(), m_font.c_str());

    label->setScale(m_scale);
    label->setAnchorPoint({ 0, 0 });
    label->setPosition({ 0, top });

    return label;
}

CCLabelBMFont* SimpleTextArea::moveOverflow(CCLabelBMFont* line, const char c, const float top) {
    const std::string text = line->getString();
    const char back = text.back();
    const bool lastIsSpace = back == ' ';
    CCLabelBMFont* newLine = this->createLabel(std::string(!lastIsSpace, back).append(std::string(c != ' ', c)), top);

    if (!lastIsSpace) {
        if (text[text.size() - 2] == ' ') {
            line->setString(text.substr(0, text.size() - 1).c_str());
        } else {
            line->setString((text.substr(0, text.size() - 1) + '-').c_str());
        }
    }

    m_lines.push_back(newLine);

    return newLine;
}

float SimpleTextArea::calculateOffset(CCLabelBMFont* label) {
    return m_linePadding + label->getContentSize().height * m_scale;
}

void SimpleTextArea::updateLines() {
    float top = 0;
    CCLabelBMFont* line = this->createLabel("", top);
    m_lines = { line };

    for (const char c : m_text) {
        if (m_maxLines && m_lines.size() > m_maxLines) {
            CCLabelBMFont* last = m_lines.at(m_maxLines - 1);
            const std::string text = last->getString();

            m_lines.pop_back();
            last->setString(text.substr(0, text.size() - 3).append("...").c_str());

            break;
        } else if (c == '\n') {
            line = this->createLabel("", top -= this->calculateOffset(line));

            m_lines.push_back(line);
        } else if (m_artificialWidth && line->getContentSize().width >= this->getWidth()) {
            line = this->moveOverflow(line, c, top -= this->calculateOffset(line));
        } else {
            const std::string text = line->getString();

            line->setString((text + c).c_str());
        }
    }
}

void SimpleTextArea::updateContents() {
    this->updateLines();
    const size_t lineCount = m_lines.size();
    const float width = this->getWidth();

    if (lineCount > 0) {
        m_lineHeight = m_lines.back()->getContentSize().height * m_scale;
    } else {
        m_lineHeight = 0;
    }

    float height = m_lineHeight * lineCount + m_linePadding * (lineCount - 1);

    this->setContentSize({ width, height });
    m_container->setContentSize(this->getContentSize());
    m_container->removeAllChildren();

    height -= m_lineHeight;

    for (CCLabelBMFont* line : m_lines) {
        const float y = height + line->getPositionY();

        switch (m_alignment) {
            case kCCTextAlignmentLeft: {
                line->setAnchorPoint({ 0, 0 });
                line->setPosition({ 0, y });
            } break;
            case kCCTextAlignmentCenter: {
                line->setAnchorPoint({ 0.5f, 0 });
                line->setPosition({ width / 2, y });
            } break;
            case kCCTextAlignmentRight: {
                line->setAnchorPoint({ 1, 0 });
                line->setPosition({ width, y });
            } break;
        }

        m_container->addChild(line);
    }
}