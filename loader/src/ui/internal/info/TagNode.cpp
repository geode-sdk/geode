#include "TagNode.hpp"
#include <Geode/utils/general.hpp>
#include <cocos-ext.h>
#include <Geode/loader/Mod.hpp>

static std::array<ccColor3B, 6> TAG_COLORS {{
    { 114, 255, 138, }, // green
    { 114, 255, 255, }, // aqua
    { 255, 114, 255, }, // pink
    { 255, 114, 114, }, // red
    { 255, 152, 114, }, // orange
    { 255, 229, 114, }, // yellow
}};

ccColor3B TagNode::tagToColor(std::string const& category) {
    // all we need is to convert a string into some number
    // between 0 and 360 and for that number to always be the
    // same for the same string
    return TAG_COLORS[hash(category.c_str()) % 5932 % TAG_COLORS.size()];
}

bool TagNode::init(std::string const& category, TagNodeStyle style) {
    if (style == TagNodeStyle::Dot) {
        auto dot = CCSprite::createWithSpriteFrameName("category-dot.png"_spr);
        dot->setColor(tagToColor(category));
        dot->setPosition({ 20.f, 20.f });
        dot->setScale(.7f);
        this->addChild(dot);

        auto label = CCLabelBMFont::create(category.c_str(), "bigFont.fnt");
        label->limitLabelWidth(180.f, 1.5f, .1f);
        label->setAnchorPoint({ .0f, .4f });
        label->setPosition({ 40.f, 20.f });
        this->addChild(label);

        this->setContentSize({ label->getScaledContentSize().width + 30.f, 40.f });
    }
    else {
        auto bg = CCScale9Sprite::create("square02b_small.png");
        bg->setColor({ 0, 0, 0 });
        bg->setOpacity(145);
        this->addChild(bg);

        auto label = CCLabelBMFont::create(category.c_str(), "mdFont.fnt"_spr);
        label->limitLabelWidth(180.f, 1.5f, .1f);
        label->setAnchorPoint({ .5f, .4f });
        label->setColor(tagToColor(category));
        this->addChild(label);

        bg->setContentSize({ label->getScaledContentSize().width + 30.f, 30.f });
        bg->setPosition(bg->getScaledContentSize() / 2);
        label->setPosition(bg->getScaledContentSize() / 2);
        this->setContentSize(bg->getScaledContentSize());
    }

    return true;
}

TagNode* TagNode::create(std::string const& category, TagNodeStyle style) {
    auto ret = new TagNode();
    if (ret && ret->init(category, style)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
