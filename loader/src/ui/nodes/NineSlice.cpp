#include <Geode/ui/NineSlice.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

class NineSlice::Impl final {
public:
    Ref<CCSpriteBatchNode> m_batchNode;
    Ref<CCArray> m_children;

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

NineSlice::NineSlice() : m_impl(std::make_unique<Impl>()) {}

NineSlice::~NineSlice() {}

NineSlice* NineSlice::create(ZStringView file, CCRect const& rect, Insets const& insets) {
    auto ret = new NineSlice();
    if (ret->initWithFile(file, rect, insets)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

NineSlice* NineSlice::createWithSpriteFrameName(ZStringView spriteFrameName, Insets const& insets) {
    auto ret = new NineSlice();
    if (ret->initWithSpriteFrameName(spriteFrameName, insets)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

NineSlice* NineSlice::createWithSpriteFrame(CCSpriteFrame* spriteFrame, Insets const& insets) {
    auto ret = new NineSlice();
    if (ret->initWithSpriteFrame(spriteFrame, insets)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void NineSlice::setup(Insets const& insets, CCRect const& rect) {
    m_impl->m_insets = insets;
    m_impl->m_spriteRect = rect;

    m_impl->m_children = CCArray::create();

    if (m_impl->m_spriteRect == CCRect{}) {
        auto size = m_impl->m_batchNode->getTexture()->getContentSize();
        m_impl->m_spriteRect = {0, 0, size.width, size.height};
    }

    if (m_impl->m_insets == Insets{}) {
        auto size = m_impl->m_spriteRect.size;
        m_impl->m_insets = {size.height/3, size.width/3, size.height/3, size.width/3};
    }

    m_impl->m_batchNode->setID("slice-batch");

    CCNodeRGBA::addChild(m_impl->m_batchNode, 0, 0);

    setAnchorPoint({0.5f, 0.5f});
    setContentSize(m_impl->m_spriteRect.size);

    createSprites();
}

bool NineSlice::initWithFile(ZStringView file, CCRect const& rect, Insets const& insets) {
    if (!CCNodeRGBA::init()) return false;

    m_impl->m_batchNode = CCSpriteBatchNode::create(file.c_str(), 9);
    if (!m_impl->m_batchNode) return false;

    setup(insets, rect);
    return true;
}

bool NineSlice::initWithSpriteFrameName(ZStringView spriteFrameName, Insets const& insets) {
    auto frame = CCSpriteFrameCache::get()->spriteFrameByName(spriteFrameName.c_str());
    return initWithSpriteFrame(frame, insets);
}

bool NineSlice::initWithSpriteFrame(CCSpriteFrame* spriteFrame, const Insets& insets) {
    if (!spriteFrame) return false;
    auto texture = spriteFrame->getTexture();

    m_impl->m_batchNode = CCSpriteBatchNode::createWithTexture(texture, 9);
    if (!m_impl->m_batchNode) return false;

    m_impl->m_rectRotated = spriteFrame->isRotated();

    setup(insets, spriteFrame->getRect());
    return true;
}

void NineSlice::createSprites() {
    createSprite(m_impl->m_topLeft, "top-left");
    createSprite(m_impl->m_top, "top");
    createSprite(m_impl->m_topRight, "top-right");
    createSprite(m_impl->m_left, "left");
    createSprite(m_impl->m_center, "center");
    createSprite(m_impl->m_right, "right");
    createSprite(m_impl->m_bottomLeft, "bottom-left");
    createSprite(m_impl->m_bottom, "bottom");
    createSprite(m_impl->m_bottomRight, "bottom-right");

    updateSprites();
}

void NineSlice::createSprite(CCSprite*& spr, ZStringView id) {
    spr = CCSprite::createWithTexture(m_impl->m_batchNode->getTexture());
    spr->setAnchorPoint({0.f, 0.f});
    spr->setID(id);
    spr->setCascadeColorEnabled(true);
    spr->setCascadeOpacityEnabled(true);
    m_impl->m_batchNode->addChild(spr);
}

void NineSlice::setSpriteRect(CCSprite* spr, CCRect rect, CCAffineTransform transform) {
    auto originalOrigin = rect.origin;
    rect = CCRectApplyAffineTransform(rect, transform);
    if (m_impl->m_rectRotated) {
        rect.origin = originalOrigin;
    }

    spr->setTextureRect(rect, m_impl->m_rectRotated, rect.size);
}

void NineSlice::updateSprites() {
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
    CCRect bottomCenterRect = {texRect.origin.x + texLeft, texRect.origin.y + texRect.size.height - texBottom, texCenterW, texBottom};
    CCRect bottomRightRect = {texRect.origin.x + texRect.size.width - texRight, texRect.origin.y + texRect.size.height - texBottom, texRight, texBottom};

    CCAffineTransform t = CCAffineTransformMakeIdentity();
    if (!rotated) {
        t = CCAffineTransformTranslate(t, 0, 0);
    }
    else {
        t = CCAffineTransformTranslate(t, texRect.size.height, 0);
        t = CCAffineTransformRotate(t, M_PI / 2.f);
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

void NineSlice::createRepeatingSprites(CCSprite* spr, int horizontalAmount, int verticalAmount, float lastHorizontalFactor, float lastVerticalFactor) {
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

void NineSlice::setScaleMultiplier(float scaleMultiplier) {
    m_impl->m_scaleMultiplier = scaleMultiplier;
    m_impl->m_dirty = true;
}

void NineSlice::setRepeatCenter(bool repeat) {
    m_impl->m_repeatCenter = repeat;
    m_impl->m_dirty = true;
}

bool NineSlice::getRepeatCenter() const {
    return m_impl->m_repeatCenter;
}

NineSlice::Insets NineSlice::getInsets() const {
    return m_impl->m_insets;
}

void NineSlice::setInsets(Insets const& insets) {
    m_impl->m_insets = insets;
    m_impl->m_dirty = true;
}

void NineSlice::setInsetTop(float top) {
    m_impl->m_insets.top = top;
    m_impl->m_dirty = true;
}

void NineSlice::setInsetRight(float right) {
    m_impl->m_insets.right = right;
    m_impl->m_dirty = true;
}

void NineSlice::setInsetBottom(float bottom) {
    m_impl->m_insets.bottom = bottom;
    m_impl->m_dirty = true;
}

void NineSlice::setInsetLeft(float left) {
    m_impl->m_insets.left = left;
    m_impl->m_dirty = true;
}

float NineSlice::getInsetTop() const {
    return m_impl->m_insets.top;
}

float NineSlice::getInsetRight() const {
    return m_impl->m_insets.right;
}

float NineSlice::getInsetBottom() const {
    return m_impl->m_insets.bottom;
}

float NineSlice::getInsetLeft() const {
    return m_impl->m_insets.left;
}

CCSprite* NineSlice::getTopLeft() {
    return m_impl->m_topLeft;
}

CCSprite* NineSlice::getTopRight() {
    return m_impl->m_topRight;
}

CCSprite* NineSlice::getBottomLeft() {
    return m_impl->m_bottomLeft;
}

CCSprite* NineSlice::getBottomRight() {
    return m_impl->m_bottomRight;
}

CCSprite* NineSlice::getTop() {
    return m_impl->m_top;
}

CCSprite* NineSlice::getBottom() {
    return m_impl->m_bottom;
}

CCSprite* NineSlice::getLeft() {
    return m_impl->m_left;
}

CCSprite* NineSlice::getRight() {
    return m_impl->m_right;
}

CCSprite* NineSlice::getCenter() {
    return m_impl->m_center;
}

CCSpriteBatchNode* NineSlice::getBatchNode() {
    return m_impl->m_batchNode;
}

void NineSlice::setContentSize(CCSize const& size) {
    CCNodeRGBA::setContentSize(size);
    m_impl->m_dirty = true;
}

// We don't want the batch node to be a child, this makes it easier for devs to add to the node, use layouts, etc.
void NineSlice::addChild(CCNode* child, int zOrder, int tag) {
    CCNodeRGBA::addChild(child, zOrder, tag);
    m_impl->m_children->addObject(child);
}

void NineSlice::removeChild(CCNode* child, bool cleanup) {
    CCNodeRGBA::removeChild(child, cleanup);
    m_impl->m_children->removeObject(child);
}

// Return all children only when alpha testing to ensure clipping nodes work
cocos2d::CCArray* NineSlice::getChildren() {
    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColorAlphaTest);
    if (m_pShaderProgram == program) {
        return m_pChildren;
    }
    return m_impl->m_children;
}

unsigned int NineSlice::getChildrenCount() const {
    CCGLProgram* program = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColorAlphaTest);
    if (m_pShaderProgram == program) {
        return m_pChildren->count();
    }
    return m_impl->m_children->count();
}

void NineSlice::visit() {
    if (m_impl->m_dirty) {
        updateSprites();
        m_impl->m_dirty = false;
    }

    CCNodeRGBA::visit();
}

void NineSlice::setColor(ccColor3B const& color) {
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) {
        child->setColor(color);
    }
    CCNodeRGBA::setColor(color);
}

void NineSlice::setOpacity(GLubyte opacity) {
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) {
        child->setOpacity(opacity);
    }
    CCNodeRGBA::setOpacity(opacity);
}

void NineSlice::setOpacityModifyRGB(bool var) {
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) {
        child->setOpacityModifyRGB(var);
    }
    CCNodeRGBA::setOpacityModifyRGB(var);
}

void NineSlice::updateDisplayedOpacity(GLubyte parentOpacity) {
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) {
        child->updateDisplayedOpacity(parentOpacity);
    }
    CCNodeRGBA::updateDisplayedOpacity(parentOpacity);
}

void NineSlice::updateDisplayedColor(cocos2d::ccColor3B const& parentColor) {
    for (auto child : m_impl->m_batchNode->getChildrenExt<CCSprite>()) {
        child->updateDisplayedColor(parentColor);
    }
    CCNodeRGBA::updateDisplayedColor(parentColor);
}
