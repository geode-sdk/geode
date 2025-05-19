#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include "../sources/ModListSource.hpp"
#include "../GeodeStyle.hpp"
#include <server/Server.hpp>

using namespace geode::prelude;

class FiltersPopup : public GeodePopup<ModListSource*> {
protected:
    ModListSource* m_source;
    CCMenu* m_tagsMenu;
    std::unordered_set<std::string> m_selectedTags;
    EventListener<server::ServerRequest<std::vector<server::ServerTag>>> m_tagsListener;
    CCMenuItemToggler* m_enabledModsOnly = nullptr;
    CCMenuItemToggler* m_enabledModsFirst = nullptr;
    TextInput* m_developerNameInput = nullptr;

    bool setup(ModListSource* src) override;
    void updateTags();

    void onLoadTags(typename server::ServerRequest<std::vector<server::ServerTag>>::Event* event);
    void onResetTags(CCObject*);
    void onResetDevName(CCObject*);
    void onSelectTag(CCObject* sender);
    void onClose(CCObject* sender) override;

public:
    static FiltersPopup* create(ModListSource* src);
};
