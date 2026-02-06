#include <Geode/ui/Border.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class Border::Impl final {
public:
    Padding padding;
};

Border* Border::create(CCNode* node, const ccColor4B& backgroundColor, const CCSize& size, const CCPoint& padding) {
    Border* instance = new Border(padding);

    if (instance->init(node, backgroundColor, size)) {
        instance->autorelease();
        return instance;
    }

    delete instance;
    return nullptr;
}

Border::Border(const CCPoint& padding) : m_impl(std::make_unique<Impl>()) {
    m_impl->padding = { padding.x, padding.y, padding.x, padding.y };
}

 Border::~Border() { }

bool Border::init(const ccColor4B& backgroundColor, const CCSize& size) {
    return this->init(nullptr, backgroundColor, size);
}

bool Border::init(CCNode* node, const ccColor4B& backgroundColor, const CCSize& size) {
    if (!this->initWithColor(backgroundColor)) {
        return false;
    }

    NineSlice* border = NineSlice::create("inverseborder.png"_spr);
    CCLayer* content = CCLayer::create();

    border->setID("border_sprite"_spr);
    border->setAnchorPoint({ 0, 0 });
    border->setPosition({ 0, 0 });
    border->setZOrder(1);
    content->setID("border_content"_spr);
    content->setContentSize(size);
    this->addChild(border);
    this->addChild(content);

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
    return m_impl->padding;
}

float Border::getPaddingX() {
    return (m_impl->padding.left + m_impl->padding.right) / 2;
}

void Border::setPaddingY(float y) {
    this->setPaddingTop(y);
    this->setPaddingBottom(y);
}

float Border::getPaddingY() {
    return (m_impl->padding.top + m_impl->padding.bottom) / 2;
}

void Border::setPaddingTop(float top) {
    m_impl->padding.top = top;

    this->updatePadding();
}

float Border::getPaddingTop() {
    return m_impl->padding.top;
}

void Border::setPaddingRight(float right) {
    m_impl->padding.right = right;

    this->updatePadding();
}

float Border::getPaddingRight() {
    return m_impl->padding.right;
}

void Border::setPaddingBottom(float bottom) {
    m_impl->padding.bottom = bottom;

    this->updatePadding();
}

float Border::getPaddingBottom() {
    return m_impl->padding.bottom;
}

void Border::setPaddingLeft(float left) {
    m_impl->padding.left = left;

    this->updatePadding();
}

float Border::getPaddingLeft() {
    return m_impl->padding.left;
}

void Border::setBackgroundColor(const ccColor4B& color) {
    this->setColor({ color.r, color.g, color.b });
    this->setOpacity(color.a);
}

ccColor4B Border::getBackgroundColor() {
    return { _displayedColor.r, _displayedColor.g, _displayedColor.b, _displayedOpacity };
}

void Border::setNode(CCNode* node) {
    CCNode* content = this->getChildByID("border_content"_spr);

    // Can't assume an ID as the node is a user input and may have its ID changed
    if (CCNode* oldNode = cocos::getChild<CCNode>(content, 0)) {
        // Not going to mess with releasing the node, I'll leave that to the user
        oldNode->removeFromParent();
    }

    content->addChild(node);

    this->updatePadding();
}

CCNode* Border::getNode() {
    if (CCNode* node = cocos::getChild<CCNode>(this->getChildByID("border_content"_spr), 0)) {
        return node;
    } else {
        return nullptr;
    }
}

void Border::setSize(const CCSize& size) {
    this->setContentSize(size);
    this->getChildByID("border_sprite"_spr)->setContentSize(size);
    this->getChildByID("border_content"_spr)->setContentSize(size);
    this->updatePadding();
}

void Border::updatePadding() {
    if (CCNode* node = this->getNode()) {
        CCSize size = this->getContentSize();

        node->setAnchorPoint({ 0, 0 });
        node->setPosition({ m_impl->padding.left, m_impl->padding.bottom });
        node->setContentSize(size - ccp(this->getPaddingX(), this->getPaddingY()) * 2);
    }
}