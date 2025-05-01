#include <Geode/ui/extras/GridLayoutSettings.hpp>

using namespace geode::prelude;

bool GridLayoutSettings::setup(GridLayout* layout, CCNode* node) {
	
	m_layout = layout;
	m_node = node;

	this->setTitle("Grid Layout Settings");


	// expand axis

	auto expandAxisColumnSpr = CCSprite::create("GJ_moveBtn.png");
	expandAxisColumnSpr->setRotation(90.f);
	expandAxisColumnSpr->setFlipY(true);
	auto expandAxisRowSpr = CCSprite::create("GJ_moveBtn.png");
	m_expandAxisToggler = CCMenuItemToggler::create(
		expandAxisRowSpr, expandAxisColumnSpr, this, nullptr
	);
	m_expandAxisToggler->toggle(m_layout->getExpandAxis() == Axis::Column);
	m_buttonMenu->addChildAtPosition(m_expandAxisToggler, Anchor::TopRight, {-30.f, -30.f});


	// gaps

	auto gapX = m_layout->getGapX();
	auto gapXSlider = Slider::create(this, menu_selector(GridLayoutSettings::onChangeGap));
	gapXSlider->setValue(gapX / 100.f);
	gapXSlider->setTag(1);
	m_mainLayer->addChildAtPosition(gapXSlider, Anchor::Bottom, {0.f, 110.f});
	auto gapXLabel = CCLabelBMFont::create("Gap X", "bigFont.fnt");
	gapXLabel->setScale(.5f);
	m_mainLayer->addChildAtPosition(gapXLabel, Anchor::Bottom, {-150.f, 110.f});
	m_gapXLabel = CCLabelBMFont::create(fmt::format("{}", gapX).c_str(), "bigFont.fnt");
	m_gapXLabel->setScale(.75f);
	m_gapXLabel->setAnchorPoint({0.f, .5f});
	m_mainLayer->addChildAtPosition(m_gapXLabel, Anchor::Bottom, {120.f, 110.f});
	

	auto gapY = m_layout->getGapY();
	auto gapYSlider = Slider::create(this, menu_selector(GridLayoutSettings::onChangeGap));
	gapYSlider->setValue(gapY / 100.f);
	gapYSlider->setTag(2);
	m_mainLayer->addChildAtPosition(gapYSlider, Anchor::Bottom, {0.f, 75.f});
	auto gapYLabel = CCLabelBMFont::create("Gap Y", "bigFont.fnt");
	gapYLabel->setScale(.5f);
	m_mainLayer->addChildAtPosition(gapYLabel, Anchor::Bottom, {-150.f, 75.f});
	m_gapYLabel = CCLabelBMFont::create(fmt::format("{}", gapY).c_str(), "bigFont.fnt");
	m_gapYLabel->setScale(.75f);
	m_gapYLabel->setAnchorPoint({0.f, .5f});
	m_mainLayer->addChildAtPosition(m_gapYLabel, Anchor::Bottom, {120.f, 75.f});


	/*
		max count
	*/

	// input
	m_maxCountInput = TextInput::create(50.f, "Max count");
	m_maxCountInput->setString(fmt::format("{}", m_layout->getMaxCountCrossExpandAxis()));
	m_maxCountInput->setFilter("0123456789");
	m_maxCountInput->setLabel("Max count");
	// container
	auto maxCountContainer = CCNode::create();
	maxCountContainer->setContentSize(m_maxCountInput->getContentSize());
	maxCountContainer->setScale(2.5f);
	maxCountContainer->setAnchorPoint({.5f, .5f});
	maxCountContainer->addChildAtPosition(m_maxCountInput, Anchor::Center);
	m_mainLayer->addChildAtPosition(maxCountContainer, Anchor::Right, {-130.f, 40.f});
	// menu
	auto maxCountArrowsMenu = CCMenu::create();
	maxCountArrowsMenu->setContentSize(m_maxCountInput->getContentSize());
	maxCountContainer->addChildAtPosition(maxCountArrowsMenu, Anchor::Center);
	// arrow 1
	auto maxCountArrow1Spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	maxCountArrow1Spr->setScale(.5f);
	auto maxCountArrow1 = CCMenuItemSpriteExtra::create(
		maxCountArrow1Spr, this, menu_selector(GridLayoutSettings::onMaxCountArrow)
	);
	maxCountArrow1->setAnchorPoint({1.f, .5f});
	maxCountArrow1->setTag(-1);
	maxCountArrowsMenu->addChildAtPosition(maxCountArrow1, Anchor::Left, {-5.f, 0.f});
	// arrow 2
	auto maxCountArrow2Spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
	maxCountArrow2Spr->setScale(.5f);
	maxCountArrow2Spr->setFlipX(true);
	auto maxCountArrow2 = CCMenuItemSpriteExtra::create(
		maxCountArrow2Spr, this, menu_selector(GridLayoutSettings::onMaxCountArrow)
	);
	maxCountArrow2->setAnchorPoint({0.f, .5f});
	maxCountArrow2->setTag(1);
	maxCountArrowsMenu->addChildAtPosition(maxCountArrow2, Anchor::Right, {5.f, 0.f});
	// info
	auto maxCountInfoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	maxCountInfoSpr->setScale(.375f);
	auto maxCountInfoBtn = CCMenuItemSpriteExtra::create(
		maxCountInfoSpr, this, menu_selector(GridLayoutSettings::onMaxCountInfo)
	);
	maxCountInfoBtn->setAnchorPoint({1.f, 0.f});
	maxCountArrowsMenu->addChildAtPosition(maxCountInfoBtn, Anchor::TopLeft, {-1.f, 1.f});
	

	// auto adjust size

	m_autoAdjustSizeToggler = CCMenuItemToggler::createWithStandardSprites(
		this, nullptr, 1.f
	);
	m_autoAdjustSizeToggler->toggle(m_layout->isAutoAdjustSize());
	m_buttonMenu->addChildAtPosition(m_autoAdjustSizeToggler, Anchor::TopLeft, {40.f, -60.f});
	auto autoAdjustSizeLabel = CCLabelBMFont::create("auto adjust size", "bigFont.fnt");
	autoAdjustSizeLabel->limitLabelWidth(170.f, 1.f, 0.f);
	m_mainLayer->addChildAtPosition(autoAdjustSizeLabel, Anchor::TopLeft, {150.f, -60.f});


	// reverse axes

	m_reverseExpandAxisToggler = CCMenuItemToggler::createWithStandardSprites(
		this, nullptr, 1.f
	);
	m_reverseExpandAxisToggler->toggle(m_layout->isReverseExpandAxis());
	m_buttonMenu->addChildAtPosition(m_reverseExpandAxisToggler, Anchor::TopLeft, {40.f, -100.f});
	auto reverseExpandAxisLabel = CCLabelBMFont::create("reverse expand axis", "bigFont.fnt");
	reverseExpandAxisLabel->limitLabelWidth(170.f, 1.f, 0.f);
	m_mainLayer->addChildAtPosition(reverseExpandAxisLabel, Anchor::TopLeft, {150.f, -100.f});

	m_reverseCrossExpandAxisToggler = CCMenuItemToggler::createWithStandardSprites(
		this, nullptr, 1.f
	);
	m_reverseCrossExpandAxisToggler->toggle(m_layout->isReverseCrossExpandAxis());
	m_buttonMenu->addChildAtPosition(m_reverseCrossExpandAxisToggler, Anchor::TopLeft, {40.f, -140.f});
	auto reverseCrossExpandAxisLabel = CCLabelBMFont::create("reverse cross expand axis", "bigFont.fnt");
	reverseCrossExpandAxisLabel->limitLabelWidth(170.f, 1.f, 0.f);
	m_mainLayer->addChildAtPosition(reverseCrossExpandAxisLabel, Anchor::TopLeft, {150.f, -140.f});


	// apply

	auto applyBtn = CCMenuItemSpriteExtra::create(
		ButtonSprite::create("Apply"),
		this, menu_selector(GridLayoutSettings::onApply)	
	);
	m_buttonMenu->addChildAtPosition(applyBtn, Anchor::Bottom, {0.f, 30.f});

	return true;
}


