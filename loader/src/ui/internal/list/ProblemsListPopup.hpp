#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/loader/Loader.hpp>

using namespace geode::prelude;

class ProblemsListPopup : public Popup<Mod*> {
protected:
    CCNode* m_listParent;
    ListView* m_list;

    bool setup(Mod* scrollTo) override;

    void createList(Mod* scrollTo);
    CCArray* createCells(Mod* scrollTo, float& scrollValue);
    CCSize getCellSize() const;
    CCSize getListSize() const;

public:
    static ProblemsListPopup* create(Mod* scrollTo);
};
