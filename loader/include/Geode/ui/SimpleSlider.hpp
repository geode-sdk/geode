#pragma once

#include <cocos2d.h>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <Geode/platform/platform.hpp>

namespace geode {

#pragma warning(push)
#pragma warning(disable: 4275)

class SimpleSlider;

/**
 * Delegate for the `SimpleSlider` class.
 */
class GEODE_DLL SimpleSliderDelegate {

	friend class SimpleSlider;

protected:

	/**
	 * Override this function in your class to make something happen when
	 * the slider starts moving.
	 */
	virtual void sliderStarted(SimpleSlider* slider, float value);
	/**
	 * Override this function in your class to make something happen when
	 * the slider has been moved.
	 */
	virtual void sliderChanged(SimpleSlider* slider, float value, float difference);
	/**
	 * Override this function in your class to make something happen when
	 * the slider is released.
	 */
	virtual void sliderEnded(SimpleSlider* slider, float value, float difference);

	/**
	 * Override this function in your class to make something happen when
	 * the slider reaches its minimum value.
	 */
	virtual void sliderReachedMinimum(SimpleSlider* slider);
	/**
	 * Override this function in your class to make something happen when
	 * the slider reaches its maximum value.     *
	 */
	virtual void sliderReachedMaximum(SimpleSlider* slider);
};

/**
 * Slider class with various utilities that are not provided by GD's sliders.
 */
class GEODE_DLL SimpleSlider : public cocos2d::CCLayerRGBA {
protected:
	class Impl;
	std::unique_ptr<Impl> m_impl;

	class SimpleSliderThumb : public cocos2d::CCNodeRGBA {

		friend class SimpleSlider;

	protected:

		bool m_held = false;

		cocos2d::CCNode* m_normalSprite;
		cocos2d::CCNode* m_heldSprite;

		bool init(cocos2d::CCNode* normalSprite, cocos2d::CCNode* heldSprite);

		virtual void updateState(bool isHeld);
		
	public:

		static SimpleSliderThumb* create(cocos2d::CCNode* normalSprite, cocos2d::CCNode* heldSprite);
	};

	static auto constexpr HEIGHT = 16.f;

	SimpleSlider();

	/**
	 * Initializes the slider.
	 * See the create functionss for more info.
	 */
	bool init(
		float minValue, float maxValue, float width,
		cocos2d::extension::CCScale9Sprite*, cocos2d::CCSprite* fill,
		cocos2d::CCNode* thumb, cocos2d::CCNode* thumbHeld
	);

