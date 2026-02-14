#include <Geode/loader/Mod.hpp>
#include <Geode/ui/LoadingSpinner.hpp>
#include <Geode/ui/OverlayManager.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

constexpr auto NOTIFICATION_FADEIN = .3f;
constexpr auto NOTIFICATION_FADEOUT = .5f;

static std::deque<Ref<Notification>> s_queue;

class Notification::Impl final {
public:
    NineSlice* bg;
    CCLabelBMFont* label;
    CCNodeRGBA* content;
    CCNode* icon = nullptr;
    float time;
    bool showing = false;
};

Notification::Notification() : m_impl(std::make_unique<Impl>()) { }

Notification::~Notification() { }

bool Notification::init(ZStringView text, CCNode* icon, float time) {
    if (!CCNodeRGBA::init()) return false;

    m_impl->icon = icon;
    m_impl->time = time;

    m_impl->bg = NineSlice::create("square02b_small.png", { 0, 0, 40, 40 });
    m_impl->bg->setColor({ 0, 0, 0 });
    this->addChild(m_impl->bg);

    m_impl->content = cocos2d::CCNodeRGBA::create();
    m_impl->content->setAnchorPoint({ .5f, .5f });
    m_impl->content->setCascadeOpacityEnabled(true);
    m_impl->content->setLayout(
        RowLayout::create()
            ->setGap(5.f)
            ->setAutoGrowAxis(0.f)
            ->setAutoScale(false)
    );
    this->addChild(m_impl->content);

    if (icon) {
        m_impl->content->addChild(icon);
    }

    m_impl->label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
    m_impl->label->setScale(.6f);
    m_impl->content->addChild(m_impl->label);

    this->setScale(.75f);
    this->updateLayout();

    return true;
}

void Notification::updateLayout() {
    if (m_impl->icon) {
        limitNodeWidth(m_impl->icon, 19.f, 1.f, 0.f);
    }

    m_impl->content->updateLayout();
    auto size = m_impl->content->getContentSize();
    m_impl->bg->setContentSize(size + CCSize{ 10.f, 10.f });
}

void Notification::showNextNotification() {
    m_impl->showing = false;
    OverlayManager::get()->removeChild(this);
    this->removeFromParent();

    // remove self from front of queue and show next popup if it exists
    s_queue.pop_front();

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

void Notification::setIcon(CCNode* icon) {
    if (m_impl->icon) {
        m_impl->icon->removeFromParent();
    }

    m_impl->icon = icon;

    if (icon) {
        m_impl->content->addChild(icon);
    }

    this->updateLayout();
}

CCNode* Notification::getIcon() {
    return m_impl->icon;
}

void Notification::setTime(float time) {
    m_impl->time = time;
    this->waitThenHide(); // reset timer
}

float Notification::getTime() {
    return m_impl->time;
}

NineSlice* Notification::getBG() {
    return m_impl->bg;
}

CCLabelBMFont* Notification::getLabel() {
    return m_impl->label;
}

CCNodeRGBA* Notification::getContent() {
    return m_impl->content;
}

bool Notification::isShowing() {
    return m_impl->showing;
}

void Notification::show() {
    if (m_impl->showing) return;

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
    m_impl->showing = true;

    m_impl->content->setOpacity(0);
    m_impl->content->setScale(.6f);
    m_impl->content->setPositionY(-60.f);
    m_impl->bg->setOpacity(0);
    m_impl->bg->setScale(.6f);
    m_impl->bg->setPositionY(-60.f);

    this->runAction(CCSequence::create(
        CallFuncExt::create([this] {
            m_impl->content->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 255));
            m_impl->content->runAction(CCEaseExponentialOut::create(CCScaleTo::create(NOTIFICATION_FADEIN, 1.f, 1.f)));
            m_impl->content->runAction(CCEaseExponentialOut::create(CCMoveBy::create(NOTIFICATION_FADEIN, { 0.f, 60.f })));

            m_impl->bg->runAction(CCFadeTo::create(NOTIFICATION_FADEIN, 150));
            m_impl->bg->runAction(CCEaseExponentialOut::create(CCScaleTo::create(NOTIFICATION_FADEIN, 1.f, 1.f)));
            m_impl->bg->runAction(CCEaseExponentialOut::create(CCMoveBy::create(NOTIFICATION_FADEIN, { 0.f, 60.f })));
        }),

        CCDelayTime::create(NOTIFICATION_FADEIN),
        CCCallFunc::create(this, callfunc_selector(Notification::waitThenHide)),
        nullptr
    ));
}

void Notification::waitThenHide() {
    this->stopAllActions();

    if (m_impl->time != 0.f) {
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
            m_impl->content->runAction(CCEaseExponentialIn::create(CCFadeTo::create(NOTIFICATION_FADEOUT, 0)));
            m_impl->content->runAction(CCEaseExponentialIn::create(CCMoveBy::create(NOTIFICATION_FADEOUT, { 0.f, -25.f })));

            m_impl->bg->runAction(CCEaseExponentialIn::create(CCFadeTo::create(NOTIFICATION_FADEOUT, 0)));
            m_impl->bg->runAction(CCEaseExponentialIn::create(CCMoveBy::create(NOTIFICATION_FADEOUT, { 0.f, -25.f })));
        }),

        CCDelayTime::create(NOTIFICATION_FADEOUT * .25f),

        CallFuncExt::create([this] {
            m_impl->content->runAction(CCEaseExponentialIn::create(CCScaleTo::create(NOTIFICATION_FADEOUT, .2f, .2f)));
            m_impl->bg->runAction(CCEaseExponentialIn::create(CCScaleTo::create(NOTIFICATION_FADEOUT, .2f, .2f)));
        }),

        CCDelayTime::create(NOTIFICATION_FADEOUT * .75f),

        CCCallFunc::create(this, callfunc_selector(Notification::showNextNotification)),
        nullptr
    ));
}

void Notification::cancel() {
    if (m_impl->showing) return this->hide();

    // remove from queue if not showing right now
    auto index = std::find(s_queue.begin(), s_queue.end(), this);
    if (index != s_queue.end()) {
        s_queue.erase(index);
    }
}
