// geode additions to make stl containers easier
// clang-format off
class GDString {
    void winDtor() = win 0xf6e0;
    GDString& winAssign(GDString const&, size_t, size_t) = win 0xf720;
    GDString& winAssign(char const*) = win 0xf680;
    GDString& winAssign(char const*, size_t) = win 0xf840;

    static uintptr_t macEmptyContainer() {
        return geode::base::get() + 0x6030d0;
    }
    void macCtor(char const*) = mac 0x489fc0;
    void macCtor(GDString const&) = mac 0x489fcc;
    GDString& macAssign(char const*) = mac 0x489f96;
    GDString& macAssign(GDString const&) = mac 0x489f9c;
    void macDestroy() = mac 0x489f78;
}

void testFunction(int a) = mac 0x100000;

class AchievementBar : cocos2d::CCNodeRGBA {
    static AchievementBar* create(const char* title, const char* desc, const char* icon, bool quest) = mac 0x379f80, win 0x3b120, ios 0x1a4784;

    PAD = win 0x24;
}

class AchievementCell : cocos2d::CCLayer {
    void loadFromDict(cocos2d::CCDictionary*) = mac 0x10eaa0, win 0x59010;

    virtual bool init() = win 0x11070;
}

class AchievementManager : cocos2d::CCNode {
    void getAllAchievements() = mac 0x434d60;
    static AchievementManager* sharedState() = mac 0x424420, win 0x7b10;

    PAD = win 0x10;
    cocos2d::CCDictionary* m_achievements;
    PAD = win 0x4;
}


class AchievementNotifier : cocos2d::CCNode {
    void notifyAchievement(const char* title, const char* desc, const char* icon, bool quest) {
        m_queue->addObject(AchievementBar::create(title, desc, icon, quest));
        if (!m_currentAchievement) {
            this->showNextAchievement();
        }
    }

    static AchievementNotifier* sharedState() = mac 0x464e00, win 0xfc90;
    void willSwitchToScene(cocos2d::CCScene*) = mac 0x4650b0, win 0xfea0;
    void showNextAchievement() = mac 0x464f10, win 0xfd60, ios 0x13f508;

    cocos2d::CCScene* m_currentScene;
    cocos2d::CCArray* m_queue;
    AchievementBar* m_currentAchievement;
}

class AchievementsLayer : GJDropDownLayer {
    void customSetup() = mac 0x1bdea0, win 0x3c2f0;
    void loadPage(int) = mac 0x1be190, win 0x3c600;
}

class AnimatedGameObject : GameObject, AnimatedSpriteDelegate, SpritePartDelegate {
    void playAnimation(int) = mac 0xc93d0, win 0x2539e0;
    void updateChildSpriteColor(cocos2d::_ccColor3B) = mac 0xc8450, win 0x2531f0;
}

class AnimatedShopKeeper : CCAnimatedSprite {
    void animationFinished(const char*) {}

    static AnimatedShopKeeper* create(ShopType type) = mac 0x1a57d0, win 0x14c4d0;
    void startAnimating() = win 0x14c690;

    float m_unknown1;
    float m_unknown2;
    bool m_unknown;
}

class AnimatedSpriteDelegate {
    virtual void animationFinished(const char*) {}
}

class AppDelegate : cocos2d::CCApplication, cocos2d::CCSceneDelegate {
    void bgScale() = mac 0x3aaab0;
    virtual bool applicationDidFinishLaunching() = mac 0x3aa900, win 0x3cbb0;
    virtual void applicationDidEnterBackground() = mac 0x3aabe0, win 0x3cf40;
    virtual void applicationWillEnterForeground() = mac 0x3aac80, win 0x3d130;
    virtual void applicationWillBecomeActive() = mac 0x3aab30, win 0x3ce90;
    virtual void applicationWillResignActive() = mac 0x3aab50, win 0x3cf20;
    virtual void trySaveGame() = mac 0x3aaf10, win 0x3d5e0, ios 0x1a28f0;
    virtual void willSwitchToScene(cocos2d::CCScene*) = mac 0x3aaf40, win 0x3d690;
    static AppDelegate* get() {
        return static_cast<AppDelegate*>(cocos2d::CCApplication::sharedApplication());
    }
    bool musicTest() = win 0x3d580;
    void pauseGame() = mac 0x3aab60, win 0x3d3e0;
    void resumeSound() = win 0x3d4d0;
    void setupGLView() = win 0x3c950;

    bool m_unk0;
    bool m_unk1;
    cocos2d::CCScene* m_runningScene;
    bool m_loadingFinished;
    bool m_unk2;
    bool m_unk3;
    bool m_use16BitFormatForColorPicker;
    bool m_unk4;
}

class ArtistCell : TableViewCell {
    virtual void draw() = mac 0x11c980, win 0x59d40; // same as StatsCell
    virtual bool init() = mac 0x11c7c0, win 0x11250;
    void loadFromObject(SongInfoObject*) = mac 0x1118b0, win 0x5ede0;
    void onNewgrounds(cocos2d::CCObject*) = mac 0x11c7e0, win 0x5f0c0;
    void updateBGColor(int) = mac 0x110460, win 0x5c6b0; // same as GJUserCell

    void* m_idk;
}

class AudioEffectsLayer : cocos2d::CCLayerColor {
    void audioStep(float) = mac 0x271f40, win 0x3daa0;
    static AudioEffectsLayer* create(gd::string) = mac 0x271a00, win 0x3d850;
    void resetAudioVars() = mac 0x271ee0, win 0x3da40;
    void goingDown() = win 0x3dd40;
    virtual void updateTweenAction(float, const char*) = win 0x3dc70;

    int m_unk198;
    cocos2d::CCArray* m_unk19C;
    cocos2d::CCArray* m_unk1A0;
    float m_unk1A4;
    float m_unk1A8;
    bool m_unk1AC;
    float m_unk1B0;
    bool m_unk1B4;
}

class BoomListView : cocos2d::CCLayer, TableViewDelegate, TableViewDataSource {
    BoomListView() {}
    ~BoomListView() {
        CC_SAFE_RELEASE(m_entries);
    }

    static BoomListView* create(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x18ecb0;
    bool init(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x18ee00, win 0x10c20, ios 0x12c5a8;
    virtual void draw() {}

    virtual void setupList() = mac 0x18ef90, win 0x10dc0;
    virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
    virtual float cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f070, win 0x10e50;
    virtual void didSelectRowAtIndexPath(CCIndexPath&, TableView*) {}
    virtual int numberOfRowsInSection(unsigned int, TableView*) = mac 0x18f0b0, win 0x10e60;
    virtual unsigned int numberOfSectionsInTableView(TableView*) = mac 0x18f0e0, win 0x10a70;
    virtual TableViewCell* cellForRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f100, win 0x10e70;
    virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&) {}
    virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
    virtual TableViewCell* getListCell(const char*) = mac 0x18f200, win 0x10ed0;
    virtual void loadCell(TableViewCell*, int) = mac 0x18f4a0, win 0x10ff0;
    inline bool init(cocos2d::CCArray* entries, BoomListType type, float width, float height) {
        return this->init(entries, height, width, 0, type);
    }

    TableView* m_tableView;
    cocos2d::CCArray* m_entries;
    BoomListType m_type;
    float m_height;
    float m_width;
    float m_itemSeparation;
    int m_currentPage;
}

class BoomScrollLayer : cocos2d::CCLayer {
    BoomScrollLayer() = mac 0x1e42f0, win 0x11510;
    void instantMoveToPage(int page) = win 0x12330;
    void moveToPage(int page) = win 0x12400;

    cocos2d::CCArray* m_dots;
    PAD = win 0x40;
    ExtendedLayer* m_layer;
    PAD = win 0x48;
    int m_page;
}

class ButtonSprite : cocos2d::CCSprite {
    static ButtonSprite* create(
        const char* caption,
        int width,
        int unknown,
        float scale,
        bool absoluteWidth,
        const char* font,
        const char* texture,
        float height
    ) = win 0x137d0, mac 0x4fa90, ios 0x38c7c;

    static ButtonSprite* create(
        cocos2d::CCSprite* topSprite,
        int width,
        int unknown,
        float height,
        float scale,
        bool absoluteWidth,
        const char* texture,
        bool bUnknown
    ) = win 0x134b0, mac 0x4f1d0;

