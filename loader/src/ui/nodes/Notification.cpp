#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

constexpr auto NOTIFICATION_FADEIN = .3f;
constexpr auto NOTIFICATION_FADEOUT = .5f;

std::vector<Ref<Notification>> Notification::s_queue;

bool Notification::init(ZStringView text, CCNode* icon, float time) {
    if (!CCNodeRGBA::init()) return false;

    m_icon = icon;
    m_time = time;

    m_bg = CCScale9Sprite::create("square02b_small.png", { 0, 0, 40, 40 });
    m_bg->setColor({ 0, 0, 0 });
    this->addChild(m_bg);

    m_content = cocos2d::CCNodeRGBA::create();
    m_content->setAnchorPoint({ .5f, .5f });
    m_content->setCascadeOpacityEnabled(true);
    m_content->setLayout(
        RowLayout::create()
            ->setGap(5.f)
            ->setAutoGrowAxis(0.f)
            ->setAutoScale(false)
    );
    this->addChild(m_content);

    if (icon) {
        m_content->addChild(icon);
    }

    m_label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
    m_label->setScale(.6f);
    m_content->addChild(m_label);

    this->setScale(.75f);
    this->updateLayout();

    return true;
}

void Notification::updateLayout() {
    if (m_icon) {
        limitNodeWidth(m_icon, 19.f, 1.f, 0.f);
    }

    m_content->updateLayout();
    auto size = m_content->getContentSize();
    m_bg->setContentSize(size + CCSize{ 10.f, 10.f });
}

void Notification::showNextNotification() {
    m_showing = false;
    OverlayManager::get()->removeChild(this);
    this->removeFromParent();

    // remove self from front of queue and show next popup
    s_queue.erase(s_queue.begin());

    if (s_queue.size() != 0) {
        s_queue.at(0)->show();
    }
}

CCNode* Notification::createIcon(NotificationIcon icon) {
    switch (icon) {
        default:
        case NotificationIcon::None: {
            return nullptr;
        } break;

        case NotificationIcon::Loading: {
            // gets resized later so size doesn't matter
            return LoadingSpinner::create(20.f);
        } break;

        case NotificationIcon::Success: {
            return CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        } break;

        case NotificationIcon::Warning: {
            return CCSprite::createWithSpriteFrameName("info-alert.png"_spr);
        } break;

        case NotificationIcon::Error: {
            return CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
        } break;

        case NotificationIcon::Info: {
            return CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        } break;
    }
}

Notification* Notification::create(ZStringView text, NotificationIcon icon, float time) {
    return Notification::create(text, createIcon(icon), time);
}

Notification* Notification::create(ZStringView text, CCNode* icon, float time) {
    auto ret = new Notification();
    if (ret->init(text, icon, time)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void Notification::setString(ZStringView text) {
    m_impl->label->setString(text.c_str());
    this->updateLayout();
}

void Notification::setIcon(NotificationIcon icon) {
    this->setIcon(createIcon(icon));
}

void Notification::setIcon(cocos2d::CCNode* icon) {
    if (m_icon) {
        m_icon->removeFromParent();
    }

    m_icon = icon;

    if (icon) {
        m_content->addChild(icon);
    }

    this->updateLayout();
}

void Notification::setTime(float time) {
    m_time = time;
    this->waitThenHide(); // reset timer
}

void Notification::show() {
    if (m_showing) return;

    if (std::find(s_queue.begin(), s_queue.end(), this) == s_queue.end()) {
        s_queue.push_back(this);
    }

    // if we're not the current notification, return
    if (s_queue.at(0) != this) {
        return;
    }

    auto winSize = CCDirector::get()->getWinSize();
    this->setPosition(winSize.width / 2, winSize.height / 4);
    this->setZOrder(CCScene::get()->getChildrenCount() > 0 ? CCScene::get()->getHighestChildZ() + 2 : 10);

    OverlayManager::get()->addChild(this);
    m_showing = true;

    m_content->setOpacity(0);
    m_content->setScale(.6f);
    m_content->setPositionY(-60.f);
    m_bg->setOpacity(0);
    m_bg->setScale(.6f);
    m_bg->setPositionY(-60.f);

    this->runAction(CCSequence::create(
        CallFuncExt::create([this] {
            m_content->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 255));
            m_content->runAction(CCEaseExponentialOut::create(CCScaleTo::create(NOTIFICATION_FADEIN, 1.f, 1.f)));
            m_content->runAction(CCEaseExponentialOut::create(CCMoveBy::create(NOTIFICATION_FADEIN, { 0.f, 60.f })));

            m_bg->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 150));
            m_bg->runAction(CCEaseExponentialOut::create(CCScaleTo::create(NOTIFICATION_FADEIN, 1.f, 1.f)));
            m_bg->runAction(CCEaseExponentialOut::create(CCMoveBy::create(NOTIFICATION_FADEIN, { 0.f, 60.f })));
        }),

        CCDelayTime::create(NOTIFICATION_FADEIN),
        CCCallFunc::create(this, callfunc_selector(Notification::waitThenHide)),
        nullptr
    ));
}

void Notification::waitThenHide() {
    this->stopAllActions();

    if (m_time != 0.f) {
        this->runAction(CCSequence::create(
            CCDelayTime::create(m_impl->time),
            CCCallFunc::create(this, callfunc_selector(Notification::hide)),
            nullptr
        ));
    }
}

void Notification::hide() {
    this->stopAllActions();

    this->runAction(CCSequence::create(
        CallFuncExt::create([this] {
            m_content->runAction(CCEaseExponentialIn::create(CCFadeTo::create(NOTIFICATION_FADEOUT, 0)));
            m_content->runAction(CCEaseExponentialIn::create(CCMoveBy::create(NOTIFICATION_FADEOUT, { 0.f, -25.f })));

            m_bg->runAction(CCEaseExponentialIn::create(CCFadeTo::create(NOTIFICATION_FADEOUT, 0)));
            m_bg->runAction(CCEaseExponentialIn::create(CCMoveBy::create(NOTIFICATION_FADEOUT, { 0.f, -25.f })));
        }),

        CCDelayTime::create(NOTIFICATION_FADEOUT * .25f),

        CallFuncExt::create([this] {
            m_content->runAction(CCEaseExponentialIn::create(CCScaleTo::create(NOTIFICATION_FADEOUT, .2f, .2f)));
            m_bg->runAction(CCEaseExponentialIn::create(CCScaleTo::create(NOTIFICATION_FADEOUT, .2f, .2f)));
        }),

        CCDelayTime::create(NOTIFICATION_FADEOUT * .75f),

        CCCallFunc::create(this, callfunc_selector(Notification::showNextNotification)),
        nullptr
    ));
}

void Notification::cancel() {
    if (m_showing) return this->hide();

    // remove from queue if not showing right now
    auto index = std::find(s_queue.begin(), s_queue.end(), this);
    if (index != s_queue.end()) {
        s_queue.erase(index);
    }
}
