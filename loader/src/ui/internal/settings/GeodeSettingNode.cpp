/*#pragma warning(disable: 4067)

#include "GeodeSettingNode.hpp"
#include <general/TextRenderer.hpp>
#include <random>
#include <utils/WackyGeodeMacros.hpp>

USE_GEODE_NAMESPACE();

#define GEODE_GENERATE_SETTING_CREATE(_sett_, _height_)                \
	_sett_##Node* _sett_##Node::create(_sett_* setting, float width) { \
		auto ret = new _sett_##Node(width, _height_);                  \
		if (ret && ret->init(setting)) {                               \
			ret->autorelease();                                        \
			return ret;                                                \
		} CC_SAFE_DELETE(ret); return nullptr; }
	
template <typename T>
std::string toStringWithPrecision(const T a_value, const size_t n = 6, bool cutZeros = true) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    auto str = out.str();
	while (
		cutZeros &&
		string_utils::contains(str, '.') &&
		(str.back() == '0' || str.back() == '.')
	) {
		str = str.substr(0, str.size() - 1);
	}
	return str;
}

// bool

bool BoolSettingNode::init(BoolSetting* setting) {
	if (!GeodeSettingNode<BoolSetting>::init(setting))
		return false;

	m_toggle = CCMenuItemToggler::createWithStandardSprites(
		this, menu_selector(BoolSettingNode::onToggle), .65f
	);
	m_toggle->setPosition(-m_toggle->m_onButton->getScaledContentSize().width / 2, 0);
	m_toggle->toggle(setting->getValue());
	m_buttonMenu->addChild(m_toggle);

	return true;
}

void BoolSettingNode::onToggle(CCObject* pSender) {
	m_value = !m_toggle->isToggled();
	this->updateSettingsList();
}

void BoolSettingNode::updateState() {
	m_toggle->toggle(m_value);
}

// int

bool IntSettingNode::init(IntSetting* setting) {
	if (!GeodeSettingNode<IntSetting>::init(setting))
		return false;

	auto controls = CCArray::create();
	CCScale9Sprite* bgSprite = nullptr;
	if (setting->hasInput()) {
		bgSprite = CCScale9Sprite::create(
			"square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
		);
		bgSprite->setScale(.25f);
		bgSprite->setColor({ 0, 0, 0 });
		bgSprite->setOpacity(75);
		bgSprite->setContentSize({ 45.f * 4, m_height * 3 });
		bgSprite->setPosition(-20, 0);
		m_buttonMenu->addChild(bgSprite);
		controls->addObject(bgSprite);
	}

	m_valueInput = MenuInputNode::create(45.f, m_height, "Num", "bigFont.fnt");
	m_valueInput->setPosition(-20.f, .0f);
	m_valueInput->getInput()->setAllowedChars("0123456789+-. ");
	m_valueInput->getInput()->setMaxLabelScale(.5f);
    m_valueInput->getInput()->setLabelPlaceholderColor({ 150, 150, 150 });
    m_valueInput->getInput()->setLabelPlaceholderScale(.75f);
	m_valueInput->getInput()->setDelegate(this);
	m_valueInput->setEnabled(setting->hasInput());
	m_buttonMenu->addChild(m_valueInput);
	controls->addObject(m_valueInput);

	if (setting->hasArrows()) {
		m_valueInput->setPositionX(-30.f);
		if (bgSprite) bgSprite->setPositionX(-30.f);

		auto decSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
		decSpr->setScale(.3f);
		decSpr->setFlipX(true);

		auto decBtn = CCMenuItemSpriteExtra::create(
			decSpr, this, menu_selector(IntSettingNode::onArrow)
		);
		decBtn->setTag(-1);
		decBtn->setPosition(-60.f, 0);
		m_buttonMenu->addChild(decBtn);
		controls->addObject(decBtn);

		auto incSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
		incSpr->setScale(.3f);

		auto incBtn = CCMenuItemSpriteExtra::create(
			incSpr, this, menu_selector(IntSettingNode::onArrow)
		);
		incBtn->setTag(1);
		incBtn->setPosition(0.f, 0);
		m_buttonMenu->addChild(incBtn);
		controls->addObject(incBtn);
	}

	if (setting->hasSlider()) {
		m_height += 20.f;
		this->setContentSize({ m_width, m_height });
		m_backgroundLayer->setContentSize(this->getContentSize());
		
		m_nameLabel->setPositionY(m_nameLabel->getPositionY() + 10.f);
		if (m_descButton) {
			m_descButton->setPositionY(m_descButton->getPositionY() - 8.f);
		}
		m_buttonMenu->setPositionY(m_buttonMenu->getPositionY() + 18.f);

		CCARRAY_FOREACH_B_TYPE(controls, node, CCNode) {
			node->setPositionX(node->getPositionX() - (setting->hasArrows() ? 35.f : 45.f));
		}

		m_slider = Slider::create(this, menu_selector(IntSettingNode::onSlider), .65f);
		m_slider->setPosition(-65.f, -22.f);
		m_slider->setValue(
			// normalized to 0-1
			(m_value - m_setting->getMin()) /
			(m_setting->getMax() - m_setting->getMin())
		);
		m_buttonMenu->addChild(m_slider);
	}

	this->updateValue();

	return true;
}

void IntSettingNode::textChanged(CCTextInputNode* input) {
	try {
		m_value = std::stoi(input->getString());
	} catch(...) {}
	this->updateValue(false);
}

void IntSettingNode::textInputClosed(CCTextInputNode* input) {
	try {
		m_value = std::stoi(input->getString());
	} catch(...) {}
	this->updateValue();
}

void IntSettingNode::onSlider(CCObject* pSender) {
	m_value = 
		as<SliderThumb*>(pSender)->getValue() *
		(m_setting->getMax() - m_setting->getMin()) + 
		m_setting->getMin();
	m_valueInput->getInput()->detachWithIME();
	this->updateValue();
}

void IntSettingNode::onArrow(CCObject* pSender) {
	m_value = m_value + pSender->getTag() * m_setting->getStep();
	m_valueInput->getInput()->detachWithIME();
	this->updateValue();
}

void IntSettingNode::updateValue(bool updateInput) {
	if (m_value < m_setting->getMin()) {
		m_value = m_setting->getMin();
	}
	if (m_value > m_setting->getMax()) {
		m_value = m_setting->getMax();
	}
	if (updateInput) {
		m_valueInput->getInput()->setString(
			std::to_string(m_value).c_str()
		);
	}
	if (m_slider) {
		m_slider->setValue(
			static_cast<float>(m_value - m_setting->getMin()) /
			(m_setting->getMax() - m_setting->getMin())
		);
		m_slider->updateBar();
	}
	this->updateSettingsList();
}

void IntSettingNode::updateState() {
	this->updateValue();
}

// float

bool FloatSettingNode::init(FloatSetting* setting) {
	if (!GeodeSettingNode<FloatSetting>::init(setting))
		return false;
	
	auto controls = CCArray::create();
	CCScale9Sprite* bgSprite = nullptr;
	if (setting->hasInput()) {
		bgSprite = CCScale9Sprite::create(
			"square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
		);
		bgSprite->setScale(.25f);
		bgSprite->setColor({ 0, 0, 0 });
		bgSprite->setOpacity(75);
		bgSprite->setContentSize({ 45.f * 4, m_height * 3 });
		bgSprite->setPosition(-20, 0);
		m_buttonMenu->addChild(bgSprite);
		controls->addObject(bgSprite);
	}

	m_valueInput = MenuInputNode::create(45.f, m_height, "Num", "bigFont.fnt");
	m_valueInput->setPosition(-20.f, .0f);
	m_valueInput->getInput()->setAllowedChars("0123456789+-. ");
	m_valueInput->getInput()->setMaxLabelScale(.5f);
    m_valueInput->getInput()->setLabelPlaceholderColor({ 150, 150, 150 });
    m_valueInput->getInput()->setLabelPlaceholderScale(.5f);
	m_valueInput->getInput()->setDelegate(this);
	m_valueInput->setEnabled(setting->hasInput());
	m_buttonMenu->addChild(m_valueInput);
	controls->addObject(m_valueInput);

	CCMenuItemSpriteExtra* decBtn = nullptr;
	CCMenuItemSpriteExtra* incBtn = nullptr;
	if (setting->hasArrows()) {
		m_valueInput->setPositionX(-30.f);
		if (bgSprite) bgSprite->setPositionX(-30.f);

		auto decSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
		decSpr->setScale(.3f);
		decSpr->setFlipX(true);

		decBtn = CCMenuItemSpriteExtra::create(
			decSpr, this, menu_selector(FloatSettingNode::onArrow)
		);
		decBtn->setTag(-1);
		decBtn->setPosition(-60.f, 0);
		m_buttonMenu->addChild(decBtn);
		controls->addObject(decBtn);

		auto incSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
		incSpr->setScale(.3f);

		incBtn = CCMenuItemSpriteExtra::create(
			incSpr, this, menu_selector(FloatSettingNode::onArrow)
		);
		incBtn->setTag(1);
		incBtn->setPosition(0.f, 0);
		m_buttonMenu->addChild(incBtn);
		controls->addObject(incBtn);
	}

	if (setting->hasSlider()) {
		m_height += 20.f;
		this->setContentSize({ m_width, m_height });
		m_backgroundLayer->setContentSize(this->getContentSize());
		
		m_nameLabel->setPositionY(m_nameLabel->getPositionY() + 10.f);
		if (m_descButton) {
			m_descButton->setPositionY(m_descButton->getPositionY() - 8.f);
		}
		m_buttonMenu->setPositionY(m_buttonMenu->getPositionY() + 18.f);

		CCARRAY_FOREACH_B_TYPE(controls, node, CCNode) {
			node->setPositionX(node->getPositionX() - (setting->hasArrows() ? 35.f : 45.f));
		}

		m_slider = Slider::create(this, menu_selector(FloatSettingNode::onSlider), .65f);
		m_slider->setPosition(-65.f, -22.f);
		m_slider->setValue(
			// normalized to 0-1
			(m_value - m_setting->getMin()) /
			(m_setting->getMax() - m_setting->getMin())
		);
		m_buttonMenu->addChild(m_slider);
	}

	this->updateValue();

	return true;
}

void FloatSettingNode::textChanged(CCTextInputNode* input) {
	try {
		m_value = std::stof(input->getString());
	} catch(...) {}
	this->updateValue(false);
}

void FloatSettingNode::textInputClosed(CCTextInputNode* input) {
	try {
		m_value = std::stof(input->getString());
	} catch(...) {}
	this->updateValue();
}

void FloatSettingNode::onSlider(CCObject* pSender) {
	m_value = 
		as<SliderThumb*>(pSender)->getValue() *
		(m_setting->getMax() - m_setting->getMin()) + 
		m_setting->getMin();
	m_valueInput->getInput()->detachWithIME();
	this->updateValue();
}

void FloatSettingNode::onArrow(CCObject* pSender) {
	m_value = m_value + pSender->getTag() * m_setting->getStep();
	this->updateValue();
}

void FloatSettingNode::updateValue(bool updateInput) {
	if (m_value < m_setting->getMin()) {
		m_value = m_setting->getMin();
	}
	if (m_value > m_setting->getMax()) {
		m_value = m_setting->getMax();
	}
	if (updateInput) {
		m_valueInput->getInput()->setString(toStringWithPrecision(
			m_value, m_setting->getPrecision()
		).c_str());
	}
	if (m_slider) {
		m_slider->setValue(
			// normalized to 0-1
			(m_value - m_setting->getMin()) /
			(m_setting->getMax() - m_setting->getMin())
		);
		m_slider->updateBar();
	}
	this->updateSettingsList();
}

void FloatSettingNode::updateState() {
	this->updateValue();
}

// string

bool StringSettingNode::init(StringSetting* setting) {
	if (!GeodeSettingNode<StringSetting>::init(setting))
		return false;
	
	auto bgSprite = CCScale9Sprite::create(
		"square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
	);
	bgSprite->setScale(.25f);
	bgSprite->setColor({ 0, 0, 0 });
	bgSprite->setOpacity(75);
	bgSprite->setContentSize({ (m_width / 2 - 30.f) * 4, m_height * 3 });
	bgSprite->setPosition(-m_width / 4 + 18.f, 0);
	m_buttonMenu->addChild(bgSprite);

	m_input = MenuInputNode::create(
		m_width / 2 - 30.f, m_height,
		"...", "bigFont.fnt"
	);
	m_input->setPositionX(-m_width / 4 + 18.f);
	m_input->getInput()->setAllowedChars(setting->getFilter());
	m_input->getInput()->setMaxLabelScale(.5f);
    m_input->getInput()->setLabelPlaceholderColor({ 150, 150, 150 });
    m_input->getInput()->setLabelPlaceholderScale(.5f);
	m_input->getInput()->setDelegate(this);
	m_buttonMenu->addChild(m_input);

	m_input->getInput()->setString(m_value.c_str());

	return true;
}

void StringSettingNode::textChanged(CCTextInputNode* input) {
	m_value = input->getString();
	this->updateSettingsList();
}

void StringSettingNode::updateState() {
	m_input->getInput()->setString(m_value);
}

// color

bool ColorSettingNode::init(ColorSetting* setting) {
	if (!GeodeSettingNode<ColorSetting>::init(setting))
		return false;

	m_colorSprite = ColorChannelSprite::create();
	m_colorSprite->setColor(setting->getValue());
	m_colorSprite->setScale(.65f);
	
	auto button = CCMenuItemSpriteExtra::create(
		m_colorSprite, this, menu_selector(ColorSettingNode::onPickColor)
	);
	button->setPosition({ -m_colorSprite->getScaledContentSize().width / 2, 0 });
	m_buttonMenu->addChild(button);

	return true;
}

void ColorSettingNode::onPickColor(CCObject*) {
	ColorPickPopup::create(this)->show();
}

void ColorSettingNode::updateState() {
	m_colorSprite->setColor(m_value);
}

// rgba

bool ColorAlphaSettingNode::init(ColorAlphaSetting* setting) {
	if (!GeodeSettingNode<ColorAlphaSetting>::init(setting))
		return false;

	m_colorSprite = ColorChannelSprite::create();
	m_colorSprite->setColor(to3B(setting->getValue()));
	m_colorSprite->updateOpacity(setting->getValue().a / 255.f);
	m_colorSprite->setScale(.65f);
	
	auto button = CCMenuItemSpriteExtra::create(
		m_colorSprite, this, menu_selector(ColorAlphaSettingNode::onPickColor)
	);
	button->setPosition({ -m_colorSprite->getScaledContentSize().width / 2, 0 });
	m_buttonMenu->addChild(button);

	return true;
}

void ColorAlphaSettingNode::onPickColor(CCObject*) {
	ColorPickPopup::create(this)->show();
}

void ColorAlphaSettingNode::updateState() {
	m_colorSprite->setColor(to3B(m_value));
}

// path

bool PathSettingNode::init(PathSetting* setting) {
	if (!GeodeSettingNode<PathSetting>::init(setting))
		return false;

	auto bgSprite = CCScale9Sprite::create(
		"square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
	);
	bgSprite->setScale(.25f);
	bgSprite->setColor({ 0, 0, 0 });
	bgSprite->setOpacity(75);
	bgSprite->setContentSize({ (m_width / 2 - 50.f) * 4, m_height * 3 });
	bgSprite->setPosition(-m_width / 4, 0);
	m_buttonMenu->addChild(bgSprite);

	m_input = MenuInputNode::create(
		m_width / 2 - 50.f, m_height,
		"...", "chatFont.fnt"
	);
	m_input->setPositionX(-m_width / 4);
	m_input->getInput()->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,/\\_-.+%#@!';");
	m_input->getInput()->setMaxLabelScale(.7f);
    m_input->getInput()->setLabelPlaceholderColor({ 150, 150, 150 });
    m_input->getInput()->setLabelPlaceholderScale(.7f);
	m_input->getInput()->setDelegate(this);
	m_buttonMenu->addChild(m_input);

	auto folder = CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png");
	folder->setScale(.65f);
	auto button = CCMenuItemSpriteExtra::create(
		folder, this, menu_selector(PathSettingNode::onSelectFile)
	);
	button->setPosition({ -folder->getScaledContentSize().width / 2, 0 });
	m_buttonMenu->addChild(button);
	
	m_input->getInput()->setString(m_value.string().c_str());

	return true;
}

void PathSettingNode::onSelectFile(CCObject*) {
	FLAlertLayer::create("todo", "implement file picker", "OK")->show();
	this->updateSettingsList();
}

void PathSettingNode::textChanged(CCTextInputNode* input) {
	m_value = input->getString();
	this->updateSettingsList();
}

void PathSettingNode::updateState() {
	m_input->getInput()->setString(m_value.string());
}

// string[]

bool StringSelectSettingNode::init(StringSelectSetting* setting) {
	if (!GeodeSettingNode<StringSelectSetting>::init(setting))
		return false;

	m_selectedLabel = CCLabelBMFont::create(setting->getValue().c_str(), "bigFont.fnt");
	m_selectedLabel->setPosition(-m_width / 4 + 20.f, .0f);
	m_selectedLabel->limitLabelWidth(m_width / 2 - 60.f, .5f, .1f);
	m_buttonMenu->addChild(m_selectedLabel);

	auto decSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
	decSpr->setScale(.3f);
	decSpr->setFlipX(true);

	auto decBtn = CCMenuItemSpriteExtra::create(
		decSpr, this, menu_selector(StringSelectSettingNode::onChange)
	);
	decBtn->setTag(-1);
	decBtn->setPosition(-m_width / 2 + 40.f, 0);
	m_buttonMenu->addChild(decBtn);

	auto incSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
	incSpr->setScale(.3f);

	auto incBtn = CCMenuItemSpriteExtra::create(
		incSpr, this, menu_selector(StringSelectSettingNode::onChange)
	);
	incBtn->setTag(1);
	incBtn->setPosition(0.f, 0);
	m_buttonMenu->addChild(incBtn);

	return true;
}

void StringSelectSettingNode::onChange(CCObject* pSender) {
	if (pSender) {
		long newValue = m_value + pSender->getTag();
		if (newValue < 0) m_value = m_setting->getOptions().size() - 1;
		else if (newValue > static_cast<long>(m_setting->getOptions().size() - 1)) m_value = 0;
		else m_value = newValue;
	}

	m_selectedLabel->setString(m_setting->getValueAt(m_value).c_str());
	m_selectedLabel->limitLabelWidth(m_width / 2 - 60.f, .5f, .1f);
	this->updateSettingsList();
}

void StringSelectSettingNode::updateState() {
	this->onChange(nullptr);
}

// custom

bool CustomSettingPlaceHolderNode::init(CustomSettingPlaceHolder* setting, bool isLoaded) {
	if (!CCNode::init())
		return false;

	auto pad = m_height;
	this->setContentSize({ m_width, m_height });
	m_backgroundLayer->setContentSize(this->getContentSize());

	// i'm using TextRenderer instead of TextArea because 
	// i couldn't get TextArea to work for some reason 
	// and TextRenderer should be fast enough for short 
	// static text

	auto render = TextRenderer::create();

	render->begin(this, CCPointZero, { m_width - pad * 2, m_height });

	render->pushFont(
		[](int) -> TextRenderer::Label {
			return CCLabelBMFont::create("", "chatFont.fnt");
		}
	);
	render->pushHorizontalAlign(TextAlignment::Begin);
	render->pushVerticalAlign(TextAlignment::Begin);
	render->pushScale(.7f);
	auto rendered = render->renderString(
		isLoaded ?
			"This setting (id: " + setting->getKey() + ") is a "
			"custom setting which has no registered setting node. "
			"This is likely a bug in the mod; report it to the "
			"developer." :
			"This setting (id: " + setting->getKey() + ") is a " 
			"custom setting, which means that you need to enable "
			"& load the mod to change its value."
	);

	render->end(true, TextAlignment::Center, TextAlignment::Center);
	render->release();

	m_height = this->getContentSize().height + pad / 4;
	m_width = this->getContentSize().width;
	m_width += pad * 2;
	this->setContentSize({ m_width, m_height });
	m_backgroundLayer->setContentSize(this->getContentSize());

	for (auto& label : rendered) {
		label.m_node->setPositionX(pad * 1.5f);
		label.m_node->setPositionY(label.m_node->getPositionY() + pad / 8);
	}

    auto bgSprite = CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    bgSprite->setScale(.5f);
    bgSprite->setColor({ 0, 0, 0 });
    bgSprite->setOpacity(75);
    bgSprite->setZOrder(-1);
    bgSprite->setContentSize(m_obContentSize * 2 - CCSize { pad, 0 });
    bgSprite->setPosition(m_obContentSize / 2);
    this->addChild(bgSprite);

	auto iconSprite = CCSprite::createWithSpriteFrameName(
		isLoaded ? "info-warning.png"_spr : "GJ_infoIcon_001.png"
	);
	iconSprite->setPosition({ pad * .9f, m_height / 2 });
	iconSprite->setScale(.8f);
	this->addChild(iconSprite);

	return true;
}

CustomSettingPlaceHolderNode* CustomSettingPlaceHolderNode::create(CustomSettingPlaceHolder* setting, bool isLoaded, float width) {
	auto ret = new CustomSettingPlaceHolderNode(width, 30.f);
	if (ret && ret->init(setting, isLoaded)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void ColorPickPopup::setup(
	ColorSettingNode* colorNode,
	ColorAlphaSettingNode* alphaNode
) {
	m_noElasticity = true;
	
	m_colorNode = colorNode;
	m_colorAlphaNode = alphaNode;

	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto picker = CCControlColourPicker::colourPicker();
	picker->setColorValue(
		colorNode ?
			colorNode->m_value :
			to3B(alphaNode->m_value)
	);
	picker->setColorTarget(colorNode ? 
		colorNode->m_colorSprite :
		alphaNode->m_colorSprite
	);
	picker->setPosition(winSize.width / 2, winSize.height / 2 + (colorNode ? 0.f : 20.f));
	picker->setDelegate(this);
	m_mainLayer->addChild(picker);

	if (alphaNode) {
		m_alphaSlider = Slider::create(this, menu_selector(ColorPickPopup::onSlider), .75f);
		m_alphaSlider->setPosition(winSize.width / 2, winSize.height / 2 - 90.f);
		m_alphaSlider->setValue(alphaNode->m_value.a / 255.f);
		m_alphaSlider->updateBar();
		m_mainLayer->addChild(m_alphaSlider);

		auto bgSprite = CCScale9Sprite::create(
			"square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
		);
		bgSprite->setScale(.25f);
		bgSprite->setColor({ 0, 0, 0 });
		bgSprite->setOpacity(75);
		bgSprite->setContentSize({ 200.f, 100.f });
		bgSprite->setPosition(winSize.width / 2 + 115.f, winSize.height / 2 - 90.f);
		m_mainLayer->addChild(bgSprite);

		m_alphaInput = MenuInputNode::create(50.f, 25.f, "...", "bigFont.fnt");
		m_alphaInput->setPosition(winSize.width / 2 + 115.f, winSize.height / 2 - 90.f);
		m_alphaInput->getInput()->setDelegate(this);
		m_alphaInput->getInput()->setAllowedChars("0123456789. ");
		m_alphaInput->getInput()->setString(
			toStringWithPrecision(alphaNode->m_value.a / 255.f, 2, false)
		);
		m_alphaInput->setScale(.8f);
		m_mainLayer->addChild(m_alphaInput);
	}
}

void ColorPickPopup::onSlider(CCObject* pSender) {
	if (m_colorAlphaNode) {
		m_colorAlphaNode->m_value = to4B(
			to3B(m_colorAlphaNode->m_value),
			static_cast<GLubyte>(as<SliderThumb*>(pSender)->getValue() * 255.f)
		);
		m_alphaInput->getInput()->setString(
			toStringWithPrecision(as<SliderThumb*>(pSender)->getValue(), 2, false)
		);
		m_colorAlphaNode->m_colorSprite->updateOpacity(
			m_colorAlphaNode->m_value.a / 255.f
		);
		m_colorAlphaNode->updateSettingsList();
	}
}

void ColorPickPopup::textChanged(CCTextInputNode* input) {
	try {
		m_colorAlphaNode->m_value = to4B(
			to3B(m_colorAlphaNode->m_value),
			static_cast<GLubyte>(std::stof(input->getString()) * 255.f)
		);
		m_colorAlphaNode->m_colorSprite->updateOpacity(
			m_colorAlphaNode->m_value.a / 255.f
		);
		m_alphaSlider->setValue(std::stof(input->getString()));
		m_colorAlphaNode->updateSettingsList();
	} catch(...) {}
}

void ColorPickPopup::colorValueChanged(ccColor3B color) {
	if (m_colorNode) {
		m_colorNode->m_value = color;
		m_colorNode->updateSettingsList();
	} else {
		m_colorAlphaNode->m_value = to4B(color, m_colorAlphaNode->m_value.a);
		m_colorAlphaNode->updateSettingsList();
	}
}

ColorPickPopup* ColorPickPopup::create(
	ColorSettingNode* colorNode,
	ColorAlphaSettingNode* colorAlphaNode
) {
	auto ret = new ColorPickPopup;
	if (ret && ret->init(320.f, 250.f, colorNode, colorAlphaNode, "GJ_square02.png")) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

ColorPickPopup* ColorPickPopup::create(ColorSettingNode* colorNode) {
	return ColorPickPopup::create(colorNode, nullptr);
}

ColorPickPopup* ColorPickPopup::create(ColorAlphaSettingNode* colorNode) {
	return ColorPickPopup::create(nullptr, colorNode);
}

GEODE_GENERATE_SETTING_CREATE(BoolSetting, 30.f);
GEODE_GENERATE_SETTING_CREATE(IntSetting, 30.f);
GEODE_GENERATE_SETTING_CREATE(FloatSetting, 30.f);
GEODE_GENERATE_SETTING_CREATE(StringSetting, 30.f);
GEODE_GENERATE_SETTING_CREATE(ColorSetting, 30.f);
GEODE_GENERATE_SETTING_CREATE(ColorAlphaSetting, 30.f);
GEODE_GENERATE_SETTING_CREATE(PathSetting, 30.f);
GEODE_GENERATE_SETTING_CREATE(StringSelectSetting, 30.f);*/
