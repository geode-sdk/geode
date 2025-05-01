#include <Geode/ui/GSlider.hpp>

using namespace geode::prelude;


// needed to make the class fully abstract so that i dont need to implement the other pure virtuals
inline GSliderDelegate::~GSliderDelegate() = default;


bool GSlider::GSliderThumb::init(CCNode* normalSprite, CCNode* heldSprite) {
	if (!CCNodeRGBA::init()) return false;

	this->setCascadeColorEnabled(true);
	this->setCascadeOpacityEnabled(true);

	if (!normalSprite) normalSprite = CCSprite::create("sliderthumb.png");
	if (!heldSprite) heldSprite = CCSprite::create("sliderthumbsel.png");

	m_normalSprite = normalSprite;
	m_heldSprite = heldSprite;

	m_obContentSize = m_normalSprite->getContentSize();

	heldSprite->setVisible(false);

	this->addChildAtPosition(m_normalSprite, Anchor::Center/* , {}, false */);
	this->addChildAtPosition(m_heldSprite,   Anchor::Center/* , {}, false */);

	return true;
}

void GSlider::GSliderThumb::updateState(bool isHeld) {
	m_held = isHeld;

	m_normalSprite->setVisible(!isHeld);
	m_heldSprite->setVisible(isHeld);
	m_obContentSize = (isHeld ? m_heldSprite : m_normalSprite)->getContentSize();
}

