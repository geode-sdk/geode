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
    // Current color of the filled progress bar
    ccColor3B progressBarFillColor = { 31, 162, 255 };
    // Whether to show the label showing the percentage of the current progress
    bool showProgressPercentLabel = false;
    // Precision of the percentage on the progress percent label
    size_t precision = 0;

    // Max width for the progress fill bar node
    float progressBarFillMaxWidth = 0.0f;
    // Max height for the progress fill bar node
    float progressBarFillMaxHeight = 0.0f;
};

ProgressBar::ProgressBar() {
    m_impl = std::make_unique<Impl>();
};

ProgressBar::~ProgressBar() {};

void ProgressBar::reloadStyle() {
    switch (m_impl->style) {
    default: [[fallthrough]];

    case ProgressBarStyle::Level: {
        m_impl->progressBar = CCSprite::create("slidergroove2.png");
        m_impl->progressBar->setID("progress-bar");
        m_impl->progressBar->setAnchorPoint({ 0.5, 0.5 });
        m_impl->progressBar->setPosition({ m_impl->progressBar->getScaledContentWidth() / 2.0f, m_impl->progressBar->getScaledContentHeight() / 2.0f });
        m_impl->progressBar->setZOrder(1);

        m_impl->progressBarFill = CCSprite::create("sliderBar2.png");
        m_impl->progressBarFill->setID("progress-bar-fill");
        m_impl->progressBarFill->setAnchorPoint({ 0, 0.5 });
        m_impl->progressBarFill->setPosition({ 2.0f, m_impl->progressBar->getScaledContentHeight() / 2.0f });
        m_impl->progressBarFill->setColor(m_impl->progressBarFillColor);
        m_impl->progressBarFill->setZOrder(-1);

        m_impl->progressBarFillMaxWidth = m_impl->progressBar->getScaledContentWidth() - 4.0f;
        m_impl->progressBarFillMaxHeight = m_impl->progressBarFill->getScaledContentHeight() - 0.5f;

        m_impl->progressPercentLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
        m_impl->progressPercentLabel->setID("progress-percent-label");
        m_impl->progressPercentLabel->setScale(0.5f);
        m_impl->progressPercentLabel->setAnchorPoint({ 0, 0.5 });
        m_impl->progressPercentLabel->setPosition({ m_impl->progressBar->getScaledContentWidth() + 2.5f, m_impl->progressBar->getScaledContentHeight() / 2.0f });
        m_impl->progressPercentLabel->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
        m_impl->progressPercentLabel->setVisible(m_impl->showProgressPercentLabel);
        m_impl->progressPercentLabel->setZOrder(1);
    } break;

    case ProgressBarStyle::Solid: {
        m_impl->progressBar = CCSprite::create("GJ_progressBar_001.png");
        m_impl->progressBar->setID("progress-bar");
        m_impl->progressBar->setAnchorPoint({ 0.5, 0.5 });
        m_impl->progressBar->setPosition({ m_impl->progressBar->getScaledContentWidth() / 2.0f, m_impl->progressBar->getScaledContentHeight() / 2.0f });
        m_impl->progressBar->setColor({ 0, 0, 0 });
        m_impl->progressBar->setOpacity(125);
        m_impl->progressBar->setZOrder(-1);

        m_impl->progressBarFill = CCSprite::create("GJ_progressBar_001.png");
        m_impl->progressBarFill->setID("progress-bar-fill");
        m_impl->progressBarFill->setScale(0.992f);
        m_impl->progressBarFill->setScaleY(0.86f);
        m_impl->progressBarFill->setAnchorPoint({ 0, 0.5 });
        m_impl->progressBarFill->setPosition({ 1.36f, m_impl->progressBar->getScaledContentHeight() / 2.0f });
        m_impl->progressBarFill->setColor(m_impl->progressBarFillColor);
        m_impl->progressBarFill->setZOrder(0);

        m_impl->progressBarFillMaxWidth = m_impl->progressBar->getScaledContentWidth();
        m_impl->progressBarFillMaxHeight = 20.0f;

        m_impl->progressPercentLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
        m_impl->progressPercentLabel->setID("progress-percent-label");
        m_impl->progressPercentLabel->setScale(0.5f);
        m_impl->progressPercentLabel->setAnchorPoint({ 0.5, 0.5 });
        m_impl->progressPercentLabel->setPosition({ m_impl->progressBar->getScaledContentWidth() / 2.0f, m_impl->progressBar->getScaledContentHeight() / 2.0f });
        m_impl->progressPercentLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        m_impl->progressPercentLabel->setVisible(m_impl->showProgressPercentLabel);
        m_impl->progressPercentLabel->setZOrder(1);
    } break;
    };

    this->setContentSize(m_impl->progressBar->getScaledContentSize());

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
        this->reloadStyle(); // recreate with new style
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