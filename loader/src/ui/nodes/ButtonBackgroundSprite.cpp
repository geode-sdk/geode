#include <Geode/ui/ButtonBackgroundSprite.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class ButtonBackgroundSprite::Impl {
public:
    NineSlice* bg = nullptr;
    Label* label = nullptr;
    CCNode* node = nullptr;

    std::string bgTexture = "GJ_button_01.png";
    bool bgTextureIsFrame = false;
    std::string text;
    std::string font = "goldFont.fnt";
    NodeAlignment nodeAlignment = NodeAlignment::Left;

    float labelScale = 1.f;
    float nodeScale = 1.f;

    std::optional<float> width = std::nullopt;
    float minWidth = 0.f;
    float maxWidth = std::numeric_limits<float>::max();
    std::optional<float> height = std::nullopt;
    float minHeight = 0.f;
    float maxHeight = std::numeric_limits<float>::max();

    Padding padding = {4.f, 4.f, 4.f, 4.f};
    float gap = 5.f;
    CrossAxisAlignment lineAlignment = CrossAxisAlignment::Center;
};

ButtonBackgroundSprite::ButtonBackgroundSprite() : m_impl(std::make_unique<Impl>()) {}

ButtonBackgroundSprite::~ButtonBackgroundSprite() {}

bool ButtonBackgroundSprite::init(cocos2d::CCNode* node, std::optional<ZStringView> text, ZStringView font, ZStringView background) {
    if (!CCNode::init()) {
        return false;
    }

    setAnchorPoint({0.5f, 0.5f});

    setBG(background, false);

    if (node) {
        setNode(node);
    }

    if (text.has_value()) {
        setString(text->c_str());
        setFont(font);
    }

    return true;
}