    [[docs("
        Create a ButtonSprite with a top sprite and a texture.
        @param topSprite The top sprite to add on top of the sprite
        @param width Sprite width; ignored if `absolute` is false
        @param absolute Whether to use absolute width or not
        @param texture The name of the background sprite file (can't be in a spritesheet)
        @param height The height of the button, leave 0 for automatic
        @param scale Scale of top sprite
    ")]]
    static ButtonSprite* create(
        cocos2d::CCSprite* topSprite,
        int width,
        bool absolute,
        float height,
        const char* texture,
        float scale
    ) {
        return create(topSprite, width, 0, height, scale, absolute, texture, true);
    }

    [[docs("
        Create a ButtonSprite with text, a font and a texture.
        @param caption The text of the ButtonSprite
        @param width Sprite width; ignored if `absolute` is false
        @param absolute Whether to use absolute width or not
        @param font The name of the BM font file to use
        @param texture The name of the background sprite file (can't be in a spritesheet)
        @param height The height of the button, leave 0 for automatic
        @param scale Scale of text
        @returns Pointer to the created ButtonSprite, or nullptr on error
    ")]]
    static ButtonSprite* create(const char* caption, int width, bool absolute, const char* font, const char* texture, float height, float scale) {
        return create(caption, width, 0, scale, absolute, font, texture, height);
    }

    static ButtonSprite* create(char const* caption) {
        return ButtonSprite::create(caption, 0, 0, "goldFont.fnt", "GJ_button_01.png", .0f, 1.f);
    }
    static ButtonSprite* create(char const* caption, const char* font, const char* texture) {
        return ButtonSprite::create(caption, 0, 0, font, texture, .0f, 1.f);
    }
    static ButtonSprite* create(char const* caption, const char* font, const char* texture, float scale) {
        return ButtonSprite::create(caption, 0, 0, font, texture, .0f, scale);
    }
    static ButtonSprite* create(char const*, int, int, float, bool) = mac 0x4fa40;
    void updateBGImage(const char*) = mac 0x502d0, win 0x13af0;
    static ButtonSprite* create(char const*, float) = mac 0x4fa60;
    void setString(const char* txt) = mac 0x4fec0, win 0x14030;
    void updateSpriteBGSize() = win 0x13c00;
    void setColor(cocos2d::ccColor3B color) = mac 0x50410, win 0x143e0;

    PAD = mac 0x18, win 0x18;
    cocos2d::CCLabelBMFont* m_label;
    cocos2d::CCSprite* m_subSprite;
    cocos2d::CCSprite* m_subBGSprite;
    cocos2d::extension::CCScale9Sprite* m_BGSprite;
    PAD = win 0x8;
    cocos2d::CCPoint m_spritePosition;
}

class CCAnimatedSprite : cocos2d::CCSprite {
    void runAnimation(gd::string) = mac 0x1a6430, win 0x14f60;
    void tweenToAnimation(gd::string, float) = mac 0x1a65b0, win 0x15080;
    static CCAnimatedSprite* createWithType(const char* file) = win 0x14540;

    gd::string m_unknown1;
    gd::string m_unknown2;
    PAD = win 0x14;
    gd::string m_unknown3;
    PAD = win 0x4;
}

class CCAnimateFrameCache : cocos2d::CCObject {
    static CCAnimateFrameCache* sharedSpriteFrameCache() = mac 0x2e4df0, win 0x158f0;
    void addSpriteFramesWithFile(const char* file) = win 0x159b0;

    cocos2d::CCDictionary* m_unknown1;
    cocos2d::CCDictionary* m_unknown2;
    cocos2d::CCDictionary* m_unknown3;
}

class CCBlockLayer : cocos2d::CCLayerColor {
    inline CCBlockLayer() {}
    virtual bool init() = mac 0x2a59c0, win 0x168d0;
    virtual void draw() = mac 0x2a5c20, win 0x16a80; // same as GJDropDownLayer
    virtual void registerWithTouchDispatcher() = mac 0x2a5ad0, win 0x16990; // shared with many
    virtual void enterLayer() = mac 0x2a5aa0, win 0x16970;
    virtual void exitLayer() = mac 0x2a5b40, win 0x169e0; // same as PauseLayer, EditorPauseLayer
    virtual void showLayer(bool) = mac 0x2a5b90, win 0x16a20; // same as PauseLayer, EditorPauseLayer
    virtual void hideLayer(bool) = mac 0x2a5ba0, win 0x16a30; // same as PauseLayer, EditorPauseLayer
    virtual void layerVisible() = mac 0x2a5bc0, win 0x16a40; // same as GJDropDownLayer
    virtual void layerHidden() = mac 0x2a5be0, win 0x16a50; // same as PauseLayer, EditorPauseLayer
    virtual void enterAnimFinished() {}
    virtual void disableUI() {}
    virtual void enableUI() {}

    bool m_unknown;
    bool m_unknown2;
}

class CCCircleWave : cocos2d::CCNode {
    static CCCircleWave* create(float, float, float, bool) = mac 0xbd270;
    static CCCircleWave* create(float, float, float, bool, bool) = mac 0xbd290, win 0x16c00;
    bool init(float, float, float, bool, bool) = mac 0xbd380, win 0x16cd0;
    void followObject(cocos2d::CCNode*, bool) = mac 0xbd670, win 0x16f20;
    callback void updatePosition(float) = mac 0xbd630, win 0x16f00;
    virtual void setPosition(cocos2d::CCPoint const& pos) = mac 0xbd600, win 0x16ed0;
    virtual void removeMeAndCleanup() = mac 0xbdac0, win 0x17280;
    virtual void draw() = mac 0xbd960, win 0x17100;
    virtual void updateTweenAction(float dt, const char* key) = mac 0xbd960, win 0x16f90;

    cocos2d::CCNode* m_child;
    PAD = win 0x4;
    float m_currentRadius;
    float m_currentOpacity;
    cocos2d::ccColor3B m_color;
    cocos2d::CCPoint m_circleCenter;
    int m_filled;
    int m_lineWidth;
    float m_opacityMultiplier;
    bool m_blendFuncDirty;
}

class CCCircleWaveDelegate {
    virtual void circleWaveWillBeRemoved(CCCircleWave* wave) {}
}

class CCContentLayer : cocos2d::CCLayerColor {
    inline CCContentLayer() {}
    static CCContentLayer* create(cocos2d::ccColor4B const& color, float width, float height) = win 0x172a0;
    virtual void setPosition(cocos2d::CCPoint const& pos) = mac 0x464c60, win 0x17400;
}

class CCIndexPath : cocos2d::CCObject {
    static CCIndexPath* CCIndexPathWithSectionRow(int idk1, int idk2) = win 0x30e40;
    inline CCIndexPath() = default;
    int m_unknown1;
    int m_unknown2;
}

class CCLightFlash {
    static CCLightFlash* create() = mac 0x295870, win 0x17920;
    void playEffect(cocos2d::CCPoint, cocos2d::_ccColor3B, float, float, float, float, float, float, float, float, float, float, float, float, float, float, int, bool, bool, float) = mac 0x295900, win 0x179f0;
}

class CCMenuItemSpriteExtra : cocos2d::CCMenuItemSprite {
    void useAnimationType(MenuAnimationType type) {
        m_startPosition = this->getNormalImage()->getPosition();
        m_animationType = type;
    }
    void setDestination(cocos2d::CCPoint const& pos) {
        m_destPosition = pos;
    }
    void setOffset(cocos2d::CCPoint const& pos) {
        m_offset = pos;
    }
    void setScale(float scale) {
        this->CCMenuItemSprite::setScale(scale);
        m_baseScale = scale;
    }

    static CCMenuItemSpriteExtra* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1253c0, win 0x18ee0, ios 0xe0740;
    inline static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
        return CCMenuItemSpriteExtra::create(sprite, nullptr, target, callback);
    }
    void setSizeMult(float) = mac 0x1255e0, win 0x19080;
    CCMenuItemSpriteExtra() = mac 0x32670, win 0x18db0;
    ~CCMenuItemSpriteExtra() = win 0x18eb0;
    bool init(
        cocos2d::CCNode* normalImage,
        cocos2d::CCNode* selectedImage,
        cocos2d::CCObject* target,
        cocos2d::SEL_MenuHandler callback
    ) {
        if (!cocos2d::CCMenuItemSprite::initWithNormalSprite(
            normalImage, selectedImage, nullptr, target, callback
        )) return false;

        this->setAnchorPoint({ .5f, .5f });
        this->setContentSize(normalImage->getScaledContentSize());
        normalImage->setPosition(m_obContentSize / 2);

        m_baseScale = 1.f;
        m_animationEnabled = true;
        m_scaleMultiplier = 1.26f;

        return true;
    }
    void activate() = mac 0x125730, win 0x191c0;
    void selected() = mac 0x125840, win 0x19270;
    void unselected() = mac 0x125a70, win 0x19430;

    float m_scaleMultiplier;
    float m_baseScale;
    bool m_animationEnabled;
    bool m_colorEnabled;
    float m_unknown;
    gd::string m_unknown2;
    gd::string m_unknown3;
    float m_colorDip;
    cocos2d::CCPoint m_destPosition;
    cocos2d::CCPoint m_offset;
    MenuAnimationType m_animationType;
    cocos2d::CCPoint m_startPosition;
}

class CCMenuItemToggler : cocos2d::CCMenuItem {
    inline CCMenuItemToggler() : CCMenuItem(), m_onButton(nullptr), m_offButton(nullptr), m_toggled(false), m_notClickable(false) {}

    static CCMenuItemToggler* createWithSize(const char* spr1, const char* spr2, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale) {
        auto sprOff = cocos2d::CCSprite::createWithSpriteFrameName(spr1);
        auto sprOn = cocos2d::CCSprite::createWithSpriteFrameName(spr2);
    
        sprOff->setScale(scale);
        sprOn->setScale(scale);
    
        return create(sprOff, sprOn, target, callback);
    }
    static CCMenuItemToggler* createWithStandardSprites(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale) {
        auto sprOff = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        auto sprOn = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    
        sprOff->setScale(scale);
        sprOn->setScale(scale);
    
        return create(sprOff, sprOn, target, callback);
    }
    bool isOn() {
        return m_toggled;
    }
    bool isToggled() {
        return m_toggled;
    }
    void setClickable(bool on) {
        m_notClickable = !on;
    }
    void toggleWithCallback(bool on) {
        this->activate();
        this->toggle(on);
    }

    static CCMenuItemToggler* create(cocos2d::CCNode* offSpr, cocos2d::CCNode* onSpr, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) = mac 0x38400, win 0x19600, ios 0xf5594;
    void setSizeMult(float) = mac 0x38a40, win 0x19850;
    void toggle(bool) = mac 0x38950, win 0x199b0, ios 0xf5848;
    bool init(cocos2d::CCNode* off, cocos2d::CCNode* on, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handler) = mac 0x38500, win 0x196e0;
    void activate() = mac 0x38bc0, win 0x198d0, ios 0xf59ac;
    void selected() = mac 0x38b80, win 0x198a0;
    void unselected() = mac 0x38c00, win 0x19900;
    void setEnabled(bool enabled) = mac 0x38c40, win 0x19930;

    CCMenuItemSpriteExtra* m_offButton;
    CCMenuItemSpriteExtra* m_onButton;
    bool m_toggled;
    bool m_notClickable;
}

class CCMoveCNode : cocos2d::CCNode {
    static CCMoveCNode* create() = mac 0x1842a0, win 0x121c70;
    ~CCMoveCNode() = mac 0x18b2c0;

    float m_staticMoveX;
    float m_staticMoveY;
    float m_staticRotation;
    bool m_lockObjectRotation;
    int m_parentGroupId;
    bool m_forceUpdate;
    int m_unk104;
    float m_followXMod;
    float m_followYMod;
    bool m_unk110;
    float m_optimizedMoveX;
    float m_optimizedMoveY;
    float m_optimizedRotation;
    float m_playerFollowSpeed;
    float m_playerFollowDelay;
    float m_playerFollowOffset;
    float m_playerFollowMaxSpeed;
    GroupCommandObject* m_followCommand;
}

class GroupCommandObject : cocos2d::CCNode {
    // too lazy to re rn
    // the ints are definitely real members but idk their types
    cocos2d::CCPoint m_ec;
    int m_f4;
    int m_f8;
    int m_fc;
    int m_100;
    int m_104;
    int m_108;
    cocos2d::CCPoint m_10c;
    cocos2d::CCPoint m_114;
    cocos2d::CCPoint m_11c;
    int m_124;
    int m_128;
    int m_12c;
    int m_130;
    int m_134;
    bool m_138;
    float m_13c;
    float m_140;
    bool m_144;
    int m_148;
    int m_14c;
    int m_150;
    int m_154;
    int m_158;
    int m_15c;
    cocos2d::CCPoint m_160;
    cocos2d::CCPoint m_168;
    int m_170;
    int m_174;
    int m_178;
    int m_17c;
    int m_180;
    bool m_184;
}

class CCNodeContainer : cocos2d::CCNode {
    static CCNodeContainer* create() = mac 0xb1090, win 0x112370;
    virtual bool init() = mac 0xba950, win 0x33b40;
    void visit() = mac 0xba960, win 0x112420;
}

class CCScrollLayerExt : cocos2d::CCLayer {
    inline CCScrollLayerExt() {}

    static CCScrollLayerExt* create(cocos2d::CCRect rect, bool vertical) {
        auto pRet = new CCScrollLayerExt(rect);
        if (pRet) {
            pRet->autorelease();
            pRet->m_disableVertical = !vertical;
            pRet->m_disableHorizontal = vertical;
            pRet->m_cutContent = true;
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
    float getMinY() {
        return this->getContentSize().height -
            m_contentLayer->getContentSize().height -
            m_scrollLimitTop;
    }
    float getMaxY() {
        return m_scrollLimitBottom;
    }

    // todo: add this back when CCDestructor works and 
    // this no longer causes crashing
    // virtual ~CCScrollLayerExt() = mac 0x2359b0, win 0x1b520;
    virtual void visit() = mac 0x236550, win 0x1bed0;
    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x235ef0, win 0x1b9b0;
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x236300, win 0x1bce0;
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x236020, win 0x1baa0;
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2362a0, win 0x1bcc0;
    virtual void registerWithTouchDispatcher() = mac 0x235eb0, win 0x1b980;
    virtual void preVisitWithClippingRect(cocos2d::CCRect) = mac 0x2366a0, win 0x1c000;
    virtual void postVisit() = mac 0x236720, win 0x1c090;
    void moveToTop() = mac 0x235870, win 0x1b4a0;
    void moveToTopWithOffset(float) = mac 0x2357d0, win 0x1b420;
    CCScrollLayerExt(cocos2d::CCRect rect) = mac 0x235130, win 0x1b020, ios 0x21f05c;
    void scrollLayer(float scroll) = mac 0x236490, win 0x1be20;
    void updateIndicators(float unknown) = win 0x1b710;

    cocos2d::CCTouch* m_touch;
    cocos2d::CCPoint m_touchPosition;
    cocos2d::CCPoint m_touchStartPosition;
    cocos2d::cc_timeval m_timeValue;
    bool m_touchDown;
    bool m_notAtEndOfScroll;
    cocos2d::CCLayerColor* m_verticalScrollbar;
    cocos2d::CCLayerColor* m_horizontalScrollbar;
    CCScrollLayerExtDelegate* m_delegate;
    CCContentLayer* m_contentLayer;
    bool m_cutContent;
    bool m_vScrollbarVisible;
    bool m_hScrollbarVisible;
    bool m_disableHorizontal;
    bool m_disableVertical;
    bool m_disableMovement;
    float m_scrollLimitTop;
    float m_scrollLimitBottom;
    float m_peekLimitTop;
    float m_peekLimitBottom;
}

class CCScrollLayerExtDelegate {
    virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt*) {}
    virtual void scrollViewDidEndDecelerating(CCScrollLayerExt*) {}
    virtual void scrollViewTouchMoving(CCScrollLayerExt*) {}
    virtual void scrollViewDidEndMoving(CCScrollLayerExt*) {}
    virtual void scrollViewTouchBegin(CCScrollLayerExt*) {}
    virtual void scrollViewTouchEnd(CCScrollLayerExt*) {}
}

class CCSpritePart : CCSpritePlus {

}

class CCSpritePlus : cocos2d::CCSprite {
    virtual bool initWithSpriteFrameName(const char*) = mac 0x248670, win 0x1c1e0;
    virtual void setScaleX(float scale) = win 0x1c440;
    virtual void setScaleY(float scale) = win 0x1c4c0;
    virtual void setScale(float scale) = win 0x1c540;
    virtual void setPosition(const cocos2d::CCPoint& pos) = win 0x1c220;
    virtual void setRotation(float rotation) = win 0x1c280;
    virtual bool initWithTexture(cocos2d::CCTexture2D* texture) = win 0x1c200;
    virtual void setFlipX(bool flip) = win 0x1c300;
    virtual void setFlipY(bool flip) = win 0x1c3a0;
    static CCSpritePlus* createWithSpriteFrame(cocos2d::CCSpriteFrame* frame) = win 0x1c130;

    cocos2d::CCArray* m_followers;
    CCSpritePlus* m_followingSprite;
    bool m_hasFollower;
    bool m_propagateScaleChanges;
    bool m_propagateFlipChanges;
}

class CCTextInputNode : cocos2d::CCLayer, cocos2d::CCIMEDelegate, cocos2d::CCTextFieldDelegate {
    inline CCTextInputNode() : cocos2d::CCLayer(), cocos2d::CCIMEDelegate(), cocos2d::CCTextFieldDelegate(), m_caption(), m_allowedChars() {
        m_numberInput = false;
        m_unknown1 = 0;
        m_selected = false;
        m_unknown2 = false;
        m_maxLabelWidth = 0.f;
        m_maxLabelScale = 0.f;
        m_placeholderScale = 0.f;
        m_placeholderColor = cocos2d::ccc3(0,0,0);
        m_textColor = cocos2d::ccc3(0,0,0);
        m_cursor = nullptr;
        m_textField = nullptr;
        m_delegate = nullptr;
        m_maxLabelLength = 0;
        m_placeholderLabel = nullptr;
        m_unknown3 = false;
        m_usePasswordChar = false;
        m_forceOffset = false;
    }
    inline ~CCTextInputNode() {
        if (m_selected) m_textField->detachWithIME();
    }
    void setLabelNormalColor(cocos2d::ccColor3B color) {
        m_textColor = color;
        this->refreshLabel();
    }
    void setLabelPlaceholderColor(cocos2d::ccColor3B color) {
        m_placeholderColor = color;
        this->refreshLabel();
    }
    void setLabelPlaceholderScale(float scale) {
        m_placeholderScale = scale;
        this->refreshLabel();
    }
    void setMaxLabelScale(float scale) {
        m_maxLabelScale = scale;
        this->refreshLabel();
    }
    void setMaxLabelWidth(int length) {
        m_maxLabelLength = length;
        this->refreshLabel();
    }
    void setAllowedChars(gd::string filter) {
        m_allowedChars = filter;
    }
    void forceOffset() {
        m_forceOffset = true;
    }
    void setString(gd::string text) = mac 0x5d3e0, win 0x21070;
    const char* getString() {
        return m_textField->getString();
    }
    cocos2d::CCTextFieldTTF* getTextField() {
        return m_textField;
    }
    cocos2d::CCLabelBMFont* getPlaceholderLabel() {
        return m_placeholderLabel;
    }
    void setDelegate(TextInputDelegate* delegate) {
        m_delegate = delegate;
    }

    inline static CCTextInputNode* create(float width, float height, char const* placeholder, char const* fontPath) {
        return CCTextInputNode::create(width, height, placeholder, 0x18, fontPath);
    }
    inline static CCTextInputNode* create(float width, float height, char const* placeholder, int fontSize, char const* fontPath) {
        auto ret = new CCTextInputNode();
        if (ret && ret->init(width, height, placeholder, "Thonburi", fontSize, fontPath)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    bool init(float width, float height, const char* caption, const char* thonburi, int maxCharCount, const char* font) = mac 0x5d180, win 0x20e50, ios 0xe261c;

    void refreshLabel() = mac 0x5d730, win 0x21330, ios 0xe2af4;
    void updateLabel(gd::string) = mac 0x5d4a0, win 0x21190;
    void updateBlinkLabel() = mac 0x5d920;
    virtual void registerWithTouchDispatcher() = mac 0x5eec0, win 0x220e0;
    virtual void visit() = mac 0x5d380, win 0x21000;
    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ec80, win 0x21f20;
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) {}
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) {}
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) {}
    virtual void textChanged() = mac 0x5dd70, win 0x216e0;
    virtual void onClickTrackNode(bool) = mac 0x5dd40, win 0x216b0;
    virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5dad0, win 0x21580;
    virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5dc20, win 0x21650;
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int) = mac 0x5de50, win 0x21760;
    virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF*) = mac 0x5e2c0, win 0x21b10;
    virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF*) = mac 0x5e610, win 0x21d60;

    bool m_numberInput;
    gd::string m_caption;
    int m_unknown1;
    bool m_selected;
    bool m_unknown2;
    gd::string m_allowedChars;
    float m_maxLabelWidth;
    float m_maxLabelScale;
    float m_placeholderScale;
    cocos2d::ccColor3B m_placeholderColor;
    cocos2d::ccColor3B m_textColor;
    cocos2d::CCLabelBMFont* m_cursor;
    cocos2d::CCTextFieldTTF* m_textField;
    TextInputDelegate* m_delegate;
    int m_maxLabelLength;
    cocos2d::CCLabelBMFont* m_placeholderLabel;
    bool m_unknown3;
    bool m_usePasswordChar;
    bool m_forceOffset;
}

class ChallengesPage {
    static ChallengesPage* create() = mac 0x1db350, win 0x3e050;
}

class CheckpointObject : cocos2d::CCNode {
    static CheckpointObject* create() = mac 0x7e7d0, win 0x20DDD0;
    void getObject() = mac 0x7ef50;

    GameObject* m_gameObject;
    PlayerCheckpoint* m_player1;
    PlayerCheckpoint* m_player2;
    bool m_isDual;
    bool m_isFlipped;
    cocos2d::CCPoint m_cameraPos;
    int m_unk104;
    GameObject* m_lastPortal;
    PAD = win 0x4;
    double m_unk110;
    gd::string m_currentStateString;
    gd::string m_objectsStateString;
}

class CollisionBlockPopup {
    static CollisionBlockPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x130010;
    void onNextItemID(cocos2d::CCObject*) = mac 0x130e60;
}

class CollisionTriggerAction : cocos2d::CCNode {
    static CollisionTriggerAction* createFromString(gd::string) = mac 0x176ee0;
}

class ColorAction : cocos2d::CCNode {
    static ColorAction* create() = mac 0x17c990, win 0x11ab10;
    static ColorAction* create(cocos2d::ccColor3B, cocos2d::ccColor3B, float, double, bool, int, float, float) = win 0x11ab60, mac 0x17cab0;
    gd::string getSaveString() = mac 0x17d080, win 0x11ada0;
    void setupFromDict(cocos2d::CCDictionary*) = mac 0x17f310, win 0x11b7c0;
    void setupFromString(gd::string) = mac 0x17f270, win 0x11b730;

    bool m_stepFinished;
    cocos2d::ccColor3B m_color;
    float m_currentOpacity;
    float m_deltaTime;
    cocos2d::ccColor3B m_fromColor;
    cocos2d::ccColor3B m_targetColor;
    float m_duration;
    bool m_blending;
    int m_playerColor;
    int m_colorID;
    float m_opacity;
    float m_targetOpacity;
    cocos2d::ccHSVValue m_copyHSV;
    int m_copyID;
    PAD = mac 0x1;
    bool m_copyOpacity;
    ColorActionSprite* m_colorSprite;
    PAD = mac 0x10;
}

class ColorActionSprite : cocos2d::CCNode {
    float m_opacity;
    cocos2d::_ccColor3B m_f0124;
    cocos2d::_ccColor3B m_activeColor;
}

class ColorChannelSprite : cocos2d::CCSprite {
    void updateBlending(bool) = mac 0x16e1d0, win 0x114710;
    void updateCopyLabel(int, bool) = mac 0x16ded0, win 0x114440;
    void updateOpacity(float) = mac 0x16e080, win 0x1145c0;
    void updateValues(ColorAction*) = mac 0x16e2e0, win 0x114800;
    static ColorChannelSprite* create() = mac 0x16de00, win 0x114340;

    cocos2d::CCLabelBMFont* m_copyLabel;
    cocos2d::CCLabelBMFont* m_opacityLabel;
    cocos2d::CCLabelBMFont* m_blendingDot;
}

class ColorSelectDelegate {
    virtual void colorSelectClosed(cocos2d::CCNode*) {}
}

class ColorSelectLiveOverlay : FLAlertLayer {
    bool init(ColorAction*, ColorAction*, EffectGameObject*) = mac 0x2e2790, win 0x41db0;
    void sliderChanged(cocos2d::CCObject*) = mac 0x2e3830;
    void onSelectTab(cocos2d::CCObject*) = mac 0x2e3f40, win 0x430f0;
    EffectGameObject* m_effectGameObject;
    cocos2d::CCArray* m_barSprites;
    cocos2d::CCArray* m_12buttons;
    ColorAction* m_baseColorAction;
    ColorAction* m_detailColorAction;
    PAD = win 0x64;
    bool m_detailMode;
}

class ColorSelectPopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
    virtual void colorValueChanged(cocos2d::ccColor3B color) = mac 0x423320, win 0x46ee0;
    ColorSelectPopup() {}
    bool init(EffectGameObject* triggerObj, cocos2d::CCArray* triggerObjs, ColorAction* colorAction) = mac 0x41ee70, win 0x43ae0;
    void updateColorValue() = win 0x46f30;
    void updateCopyColorTextInputLabel() = win 0x479c0, mac 0x422ed0;
    void closeColorSelect(cocos2d::CCObject* sender) = mac 0x421af0, win 0x46d80;

    cocos2d::extension::CCControlColourPicker* m_colorPicker;
    cocos2d::CCLabelBMFont* m_unk1DC;
    cocos2d::CCLabelBMFont* m_label;
    Slider* m_unk1E4;
    Slider* m_slider;
    EffectGameObject* m_targetObject;
    cocos2d::CCArray* m_targetObjects;
    CCMenuItemToggler* m_toggler1;
    CCMenuItemToggler* m_toggler2;
    unsigned int m_rgbLastColor;
    cocos2d::CCSprite* m_unk200;
    cocos2d::CCSprite* m_unk204;
    cocos2d::ccColor3B m_selectedColor;
    GJColorSetupLayer* m_colorSetupLayer;
    float m_fadeTime;
    int m_playerColor;
    bool m_isBlending;
    float m_opacity;
    ColorAction* m_colorAction;
    CCTextInputNode* m_textInput1;
    bool m_unk228;
    bool m_isTouchTrigger;
    bool m_unk22A;
    bool m_isMultipleColorTrigger;
    bool m_unk22C;
    bool m_isColorTrigger;
    int m_colorID;
    bool m_unk234;
    int m_copyChannelID;
    bool m_copyOpacity;
    ConfigureHSVWidget* m_hsvWidget;
    PAD = win 0x10, mac 0x10;
    cocos2d::CCArray* m_unk254;
    cocos2d::CCArray* m_unk258;
    CCTextInputNode* m_textInput2;
    PAD = win 0x4, mac 0x8;
    CCMenuItemToggler* m_toggler3;
    CCMenuItemToggler* m_toggler4;
    PAD = win 0x8, mac 0x10;
    cocos2d::CCArray* m_unk274;
    bool m_spawnTrigger;
    bool m_multiTrigger;
    bool m_copyColor;
}

class ColorSetupDelegate {
    virtual void colorSetupClosed(int channel) {}
}

class CommentCell : TableViewCell, LikeItemDelegate, FLAlertLayerProtocol {
    void loadFromComment(GJComment*) = mac 0x111c70, win 0x5f3d0;
    void onConfirmDelete(cocos2d::CCObject* sender) = mac 0x11d100, win 0x61140;
    void onLike(cocos2d::CCObject* sender) = mac 0x11d000, win 0x60F90;
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn) = mac 0x11d540, win 0x61260;
    virtual void likedItem(LikeItemType type, int id, bool special) = mac 0x11d340, win 0x61070;

    cocos2d::CCSprite* m_iconSprite;
    cocos2d::CCLabelBMFont* m_likeLabel;
    GJComment* m_comment;
    bool m_accountComment;
}

class CommentUploadDelegate {
    virtual void commentUploadFinished(int) {}
    virtual void commentUploadFailed(int, CommentError) {}
    virtual void commentDeleteFailed(int, int) {}
}

class ConfigureHSVWidget : cocos2d::CCNode {
    bool init(int abs, cocos2d::ccHSVValue val) = win 0x4a3f0, mac 0x237310;
    void updateLabels() = win 0x4adf0, mac 0x237df0;
    cocos2d::CCLabelBMFont* m_hueLabel;
    cocos2d::CCLabelBMFont* m_saturationLabel;
    cocos2d::CCLabelBMFont* m_brightnessLabel;
    Slider* m_hueSlider;
    Slider* m_saturationSlider;
    Slider* m_brightnessSlider;
    cocos2d::ccHSVValue m_value;
}

class CountTriggerAction : cocos2d::CCNode {
    static CountTriggerAction* createFromString(gd::string) = mac 0x1754f0;

    int m_previousCount;
    int m_targetCount;
    int m_targetID;
    bool m_activateGroup;
    bool m_multiActivate;
}

class CreateGuidelinesLayer : FLAlertLayer, FLAlertLayerProtocol {
    void doClearGuidelines() = win 0x4cf60, mac 0x299690;
    void onStop(cocos2d::CCObject* sender) = win 0x4d2c0, mac 0x2990f0;
    PAD = win 0x4, mac 0x8;
    LevelSettingsObject* m_levelSettings;
    PAD = win 0x1c;
    gd::string m_guidelineString;
}

class CreateMenuItem : CCMenuItemSpriteExtra {
    static CreateMenuItem* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1c580, win 0x93db0;

    PAD = win 0x18;
    int m_objectID;
    int m_buildTabPage;
    int m_buildTab;
}

class CreatorLayer : cocos2d::CCLayer, cocos2d::CCSceneTransitionDelegate, DialogDelegate {
    void onBack(cocos2d::CCObject*) = win 0x4fae0;
    void onChallenge(cocos2d::CCObject*) = mac 0x142960, win 0x4f1b0;
    void onLeaderboards(cocos2d::CCObject*) = win 0x4ed20;
    void onMyLevels(cocos2d::CCObject*) = mac 0x142b70, win 0x4eaa0;
    void onSavedLevels(cocos2d::CCObject*) = mac 0x142860, win 0x4ebe0;
    void onDailyLevel(cocos2d::CCObject*) = win 0x4f170;
    void onWeeklyLevel(cocos2d::CCObject*) = win 0x4f190;
    void onFeaturedLevels(cocos2d::CCObject*) = win 0x4edf0;
    void onFameLevels(cocos2d::CCObject*) = win 0x4ee70;
    void onMapPacks(cocos2d::CCObject*) = win 0x4efb0;
    void onOnlineLevels(cocos2d::CCObject*) = win 0x4ef60;
    void onGauntlets(cocos2d::CCObject*) = mac 0x142b20, win 0x4f0a0;
    void onSecretVault(cocos2d::CCObject*) = win 0x4f1d0;
    void onTreasureRoom(cocos2d::CCObject*) = win 0x4f540;
    virtual void sceneWillResume() = mac 0x1438F0, win 0x4fb50;
    virtual bool init() = mac 0x141c10, win 0x4de40;
    static CreatorLayer* create() = win 0x4dda0;
}

class CurrencyRewardLayer : cocos2d::CCLayer {
    inline CurrencyRewardLayer() {}
    ~CurrencyRewardLayer() = mac 0x447950, win 0x4ffb0;
    virtual void update(float) = mac 0x44a5c0, win 0x52350;

    static CurrencyRewardLayer* create(
        int, int, int,
        CurrencySpriteType, int,
        CurrencySpriteType, int,
        cocos2d::CCPoint, CurrencyRewardType, float
    ) = win 0x50050;
}

class CurrencyRewardDelegate {
    virtual void currencyWillExit(CurrencyRewardLayer*) {}
}

class CustomListView : BoomListView {
    static CustomListView* create(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x10d410, win 0x57f90, ios 0x99154;
    inline static CustomListView* create(cocos2d::CCArray* entries, BoomListType type, float width, float height) {
        return CustomListView::create(entries, width, height, 0, type);
    }
    virtual TableViewCell* getListCell(const char*) = mac 0x10d560, win 0x58050;
    virtual void loadCell(TableViewCell*, int) = mac 0x10e610, win 0x585c0;
    virtual void setupList() = mac 0x116e70, win 0x58870;
    inline CustomListView() {}
}

class CustomSongCell : TableViewCell {
    void loadFromObject(SongInfoObject*) = mac 0x110220, win 0x5e9f0;
}

class CustomSongLayer : FLAlertLayer, FLAlertLayerProtocol, TextInputDelegate, GJDropDownLayerDelegate {
    bool init(LevelSettingsObject*) = mac 0xf06f0, win 0x65c10;
    void onArtists(cocos2d::CCObject*) = mac 0xf1950;
    void onSongBrowser(cocos2d::CCObject*) = mac 0xf18a0, win 0x67080;

    LevelSettingsObject* m_levelSettings;
    CCTextInputNode* m_songIDInput;
    CustomSongWidget* m_songWidget;
    LevelSettingsLayer* m_levelSettingsLayer;
}

class CustomSongWidget : cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {
    bool init(SongInfoObject*, LevelSettingsObject*, bool, bool, bool, bool, bool hideBackground) = mac 0x37be20, win 0x685b0;
    void FLAlert_Clicked(FLAlertLayer*, bool) {}
    void loadSongInfoFinished(SongInfoObject*) {}

    void updateSongObject(SongInfoObject* song) = win 0x69280, mac 0x37d690;

    SongInfoObject* m_songInfo;
    PAD = win 0x1C;
    CCMenuItemSpriteExtra* m_downloadBtn;
    PAD = win 0x30;
}

class CustomizeObjectLayer : FLAlertLayer, TextInputDelegate, HSVWidgetPopupDelegate, ColorSelectDelegate, ColorSetupDelegate {
    void onNextColorChannel(cocos2d::CCObject* sender) = mac 0xe0270, win 0x56c80;
    void onSelectColor(cocos2d::CCObject* sender) = mac 0xdfdc0, win 0x577b0;
    void onSelectMode(cocos2d::CCObject* sender) = mac 0xdf820, win 0x56db0;
    int getActiveMode(bool unknown) = mac 0xe0740, win 0x57210;
    void onClose(cocos2d::CCObject*) = mac 0xdf660, win 0x57ac0;
    void updateSelected(int channelID) = mac 0xe0970, win 0x57850;
    bool init(GameObject* target, cocos2d::CCArray* targets) = mac 0xdd560, win 0x53e00;
    void onHSV(cocos2d::CCObject* sender) = win 0x567c0, mac 0xdfa00;
    void toggleVisible() = mac 0xe1140, win 0x56fb0;
    void highlightSelected(ButtonSprite* target) = mac 0xe0aa0, win 0x579d0;
    void updateCustomColorLabels() = mac 0xdff40, win 0x576d0;
    void onUpdateCustomColor(cocos2d::CCObject* sender) = mac 0xdfe70, win 0x57350;
    void updateColorSprite() = mac 0xe0610, win 0x56370;
    // inlined in most places
    void updateChannelLabel(int channel) = mac 0xe06e0, win 0x56f50;
    virtual void hsvPopupClosed(HSVWidgetPopup* popup, cocos2d::ccHSVValue value) = win 0x56990;
    virtual void colorSelectClosed(cocos2d::CCNode*) = mac 0xe0610, win 0x564a0;
    virtual void textChanged(CCTextInputNode* input) = mac 0xe1470, win 0x574d0;
    inline CustomizeObjectLayer() {}
    ~CustomizeObjectLayer() = win 0x53c30;

    GameObject* m_targetObject;
    cocos2d::CCArray* m_targetObjects;
    cocos2d::CCArray* m_colorButtons;
    cocos2d::CCArray* m_colorTabNodes;
    cocos2d::CCArray* m_textTabNodes;
    PAD = win 0x4, mac 0x8;
    cocos2d::CCArray* m_detailTabNodes;
    int m_selectedMode;
    int m_customColorChannel;
    bool m_unk0x200;
    bool m_unk0x201;
    bool m_glowDisabled;
    CCMenuItemSpriteExtra* m_baseButton;
    CCMenuItemSpriteExtra* m_detailButton;
    CCMenuItemSpriteExtra* m_textButton;
    CCMenuItemSpriteExtra* m_baseColorHSV;
    CCMenuItemSpriteExtra* m_detailColorHSV;
    cocos2d::CCLabelBMFont* m_titleLabel;
    cocos2d::CCLabelBMFont* m_selectedColorLabel;
    CCTextInputNode* m_customColorInput;
    CCTextInputNode* m_textInput;
    ButtonSprite* m_customColorButtonSprite;
    CCMenuItemSpriteExtra* m_customColorButton;
    CCMenuItemSpriteExtra* m_arrowDown;
    CCMenuItemSpriteExtra* m_arrowUp;
    cocos2d::extension::CCScale9Sprite* m_customColorInputBG;
    ColorChannelSprite* m_colorSprite;
    CCMenuItemSpriteExtra* m_colorSpriteButton;
    bool m_showTextInput;
    bool m_customColorSelected;
}

class DailyLevelPage : FLAlertLayer, FLAlertLayerProtocol, GJDailyLevelDelegate, LevelDownloadDelegate {
    static DailyLevelPage* create(bool weekly) = win 0x6a860;
    bool init(bool weekly) = mac 0x108C90, win 0x6a900;
    virtual void updateTimers(float) = mac 0x109780, win 0x6bef0;
    virtual void show() = mac 0x10a4b0, win 0x3f360;

    PAD = mac 0x29, win 0x15;
    bool m_weekly;
}

class DialogLayer : cocos2d::CCLayerColor {
    static DialogLayer* create(DialogObject* dialog, int color) {
        return DialogLayer::createDialogLayer(dialog, nullptr, color);
    }

    static DialogLayer* createWithObjects(cocos2d::CCArray* dialogs, int color) {
        return DialogLayer::createDialogLayer(nullptr, dialogs, color);
    }

    static DialogLayer* createDialogLayer(DialogObject*, cocos2d::CCArray*, int) = mac 0x2047d0, win 0x6D470, ios 0x459d0;
    bool init(DialogObject*, cocos2d::CCArray*, int) = win 0x6D520;
    void animateIn(DialogAnimationType location) = win 0x6E130, mac 0x205a70;
    void updateChatPlacement(DialogChatPlacement placement) = mac 0x205360;

    ~DialogLayer() = mac 0x204720;
    virtual void onEnter() = mac 0x205900, win 0x23750; // same as every other FLAlertLayer
    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205790, win 0x6e000;
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) {}
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2057e0, win 0x6e030;
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205840, win 0x6e050;
    virtual void registerWithTouchDispatcher() = mac 0x205890, win 0x236f0; // same as every other FLAlertLayer
    virtual void keyBackClicked() = mac 0x2056a0, win 0x6df60;
    virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x205ce0, win 0x6e370;
    virtual void fadeInTextFinished(TextArea*) = mac 0x205930, win 0x6e070;
}

class DialogDelegate {
    virtual void dialogClosed(DialogLayer*) {}
}

class DialogObject : cocos2d::CCObject {
    DialogObject() {}

    static DialogObject* create(gd::string title, gd::string text, int portrait, float text_scale, bool is_unskippable, cocos2d::ccColor3B text_color) = win 0x6D160;
    bool init(gd::string title, gd::string text, int portrait, float text_scale, bool is_unskippable, cocos2d::ccColor3B text_color) = win 0x6D2E0;

    gd::string m_text;
    gd::string m_title;
    int m_dialogType;
    cocos2d::ccColor3B m_colour;
    float m_textWidth;
    bool m_canSkip;
}

class DownloadMessageDelegate {}

class DrawGridLayer : cocos2d::CCLayer {
    static DrawGridLayer* get() {
        auto editorLayer = LevelEditorLayer::get();
        if (!editorLayer) return nullptr;
        return editorLayer->m_drawGridLayer;
    }

    bool init(cocos2d::CCNode* grid, LevelEditorLayer* editor) = win 0x16c4d0;
    virtual void draw() = win 0x16ce90, mac 0xa3c40;
    virtual void update(float) = win 0x16cd80;
    void clearPlayerPoints() {
        m_playerNodePoints->removeAllObjects();
        m_player2NodePoints->removeAllObjects();
    }

    void loadTimeMarkers(gd::string guidelines) = mac 0x94010;

    std::array<cocos2d::CCPoint, 400>* m_commonLines;
    std::array<cocos2d::CCPoint, 200>* m_yellowGuidelines;
    std::array<cocos2d::CCPoint, 200>* m_greenGuidelines;
    float m_songOffset1;
    float m_songOffset2;
    float m_lastMusicXPosition;
    bool m_effectSortDirty;
    LevelEditorLayer* m_editor;
    gd::string m_guidelineString;
    cocos2d::CCNode* m_grid;
    cocos2d::CCArray* m_guidelines;
    cocos2d::CCArray* m_effects;
    cocos2d::CCArray* m_guides;
    cocos2d::CCArray* m_speedObjects1;
    cocos2d::CCArray* m_speedObjects2;
    cocos2d::CCArray* m_playerNodePoints;
    cocos2d::CCArray* m_player2NodePoints;
    double m_unkDouble;
    float m_guidelineSpacing;
    float m_slowGuidelineSpacing;
    float m_normalGuidelineSpacing;
    float m_fastGuidelineSpacing;
    float m_fasterGuidelineSpacing;
    float m_fastestGuidelineSpacing;
    bool m_updatingTimeMarkers;
    bool m_timeNeedsUpdate;
    float m_activeGridNodeSize;
    float m_gridSize;
}

class EditButtonBar : cocos2d::CCNode {
    void removeAllItems() {
        m_buttonArray->removeAllObjects();
        this->reloadItemsInNormalSize();
    }
    void reloadItems(int rowCount, int columnCount) {
        if (m_buttonArray)
            this->loadFromItems(m_buttonArray, rowCount, columnCount, m_unknown);
    }
    void reloadItemsInNormalSize() {
        // TODO: fix this
        // this->reloadItems(
        //     GameManager::sharedState()->getIntGameVariable("0049"),
        //     GameManager::sharedState()->getIntGameVariable("0050")
        // );
    }
    void addButton(CCMenuItemSpriteExtra* btn, bool reload) {
        if (m_buttonArray)
            m_buttonArray->addObject(btn);
        if (reload)
            this->reloadItemsInNormalSize();
    }

    void loadFromItems(cocos2d::CCArray* buttons, int rowCount, int columnCount, bool idk) = mac 0x351010, win 0x6e5e0, ios 0x2dd060;
    static EditButtonBar* create(
        cocos2d::CCArray* buttons,
        cocos2d::CCPoint point,
        int idk, bool idk0,
        int rowCount, int columnCount
    ) = mac 0x350e60, win 0x6e450;

    cocos2d::CCPoint m_position;
    int m_unknown;
    bool m_unknownBool;
    cocos2d::CCArray* m_buttonArray;
    BoomScrollLayer* m_scrollLayer;
    cocos2d::CCArray* m_pagesArray;
}

class EditLevelLayer : cocos2d::CCLayer, FLAlertLayerProtocol, TextInputDelegate, UploadActionDelegate, UploadPopupDelegate, SetIDPopupDelegate {
    static cocos2d::CCScene* scene(GJGameLevel* level) {
        auto scene = cocos2d::CCScene::create();
        scene->addChild(EditLevelLayer::create(level));

        AppDelegate::get()->m_runningScene = scene;
        return scene;
    }

    static EditLevelLayer* create(GJGameLevel* level) = mac 0xe1e50, win 0x6f530, ios 0x82420;
    bool init(GJGameLevel* level) = mac 0xe1fd0, win 0x6f5d0;
    void onLevelInfo(cocos2d::CCObject*) = mac 0xe4f60, win 0x70660;
    void onPlay(cocos2d::CCObject*) = mac 0xe3ae0, win 0x71700;
    void onEdit(cocos2d::CCObject*) = mac 0xe3970, win 0x71ac0;
    void onShare(cocos2d::CCObject*) = mac 0xe3c60, win 0x71be0;

    cocos2d::CCMenu* m_buttonMenu;
    GJGameLevel* m_level;
    TextArea* m_descriptionInput;
    cocos2d::CCArray* m_someArray;
    cocos2d::CCLabelBMFont* m_someFont;
}

class EditorOptionsLayer {
    void onButtonsPerRow(cocos2d::CCObject*) = mac 0x147b30;
}

class EditorPauseLayer : CCBlockLayer, FLAlertLayerProtocol {
    static EditorPauseLayer* get() {
        if (!EditorUI::get()) return nullptr;

        auto editor = LevelEditorLayer::get();
        for (auto i = 0u; i < editor->getChildrenCount(); ++i) {
            if (auto layer = cast::safe_cast<EditorPauseLayer*>(editor->getChildren()->objectAtIndex(i))) {
                return layer;
            }
        }
        return nullptr;
    }
    static EditorPauseLayer* create(LevelEditorLayer* editor) {
        auto pRet = new EditorPauseLayer();
        if (pRet && pRet->init(editor)) {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
    inline EditorPauseLayer() {}
    ~EditorPauseLayer() = mac 0x13c3b0, win 0x73020;
    virtual void keyBackClicked() = mac 0x13f320, win 0x758d0;
    virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x13f3a0, win 0x758d0;
    virtual void customSetup() = mac 0x13cc00, win 0x73550;
    virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x13f1b0, win 0x75780;
    void saveLevel() = mac 0x13ebd0, win 0x75010;
    bool init(LevelEditorLayer*) = mac 0x13c7a0, win 0x730e0, ios 0x280cb8;
    void onExitEditor(cocos2d::CCObject* sender) = mac 0x13f180, win 0x75660;
    void playStep2() = mac 0x13f040, win 0x75440;
    void onResume(cocos2d::CCObject* sender) = mac 0x13e170, win 0x74fe0;
    void onSaveAndPlay(cocos2d::CCObject* sender) = mac 0x13e1b0, win 0x753d0;
    void onSaveAndExit(cocos2d::CCObject* sender) = mac 0x13e230, win 0x75620;
    void onSave(cocos2d::CCObject* sender) = mac 0x13e290, win 0x755a0;
    void onExitNoSave(cocos2d::CCObject* sender) = win 0x75700;
    void uncheckAllPortals(cocos2d::CCObject* sender) = win 0x74760;
    void onResetUnusedColors(cocos2d::CCObject* sender) = win 0x74810;
    void doResetUnused() = win 0x165070;
    void updateSongButton() = win 0x74f10, mac 0x13e530;
    void onSong(cocos2d::CCObject*) = win 0x74e70, mac 0x13e470;

    bool m_saved;
    CCMenuItemSpriteExtra* m_guidelinesOffButton;
    CCMenuItemSpriteExtra* m_guidelinesOnButton;
    LevelEditorLayer* m_editorLayer;
}

class EditorUI : cocos2d::CCLayer, FLAlertLayerProtocol, ColorSelectDelegate, GJRotationControlDelegate, GJScaleControlDelegate, MusicDownloadDelegate {
    static EditorUI* get() {
        auto lel = LevelEditorLayer::get();
        if (!lel) return nullptr;
        return lel->m_editorUI;
    }
    inline EditorUI() {}
    bool editButton2Usable() = win 0x8b890;
    inline void constrainGameLayerPosition() {
        this->constrainGameLayerPosition(-3.f, -6.f);
    }

    void create(LevelEditorLayer*) = mac 0x8a80, win 0x76270;
    cocos2d::CCArray* createCustomItems() = mac 0x1ddf0, win 0x7a370;
    void onDeleteCustomItem(cocos2d::CCObject* pSender) = win 0x7a280, mac 0x29860;
    void onNewCustomItem(cocos2d::CCObject* pSender) = win 0x79fd0, mac 0x24480;
    void deselectAll() = mac 0x1f300, win 0x86af0;
    void onDeselectAll(cocos2d::CCObject*) = mac 0x19cd0, win 0x86ac0;
    void disableButton(CreateMenuItem*) = mac 0x1c0f0, win 0x78af0;
    void editButtonUsable() = mac 0x28f30, win 0x8b040;
    void editObject(cocos2d::CCObject*) = mac 0x195a0, win 0x8ca50;
    void enableButton(CreateMenuItem*) = mac 0x1bff0, win 0x78990;
    CCMenuItemSpriteExtra* getCreateBtn(int, int) = mac 0x1f6c0, win 0x85120;
    cocos2d::CCPoint getGroupCenter(cocos2d::CCArray*, bool) = mac 0x23470, win 0x8fc30;
    cocos2d::CCArray* getSelectedObjects() = mac 0x23f30, win 0x86900;
    bool init(LevelEditorLayer*) = mac 0x8ae0, win 0x76310;
    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2ed60, win 0x907b0;
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2f3d0, win 0x90cd0;
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2fb00, win 0x911a0;
    virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x30790, win 0x91a30;
    virtual void keyUp(cocos2d::enumKeyCodes) = mac 0x312b0, win 0x92180;
    CreateMenuItem* menuItemFromObjectString(gd::string, int) = mac 0x1e130, win 0x84d00;
    void moveObject(GameObject*, cocos2d::CCPoint) = mac 0x24b10, win 0x8ddb0;
    void onDuplicate(cocos2d::CCObject*) = mac 0x18ba0, win 0x87d20;
    bool onCreate() = mac 0x1b960, win 0x85680;
    void onCreateObject(int) = mac 0x200d0, win 0x85750;
    void copyObjects(cocos2d::CCArray* objects) = win 0x87ea0;
    cocos2d::CCArray* pasteObjects(gd::string) = mac 0x232d0, win 0x88240;
    void playerTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2ebf0, win 0x90680;
    void playtestStopped() = mac 0x24790, win 0x87720;
    void redoLastAction(cocos2d::CCObject*) = mac 0xb8e0, win 0x870f0;
    void replaceGroupID(GameObject*, int, int) = mac 0x27470;
    virtual void scaleChangeBegin() = win 0x88d50;
    virtual void scaleChanged(float) = mac 0x25490, win 0x88df0;
    virtual void scaleChangeEnded() = win 0x88de0;
    void scaleObjects(cocos2d::CCArray*, float, cocos2d::CCPoint) = mac 0x252e0, win 0x8f150;
    void selectObjects(cocos2d::CCArray* objs, bool ignoreFilters) = mac 0x23940, win 0x864a0;
    void setupCreateMenu() = mac 0xcb50, win 0x7caf0;
    void undoLastAction(cocos2d::CCObject*) = mac 0xb830, win 0x87070;
    void updateButtons() = mac 0x1a300, win 0x78280;
    void updateEditMenu() = win 0x8b010;
    void updateObjectInfoLabel() = mac 0x1cb10, win 0x793b0;
    void updateSlider() = mac 0x18a90, win 0x78f10;
    void updateZoom(float) = mac 0x248c0, win 0x878a0;
    void selectObject(GameObject* obj, bool filter) = mac 0x1bd60, win 0x86250;
    void deselectObject(GameObject* object) = mac 0x1f220, win 0x86a50;
    void deleteObject(GameObject* object, bool filter) = mac 0x1f130, win 0x7bed0;
    void selectAll() = mac 0x23fd0, win 0x86c40;
    void selectAllWithDirection(bool left) = mac 0x24210, win 0x86d80;
    cocos2d::CCPoint getTouchPoint(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) = mac 0x2eb80, win 0x90620;
    cocos2d::CCPoint getGridSnappedPos(cocos2d::CCPoint pos) = mac 0x20b10, win 0x90550;
    void onSelectBuildTab(cocos2d::CCObject* sender) = mac 0x1fb60, win 0x887f0;
    void onCreateButton(cocos2d::CCObject* sender) = mac 0x1fd70, win 0x854f0;
    CCMenuItemSpriteExtra* getSpriteButton(const char* sprite, cocos2d::SEL_MenuHandler callback, cocos2d::CCMenu* menu, float scale) = mac 0xb500, win 0x78bf0;
    cocos2d::CCPoint offsetForKey(int objID) = win 0x92310;
    void updateDeleteMenu() = win 0x7c5d0, mac 0x1e960;
    void updateCreateMenu(bool updateTab) = mac 0x1ba80, win 0x85530;
    void toggleMode(cocos2d::CCObject* sender) = mac 0x187b0, win 0x7ad20;
    void zoomIn(cocos2d::CCObject* sender) = mac 0xc0c0, win 0x877c0;
    void zoomOut(cocos2d::CCObject* sender) = mac 0xc120, win 0x87830;
    void rotateObjects(cocos2d::CCArray* objects, float angle, cocos2d::CCPoint center) = mac 0x236a0, win 0x8ee80;
    void flipObjectsX(cocos2d::CCArray* objects) = mac 0x2ad10, win 0x8e550;
    void flipObjectsY(cocos2d::CCArray* objects) = mac 0x2aee0, win 0x8e9e0;
    void updateGridNodeSize() = mac 0x1c8a0, win 0x78f60;
    void updateScaleControl() {
        if (m_scaleControl->isVisible()) {
            m_scaleControl->loadValues(m_selectedObject, m_selectedObjects);
        }
    }
    void updateSpecialUIElements() {
        this->updateObjectInfoLabel();
        this->updateScaleControl();
    }
    void constrainGameLayerPosition(float x, float y) = mac 0x18890, win 0x8f920;
    void moveGamelayer(cocos2d::CCPoint pos) = mac 0x1ca90, win 0x79290;
    void showUI(bool show) = mac 0x245b0, win 0x87180;
    void resetUI() = mac 0x18520, win 0x7ac10;
    void editObject2(cocos2d::CCObject* sender) = win 0x8d1b0;
    void editGroup(cocos2d::CCObject* sender) = win 0x8d720;
    void moveObjectCall(cocos2d::CCObject* sender) = mac 0x29830, win 0x8db30;
    void moveObjectCall(EditCommand command) = mac 0x29b80, win 0x8db50;
    void transformObjectCall(cocos2d::CCObject* sender) = mac 0x29860, win 0x8def0;
    void transformObjectCall(EditCommand command) = mac 0x29d90, win 0x8df10;
    void transformObject(GameObject* obj, EditCommand command, bool snap) = mac 0x2b0b0, win 0x8e250;
    void onDelete(cocos2d::CCObject* sender) = mac 0x1b3d0, win 0x7b8d0;
    void onDeleteSelected(cocos2d::CCObject* sender) = mac 0xb990, win 0x7bf50;
    void onDeleteSelectedType(cocos2d::CCObject* sender) = mac 0x1e7a0, win 0x7c480;
    void onDeleteStartPos(cocos2d::CCObject* sender) = mac 0x1e770, win 0x7c4c0;
    void onCopy(cocos2d::CCObject* sender) = mac 0x18dc0, win 0x87fb0;
    void onPaste(cocos2d::CCObject* sender) = mac 0x18ee0, win 0x880c0;
    void tryUpdateTimeMarkers() = mac 0x1ca50, win 0x88940;
    void toggleEnableRotate(cocos2d::CCObject* sender) = mac 0xb700, win 0x860d0;
    void toggleFreeMove(cocos2d::CCObject* sender) = mac 0xb610, win 0x85eb0;
    void toggleSwipe(cocos2d::CCObject* sender) = mac 0xb490, win 0x85dd0;
    void toggleSnap(cocos2d::CCObject* sender) = mac 0xb680, win 0x85fa0;
    void onPlayback(cocos2d::CCObject* sender) = mac 0xbcb0, win 0x87340;
    void onPlaytest(cocos2d::CCObject* sender) = mac 0xbec0, win 0x87600;
    void onStopPlaytest(cocos2d::CCObject* sender) = mac 0xbfd0, win 0x876e0;
    void onGroupUp(cocos2d::CCObject* sender) = mac 0x1a1a0, win 0x8d780;
    void onGroupDown(cocos2d::CCObject* sender) = mac 0x1a200, win 0x8d7e0;
    void selectBuildTab(int tab) = mac 0x1fb90, win 0x88810;
    void onPause(cocos2d::CCObject* sender) = mac 0x18650, win 0x78020;
    void onSettings(cocos2d::CCObject* sender) = win 0x77fe0;
    void activateRotationControl(cocos2d::CCObject* sender) = win 0x8fe70, mac 0x24480;
    void activateScaleControl(cocos2d::CCObject* sender) = mac 0x24c80, win 0x889b0;
    void dynamicGroupUpdate(bool idk) = win 0x8ad10;
    void createRockOutline() = win 0x89c10;
    void createRockEdges() = win 0x88ec0;
    void createRockBase() = win 0x8a2c0;
    void onCopyState(cocos2d::CCObject* sender) = win 0x88490;
    void onPasteColor(cocos2d::CCObject* sender) = mac 0x19f40, win 0x88580;
    void onPasteState(cocos2d::CCObject* sender) = mac 0x19ee0, win 0x884c0;
    void onGroupSticky(cocos2d::CCObject* sender) = mac 0xc180, win 0x87a80;
    void onUngroupSticky(cocos2d::CCObject* sender) = mac 0xc1d0, win 0x87ac0;
    void onGoToLayer(cocos2d::CCObject* sender) = win 0x886b0;
    void onGoToBaseLayer(cocos2d::CCObject* sender) = win 0x88790;
    void onToggleGuide(cocos2d::CCObject* sender) = mac 0x19da0, win 0x79160;
    void editColor(cocos2d::CCObject* sender) = mac 0x19190, win 0x8d3c0;
    void alignObjects(cocos2d::CCArray* objs, bool alignY) = mac 0x2cea0, win 0x8f320;
    virtual void scrollWheel(float vertical, float horizontal) = win 0x921d0, mac 0x31370, ios 0x2c4884;
    void createMoveMenu() = mac 0x275e0, win 0x8c0d0;
    void sliderChanged(cocos2d::CCObject* slider) = mac 0xaed0, win 0x78cc0;
    void repositionObjectsToCenter(cocos2d::CCArray* objs, cocos2d::CCPoint center, bool ignoreGroupParent) = mac 0x1fcd0, win 0x88410;
    virtual void draw() = win 0x8fbe0;
    float valueFromXPos(float val) = win 0x78e30, mac 0x1c810;

    bool m_isPlayingMusic;
    EditButtonBar* m_buttonBar;
    PAD = mac 0x8, win 0x4;
    cocos2d::CCArray* m_hideableUIElementArray;
    float m_gridSize;
    PAD = mac 0x18, win 0x14;
    bool m_moveModifier;
    bool m_swipeModifier;
    int m_rotationTouchID;
    int m_scaleTouchID;
    int m_touchID;
    cocos2d::CCLabelBMFont* m_objectInfoLabel;
    GJRotationControl* m_rotationControl;
    cocos2d::CCPoint m_scalePos;
    bool m_touchDown;
    GJScaleControl* m_scaleControl;
    cocos2d::CCDictionary* m_editButtonDict;
    EditButtonBar* m_createButtonBar;
    EditButtonBar* m_editButtonBar;
    Slider* m_positionSlider;
    float m_unknown0;
    float m_minYLimit;
    float m_unknown2;
    bool m_swipeEnabled;
    bool m_swiping;
    PAD = mac 0x2, win 0x2, android 0x2;
    bool m_freeMoveEnabled;
    bool m_unkSwipeRelated;
    PAD = mac 0x2, win 0x2, android 0x2;
    bool m_updateTimeMarkers;
    PAD = mac 0x8, win 0x8, android 0x2;
    cocos2d::CCArray* m_unknownArray2;
    PAD = mac 0x8, win 0x8, android 0x8;
    cocos2d::CCArray* m_selectedObjects;
    cocos2d::CCMenu* m_deleteMenu;
    cocos2d::CCArray* m_unknownArray4;
    CCMenuItemSpriteExtra* m_deleteModeBtn;
    CCMenuItemSpriteExtra* m_buildModeBtn;
    CCMenuItemSpriteExtra* m_editModeBtn;
    CCMenuItemSpriteExtra* m_swipeBtn;
    CCMenuItemSpriteExtra* m_freeMoveBtn;
    CCMenuItemSpriteExtra* m_deselectBtn;
    CCMenuItemSpriteExtra* m_snapBtn;
    CCMenuItemSpriteExtra* m_rotateBtn;
    CCMenuItemSpriteExtra* m_playbackBtn;
    CCMenuItemSpriteExtra* m_playtestBtn;
    CCMenuItemSpriteExtra* m_playtestStopBtn;
    CCMenuItemSpriteExtra* m_trashBtn;
    CCMenuItemSpriteExtra* m_linkBtn;
    CCMenuItemSpriteExtra* m_unlinkBtn;
    CCMenuItemSpriteExtra* m_undoBtn;
    CCMenuItemSpriteExtra* m_redoBtn;
    CCMenuItemSpriteExtra* m_editObjectBtn;
    CCMenuItemSpriteExtra* m_editGroupBtn;
    CCMenuItemSpriteExtra* m_editHSVBtn;
    CCMenuItemSpriteExtra* m_editSpecialBtn;
    CCMenuItemSpriteExtra* m_copyPasteBtn;
    CCMenuItemSpriteExtra* m_copyBtn;
    CCMenuItemSpriteExtra* m_pasteBtn;
    CCMenuItemSpriteExtra* m_copyValuesBtn;
    CCMenuItemSpriteExtra* m_pasteStateBtn;
    CCMenuItemSpriteExtra* m_pasteColorBtn;
    CCMenuItemSpriteExtra* m_goToLayerBtn;
    CCMenuItemToggler* m_guideToggle;
    cocos2d::CCArray* m_createButtonBars;
    cocos2d::CCMenu* m_tabsMenu;
    cocos2d::CCArray* m_tabsArray;
    cocos2d::CCSprite* m_idkSprite0;
    cocos2d::CCSprite* m_idkSprite1;
    CCMenuItemSpriteExtra* m_button27;
    CCMenuItemSpriteExtra* m_button28;
    CCMenuItemSpriteExtra* m_deleteFilterNone;
    CCMenuItemSpriteExtra* m_deleteFilterStatic;
    CCMenuItemSpriteExtra* m_deleteFilterDetails;
    CCMenuItemSpriteExtra* m_deleteFilterCustom;
    cocos2d::CCLabelBMFont* m_currentLayerLabel;
    CCMenuItemSpriteExtra* m_layerNextBtn;
    CCMenuItemSpriteExtra* m_layerPrevBtn;
    CCMenuItemSpriteExtra* m_goToBaseBtn;
    PAD = mac 0x10, win 0x8;
    int m_selectedCreateObjectID;
    cocos2d::CCArray* m_createButtonArray;
    cocos2d::CCArray* m_customObjectButtonArray;
    cocos2d::CCArray* m_unknownArray9;
    int m_selectedMode;
    LevelEditorLayer* m_editorLayer;
    cocos2d::CCPoint m_swipeStart;
    cocos2d::CCPoint m_swipeEnd;
    PAD = mac 0x8, win 0x8;
    cocos2d::CCPoint m_lastTouchPoint;
    cocos2d::CCPoint m_cameraTest;
    PAD = mac 0x8, win 0x8;
    GameObject* m_selectedObject;
    PAD = mac 0x8, win 0x4;
    bool m_unk2cc;
    bool m_freeMovingObject;
    gd::string m_clipboard;
    PAD = mac 0x10, win 0x8;
    int m_selectedTab;
    int m_timesSelected;
    PAD = win 0x20;
    bool m_spaceKeyPressed;
}

class EffectGameObject : GameObject {
    void updateLabel() {
        auto label = static_cast<cocos2d::CCLabelBMFont*>(this->getChildByTag(999));
        if (label) {
            switch (m_objectID) {
                // instant count, collision block, pickup
                case 0x713: [[fallthrough]];
                case 0x718: [[fallthrough]];
                case 0x716: 
                    label->setString(
                        cocos2d::CCString::createWithFormat("%i", m_itemBlockAID)->getCString()
                    );
                    break;
                //   color,    pulse
                case 899: [[fallthrough]];
                case 1006: {
                    int target = m_objectID == 1006 ? m_targetGroupID : m_targetColorID;
                    if (target > 999) {
                        label->setString(GJSpecialColorSelect::textForColorIdx(target));
                    } else {
                        label->setString(
                            cocos2d::CCString::createWithFormat("%i", target)->getCString()
                        );
                    }
                    label->limitLabelWidth(30.0f, 0.5f, 0.0f);
                    } break;
                default:
                    label->setString(
                        cocos2d::CCString::createWithFormat("%i", m_targetGroupID)->getCString()
                    );
            }
        }
    }

    static EffectGameObject* create(const char*) = mac 0xc9790, win 0x253c30;
    bool init(char const*) = win 0x253CD0;
    void getTargetColorIndex() = mac 0xca1f0;
    virtual void triggerObject(GJBaseGameLayer*) = mac 0xc9870, win 0x253d60;
    virtual gd::string getSaveString() = win 0x257560;
    void updateSpecialColor() = win 0x254980;
    virtual void spawnXPosition() = win 0x254A00;
    virtual void triggerActivated(float) = win 0x254A30;

    cocos2d::ccColor3B m_colColor;
    float m_duration;
    float m_opacity;
    int m_targetGroupID;
    int m_centerGroupID;
    float m_shakeStrength;
    float m_shakeInterval;
    bool m_tintGround;
    bool m_playerColor1;
    bool m_playerColor2;
    bool m_blending;
    cocos2d::CCPoint m_move;
    EasingType m_easingType;
    float m_easingRate;
    bool m_lockToPlayerX;
    bool m_lockToPlayerY;
    bool m_useTarget;
    MoveTargetType m_moveTargetType;
    int m_rotateDegrees;
    int m_times360;
    bool m_lockObjectRotation;
    cocos2d::CCPoint m_followMod;
    bool UndocuementedLevelProperty74;
    float m_followYSpeed;
    float m_followYDelay;
    int m_followYOffset;
    float m_followYMaxSpeed;
    float m_fadeInTime;
    float m_holdTime;
    float m_fadeOutTime;
    int m_pulseHSVMode;
    int m_pulseGroupMode;
    cocos2d::ccHSVValue m_hsvValue;
    int m_copyColorID;
    bool m_copyOpacity;
    bool m_pulseMainOnly;
    bool m_pulseDetailOnly;
    bool m_pulseExclusive;
    bool m_activateGroup;
    bool m_touchHoldMode;
    TouchToggleMode m_touchToggleMode;
    bool m_touchDualMode;
    int m_animationID;
    float m_spawnDelay;
    cocos2d::CCPoint m_spawnPosition;
    bool m_multiTrigger;
    bool m_editorDisabled;
    int m_targetCount;
    bool m_subtractCount;
    ComparisonType m_comparisonType;
    bool m_multiActivate;
    bool m_triggerOnExit;
    int m_blockBID;
    bool m_dynamicBlock;
    int m_itemBlockAID;
    int m_pickupMode;
    PAD = mac 0x28, win 0x24;
}

class EndLevelLayer : cocos2d::CCLayer {
    static EndLevelLayer* create() = mac 0x2787d0, win 0x94b50;

    void customSetup() = win 0x94cb0;
    const char* getCoinString(void* p0) = win 0x96270;
    const char* getEndText() = win 0x964A0;
    void onMenu(cocos2d::CCObject* sender) = mac 0x27a500, win 0x96c10;
    void onEdit(cocos2d::CCObject* sender) = mac 0x27a640, win 0x96d30;
}

class EndPortalObject : GameObject {
    static EndPortalObject* create() = mac 0x1da8f0, win 0x98a50;
    void updateColors(cocos2d::_ccColor3B) = mac 0x1dacb0, win 0x98eb0;
}

class ExtendedLayer {}

class FLAlertLayer : cocos2d::CCLayerColor {
    inline virtual ~FLAlertLayer() {
        cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->decrementForcePrio(2);
    }
    inline FLAlertLayer() {
        m_buttonMenu = nullptr;
        m_controlConnected = -1;
        m_mainLayer = nullptr;
        m_ZOrder = 0;
        m_noElasticity = false;
        m_reverseKeyBack = false;
        m_scene = nullptr;
        m_alertProtocol = nullptr;
        m_scrollingLayer = nullptr;
        m_button2 = nullptr;
        m_button1 = nullptr;
        m_joystickConnected = -1;
        m_containsBorder = 0;
    }

    static FLAlertLayer* create(char const* title, const gd::string& desc, char const* btn) {
        return FLAlertLayer::create(nullptr, title, desc, btn, nullptr, 300.0);
    }

    virtual void onEnter() = mac 0x25f350, win 0x23750;
    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25ee40, win 0x233c0;
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25f0a0, win 0x23510;
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25ef60, win 0x23450;
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25f020, win 0x234c0;
    virtual void registerWithTouchDispatcher() = mac 0x25f2e0, win 0x236f0;
    virtual void keyBackClicked() = mac 0x25ed90, win 0x232c0;
    virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x25ece0, win 0x23250;
    virtual void show() = mac 0x25f120, win 0x23560, ios 0x1feff4;

    bool init(
        FLAlertLayerProtocol* protocol,
        char const* title,
        gd::string content,
        char const* btn1,
        char const* btn2,
        float width,
        bool scrollable,
        float height
    ) = mac 0x25e1b0, win 0x228e0;
    static FLAlertLayer* create(FLAlertLayerProtocol* protocol, char const* title, gd::string content, char const* btn1, char const* btn2) = mac 0x25de00, win 0x22680;
    static FLAlertLayer* create(FLAlertLayerProtocol* protocol, char const* title, gd::string content, char const* btn1, char const* btn2, float width) = mac 0x25e0e0, win 0x22730, ios 0x1fe374;
    static FLAlertLayer* create(FLAlertLayerProtocol* protocol, char const* title, gd::string content, char const* btn1, char const* btn2, float width, bool scrollable, float height) = mac 0x25dec0, win 0x227e0;
    void onBtn1(cocos2d::CCObject*) = mac 0x25ec20, win 0x23340;
    void onBtn2(cocos2d::CCObject*) = mac 0x25ec80, win 0x23380;

    cocos2d::CCMenu* m_buttonMenu;
    int m_controlConnected;
    FLAlertLayerProtocol* m_alertProtocol;
    cocos2d::CCNode* m_scene;
    bool m_reverseKeyBack;
    cocos2d::ccColor3B m_color;
    cocos2d::CCLayer* m_mainLayer;
    int m_ZOrder;
    bool m_noElasticity;
    cocos2d::ccColor3B m_color2;
    ButtonSprite* m_button1;
    ButtonSprite* m_button2;
    cocos2d::CCLayerColor* m_scrollingLayer;
    int m_joystickConnected;
    bool m_containsBorder;
    bool m_noAction;
}

class FLAlertLayerProtocol {
    virtual void FLAlert_Clicked(FLAlertLayer*, bool) {}
}

class FMODAudioEngine : cocos2d::CCNode {
    static FMODAudioEngine* sharedEngine() = mac 0x20ef80, win 0x239f0;
    void preloadEffect(gd::string filename) = win 0x24240;
    bool isBackgroundMusicPlaying() = win 0x24050;
    bool isBackgroundMusicPlaying(gd::string path) = win 0x24080;
    void playBackgroundMusic(gd::string path, bool fade, bool paused) = win 0x23d80;
    
    virtual void update(float) = win 0x23b20;

    cocos2d::CCDictionary* m_dictionary;
    std::string m_filePath;
    float m_backgroundMusicVolume;
    float m_effectsVolume;
    float m_pulse1;
    float m_pulse2;
    float m_pulse3;
    int m_pulseCounter;
    bool m_metering;
    bool m_fading;
    bool m_fadeIn;
    float m_fadeInDuration;
    FMOD::System* m_system;
    FMOD::Sound* m_sound;
    FMOD::Channel* m_currentSoundChannel;
    FMOD::Channel* m_globalChannel;
    FMOD::DSP* m_DSP;
    FMOD_RESULT m_lastResult;
    int m_version;
    void* m_extraDriverData;
    int m_musicOffset;
}

class FMODSound : cocos2d::CCNode {
    static FMODSound* create(FMOD::Sound* sound) = win 0x246b0;

    FMOD::Sound* m_sound;
}

class FriendRequestDelegate {}

class FriendsProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, UserListDelegate {
    static FriendsProfilePage* create(UserListType) = win 0x9ce80, mac 0x3a9570;
    bool init(UserListType) = win 0x9cf30, mac 0x3a9770;
}

class GJAccountBackupDelegate {
    virtual bool backupAccountFailed(BackupAccountError) {
        return false;
    }
    virtual bool backupAccountFinished() {
        return false;
    }
}

class GJAccountDelegate {
    virtual bool accountStatusChanged() {
        return false;
    }
}

class GJAccountLoginDelegate {
    virtual bool loginAccountFailed(AccountError) {
        return false;
    }
    virtual bool loginAccountFinished(int, int) {
        return false;
    }
}

class GJAccountManager : cocos2d::CCNode {
    static GJAccountManager* sharedState() = mac 0x85070, win 0x107d50;
    static GJAccountManager* get() {
        return GJAccountManager::sharedState();
    }
    gd::string getGJP() = mac 0x89520, win 0x10abb0;

    PAD = mac 0x8, win 0x4;
    gd::string m_password;
    gd::string m_username;
    int m_accountID;
    int m_playerID;
}

class GJAccountSyncDelegate {
    virtual bool syncAccountFailed(BackupAccountError) {
        return false;
    }
    virtual bool syncAccountFinished() {
        return false;
    }
}

class GJBaseGameLayer : cocos2d::CCLayer, TriggerEffectDelegate {
    cocos2d::CCLayer* getObjectLayer() {
        return m_objectLayer;
    }
    cocos2d::CCArray* getAllObjects() {
        return m_objects;
    }

    static GJBaseGameLayer* get() {
        return PlayLayer::get() ? static_cast<GJBaseGameLayer*>(PlayLayer::get()) : static_cast<GJBaseGameLayer*>(LevelEditorLayer::get());
    }

    inline GJBaseGameLayer() {}

    virtual void objectsCollided(int, int) = mac 0xb6d90, win 0x10f240;
    virtual void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int) = mac 0xb73a0, win 0x10f680;
    virtual void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*) = mac 0xb7420, win 0x10f6d0;
    virtual void flipGravity(PlayerObject*, bool, bool) {}
    virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0xba9a0, win 0x10ad60;
    virtual void toggleGroupTriggered(int, bool) = mac 0xb75a0, win 0x10f890;
    virtual void spawnGroup(int) = mac 0xb7050, win 0x10f400;
    virtual void addToSection(GameObject*) = mac 0xb7b70, win 0x10fd00;
    virtual void addToGroup(GameObject*, int, bool) = mac 0xb77f0, win 0x10fa50;
    virtual void removeFromGroup(GameObject*, int) = mac 0xb7a60, win 0x10fbd0;
    virtual bool init() = mac 0xafc90, win 0x10b020;
    void addObjectCounter(LabelGameObject*, int) = mac 0xb9eb0;
    void addToGroups(GameObject*, bool) = mac 0xb7780, win 0x10f9f0;
    void atlasValue(int) = mac 0xb21e0;
    void bumpPlayer(PlayerObject*, GameObject*) = mac 0xb6860, win 0x10ed50;
    void calculateOpacityValues(EffectGameObject*, EffectGameObject*, float, GJEffectManager*) = mac 0xb5be0;
    void checkSpawnObjects() = mac 0xb6f90, win 0x10f320;
    void collectItem(int, int) = mac 0xb9e20, win 0x111890;
    void collectedObject(EffectGameObject*) = mac 0xb9b60, win 0x111830;
    void createTextLayers() = mac 0xb5260, win 0x10dea0;
    cocos2d::CCArray* damagingObjectsInRect(cocos2d::CCRect) = mac 0xb6140, win 0x10e740;
    void enableHighCapacityMode() = mac 0xb11e0, win 0x10bf60;
    void getCapacityString() = mac 0xb2210, win 0x10c9b0;
    void getGroundHeightForMode(int) = mac 0xb6630;
    void getGroup(int) = mac 0xb6f20, win 0x10fcb0;
    void getMoveDeltaForObjects(int, int) = mac 0xb6db0;
    void getOptimizedGroup(int) = mac 0xb7940;
    void getStaticGroup(int) = mac 0xb79a0;
    void isGroupDisabledForObject(GameObject*) = mac 0xb5cc0;
    void isGroupDisabledForObjectFull(GameObject*, cocos2d::CCArray*) = mac 0xb5de0;
    void loadUpToPosition(float) = mac 0xba680, win 0x112070;
    void objectIntersectsCircle(GameObject*, GameObject*) = mac 0xb66e0, win 0x10e990;
    void objectTriggered(EffectGameObject*) = mac 0xb71b0;
    void optimizeMoveGroups() = mac 0xb96c0, win 0x1112f0;
    cocos2d::CCNode* parentForZLayer(int, bool, int) = mac 0xb55d0, win 0x10e220;
    void playerTouchedRing(PlayerObject*, GameObject*) = mac 0xb69e0;
    void processColorObject(EffectGameObject*, int, cocos2d::CCDictionary*, float, GJEffectManager*) = mac 0xb5a90;
    void processFollowActions() = mac 0xb8fd0, win 0x110e00;
    void processMoveActions() = mac 0xb86c0, win 0x1107e0;
    void processMoveActionsStep(float) = mac 0xb7ea0, win 0x10ffa0;
    void processOpacityObject(EffectGameObject*, cocos2d::CCDictionary*, float, GJEffectManager*) = mac 0xb5ae0;
    void processPlayerFollowActions(float) = mac 0xb8b50, win 0x110b10;
    void processRotationActions() = mac 0xb7fd0, win 0x1101a0;
    void pushButton(int, bool) = mac 0xb9920, win 0x111500;
    void rectIntersectsCircle(cocos2d::CCRect, cocos2d::CCPoint, float) = mac 0xb6470, win 0x10eb50;
    void refreshCounterLabels() = mac 0xb9fc0, win 0x111b80;
    void releaseButton(int, bool) = mac 0xb9a00, win 0x111660;
    void removeFromGroups(GameObject*) = mac 0xb7a00, win 0x10fb70;
    void removeObjectFromSection(GameObject*) = mac 0xb7e00, win 0x10ff30;
    void reorderObjectSection(GameObject*) = mac 0xb7cb0, win 0x10fe10;
    void resetGroupCounters(bool) = mac 0xba300, win 0x111dd0;
    void resetMoveOptimizedValue() = mac 0xb9670;
    // inlined on windows
    int sectionForPos(float x) {
        int section = x / 100;
        if (section < 0) section = 0;
        return section;
    }
    void setupLayers() = mac 0xaffe0, win 0x10b1f0;
    void shouldExitHackedLevel() = mac 0xb1100, win 0x10bea0;
    void spawnGroupTriggered(int, float, int) = mac 0xb7020;
    cocos2d::CCArray* staticObjectsInRect(cocos2d::CCRect) = mac 0xb5f90;
    void testInstantCountTrigger(int, int, int, bool, int) = mac 0xb9ae0;
    void toggleGroup(int, bool) = mac 0xb75f0, win 0x10f8c0;
    void togglePlayerVisibility(bool) = mac 0xba910, win 0x112340;
    void triggerMoveCommand(EffectGameObject*) = mac 0xb7290;
    void updateCollisionBlocks() = mac 0xb6a30, win 0x10ef70;
    void updateCounters(int, int) = mac 0xb9bc0, win 0x111900;
    void updateDisabledObjectsLastPos(cocos2d::CCArray*) = mac 0xb95b0, win 0x111280;
    void updateLayerCapacity(gd::string) = mac 0xb1680, win 0x10c2d0;
    void updateLegacyLayerCapacity(int, int, int, int) = mac 0xb1590, win 0x10c200;
    void updateOBB2(cocos2d::CCRect) = mac 0xb63f0;
    void updateQueuedLabels() = mac 0xb9f30, win 0x111b00;
    virtual ~GJBaseGameLayer() = mac 0xaf990, win 0x10add0;

    OBB2D* m_boundingBox;
    GJEffectManager* m_effectManager;
    cocos2d::CCLayer* m_objectLayer;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddTop4;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddTop4;
    cocos2d::CCSpriteBatchNode* m_batchNodeTop3;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddTop3;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddGlowTop3;
    CCNodeContainer* m_batchNodeTop3Container;
    cocos2d::CCSpriteBatchNode* m_batchNodeTextTop3;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddTextTop3;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeTop3;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddTop3;
    cocos2d::CCSpriteBatchNode* m_batchNodeTop2;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddTop2;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddGlowTop2;
    CCNodeContainer* m_batchNodeTop2Container;
    cocos2d::CCSpriteBatchNode* m_batchNodeTextTop2;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddTextTop2;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeTop2;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddTop2;
    cocos2d::CCSpriteBatchNode* m_batchNode;
    cocos2d::CCSpriteBatchNode* m_batchNodeAdd;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddGlow;
    CCNodeContainer* m_batchNodeTop1Container;
    cocos2d::CCSpriteBatchNode* m_batchNodeTextTop1;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddTextTop1;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeTop1;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddTop1;
    cocos2d::CCSpriteBatchNode* m_batchNodePlayer;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddPlayer;
    cocos2d::CCSpriteBatchNode* m_batchNodePlayerGlow;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddMid;
    cocos2d::CCSpriteBatchNode* m_batchNodeBottom;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddBottomGlow;
    CCNodeContainer* m_batchNodeBot1Container;
    cocos2d::CCSpriteBatchNode* m_batchNodeText;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddText;
    cocos2d::CCSpriteBatchNode* m_effectBatchNode;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeAdd;
    cocos2d::CCSpriteBatchNode* m_batchNodeBottom2;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom2;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom2Glow;
    CCNodeContainer* m_batchNodeBot2Container;
    cocos2d::CCSpriteBatchNode* m_batchNodeTextBot2;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddTextBot2;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeBot2;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddBot2;
    cocos2d::CCSpriteBatchNode* m_batchNodeBottom3;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom3;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom3Glow;
    CCNodeContainer* m_batchNodeBot3Container;
    cocos2d::CCSpriteBatchNode* m_batchNodeTextBot3;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddTextBot3;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeBot3;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddBot3;
    cocos2d::CCSpriteBatchNode* m_batchNodeBottom4;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom4;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom4Glow;
    CCNodeContainer* m_batchNodeBot4Container;
    cocos2d::CCSpriteBatchNode* m_batchNodeTextBot4;
    cocos2d::CCSpriteBatchNode* m_batchNodeAddTextBot4;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeBot4;
    cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddBot4;
    PlayerObject* m_player1;
    PlayerObject* m_player2;
    LevelSettingsObject* m_levelSettings;
    cocos2d::CCDictionary* m_disabledGroupsDict;
    cocos2d::CCArray* m_objects;
    cocos2d::CCArray* m_sectionObjects;
    cocos2d::CCArray* m_sections;
    cocos2d::CCArray* m_collisionBlocks;
    cocos2d::CCArray* m_spawnObjects;
    cocos2d::CCArray* m_spawnObjects2;
    cocos2d::CCNode* m_groupNodes;
    gd::vector<GameObject*> m_objectsVec;
    gd::vector<GameObject*> m_disabledObjects;
    cocos2d::CCDictionary* m_groupDict;
    cocos2d::CCDictionary* m_staticGroupDict;
    cocos2d::CCDictionary* m_optimizedGroupDict;
    gd::vector<cocos2d::CCArray*> m_groups;
    gd::vector<cocos2d::CCArray*> m_staticGroups;
    gd::vector<cocos2d::CCArray*> m_optimizedGroups;
    cocos2d::CCArray* m_batchNodes;
    cocos2d::CCArray* m_processedGroups;
    cocos2d::CCDictionary* m_counterDict;
    cocos2d::CCDictionary* m_spawnedGroups;
    bool m_didUpdateNormalCapacity;
    bool m_isDualMode;
    int m_activeEnterEffect;
    bool m_activeDualTouch;
    int m_attemptClickCount;
    int m_firstVisibleSection;
    int m_lastVisibleSection;
    bool m_objectsAreDisabled;
    bool m_blending;
    PAD = mac 0x8, win 0x8;
}

class GJChallengeDelegate {}

class GJChallengeItem : cocos2d::CCObject {

    GJChallengeType m_challengeType;
    geode::SeedValueSRV m_count;
    geode::SeedValueSRV m_reward;
    geode::SeedValueSRV m_goal;
    int m_timeLeft;
    bool m_canClaim;
    int m_position;
    gd::string m_name;
}

class GJColorSetupLayer {}

class GJComment : cocos2d::CCNode {
    ~GJComment() = mac 0x2dfd70, win 0xc3640;
    virtual bool init() = mac 0x2dfec0, win 0xc3ef0;
    static GJComment* create(cocos2d::CCDictionary* dict) = win 0xc3740;

    gd::string m_commentString;
    gd::string m_userName;
    int m_commentID;
    int m_userID;
    int m_likeCount;
    int m_levelID;
    bool m_isSpam;
    int m_accountID;
    gd::string m_uploadDate;
    bool m_commentDeleted;
    int m_percentage;
    int m_modBadge;
    cocos2d::ccColor3B m_color;
    bool m_hasLevelID;
    GJUserScore* m_userScore;
}

class GJCommentListLayer : cocos2d::CCLayerColor {
    ~GJCommentListLayer() = mac 0x1482a0;
    static GJCommentListLayer* create(BoomListView*, char const*, cocos2d::_ccColor4B, float, float, bool) = mac 0x147d00, win 0x13a880;

    BoomListView* m_list;
}

class GJDailyLevelDelegate {
    virtual void dailyStatusFinished(bool) {}
    virtual void dailyStatusFailed(bool) {}
}

class GJDropDownLayer : cocos2d::CCLayerColor {
    virtual void customSetup() {}
    virtual void enterLayer() = mac 0x3525c0, win 0x16970;
    virtual void exitLayer(cocos2d::CCObject*) = mac 0x352670, win 0x113980;
    virtual void showLayer(bool) = mac 0x3526c0, win 0x1139c0, ios 0x29801c;
    virtual void hideLayer(bool) = mac 0x3527b0, win 0x113a90;
    virtual void layerVisible() = mac 0x3528b0, win 0x16a40;
    virtual void layerHidden() = mac 0x3528d0, win 0x113b60;
    virtual void enterAnimFinished() {}
    virtual void disableUI() = mac 0x352580, win 0x113920;
    virtual void enableUI() = mac 0x3525a0, win 0x113940;

    static GJDropDownLayer* create(const char* title, float height) {
        GJDropDownLayer* pRet = new GJDropDownLayer();
        if (pRet && pRet->init(title, height)) {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
        return true;
    }
    virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}
    virtual void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}

    virtual void draw() = mac 0x352910, win 0x16a80;
    bool init(const char* title, float height) = mac 0x352100, win 0x113530, ios 0x297afc;
    virtual void registerWithTouchDispatcher() = mac 0x3525f0, win 0x16990;
    virtual void keyBackClicked() = mac 0x352630, win 0x113960;
    inline GJDropDownLayer() {
        m_endPosition = cocos2d::CCPointMake(0.f, 0.f);
        m_startPosition = cocos2d::CCPointMake(0.f, 0.f);
        m_buttonMenu = nullptr;
        m_listLayer = nullptr;
        m_controllerEnabled = false;
        m_mainLayer = nullptr;
        m_hidden = false;
        m_delegate = nullptr;
    }

    cocos2d::CCPoint m_endPosition;
    cocos2d::CCPoint m_startPosition;
    cocos2d::CCMenu* m_buttonMenu;
    GJListLayer* m_listLayer;
    bool m_controllerEnabled;
    cocos2d::CCLayer* m_mainLayer;
    bool m_hidden;
    GJDropDownLayerDelegate* m_delegate;
}

class GJDropDownLayerDelegate {
    virtual void dropDownLayerWillClose(GJDropDownLayer*) {}
}

class GJEffectManager : cocos2d::CCNode {
    static GJEffectManager* get() {
        return GJEffectManager::fromGameLayer();
    }

