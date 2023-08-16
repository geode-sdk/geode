#include "ProblemsListPopup.hpp"
#include "ProblemsListCell.hpp"

#include <utility>
#include <queue>

bool ProblemsListPopup::setup(Mod* scrollTo) {
    m_noElasticity = true;
    this->setTitle("Problems");
    this->createList(scrollTo);
    return true;
}

void ProblemsListPopup::createList(Mod* scrollTo) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_listParent = CCNode::create();
    m_listParent->setPositionY(-7.f);
    m_mainLayer->addChild(m_listParent);

    float scroll = 0.f;
    auto items = this->createCells(scrollTo, scroll);
    m_list = ListView::create(
        items,
        this->getCellSize().height,
        this->getListSize().width,
        this->getListSize().height
    );
    m_list->setPosition(winSize / 2 - m_list->getScaledContentSize() / 2);
    m_listParent->addChild(m_list);

    m_list->m_tableView->m_contentLayer->setPositionY(m_list->m_tableView->m_contentLayer->getPositionY() + scroll);

    addListBorders(m_listParent, winSize / 2, m_list->getScaledContentSize());
}

CCArray* ProblemsListPopup::createCells(Mod* scrollTo, float& scrollValue) {
    std::vector<ProblemsListCell*> top;
    std::vector<ProblemsListCell*> middle;
    std::vector<ProblemsListCell*> bottom;

    for (auto const& problem : Loader::get()->getProblems()) {
        switch (problem.type) {
            case geode::LoadProblem::Type::Suggestion:
                bottom.push_back(ProblemsListCell::create(problem, this, this->getCellSize()));
                break;
            case geode::LoadProblem::Type::Recommendation:
                middle.push_back(ProblemsListCell::create(problem, this, this->getCellSize()));
                break;
            default:
                top.push_back(ProblemsListCell::create(problem, this, this->getCellSize()));
                break;
        }
    }

    auto final = CCArray::create();

    // find the highest scrollTo element
    bool scrollFound = false;
    auto tryFindScroll = [&](auto const& item) {
        if (!scrollTo || scrollFound ||
            !std::holds_alternative<Mod*>(item->getProblem().cause) ||
            std::get<Mod*>(item->getProblem().cause) != scrollTo)
            return;
        scrollValue = (float)final->count() * this->getCellSize().height;
        scrollFound = true;
    };

    for (auto const& item : top) {
        tryFindScroll(item);
        final->addObject(item);
    }
    for (auto const& item : middle) {
        tryFindScroll(item);
        final->addObject(item);
    }
    for (auto const& item : bottom) {
        tryFindScroll(item);
        final->addObject(item);
    }

    return final;
}

// Getters

CCSize ProblemsListPopup::getListSize() const {
    return { 340.f, 190.f };
}

CCSize ProblemsListPopup::getCellSize() const {
    return { getListSize().width, 40.f };
}

// Static

ProblemsListPopup* ProblemsListPopup::create(Mod* scrollTo) {
    auto ret = new ProblemsListPopup();
    if (!ret->init(380.f, 250.f, scrollTo)) {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    ret->autorelease();
    return ret;
}

