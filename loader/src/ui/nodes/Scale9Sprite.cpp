#include <Geode/ui/Scale9Sprite.hpp>

using namespace geode::prelude;

class Scale9Sprite::Impl final {
public:
    Ref<CCSpriteBatchNode> m_batchNode;

    CCSprite* m_topLeft = nullptr;
    CCSprite* m_top = nullptr;
    CCSprite* m_topRight = nullptr;
    CCSprite* m_left = nullptr;
    CCSprite* m_center = nullptr;
    CCSprite* m_right = nullptr;
    CCSprite* m_bottomLeft = nullptr;
    CCSprite* m_bottom = nullptr;
    CCSprite* m_bottomRight = nullptr;

    float m_scaleMultiplier = 1.f;
    Insets m_insets;
    CCRect m_spriteRect;
    bool m_rectRotated;
    bool m_repeatCenter = false;
    bool m_dirty = false;
};

Scale9Sprite::Scale9Sprite() : m_impl(std::make_unique<Impl>()) {}

Scale9Sprite::~Scale9Sprite() {}

Scale9Sprite* Scale9Sprite::create(geode::ZStringView file, const cocos2d::CCRect& rect, const Insets& insets) {
    auto ret = new Scale9Sprite();
    if (ret->initWithFile(file, rect, insets)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
    
Scale9Sprite* Scale9Sprite::createWithSpriteFrameName(geode::ZStringView spriteFrameName, const Insets& insets) {
    auto ret = new Scale9Sprite();
    if (ret->initWithSpriteFrameName(spriteFrameName, insets)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Scale9Sprite* Scale9Sprite::createWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, const Insets& insets) {
    auto ret = new Scale9Sprite();
    if (ret->initWithSpriteFrame(spriteFrame, insets)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void Scale9Sprite::setup(const Insets& insets, const cocos2d::CCRect& rect) {
    m_impl->m_insets = insets;
    m_impl->m_spriteRect = rect;

    // handle transformations properly
    m_impl->m_batchNode->setParent(this);

    if (m_impl->m_spriteRect == CCRect{}) {
        auto size = m_impl->m_batchNode->getTexture()->getContentSize();
        m_impl->m_spriteRect = {0, 0, size.width, size.height};
    }

    if (m_impl->m_insets == Insets{}) {
        auto size = m_impl->m_spriteRect.size;
        m_impl->m_insets = {size.height/3, size.width/3, size.height/3, size.width/3};
    }

    setAnchorPoint({0.5f, 0.5f});
    setContentSize(m_impl->m_spriteRect.size);

    createSprites();
}

bool Scale9Sprite::initWithFile(geode::ZStringView file, const cocos2d::CCRect& rect, const Insets& insets) {
    if (!CCNodeRGBA::init()) return false;

    m_impl->m_batchNode = CCSpriteBatchNode::create(file.c_str(), 9);
    if (!m_impl->m_batchNode) return false;

    setup(insets, rect);
    return true;
}

bool Scale9Sprite::initWithSpriteFrameName(geode::ZStringView spriteFrameName, const Insets& insets) {    
    auto frame = CCSpriteFrameCache::get()->spriteFrameByName(spriteFrameName.c_str());
    return initWithSpriteFrame(frame, insets);
}

bool Scale9Sprite::initWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, const Insets& insets) {
    if (!spriteFrame) return false;
    auto texture = spriteFrame->getTexture();

    m_impl->m_batchNode = CCSpriteBatchNode::createWithTexture(texture, 9);
    if (!m_impl->m_batchNode) return false;

    m_impl->m_rectRotated = spriteFrame->isRotated();

    setup(insets, spriteFrame->getRect());
    return true;
}

void Scale9Sprite::createSprites() {
    createSprite(m_impl->m_topLeft);
    createSprite(m_impl->m_top);
    createSprite(m_impl->m_topRight);
    createSprite(m_impl->m_left);
    createSprite(m_impl->m_center);
    createSprite(m_impl->m_right);
    createSprite(m_impl->m_bottomLeft);
    createSprite(m_impl->m_bottom);
    createSprite(m_impl->m_bottomRight);

    updateSprites();
}

void Scale9Sprite::createSprite(cocos2d::CCSprite*& spr) {
    spr = CCSprite::createWithTexture(m_impl->m_batchNode->getTexture());
    spr->setAnchorPoint({0.f, 0.f});
    spr->setCascadeColorEnabled(true);
    spr->setCascadeOpacityEnabled(true);
    m_impl->m_batchNode->addChild(spr);
}

void Scale9Sprite::setSpriteRect(cocos2d::CCSprite* spr, cocos2d::CCRect rect, cocos2d::CCAffineTransform transform) {
    auto originalOrigin = rect.origin;
    rect = CCRectApplyAffineTransform(rect, transform);
    if (m_impl->m_rectRotated) {
        rect.origin = originalOrigin;
    }

    spr->setTextureRect(rect, m_impl->m_rectRotated, rect.size);
}

void Scale9Sprite::updateSprites() {
    auto multiplier = m_impl->m_scaleMultiplier > 0 ? m_impl->m_scaleMultiplier : 1.f;

    auto size = CCSize{std::abs(getContentWidth()), std::abs(getContentHeight())} / multiplier;
    auto texRect = m_impl->m_spriteRect;
    bool rotated = m_impl->m_rectRotated;

    m_impl->m_batchNode->setVisible(m_impl->m_scaleMultiplier > 0);

    m_impl->m_batchNode->setScaleX((getContentWidth() < 0 ? -1 : 1) * multiplier);
    m_impl->m_batchNode->setScaleY((getContentHeight() < 0 ? -1 : 1) * multiplier);

    m_impl->m_insets.left = std::max(m_impl->m_insets.left, 0.f);
    m_impl->m_insets.right = std::max(m_impl->m_insets.right, 0.f);
    m_impl->m_insets.top = std::max(m_impl->m_insets.top, 0.f);
    m_impl->m_insets.bottom = std::max(m_impl->m_insets.bottom, 0.f);

    float centerWidth = std::max(0.f, size.width - m_impl->m_insets.left - m_impl->m_insets.right);
    float centerHeight = std::max(0.f, size.height - m_impl->m_insets.top - m_impl->m_insets.bottom);

    float factorX = std::min(size.width / (m_impl->m_insets.left + m_impl->m_insets.right), 1.f);
    float factorY = std::min(size.height / (m_impl->m_insets.top + m_impl->m_insets.bottom), 1.f);

    float leftInset = m_impl->m_insets.left * factorX;
    float rightInset = m_impl->m_insets.right * factorX;
    float topInset = m_impl->m_insets.top * factorY;
    float bottomInset = m_impl->m_insets.bottom * factorY;

    float texLeft = rotated ? bottomInset : leftInset;
    float texRight = rotated ? topInset : rightInset;
    float texTop = rotated ? leftInset : topInset;
    float texBottom = rotated ? rightInset : bottomInset;

    float texCenterW = std::max(0.f, texRect.size.width  - texLeft - texRight);
    float texCenterH = std::max(0.f, texRect.size.height - texTop  - texBottom);

    CCRect topLeftRect = {texRect.origin.x + 0, texRect.origin.y + 0, texLeft, texTop};
    CCRect topCenterRect = {texRect.origin.x + texLeft, texRect.origin.y + 0, texCenterW, texTop};
    CCRect topRightRect = {texRect.origin.x + texRect.size.width - texRight, texRect.origin.y + 0, texRight, texTop};

    CCRect leftCenterRect = {texRect.origin.x + 0, texRect.origin.y + texTop, texLeft, texCenterH};
    CCRect centerRect  = {texRect.origin.x + texLeft, texRect.origin.y + texTop, texCenterW, texCenterH};
    CCRect rightCenterRect = {texRect.origin.x + texRect.size.width - texRight, texRect.origin.y + texTop, texRight, texCenterH};

    CCRect bottomLeftRect = {texRect.origin.x + 0, texRect.origin.y + texRect.size.height - texBottom, texLeft, texBottom};
    CCRect bottomCenterRect= {texRect.origin.x + texLeft, texRect.origin.y + texRect.size.height - texBottom, texCenterW, texBottom};
    CCRect bottomRightRect = {texRect.origin.x + texRect.size.width - texRight, texRect.origin.y + texRect.size.height - texBottom, texRight, texBottom};

    CCAffineTransform t = CCAffineTransformMakeIdentity();
    if (!rotated) {
        t = CCAffineTransformTranslate(t, 0, 0);
    }
    else {
        t = CCAffineTransformTranslate(t, texRect.size.height, 0);
        t = CCAffineTransformRotate(t, 1.57079633f);
    }

    setSpriteRect(m_impl->m_topLeft, topLeftRect, t);
    setSpriteRect(m_impl->m_top, topCenterRect, t);
    setSpriteRect(m_impl->m_topRight, topRightRect, t);
    setSpriteRect(m_impl->m_left, leftCenterRect, t);
    setSpriteRect(m_impl->m_center, centerRect, t);
    setSpriteRect(m_impl->m_right, rightCenterRect, t);
    setSpriteRect(m_impl->m_bottomLeft, bottomLeftRect, t);
    setSpriteRect(m_impl->m_bottom, bottomCenterRect, t);
    setSpriteRect(m_impl->m_bottomRight, bottomRightRect, t);

    m_impl->m_top->setPosition({leftInset, bottomInset + centerHeight});
    m_impl->m_bottom->setPosition({leftInset, 0});
    m_impl->m_left->setPosition({0, bottomInset});
    m_impl->m_right->setPosition({leftInset + centerWidth, bottomInset});
    m_impl->m_topLeft->setPosition({0, bottomInset + centerHeight});
    m_impl->m_bottomLeft->setPosition({0, 0});
    m_impl->m_topRight->setPosition({leftInset + centerWidth, bottomInset + centerHeight});
    m_impl->m_bottomRight->setPosition({leftInset + centerWidth, 0});
    m_impl->m_center->setPosition({leftInset, bottomInset});

    float horizontalScale = texCenterW > 0 ? centerWidth / texCenterW : 0.f;
    float verticalScale = texCenterH > 0 ? centerHeight / texCenterH : 0.f;

    if (!m_impl->m_repeatCenter) {
        m_impl->m_top->setScaleX(horizontalScale);
        m_impl->m_bottom->setScaleX(horizontalScale);
        m_impl->m_center->setScaleX(horizontalScale);
        m_impl->m_left->setScaleY(verticalScale);
        m_impl->m_right->setScaleY(verticalScale);
        m_impl->m_center->setScaleY(verticalScale);
    }
    else {
        int h = static_cast<int>(horizontalScale);
        int v = static_cast<int>(verticalScale);
        createRepeatingSprites(m_impl->m_top, h, 1, horizontalScale - h, 0);
        createRepeatingSprites(m_impl->m_bottom, h, 1, horizontalScale - h, 0);
        createRepeatingSprites(m_impl->m_center, h, v, horizontalScale - h, verticalScale - v);
        createRepeatingSprites(m_impl->m_left, 1, v, 0, verticalScale - v);
        createRepeatingSprites(m_impl->m_right, 1, v, 0, verticalScale - v);
    }
}

void Scale9Sprite::createRepeatingSprites(CCSprite* spr, int horizontalAmount, int verticalAmount, float lastHorizontalFactor, float lastVerticalFactor) {
    CCRect rect = spr->getTextureRect();
    bool rotated = spr->isTextureRectRotated();

    float tileW = rotated ? rect.size.height : rect.size.width;
    float tileH = rotated ? rect.size.width  : rect.size.height;

    // we want the original sprite to be invisible so we can scale the new children accordingly for repeat ones
    spr->setTextureRect({});
    spr->removeAllChildren();

    for (int x = 0; x <= horizontalAmount; ++x) {
        if (x == horizontalAmount && lastHorizontalFactor == 0) continue;

        for (int y = 0; y <= verticalAmount; ++y) {
            if (y == verticalAmount && lastVerticalFactor == 0) continue;

            CCSprite* newSpr = CCSprite::createWithTexture(spr->getTexture());
            newSpr->setAnchorPoint(ccp(0, 0));
            newSpr->setPosition(ccp(tileW * x, tileH * y));
            newSpr->setOpacity(spr->getOpacity());
            newSpr->setColor(spr->getColor());

            CCRect newRect = rect;

            if (x == horizontalAmount) {
                float& size = rotated ? newRect.size.height : newRect.size.width;
                size *= lastHorizontalFactor;
            }

            if (y == verticalAmount) {
                float& size = rotated ? newRect.size.width : newRect.size.height;
                float& origin = rotated ? newRect.origin.x : newRect.origin.y;

                float original = size;
                size *= lastVerticalFactor;
                origin += original - size;
            }

            newSpr->setTextureRect(newRect, rotated, newRect.size);
            spr->addChild(newSpr);
        }
    }
}

void Scale9Sprite::setScaleMultiplier(float scaleMultiplier) {
    m_impl->m_scaleMultiplier = scaleMultiplier;
    m_impl->m_dirty = true;
}

void Scale9Sprite::setRepeatCenter(bool repeat) {
    m_impl->m_repeatCenter = repeat;
    m_impl->m_dirty = true;
}

bool Scale9Sprite::getRepeatCenter() const {
    return m_impl->m_repeatCenter;
}

Scale9Sprite::Insets Scale9Sprite::getInsets() const {
    return m_impl->m_insets;
}

void Scale9Sprite::setInsets(const Insets& insets) {
    m_impl->m_insets = insets;
    m_impl->m_dirty = true;
}

void Scale9Sprite::setInsetTop(float top) {
    m_impl->m_insets.top = top;
    m_impl->m_dirty = true;
}

void Scale9Sprite::setInsetRight(float right) {
    m_impl->m_insets.right = right;
    m_impl->m_dirty = true;
}

void Scale9Sprite::setInsetBottom(float bottom) {
    m_impl->m_insets.bottom = bottom;
    m_impl->m_dirty = true;
}

void Scale9Sprite::setInsetLeft(float left) {
    m_impl->m_insets.left = left;
    m_impl->m_dirty = true;
}

float Scale9Sprite::getInsetTop() const {
    return m_impl->m_insets.top;
}

float Scale9Sprite::getInsetRight() const {
    return m_impl->m_insets.right;
}

float Scale9Sprite::getInsetBottom() const {
    return m_impl->m_insets.bottom;
}

float Scale9Sprite::getInsetLeft() const {
    return m_impl->m_insets.left;
}

void Scale9Sprite::setContentSize(const cocos2d::CCSize& size) {
    CCNodeRGBA::setContentSize(size);
    m_impl->m_dirty = true;
}

// We don't want the batch node to be a child, this makes it easier for devs to add to the node, use layouts, etc.
void Scale9Sprite::visit() {
    if (m_impl->m_dirty) {
        updateSprites();
        m_impl->m_dirty = false;
    }

    kmGLPushMatrix();
    transform();

    m_impl->m_batchNode->visit();

    kmGLPopMatrix();

    CCNodeRGBA::visit();
}

void Scale9Sprite::setColor(const cocos2d::ccColor3B& color) {
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) {
        child->setColor(color);
    }
    CCNodeRGBA::setColor(color);
}

void Scale9Sprite::setOpacity(GLubyte opacity) {
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) {
        child->setOpacity(opacity);
    }
    CCNodeRGBA::setOpacity(opacity);
}