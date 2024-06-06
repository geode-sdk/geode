#include <Geode/ui/Border.hpp>
#include <cocos2d.h>
#include <cocos-ext.h>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

Border* Border::create(CCNode* node, const ccColor4B& backgroundColor) {
    return Border::create(node, backgroundColor, { 0, 0 });
}

Border* Border::create(CCNode* node, const ccColor4B& backgroundColor, const CCSize& size) {
    return Border::create(node, backgroundColor, size, { 0, 0 });
}

Border* Border::create(CCNode* node, const ccColor4B& backgroundColor, const CCSize& size, const CCPoint& padding) {
    Border* instance = new Border(padding);

    if (instance && instance->init(node, backgroundColor, size)) {
        instance->autorelease();

        return instance;
    } else {
        CC_SAFE_DELETE(instance);

        return nullptr;
    }
}

Border::Border(const CCPoint& padding) : m_size(ccp(0, 0)),
    m_padding({ padding.x, padding.y, padding.x, padding.y }),
    m_customZOrder(false) { }

bool Border::init(const ccColor4B& backgroundColor, const CCSize& size) {
    return this->init(nullptr, backgroundColor, size);
}

bool Border::init(CCNode* node, const ccColor4B& backgroundColor, const CCSize& size) {
    if (!this->initWithColor(backgroundColor)) {
        return false;
    }

    CCScale9Sprite* border = CCScale9Sprite::create("inverseborder.png"_spr);

    border->setID("border_sprite"_spr);
    border->setAnchorPoint({ 0, 0 });
    border->setPosition({ 0, 0 });
    this->addChild(border);

    if (node != nullptr) {
        this->setNode(node);
    }

    this->setSize(size);

    return true;
}

void Border::setPadding(const CCPoint& padding) {
    this->setPadding(padding.x, padding.y);
}

void Border::setPadding(float x, float y) {
    this->setPaddingX(x);
    this->setPaddingY(y);
}

void Border::setPadding(float padding) {
    this->setPadding(padding, padding);
}

void Border::setPaddingX(float x) {
    this->setPaddingLeft(x);
    this->setPaddingRight(x);
}

Border::Padding Border::getPadding() {
    return m_padding;
}

float Border::getPaddingX() {
    return (m_padding.left + m_padding.right) / 2;
}

void Border::setPaddingY(float y) {
    this->setPaddingTop(y);
    this->setPaddingBottom(y);
}

float Border::getPaddingY() {
    return (m_padding.top + m_padding.bottom) / 2;
}

void Border::setPaddingTop(float top) {
    m_padding.top = top;
}

float Border::getPaddingTop() {
    return m_padding.top;
}

void Border::setPaddingRight(float right) {
    m_padding.right = right;
}

float Border::getPaddingRight() {
    return m_padding.right;
}

void Border::setPaddingBottom(float bottom) {
    m_padding.bottom = bottom;
}

float Border::getPaddingBottom() {
    return m_padding.bottom;
}

void Border::setPaddingLeft(float left) {
    m_padding.left = left;
}

float Border::getPaddingLeft() {
    return m_padding.left;
}

void Border::setBackgroundColor(const ccColor4B& color) {
    this->setColor({ color.r, color.g, color.b });
    this->setOpacity(color.a);
}

ccColor4B Border::getBackgroundColor() {
    return { _displayedColor.r, _displayedColor.g, _displayedColor.b, _displayedOpacity };
}

void Border::setNode(CCNode* node) {
    // Can't assume an ID as the node is a user input and may have its ID changed
    if (CCNode* node = cocos::getChild<CCNode>(this, 1)) {
        // Not going to mess with releasing the node, I'll leave that to the user
        node->removeFromParent();
    }

    if (!m_customZOrder) {
        this->getChildByID("border_sprite"_spr)->setZOrder(node->getZOrder() + 1);
    }

    this->addChild(node);
}

CCNode* Border::getNode() {
    if (CCNode* node = cocos::getChild<CCNode>(this, 1)) {
        return node;
    } else {
        return nullptr;
    }
}

void Border::setSize(const CCSize& size) {
    m_size = size;
    this->setContentSize(size);
    this->getChildByID("border_sprite"_spr)->setContentSize(size);
    this->updatePadding();
}

void Border::setZOrder(int zOrder) {
    m_customZOrder = true;

    this->getChildByID("border_sprite"_spr)->setZOrder(zOrder);
}

void Border::resetZOrder() {
    m_customZOrder = false;

    if (CCNode* node = this->getNode()) {
        this->getChildByID("border_sprite"_spr)->setZOrder(node->getZOrder() + 1);
    } else {
        this->getChildByID("border_sprite"_spr)->setZOrder(0);
    }
}

void Border::updatePadding() {
    if (CCNode* node = this->getNode()) {
        CCSize size;

        if (m_size.width == 0 && m_size.height == 0) {
            size = this->getContentSize();
        } else {
            size = m_size;
        }

        node->setAnchorPoint({ 0, 0 });
        node->setPosition({ m_padding.left, m_padding.bottom });
        node->setContentSize({ size.width - this->getPaddingX() * 2, size.height - this->getPaddingY() * 2 });
    }
}