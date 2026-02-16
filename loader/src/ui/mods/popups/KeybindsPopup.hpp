#pragma once

#include "../GeodeStyle.hpp"

class KeybindsPopup : public GeodePopup {
protected:
    bool init();

public:
    static KeybindsPopup* create();
};
