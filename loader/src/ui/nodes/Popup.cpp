#include <Geode/ui/Popup.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>

using namespace geode::prelude;
using CloseEvent = Popup::CloseEvent;
using CloseEventFilter = Popup::CloseEventFilter;

// static void fixChildPositions(CCNode* in, CCSize const& size) {
//     auto winSize = CCDirector::get()->getWinSize();
//     auto offset = size / 2 - in->getContentSize() / 2;

//     for (auto node : CCArrayExt<CCNode*>(in->getChildren())) {
//         node->setPosition(node->getPosition() + offset);

//         if (node->isIgnoreAnchorPointForPosition()) {
//             node->setPosition(node->getPosition() + node->getScaledContentSize() * node->getAnchorPoint());
//             node->ignoreAnchorPointForPosition(false);
//         }

//         constexpr int LEFT   = 0b0001;
//         constexpr int RIGHT  = 0b0010;
//         constexpr int BOTTOM = 0b0100;
//         constexpr int TOP    = 0b1000;

//         int p = 0b0000;
//         if (node->getPositionX() <= winSize.width / 2 - size.width * 0.25) {
//             p |= LEFT;
//         }
//         else if (node->getPositionX() >= winSize.width / 2 + size.width * 0.25) {
//             p |= RIGHT;
//         }
//         if (node->getPositionY() <= winSize.height / 2 - size.height * 0.25) {
//             p |= BOTTOM;
//         }
//         else if (node->getPositionY() >= winSize.height / 2 + size.height * 0.25) {
//             p |= TOP;
//         }

//         Anchor anchor = Anchor::Center;
//         switch (p) {
//             case LEFT | BOTTOM:  anchor = Anchor::BottomLeft; break;
//             case LEFT | TOP:     anchor = Anchor::TopLeft; break;
//             case LEFT:           anchor = Anchor::Left; break;
//             case RIGHT | BOTTOM: anchor = Anchor::BottomRight; break;
//             case RIGHT | TOP:    anchor = Anchor::TopRight; break;
//             case RIGHT:          anchor = Anchor::Right; break;
//             case TOP:            anchor = Anchor::Top; break;
//             case BOTTOM:         anchor = Anchor::Bottom; break;
//         }

//         auto anchorPos = AnchorLayout::getAnchoredPosition(in, anchor, ccp(0, 0));
//         node->setLayoutOptions(
//             AnchorLayoutOptions::create()
//                 ->setAnchor(anchor)
//                 ->setOffset(node->getPosition() - anchorPos)
//         );
//     }

//     in->ignoreAnchorPointForPosition(false);
// }

// void geode::enableDynamicLayoutForPopup(FLAlertLayer* alert, CCNode* bg) {
//     auto winSize = CCDirector::get()->getWinSize();

//     auto size = bg->getContentSize();

//     alert->m_mainLayer->ignoreAnchorPointForPosition(false);
//     alert->m_mainLayer->setContentSize(size);
//     alert->m_mainLayer->setPosition(winSize / 2);
//     alert->m_mainLayer->setLayout(AutoPopupLayout::create(alert->m_buttonMenu, bg));
// }

class CloseEvent::Impl final {
private:
    Popup* popup;
    friend class CloseEvent;
};

CloseEvent::CloseEvent(Popup* popup) : m_impl(std::make_shared<Impl>()) {
    m_impl->popup = popup;
}

Popup* CloseEvent::getPopup() const {
    return m_impl->popup;
}

bool CloseEvent::filter(Popup* popup) const {
    return m_impl->popup == popup; 
}

class CloseEventFilter::Impl final {
private:
    Popup* popup;
    friend class CloseEventFilter;
};

CloseEventFilter::CloseEventFilter(Popup* popup) : m_impl(std::make_shared<Impl>()) {
    m_impl->popup = popup;
}

