#pragma once

#include <Geode/ui/Popup.hpp>
#include "../sources/ModListSource.hpp"
#include "../GeodeStyle.hpp"

using namespace geode::prelude;

class TagsPopup : public GeodePopup<ModListSource*> {
protected:
    ModListSource* m_source;

    bool setup(ModListSource* src) override;

    void onClose(CCObject*) override;

public:
    static TagsPopup* create(ModListSource* src);
};