	virtual bool ccTouchBegan    (cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
	virtual void ccTouchMoved    (cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
	virtual void ccTouchEnded    (cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
	virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;

	/**
	 * Updates the state of the slider.
	 */
	virtual void updateState(float newValue);

public:
	/**
	 * Creates and initializes a slider. Use the non-static member 
	 * functionsto configure it.
	 * The minimum value will be 0 and the maximum will be 1.
	 * @return Slider with default settings
	 */
	static SimpleSlider* create();
	/**
	 * Creates a slider with a custom range of values.
	 * @param minValue Minimum value the slider could be.
	 * @param maxValue Maximum value the slider could be.
	 * @return Slider with the specified minimum and maximum values, 
	 * default width and textures
	 */
	static SimpleSlider* create(float minValue, float maxValue);
	/**
	 * Creates a slider, and lets you specify what texture 
	 * to use for each part of the slider.
	 * @param minValue Minimum value the slider could be.
	 * @param maxValue Maximum value the slider could be.
	 * @param width Width for the slider.
	 * @param outline Node for the outline of the bar. Optional.
	 * @param fill Node for the filling of the bar. Optional.
	 * @param thumb Node for the thumb of the slider. Optional.
	 * @param thumbHeld Node for the thumb while it's held. Optional.
	 * @return Slider with the specified components
	 * @warning The bar's outline and fill will be automatically 
	 * resized to fit the size of GD sliders.
	 */
	static SimpleSlider* create(
		float minValue, float maxValue, float width,
		cocos2d::extension::CCScale9Sprite* outline = cocos2d::extension::CCScale9Sprite::create(
			"slidergroove.png", { 0.f, 0.f, 210.f, 16.f }, { 5.5f, 7.5f, 199.f, 1.f }
		),
		cocos2d::CCSprite* fill                     = cocos2d::CCSprite::create("sliderBar.png"),
		cocos2d::CCNode* thumb                      = cocos2d::CCSprite::create("sliderthumb.png"),
		cocos2d::CCNode* thumbHeld                  = cocos2d::CCSprite::create("sliderthumbsel.png")
	);

	/** 
	 * Resizes the slider and all of its components to fit the new size.
	 */
	virtual void setContentSize(cocos2d::CCSize const& size) override;

	/**
	 * Sets the actual value of the slider.
	 * @param value Value the slider should change to.
	 * @param triggerCallback Whether to trigger the callback
	 * function / delegate's sliderChanged event.
	 */
	void setValue(float value, bool triggerCallback);
	/**
	 * Sets the minimum value that the slider can get to.
	 */
	void setMinValue(float minValue);
	/**
	 * Sets the maximum value that the slider can get to.
	 */
	void setMaxValue(float maxValue);
	/**
	 * Sets the size of step the snapping uses.
	 * For example, if the snap is set to 0.1, the slider will snap
	 * to 0.1, then 0.2, then 0.3, etc.
	 * @param snapStep Step between each snap
	 * @param useSnap Wheter to actually snap. Defaults to true
	 */
	void setSnap(float snapStep, bool useSnap = true);
	/**
	 * Sets the string of the label that is attached to the slider.
	 * Always prefer to keep it short.
	 */
	void setLabel(std::string const& label, std::string const& font = "bigFont.fnt");
	/** 
	 * Allows you to make the thumb switch to its held state. Visual only
	 */
	void setHeld(bool held);
	/**
	 * If true, the value label will be visible. Otherwise, it will be invisible.
	 */
	void showValueLabel(bool show, size_t amountOfDigitsToShow = 2);

	/**
	 * Add a callback to be activated when the slider is moved.
	 * First float is the current value, second float is the difference
	 * from when you started holding the slider.
	 * @param ID Unique identifirer for the callback.
	 * @param callback The callback to add to the slider.
	 */
	void addCallback(std::string const& ID, std::function<void(float, float)> callback);
	/**
	 * Remove a callback by its unique ID.
	 */
	 void removeCallback(std::string const& ID);
	 /**
	 * Get a callback by its unique ID.
	 * @param ID The unique identifier for the desired callback.
	 * @return The desired callback, or `nullptr` if it doesn't exist.
	 */
	std::function<void(float, float)> getCallback(std::string const& ID);

	/**
	 * Add a slider delegate to be activated when the slider is moved.
	 * First float is the current value, second float is the difference
	 * from when you started holding the slider.
	 * @param ID Uniqur identifirer for the delegate.
	 * @param delegate The delegate to add to the slider.
	 */
	void addDelegate(std::string const& ID, SimpleSliderDelegate* delegate);
	/**
	 * Remove a delegate by its unique ID.
	 */
	void removeDelegate(std::string const& ID);
	/**
	 * Get a slider delegate by its unique ID.
	 * @param ID The unique identifier for the desired delegate.
	 * @return The desired delegate, or `nullptr` if it doesn't exist.
	 */
	SimpleSliderDelegate* getDelegate(std::string const& ID);

	/**
	 * @return Current value of the slider.
	 */
	float getValue() const;
	/**
	 * @return Minimum value set.
	 */
	float getMinValue() const;
	/**
	 * @return Maximum value set
	 */
	float getMaxValue() const;

	cocos2d::CCLabelBMFont* getLabel() const;
	cocos2d::CCLabelBMFont* getValueLabel() const;
	cocos2d::extension::CCScale9Sprite* getBarOutline() const;
	cocos2d::CCSprite* getBarFill() const;
	SimpleSliderThumb* getThumb() const;

};

#pragma warning(pop)

}