    static GJEffectManager* fromGameLayer() {
        auto gameLayer = GJBaseGameLayer::get();
        if (gameLayer) return gameLayer->m_effectManager;
        return nullptr;
    }

    static GJEffectManager* fromLevelSetting() {
        auto gameLayer = GJBaseGameLayer::get();
        if (gameLayer && gameLayer->m_levelSettings) return gameLayer->m_levelSettings->m_effectManager;
        return nullptr;
    }

    virtual bool init() = mac 0x180230, win 0x11c1b0;
    cocos2d::_ccColor3B activeColorForIndex(int) = mac 0x180cb0, win 0x11c6e0;
    void activeOpacityForIndex(int) = mac 0x180e10;
    void addAllInheritedColorActions(cocos2d::CCArray*) = mac 0x1817a0;
    void addGroupPulseEffect(PulseEffectAction*) = mac 0x184c10;
    void calculateBaseActiveColors() = mac 0x180f70, win 0x11c7c0;
    void calculateInheritedColor(int, ColorAction*) = mac 0x1818f0, win 0x11cb80;
    void calculateLightBGColor(cocos2d::_ccColor3B) = mac 0x185b90, win 0x11f420;
    void colorActionChanged(ColorAction* action) {
        if (0 < action->m_copyID) {

        }
    }
    void colorExists(int) = mac 0x181da0;
    void colorForEffect(cocos2d::_ccColor3B, cocos2d::_ccHSVValue) = mac 0x182650;
    cocos2d::_ccColor3B colorForGroupID(int, cocos2d::_ccColor3B const&, bool) = mac 0x184f90;
    void colorForIndex(int) = mac 0x180ad0;
    void colorForPulseEffect(cocos2d::_ccColor3B const&, PulseEffectAction*) = mac 0x181bf0, win 0x11edd0;
    void countChangedForItem(int) = mac 0x185a40, win 0x11f330;
    void countForItem(int) = mac 0x185a10;
    static GJEffectManager* create() = mac 0x1800f0, win 0x11c110;
    void createFollowCommand(float, float, float, int, int, bool, int) = mac 0x182ed0;
    void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int) = mac 0x182cc0, win 0x11d340;
    void createPlayerFollowCommand(float, float, int, float, float, int, int) = mac 0x182fe0;
    void createRotateCommand(int, float, int, int, int, float, bool, int) = mac 0x182df0;
    void getAllColorActions() = mac 0x180980, win 0x11c600;
    void getAllColorSprites() = mac 0x1809e0;
    ColorAction* getColorAction(int) = mac 0x180b00, win 0x11cde0;
    const cocos2d::_ccColor3B& getColorSprite(int) = mac 0x180d00, win 0x11ce20;
    void getCurrentStateString() = mac 0x1867e0, win 0x11fac0;
    void getLoadedMoveOffset() = mac 0x184390;
    static cocos2d::_ccColor3B getMixedColor(cocos2d::_ccColor3B, cocos2d::_ccColor3B, float) = mac 0x185d30, win 0x11f610;
    uint8_t getOpacityActionForGroup(int) = mac 0x1845b0;
    gd::string getSaveString() = mac 0x185e90;
    void handleObjectCollision(bool, int, int) = mac 0x1828f0, win 0x11d2a0;
    void hasActiveDualTouch() = mac 0x185540;
    void hasBeenTriggered(int) = mac 0x1853b0;
    void hasPulseEffectForGroupID(int) = mac 0x184f60;
    bool isGroupEnabled(int) = mac 0x1853d0;
    void keyForGroupIDColor(int, cocos2d::_ccColor3B const&, bool) = mac 0x184c90;
    void loadState(gd::string) = mac 0x188db0, win 0x1205b0;
    void objectsCollided(int, int) = mac 0x182a00, win 0x11d200;
    void opacityForIndex(int) = mac 0x180c80;
    float opacityModForGroup(int) = mac 0x184740;
    void playerButton(bool, bool) = mac 0x1855a0, win 0x11f120;
    void playerDied() = mac 0x185860;
    void postCollisionCheck() = mac 0x182720, win 0x11d0b0;
    void preCollisionCheck() = mac 0x182680, win 0x11d030;
    void prepareMoveActions(float, bool) = mac 0x183660, win 0x11da30;
    void processColors() { // mac 0x180e70
        this->calculateBaseActiveColors();
        this->processPulseActions();
        this->processInheritedColors();
        this->processCopyColorPulseActions();
    }
    void processCopyColorPulseActions() = mac 0x181530, win 0x11ebc0;
    void processInheritedColors() = mac 0x181190, win 0x11c8a0;
    void processPulseActions() = mac 0x181040, win 0x11ea50;
    void registerCollisionTrigger(int, int, int, bool, bool, int) = mac 0x182b70;
    void removeAllPulseActions() = mac 0x1825e0;
    void removeColorAction(int) = mac 0x181d60;
    void reset() = mac 0x180690, win 0x11c4f0;
    void resetColorCache() = mac 0x185280;
    void resetEffects() = mac 0x1807d0, win 0x11cf10;
    void resetMoveActions() = mac 0x180940;
    void resetToggledGroups() = mac 0x1853f0;
    void resetTriggeredIDs() = mac 0x182630;
    void runCountTrigger(int, int, bool, int, bool, int) = mac 0x1858d0;
    void runDeathTrigger(int, bool, int) = mac 0x1857a0;
    OpacityEffectAction* runOpacityActionOnGroup(int, float, float, int) = mac 0x1845d0;
    void runPulseEffect(int, bool, float, float, float, PulseEffectType, cocos2d::_ccColor3B, cocos2d::_ccHSVValue, int, bool, bool, bool, int) = mac 0x184890, win 0x11e5c0;
    void runTouchTriggerCommand(int, bool, TouchTriggerType, bool, int) = mac 0x185460;
    void setColorAction(ColorAction*, int) = mac 0x181d00, win 0x11ce70;
    void setFollowing(int, int, bool) = mac 0x185e00;
    void setupFromString(gd::string) = mac 0x186290, win 0x11f8f0;
    bool shouldBlend(int) = mac 0x180e40;
    void spawnGroup(int, float, int) = mac 0x1852a0;
    void stopActionsForTrigger(EffectGameObject*) = mac 0x183150;
    void stopMoveActionsForGroup(int) = mac 0x1830e0;
    void storeTriggeredID(int) = mac 0x185380;
    void toggleGroup(int item, bool value) { // mac 0x182c80;
        m_groupToggled[item] = value;
    }
    void traverseInheritanceChain(InheritanceNode*) = mac 0x181850, win 0x11caf0;
    void updateActiveOpacityEffects() = mac 0x1847e0;
    void updateColorAction(ColorAction*) = mac 0x184560;
    void updateColorEffects(float) = mac 0x181f40, win 0x11e1d0;
    void updateColors(cocos2d::_ccColor3B, cocos2d::_ccColor3B) = mac 0x180a40, win 0x11c660;
    void updateEffects(float unk) {
        this->updateColorEffects(unk);
        this->updatePulseEffects(unk);
        this->updateOpacityEffects(unk);
        this->updateSpawnTriggers(unk);
    }
    void updateOpacityAction(OpacityEffectAction*) = mac 0x184780;
    void updateOpacityEffects(float) = mac 0x1823e0, win 0x11e490;
    void updatePulseEffects(float) = mac 0x182130, win 0x11e7f0;
    void updateSpawnTriggers(float) = mac 0x182510, win 0x11f000;
    void wasFollowing(int, int) = mac 0x185e60;
    void wouldCreateLoop(InheritanceNode*, int) = mac 0x181820, win 0x11cdb0;
    ~GJEffectManager() = mac 0x17fe00, win 0x11be80;

