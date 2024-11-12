#include <Geode/ui/Popup.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>

using namespace geode::prelude;

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

class QuickPopup : public FLAlertLayer, public FLAlertLayerProtocol {
protected:
    std::function<void(FLAlertLayer*, bool)> m_selected;
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
        char const* title, std::string const& content, char const* btn1, char const* btn2,
        float width, std::function<void(FLAlertLayer*, bool)> selected, bool cancelledByEscape
    ) {
        auto inst = new QuickPopup;
        inst->m_selected = selected;
        inst->m_cancelledByEscape = cancelledByEscape;
        if (inst->init(inst, title, content, btn1, btn2, width, false, .0f, 1.0f)) {
            inst->autorelease();
            return inst;
        }

        delete inst;
        return nullptr;
    }
};

FLAlertLayer* geode::createQuickPopup(
    char const* title, std::string const& content, char const* btn1, char const* btn2, float width,
    std::function<void(FLAlertLayer*, bool)> selected, bool doShow
) {
    auto ret = QuickPopup::create(title, content, btn1, btn2, width, selected, false);
    if (doShow) {
        ret->show();
    }
    return ret;
}

FLAlertLayer* geode::createQuickPopup(
    char const* title, std::string const& content, char const* btn1, char const* btn2,
    std::function<void(FLAlertLayer*, bool)> selected, bool doShow
) {
    return createQuickPopup(title, content, btn1, btn2, 350.f, selected, doShow);
}

FLAlertLayer* geode::createQuickPopup(
    char const* title, std::string const& content, char const* btn1, char const* btn2, float width,
    std::function<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape
) {
    auto ret = QuickPopup::create(title, content, btn1, btn2, width, selected, cancelledByEscape);
    if (doShow) {
        ret->show();
    }
    return ret;
}

FLAlertLayer* geode::createQuickPopup(
    char const* title, std::string const& content, char const* btn1, char const* btn2,
    std::function<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape
) {
    return createQuickPopup(title, content, btn1, btn2, 350.f, selected, doShow, cancelledByEscape);
}
