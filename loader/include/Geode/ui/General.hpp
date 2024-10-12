#pragma once

#include <Geode/DefaultInclude.hpp>
#include <cocos2d.h>
#include <cocos-ext.h>

namespace geode {
    /**
     * Creates the usual blue gradient BG for a layer. You should use this over
     * creating the sprite manually, as in the future we may provide texture
     * packs the ability to override this function.
     */
    GEODE_DLL cocos2d::CCSprite* createLayerBG();

    enum class SideArt {
        BottomLeft   = 0b0001,
        BottomRight  = 0b0010,
        TopLeft      = 0b0100,
        TopRight     = 0b1000,
        Bottom       = 0b0011,
        Top          = 0b1100,
        All          = 0b1111,
    };
    constexpr SideArt operator|(SideArt a, SideArt b) {
        return static_cast<SideArt>(static_cast<int>(a) | static_cast<int>(b));
    }
    constexpr bool operator&(SideArt a, SideArt b) {
        return static_cast<bool>(static_cast<int>(a) & static_cast<int>(b));
    }

    enum class SideArtStyle {
        Layer,
        LayerGray,
        PopupBlue,
        PopupGold,
    };

    /**
     * Add side art (corner pieces) for a layer
     * @param to Layer to add corner pieces to
     * @param sides Which corners to populate; by default, populates all
     * @param useAnchorLayout If true, `to` is given an `AnchorLayout` and the 
     * corners' positions are dynamically updated
     */
    GEODE_DLL void addSideArt(
        cocos2d::CCNode* to,
        SideArt sides = SideArt::All,
        bool useAnchorLayout = false
    );
    /**
     * Add side art (corner pieces) for a layer
     * @param to Layer to add corner pieces to
     * @param sides Which corners to populate; by default, populates all
     * @param style Which side art sprites to use
     * @param useAnchorLayout If true, `to` is given an `AnchorLayout` and the 
     * corners' positions are dynamically updated
     */
    GEODE_DLL void addSideArt(
        cocos2d::CCNode* to,
        SideArt sides,
        SideArtStyle style,
        bool useAnchorLayout = false
    );

    /**
     * Add the rounded comment borders to a node
     * @note Use the `ListBorders` class for increased control
     */
    GEODE_DLL void addListBorders(
        cocos2d::CCNode* to,
        cocos2d::CCPoint const& center,
        cocos2d::CCSize const& size
    );
    
    class GEODE_DLL ListBorders : public cocos2d::CCNode {
    protected:
        cocos2d::extension::CCScale9Sprite* m_top = nullptr;
        cocos2d::extension::CCScale9Sprite* m_bottom = nullptr;
        cocos2d::CCSprite* m_left = nullptr;
        cocos2d::CCSprite* m_right = nullptr;
        float m_topPadding = 7.5f;
        float m_bottomPadding = 7.5f;
    
        bool init() override;

    public:
        static ListBorders* create();

        void setSpriteFrames(const char* topAndBottom, const char* sides, float horizontalPadding = 7.5f);
        void setSprites(
            cocos2d::extension::CCScale9Sprite* top,
            cocos2d::extension::CCScale9Sprite* bottom,
            cocos2d::CCSprite* left,
            cocos2d::CCSprite* right,
            float topPadding = 7.5f,
            float bottomPadding = 7.5f
        );
        void setContentSize(cocos2d::CCSize const& size) override;
    };
}