    TriggerEffectDelegate* m_effectDelegate;
    cocos2d::CCDictionary* m_colorActions;
    cocos2d::CCDictionary* m_colorSprites;
    cocos2d::CCDictionary* m_pulseActionsForGroup;
    cocos2d::CCDictionary* m_colorCache;
    cocos2d::CCDictionary* m_opacityActionsForGroup;
    cocos2d::CCDictionary* m_f0150;
    cocos2d::CCArray* m_opacityActions;
    cocos2d::CCArray* m_touchActions;
    cocos2d::CCDictionary* m_countActions;
    cocos2d::CCArray* m_onDeathActions;
    cocos2d::CCArray* m_collisionActions;
    cocos2d::CCDictionary* m_f0180;
    cocos2d::CCDictionary* m_f0188;
    gd::vector<InheritanceNode*> m_inheritanceNodesForColor;
    cocos2d::CCDictionary* m_f01a8;
    cocos2d::CCDictionary* m_collisionActionsForGroup1;
    cocos2d::CCDictionary* m_collisionActionsForGroup2;
    gd::vector<ColorAction*> m_colorActionsForColor;
    gd::vector<ColorActionSprite*> m_colorSpritesForColor;
    std::array<bool, 1100> m_pulseExistsForGroup;
    bool m_f063c;
    std::array<bool, 1100> m_opactiyExistsForGroup;
    std::array<int, 1100> m_itemValues;
    int m_unusued;
    int* m_unused2;
    cocos2d::CCArray* m_f1bc8;
    cocos2d::CCArray* m_inheritanceChain;
    cocos2d::CCDictionary* m_f1bd8;
    gd::vector<bool> m_groupToggled;
    cocos2d::CCDictionary* m_triggeredIDs;
    cocos2d::CCDictionary* m_followActions;
    cocos2d::CCArray* m_spawnActions;
    cocos2d::CCArray* m_moveActions;
    cocos2d::CCArray* m_groupCommands; // array of GroupCommandObject
    cocos2d::CCNode* m_f1c30;
    cocos2d::CCDictionary* m_moveNodes; // dict of CCMoveCNode
    cocos2d::CCDictionary* m_rotationNodes; // dict of CCMoveCNode
    cocos2d::CCDictionary* m_followNodes; // dict of CCMoveCNode
    cocos2d::CCDictionary* m_playerFollowNodes; // dict of CCMoveCNode
    float m_time;
    float m_velocity;
    float m_acceleration;
    bool m_moveOptimizationEnabled;
}

class GJGameLevel : cocos2d::CCNode {
    static GJGameLevel* createWithCoder(DS_Dictionary* dict) {
        auto ret = GJGameLevel::create();
        ret->dataLoaded(dict);
        return ret;
    }

    inline GJGameLevel() {}
    virtual ~GJGameLevel() = mac 0x2db2d0, win 0xbbdb0;
    virtual void encodeWithCoder(DS_Dictionary*) = mac 0x2dd1c0, win 0xbecb0;
    virtual bool canEncode() = mac 0x2ddae0, win 0x11070; // shared with lots of stuff
    virtual bool init() = mac 0x2db310, win 0xbd350;
    static GJGameLevel* create() = mac 0x2b83e0, win 0xbd2b0, ios 0x51fe8;
    gd::string getAudioFileName() = mac 0x2dbe70, win 0xbdc70;
    const char* getCoinKey(int) = mac 0x2ce360, win 0xbda50;
    void getLengthKey(int) = mac 0x2dbba0;
    void getNormalPercent() = mac 0x2b8b20;
    void levelWasAltered() = mac 0x2db530, win 0xbd550;
    void savePercentage(int, bool, int, int, bool) = mac 0x2db700, win 0xbd5c0;
    void dataLoaded(DS_Dictionary* dict) = mac 0x2dc0e0, win 0xbded0, ios 0x6fca4;
    GJDifficulty getAverageDifficulty() = win 0xbd9b0;
    gd::string getUnpackedLevelDescription() = mac 0x2DDB50, win 0xbf890;
    gd::string lengthKeyToString(int key) = win 0xbd910;

    static GJGameLevel* getCurrent() {
        auto playLayer = PlayLayer::get();
        if (playLayer) return playLayer->m_level;
        auto editorLayer = LevelEditorLayer::get();
        if (editorLayer) return editorLayer->m_level;
        return nullptr;
    }

    cocos2d::CCDictionary* m_lastBuildSave;
    geode::SeedValueRSV m_levelID;
    gd::string m_levelName;
    gd::string m_levelDesc;
    gd::string m_levelString;
    gd::string m_creatorName;
    gd::string m_recordString;
    gd::string m_uploadDate;
    gd::string m_updateDate;
    geode::SeedValueRSV m_userID;
    geode::SeedValueRSV m_accountID;
    GJDifficulty m_difficulty;
    int m_audioTrack;
    int m_songID;
    int m_levelRev;
    bool m_unlisted;
    geode::SeedValueRSV m_objectCount;
    int m_levelIndex;
    int m_ratings;
    int m_ratingsSum;
    int m_downloads;
    bool m_isEditable;
    bool m_gauntletLevel;
    bool m_gauntletLevel2;
    int m_workingTime;
    int m_workingTime2;
    bool m_lowDetailMode;
    bool m_lowDetailModeToggled;
    geode::SeedValueRS m_isVerified;
    bool m_isVerifiedRaw; // honestly i dont think this is need to be used
    bool m_isUploaded;
    bool m_hasBeenModified;
    int m_levelVersion;
    int m_gameVersion;
    geode::SeedValueRSV m_attempts;
    geode::SeedValueRSV m_jumps;
    geode::SeedValueRSV m_clicks;
    geode::SeedValueRSV m_attemptTime;
    int m_chk;
    bool m_isChkValid;
    bool m_isCompletionLegitimate;
    geode::SeedValueVSR m_normalPercent;
    geode::SeedValueRSV m_orbCompletion;
    geode::SeedValueRSV m_newNormalPercent2;
    int m_practicePercent;
    int m_likes;
    int m_dislikes;
    int m_levelLength;
    int m_featured;
    bool m_isEpic;
    bool m_levelFavorited;
    int m_levelFolder;
    geode::SeedValueRSV m_dailyID;
    geode::SeedValueRSV m_demon;
    int m_demonDifficulty;
    geode::SeedValueRSV m_stars;
    bool m_autoLevel;
    int m_coins;
    geode::SeedValueRSV m_coinsVerified;
    geode::SeedValueRS m_password;
    geode::SeedValueRSV m_originalLevel;
    bool m_twoPlayerMode;
    int m_failedPasswordAttempts;
    geode::SeedValueRSV m_firstCoinVerified;
    geode::SeedValueRSV m_secondCoinVerified;
    geode::SeedValueRSV m_thirdCoinVerified;
    int m_starsRequested;
    bool m_showedSongWarning;
    int m_starRatings;
    int m_starRatingsSum;
    int m_maxStarRatings;
    int m_minStarRatings;
    int m_demonVotes;
    int m_rateStars;
    int m_rateFeature;
    gd::string m_rateUser;
    bool m_dontSave;
    bool m_levelNotDownloaded;
    int m_requiredCoins;
    bool m_isUnlocked;
    cocos2d::CCPoint m_lastCameraPos;
    float m_fastEditorZoom;
    int m_lastBuildTab;
    int m_lastBuildPage;
    int m_lastBuildGroupID;
    GJLevelType m_levelType;
    int m_M_ID;
    gd::string m_tempName;
    gd::string m_capacityString;
    bool m_highObjectsEnabled;
    gd::string m_personalBests;
}

class GJGarageLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, GameRateDelegate, ListButtonBarDelegate, DialogDelegate {
    inline GJGarageLayer() {
        m_nameInput = nullptr;
        m_playerPreview = nullptr;
        m_colorSelector1 = nullptr;
        m_colorSelector2 = nullptr;
        m_unkButton0x148 = nullptr;
        m_unkButton0x14c = nullptr;
        m_pagesArray = nullptr;
        m_tabToggleCube = nullptr;
        m_tabToggleShip = nullptr;
        m_tabToggleBall = nullptr;
        m_tabToggleUfo = nullptr;
        m_tabToggleWave = nullptr;
        m_tabToggleRobot = nullptr;
        m_tabToggleSpider = nullptr;
        m_tabToggleSpecial = nullptr;
        m_tabToggleDeathEffect = nullptr;
        m_updateSelector = false;
    }
    void onPlayerColor1(cocos2d::CCObject*) = mac 0x1ba640, win 0x129470, ios 0x22531c;
    void onPlayerColor2(cocos2d::CCObject*) = mac 0x1ba8c0, win 0x129590, ios 0x225408;
    static GJGarageLayer* node() = win 0x125220;
    virtual bool init() = mac 0x1b4980, win 0x1255d0;
    void onSelectTab(cocos2d::CCObject* sender) = win 0x127c30;
    void onPlayerIcon(cocos2d::CCObject* sender) = win 0x127f30;
    void onShipIcon(cocos2d::CCObject* sender) = win 0x1281e0;
    void onBallIcon(cocos2d::CCObject* sender) = win 0x1282a0;
    void onBirdIcon(cocos2d::CCObject* sender) = win 0x128360;
    void onDartIcon(cocos2d::CCObject* sender) = win 0x128420;
    void onRobotIcon(cocos2d::CCObject* sender) = win 0x1286d0;
    void onSpiderIcon(cocos2d::CCObject* sender) = win 0x128890;
    void onShards(cocos2d::CCObject* sender) = win 0x12ad70;
    void onBack(cocos2d::CCObject* sender) = win 0x12adf0;
    void onShop(cocos2d::CCObject* sender) = win 0x12ad90;
    void setupColorSelect() = mac 0x1b7500;
    PAD = mac 0x10, win 0x8;
    CCTextInputNode* m_nameInput;
    SimplePlayer* m_playerPreview;
    PAD = mac 0x10, win 0x8;
    cocos2d::CCSprite* m_colorSelector1;
    cocos2d::CCSprite* m_colorSelector2;
    CCMenuItemSpriteExtra* m_unkButton0x148;
    CCMenuItemSpriteExtra* m_unkButton0x14c;
    PAD = win 0x8;
    cocos2d::CCArray* m_pagesArray;
    PAD = win 0x8;
    CCMenuItemToggler* m_tabToggleCube;
    CCMenuItemToggler* m_tabToggleShip;
    CCMenuItemToggler* m_tabToggleBall;
    CCMenuItemToggler* m_tabToggleUfo;
    CCMenuItemToggler* m_tabToggleWave;
    CCMenuItemToggler* m_tabToggleRobot;
    CCMenuItemToggler* m_tabToggleSpider;
    CCMenuItemToggler* m_tabToggleSpecial;
    CCMenuItemToggler* m_tabToggleDeathEffect;
    PAD = win 0x4;
    bool m_updateSelector;
}

class GJGroundLayer : cocos2d::CCLayer {
    void updateGroundWidth() = mac 0x356790, win 0x12dda0;
}

class GJItemIcon : cocos2d::CCSprite {
    bool init(
        UnlockType, int, cocos2d::ccColor3B, cocos2d::ccColor3B,
        bool, bool, bool, cocos2d::ccColor3B
    ) = win 0x12ccf0;

    static GJItemIcon* createBrowserIcon(UnlockType _type, int _id) {
        return GJItemIcon::create(_type, _id,
            { 0xaf, 0xaf, 0xaf }, { 0xff, 0xff, 0xff },
            false, true, true,
            { 0xff, 0xff, 0xff }
        );
    }

    static GJItemIcon* create(UnlockType _type, int _id, cocos2d::ccColor3B _col1, cocos2d::ccColor3B _col2, bool _un0, bool _un1, bool _un2, cocos2d::ccColor3B _col3) = mac 0x1bb330, win 0x12cbf0, ios 0x227188;
}

class GJLevelScoreCell : TableViewCell {
    void updateBGColor(int index) = win 0x5c6b0;
}

class GJListLayer : cocos2d::CCLayerColor {
    ~GJListLayer() = mac 0x344350;
    static GJListLayer* create(BoomListView* target, const char* title, cocos2d::ccColor4B color, float width, float height) = mac 0x343e10, win 0x12e000;

    BoomListView* m_listView;
}

class GJMapPack : cocos2d::CCNode {
    cocos2d::CCArray* m_levels;
    int m_packID;
    GJDifficulty m_difficulty;
    int m_stars;
    int m_coins;
    gd::string m_packName;
    gd::string m_levelStrings;
    cocos2d::ccColor3B m_textColour;
    cocos2d::ccColor3B m_barColour;
    int m_MId;
    bool m_isGauntlet;

    virtual bool init() = win 0xc0080;
    static GJMapPack *create() = win 0xbffe0;
}

class GJMessageCell : TableViewCell {
    void updateBGColor(int index) = win 0x5c6b0;
    void loadFromMessage(GJUserMessage *) = win 0x64b60;
}

class GJOptionsLayer : FLAlertLayer {
    static GJOptionsLayer* create() = mac 0x145ae0, win 0x0, ios 0x0;
    void addToggle(char const*, char const*, char const*) = mac 0x1464e0, win 0x0, ios 0x0;
}

class GJRequestCell : TableViewCell {
    void updateBGColor(int index) = win 0x5c6b0;
}

class GJRewardDelegate {}

class GJRewardItem : cocos2d::CCObject {
    int m_chestID;
    int m_timeRemaining;
    GJRewardType m_rewardType;
    cocos2d::CCArray* m_rewardObjects;
    bool m_unk;
}

class GJRewardObject : cocos2d::CCObject {
    SpecialRewardItem m_specialRewardItem;
    UnlockType m_unlockType;
    int m_itemID;
    int m_total;
}

class GJRobotSprite : CCAnimatedSprite {
    // virtual ~GJRobotSprite() = mac 0x34aaf0; inlined on windows, RE and recreate 
    virtual bool init() = mac 0x34ad50, win 0x145840;
    virtual void setOpacity(unsigned char) = mac 0x34bcc0, win 0x146640; // shared with GJSpriteSprite
    virtual void hideSecondary() = mac 0x34c3b0, win 0x146c90;
    static GJRobotSprite* create() = mac 0x34ac00, win 0x1457a0;
    void updateColor02(cocos2d::_ccColor3B) = mac 0x34bbd0, win 0x1461c0;
    void updateFrame(int) = mac 0x34bdd0, win 0x146700;
    void hideGlow() = mac 0x34b860;

    cocos2d::CCArray* m_unk244;
    bool m_unk248;
    cocos2d::ccColor3B m_mainColor;
    cocos2d::ccColor3B m_secondaryColor;
    cocos2d::CCArray* m_unk250;
    cocos2d::CCSprite* m_glowSprite;
    cocos2d::CCSprite* m_unk258;
    PAD = win 0x4;
    cocos2d::CCSprite* m_flameSprite;
    CCSpritePart* m_headSpritePart;
}

class GJRotationControl : cocos2d::CCLayer {
    void setAngle(float angle) {
        m_sliderPosition = cocos2d::CCPointMake(sinf(angle) * 60.0f, cosf(angle) * 60.0f);
        m_angle = angle;
    
        m_sliderThumb->setPosition(m_sliderPosition);
    }

    void updateSliderPosition(cocos2d::CCPoint pos) = win 0x94020;

    float m_unknown0;
    float m_unknown1;
    cocos2d::CCPoint m_sliderPosition;
    cocos2d::CCSprite* m_sliderThumb;
    float m_objAngle;
    float m_angle;
    int m_touchID;
}

class GJRotationControlDelegate {
    virtual void angleChangeBegin() {}
    virtual void angleChangeEnded() {}
    virtual void angleChanged(float) {}
}

class GJScaleControl : cocos2d::CCLayer {
    virtual bool init() = mac 0x31b30, win 0x94490;
    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x31d30, win 0x947a0;
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x31e60, win 0x94840;
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x31fb0, win 0x94940;
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x32060, win 0x2dea0; // shared with many others
    void updateLabel(float value) = win 0x94990, mac 0x31c90;
    void loadValues(GameObject* obj, cocos2d::CCArray* objs) = win 0x94590, mac 0x24f40;

    Slider* m_slider;
    unsigned int m_touchID;
    float m_value;
    bool m_shouldSnapAt1;
    cocos2d::CCLabelBMFont* m_label;
    GJScaleControlDelegate* m_delegate;
}

class GJScaleControlDelegate {
    virtual void scaleChanged(float) {}
    virtual void scaleChangeBegin() {}
    virtual void scaleChangeEnded() {}
}

class GJScoreCell : TableViewCell, FLAlertLayerProtocol {
    virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x11D8E0, win 0x624a0;
    void loadFromScore(GJUserScore* score) = mac 0x113AA0, win 0x61440;
    void onViewProfile(cocos2d::CCObject* sender) = win 0x62380;
    void updateBGColor(int index) = mac 0x113A40, win 0x5c6b0;
    inline GJScoreCell(char const* identifier, float parentHeight, float height) : TableViewCell(identifier, parentHeight, height) {}

    GJUserScore* m_score;
}

class GJSearchObject : cocos2d::CCNode {
    SearchType getType() {
        return m_searchType;
    }

    static GJSearchObject* create(SearchType nID) = win 0xc2b90, mac 0x2df120;
    static GJSearchObject* create(SearchType nID, gd::string str) = win 0xc2c80, mac 0x2df310;
    static GJSearchObject* createFromKey(const char* key) = mac 0x2C0620, win 0xC2760;
    const char* getKey() = mac 0x2C6A40, win 0xC30A0;
    const char* getNextPageKey() = win 0xC31F0;

    SearchType m_searchType;
    gd::string m_searchQuery;
    gd::string m_difficulty;
    gd::string m_length;
    int m_page;
    bool m_starFilter;
    bool m_noStarFilter;
    int m_unknownMember;
    bool m_uncompletedFilter;
    bool m_completedFilter;
    bool m_featuredFilter;
    bool m_originalFilter;
    bool m_twoPlayerFilter;
    bool m_coinsFilter;
    bool m_epicFilter;
    GJDifficulty m_demonFilter;
    int m_unk;
    int m_songID;
    bool m_customSongFilter;
    bool m_songFilter;
}

class GJSpecialColorSelect {
    static const char* textForColorIdx(int id) = mac 0x383a50, win 0x14e1d0, ios 0x113f6c;
}

class GJSpecialColorSelectDelegate {
    virtual void colorSelectClosed(GJSpecialColorSelect*, int) {}
}

class GJSpiderSprite : GJRobotSprite {
    // ~GJSpiderSprite() = mac 0x34c4a0; same as ~CCRobotSprite, fully merged on windows
    virtual bool init() = mac 0x34c700, win 0x146db0;
    static GJSpiderSprite* create() = mac 0x34c5b0, win 0x146cf0;
}

class GJSpriteColor : cocos2d::CCNode {
    int m_colorID;
    int m_defaultColorID;
    float m_unk_0F4;
    cocos2d::ccHSVValue m_hsv;
    bool m_usesHSV;
    float unk_10C;
    bool unk_110;

    void resetCustomColorMode() = mac 0x342f10;
}

class GJUserCell : TableViewCell {
    void updateBGColor(int index) = win 0x5c6b0;
}

class GJUserMessage : cocos2d::CCNode {
    virtual bool init() = win 0x33b40;
}

class GJUserScore : cocos2d::CCNode {
    IconType getIconType() const { 
        return m_iconType; 
    }
    int getPlayerCube() const { 
        return m_playerCube; 
    }
    int getPlayerShip() const { 
        return m_playerShip;
    }
    int getPlayerBall() const {
        return m_playerBall;
    }
    int getPlayerUfo() const {
        return m_playerUfo;
    }
    int getPlayerWave() const { 
        return m_playerWave; 
    }
    int getPlayerRobot() const { 
        return m_playerRobot; 
    }
    int getPlayerSpider() const { 
        return m_playerSpider; 
    }
    int getPlayerStreak() const { 
        return m_playerStreak; 
    }
    bool getGlowEnabled() const { 
        return m_glowEnabled; 
    }
    int getPlayerExplosion() const { 
        return m_playerExplosion; 
    }
    int getPlayerColor1() const { 
        return m_color1;
    }
    int getPlayerColor2() const { 
        return m_color2; 
    }
    gd::string getPlayerName() const { 
        return m_userName; 
    }
    static GJUserScore* create() {
        auto pRet = new GJUserScore();

        if (pRet) {
            pRet->autorelease();
            return pRet;
        }

        CC_SAFE_DELETE(pRet);
        return nullptr;
    }
    inline GJUserScore() : CCNode() {}
    static GJUserScore* create(cocos2d::CCDictionary*) = mac 0x2BD020, win 0xc0750;

    gd::string m_userName;
    gd::string m_userUDID;
    int m_scoreType;
    int m_userID;
    int m_accountID;
    int m_stars;
    int m_diamonds;
    int m_demons;
    int m_playerRank;
    int m_creatorPoints;
    int m_secretCoins;
    int m_userCoins;
    int m_iconID;
    int m_color1;
    int m_color2;
    int m_special;
    IconType m_iconType;
    int m_messageState;
    int m_friendStatus;
    int m_commentHistoryStatus;
    gd::string m_youtubeURL;
    gd::string m_twitterURL;
    gd::string m_twitchURL;
    int m_playerCube;
    int m_playerShip;
    int m_playerBall;
    int m_playerUfo;
    int m_playerWave;
    int m_playerRobot;
    int m_playerSpider;
    int m_playerStreak;
    bool m_glowEnabled;
    int m_playerExplosion;
    int m_modBadge;
    int m_globalRank;
    int m_friendReqStatus;
    int m_newMsgCount;
    int m_friendReqCount;
    bool m_isBlocked;
    gd::string m_lastScoreAge;
}

class GManager : cocos2d::CCNode {
    virtual void setup() = mac 0x26EE20, win 0x28F60;
    virtual void encodeDataTo(DS_Dictionary* data) {}
    virtual void dataLoaded(DS_Dictionary* data) {}
    virtual void firstLoad() {}

    void save() {
        saveGMTo(m_fileName);
    }
    void saveData(DS_Dictionary*, gd::string) = mac 0x26f4b0;
    void saveGMTo(gd::string) = mac 0x26f3b0, win 0x29250;

    inline GManager() {}

    gd::string m_fileName;
    bool m_setup;
    bool m_saved;
    bool m_quickSave;
}

class GooglePlayDelegate {
    virtual void googlePlaySignedIn() {}
}

class GooglePlayManager : cocos2d::CCNode {
    virtual bool init() = win 0x11070;

    static GooglePlayManager* sharedState() = win 0x295a0;
}

class GameLevelManager : cocos2d::CCNode {
    cocos2d::CCArray* createAndGetScores(gd::string, GJScoreType) = win 0xa2780;
    GJGameLevel* createNewLevel() = mac 0x2b8180, win 0xa0db0;
    static GameLevelManager* sharedState() = mac 0x2a8340, win 0x9f860;
    void limitSavedLevels() = mac 0x2C0C30, win 0xA43B0;
    cocos2d::CCArray* getCompletedLevels(bool newFilter) = mac 0x2BEEE0, win 0xa2d20;
    void getGJUserInfo(int) = mac 0x2CEBB0, win 0xb00b0;
    void getLevelLeaderboard(GJGameLevel* level, LevelLeaderboardType leaderboardType) = mac 0x2CD6F0, win 0xAED70;
    void getOnlineLevels(GJSearchObject*) = mac 0x2C5920, win 0xa7bc0;
    void getPageInfo(char const*) = mac 0x2c0050;
    cocos2d::CCArray* getSavedLevels(bool favorite, int levelFolder) = mac 0x2BE910, win 0xa2960;
    cocos2d::CCArray* getStoredOnlineLevels(char const*) = mac 0x2bfe80, win 0xa3a90;
    void getTopArtists(int, int) = mac 0x2ce3d0;
    void getTopArtistsKey(int) = mac 0x2ce7a0;
    void makeTimeStamp(char const*) = mac 0x2bfd90, win 0xa3f00;
    GJGameLevel* getMainLevel(int id, bool unk) = win 0xa0940;
    bool hasLikedItemFullCheck(LikeItemType type, int itemID, int commentSourceID) = mac 0x2d83d0, win 0xB86B0;
    bool hasRatedLevelStars(int levelID) = mac 0x2ca3a0;
    callback void ProcessHttpRequest(gd::string, gd::string, gd::string, GJHttpType) = mac 0x2a8670, win 0x9f8e0;
    cocos2d::CCDictionary* responseToDict(gd::string response, bool comment) = win 0xbba50;
    void storeUserNames(gd::string) = win 0xa1840;
    void storeUserName(int userID, int accountID, gd::string str) = mac 0x2B9020, win 0xa1a70;
    gd::string userNameForUserID(int id) = mac 0x2B91D0, win 0xa1c20;
    void updateUserScore() = mac 0x2CB6A0, win 0xada60;
    void downloadLevel(int id, bool downloadData) = win 0xaa730;
    bool hasDownloadedLevel(int id) = win 0xab830;
    GJGameLevel* getSavedLevel(int id) = win 0xa2ee0;
    void saveLevel(GJGameLevel* level) = win 0xa31c0;
    void deleteLevel(GJGameLevel* level) = mac 0x2b88d0, win 0xa1640;
    void resetCommentTimersForLevelID(int id, bool commentHistory) = mac 0x2D43D0, win 0xB3F10;
    void resetStoredUserInfo(int id) {
        m_storedUserInfo->removeObjectForKey(id);
    }
    void resetAccountComments(int id){
        int i = 0;
        while(true){
            auto key = this->getAccountCommentKey(id, i++);
            auto levels = getStoredOnlineLevels(key);
            if(levels != nullptr){
                m_storedLevels->removeObjectForKey(key);
            }else{
                break;
            }
        }
    }
    const char* getAccountCommentKey(int a2, int a3){
        return cocos2d::CCString::createWithFormat("a_%i_%i", a2, a3)->getCString();
    }

    inline static GameLevelManager* get() {
        return GameLevelManager::sharedState();
    }

    //cocos2d::CCDictionary* timerDict = mac 0x1e8;
    cocos2d::CCDictionary* m_mainLevels;
    cocos2d::CCDictionary* m_searchFilters;
    cocos2d::CCDictionary* m_onlineLevels;
    cocos2d::CCDictionary* m_unkDict;
    cocos2d::CCDictionary* m_followedCreators;
    cocos2d::CCDictionary* m_downloadedLevels;
    cocos2d::CCDictionary* m_likedLevels;
    cocos2d::CCDictionary* m_ratedLevels;
    cocos2d::CCDictionary* m_ratedDemons;
    cocos2d::CCDictionary* m_reportedLevels;
    cocos2d::CCDictionary* m_onlineFolders;
    cocos2d::CCDictionary* m_localLevelsFolders;
    cocos2d::CCDictionary* m_dailyLevels;
    int m_dailyTimeLeft;
    int m_dailyID;
    int m_dailyIDUnk;
    PAD = mac 0x10, win 0x4;
    int m_weeklyTimeLeft;
    int m_weeklyID;
    int m_weeklyIDUnk;
    cocos2d::CCDictionary* m_gauntletLevels;
    gd::map<gd::string, bool> m_availableFilters;
    cocos2d::CCDictionary* m_timerDict;
    cocos2d::CCDictionary* m_knownUsers;
    cocos2d::CCDictionary* m_accountIDtoUserIDDict;
    cocos2d::CCDictionary* m_userIDtoAccountIDDict;
    cocos2d::CCDictionary* m_storedLevels;
    cocos2d::CCDictionary* m_pageInfo;
    cocos2d::CCDictionary* m_unkDict20;
    cocos2d::CCDictionary* m_savedPacks;
    cocos2d::CCDictionary* m_savedGauntlets;
    cocos2d::CCDictionary* m_downloadObjects;
    cocos2d::CCDictionary* m_unkDict24;
    cocos2d::CCDictionary* m_storedUserInfo;
    cocos2d::CCDictionary* m_friendRequests;
    cocos2d::CCDictionary* m_userMessages;
    cocos2d::CCDictionary* m_userReplies;
    gd::string m_unkStr1;
    gd::string m_unkStr2;
    LeaderboardState m_leaderboardState;
    bool m_unkEditLevelLayerOnBack;
    OnlineListDelegate* m_onlineListDelegate;
    LevelDownloadDelegate* m_levelDownloadDelegate;
    LevelCommentDelegate* m_levelCommentDelegate;
    CommentUploadDelegate* m_commentUploadDelegate;
    LevelUploadDelegate* m_levelUploadDelegate;
    LevelUpdateDelegate* m_levelUpdateDelegate;
    LeaderboardManagerDelegate* m_leaderboardManagerDelegate;
    LevelDeleteDelegate* m_levelDeleteDelegate;
    UserInfoDelegate* m_userInfoDelegate;
    LevelManagerDelegate* m_levelManagerDelegate;
    void* m_unkDelegate;
    FriendRequestDelegate* m_friendRequestDelegate;
    MessageListDelegate* m_messageListDelegate;
    DownloadMessageDelegate* m_downloadMessageDelegate;
    UploadMessageDelegate* m_uploadMessageDelegate;
    GJRewardDelegate* m_GJRewardDelegate;
    GJChallengeDelegate* m_GJChallengeDelegate;
    GJDailyLevelDelegate* m_GJDailyLevelDelegate;
    MusicDownloadDelegate* m_musicDownloadDelegate;
    int m_unkDownload;
    PAD = win 0x4;
    gd::string m_unkStr3;
    cocos2d::CCString* m_unkStr4;
}

class GameManager : GManager {
    int getPlayerFrame() {
        return m_playerFrame;
    }
    int getPlayerShip() {
        return m_playerShip;
    }
    int getPlayerBall() {
        return m_playerBall;
    }
    int getPlayerBird() {
        return m_playerBird;
    }
    int getPlayerDart() {
        return m_playerDart;
    }
    int getPlayerRobot() { 
        return m_playerRobot; 
    }
    int getPlayerSpider() { 
        return m_playerSpider; 
    }
    int getPlayerStreak() { 
        return m_playerStreak; 
    }
    int getPlayerDeathEffect() {
        return m_playerDeathEffect; 
    }
    int getPlayerColor() {
        return m_playerColor;
    }
    int getPlayerColor2() {
        return m_playerColor2;
    }
    bool getPlayerGlow() {
        return m_playerGlow; 
    }
    IconType getPlayerIconType() {
        return m_playerIconType; 
    }
    void setPlayerFrame(int id) {
        m_playerFrame = id;
    }
    void setPlayerShip(int id) {
        m_playerShip = id;
    }
    void setPlayerBall(int id) {
        m_playerBall = id;
    }
    void setPlayerBird(int id) {
        m_playerBird = id;
    }
    void setPlayerDart(int id) {
        m_playerDart = id;
    }
    void setPlayerRobot(int id) {
        m_playerRobot = id;
    }
    void setPlayerSpider(int id) {
        m_playerSpider = id;
    }
    void setPlayerStreak(int id) {
        m_playerStreak = id;
    }
    void setPlayerDeathEffect(int id) {
        m_playerDeathEffect = id;
    }
    void setPlayerColor(int id) {
        m_playerColor = id;
    }
    void setPlayerColor2(int id) {
        m_playerColor2 = id;
    }
    void setPlayerGlow(bool v) {
        m_playerGlow = v;
    }
    void setPlayerIconType(IconType v) {
        m_playerIconType = v;
    }
    void setQuality(cocos2d::TextureQuality quality) {
        m_quality = quality;
    }
    PlayLayer* getPlayLayer() { 
        return m_playLayer;
    }
    LevelEditorLayer* getEditorLayer() { 
        return m_levelEditorLayer;
    }
    bool getGameVariableDefault(const char* key, bool defaultValue) {
        auto object = static_cast<cocos2d::CCString*>(m_valueKeeper->objectForKey(std::string("gv_") + key));
        if (object == nullptr)
            return defaultValue;
        return object->boolValue();
    }
    int getIntGameVariableDefault(const char* key, int defaultValue) {
        auto object = static_cast<cocos2d::CCString*>(m_valueKeeper->objectForKey(std::string("gv_") + key));
        if (object == nullptr)
            return defaultValue;
        return object->intValue();
    }
    static GameManager* get() {
        return GameManager::sharedState();
    }

    void accountStatusChanged() = mac 0x1cdad0;
    cocos2d::_ccColor3B colorForIdx(int) = mac 0x1cbc80, win 0xc8d10, ios 0x237488;
    void didExitPlayscene() = mac 0x1d0230;
    void doQuickSave() = mac 0x1d0200;
    void fadeInMusic(const char*) = mac 0x1c2ff0, win 0xc4bd0;
    void getBGTexture(int) = mac 0x1cca00;
    void getFontFile(int) = mac 0x1cc5f0;
    void getFontTexture(int) = mac 0x1cc640;
    bool getGameVariable(const char*) = mac 0x1cccd0, win 0xc9d30;
    int getIntGameVariable(const char*) = mac 0x1cd1d0, win 0xca330;
    bool getUGV(const char*) = mac 0x1ccfa0, win 0xca0d0;
    void loadBackground(int) = mac 0x1cc820, win 0xc9990;
    void loadDeathEffect(int) = mac 0x1cc690, win 0xc9850;
    void loadFont(int) = mac 0x1cc550, win 0xc9770;
    void loadGround(int) = mac 0x1cc8e0, win 0xc9a50;
    void reloadAll(bool, bool, bool) = mac 0x1d08a0, win 0xce950;
    void reloadAllStep2() = mac 0x1d0940, win 0xce9e0, ios 0x23b1f4;
    void reloadAllStep3() = win 0xceb10;
    void reloadAllStep4() = win 0xceb80;
    void reloadAllStep5() = mac 0x1d0b00, win 0xcebf0;
    void reportPercentageForLevel(int, int, bool) = mac 0x1c5b00;
    void setGameVariable(const char*, bool) = mac 0x1cca80, win 0xc9b50;
    void setIntGameVariable(const char*, int) = mac 0x1cd0e0, win 0xca230;
    void setUGV(char const*, bool) = mac 0x1cce50, win 0xc9f90;
    static GameManager* sharedState() = mac 0x1c2b30, win 0xc4a50, ios 0x2321b8;
    ~GameManager() = mac 0x1d0e00, win 0xcf100;
    void getGTexture(int) = mac 0x1cca40, win 0xc9a50;
    virtual bool init() = mac 0x1c2ec0, win 0xc4ad0;
    void reportAchievementWithID(char const*, int, bool) = mac 0x1c6460, win 0xc64c0;
    cocos2d::CCSize resolutionForKey(int) = mac 0x1d0b40, win 0xceca0;
    virtual void update(float) = mac 0x1d0270, win 0xce440;
    bool isColorUnlocked(int _id, bool _type) = mac 0x1c3b90, win 0xc53f0;
    bool isIconUnlocked(int _id, IconType _type) = mac 0x1c35b0, win 0xc4fc0;
    void toggleGameVariable(const char* key) = win 0xc9e90;
    void returnToLastScene(GJGameLevel* level) = mac 0x1d0500, win 0xce6a0;

