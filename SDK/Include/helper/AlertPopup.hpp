#pragma once

#include "layers_scenes_transitions_nodes/CCLayer.h"
#include <string>

namespace geode::helper {

	class AlertPopupDelegate;

	/**
	 * Literally a minimal recreation of FLAlertLayer's alert property
	 */
	class AlertPopup final : cocos2d::CCLayerColor {
	private:
		AlertPopupDelegate* m_delegate;
		cocos2d::CCMenu* m_buttonMenu;

	public:
		// static AlertPopup* create(std::string const& title, std::string const& caption, std::string const& button);
		static AlertPopup* create(std::string const& title, std::string const& caption, std::string const& button1, std::string const& button2);

		AlertPopup* withDelegate(AlertPopupDelegate* delegate);

		AlertPopup* show();
		// AlertPopup* showWithAction(cocos2d::CCAction* enter, cocos2d::CCAction* leave);

	private:
		bool init(std::string const& title, std::string const& caption, std::string const& button1, std::string const& button2);

		// bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
		// void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
		// void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
		// void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
		void keyBackClicked() override;

		// void registerWithTouchDispatcher() override;
		// void keyDown(cocos2d::enumKeyCodes keyCode) override;
		// void onEnter() override;
		void onButton1(cocos2d::CCObject* object);
		void onButton2(cocos2d::CCObject* object);

		void close();

		~AlertPopup() override;
	};

	/**
	 * A delegate for the alert popup
	 */
	class AlertPopupDelegate {
	protected:
		/**
		 * User clicked the left button
		 */
		virtual void onButton1() {}
		/**
		 * User clicked the right button
		 */
		virtual void onButton2() {}
		/**
		 * User dismissed the popup without selecting any button
		 */
		virtual void onDismiss() {}

		friend class AlertPopup;
	};
}
