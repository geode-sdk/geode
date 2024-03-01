#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include "ModSource.hpp"

using namespace geode::prelude;

enum class ModMarkdownData {
    Details,
    Changelog,
};

std::optional<std::string> getModMarkdownData(ModSource const& src, ModMarkdownData data);

class ModPopup : public Popup<ModSource&&> {
protected:
    ModSource m_source;
    MDTextArea* m_mdArea;
    CCMenu* m_tabsMenu;

    bool setup(ModSource&& src) override;

    void onMDTab(CCObject* sender);

public:
    static ModPopup* create(ModSource&& src);
};
