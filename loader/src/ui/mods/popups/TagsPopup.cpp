#include "TagsPopup.hpp"

bool TagsPopup::setup(ModListSource* src, MiniFunction<void()> onClose) {
    m_noElasticity = true;
    m_source = src;
    m_onClose = onClose;

    this->setTitle("Select Tags");

    // todo: need a "get available tags" endpoint first...

    return true;
}

void TagsPopup::onClose(CCObject* sender) {
    if (m_onClose) m_onClose();
    Popup::onClose(sender);
}

TagsPopup* TagsPopup::create(ModListSource* src, MiniFunction<void()> onClose) {
    auto ret = new TagsPopup();
    if (ret && ret->init(260, 200, src, onClose)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
