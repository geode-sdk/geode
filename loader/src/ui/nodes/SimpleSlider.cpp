#include <Geode/ui/SimpleSlider.hpp>

#include <Geode/ui/Layout.hpp>
#include <fmt/core.h>

using namespace geode::prelude;


void SimpleSliderDelegate::sliderStarted       (SimpleSlider* slider, float value)                   { }
void SimpleSliderDelegate::sliderChanged       (SimpleSlider* slider, float value, float difference) { }
void SimpleSliderDelegate::sliderEnded         (SimpleSlider* slider, float value, float difference) { }
void SimpleSliderDelegate::sliderReachedMinimum(SimpleSlider* slider)                                { }
void SimpleSliderDelegate::sliderReachedMaximum(SimpleSlider* slider)                                { }


bool SimpleSlider::SimpleSliderThumb::init(CCNode* normalSprite, CCNode* heldSprite) {
	if (!CCNodeRGBA::init()) return false;

	this->setCascadeColorEnabled(true);
	this->setCascadeOpacityEnabled(true);

	if (!normalSprite) normalSprite = CCSprite::create("sliderthumb.png");
	if (!heldSprite) heldSprite = CCSprite::create("sliderthumbsel.png");

	m_normalSprite = normalSprite;
	m_heldSprite = heldSprite;

	this->setContentSize(m_normalSprite->getContentSize());

	heldSprite->setVisible(false);

	this->addChildAtPosition(m_normalSprite, Anchor::Center);
	this->addChildAtPosition(m_heldSprite,   Anchor::Center);

	return true;
}

void SimpleSlider::SimpleSliderThumb::updateState(bool isHeld) {
	m_held = isHeld;

	m_normalSprite->setVisible(!isHeld);
	m_heldSprite->setVisible(isHeld);
	this->setContentSize((isHeld ? m_heldSprite : m_normalSprite)->getContentSize());
}

class SimpleSlider::Impl final {
	friend class SimpleSlider;

	float m_xOffsetOfTouchFromThumb;
	float m_touchStartValue;
	
public:

	/**
	 * Delegates to post events to.
	 */
	std::unordered_map<std::string, SimpleSliderDelegate*> m_delegates;
	/**
	 * Callbacks to be activated when the slider is moved.
	 */
	std::unordered_map<std::string, std::function<void(float, float)>> m_callbacks;

	/**
	 * Visual label attached to the slider.
	 */
	CCLabelBMFont* m_label = nullptr;
	/**
	 * Shows the value of the slider in real time.
	 */
	CCLabelBMFont* m_valueLabel = nullptr;

	/**
	 * The outline of the slider. Needs to be `CCScale9Sprite` to make it
	 * possible to stretch the slider without it looking bad.
	 */
	CCScale9Sprite* m_barOutline;
	/**
	 * The fill of the slider.
	 */
	CCSprite* m_barFill;
	/**
	 * The thumb of the slider.
	 * `SimpleSliderThumb` contains one sprite for the normal state of the thumb,
	 * and one sprite for the selected state of it.
	 */
	SimpleSliderThumb* m_thumb;

	/** 
	 * The value of the slider.
	 */
	float m_value;

	float m_minValue = 0.f;
	float m_maxValue = 1.f;

	/**
	 * Level of accuracy the snapping uses.
	 */
	float m_snapStep = .01f;
	bool m_useSnap = false;
	
	int m_amountOfDigitsToShow = 2;
};

SimpleSlider::SimpleSlider() : m_impl(std::make_unique<Impl>()) { }
SimpleSlider::~SimpleSlider() { }

