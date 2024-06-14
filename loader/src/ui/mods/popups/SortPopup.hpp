#pragma once

#include <Geode/ui/Popup.hpp>
#include "../sources/ModListSource.hpp"
#include "../GeodeStyle.hpp"
#include <server/Server.hpp>

using namespace geode::prelude;

class SortPopup : public GeodePopup<ModListSource*> {
protected:
    ModListSource* m_source;
    std::vector<CCMenuItemToggler*> m_options;
    size_t m_selected;

    bool setup(ModListSource* src) override;

    void onClose(CCObject* sender) override;
    void onSelect(CCObject*);

public:
    static SortPopup* create(ModListSource* src);
};
