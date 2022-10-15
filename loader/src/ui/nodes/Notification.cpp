#include <Geode/ui/Notification.hpp>
#include <Geode/ui/TextRenderer.hpp>
#include <Geode/binding/GameSoundManager.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/container.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Mod.hpp>

USE_GEODE_NAMESPACE();

// todo: make sure notifications dont disappear 
// off the screen if the user happens to switch 
// scenes or smth that causes actions from being 
// run / completed

Notification::Notification() {}

Notification::~Notification() {
    CCDirector::sharedDirector()->getTouchDispatcher()->decrementForcePrio(2);
}

void Notification::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
        this,
        0,
        true
    );
}

static bool isHovered(CCNode* node, CCTouch* touch) {
    auto csize = node->getScaledContentSize();
    if (
        CCRect {
            node->getPositionX() - csize.width / 2,
            node->getPositionY() - csize.height / 2,
            csize.width,
            csize.height
        }.containsPoint(touch->getLocation())
    ) {
        return true;
    }
    return false;
}

static bool shouldHideNotification(
    CCTouch* touch, NotificationLocation const& location
) {
    static constexpr const float HIDE_THRESHOLD = 20.f;
    auto dist = touch->getLocation() - touch->getStartLocation();
    switch (location) {
        case NotificationLocation::BottomLeft:
        case NotificationLocation::TopLeft:
            return dist.x < -HIDE_THRESHOLD;

        case NotificationLocation::BottomRight:
        case NotificationLocation::TopRight:
            return dist.x > HIDE_THRESHOLD;

        case NotificationLocation::BottomCenter:
            return dist.y < -HIDE_THRESHOLD;

        case NotificationLocation::TopCenter:
            return dist.y > HIDE_THRESHOLD;
    }
    return false;
}

void Notification::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    auto dist = touch->getLocation() - touch->getStartLocation();
    switch (m_location) {
        case NotificationLocation::BottomLeft:
        case NotificationLocation::TopLeft:
            this->setPositionX(m_posAtTouchStart.x + dist.x);
            if (this->getPositionX() > m_showDest.x) {
                this->setPositionX(m_showDest.x);
            }
            break;
            
        case NotificationLocation::BottomRight:
        case NotificationLocation::TopRight:
            this->setPositionX(m_posAtTouchStart.x + dist.x);
            if (this->getPositionX() < m_showDest.x) {
                this->setPositionX(m_showDest.x);
            }
            break;

        case NotificationLocation::BottomCenter:
            this->setPositionY(m_posAtTouchStart.y + dist.y);
            if (this->getPositionY() > m_showDest.y) {
                this->setPositionY(m_showDest.y);
            }
            break;

        case NotificationLocation::TopCenter:
            this->setPositionY(m_posAtTouchStart.y + dist.y);
            if (this->getPositionY() < m_showDest.y) {
                this->setPositionY(m_showDest.y);
            }
            break;
    }
    auto clicking = !shouldHideNotification(touch, m_location);
    if (m_clicking != clicking) {
        m_clicking = clicking;
        this->animateClicking();
    }
    auto hovered = isHovered(this, touch);
    if (m_hovered != hovered) {
        m_hovered = hovered;
        if (hovered) {
            m_bg->setColor({ 150, 150, 150 });
        } else {
            m_bg->setColor({ 255, 255, 255 });
        }
        this->animateClicking();
    }
}

bool Notification::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (!isHovered(this, touch)) {
        return false;
    }
    m_bg->setColor({ 150, 150, 150 });
    this->stopAllActions();
    m_posAtTouchStart = this->getPosition();
    m_clicking = true;
    m_hovered = true;
    this->animateClicking();
    return true;
}

void Notification::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    m_clicking = false;
    m_hovered = false;
    this->animateClicking();
    m_bg->setColor({ 255, 255, 255 });
    if (shouldHideNotification(touch, m_location)) {
        return this->hide();
    }
    if (isHovered(this, touch)) {
        this->animateIn();
        this->clicked();
    }
}

void Notification::clicked() {
    if (m_callback) {
        m_callback(this);
        if (m_hideOnClicked) {
            this->animateOutClicked();
        }
    }
}

