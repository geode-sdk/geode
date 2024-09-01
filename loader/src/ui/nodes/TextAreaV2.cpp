#include <Geode/ui/TextAreaV2.hpp>
#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#define IMPL_GETTER(type, name, methodName) \
    type geode::TextAreaV2::get##methodName() const { return m_##name; }
#define IMPL_GETTER_SETTER(type, paramType, name, methodName) \
    void geode::TextAreaV2::set##methodName(const paramType name) { m_##name = name; this->update(); } \
    IMPL_GETTER(type, name, methodName)

geode::TextAreaV2* geode::TextAreaV2::create(const std::string& text, const std::string& font, const float scale, const float width, const float height, const bool deferUpdates) {
    TextAreaV2* area = new TextAreaV2(text, font, scale, width, height, deferUpdates);

    if (area && area->init()) {
        area->autorelease();

        return area;
    } else {
        CC_SAFE_DELETE(area);

        return nullptr;
    }
}

geode::TextAreaV2::TextAreaV2(const std::string& text, const std::string& font, const float scale, const float width, const float height, const bool deferUpdates) :
    PaddingNode(CCNode::create()),
    m_text(text),
    m_font(font),
    m_textColor({ 255, 255, 255, 255 }),
    m_alignment(Alignment::Left),
    m_wrappingMode(WrappingMode::WordWrap),
    m_maxLines(-1),
    m_textScale(scale),
    m_linePadding(0),
    m_minWidth(-1),
    m_maxWidth(width),
    m_minHeight(-1),
    m_maxHeight(height),
    m_hyphenate(true),
    m_ellipsis(true),
    m_deferUpdates(deferUpdates),
    m_deferred(false) { }

bool geode::TextAreaV2::init() {
    if (!PaddingNode::init()) {
        return false;
    }

    this->update();

    return true;
}

IMPL_GETTER_SETTER(std::string, std::string&, font, Font)
IMPL_GETTER_SETTER(std::string, std::string&, text, Text)
IMPL_GETTER_SETTER(ccColor4B, ccColor4B&, textColor, TextColor)
IMPL_GETTER_SETTER(geode::TextAreaV2::Alignment, Alignment, alignment, Alignment)
IMPL_GETTER_SETTER(geode::TextAreaV2::WrappingMode, WrappingMode, wrappingMode, WrappingMode)
IMPL_GETTER_SETTER(int, int, maxLines, MaxLines)
IMPL_GETTER_SETTER(float, float, textScale, TextScale)
IMPL_GETTER_SETTER(float, float, linePadding, LinePadding)
IMPL_GETTER_SETTER(float, float, minWidth, MinWidth)
IMPL_GETTER_SETTER(float, float, maxWidth, MaxWidth)
IMPL_GETTER_SETTER(float, float, minHeight, MinHeight)
IMPL_GETTER_SETTER(float, float, maxHeight, MaxHeight)
IMPL_GETTER_SETTER(bool, bool, hyphenate, Hyphenate)
IMPL_GETTER_SETTER(bool, bool, ellipsis, Ellipsis)
IMPL_GETTER(bool, deferUpdates, DeferUpdates)
IMPL_GETTER(std::vector<geode::TextAreaV2::Line>, lines, Lines)

void geode::TextAreaV2::resetMinWidth() {
    this->setMinWidth(-1);
}

void geode::TextAreaV2::resetMaxWidth() {
    this->setMaxWidth(-1);
}

void geode::TextAreaV2::resetMinHeight() {
    this->setMinHeight(-1);
}

void geode::TextAreaV2::resetMaxHeight() {
    this->setMaxHeight(-1);
}

void geode::TextAreaV2::resetMinBounds() {
    this->resetMinWidth();
    this->resetMinHeight();
}

void geode::TextAreaV2::resetMaxBounds() {
    this->resetMaxWidth();
    this->resetMaxHeight();
}

void geode::TextAreaV2::setDeferUpdates(const bool defer) {
    m_deferUpdates = defer;

    // If already deferred, update the container immediately and unschedule the deferred update
    if (m_deferred) {
        this->unschedule(schedule_selector(TextAreaV2::updateContainer));
        
        this->updateContainer();
    }
}

float geode::TextAreaV2::getLineHeight() const {
    return m_lines.empty() ? 0 : m_lines.front().label->getScaledContentHeight() + m_linePadding;
}

void geode::TextAreaV2::update() {
    if (!m_deferred) {
        if (m_deferUpdates) {
            m_deferred = true;

            this->scheduleOnce(schedule_selector(TextAreaV2::updateContainer), 0);
        } else {
            this->updateContainer();
        }
    }
}

void geode::TextAreaV2::updatePadding() {
    this->update();
}

