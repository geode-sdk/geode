#include "ModListItem.hpp"

bool ModListItem::init() {
    if (!CCNode::init()) return false;
        
    m_bg = NineSlice::create("square02b_small.png");
    m_bg->setID("background");
    m_bg->setOpacity(0);
    m_bg->ignoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint({ 0.5f, 0.5f });
    m_bg->setScaleMultiplier(0.7f);
    this->addChildAtPosition(m_bg, Anchor::Center);

    m_viewMenu = CCNode::create();
    m_viewMenu->setID("view-menu");
    m_viewMenu->setScale(0.55f);
    m_viewMenu->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisDirection(AxisDirection::RightToLeft)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(AxisScaling::Scale)
            ->setCrossAxisScaling(AxisScaling::Scale)
            ->setMinRelativeScale(1.0f)
            ->setGap(10.0f)
    );
    this->addChildAtPosition(m_viewMenu, Anchor::Right, {-10.0f, 0.0f});

    return true;
}

void ModListItem::updateState() {
    // Update the size of the mod cell itself
    if (m_display == ModListDisplay::Grid) {
        auto widthWithoutGaps = m_targetWidth - 7.5f;
        this->setContentSize({widthWithoutGaps / roundf(widthWithoutGaps / 80.0f), 100.0f});
        m_bg->setContentSize(this->getContentSize());
    }
    else {
        this->setContentSize({m_targetWidth, m_display == ModListDisplay::BigList ? 40.0f : 30.0f});
        m_bg->setContentSize((this->getContentSize() - CCSize{6.0f, 0.0f}));
    }

    auto const& size = this->getContentSize();

    if (m_display == ModListDisplay::Grid) {
        m_viewMenu->setContentWidth(size.width / m_viewMenu->getScaleX());
        m_viewMenu->updateAnchoredPosition(Anchor::Bottom, {0.0f, 5.0f}, {0.5f, 0.0f});
        m_viewMenu->setScale(0.45f);
        static_cast<SimpleRowLayout*>(m_viewMenu->getLayout())->setMainAxisAlignment(MainAxisAlignment::Center);
    }
    else {
        m_viewMenu->setContentWidth(size.width / m_viewMenu->getScaleX() / 2.0f - 20.0f);
        m_viewMenu->updateAnchoredPosition(Anchor::Right, {-10.0f, 0.0f}, {1.0f, 0.5f});
        m_viewMenu->setScale(0.55f);
        static_cast<SimpleRowLayout*>(m_viewMenu->getLayout())->setMainAxisAlignment(MainAxisAlignment::Start);
    }

    m_viewMenu->setContentHeight(40.0f);
    m_viewMenu->updateLayout();
    
    this->updateLayout();
}

void ModListItem::updateDisplay(float width, ModListDisplay display) {
    m_display = display;
    m_targetWidth = width;
    this->updateState();
}
