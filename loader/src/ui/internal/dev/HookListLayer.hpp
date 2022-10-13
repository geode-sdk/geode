#pragma once

#include "HookListView.hpp"
#include <Geode/binding/GJDropDownLayer.hpp>

class HookListLayer : public GJDropDownLayer {
    protected:
        bool init(Mod* mod);

    public:
        static HookListLayer* create(Mod* mod);
};
