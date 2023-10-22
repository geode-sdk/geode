#pragma once

#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ModListLayer;

class DevProfilePopup : public Popup<std::string const&, ModListLayer*> {
protected:
    ModListLayer* m_layer;

    bool setup(std::string const& developer, ModListLayer* list) override;

public:
    static DevProfilePopup* create(std::string const& developer, ModListLayer* list);
};
