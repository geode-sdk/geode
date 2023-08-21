#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class QuickPopup : public FLAlertLayer, public FLAlertLayerProtocol {
protected:
    MiniFunction<void(FLAlertLayer*, bool)> m_selected;
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
        float width, MiniFunction<void(FLAlertLayer*, bool)> selected, bool cancelledByEscape
    ) {
        auto inst = new QuickPopup;
        inst->m_selected = selected;
        inst->m_cancelledByEscape = cancelledByEscape;
        if (inst && inst->init(inst, title, content, btn1, btn2, width, false, .0f)) {
            inst->autorelease();
            return inst;
        }
        CC_SAFE_DELETE(inst);
        return nullptr;
    }
};

FLAlertLayer* geode::createQuickPopup(
    char const* title, std::string const& content, char const* btn1, char const* btn2, float width,
    MiniFunction<void(FLAlertLayer*, bool)> selected, bool doShow
) {
    auto ret = QuickPopup::create(title, content, btn1, btn2, width, selected, false);
    if (doShow) {
        ret->show();
    }
    return ret;
}

FLAlertLayer* geode::createQuickPopup(
    char const* title, std::string const& content, char const* btn1, char const* btn2,
    MiniFunction<void(FLAlertLayer*, bool)> selected, bool doShow
) {
    return createQuickPopup(title, content, btn1, btn2, 350.f, selected, doShow);
}

FLAlertLayer* geode::createQuickPopup(
    char const* title, std::string const& content, char const* btn1, char const* btn2, float width,
    MiniFunction<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape
) {
    auto ret = QuickPopup::create(title, content, btn1, btn2, width, selected, cancelledByEscape);
    if (doShow) {
        ret->show();
    }
    return ret;
}

FLAlertLayer* geode::createQuickPopup(
    char const* title, std::string const& content, char const* btn1, char const* btn2,
    MiniFunction<void(FLAlertLayer*, bool)> selected, bool doShow, bool cancelledByEscape
) {
    return createQuickPopup(title, content, btn1, btn2, 350.f, selected, doShow, cancelledByEscape);
}
