#include <Geode/ui/Label.hpp>
#include <Geode/ui/ProgressBar.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class ProgressBar::Impl final {
public:
    CCSprite* m_progressBar = nullptr;
    CCSprite* m_progressBarFill = nullptr;
    Label* m_progressPercentLabel = nullptr;

    ProgressBarStyle m_style = ProgressBarStyle::Level;

    // Current progress bar fill percentage ranging from 0 to 100
    float m_progress = .0f;

    // Color of the filled progress bar
    ccColor3B m_progressBarFillColor = {255, 255, 255};
    // Whether to show the label showing the percentage of the current progress
    bool m_showProgressPercentLabel = false;
    // Precision of the percentage on the progress percent label
    size_t m_precision = 0;

    float m_progressBarFillMaxWidth = .0f;
    float m_progressBarFillMaxHeight = .0f;

    float m_animDuration = .0f;
    float m_animElapsed = .0f;
    float m_animProgressFrom = .0f;
    float m_animProgressTo = 100.0f;
    EasingType m_animEase = EasingType::None;

    /**
     * Set up `Level`/`Slider` progress bar style
     *
     * @param barSpr Sprite of the bar
     * @param fillSpr Sprite of the fill
     */
    void setupBarStyle(ZStringView barSpr, ZStringView fillSpr) {
        m_progressBar = CCSprite::create(barSpr.c_str());
        m_progressBar->setID("progress-bar");
        m_progressBar->setAnchorPoint({.5, .5});
        m_progressBar->setPosition({
            m_progressBar->getScaledContentWidth() / 2.0f,
            m_progressBar->getScaledContentHeight() / 2.0f,
        });
        m_progressBar->setZOrder(1);

        m_progressBarFill = CCSprite::create(fillSpr.c_str());
        m_progressBarFill->setID("progress-bar-fill");
        m_progressBarFill->setAnchorPoint({0, .5});
        m_progressBarFill->setPosition({2.0f, m_progressBar->getScaledContentHeight() / 2.0f});
        m_progressBarFill->setColor(m_progressBarFillColor);
        m_progressBarFill->setZOrder(-1);

        auto params = ccTexParams{GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        m_progressBarFill->getTexture()->setTexParameters(&params);

        m_progressBarFillMaxWidth = m_progressBar->getScaledContentWidth() - 4.0f;
        m_progressBarFillMaxHeight = m_progressBarFill->getScaledContentHeight() - .5f;

        m_progressPercentLabel = Label::create("0%", "bigFont.fnt");
        m_progressPercentLabel->setID("progress-percent-label");
        m_progressPercentLabel->setScale(.5f);
        m_progressPercentLabel->setAnchorPoint({0, .5});
        m_progressPercentLabel->setPosition({
            m_progressBar->getScaledContentWidth() + 2.5f,
            m_progressBar->getScaledContentHeight() / 2.0f,
        });
        m_progressPercentLabel->setAlignment(Label::Alignment::Left);
        m_progressPercentLabel->setVisible(m_showProgressPercentLabel);
        m_progressPercentLabel->setZOrder(1);
    };

    // Set up `Solid` progress bar style
    void setupSolidStyle() {
        m_progressBar = CCSprite::create("GJ_progressBar_001.png");
        m_progressBar->setID("progress-bar");
        m_progressBar->setAnchorPoint({.5, .5});
        m_progressBar->setPosition({
            m_progressBar->getScaledContentWidth() / 2.0f,
            m_progressBar->getScaledContentHeight() / 2.0f,
        });
        m_progressBar->setColor({0, 0, 0});
        m_progressBar->setOpacity(125);
        m_progressBar->setZOrder(-1);

        m_progressBarFill = CCSprite::create("GJ_progressBar_001.png");
        m_progressBarFill->setID("progress-bar-fill");
        m_progressBarFill->setScale(.992f);
        m_progressBarFill->setScaleY(.86f);
        m_progressBarFill->setAnchorPoint({0, .5});
        m_progressBarFill->setPosition({
            1.36f,
            m_progressBar->getScaledContentHeight() / 2.0f,
        });
        m_progressBarFill->setColor(m_progressBarFillColor);
        m_progressBarFill->setZOrder(0);

        m_progressBarFillMaxWidth = m_progressBar->getScaledContentWidth();
        m_progressBarFillMaxHeight = 20.0f;

        m_progressPercentLabel = Label::create("0%", "bigFont.fnt");
        m_progressPercentLabel->setID("progress-percent-label");
        m_progressPercentLabel->setScale(.5f);
        m_progressPercentLabel->setAnchorPoint({.5, .5});
        m_progressPercentLabel->setPosition({
            m_progressBar->getScaledContentWidth() / 2.0f,
            m_progressBar->getScaledContentHeight() / 2.0f,
        });
        m_progressPercentLabel->setAlignment(Label::Alignment::Center);
        m_progressPercentLabel->setVisible(m_showProgressPercentLabel);
        m_progressPercentLabel->setZOrder(1);
    };

    void updateBar(float value) {
        if (value > 100.0f) value = 100.0f;
        if (value < .0f) value = .0f;

        m_progress = value;

        float width = m_progressBarFillMaxWidth * (m_progress / 100.0f);
        m_progressBarFill->setTextureRect({.0f, .0f, width, m_progressBarFillMaxHeight});

        if (m_showProgressPercentLabel) {
            auto percentString = (m_precision > 0) ?
                fmt::format("{}%", numToString(value, m_precision)) :
                fmt::format("{:.0f}%", value);
            m_progressPercentLabel->setText(std::move(percentString));
        };
    };
};

