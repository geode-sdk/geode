#include <Geode/ui/TextRenderer.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/string.hpp>

using namespace geode::prelude;
using namespace std::string_literals;

bool TextDecorationWrapper::init(
    TextRenderer::Label const& label, int deco, ccColor3B const& color, GLubyte opacity
) {
    if (!CCNodeRGBA::init()) return false;

    label.m_node->removeFromParent();
    this->addChild(label.m_node);

    m_label = label;
    m_deco = deco;
    this->setColor(color);
    this->setOpacity(opacity);

    return true;
}

void TextDecorationWrapper::draw() {
    // some nodes sometimes set the blend func to
    // something else without resetting it back
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (m_deco & TextDecorationUnderline) {
        ccDrawSolidRect(
            { 0, 0 }, { m_obContentSize.width, 1.f },
            { _realColor.r / 255.f, _realColor.g / 255.f, _realColor.b / 255.f,
              _realOpacity / 255.f }
        );
    }
    if (m_deco & TextDecorationStrikethrough) {
        ccDrawSolidRect(
            { 0, m_obContentSize.height * .4f - .75f },
            { m_obContentSize.width, m_obContentSize.height * .4f + .75f },
            { _realColor.r / 255.f, _realColor.g / 255.f, _realColor.b / 255.f,
              _realOpacity / 255.f }
        );
    }
    CCNode::draw();
}

void TextDecorationWrapper::setString(char const* text) {
    m_label.m_labelProtocol->setString(text);
    this->setContentSize(m_label.m_node->getScaledContentSize());
    m_label.m_node->setPosition(m_obContentSize / 2);
}

char const* TextDecorationWrapper::getString() {
    return m_label.m_labelProtocol->getString();
}

void TextDecorationWrapper::setColor(cocos2d::ccColor3B const& color) {
    m_label.m_rgbaProtocol->setColor(color);
    return CCNodeRGBA::setColor(color);
}

void TextDecorationWrapper::setOpacity(GLubyte opacity) {
    m_label.m_rgbaProtocol->setOpacity(opacity);
    return CCNodeRGBA::setOpacity(opacity);
}

void TextDecorationWrapper::updateDisplayedColor(ccColor3B const& color) {
    m_label.m_rgbaProtocol->updateDisplayedColor(color);
    return CCNodeRGBA::updateDisplayedColor(color);
}

void TextDecorationWrapper::updateDisplayedOpacity(GLubyte opacity) {
    m_label.m_rgbaProtocol->updateDisplayedOpacity(opacity);
    return CCNodeRGBA::updateDisplayedOpacity(opacity);
}

