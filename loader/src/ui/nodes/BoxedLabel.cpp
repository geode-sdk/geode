#include <Geode/ui/BoxedLabel.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class BoxedLabel::Impl final {
public:
    float m_width = .0f;
    float m_padding = 10.0f;
    bool m_lockBoxWidth = false;

    Label* m_label = nullptr;
    NineSlice* m_bg = nullptr;

    void setupBg(ZStringView bg) {
        if (m_bg) m_bg->removeFromParent();

        m_bg = NineSlice::create(bg);
        m_bg->setZOrder(-1);
        m_bg->setOpacity(125);
        m_bg->setColor({0, 0, 0});
    };
};

BoxedLabel::BoxedLabel() : m_impl(std::make_unique<Impl>()) {};
BoxedLabel::~BoxedLabel() {};

bool BoxedLabel::init(std::string text, ZStringView font, ZStringView bg, float width) {
    if (!CCNode::init()) return false;

    m_impl->m_width = width;

    this->setAnchorPoint({.5f, .5f});

    m_impl->m_label = Label::createRich(std::move(text), font);

    m_impl->m_label->setAlignment(Label::Alignment::Center);
    m_impl->m_label->setBreakWords(true);

    m_impl->setupBg(bg);

    this->resize();

    this->addChildAtPosition(m_impl->m_label, Anchor::Center);
    this->addChildAtPosition(m_impl->m_bg, Anchor::Center);

    return true;
};

void BoxedLabel::resize() {
    if (m_impl->m_width > .0f) {
        float pad = m_impl->m_lockBoxWidth ? m_impl->m_padding : .0f;
        m_impl->m_label->setMaxWidth((m_impl->m_width - pad) / m_impl->m_label->getScale());
    }

    bool mustLockWidth = m_impl->m_lockBoxWidth && m_impl->m_width > .0f;

    this->setContentSize({
        mustLockWidth ? m_impl->m_width :
                        (m_impl->m_label->getScaledContentWidth() + m_impl->m_padding),
        m_impl->m_label->getScaledContentHeight() + m_impl->m_padding,
    });

    m_impl->m_bg->setContentSize(this->getScaledContentSize());

    this->updateLayout();
};

void BoxedLabel::setText(std::string text) {
    m_impl->m_label->setRichText(std::move(text));
    this->resize();
};

void BoxedLabel::setFont(ZStringView font) {
    m_impl->m_label->setFont(font);
    this->resize();
};

void BoxedLabel::setTextScale(float scale) {
    m_impl->m_label->setScale(scale);
    this->resize();
};

void BoxedLabel::setTextAlignment(Label::Alignment alignment) {
    m_impl->m_label->setAlignment(alignment);
};

void BoxedLabel::setMaxLabelWidth(float width) {
    m_impl->m_width = width;
    this->resize();
};

void BoxedLabel::setPadding(float padding) {
    m_impl->m_padding = padding;
    this->resize();
};

void BoxedLabel::setBGSprite(ZStringView spriteName) {
    m_impl->setupBg(spriteName);
    m_impl->m_bg->setContentSize(this->getScaledContentSize());

    this->addChildAtPosition(m_impl->m_bg, Anchor::Center);

    this->resize();
};

void BoxedLabel::setBGColor(ccColor3B const& color) {
    m_impl->m_bg->setColor(color);
};

void BoxedLabel::setBGOpacity(GLubyte opacity) {
    m_impl->m_bg->setOpacity(opacity);
};

void BoxedLabel::setBGScale(float scale) {
    m_impl->m_bg->setScaleMultiplier(scale);
};

void BoxedLabel::lockBGWidth(bool lock) {
    m_impl->m_lockBoxWidth = lock;
    this->resize();
};

float BoxedLabel::getMaxLabelWidth() const noexcept {
    return m_impl->m_width;
};

float BoxedLabel::getPadding() const noexcept {
    return m_impl->m_padding;
};

bool BoxedLabel::isBGWidthLocked() const noexcept {
    return m_impl->m_lockBoxWidth;
};

Label* BoxedLabel::getLabelNode() const noexcept {
    return m_impl->m_label;
};

NineSlice* BoxedLabel::getBGSprite() const noexcept {
    return m_impl->m_bg;
};

BoxedLabel* BoxedLabel::create(std::string text, ZStringView font) {
    return create(std::move(text), font, "white-square.png"_spr, .0f);
};

BoxedLabel* BoxedLabel::create(std::string text, ZStringView font, float width) {
    return create(std::move(text), font, "white-square.png"_spr, width);
};

BoxedLabel* BoxedLabel::create(std::string text, ZStringView font, ZStringView bg, float width) {
    auto ret = new BoxedLabel();
    if (ret->init(std::move(text), font, bg, width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};