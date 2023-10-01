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
    this->m_font = font;
    this->m_text = text;
    this->m_maxLines = 0;
    this->m_scale = scale;
    this->m_linePadding = 0;
    this->m_artificialWidth = artificialWidth;
    this->m_container = CCMenu::create();

    this->setAnchorPoint({ 0.5f, 0.5f });
    this->m_container->setPosition({ 0, 0 });
    this->m_container->setAnchorPoint({ 0, 1 });
    this->m_container->setContentSize({ width, 0 });

    this->addChild(this->m_container);
    this->updateContents();
}

void SimpleTextArea::setFont(const std::string& font) {
    this->m_font = font;

    this->updateContents();
}

std::string SimpleTextArea::getFont() {
    return this->m_font;
}

void SimpleTextArea::setAlignment(const CCTextAlignment alignment) {
    this->m_alignment = alignment;

    this->updateContents();
}

CCTextAlignment SimpleTextArea::getAlignment() {
    return this->m_alignment;
}

void SimpleTextArea::setText(const std::string& text) {
    this->m_text = text;

    this->updateContents();
}

std::string SimpleTextArea::getText() {
    return this->m_text;
}

void SimpleTextArea::setMaxLines(const size_t maxLines) {
    this->m_maxLines = maxLines;

    this->updateContents();
}

size_t SimpleTextArea::getMaxLines() {
    return this->m_maxLines;
}

void SimpleTextArea::setWidth(const float width) {
        this->m_artificialWidth = true;

        this->setContentSize({ width, this->getContentSize().height });
        this->m_container->setContentSize({ width, this->m_container->getContentSize().height });
    }

float SimpleTextArea::getWidth() {
    return this->m_container->getContentSize().width;
}

void SimpleTextArea::setScale(const float scale) {
    this->m_scale = scale;

    this->updateContents();
}

float SimpleTextArea::getScale() {
    return this->m_scale;
}

void SimpleTextArea::setLinePadding(const float padding) {
    this->m_linePadding = padding;

    this->updateContents();
}

float SimpleTextArea::getLinePadding() {
    return this->m_linePadding;
}

std::vector<CCLabelBMFont*> SimpleTextArea::getLines() {
    return this->m_lines;
}

float SimpleTextArea::getHeight() {
    return this->m_container->getContentSize().height;
}

float SimpleTextArea::getLineHeight() {
    return this->m_lineHeight;
}

CCLabelBMFont* SimpleTextArea::createLabel(const std::string& text, const float top) {
    CCLabelBMFont* label = CCLabelBMFont::create(text.c_str(), this->m_font.c_str());

    label->setScale(this->m_scale);
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

    this->m_lines.push_back(newLine);

    return newLine;
}

float SimpleTextArea::calculateOffset(CCLabelBMFont* label) {
    return this->m_linePadding + label->getContentSize().height * this->m_scale;
}

void SimpleTextArea::updateLines() {
    float top = 0;
    CCLabelBMFont* line = this->createLabel("", top);
    this->m_lines = { line };

    for (const char c : this->m_text) {
        if (this->m_maxLines && this->m_lines.size() > this->m_maxLines) {
            std::cout << this->m_lines.size() << std::endl;
            CCLabelBMFont* last = this->m_lines.at(this->m_maxLines - 1);
            const std::string text = last->getString();

            this->m_lines.pop_back();
            last->setString(text.substr(0, text.size() - 3).append("...").c_str());

            break;
        } else if (c == '\n') {
            line = this->createLabel("", top -= this->calculateOffset(line));

            this->m_lines.push_back(line);
        } else if (this->m_artificialWidth && line->getContentSize().width >= this->getWidth()) {
            line = this->moveOverflow(line, c, top -= this->calculateOffset(line));
        } else {
            const std::string text = line->getString();

            line->setString((text + c).c_str());
        }
    }
}

void SimpleTextArea::updateContents() {
    this->updateLines();
    const size_t lineCount = this->m_lines.size();
    const float width = this->getWidth();

    if (lineCount > 0) {
        this->m_lineHeight = this->m_lines.back()->getContentSize().height * this->m_scale;
    } else {
        this->m_lineHeight = 0;
    }

    float height = this->m_lineHeight * lineCount + this->m_linePadding * (lineCount - 1);

    this->setContentSize({ width, height });
    this->m_container->setContentSize(this->getContentSize());
    this->m_container->removeAllChildren();

    height -= this->m_lineHeight;

    for (CCLabelBMFont* line : this->m_lines) {
        const float y = height + line->getPositionY();

        switch (this->m_alignment) {
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

        this->m_container->addChild(line);
    }
}