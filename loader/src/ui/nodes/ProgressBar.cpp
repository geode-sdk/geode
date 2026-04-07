#include <Geode/ui/ProgressBar.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class ProgressBar::Impl final {
public:
    // Progress bar outline
    Ref<CCSprite> progressBar = nullptr;
    // Progress bar fill
    CCSprite* progressBarFill = nullptr;
    // The text label displaying the percentage
    Ref<CCLabelBMFont> progressPercentLabel = nullptr;

    // Current progress bar fill percentage ranging from 0 to 100
    float progress = 0.0f;

    // Style of the progress bar
    ProgressBarStyle style = ProgressBarStyle::Level;
    // Color of the filled progress bar
    ccColor3B progressBarFillColor = { 255, 255, 255 };
    // Whether to show the label showing the percentage of the current progress
    bool showProgressPercentLabel = false;
    // Precision of the percentage on the progress percent label
    size_t precision = 0;

    // Max width for the progress fill bar node
    float progressBarFillMaxWidth = 0.0f;
    // Max height for the progress fill bar node
    float progressBarFillMaxHeight = 0.0f;

    /**
     * Set up `Level`/`Slider` progress bar style
     *
     * @param barSpr Sprite of the bar
     * @param fillSpr Sprite of the fill
     */
    void setupBarStyle(const char* barSpr, const char* fillSpr) {
        progressBar = CCSprite::create(barSpr);
        progressBar->setID("progress-bar");
        progressBar->setAnchorPoint({ 0.5, 0.5 });
        progressBar->setPosition({ progressBar->getScaledContentWidth() / 2.0f, progressBar->getScaledContentHeight() / 2.0f });
        progressBar->setZOrder(1);

        progressBarFill = CCSprite::create(fillSpr);
        progressBarFill->setID("progress-bar-fill");
        progressBarFill->setAnchorPoint({ 0, 0.5 });
        progressBarFill->setPosition({ 2.0f, progressBar->getScaledContentHeight() / 2.0f });
        progressBarFill->setColor(progressBarFillColor);
        progressBarFill->setZOrder(-1);

        progressBarFillMaxWidth = progressBar->getScaledContentWidth() - 4.0f;
        progressBarFillMaxHeight = progressBarFill->getScaledContentHeight() - 0.5f;

        progressPercentLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
        progressPercentLabel->setID("progress-percent-label");
        progressPercentLabel->setScale(0.5f);
        progressPercentLabel->setAnchorPoint({ 0, 0.5 });
        progressPercentLabel->setPosition({ progressBar->getScaledContentWidth() + 2.5f, progressBar->getScaledContentHeight() / 2.0f });
        progressPercentLabel->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
        progressPercentLabel->setVisible(showProgressPercentLabel);
        progressPercentLabel->setZOrder(1);
    };

    // Set up `Solid` progress bar style
    void setupSolidStyle() {
        progressBar = CCSprite::create("GJ_progressBar_001.png");
        progressBar->setID("progress-bar");
        progressBar->setAnchorPoint({ 0.5, 0.5 });
        progressBar->setPosition({ progressBar->getScaledContentWidth() / 2.0f, progressBar->getScaledContentHeight() / 2.0f });
        progressBar->setColor({ 0, 0, 0 });
        progressBar->setOpacity(125);
        progressBar->setZOrder(-1);

        progressBarFill = CCSprite::create("GJ_progressBar_001.png");
        progressBarFill->setID("progress-bar-fill");
        progressBarFill->setScale(0.992f);
        progressBarFill->setScaleY(0.86f);
        progressBarFill->setAnchorPoint({ 0, 0.5 });
        progressBarFill->setPosition({ 1.36f, progressBar->getScaledContentHeight() / 2.0f });
        progressBarFill->setColor(progressBarFillColor);
        progressBarFill->setZOrder(0);

        progressBarFillMaxWidth = progressBar->getScaledContentWidth();
        progressBarFillMaxHeight = 20.0f;

        progressPercentLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
        progressPercentLabel->setID("progress-percent-label");
        progressPercentLabel->setScale(0.5f);
        progressPercentLabel->setAnchorPoint({ 0.5, 0.5 });
        progressPercentLabel->setPosition({ progressBar->getScaledContentWidth() / 2.0f, progressBar->getScaledContentHeight() / 2.0f });
        progressPercentLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        progressPercentLabel->setVisible(showProgressPercentLabel);
        progressPercentLabel->setZOrder(1);
    };
};

ProgressBar::ProgressBar() : m_impl(std::make_unique<Impl>()) {};

ProgressBar::~ProgressBar() {};

void ProgressBar::reloadStyle() {
    switch (m_impl->style) {
        default: [[fallthrough]];

        case ProgressBarStyle::Level: m_impl->setupBarStyle("slidergroove2.png", "sliderBar2.png"); break;
        case ProgressBarStyle::Slider: m_impl->setupBarStyle("slidergroove.png", "sliderBar.png"); break;
        case ProgressBarStyle::Solid: m_impl->setupSolidStyle(); break;
    };

    this->setContentSize(m_impl->progressBar->getContentSize());

    m_impl->progressBar->addChild(m_impl->progressBarFill);

    this->addChild(m_impl->progressBar);
    this->addChild(m_impl->progressPercentLabel);

    this->updateProgress(m_impl->progress);
};

bool ProgressBar::init(ProgressBarStyle style) {
    if (!CCNode::init()) return false;

    m_impl->style = style;

    this->reloadStyle();

    return true;
};

void ProgressBar::setStyle(ProgressBarStyle style) {
    if (m_impl->style != style) {
        m_impl->style = style;

        this->removeAllChildren();
        this->reloadStyle(); // recreate nodes with new style
    };
};

void ProgressBar::setFillColor(ccColor3B color) {
    m_impl->progressBarFillColor = color;
    if (m_impl->progressBarFill) m_impl->progressBarFill->setColor(color);
};

void ProgressBar::setPrecision(size_t precision) {
    m_impl->precision = precision;
    this->updateProgress(m_impl->progress); // update label with new precision
};

void ProgressBar::updateProgress(float value) {
    if (value > 100.0f) value = 100.0f;
    if (value < 0.0f) value = 0.0f;

    m_impl->progress = value;

    if (m_impl->progressBarFill) {
        float width = m_impl->progressBarFillMaxWidth * (m_impl->progress / 100.0f);
        m_impl->progressBarFill->setTextureRect({ 0.0f, 0.0f, width, m_impl->progressBarFillMaxHeight });
    };

    if (m_impl->progressPercentLabel) {
        auto percentString = fmt::format("{}%", geode::utils::numToString(m_impl->progress, m_impl->precision));
        m_impl->progressPercentLabel->setCString(percentString.c_str());
    };
};

void ProgressBar::showProgressLabel(bool show) {
    m_impl->showProgressPercentLabel = show;
    if (m_impl->progressPercentLabel) m_impl->progressPercentLabel->setVisible(show);
};

float ProgressBar::getProgress() const noexcept {
    return m_impl->progress;
};

CCLabelBMFont* ProgressBar::getProgressLabel() const noexcept {
    return m_impl->progressPercentLabel;
};

ProgressBarStyle ProgressBar::getStyle() const noexcept {
    return m_impl->style;
};

ccColor3B ProgressBar::getFillColor() const noexcept {
    return m_impl->progressBarFillColor;
};

size_t ProgressBar::getPrecision() const noexcept {
    return m_impl->precision;
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