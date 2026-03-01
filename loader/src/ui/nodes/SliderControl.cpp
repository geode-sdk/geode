#include <Geode/ui/SliderControl.hpp>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SliderControl::Impl final {
public:
    float m_min = 0.f;
    float m_max = 100.f;
    float m_snapStep = 0.f;
    float m_percent = 0.f;
    float m_dragOffsetX = 0.f;

    bool m_enabled = true;
    bool m_readOnly = false;
    bool m_sliderBypass = false;
    bool m_dragging = false;

    CCSize m_barOffset;
    CCNodeRGBA* m_thumb;

    CCSprite* m_thumbRegular;
    CCSprite* m_thumbSelected;
    CCSprite* m_bar;
    geode::NineSlice* m_groove;
    
    SliderCallback m_slideCallback;
    SliderCallback m_clickCallback;
    SliderCallback m_releaseCallback;
    
    geode::TextInput* m_linkedTextInput;
    unsigned int m_textInputPrecision;

    CCLabelBMFont* m_linkedLabel;
    unsigned int m_labelPrecision;

    int m_touchPriority = kCCMenuHandlerPriority;
};

SliderControl::SliderControl() : m_impl(std::make_unique<Impl>()) {}

SliderControl::~SliderControl() {}

SliderControl* SliderControl::createCustom(CCSprite* thumb, CCSprite* thumbSelected, geode::NineSlice* groove, ZStringView bar, SliderCallback callback, cocos2d::CCSize const& barOffset) {
    auto ret = new SliderControl();
    if (ret->initCustom(thumb, thumbSelected, groove, std::move(bar), std::move(callback), barOffset)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

SliderControl* SliderControl::create(SliderCallback callback, bool alt) {
    auto ret = new SliderControl();
    if (ret->initStandard(std::move(callback), alt)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool SliderControl::initCustom(CCSprite* thumb, CCSprite* thumbSelected, NineSlice* groove, ZStringView bar, SliderCallback callback, CCSize const& barOffset) {
    if (!CCNode::init()) return false;
    if (!thumb || !thumbSelected || !groove) return false;

    m_impl->m_bar = CCSprite::create(bar.c_str());
    if (!m_impl->m_bar) return false;

    m_impl->m_thumbRegular = thumb;
    m_impl->m_thumbSelected = thumbSelected;

    m_impl->m_thumbSelected->setVisible(false);

    m_impl->m_thumb = CCNodeRGBA::create();
    m_impl->m_thumb->setAnchorPoint({0.5f, 0.5f});
    m_impl->m_thumb->setContentSize(m_impl->m_thumbRegular->getContentSize());
    m_impl->m_thumb->setZOrder(1);
    m_impl->m_thumb->setCascadeColorEnabled(true);
    m_impl->m_thumb->setCascadeOpacityEnabled(true);

    m_impl->m_thumb->addChild(m_impl->m_thumbRegular);
    m_impl->m_thumb->addChild(m_impl->m_thumbSelected);
    
    addChild(m_impl->m_thumb);

    m_impl->m_groove = groove;
    m_impl->m_bar->setZOrder(-1);
    
    auto repeat = ccTexParams{GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    m_impl->m_bar->getTexture()->setTexParameters(&repeat);
    m_impl->m_bar->setAnchorPoint({0.f, 0.5f});

    addChild(m_impl->m_groove);
    addChild(m_impl->m_bar);

    m_impl->m_slideCallback = std::move(callback);
    m_impl->m_barOffset = barOffset;

    setAnchorPoint({0.5f, 0.5f});
    setCascadeColorEnabled(true);
    setCascadeOpacityEnabled(true);

    setContentSize({200, 10});

    m_impl->m_thumbRegular->setPosition(m_impl->m_thumb->getContentSize() / 2.f);
    m_impl->m_thumbSelected->setPosition(m_impl->m_thumb->getContentSize() / 2.f);

    m_impl-> m_bar->setPosition(m_impl->m_barOffset);

    updateSize();
    return true;
}

bool SliderControl::initStandard(SliderCallback callback, bool alt) {
    auto thumb = CCSprite::create("sliderthumb.png");
    auto thumbSel = CCSprite::create("sliderthumbsel.png");
    
    if (alt) {
        auto groove = geode::NineSlice::create("slider-groove-2.png"_spr);
        return initCustom(thumb, thumbSel, groove, "sliderBar2.png", std::move(callback), {2.f, 2.f});
    }
    else {
        auto groove = geode::NineSlice::create("slider-groove.png"_spr);
        return initCustom(thumb, thumbSel, groove, "sliderBar.png", std::move(callback), {2.f, 2.f});
    }
}

void SliderControl::updateSize() {
    m_impl->m_groove->setContentSize(getContentSize());
    m_impl->m_groove->setPosition(getContentSize() / 2.f);
    
    m_impl->m_bar->setContentWidth(getContentWidth() - m_impl->m_barOffset.width * 2);

    float usableHeight = getContentHeight() - m_impl->m_barOffset.height * 2.f;
    float scale = usableHeight / m_impl->m_bar->getContentHeight();
    m_impl->m_bar->setScale(scale);

    auto rect = m_impl->m_bar->getTextureRect();
    rect.size.width = ((getContentWidth() - m_impl->m_barOffset.width * 2.f) / scale) * std::clamp(m_impl->m_percent, 0.f, 1.f);
    m_impl->m_bar->setTextureRect(rect, m_impl->m_bar->isTextureRectRotated(), rect.size);

    m_impl->m_bar->setPositionY(getContentHeight() / 2.f);
    m_impl->m_thumb->setPosition({getContentWidth() * m_impl->m_percent, getContentHeight() / 2.f});
}

void SliderControl::setSlideCallback(SliderCallback callback) {
    m_impl->m_slideCallback = std::move(callback);
}

void SliderControl::setClickCallback(SliderCallback callback) {
    m_impl->m_clickCallback = std::move(callback);
}

void SliderControl::setReleaseCallback(SliderCallback callback) {
    m_impl->m_releaseCallback = std::move(callback);
}

void SliderControl::setMin(float min) {
    m_impl->m_min = min;
}

float SliderControl::getMin() {
    return m_impl->m_min;
}

void SliderControl::setMax(float max) {
    m_impl->m_max = max;
}

float SliderControl::getMax() {
    return m_impl->m_max;
}

void SliderControl::setEnabled(bool enabled) {
    m_impl->m_enabled = enabled;
}

bool SliderControl::isEnabled() {
    return m_impl->m_enabled;
}

void SliderControl::setValue(float value) {
    setPercent((value - m_impl->m_min) / (m_impl->m_max - m_impl->m_min));
}

float SliderControl::getValue() {
    return std::lerp(m_impl->m_min, m_impl->m_max, m_impl->m_percent);
}

void SliderControl::setPercent(float percent) {
    m_impl->m_percent = m_impl->m_sliderBypass ? percent : std::clamp(percent, 0.f, 1.f);
    m_impl->m_thumb->setPositionX(getContentWidth() * m_impl->m_percent);

    updateLinkedTextInput();
    updateLinkedLabel();
    updateSize();
}

float SliderControl::getPercent() {
    return m_impl->m_percent;
}

void SliderControl::setSnapStep(float snapStep) {
    m_impl->m_snapStep = snapStep;
}

float SliderControl::getSnapStep() {
    return m_impl->m_snapStep;
}

void SliderControl::updateLinkedTextInput() {
    if (m_impl->m_linkedTextInput) {
        if (m_impl->m_textInputPrecision != 0) {
            m_impl->m_linkedTextInput->setString(numToString(getValue(), m_impl->m_textInputPrecision));
        }
        else {
            m_impl->m_linkedTextInput->setString(numToString<int>(std::round(getValue())));
        }
    }
}

void SliderControl::updateLinkedLabel() {
    if (m_impl->m_linkedLabel) {
        if (m_impl->m_labelPrecision != 0) {
            m_impl->m_linkedLabel->setString(numToString(getValue(), m_impl->m_labelPrecision).c_str());
        }
        else {
            m_impl->m_linkedLabel->setString(numToString<int>(std::round(getValue())).c_str());
        }
    }
}

void SliderControl::linkTextInput(geode::TextInput* input, unsigned int precision) {
    m_impl->m_linkedTextInput = input;
    m_impl->m_textInputPrecision = precision;

    if (m_impl->m_linkedTextInput) {
        m_impl->m_linkedTextInput->setCommonFilter(m_impl->m_textInputPrecision != 0 ? CommonFilter::Float : CommonFilter::Int);
        m_impl->m_linkedTextInput->setCallback([this] (const std::string& str) {
            if (m_impl->m_readOnly) {
                updateLinkedTextInput();
                return;
            }
            auto num = numFromString<float>(str);
            if (num) {
                m_impl->m_percent = std::clamp((num.unwrap() - m_impl->m_min) / (m_impl->m_max - m_impl->m_min), 0.f, 1.f);
                m_impl->m_thumb->setPositionX(getContentWidth() * m_impl->m_percent);
                updateSize();
                updateLinkedLabel();
            }
        });
    }
    updateLinkedTextInput();
}

void SliderControl::unlinkTextInput() {
    m_impl->m_linkedTextInput = nullptr;
}

void SliderControl::setTextInputPrecision(unsigned int precision) {
    m_impl->m_textInputPrecision = precision;
    updateLinkedTextInput();
}

unsigned int SliderControl::getTextInputPrecision() {
    return m_impl->m_textInputPrecision;
}

geode::TextInput* SliderControl::getLinkedTextInput() {
    return m_impl->m_linkedTextInput;
}

void SliderControl::linkLabel(CCLabelBMFont* label, unsigned int precision) {
    m_impl->m_linkedLabel = label;
    m_impl->m_labelPrecision = precision;
    updateLinkedLabel();
}

void SliderControl::unlinkLabel() {
    m_impl->m_linkedLabel = nullptr;
}

void SliderControl::setLabelPrecision(unsigned int precision) {
    m_impl->m_labelPrecision = precision;
    updateLinkedLabel();
}

unsigned int SliderControl::getLabelPrecision() {
    return m_impl->m_labelPrecision;
}

CCLabelBMFont* SliderControl::getLinkedLabel() {
    return m_impl->m_linkedLabel;
}

void SliderControl::setReadOnly(bool readOnly) {
    m_impl->m_readOnly = readOnly;
    m_impl->m_thumb->setVisible(!readOnly);
}

bool SliderControl::isReadOnly() {
    return m_impl->m_readOnly;
}

void SliderControl::setSliderBypass(bool enabled) {
    m_impl->m_sliderBypass = enabled;
}

bool SliderControl::hasSliderBypass() {
    return m_impl->m_sliderBypass;
}

CCNodeRGBA* SliderControl::getThumb() {
    return m_impl->m_thumb;
}

geode::NineSlice* SliderControl::getGroove() {
    return m_impl->m_groove;
}

CCSprite* SliderControl::getBar() {
    return m_impl->m_bar;
}

void SliderControl::setTouchPriority(int priority) {
    m_impl->m_touchPriority = priority;
}

int SliderControl::getTouchPriority() {
    return m_impl->m_touchPriority;
}

void SliderControl::setContentSize(CCSize const& size) {
    CCNodeRGBA::setContentSize(size);
    updateSize();
}

float SliderControl::applySnap(float value) {
    if (m_impl->m_snapStep <= 0.f) return value;

    float origin = 0.f;
    if (origin < m_impl->m_min) origin = m_impl->m_min;
    if (origin > m_impl->m_max) origin = m_impl->m_max;

    return origin + std::round((value - origin) / m_impl->m_snapStep) * m_impl->m_snapStep;
}

void SliderControl::updateFromTouch(CCTouch* touch) {
    auto location = convertToNodeSpace(touch->getLocation());
    float x = location.x - m_impl->m_dragOffsetX;

    float percent = x / getContentWidth();

    float value = applySnap(std::lerp(m_impl->m_min, m_impl->m_max, percent));
    percent = (value - m_impl->m_min) / (m_impl->m_max - m_impl->m_min);

    setPercent(percent);

    if (m_impl->m_slideCallback) m_impl->m_slideCallback(this, getValue());
}

bool SliderControl::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (!m_impl->m_enabled || !m_impl->m_thumb->isVisible() || m_impl->m_readOnly) return false;

    auto location = m_impl->m_thumb->convertToNodeSpace(touch->getLocation());
    auto bounds = CCRect{0.f, 0.f, m_impl->m_thumb->getContentWidth(), m_impl->m_thumb->getContentHeight()};

    if (!bounds.containsPoint(location)) return false;

    m_impl->m_dragging = true;

    auto sliderSpace = convertToNodeSpace(touch->getLocation());
    m_impl->m_dragOffsetX = sliderSpace.x - m_impl->m_thumb->getPositionX();

    m_impl->m_thumbRegular->setVisible(false);
    m_impl->m_thumbSelected->setVisible(true);

    updateFromTouch(touch);

    if (m_impl->m_clickCallback) m_impl->m_clickCallback(this, getValue());

    return true;
}

void SliderControl::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    if (!m_impl->m_dragging) return;
    updateFromTouch(touch);
}

void SliderControl::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    if (!m_impl->m_dragging) return;

    m_impl->m_dragging = false;

    m_impl->m_thumbRegular->setVisible(true);
    m_impl->m_thumbSelected->setVisible(false);

    if (m_impl->m_releaseCallback) m_impl->m_releaseCallback(this, getValue());
}

void SliderControl::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
    ccTouchEnded(touch, event);
}

void SliderControl::onEnter() {
    CCNodeRGBA::onEnter();
    registerWithTouchDispatcher();
}

void SliderControl::onExit() {
    CCNodeRGBA::onExit();
    CCTouchDispatcher::get()->removeDelegate(this);
}

void SliderControl::registerWithTouchDispatcher() {
    CCTouchDispatcher::get()->addTargetedDelegate(this, m_impl->m_touchPriority, true);
}