bool Notification::init(
    Mod* owner,
    std::string const& title,
    std::string const& text,
    CCNode* icon,
    const char* bg,
    std::function<void(Notification*)> callback,
    bool hideOnClick
) {
    if (!CCLayer::init())
        return false;
    
    m_owner = owner;
    m_callback = callback;
    m_hideOnClicked = hideOnClick;

    // m_labels is Ref so no need to call 
    // retain manually
    m_labels = CCArray::create();

    m_bg = CCScale9Sprite::create(bg);
    m_bg->setScale(.6f);

    // using TextRenderer to create the text 
    // so it automatically wraps the lines
    auto renderer = TextRenderer::create();
    renderer->begin(this, CCPointZero, { 120.f, 20.f });

    renderer->pushBMFont("chatFont.fnt");
    renderer->pushScale(.4f);
    for (auto& label : renderer->renderString(
        text + "\n(from " + owner->getName() + ")"
    )) {
        m_labels->addObject(label.m_node);
    }

    renderer->end();
    renderer->release();

    // add icon
    float iconSpace = .0f;
    if (icon) {
        m_icon = icon;
        iconSpace = 20.f;
        m_icon->setPosition({
            -m_obContentSize.width / 2 + iconSpace / 2,
            .0f
        });
        limitNodeSize(m_icon,
            { iconSpace - 8.f, m_obContentSize.height - 8.f },
            1.f, .1f
        );
        this->addChild(m_icon);
    }

    // add title
    if (title.size()) {
        m_title = CCLabelBMFont::create(title.c_str(), "goldFont.fnt");
        m_title->limitLabelWidth(m_obContentSize.width - iconSpace, .4f, .01f);
        m_obContentSize.height += 14;
        m_title->setPosition(
            -m_obContentSize.width / 2 + iconSpace,
            m_obContentSize.height / 2 - 6.f
        );
        m_title->setAnchorPoint({ .0f, .5f });
        this->addChild(m_title);
    }

    // move text content if an icon is present
    m_obContentSize.width += iconSpace;
    m_icon->setPositionX(m_icon->getPositionX() - iconSpace / 2);
    m_title->setPositionX(m_title->getPositionX() - iconSpace / 2);
    for (auto label : CCArrayExt<CCNode>(m_labels)) {
        label->setPosition(
            label->getPositionX() + iconSpace - m_obContentSize.width / 2,
            label->getPositionY() - m_obContentSize.height / 2 + 2.f
        );
    }

    // fit bg to content
    m_bg->setContentSize(
        m_obContentSize / m_bg->getScale() + CCSize { 6.f, 6.f }
    );
    m_bg->setPosition(0, 0);
    m_bg->setZOrder(-1);
    this->addChild(m_bg);

    // set anchor point to middle so the 
    // notification properly scales
    this->setAnchorPoint({ .0f, .0f });
    this->setVisible(false);

    // make sure ~CCLayer properly removes 
    // the notification from touch dispatcher
    this->setTouchEnabled(true);

    // make this notification the most important 
    // touch fella on the screen
    CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
    this->registerWithTouchDispatcher();

    return true;
}

