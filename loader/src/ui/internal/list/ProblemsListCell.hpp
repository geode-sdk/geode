#pragma once

#include <Geode/binding/TableViewCell.hpp>
#include <Geode/binding/FLAlertLayerProtocol.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/ModMetadata.hpp>
#include <Geode/loader/Index.hpp>

using namespace geode::prelude;

class ProblemsListPopup;

class ProblemsListCell : public CCLayer {
protected:
    float m_width;
    float m_height;
    ProblemsListPopup* m_layer;
    CCMenu* m_menu;
    LoadProblem m_problem;
    std::string m_longMessage;

    bool init(LoadProblem problem, ProblemsListPopup* list, CCSize const& size);
    void draw() override;

    void onMore(CCObject*);

    float getLogoSize() const;

public:
    LoadProblem getProblem() const;

    static ProblemsListCell* create(LoadProblem problem, ProblemsListPopup* list, CCSize const& size);
};
