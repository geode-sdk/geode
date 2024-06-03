#pragma once

#include "../GeodeStyle.hpp"
#include "../sources/ModSource.hpp"
#include <server/Server.hpp>

class DevListPopup : public GeodePopup<ModMetadata const&> {
protected:
    bool setup(ModMetadata const& src);

    void onMoreByThisDev(CCObject* sender);

public:
    static DevListPopup* create(ModMetadata const& src);
};