void GridLayoutSettings::onChangeGap(CCObject* sender) {
	auto slider = static_cast<Slider*>(static_cast<CCNode*>(sender)->getParent()->getParent());
	if (slider->getTag() == 1) m_gapXLabel->setString(fmt::format("{}", slider->getValue() * 100.f).c_str());
	else m_gapYLabel->setString(fmt::format("{}", slider->getValue() * 100.f).c_str());
}

void GridLayoutSettings::onMaxCountArrow(CCObject* sender) {
	m_maxCountInput->setString(fmt::format(
		"{}", 
		stoi(m_maxCountInput->getString()) + sender->getTag()
	));
}

void GridLayoutSettings::onMaxCountInfo(CCObject*) {
	FLAlertLayer::create(
		nullptr, "Max Count Help", 
		"The 'max count' setting lets you specify how many\n"
		"different nodes should fit into one line accross the expand axis\n"
		"in the layout.\n"
		"For example, if the expand axis is Axis::Column, and you set the\n"
		"max count setting to 3, in every horizontal line there will be up to 3 nodes.\n"
		"P.S. I didn't have enough space to make it clear enough without an info button ._.", 
		"OK", "", 500.f
	)->show();
}

void GridLayoutSettings::onApply(CCObject*) {
	(void) m_layout
	->setExpandAxis(m_expandAxisToggler->isToggled() ? Axis::Column : Axis::Row)
	->setGapX(std::stof(m_gapXLabel->getString()))
	->setGapY(std::stof(m_gapYLabel->getString()))
	->setMaxCountCrossExpandAxis(stoi(m_maxCountInput->getString()))
	->autoAdjustSize(m_autoAdjustSizeToggler->isToggled())
	->reverseExpandAxis(m_reverseExpandAxisToggler->isToggled())
	->reverseCrossExpandAxis(m_reverseCrossExpandAxisToggler->isToggled());

	m_node->updateLayout();
}


GridLayoutSettings* GridLayoutSettings::create(GridLayout* layout, CCNode* node) {
	auto ret = new GridLayoutSettings();
	if (ret->initAnchored(500.f, 300.f, layout, node)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}