    bool m_switchModes;
    bool m_toFullscreen;
    bool m_reloading;
    bool m_unknown0;
    PAD = mac 0x4, win 0x4;
    cocos2d::CCDictionary* m_valueKeeper;
    cocos2d::CCDictionary* m_unlockValueKeeper;
    cocos2d::CCDictionary* m_customObjectDict;
    double m_adTimer;
    double m_adCache;
    PAD = mac 0x8, win 0x8;
    double m_unknownDouble;
    // TODO: according to wylie's decomp this pad was 0x4, gd.h has it as 0x8 (and its correct)
    // i have a feeling android gm and windows gm are different structs
    // so the extra member could be here (for windows)
    PAD = mac 0x4, win 0x8;
    bool m_loaded;
    gd::string m_unknownString;
    PlayLayer* m_playLayer;
    LevelEditorLayer* m_levelEditorLayer;
    int m_unknown1;
    MenuLayer* m_menuLayer;
    bool m_inMenuLayer;
    void* m_unknownVariable;
    bool m_unknownBool3;
    bool m_unknownPlayLayerBool;
    bool m_unknownBool4;
    bool m_unknownBool5;
    gd::string m_playerUDID;
    gd::string m_playerName;
    bool m_commentsEnabled;
    geode::SeedValueRSV m_playerUserID;
    float m_backgroundMusicVolume;
    float m_effectsVolume;
    float m_timeOffset;
    bool m_ratedGame;
    bool m_likedFacebook;
    bool m_followedTwitter;
    bool m_subbedYoutube;
    // there are 4 bytes too many between m_timeOffset and m_playerFrameRand1
    // and i'm guessing it's this one
    // int m_unknownInt;
    double m_socialsDuration;
    bool m_showedAd;
    bool m_unknownBool;
    bool m_editorEnabled;
    int m_sceneEnum;
    int m_searchObjectType;
    bool m_unknownBool6;
    geode::SeedValueRSV m_playerFrame;
    geode::SeedValueRSV m_playerShip;
    geode::SeedValueRSV m_playerBall;
    geode::SeedValueRSV m_playerBird;
    geode::SeedValueRSV m_playerDart;
    geode::SeedValueRSV m_playerRobot;
    geode::SeedValueRSV m_playerSpider;
    geode::SeedValueRSV m_playerColor;
    geode::SeedValueRSV m_playerColor2;
    geode::SeedValueRSV m_playerStreak;
    geode::SeedValueRSV m_playerDeathEffect;
    geode::SeedValueSR m_chk;
    geode::SeedValueSR m_secretNumber;
    bool m_playerGlow;
    IconType m_playerIconType;
    bool m_everyPlaySetup;
    bool m_showSongMarkers; 
    bool m_showBPMMarkers;
    bool m_recordGameplay;
    bool m_showProgressBar;
    bool m_performanceMode;
    bool m_clickedGarage; // this should be 0x2a2 on macos and 0x28a on windows
    bool m_clickedEditor;
    bool m_clickedName;
    bool m_clickedPractice;
    bool m_showedEditorGuide;
    bool m_showedRateDiffDialog;
    bool m_showedRateStarDialog;
    bool m_showedLowDetailDialog;
    PAD = mac 0x3c, win 0x30;
    int m_bootups;
    bool m_hasRatedGame;
    bool m_unk0;
    bool m_unk1;
    bool m_unk2;
    bool m_gameCenterEnabled;
    bool m_smoothFix;
    geode::SeedValueSRV m_ratePower;
    bool m_canGetLevelSaveData;
    int m_resolution;
    cocos2d::TextureQuality m_quality;
}

class GameObject : CCSpritePlus {
    cocos2d::CCPoint getStartPosition() { 
        return m_startPosition;
    }
    void setStartPosition(cocos2d::CCPoint const& p) {
        m_startPosition = p;
    }
    unsigned int getUniqueID() {
        return m_uniqueID;
    }
    short getGroupID(int ix) {
        return m_groups->at(ix);
    }
    short getGroupIDCount() {
        return m_groupCount; 
    }
    gd::vector<short> getGroupIDs() {
        std::vector<short> res;
    
        if (m_groups && m_groups->at(0))
            for (auto i = 0; i < m_groupCount; i++)
                res.push_back(m_groups->at(i));
    
        return res;
    }
    int getGameZOrder() {
        return m_gameZOrder;
    }
    void setGameZOrder(int z) {
        m_gameZOrder = z;
    }
    void setGameObjType(GameObjectType t) {
        m_objectType = t;
    }
    GJSpriteColor* getBaseColor() {
        return m_baseColor;
    }
    GJSpriteColor* getDetailColor() {
        return m_detailColor;
    }

    GameObject() = mac 0xdc4c0, win 0x983e0; // yeah why is this here wtf, seems life ego ctor created this
    ~GameObject() = mac 0x2f4ca0, win 0xcf340;
    virtual void update(float) = mac 0x2fbb90, win 0x30e00;
    virtual void setScaleX(float) = mac 0x335b00, win 0xe5050;
    virtual void setScaleY(float) = mac 0x335b90, win 0xe50e0;
    virtual void setScale(float) = mac 0x335c20, win 0xe5170;
    virtual void setPosition(const cocos2d::CCPoint&) = mac 0x335850, win 0xe4de0;
    virtual void setVisible(bool) = mac 0x336280, win 0xe57c0;
    virtual void setRotation(float) = mac 0x335970, win 0xe4ed0;
    virtual void setOpacity(GLubyte) = mac 0x335f10, win 0xe53c0;
    virtual bool initWithTexture(cocos2d::CCTexture2D*) = mac 0x2f56a0, win 0xcfa80;
    virtual void setChildColor(const cocos2d::ccColor3B&) = mac 0x341f20, win 0xee900;
    virtual void setFlipX(bool) = mac 0x335a60, win 0xe4fd0;
    virtual void setFlipY(bool) = mac 0x335ab0, win 0xe5010;
    virtual void customSetup() = mac 0x2fbba0, win 0xd1c10;
    virtual void setupCustomSprites() = mac 0x307f60, win 0xd7d50;
    virtual void addMainSpriteToParent(bool) = mac 0x33a5b0, win 0xeb250;
    virtual void resetObject() = mac 0x2fa620, win 0xd1470, ios 0xd1470;
    virtual void triggerObject(GJBaseGameLayer*) = mac 0x2fa8f0, win 0xd1790;
    virtual void activateObject() = mac 0x2faf60, win 0xd1870;
    virtual void deactivateObject(bool) = mac 0x2fb8f0, win 0xd19b0;
    virtual cocos2d::CCRect const& getObjectRect() = mac 0x3352b0, win 0xe4a40;
    virtual cocos2d::CCRect getObjectRect(float, float) = mac 0x3352d0, win 0xe4a70;
    virtual cocos2d::CCRect getObjectRect2(float, float) = mac 0x3354e0, win 0xe4b90;
    virtual cocos2d::CCRect const& getObjectTextureRect() = mac 0x3355b0, win 0xe4c40;
    virtual cocos2d::CCPoint getRealPosition() = mac 0x335750, win 0xe4d90;
    virtual void setStartPos(cocos2d::CCPoint) = mac 0x2fa520, win 0xd1390;
    virtual void updateStartValues() = mac 0x2fa800, win 0xd1610;
    virtual void customObjectSetup(gd::map<gd::string, gd::string>&) {}
    virtual gd::string getSaveString() = mac 0x33d3d0, win 0xed0c0;
    virtual bool isFlipX() = mac 0x335a40, win 0xe4fb0;
    virtual bool isFlipY() = mac 0x335a50, win 0xe4fc0;
    virtual void setRScaleX(float) = mac 0x335cb0, win 0xe5200;
    virtual void setRScaleY(float) = mac 0x335d60, win 0xe5240;
    virtual void setRScale(float) = mac 0x335e10, win 0xe5280;
    virtual void getRScaleX() = mac 0x335e50, win 0xe52c0;
    virtual void getRScaleY() = mac 0x335e80, win 0xe5300;
    virtual void calculateSpawnXPos() = mac 0x336970, win 0xe5d20;
    virtual void triggerActivated(float) = mac 0x336990, win 0xe5d30;
    virtual void powerOnObject() = mac 0x3369a0, win 0xe5d40;
    virtual void powerOffObject() = mac 0x3369c0, win 0xe5d60;
    virtual void setObjectColor(const cocos2d::ccColor3B&) = mac 0x341c90, win 0xee620;
    virtual void setGlowColor(cocos2d::_ccColor3B const&) = mac 0x341ed0, win 0xee8c0;
    virtual void getOrientedBox() = mac 0x342ad0, win 0xef160;
    virtual void addToGroup(int) = mac 0x33ad00, win 0xeb8d0;
    virtual void removeFromGroup(int) = mac 0x33ada0, win 0xeb930;
    virtual void spawnXPosition() = mac 0xdc1b0, win 0x98990;
    virtual void getObjectRectDirty() const = mac 0xdc1d0, win 0x989a0;
    virtual void setObjectRectDirty(bool) = mac 0xdc1e0, win 0x989b0;
    virtual void getOrientedRectDirty() const = mac 0xdc1f0, win 0x989c0;
    virtual void setOrientedRectDirty(bool) = mac 0xdc200, win 0x989d0;
    virtual GameObjectType getType() const = mac 0xdc210, win 0x989e0;
    virtual void setType(GameObjectType) = mac 0xdc220, win 0x989f0;
    virtual cocos2d::CCPoint getStartPos() const = mac 0xdc230, win 0x98a00;
    void activatedByPlayer(GameObject*) = mac 0x342a20, win 0xef0e0;
    void addColorSprite() = mac 0x2f7fe0, win 0xd0670;
    void addColorSpriteToParent(bool) = mac 0x2fb470, win 0xeb3f0;
    void addGlow() = mac 0x2f5c10, win 0xcfef0;
    void addToTempOffset(float, float) = mac 0x335700;
    void calculateOrientedBox() = mac 0x342b20, win 0xef1a0;
    void canChangeCustomColor() = mac 0x342db0;
    cocos2d::_ccColor3B& colorForMode(int, bool) = mac 0x343460, win 0xef8d0;
    float groupOpacityMod() = win 0xebda0;
    void commonSetup() = mac 0x2f5570, win 0xcfac0;
    void copyGroups(GameObject*) = mac 0x33ae30, win 0xeb9d0;
    static GameObject* createWithFrame(const char*) = mac 0x2f5490, win 0xcf8f0;
    static GameObject* createWithKey(int) = mac 0x2f4ce0, win 0xcf4f0;
    void destroyObject() = mac 0x336a00;
    void determineSlopeDirection() = mac 0x33a9e0, win 0xeb670;
    cocos2d::_ccColor3B& getActiveColorForMode(int, bool) = mac 0x343860, win 0xefb10;
    void getBallFrame(int) = mac 0x341bf0;
    cocos2d::CCPoint getBoxOffset() = mac 0x3353d0, win 0xef350;
    const cocos2d::_ccColor3B& getColorIndex() = mac 0x343b90;
    void getDidUpdateLastPosition() = mac 0x343a20;
    void getLastPosition() = mac 0x3439d0;
    void getMainColorMode() = mac 0x334c30;
    void getObjectZOrder() = mac 0x337d70;
    // inlined on windows
    float getObjectRadius() {
        float radius = m_objectRadius;
        if (m_scale != 1.0f)
            radius *= m_scale;
        return radius;
    }
    void getSecondaryColorMode() = mac 0x341c20;
    void getSectionIdx() = mac 0x343a00;
    void groupWasDisabled() = mac 0x33b110;
    void groupWasEnabled() = mac 0x33b0f0;
    void hasBeenActivated() = mac 0x342a80;
    void hasBeenActivatedByPlayer(GameObject*) = mac 0x342a50;
    void hasSecondaryColor() = mac 0x342f80;
    void ignoreEnter() = mac 0x3352a0;
    void ignoreFade() = mac 0x335290;
    void isBasicTrigger() = mac 0x343d10;
    void isColorTrigger() = mac 0x343b40;
    void isSpawnableTrigger() = mac 0x343a60, win 0xeff20;
    void isSpecialObject() = mac 0x343c40;
    void loadGroupsFromString(gd::string str) = mac 0x33b380, win 0xebcb0;
    static GameObject* objectFromString(gd::string, bool) = mac 0x33b720, win 0xebe50;
    void playShineEffect() = mac 0x2fa9d0, win 0xeab20;
    void quickUpdatePosition() = mac 0x335790;
    void removeGlow() = mac 0x2f7f70;
    void resetGroupDisabled() = mac 0x2fa7e0;
    void saveActiveColors() = mac 0x33d250, win 0xee3e0;
    void selectObject(cocos2d::ccColor3B) = mac 0x341f90, win 0xee960;
    void setDefaultMainColorMode(int) = mac 0x304fc0;
    void setDidUpdateLastPosition(bool const&) = mac 0x343a30;
    void setGlowOpacity(unsigned char) = mac 0x336200;
    void setLastPosition(cocos2d::CCPoint const&) = mac 0x3439e0;
    void setMainColorMode(int) = mac 0x342e70;
    void setSectionIdx(int const&) = mac 0x343a10;
    void setupCoinArt() = mac 0x337dd0, win 0xe7050;
    void slopeFloorTop() = mac 0x342800;
    void slopeWallLeft() = mac 0x3427e0;
    void updateCustomScale(float) = mac 0x335eb0, win 0xe5340;
    void updateMainColor() = mac 0x343340;
    void updateObjectEditorColor() = mac 0x3423c0, win 0xeee50;
    void updateOrientedBox() = mac 0x342b50, win 0xef1c0;
    void updateSecondaryColor() = mac 0x343740;
    void updateStartPos() = mac 0x2fa590, win 0xd13f0;
    void updateState() = mac 0x3369e0;
    void updateSyncedAnimation(float) = mac 0x337f00, win 0xe7320;
    void updateTextObject(gd::string, bool) = mac 0x2f58d0, win 0xcfc60;
    void deselectObject() = mac 0x3423a0, win 0xeee40;
    cocos2d::CCRepeatForever* createRotateAction(float f, int n) = win 0xe49b0;
    void setMyAction(cocos2d::CCAction* pAction) = win 0xd1b90;
    bool canAllowMultiActivate() = mac 0x343ca0, win 0xf06b0;
    void createGroupContainer(int size) = mac 0x33aca0, win 0xeb870;

    bool m_unk3;
    bool m_isBlueMaybe;
    float m_unk2;
    float m_unk;
    float m_unk3f;
    float m_unk4;
    bool m_unkidk;
    float m_animSpeed2;
    bool m_isEffectObject;
    bool m_randomisedAnimStart;
    float m_animSpeed;
    bool m_isBlackObject;
    bool m_isBlackObjectWithOutline;
    float m_blackChildOpacity;
    bool field_21C;
    bool m_editor;
    bool m_groupDisabled;
    bool m_colourOnTop;
    //GJSpriteColor* m_mainColourMode;
    //GJSpriteColor* m_secondaryColourMode;
    //bool m_col1;
    //bool m_col2;
    int m_baseColorID; //0x27c on macos
    int m_detailColorID;
    bool m_baseColorHSVModified;
    bool m_detailColorHSVModified;
    cocos2d::CCPoint m_startPosOffset;
    float m_rotateOffset;
    bool m_tintTrigger;
    bool m_isFlippedX;
    bool m_isFlippedY;
    cocos2d::CCPoint m_boxOffset;
    bool m_isOriented;
    cocos2d::CCPoint m_boxOffset2;
    OBB2D* m_objectOBB2D;
    bool m_oriented;
    cocos2d::CCSprite* m_glowSprite;
    bool m_notEditor;
    cocos2d::CCAction* m_myAction;
    bool m_unk1;
    bool m_runActionWithTag;
    bool m_objectPoweredOn;
    cocos2d::CCSize m_objectSize;
    bool m_modifier;
    // bool unknown2e5; m_active is 0x2e5 on macos
    bool m_active;
    bool m_animationFinished;
    cocos2d::CCParticleSystemQuad* m_particleSystem;
    gd::string m_effectPlistName;
    bool m_particleAdded;
    bool m_hasParticles;
    bool m_unkCustomRing;
    cocos2d::CCPoint m_portalPosition;
    bool m_unkParticleSystem;
    cocos2d::CCRect m_objectTextureRect;
    bool m_textureRectDirty;
    float m_rectXCenterMaybe;
    cocos2d::CCRect m_objectRect2;
    bool m_isObjectRectDirty;
    bool m_isOrientedRectDirty;
    bool m_hasBeenActivated;
    bool m_hasBeenActivatedP2;
    bool m_hasDetailColor;
    bool m_isPulseStick;
    bool m_hasOrientedBox;
    int m_linkedGroup;
    bool m_isSaw;
    int m_customRotateSpeed;
    bool m_sawIsDisabled;
    bool m_unknownVisibility345;
    bool m_unknown346;
    bool m_unknownVisibility347;
    cocos2d::CCSprite* m_baseSprite;
    cocos2d::CCSprite* m_detailSprite;
    bool m_unk2e8;
    float m_objectRadius;
    bool m_isRotatedSide;
    float m_unk2F4;
    float m_unk2F8;
    int m_uniqueID;
    GameObjectType m_objectType;
    int m_section;
    bool m_touchTriggered;
    bool m_spawnTriggered;
    cocos2d::CCPoint m_startPosition;
    gd::string m_textureName;
    bool m_useAudioScale;
    bool m_sleeping;
    float m_rotation;
    cocos2d::CCSize m_obStartScale;
    bool m_startFlipX;
    bool m_startFlipY;
    bool m_shouldHide;
    float m_spawnXPosition;
    bool m_invisible;
    float m_enterAngle;
    int m_activeEnterEffect;
    int m_parentMode;
    bool m_isGlowDisabled;
    int m_targetColorID;
    float m_scale;
    int m_objectID;
    bool m_unk364;
    bool m_unk365;
    bool m_unk366;
    bool m_ignoreFade;
    bool m_unk368;
    bool m_unk369;
    bool m_unk36A;
    bool m_isDontEnter;
    bool m_isDontFade;
    bool m_unk36D;
    int m_defaultZOrder;
    bool m_useSecondSheet;
    bool m_isPortal;
    bool m_customAudioScale;
    bool m_lockColourAsChild;
    float m_minAudioScale;
    float m_maxAudioScale;
    bool m_unkParticleSystem2;
    int m_secretCoinID;
    int m_unkUnusedSaveStringKey53;
    bool m_invisibleMode;
    bool m_glowUserBackgroundColour;
    bool m_useSpecialLight;
    bool m_orbOrPad;
    float m_glowOpacityMod;
    bool m_upSlope;
    int m_slopeType;
    float m_slopeAngle;
    bool m_hazardousSlope;
    float m_realOpacity;
    GJSpriteColor* m_baseColor;
    GJSpriteColor* m_detailColor;
    bool m_unk3b0;
    ZLayer m_defaultZLayer;
    ZLayer m_zLayer;
    int m_gameZOrder;
    gd::string m_textObjectString;
    bool m_showGamemodeBorders;
    bool m_unk3D9;
    bool m_isSelected;
    int m_globalClickCounter;
    PAD = mac 0x8, win 0x8;
    bool m_shouldUpdateColorSprite;
    float m_multiScaleMultiplier;
    bool m_isGroupParent;
    std::array<short, 10>* m_groups;
    short m_groupCount;
    std::array<short, 10>* m_pulseGroups;
    short m_pulseGroupCount; // mac 0x470
    std::array<short, 10>* m_alphaGroups;
    short m_alphaGroupCount; // mac 0x480
    int m_editorLayer;
    int m_editorLayer2;
    int m_unk414;
    PAD = mac 0xc, win 0xc;
    cocos2d::CCPoint m_firstPosition;
    bool m_unk42C;
    bool m_unk42D;
    PAD = mac 0x6, win 0x6;
    bool m_isAnimated;
    PAD = mac 0x7, win 0x7;
    bool m_hasEffectLine;
    bool m_specialUnk43d;
    PAD = mac 0x1, win 0x1;
    bool m_hasDurationLine;
    bool m_isTriggerable;
    bool m_triggeredInEditor;
    PAD = mac 0x6, win 0x6;
    bool m_highDetail;
    ColorActionSprite* m_colorActionSpriteBase;
    ColorActionSprite* m_colorActionSpriteDetail;
    GJEffectManager* m_effectManager;
    bool m_unk458;
    bool m_unk459;
    bool m_unk45A;
    bool m_wasForcedRotatedPositionUpdateIdk;
    PAD = mac 0x8, win 0x8;
    bool m_orbMultiActivate;
}

class GameObjectCopy : cocos2d::CCObject {
    virtual ~GameObjectCopy() = mac 0xa3290;
    static GameObjectCopy* create(GameObject*) = mac 0x975a0, win 0x16c020;
    void resetObject() = mac 0x976a0;
    GameObject* m_object;
}

class GameRateDelegate {}

class GameSoundManager : cocos2d::CCNode {
    void disableMetering() = mac 0x362d80, win 0x257D0;
    void enableMetering() = mac 0x362d00, win 0x256F0;
    float getMeteringValue() = mac 0x362db0, win 0x258f0;
    void playBackgroundMusic(gd::string, bool, bool) = mac 0x362070, win 0x252B0;
    void playEffect(gd::string, float, float, float) = mac 0x3623d0, win 0x25450;
    void stopBackgroundMusic() = mac 0x362130, win 0x253A0;
    void asynchronousSetup() = win 0x25520;
    ~GameSoundManager() = mac 0x362c00, win 0x25640;
    static GameSoundManager* sharedManager() = mac 0x3610f0, win 0x24800;
    inline static GameSoundManager* get() {
        return GameSoundManager::sharedManager();
    }

    cocos2d::CCDictionary* m_dictionary1;
    cocos2d::CCDictionary* m_dictionary2;
    PAD = win 0xc;
    bool m_preloaded;
    PAD = win 0x4;
    gd::string m_filePath;
}

class GameStatsManager : cocos2d::CCNode {
    void awardCurrencyForLevel(GJGameLevel*) = mac 0x43600;
    void awardDiamondsForLevel(GJGameLevel*) = mac 0x43c60;
    void awardSecretKey() = mac 0x4b1e0;
    int getAwardedCurrencyForLevel(GJGameLevel*) = mac 0x432E0, win 0xf83e0;
    int getBaseCurrencyForLevel(GJGameLevel*) = mac 0x43470, win 0xf8530;
    GJChallengeItem* getChallenge(int id) = mac 0x451f0, win 0xa2fb0;
    void getSecretCoinKey(char const*) = mac 0x429f0;
    int getStat(char const*) = mac 0x3d310, win 0xf3580;
    void hasPendingUserCoin(char const*) = mac 0x42730, win 0xf7c50;
    void hasSecretCoin(char const*) = mac 0x40730, win 0xf7dc0;
    void hasUserCoin(char const*) = mac 0x427e0, win 0xf7ae0;
    void incrementChallenge(GJChallengeType type, int count) = win 0xf9ae0;
    void incrementStat(char const*) = mac 0x3d6d0;
    void incrementStat(char const*, int) = mac 0x3d6e0, win 0xf3460;
    static GameStatsManager* sharedState() = mac 0x38f20, win 0xf1e50;
    void storePendingUserCoin(char const*) = mac 0x42940;
    void storeSecretCoin(char const*) = mac 0x42a10;
    void storeUserCoin(char const*) = mac 0x42890;
    bool isItemUnlocked(UnlockType type, int id) = win 0xfbb80;

    PAD = mac 0x50, win 0x28;
    cocos2d::CCDictionary* m_dailyChests;
    cocos2d::CCDictionary* m_worldAdvertChests;
    cocos2d::CCDictionary* m_activeChallenges;
    cocos2d::CCDictionary* m_upcomingChallenges;
    PAD = mac 0x18, win 0xc;
    cocos2d::CCDictionary* m_playerStats;
    PAD = mac 0x50, win 0x10;
    cocos2d::CCDictionary* m_completedLevels;
    cocos2d::CCDictionary* m_verifiedUserCoins;
    cocos2d::CCDictionary* m_pendingUserCoins;
    cocos2d::CCDictionary* m_purchasedItems;
    cocos2d::CCDictionary* m_onlineCurrencyScores;
    cocos2d::CCDictionary* m_mainCurrencyScores;
    cocos2d::CCDictionary* m_gauntletCurrencyScores;
    cocos2d::CCDictionary* m_timelyCurrencyScores;
    cocos2d::CCDictionary* m_onlineStars;
    cocos2d::CCDictionary* m_timelyStars;
    cocos2d::CCDictionary* m_gauntletDiamondScores;
    cocos2d::CCDictionary* m_timelyDiamondScores;
    cocos2d::CCDictionary* m_unusedCurrencyAwardDict;
    cocos2d::CCDictionary* m_challengeDiamonds;
    cocos2d::CCDictionary* m_completedMappacks;
    cocos2d::CCDictionary* m_weeklyChest;
    cocos2d::CCDictionary* m_treasureRoomChests;
    geode::SeedValueSRV m_bonusKey;
    cocos2d::CCDictionary* m_miscChests;
}

class GameToolbox {
    static gd::string stringFromHSV(cocos2d::ccHSVValue hsv, const char* separator) = mac 0x28cf90;
    static cocos2d::ccHSVValue hsvFromString(gd::string str, char const* separator) = mac 0x28cc30, win 0x26da0;
    static cocos2d::CCDictionary* stringSetupToDict(gd::string text, char const* delimeter) = mac 0x28d700, win 0x272a0;
    static CCMenuItemToggler* createToggleButton(gd::string text, cocos2d::SEL_MenuHandler onToggled, bool isToggled, cocos2d::CCMenu* toggleMenu, cocos2d::CCPoint position, cocos2d::CCNode* callbackTarget, cocos2d::CCNode* labelParent, cocos2d::CCArray* toggleArray) = mac 0x28bc90, win 0x25fe0;
    static CCMenuItemToggler* createToggleButton(gd::string text, cocos2d::SEL_MenuHandler onToggled, bool isToggled, cocos2d::CCMenu* toggleMenu, cocos2d::CCPoint position, cocos2d::CCNode* callbackTarget, cocos2d::CCNode* labelParent, float checkboxScale, float labelSize, float maxWidth, cocos2d::CCPoint labelOffset, const char* unknown, bool anchorHorizontally, int toggleTag, cocos2d::CCArray* toggleArray) = mac 0x28bdd0, win 0x25fe0;
    static cocos2d::ccColor3B transformColor(cocos2d::ccColor3B const& src, cocos2d::ccHSVValue hsv) = win 0x26a60, mac 0x28c950;
    static void alignItemsHorisontally(cocos2d::CCArray* array, float pad, cocos2d::CCPoint start, bool idk) = win 0x25b20;
    static gd::map<gd::string, gd::string> stringSetupToMap(gd::string, char const*) = mac 0x28d4c0;
    static cocos2d::ccColor3B multipliedColorValue(cocos2d::ccColor3B color1, cocos2d::ccColor3B color2, float factor) = win 0x26CE0;
}

class GaragePage : cocos2d::CCLayer, ListButtonBarDelegate {
    static GaragePage* create(IconType type, GJGarageLayer* pGarage, cocos2d::SEL_MenuHandler pSelectCallback) {
        auto pRet = new GaragePage();
    
        if (pRet && pRet->init(type, pGarage, pSelectCallback)) {
            pRet->autorelease();
            return pRet;
        }
    
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }

    void listButtonBarSwitchedPage(ListButtonBar* bar, int idk) = win 0x12bb40;
    inline GaragePage() {}
    bool init(IconType type, GJGarageLayer* pGarage, cocos2d::SEL_MenuHandler pSelectCallback) = mac 0x1bb710, win 0x12af70, ios 0x225d5c;

    GJGarageLayer* m_garage;
    cocos2d::SEL_MenuHandler m_handler;
    cocos2d::CCSprite* m_selectSprite;
    CCMenuItemSpriteExtra* m_unkNode0x12c;
    IconType m_type;
    int m_unknown;
    PAD = win 0x4;
}

class GauntletSelectLayer {
    static GauntletSelectLayer* create(int) = win 0x105120;
}

class GhostTrailEffect {}

class HSVWidgetPopup : FLAlertLayer {
    bool init(cocos2d::_ccHSVValue value, HSVWidgetPopupDelegate* delegate, gd::string title) = win 0x49f10, mac 0x236d30;
    void onClose(cocos2d::CCObject* sender) = win 0x4a280;

    ConfigureHSVWidget* m_configureWidget;
    HSVWidgetPopupDelegate* m_delegate;
}

class HSVWidgetPopupDelegate {
    virtual void hsvPopupClosed(HSVWidgetPopup* popup, cocos2d::ccHSVValue value) {}
}

class HardStreak : cocos2d::CCDrawNode {
    // ~HardStreak() = mac 0x5bf00; inlined on windows
    virtual bool init() = mac 0x5c090, win 0x14e430;
    void addPoint(cocos2d::CCPoint) = mac 0x5c950, win 0x14ebc0;
    void clearBehindXPos(float) = mac 0x5cb40, win 0x14ec00;
    static HardStreak* create() = mac 0x5bfd0, win 0x14e390;
    void firstSetup() = mac 0x5c160, win 0x14e490;
    double normalizeAngle(double) = mac 0x5cbe0;
    cocos2d::CCPoint quadCornerOffset(cocos2d::CCPoint, cocos2d::CCPoint, float) = mac 0x5c990;
    void reset() = mac 0x5c930;
    void resumeStroke() = mac 0x5c210;
    void stopStroke() = mac 0x5c8f0, win 0x14e460;
    callback void updateStroke(float) = mac 0x5c240, win 0x14e530;

    cocos2d::CCArray* m_pointsArr;
    cocos2d::CCPoint m_currentPoint;
    float m_waveSize;
    float m_pulseSize;
    bool m_isSolid;
}

class InfoAlertButton : CCMenuItemSpriteExtra {
    bool init(gd::string title, gd::string text, float scale) = win 0x14ef50;
    void activate() = win 0x14f050;
    inline InfoAlertButton() {}
    static InfoAlertButton* create(gd::string title, gd::string text, float scale) = win 0x14ed20, mac 0x2ecad0;
}

class InfoLayer : FLAlertLayer, LevelCommentDelegate, CommentUploadDelegate, FLAlertLayerProtocol {
    bool init(GJGameLevel* level, GJUserScore* score) = mac 0x456850, win 0x14f5a0;
    void setupCommentsBrowser(cocos2d::CCArray* comments) = mac 0x458590, win 0x152270;
    void onMore(cocos2d::CCObject* sender) = mac 0x459400, win 0x151500;
    void onLevelInfo(cocos2d::CCObject* sender) = mac 0x459400, win 0x151850;
    void loadPage(int page, bool) = mac 0x458FB0, win 0x151e70;
    static InfoLayer* create(GJGameLevel* level, GJUserScore* score) = mac 0x456600, win 0x14f4f0;

    GJGameLevel* m_level;
    GJUserScore* m_score;
    gd::string m_commentKey;
    LoadingCircle* m_loadingCircle;
    cocos2d::CCLabelBMFont* m_pageLabel;
    cocos2d::CCLabelBMFont* m_commentsGoldLabel;
    GJCommentListLayer* m_list;
    CCMenuItemSpriteExtra* m_rightArrow;
    CCMenuItemSpriteExtra* m_leftArrow;
    CCMenuItemSpriteExtra* m_likeBtn;
    CCMenuItemSpriteExtra* m_timeBtn;
    CCMenuItemSpriteExtra* m_reportBtn;
    CCMenuItemSpriteExtra* m_commentsBtn;
    CCMenuItemSpriteExtra* m_refreshCommentsBtn;
    int m_itemCount;
    int m_pageStartIdx;
    int m_pageEndIdx;
    int m_page;
    bool m_canUpdateUserScore;
    bool m_commentHistory;
}

class InheritanceNode : cocos2d::CCObject {}

class KeybindingsLayer : FLAlertLayer {
    void onClose(cocos2d::CCObject* sender) = win 0x49c60;
    void onPrevPage(cocos2d::CCObject* sender) = win 0x153cd0;
    void onNextPage(cocos2d::CCObject* sender) = win 0x153cc0;
    void goToPage(int page) = win 0x153ce0;

    int m_currentPage;
    int m_itemCount;
    int m_pageCount;
    cocos2d::CCDictionary* m_pages;
    cocos2d::CCDictionary* m_unused;
    cocos2d::CCNode* m_leftArrow;
    cocos2d::CCNode* m_rightArrow;
}

class LabelGameObject : GameObject {
    virtual bool init() = mac 0x2f5520, win 0x25b7c0;
    void setObjectColor(cocos2d::_ccColor3B const&) = mac 0xdbca0;
}

class LeaderboardManagerDelegate {
    virtual void updateUserScoreFinished() {}
    virtual void updateUserScoreFailed() {}
    virtual void loadLeaderboardFinished(cocos2d::CCArray*, const char*) {}
    virtual void loadLeaderboardFailed(const char*) {}
}

class LeaderboardsLayer : cocos2d::CCLayer {
    static LeaderboardsLayer* create(LeaderboardState state) = mac 0x29F590, win 0x158710;
    bool init(LeaderboardState state) = mac 0x29f6d0, win 0x1587b0;
}

class LevelBrowserLayer : cocos2d::CCLayer, LevelManagerDelegate, FLAlertLayerProtocol, SetIDPopupDelegate, SetTextPopupDelegate {
    static cocos2d::CCScene* scene(GJSearchObject* search) {
        auto scene = cocos2d::CCScene::create();
        scene->addChild(LevelBrowserLayer::create(search));

        AppDelegate::get()->m_runningScene = scene;
        return scene;
    }

