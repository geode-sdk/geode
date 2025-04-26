#include <Geode/ui/GSlider.hpp>

using namespace geode::prelude;


// needed to make the class fully abstract so that i dont need to implement the other pure virtuals
inline GSliderDelegate::~GSliderDelegate() = default;


void GSlider::GSliderThumb::update(float) {
	m_obAnchorPoint = CCPoint(.5f, .5f);
	m_bIgnoreAnchorPointForPosition = false;
}

bool GSlider::GSliderThumb::init(CCNode* normalSprite, CCNode* heldSprite) {
	if (!CCNodeRGBA::init()) return false;

	m_obAnchorPoint = CCPoint(.5f, .5f);
	m_bIgnoreAnchorPointForPosition = false;

	if (!normalSprite) normalSprite = CCSprite::create("sliderthumb.png");
	if (!heldSprite) heldSprite = CCSprite::create("sliderthumbsel.png");

	m_normalSprite = normalSprite;
	m_heldSprite = heldSprite;

	m_obContentSize = normalSprite->getContentSize();

	heldSprite->setVisible(false);

	this->addChildAtPosition(normalSprite, Anchor::Center);
	this->addChildAtPosition(heldSprite, Anchor::Center);

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

	m_obContentSize = CCSize(width - 10.f, 16.f); 

	m_obAnchorPoint = CCPoint(.5f, .5f);
	m_bIgnoreAnchorPointForPosition = false;

	m_minValue = minValue;
	m_maxValue = maxValue;
	m_value = (m_maxValue - m_minValue) / 2;

	m_barOutline = outline;
	m_barOutline->setContentSize({width, m_obContentSize.height});

	m_barFill = fill;
	m_barFill->setContentWidth(m_barFill->getContentWidth() / 2);
	m_barFill->setAnchorPoint({0.f, 0.f});
	m_barFill->setZOrder(-1);
	m_barFill->setTextureRect({ 0.f, 0.f, (width - 4.f) / 2, 8.f });
	ccTexParams texParams = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
	m_barFill->getTexture()->setTexParameters(&texParams);

	m_thumb = GSliderThumb::create(thumb, thumbHeld);
	m_thumb->m_obAnchorPoint = CCPoint(.5f, .5f);
	m_thumb->m_bIgnoreAnchorPointForPosition = false;
	m_thumb->setPosition(m_obContentSize / 2);


	m_valueLabel = CCLabelBMFont::create(fmt::format("{}", m_value).c_str(), "bigFont.fnt");
	m_valueLabel->setVisible(false);

	this->addChildAtPosition(m_barOutline, Anchor::Center);
	m_barOutline->addChildAtPosition(m_barFill, Anchor::Center);
	this->addChild(m_thumb);
	this->addChildAtPosition(m_valueLabel, Anchor::Right, {10.f, 0.f}, {0.f, .5f});

	updateState(m_value);
	return true;
}

bool GSlider::ccTouchBegan(CCTouch* touch, CCEvent* event) {
	auto pos = this->convertTouchToNodeSpace(touch);
	auto thumbPos = m_thumb->getPosition();
	auto thumbSize = m_thumb->getContentSize();

	bool sliderStart = false;
	if (
		pos.x > thumbPos.x - (thumbSize.width / 2) && pos.y > thumbPos.y - (thumbSize.height / 2) &&
		pos.x < thumbPos.x + (thumbSize.width / 2) && pos.y < thumbPos.y + (thumbSize.height / 2)
	) {
		m_touchStartValue = m_value;
		sliderStart = true;
		for (auto [key, delegate] : m_delegates) {
			delegate->sliderStarted(this, m_value);
		}
	}

	m_touchStartPos = pos;
	m_thumb->updateState(sliderStart);
	updateState(m_value);
	return sliderStart;
}
void GSlider::ccTouchMoved(CCTouch* touch, CCEvent* event) {
	float deltaX = touch->getLocation().x - m_touchStartPos.x;

	float newThumbXPos = m_thumb->getPositionX() + deltaX;

	auto width = m_obContentSize.width;

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
		delegate->sliderChanged(this, newValue, newValue - m_touchStartValue);
	}

	if (sliderAtMin) 
		for (auto [key, delegate] : m_delegates) {
			delegate->sliderReachedMinimum(this);
		}
	else if (sliderAtMax)  
		for (auto [key, delegate] : m_delegates) {
			delegate->sliderReachedMaximum(this);
		}

	updateState(newValue);
}
void GSlider::ccTouchEnded(CCTouch* touch, CCEvent* event) {
	m_thumb->updateState(false);
	for (auto [key, delegate] : m_delegates) {
		delegate->sliderEnded(this, m_value, m_value - m_touchStartValue);
	}

	updateState(m_value);

	m_touchStartPos = CCPoint(0.f, 0.f);
}
void GSlider::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
	ccTouchEnded(touch, event);
}

void GSlider::updateState(float newValue) {

	if (m_useSnap) m_value = std::round(newValue / m_snapStep) * m_snapStep;
	else m_value = newValue;

	m_barFill->setTextureRect({ 
		0.f, 0.f, 
		(m_obContentSize.width + 6.f) * ((m_value - m_minValue) / (m_maxValue - m_minValue)), 
		8.f 
	});

	m_barFill->setContentWidth(
		(m_obContentSize.width + 6.f) * ((m_value - m_minValue) / (m_maxValue - m_minValue))
	);
	
	m_thumb->m_obAnchorPoint = CCPoint(.5f, .5f);
	m_thumb->m_bIgnoreAnchorPointForPosition = false;
	m_thumb->setPositionX(m_obContentSize.width * ((m_value - m_minValue) / (m_maxValue - m_minValue)));

	m_valueLabel->setString(fmt::format("{:.2f}", m_value).c_str());

	for (auto [key, callback] : m_callbacks) {
		callback(newValue, newValue - m_touchStartValue);
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
	m_barOutline->setContentWidth(size.width);
	m_obContentSize.width = size.width - 10.f;
	updateState(m_value);
};

void GSlider::setValue(float value, bool triggerCallback) {
	auto tempCallbacks = m_callbacks;
	if (!triggerCallback) m_callbacks = {};
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
			this->addChildAtPosition(m_label, Anchor::Top, {0.f, 5.f}, {.5f, 0.f});
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
