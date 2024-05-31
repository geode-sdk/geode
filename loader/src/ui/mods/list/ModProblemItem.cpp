#include "ModProblemItem.hpp"

#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/cocos/base_nodes/Layout.hpp>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
#include <Geode/cocos/platform/CCPlatformMacros.h>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <ccTypes.h>
#include <fmt/core.h>
#include <sstream>

bool ModProblemItem::init(Mod* source, LoadProblem problem, CCSize const& size) {
    if (!CCNode::init()) {
        return false;
    }

    this->setContentSize(size);
    this->setAnchorPoint({ 0.5f, 0.5f });

    m_severity = this->computeSeverity();
    m_source = source;
    m_problem = problem;

    ccColor4B bgColor = ColorProvider::get()->color("mod-problems-item-bg"_spr);

    m_bg = CCScale9Sprite::create("square02b_001.png");
    m_bg->setColor(to3B(bgColor));
    m_bg->setOpacity(bgColor.a);
    m_bg->setScale(.3f);
    m_bg->setContentSize(CCSize {
        size.width,
        size.height
    } / m_bg->getScale());
    this->addChildAtPosition(
        m_bg,
        Anchor::Center
    );

    CCSprite* icon = this->createSeverityIcon();
    std::string message = this->createProblemMessage();

    CCNode* infoContainer = CCNode::create();
    infoContainer->setAnchorPoint({ 0.0f, 0.5f });
    infoContainer->setContentSize({ 
        size.width - 10.f,
        icon->getScaledContentHeight() 
    });
    infoContainer->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setAutoScale(true)
            ->setGap(5.0f)
    );

    auto label = CCLabelBMFont::create(
        message.c_str(),
        "bigFont.fnt"
    );
    label->setAnchorPoint({ 0.0f, 0.5f });
    // This is such a stupid hack
    label->limitLabelWidth(
        infoContainer->getContentWidth() * 2.f / 3.f,
        1.0f,
        0.1f
    );
    label->setLayoutOptions(
        AxisLayoutOptions::create()
        ->setRelativeScale(label->getScale())
    );

    infoContainer->addChild(icon);
    infoContainer->addChild(label);

    infoContainer->updateLayout();
    this->addChildAtPosition(
        infoContainer,
        Anchor::Left,
        { 10.f, 0.0f }
    );

    return true;
}

ModProblemItem::Severity ModProblemItem::computeSeverity() {
    switch (m_problem.type) {
        case LoadProblem::Type::Suggestion:
        case LoadProblem::Type::Recommendation:
            return Severity::Info;
        default:
            return Severity::Error;
    }
}

CCSprite* ModProblemItem::createSeverityIcon() {
    switch (m_severity) {
        case ModProblemItem::Severity::Error:
            return CCSprite::createWithSpriteFrameName("info-alert.png"_spr);
        default:
            return CCSprite::createWithSpriteFrameName("info-warning.png"_spr);
    }
}

std::string ModProblemItem::createProblemMessage() {
    std::stringstream ss;
    ss << m_source->getName() << " ";

    switch (m_problem.type) {
        case LoadProblem::Type::Unknown:
            ss << "has encountered an unknown error while loading.";
            return ss.str();
        case LoadProblem::Type::Suggestion: {
            std::string id = m_problem.message.substr(0, m_problem.message.find(" "));
            if (auto found = Loader::get()->getInstalledMod(id)) {
                ss << fmt::format(
                    "suggests enabling the {} mod.",
                    found->getName()
                );
                return ss.str();
            }
            ss << "suggests " << m_problem.message << " to be installed.";
            return ss.str();
        }
        case LoadProblem::Type::Recommendation: {
            std::string id = m_problem.message.substr(0, m_problem.message.find(" "));
            if (auto found = Loader::get()->getInstalledMod(id)) {
                ss << fmt::format(
                    "recommends enabling the {} mod.",
                    found->getName()
                );
                return ss.str();
            }
            ss << "recommends" << m_problem.message << " to be installed.";
            return ss.str();
        }
        case LoadProblem::Type::OutdatedConflict:
        case LoadProblem::Type::Conflict: {
            if (auto found = Loader::get()->getInstalledMod(m_problem.message)) {
                ss << fmt::format(
                    "conflicts with the {} mod.",
                    found->getName()
                );
                return ss.str();
            }
        }
        case LoadProblem::Type::OutdatedIncompatibility:
        case LoadProblem::Type::PresentIncompatibility: {
            if (auto found = Loader::get()->getInstalledMod(m_problem.message)) {
                ss << fmt::format(
                    "cannot work if the {} mod is enabled.",
                    found->getName()
                );
                return ss.str();
            }
        }
        case LoadProblem::Type::InvalidFile: {
            ss << "has an invalid .geode file.";
            return ss.str();
        }
        case LoadProblem::Type::Duplicate: {
            ss << "is a duplicate. Remove one of the installed copies.";
            return ss.str();
        }
        case LoadProblem::Type::SetupFailed: {
            ss << "couldn't set up. Reason: " << m_problem.message << ".";
            return ss.str();
        }
        case LoadProblem::Type::LoadFailed: {
            ss << "couldn't load its binary.";
            return ss.str();
        }
        // This one isn't set in LoaderImpl at all
        case LoadProblem::Type::EnableFailed: {
            ss << "couldn't be enabled.";
            return ss.str();
        }
        case LoadProblem::Type::UnzipFailed: {
            ss << "coulsn't be unzipped. Reason: " << m_problem.message;
            return ss.str();
        }
        case LoadProblem::Type::UnsupportedVersion: {
            ss << fmt::format(
                "requires Geometry Dash {} to run. You have {}.",
                m_problem.message,
                GEODE_STR(GEODE_GD_VERSION)
            );
            return ss.str();
        }
        case LoadProblem::Type::NeedsNewerGeodeVersion:
        case LoadProblem::Type::UnsupportedGeodeVersion: {
            ss << fmt::format(
                "requires Geode {} to run (installed: {})",
                m_source->getVersion().toNonVString(),
                Loader::get()->getVersion().toNonVString()
            );
            return ss.str();
        }
        case LoadProblem::Type::OutdatedDependency: {
            if (auto found = Loader::get()->getInstalledMod(m_problem.message)) {
                ss << fmt::format(
                    "requires the {} mod to be updated.",
                    found->getName()
                );
                return ss.str();
            }
        }
        case LoadProblem::Type::DisabledDependency: {
            if (auto found = Loader::get()->getInstalledMod(m_problem.message)) {
                ss << fmt::format(
                    "requires the {} mod to be enabled.",
                    found->getName()
                );
                return ss.str();
            }
        }
        case LoadProblem::Type::MissingDependency: {
            std::string id = m_problem.message.substr(0, m_problem.message.find(" "));
            ss << "requires the " << m_problem.message << " mod to be installed.";
            return ss.str();
        }
        default:
            ss << "default case oops";
            return ss.str();
    }
}

ModProblemItem* ModProblemItem::create(Mod* source, LoadProblem problem, CCSize const& size) {
    auto ret = new ModProblemItem;
    if (!ret || !ret->init(source, problem, size)) {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    ret->autorelease();
    return ret;
}