ButtonBackgroundSprite* ButtonBackgroundSprite::create(ZStringView text, ZStringView font, ZStringView background) {
    auto ret = new ButtonBackgroundSprite();
    if (ret->init(nullptr, text, font, background)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

ButtonBackgroundSprite* ButtonBackgroundSprite::createWithNode(cocos2d::CCNode* node, ZStringView background) {
    auto ret = new ButtonBackgroundSprite();
    if (ret->init(node, std::nullopt, "", background)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

ButtonBackgroundSprite* ButtonBackgroundSprite::createWithNodeLabel(cocos2d::CCNode* node, ZStringView text, ZStringView font, ZStringView background) {
    auto ret = new ButtonBackgroundSprite();
    if (ret->init(node, text, font, background)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void ButtonBackgroundSprite::setBG(ZStringView background, bool isFrame) {
    if (m_impl->bg) {
        m_impl->bg->removeFromParent();

        if (m_impl->label) {
            m_impl->label->removeFromParent();
        }
        if (m_impl->node) {
            m_impl->node->removeFromParent();
        }
    }

    if (isFrame) {
        m_impl->bg = NineSlice::createWithSpriteFrameName(background);
    }
    else {
        m_impl->bg = NineSlice::create(background);
    }

    m_impl->bg->setLayout(AxisLayout::create());

    if (m_impl->label) {
        m_impl->bg->addChild(m_impl->label);
    }
    if (m_impl->node) {
        m_impl->bg->addChild(m_impl->node);
    }

    m_impl->bgTexture = background;
    m_impl->bgTextureIsFrame = isFrame;

    addChild(m_impl->bg);

    updateElements();
}

NineSlice* ButtonBackgroundSprite::getBG() const {
    return m_impl->bg;
}

void ButtonBackgroundSprite::setNode(cocos2d::CCNode* node) {
    if (m_impl->node) {
        m_impl->node->removeFromParent();
    }

    m_impl->node = node;
    
    m_impl->node->setZOrder(1);

    auto options = typeinfo_cast<AxisLayoutOptions*>(m_impl->node->getLayoutOptions());

    if (!options) {
        options = AxisLayoutOptions::create();
        m_impl->node->setLayoutOptions(options, false);
    }

    options->setScaleLimits(std::nullopt, node->getScale());

    m_impl->nodeScale = node->getScale();

    if (!m_impl->bg) {
        setBG(m_impl->bgTexture, m_impl->bgTextureIsFrame);
    }

    m_impl->bg->addChild(m_impl->node);

    updateElements();
}

void ButtonBackgroundSprite::removeNode() {
    if (m_impl->node) {
        m_impl->node->removeFromParent();
    }

    m_impl->node = nullptr;

    updateElements();
}

cocos2d::CCNode* ButtonBackgroundSprite::getNode() const {
    return m_impl->node;
}

void ButtonBackgroundSprite::setNodeAlignment(NodeAlignment alignment) {
    m_impl->nodeAlignment = alignment;
    
    updateElements();
}

ButtonBackgroundSprite::NodeAlignment ButtonBackgroundSprite::getNodeAlignment() const {
    return m_impl->nodeAlignment;
}

void ButtonBackgroundSprite::setString(char const* label) {
    if (m_impl->label) {
        m_impl->label->setText(label);
    }
    else {
        m_impl->label = Label::create(label, m_impl->font);
        m_impl->label->setLayoutOptions(AxisLayoutOptions::create()
            ->setScalePriority(AXISLAYOUT_DEFAULT_PRIORITY + 1)
        );

        if (!m_impl->bg) {
            setBG(m_impl->bgTexture, m_impl->bgTextureIsFrame);
        }

        m_impl->bg->addChild(m_impl->label);
    }

    updateElements();
}

char const* ButtonBackgroundSprite::getString() {
    return m_impl->label ? m_impl->label->getString() : "";
}

void ButtonBackgroundSprite::removeLabel() {
    if (m_impl->label) {
        m_impl->label->removeFromParent();
    }

    m_impl->label = nullptr;

    updateElements();
}

Label* ButtonBackgroundSprite::getLabel() const {
    return m_impl->label;
}

void ButtonBackgroundSprite::setFont(ZStringView font) {
    if (m_impl->label) {
        m_impl->label->setFont(font);
    }

    m_impl->font = font;

    updateElements();
}

ZStringView ButtonBackgroundSprite::getFont() const {
    return m_impl->font;
}

void ButtonBackgroundSprite::setLabelScale(float scale) {
    m_impl->labelScale = scale;

    updateElements();
}

float ButtonBackgroundSprite::getLabelScale() const {
    return m_impl->labelScale;
}

void ButtonBackgroundSprite::setNodeScale(float scale) {
    m_impl->nodeScale = scale;

    updateElements();
}

float ButtonBackgroundSprite::getNodeScale() const{
    return m_impl->nodeScale;
}

void ButtonBackgroundSprite::setWidth(std::optional<float> width) {
    m_impl->width = width;

    updateElements();
}

std::optional<float> ButtonBackgroundSprite::getWidth() const {
    return m_impl->width;
}

void ButtonBackgroundSprite::setMinWidth(float width) {
    m_impl->minWidth = width;
    
    updateElements();
}

float ButtonBackgroundSprite::getMinWidth() const {
    return m_impl->minWidth;
}

void ButtonBackgroundSprite::setMaxWidth(float width) {
    m_impl->maxWidth = width;

    updateElements();
}

float ButtonBackgroundSprite::getMaxWidth() const {
    return m_impl->maxWidth;
}

void ButtonBackgroundSprite::setHeight(std::optional<float> height) {
    m_impl->height = height;

    updateElements();
}

std::optional<float> ButtonBackgroundSprite::getHeight() const {
    return m_impl->height;
}

void ButtonBackgroundSprite::setMinHeight(float height) {
    m_impl->minHeight = height;

    updateElements();
}

float ButtonBackgroundSprite::getMinHeight() const {
    return m_impl->minHeight;
}

void ButtonBackgroundSprite::setMaxHeight(float height) {
    m_impl->maxHeight = height;

    updateElements();
}

float ButtonBackgroundSprite::getMaxHeight() const {
    return m_impl->maxHeight;
}

void ButtonBackgroundSprite::setPadding(const Padding& padding) {
    m_impl->padding = padding;

    updateElements();
}

const Padding& ButtonBackgroundSprite::getPadding() const {
    return m_impl->padding;
}

void ButtonBackgroundSprite::setGap(float gap) {
    m_impl->gap = gap;

    updateElements();
}

float ButtonBackgroundSprite::getGap() const {
    return m_impl->gap;
}

void ButtonBackgroundSprite::setLineAlignment(CrossAxisAlignment alignment) {
    m_impl->lineAlignment = alignment;

    updateElements();
}

CrossAxisAlignment ButtonBackgroundSprite::getLineAlignment() const {
    return m_impl->lineAlignment;
}

void ButtonBackgroundSprite::setColor(cocos2d::ccColor3B const& color) {
    if (m_impl->bg) {
        m_impl->bg->setColor(color);
    }
    if (auto rgba = typeinfo_cast<CCNodeRGBA*>(m_impl->node)) {
        rgba->setColor(color);
    }
    if (m_impl->label) {
        m_impl->label->setColor(color);
    }

    updateElements();
}

void ButtonBackgroundSprite::setOpacity(GLubyte opacity) {
    if (m_impl->bg) {
        m_impl->bg->setOpacity(opacity);
    }
    if (auto rgba = typeinfo_cast<CCNodeRGBA*>(m_impl->node)) {
        rgba->setOpacity(opacity);
    }
    if (m_impl->label) {
        m_impl->label->setOpacity(opacity);
    }

    updateElements();
}

void ButtonBackgroundSprite::updateElements() {
    if (!m_impl->bg) {
        return;
    }

    if (m_impl->label) {
        static_cast<AxisLayoutOptions*>(m_impl->label->getLayoutOptions())
            ->setScaleLimits(std::nullopt, m_impl->labelScale);
    }
    if (m_impl->node) {
        static_cast<AxisLayoutOptions*>(m_impl->node->getLayoutOptions())
            ->setScaleLimits(std::nullopt, m_impl->nodeScale);
    }

    // its kinda dumb to do 2 layout passes but the only other solution is dumb bs math so wtv
    static_cast<AxisLayout*>(m_impl->bg->getLayout())
        ->setAxisReverse(m_impl->nodeAlignment == NodeAlignment::Top || m_impl->nodeAlignment == NodeAlignment::Right)
        ->setAxis((m_impl->nodeAlignment == NodeAlignment::Top || m_impl->nodeAlignment == NodeAlignment::Bottom) ? Axis::Column : Axis::Row)
        ->setGap(m_impl->gap)
        ->setPadding(m_impl->padding)
        ->setCrossAxisLineAlignment(static_cast<AxisAlignment>(std::to_underlying(m_impl->lineAlignment)))
        ->setAxisAlignment(AxisAlignment::Center)
        ->setAutoGrowAxis(0.f)
        ->setDefaultScaleLimits(0.1f, 1.f)
        ->setCrossAxisOverflow(true)
        ->setAutoScale(false);
    m_impl->bg->updateLayout();

    const CCSize size{
        m_impl->width.value_or(std::clamp(m_impl->bg->getContentWidth(), m_impl->minWidth, m_impl->maxWidth)),
        m_impl->height.value_or(std::clamp(m_impl->bg->getContentHeight(), m_impl->minHeight, m_impl->maxHeight))
    };

    if (size != m_impl->bg->getContentSize()) {
        static_cast<AxisLayout*>(m_impl->bg->getLayout())
            ->setAutoGrowAxis(std::nullopt)
            ->setCrossAxisOverflow(false)
            ->setAutoScale(true);
        m_impl->bg->setContentSize(size);
        m_impl->bg->updateLayout();
    }

    // so the label is centered like in robs buttonsprite
    if (m_impl->label) {
        m_impl->label->setPositionY(m_impl->label->getPositionY() - 4.0f * m_impl->label->getScale());
    }
    
    setContentSize(size);
    
    m_impl->bg->setPosition(size / 2);
}