Notification* Notification::create(
    Mod* owner,
    std::string const& title,
    std::string const& text,
    CCNode* icon,
    const char* bg,
    std::function<void(Notification*)> callback,
    bool hideOnClick
) {
    auto ret = new Notification();
    if (ret && ret->init(
        owner, title, text, icon, bg, callback, hideOnClick
    )) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void Notification::showForReal() {
    if (!m_pParent) {
        CCDirector::sharedDirector()->getRunningScene()->addChild(this);
    }
    SceneManager::get()->keepAcrossScenes(this);
    // haha i am incredibly mature
    this->setZOrder(0xB00B1E5);
    this->setVisible(true);

    static constexpr const float pad = 15.f;

    float xMovement = .0f, yMovement = .0f;
    float xStart = .0f, yStart = .0f;
    switch (m_location) {
        case NotificationLocation::TopLeft: {
            xMovement = this->getScaledContentSize().width + pad * 2;
            xStart = -this->getScaledContentSize().width / 2 - pad;
            yStart = m_pParent->getContentSize().height
                - pad - this->getScaledContentSize().height / 2;
        } break;

        case NotificationLocation::BottomLeft: {
            xMovement = this->getScaledContentSize().width + pad * 2;
            xStart = -this->getScaledContentSize().width / 2 - pad;
            yStart = pad + this->getScaledContentSize().height / 2;
        } break;

        case NotificationLocation::TopRight: {
            xMovement = -this->getScaledContentSize().width - pad * 2;
            xStart = m_pParent->getContentSize().width +
                this->getScaledContentSize().width / 2 + pad;
            yStart = m_pParent->getContentSize().height
                - pad - this->getScaledContentSize().height / 2;
        } break;

        case NotificationLocation::BottomRight: {
            xMovement = -this->getScaledContentSize().width - pad * 2;
            xStart = m_pParent->getContentSize().width +
                this->getScaledContentSize().width / 2 + pad;
            yStart = pad + this->getScaledContentSize().height / 2;
        } break;

        case NotificationLocation::BottomCenter: {
            yMovement = pad * 2 + this->getScaledContentSize().height;
            xStart = m_pParent->getContentSize().width / 2;
            yStart = -pad - this->getScaledContentSize().height / 2;
        } break;

        case NotificationLocation::TopCenter: {
            yMovement = -pad * 2 - this->getScaledContentSize().height;
            xStart = m_pParent->getContentSize().width / 2;
            yStart = m_pParent->getContentSize().height + pad +
                this->getScaledContentSize().height / 2;
        } break;
    }

    m_hideDest = CCPoint { xStart, yStart };
    m_showDest = CCPoint { xStart + xMovement, yStart + yMovement };

    GameSoundManager::sharedManager()->playEffect(
        "newNotif03.ogg"_spr, 1.f, 1.f, 1.f
    );

    this->setPosition(xStart, yStart);
    this->animateIn();
}

void Notification::hide() {
    // if this notification has already been hidden, 
    // don't do anything
    if (m_hiding || !NotificationManager::get()->isInQueue(this)) {
        return;
    }
    GameSoundManager::sharedManager()->playEffect(
        "byeNotif00.ogg"_spr, 1.f, 1.f, 1.f
    );
    m_hiding = true;
    this->animateOut();
}

void Notification::animateIn() {
    this->runAction(CCEaseInOut::create(
        CCMoveTo::create(.3f, m_showDest),
        6.f
    ));
    if (m_time) {
        this->runAction(CCSequence::create(
            CCDelayTime::create(m_time),
            CCCallFunc::create(this, callfunc_selector(Notification::hide)),
            nullptr
        ));
    }
}

void Notification::animateOut() {
    this->runAction(CCSequence::create(
        CCEaseInOut::create(
            CCMoveTo::create(.3f, { m_hideDest }),
            6.f
        ),
        CCCallFunc::create(this, callfunc_selector(Notification::hidden)),
        nullptr
    ));
}

void Notification::animateOutClicked() {
    this->runAction(CCSequence::create(
        CCEaseBackIn::create(
            CCScaleTo::create(.2f, .0f)
        ),
        CCCallFunc::create(this, callfunc_selector(Notification::hidden)),
        nullptr
    ));
}

void Notification::animateClicking() {
    this->runAction(CCEaseInOut::create(
        CCScaleTo::create(.1f, (
            (m_clicking && m_hovered) ? m_targetScale * .9f : m_targetScale
        )), 2.f
    ));
}

void Notification::show(NotificationLocation location, float time) {
    if (location == NotificationLocation::TopCenter) {
        // the notification is larger at top center to 
        // be more easily readable on mobile
        this->setScale(1.5f);
    } else {
        this->setScale(1.2f);
    }
    m_targetScale = m_fScaleX;

    m_time = time;
    m_location = location;
    NotificationManager::get()->push(this);
}

void Notification::hidden() {
    NotificationManager::get()->pop(this);
    this->removeFromParent();
    SceneManager::get()->forget(this);
}

NotificationBuilder Notification::build() {
    return std::move(NotificationBuilder());
}

Notification* NotificationBuilder::show() {
    auto icon = m_iconNode;
    if (!icon && m_icon.size()) {
        icon = CCSprite::create(m_icon.c_str());
        if (!icon) icon = CCSprite::createWithSpriteFrameName(m_icon.c_str());
    }
    auto notif = Notification::create(
        m_owner, m_title, m_text, icon,
        m_bg.c_str(), m_callback, m_hideOnClick
    );
    notif->show(m_location, m_time);
    return notif;
}


bool NotificationManager::isInQueue(Notification* notification) {
    auto location = notification->m_location;
    if (m_notifications.count(location)) {
        return utils::ranges::contains(
            m_notifications.at(location), Ref(notification)
        );
    }
    return false;
}

void NotificationManager::push(Notification* notification) {
    auto location = notification->m_location;
    if (!m_notifications.count(location)) {
        m_notifications[location] = { notification };
        notification->showForReal();
    } else {
        m_notifications[location].push_back(notification);
    }
}

void NotificationManager::pop(Notification* notification) {
    auto location = notification->m_location;
    if (m_notifications.count(location)) {
        auto ref = Ref(notification);
        ranges::remove(m_notifications.at(location), ref);
        if (!m_notifications.at(location).size()) {
            m_notifications.erase(location);
        } else {
            m_notifications.at(location).front()->showForReal();
        }
    }
}

NotificationManager* NotificationManager::get() {
    static auto inst = new NotificationManager;
    return inst;
}
