#include "ProblemsListCell.hpp"
#include "ProblemsListPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/StatsCell.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <loader/LoaderImpl.hpp>
#include <utility>

void ProblemsListCell::draw() {
    reinterpret_cast<StatsCell*>(this)->StatsCell::draw();
}

float ProblemsListCell::getLogoSize() const {
    return m_height / 1.5f;
}

bool ProblemsListCell::init(LoadProblem problem, ProblemsListPopup* list, CCSize const& size) {
    m_width = size.width;
    m_height = size.height;
    m_layer = list;
    this->setContentSize(size);
    this->setID("problems-list-cell");

    std::string cause = "unknown";
    if (std::holds_alternative<ghc::filesystem::path>(problem.cause)) {
        cause = std::get<ghc::filesystem::path>(problem.cause).filename().string();
    }
    else if (std::holds_alternative<ModMetadata>(problem.cause)) {
        cause = std::get<ModMetadata>(problem.cause).getName();
    }
    else if (std::holds_alternative<Mod*>(problem.cause)) {
        cause = std::get<Mod*>(problem.cause)->getName();
    }

    std::string icon;
    std::string message;
    switch (problem.type) {
        case LoadProblem::Type::Unknown:
            message = fmt::format("Unknown error in {}", cause);
            m_longMessage = problem.message;
            break;
        case LoadProblem::Type::Suggestion:
            icon = "GJ_infoIcon_001.png";
            message = fmt::format("{} suggests {}", cause, problem.message);
            break;
        case LoadProblem::Type::Recommendation:
            icon = "GJ_infoIcon_001.png";
            message = fmt::format("{} recommends {}", cause, problem.message);
            break;
        case LoadProblem::Type::Conflict:
            icon = "info-warning.png"_spr;
            message = fmt::format("{} conflicts with {}", cause, problem.message);
            break;
        case LoadProblem::Type::InvalidFile:
            icon = "info-alert.png"_spr;
            message = fmt::format("{} is an invalid .geode file", cause);
            m_longMessage = problem.message;
            break;
        case LoadProblem::Type::Duplicate:
            icon = "info-alert.png"_spr;
            message = fmt::format("{} is installed more than once", cause);
            m_longMessage = problem.message;
            break;
        case LoadProblem::Type::SetupFailed:
            icon = "info-alert.png"_spr;
            message = fmt::format("{} has failed setting up", cause);
            m_longMessage = problem.message;
            break;
        case LoadProblem::Type::LoadFailed:
            icon = "info-alert.png"_spr;
            message = fmt::format("{} has failed loading", cause);
            m_longMessage = problem.message;
            break;
        case LoadProblem::Type::EnableFailed:
            icon = "info-alert.png"_spr;
            message = fmt::format("{} has failed enabling", cause);
            m_longMessage = problem.message;
            break;
        case LoadProblem::Type::MissingDependency:
            icon = "info-alert.png"_spr;
            message = fmt::format("{} depends on {}", cause, problem.message);
            break;
        case LoadProblem::Type::PresentIncompatibility:
            icon = "info-alert.png"_spr;
            message = fmt::format("{} is incompatible with {}", cause, problem.message);
            break;
    }

    m_problem = std::move(problem);

    m_menu = CCMenu::create();
    m_menu->setPosition(m_width - 40.f, m_height / 2);
    this->addChild(m_menu);

    auto logoSize = this->getLogoSize();

    if (!icon.empty()) {
        auto logoSpr = CCSprite::createWithSpriteFrameName(icon.c_str());
        limitNodeSize(logoSpr, size, 1.f, .1f);
        logoSpr->setPosition({logoSize / 2 + 12.f, m_height / 2});
        this->addChild(logoSpr);
    }

    auto messageLabel = CCLabelBMFont::create(message.c_str(), "bigFont.fnt");
    messageLabel->setAnchorPoint({ .0f, .5f });
    messageLabel->setPosition(m_height / 2 + logoSize / 2 + 13.f, m_height / 2);
    messageLabel->limitLabelWidth(m_width - 120.f, 1.f, .1f);
    this->addChild(messageLabel);

    if (!m_longMessage.empty()) {
        auto viewSpr = ButtonSprite::create("More", "bigFont.fnt", "GJ_button_01.png", .8f);
        viewSpr->setScale(.65f);

        auto viewBtn =
            CCMenuItemSpriteExtra::create(viewSpr, this, menu_selector(ProblemsListCell::onMore));
        m_menu->addChild(viewBtn);
    }

    return true;
}

void ProblemsListCell::onMore(cocos2d::CCObject*) {
    FLAlertLayer::create("Problem Info", m_longMessage, "OK")->show();
}

LoadProblem ProblemsListCell::getProblem() const {
    return m_problem;
}

ProblemsListCell* ProblemsListCell::create(LoadProblem problem, ProblemsListPopup* list, CCSize const& size) {
    auto ret = new ProblemsListCell();
    if (ret->init(problem, list, size)) {
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
