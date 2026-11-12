#pragma once

#include "../GeodeStyle.hpp"
#include "../sources/ModSource.hpp"
#include <server/Server.hpp>

class DevListPopup : public GeodePopup {
protected:
    void createList();
    bool init(ModSource source);

    ModSource m_source;

public:
    static DevListPopup* create(ModSource source);
};