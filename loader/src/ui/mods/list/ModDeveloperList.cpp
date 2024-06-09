#include "ModDeveloperList.hpp"

#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/cocos/base_nodes/Layout.hpp>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/platform/CCPlatformMacros.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

#include "ui/mods/list/ModDeveloperItem.hpp"

bool ModDeveloperList::init(DevListPopup* popup, ModMetadata const& meta, CCSize const& size) {
    if (!CCNode::init()) {
        return false;
    }

    m_meta = meta;
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
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(size.height)
            ->setGap(5.0f)
    );
    this->addChildAtPosition(
        m_list,
        Anchor::Center,
        -m_list->getScaledContentSize() / 2
    );

    CCSize itemSize = {
        m_list->getScaledContentWidth(),
        20.f
    };

    for (std::string dev : m_meta.getDevelopers()) {
        m_list->m_contentLayer->addChild(ModDeveloperItem::create(popup, dev, itemSize));
    }
    m_list->m_contentLayer->updateLayout();
    m_list->scrollToTop();
    return true;
}

ModDeveloperList* ModDeveloperList::create(DevListPopup* popup, ModMetadata const& meta, CCSize const& size) {
    auto ret = new ModDeveloperList();
    if (!ret || !ret->init(popup, meta, size)) {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    ret->autorelease();
    return ret;
}