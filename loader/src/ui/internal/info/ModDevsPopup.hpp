#pragma once

#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ModListLayer;

class ModDevsPopup : public Popup<ModMetadata const&, ModListLayer*> {
protected:
    ModListLayer* m_layer;

    bool setup(ModMetadata const& mod, ModListLayer* list) override;

    void onViewDev(CCObject* sender);

public:
    static ModDevsPopup* create(ModMetadata const& mod, ModListLayer* list);
};
