#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/loader/Index.hpp>

#include "InstallListCell.hpp"

using namespace geode::prelude;

class InstallListPopup : public Popup<IndexItemHandle, MiniFunction<void(IndexInstallList const&)>> {
protected:
    IndexItemHandle m_item;
    CCNode* m_listParent;
    ListView* m_list;
    MiniFunction<void(IndexInstallList const&)> m_callback;

    bool setup(IndexItemHandle item, MiniFunction<void(IndexInstallList const&)> callback) override;

    void createList();
    CCArray* createCells(std::unordered_map<std::string, InstallListCell*> const& oldCells);
    CCSize getCellSize() const;
    CCSize getListSize() const;

    void onInstall(CCObject* obj);

public:
    void onCellToggle(CCObject* obj);

    static InstallListPopup* create(IndexItemHandle item, MiniFunction<void(IndexInstallList const&)> onInstall);
};
