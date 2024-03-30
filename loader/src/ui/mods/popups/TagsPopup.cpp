#include "TagsPopup.hpp"

bool TagsPopup::setup(ModListSource* src) {
    m_noElasticity = true;
    m_source = src;

    this->setTitle("Select Tags");

    // todo: need a "get available tags" endpoint first...

    return true;
}

void TagsPopup::onClose(CCObject* sender) {
    InvalidateCacheEvent(m_source).post();
    Popup::onClose(sender);
}

TagsPopup* TagsPopup::create(ModListSource* src) {
    auto ret = new TagsPopup();
    if (ret && ret->init(260, 200, src)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
