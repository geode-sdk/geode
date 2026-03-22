#include "SpecialModListItem.hpp"

bool SpecialModListItem::init(SpecialModListItemSource&& item) {
    if (!ModListItem::init())
        return false;

    m_item = std::move(item);

    return true;
}

SpecialModListItem* SpecialModListItem::create(SpecialModListItemSource&& item) {
    auto ret = new SpecialModListItem();
    if (ret->init(std::move(item))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
