#include <Geode/ui/GSlider.hpp>

using namespace geode::prelude;

// needed to make the class fully abstract so that i dont need to implement the other pure virtuals
inline GSliderDelegate::~GSliderDelegate() = default;


bool GSlider::GSliderThumb::init(CCNode* normalSprite, CCNode* heldSprite) {
	if (!CCNodeRGBA::init()) return false;

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
	float minValue, float maxValue,
	CCScale9Sprite* outline, CCSprite* fill,
	CCNode* thumb, CCNode* thumbHeld
) {

	return true;
}

bool GSlider::ccTouchBegan(CCTouch* touch, CCEvent* event) {

	return true;
}
void GSlider::ccTouchMoved(CCTouch* touch, CCEvent* event) {}
void GSlider::ccTouchEnded(CCTouch* touch, CCEvent* event) {}
void GSlider::ccTouchCancelled(CCTouch* touch, CCEvent* event) {}

void GSlider::updateState() {}

GSlider* GSlider::create() {
	return create(0.f, 1.f);
}

GSlider* GSlider::create(float minValue, float maxValue, CCScale9Sprite* outline, CCSprite* fill, CCNode* thumb, CCNode* thumbHeld) {
	auto ret = new GSlider();
	if (ret->init(minValue, maxValue, outline, fill, thumb, thumbHeld)) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

void GSlider::setValue(float value, bool triggerCallback) {}
void GSlider::setMinValue(float minValue) {}
void GSlider::setMaxValue(float maxValue) {}
void GSlider::setSnap(float snapAccuracy) {}
void GSlider::setLabel(std::string const& label) {}
void GSlider::showValueLabel(bool show) {}

void GSlider::addCallback(std::string const& ID, std::function<void(float, float)> callback) {}
void GSlider::removeCallback(std::string const& ID) {}
std::function<void(float, float)> GSlider::getCallback(std::string const& ID) {
	if (m_callbacks.contains(ID)) return m_callbacks[ID];
	else return nullptr;
}

void GSlider::addDelegate(std::string const& ID, GSliderDelegate* delegate) {}
void GSlider::removeDelegate(std::string const& ID) {}
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
