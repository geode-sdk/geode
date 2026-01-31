#pragma once

#include "ModListItem.hpp"
#include "../sources/ModListSource.hpp"

class SpecialModListItem : public ModListItem {
protected:
    SpecialModListItemSource m_item;

    bool init(SpecialModListItemSource&& item);

public:
    static SpecialModListItem* create(SpecialModListItemSource&& item);
};
