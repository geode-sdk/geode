#include <cocos2d.h>
#include <Geode/ui/ProgressBar.hpp>

using namespace geode::prelude;

class ProgressBar::Impl final {
public:
    Ref<CCSprite> progressBar = nullptr; // Progress bar outline
    CCSprite* progressBarFill = nullptr; // Progress bar fill
    CCLabelBMFont* progressPercentLabel = nullptr; // The text label displaying the percentage

    float progress = 0.0f; // Current progress bar fill percentage ranging from 0 to 100

    ProgressBarStyle style = ProgressBarStyle::Level; // Style of the progress bar

    float progressBarFillMaxWidth = 0.0f; // Max width for the progress fill bar node
    float progressBarFillMaxHeight = 0.0f; // Max height for the progress fill bar node

    bool showProgressPercentLabel = false; // Whether to show the label showing the percentage of the current progress
};

ProgressBar::ProgressBar() {
    m_impl = std::make_unique<Impl>();
};

ProgressBar::~ProgressBar() {};

bool ProgressBar::init() {
    if (!CCNode::init()) return false;

    switch (m_impl->style) {
    case ProgressBarStyle::Level:
        m_impl->progressBar = CCSprite::create("slidergroove2.png");
        m_impl->progressBar->setID("progress-bar");
        m_impl->progressBar->setAnchorPoint({ 0.5, 0.5 });
        m_impl->progressBar->setPosition({ m_impl->progressBar->getScaledContentWidth() / 2.0f, m_impl->progressBar->getScaledContentHeight() / 2.0f });
        m_impl->progressBar->setZOrder(1);

        m_impl->progressBarFill = CCSprite::create("sliderBar2.png");
        m_impl->progressBarFill->setID("progress-bar-fill");
        m_impl->progressBarFill->setAnchorPoint({ 0, 0.5 });
        m_impl->progressBarFill->setPosition({ 2.0f, m_impl->progressBar->getScaledContentHeight() / 2.0f });
        m_impl->progressBarFill->setColor({ 255, 255, 255 });
        m_impl->progressBarFill->setZOrder(-1);

        m_impl->progressBarFillMaxWidth = m_impl->progressBar->getScaledContentWidth() - 4.0f;
        m_impl->progressBarFillMaxHeight = m_impl->progressBarFill->getScaledContentHeight() - 0.5f;

        m_impl->progressPercentLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
        m_impl->progressPercentLabel->setID("progress-percent-label");
        m_impl->progressPercentLabel->setScale(0.5f);
        m_impl->progressPercentLabel->setAnchorPoint({ 0, 0.5 });
        m_impl->progressPercentLabel->setAlignment(CCTextAlignment::kCCTextAlignmentLeft);
        m_impl->progressPercentLabel->setPosition({ m_impl->progressBar->getScaledContentWidth() + 2.5f, m_impl->progressBar->getScaledContentHeight() / 2.0f });
        m_impl->progressPercentLabel->setVisible(m_impl->showProgressPercentLabel);
        m_impl->progressPercentLabel->setZOrder(1);
        break;

    case ProgressBarStyle::Solid:
        m_impl->showProgressPercentLabel = true;

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
        m_impl->progressBarFill->setColor({ 255, 255, 255 });
        m_impl->progressBarFill->setZOrder(0);

        m_impl->progressBarFillMaxWidth = m_impl->progressBar->getScaledContentWidth();
        m_impl->progressBarFillMaxHeight = 20.0f;

        m_impl->progressPercentLabel = CCLabelBMFont::create("0%", "bigFont.fnt");
        m_impl->progressPercentLabel->setID("progress-percent-label");
        m_impl->progressPercentLabel->setScale(0.5f);
        m_impl->progressPercentLabel->setAnchorPoint({ 0.5, 0.5 });
        m_impl->progressPercentLabel->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
        m_impl->progressPercentLabel->setPosition({ m_impl->progressBar->getScaledContentWidth() / 2.0f, m_impl->progressBar->getScaledContentHeight() / 2.0f });
        m_impl->progressPercentLabel->setVisible(m_impl->showProgressPercentLabel);
        m_impl->progressPercentLabel->setZOrder(1);
        break;
    };

    this->setScaledContentSize(m_impl->progressBar->getScaledContentSize());

    m_impl->progressBar->addChild(m_impl->progressBarFill);

    this->addChild(m_impl->progressBar);
    this->updateProgress(m_impl->progress);

    if (m_impl->progressPercentLabel) this->addChild(m_impl->progressPercentLabel);

    return true;
};

void ProgressBar::setStyle(ProgressBarStyle style) {
    if (m_impl->style != style) {
        m_impl->style = style;

        this->removeAllChildren();
        init(); // init again with new style
    };
};

void ProgressBar::setProgressBarFillColor(ccColor3B color) {
    if (m_impl->progressBarFill) m_impl->progressBarFill->setColor(color);
};

void ProgressBar::updateProgress(float value) {
    if (value > 100.0f) value = 100.0f;
    if (value < 0.0f) value = 0.0f;

    m_impl->progress = value;

    if (m_impl->progressBar && m_impl->progressBarFill) {
        float width = m_impl->progressBarFillMaxWidth * (m_impl->progress / 100.0f);
        m_impl->progressBarFill->setTextureRect({ 0.0f, 0.0f, width, m_impl->progressBarFillMaxHeight });
    };

    if (m_impl->progressPercentLabel) {
        auto percentString = fmt::format("{}%", static_cast<int>(m_impl->progress));
        m_impl->progressPercentLabel->setCString(percentString.c_str());
    };
};

float ProgressBar::getProgress() const {
    return m_impl->progress;
};

CCLabelBMFont* ProgressBar::getProgressLabel() const {
    return m_impl->progressPercentLabel;
};

ProgressBarStyle ProgressBar::getProgressBarStyle() const {
    return m_impl->style;
};

void ProgressBar::showProgressLabel(bool show) {
    m_impl->showProgressPercentLabel = show;
    if (m_impl->progressPercentLabel) m_impl->progressPercentLabel->setVisible(show);
};

ProgressBar* ProgressBar::create() {
    auto ret = new ProgressBar();

    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    };

    delete ret;
    return nullptr;
};