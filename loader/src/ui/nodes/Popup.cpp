#include <Geode/Enums.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>

using namespace geode::prelude;

Popup::~Popup() {
    CCTouchDispatcher::get()->unregisterForcePrio(this);
}

void Popup::registerWithTouchDispatcher() {
    CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
}

bool Popup::init(float width, float height, ZStringView bg, CCRect bgRect) {
    m_size = CCSize{width, height};

    CCTouchDispatcher::get()->registerForcePrio(this, 2, 0);

    if (!this->initWithColor({0, 0, 0, 105})) return false;

    m_noElasticity = GameManager::get()->getGameVariable(GameVar::FastMenu);

    auto winSize = CCDirector::get()->getWinSize();

    m_mainLayer = CCLayer::create();
    m_mainLayer->setID("main-layer");
    this->addChild(m_mainLayer);

    m_bgSprite = NineSlice::create(bg, bgRect);
    m_bgSprite->setID("background");
    m_bgSprite->setContentSize(m_size);
    m_bgSprite->setPosition(winSize / 2.f);
    m_mainLayer->addChild(m_bgSprite);

    m_buttonMenu = CCMenu::create();
    m_buttonMenu->setID("button-menu");
    m_buttonMenu->setZOrder(100);
    m_mainLayer->addChild(m_buttonMenu);

    m_mainLayer->ignoreAnchorPointForPosition(false);
    m_mainLayer->setPosition(winSize / 2.f);
    m_mainLayer->setContentSize(m_size);
    m_mainLayer->setLayout(CopySizeLayout::create()->add(m_buttonMenu)->add(m_bgSprite));

    this->setTouchEnabled(true);

    m_closeBtn = Button::createWithSpriteFrameName(
        "GJ_closeBtn_001.png",
        [this](auto btn) {
            this->onClose(btn);
        });
    m_closeBtn->setID("close-button");
    m_closeBtn->setScale(.8f);
    m_closeBtn->setZOrder(100);
    m_mainLayer->addChildAtPosition(m_closeBtn, Anchor::TopLeft, {3.f, -3.f});

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    return true;
}

bool Popup::init(CCSize size, ZStringView bg, CCRect bgRect) {
    return this->init(size.width, size.height, bg, bgRect);
}

void Popup::keyBackClicked() {
    this->onClose(nullptr);
}

void Popup::keyDown(enumKeyCodes key, double p1) {
    if (key == KEY_Space) return;
    return FLAlertLayer::keyDown(key, p1);
}

void Popup::onClose(CCObject*) {
    CloseEvent(this).send();
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParent();
}

void Popup::setTitle(std::string title, ZStringView font, float scale, float offset) {
    if (!m_title) {
        m_title = Label::create("", font);
        m_title->setID("title-label");
        m_title->setZOrder(2);
        m_mainLayer->addChildAtPosition(m_title, Anchor::Top, {0, -offset});
    }

    m_title->setText(std::move(title));
    m_title->setLimitLabelWidth(m_size.width - 20.f, scale, .1f);
}

void Popup::setCloseButtonSpr(CCNode* spr, float scale) {
    m_closeBtn->setDisplayNode(spr);
    m_closeBtn->setScale(scale);
}

Popup::CloseEvent Popup::listenForClose() {
    return Popup::CloseEvent(this);
}

class QuickPopup : public FLAlertLayer, public FLAlertLayerProtocol {
protected:
    Function<void(FLAlertLayer*, bool)> m_selected;
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
        ZStringView title, std::string content, ZStringView btn1, ZStringView btn2, float width,
        Function<void(FLAlertLayer*, bool)> selected, bool cancelledByEscape
    ) {
        auto inst = new QuickPopup;
        inst->m_selected = std::move(selected);
        inst->m_cancelledByEscape = cancelledByEscape;

        if (inst->init(
            inst, title.c_str(), std::move(content),
            btn1.c_str(), btn2.empty() ? nullptr : btn2.c_str(),
            width, false, .0f, 1.f
        )) {
            inst->autorelease();
            return inst;
        }

        delete inst;
        return nullptr;
    }
};

FLAlertLayer* geode::createQuickPopup(
    ZStringView title, std::string content, ZStringView btn1, ZStringView btn2, float width,
    Function<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape
) {
    auto ret = QuickPopup::create(title, std::move(content), btn1, btn2, width, std::move(selected), cancelledByEscape);
    if (doShow) {
        ret->show();
    }
    return ret;
}

FLAlertLayer* geode::createQuickPopup(
    ZStringView title, std::string content, ZStringView btn1, ZStringView btn2,
    Function<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape
) {
    return createQuickPopup(title, std::move(content), btn1, btn2, 350.f, std::move(selected), doShow, cancelledByEscape);
}