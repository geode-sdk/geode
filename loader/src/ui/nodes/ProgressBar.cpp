#include <Geode/ui/Label.hpp>
#include <Geode/ui/ProgressBar.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class ProgressBar::Impl final {
public:
    CCSprite* m_bar = nullptr;
    CCSprite* m_barFill = nullptr;
    Label* m_percentLabel = nullptr;

    // Current progress bar fill percentage ranging from 0 to 100
    float m_progress = .0f;

    Style m_style = Style::Level;
    ccColor3B m_barFillColor = {255, 255, 255};
    bool m_showPercentLabel = false;

    // Precision of the percentage on the progress percent label
    size_t m_precision = 0;

    float m_barFillMaxWidth = .0f;
    float m_barFillMaxHeight = .0f;

    // Animation stuff
    bool m_isAnimating = false;
    float m_animDuration = .0f;
    float m_animElapsed = .0f;
    float m_animProgressFrom = .0f;
    float m_animProgressTo = 100.0f;
    EasingType m_animEase = EasingType::None;
    AnimationCallback m_animCallback = nullptr;

    void setupBarStyle(ZStringView barSpr, ZStringView fillSpr) {
        m_bar = CCSprite::create(barSpr.c_str());
        m_bar->setID("progress-bar");
        m_bar->setAnchorPoint({.5, .5});
        m_bar->setPosition(m_bar->getScaledContentSize() / 2.0f);
        m_bar->setZOrder(1);

        m_barFill = CCSprite::create(fillSpr.c_str());
        m_barFill->setID("progress-bar-fill");
        m_barFill->setAnchorPoint({0, .5});
        m_barFill->setPosition({2.0f, m_bar->getScaledContentHeight() / 2.0f});
        m_barFill->setColor(m_barFillColor);
        m_barFill->setZOrder(-1);

        ccTexParams params{GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        m_barFill->getTexture()->setTexParameters(&params);

        m_barFillMaxWidth = m_bar->getScaledContentWidth() - 4.0f;
        m_barFillMaxHeight = m_barFill->getScaledContentHeight() - .5f;

        m_percentLabel = Label::create("0%", "bigFont.fnt");
        m_percentLabel->setID("progress-percent-label");
        m_percentLabel->setScale(.5f);
        m_percentLabel->setAnchorPoint({0, .5});
        m_percentLabel->setPosition({
            m_bar->getScaledContentWidth() + 2.5f,
            m_bar->getScaledContentHeight() / 2.0f,
        });
        m_percentLabel->setAlignment(Label::Alignment::Left);
        m_percentLabel->setVisible(m_showPercentLabel);
        m_percentLabel->setZOrder(1);
    }

    void setupSolidStyle() {
        m_bar = CCSprite::create("GJ_progressBar_001.png");
        m_bar->setID("progress-bar");
        m_bar->setAnchorPoint({.5, .5});
        m_bar->setPosition(m_bar->getScaledContentSize() / 2.0f);
        m_bar->setColor({0, 0, 0});
        m_bar->setOpacity(125);
        m_bar->setZOrder(-1);

        m_barFill = CCSprite::create("GJ_progressBar_001.png");
        m_barFill->setID("progress-bar-fill");
        m_barFill->setScale(.992f);
        m_barFill->setScaleY(.86f);
        m_barFill->setAnchorPoint({0, .5});
        m_barFill->setPosition({
            1.36f,
            m_bar->getScaledContentHeight() / 2.0f,
        });
        m_barFill->setColor(m_barFillColor);
        m_barFill->setZOrder(0);

        m_barFillMaxWidth = m_bar->getScaledContentWidth();
        m_barFillMaxHeight = 20.0f;

        m_percentLabel = Label::create("0%", "bigFont.fnt");
        m_percentLabel->setID("progress-percent-label");
        m_percentLabel->setScale(.5f);
        m_percentLabel->setAnchorPoint({.5, .5});
        m_percentLabel->setPosition(m_bar->getScaledContentSize() / 2.0f);
        m_percentLabel->setAlignment(Label::Alignment::Center);
        m_percentLabel->setVisible(m_showPercentLabel);
        m_percentLabel->setZOrder(1);
    }

    void updateBarLabel() {
        auto percentString = (m_precision > 0) ?
            fmt::format("{}%", numToString(m_progress, m_precision)) :
            fmt::format("{:.0f}%", m_progress);
        m_percentLabel->setText(std::move(percentString));
    }

    void updateBar(float value) {
        if (value > 100.0f) value = 100.0f;
        if (value < .0f) value = .0f;

        m_progress = value;

        float width = m_barFillMaxWidth * (m_progress / 100.0f);
        m_barFill->setTextureRect({.0f, .0f, width, m_barFillMaxHeight});

        if (m_showPercentLabel) this->updateBarLabel();
    }
};

