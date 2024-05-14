#include "ModProblemItem.hpp"
#include "Geode/utils/ColorProvider.hpp"
#include "Geode/utils/cocos.hpp"

#include <Geode/cocos/base_nodes/Layout.hpp>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/label_nodes/CCLabelBMFont.h>
#include <Geode/cocos/platform/CCPlatformMacros.h>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include <Geode/loader/Loader.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <ccTypes.h>

bool ModProblemItem::init(LoadProblem problem, CCSize const& size) {
    if (!CCNode::init()) {
        return false;
    }

    this->setContentSize(size);
    this->setAnchorPoint({ 0.5f, 0.5f });

    CCLabelBMFont* label = CCLabelBMFont::create("hi im problem", "bigFont.fnt");
    label->limitLabelWidth(100.f, 1.0f, 0.1f);
    label->setPosition(size / 2);

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

    this->addChild(label);
    return true;
}

ModProblemItem* ModProblemItem::create(LoadProblem problem, CCSize const& size) {
    auto ret = new ModProblemItem;
    if (!ret || !ret->init(problem, size)) {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    ret->autorelease();
    return ret;
}