SimpleSlider::SimpleSliderThumb* SimpleSlider::SimpleSliderThumb::create(CCNode* normalSprite, CCNode* heldSprite) {
	auto ret = new SimpleSliderThumb();
	if (ret->init(normalSprite, heldSprite)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool SimpleSlider::init(
	float minValue, float maxValue, float width,
	CCScale9Sprite* outline, CCSprite* fill,
	CCNode* thumb, CCNode* thumbHeld
) {

	if (!CCLayerRGBA::init()) return false;

	this->registerWithTouchDispatcher();
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);

	this->setCascadeColorEnabled(true);
	this->setCascadeOpacityEnabled(true);

	this->setContentSize({width, HEIGHT}); 

	this->ignoreAnchorPointForPosition(false);

	m_impl->m_minValue = minValue;
	m_impl->m_maxValue = maxValue;
	m_impl->m_value = (m_impl->m_maxValue + m_impl->m_minValue) / 2;

	if (!outline) outline = CCScale9Sprite::create("slidergroove.png", 
		{ 0.f, 0.f, 210.f, HEIGHT }, { 5.5f, 7.5f, 199.f, 1.f }
	);
	m_impl->m_barOutline = outline;
	m_impl->m_barOutline->setContentSize({width, m_obContentSize.height});
	m_impl->m_barOutline->setCascadeColorEnabled(true);
	m_impl->m_barOutline->setCascadeOpacityEnabled(true);

	if (!fill) fill = CCSprite::create("sliderBar.png");
	m_impl->m_barFill = fill;
	m_impl->m_barFill->setContentWidth(m_impl->m_barFill->getContentWidth() / 2);
	m_impl->m_barFill->setZOrder(-1);
	m_impl->m_barFill->setTextureRect({ 0.f, 0.f, (width - 4.f) / 2, 8.f });
	m_impl->m_barFill->getTexture()->setTexParameters(new ccTexParams{ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

	if (!thumb)     thumb     = CCSprite::create("sliderthumb.png");
	if (!thumbHeld) thumbHeld = CCSprite::create("sliderthumbsel.png");
	m_impl->m_thumb = SimpleSliderThumb::create(thumb, thumbHeld);
	// idk why i need to do this but for some reason i do
	m_impl->m_thumb->setAnchorPoint({});
	m_impl->m_thumb->setAnchorPoint({.5f, .5f});

	m_impl->m_valueLabel = CCLabelBMFont::create(fmt::format("{:.2f}", m_impl->m_value).c_str(), "bigFont.fnt");
	m_impl->m_valueLabel->setVisible(false);
	m_impl->m_valueLabel->setScale(.75f);

	m_impl->m_barOutline->addChildAtPosition(m_impl->m_barFill, Anchor::Left, {2.f, 0.f}, {0.f, .5f});
	this->addChildAtPosition(m_impl->m_barOutline, Anchor::Center);
	this->addChildAtPosition(m_impl->m_thumb,      Anchor::Left,  {m_obContentSize.width / 2, 0.f}, {.5f, .5f});
	this->addChildAtPosition(m_impl->m_valueLabel, Anchor::Right, {10.f, 0.f},                      {0.f, .5f});

	updateState(m_impl->m_value);

	return true;
}

bool SimpleSlider::ccTouchBegan(CCTouch* touch, CCEvent* event) {
	auto thumbSize = m_impl->m_thumb->getContentSize();
	auto thumbPos = m_impl->m_thumb->getPosition();
	auto touchPos = this->convertTouchToNodeSpace(touch);
	m_impl->m_xOffsetOfTouchFromThumb = touchPos.x - m_impl->m_thumb->getPosition().x;

	bool sliderStarted = false;
	if (
		touchPos.x > thumbPos.x - (thumbSize.width / 2) && touchPos.y > thumbPos.y - (thumbSize.height / 2) &&
		touchPos.x < thumbPos.x + (thumbSize.width / 2) && touchPos.y < thumbPos.y + (thumbSize.height / 2)
	) {
		m_impl->m_touchStartValue = m_impl->m_value;
		sliderStarted = true;
		for (auto& [key, delegate] : m_impl->m_delegates) 
			if (delegate) delegate->sliderStarted(this, m_impl->m_value);
	}

	m_impl->m_thumb->updateState(sliderStarted);
	return sliderStarted;
}
void SimpleSlider::ccTouchMoved(CCTouch* touch, CCEvent* event) {
	float newThumbXPos = this->convertTouchToNodeSpace(touch).x - m_impl->m_xOffsetOfTouchFromThumb;

	float width = m_obContentSize.width;

	bool sliderAtMin = false;
	bool sliderAtMax = false;
	if (newThumbXPos < 0.f) {
		newThumbXPos = 0.f;
		sliderAtMin = true;
	} else if (newThumbXPos > width) {
		newThumbXPos = width;
		sliderAtMax = true;
	}

	float newValue = (m_impl->m_maxValue - m_impl->m_minValue) * (newThumbXPos / width) + m_impl->m_minValue;
	for (auto& [key, delegate] : m_impl->m_delegates) 
		if (delegate) delegate->sliderChanged(this, newValue, newValue - m_impl->m_touchStartValue);

	if (sliderAtMin) for (auto& [key, delegate] : m_impl->m_delegates) {
		if (delegate) delegate->sliderReachedMinimum(this);
	}
	else if (sliderAtMax) for (auto& [key, delegate] : m_impl->m_delegates) 
		if (delegate) delegate->sliderReachedMaximum(this);

	updateState(newValue);
}
void SimpleSlider::ccTouchEnded(CCTouch* touch, CCEvent* event) {

	m_impl->m_thumb->updateState(false);

	for (auto& [key, delegate] : m_impl->m_delegates) 
		if (delegate) delegate->sliderEnded(this, m_impl->m_value, m_impl->m_value - m_impl->m_touchStartValue);
}
void SimpleSlider::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
	ccTouchEnded(touch, event);
}

void SimpleSlider::updateState(float newValue) {

	if (m_impl->m_useSnap) {
		m_impl->m_value = std::min(
			std::round((newValue - m_impl->m_minValue) / m_impl->m_snapStep) * m_impl->m_snapStep + m_impl->m_minValue, 
			m_impl->m_maxValue);
		if (m_impl->m_value + (m_impl->m_snapStep / 2) > m_impl->m_maxValue) m_impl->m_value = m_impl->m_maxValue;
	}
	else m_impl->m_value = newValue;

	// between 1 and 0
	float partOfSliderCovered = (m_impl->m_value - m_impl->m_minValue) / (m_impl->m_maxValue - m_impl->m_minValue);

	m_impl->m_barFill->setTextureRect({ 
		0.f, 0.f, 
		(m_obContentSize.width - 4.f) * partOfSliderCovered, 
		8.f 
	});
	m_impl->m_barFill->setContentWidth(
		(m_obContentSize.width - 4.f) * partOfSliderCovered
	);
	
	m_impl->m_thumb->updateAnchoredPosition(
		Anchor::Left, 
		{(m_obContentSize.width - 10.f) * partOfSliderCovered + 5.f, 0.f}, 
		{.5f, .5f}
	);

	m_impl->m_valueLabel->setString(fmt::format("{:.{}f}", m_impl->m_value, m_impl->m_amountOfDigitsToShow).c_str());

	for (auto& [key, callback] : m_impl->m_callbacks) 
		if (callback) callback(newValue, newValue - m_impl->m_touchStartValue);
}

SimpleSlider* SimpleSlider::create() {
	return create(0.f, 1.f);
}

SimpleSlider* SimpleSlider::create(float minValue, float maxValue) {
	return create(minValue, maxValue, 210.f); 
}

SimpleSlider* SimpleSlider::create(
	float minValue, float maxValue, float width, 
	CCScale9Sprite* outline, CCSprite* fill, CCNode* thumb, CCNode* thumbHeld
) {
	if (minValue >= maxValue) maxValue = minValue + 1;
	auto ret = new SimpleSlider();
	if (ret->init(minValue, maxValue, width, outline, fill, thumb, thumbHeld)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

void SimpleSlider::setContentSize(cocos2d::CCSize const& size) {
	if (size.width == m_obContentSize.width) return CCLayerRGBA::setContentSize(size);
	CCLayerRGBA::setContentSize(size);

	if (m_impl->m_label) m_impl->m_label->limitLabelWidth(m_obContentSize.width, .4f, .2f);
	if (m_impl->m_barOutline) m_impl->m_barOutline->setContentWidth(size.width);
	if (m_impl->m_barFill) {
		auto fillWidth = (m_obContentSize.width - 4.f) * (m_impl->m_value - m_impl->m_minValue) / (m_impl->m_maxValue - m_impl->m_minValue);
		m_impl->m_barFill->setTextureRect({ 0.f, 0.f, fillWidth, 8.f });
		m_impl->m_barFill->setContentWidth(fillWidth);
	}
}


void SimpleSlider::setValue(float value, bool triggerCallback) {
	auto tempCallbacks = std::move(m_impl->m_callbacks); // std::move for better performance
	if (triggerCallback) for (auto& [key, delegate] : m_impl->m_delegates) 
		if (delegate) delegate->sliderChanged(this, value, 0.f);
	updateState(value);
	m_impl->m_callbacks = std::move(tempCallbacks);
}
void SimpleSlider::setMinValue(float minValue) {
	m_impl->m_minValue = minValue;
}
void SimpleSlider::setMaxValue(float maxValue) {
	m_impl->m_maxValue = maxValue;
}
void SimpleSlider::setSnap(float snapStep, bool useSnap) {
	m_impl->m_snapStep = snapStep;
	m_impl->m_useSnap = useSnap;
}
// all of this is stolen from the text input labels code lmao (except the font part)
void SimpleSlider::setLabel(std::string const& label, std::string const& font) { 
	if (label.size()) {
		if (m_impl->m_label) {
			m_impl->m_label->setString(label.c_str());
			m_impl->m_label->setFntFile(font.c_str());
		}
		else {
			m_impl->m_label = CCLabelBMFont::create(label.c_str(), font.c_str());
			this->addChildAtPosition(m_impl->m_label, Anchor::Top, {0.f, 15.f}, {.5f, 0.f});
		}
		m_impl->m_label->limitLabelWidth(m_obContentSize.width, .4f, .2f);
	}
	else {
		if (m_impl->m_label) {
			m_impl->m_label->removeFromParent();
			m_impl->m_label = nullptr;
		}
	}
}
void SimpleSlider::setHeld(bool held) {
	m_impl->m_thumb->updateState(held);
}
void SimpleSlider::showValueLabel(bool show, size_t amountOfDigitsToShow) {
	m_impl->m_valueLabel->setVisible(show);
}

void SimpleSlider::addCallback(std::string const& ID, std::function<void(float, float)> callback) {
	m_impl->m_callbacks[ID] = callback;
}
void SimpleSlider::removeCallback(std::string const& ID) {
	m_impl->m_callbacks.erase(ID);
}
std::function<void(float, float)> SimpleSlider::getCallback(std::string const& ID) {
	if (m_impl->m_callbacks.contains(ID)) return m_impl->m_callbacks[ID];
	else return nullptr;
}

void SimpleSlider::addDelegate(std::string const& ID, SimpleSliderDelegate* delegate) {
	m_impl->m_delegates[ID] = delegate;
}
void SimpleSlider::removeDelegate(std::string const& ID) {
	m_impl->m_delegates.erase(ID);
}
SimpleSliderDelegate* SimpleSlider::getDelegate(std::string const& ID) {
	if (m_impl->m_delegates.contains(ID)) return m_impl->m_delegates[ID];
	else return nullptr;
}

float SimpleSlider::getValue() const {
	return m_impl->m_value;
}
float SimpleSlider::getMinValue() const {
	return m_impl->m_minValue;
}
float SimpleSlider::getMaxValue() const {
	return m_impl->m_maxValue;
}

CCLabelBMFont* SimpleSlider::getLabel() const {
	return m_impl->m_label;
}
CCLabelBMFont* SimpleSlider::getValueLabel() const {
	return m_impl->m_valueLabel;
}
CCScale9Sprite* SimpleSlider::getBarOutline() const {
	return m_impl->m_barOutline;
}
CCSprite* SimpleSlider::getBarFill() const {
	return m_impl->m_barFill;
}
SimpleSlider::SimpleSliderThumb* SimpleSlider::getThumb() const {
	return m_impl->m_thumb;
}
