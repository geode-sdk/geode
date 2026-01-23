#include "ModListItem.hpp"

bool ModListItem::init() {
    if (!CCNode::init())
        return false;
        
    m_bg = CCScale9Sprite::create("square02b_small.png");
    m_bg->setID("bg");
    m_bg->setOpacity(0);
    m_bg->ignoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint({ .5f, .5f });
    m_bg->setScale(.7f);
    this->addChildAtPosition(m_bg, Anchor::Center);

    m_viewMenu = CCMenu::create();
    m_viewMenu->setID("view-menu");
    m_viewMenu->setScale(.55f);
    m_viewMenu->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisDirection(AxisDirection::RightToLeft)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(AxisScaling::Scale)
            ->setCrossAxisScaling(AxisScaling::Scale)
            ->setMinRelativeScale(1.f)
            ->setGap(10)
    );
    this->addChildAtPosition(m_viewMenu, Anchor::Right, ccp(-10, 0));

    return true;
}

void ModListItem::updateState() {
    // Update the size of the mod cell itself
    if (m_display == ModListDisplay::Grid) {
        auto widthWithoutGaps = m_targetWidth - 7.5f;
        this->setContentSize(ccp(widthWithoutGaps / roundf(widthWithoutGaps / 80), 100));
        m_bg->setContentSize(m_obContentSize / m_bg->getScale());
    }
    else {
        this->setContentSize(ccp(m_targetWidth, m_display == ModListDisplay::BigList ? 40 : 30));
        m_bg->setContentSize((m_obContentSize - ccp(6, 0)) / m_bg->getScale());
    }
    
    if (m_display == ModListDisplay::Grid) {
        m_viewMenu->setContentWidth(m_obContentSize.width / m_viewMenu->getScaleX());
        m_viewMenu->updateAnchoredPosition(Anchor::Bottom, ccp(0, 5), ccp(.5f, 0));
        m_viewMenu->setScale(.45f);
        static_cast<SimpleRowLayout*>(m_viewMenu->getLayout())->setMainAxisAlignment(MainAxisAlignment::Center);
    }
    else {
        m_viewMenu->setContentWidth(m_obContentSize.width / m_viewMenu->getScaleX() / 2 - 20);
        m_viewMenu->updateAnchoredPosition(Anchor::Right, ccp(-10, 0), ccp(1, .5f));
        m_viewMenu->setScale(.55f);
        static_cast<SimpleRowLayout*>(m_viewMenu->getLayout())->setMainAxisAlignment(MainAxisAlignment::Start);
    }

    m_viewMenu->setContentHeight(40.f);
    m_viewMenu->updateLayout();
}

void ModListItem::updateDisplay(float width, ModListDisplay display) {
    m_display = display;
    m_targetWidth = width;
    this->updateState();
}