TextDecorationWrapper* TextDecorationWrapper::create(
    TextRenderer::Label const& label, int deco, ccColor3B const& color, GLubyte opacity
) {
    auto ret = new TextDecorationWrapper;
    if (ret->init(label, deco, color, opacity)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

TextDecorationWrapper* TextDecorationWrapper::wrap(
    TextRenderer::Label const& label, int deco, ccColor3B const& color, GLubyte opacity
) {
    auto pos = label.m_node->getPosition();
    auto wrapper = TextDecorationWrapper::create(label, deco, color, opacity);
    wrapper->setPosition(pos);
    return wrapper;
}

bool TextLinkedButtonWrapper::init(
    TextRenderer::Label const& label, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handler
) {
    if (!CCMenuItemSprite::initWithNormalSprite(label.m_node, nullptr, nullptr, target, handler))
        return false;

    m_label = label;

    label.m_node->removeFromParent();
    this->addChild(label.m_node);

    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);

    return true;
}

void TextLinkedButtonWrapper::link(TextLinkedButtonWrapper* other) {
    if (this != other) {
        m_linked.push_back(other);
    }
}

void TextLinkedButtonWrapper::setString(char const* text) {
    m_label.m_labelProtocol->setString(text);
    this->setContentSize(m_label.m_node->getScaledContentSize());
    m_label.m_node->setPosition(m_obContentSize * m_label.m_node->getAnchorPoint());
}

char const* TextLinkedButtonWrapper::getString() {
    return m_label.m_labelProtocol->getString();
}

void TextLinkedButtonWrapper::setColor(cocos2d::ccColor3B const& color) {
    m_label.m_rgbaProtocol->setColor(color);
    return CCMenuItemSprite::setColor(color);
}

void TextLinkedButtonWrapper::setOpacity(GLubyte opacity) {
    m_label.m_rgbaProtocol->setOpacity(opacity);
    return CCMenuItemSprite::setOpacity(opacity);
}

void TextLinkedButtonWrapper::updateDisplayedColor(ccColor3B const& color) {
    m_label.m_rgbaProtocol->updateDisplayedColor(color);
    return CCMenuItemSprite::updateDisplayedColor(color);
}

void TextLinkedButtonWrapper::updateDisplayedOpacity(GLubyte opacity) {
    m_label.m_rgbaProtocol->updateDisplayedOpacity(opacity);
    return CCMenuItemSprite::updateDisplayedOpacity(opacity);
}

void TextLinkedButtonWrapper::selectedWithoutPropagation(bool selected) {
    if (selected) {
        m_opacity = this->getOpacity();
        m_color = this->getColor();
        this->setOpacity(150);
        this->setColor({ 255, 255, 255 });
    }
    else {
        this->setOpacity(m_opacity);
        this->setColor(m_color);
    }
}

void TextLinkedButtonWrapper::selected() {
    this->selectedWithoutPropagation(true);
    for (auto& node : m_linked) {
        node->selectedWithoutPropagation(true);
    }
}

void TextLinkedButtonWrapper::unselected() {
    this->selectedWithoutPropagation(false);
    for (auto& node : m_linked) {
        node->selectedWithoutPropagation(false);
    }
}

TextLinkedButtonWrapper* TextLinkedButtonWrapper::create(
    TextRenderer::Label const& label, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handler
) {
    auto ret = new TextLinkedButtonWrapper;
    if (ret->init(label, target, handler)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

TextLinkedButtonWrapper* TextLinkedButtonWrapper::wrap(
    TextRenderer::Label const& label, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handler
) {
    auto pos = label.m_node->getPosition();
    auto anchor = label.m_node->getAnchorPoint();
    auto wrapper = TextLinkedButtonWrapper::create(label, target, handler);
    wrapper->setPosition(pos - wrapper->getScaledContentSize() * anchor);
    return wrapper;
}

bool TextRenderer::init() {
    return true;
}

void TextRenderer::begin(CCNode* target, CCPoint const& pos, CCSize const& size) {
    m_target = target ? target : CCNode::create();
    m_target->setContentSize(size);
    m_target->setPosition(pos);
    m_target->removeAllChildren();
    m_cursor = CCPointZero;
    m_origin = pos;
    m_size = size;
}

CCNode* TextRenderer::end(
    bool fitToContent, TextAlignment horizontalAlign, TextAlignment verticalAlign
) {
    // adjust vertical alignments of last line
    this->adjustLineAlignment();
    // resize target
    if (fitToContent && m_target) {
        // figure out area covered by target children
        auto coverage = calculateChildCoverage(m_target);
        // convert area from rect to size
        auto renderedWidth = -coverage.origin.x + coverage.size.width;
        auto renderedHeight = -coverage.origin.y + coverage.size.height;
        // target size is always at least requested size
        m_target->setContentSize({ std::max(renderedWidth, m_size.width),
                                   std::max(renderedHeight, m_size.height) });
        // calculate paddings
        float padX;
        float padY;
        switch (horizontalAlign) {
            default:
            case TextAlignment::Begin: padX = .0f; break;
            case TextAlignment::Center:
                padX = std::max(m_size.width - renderedWidth, 0.f) / 2;
                break;
            case TextAlignment::End: padX = std::max(m_size.width - renderedWidth, 0.f); break;
        }
        switch (verticalAlign) {
            default:
            case TextAlignment::Begin: padY = .0f; break;
            case TextAlignment::Center:
                padY = std::max(m_size.height - renderedHeight, 0.f) / 2;
                break;
            case TextAlignment::End: padY = std::max(m_size.height - renderedHeight, 0.f); break;
        }
        // adjust child positions
        for (auto child : CCArrayExt<CCNode*>(m_target->getChildren())) {
            child->setPosition(
                child->getPositionX() + padX,
                child->getPositionY() + m_target->getContentSize().height - coverage.size.height -
                    padY
            );
        }
    }

    // clear stacks
    m_fontStack.clear();
    m_scaleStack.clear();
    m_styleStack.clear();
    m_colorStack.clear();
    m_opacityStack.clear();
    m_decorationStack.clear();
    m_capsStack.clear();
    m_lastRendered.clear();
    m_indentationStack.clear();
    m_wrapOffsetStack.clear();
    m_hAlignmentStack.clear();
    m_vAlignmentStack.clear();

    // reset
    m_cursor = CCPointZero;
    m_size = CCSizeZero;
    auto ret = m_target;
    m_target = nullptr;
    m_lastRendered = {};
    CC_SAFE_RELEASE_NULL(m_lastRenderedNode);
    m_renderedLine.clear();

    return ret;
}

void TextRenderer::moveCursor(CCPoint const& pos) {
    m_cursor = pos;
}

CCPoint TextRenderer::getCursorPos() {
    return m_cursor;
}

bool TextRenderer::render(std::string const& word, CCNode* to, CCLabelProtocol* label) {
    auto origLabelStr = label->getString();
    auto str = ((origLabelStr && strlen(origLabelStr)) ? origLabelStr : "") + word;
    if (m_size.width) {
        std::string orig = origLabelStr;
        label->setString(str.c_str());
        if (m_cursor.x + to->getScaledContentSize().width >
            m_size.width - this->getCurrentWrapOffset()) {
            label->setString(orig.c_str());
            return false;
        }
        return true;
    }
    else {
        label->setString(str.c_str());
        return true;
    }
}

TextRenderer::Label TextRenderer::addWrappers(
    Label const& label, bool isButton, CCObject* target, SEL_MenuHandler callback
) {
    Label ret = label;
    if (this->getCurrentDeco() != TextDecorationNone) {
        auto wrapper = TextDecorationWrapper::wrap(
            ret, this->getCurrentDeco(), this->getCurrentColor(), this->getCurrentOpacity()
        );
        ret = Label(wrapper);
    }
    if (isButton) {
        auto wrapper = TextLinkedButtonWrapper::wrap(ret, target, callback);
        ret = Label(wrapper);
    }
    ret.m_lineHeight = label.m_lineHeight;
    return ret;
}

std::vector<TextRenderer::Label> TextRenderer::renderStringEx(
    std::string const& str, Font font, float scale, ccColor3B color, GLubyte opacity, int style,
    int deco, TextCapitalization caps, bool addToTarget, bool isButton, CCObject* target,
    SEL_MenuHandler callback
) {
    std::vector<Label> res;

    if (!target) target = m_target;

    Label label;
    bool newLine = true;

    auto lastIndent =
        m_indentationStack.size() > 1 ? m_indentationStack.at(m_indentationStack.size() - 1) : .0f;

    if (m_cursor.x < m_origin.x + this->getCurrentIndent()) {
        m_cursor.x = this->getCurrentIndent();
    }

    auto createLabel = [&]() -> bool {
        // create label through font and add
        // decorations (underline, strikethrough) +
        // buttonize (new word just dropped)
        label = this->addWrappers(font(style), isButton, target, callback);

        label.m_node->setScale(scale);
        label.m_node->setPosition(m_cursor);
        label.m_node->setAnchorPoint({ .0f, label.m_node->getAnchorPoint().y });
        label.m_rgbaProtocol->setColor(color);
        label.m_rgbaProtocol->setOpacity(opacity);

        res.push_back(label);
        m_renderedLine.push_back(label.m_node);
        if (addToTarget) {
            m_target->addChild(label.m_node);
        }

        return true;
    };

    auto nextLine = [&]() -> bool {
        this->breakLine(label.m_lineHeight * scale);
        if (!createLabel()) return false;
        newLine = true;
        return true;
    };

    // create initial label
    if (!createLabel()) return {};

    bool firstLine = true;
    for (auto line : utils::string::split(str, "\n")) {
        if (!firstLine && !nextLine()) {
            return {};
        }
        firstLine = false;
        for (auto word : utils::string::split(line, " ")) {
            // add extra space in front of word if not on
            // new line
            if (!newLine) word = " " + word;
            newLine = false;

            // update capitalization
            switch (caps) {
                case TextCapitalization::AllUpper: utils::string::toUpperIP(word); break;
                case TextCapitalization::AllLower: utils::string::toLowerIP(word); break;
                default: break;
            }

            // try to render at the end of current line
            if (this->render(word, label.m_node, label.m_labelProtocol)) continue;

            // try to create a new line
            if (!nextLine()) return {};

            if (utils::string::startsWith(word, " ")) word = word.substr(1);
            newLine = false;

            // try to render on new line
            if (this->render(word, label.m_node, label.m_labelProtocol)) continue;

            // no need to create a new line as we know
            // the current one has no content and is
            // supposed to receive this one

            // render character by character
            for (auto& c : word) {
                if (!this->render(std::string(1, c), label.m_node, label.m_labelProtocol)) {
                    if (!nextLine()) return {};

                    if (utils::string::startsWith(word, " ")) word = word.substr(1);
                    newLine = false;
                }
            }
        }
        // increment cursor position
        m_cursor.x += label.m_node->getScaledContentSize().width;
    }

    if (isButton) {
        for (auto& btn : res)
            for (auto& b : res) {
                if (btn.m_node != b.m_node) {
                    static_cast<TextLinkedButtonWrapper*>(btn.m_node)
                        ->link(static_cast<TextLinkedButtonWrapper*>(b.m_node));
                }
            }
    }

    CC_SAFE_RELEASE_NULL(m_lastRenderedNode);
    m_lastRendered = res;

    return res;
}

std::vector<TextRenderer::Label> TextRenderer::renderString(std::string const& str) {
    return this->renderStringEx(
        str, this->getCurrentFont(), this->getCurrentScale(), this->getCurrentColor(),
        this->getCurrentOpacity(), this->getCurrentStyle(), this->getCurrentDeco(),
        this->getCurrentCaps(), true, false, nullptr, nullptr
    );
}

std::vector<TextRenderer::Label> TextRenderer::renderStringInteractive(
    std::string const& str, CCObject* target, SEL_MenuHandler callback
) {
    return this->renderStringEx(
        str, this->getCurrentFont(), this->getCurrentScale(), this->getCurrentColor(),
        this->getCurrentOpacity(), this->getCurrentStyle(), this->getCurrentDeco(),
        this->getCurrentCaps(), true, true, target, callback
    );
}

CCNode* TextRenderer::renderNode(CCNode* node) {
    m_cursor.x += node->getScaledContentSize().width * node->getAnchorPoint().x;
    node->setPosition(m_cursor);
    m_target->addChild(node);
    m_cursor.x += node->getScaledContentSize().width * (1.f - node->getAnchorPoint().x);
    m_lastRendered.clear();
    CC_SAFE_RELEASE(m_lastRenderedNode);
    m_lastRenderedNode = node;
    CC_SAFE_RETAIN(m_lastRenderedNode);
    m_renderedLine.push_back(node);
    return node;
}

void TextRenderer::breakLine(float incY) {
    auto h = this->adjustLineAlignment();
    m_renderedLine.clear();
    float y = incY;
    if (!y && m_fontStack.size()) {
        y = m_fontStack.back()(this->getCurrentStyle()).m_lineHeight * this->getCurrentScale();
        if (!y && m_lastRendered.size()) {
            y = static_cast<CCNode*>(m_lastRendered.back().m_node)->getScaledContentSize().height;
        }
        if (!y && m_lastRenderedNode) {
            y = m_lastRenderedNode->getScaledContentSize().height;
        }
    }
    if (h > y) y = h;
    m_cursor.y -= y;
    m_cursor.x = m_origin.x;
}

float TextRenderer::adjustLineAlignment() {
    auto coverage = calculateNodeCoverage(m_renderedLine);
    auto maxWidth = -coverage.origin.x + coverage.size.width;
    auto maxHeight = .0f;
    for (auto& node : m_renderedLine) {
        if (node->getScaledContentSize().height > maxHeight) {
            maxHeight = node->getScaledContentSize().height;
        }
    }
    for (auto& node : m_renderedLine) {
        auto height = node->getScaledContentSize().height;
        auto anchor = node->getAnchorPoint().y;
        switch (this->getCurrentVerticalAlign()) {
            case TextAlignment::Begin:
            default: {
                node->setPositionY(m_cursor.y - height * (1.f - anchor));
            } break;

            case TextAlignment::Center: {
                node->setPositionY(m_cursor.y - maxHeight / 2 + height * (.5f - anchor));
            } break;

            case TextAlignment::End: {
                node->setPositionY(m_cursor.y - maxHeight + height * anchor);
            } break;
        }
        switch (this->getCurrentHorizontalAlign()) {
            case TextAlignment::Begin:
            default: {
                // already correct
            } break;

            case TextAlignment::Center: {
                node->setPositionX(node->getPositionX() + (m_size.width - maxWidth) / 2);
            } break;

            case TextAlignment::End: {
                node->setPositionX(
                    node->getPositionX() + m_size.width - maxWidth - this->getCurrentIndent()
                );
            } break;
        }
    }
    return maxHeight;
}

void TextRenderer::pushBMFont(char const* bmFont) {
    m_fontStack.push_back([bmFont](int) -> Label {
        return CCLabelBMFont::create("", bmFont);
    });
}

void TextRenderer::pushFont(Font const& font) {
    m_fontStack.push_back(font);
}

void TextRenderer::popFont() {
    if (m_fontStack.size()) m_fontStack.pop_back();
}

TextRenderer::Font TextRenderer::getCurrentFont() const {
    if (!m_fontStack.size()) {
        return [](int) -> Label {
            return CCLabelBMFont::create("", "bigFont.fnt");
        };
    }
    return m_fontStack.back();
}

void TextRenderer::pushScale(float scale) {
    m_scaleStack.push_back(scale);
}

void TextRenderer::popScale() {
    if (m_scaleStack.size()) m_scaleStack.pop_back();
}

float TextRenderer::getCurrentScale() const {
    return m_scaleStack.size() ? m_scaleStack.back() : 1.f;
}

void TextRenderer::pushStyleFlags(int style) {
    int oldStyle = TextStyleRegular;
    if (m_styleStack.size()) oldStyle = m_styleStack.back();
    m_styleStack.push_back(oldStyle | style);
}

void TextRenderer::popStyleFlags() {
    if (m_styleStack.size()) m_styleStack.pop_back();
}

int TextRenderer::getCurrentStyle() const {
    return m_styleStack.size() ? m_styleStack.back() : TextStyleRegular;
}

void TextRenderer::pushColor(ccColor3B const& color) {
    m_colorStack.push_back(color);
}

void TextRenderer::popColor() {
    if (m_colorStack.size()) m_colorStack.pop_back();
}

ccColor3B TextRenderer::getCurrentColor() const {
    return m_colorStack.size() ? m_colorStack.back() : ccColor3B { 255, 255, 255 };
}

void TextRenderer::pushOpacity(GLubyte opacity) {
    m_opacityStack.push_back(opacity);
}

void TextRenderer::popOpacity() {
    if (m_opacityStack.size()) m_opacityStack.pop_back();
}

GLubyte TextRenderer::getCurrentOpacity() const {
    return m_opacityStack.size() ? m_opacityStack.back() : 255;
}

void TextRenderer::pushDecoFlags(int deco) {
    int oldDeco = TextDecorationNone;
    if (m_decorationStack.size()) oldDeco = m_decorationStack.back();
    m_decorationStack.push_back(oldDeco | deco);
}

void TextRenderer::popDecoFlags() {
    if (m_decorationStack.size()) m_decorationStack.pop_back();
}

int TextRenderer::getCurrentDeco() const {
    return m_decorationStack.size() ? m_decorationStack.back() : TextDecorationNone;
}

void TextRenderer::pushCaps(TextCapitalization caps) {
    m_capsStack.push_back(caps);
}

void TextRenderer::popCaps() {
    if (m_capsStack.size()) m_capsStack.pop_back();
}

TextCapitalization TextRenderer::getCurrentCaps() const {
    return m_capsStack.size() ? m_capsStack.back() : TextCapitalization::Normal;
}

void TextRenderer::pushIndent(float indent) {
    m_indentationStack.push_back(indent);
}

void TextRenderer::popIndent() {
    if (m_indentationStack.size()) m_indentationStack.pop_back();
}

float TextRenderer::getCurrentIndent() const {
    float res = .0f;
    for (auto& indent : m_indentationStack) {
        res += indent;
    }
    return res;
}

void TextRenderer::pushWrapOffset(float wrapOffset) {
    m_wrapOffsetStack.push_back(wrapOffset);
}

void TextRenderer::popWrapOffset() {
    if (m_wrapOffsetStack.size()) m_wrapOffsetStack.pop_back();
}

float TextRenderer::getCurrentWrapOffset() const {
    float res = .0f;
    for (auto& offset : m_wrapOffsetStack) {
        res += offset;
    }
    return res;
}

void TextRenderer::pushVerticalAlign(TextAlignment align) {
    m_vAlignmentStack.push_back(align);
}

void TextRenderer::popVerticalAlign() {
    if (m_vAlignmentStack.size()) m_vAlignmentStack.pop_back();
}

TextAlignment TextRenderer::getCurrentVerticalAlign() const {
    return m_vAlignmentStack.size() ? m_vAlignmentStack.back() : TextAlignment::Center;
}

void TextRenderer::pushHorizontalAlign(TextAlignment align) {
    m_hAlignmentStack.push_back(align);
}

void TextRenderer::popHorizontalAlign() {
    if (m_hAlignmentStack.size()) m_hAlignmentStack.pop_back();
}

TextAlignment TextRenderer::getCurrentHorizontalAlign() const {
    return m_hAlignmentStack.size() ? m_hAlignmentStack.back() : TextAlignment::Begin;
}

TextRenderer::~TextRenderer() {
    this->end();
}

TextRenderer* TextRenderer::create() {
    auto ret = new TextRenderer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
