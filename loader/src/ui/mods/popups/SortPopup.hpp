#pragma once

#include <Geode/ui/Popup.hpp>
#include "../sources/ModListSource.hpp"
#include "../GeodeStyle.hpp"

using namespace geode::prelude;

class SortPopup : public GeodePopup {
protected:
    ModListSource* m_source;
    std::vector<CCMenuItemToggler*> m_options;
    size_t m_selected;

    bool init(ModListSource* src);

    void onClose(CCObject* sender) override;
    void onSelect(CCObject*);

public:
    static SortPopup* create(ModListSource* src);
};
