#pragma once

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>

namespace geode {

    template<class T>
    T do_nothing(T t) {
        return t;
    }

    template<class T = std::string, auto Stringify = do_nothing<std::string>>
    class SelectList : public cocos2d::CCMenu {
    protected:
        std::vector<T> m_list;
        size_t m_index = 0;
        std::function<void(T const&, size_t)> m_onChange;
        cocos2d::CCLabelBMFont* m_label;
        CCMenuItemSpriteExtra* m_prevBtn;
        CCMenuItemSpriteExtra* m_nextBtn;
    
        bool init(
            float width,
            std::vector<T> const& list,
            std::function<void(T const&, size_t)> onChange
        ) {
            if (!cocos2d::CCMenu::init()) 
                return false;
            
            m_list = list;
            m_onChange = onChange;

            this->setContentSize({ width, 30.f });

            auto prevSpr = cocos2d::CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
            prevSpr->setFlipX(true);
            prevSpr->setScale(.3f);

            m_prevBtn = CCMenuItemSpriteExtra::create(
                prevSpr, this, menu_selector(SelectList<T>::onPrev)
            );
            m_prevBtn->setPosition(-width / 2 + 10.f, 0.f);
            this->addChild(m_prevBtn);

            auto nextSpr = cocos2d::CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
            nextSpr->setScale(.3f);

            m_nextBtn = CCMenuItemSpriteExtra::create(
                nextSpr, this, menu_selector(SelectList<T>::onNext)
            );
            m_nextBtn->setPosition(width / 2 - 10.f, 0.f);
            this->addChild(m_nextBtn);

            m_label = cocos2d::CCLabelBMFont::create("", "bigFont.fnt");
            this->addChild(m_label);

            this->updateLabel();

            cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
            this->registerWithTouchDispatcher();

            return true;
        }

        void updateLabel() {
            if (m_list.size()) {
                m_label->setString(Stringify(m_list.at(m_index)).c_str());
                m_prevBtn->setEnabled(true);
                m_nextBtn->setEnabled(true);
            } else {
                m_label->setString("-");
                m_prevBtn->setEnabled(false);
                m_nextBtn->setEnabled(false);
            }
            m_label->limitLabelWidth(m_obContentSize.width - 40.f, .6f, .1f);
        }

        void onPrev(CCObject* sender) {
            if (m_index == 0) {
                m_index = m_list.size() - 1;
            } else {
                m_index--;
            }
            this->updateLabel();
            m_onChange(m_list.at(m_index), m_index);
        }

        void onNext(CCObject* sender) {
            if (m_index == m_list.size() - 1) {
                m_index = 0;
            } else {
                m_index++;
            }
            this->updateLabel();
            m_onChange(m_list.at(m_index), m_index);
        }

    public:
        static SelectList* create(
            float width,
            std::vector<T> const& list,
            std::function<void(T const&, size_t)> onChange
        ) {
            auto ret = new SelectList();
            if (ret && ret->init(width, list, onChange)) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        void setItems(std::vector<T> const& list) {
            m_index = 0;
            m_list = list;
            this->updateLabel();
        }
    };

}
