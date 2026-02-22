#include <Geode/Geode.hpp>
#include <Geode/modify/CCSprite.hpp>
#include <Geode/modify/CCSpriteFrameCache.hpp>
#include <Geode/modify/CCSpriteBatchNode.hpp>
#include <Geode/modify/CCTextureCache.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

static constexpr int FALLBACK_TAG = 105871529;

// Geode extension functions

bool cocos2d::CCSprite::isUsingFallback() {
    return getUserFlag("fallback"_spr);
}

bool cocos2d::CCSpriteFrame::isUsingFallback() {
    return getTag() == FALLBACK_TAG;
}

bool cocos2d::CCSpriteBatchNode::isUsingFallback() {
    return getUserFlag("fallback"_spr);
}


unsigned char* getFallbackData(int width, int height) {
    const int bytesPerPixel = 3;
    unsigned char* data = new unsigned char[width * height * bytesPerPixel];

    const unsigned char fallback[2][2][3] =
    {
        { {255, 0, 220}, {0, 0, 0} },
        { {0, 0, 0},     {255, 0, 220} }
    };

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int index = (y * width + x) * bytesPerPixel;

            int fx = x > width / 2 ? 1 : 0;
            int fy = y > height / 2 ? 1 : 0;

            data[index + 0] = fallback[fy][fx][0];
            data[index + 1] = fallback[fy][fx][1];
            data[index + 2] = fallback[fy][fx][2];
        }
    }

    return data;
}

cocos2d::CCTexture2D* getFallbackTexture() {
    CCTexture2D* fallback = static_cast<CCTexture2D*>(CCTextureCache::get()->m_pTextures->objectForKey("fallback.png"_spr));

    if (!fallback) {
        fallback = new CCTexture2D();

        auto factor = CCDirector::get()->getContentScaleFactor();

        int width = 32 * factor;
        int height = 32 * factor;

        auto data = getFallbackData(width, height);

        fallback->initWithData(data, kCCTexture2DPixelFormat_RGB888, width, height, CCSizeMake(width, height));
        fallback->autorelease();

        delete[] data;

        CCTextureCache::get()->m_pTextures->setObject(fallback, "fallback.png"_spr);
    }

    return fallback;
}

static void assignFallbackObj(CCNode* node) {
    if (!node) return;
    node->setUserFlag("fallback"_spr, true);
}

class $modify(CCSprite) {
    static CCSprite* create(const char* name) {
        auto* sprite = CCSprite::create(name);
        if (sprite == nullptr) {
            sprite = CCSprite::createWithTexture(getFallbackTexture());
            assignFallbackObj(sprite);
        }
        return sprite;
    }

    static CCSprite* createWithSpriteFrameName(const char* name) {
        auto* spriteFrame = CCSpriteFrameCache::get()->spriteFrameByName(name);

        // we check for tag instead of the frame name because this is significantly better for performance
        bool needFallback = !spriteFrame || spriteFrame->getTag() == FALLBACK_TAG;

        if (!needFallback) {
            return CCSprite::createWithSpriteFrame(spriteFrame);
        }

        CCSprite* sprite = CCSprite::createWithTexture(getFallbackTexture());
        assignFallbackObj(sprite);
        return sprite;
    }

    virtual bool initWithSpriteFrame(CCSpriteFrame* frame) {
        if (CCSprite::initWithSpriteFrame(frame))
            return true;

        assignFallbackObj(this);
        return CCSprite::initWithTexture(getFallbackTexture());
    }

    virtual bool initWithFile(const char *pszFilename) {
        if (CCSprite::initWithFile(pszFilename))
            return true;

        assignFallbackObj(this);
        return CCSprite::initWithTexture(getFallbackTexture());
    }

    virtual bool initWithFile(const char *pszFilename, const CCRect& rect) {
        if (CCSprite::initWithFile(pszFilename, rect))
            return true;

        assignFallbackObj(this);
        return CCSprite::initWithTexture(getFallbackTexture());
    }
};

class $modify(CCSpriteFrameCache) {
    CCSpriteFrame* spriteFrameByName(char const* name) {
        auto* frame = CCSpriteFrameCache::spriteFrameByName(name);

        if (frame != nullptr) {
            return frame;
        }

        // this is stupid but rob intentionally doesnt load all icons at startup,
        // probably to save memory, so do this to not use fallback on icons
        static constexpr std::string_view prefixes[] = {
            "player_",
            "ship_",
            "dart_",
            "bird_",
            "robot_",
            "spider_",
            "swing_",
            "jetpack_",
        };
        const std::string_view nameStr = name;
        for (auto const& prefix : prefixes) {
            if (nameStr.find(prefix) != -1) {
                return frame;
            }
        }

        // check if the fallback was already added
        auto fallbackFrame = CCSpriteFrameCache::spriteFrameByName("fallback.png"_spr);
        if (fallbackFrame) {
            return fallbackFrame;
        }

        // create the fallback frame and add to cache
        fallbackFrame = CCSpriteFrame::createWithTexture(getFallbackTexture(), {ccp(0, 0), ccp(128, 128)});

        if (fallbackFrame) {
            fallbackFrame->setTag(FALLBACK_TAG);
            this->addSpriteFrame(fallbackFrame, "fallback.png"_spr);
        }

        return fallbackFrame;
    }
};

class $modify (CCSpriteBatchNode)
{
    bool initWithTexture(CCTexture2D *tex, unsigned int capacity)
    {
        if (!tex)
        {
            tex = getFallbackTexture();
            assignFallbackObj(this);
        }

        return CCSpriteBatchNode::initWithTexture(tex, capacity);
    }
};