#pragma once

#include "Setting.hpp"
#include <Geode/Bindings.hpp>
#include <Geode/ui/InputNode.hpp>

namespace geode {
    class SettingNode : public cocos2d::CCNode {
    protected:
        std::shared_ptr<Setting> m_setting;

        bool init(std::shared_ptr<Setting> setting);

    public:
        virtual void commit() = 0;
        virtual bool hasUncommittedChanges() = 0;
    };

    namespace {
        template<class N, class T>
        class GeodeSettingNode : public SettingNode {
        public:
            using value_t = typename T::value_t;

        protected:
            value_t m_uncommittedValue;
            cocos2d::CCMenu* m_menu;
            cocos2d::CCLabelBMFont* m_nameLabel;
            cocos2d::CCLabelBMFont* m_editedIndicator;

            bool init(std::shared_ptr<T> setting, float width) {
                if (!SettingNode::init(std::static_pointer_cast<Setting>(setting)))
                    return false;

                this->setContentSize({ width, 40.f });

                m_uncommittedValue = setting->getValue();
                
                m_nameLabel = cocos2d::CCLabelBMFont::create(
                    (setting->getName().size() ?
                        setting->getName() :
                        setting->getKey()).c_str(),
                    "bigFont.fnt"
                );
                m_nameLabel->setAnchorPoint({ .0f, .5f });
                m_nameLabel->limitLabelWidth(width / 2 - 50.f, .5f, .1f);
                m_nameLabel->setPosition({
                    m_obContentSize.height / 2,
                    m_obContentSize.height / 2
                });
                this->addChild(m_nameLabel);

                m_editedIndicator = cocos2d::CCLabelBMFont::create("*", "bigFont.fnt");
                m_editedIndicator->setColor({ 255, 120, 80 });
                m_editedIndicator->setPosition(
                    m_obContentSize.height / 2 +
                        m_nameLabel->getScaledContentSize().width + 10.f,
                    m_obContentSize.height / 2
                );
                m_editedIndicator->setVisible(false);
                this->addChild(m_editedIndicator);

                m_menu = cocos2d::CCMenu::create();
                m_menu->setPosition({
                    m_obContentSize.width - m_obContentSize.height / 2,
                    m_obContentSize.height / 2
                });
                this->addChild(m_menu);

                cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
                m_menu->registerWithTouchDispatcher();

                if (!this->setup(setting, width))
                    return false;
                
                return true;
            }

            virtual bool setup(std::shared_ptr<T> setting, float width) = 0;

            void valueChanged() {
                m_editedIndicator->setVisible(
                    this->hasUncommittedChanges()
                );
            }
        
        public:
            static N* create(
                std::shared_ptr<T> setting,
                float width
            ) {
                auto ret = new N();
                if (ret && ret->init(setting, width)) {
                    ret->autorelease();
                    return ret;
                }
                CC_SAFE_DELETE(ret);
                return nullptr;
            }

            void commit() override {
                std::static_pointer_cast<T>(m_setting)->setValue(m_uncommittedValue);
                this->valueChanged();
            }

            bool hasUncommittedChanges() override {
                return m_uncommittedValue !=
                    std::static_pointer_cast<T>(m_setting)->getValue();
            }
        };
    }

    class BoolSettingNode :
        public GeodeSettingNode<BoolSettingNode, BoolSetting>
    {
    protected:
        bool setup(std::shared_ptr<BoolSetting> setting, float width) override;
    };

    class IntSettingNode :
        public GeodeSettingNode<IntSettingNode, IntSetting>
    {
    protected:
        bool setup(std::shared_ptr<IntSetting> setting, float width) override;
    };

    class FloatSettingNode :
        public GeodeSettingNode<FloatSettingNode, FloatSetting>
    {
    protected:
        bool setup(std::shared_ptr<FloatSetting> setting, float width) override;
    };

    class StringSettingNode :
        public GeodeSettingNode<StringSettingNode, StringSetting>
    {
    protected:
        InputNode* m_input;

        void updateLabel();
        
        bool setup(std::shared_ptr<StringSetting> setting, float width) override;
    };

}
