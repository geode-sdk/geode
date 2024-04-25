#include "FixIssuesPopup.hpp"
#include <Geode/loader/Loader.hpp>

bool FixIssuesPopup::setup() {
    m_noElasticity = true;

    this->setTitle("Problems Loading Mods");

    return true;
}

FixIssuesPopup* FixIssuesPopup::create() {
    auto ret = new FixIssuesPopup();
    if (ret && ret->init(350, 280)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void checkLoadingIssues(CCNode* targetScene) {
    if (Loader::get()->getProblems().size()) {
        auto popup = FixIssuesPopup::create();
        popup->m_scene = targetScene;
        popup->show();
    }
}