void geode::TextAreaV2::updateContainer(const float dt) {
    PaddingNode::updatePadding();
    m_container->removeAllChildren();

    m_deferred = false;
    m_lines = this->createLines();

    const bool upperBoundWidth = m_maxWidth >= 0;
    const bool upperBoundHeight = m_maxHeight >= 0;
    const bool artificialWidth = m_minWidth >= 0 || upperBoundWidth;
    const bool artificialHeight = m_minHeight >= 0 || upperBoundHeight;
    const bool inheritedNodeSize = this->getContentSize() == this->getPaddedContainerSize();
    float height = m_lines.empty() ? 0 : m_lines.back().currentHeight;
    float width = 0;

    // First determine the container size before manipulating the node anchors and true positions
    for (const Line& line : m_lines) {
        width = std::max(width, line.label->getScaledContentWidth());
    }

    if (artificialWidth) {
        width = std::max(m_minWidth, width);

        m_container->setContentWidth(upperBoundWidth ? std::min(m_maxWidth, width) : width);
    } else {
        m_container->setContentWidth(width);
    }

    if (artificialHeight) {
        height = std::max(m_minHeight, height);

        m_container->setContentHeight(upperBoundHeight ? std::min(m_maxHeight, height) : height);
    } else {
        m_container->setContentHeight(height);
    }

    if (inheritedNodeSize) {
        this->setContentSize(this->getPaddedContainerSize());
    }

    for (const Line& line : m_lines) {
        this->updateLineAlignment(line.label);

        // Correct the Y position to be relative to the container height
        line.label->setPositionY(m_container->getContentHeight() - line.currentHeight + line.label->getScaledContentHeight());
        m_container->addChild(line.label);
    }
}

void geode::TextAreaV2::updateLineAlignment(CCLabelBMFont* line) {
    switch (m_alignment) {
        case Alignment::Left:
            line->setAnchorPoint({ 0, 1 });
            line->setPositionX(0);
            break;
        case Alignment::Center:
            line->setAnchorPoint({ 0.5f, 1 });
            line->setPositionX(m_container->getContentWidth() / 2);
            break;
        case Alignment::Right:
            line->setAnchorPoint({ 1, 1 });
            line->setPositionX(m_container->getContentWidth());
            break;
    }
}

void geode::TextAreaV2::addEllipsis(Line& line) {
    if (!m_ellipsis || line.lineNumber == -1) {
        return;
    }

    line.text = line.text.find_first_not_of(' ') == std::string::npos ?
        "..." :
        line.text + std::string("...").substr(std::min<size_t>(3, line.text.size() - line.text.find_last_not_of('.') - 1));

    line.label->setString(line.text.c_str());

    if (this->isWidthOverflowing(line.label)) {
        const size_t lineSize = line.text.size();
        const size_t overflow = this->getOverflowAmount(line.label, lineSize + 3);

        line.overflow = line.text.substr(lineSize - overflow - 3, overflow) + line.overflow;

        line.label->setString((line.text = line.text.substr(0, lineSize - overflow - 3) + "...").c_str());
    }
}

bool geode::TextAreaV2::isWidthOverflowing(const CCLabelBMFont* line) {
    return m_wrappingMode != WrappingMode::NoWrap && m_maxWidth >= 0 && line->getScaledContentWidth() + this->getTotalPaddingX() > m_maxWidth;
}

size_t geode::TextAreaV2::getOverflowAmount(CCLabelBMFont* line, const size_t lineSize) {
    for (size_t overflow = 1; overflow < lineSize; overflow++) {
        CCNode* character = cocos::getChild(line, lineSize - overflow);

        if ((character->getPositionX() - character->getContentWidth() / 2) * m_textScale <= m_maxWidth) {
            return overflow;
        }
    }

    return 0;
}

geode::TextAreaV2::Line geode::TextAreaV2::createLine(const std::string& text, Line& previousLine) {
    CCLabelBMFont* line = CCLabelBMFont::create(text.c_str(), m_font.c_str());

    line->setScale(m_textScale);
    line->setColor({ m_textColor.r, m_textColor.g, m_textColor.b });
    line->setOpacity(m_textColor.a);

    const size_t lineSize = text.size();
    const float currentHeight = m_linePadding + previousLine.currentHeight + line->getScaledContentHeight();
    const size_t overflow = this->isWidthOverflowing(line) ? this->getOverflowAmount(line, lineSize) : 0;
    const size_t overflowStart = lineSize - overflow;
    const std::string finalText = text.substr(0, overflowStart);
    const bool exceededLines = (m_maxLines != -1 && previousLine.lineNumber + 1 >= m_maxLines) ||
        (m_maxHeight != -1 && currentHeight + this->getTotalPaddingY() > m_maxHeight);

    if (overflow > 0) {
        line->setString(finalText.c_str());
    }

    previousLine.isLastLine = exceededLines;

    return {
        .text = finalText,
        .overflow = text.substr(overflowStart, overflow),
        .label = line,
        .lineNumber = previousLine.lineNumber + 1,
        .currentHeight = currentHeight,
        .isLastLine = false
    };
}

