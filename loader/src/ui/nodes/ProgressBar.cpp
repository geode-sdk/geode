#include <cocos2d.h>
#include <Geode/ui/ProgressBar.hpp>

using namespace geode::prelude;

bool ProgressBar::init() {
    if (!CCNode::init()) return false;

    this->setID("bar"_spr);

    m_progressBar = CCSprite::create("slidergroove2.png");
    m_progressBar->setID("progress-bar");
    m_progressBar->setPosition({ m_progressBar->getScaledContentWidth() / 2.f, m_progressBar->getScaledContentHeight() / 2.f });
    m_progressBar->setAnchorPoint({ 0.5, 0.5 });
    m_progressBar->setZOrder(501);

    this->setScaledContentSize(m_progressBar->getScaledContentSize());

    m_progressBarFill = CCSprite::create("sliderBar2.png");
    m_progressBarFill->setID("bar-fill");
    m_progressBarFill->setAnchorPoint({ 0, 0.5 });
    m_progressBarFill->setPosition({ 2.f, m_progressBar->getScaledContentHeight() / 2.f });
    m_progressBarFill->setColor({ 255, 255, 255 });
    m_progressBarFill->setZOrder(-1);

    m_progressBarFillMaxWidth = m_progressBar->getScaledContentWidth() - 4.f;
    m_progressBarFillMaxHeight = m_progressBarFill->getScaledContentHeight();

    m_progressBar->addChild(m_progressBarFill);

    this->addChild(m_progressBar);
    this->updateProgress(0.0f);

    return true;
};

void ProgressBar::setProgressBarFillColor(ccColor3B color) {
    if (m_progressBarFill) m_progressBarFill->setColor(color);
};

void ProgressBar::updateProgress(float value) {
    if (value > 100.0f) value = 100.0f;
    if (value < 0.0f) value = 0.0f;

    m_progress = value;

    if (m_progressBar && m_progressBarFill) {
        float width = m_progressBarFillMaxWidth * (m_progress / 100.f);
        m_progressBarFill->setTextureRect({ 0.f, 0.f, width, m_progressBarFillMaxHeight });
    };
};

float ProgressBar::getProgress() {
    return m_progress;
};

ProgressBar* ProgressBar::create() {
    auto ret = new ProgressBar();

    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};