#pragma once

#include "HookListView.hpp"

class HookListLayer : public GJDropDownLayer {
    protected:
        bool init(Mod* mod);

    public:
        static HookListLayer* create(Mod* mod);
};
