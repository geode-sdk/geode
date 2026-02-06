#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

constexpr auto NOTIFICATION_FADEIN = .3f;
constexpr auto NOTIFICATION_FADEOUT = 1.f;

CCArray* Notification::s_queue = nullptr;

class Notification::Impl final {
public:
    NineSlice* bg;
    cocos2d::CCLabelBMFont* label;
    cocos2d::CCSprite* icon = nullptr;
    float time;
    bool showing = false;
};

Notification::Notification() : m_impl(std::make_unique<Impl>()) { }

Notification::~Notification() { }

bool Notification::init(ZStringView text, CCSprite* icon, float time) {
    if (!CCNodeRGBA::init()) return false;

    m_impl->time = time;

    m_impl->bg = NineSlice::create("square02b_small.png", { 0, 0, 40, 40 });
    m_impl->bg->setColor({ 0, 0, 0 });
    this->addChild(m_impl->bg);

    m_impl->label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
    m_impl->label->setScale(.6f);
    m_impl->bg->addChild(m_impl->label);

    if ((m_impl->icon = icon)) {
        m_impl->bg->addChild(icon);
    }

    this->setScale(.75f);
    this->updateLayout();

    return true;
}

void Notification::updateLayout() {
    constexpr auto PADDING = 5.f;
    auto size = m_impl->label->getScaledContentSize();

    float spaceForIcon = 0.f;
    if (m_impl->icon) {
        limitNodeSize(m_impl->icon, { size.height, size.height }, 1.f, .1f);
        spaceForIcon += m_impl->icon->getScaledContentSize().width + PADDING;
    }
    size += CCSize { spaceForIcon + PADDING * 2, PADDING * 2 };
    m_impl->bg->setContentSize(size);

    if (m_impl->icon) {
        m_impl->icon->setPosition({ size.height / 2, size.height / 2 });
        m_impl->label->setPosition(size / 2 + CCSize { spaceForIcon / 2, .0f });
    }
    else {
        m_impl->label->setPosition(size / 2);
    }
}

void Notification::showNextNotification() {
    m_impl->showing = false;
    if (!s_queue) {
        s_queue = CCArray::create();
        s_queue->retain();
    }
    OverlayManager::get()->removeChild(this);
    // remove self from front of queue
    s_queue->removeFirstObject();
    if (auto obj = s_queue->firstObject()) {
        static_cast<Notification*>(obj)->show();
    }
    this->removeFromParent();
}

CCSprite* Notification::createIcon(NotificationIcon icon) {
    switch (icon) {
        default:
        case NotificationIcon::None: {
            return nullptr;
        } break;

        case NotificationIcon::Loading: {
            auto icon = CCSprite::create("loadingCircle.png");
            icon->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));
            icon->setBlendFunc({ GL_ONE, GL_ONE });
            return icon;
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

Notification* Notification::create(ZStringView text, CCSprite* icon, float time) {
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

void Notification::setIcon(cocos2d::CCSprite* icon) {
    if (m_impl->icon) {
        m_impl->icon->removeFromParent();
    }
    if ((m_impl->icon = icon)) {
        m_impl->bg->addChild(icon);
    }
    this->updateLayout();
}

void Notification::setTime(float time) {
    m_impl->time = time;
    this->wait();
}

NineSlice* Notification::getBg() {
    return m_impl->bg;
}

cocos2d::CCLabelBMFont* Notification::getLabel() {
    return m_impl->label;
}

cocos2d::CCSprite* Notification::getIcon() {
    return m_impl->icon;
}

float Notification::getTime() {
    return m_impl->time;
}

bool Notification::isShowing() {
    return m_impl->showing;
}

void Notification::animateIn() {
    m_impl->label->setOpacity(0);
    if (m_impl->icon) {
        m_impl->icon->setOpacity(0);
    }
    m_impl->bg->setOpacity(0);
    m_impl->label->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 255));
    if (m_impl->icon) {
        m_impl->icon->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 255));
    }
    m_impl->bg->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 150));
}

void Notification::animateOut() {
    m_impl->label->runAction(CCFadeTo::create(NOTIFICATION_FADEOUT, 0));
    if (m_impl->icon) {
        m_impl->icon->runAction(CCFadeTo::create(NOTIFICATION_FADEOUT, 0));
    }
    m_impl->bg->runAction(CCFadeTo::create(NOTIFICATION_FADEOUT, 0));
}

void Notification::waitAndHide() {
    this->setTime(NOTIFICATION_DEFAULT_TIME);
}

void Notification::show() {
    if (!s_queue) {
        s_queue = CCArray::create();
        s_queue->retain();
    }
    if (!m_impl->showing) {
        if (!s_queue->containsObject(this)) {
            s_queue->addObject(this);
        }
        if (s_queue->firstObject() != this) {
            return;
        }
        if (!this->getParent()) {
            auto winSize = CCDirector::get()->getWinSize();
            this->setPosition(winSize.width / 2, winSize.height / 4);
            this->setZOrder(CCScene::get()->getChildrenCount() > 0 ? CCScene::get()->getHighestChildZ() + 2 : 10);
        }
        OverlayManager::get()->addChild(this);
        m_impl->showing = true;
    }
    this->runAction(CCSequence::create(
        CCCallFunc::create(this, callfunc_selector(Notification::animateIn)),
        // wait for fade-in to finish
        CCDelayTime::create(NOTIFICATION_FADEIN),
        CCCallFunc::create(this, callfunc_selector(Notification::wait)),
        nullptr
    ));
}

void Notification::wait() {
    this->stopAllActions();
    if (m_impl->time) {
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
        CCCallFunc::create(this, callfunc_selector(Notification::animateOut)),
        // wait for fade-out to finish
        CCDelayTime::create(NOTIFICATION_FADEOUT),
        CCCallFunc::create(this, callfunc_selector(Notification::showNextNotification)),
        nullptr
    ));
}

void Notification::cancel() {
    if(m_pParent) return this->hide();

    if (s_queue->containsObject(this)) {
        s_queue->removeObject(this);
    }
}