ProgressBar::ProgressBar() : m_impl(std::make_unique<Impl>()) {};
ProgressBar::~ProgressBar() {};

void ProgressBar::reloadStyle() {
    switch (m_impl->m_style) {
        default: [[fallthrough]];

        case ProgressBarStyle::Level:
            m_impl->setupBarStyle("slidergroove2.png", "sliderBar2.png");
            break;

        case ProgressBarStyle::Slider:
            m_impl->setupBarStyle("slidergroove.png", "sliderBar.png");
            break;

        case ProgressBarStyle::Solid: m_impl->setupSolidStyle(); break;
    };

    this->setContentSize(m_impl->m_progressBar->getContentSize());

    m_impl->m_progressBar->addChild(m_impl->m_progressBarFill);

    this->addChild(m_impl->m_progressBar);
    this->addChild(m_impl->m_progressPercentLabel);

    this->updateProgress(m_impl->m_progress);
};

bool ProgressBar::init(ProgressBarStyle style) {
    if (!CCNode::init()) return false;

    m_impl->m_style = style;

    this->setAnchorPoint({.5f, .5f});

    this->reloadStyle();

    return true;
};

void ProgressBar::onAnimate(float dt) {
    m_impl->m_animElapsed += dt;
    float t = std::min(m_impl->m_animElapsed / m_impl->m_animDuration, 1.0f);

    float eased = GameToolbox::getEasedValue(t, static_cast<int>(m_impl->m_animEase), 2.0f);
    m_impl->updateBar(
        m_impl->m_animProgressFrom + (m_impl->m_animProgressTo - m_impl->m_animProgressFrom) * eased
    );

    if (t >= 1.0f) {
        m_impl->updateBar(m_impl->m_animProgressTo);
        this->unschedule(schedule_selector(ProgressBar::onAnimate));
    };
};

void ProgressBar::setStyle(ProgressBarStyle style) {
    if (m_impl->m_style != style) {
        m_impl->m_style = style;

        this->removeAllChildren();
        this->reloadStyle(); // recreate nodes with new style
    };
};

void ProgressBar::setFillColor(ccColor3B color) {
    m_impl->m_progressBarFillColor = color;
    m_impl->m_progressBarFill->setColor(color);
};

void ProgressBar::setPrecision(size_t precision) {
    m_impl->m_precision = precision;
    this->updateProgress(m_impl->m_progress); // update label with new precision
};

void ProgressBar::updateProgress(float value) {
    this->stopProgressAnimation();
    m_impl->updateBar(value);
};

void ProgressBar::updateProgress(float value, float duration, EasingType ease) {
    this->stopProgressAnimation();

    value = std::clamp(value, .0f, 100.0f);

    if (duration <= .0f) {
        this->updateProgress(value);
        return;
    };

    m_impl->m_animProgressFrom = m_impl->m_progress;
    m_impl->m_animProgressTo = value;
    m_impl->m_animEase = ease;
    m_impl->m_animDuration = duration;
    m_impl->m_animElapsed = .0f;

    this->schedule(schedule_selector(ProgressBar::onAnimate));
};

void ProgressBar::stopProgressAnimation() {
    this->unschedule(schedule_selector(ProgressBar::onAnimate));

    m_impl->m_animProgressFrom = m_impl->m_progress;
    m_impl->m_animDuration = .0f;
    m_impl->m_animElapsed = .0f;
};

void ProgressBar::showProgressLabel(bool show) {
    m_impl->m_showProgressPercentLabel = show;
    m_impl->m_progressPercentLabel->setVisible(show);
};

float ProgressBar::getProgress() const noexcept {
    return m_impl->m_progress;
};

Label* ProgressBar::getProgressLabel() const noexcept {
    return m_impl->m_progressPercentLabel;
};

CCSprite* ProgressBar::getBarFill() const noexcept {
    return m_impl->m_progressBarFill;
};

ProgressBarStyle ProgressBar::getStyle() const noexcept {
    return m_impl->m_style;
};

ccColor3B ProgressBar::getFillColor() const noexcept {
    return m_impl->m_progressBarFillColor;
};

size_t ProgressBar::getPrecision() const noexcept {
    return m_impl->m_precision;
};

ProgressBar* ProgressBar::create(ProgressBarStyle style) {
    auto ret = new ProgressBar();
    if (ret->init(style)) {
        ret->autorelease();
        return ret;
    };

    delete ret;
    return nullptr;
};