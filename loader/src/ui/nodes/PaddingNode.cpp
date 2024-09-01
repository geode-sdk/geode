#include <Geode/ui/PaddingNode.hpp>

using namespace cocos2d;

geode::PaddingNode::Padding::operator float() const {
    return (top + right + bottom + left) / 4;
}

geode::PaddingNode::Padding::operator CCPoint() const {
    return { (left + right) / 2, (top + bottom) / 2 };
}

geode::PaddingNode::Padding::Padding() :
    top(0),
    right(0),
    bottom(0),
    left(0) { }

geode::PaddingNode::Padding::Padding(CCPoint padding) :
    top(padding.y),
    right(padding.x),
    bottom(padding.y),
    left(padding.x) { }

geode::PaddingNode::Padding::Padding(const float x, const float y) :
    top(y),
    right(x),
    bottom(y),
    left(x) { }

geode::PaddingNode::Padding::Padding(const float top, const float right, const float bottom, const float left) :
    top(top),
    right(right),
    bottom(bottom),
    left(left) { }

geode::PaddingNode* geode::PaddingNode::create(CCNode* container, const Padding& padding, const PaddingType type) {
    geode::PaddingNode* node = new geode::PaddingNode(container, padding, type);

    if (node && node->init()) {
        node->autorelease();

        return node;
    } else {
        CC_SAFE_DELETE(node);

        return nullptr;
    }
}

geode::PaddingNode::PaddingNode(CCNode* container, geode::PaddingNode::Padding padding, const PaddingType type) :
    m_container(container),
    m_padding(padding),
    m_type(type) { }

bool geode::PaddingNode::init() {
    if (!CCNode::init()) {
        return false;
    }

    this->addChild(m_container);
    this->updatePadding();

    return true;
}

void geode::PaddingNode::updatePadding() {
    if (m_type == PaddingType::Contained) {
        const CCSize size = this->getContentSize();

        m_container->setContentSize({
            size.width - this->getTotalPaddingX(),
            size.height - this->getTotalPaddingY()
        });
    } else {
        const CCSize containerSize = m_container->getContentSize();
        
        this->setContentSize({
            containerSize.width + this->getTotalPaddingX(),
            containerSize.height + this->getTotalPaddingY()
        });
    }

    m_container->setAnchorPoint({ 0, 0 });
    m_container->setPosition({
        m_padding.left,
        m_padding.bottom
    });
}

void geode::PaddingNode::setPadding(const Padding& padding) {
    m_padding = padding;
    this->updatePadding();
}

geode::PaddingNode::Padding geode::PaddingNode::getPadding() const {
    return m_padding;
}

float geode::PaddingNode::getAveragePadding() const {
    return m_padding;
}

float geode::PaddingNode::getTotalPadding() const {
    return m_padding.top + m_padding.right + m_padding.bottom + m_padding.left;
}

void geode::PaddingNode::setPaddingX(const float x) {
    m_padding.left = m_padding.right = x;
    this->updatePadding();
}

float geode::PaddingNode::getPaddingX() const {
    return this->getTotalPaddingX() / 2;
}

float geode::PaddingNode::getTotalPaddingX() const {
    return m_padding.left + m_padding.right;
}

void geode::PaddingNode::setPaddingY(const float y) {
    m_padding.top = m_padding.bottom = y;
    this->updatePadding();
}

float geode::PaddingNode::getPaddingY() const {
    return this->getTotalPaddingY() / 2;
}

float geode::PaddingNode::getTotalPaddingY() const {
    return m_padding.top + m_padding.bottom;
}

void geode::PaddingNode::setPaddingTop(const float top) {
    m_padding.top = top;
    this->updatePadding();
}

float geode::PaddingNode::getPaddingTop() const {
    return m_padding.top;
}

void geode::PaddingNode::setPaddingRight(const float right) {
    m_padding.right = right;
    this->updatePadding();
}

float geode::PaddingNode::getPaddingRight() const {
    return m_padding.right;
}

void geode::PaddingNode::setPaddingBottom(const float bottom) {
    m_padding.bottom = bottom;
    this->updatePadding();
}

float geode::PaddingNode::getPaddingBottom() const {
    return m_padding.bottom;
}

void geode::PaddingNode::setPaddingLeft(const float left) {
    m_padding.left = left;
    this->updatePadding();
}

float geode::PaddingNode::getPaddingLeft() const {
    return m_padding.left;
}

void geode::PaddingNode::setPaddingType(const PaddingType type) {
    m_type = type;
    this->updatePadding();
}

geode::PaddingNode::PaddingType geode::PaddingNode::getPaddingType() const {
    return m_type;
}

void geode::PaddingNode::setContainer(CCNode* content) {
    m_container->removeFromParent();

    m_container = content;
    this->updatePadding();
}

CCNode* geode::PaddingNode::getContainer() const {
    return m_container;
}

CCSize geode::PaddingNode::getPaddedContainerSize() const {
    return m_container->getContentSize() + CCSize(this->getTotalPaddingX(), this->getTotalPaddingY());
}