    bool init(GJSearchObject* search) = mac 0x2513f0, win 0x15a040;
    void loadPage(GJSearchObject* search) = mac 0x253650, win 0x15b160;
    void setupLevelBrowser(cocos2d::CCArray* levels) = win 0x15bb40;
    virtual void setupPageInfo(gd::string, char const*) = mac 0x255050, win 0x15C1C0;
    void updateLevelsLabel() = mac 0x255450, win 0x15c350;
    void onRefresh(cocos2d::CCObject* sender) = mac 0x253090;
    void onInfo(cocos2d::CCObject* sender) = mac 0x253170, win 0x15cb00;
    void onNew(cocos2d::CCObject* sender) = win 0x15cbf0, mac 0x252ac0;
    static LevelBrowserLayer* create(GJSearchObject* search) = mac 0x251210, win 0x159fa0, ios 0x2d0a00;

    PAD = win 0x4, mac 0x8;
    TextArea* m_noInternet;
    GJListLayer* m_list;
    CCMenuItemSpriteExtra* m_rightArrow;
    CCMenuItemSpriteExtra* m_leftArrow;
    CCMenuItemSpriteExtra* m_lastBtn;
    CCMenuItemSpriteExtra* m_cancelSearchBtn;
    void* m_unk4;
    cocos2d::CCArray* m_array;
    GJSearchObject* m_searchObject;
    cocos2d::CCLabelBMFont* m_countText;
    cocos2d::CCLabelBMFont* m_pageText;
    CCMenuItemSpriteExtra* m_pageBtn;
    cocos2d::CCLabelBMFont* m_folderText;
    CCMenuItemSpriteExtra* m_folderBtn;
    int m_itemCount;
    int m_pageStartIdx;
    int m_pageEndIdx;
}

class LevelCell : TableViewCell {
    CCMenuItemSpriteExtra* m_button;
    GJGameLevel* m_level;
    bool m_cellDrawn;

    void onViewProfile(cocos2d::CCObject*) = mac 0x11a4a0, win 0x5c790;
    void loadCustomLevelCell() = mac 0x1183b0, win 0x5a020;
    void updateBGColor(int index) = mac 0x110460, win 0x5c6b0;
    void loadFromLevel(GJGameLevel* level) = mac 0x110410, win 0x59FD0;
    inline LevelCell(char const* identifier, float parentHeight, float height) : TableViewCell(identifier, parentHeight, height) {}
}

class LevelCommentDelegate {
    virtual void loadCommentsFinished(cocos2d::CCArray*, char const*)  {}
    virtual void loadCommentsFailed(char const*)  {}
    virtual void updateUserScoreFinished()  {}
    virtual void setupPageInfo(gd::string, char const*)  {}
}

class LevelDeleteDelegate {
    virtual void levelDeleteFinished(int) {}
    virtual void levelDeleteFailed(int) {}
}

class LevelDownloadDelegate {
    virtual void levelDownloadFinished(GJGameLevel*) {}
    virtual void levelDownloadFailed(int) {}
}

class LevelEditorLayer : GJBaseGameLayer, LevelSettingsDelegate {
    static LevelEditorLayer* get() {
        return GameManager::sharedState()->m_levelEditorLayer;
    }
    static cocos2d::CCScene* scene(GJGameLevel* level) {
        auto scene = cocos2d::CCScene::create();
        scene->addChild(LevelEditorLayer::create(level));
        scene->setObjType(cocos2d::CCObjectType::LevelEditorLayer);

        AppDelegate::get()->m_runningScene = scene;
        return scene;
    }

    inline LevelEditorLayer() {}

    ~LevelEditorLayer() = mac 0x90a00, win 0x15e8d0;
    virtual void update(float) = mac 0xa1b70, win 0x16a660;
    virtual void draw() = mac 0xa2a70, win 0x16b7c0;
    virtual void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*) = mac 0x9c200, win 0x1664a0;
    virtual void flipGravity(PlayerObject*, bool, bool) = mac 0xa04e0, win 0x1691d0;
    virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0x9c590, win 0x166f90;
    virtual void addToGroup(GameObject*, int, bool) = mac 0x9dab0, win 0x167310;
    virtual void removeFromGroup(GameObject*, int) = mac 0x9db60, win 0x1673a0;
    virtual void timeForXPos(float) = mac 0x9c7d0, win 0x167210;
    virtual void xPosForTime(float) = mac 0x9c800, win 0x167250;
    virtual void levelSettingsUpdated() = mac 0x93f30, win 0x1606c0;
    static LevelEditorLayer* create(GJGameLevel* level) = mac 0x90fb0, win 0x15ed60, ios 0x261628;
    void activateTriggerEffect(EffectGameObject*, float, float, float) = mac 0x9b520, win 0x165ec0;
    GameObject* addObjectFromString(gd::string) = mac 0x94640, win 0x160c80;
    void addSpecial(GameObject*) = mac 0x94f30, win 0x162650;
    void addToRedoList(UndoObject*) = mac 0x96f80;
    void addToUndoList(UndoObject*, bool) = mac 0x94e20, win 0x162410;
    void animateInDualGround(GameObject*, float, bool) = mac 0xa2780, win 0x16b4b0;
    void assignNewStickyGroups(cocos2d::CCArray*) = mac 0x99fa0, win 0x1649f0;
    bool checkCollisions(PlayerObject*, float) = mac 0x9e620, win 0x167F10;
    void createBackground() = mac 0x929f0, win 0x160510;
    void createGroundLayer() = mac 0x92840, win 0x160260;
    GameObject* createObject(int, cocos2d::CCPoint, bool) = mac 0x957c0, win 0x160d70;
    void createObjectsFromSetup(gd::string) = mac 0x92230, win 0x160720;
    cocos2d::CCArray* createObjectsFromString(gd::string, bool) = mac 0x94730, win 0x160980;
    void getLastObjectX() = mac 0x9c860, win 0x167290;
    gd::string getLevelString() = mac 0x97790, win 0x162480;
    void getNextColorChannel() = mac 0x9a610;
    void getNextFreeBlockID(cocos2d::CCArray*) = mac 0x9a4e0;
    int getNextFreeGroupID(cocos2d::CCArray*) = mac 0x9a1b0, win 0x164ae0;
    void getNextFreeItemID(cocos2d::CCArray*) = mac 0x9a390;
    cocos2d::CCRect getObjectRect(GameObject* obj, bool updateRect) = mac 0x96240, win 0x1616b0;
    void getRelativeOffset(GameObject*) = mac 0x96840;
    bool hasAction(bool) = mac 0x96ff0;
    void handleAction(bool, cocos2d::CCArray*) = mac 0x97020, win 0x162010;
    bool init(GJGameLevel*) = mac 0x91010, win 0x15EE00;
    GameObject* objectAtPosition(cocos2d::CCPoint position) = mac 0x960c0, win 0x161300;
    cocos2d::CCArray* objectsAtPosition(cocos2d::CCPoint position) = mac 0x96690, win 0x1614d0;
    void objectMoved(GameObject*) = mac 0x999f0, win 0x162d40;
    cocos2d::CCArray* objectsInRect(cocos2d::CCRect rect, bool ignoreLayer) = mac 0x95e60, win 0x161ad0;
    void onPlaytest() = mac 0xa06b0, win 0x1695A0;
    void onResumePlaytest() = mac 0xa15e0, win 0x169D90;
    void onPausePlaytest() = mac 0xa1570, win 0x169CC0;
    void onStopPlaytest() = mac 0xa1780, win 0x169F10;
    void playMusic() = mac 0xa13c0, win 0x169b00;
    void recreateGroups() = mac 0x9dbf0, win 0x167450;
    void redoLastAction() = mac 0x97750;
    void removeAllObjects() = mac 0x93d80, win 0x161d60;
    void removeAllObjectsOfType(int) = mac 0x96d40;
    void removeObject(GameObject*, bool) = mac 0x96890, win 0x161cb0;
    void removeSpecial(GameObject*) = mac 0x969c0, win 0x162ff0;
    void resetMovingObjects() = mac 0x9ddc0, win 0x167690;
    void resetObjectVector() = mac 0x9c4b0;
    void resetToggledGroups() = mac 0x9aa70;
    void resetToggledGroupsAndObjects() = mac 0x9c3c0, win 0x166b80;
    void resetEffectTriggerOptim(GameObject* obj, cocos2d::CCArray* objs) = mac 0x9e5b0, win 0x167e00;
    void resetUnusedColorChannels() = mac 0x9a870;
    void rotationForSlopeNearObject(GameObject*) = mac 0x95cd0, win 0x161130;
    void runColorEffect(EffectGameObject*, int, float, float, bool) = mac 0x9bd30;
    void setupLevelStart(LevelSettingsObject*) = mac 0xa0ca0, win 0x16a1d0;
    void sortStickyGroups() = mac 0x92b10;
    void stopTriggersInGroup(int, float) = mac 0x9c030;
    void toggleDualMode(GameObject*, bool, PlayerObject*, bool) = mac 0xa0200, win 0x16b170;
    void toggleGroupPreview(int, bool) = mac 0x9bea0, win 0x166950;
    void transferDefaultColors(GJEffectManager*, GJEffectManager*) = mac 0x9ab50, win 0x165570;
    void undoLastAction() = mac 0x97770;
    void updateBGAndGColors() = mac 0x9b9b0, win 0x166d10;
    void updateBlendValues() = mac 0x9bc60, win 0x166c10;
    void updateDualGround(PlayerObject*, int, bool) = mac 0xa1a60, win 0x16b390;
    void updateEditorMode() = mac 0x93b50, win 0x1652b0;
    void updateGameObjectsNew() = mac 0x9adc0, win 0x1659a0;
    void updateGround(float) = mac 0x93a60;
    void updateGroundWidth() {
        m_groundLayer->updateGroundWidth();
        if (m_previewMode) {
            this->updateLevelColors();
        }
    }
    void updateLevelFont(int) = mac 0x9df00, win 0x1677a0;
    void updateLevelColors() {
        // literally empty
    }
    void updateOptions() = mac 0x91ed0, win 0x15fcc0;
    void updateToggledGroups() = mac 0x9bb10;
    callback void updateVisibility(float delta) = mac 0x92c70, win 0x1632b0;

    void groupStickyObjects(cocos2d::CCArray* objects) = mac 0x99dd0, win 0x164860;
    void ungroupStickyObjects(cocos2d::CCArray* objects) = mac 0x99ee0, win 0x164950;

    void copyObjectState(GameObject* target) = win 0x16b600;
    // sic
    void pasteAtributeState(GameObject* target, cocos2d::CCArray* targets) = mac 0xa29e0, win 0x16b740;
    void pasteColorState(GameObject* target, cocos2d::CCArray* targets) = mac 0xa2950, win 0x16b6c0;

    void setStartPosObject(StartPosObject* obj) {
        CC_SAFE_RETAIN(obj);
        CC_SAFE_RELEASE(m_currentStartPos);
        m_currentStartPos = obj;
    }

     bool m_ignoreDamage;
    bool m_followPlayer;
    bool m_drawTriggerBoxes;
    bool m_debugDraw;
    bool m_gridEnabled;
    bool m_hideGridOnPlay;
    bool m_effectLines;
    bool m_showGround;
    bool m_durationLines;
    bool m_moreUndo;
    bool m_hideBackground;
    bool m_smoothFixInEditor; // not entirely sure. used in onPlaytest
    bool m_highDetail;
    cocos2d::CCArray* m_touchTriggeredGroups;
    cocos2d::CCArray* m_triggeredGroups;
    cocos2d::CCDictionary* m_stickyGroups;
    int m_stickyGroupID;
    cocos2d::CCArray* m_unkObjectArr;
    cocos2d::CCArray* m_pulseTriggers;
    cocos2d::CCArray* m_colourObjects;
    cocos2d::CCArray* m_alphaTriggers;
    cocos2d::CCArray* m_spawnTriggers;
    cocos2d::CCArray* m_moveTriggers;
    cocos2d::CCDictionary* m_unkDict5;
    cocos2d::CCArray* m_enabledGroups;
    GameObject* m_copyStateObject;
    cocos2d::CCDictionary* m_unkDict6;
    cocos2d::CCArray* m_unkArray12;
    bool field_14;
    bool field_31D;
    geode::SeedValueRSV m_coinCount;
    bool m_moveTrigger;
    bool m_colorTrigger;
    bool m_pulseTrigger;
    bool m_alphaTrigger;
    bool m_spawnTrigger;
    cocos2d::CCArray* m_toggleTriggersMaybe;
    bool m_unkArr2Obj;
    cocos2d::CCArray* m_delayedSpawnArray2;
    bool m_delaySpawnNode;
    cocos2d::CCDictionary* m_unkDict3;
    cocos2d::CCDictionary* m_unkDict4;
    bool m_editorInitialising;
    bool field_34D;
    float m_timeMod;
    int m_currentLayer;
    StartPosObject* m_currentStartPos;
    float m_prevObjLayerScale;
    OBB2D* m_OBB2D;
    cocos2d::CCSprite* m_crossSprite;
    cocos2d::CCPoint m_unkPoint2;
    float m_unkFloat1;
    bool m_isDualMode;
    bool m_unkRectBool;
    GameObject* m_currentPortal;
    GameObject* m_portal;
    EditorUI* m_editorUI; // 0x5d8 on macos!! 
    cocos2d::CCSprite* m_backgroundLayer;
    cocos2d::CCArray* m_undoObjects;
    cocos2d::CCArray* m_redoObjects;
    cocos2d::CCPoint m_unkPoint1;
    geode::SeedValueRSV m_objectCount;
    DrawGridLayer* m_drawGridLayer;
    GJGameLevel* m_level;
    PlaybackMode m_playbackMode;
    cocos2d::CCPoint m_groundTopMaybe;
    float m_time;
    cocos2d::CCDictionary* m_enabledGroupsDict;
    bool m_3d;
    bool m_previewMode;
    GJGroundLayer* m_groundLayer;
    std::string m_rawLevelString;
    void* m_triggerHitbox; // why are these std vector bruh
    std::vector<GameObject*> m_objectVector;
    std::vector<GameObject*> m_groupVector;
    std::vector<cocos2d::CCArray*> m_nestedObjects;
    cocos2d::CCDictionary* m_triggerGroupsDict;
    std::vector<cocos2d::CCArray*> m_triggerGroupsVector;
    bool m_toggleGroupsMaybe;
    std::vector<bool> m_unkVector3; // everything set to false in playback mode
    std::vector<bool> m_disabledGroupVector;
    std::vector<bool> m_blendObjectsVector;
    std::vector<bool> m_blendColorVector;
    std::vector<uint8_t> m_toggledGroupsVector;
    std::vector<float> m_previewGroupsVector;
    double m_unkDouble1;
    cocos2d::CCArray* m_delayedSpawnArray1;
    bool m_removingObjects;
}

class LevelInfoLayer : cocos2d::CCLayer, LevelDownloadDelegate, LevelUpdateDelegate, RateLevelDelegate, LikeItemDelegate, FLAlertLayerProtocol, LevelDeleteDelegate, NumberInputDelegate, SetIDPopupDelegate {
    static LevelInfoLayer* create(GJGameLevel* level) = mac 0x15f290, win 0x175d50;
    bool init(GJGameLevel* level) = win 0x175df0, mac 0x15f520;
    void onGarage(cocos2d::CCObject* sender) = win 0x177c10, mac 0x163ac0;
    void onViewProfile(cocos2d::CCObject* sender) = mac 0x1617d0, win 0x17ac90;
    void onLevelInfo(cocos2d::CCObject* sender) = mac 0x163880, win 0x17acf0;
    void setupProgressBars() = win 0x177fc0, mac 0x1627e0;
    void setupLevelInfo() = mac 0x161C80, win 0x178680;
    void downloadLevel() = win 0x177d90, mac 0x161b90;
    void onPlay(cocos2d::CCObject* sender) = mac 0x161840, win 0x179730;
    virtual void levelDownloadFinished(GJGameLevel*) = mac 0x164C00, win 0x1790C0;
    virtual void levelUpdateFinished(GJGameLevel*, UpdateResponse) = mac 0x164E60, win 0x1792B0;
    void showUpdateAlert(UpdateResponse) = mac 0x164ED0, win 0x179300;
    void updateLabelValues() = mac 0x164090, win 0x17b170;

    PAD = win 0x4, mac 0x8;
    cocos2d::CCMenu* m_playBtnMenu;
    GJGameLevel* m_level;
    cocos2d::CCArray* m_unknown;
    CCMenuItemSpriteExtra* m_likeBtn;
    CCMenuItemSpriteExtra* m_starRateBtn;
    CCMenuItemSpriteExtra* m_demonRateBtn;
    PAD = win 0x4, mac 0x8;
    CCMenuItemToggler* m_ldmToggler;
    cocos2d::CCLabelBMFont* m_ldmLabel;
    cocos2d::CCLabelBMFont* m_lengthLabel;
    cocos2d::CCLabelBMFont* m_downloadsLabel;
    cocos2d::CCLabelBMFont* m_likesLabel;
    cocos2d::CCLabelBMFont* m_orbsLabel;
    cocos2d::CCLabelBMFont* m_folderLabel;
    CCMenuItemSpriteExtra* m_cloneBtn;
    PAD = win 0x4, mac 0x8;
}

class LevelLeaderboard : FLAlertLayer, LeaderboardManagerDelegate {
    void onChangeType(cocos2d::CCObject* sender) = mac 0x20e310, win 0x17d090;
    void onUpdate(cocos2d::CCObject* sender) = mac 0x20e240, win 0x17d1b0;
    bool init(GJGameLevel* level, LevelLeaderboardType type) = mac 0x20d710, win 0x17c4f0;
    static LevelLeaderboard* create(GJGameLevel* level, LevelLeaderboardType leaderboardType) = mac 0x20D550, win 0x17c440;
    void onClose(cocos2d::CCObject*) = mac 0x20e210, win 0x49C60;

    GJGameLevel* m_level;
    LevelLeaderboardType m_type;
}

class LevelManagerDelegate {
    virtual void loadLevelsFinished(cocos2d::CCArray *,char const*) {}
    virtual void loadLevelsFailed(char const*) {}
    virtual void setupPageInfo(gd::string,char const*) {}
}

class LevelPage : cocos2d::CCLayer, DialogDelegate {
    PAD = mac 0x8, win 0x4;
    GJGameLevel* m_level;

    void onInfo(cocos2d::CCObject* sender) = mac 0x23AAE0, win 0x189070;
}

class LevelSearchLayer : cocos2d::CCLayer {
    static LevelSearchLayer* create() = win 0x17d9c0;
    virtual bool init() = mac 0x384770, win 0x17da60;
    GJSearchObject* getSearchObject(SearchType, gd::string) = mac 0x388a50, win 0x1805f0;
    void onMoreOptions(cocos2d::CCObject*) = win 0x17f500;
    void onSearch(cocos2d::CCObject*) = win 0x180fc0;

    PAD = mac 0x18, win 0xC;
    CCTextInputNode* m_searchInput;

}

class LevelSelectLayer : cocos2d::CCLayer {
    static LevelSelectLayer* create(int lvl) = win 0x185500;

    PAD = win 0x10;
    BoomScrollLayer* m_scrollLayer;
}

class LevelSettingsDelegate {
    virtual void levelSettingsUpdated() {}
}

class SecretLayer2 : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DialogDelegate {
    static SecretLayer2* create() = win 0x21FD70;

    bool init() = win 0x21FE10, mac 0x25fe70;
    bool onSubmit(cocos2d::CCObject*) = win 0x221ac0, mac 0x2611a0;
    void updateSearchLabel(const char* text) = win 0x222FC0, mac 0x260e10;
    void showCompletedLevel() = win 0x220C10;
}

class SecretLayer4 : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DialogDelegate {
    static SecretLayer4* create() = mac 0x1ed500;
    static cocos2d::CCScene* scene() = mac 0x1ed4c0;

    bool init() = mac 0x1ed640;
    void showDialog(int message) = mac 0x1effd0;
}

class SelectArtDelegate {
    virtual void selectArtClosed(SelectArtLayer*) {}
}

class CustomSongLayerDelegate {
    virtual void customSongLayerClosed() {}
}

class LevelSettingsLayer : FLAlertLayer, ColorSelectDelegate, SelectArtDelegate, FLAlertLayerProtocol, CustomSongLayerDelegate {
    static LevelSettingsLayer* create(LevelSettingsObject* levelSettings, LevelEditorLayer* editor) = win 0x170d90;
    bool init(LevelSettingsObject* levelSettings, LevelEditorLayer* editor) = mac 0xa7e00, win 0x170e50;
    void onClose(cocos2d::CCObject* sender) = mac 0xab8e0, win 0x173ec0;
	int m_songIndex;
	Speed m_speed;
	ColorChannelSprite* m_backgroundChannelSprite;
	ColorChannelSprite* m_ground1ChannelSprite;
	ColorChannelSprite* m_ground2ChannelSprite;
	ColorChannelSprite* m_lineChannelSprite;
	ColorChannelSprite* m_objectChannelSprite;
	ColorChannelSprite* m_3DChannelSprite;
	cocos2d::CCSprite* m_colourBtnSprite;
	cocos2d::CCSprite* m_backgroundSprite;
	cocos2d::CCSprite* m_groundSprite;
	LevelSettingsObject* m_settingsObject;
	cocos2d::CCLabelBMFont* m_mainSongLabel;
	cocos2d::CCArray* m_gameModeArray;
	cocos2d::CCArray* m_speedArray;
	LevelSettingsDelegate* m_levelSettingsDelegate;
	LevelEditorLayer* m_editorLayer;
	cocos2d::CCArray* m_mainSongArray;
	cocos2d::CCArray* m_btnArray;
	CCMenuItemSpriteExtra* m_mainSongBtn;
	CCMenuItemSpriteExtra* m_customSongBtn;
	CCMenuItemSpriteExtra* m_selectCustomSongBtn;
	CCMenuItemSpriteExtra* m_changeSongBtn;
	CustomSongWidget* m_customSongWidget;
}

class LevelSettingsObject : cocos2d::CCNode {
    virtual ~LevelSettingsObject() = mac 0xa5650, win 0x16e800;
    virtual bool init() = mac 0xa5690, win 0x16e940;
    static LevelSettingsObject* create() = mac 0x92760, win 0x16e8a0;
    static LevelSettingsObject* objectFromDict(cocos2d::CCDictionary*) = mac 0xa5810, win 0x16f4d0;
    static LevelSettingsObject* objectFromString(gd::string) = mac 0x945a0, win 0x16f440;
    void setupColorsFromLegacyMode(cocos2d::CCDictionary*) = mac 0xa6a30, win 0x170050;

    static LevelSettingsObject* get() {
        auto baseLayer = GJBaseGameLayer::get();
        if (baseLayer) return baseLayer->m_levelSettings;
        return nullptr;
    }

    gd::string getSaveString() = mac 0x979c0, win 0x16ebf0;

    GJEffectManager* m_effectManager;
    // TODO: make enums for these
    int m_startMode;
    Speed m_startSpeed;
    bool m_startMini;
    bool m_startDual;
    bool m_twoPlayerMode;
    float m_songOffset;
    bool m_fadeIn;
    bool m_fadeOut;
    int m_backgroundIndex;
    int m_groundIndex;
    int m_fontIndex;
    bool m_startsWithStartPos;
    bool m_isFlipped;
    GJGameLevel* m_level;
    gd::string m_guidelineString;
    int m_unknown;
    int m_colorPage;
    int m_groundLineIndex;
}

class LevelUploadDelegate {}

class LevelUpdateDelegate {
    virtual void levelUpdateFinished(GJGameLevel*, UpdateResponse) {}
    virtual void levelUpdateFailed(int) {}
}

class LikeItemDelegate {
    virtual void likedItem(LikeItemType, int, bool) {}
}

class LikeItemLayer : FLAlertLayer {
    LikeItemType m_itemType;
    int m_itemID;
    int m_commentSourceID;
    LikeItemDelegate* m_likeDelegate;

    static LikeItemLayer* create(LikeItemType type, int itemID, int commentSourceID) = mac 0x35f760, win 0x18B940;
    bool init(LikeItemType type, int itemID, int commentSourceID) = mac 0x35f900;
}

class ListButtonBar : cocos2d::CCNode {
    BoomScrollLayer* m_scrollLayer;
}

class ListButtonBarDelegate {}

class LoadingCircle : cocos2d::CCLayerColor {
    void setParentLayer(cocos2d::CCLayer* layer) {
        m_parentLayer = layer;
    }
    void setFade(bool fade) {
        m_fade = fade;
    }

    static LoadingCircle* create() = mac 0x277d50, win 0x2a0d0;
    void show() = mac 0x277fd0, win 0x2a290;
    void fadeAndRemove() = mac 0x2780d0, win 0x2a370;

    cocos2d::CCSprite* m_sprite;
    cocos2d::CCLayer* m_parentLayer;
    bool m_fade;
}

class LoadingLayer : cocos2d::CCLayer {
    void setFromRefresh(bool value) {
        m_fromRefresh = value;
    }

    LoadingLayer() {}
    static LoadingLayer* create(bool fromReload) = mac 0x1df1f0, win 0x18bfe0, ios 0x130278;
    bool init(bool fromReload) = mac 0x1df2f0, win 0x18c080;
    const char* getLoadingString() = win 0x18cf40;
    void loadAssets() = mac 0x1dfb20, win 0x18c8e0, ios 0x130278;
    void loadingFinished() = win 0x18c790;

    static cocos2d::CCScene* scene(bool fromReload) {
        auto scene = cocos2d::CCScene::create();
        scene->addChild(LoadingLayer::create(fromReload));

        return scene;
    }

    bool m_unknown;
    bool m_unknown2;
    int m_loadStep;
    cocos2d::CCLabelBMFont* m_caption;
    TextArea* m_textArea;
    cocos2d::CCSprite* m_sliderBar;
    float m_sliderGrooveXPos;
    float m_sliderGrooveHeight;
    bool m_fromRefresh;
}

class LocalLevelManager : GManager {
    static LocalLevelManager* sharedState() = mac 0x35dd60, win 0x18d260;
    inline static LocalLevelManager* get() {
        return LocalLevelManager::sharedState();
    }
    bool init() = mac 0x2384e0;
    virtual void encodeDataTo(DS_Dictionary* data) = mac 0x35ed60, win 0x18e040;
    virtual void dataLoaded(DS_Dictionary* data) = mac 0x35eda0, win 0x18e070;

    cocos2d::CCDictionary* getAllLevelsInDict() = mac 0x35e3d0, win 0x18d7c0;

    cocos2d::CCDictionary* m_loadData;
    cocos2d::CCDictionary* m_levelData;
    cocos2d::CCArray* m_localLevels;
}

class MapPackCell : TableViewCell {
    void updateBGColor(int index) = win 0x5c6b0;
    void loadFromMapPack(GJMapPack *) = win 0x5cac0;
}

class MenuGameLayer : cocos2d::CCLayer {
    void resetPlayer() = mac 0x28fdc0, win 0x18f4b0;
    void destroyPlayer() = win 0x190100;
    virtual void update(float) = mac 0x28fa70, win 0x18f190;
    virtual bool init() = win 0x18e770;
    void updateColors() = win 0x18edd0;
}

class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol, GooglePlayDelegate {
    inline MenuLayer() {}
    virtual ~MenuLayer() = mac 0x1d1230, win 0x190620;
    virtual bool init() = mac 0x1d14b0, win 0x1907b0;
    virtual void keyBackClicked() = mac 0x1d3160, win 0x191f90;
    virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x1d33d0, win 0x1922c0;
    virtual void googlePlaySignedIn() = mac 0x1d2f30, win 0x191b20;
    virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x1d3190, win 0x192150;
    void onMoreGames(cocos2d::CCObject*) = mac 0x1d2ad0, win 0x1919c0, ios 0x19fbb4;
    void onGarage(cocos2d::CCObject*) = win 0x191dc0;
    void onQuit(cocos2d::CCObject*) = mac 0x1d2b40, win 0x191fa0;
    void onMyProfile(cocos2d::CCObject*) = mac 0x1d2c20, win 0x1e0950;
    void onPlay(cocos2d::CCObject*) = win 0x191b50;
    void onCreator(cocos2d::CCObject*) = win 0x191cd0;
    void onRobTop(cocos2d::CCObject*) = win 0x191940;
    void onNewgrounds(cocos2d::CCObject*) = win 0x191e90;
    void onDaily(cocos2d::CCObject*) = win 0x1916e0;
    void onOptions(cocos2d::CCObject*) = win 0x191c30;
    void onAchievements(cocos2d::CCObject*) = win 0x191af0;
    void onStats(cocos2d::CCObject*) = win 0x191ca0;
    void onFacebook(cocos2d::CCObject*) = win 0x191960;
    void onTwitter(cocos2d::CCObject*) = win 0x191980;
    void onYouTube(cocos2d::CCObject*) = win 0x1919A0;
    static cocos2d::CCScene* scene(bool) = mac 0x1d12d0, win 0x190720, ios 0x19e57c;

    cocos2d::CCSprite* m_googlePlaySprite;
    cocos2d::CCSprite* m_viewProfileInfoText;
    cocos2d::CCLabelBMFont* m_playerNameLabel;
    CCMenuItemSpriteExtra* m_profileBtn;
}

class MessageListDelegate {}

class MoreSearchLayer : FLAlertLayer {
    static MoreSearchLayer* create() = mac 0x38ab40, win 0x182520;
    virtual bool init() = mac 0x3896b0, win 0x1825c0;
    void onClose(cocos2d::CCObject*) = mac 0x38aa40, win 0x1848f0;
}

class MoreOptionsLayer : FLAlertLayer, TextInputDelegate, GooglePlayDelegate {
    static MoreOptionsLayer* create() = win 0x1de850;
    virtual bool init() = mac 0x43f470, win 0x1DE8F0;
    void addToggle(const char* name, const char* key, const char* info) = mac 0x440430, win 0x1df6b0;
    void onKeybindings(cocos2d::CCObject* sender) = mac 0x4410e0, win 0x749d0;
    void onToggle(cocos2d::CCObject* sender) = mac 0x441370;
}

class MoreVideoOptionsLayer : FLAlertLayer {
    static MoreVideoOptionsLayer* create() = mac 0x443c10, win 0x1e24f0;
    virtual bool init() = mac 0x444150, win 0x1e2590;
    void onClose(cocos2d::CCObject*) = mac 0x444aa0;
}

class MultilineBitmapFont : cocos2d::CCSprite {}

class MusicDownloadDelegate {
    virtual void downloadSongFailed(int, GJSongError) {}
    virtual void downloadSongFinished(SongInfoObject*) {}
    virtual void loadSongInfoFailed(int, GJSongError) {}
    virtual void loadSongInfoFinished(SongInfoObject*) {}
    virtual void songStateChanged() {}
}

class MusicDownloadManager : cocos2d::CCNode, PlatformDownloadDelegate {
    void incrementPriorityForSong(int) = mac 0x2ef750;
    static MusicDownloadManager* sharedState() = mac 0x2ee4c0, win 0x1945b0;
    gd::string pathForSong(int id) = mac 0x2f1140, win 0x1960e0;
    cocos2d::CCArray* getDownloadedSongs() = win 0x195640;
    void songStateChanged() = win 0x194d90;
    void onGetSongInfoCompleted(gd::string, gd::string) = mac 0x2eec60;

    cocos2d::CCDictionary* m_unknownDict;
    cocos2d::CCArray* m_handlers;
    cocos2d::CCDictionary* m_songsDict;
    int m_priority;
}

class NumberInputDelegate {
    virtual void numberInputClosed(NumberInputLayer*) {}
}

class NumberInputLayer {}

class OBB2D : cocos2d::CCNode {
    void calculateWithCenter(cocos2d::CCPoint, float, float, float) = mac 0x35a9c0, win 0x2bc10;
    static OBB2D* create(cocos2d::CCPoint, float, float, float) = mac 0x35a890, win 0x2ba80;
    cocos2d::CCRect getBoundingRect() = mac 0x35b2b0, win 0x2c140;
    void overlaps(OBB2D*) = mac 0x35b0a0;
    void overlaps1Way(OBB2D*) = mac 0x35b0d0, win 0x2c020;

    cocos2d::CCPoint m_p1_1;
    cocos2d::CCPoint m_p1_2;
    cocos2d::CCPoint m_p1_3;
    cocos2d::CCPoint m_p1_4;
    cocos2d::CCPoint m_p2_1;
    cocos2d::CCPoint m_p2_2;
    cocos2d::CCPoint m_p2_3;
    cocos2d::CCPoint m_p2_4;
    cocos2d::CCPoint m_p3_1;
    cocos2d::CCPoint m_p3_2;
    cocos2d::CCPoint m_p3_3;
    cocos2d::CCPoint m_p3_4;
    double m_rot1;
    double m_rot2;
    cocos2d::CCPoint m_center;
}

class ObjectManager : cocos2d::CCNode {
    static ObjectManager* instance() = win 0x2c2c0;
    void setup() = win 0x2c3b0;
}

class KeysLayer : cocos2d::CCLayer {
    virtual bool init() = win 0x154560;
}

class ObjectToolbox : cocos2d::CCNode {
    cocos2d::CCArray* allKeys() {
        return m_frameToKeyDict->allKeys();
    }
    const char* frameToKey(const char* frame) {
        return static_cast<cocos2d::CCString*>(m_frameToKeyDict->objectForKey(frame))->getCString();
    }
    const char* intKeyToFrame(int key) {
        return static_cast<cocos2d::CCString*>(m_keyToFrameDict->objectForKey(key))->getCString();
    }
    const char* keyToFrame(const char* key) {
        return intKeyToFrame(atoi(key));
    }
    void addObject(int id, const char* frame) {
        m_frameToKeyDict->setObject(cocos2d::CCString::createWithFormat("%i", id), frame);
        m_keyToFrameDict->setObject(cocos2d::CCString::create(frame), id);
    }

    virtual bool init() = mac 0x3b2d80, win 0x198b20;
    static ObjectToolbox* sharedState() = mac 0x3b2bc0, win 0x198a80;
    float gridNodeSizeForKey(int key) = win 0x1dc920;
    const char* perspectiveBlockFrame(int key) = win 0x1dcd00;

    cocos2d::CCDictionary* m_frameToKeyDict;
    cocos2d::CCDictionary* m_keyToFrameDict;
}