GSlider::GSliderThumb* GSlider::GSliderThumb::create(CCNode* normalSprite, CCNode* heldSprite) {
	auto ret = new GSliderThumb();
	if (ret->init(normalSprite, heldSprite)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool GSlider::init(
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

	this->setContentSize({width, 16.f}); 

	this->ignoreAnchorPointForPosition(false);

	m_minValue = minValue;
	m_maxValue = maxValue;
	m_value = (m_maxValue + m_minValue) / 2;

	if (!outline) outline = CCScale9Sprite::create("slidergroove.png", 
		{ 0.f, 0.f, 210.f, 16.f }, { 5.5f, 7.5f, 199.f, 1.f }
	);
	m_barOutline = outline;
	m_barOutline->setContentSize({width, m_obContentSize.height});
	m_barOutline->setCascadeColorEnabled(true);
	m_barOutline->setCascadeOpacityEnabled(true);

	if (!fill) fill = CCSprite::create("sliderBar.png");
	m_barFill = fill;
	m_barFill->setContentWidth(m_barFill->getContentWidth() / 2);
	m_barFill->setZOrder(-1);
	m_barFill->setTextureRect({ 0.f, 0.f, (width - 4.f) / 2, 8.f });
	m_barFill->getTexture()->setTexParameters(new ccTexParams{ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

	if (!thumb)     thumb     = CCSprite::create("sliderthumb.png");
	if (!thumbHeld) thumbHeld = CCSprite::create("sliderthumbsel.png");
	m_thumb = GSliderThumb::create(thumb, thumbHeld);
	// idk why i need to do this but for some reason i do
	m_thumb->setAnchorPoint({});
	m_thumb->setAnchorPoint({.5f, .5f});

	m_valueLabel = CCLabelBMFont::create(fmt::format("{}", m_value).c_str(), "bigFont.fnt");
	m_valueLabel->setVisible(false);
	m_valueLabel->setScale(.75f);

	m_barOutline->addChildAtPosition(m_barFill, Anchor::Left, {2.f, 0.f}, {0.f, .5f});
	this->addChildAtPosition(m_barOutline, Anchor::Center);
	this->addChildAtPosition(m_thumb,      Anchor::Left,  {m_obContentSize.width / 2, 0.f}, {.5f, .5f});
	this->addChildAtPosition(m_valueLabel, Anchor::Right, {10.f, 0.f},                      {0.f, .5f});

	updateState(m_value);

	return true;
}

bool GSlider::ccTouchBegan(CCTouch* touch, CCEvent* event) {
	auto thumbSize = m_thumb->getContentSize();
	auto thumbPos = m_thumb->getPosition();
	auto touchPos = this->convertTouchToNodeSpace(touch);
	m_xOffsetOfTouchFromThumb = touchPos.x - m_thumb->getPosition().x;

	bool sliderStarted = false;
	if (
		touchPos.x > thumbPos.x - (thumbSize.width / 2) && touchPos.y > thumbPos.y - (thumbSize.height / 2) &&
		touchPos.x < thumbPos.x + (thumbSize.width / 2) && touchPos.y < thumbPos.y + (thumbSize.height / 2)
	) {
		m_touchStartValue = m_value;
		sliderStarted = true;
		for (auto [key, delegate] : m_delegates) {
			if (delegate) delegate->sliderStarted(this, m_value);
		}
	}

	m_thumb->updateState(sliderStarted);
	return sliderStarted;
}
void GSlider::ccTouchMoved(CCTouch* touch, CCEvent* event) {
	float newThumbXPos = this->convertTouchToNodeSpace(touch).x - m_xOffsetOfTouchFromThumb;

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

	float newValue = (m_maxValue - m_minValue) * (newThumbXPos / width) + m_minValue;
	for (auto [key, delegate] : m_delegates) {
		if (delegate) delegate->sliderChanged(this, newValue, newValue - m_touchStartValue);
	}

	if (sliderAtMin) 
		for (auto [key, delegate] : m_delegates) {
			if (delegate) delegate->sliderReachedMinimum(this);
		}
	else if (sliderAtMax)  
		for (auto [key, delegate] : m_delegates) {
			if (delegate) delegate->sliderReachedMaximum(this);
		}

	updateState(newValue);
}
void GSlider::ccTouchEnded(CCTouch* touch, CCEvent* event) {

	m_thumb->updateState(false);

	for (auto [key, delegate] : m_delegates) {
		if (delegate) delegate->sliderEnded(this, m_value, m_value - m_touchStartValue);
	}
}
void GSlider::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
	ccTouchEnded(touch, event);
}

void GSlider::updateState(float newValue) {

	if (m_useSnap) {
		m_value = std::min(
			std::round((newValue - m_minValue) / m_snapStep) * m_snapStep + m_minValue, 
		m_maxValue);
		if (m_value + (m_snapStep / 2) > m_maxValue) m_value = m_maxValue;
	}
	else m_value = newValue;

	// between 1 and 0
	float partOfSliderCovered = (m_value - m_minValue) / (m_maxValue - m_minValue);

	m_barFill->setTextureRect({ 
		0.f, 0.f, 
		(m_obContentSize.width - 4.f) * partOfSliderCovered, 
		8.f 
	});
	m_barFill->setContentWidth(
		(m_obContentSize.width - 4.f) * partOfSliderCovered
	);
	
	m_thumb->updateAnchoredPosition(
		Anchor::Left, 
		{(m_obContentSize.width - 10.f) * partOfSliderCovered + 5.f, 0.f}, 
		{.5f, .5f}
	);

	m_valueLabel->setString(fmt::format("{:.2f}", m_value).c_str());

	for (auto [key, callback] : m_callbacks) {
		if (callback) callback(newValue, newValue - m_touchStartValue);
	}
}

GSlider* GSlider::create() {
	return create(0.f, 1.f);
}

GSlider* GSlider::create(float minValue, float maxValue) {
	return create(minValue, maxValue, 210.f); 
}

GSlider* GSlider::create(
	float minValue, float maxValue, float width, 
	CCScale9Sprite* outline, CCSprite* fill, CCNode* thumb, CCNode* thumbHeld
) {
	auto ret = new GSlider();
	if (ret->init(minValue, maxValue, width, outline, fill, thumb, thumbHeld)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

void GSlider::setContentSize(cocos2d::CCSize const& size) {
	if (size.width == m_obContentSize.width) return CCLayerRGBA::setContentSize(size);
	CCLayerRGBA::setContentSize(size);

	if (m_label)      m_label->limitLabelWidth(m_obContentSize.width, .4f, .2f);
	if (m_barOutline) m_barOutline->setContentWidth(size.width);
	if (m_barFill) {
		m_barFill->setTextureRect({ 
			0.f, 0.f, 
			(m_obContentSize.width - 4.f) * (m_value - m_minValue) / (m_maxValue - m_minValue), 
			8.f 
		});
		m_barFill->setContentWidth(
			(m_obContentSize.width - 4.f) * (m_value - m_minValue) / (m_maxValue - m_minValue)
		);
	}
}


void GSlider::setValue(float value, bool triggerCallback) {
	auto tempCallbacks = m_callbacks;
	if (!triggerCallback) {
		m_callbacks = {};
		for (auto [key, delegate] : m_delegates) {
			if (delegate) delegate->sliderChanged(this, value, 0.f);
		}
	}
	updateState(value);
	m_callbacks = tempCallbacks;
}
void GSlider::setMinValue(float minValue) {
	m_minValue = minValue;
}
void GSlider::setMaxValue(float maxValue) {
	m_maxValue = maxValue;
}
void GSlider::setSnap(float snapStep, bool useSnap) {
	m_snapStep = snapStep;
	m_useSnap = useSnap;
}
// all of this is stolen from the text input labels code lmao (except the font part)
void GSlider::setLabel(std::string const& label, std::string const& font) { 
	if (label.size()) {
		if (m_label) {
			m_label->setString(label.c_str());
			m_label->setFntFile(font.c_str());
		}
		else {
			m_label = CCLabelBMFont::create(label.c_str(), font.c_str());
			this->addChildAtPosition(m_label, Anchor::Top, {0.f, 15.f}, {.5f, 0.f});
		}
		m_label->limitLabelWidth(m_obContentSize.width, .4f, .2f);
	}
	else {
		if (m_label) {
			m_label->removeFromParent();
			m_label = nullptr;
		}
	}
}
void GSlider::setHeld(bool held) {
	m_thumb->updateState(held);
}
void GSlider::showValueLabel(bool show) {
	m_valueLabel->setVisible(show);
}

void GSlider::addCallback(std::string const& ID, std::function<void(float, float)> callback) {
	m_callbacks[ID] = callback;
}
void GSlider::removeCallback(std::string const& ID) {
	m_callbacks.erase(ID);
}
std::function<void(float, float)> GSlider::getCallback(std::string const& ID) {
	if (m_callbacks.contains(ID)) return m_callbacks[ID];
	else return nullptr;
}

void GSlider::addDelegate(std::string const& ID, GSliderDelegate* delegate) {
	m_delegates[ID] = delegate;
}
void GSlider::removeDelegate(std::string const& ID) {
	m_delegates.erase(ID);
}
GSliderDelegate* GSlider::getDelegate(std::string const& ID) {
	if (m_delegates.contains(ID)) return m_delegates[ID];
	else return nullptr;
}

float GSlider::getValue() const {
	return m_value;
}
float GSlider::getMinValue() const {
	return m_minValue;
}
float GSlider::getMaxValue() const {
	return m_maxValue;
}

CCLabelBMFont* GSlider::getLabel() const {
	return m_label;
}
CCLabelBMFont* GSlider::getValueLabel() const {
	return m_valueLabel;
}
CCScale9Sprite* GSlider::getBarOutline() const {
	return m_barOutline;
}
CCSprite* GSlider::getBarFill() const {
	return m_barFill;
}
GSlider::GSliderThumb* GSlider::getThumb() const {
	return m_thumb;
}
