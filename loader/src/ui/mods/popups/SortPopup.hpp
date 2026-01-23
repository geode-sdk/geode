#pragma once

#include <Geode/ui/Popup.hpp>
#include "../sources/ModListSource.hpp"
#include "../GeodeStyle.hpp"
#include <server/Server.hpp>

using namespace geode::prelude;

class SortPopup : public GeodePopup {
protected:
    ModListSource* m_source;
    std::vector<CCMenuItemToggler*> m_options;
    server::ModsSort m_selected;

    bool init(ModListSource* src);

    void onClose(CCObject* sender) override;
    void onSelect(CCObject*);

public:
    static SortPopup* create(ModListSource* src);
};
