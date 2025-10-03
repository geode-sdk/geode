#include "ModProblemList.hpp"

#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/platform/CCPlatformMacros.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

#include "ui/mods/list/ModProblemItem.hpp"

bool ModProblemList::init(
    Mod* mod,
    CCSize const& size
) {
    if (!CCNode::init()) {
        return false;
    }
    m_mod = mod;
    this->setContentSize(size);
    this->setAnchorPoint({ 0.5f, 0.5f });

    CCScale9Sprite* bg = CCScale9Sprite::create("square02b_001.png");
    bg->setColor({ 0, 0, 0 });
    bg->setOpacity(75);
    bg->setScale(.3f);
    bg->setContentSize(size / bg->getScale());
    m_bg = bg;
    this->addChildAtPosition(
        m_bg,
        Anchor::Center
    );

    // mfw fod created a scrolllayer with layouts
    m_list = ScrollLayer::create({ size.width - 10.f, size.height - 10.f });
    m_list->m_contentLayer->setLayout(
        SimpleColumnLayout::create()
            ->setMainAxisDirection(AxisDirection::TopToBottom)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(AxisScaling::Grow)
            ->setGap(5.0f)
    );
    this->addChildAtPosition(
        m_list,
        Anchor::Center,
        -m_list->getScaledContentSize() / 2
    );

    CCSize itemSize = {
        m_list->getScaledContentWidth(),
        70.f
    };

    for (LoadProblem problem : m_mod->getAllProblems()) {
        m_list->m_contentLayer->addChild(ModProblemItem::create(m_mod, problem, itemSize));
    }
    m_list->m_contentLayer->updateLayout();
    m_list->scrollToTop();
    return true;
}

ModProblemList* ModProblemList::create(
    Mod* mod,
    CCSize const& size
) {
    auto ret = new ModProblemList();
    if (ret->init(mod, size)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}