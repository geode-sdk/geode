#include "ModProblemItem.hpp"

#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/ui/Layout.hpp>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
#include <Geode/cocos/platform/CCPlatformMacros.h>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/TextArea.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <ccTypes.h>
#include <fmt/core.h>
#include <sstream>
#include "../GeodeStyle.hpp"

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
    icon->setAnchorPoint({ 0.0f, 0.5f });
    std::string message = this->createProblemMessage();

    this->addChildAtPosition(
        icon,
        Anchor::Left,
        CCPoint { 10.0f, 0.0f }
    );

    auto label = SimpleTextArea::create(
        message.c_str(),
        "bigFont.fnt"
    );
    label->setWrappingMode(WrappingMode::SPACE_WRAP);
    label->setAnchorPoint({ 0.0f, 0.5f });
    label->setMaxLines(4);
    if (this->showFixButton() || this->showInfoButton()) {
        label->setWidth(size.width * 0.7f);

        auto helpMenu = CCMenu::create();
        helpMenu->setAnchorPoint({ 1.0f, 0.5f });

        if (this->showInfoButton()) {
            auto infoSpr = createGeodeButton("More");
            infoSpr->setScale(0.6f);

            auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(ModProblemItem::onInfo));

            helpMenu->addChild(infoBtn);
        }

        // this layout isn't perfect but whoever adds the fix button can figure it out imo
        helpMenu->setContentSize({size.width, size.height});

        auto layout = RowLayout::create();
        layout->setAxisAlignment(AxisAlignment::End);
        layout->setAxisReverse(true);
        helpMenu->setLayout(layout);

        this->addChildAtPosition(helpMenu, Anchor::Right, CCPoint { -13.0f, 0 });
    } else {
        // Left + Right + Space between
        constexpr float paddings = 30.0f;
        float calc = size.width - paddings - icon->getScaledContentWidth();
        label->setWidth(calc);
    }
    label->setScale(0.4f);
    this->addChildAtPosition(
        label,
        Anchor::Left,
        CCPoint { 20.0f + icon->getScaledContentWidth(), 0.0f }
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

void ModProblemItem::onInfo(CCObject*) {
    // someone should really add that fix button. not me though
    if (m_problem.type == LoadProblem::Type::MissingDependency) {
        (void)openInfoPopup(m_problem.message);
        return;
    }

    if (m_problem.message.length() > 400) {
        // show message in a scrolling layer if it's too long
        FLAlertLayer::create(nullptr, "Error Details", m_problem.message, "OK", nullptr, 400.0f, true, 280.0f, 1.0f)->show();
    } else {
        FLAlertLayer::create("Error Details", m_problem.message, "OK")->show();
    }
}

bool ModProblemItem::showInfoButton() {
    if (m_problem.message.empty()) {
        return false;
    }

    // should only show for types that don't already show their message (some do!)
    switch (m_problem.type) {
        case LoadProblem::Type::Unknown:
        case LoadProblem::Type::InvalidFile:
        case LoadProblem::Type::LoadFailed:
        case LoadProblem::Type::EnableFailed:
        case LoadProblem::Type::MissingDependency:
            return true;
        default:
            return false;
    }
}

bool ModProblemItem::showFixButton() {
    // TODO: Actually implement the fix button ;)
    return false;
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
            } else {
                ss << "suggests " << m_problem.message << " to be installed.";
            }
            return ss.str();
        }
        case LoadProblem::Type::Recommendation: {
            std::string id = m_problem.message.substr(0, m_problem.message.find(" "));
            if (auto found = Loader::get()->getInstalledMod(id)) {
                ss << fmt::format(
                    "recommends enabling the {} mod.",
                    found->getName()
                );
            } else {
                ss << "recommends" << m_problem.message << " to be installed.";
            }
            return ss.str();
        }
        case LoadProblem::Type::OutdatedConflict:
        case LoadProblem::Type::Conflict: {
            if (auto found = Loader::get()->getInstalledMod(m_problem.message)) {
                ss << fmt::format(
                    "conflicts with the {} mod.",
                    found->getName()
                );
            } else {
                ss << fmt::format(
                    "conflicts with the {} mod.",
                    m_problem.message
                );
            }
            return ss.str();
        }
        case LoadProblem::Type::OutdatedIncompatibility:
        case LoadProblem::Type::PresentIncompatibility: {
            if (auto found = Loader::get()->getInstalledMod(m_problem.message)) {
                ss << fmt::format(
                    "cannot work if the {} mod is enabled.",
                    found->getName()
                );
            } else {
                ss << fmt::format(
                    "cannot work if the {} mod is enabled.",
                    m_problem.message
                );
            }
            return ss.str();
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
            ss << "couldn't be unzipped. Reason: " << m_problem.message;
            return ss.str();
        }
        case LoadProblem::Type::UnsupportedVersion: {
            return m_problem.message;
        }
        case LoadProblem::Type::NeedsNewerGeodeVersion:
        case LoadProblem::Type::UnsupportedGeodeVersion: {
            ss << fmt::format(
                "requires Geode {} to run (installed: {})",
                m_source->getMetadataRef().getGeodeVersion(),
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
            } else {
                ss << fmt::format(
                    "requires the {} mod to be updated.",
                    m_problem.message
                );
            }
            return ss.str();
        }
        case LoadProblem::Type::DisabledDependency: {
            if (auto found = Loader::get()->getInstalledMod(m_problem.message)) {
                ss << fmt::format(
                    "requires the {} mod to be enabled.",
                    found->getName()
                );
            } else {
                ss << fmt::format(
                    "requires the {} mod to be enabled.",
                    m_problem.message
                );
            }
            return ss.str();
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
    auto ret = new ModProblemItem();
    if (ret->init(source, problem, size)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}