std::vector<geode::TextAreaV2::Line> geode::TextAreaV2::createLines() {
    Line placeholderLine = {
        .lineNumber = -1,
        .currentHeight = -m_linePadding
    };

    switch (m_wrappingMode) {
        case WrappingMode::NoWrap: return this->createNotWrap(placeholderLine);
        case WrappingMode::WordWrap: return this->createDelimitedWrap(m_text, placeholderLine, " `~!@#$%^&*()-_=+[{}];:'\",<.>/?\\|");
        case WrappingMode::SpaceWrap: return this->createDelimitedWrap(m_text, placeholderLine, " ");
        case WrappingMode::CutoffWrap: return this->createCutoffWrap(m_text, placeholderLine);
    }
}

std::vector<geode::TextAreaV2::Line> geode::TextAreaV2::createNotWrap(const Line& reference) {
    Line previousLine = reference;
    std::stringstream stream(m_text);
    std::vector<Line> lines;
    std::string line;

    while (std::getline(stream, line)) {
        const Line currentLine = this->createLine(line, previousLine);

        if (previousLine.isLastLine) {
            this->addEllipsis(previousLine);

            break;
        } else {
            lines.push_back(previousLine = currentLine);
        }
    }

    return lines;
}

std::vector<geode::TextAreaV2::Line> geode::TextAreaV2::createCutoffWrap(const std::string& text, Line& reference) {
    return this->wrapper(text, reference, [this](Line& currentLine) {
        if (currentLine.text.empty()) return std::vector<Line>();

        if (m_hyphenate && currentLine.text.back() != '-') {
            currentLine.overflow = currentLine.text.back() + currentLine.overflow;
            currentLine.text.pop_back();
            currentLine.text = currentLine.text.substr(0, currentLine.text.find_last_not_of(' ') + 1) + "-";

            currentLine.label->setString(currentLine.text.c_str());
        }

        return this->createCutoffWrap(currentLine.overflow.erase(0, currentLine.overflow.find_first_not_of(' ')), currentLine);
    });
}

std::vector<geode::TextAreaV2::Line> geode::TextAreaV2::createDelimitedWrap(const std::string& text, Line& reference, const std::string& delimiters) {
    return this->wrapper(text, reference, [this, delimiters](Line& currentLine) {
        if (currentLine.text.empty()) return std::vector<Line>();

        const size_t lineSize = currentLine.text.size();
        size_t additionalOverflow = 0;

        while (additionalOverflow < lineSize && delimiters.find(currentLine.text[lineSize - additionalOverflow - 1]) == std::string::npos) {
            additionalOverflow++;
        }

        if (additionalOverflow == lineSize) additionalOverflow = 0;

        currentLine.overflow = currentLine.text.substr(lineSize - additionalOverflow) + currentLine.overflow;
        currentLine.text = currentLine.text.substr(0, lineSize - additionalOverflow);

        if (m_hyphenate && currentLine.text.back() != '-') {
            if (additionalOverflow == 0) {
                currentLine.overflow = currentLine.text.back() + currentLine.overflow;
                currentLine.text.pop_back();
            }

            currentLine.text = currentLine.text.substr(0, currentLine.text.find_last_not_of(' ') + 1) + "-";
        }

        currentLine.label->setString(currentLine.text.c_str());

        return this->createDelimitedWrap(currentLine.overflow.erase(0, currentLine.overflow.find_first_not_of(' ')), currentLine, delimiters);
    });
}

std::vector<geode::TextAreaV2::Line> geode::TextAreaV2::wrapper(const std::string& text, Line& reference, const std::function<std::vector<Line>(Line& currentLine)>& onOverflow) {
    Line previousLine = reference;
    std::stringstream stream(text);
    std::vector<Line> lines;
    std::string line;

    while (std::getline(stream, line)) {
        Line currentLine = this->createLine(line, previousLine);

        if (previousLine.isLastLine) {
            if (previousLine.lineNumber == reference.lineNumber) {
                reference.isLastLine = true;

                this->addEllipsis(reference);
            } else {
                this->addEllipsis(previousLine);
            }

            break;
        } else if (currentLine.overflow.empty()) {
            lines.push_back(previousLine = currentLine);
        } else {
            const std::vector<Line> overflowedLines = onOverflow(currentLine);

            lines.push_back(currentLine);
            lines.insert(lines.end(), overflowedLines.begin(), overflowedLines.end());

            if ((previousLine = lines.back()).isLastLine) break;
        }
    }

    return lines;
}