class OpacityEffectAction : cocos2d::CCNode {
    static OpacityEffectAction* create(float, float, float, int) = mac 0x1789f0;
    static OpacityEffectAction* createFromString(gd::string) = mac 0x178c10;
    bool init(float, float, float, int) = mac 0x178b00;
    void step(float) = mac 0x178b90;

    float m_time;
    float m_beginOpacity;
    float m_endOpacity;
    bool m_finished;
    float m_elapsed;
    int m_group;
    float m_opacity;
    int m_uuid;
    float m_delta;
}

class OnlineListDelegate {
    virtual void loadListFinished(cocos2d::CCArray*, const char*) {}
    virtual void loadListFailed(const char*) {}
    virtual void setupPageInfo(gd::string, const char*) {}
}

class OptionsLayer : GJDropDownLayer, FLAlertLayerProtocol {
    static OptionsLayer* addToCurrentScene(bool noTransition) {
        auto layer = OptionsLayer::create();
        cocos2d::CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
        layer->showLayer(noTransition);
        return layer;
    }

    static OptionsLayer* create() = mac 0x43db60, win 0x1dd310, ios 0x86314;

    virtual void customSetup() = mac 0x43dc70, win 0x1dd420;
    virtual void layerHidden() = mac 0x43f200, win 0x1de6d0;

    cocos2d::CCMenu* m_optionsMenu;
}

class PauseLayer : CCBlockLayer {
    static PauseLayer* create(bool) = mac 0x20b1e0, win 0x1e4570;
    void onEdit(cocos2d::CCObject*) = mac 0x20c630, win 0x1e60e0;
    void goEdit() = mac 0x20cc10, win 0x1e62f0;

    void onQuit(cocos2d::CCObject*) = mac 0x20c790, win 0x1e63d0;
    void createToggleButton(gd::string caption, cocos2d::SEL_MenuHandler callback, bool on, cocos2d::CCMenu* menu, cocos2d::CCPoint pos) = mac 0x20c890, win 0x1e5570;
    virtual void customSetup() = mac 0x20b300, win 0x1e4620;

    void onRestart(cocos2d::CCObject* sender) = mac 0x20c860, win 0x1e6040;
    void onPracticeMode(cocos2d::CCObject* sender) = mac 0x20c6d0, win 0x1e5f30;
    void onNormalMode(cocos2d::CCObject* sender) = mac 0x20c720, win 0x1e5f60;
    void onResume(cocos2d::CCObject* sender) = mac 0x20c760, win 0x1e5fa0;
    virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x20cc80, win 0x1E6580;

    void musicSliderChanged(cocos2d::CCObject* sender) = win 0x1e5ce0, mac 0x20cb00;
    void sfxSliderChanged(cocos2d::CCObject* sender) = win 0x1ddfa0, mac 0x20cb40;
    
    bool m_unknown;
    bool m_unknown2;
}

class PlatformDownloadDelegate {
    virtual void downloadFinished(char const*) {}
    virtual void downloadFailed(char const*) {}
}

class PlatformToolbox {
    static void hideCursor() = mac 0x27c340;
    static void showCursor() = mac 0x27c360;
    static bool isControllerConnected() {
        #ifdef GEODE_IS_WINDOWS
            return cocos2d::CCApplication::sharedApplication()->getControllerConnected();
        #elif defined(GEODE_IS_MACOS)
            // this is just gd.h
            return reinterpret_cast<bool(*)()>(geode::base::get() + 0x27d1b0)();
        #else
            return false;
        #endif
    }
    static void toggleFullScreen(bool fullscreen) = mac 0x27cf90;
    static void resizeWindow() = mac 0x27cfe0;
}

class PlayLayer : GJBaseGameLayer, CCCircleWaveDelegate, CurrencyRewardDelegate, DialogDelegate {
    static PlayLayer* get() {
        return GameManager::sharedState()->getPlayLayer();
    }

    PlayLayer() = mac 0x80e20, win 0x1faa90;

    void addCircle(CCCircleWave*) = mac 0x7e0f0;
    void addObject(GameObject*) = mac 0x70e50, win 0x2017e0;
    void addToGroupOld(GameObject*) = mac 0x77680;
    void addToSpeedObjects(GameObject*) = mac 0x7cfc0, win 0x208730;
    void animateInDualGround(GameObject*, float, bool) = mac 0x7d710, win 0x208b10;
    void animateInGround(bool) = mac 0x7d9d0, win 0x209b20;
    void animateOutGround(bool) = mac 0x6f350, win 0x209e60;
    void animateOutGroundFinished() = mac 0x7de80;
    void applyEnterEffect(GameObject*) = mac 0x7c310, win 0x206880;
    virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0x7aa10, win 0x203940;
    void cameraMoveX(float, float, float) = mac 0x7cbe0;
    void cameraMoveY(float, float, float) = mac 0x7cc60, win 0x207c80;
    void checkCollisions(PlayerObject*, float) = mac 0x78c90, win 0x203CD0;
    void circleWaveWillBeRemoved(CCCircleWave*) = mac 0x7e110, win 0x20aa90;
    cocos2d::CCParticleSystemQuad* claimParticle(gd::string) = mac 0x76ba0, win 0x200dd0;
    void clearPickedUpItems() = mac 0x7cfa0;
    void colorObject(int, cocos2d::_ccColor3B) = mac 0x77810;
    void commitJumps() = mac 0x737e0;
    static PlayLayer* create(GJGameLevel*) = mac 0x6b590, win 0x1fb6d0;
    CheckpointObject* createCheckpoint() = mac 0x7e470, win 0x20b050;
    void createObjectsFromSetup(gd::string) = mac 0x6d130, win 0x201310;
    void createParticle(int, char const*, int, cocos2d::tCCPositionType) = mac 0x76800;
    void currencyWillExit(CurrencyRewardLayer*) = mac 0x7e070, win 0x20a8d0;
    void delayedResetLevel() = mac 0x7e050;
    void destroyPlayer(PlayerObject*, GameObject*) = mac 0x7ab80, win 0x20a1a0;
    void dialogClosed(DialogLayer*) = mac 0x7e0b0, win 0x20a8f0;
    virtual void draw() = mac 0x7d160, win 0x208870;
    void enterDualMode(GameObject*, bool) = mac 0x7d6a0;
    void exitAirMode() = mac 0x7dd40;
    void exitBirdMode(PlayerObject*) = mac 0x7dd80;
    void exitDartMode(PlayerObject*) = mac 0x7ddd0;
    void exitFlyMode(PlayerObject*) = mac 0x7dcf0;
    void exitRobotMode(PlayerObject*) = mac 0x7de20;
    void exitRollMode(PlayerObject*) = mac 0x7de60;
    void exitSpiderMode(PlayerObject*) = mac 0x7de40;
    void flipFinished() = mac 0x7e150, win 0x20ad10;
    virtual void flipGravity(PlayerObject*, bool, bool) = mac 0x7cd10, win 0x207d30;
    void flipObjects() = mac 0x76130, win 0x2000c0;
    void fullReset() = mac 0x7f8e0, win 0x20bdf0;
    void getLastCheckpoint() = mac 0x7f840, win 0x20b800;
    void getMaxPortalY() = mac 0x7b4e0;
    void getMinPortalY() = mac 0x7b550;
    void getObjectsState() = mac 0x7e9d0, win 0x20b8b0;
    void getOtherPlayer(PlayerObject*) = mac 0x7dcc0;
    void getParticleKey(int, char const*, int, cocos2d::tCCPositionType) = mac 0x764d0;
    void getParticleKey2(gd::string) = mac 0x767b0, win 0x200a30;
    void getRelativeMod(cocos2d::CCPoint, float, float, float) = mac 0x7c2a0;
    void getTempMilliTime() = mac 0x778e0;
    void gravityEffectFinished() = mac 0x7cec0;
    void hasItem(int) = mac 0x7cee0;
    void hasUniqueCoin(GameObject*) = mac 0x77510, win 0x202250;
    void incrementJumps() = mac 0x7ff40;
    bool init(GJGameLevel*) = mac 0x6b5f0, win 0x1fb780;
    void isFlipping() = mac 0x76100;
    void levelComplete() = mac 0x72b80, win 0x1fd3d0;
    void lightningFlash(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::_ccColor3B, float, float, int, bool, float) = mac 0x75cc0, win 0x1ffe70;
    void lightningFlash(cocos2d::CCPoint, cocos2d::_ccColor3B) = mac 0x75bf0, win 0x1ffd80;
    void loadDefaultColors() = mac 0x6ef30, win 0x206e10;
    void loadFromCheckpoint(CheckpointObject*) = mac 0x7f000, win 0x20b4a0;
    void loadLastCheckpoint() = mac 0x7efc0;
    void loadSavedObjectsState(gd::string) = mac 0x7f3d0;
    void markCheckpoint() = mac 0x7ef60, win 0x20b450;
    void moveCameraToPos(cocos2d::CCPoint) = mac 0x7c980, win 0x207bf0;
    virtual void onEnterTransitionDidFinish() = mac 0x806e0, win 0x20dbd0;
    virtual void onExit() = mac 0x80710, win 0x20D810;
    void onQuit() = mac 0x72710, win 0x20d810;
    void optimizeColorGroups() = mac 0x6dad0, win 0x202370;
    void optimizeOpacityGroups() = mac 0x6dc20, win 0x202530;
    void optimizeSaveRequiredGroups() = mac 0x6dd70, win 0x2026f0;
    void pauseGame(bool) = mac 0x802d0, win 0x20d3c0;
    void pickupItem(GameObject*) = mac 0x7c1d0;
    void playAnimationCommand(int, int) = mac 0x75930, win 0x1ff2f0;
    void playEndAnimationToPos(cocos2d::CCPoint) = mac 0x759a0;
    void playExitDualEffect(PlayerObject*) = mac 0x7d1d0, win 0x208e20;
    void playFlashEffect(float, int, float) = mac 0x75e50;
    void playGravityEffect(bool) = mac 0x7b5a0, win 0x207e00;
    void playSpeedParticle(float) = mac 0x77030, win 0x201130;
    void playerWillSwitchMode(PlayerObject*, GameObject*) = mac 0x7b820, win 0x16b290;
    void prepareSpawnObjects() = mac 0x7fc00, win 0x20c850;
    void processItems() = mac 0x735c0;
    void processLoadedMoveActions() = mac 0x7a7c0;
    void recordAction(bool, PlayerObject*) = mac 0x7e190;
    void registerActiveObject(GameObject*) = mac 0x77620;
    void registerStateObject(GameObject*) = mac 0x777b0;
    void removeAllObjects() = mac 0x727b0, win 0x20da00;
    void removeFromGroupOld(GameObject*) = mac 0x77750;
    void removeLastCheckpoint() = mac 0x7f870, win 0x20b830;
    void removePlayer2() = mac 0x7d630;
    void resetLevel() = mac 0x71c50, win 0x20bf00;
    void resume() = mac 0x80480, win 0x20d5c0;
    void resumeAndRestart() = mac 0x80400, win 0x20d4c0;
    void saveRecordAction(bool, PlayerObject*) = mac 0x78750, win 0x20ad40;

    static cocos2d::CCScene* scene(GJGameLevel* level) {
        auto scene = cocos2d::CCScene::create();
        scene->addChild(PlayLayer::create(level));
        scene->setObjType(cocos2d::CCObjectType::PlayLayer);

        AppDelegate::get()->m_runningScene = scene;
        return scene;
    }

    void setupLevelStart(LevelSettingsObject*) = mac 0x6f560, win 0x1fb780;
    void setupReplay(gd::string) = mac 0x7e1e0;
    void shakeCamera(float, float, float) = mac 0x744a0, win 0x1ff210;
    void shouldBlend(int) = mac 0x771b0;
    void showCompleteEffect() = mac 0x738e0, win 0x1fe060;
    void showCompleteText() = mac 0x73be0, win 0x1fda90;
    void showEndLayer() = mac 0x74450, win 0x1ffd00;
    void showHint() = mac 0x7deb0;
    void showNewBest(bool, int, int, bool, bool, bool) = mac 0x74580, win 0x1fe3a0;
    void showRetryLayer() = mac 0x75ba0;
    void showTwoPlayerGuide() = mac 0x6df00, win 0x2003b0;
    void sortGroups() = mac 0x6d9e0, win 0x202890;
    void spawnCircle() = mac 0x73820;
    void spawnFirework() = mac 0x74200;
    void spawnParticle(char const*, int, cocos2d::tCCPositionType, cocos2d::CCPoint) = mac 0x76330;
    void spawnPlayer2() = mac 0x7d170, win 0x2089e0;
    void startGame() = mac 0x726b0, win 0x1fd390;
    void startMusic() = mac 0x72910, win 0x20C8F0;
    void startRecording() = mac 0x7fec0;
    void startRecordingDelayed() = mac 0x7fed0;
    void stopCameraShake() = mac 0x75900, win 0x1ff2d0;
    void stopRecording() = mac 0x6d090;
    void storeCheckpoint(CheckpointObject*) = mac 0x7ef10, win 0x20b420;
    void switchToFlyMode(PlayerObject*, GameObject*, bool, int) = mac 0x7baf0;
    void switchToRobotMode(PlayerObject*, GameObject*, bool) = mac 0x7bc80;
    void switchToRollMode(PlayerObject*, GameObject*, bool) = mac 0x7bbe0;
    void switchToSpiderMode(PlayerObject*, GameObject*, bool) = mac 0x7bd20;
    void timeForXPos(float) = mac 0x7d120, win 0x2087d0;
    void timeForXPos2(float, bool) = mac 0x293eb0, win 0x1fd3d0;
    void toggleBGEffectVisibility(bool) = mac 0x7fe80;
    void toggleDualMode(GameObject*, bool, PlayerObject*, bool) = mac 0x7bf90, win 0x208880;
    void toggleFlipped(bool, bool) = mac 0x7bdc0, win 0x20ab20;
    void toggleGhostEffect(int) = mac 0x7fe40, win 0x1f8930;
    void toggleGlitter(bool) = mac 0x70e00, win 0x20a0d0;
    void togglePracticeMode(bool) = mac 0x7f9e0, win 0x20d0d0;
    void toggleProgressbar() = mac 0x6eeb0, win 0x208160;
    void tryStartRecord() = mac 0x7fe00;
    void unclaimParticle(char const*, cocos2d::CCParticleSystemQuad*) = mac 0x76e00, win 0x200fb0;
    void unregisterActiveObject(GameObject*) = mac 0x77660;
    void unregisterStateObject(GameObject*) = mac 0x777f0;
    virtual void update(float) = mac 0x77900, win 0x2029C0, ios 0xb2f08;
    void updateAttempts() = mac 0x7fcd0, win 0x20ced0;
    void updateCamera(float) = mac 0x6e2b0, win 0x2071d0;
    virtual void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*) = mac 0x7c7f0, win 0x207070;
    void updateDualGround(PlayerObject*, int, bool) = mac 0x7caa0, win 0x209680;
    void updateEffectPositions() = mac 0x7a6d0, win 0x20aab0;
    void updateLevelColors() = mac 0x6f1e0, win 0x203a90;
    void updateMoveObjectsLastPosition() = mac 0x7a720;
    void updateProgressbar() = mac 0x6ed70, win 0x208020;
    void updateReplay(float) = mac 0x78b60, win 0x20af40;
    void updateTimeMod(float, bool) = mac 0x786f0;
    virtual void updateTweenAction(float, char const*) = mac 0x7ffb0, win 0x20d1f0;
    void updateVisibility() = mac 0x6fb90, win 0x205460;
    void vfDChk() = mac 0x7fcb0, win 0x20d3a0;
    virtual void visit() = mac 0x75ef0, win 0x200020;
    void visitWithColorFlash() = mac 0x761f0, win 0x200190;
    void willSwitchToMode(int, PlayerObject*) = mac 0x7b9e0;
    void xPosForTime(float) = mac 0x7d140, win 0x208840;
    ~PlayLayer() = mac 0x6b090, win 0x1fafc0;

    float unused4c8;
    bool unused4cc;
    bool m_hasCheated;
    geode::SeedValueRS m_dontSave;
    int unknown4d8;
    bool m_debugPauseOff;
    bool m_shouldSmoothCamera;
    float unused_4e0;
    PAD = mac 0x4, win 0x4;
    cocos2d::CCDrawNode* unknown4e8;
    float m_camera4f0;
    int unused4f4;
    float m_somegroup4f8;
    float m_groundRestriction;
    float m_ceilRestriction;
    bool m_fullReset;
    bool unknown505;
    float unknown508;
    float unknown50c;
    float unknown510;
    float unknown514;
    float unknown518;
    PAD = mac 0x4, win 0x4;
    StartPosObject* m_startPos;
    CheckpointObject* m_startPosCheckpoint;
    EndPortalObject* m_endPortal;
    cocos2d::CCArray* m_checkpoints;
    cocos2d::CCArray* m_speedObjects;
    cocos2d::CCArray* unk340;
    cocos2d::CCArray* unk344;
    cocos2d::CCSprite* unk348;
    float m_backgroundRepeat;
    bool unk350;
    cocos2d::CCArray* unk354;
    cocos2d::CCArray* unk358;
    cocos2d::CCArray* m_objectsToUpdate;
    cocos2d::CCArray* unk360;
    bool m_isMute;
    bool unk365;
    bool unk366;
    bool unk367;
    bool unk368;
    bool unk369;
    bool unk36A;
    bool unk36B;
    cocos2d::CCArray* m_screenRingObjects;
    cocos2d::CCParticleSystemQuad* m_particleSystem;
    cocos2d::CCDictionary* m_pickedUpItems;
    cocos2d::CCArray* m_circleWaves;
    cocos2d::CCArray* unk37C;
    AudioEffectsLayer* m_audioEffectsLayer;
    float unknown5c0;
    float unknown5c4;
    GJGroundLayer* m_bottomGround;
    GJGroundLayer* m_topGround;
    PAD = mac 0x8, win 0x8;
    bool m_isDead;
    bool m_startCameraAtCorner;
    bool m_cameraYLocked;
    bool m_cameraXLocked;
    bool unknown5e4;
    int m_ballFrameSeed;
    float unknown5ec;
    float m_lockGroundToCamera;
    float unknown5f4;
    float m_levelLength;
    float m_realLevelLength;
    cocos2d::CCLabelBMFont* m_attemptLabel;
    cocos2d::CCLabelBMFont* m_percentLabel;
    bool m_isCameraShaking;
    float m_currentShakeStrength;
    float m_currentShakeInterval;
    double m_lastShakeTime;
    cocos2d::CCPoint m_cameraShakeOffset;
    bool unk3E0;
    float m_backgroundScrollOffset;
    float m_cameraFlip;
    bool unk3EC;
    int unk3F0;
    cocos2d::CCDictionary* m_particleSystemsPlist;
    cocos2d::CCDictionary* unk3F8;
    cocos2d::CCArray* m_particleSystems;
    cocos2d::CCNode* unk400;
    cocos2d::CCSprite* m_sliderGrooveSprite;
    cocos2d::CCSprite* m_sliderBarSprite;
    cocos2d::CCSize m_sliderSize;
    int unknown680;
    int m_activeGravityEffects;
    int m_gravityEffectStatus; // ??
    PAD = mac 0x4, win 0x4;
    cocos2d::CCArray* m_gravitySprites;
    bool unk428;
    bool m_shouldRecordActions;
    bool unk42A;
    bool m_isPaused;
    bool unk42C;
    bool m_isPlayer2Frozen;
    gd::string m_previousRecords;
    cocos2d::CCArray* m_replayInputs;
    double m_time;
    int unknown6b8;
    int unknown6bc;
    bool unk460;
    bool m_isAudioMeteringSupported;
    cocos2d::CCDictionary* unk464;
    gd::map<short, bool> m_hasColors;
    bool m_collisionDisabled;
    bool unknown701;
    GameObject* m_latestVehicle;
    GameObject* m_dualObject;
    bool m_isFlipped;
    float m_mirrorTransition;
    UILayer* m_UILayer;
    GJGameLevel* m_level;
    cocos2d::CCPoint m_cameraPosition;
    bool m_isTestMode;
    bool m_isPracticeMode;
    bool unk496;
    bool unk497;
    cocos2d::CCArray* unk498;
    bool m_unk49C;
    cocos2d::CCPoint m_playerStartPosition;
    int m_currentAttempt;
    int m_jumpCount;
    bool unk4B0;
    float m_totalTime;
    int m_attemptJumpCount;
    bool unk4BC;
    bool m_hasLevelCompleteMenu;
    bool m_hasCompletedLevel;
    bool m_delayedResetLevel;
    int m_lastDeathPercent;
    bool unk4C4;
    PAD = mac 0xb, win 0xb;
    bool unk4D0;
    bool unk4D1;
    cocos2d::CCArray* unk4D4;
    cocos2d::CCDictionary* unk4D8;
    double m_inlineCalculatedKickTime;
    double m_accumulatedKickDeltaTime;
    double m_unusedKickDouble;
    bool m_shouldTryToKick;
    float m_kickCheckDeltaSnapshot;
    int m_accumulatedKickCounter;
    PAD = mac 0x4, win 0x4;
    double unk508;
    PAD = mac 0x8, win 0x8;
    double unk518;
    double unk520;
    PAD = mac 0x4, win 0x4;
    bool m_hasGlitter;
    bool m_isBgEffectOff;
    bool unk52F;
    GameObject* m_antiCheatObject;
    bool m_antiCheatPassed;
    bool unk535;
    bool m_disableGravityEffect;
}

class PlayerCheckpoint : cocos2d::CCNode {
    // virtual ~PlayerCheckpoint() = mac 0x80760; inlined on windows
    static PlayerCheckpoint* create() = mac 0x7e8c0, win 0x20dc40;

    cocos2d::CCPoint m_position;
    float m_yVelocity;
    bool m_isUpsideDown;
    bool m_isShip;
    bool m_isBall;
    bool m_isUFO;
    bool m_isWave;
    bool m_isRobot;
    bool m_isSpider;
    bool m_isOnGround;
    int m_hasGhostTrail;
    bool m_small;
    float m_speed;
    bool m_hidden;
}

class PlayerObject : GameObject, AnimatedSpriteDelegate {
    virtual ~PlayerObject() = mac 0x217220, win 0x1e6be0;
    virtual void update(float) = mac 0x218bf0, win 0x1e8200;
    virtual void setScaleX(float) = mac 0x22e7f0, win 0x1fa7a0;
    virtual void setScaleY(float) = mac 0x22e830, win 0x1fa7e0;
    virtual void setScale(float) = mac 0x22e870, win 0x1fa820;
    virtual void setPosition(cocos2d::CCPoint const&) = mac 0x22c8b0, win 0x1f7370;
    virtual void setVisible(bool) = mac 0x22e8b0, win 0x1fa860;
    virtual void setRotation(float) = mac 0x22e6e0, win 0x1fa650;
    virtual void setOpacity(unsigned char) = mac 0x22d400, win 0x1f7d40;
    virtual void setColor(cocos2d::_ccColor3B const&) = mac 0x22cdf0, win 0x1f77f0;
    virtual void setFlipX(bool) = mac 0x22e720, win 0x1fa690;
    virtual void setFlipY(bool) = mac 0x22e7b0, win 0x1fa740;
    virtual void resetObject() = mac 0x223170, win 0x1eecd0;
    virtual cocos2d::CCPoint getRealPosition() = mac 0x22d5f0, win 0x1f7e20;
    virtual void getOrientedBox() = mac 0x22dee0, win 0x1f95d0;
    virtual void animationFinished(char const*) = mac 0x22e9d0, win 0x1fa8a0;
    void activateStreak() = mac 0x21aef0, win 0x1f9080;
    void addAllParticles() = mac 0x2189b0, win 0x1e7d60;
    void addToTouchedRings(GameObject*) = mac 0x22b800;
    void boostPlayer(float) = mac 0x21d6b0, win 0x1f8f30;
    void bumpPlayer(float, int) = mac 0x22d890;
    void buttonDown(PlayerButton) = mac 0x22b7e0;
    void checkSnapJumpToObject(GameObject*) = mac 0x2217f0;
    bool collidedWithObject(float fl, GameObject* obj) {
        auto rect = obj->getObjectRect();
        return collidedWithObject(fl, obj, rect);
    }
    bool collidedWithObject(float, GameObject*, cocos2d::CCRect) = mac 0x21f0b0, win 0x1ebdd0;
    bool collidedWithSlope(float, GameObject*, bool) = mac 0x21d8d0;
    void convertToClosestRotation(float) = mac 0x21c860, win 0x1e9ac0;
    void copyAttributes(PlayerObject*) = mac 0x22dc70, win 0x1f93f0;
    static PlayerObject* create(int, int, cocos2d::CCLayer*) = mac 0x217260, win 0x1e6cf0;
    void deactivateParticle() = mac 0x21a540, win 0x1e8f50;
    void deactivateStreak(bool) = mac 0x218b30;
    void fadeOutStreak2(float) = mac 0x225890, win 0x1f9110;
    void flashPlayer(float, float, cocos2d::_ccColor3B, cocos2d::_ccColor3B) = mac 0x221c80;
    void flipGravity(bool, bool) = mac 0x21c090, win 0x1f59d0;
    void flipMod() = mac 0x21a4c0;
    void getActiveMode() = mac 0x22b950, win 0x1f5df0;
    float getModifiedSlopeYVel() = mac 0x21bff0, win 0x1ea870;
    void getOldPosition(float) = mac 0x21a830;
    void getSecondColor() = mac 0x22cee0;
    void gravityDown() = mac 0x22e930;
    void gravityUp() = mac 0x22e900;
    void hardFlipGravity() = mac 0x22b860;
    void hitGround(bool) = mac 0x220a30;
    void incrementJumps() = mac 0x21c050, win 0x1e9a20;
    bool init(int, int, cocos2d::CCLayer*) = mac 0x2172e0, win 0x1e6da0;
    void isBoostValid(float) = mac 0x21d650, win 0x1ea810;
    void isFlying() = mac 0x21a4e0, win 0x1f5dc0;
    void isSafeFlip(float) = mac 0x2209f0;
    void isSafeMode(float) = mac 0x2209b0;
    void isSafeSpiderFlip(float) = mac 0x221be0;
    void levelFlipFinished() = mac 0x21b060, win 0x1e8e70;
    bool levelFlipping() = mac 0x21a510, win 0x1f6820;
    void levelWillFlip() = mac 0x21b020;
    void loadFromCheckpoint(PlayerCheckpoint*) = mac 0x22e420, win 0x1fa080;
    void lockPlayer() = mac 0x22d680;
    void logValues() = mac 0x221220;
    void modeDidChange() = mac 0x22bfd0;
    void placeStreakPoint() = mac 0x21af90, win 0x1f95e0;
    void playBurstEffect() = mac 0x21c780, win 0x1f6790;
    void playDeathEffect() = mac 0x225930, win 0x1efbe0;
    void playDynamicSpiderRun() = mac 0x222ec0, win 0x1f9d80;
    void playerDestroyed(bool) = mac 0x2256d0, win 0x1efaa0;
    bool playerIsFalling() = mac 0x21c730, win 0x1f5d60;
    void playerTeleported() = mac 0x22b840;
    void playingEndEffect() = mac 0x22d7e0;
    void postCollision(float) = mac 0x21cd10, win 0x1e9f50;
    void preCollision() = mac 0x21ccc0;
    void preSlopeCollision(float, GameObject*) = mac 0x21ec80;
    void propellPlayer(float) = mac 0x22d8e0, win 0x1f8b50;
    void pushButton(PlayerButton) = mac 0x22aa00, win 0x1f4e40;
    void pushDown() = mac 0x22dbd0;
    void pushPlayer(float) = mac 0x22dbb0;
    void releaseButton(PlayerButton) = mac 0x22b6f0, win 0x1f4f70;
    void removeAllParticles() = mac 0x218ac0;
    void removePendingCheckpoint() = mac 0x2237b0, win 0x1fa260;
    void resetAllParticles() = mac 0x21adb0, win 0x1e8ef0;
    void resetCollisionLog() = mac 0x21cc20;
    void resetPlayerIcon() = mac 0x22be00, win 0x1f65e0;
    void resetStateVariables() = mac 0x223760;
    void resetStreak() = mac 0x21ae10, win 0x1e8d50;
    void ringJump(GameObject*) = mac 0x22abf0, win 0x1f4ff0;
    void runBallRotation(float) = mac 0x21ca10, win 0x1e9d10;
    void runBallRotation2() = mac 0x21cb10;
    void runNormalRotation() = mac 0x21c960, win 0x1e9c50;
    void runRotateAction(bool) = mac 0x21c570, win 0x1e9bf0;
    void saveToCheckpoint(PlayerCheckpoint*) = mac 0x22e2f0, win 0x1f9ee0;
    void setSecondColor(cocos2d::_ccColor3B const&) = mac 0x219610, win 0x1f7870;
    void setupStreak() = mac 0x218720, win 0x1e7e90;
    void spawnCircle() = mac 0x2251b0;
    void spawnCircle2() = mac 0x2252a0;
    void spawnDualCircle() = mac 0x2255c0;
    void spawnFromPlayer(PlayerObject*) = mac 0x22dde0, win 0x1f9540;
    void spawnPortalCircle(cocos2d::_ccColor3B, float) = mac 0x225350, win 0x1ef680;
    void spawnScaleCircle() = mac 0x225480, win 0x1ef810;
    void specialGroundHit() = mac 0x22dbf0;
    void speedDown() = mac 0x22e970;
    void speedUp() = mac 0x22e950;
    void spiderTestJump(bool) = mac 0x21b160, win 0x1ed360;
    void startDashing(GameObject*) = mac 0x221d70;
    void stopBurstEffect() = mac 0x22c680, win 0x1f6810;
    void stopDashing() = mac 0x222990, win 0x1ee6e0;
    void stopRotation(bool) = mac 0x21c830, win 0x1e9aa0;
    void storeCollision(bool, int) = mac 0x21cc60;
    void switchedToMode(GameObjectType) = mac 0x22b9a0, win 0x1f6f70;
    void testForMoving(float, GameObject*) = mac 0x21eb70;
    void toggleBirdMode(bool) = mac 0x224070, win 0x1f6050;
    void toggleDartMode(bool) = mac 0x2243f0, win 0x1f62c0;
    void toggleFlyMode(bool) = mac 0x223820, win 0x1f5e40;
    void toggleGhostEffect(GhostType) = mac 0x225000, win 0x1f8930;
    void togglePlayerScale(bool) = mac 0x224bd0, win 0x1f9640;
    void toggleRobotMode(bool) = mac 0x223c70, win 0x1f6a10;
    void toggleRollMode(bool) = mac 0x223b20, win 0x1f68e0;
    void toggleSpiderMode(bool) = mac 0x224830, win 0x1f94d1;
    void toggleVisibility(bool) = mac 0x21abf0, win 0x1e8c30;
    void touchedObject(GameObject*) = mac 0x22e660;
    void tryPlaceCheckpoint() = mac 0x21a950, win 0x1fa2d0;
    void updateCheckpointMode(bool) = mac 0x218980;
    void updateCheckpointTest() = mac 0x21a890, win 0x1e8b50;
    void updateCollide(bool, int) = mac 0x220f10;
    void updateCollideBottom(float, int) = mac 0x221790;
    void updateCollideTop(float, int) = mac 0x221c20;
    void updateDashAnimation() = mac 0x21a570, win 0x1eea80;
    void updateDashArt() = mac 0x222520, win 0x1ee3c0;
    void updateGlowColor() = mac 0x22cf10;
    void updateJump(float) = mac 0x219680, win 0x1e8f80;
    void updateJumpVariables() = mac 0x21a740;
    void updatePlayerBirdFrame(int) = mac 0x22bfe0, win 0x1f81a0;
    void updatePlayerDartFrame(int) = mac 0x22c260, win 0x1f8580;
    void updatePlayerFrame(int) = mac 0x22c470, win 0x1f7e40;
    void updatePlayerGlow() = mac 0x22bc50, win 0x1f9290;
    void updatePlayerRobotFrame(int) = mac 0x22d620, win 0x1f8550;
    void updatePlayerRollFrame(int) = mac 0x22c6a0, win 0x1f83a0;
    void updatePlayerScale() = mac 0x22b8b0, win 0x1f9200;
    void updatePlayerShipFrame(int) = mac 0x22ba40, win 0x1f7ff0;
    void updatePlayerSpiderFrame(int) = mac 0x22d650;
    void updatePlayerSpriteExtra(gd::string) = mac 0x218440, win 0x1f8730;
    void updateRobotAnimationSpeed() = mac 0x22df40, win 0x1f9b80;
    void updateRotation(float) = mac 0x2214b0, win 0x1ebc00;
    void updateRotation(float, float) = mac 0x221230, win 0x1ebce0;
    void updateShipRotation(float) = mac 0x221310, win 0x1eba60;
    void updateShipSpriteExtra(gd::string) = mac 0x218510, win 0x1f8830;
    void updateSlopeRotation(float) = mac 0x221030;
    void updateSlopeYVelocity(float) = mac 0x22e920;
    void updateSpecial(float) = mac 0x21a790, win 0x1e8ab0;
    void updateStateVariables() = mac 0x21a770;
    void updateTimeMod(float) = mac 0x2185e0, win 0x1f9a20;
    void usingWallLimitedMode() = mac 0x22df00;
    void yStartDown() = mac 0x22e9b0;
    void yStartUp() = mac 0x22e990;