ListenerResult CloseEventFilter::handle(geode::Function<Callback>& fn, CloseEvent* event) {
    if (event->getPopup() == m_impl->popup) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

// Popup impl

Popup::~Popup() {
    CCTouchDispatcher::get()->unregisterForcePrio(this);
}

void Popup::registerWithTouchDispatcher() {
    CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
}

bool Popup::init(
    float width, float height, char const* bg, CCRect bgRect
) {
    m_size = CCSize{ width, height };
    CCTouchDispatcher::get()->registerForcePrio(this, 2);

    if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
    
    auto winSize = CCDirector::get()->getWinSize();

    m_mainLayer = CCLayer::create();
    this->addChild(m_mainLayer);

    m_bgSprite = CCScale9Sprite::create(bg, bgRect);
    m_bgSprite->setContentSize(m_size);
    m_bgSprite->setPosition(winSize.width / 2, winSize.height / 2);
    m_mainLayer->addChild(m_bgSprite);

    m_buttonMenu = CCMenu::create();
    m_buttonMenu->setZOrder(100);
    m_mainLayer->addChild(m_buttonMenu);

    m_mainLayer->ignoreAnchorPointForPosition(false);
    m_mainLayer->setPosition(winSize / 2);
    m_mainLayer->setContentSize(m_size);
    m_mainLayer->setLayout(
        CopySizeLayout::create()
            ->add(m_buttonMenu)
            ->add(m_bgSprite)
    );

    this->setTouchEnabled(true);

    m_closeBtn = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_closeBtn_001.png", 0.8f, [this](auto btn) {
        this->onClose(btn);
    });
    m_buttonMenu->addChildAtPosition(m_closeBtn, geode::Anchor::TopLeft, { 3.f, -3.f });

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    return true;
}

void Popup::keyBackClicked() {
    this->onClose(nullptr);
}

void Popup::keyDown(enumKeyCodes key, double p1) {
    if (key == KEY_Space) return;
    return FLAlertLayer::keyDown(key, p1);
}

void Popup::onClose(CCObject*) {
    CloseEvent(this).post();
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParent();
}

void Popup::setTitle(
    ZStringView title,
    const char* font,
    float scale,
    float offset
) {
    if (!m_title) {
        m_title = CCLabelBMFont::create("", font);
        m_title->setZOrder(2);
        m_mainLayer->addChildAtPosition(m_title, Anchor::Top, {0, -offset});
    }
    
    m_title->setString(title.c_str());
    m_title->limitLabelWidth(m_size.width - 20.f, scale, .1f);
}

void Popup::setCloseButtonSpr(CCSprite* spr, float scale) {
    // Store original attributes of the close button
    auto origSize = m_closeBtn->getContentSize();
    Ref orig = m_closeBtn->getNormalImage();

    // Replace the close button sprite
    m_closeBtn->setNormalImage(spr);

    // Restore size and position
    spr->setScale(scale);
    spr->setPosition(orig->getPosition());
    spr->setAnchorPoint(orig->getAnchorPoint());
    m_closeBtn->setContentSize(origSize);
}

CloseEventFilter Popup::listenForClose() {
    return CloseEventFilter(this);
}

class QuickPopup : public FLAlertLayer, public FLAlertLayerProtocol {
protected:
    geode::Function<void(FLAlertLayer*, bool)> m_selected;
    bool m_cancelledByEscape;
    bool m_usedEscape = false;

    void keyBackClicked() override {
        m_usedEscape = true;
        FLAlertLayer::keyBackClicked();
    }

    void FLAlert_Clicked(FLAlertLayer* layer, bool btn2) override {
        if (m_cancelledByEscape && m_usedEscape) {
            return;
        }
        if (m_selected) {
            m_selected(layer, btn2);
        }
    }

public:
    static QuickPopup* create(
        char const* title, std::string content, char const* btn1, char const* btn2,
        float width, geode::Function<void(FLAlertLayer*, bool)> selected, bool cancelledByEscape
    ) {
        auto inst = new QuickPopup;
        inst->m_selected = std::move(selected);
        inst->m_cancelledByEscape = cancelledByEscape;
        if (inst->init(inst, title, std::move(content), btn1, btn2, width, false, .0f, 1.0f)) {
            inst->autorelease();
            return inst;
        }

        delete inst;
        return nullptr;
    }
};

FLAlertLayer* geode::createQuickPopup(
    char const* title, std::string content, char const* btn1, char const* btn2, float width,
    geode::Function<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape
) {
    auto ret = QuickPopup::create(title, std::move(content), btn1, btn2, width, std::move(selected), cancelledByEscape);
    if (doShow) {
        ret->show();
    }
    return ret;
}

FLAlertLayer* geode::createQuickPopup(
    char const* title, std::string content, char const* btn1, char const* btn2,
    geode::Function<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape
) {
    return createQuickPopup(title, std::move(content), btn1, btn2, 350.f, std::move(selected), doShow, cancelledByEscape);
}
