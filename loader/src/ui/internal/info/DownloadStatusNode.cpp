#include "ModInfoPopup.hpp"
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SliderTouchLogic.hpp>
#include <Geode/binding/SliderThumb.hpp>

bool DownloadStatusNode::init() {
    if (!CCNode::init()) return false;

    this->setContentSize({ 150.f, 25.f });

    auto bg = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setScale(.33f);
    bg->setColor({ 0, 0, 0 });
    bg->setOpacity(75);
    bg->setContentSize(m_obContentSize * 3);
    this->addChild(bg);

    m_bar = Slider::create(this, nullptr, .6f);
    m_bar->setValue(.0f);
    m_bar->updateBar();
    m_bar->setPosition(0.f, -5.f);
    m_bar->m_touchLogic->m_thumb->setVisible(false);
    this->addChild(m_bar);

    m_label = CCLabelBMFont::create("", "bigFont.fnt");
    m_label->setAnchorPoint({ .0f, .5f });
    m_label->setScale(.45f);
    m_label->setPosition(-m_obContentSize.width / 2 + 15.f, 5.f);
    this->addChild(m_label);

    return true;
}

DownloadStatusNode* DownloadStatusNode::create() {
    auto ret = new DownloadStatusNode();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void DownloadStatusNode::setProgress(uint8_t progress) {
    m_bar->setValue(progress / 100.f);
    m_bar->updateBar();
}

void DownloadStatusNode::setStatus(std::string const& text) {
    m_label->setString(text.c_str());
    m_label->limitLabelWidth(m_obContentSize.width - 30.f, .5f, .1f);
}