ProgressBar::ProgressBar() : m_impl(std::make_unique<Impl>()) {}
ProgressBar::~ProgressBar() {}

void ProgressBar::reloadStyle() {
    switch (m_impl->m_style) {
        default: [[fallthrough]];

        case Style::Level: m_impl->setupBarStyle("slidergroove2.png", "sliderBar2.png"); break;
        case Style::Slider: m_impl->setupBarStyle("slidergroove.png", "sliderBar.png"); break;
        case Style::Solid: m_impl->setupSolidStyle(); break;
    }

    this->setContentSize(m_impl->m_bar->getContentSize());

    m_impl->m_bar->addChild(m_impl->m_barFill);

    this->addChild(m_impl->m_bar);
    this->addChild(m_impl->m_percentLabel);

    this->updateProgress(m_impl->m_progress);
}

bool ProgressBar::init(float progress, Style style) {
    if (!CCNode::init()) return false;

    m_impl->m_progress = progress;
    m_impl->m_style = style;

    this->setAnchorPoint({.5f, .5f});

    this->reloadStyle();

    return true;
}

void ProgressBar::onAnimate(float dt) {
    m_impl->m_animElapsed += dt;
    float t = std::min(m_impl->m_animElapsed / m_impl->m_animDuration, 1.0f);

    float eased = GameToolbox::getEasedValue(t, static_cast<int>(m_impl->m_animEase), 2.0f);
    m_impl->updateBar(m_impl->m_animProgressFrom + (m_impl->m_animProgressTo - m_impl->m_animProgressFrom) * eased);

    if (t >= 1.0f) {
        m_impl->m_isAnimating = false;

        m_impl->updateBar(m_impl->m_animProgressTo);
        this->unschedule(schedule_selector(ProgressBar::onAnimate));

        if (m_impl->m_animCallback) m_impl->m_animCallback(this, true);
    }
}

void ProgressBar::setStyle(Style style) {
    if (m_impl->m_style != style) {
        m_impl->m_style = style;

        this->removeAllChildren();
        this->reloadStyle(); // recreate nodes with new style
    }
}

void ProgressBar::setFillColor(ccColor3B color) {
    m_impl->m_barFillColor = color;
    m_impl->m_barFill->setColor(color);
}

void ProgressBar::setPrecision(size_t precision) {
    m_impl->m_precision = precision;
    this->updateProgress(m_impl->m_progress); // also updates label with new precision
}

void ProgressBar::updateProgress(float value) {
    this->stopProgressAnimation();
    m_impl->updateBar(value);
}

void ProgressBar::updateProgress(float value, float duration, EasingType ease, AnimationCallback callback) {
    this->stopProgressAnimation();

    value = std::clamp(value, .0f, 100.0f);

    if (duration <= .0f) {
        this->updateProgress(value);
        return;
    }

    m_impl->m_animProgressFrom = m_impl->m_progress;
    m_impl->m_animProgressTo = value;
    m_impl->m_animEase = ease;
    m_impl->m_animDuration = duration;
    m_impl->m_animElapsed = .0f;

    if (callback) m_impl->m_animCallback = std::move(callback);

    this->schedule(schedule_selector(ProgressBar::onAnimate));
    m_impl->m_isAnimating = true;
}

void ProgressBar::stopProgressAnimation() {
    this->unschedule(schedule_selector(ProgressBar::onAnimate));

    m_impl->m_animProgressFrom = m_impl->m_progress;
    m_impl->m_animDuration = .0f;
    m_impl->m_animElapsed = .0f;

    if (m_impl->m_isAnimating && m_impl->m_animCallback) m_impl->m_animCallback(this, false);
    m_impl->m_isAnimating = false;
}

void ProgressBar::showProgressLabel(bool show) {
    m_impl->m_showPercentLabel = show;
    if (show) m_impl->updateBarLabel();

    m_impl->m_percentLabel->setVisible(show);
}

float ProgressBar::getProgress() const noexcept {
    return m_impl->m_progress;
}

Label* ProgressBar::getProgressLabel() const noexcept {
    return m_impl->m_percentLabel;
}

CCSprite* ProgressBar::getBarFill() const noexcept {
    return m_impl->m_barFill;
}

ProgressBar::Style ProgressBar::getStyle() const noexcept {
    return m_impl->m_style;
}

ccColor3B ProgressBar::getFillColor() const noexcept {
    return m_impl->m_barFillColor;
}

size_t ProgressBar::getPrecision() const noexcept {
    return m_impl->m_precision;
}

ProgressBar* ProgressBar::create(Style style) {
    return create(.0f, style);
}

ProgressBar* ProgressBar::create(float progress, Style style) {
    auto ret = new ProgressBar();
    if (ret->init(progress, style)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}