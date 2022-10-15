#include <Geode/ui/Popup.hpp>

USE_GEODE_NAMESPACE();

class QuickPopup : public FLAlertLayer, public FLAlertLayerProtocol {
protected:
    std::function<void(FLAlertLayer*, bool)> m_selected;

    void FLAlert_Clicked(FLAlertLayer* layer, bool btn2) override {
        if (m_selected) {
            m_selected(layer, btn2);
        }
    }

public:
    static QuickPopup* create(
        const char* title,
        std::string const& content,
        const char* btn1,
        const char* btn2,
        float width,
        std::function<void(FLAlertLayer*, bool)> selected
    ) {
        auto inst = new QuickPopup;
        inst->m_selected = selected;
        if (inst && inst->init(
            inst, title, content,
            btn1, btn2, width, false, .0f
        )) {
            inst->autorelease();
            return inst;
        }
        CC_SAFE_DELETE(inst);
        return nullptr;
    }
};

FLAlertLayer* geode::createQuickPopup(
    const char* title,
    std::string const& content,
    const char* btn1,
    const char* btn2,
    float width,
    std::function<void(FLAlertLayer*, bool)> selected,
    bool doShow
) {
    auto ret = QuickPopup::create(
        title,
        content,
        btn1, btn2,
        width,
        selected
    );
    if (doShow) {
        ret->show();
    }
    return ret;
}

FLAlertLayer* geode::createQuickPopup(
    const char* title,
    std::string const& content,
    const char* btn1,
    const char* btn2,
    std::function<void(FLAlertLayer*, bool)> selected,
    bool doShow
) {
    return createQuickPopup(
        title, content, btn1, btn2, 350.f,
        selected, doShow
    );
}
