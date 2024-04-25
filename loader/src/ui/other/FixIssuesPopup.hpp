#pragma once

#include <Geode/ui/Popup.hpp>
#include "../mods/GeodeStyle.hpp"
#include "../mods/list/ModProblemItemList.hpp"

using namespace geode::prelude;

class FixIssuesPopup : public GeodePopup<> {
protected:
    ModProblemItemList* m_list;

    bool setup() override;

public:
    static FixIssuesPopup* create();
};

void checkLoadingIssues(CCNode* targetScene = nullptr);
