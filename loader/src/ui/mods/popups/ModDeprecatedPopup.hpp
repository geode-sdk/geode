#pragma once

#include <Geode/ui/Popup.hpp>
#include "../GeodeStyle.hpp"
#include <server/Server.hpp>

using namespace geode::prelude;

class ModDeprecatedPopup : public GeodePopup {
protected:
    server::ServerModDeprecation m_deprecation;

    bool init(Mod* mod, server::ServerModDeprecation dep);

public:
    static ModDeprecatedPopup* create(Mod* mod, server::ServerModDeprecation dep);
};
