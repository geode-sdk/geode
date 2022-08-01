/*#pragma once

#pragma warning(disable: 4067)

#include <Geode.hpp>
#include <nodes/MenuInputNode.hpp>
#include <nodes/Popup.hpp>

USE_GEODE_NAMESPACE();

namespace geode {
	class ColorPickPopup;

	template<class SettingClass>
	class GeodeSettingNode : public SettingNode {
	protected:
		SettingClass* m_setting;
		CCMenu* m_buttonMenu;
		CCLabelBMFont* m_nameLabel;
		CCMenuItemSpriteExtra* m_descButton = nullptr;
		typename SettingClass::value_type_t m_value;

		bool init(SettingClass* setting) {
			if (!CCNode::init())
				return false;
			
			m_setting = setting;
			if constexpr (std::is_same_v<typename SettingClass::value_type_t, size_t>) {
				m_value = m_setting->getIndex();
			} else {
				m_value = m_setting->getValue();
			}

			this->setContentSize({ m_width, m_height });

			auto text = setting->getName().size() ? setting->getName() : setting->getKey();
			m_nameLabel = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
			m_nameLabel->setAnchorPoint({ .0f, .5f });
			m_nameLabel->setPosition(m_height / 2, m_height / 2);
			m_nameLabel->limitLabelWidth(m_width / 2 - 40.f, .5f, .1f);
			this->addChild(m_nameLabel);

			m_buttonMenu = CCMenu::create();
			m_buttonMenu->setPosition(m_width - m_height / 2, m_height / 2);
			this->addChild(m_buttonMenu);

			if (m_setting->getDescription().size()) {
				auto descBtnSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
				descBtnSpr->setScale(.7f);
				m_descButton = CCMenuItemSpriteExtra::create(
					descBtnSpr, this, menu_selector(GeodeSettingNode::onDescription)
				);
				m_descButton->setPosition(
					-m_buttonMenu->getPositionX() +
						m_nameLabel->getPositionX() +
						m_nameLabel->getScaledContentSize().width +
						m_height / 2,
					.0f
				);
				m_buttonMenu->addChild(m_descButton);
			}

			CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
			this->registerWithTouchDispatcher();

			return true;
		}

		void onDescription(CCObject*) {
			FLAlertLayer::create(
				m_nameLabel->getString(),
				m_setting->getDescription(),
				"OK"
			)->show();
		}

		bool hasUnsavedChanges() const override {
			if constexpr (std::is_same_v<typename SettingClass::value_type_t, size_t>) {
				return m_value != m_setting->getIndex();
			} else {
				return m_value != m_setting->getValue();
			}
		}

		void commitChanges() override {
			if constexpr (std::is_same_v<typename SettingClass::value_type_t, size_t>) {
				m_setting->setIndex(m_value);
			} else {
				m_setting->setValue(m_value);
			}
		}

		void resetToDefault() override {
			m_setting->resetToDefault();
			if constexpr (std::is_same_v<typename SettingClass::value_type_t, size_t>) {
				m_value = m_setting->getIndex();
			} else {
				m_value = m_setting->getValue();
			}
			this->updateState();
		}

		virtual void updateState() = 0;

		~GeodeSettingNode() {
			CCDirector::sharedDirector()->getTouchDispatcher()->decrementForcePrio(2);
		}

		GeodeSettingNode(float width, float height) : SettingNode(width, height) {}
	};

	class BoolSettingNode : public GeodeSettingNode<BoolSetting> {
	protected:
		CCMenuItemToggler* m_toggle;

		bool init(BoolSetting* setting);

		void onToggle(CCObject*);
		void updateState() override;

		BoolSettingNode(float width, float height) : GeodeSettingNode<BoolSetting>(width, height) {}

	public:
		static BoolSettingNode* create(BoolSetting* setting, float width);
	};

	class IntSettingNode : public GeodeSettingNode<IntSetting>, public TextInputDelegate {
	protected:
		MenuInputNode* m_valueInput;
		Slider* m_slider = nullptr;

		bool init(IntSetting* setting);

		void updateValue(bool updateInput = true);
		void onArrow(CCObject*);
		void onSlider(CCObject*);
		void textInputClosed(CCTextInputNode* input) override;
		void textChanged(CCTextInputNode* input) override;
		void updateState() override;

		IntSettingNode(float width, float height) : GeodeSettingNode<IntSetting>(width, height) {}

	public:
		static IntSettingNode* create(IntSetting* setting, float width);
	};

	class FloatSettingNode : public GeodeSettingNode<FloatSetting>, public TextInputDelegate {
	protected:
		MenuInputNode* m_valueInput;
		Slider* m_slider = nullptr;

		bool init(FloatSetting* setting);

		void updateValue(bool updateInput = true);
		void onArrow(CCObject*);
		void onSlider(CCObject*);
		void textInputClosed(CCTextInputNode* input) override;
		void textChanged(CCTextInputNode* input) override;
		void updateState() override;

		FloatSettingNode(float width, float height) : GeodeSettingNode<FloatSetting>(width, height) {}

	public:
		static FloatSettingNode* create(FloatSetting* setting, float width);
	};

	class StringSettingNode : public GeodeSettingNode<StringSetting>, public TextInputDelegate {
	protected:
		MenuInputNode* m_input;

		bool init(StringSetting* setting);
		void textChanged(CCTextInputNode* input) override;
		void updateState() override;

		StringSettingNode(float width, float height) : GeodeSettingNode<StringSetting>(width, height) {}
		
	public:
		static StringSettingNode* create(StringSetting* setting, float width);
	};

	class ColorSettingNode : public GeodeSettingNode<ColorSetting> {
	protected:
		ColorChannelSprite* m_colorSprite;

		bool init(ColorSetting* setting);
		void onPickColor(CCObject*);
		void updateState() override;

		ColorSettingNode(float width, float height) : GeodeSettingNode<ColorSetting>(width, height) {}

		friend class ColorPickPopup;

	public:
		static ColorSettingNode* create(ColorSetting* setting, float width);
	};

	class ColorAlphaSettingNode : public GeodeSettingNode<ColorAlphaSetting> {
	protected:
		ColorChannelSprite* m_colorSprite;

		bool init(ColorAlphaSetting* setting);
		void onPickColor(CCObject*);
		void updateState() override;
		
		ColorAlphaSettingNode(float width, float height) : GeodeSettingNode<ColorAlphaSetting>(width, height) {}

		friend class ColorPickPopup;


	public:
		static ColorAlphaSettingNode* create(ColorAlphaSetting* setting, float width);
	};

	class PathSettingNode : public GeodeSettingNode<PathSetting>, public TextInputDelegate {
	protected:
		MenuInputNode* m_input;

		bool init(PathSetting* setting);
		void textChanged(CCTextInputNode* input) override;
		void onSelectFile(CCObject*);
		void updateState() override;

		PathSettingNode(float width, float height) : GeodeSettingNode<PathSetting>(width, height) {}

	public:
		static PathSettingNode* create(PathSetting* setting, float width);
	};

	class StringSelectSettingNode : public GeodeSettingNode<StringSelectSetting> {
	protected:
		CCLabelBMFont* m_selectedLabel;

		bool init(StringSelectSetting* setting);
		void onChange(CCObject* pSender);
		void updateState() override;

		StringSelectSettingNode(float width, float height) : GeodeSettingNode<StringSelectSetting>(width, height) {}

	public:
		static StringSelectSettingNode* create(StringSelectSetting* setting, float width);
	};

	class CustomSettingPlaceHolderNode : public SettingNode {
	protected:
		bool init(CustomSettingPlaceHolder* setting, bool isLoaded);

		bool hasUnsavedChanges() const override {
			return false;
		}
		void commitChanges() override {} 
		void resetToDefault() override {}

		CustomSettingPlaceHolderNode(float width, float height) : SettingNode(width, height) {}

	public:
		static CustomSettingPlaceHolderNode* create(CustomSettingPlaceHolder* setting, bool isLoaded, float width);
	};

	class ColorPickPopup :
		public Popup<ColorPickPopup, ColorSettingNode*, ColorAlphaSettingNode*>,
		public ColorPickerDelegate,
		public TextInputDelegate
	{
	protected:
		ColorSettingNode* m_colorNode = nullptr;
		ColorAlphaSettingNode* m_colorAlphaNode = nullptr;
		MenuInputNode* m_alphaInput = nullptr;
		Slider* m_alphaSlider = nullptr;
	
		void setup(ColorSettingNode* colorNode, ColorAlphaSettingNode* alphaNode) override;
		void colorValueChanged(ccColor3B) override;
		void textChanged(CCTextInputNode* input) override;
		void onSlider(CCObject*);

	public:
		static ColorPickPopup* create(ColorSettingNode* colorNode, ColorAlphaSettingNode* colorAlphaNode);
		static ColorPickPopup* create(ColorSettingNode* colorNode);
		static ColorPickPopup* create(ColorAlphaSettingNode* colorNode);
	};
}

#pragma warning(default: 4067)*/
