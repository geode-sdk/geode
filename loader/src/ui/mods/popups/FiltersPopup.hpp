#pragma once

#include <Geode/ui/Popup.hpp>
#include "../sources/ModListSource.hpp"
#include "../GeodeStyle.hpp"
#include <server/Server.hpp>

using namespace geode::prelude;

class FiltersPopup : public GeodePopup<ModListSource*> {
protected:
    ModListSource* m_source;
    CCMenu* m_tagsMenu;
    std::unordered_set<std::string> m_selectedTags;
    EventListener<PromiseEventFilter<std::unordered_set<std::string>, server::ServerError>> m_tagsListener;

    bool setup(ModListSource* src) override;
    void updateTags();

    void onLoadTags(PromiseEvent<std::unordered_set<std::string>, server::ServerError>* event);
    void onResetTags(CCObject*);
    void onSelectTag(CCObject* sender);
    void onClose(CCObject* sender) override;

public:
    static FiltersPopup* create(ModListSource* src);
};
