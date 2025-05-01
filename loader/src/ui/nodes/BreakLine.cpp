#include <cocos2d.h>
#include <Geode/ui/BreakLine.hpp>

using namespace geode::prelude;

bool BreakLine::init(float width, float height, ccColor4F color) {
    if (!CCNode::init())
        return false;

    this->setContentSize({ width, height });
    m_color = color;

    return true;
}

void BreakLine::draw() {
    // some nodes sometimes set the blend func to
    // something else without resetting it back
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ccDrawSolidRect({ 0, 0 }, this->getContentSize(), m_color);
    CCNode::draw();
}

BreakLine* BreakLine::create(float width, float height, ccColor4F color) {
    auto ret = new BreakLine;
    if (ret->init(width, height, color)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
