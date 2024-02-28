#pragma once

#include <Geode/ui/Popup.hpp>
#include "ModListSource.hpp"

using namespace geode::prelude;

class TagsPopup : public Popup<ModListSource*, MiniFunction<void()>> {
protected:
    ModListSource* m_source;
    MiniFunction<void()> m_onClose;

    bool setup(ModListSource* src, MiniFunction<void()> onClose) override;

    void onClose(CCObject*) override;

public:
    static TagsPopup* create(ModListSource* src, MiniFunction<void()> onClose);
};