    PAD = mac 0x14, win 0x14;
    bool m_unk480;
    cocos2d::CCNode* m_unk484;
    cocos2d::CCDictionary* m_collisionLog;
    cocos2d::CCDictionary* m_collisionLog1;
    GameObject* m_collidedSlope;
    PAD = mac 0x30, win 0x1c;
    bool m_unk4B0;
    cocos2d::CCSprite* m_unk4B4;
    int m_collidedGroundObjectUniqueID;
    int m_collidedCeilObjectUniqueID;
    PAD = mac 0x14, win 0x14;
    bool m_unk4D4;
    cocos2d::CCArray* m_particleSystems;
    bool m_unk4DC;
    bool m_isHidden;
    int m_hasGhostTrail;
    GhostTrailEffect* m_ghostTrail;
    cocos2d::CCSprite* m_unknownSprite;
    cocos2d::CCSprite* m_iconSprite;
    cocos2d::CCSprite* m_iconSpriteSecondary;
    cocos2d::CCSprite* m_iconSpriteWhitener;
    cocos2d::CCSprite* m_iconGlow;
    cocos2d::CCSprite* m_vehicleSprite;
    cocos2d::CCSprite* m_vehicleSpriteSecondary;
    cocos2d::CCSprite* m_unk500;
    cocos2d::CCSprite* m_vehicleSpriteWhitener;
    cocos2d::CCSprite* m_vehicleGlow;
    cocos2d::CCMotionStreak* m_regularTrail;
    HardStreak* m_waveTrail;
    double m_xVelocity;
    double m_jumpAccel;
    double m_gravity;
    double m_unknown20;
    bool m_unk538;
    bool m_unk539;
    bool m_unk53A;
    bool m_unk53B;
    bool m_isInPlayLayer;
    bool m_unk53D;
    bool m_unk53E;
    bool m_unk53F;
    bool m_isCheckpointQueued;
    PAD = mac 0xf, win 0xf;
    double m_lastJumpTime;
    double m_unk558;
    double m_unk560;
    PAD = mac 0x1c, win 0x1c;
    float m_decelerationRate;
    PAD = mac 0x13, win 0x13;
    bool m_hasHitRing;
    GameObject* m_objectSnappedTo;
    CheckpointObject* m_checkpoint;
    int m_unknown6a0;
    GJRobotSprite* m_robotSprite;
    GJSpiderSprite* m_spiderSprite;
    bool m_unk5B0;
    cocos2d::CCParticleSystemQuad* m_playerGroundParticles;
    cocos2d::CCParticleSystemQuad* m_unk5B8;
    cocos2d::CCParticleSystemQuad* m_shipBoostParticles;
    cocos2d::CCParticleSystemQuad* m_vehicleGroundParticles;
    cocos2d::CCParticleSystemQuad* m_unk5C4;
    cocos2d::CCParticleSystemQuad* m_robotJumpParticles;
    cocos2d::CCParticleSystemQuad* m_unk5CC;
    void* m_unk5D0;
    cocos2d::CCParticleSystemQuad* m_unk5D4;
    cocos2d::CCParticleSystemQuad* m_unk5D8;
    PAD = mac 0x20, win 0x20;
    // int m_streakID;
    // float m_wellIdk;
    // PAD = win 0x10;
    bool m_unk5FC;
    bool m_unk5FD;
    bool m_hasHitPortal;
    PAD = mac 0xb, win 0xb;
    bool m_unknown73d;
    cocos2d::_ccColor3B m_unknown73e;
    cocos2d::_ccColor3B m_unknown741;
    bool m_isHolding;
    bool m_hasJustHeld;
    bool m_isHolding2;
    bool m_hasJustHeld2;
    bool m_unk615;
    bool m_unk616;
    int m_unk618;
    float m_unk61C;
    int m_unk620;
    bool m_canRobotJump;
    double m_yVelocity;
    bool m_isOnSlope;
    bool m_wasOnSlope;
    float m_unk634;
    bool m_isShip;
    bool m_isBird;
    bool m_isBall;
    bool m_isDart;
    bool m_isRobot;
    bool m_isSpider;
    bool m_isUpsideDown;
    bool m_isDead;
    bool m_isOnGround;
    bool m_isDashing;
    float m_vehicleSize;
    float m_playerSpeed;
    cocos2d::CCPoint m_unk64C;
    cocos2d::CCPoint m_lastPortalPos;
    cocos2d::CCLayer* m_unk65C;
    bool m_isSliding;
    bool m_isRising;
    bool m_isLocked;
    cocos2d::CCPoint m_lastGroundedPos;
    cocos2d::CCArray* m_touchingRings;
    GameObject* m_lastActivatedPortal;
    bool m_unk674;
    bool m_unk675;
    cocos2d::ccColor3B m_playerColor1;
    cocos2d::ccColor3B m_playerColor2;
    cocos2d::CCPoint m_position;
    bool m_unk684;
    bool m_unk685;
    double m_unk688;
    PAD = win 0x4;
    float m_meteringValue;
    float m_groundHeight;
    float m_unk69C;
    PAD = win 0x4;
    std::array<float, 200> m_unk6A4;
    PAD = win 0x1c;
}

class PointNode : cocos2d::CCObject {
    static PointNode* create(cocos2d::CCPoint point) = win 0x14ec80;

    cocos2d::CCPoint m_point;
}

class ProfilePage : FLAlertLayer, FLAlertLayerProtocol, LevelCommentDelegate, CommentUploadDelegate, UserInfoDelegate, UploadActionDelegate, UploadPopupDelegate, LeaderboardManagerDelegate {
    static ProfilePage* create(int accountID, bool idk) = mac 0x45eed0, win 0x20ee50;
    virtual void getUserInfoFailed(int) = mac 0x463FB0, win 0x2133e0;
    virtual void userInfoChanged(GJUserScore*) = mac 0x464070, win 0x213430;
    bool init(int accountID, bool idk) = mac 0x45f170, win 0x20ef00;
    void onMyLevels(cocos2d::CCObject*) = mac 0x462d70, win 0x211bb0;
    void onUpdate(cocos2d::CCObject*) = mac 0x460150, win 0x20fa20;
    void onClose(cocos2d::CCObject*) = mac 0x45fd20, win 0x49C60;
    virtual void keyBackClicked() = mac 0x464A60, win 0x49C80;
    void loadPageFromUserInfo(GJUserScore* score) = mac 0x460480, win 0x210040;

    GJUserScore* m_score;
    int m_accountID;
    PAD = mac 0x44, win 0x38, android 0x24;
    cocos2d::CCArray* m_buttons;
}

class PulseEffectAction : cocos2d::CCNode {
    static PulseEffectAction* createFromString(gd::string) = mac 0x179e90, win 0x1198a0;
    gd::string getSaveString() = mac 0x17a850, win 0x119e70;
}

class RateLevelDelegate {
    virtual void rateLevelClosed() {}
}

class RateStarsLayer : FLAlertLayer, UploadPopupDelegate, UploadActionDelegate {
    bool m_uploadFinished;
    bool m_isModSuggest;
    bool m_selectedFeature;
    CCMenuItemSpriteExtra* m_confirmBtn;
    cocos2d::CCArray* m_starBtns;
    cocos2d::CCSprite* m_difficultySprite;
    int m_levelID;
    int m_selectedRating;
    bool m_selectedCoin;
    cocos2d::CCSprite* m_featureSprite;
    UploadActionPopup* m_rateAction;
    RateLevelDelegate* m_rateDelegate;

    static RateStarsLayer* create(int levelID, bool isModSuggest) = mac 0x135e50;
    bool init(int levelID, bool isModSuggest) = mac 0x136050;
}

class RetryLevelLayer {
    static RetryLevelLayer* create() = mac 0x28dd60, win 0x216390;
}

class RewardsPage : FLAlertLayer {
    bool init() = mac 0xf3800, win 0x2178F0;
}

class RingObject : EffectGameObject {
    bool create(char const*) = win 0x252220;
    bool init(char const*) = win 0x2522E0;
    void spawnCircle() = win 0x2523A0;
    virtual void setRotation(float) = win 0x252640;
    virtual void setScale(float) = win 0x252580;
    virtual void resetObject() = win 0x252560;
    virtual void customObjectSetup(gd::map<gd::string, gd::string>&) = win 0x2526A0;
    virtual gd::string getSaveString() = win 0x2527F0;
    virtual void powerOnObject() = win 0x252360;
    virtual void powerOffObject() = win 0xe5d60; // same as GameObject
    virtual void setRScale(float) = win 0x2525D0;
    virtual void triggerActivated(float) = win 0x252350;
}

class ScrollingLayer : cocos2d::CCLayerColor {
    static ScrollingLayer* create(cocos2d::CCSize, cocos2d::CCPoint, float) = mac 0x41a900, win 0x2d8e0;
}

class SelectArtLayer {
    static SelectArtLayer* create(SelectArtType type) = win 0x174b00;
}

class SelectFontLayer : FLAlertLayer {
    static SelectFontLayer* create(LevelEditorLayer* lel) = win 0x137270;
    bool init(LevelEditorLayer* lel) = win 0x137310;
    void onChangeFont(cocos2d::CCObject* sender) = win 0x1379b0;
    void updateFontLabel() = win 0x137880;
    // Shared with many other classes
    void onClose(cocos2d::CCObject* sender) = win 0x137a30;
    // Shared with many other classes
    virtual void keyBackClicked() = win 0x137a60;
    virtual ~SelectFontLayer() = win 0x1371b0;

    int m_selectedFont;
    LevelEditorLayer* m_editorLayer;
    cocos2d::CCLabelBMFont* m_fontLabel;
}

class SetGroupIDLayer : FLAlertLayer, TextInputDelegate {
    bool init(GameObject* object, cocos2d::CCArray* objects) = mac 0x1947c0, win 0x22b670; 
    void onNextGroupID1(cocos2d::CCObject*) = mac 0x1967a0, win 0x22d790;
    void textChanged(CCTextInputNode*) = mac 0x197af0, win 0x22d610;
    void updateGroupIDLabel() = mac 0x197260, win 0x22e450;
    ~SetGroupIDLayer() = mac 0x194410, win 0x22b4d0;
    void updateEditorLayerID() = win 0x22e0b0;
    void updateEditorLayerID2() = win 0x22e110;
    void updateZOrder() = win 0x22e3d0;
    void onAddGroup(cocos2d::CCObject* sender) = mac 0x1967d0, win 0x22de20;
    void onClose(cocos2d::CCObject* sender) = mac 0x1966a0, win 0x22e830;
    void onEditorLayer(cocos2d::CCObject* sender) = win 0x22d690, mac 0x196800;
    void onEditorLayer2(cocos2d::CCObject* sender) = win 0x22d710, mac 0x196a40;
    void onZOrder(cocos2d::CCObject* sender) = win 0x22de80, mac 0x196920;

    GameObject* m_targetObject;
    cocos2d::CCArray* m_targetObjects;
    cocos2d::CCArray* m_array0;
    cocos2d::CCArray* m_array1;
    cocos2d::CCLabelBMFont* m_editorLayerText;
    cocos2d::CCLabelBMFont* m_editorLayer2Text;
    cocos2d::CCLabelBMFont* m_zOrderText;
    CCTextInputNode* m_groupIDInput;
    int m_groupIDValue;
    int m_editorLayerValue;
    int m_editorLayer2Value;
    int m_zOrderValue;
    ZLayer m_zLayerValue;
    bool m_unk204;
    bool m_unk205;
    bool m_highDetail;
    bool m_dontFade;
    bool m_dontEnter;
    void* m_somePointerProlly;
    bool m_groupParent;
    bool m_hasEditedGroups;
    int m_unknown;
}

class SetIDLayer {
    static SetIDLayer* create(GameObject*) = mac 0x168f20, win 0x22eb90;
}

class SetIDPopup : FLAlertLayer, TextInputDelegate {
    void valueChanged() = win 0x10a60;

    bool init(int, int, int, gd::string, gd::string, bool, int) = win 0x143270;

    static SetIDPopup *create(int, int, int, gd::string, gd::string, bool, int) = win 0x143130;
}

class SetIDPopupDelegate {
    virtual void setIDPopupClosed(SetIDPopup*, int) {}
}

class SetItemIDLayer {
    static SetItemIDLayer* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x5a830;
}

class SetTargetIDLayer {
    static SetTargetIDLayer* create(EffectGameObject*, cocos2d::CCArray*, gd::string) = mac 0x159d20;
    void onTargetIDArrow(cocos2d::CCObject*) = mac 0x15aed0;
    void textChanged(CCTextInputNode*) = mac 0x15b6c0;
    void updateTargetID() = mac 0x15b4a0;
}

class SetTextPopupDelegate {
    virtual void setIDPopupClosed(void*, gd::string) {}
}

class SetupAnimationPopup : FLAlertLayer {
    static SetupAnimationPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x208b70;
    void onTargetIDArrow(cocos2d::CCObject*) = mac 0x209fc0;
    void textChanged(CCTextInputNode*) = mac 0x20ab30;
    void updateTargetID() = mac 0x20a910;
}

class SetupCollisionTriggerPopup : FLAlertLayer {
    static SetupCollisionTriggerPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x1d6120;
    void onTargetIDArrow(cocos2d::CCObject*) = mac 0x1d77b0;
    void textChanged(CCTextInputNode*) = mac 0x1d84d0;
    void updateTargetID() = mac 0x1d82b0;
    bool init(EffectGameObject*, cocos2d::CCArray*) = win 0x2341f0;
    void onClose(cocos2d::CCObject*) = win 0x235970;
}

class SetupCountTriggerPopup : FLAlertLayer {
    static SetupCountTriggerPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x15c6c0;
    void onTargetIDArrow(cocos2d::CCObject*) = mac 0x15dd40;
    void textChanged(CCTextInputNode*) = mac 0x15e9a0;
    void updateTargetID() = mac 0x15e8a0;
}

class SetupInstantCountPopup : FLAlertLayer {
    static SetupInstantCountPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x352c10;
    void onTargetIDArrow(cocos2d::CCObject*) = mac 0x354520;
    void textChanged(CCTextInputNode*) = mac 0x355270;
    void updateTargetID() = mac 0x355170;
}

class SetupInteractObjectPopup : FLAlertLayer {
    static SetupInteractObjectPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x29a400;
    void onTargetIDArrow(cocos2d::CCObject*) = mac 0x29bbc0;
    void textChanged(CCTextInputNode*) = mac 0x29c2b0;
    void updateTargetID() = mac 0x29c120;
}

class SetupObjectTogglePopup : FLAlertLayer {
    static SetupObjectTogglePopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x1c0860;
    bool init(EffectGameObject*, cocos2d::CCArray*) = mac 0x1c0a40;
    void onTargetIDArrow(cocos2d::CCObject*) = mac 0x1c1c40;
    void textChanged(CCTextInputNode*) = mac 0x1c2660;
    void updateTargetID() = mac 0x1c2440;
}

class SetupOpacityPopup : FLAlertLayer {
    static SetupOpacityPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x32b70, win 0x23ba30;
    void onTargetIDArrow(cocos2d::CCObject*) = mac 0x340a0;
    void textChanged(CCTextInputNode*) = mac 0x34a60;
    void updateTargetID() = mac 0x34760;
}

class SetupPickupTriggerPopup : FLAlertLayer {
    static SetupPickupTriggerPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x35e70, win 0x23d4a0;
    bool init(EffectGameObject* obj, cocos2d::CCArray* arr) = win 0x23d550, mac 0x36070;
    void onItemIDArrow(cocos2d::CCObject*) = mac 0x37100;
    void onNextItemID(cocos2d::CCObject*) = mac 0x37260;
    void textChanged(CCTextInputNode*) = mac 0x37ca0;
    void updateItemID() = mac 0x37ab0, win 0x23e4f0;

    PAD = win 0xc, mac 0x18;
    CCTextInputNode* m_itemIDInput;
    CCTextInputNode* m_countInput;
}

class SetupPulsePopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
    virtual void colorValueChanged(cocos2d::ccColor3B color) = win 0x242660, mac 0x1ec680;
    SetupPulsePopup() {}

    bool init(EffectGameObject* triggerObj, cocos2d::CCArray* triggerObjs) = win 0x23e980, mac 0x1e7010;
    void updateColorValue() = win 0x2426b0;
    void onSelectPulseMode(cocos2d::CCObject*) = win 0x241420, mac 0x1eb020;
    void updatePulseMode() = win 0x242cf0;


    cocos2d::extension::CCControlColourPicker* m_colorPicker;
    PAD = win 0x30, mac 0x60;
    cocos2d::CCSprite* m_currentColorSpr;
    cocos2d::CCSprite* m_prevColorSpr;
    PAD = win 0x64, mac 0xac;
    int m_pulseMode; // 0x38c on mac
}

class SetupShakePopup : FLAlertLayer {
    static SetupShakePopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x3adc00;
}

class SetupSpawnPopup : FLAlertLayer {
    static SetupSpawnPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x139790;
    void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*) = mac 0x13b0e0;
    void onTargetIDArrow(cocos2d::CCObject*) = mac 0x13ad80;
    void textChanged(CCTextInputNode*) = mac 0x13b990;
    void updateTargetID() = mac 0x13b770;

    EffectGameObject* m_selectedObject;
    cocos2d::CCArray* m_selectedArray;
    CCTextInputNode* m_delayTextNode;
    Slider* m_delaySlider;
    float m_delay;
    bool m_touchTriggered;
    bool m_spawnTriggered;
    CCMenuItemToggler* m_touchTriggeredToggle;
    CCMenuItemToggler* m_spawnTriggeredToggle;
    CCTextInputNode* m_targetIDTextNode;
    int m_targetIDValue;
    bool m_textUpdateLock;
    // the toggle is inside the array for some obscure reason
    cocos2d::CCArray* m_multiTriggerToggle;
    bool m_multiTrigger;
    bool m_editorDisable;
}

class SetupTouchTogglePopup : FLAlertLayer {
    static SetupTouchTogglePopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x1576a0;
    void onTargetIDArrow(cocos2d::CCObject*) = mac 0x158b60;
    void textChanged(CCTextInputNode*) = mac 0x1596a0;
    void updateTargetID() = mac 0x159480;
}

class SimplePlayer : cocos2d::CCSprite {
    SimplePlayer() {}
    void setSecondColor(const cocos2d::ccColor3B& color) {
        
        m_secondLayer->setColor(color);
        updateColors();
    }
    bool hasGlowOutline() {
        return m_hasGlowOutline;
    }
    void setGlowOutline(bool value) {
        m_hasGlowOutline = value;
        updateColors();
    }

    bool init(int iconID) = mac 0x1bc570, win 0x12be20;
    static SimplePlayer* create(int iconID) = mac 0x1b6140, win 0x12bd80;
    void updatePlayerFrame(int iconID, IconType iconType) = mac 0x1b62f0, win 0x12c650;
    void updateColors() = mac 0x1ba1f0, win 0x12c440, ios 0x224f2c;
    void setFrames(const char* firstLayer, const char* secondLayer, const char* birdDome, const char* outlineSprite, const char* detailSprite) = mac 0x1bca10, win 0x12c9e0;
    virtual void setColor(const cocos2d::ccColor3B& color) = mac 0x1bc9b0, win 0x12c410;
    virtual void setOpacity(unsigned char opacity) = mac 0x135370, win 0x12cb90;

    cocos2d::CCSprite* m_firstLayer;
    cocos2d::CCSprite* m_secondLayer;
    cocos2d::CCSprite* m_birdDome;
    cocos2d::CCSprite* m_outlineSprite;
    cocos2d::CCSprite* m_detailSprite;
    GJRobotSprite* m_robotSprite;
    GJSpiderSprite* m_spiderSprite;
    int m_unknown;
    bool m_hasGlowOutline;
}

class Slider : cocos2d::CCLayer {
    void setValue(float val) {
        m_touchLogic->getThumb()->setValue(val);
    }
    void setBarVisibility(bool v) {
        m_sliderBar->setVisible(v);
    }
    static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler click, float scale) {
        return create(target, click, "sliderBar.png", "slidergroove.png", "sliderthumb.png", "sliderthumbsel.png", scale);
    }

    float getValue() = mac 0x18e0c0, win 0x2e970;
    void updateBar() = mac 0x18d970, win 0x2ea10;
    static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler click, const char* unused, const char* grooveSpr, const char* thumbSpr, const char* thumbSprSel, float scale) = mac 0x18dd80, win 0x2e6e0, ios 0x2113f4;

    SliderTouchLogic* m_touchLogic;
    cocos2d::CCSprite* m_sliderBar;
    cocos2d::CCSprite* m_groove;
    float m_width;
    float m_height;
}

class SliderThumb : cocos2d::CCMenuItemImage {
    void setValue(float val) = mac 0x18ce80, win 0x2e1b0, ios 0x210db4;
    float getValue() {
        return (m_fScaleX * m_length * .5f +
                (m_vertical ?
                    this->getPositionY() : 
                    this->getPositionX())
            ) / (m_fScaleX * m_length);
    }

    float m_length;
    bool m_vertical;
}

class SliderTouchLogic : cocos2d::CCMenu {
    SliderThumb* getThumb() const { return m_thumb; }

    float m_unknownUnused;
    float m_length;
    SliderThumb* m_thumb;
    Slider* m_slider;
    bool m_activateThumb;
    cocos2d::CCPoint m_position;
    bool m_rotated;
}

class SongCell : TableViewCell {
    void updateBGColor(int index) = win 0x5c6b0;
}

class SongInfoLayer : FLAlertLayer {
    static SongInfoLayer* create(int songID) = win 0x250520;
    static SongInfoLayer* create(gd::string songName, gd::string artistName, gd::string downloadLink, gd::string artistNG, gd::string artistYT, gd::string artistFB) = win 0x250830;

    gd::string m_downloadLink;
    gd::string m_artistNewgrounds;
    gd::string m_artistYoutube;
    gd::string m_artistFacebook;
}

class SongInfoObject : cocos2d::CCNode {
    virtual ~SongInfoObject() = mac 0x2f2040, win 0x196600;
    virtual void encodeWithCoder(DS_Dictionary*) = mac 0x2f2c70, win 0x1975c0;
    virtual bool canEncode() = mac 0x2f2da0, win 0x11070; // shared with tons

    int m_songID;
    gd::string m_songName;
    gd::string m_artistName;
    gd::string m_youtubeVideo;
    gd::string m_youtubeChannel;
    gd::string m_songURL;
    int m_artistID;
    float m_fileSize;
    bool m_isUnknownSong;
    int m_priority;
    LevelSettingsObject* m_levelSettings;
}

class SpawnTriggerAction : cocos2d::CCNode {
    static SpawnTriggerAction* createFromString(gd::string) = mac 0x17bf50;

    bool m_timerEnded;
    float m_delay;
    float m_timer;
    int m_group;
    int m_uuid;
}

class SpeedObject : cocos2d::CCNode {
    float m_unknown;
    float m_somethingToCompare;
    float m_idk3;
    float m_idk4;

    static SpeedObject* create(GameObject*, int, float) = win 0x20DE70;
}

class SpritePartDelegate {}

class StartPosObject : EffectGameObject {
    static StartPosObject* create() = mac 0xda7c0, win 0x25aa40;
    void setSettings(LevelSettingsObject*) = mac 0xda910;

    LevelSettingsObject* m_levelSettings;
}

class StatsCell : TableViewCell {
    inline StatsCell() {}
    inline StatsCell(char const* identifier, float parentHeight, float height) : TableViewCell(identifier, parentHeight, height) {}
    void updateBGColor(int index) = win 0x59cf0;
    virtual void draw() = mac 0x11bf80, win 0x59d40;
}

class StatsLayer : GJDropDownLayer {
    static StatsLayer* create() = win 0x25BCF0;
}

class TableView : CCScrollLayerExt, CCScrollLayerExtDelegate {
    inline TableView() {}
    inline TableView(cocos2d::CCRect rect) : CCScrollLayerExt(rect) {}

    static TableView* create(TableViewDelegate*, TableViewDataSource*, cocos2d::CCRect) = mac 0x37eb30, win 0x30ed0;
    void reloadData() = mac 0x37f970, win 0x317e0;

    virtual void onEnter() {
        CCLayer::onEnter();
    }
    virtual void onExit() {
        CCLayer::onExit();
    }
    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x380120, ios 0x21de24, win 0x31de0;
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x380be0, ios 0x21e5e8, win 0x31f30;
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x3809a0, ios 0x21e46c, win 0x31e80;
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x380b20, ios 0x21e580, win 0x31ed0;
    virtual void registerWithTouchDispatcher() = mac 0x37ff50, ios 0x21dcb4, win 0x12aa0;
    virtual void scrollWheel(float, float) = mac 0x380cd0, ios 0x21e6b4, win 0x320a0;
    virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt*) {}
    virtual void scrollViewDidEndDecelerating(CCScrollLayerExt*) {}
    virtual void scrollViewTouchMoving(CCScrollLayerExt*) {}
    virtual void scrollViewDidEndMoving(CCScrollLayerExt*) {}

    bool m_touchOutOfBoundary;
    cocos2d::CCTouch* m_touchStart;
    cocos2d::CCPoint m_touchStartPosition2;
    cocos2d::CCPoint m_unknown2;
    cocos2d::CCPoint m_touchPosition2;
    void* m_idk;
    bool m_touchMoved;
    cocos2d::CCArray* m_cellArray;
    cocos2d::CCArray* m_array2;
    cocos2d::CCArray* m_array3;
    TableViewDelegate* m_tableDelegate;
    TableViewDataSource* m_dataSource;
    int m_unused1;
    int m_unused2;
    void* m_unused3;
    int m_unused4;
    float m_touchLastY;
    bool m_cancellingTouches;
}

[[depends(CCIndexPath)]]
class TableViewCell : cocos2d::CCLayer {
    inline TableViewCell() {}
    inline ~TableViewCell() {
        removeAllChildrenWithCleanup(true);
    }

    inline TableViewCell(const char* p0, float p1, float p2) : m_unknownString(p0), m_width(p1), m_height(p2) {
        m_backgroundLayer = cocos2d::CCLayerColor::create(cocos2d::ccc4(0,0,0,0), m_width, m_height);
        addChild(m_backgroundLayer, -1);
        m_mainLayer = cocos2d::CCLayer::create();
        addChild(m_mainLayer);
        // = mac 0x383de0, win 0x32e70;
    }

    static TableViewCell* create(const char* name, float width, float height) {
        auto pRet = new TableViewCell(name, width, height);

        if (pRet) {
            pRet->autorelease();
            return pRet;
        }

        CC_SAFE_DELETE(pRet);
        return nullptr;
    }

    bool m_unknown;
    TableView* m_tableView;
    CCIndexPath m_indexPath;
    int m_unknownThing; // don't even know if this is an int, it's always set to 0
    gd::string m_unknownString;
    float m_width;
    float m_height;
    cocos2d::CCLayerColor* m_backgroundLayer;
    cocos2d::CCLayer* m_mainLayer;
}

class TableViewDataSource {
    virtual int numberOfRowsInSection(unsigned int, TableView*) { return 0; }
    virtual unsigned int numberOfSectionsInTableView(TableView*) { return 0; }
    virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&) {}
    virtual TableViewCell* cellForRowAtIndexPath(CCIndexPath&, TableView*) { return nullptr; }
}

class TableViewDelegate {
    virtual void willTweenToIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
    virtual void didEndTweenToIndexPath(CCIndexPath&, TableView*) {}
    virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
    virtual void TableViewDidDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
    virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
    virtual float cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) { return 0.0; }
    virtual void didSelectRowAtIndexPath(CCIndexPath&, TableView*) {}
}

class TeleportPortalObject : GameObject {
    PAD = mac 0x8, win 0x4;
    TeleportPortalObject* m_orangePortal;
    bool m_isYellowPortal;
    float m_teleportYOffset;
    bool m_teleportEase;
}

class TextAlertPopup : cocos2d::CCNode {
    static TextAlertPopup* create(gd::string text, float time, float scale) = mac 0x157080, win 0x1450b0;
}

class TextArea : cocos2d::CCSprite {
    // https://www.youtube.com/watch?v=1LVW7IUyKMg
    TextArea() = mac 0x19fba0, win 0x33110, ios 0x92c28;
    inline TextArea(TextArea const&) : m_fontFile() {}
    inline ~TextArea() {}
    virtual void draw() {}
    virtual void setOpacity(unsigned char) = mac 0x19f760, win 0x33800;
    bool init(gd::string str, char const* font, float width, float height, cocos2d::CCPoint anchor, float scale, bool disableColor) = mac 0x19ec70, win 0x33370, ios 0x92444;
    static TextArea* create(gd::string str, char const* font, float scale, float width, cocos2d::CCPoint anchor, float lineHeight, bool disableColor) = mac 0x19eb40, win 0x33270;
    void colorAllCharactersTo(cocos2d::ccColor3B color) = win 0x33830;
    void setString(gd::string str) = mac 0x19eda0, win 0x33480;

    bool m_disableColor;            // 0x1e4
    MultilineBitmapFont* m_label;   // 0x1e8
    float m_width;                  // 0x1ec
    int m_unknown;                  // 0x1f0
    gd::string m_fontFile;          // 0x1f4
    float m_height;                 // 0x20c
}

class TextInputDelegate {
    virtual void textChanged(CCTextInputNode*) {}
    virtual void textInputOpened(CCTextInputNode*) {}
    virtual void textInputClosed(CCTextInputNode*) {}
    virtual void textInputShouldOffset(CCTextInputNode*, float) {}
    virtual void textInputReturn(CCTextInputNode*) {}
    virtual bool allowTextInput(CCTextInputNode*) {return true;}
}

class ToggleTriggerAction : cocos2d::CCNode {
    static ToggleTriggerAction* createFromString(gd::string) = mac 0x1765e0;
}

class TopArtistsLayer : FLAlertLayer {
    static TopArtistsLayer* create() = mac 0x192a90, win 0x25d8e0;
    virtual ~TopArtistsLayer() = win 0x25d7c0;
    void setupLeaderboard(cocos2d::CCArray*) = mac 0x193420;
    virtual bool init() = mac 0x192c30, win 0x25d980;
    void loadPage(int) = mac 0x193b60;
    void setupPageInfo(gd::string, char const*) = mac 0x193730;
}

class TouchToggleAction : cocos2d::CCNode {
    static TouchToggleAction* createFromString(gd::string) = mac 0x177e10;
}

class TriggerEffectDelegate {
    virtual void toggleGroupTriggered(int, bool) {}
    virtual void spawnGroup(int) {}
}

class UILayer : cocos2d::CCLayerColor {
    static UILayer* create() = mac 0x27fd10, win 0x25f310;
    virtual bool init() = mac 0x27fe40, win 0x25f3b0;
    void disableMenu() = mac 0x280960;
    void enableMenu() = mac 0x280940;
    void pCommand(cocos2d::CCNode*) = mac 0x280830;
    inline void toggleCheckpointsMenu(bool toggled) {
        m_checkPointMenu->setVisible(toggled);
    }
    void onCheck(cocos2d::CCObject* sender) = mac 0x2803e0, win 0x25fb60;
    void onDeleteCheck(cocos2d::CCObject* sender) = mac 0x280410, win 0x25fc90;
    void onPause(cocos2d::CCObject* sender) = mac 0x2803c0, win 0x25fad0;
    virtual void keyDown(cocos2d::enumKeyCodes key) = mac 0x280470, win 0x25f890;
    virtual void keyUp(cocos2d::enumKeyCodes key) = mac 0x280600, win 0x25fa10;
    UILayer() = win 0x25f230;
    ~UILayer() = win 0x25fef0, mac 0x280c90;

    PAD = mac 0x16, win 0x8, android 0x8;
    cocos2d::CCMenu* m_checkPointMenu;
}

class UndoObject : cocos2d::CCObject {
    ~UndoObject() = mac 0xa2fd0;
    static UndoObject* create(GameObject*, UndoCommand) = mac 0x94ea0, win 0x16bc20;
    static UndoObject* createWithArray(cocos2d::CCArray*, UndoCommand) = mac 0x96ee0, win 0x16bee0;
    static UndoObject* createWithTransformObjects(cocos2d::CCArray* pObjects, UndoCommand nCommand) = win 0x16bcd0;

    GameObjectCopy* m_objectCopy;
    UndoCommand m_command;
    cocos2d::CCArray* m_objects;
    bool m_redo;
}

class UploadActionDelegate {
    virtual void uploadActionFinished(int, int) {}
    virtual void uploadActionFailed(int, int) {}
}

class UploadActionPopup : FLAlertLayer {
    UploadPopupDelegate* m_popupDelegate;
    TextArea* m_messageArea;
    LoadingCircle* m_loadingCircle;
    CCMenuItemSpriteExtra* m_closeBtn;
}

class UploadMessageDelegate {}

class UploadPopupDelegate {
    virtual void onClosePopup() {}
}

class UserInfoDelegate {
    virtual void getUserInfoFinished(GJUserScore *) {}
    virtual void getUserInfoFailed(int) {}
    virtual void userInfoChanged(GJUserScore *) {}
}

class UserListDelegate {
    virtual void forceReloadList(UserListType) {}
    virtual void getUserListFailed(UserListType, GJErrorCode) {}
    virtual void getUserListFinished(cocos2d::CCArray*, UserListType) {}
    virtual void userListChanged(cocos2d::CCArray*, UserListType) {}
}

class VideoOptionsLayer : FLAlertLayer {
    VideoOptionsLayer* create() = win 0x1e0d70;
    bool init() = win 0x1e0e10;
    void onAdvanced(cocos2d::CCObject*) = win 0x1e1b40;
    void onFullscreen(cocos2d::CCObject*) = win 0x1e1b60;
    void onTextureQualityNext(cocos2d::CCObject*) = win 0x1e1b80;
    void onTextureQualityPrev(cocos2d::CCObject*) = win 0x1e1b90;
    void updateTextureQuality(int index) = win 0x1e1ba0;
    void onResolutionPrev(cocos2d::CCObject*) = win 0x1e1c40;
    void onResolutionNext(cocos2d::CCObject*) = win 0x1e1c50;
    void updateResolution(int index) = win 0x1e1c60;
    void toggleResolution() = win 0x1e1d90;
    void onApply(cocos2d::CCObject*) = win 0x1e1e70;

    CCMenuItemSpriteExtra* m_prevResolution;
    CCMenuItemSpriteExtra* m_nextResolution;
    cocos2d::CCLabelBMFont* m_resolutionLabel;
    cocos2d::CCLabelBMFont* m_resolutionText;
    cocos2d::CCLabelBMFont* m_qualityLabel;
    cocos2d::TextureQuality m_quality;
    cocos2d::CCArray* m_resolutions;
    bool m_isFullscreen;
    int m_currentResolution;
}

class LevelTools {
    static gd::string base64EncodeString(gd::string) = mac 0x294470, win 0x18b310;
    static gd::string base64DecodeString(gd::string) = mac 0x294510, win 0x18b3b0;
    static GJGameLevel* getLevel(int, bool) = mac 0x2908c0, win 0x189370;
    static bool verifyLevelIntegrity(gd::string, int) = mac 0x294360, win 0x18b180;
    static float xPosForTime(float, cocos2d::CCArray*, int) = mac 0x293d90, win 0x18acd0;
    static float timeForXPos(float, cocos2d::CCArray*, int) = mac 0x293eb0, win 0x18ae70;
    static gd::string getAudioFileName(int) = mac 0x292840;
    static gd::string getAudioTitle(int) = mac 0x2922f0;
    static gd::string artistForAudio(int) = mac 0x292d90;
    static gd::string urlForAudio(int) = mac 0x292f10;
}
// clang-format on
