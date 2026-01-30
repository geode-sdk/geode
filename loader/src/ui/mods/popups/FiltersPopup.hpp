#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include "../sources/ModListSource.hpp"
#include "../GeodeStyle.hpp"
#include <server/Server.hpp>

using namespace geode::prelude;

class FiltersPopup : public GeodePopup {
protected:
    ModListSource* m_source;
    CCMenu* m_tagsMenu;
    std::unordered_set<std::string> m_selectedTags;
    ListenerHandle m_tagsHandle;
    // EventListener<server::ServerFuture<std::vector<server::ServerTag>>> m_tagsListener;
    CCMenuItemToggler* m_enabledModsOnly = nullptr;
    CCMenuItemToggler* m_enabledModsFirst = nullptr;
    TextInput* m_developerNameInput = nullptr;

    bool init(ModListSource* src);
    void updateTags();
    
    // TODO: v5
    // void onLoadTags(typename server::ServerFuture<std::vector<server::ServerTag>>::Event* event);
    void onResetTags(CCObject*);
    void onResetDevName(CCObject*);
    void onSelectTag(CCObject* sender);
    void onClose(CCObject* sender) override;

public:
    static FiltersPopup* create(ModListSource* src);
};
