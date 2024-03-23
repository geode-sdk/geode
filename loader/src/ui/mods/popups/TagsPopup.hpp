#pragma once

#include <Geode/ui/Popup.hpp>
#include "../sources/ModListSource.hpp"
#include "../GeodeStyle.hpp"

using namespace geode::prelude;

class TagsPopup : public GeodePopup<ModListSource*, MiniFunction<void()>> {
protected:
    ModListSource* m_source;
    MiniFunction<void()> m_onClose;

    bool setup(ModListSource* src, MiniFunction<void()> onClose) override;

    void onClose(CCObject*) override;

public:
    static TagsPopup* create(ModListSource* src, MiniFunction<void()> onClose);
};
