#include "CategoryNode.hpp"

ccColor3B CategoryNode::categoryToColor(std::string const& category) {
    // all we need is to convert a string into some number 
    // between 0 and 360 and for that number to always be the 
    // same for the same string

    double hue = pow(hash(category.c_str()), 1.2);
    // for some reason RGBfromHSV does not wrap around
    while (hue > 360000.0) hue -= 360000.0;
    while (hue > 360.0) hue -= 360.0;

    auto rgb = CCControlUtils::RGBfromHSV({ hue, .5, 1.0 });
    return {
        static_cast<GLubyte>(rgb.r * 255),
        static_cast<GLubyte>(rgb.g * 255),
        static_cast<GLubyte>(rgb.b * 255)
    };
}

bool CategoryNode::init(
    std::string const& category,
    CategoryNodeStyle style
) {
    if (style == CategoryNodeStyle::Dot) {
        auto dot = CCSprite::createWithSpriteFrameName("category-dot.png"_spr);
        dot->setColor(categoryToColor(category));
        dot->setPosition({ 20.f, 20.f });
        dot->setScale(.7f);
        this->addChild(dot);

        auto label = CCLabelBMFont::create(category.c_str(), "bigFont.fnt");
        label->limitLabelWidth(180.f, 1.5f, .1f);
        label->setAnchorPoint({ .0f, .4f });
        label->setPosition({ 40.f, 20.f });
        this->addChild(label);

        this->setContentSize({
            label->getScaledContentSize().width + 30.f,
            40.f
        });
    } else {
        auto bg = CCScale9Sprite::createWithSpriteFrameName("category-bg.png"_spr);
        bg->setColor(categoryToColor(category));
        this->addChild(bg);

        auto label = CCLabelBMFont::create(category.c_str(), "bigFont.fnt");
        label->limitLabelWidth(180.f, 1.5f, .1f);
        label->setAnchorPoint({ .5f, .4f });
        this->addChild(label);

        bg->setContentSize({
            label->getScaledContentSize().width + 30.f,
            40.f
        });
        bg->setPosition(bg->getScaledContentSize() / 2);
        label->setPosition(bg->getScaledContentSize() / 2);
        this->setContentSize(bg->getScaledContentSize());
    }

    return true;
}

CategoryNode* CategoryNode::create(
    std::string const& category,
    CategoryNodeStyle style
) {
    auto ret = new CategoryNode();
    if (ret && ret->init(category, style)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
