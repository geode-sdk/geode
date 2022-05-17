class AchievementBar : cocos2d::CCNodeRGBA {
	static AchievementBar* create(char const* title, char const* desc, char const* icon, bool quest) = mac 0x379f80, win 0x3b120, ios 0x1a4784;

	PAD = win 0x24;
}

class AchievementCell {
	void loadFromDict(cocos2d::CCDictionary*) = mac 0x10eaa0;
}

class AchievementManager : cocos2d::CCNode {
	void getAllAchievements() = mac 0x434d60;
	static AchievementManager* sharedState() = mac 0x424420, win 0x7b10;

	PAD = win 0x10;
	cocos2d::CCDictionary* m_achievements;
	PAD = win 0x4;
}

class AchievementNotifier : cocos2d::CCNode {
	void notifyAchievement(char const* title, char const* desc, char const* icon, bool quest) {
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

class AchievementsLayer {
	void customSetup() = mac 0x1bdea0;
	void loadPage(int) = mac 0x1be190, win 0x3c600;
}

class AnimatedGameObject {
	void playAnimation(int) = mac 0xc93d0;
	void updateChildSpriteColor(cocos2d::ccColor3B) = mac 0xc8450;
}

class AnimatedShopKeeper : CCAnimatedSprite {
	void animationFinished(char const*) {}

	static AnimatedShopKeeper* create(ShopType type) = win 0x14c4d0;
	void startAnimating() = win 0x14c690;

	float m_unknown1;
	float m_unknown2;
	bool m_unknown;
}

class AnimatedSpriteDelegate {}

class AppDelegate : cocos2d::CCApplication {
	void bgScale() = mac 0x3aaab0;
	virtual bool applicationDidFinishLaunching() = mac 0x3aa900, win 0x3cbb0;
	virtual void applicationDidEnterBackground() = mac 0x3aabe0, win 0x3cf40;
	virtual void applicationWillEnterForeground() = mac 0x3aac80, win 0x3d130;
	virtual void applicationWillBecomeActive() = mac 0x3aab30, win 0x3ce90;
	virtual void applicationWillResignActive() = mac 0x3aab50, win 0x3d3e0;
	virtual void trySaveGame() = mac 0x3aaf10, win 0x3d5e0, ios 0x1a28f0;
	virtual void willSwitchToScene(cocos2d::CCScene*) = mac 0x3aaf40, win 0x3d690;
	static AppDelegate* get() = mac 0x3aab10;

	cocos2d::CCScene* m_runningScene;
}

class ArtistCell : TableViewCell {
	void draw() = mac 0x11c980;
	virtual bool init() = mac 0x11c7c0;
	void loadFromObject(SongInfoObject*) = mac 0x1118b0;
	void onNewgrounds(cocos2d::CCObject*) = mac 0x11c7e0;
	void updateBGColor(int) = mac 0x110460, win 0x5c6b0;

	void* m_idk;
}

class AudioEffectsLayer {
	void audioStep(float) = mac 0x271f40;
	static AudioEffectsLayer* create(gd::string) = mac 0x271a00;
	void resetAudioVars() = mac 0x271ee0;
}

class BoomListView : cocos2d::CCLayer, TableViewDelegate, TableViewDataSource {
	inline ~BoomListView() {
	    CC_SAFE_RELEASE(m_entries);
	}

	static BoomListView* create(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x18ecb0;
	bool init(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x18ee00, win 0x10c20, ios 0x12c5a8;
	virtual void draw() = mac 0x18f790;

	virtual void setupList() = mac 0x18ef90, win 0x10dc0;
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) = mac 0x18f030;
	virtual float cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f070, win 0x10e50;
	virtual void didSelectRowAtIndexPath(CCIndexPath&, TableView*) {}
	virtual int numberOfRowsInSection(unsigned int, TableView*) = mac 0x18f0b0, win 0x10e60;
	virtual unsigned int numberOfSectionsInTableView(TableView*) = mac 0x18f0e0, win 0x10a70;
	virtual TableViewCell* cellForRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f100, win 0x10e70;
	virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&) = mac 0x18f770;
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) = mac 0x18f050;
	virtual TableViewCell* getListCell(char const*) = mac 0x18f200, win 0x10ed0;
	virtual void loadCell(TableViewCell*, unsigned int) = mac 0x18f4a0, win 0x10ff0;
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
	BoomScrollLayer() = mac 0x1e42f0;
	void instantMoveToPage(int page) = win 0x12330;
	void moveToPage(int page) = win 0x12400;

	cocos2d::CCArray* m_dots;
	PAD = win 0x40;
	ExtendedLayer* m_layer;
	PAD = win 0x48;
	int m_page;
}

class ButtonSprite : cocos2d::CCSprite {
	// you really should find this function or a higher overload on mac
	static ButtonSprite* create(char const* caption, int width, int unknown, float scale, bool absoluteWidth, char const* font, char const* texture, float height) = win 0x137d0, mac 0x4fa90, ios 0x38c7c;

	[[docs("
	/**
	* Create a ButtonSprite with text, a font and a texture.
	* @param caption The text of the ButtonSprite
	* @param width Sprite width; ignored if `absolute` is false
	* @param absolute Whether to use absolute width or not
	* @param font The name of the BM font file to use
	* @param texture The name of the background sprite file (can't be in a spritesheet)
	* @param height The height of the button, leave 0 for automatic
	* @param scale Scale of text
	* @returns Pointer to the created ButtonSprite, or nullptr on error
	*/
	")]]
	static ButtonSprite* create(char const* caption, int width, bool absolute, char const* font, char const* texture, float height, float scale) {
		return create(caption, width, 0, scale, absolute, font, texture, height);
	}

	inline static ButtonSprite* create(char const* caption) {
		return ButtonSprite::create(caption, 0, 0, "goldFont.fnt", "GJ_button_01.png", .0f, 1.f);
	}
	inline static ButtonSprite* create(char const* caption, char const* font, char const* texture, float scale = 1.f) {
		return ButtonSprite::create(caption, 0, 0, font, texture, .0f, scale);
	}
	static ButtonSprite* create(char const*, int, int, float, bool) = mac 0x4fa40;
	void updateBGImage(char const*) = mac 0x502d0, win 0x13af0;
	static ButtonSprite* create(char const*, float) = mac 0x4fa60;
	void setString(char const* txt) = win 0x14030;
	void updateSpriteBGSize(cocos2d::CCPoint const& offset) = win 0x13c00;
	void setColor(cocos2d::ccColor3B color) = mac 0x50410, win 0x143e0;

	PAD = win 0x18;
	cocos2d::CCLabelBMFont* m_label;
	cocos2d::CCSprite* m_subSprite;
	cocos2d::CCSprite* m_subBGSprite;
	cocos2d::extension::CCScale9Sprite* m_BGSprite;
	PAD = win 0x8;
	cocos2d::CCPoint m_spritePosition;
}

class CCAnimatedSprite : cocos2d::CCSprite {
	void runAnimation(gd::string) = mac 0x1a6430, win 0x14f60;
	void tweenToAnimation(gd::string, float) = mac 0x1a65b0;
	static CCAnimatedSprite* create(char const* file) = win 0x14540;

	gd::string m_unknown1;
	gd::string m_unknown2;
	PAD = win 0x14;
	gd::string m_unknown3;
	PAD = win 0x4;
}

class CCAnimateFrameCache : cocos2d::CCObject {
	static CCAnimateFrameCache* sharedSpriteFrameCache() = win 0x158f0;
	void addSpriteFramesWithFile(char const* file) = win 0x159b0;
}

class CCBlockLayer : cocos2d::CCLayerColor {
	void disableUI() = mac 0x2a5a80;
	void draw() = mac 0x2a5c20;
	void enableUI() = mac 0x2a5a90;
	void enterAnimFinished() = mac 0x2a5bb0;
	void enterLayer() = mac 0x2a5aa0;
	void exitLayer() = mac 0x2a5b40;
	void hideLayer(bool) = mac 0x2a5ba0;
	virtual bool init() = mac 0x2a59c0;
	void layerHidden() = mac 0x2a5be0;
	void layerVisible() = mac 0x2a5bc0;
	void registerWithTouchDispatcher() = mac 0x2a5ad0;
	void showLayer(bool) = mac 0x2a5b90;

	bool m_unknown;
	bool m_unknown2;
}

class CCCircleWave : cocos2d::CCNode {
	static CCCircleWave* create(float, float, float, bool) = mac 0xbd270;
	static CCCircleWave* create(float, float, float, bool, bool) = mac 0xbd290, win 0x16c00;
	bool init(float, float, float, bool, bool) = mac 0xbd380;
	void followObject(cocos2d::CCNode*, bool) = mac 0xbd670, win 0x16f20;
	void updatePosition(float) = mac 0xbd630, win 0x16f00;
	void setPosition(cocos2d::CCPoint const& pos) = win 0x16ed0;
	void removeMeAndCleanup() = win 0x17280;
	void draw() = win 0x17100;
	void updateTweenAction(float dt, char const* key) = win 0x16f90;

	cocos2d::CCArray* m_children;
	PAD = win 0x4;
	float m_currentRadius;
	float m_currentOpacity;
	cocos2d::ccColor3B color = mac 0x134;
	cocos2d::CCPoint m_circleCenter;
	int m_filled;
	int m_lineWidth;
	float m_opacityMultiplier;
	bool m_blendFuncDirty;
	CCCircleWaveDelegate* delegate = mac 0x150;
}

class CCCircleWaveDelegate {
	virtual void circleWaveWillBeRemoved(CCCircleWave* wave) {}
}

class CCContentLayer : cocos2d::CCLayerColor {
	static CCContentLayer* create(cocos2d::ccColor4B const& color, float width, float height) = win 0x172a0;
	virtual void setPosition(cocos2d::CCPoint const& pos) = win 0x17400;
}

class CCIndexPath : cocos2d::CCObject {
	static CCIndexPath* create(unsigned int idk1, int idk2) = win 0x30e40;

	int m_unknown1;
	int m_unknown2;
}

class CCLightFlash {
	static CCLightFlash* create() = mac 0x295870;
	void playEffect(cocos2d::CCPoint, cocos2d::ccColor3B, float, float, float, float, float, float, float, float, float, float, float, float, float, float, int, bool, bool, float) = mac 0x295900, win 0x179f0;
}

class CCMenuItemSpriteExtra : cocos2d::CCMenuItemSprite {
	void useAnimationType(MenuAnimationType type) {
	    this->m_startPosition = this->getNormalImage()->getPosition();
	    this->m_animationType = type;
	}
	void setDestination(cocos2d::CCPoint const& pos) {
	    this->m_destPosition = pos;
	}
	void setOffset(cocos2d::CCPoint const& pos) {
	    this->m_offset = pos;
	}
	void setScale(float scale) {
	    this->CCMenuItemSprite::setScale(scale);
	    this->m_baseScale = scale;
	}

	static CCMenuItemSpriteExtra* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1253c0, win 0x18ee0, ios 0xe0740;
	inline static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
		return CCMenuItemSpriteExtra::create(sprite, nullptr, target, callback);
	}
	void setSizeMult(float) = mac 0x1255e0, win 0x19080;
	CCMenuItemSpriteExtra() = mac 0x32670, win 0x18db0;
	~CCMenuItemSpriteExtra() = win 0x18eb0;
	bool init(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x125450, win 0x18fa0;
	bool init(cocos2d::CCNode* spr) = win 0x18fa0;
	void activate() = win 0x191c0;
	void selected() = win 0x19270;
	void unselected() = win 0x19430;

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

	static CCMenuItemToggler* createWithSize(char const* spr1, char const* spr2, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale) {
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
	static CCMoveCNode* create() = mac 0x1842a0;
	virtual bool init() = mac 0x18b3d0;
	~CCMoveCNode() = mac 0x18b2c0;
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
	        this->m_contentLayer->getContentSize().height -
	        this->m_scrollLimitTop;
	}
	float getMaxY() {
	    return this->m_scrollLimitBottom;
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

class CCSpritePlus : cocos2d::CCSprite {
	bool initWithSpriteFrameName(char const*) = mac 0x248670, win 0x1c1e0;
	void setScaleX(float scale) = win 0x1c440;
	void setScaleY(float scale) = win 0x1c4c0;
	void setScale(float scale) = win 0x1c540;
	void setPosition(cocos2d::CCPoint const& pos) = win 0x1c220;
	void setRotation(float rotation) = win 0x1c280;
	bool initWithTexture(cocos2d::CCTexture2D* texture) = win 0x1c200;
	void setFlipX(bool flip) = win 0x1c300;
	void setFlipY(bool flip) = win 0x1c3a0;
	static CCSpritePlus* createWithSpriteFrame(cocos2d::CCSpriteFrame* frame) = win 0x1c130;

	cocos2d::CCArray* m_followers;
	CCSpritePlus* m_followingSprite;
	bool m_hasFollower;
	bool m_propagateScaleChanges;
	bool m_propagateFlipChanges;
}

class CCTextInputNode : cocos2d::CCLayer, cocos2d::CCIMEDelegate, cocos2d::CCTextFieldDelegate {
	inline CCTextInputNode() : cocos2d::CCLayer(), cocos2d::CCIMEDelegate(), cocos2d::CCTextFieldDelegate(), m_caption(), m_allowedChars() {
		m_unknown0 = nullptr;
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
	char const* getString() {
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
	bool init(float width, float height, char const* caption, char const* thonburi, int maxCharCount, char const* font) = mac 0x5d180, win 0x20e50, ios 0xe261c;

	void refreshLabel() = mac 0x5d730, win 0x21330, ios 0xe2af4;
	void updateLabel(gd::string) = mac 0x5d4a0;
	virtual void registerWithTouchDispatcher() = mac 0x5eec0, win 0x220e0;
	virtual void visit() = mac 0x5d380, win 0x21000;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ec80, win 0x21f20;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ee80;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ee60;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5eea0;
	virtual void textChanged() = mac 0x5dd70, win 0x216e0;
	virtual void onClickTrackNode(bool) = mac 0x5dd40, win 0x216b0;
	virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5dad0, win 0x21580;
	virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5dc20, win 0x21650;
	virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int) = mac 0x5de50;
	virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF*) = mac 0x5e2c0, win 0x21b10;
	virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF*) = mac 0x5e610, win 0x21d60;

	void* m_unknown0;
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
	static ChallengesPage* create() = win 0x3e050;
}

class CheckpointObject : cocos2d::CCNode {
	static CheckpointObject* create() = mac 0x7e7d0;
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
	void getSaveString() = mac 0x17d080;
	void setupFromDict(cocos2d::CCDictionary*) = mac 0x17f310;
	void setupFromString(gd::string) = mac 0x17f270;

	PAD = mac 0xc;
	cocos2d::ccColor3B color = mac 0x12c;
	PAD = mac 0x5;
	float m_unk100;
	bool m_blending;
	PAD = mac 0x3;
	int m_playerColor;
	int m_unk10C;
	float m_opacity;
	float m_unk114;
	float m_copyHue;
	float m_copySaturation;
	float m_copyBrightness;
	bool m_saturationChecked;
	bool m_brightnessChecked;
	int m_copyID;
	PAD = mac 0x1;
	bool m_copyOpacity;
	PAD = mac 0x4;
	ColorActionSprite* m_colorSprite;
	PAD = mac 0xc;
}

class ColorActionSprite : cocos2d::CCNode {
	float m_opacity;
	cocos2d::ccColor3B m_f0124;
	cocos2d::ccColor3B m_activeColor;
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

class ColorSelectPopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
	virtual void colorValueChanged(cocos2d::ccColor3B color) = win 0x46ee0;

	bool init(EffectGameObject* triggerObj, cocos2d::CCArray* triggerObjs, ColorAction* colorAction) = win 0x43ae0;
	void updateColorValue() = win 0x46f30;
	void updateCopyColorTextInputLabel() = win 0x479c0;

	cocos2d::extension::CCControlColourPicker* m_colorPicker;
	cocos2d::CCLabelBMFont* m_unk1DC;
	cocos2d::CCLabelBMFont* m_label;
	Slider* m_unk1E4;
	Slider* m_slider;
	EffectGameObject* m_effectGameObject;
	cocos2d::CCArray* m_unk1F0;
	CCMenuItemToggler* m_toggler1;
	CCMenuItemToggler* m_toggler2;
	unsigned int m_rgbLastColor;
	cocos2d::CCSprite* m_unk200;
	cocos2d::CCSprite* m_unk204;
	unsigned int m_bgrColor; // TODO: are u sure this isnt just a cccolor3b
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
	PAD = win 0x10;
	cocos2d::CCArray* m_unk254;
	cocos2d::CCArray* m_unk258;
	CCTextInputNode* m_textInput2;
	PAD = win 0x4;
	CCMenuItemToggler* m_toggler3;
	CCMenuItemToggler* m_toggler4;
	PAD = win 0x8;
	cocos2d::CCArray* m_unk274;
	bool m_spawnTrigger;
	bool m_multiTrigger;
	bool m_copyColor;
}

class ColorSetupDelegate {}

class CommentCell : TableViewCell {
	void loadFromComment(GJComment*) = mac 0x111c70, win 0x5f3d0;

	PAD = win 0x4;
	cocos2d::CCSprite* m_iconSprite;
	cocos2d::CCLabelBMFont* m_likeLabel;
	GJComment* m_comment;
	PAD = win 0x4;
}

class CommentUploadDelegate {
	virtual void commentUploadFinished(int) {}
	virtual void commentUploadFailed(int, CommentError) {}
	virtual void commentDeleteFailed(int, int) {}
}

class ConfigureHSVWidget : cocos2d::CCNode {
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
	bool multiActivate = mac 0x138;
}

class CreateGuidelinesLayer : FLAlertLayer, FLAlertLayerProtocol {
	void onStop(cocos2d::CCObject* pSender) = win 0x4d2c0;

	PAD = win 0x24;
	gd::string m_guidelineString;
}

class CreateMenuItem : CCMenuItemSpriteExtra {
	static CreateMenuItem* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1c580;

	PAD = win 0x18;
	int m_objectID;
	int m_buildTabPage;
	int m_buildTab;
}

//TODO: inherits cocos2d::CCSceneTransitionDelegate
class CreatorLayer : cocos2d::CCLayer {
	void onBack(cocos2d::CCObject*) = win 0x4fae0;
	void onChallenge(cocos2d::CCObject*) = win 0x4f1b0;
	void onLeaderboards(cocos2d::CCObject*) = win 0x4ed20;
	void onMyLevels(cocos2d::CCObject*) = mac 0x142b70;
	void onSavedLevels(cocos2d::CCObject*) = mac 0x142860;
	virtual void sceneWillResume() = win 0x4fb50;
	bool init() = win 0x4de40;
	static CreatorLayer* create() = win 0x4dda0;
}

class CurrencyRewardLayer {
	~CurrencyRewardLayer() = mac 0x447950;
	virtual void update(float) = mac 0x44a5c0;
}

class CurrencyRewardDelegate {
	virtual void currencyWillExit(CurrencyRewardLayer*) {}
}

class CustomListView : BoomListView {
	static CustomListView* create(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x10d410, win 0x57f90, ios 0x99154;
	inline static CustomListView* create(cocos2d::CCArray* entries, BoomListType type, float width, float height) {
		return CustomListView::create(entries, width, height, 0, type);
	}
	virtual TableViewCell* getListCell(char const*) = mac 0x10d560, win 0x58050;
	virtual void loadCell(TableViewCell*, unsigned int) = mac 0x10e610, win 0x585c0;
	virtual void setupList() = mac 0x116e70, win 0x58870;
	CustomListView() = win 0x57e60;
}

class CustomSongCell : TableViewCell {
	void loadFromObject(SongInfoObject*) = mac 0x110220;
}

class CustomSongLayer : FLAlertLayer, FLAlertLayerProtocol, TextInputDelegate, GJDropDownLayerDelegate {
	bool init(LevelSettingsObject*) = mac 0xf06f0;
	void onArtists(cocos2d::CCObject*) = mac 0xf1950;
	void onSongBrowser(cocos2d::CCObject*) = mac 0xf18a0;

	LevelSettingsObject* m_levelSettings;
	CCTextInputNode* m_songIDInput;
	CustomSongWidget* m_songWidget;
	LevelSettingsLayer* m_levelSettingsLayer;
}

class CustomSongWidget : cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool) {}
	void loadSongInfoFinished(SongInfoObject*) {}

	void updateSongObject(SongInfoObject* song) = win 0x69280;

	SongInfoObject* m_songInfo;
	PAD = win 0x1C;
	CCMenuItemSpriteExtra* m_downloadBtn;
	PAD = win 0x30;
}

class CustomizeObjectLayer : FLAlertLayer, TextInputDelegate, HSVWidgetPopupDelegate, ColorSelectDelegate, ColorSetupDelegate {
	void onNextColorChannel(cocos2d::CCObject* pSender) = win 0x56c80;
	void onSelectColor(cocos2d::CCObject* pSender) = win 0x577b0;
	int getActiveMode(bool unknown) = win 0x57210;

	EffectGameObject* m_targetObject;
	cocos2d::CCArray* m_targetObjects;
	cocos2d::CCArray* m_colorButtons;
	cocos2d::CCArray* m_colorNodes;
	cocos2d::CCArray* m_textInputNodes;
	PAD = win 0x4;
	cocos2d::CCArray* m_detailColorButtons;
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

class DailyLevelPage {
	static DailyLevelPage* create(bool weekly) = win 0x6a860;
	bool init(bool weekly) = win 0x6a900;
	void updateTimers(float) = win 0x6bef0;

	PAD = win 0x1ed;
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
	bool init(DialogLayer*, DialogObject*, cocos2d::CCArray*, int) = win 0x6D520;
	cocos2d::CCAction* animateIn(int location) = win 0x6E130;

	~DialogLayer() = mac 0x204720;
	virtual void onEnter() = mac 0x205900;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205790;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205820;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2057e0;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205840;
	virtual void registerWithTouchDispatcher() = mac 0x205890;
	virtual void keyBackClicked() = mac 0x2056a0;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x205ce0;
	virtual void fadeInTextFinished(TextArea*) = mac 0x205930;
}

class DialogDelegate {
	virtual void dialogClosed(DialogLayer*) {}
}

class DialogObject : cocos2d::CCObject {
	static DialogObject* create(gd::string title, gd::string text, int portrait, float text_scale, bool is_unskippable, cocos2d::ccColor3B text_color) = win 0x6D160;
	bool init(DialogObject*, gd::string title, gd::string text, int portrait, float text_scale, bool is_unskippable, cocos2d::ccColor3B text_color) = win 0x6D2E0;

	gd::string m_text;
	gd::string m_title;
	int m_dialogType;
	cocos2d::ccColor3B m_colour;
	float m_textWidth;
	bool m_canSkip;
}

class DownloadMessageDelegate {}

class DrawGridLayer : cocos2d::CCLayer {
	inline using CCPointArray400 = cocos2d::CCPoint(*)[400];
	inline using CCPointArray200 = cocos2d::CCPoint(*)[200];

	void draw() = win 0x16ce90;

	CCPointArray400 m_commonLines;
	CCPointArray200 m_yellowGuidelines;
	CCPointArray200 m_greenGuidelines;
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
	    this->m_buttonArray->removeAllObjects();
	    this->reloadItemsInNormalSize();
	}
	void reloadItems(int rowCount, int columnCount) {
	    if (this->m_buttonArray)
	        this->loadFromItems(this->m_buttonArray, rowCount, columnCount, this->m_unknown);
	}
	void reloadItemsInNormalSize() {
		// TODO: fix this
	    // this->reloadItems(
	    //     GameManager::sharedState()->getIntGameVariable("0049"),
	    //     GameManager::sharedState()->getIntGameVariable("0050")
	    // );
	}
	void addButton(CCMenuItemSpriteExtra* btn, bool reload) {
	    if (this->m_buttonArray)
	        this->m_buttonArray->addObject(reinterpret_cast<cocos2d::CCObject*>(btn));
	    if (reload)
	        this->reloadItemsInNormalSize();
	}

	void loadFromItems(cocos2d::CCArray* buttons, int rowCount, int columnCount, bool idk) = mac 0x351010, win 0x6e5e0, ios 0x2dd060;

	cocos2d::CCPoint m_position;
	int m_unknown;
	bool m_unknownBool;
	cocos2d::CCArray* m_buttonArray;
	BoomScrollLayer* m_scrollLayer;
	cocos2d::CCArray* m_pagesArray;
}

class EditLevelLayer : cocos2d::CCLayer {
	static void scene(GJGameLevel* level) {
	    auto pScene = cocos2d::CCScene::create();
	
	    pScene->addChild(reinterpret_cast<cocos2d::CCNode*>(EditLevelLayer::create(level)));
	
	    cocos2d::CCDirector::sharedDirector()->replaceScene(
	        cocos2d::CCTransitionFade::create(.5f, pScene)
	    );
	}

	static EditLevelLayer* create(GJGameLevel* level) = mac 0xe1e50, win 0x6f530, ios 0x82420;
	void onLevelInfo() = win 0x70660;

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
	static EditorPauseLayer* create(LevelEditorLayer* editor) {
	    auto pRet = new EditorPauseLayer();
	    if (pRet && pRet->init(editor)) {
	        pRet->autorelease();
	        return pRet;
	    }
	    CC_SAFE_DELETE(pRet);
	    return nullptr;
	}

	~EditorPauseLayer() = mac 0x13c3b0;
	virtual void keyBackClicked() = mac 0x13f320, win 0x758d0;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x13f3a0;
	virtual void customSetup() = mac 0x13cc00;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x13f1b0, win 0x75780;
	void saveLevel() = mac 0x13ebd0, win 0x75010;
	bool init(LevelEditorLayer*) = mac 0x13c7a0, win 0x730e0, ios 0x280cb8;
	void onExitEditor(cocos2d::CCObject* pSender) = win 0x75660;
	void playStep2() = win 0x75440;
	void onResume(cocos2d::CCObject* pSender) = win 0x74fe0;
	void onSaveAndPlay(cocos2d::CCObject* pSender) = win 0x753d0;
	void onSaveAndExit(cocos2d::CCObject* pSender) = win 0x75620;
	void onSave(cocos2d::CCObject* pSender) = win 0x755a0;
	void onExitNoSave(cocos2d::CCObject* pSender) = win 0x75700;
	void uncheckAllPortals(cocos2d::CCObject* pSender) = win 0x74760;
	EditorPauseLayer() = win 0x72f10;

	bool m_saved;
	PAD = win 0x4;
	CCMenuItemSpriteExtra* m_button0;
	CCMenuItemSpriteExtra* m_button1;
	LevelEditorLayer* m_editorLayer;
}

class EditorUI : cocos2d::CCLayer, FLAlertLayerProtocol, ColorSelectDelegate, GJRotationControlDelegate, GJScaleControlDelegate, MusicDownloadDelegate {
	static EditorUI* get() {
	    auto lel = LevelEditorLayer::get();
	    if (!lel) return nullptr;
	    #ifdef GEODE_IS_WINDOWS
	    return lel->m_editorUI;
	    #else
	    return nullptr;
	    #endif
	}

	void constrainGameLayerPosition() = mac 0x1c6d0;
	void create(LevelEditorLayer*) = mac 0x8a80;
	void deselectAll() = mac 0x1f300, win 0x86af0;
	void onDeselectAll(cocos2d::CCObject*) = mac 0x19cd0;
	void disableButton(CreateMenuItem*) = mac 0x1c0f0, win 0x78af0;
	void editButtonUsable() = mac 0x28f30;
	void editObject(cocos2d::CCObject*) = mac 0x195a0, win 0x8ca50;
	void enableButton(CreateMenuItem*) = mac 0x1bff0, win 0x78990;
	CCMenuItemSpriteExtra* getCreateBtn(int, int) = mac 0x1f6c0, win 0x85120;
	cocos2d::CCPoint getGroupCenter(cocos2d::CCArray*, bool) = mac 0x23470, win 0x8fc30;
	cocos2d::CCArray* getSelectedObjects() = mac 0x23f30, win 0x86900;
	void init(LevelEditorLayer*) = mac 0x8ae0;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2ed60;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2f3d0;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2fb00;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x30790, win 0x91a30;
	void moveObject(GameObject*, cocos2d::CCPoint) = mac 0x24b10, win 0x8ddb0;
	void onDuplicate(cocos2d::CCObject*) = mac 0x18ba0, win 0x87d20;
	cocos2d::CCArray* pasteObjects(gd::string const&) = mac 0x232d0, win 0x88240;
	void playerTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2ebf0;
	void playtestStopped() = mac 0x24790;
	void redoLastAction(cocos2d::CCObject*) = mac 0xb8e0, win 0x870f0;
	void replaceGroupID(GameObject*, int, int) = mac 0x27470;
	void scaleChanged(float) = mac 0x25490;
	void scaleObjects(cocos2d::CCArray*, float, cocos2d::CCPoint) = mac 0x252e0, win 0x8f150;
	void selectObjects(cocos2d::CCArray*, bool) = mac 0x23940, win 0x864a0;
	void setupCreateMenu() = mac 0xcb50;
	void undoLastAction(cocos2d::CCObject*) = mac 0xb830, win 0x87070;
	void updateButtons() = mac 0x1a300, win 0x78280;
	void updateObjectInfoLabel() = mac 0x1cb10, win 0x793b0;
	void updateSlider() = mac 0x18a90, win 0x78f10;
	void updateZoom(float) = mac 0x248c0, win 0x878a0;
	void selectObject(GameObject* obj, bool idk) = mac 0x1bd60, win 0x86250;
	void selectAll() = win 0x86c40;
	void selectAllWithDirection(bool left) = win 0x86d80;
	cocos2d::CCPoint getTouchPoint(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) = win 0x90620;
	void onSelectBuildTab(cocos2d::CCObject* pSender) = win 0x887f0;
	void onCreateButton(cocos2d::CCObject* pSender) = win 0x854f0;
	CCMenuItemSpriteExtra* getSpriteButton(char const* sprite, cocos2d::SEL_MenuHandler callback, cocos2d::CCMenu* menu, float scale) = win 0x78bf0;
	cocos2d::CCPoint offsetForKey(int objID) = win 0x92310;
	void updateDeleteMenu() = win 0x7c5d0;
	void updateCreateMenu(bool updateTab) = mac 0x1e960, win 0x85530;
	void toggleMode(cocos2d::CCObject* pSender) = mac 0x187b0, win 0x7ad20;
	void zoomIn(cocos2d::CCObject* pSender) = mac 0xc0c0, win 0x877c0;
	void zoomOut(cocos2d::CCObject* pSender) = mac 0xc120, win 0x87830;
	void rotateObjects(cocos2d::CCArray* objects, float angle, cocos2d::CCPoint center) = win 0x8ee80;
	void updateGridNodeSize() = mac 0x1c8a0, win 0x78f60;
	void updateSpecialUIElements() = win 0x87030;
	void constrainGameLayerPosition(float x, float y) = mac 0x18890, win 0x8f920;
	void moveGameLayer(cocos2d::CCPoint const& pos) = mac 0x1ca90, win 0x79290;
	void showUI(bool show) = mac 0x245b0, win 0x87180;
	void editObject2(cocos2d::CCObject* pSender) = win 0x8d1b0;
	void editGroup(cocos2d::CCObject* pSender) = win 0x8d720;
	void moveObjectCall(EditCommand pSender) = mac 0x29b80, win 0x8db30;
	void transformObjectCall(EditCommand pSender) = mac 0x29d90, win 0x8def0;
	void onDeleteSelected(cocos2d::CCObject* pSender) = mac 0xb990, win 0x7bf50;
	void onCopy(cocos2d::CCObject* pSender) = mac 0x18dc0, win 0x87fb0;
	void onPaste(cocos2d::CCObject* pSender) = mac 0x18ee0, win 0x880c0;
	void toggleEnableRotate(cocos2d::CCObject* pSender) = mac 0xb700, win 0x860d0;
	void toggleFreeMove(cocos2d::CCObject* pSender) = mac 0xb610, win 0x85eb0;
	void toggleSwipe(cocos2d::CCObject* pSender) = mac 0xb490, win 0x85dd0;
	void toggleSnap(cocos2d::CCObject* pSender) = mac 0xb680, win 0x85fa0;
	void onPlayback(cocos2d::CCObject* pSender) = mac 0xbcb0, win 0x87340;
	void onPlaytest(cocos2d::CCObject* pSender) = mac 0xbec0, win 0x87600;
	void onStopPlaytest(cocos2d::CCObject* pSender) = mac 0xbfd0, win 0x876e0;
	void onGroupUp(cocos2d::CCObject* pSender) = mac 0x1a1a0, win 0x8d780;
	void onGroupDown(cocos2d::CCObject* pSender) = mac 0x1a200, win 0x8d7e0;
	void selectBuildTab(int tab) = mac 0x1fb90, win 0x88810;
	void onPause(cocos2d::CCObject* pSender) = mac 0x18650, win 0x78020;
	void onSettings(cocos2d::CCObject* pSender) = win 0x77fe0;
	void activateRotationControl(cocos2d::CCObject* pSender) = win 0x8fe70;
	void activateScaleControl(cocos2d::CCObject* pSender) = win 0x889b0;
	void dynamicGroupUpdate(bool idk) = win 0x8ad10;
	void createRockOutline() = win 0x89c10;
	void createRockEdges() = win 0x88ec0;
	void createRockBase() = win 0x8a2c0;
	void onCopyState(cocos2d::CCObject* pSender) = win 0x88490;
	void onPasteColor(cocos2d::CCObject* pSender) = win 0x88580;
	void onPasteState(cocos2d::CCObject* pSender) = win 0x884c0;
	void onGroupSticky(cocos2d::CCObject* pSender) = win 0x87a80;
	void onUngroupSticky(cocos2d::CCObject* pSender) = win 0x87ac0;
	void onGoToLayer(cocos2d::CCObject* pSender) = win 0x886b0;
	void onGoToBaseLayer(cocos2d::CCObject* pSender) = win 0x88790;
	void editColor(cocos2d::CCObject* pSender) = win 0x8d3c0;
	void alignObjects(cocos2d::CCArray* objs, bool alignY) = win 0x8f320;
	virtual void keyUp(cocos2d::enumKeyCodes key) = mac 0x312b0, win 0x92180;

	EditButtonBar* m_buttonBar;
	PAD = mac 0x8, win 0x4;
	cocos2d::CCArray* m_hideableUIElementArray;
	PAD = mac 0x8, win 0x4;
	float m_gridSize;
	PAD = mac 0x14, win 0x14;
	bool m_moveModifier;
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
	bool m_freeMoveEnabled;
	PAD = mac 0x12, win 0xc;
	cocos2d::CCArray* m_unknownArray2;
	PAD = mac 0x16, win 0x8;
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
	CCMenuItemSpriteExtra* m_PlaytestBtn;
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
	PAD = mac 0x16, win 0x8;
	int m_selectedCreateObjectID;
	cocos2d::CCArray* m_createButtonArray;
	cocos2d::CCArray* m_customObjectButtonArray;
	cocos2d::CCArray* m_unknownArray9;
	int m_selectedMode;
	LevelEditorLayer* m_editorLayer;
	cocos2d::CCPoint m_swipeStart;
	cocos2d::CCPoint m_swipeEnd;
	PAD = mac 0x40, win 0x20;
	GameObject* m_selectedObject;
	PAD = mac 0x16, win 0x8;
	gd::string m_clipboard;
	PAD = mac 0x16, win 0x8;
	int m_selectedTab;
	int m_timesSelected;
	PAD = win 0x20;
	bool m_spaceKeyPressed;
}

class EffectGameObject : GameObject {
	void updateLabel() {
		auto label = reinterpret_cast<cocos2d::CCLabelBMFont*>(this->getChildByTag(999));
		if (label) {
			switch (this->m_objectID) {
				// instant count
				case 0x713:
					label->setString(
						cocos2d::CCString::createWithFormat("%i", this->m_targetItemID)->getCString()
					);
					break;
				//   color,    pulse
				case 899: case 1006: {
					int target = this->m_objectID == 1006 ? m_targetGroupID : m_targetColorID;
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
						cocos2d::CCString::createWithFormat("%i", this->m_targetGroupID)->getCString()
					);
			}
		}
	}

	static EffectGameObject* create(char const*) = mac 0xc9790, win 0x253c30;
	void getTargetColorIndex() = mac 0xca1f0;
	virtual void triggerObject(GJBaseGameLayer*) = mac 0xc9870, win 0x253d60;
	gd::string getSaveString() = win 0x257560;

	int targetGroup = mac 0x4F8;
	bool activateGroup = mac 0x578;
	bool touchHoldMode = mac 0x579;
	int animationID = mac 0x584;
	float spawnDelay = mac 0x588;
	bool multiTrigger = mac 0x594;
	int targetCount = mac 0x598;
	int compareType = mac 0x5A0;
	int itemBlockBID = mac 0x5A8;
	int itemBlockID = mac 0x5B0;
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
	cocos2d::ccHSVValue m_HSVValue;
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
	int m_targetItemID;
	int m_pickupMode;
	PAD = win 0x24;
}

class EndLevelLayer {
	static EndLevelLayer* create() = mac 0x2787d0;
}

class EndPortalObject : GameObject {
	static EndPortalObject* create() = mac 0x1da8f0;
	void updateColors(cocos2d::ccColor3B) = mac 0x1dacb0;
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

	bool init(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float) = mac 0x25e1b0;
	static FLAlertLayer* create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*) = mac 0x25de00, win 0x22680;
	static FLAlertLayer* create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float) = mac 0x25e0e0, win 0x22730, ios 0x1fe374;
	static FLAlertLayer* create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float) = mac 0x25dec0, win 0x227e0;
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
	static GJAccountManager* sharedState() = win 0x107d50;

	PAD = win 0x4;
	gd::string m_password;
	gd::string m_username;
	int m_accountID;
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
	    return this->m_objects;
	}

	static GJBaseGameLayer* get() {
		return PlayLayer::get() ? static_cast<GJBaseGameLayer*>(PlayLayer::get()) : static_cast<GJBaseGameLayer*>(LevelEditorLayer::get());
	}

	virtual void objectsCollided(int, int) = mac 0xb6d90;
	virtual void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int) = mac 0xb73a0;
	virtual void updateColor(cocos2d::ccColor3B, float, int, bool, float, cocos2d::ccHSVValue, int, bool, int, EffectGameObject*) = mac 0xb7420;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0xba990;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0xba9a0;
	virtual void toggleGroupTriggered(int, bool) = mac 0xb75a0, win 0x10f890;
	virtual void spawnGroup(int) = mac 0xb7050, win 0x10f400;
	virtual void addToSection(GameObject*) = mac 0xb7b70;
	virtual void addToGroup(GameObject*, int, bool) = mac 0xb77f0;
	virtual void removeFromGroup(GameObject*, int) = mac 0xb7a60;
	virtual bool init() = mac 0xafc90;
	void addObjectCounter(LabelGameObject*, int) = mac 0xb9eb0;
	void addToGroups(GameObject*, bool) = mac 0xb7780;
	void atlasValue(int) = mac 0xb21e0;
	void bumpPlayer(PlayerObject*, GameObject*) = mac 0xb6860;
	void calculateOpacityValues(EffectGameObject*, EffectGameObject*, float, GJEffectManager*) = mac 0xb5be0;
	void checkSpawnObjects() = mac 0xb6f90;
	void collectItem(int, int) = mac 0xb9e20;
	void collectedObject(EffectGameObject*) = mac 0xb9b60;
	void createTextLayers() = mac 0xb5260;
	cocos2d::CCArray* damagingObjectsInRect(cocos2d::CCRect) = mac 0xb6140;
	void enableHighCapacityMode() = mac 0xb11e0;
	void getCapacityString() = mac 0xb2210, win 0x10c9b0;
	void getGroundHeightForMode(int) = mac 0xb6630;
	void getGroup(int) = mac 0xb6f20;
	void getMoveDeltaForObjects(int, int) = mac 0xb6db0;
	void getOptimizedGroup(int) = mac 0xb7940;
	void getStaticGroup(int) = mac 0xb79a0;
	void isGroupDisabledForObject(GameObject*) = mac 0xb5cc0;
	void isGroupDisabledForObjectFull(GameObject*, cocos2d::CCArray*) = mac 0xb5de0;
	void loadUpToPosition(float) = mac 0xba680;
	void objectIntersectsCircle(GameObject*, GameObject*) = mac 0xb66e0;
	void objectTriggered(EffectGameObject*) = mac 0xb71b0;
	void optimizeMoveGroups() = mac 0xb96c0;
	void parentForZLayer(int, bool, int) = mac 0xb55d0;
	void playerTouchedRing(PlayerObject*, GameObject*) = mac 0xb69e0;
	void processColorObject(EffectGameObject*, int, cocos2d::CCDictionary*, float, GJEffectManager*) = mac 0xb5a90;
	void processFollowActions() = mac 0xb8fd0;
	void processMoveActions() = mac 0xb86c0;
	void processMoveActionsStep(float) = mac 0xb7ea0;
	void processOpacityObject(EffectGameObject*, cocos2d::CCDictionary*, float, GJEffectManager*) = mac 0xb5ae0;
	void processPlayerFollowActions(float) = mac 0xb8b50;
	void processRotationActions() = mac 0xb7fd0;
	void pushButton(int, bool) = mac 0xb9920, win 0x111500;
	void rectIntersectsCircle(cocos2d::CCRect, cocos2d::CCPoint, float) = mac 0xb6470;
	void refreshCounterLabels() = mac 0xb9fc0;
	void releaseButton(int, bool) = mac 0xb9a00, win 0x111660;
	void removeFromGroups(GameObject*) = mac 0xb7a00;
	void removeObjectFromSection(GameObject*) = mac 0xb7e00;
	void reorderObjectSection(GameObject*) = mac 0xb7cb0;
	void resetGroupCounters(bool) = mac 0xba300;
	void resetMoveOptimizedValue() = mac 0xb9670;
	int sectionForPos(float) = mac 0xb6120;
	void setupLayers() = mac 0xaffe0;
	void shouldExitHackedLevel() = mac 0xb1100;
	void spawnGroupTriggered(int, float, int) = mac 0xb7020;
	cocos2d::CCArray* staticObjectsInRect(cocos2d::CCRect) = mac 0xb5f90;
	void testInstantCountTrigger(int, int, int, bool, int) = mac 0xb9ae0;
	void toggleGroup(int, bool) = mac 0xb75f0;
	void togglePlayerVisibility(bool) = mac 0xba910;
	void triggerMoveCommand(EffectGameObject*) = mac 0xb7290;
	void updateCollisionBlocks() = mac 0xb6a30;
	void updateCounters(int, int) = mac 0xb9bc0;
	void updateDisabledObjectsLastPos(cocos2d::CCArray*) = mac 0xb95b0;
	void updateLayerCapacity(gd::string) = mac 0xb1680;
	void updateLegacyLayerCapacity(int, int, int, int) = mac 0xb1590;
	void updateOBB2(cocos2d::CCRect) = mac 0xb63f0;
	void updateQueuedLabels() = mac 0xb9f30;
	~GJBaseGameLayer() = mac 0xaf990;

	// GJEffectManager* effectManager = mac 0x180;
	// cocos2d::CCLayer* objectLayer = mac 0x188;
	// cocos2d::CCArray* objects = mac 0x3a0;
	// cocos2d::CCArray* sections = mac 0x3a8;
	// PlayerObject* player1 = mac 0x380;
	// PlayerObject* player2 = mac 0x388;
	// LevelSettingsObject* levelSettings = mac 0x390;
	// cocos2d::CCDictionary* unknownDict = mac 0x398;
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
	cocos2d::CCArray* m_unk248;
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
	int m_unk2AC;
	bool m_activeDualTouch;
	int m_attemptClickCount;
	int m_currentSection;
	int m_oldSection;
	bool m_objectsAreDisabled;
	bool m_blending;
	PAD = mac 0x16, win 0x8;
}

class GJChallengeDelegate {}

class GJChallengeItem : cocos2d::CCObject {

	GJChallengeType m_challengeType;
	int m_countSeed;
	int m_countRand;
	int m_count;
	int m_rewardSeed;
	int m_rewardRand;
	int m_reward;
	int m_goalSeed;
	int m_goalRand;
	int m_goal;
	int m_timeLeft;
	bool m_canClaim;
	int m_position;
	gd::string m_name;
}

class GJColorSetupLayer {}

class GJComment : cocos2d::CCNode {
	~GJComment() = mac 0x2dfd70;
	virtual bool init() = mac 0x2dfec0;
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
	static GJCommentListLayer* create(BoomListView*, char const*, cocos2d::ccColor4B, float, float, bool) = mac 0x147d00;

	BoomListView* m_list;
}

class GJDailyLevelDelegate {}

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

	static GJDropDownLayer* create(char const* title, float height) {
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
	bool init(char const* title, float height) = mac 0x352100, win 0x113530, ios 0x297afc;
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
		m_unknown = nullptr;
	}

	cocos2d::CCPoint m_endPosition;
	cocos2d::CCPoint m_startPosition;
	cocos2d::CCMenu* m_buttonMenu;
	GJListLayer* m_listLayer;
	bool m_controllerEnabled;
	cocos2d::CCLayer* m_mainLayer;
	bool m_hidden;
	void* m_unknown;
}

class GJDropDownLayerDelegate {
	virtual void dropDownLayerWillClose(GJDropDownLayer*) {}
}

class GJEffectManager : cocos2d::CCNode {
	virtual bool init() = mac 0x180230;
	void activeColorForIndex(int) = mac 0x180cb0;
	void activeOpacityForIndex(int) = mac 0x180e10;
	void addAllInheritedColorActions(cocos2d::CCArray*) = mac 0x1817a0;
	void addGroupPulseEffect(PulseEffectAction*) = mac 0x184c10;
	void calculateBaseActiveColors() = mac 0x180f70;
	void calculateInheritedColor(int, ColorAction*) = mac 0x1818f0;
	void calculateLightBGColor(cocos2d::ccColor3B) = mac 0x185b90;
	void colorActionChanged(ColorAction*) = mac 0x181dc0;
	void colorExists(int) = mac 0x181da0;
	void colorForEffect(cocos2d::ccColor3B, cocos2d::ccHSVValue) = mac 0x182650;
	cocos2d::ccColor3B colorForGroupID(int, cocos2d::ccColor3B const&, bool) = mac 0x184f90;
	void colorForIndex(int) = mac 0x180ad0;
	void colorForPulseEffect(cocos2d::ccColor3B const&, PulseEffectAction*) = mac 0x181bf0;
	void countChangedForItem(int) = mac 0x185a40;
	void countForItem(int) = mac 0x185a10;
	static GJEffectManager* create() = mac 0x1800f0;
	void createFollowCommand(float, float, float, int, int, bool, int) = mac 0x182ed0;
	void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int) = mac 0x182cc0;
	void createPlayerFollowCommand(float, float, int, float, float, int, int) = mac 0x182fe0;
	void createRotateCommand(int, float, int, int, int, float, bool, int) = mac 0x182df0;
	void getAllColorActions() = mac 0x180980;
	void getAllColorSprites() = mac 0x1809e0;
	cocos2d::ccColor3B const& getColorAction(int) = mac 0x180b00, win 0x11cde0;
	cocos2d::ccColor3B const& getColorSprite(int) = mac 0x180d00;
	void getCurrentStateString() = mac 0x1867e0;
	void getLoadedMoveOffset() = mac 0x184390;
	void getMixedColor(cocos2d::ccColor3B, cocos2d::ccColor3B, float) = mac 0x185d30;
	uint8_t getOpacityActionForGroup(int) = mac 0x1845b0;
	void getSaveString() = mac 0x185e90;
	void handleObjectCollision(bool, int, int) = mac 0x1828f0;
	void hasActiveDualTouch() = mac 0x185540;
	void hasBeenTriggered(int) = mac 0x1853b0;
	void hasPulseEffectForGroupID(int) = mac 0x184f60;
	bool isGroupEnabled(int) = mac 0x1853d0;
	void keyForGroupIDColor(int, cocos2d::ccColor3B const&, bool) = mac 0x184c90;
	void loadState(gd::string) = mac 0x188db0;
	void objectsCollided(int, int) = mac 0x182a00;
	void opacityForIndex(int) = mac 0x180c80;
	float opacityModForGroup(int) = mac 0x184740;
	void playerButton(bool, bool) = mac 0x1855a0;
	void playerDied() = mac 0x185860;
	void postCollisionCheck() = mac 0x182720;
	void preCollisionCheck() = mac 0x182680;
	void prepareMoveActions(float, bool) = mac 0x183660;
	void processColors() = mac 0x180e70;
	void processCopyColorPulseActions() = mac 0x181530;
	void processInheritedColors() = mac 0x181190;
	void processPulseActions() = mac 0x181040;
	void registerCollisionTrigger(int, int, int, bool, bool, int) = mac 0x182b70;
	void removeAllPulseActions() = mac 0x1825e0;
	void removeColorAction(int) = mac 0x181d60;
	void reset() = mac 0x180690;
	void resetColorCache() = mac 0x185280;
	void resetEffects() = mac 0x1807d0;
	void resetMoveActions() = mac 0x180940;
	void resetToggledGroups() = mac 0x1853f0;
	void resetTriggeredIDs() = mac 0x182630;
	void runCountTrigger(int, int, bool, int, bool, int) = mac 0x1858d0;
	void runDeathTrigger(int, bool, int) = mac 0x1857a0;
	OpacityEffectAction* runOpacityActionOnGroup(int, float, float, int) = mac 0x1845d0;
	void runPulseEffect(int, bool, float, float, float, PulseEffectType, cocos2d::ccColor3B, cocos2d::ccHSVValue, int, bool, bool, bool, int) = mac 0x184890;
	void runTouchTriggerCommand(int, bool, TouchTriggerType, bool, int) = mac 0x185460;
	void setColorAction(ColorAction*, int) = mac 0x181d00;
	void setFollowing(int, int, bool) = mac 0x185e00;
	void setupFromString(gd::string) = mac 0x186290;
	void shouldBlend(int) = mac 0x180e40;
	void spawnGroup(int, float, int) = mac 0x1852a0;
	void stopActionsForTrigger(EffectGameObject*) = mac 0x183150;
	void stopMoveActionsForGroup(int) = mac 0x1830e0;
	void storeTriggeredID(int) = mac 0x185380;
	void toggleGroup(int, bool) = mac 0x182c80;
	void traverseInheritanceChain(InheritanceNode*) = mac 0x181850;
	void updateActiveOpacityEffects() = mac 0x1847e0;
	void updateColorAction(ColorAction*) = mac 0x184560;
	void updateColorEffects(float) = mac 0x181f40;
	void updateColors(cocos2d::ccColor3B, cocos2d::ccColor3B) = mac 0x180a40;
	void updateEffects(float) = mac 0x181df0;
	void updateOpacityAction(OpacityEffectAction*) = mac 0x184780;
	void updateOpacityEffects(float) = mac 0x1823e0, win 0x11e490;
	void updatePulseEffects(float) = mac 0x182130;
	void updateSpawnTriggers(float) = mac 0x182510;
	void wasFollowing(int, int) = mac 0x185e60;
	void wouldCreateLoop(InheritanceNode*, int) = mac 0x181820;
	~GJEffectManager() = mac 0x17fe00;

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
	gd::vector<InheritanceNode*> m_inheritanceNodesForGroup;
	cocos2d::CCDictionary* m_f01a8;
	cocos2d::CCDictionary* m_collisionActionsForGroup1;
	cocos2d::CCDictionary* m_collisionActionsForGroup2;
	gd::vector<ColorAction*> m_colorActionsForGroup;
	gd::vector<ColorActionSprite*> m_colorSpritesForGroup;
	bool m_pulseExistsForGroup[1100];
	bool m_f063c;
	bool m_opactiyExistsForGroup[1100];
	int m_itemValues[1100];
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
	cocos2d::CCArray* m_f1c28;
	cocos2d::CCNode* m_f1c30;
	cocos2d::CCDictionary* m_f1c38;
	cocos2d::CCDictionary* m_f1c40;
	cocos2d::CCDictionary* m_f1c48;
	cocos2d::CCDictionary* m_f1c50;
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

	~GJGameLevel() = mac 0x2db2d0;
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x2dd1c0;
	virtual bool canEncode() = mac 0x2ddae0;
	virtual bool init() = mac 0x2db310;
	static GJGameLevel* create() = mac 0x2b83e0, win 0xbd2b0, ios 0x51fe8;
	gd::string getAudioFileName() = mac 0x2dbe70, win 0xbdc70;
	void getCoinKey(int) = mac 0x2ce360;
	void getLengthKey(int) = mac 0x2dbba0;
	void getNormalPercent() = mac 0x2b8b20;
	void levelWasAltered() = mac 0x2db530, win 0xbd550;
	void savePercentage(int, bool, int, int, bool) = mac 0x2db700;
	void dataLoaded(DS_Dictionary* dict) = mac 0x2922f0, win 0xbded0, ios 0x6fca4;
	GJDifficulty getAverageDifficulty() = win 0xbd9b0;
	gd::string getUnpackedLevelDescription() = win 0xbf890;

	int levelId = mac 0x130;
	gd::string name = mac 0x138;
	gd::string levelString = mac 0x148;
	gd::string author = mac 0x150;
	int audioTrack = mac 0x18c;
	int songID = mac 0x190;
	int difficultyDenominator = mac 0x1ac;
	int difficultyNumerator = mac 0x1b0;
	bool lowDetail = mac 0x1c5;
	int bestNormal = mac 0x214;
	int bestPractice = mac 0x238;
	int score = mac 0x248;
	int epic = mac 0x24c;
	int demon = mac 0x26c;
	int stars = mac 0x27c;
	OBB2D* hitbox = mac 0x2b0;
	bool official = mac 0x324;
	cocos2d::CCDictionary* m_lastBuildSave;
	int m_levelIDRand;
	int m_levelIDSeed;
	int m_levelID;
	gd::string m_levelName;
	gd::string m_levelDesc;
	gd::string m_levelString;
	gd::string m_creatorName;
	gd::string m_recordString;
	gd::string m_uploadDate;
	gd::string m_updateDate;
	int m_userIDRand;
	int m_userIDSeed;
	int m_userID;
	int m_accountIDRand;
	int m_accountIDSeed;
	int m_accountID;
	GJDifficulty m_difficulty;
	int m_audioTrack;
	int m_songID;
	int m_levelRev;
	bool m_unlisted;
	int m_objectCountRand;
	int m_objectCountSeed;
	int m_objectCount;
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
	int m_isVerifiedRand;
	int m_isVerifiedSeed;
	bool m_isVerified;
	bool m_isUploaded;
	bool m_hasBeenModified;
	int m_levelVersion;
	int m_gameVersion;
	int m_attemptsRand;
	int m_attemptsSeed;
	int m_attempts;
	int m_jumpsRand;
	int m_jumpsSeed;
	int m_jumps;
	int m_clicksRand;
	int m_clicksSeed;
	int m_clicks;
	int m_attemptTimeRand;
	int m_attemptTimeSeed;
	int m_attemptTime;
	int m_chk;
	bool m_isChkValid;
	bool m_isCompletionLegitimate;
	int m_normalPercent;
	int m_normalPercentSeed;
	int m_normalPercentRand;
	int m_orbCompletionRand;
	int m_orbCompletionSeed;
	int m_orbCompletion;
	int m_newNormalPercent2Rand;
	int m_newNormalPercent2Seed;
	int m_newNormalPercent2;
	int m_practicePercent;
	int m_likes;
	int m_dislikes;
	int m_levelLength;
	int m_featured;
	bool m_isEpic;
	bool m_levelFavorited;
	int m_levelFolder;
	int m_dailyIDRand;
	int m_dailyIDSeed;
	int m_dailyID;
	int m_demonRand;
	int m_demonSeed;
	int m_demon;
	int m_demonDifficulty;
	int m_starsRand;
	int m_starsSeed;
	int m_stars;
	bool m_autoLevel;
	int m_coins;
	int m_coinsVerifiedRand;
	int m_coinsVerifiedSeed;
	int m_coinsVerified;
	int m_passwordRand;
	int m_passwordSeed;
	int m_originalLevelRand;
	int m_originalLevelSeed;
	int m_originalLevel;
	bool m_twoPlayerMode;
	int m_failedPasswordAttempts;
	int m_firstCoinVerifiedRand;
	int m_firstCoinVerifiedSeed;
	int m_firstCoinVerified;
	int m_secondCoinVerifiedRand;
	int m_secondCoinVerifiedSeed;
	int m_secondCoinVerified;
	int m_thirdCoinVerifiedRand;
	int m_thirdCoinVerifiedSeed;
	int m_thirdCoinVerified;
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
		m_unkSprite0x140 = nullptr;
		m_unkSprite0x144 = nullptr;
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
	static GJGarageLayer* create() = win 0x125220;
	virtual bool init() = mac 0x1b4980, win 0x1255d0;
	void onSelectTab(cocos2d::CCObject* pSender) = win 0x127c30;
	void onPlayerIcon(cocos2d::CCObject* pSender) = win 0x127f30;
	void onShipIcon(cocos2d::CCObject* pSender) = win 0x1281e0;
	void onBallIcon(cocos2d::CCObject* pSender) = win 0x1282a0;
	void onBirdIcon(cocos2d::CCObject* pSender) = win 0x128360;
	void onDartIcon(cocos2d::CCObject* pSender) = win 0x128420;
	void onRobotIcon(cocos2d::CCObject* pSender) = win 0x1286d0;
	void onSpiderIcon(cocos2d::CCObject* pSender) = win 0x128890;
	void onShards(cocos2d::CCObject* pSender) = win 0x12ad70;
	void onBack(cocos2d::CCObject* pSender) = win 0x12adf0;
	void onShop(cocos2d::CCObject* pSender) = win 0x12ad90;

	CCTextInputNode* m_nameInput;
	SimplePlayer* m_playerPreview;
	PAD = win 0x8;
	cocos2d::CCSprite* m_unkSprite0x140;
	cocos2d::CCSprite* m_unkSprite0x144;
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

class GJGroundLayer {
	void updateGroundWidth() = win 0x12dda0;
}

class GJItemIcon {
	GJItemIcon* createBrowserIcon(UnlockType _type, int _id) {
	    return GJItemIcon::create(_type, _id,
	        { 0xaf, 0xaf, 0xaf }, { 0xff, 0xff, 0xff },
	        false, true, true,
	        { 0xff, 0xff, 0xff }
	    );
	}

	static GJItemIcon* create(UnlockType _type, int _id, cocos2d::ccColor3B _col1, cocos2d::ccColor3B _col2, bool _un0, bool _un1, bool _un2, cocos2d::ccColor3B _col3) = mac 0x1bb330, win 0x12cbf0, ios 0x227188;
}

class GJLevelScoreCell : TableViewCell {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
}

class GJListLayer : cocos2d::CCLayerColor {
	~GJListLayer() = mac 0x344350;
	static GJListLayer* create(cocos2d::CCObject* target, char const* title, cocos2d::ccColor4B color, float width, float height) = mac 0x343e10, win 0x12e000;

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
}

class GJMessageCell : TableViewCell {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
}

class GJRequestCell : TableViewCell {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
}

class GJRewardDelegate {}

class GJRewardObject : cocos2d::CCObject {
	SpecialRewardItem m_specialRewardItem;
	UnlockType m_unlockType;
	int m_itemID;
	int m_total;
}

class GJRobotSprite : CCAnimatedSprite {
	~GJRobotSprite() = mac 0x34aaf0;
	virtual bool init() = mac 0x34ad50;
	virtual void setOpacity(unsigned char) = mac 0x34bcc0;
	virtual void hideSecondary() = mac 0x34c3b0;
	static GJRobotSprite* create() = mac 0x34ac00;
	void updateColor02(cocos2d::ccColor3B) = mac 0x34bbd0;
	void updateFrame(int) = mac 0x34bdd0;
	void hideGlow() = mac 0x34b860;

	PAD = win 0x8;
	cocos2d::ccColor3B m_secondaryColor;
}

class GJRotationControl : cocos2d::CCLayer {
	void setAngle(float angle) {
	    this->m_sliderPosition = cocos2d::CCPointMake(sinf(angle) * 60.0f, cosf(angle) * 60.0f);
	    this->m_angle = angle;
	
	    this->m_sliderThumb->setPosition(this->m_sliderPosition);
	}

	void updateSliderPosition(cocos2d::CCPoint const& pos) = win 0x94020;

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
	void updateLabel(float value) = win 0x94990;
	void loadValues(GameObject* obj, cocos2d::CCArray* objs) = win 0x94590;

	Slider* m_slider;
	unsigned int m_touchID;
	float m_value;
	PAD = win 0x4;
	cocos2d::CCLabelBMFont* m_label;
	GJScaleControlDelegate* m_delegate;
}

class GJScaleControlDelegate {
	virtual void scaleChangeBegin() {}
	virtual void scaleChangeEnded() {}
	virtual void scaleChanged(float) {}
}

class GJScoreCell : TableViewCell {
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = win 0x624a0;
	void loadFromScore(GJUserScore* score) = win 0x61440;
	void onViewProfile(cocos2d::CCObject* pSender) = win 0x62380;
	void updateBGColor(unsigned int index) = win 0x5c6b0;
}

class GJSearchObject : cocos2d::CCNode {
	SearchType getType() {
	    return this->m_searchType;
	}

	static GJSearchObject* create(SearchType nID) = win 0xc2b90;

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
	static char const* textForColorIdx(int id) = mac 0x383a50, win 0x14e1d0, ios 0x113f6c;
}

class GJSpecialColorSelectDelegate {
	virtual void colorSelectClosed(GJSpecialColorSelect*, int) {}
}

class GJSpiderSprite : GJRobotSprite {
	~GJSpiderSprite() = mac 0x34c4a0;
	virtual bool init() = mac 0x34c700;
	static GJSpiderSprite* create() = mac 0x34c5b0;
}

class GJSpriteColor : cocos2d::CCNode {
	int m_colorID;
	int m_defaultColorID;
	float m_unk_0F4;
	cocos2d::ccHSVValue m_hsv;
	bool unk_108;
	float unk_10C;
	bool unk_110;
}

class GJUserCell : TableViewCell {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
}

class GJUserScore : cocos2d::CCNode {
	IconType getIconType() const { 
	    return this->m_iconType; 
	}
	int getPlayerCube() const { 
	    return this->m_playerCube; 
	}
	int getPlayerShip() const { 
	    return this->m_playerShip;
	}
	int getPlayerBall() const {
	    return this->m_playerBall;
	}
	int getPlayerUfo() const {
	    return this->m_playerUfo;
	}
	int getPlayerWave() const { 
	    return this->m_playerWave; 
	}
	int getPlayerRobot() const { 
	    return this->m_playerRobot; 
	}
	int getPlayerSpider() const { 
	    return this->m_playerSpider; 
	}
	int getPlayerStreak() const { 
	    return this->m_playerStreak; 
	}
	bool getGlowEnabled() const { 
	    return this->m_glowEnabled; 
	}
	int getPlayerExplosion() const { 
	    return this->m_playerExplosion; 
	}
	int getPlayerColor1() const { 
	    return this->m_color1;
	}
	int getPlayerColor2() const { 
	    return this->m_color2; 
	}
	gd::string getPlayerName() const { 
	    return this->m_userName; 
	}
	static GJUserScore* create() = win 0xc1660;
	static GJUserScore* create(cocos2d::CCDictionary*) = win 0xc0750;

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
	virtual void setup() {}
	void encodeDataTo(DS_Dictionary* data) {}
	void dataLoaded(DS_Dictionary* data) {}
	void firstLoad() {}

	void save() = mac 0x26f300, win 0x29250;
	void saveData(DS_Dictionary*, gd::string) = mac 0x26f4b0;
	void saveGMTo(gd::string) = mac 0x26f3b0;

	gd::string m_fileName;
	bool m_setup;
	bool m_saved;
	bool m_quickSave;
}

class GooglePlayDelegate {
	virtual void googlePlaySignedIn() {}
}

class GameLevelManager : cocos2d::CCNode {
	cocos2d::CCArray* createAndGetScores(gd::string, GJScoreType) = win 0xa2780;
	GJGameLevel* createNewLevel() = mac 0x2b8180, win 0xa0db0;
	static GameLevelManager* sharedState() = mac 0x2a8340, win 0x9f860;
	void getCompletedLevels(bool newFilter) = win 0xa2d20;
	void getGJUserInfo(int) = win 0xb00b0;
	void getOnlineLevels(GJSearchObject*) = win 0xa7bc0;
	void getPageInfo(char const*) = mac 0x2c0050;
	cocos2d::CCArray* getSavedLevels(bool favorite, int levelFolder) = win 0xa2960;
	cocos2d::CCArray* getStoredOnlineLevels(char const*) = mac 0x2bfe80, win 0xa3a90;
	void getTopArtists(int, int) = mac 0x2ce3d0;
	void getTopArtistsKey(int) = mac 0x2ce7a0;
	void makeTimeStamp(char const*) = mac 0x2bfd90;
	GJGameLevel* getMainLevel(int id, bool unk) = win 0xa0940;
	cocos2d::CCDictionary* responseToDict(gd::string response, bool comment) = win 0xbba50;
	void storeUserNames(gd::string) = win 0xa1840;
	gd::string userNameForUserID(int id) = win 0xa1c20;
	void updateUserScore() = win 0xada60;


	//cocos2d::CCDictionary* timerDict = mac 0x1e8;
	cocos2d::CCDictionary* m_mainLevels;
	cocos2d::CCDictionary* m_searchFilters;
	cocos2d::CCDictionary* m_onlineLevels;
	PAD = win 0x4;
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
	PAD = win 0x4;
	int m_weeklyTimeLeft;
	int m_weeklyID;
	int m_weeklyIDUnk;
	cocos2d::CCDictionary* m_gauntletLevels;
	cocos2d::CCDictionary* m_unkDict13;
	PAD = win 0x4;
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
	    m_playerFrameRand1 = id + m_playerFrameRand2;
	}
	void setPlayerShip(int id) {
	    m_playerShip = id;
	    m_playerShipRand1 = id + m_playerShipRand2;
	}
	void setPlayerBall(int id) {
	    m_playerBall = id;
	    m_playerBallRand1 = id + m_playerBallRand2;
	}
	void setPlayerBird(int id) {
	    m_playerBird = id;
	    m_playerBirdRand1 = id + m_playerBirdRand2;
	}
	void setPlayerDart(int id) {
	    m_playerDart = id;
	    m_playerDartRand1 = id + m_playerDartRand2;
	}
	void setPlayerRobot(int id) {
	    m_playerRobot = id;
	    m_playerRobotRand1 = id + m_playerRobotRand2;
	}
	void setPlayerSpider(int id) {
	    m_playerSpider = id;
	    m_playerSpiderRand1 = id + m_playerSpiderRand2;
	}
	void setPlayerStreak(int id) {
	    m_playerStreak = id;
	    m_playerStreakRand1 = id + m_playerStreakRand2;
	}
	void setPlayerDeathEffect(int id) {
	    m_playerDeathEffect = id;
	    m_playerDeathEffectRand1 = id + m_playerDeathEffectRand2;
	}
	void setPlayerColor(int id) {
	    m_playerColor = id;
	    m_playerColorRand1 = id + m_playerColorRand2;
	}
	void setPlayerColor2(int id) {
	    m_playerColor2 = id;
	    m_playerColor2Rand1 = id + m_playerColor2Rand2;
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
	bool getGameVariableDefault(char const* key, bool defaultValue) {
	    auto object = reinterpret_cast<cocos2d::CCString*>(m_valueKeeper->objectForKey(std::string("gv_") + key));
	    if (object == nullptr)
	        return defaultValue;
	    return object->boolValue();
	}
	int getIntGameVariableDefault(char const* key, int defaultValue) {
	    auto object = reinterpret_cast<cocos2d::CCString*>(m_valueKeeper->objectForKey(std::string("gv_") + key));
	    if (object == nullptr)
	        return defaultValue;
	    return object->intValue();
	}
	static GameManager* get() {
		return GameManager::sharedState();
	}

	void accountStatusChanged() = mac 0x1cdad0;
	cocos2d::ccColor3B colorForIdx(int) = mac 0x1cbc80, win 0xc8d10, ios 0x237488;
	void didExitPlayscene() = mac 0x1d0230;
	void doQuickSave() = mac 0x1d0200;
	void fadeInMusic(char const*) = mac 0x1c2ff0, win 0xc4bd0;
	void getBGTexture(int) = mac 0x1cca00;
	void getFontFile(int) = mac 0x1cc5f0;
	bool getGameVariable(char const*) = mac 0x1cccd0, win 0xc9d30;
	int getIntGameVariable(char const*) = mac 0x1cd1d0, win 0xca330;
	bool getUGV(char const*) = mac 0x1ccfa0, win 0xca0d0;
	void loadDeathEffect(int) = mac 0x1cc690;
	void loadFont(int) = mac 0x1cc550;
	void reloadAll(bool, bool, bool) = mac 0x1d08a0, win 0xce950;
	void reloadAllStep2() = mac 0x1d0940, win 0xce9e0, ios 0x23b1f4;
	void reloadAllStep5() = mac 0x1d0b00;
	void reportPercentageForLevel(int, int, bool) = mac 0x1c5b00;
	void setGameVariable(char const*, bool) = mac 0x1cca80, win 0xc9b50;
	void setIntGameVariable(char const*, int) = mac 0x1cd0e0, win 0xca230;
	void setUGV(char const*, bool) = mac 0x1cce50;
	static GameManager* sharedState() = mac 0x1c2b30, win 0xc4a50, ios 0x2321b8;
	~GameManager() = mac 0x1d0e00;
	void getGTexture(int) = mac 0x1cca40;
	virtual bool init() = mac 0x1c2ec0;
	void reportAchievementWithID(char const*, int, bool) = mac 0x1c6460;
	cocos2d::CCSize* resolutionForKey(cocos2d::CCSize*, int) = mac 0x1d0b40, win 0xceca0;
	void update(float) = mac 0x1d0270;
	bool isColorUnlocked(int _id, bool _type) = mac 0x1c3b90, win 0xc53f0;
	bool isIconUnlocked(int _id, IconType _type) = mac 0x1c35b0, win 0xc4fc0;
	void toggleGameVariable(char const* key) = win 0xc9e90;
	static void returnToLastScene(GJGameLevel* level) = win 0xce6a0;

	bool m_switchModes;
	bool m_toFullscreen;
	bool m_reloading;
	bool m_unknown0;
	cocos2d::CCDictionary* m_valueKeeper;
	cocos2d::CCDictionary* m_unlockValueKeeper;
	cocos2d::CCDictionary* m_customObjectDict;
	double m_adTimer;
	double m_adCache;
	PAD = mac 0x8, win 0x8;
	double m_unknownDouble;
	PAD = mac 0x8, win 0x8;
	bool m_loaded;
	gd::string m_unknownString;
	PlayLayer* m_playLayer;
	LevelEditorLayer* m_levelEditorLayer;
	int m_unknown1;
	MenuLayer* m_menuLayer;
	bool m_unknownBool2;
	int m_unknownInt2;
	bool m_unknownBool3;
	bool m_unknownPlayLayerBool;
	bool m_unknownBool4;
	bool m_unknownBool5;
	gd::string m_playerUDID;
	gd::string m_playerName;
	bool m_commentsEnabled;
	int m_playerUserIDRand1;
	int m_playerUserIDRand2;
	int m_playerUserID;
	float m_backgroundMusicVolume;
	float m_effectsVolume;
	float m_timeOffset;
	bool m_ratedGame;
	bool m_likedFacebook;
	bool m_followedTwitter;
	bool m_subbedYoutube;
	int m_unknownInt;
	double m_socialsDuration;
	bool m_showedAd;
	bool m_unknownBool;
	bool m_editorEnabled;
	int m_sceneEnum;
	int m_searchObjectType;
	bool m_unknownBool6;
	int m_playerFrameRand1;
	int m_playerFrameRand2;
	int m_playerFrame;
	int m_playerShipRand1;
	int m_playerShipRand2;
	int m_playerShip;
	int m_playerBallRand1;
	int m_playerBallRand2;
	int m_playerBall;
	int m_playerBirdRand1;
	int m_playerBirdRand2;
	int m_playerBird;
	int m_playerDartRand1;
	int m_playerDartRand2;
	int m_playerDart;
	int m_playerRobotRand1;
	int m_playerRobotRand2;
	int m_playerRobot;
	int m_playerSpiderRand1;
	int m_playerSpiderRand2;
	int m_playerSpider;
	int m_playerColorRand1;
	int m_playerColorRand2;
	int m_playerColor;
	int m_playerColor2Rand1;
	int m_playerColor2Rand2;
	int m_playerColor2;
	int m_playerStreakRand1;
	int m_playerStreakRand2;
	int m_playerStreak;
	int m_playerDeathEffectRand1;
	int m_playerDeathEffectRand2;
	int m_playerDeathEffect;
	int m_chkSeed;
	int m_chkRand;
	int m_secretNumberSeed;
	int m_secretNumberRand;
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
	PAD = mac 0x30, win 0x30;
	int m_bootups;
	bool m_hasRatedGame;
	bool m_unk0;
	bool m_unk1;
	bool m_unk2;
	bool m_gameCenterEnabled;
	bool m_smoothFix;
	PAD = mac 0x18, win 0x18;
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
	    return m_groups[ix];
	}
	short getGroupIDCount() {
	    return m_groupCount; 
	}
	gd::vector<short> getGroupIDs() {
	    std::vector<short> res;
	
	    if (m_groups && m_groups[0])
	        for (auto i = 0; i < m_groupCount; i++)
	            res.push_back(m_groups[i]);
	
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

	~GameObject() = mac 0x2f4ca0;
	virtual void update(float) = mac 0x2fbb90;
	virtual void setScaleX(float) = mac 0x335b00, win 0xe5050;
	virtual void setScaleY(float) = mac 0x335b90, win 0xe50e0;
	virtual void setScale(float) = mac 0x335c20, win 0xe5170;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x335850, win 0xe4de0;
	virtual void setVisible(bool) = mac 0x336280, win 0xe57c0;
	virtual void setRotation(float) = mac 0x335970, win 0xe4ed0;
	virtual void setOpacity(unsigned char) = mac 0x335f10, win 0xe53c0;
	virtual bool initWithTexture(cocos2d::CCTexture2D*) = mac 0x2f56a0, win 0xcfa80;
	virtual void setChildColor(cocos2d::ccColor3B const&) = mac 0x341f20, win 0xee900;
	virtual void setFlipX(bool) = mac 0x335a60;
	virtual void setFlipY(bool) = mac 0x335ab0;
	virtual void customSetup() = mac 0x2fbba0;
	virtual void setupCustomSprites() = mac 0x307f60, win 0xd7d50;
	virtual void addMainSpriteToParent(bool) = mac 0x33a5b0, win 0xeb250;
	virtual void resetObject() = mac 0x2fa620, win 0xd1470;
	virtual void triggerObject(GJBaseGameLayer*) = mac 0x2fa8f0;
	virtual void activateObject() = mac 0x2faf60, win 0xEF0E0;
	virtual void deactivateObject(bool) = mac 0x2fb8f0, win 0xd19b0;
	virtual cocos2d::CCRect* getObjectRect() = mac 0x3352b0, win 0xe4a40;
	virtual cocos2d::CCRect* getObjectRect(float, float) = mac 0x3352d0, win 0xe4a70;
	virtual cocos2d::CCRect* getObjectRect2(float, float) = mac 0x3354e0, win 0xe4b90;
	virtual cocos2d::CCRect* getObjectTextureRect() = mac 0x3355b0, win 0xe4c40;
	virtual void getRealPosition() = mac 0x335750, win 0xe4d90;
	virtual void setStartPos(cocos2d::CCPoint) = mac 0x2fa520, win 0xd1390;
	virtual void updateStartValues() = mac 0x2fa800, win 0xd1610;
	virtual void customObjectSetup() = mac 0xdc1a0;
	virtual gd::string getSaveString() = mac 0x33d3d0, win 0xed0c0;
	virtual bool isFlipX() = mac 0x335a40, win 0xe4fb0;
	virtual bool isFlipY() = mac 0x335a50, win 0xe4fc0;
	virtual void setRScaleX(float) = mac 0x335cb0, win 0xe5200;
	virtual void setRScaleY(float) = mac 0x335d60, win 0xe5240;
	virtual void setRScale(float) = mac 0x335e10, win 0xe5280;
	virtual void getRScaleX() = mac 0x335e50;
	virtual void getRScaleY() = mac 0x335e80, win 0xe5300;
	virtual void calculateSpawnXPos() = mac 0x336970, win 0xe5d20;
	virtual void triggerActivated(float) = mac 0x336990;
	virtual void powerOnObject() = mac 0x3369a0, win 0xe5d40;
	virtual void powerOffObject() = mac 0x3369c0, win 0xe5d60;
	virtual void setObjectColor(cocos2d::ccColor3B const&) = mac 0x341c90, win 0xee620;
	virtual void setGlowColor(cocos2d::ccColor3B const&) = mac 0x341ed0, win 0xee8c0;
	virtual void getOrientedBox() = mac 0x342ad0, win 0xef160;
	virtual void addToGroup(int) = mac 0x33ad00, win 0xeb8d0;
	virtual void removeFromGroup(int) = mac 0x33ada0, win 0xeb930;
	virtual void spawnXPosition() = mac 0xdc1b0;
	virtual void getObjectRectDirty() const = mac 0xdc1d0;
	virtual void setObjectRectDirty(bool) = mac 0xdc1e0;
	virtual void getOrientedRectDirty() const = mac 0xdc1f0;
	virtual void setOrientedRectDirty(bool) = mac 0xdc200;
	virtual GameObjectType getType() const = mac 0xdc210, win 0x989e0;
	virtual void setType(GameObjectType) = mac 0xdc220, win 0x989f0;
	virtual void getStartPos() const = mac 0xdc230, win 0x98a00;
	void activatedByPlayer(GameObject*) = mac 0x342a20;
	void addColorSprite() = mac 0x2f7fe0;
	void addColorSpriteToParent(bool) = mac 0x2fb470;
	void addToTempOffset(float, float) = mac 0x335700;
	void calculateOrientedBox() = mac 0x342b20;
	void canChangeCustomColor() = mac 0x342db0;
	void colorForMode(int, bool) = mac 0x343460;
	void commonSetup() = mac 0x2f5570, win 0xcfac0;
	void copyGroups(GameObject*) = mac 0x33ae30;
	static GameObject* createWithFrame(char const*) = mac 0x2f5490, win 0xcf8f0;
	static GameObject* createWithKey(int) = mac 0x2f4ce0, win 0xcf4f0;
	void destroyObject() = mac 0x336a00;
	void determineSlopeDirection() = mac 0x33a9e0;
	void getActiveColorForMode(int, bool) = mac 0x343860;
	void getBallFrame(int) = mac 0x341bf0;
	cocos2d::CCPoint getBoxOffset() = mac 0x3353d0, win 0xef350;
	cocos2d::ccColor3B const& getColorIndex() = mac 0x343b90;
	void getDidUpdateLastPosition() = mac 0x343a20;
	void getLastPosition() = mac 0x3439d0;
	void getMainColorMode() = mac 0x334c30;
	void getObjectZOrder() = mac 0x337d70;
	float getObjectRadius() = mac 0x343c10;
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
	void isSpawnableTrigger() = mac 0x343a60;
	void isSpecialObject() = mac 0x343c40;
	static GameObject* objectFromString(gd::string, bool) = mac 0x33b720, win 0xebe50;
	void playShineEffect() = mac 0x2fa9d0;
	void quickUpdatePosition() = mac 0x335790;
	void removeGlow() = mac 0x2f7f70;
	void resetGroupDisabled() = mac 0x2fa7e0;
	void saveActiveColors() = mac 0x33d250;
	void selectObject(const cocos2d::ccColor3B&) = mac 0x341f90, win 0xee960;
	void setDefaultMainColorMode(int) = mac 0x304fc0;
	void setDidUpdateLastPosition(bool const&) = mac 0x343a30;
	void setGlowOpacity(unsigned char) = mac 0x336200;
	void setLastPosition(cocos2d::CCPoint const&) = mac 0x3439e0;
	void setMainColorMode(int) = mac 0x342e70;
	void setSectionIdx(int const&) = mac 0x343a10;
	void setupCoinArt() = mac 0x337dd0;
	void slopeFloorTop() = mac 0x342800;
	void slopeWallLeft() = mac 0x3427e0;
	void updateCustomScale(float) = mac 0x335eb0, win 0xe5340;
	void updateMainColor() = mac 0x343340;
	void updateOrientedBox() = mac 0x342b50;
	void updateSecondaryColor() = mac 0x343740;
	void updateStartPos() = mac 0x2fa590;
	void updateState() = mac 0x3369e0;
	void updateSyncedAnimation(float) = mac 0x337f00;
	void deselectObject() = win 0xeee50;
	cocos2d::CCRepeatForever* createRotateAction(float f, int n) = win 0xe49b0;
	void setMyAction(cocos2d::CCAction* pAction) = win 0xd1b90;

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
	bool m_blackChild;
	bool m_unkOutlineMaybe;
	float m_blackChildOpacity;
	bool field_21C;
	bool m_editor;
	bool m_groupDisabled;
	bool m_colourOnTop;
	//GJSpriteColor* m_mainColourMode;
	//GJSpriteColor* m_secondaryColourMode;
	//bool m_col1;
	//bool m_col2;
	float m_unknown27c;
	float m_unknown280;
	float m_unknown284;
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
	int m_linkedGroup;
	bool m_isSaw;
	int m_customRotateSpeed;
	bool m_sawIsDisabled;
	PAD = mac 0x4, win 0x4;
	cocos2d::CCSprite* m_detailSprite;
	PAD = mac 0x8, win 0x8;
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
	PAD = mac 0x4, win 0x4;
	bool m_unk368;
	bool m_unk369;
	bool m_unk36A;
	bool m_isDontEnter;
	bool m_isDontFade;
	int m_defaultZOrder;
	bool m_useSecondSheet;
	bool m_unknown3d9;

	//PAD = mac 0x17, win 0x17;
	bool m_isPortal;
	bool m_lockColourAsChild;
	bool m_customAudioScale;
	int m_minAudioScale;
	int m_maxAudioScale;
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
	float m_unkWin18C;
	GJSpriteColor* m_baseColor;
	GJSpriteColor* m_detailColor;
	int m_unknown420;																																																				
	ZLayer m_defaultZLayer;
	ZLayer m_zLayer;
	int m_gameZOrder;
	gd::string m_unk3C0;
	bool m_showGamemodeBorders;
	bool m_unk3D9;
	bool m_isSelected;
	int m_globalClickCounter;
	PAD = mac 0x8, win 0x8;
	bool m_unknownLayerRelated;
	float m_multiScaleMultiplier;
	bool m_isGroupParent;
	short* m_groups;
	short m_groupCount;
	PAD = mac 0x22, win 0x12;
	int m_editorLayer;
	int m_editorLayer2;
	int m_unk414;
	PAD = mac 0xc, win 0xc;
	cocos2d::CCPoint m_firstPosition;
	PAD = mac 0x1c, win 0x1c;
	bool m_highDetail;
	ColorActionSprite* m_colorActionSprite1;
	ColorActionSprite* m_colorActionSprite2;
	GJEffectManager* m_effectManager;
	PAD = mac 0x10, win 0x10;
}

class GameObjectCopy : cocos2d::CCObject {
	virtual ~GameObjectCopy() = mac 0xa3290;
	static GameObjectCopy* create(GameObject*) = mac 0x975a0;
	void resetObject() = mac 0x976a0;
}

class GameRateDelegate {}

class GameSoundManager : cocos2d::CCNode {
	void disableMetering() = mac 0x362d80;
	void enableMetering() = mac 0x362d00;
	void getMeteringValue() = mac 0x362db0;
	void playBackgroundMusic(gd::string, bool, bool) = mac 0x362070;
	void playEffect(gd::string, float, float, float) = mac 0x3623d0, win 0x25450;
	void stopBackgroundMusic() = mac 0x362130;
	~GameSoundManager() = mac 0x362c00;
	static GameSoundManager* sharedManager() = mac 0x3610f0, win 0x24800;

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
	int getAwardedCurrencyForLevel(GJGameLevel*) = win 0xf83e0;
	int getBaseCurrencyForLevel(GJGameLevel*) = win 0xf8530;
	char const* getCoinKey(int) = win 0xbda50;
	GJChallengeItem* getChallenge(int id) = win 0xa2fb0;
	void getSecretCoinKey(char const*) = mac 0x429f0;
	void getStat(char const*) = mac 0x3d310;
	void hasPendingUserCoin(char const*) = mac 0x42730;
	void hasSecretCoin(char const*) = mac 0x40730;
	void hasUserCoin(char const*) = mac 0x427e0;
	void incrementChallenge(GJChallengeType type, int count) = win 0xf9ae0;
	void incrementStat(char const*) = mac 0x3d6d0;
	void incrementStat(char const*, int) = mac 0x3d6e0;
	static GameStatsManager* sharedState() = mac 0x38f20, win 0xf1e50;
	void storePendingUserCoin(char const*) = mac 0x42940;
	void storeSecretCoin(char const*) = mac 0x42a10;
	void storeUserCoin(char const*) = mac 0x42890;

	PAD = win 0x28;
	cocos2d::CCDictionary* m_dailyChests;
	cocos2d::CCDictionary* m_worldAdvertChests;
	cocos2d::CCDictionary* m_activeChallenges;
	cocos2d::CCDictionary* m_upcomingChallenges;
	PAD = win 0xc;
	cocos2d::CCDictionary* m_playerStats;
	PAD = win 0x10;
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
	int m_bonusKeySeed;
	int m_bonusKeyRand;
	int m_bonusKey;
	cocos2d::CCDictionary* m_miscChests;
}

class GameToolbox {
	static CCMenuItemToggler* createToggleButton(gd::string text, cocos2d::SEL_MenuHandler onToggled, bool isToggled, cocos2d::CCMenu* toggleMenu, cocos2d::CCPoint position, cocos2d::CCNode* callbackTarget, cocos2d::CCNode* labelParent, cocos2d::CCArray* toggleArray);
	static CCMenuItemToggler* createToggleButton(gd::string text, cocos2d::SEL_MenuHandler onToggled, bool isToggled, cocos2d::CCMenu* toggleMenu, cocos2d::CCPoint position, cocos2d::CCNode* callbackTarget, cocos2d::CCNode* labelParent, float checkboxScale, float labelSize, float maxWidth, cocos2d::CCPoint labelOffset, char const* unknown, bool anchorHorizontally, int toggleTag, cocos2d::CCArray* toggleArray) = mac 0x28bdd0, win 0x25fe0;
	static void transformColor(cocos2d::ccColor3B* src, cocos2d::ccColor3B* dest, cocos2d::ccHSVValue hsv) = win 0x26a60;
	static void alignItemsHorisontally(cocos2d::CCArray* array, bool idk, cocos2d::CCPoint start, float pad) = win 0x25b20;
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
	GaragePage() = win 0x1252f0;
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
	static GauntletSelectLayer* create() = win 0x105120;
}

class GhostTrailEffect {}

class HSVWidgetPopup : FLAlertLayer {
	ConfigureHSVWidget* m_configureWidget;
	HSVWidgetPopupDelegate* m_delegate;
}

class HSVWidgetPopupDelegate {}

class HardStreak : cocos2d::CCDrawNode {
	~HardStreak() = mac 0x5bf00;
	virtual bool init() = mac 0x5c090;
	void addPoint(cocos2d::CCPoint) = mac 0x5c950;
	void clearBehindXPos(float) = mac 0x5cb40;
	static HardStreak* create() = mac 0x5bfd0;
	void firstSetup() = mac 0x5c160;
	double normalizeAngle(double) = mac 0x5cbe0;
	cocos2d::CCPoint quadCornerOffset(cocos2d::CCPoint, cocos2d::CCPoint, float) = mac 0x5c990;
	void reset() = mac 0x5c930;
	void resumeStroke() = mac 0x5c210;
	void stopStroke() = mac 0x5c8f0;
	void updateStroke(float) = mac 0x5c240;

	cocos2d::CCArray* m_pointsArr;
	cocos2d::CCPoint m_currentPoint;
	float m_waveSize;
	float m_pulseSize;
	bool m_isSolid;
}

class InfoAlertButton {
	bool init(gd::string const& title, gd::string const& text, float scale) = win 0x14ef50;
	void activate() = win 0x14f050;
	InfoAlertButton() = win 0x14ef50;
	static InfoAlertButton* create(gd::string const& title, gd::string const& text, float scale) = win 0x14ed20;
}

class InfoLayer : FLAlertLayer, LevelCommentDelegate, CommentUploadDelegate, FLAlertLayerProtocol {
	bool init(GJGameLevel* level, GJUserScore* score) = win 0x14f5a0;
	void setupCommentsBrowser(cocos2d::CCArray* comments) = win 0x152270;
	void onMore(cocos2d::CCObject* pSender) = win 0x151500;
	void onLevelInfo(cocos2d::CCObject* pSender) = win 0x151850;
	void loadPage(int page) = win 0x151e70;
	static InfoLayer* create(GJGameLevel* level, GJUserScore* score) = win 0x14f4f0;

	GJGameLevel* m_level;
	GJUserScore* m_score;
	gd::string m_commentKey;
	LoadingCircle* m_loadingCircle;
	cocos2d::CCLabelBMFont* m_pageLabel;
	cocos2d::CCLabelBMFont* m_commentsGoldLabel;
	GJCommentListLayer* m_list;
	CCMenuItemSpriteExtra* m_leftArrow;
	CCMenuItemSpriteExtra* m_rightArrow;
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
	void onClose(cocos2d::CCObject* pSender) = win 0x49c60;
	void onPrevPage(cocos2d::CCObject* pSender) = win 0x153cd0;
	void onNextPage(cocos2d::CCObject* pSender) = win 0x153cc0;
	void goToPage(int page) = win 0x153ce0;

	int m_currentPage;
	int m_itemCount;
	int m_pageCount;
	cocos2d::CCDictionary* m_pages;
	cocos2d::CCDictionary* m_unused;
	cocos2d::CCNode* m_leftArrow;
	cocos2d::CCNode* m_rightArrow;
}

class LabelGameObject : EffectGameObject {
	virtual bool init() = mac 0x2f5520;
	void setObjectColor(cocos2d::ccColor3B const&) = mac 0xdbca0;
}

class LeaderboardManagerDelegate {}

class LeaderboardsLayer : cocos2d::CCLayer {
	static LeaderboardsLayer* create(LeaderboardState state) = win 0x158710;
	bool init(LeaderboardState state) = win 0x1587b0;
}

class LevelBrowserLayer : cocos2d::CCLayer {
	static void scene(GJSearchObject* pSearch) {
	    auto pScene = cocos2d::CCScene::create();
	
	    pScene->addChild(LevelBrowserLayer::create(pSearch));
	
	    cocos2d::CCDirector::sharedDirector()->replaceScene(
	        cocos2d::CCTransitionFade::create(.5f, pScene)
	    );
	}

	bool init(GJSearchObject* pSearch) = win 0x15a040;
	void loadPage(GJSearchObject* pSearch) = win 0x15b160;
	void setupLevelBrowser(cocos2d::CCArray* levels) = win 0x15bb40;
	void updateLevelsLabel() = win 0x15c350;
	static LevelBrowserLayer* create(GJSearchObject* pSearch) = mac 0x251210, win 0x159fa0, ios 0x2d0a00;

	PAD = win 0x18;
	GJListLayer* m_list;
	CCMenuItemSpriteExtra* m_leftArrow;
	CCMenuItemSpriteExtra* m_rightArrow;
	PAD = win 0x10;
	GJSearchObject* m_searchObject;
	PAD = win 0x14;
	int m_itemCount;
	int m_pageStartIdx;
	int m_pageEndIdx;
}

class LevelCell : TableViewCell {
	CCMenuItemSpriteExtra* m_button;
	GJGameLevel* m_level;
	bool m_cellDrawn;

	void onViewProfile(cocos2d::CCObject*) = win 0x5c790;
	void loadCustomLevelCell() = win 0x5a020;
	void updateBGColor(unsigned int index) = win 0x5c6b0;
}

class LevelCommentDelegate {
	virtual void loadCommentsFinished(cocos2d::CCArray *, char const*)  {}
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
	    scene->setObjType(cocos2d::kCCObjectTypeLevelEditorLayer);
	
	    cocos2d::CCDirector::sharedDirector()->replaceScene(
	        cocos2d::CCTransitionFade::create(0.5f, scene)
	    );
	    return scene;
	}

	~LevelEditorLayer() = mac 0x90e80;
	virtual void update(float) = mac 0xa1b70, win 0x26a660;
	virtual void draw() = mac 0xa2a70;
	virtual void updateColor(cocos2d::ccColor3B, float, int, bool, float, cocos2d::ccHSVValue, int, bool, int, EffectGameObject*) = mac 0x9c200;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0xa04e0;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0x9c590;
	virtual void addToGroup(GameObject*, int, bool) = mac 0x9dab0;
	virtual void removeFromGroup(GameObject*, int) = mac 0x9db60;
	virtual void timeForXPos(float) = mac 0x9c7d0;
	virtual void xPosForTime(float) = mac 0x9c800;
	virtual void levelSettingsUpdated() = mac 0x93f30;
	static LevelEditorLayer* create(GJGameLevel* level) = mac 0x90fb0, win 0x15ed60, ios 0x261628;
	void activateTriggerEffect(EffectGameObject*, float, float, float) = mac 0x9b520;
	GameObject* addObjectFromString(gd::string) = mac 0x94640, win 0x160c80;
	void addSpecial(GameObject*) = mac 0x94f30, win 0x162650;
	void addToRedoList(UndoObject*) = mac 0x96f80;
	void addToUndoList(UndoObject*, bool) = mac 0x94e20;
	void animateInDualGround(GameObject*, float, bool) = mac 0xa2780;
	void checkCollisions(PlayerObject*, float) = mac 0x9e620;
	void createBackground() = mac 0x929f0;
	void createGroundLayer() = mac 0x92840;
	GameObject* createObject(int, cocos2d::CCPoint, bool) = mac 0x957c0, win 0x160d70;
	void createObjectsFromSetup(gd::string) = mac 0x92230;
	void createObjectsFromString(gd::string, bool) = mac 0x94730;
	void getLastObjectX() = mac 0x9c860, win 0x167290;
	void getLevelString() = mac 0x97790, win 0x162480;
	void getNextColorChannel() = mac 0x9a610;
	void getNextFreeBlockID(cocos2d::CCArray*) = mac 0x9a4e0;
	int getNextFreeGroupID(cocos2d::CCArray*) = mac 0x9a1b0, win 0x164ae0;
	void getNextFreeItemID(cocos2d::CCArray*) = mac 0x9a390;
	void getObjectRect(GameObject*, bool) = mac 0x96240, win 0x1616b0;
	void getRelativeOffset(GameObject*) = mac 0x96840;
	void handleAction(bool, cocos2d::CCArray*) = mac 0x97020;
	bool init(GJGameLevel*) = mac 0x91010;
	void objectAtPosition(cocos2d::CCPoint) = mac 0x960c0, win 0x161300;
	void objectMoved(GameObject*) = mac 0x999f0;
	void objectsInRect(cocos2d::CCRect, bool) = mac 0x95e60;
	void onPlaytest() = mac 0xa06b0;
	void onStopPlaytest() = mac 0xa1780;
	void pasteAttributeState(GameObject* obj, cocos2d::CCArray* objs) = win 0x16b740;
	void playMusic() = mac 0xa13c0;
	void recreateGroups() = mac 0x9dbf0;
	void redoLastAction() = mac 0x97750;
	void removeAllObjects() = mac 0x93d80, win 0x161d60;
	void removeAllObjectsOfType(int) = mac 0x96d40;
	void removeObject(GameObject*, bool) = mac 0x96890, win 0x161cb0;
	void removeSpecial(GameObject*) = mac 0x969c0;
	void resetMovingObjects() = mac 0x9ddc0;
	void resetObjectVector() = mac 0x9c4b0;
	void resetToggledGroups() = mac 0x9aa70;
	void resetToggledGroupsAndObjects() = mac 0x9c3c0;
	void resetUnusedColorChannels() = mac 0x9a870;
	void rotationForSlopeNearObject(GameObject*) = mac 0x95cd0;
	void runColorEffect(EffectGameObject*, int, float, float, bool) = mac 0x9bd30;
	void setupLevelStart(LevelSettingsObject*) = mac 0xa0ca0;
	void sortStickyGroups() = mac 0x92b10;
	void stopTriggersInGroup(int, float) = mac 0x9c030;
	void toggleDualMode(GameObject*, bool, PlayerObject*, bool) = mac 0xa0200;
	void toggleGroupPreview(int, bool) = mac 0x9bea0;
	void transferDefaultColors(GJEffectManager*, GJEffectManager*) = mac 0x9ab50;
	void undoLastAction() = mac 0x97770;
	void updateBGAndGColors() = mac 0x9b9b0;
	void updateBlendValues() = mac 0x9bc60;
	void updateDualGround(PlayerObject*, int, bool) = mac 0xa1a60;
	void updateEditorMode() = mac 0x93b50, win 0x1652b0;
	void updateGameObjectsNew() = mac 0x9adc0;
	void updateGround(float) = mac 0x93a60;
	void updateGroundWidth() = mac 0x92af0;
	void updateOptions() = mac 0x91ed0, win 0x15fcc0;
	void updateToggledGroups() = mac 0x9bb10;
	void updateVisibility(float) = mac 0x92c70;

	PAD = mac 0x8, win 0x4;
	bool m_unk2d0;
	bool m_unk2d1;
	bool m_unk2d2;
	bool m_unk2d3;
	bool m_gridEnabled;
	bool m_unk2d5;
	bool m_unk2d6;
	bool m_unk2d7;
	bool m_hasLDM;
	bool m_moreUndo;
	bool m_unk2da;
	bool m_unk2db;
	bool m_highDetail;
	PAD = mac 0x60, win 0x30;
	GameObject* m_copyStateObject;
	PAD = mac 0x12, win 0xc;
	int m_coinCountRand1;
	int m_coinCountRand2;
	int m_coinCount;
	PAD = mac 0x50, win 0x28;
	int m_currentLayer;
	PAD = mac 0x50, win 0x28;
	EditorUI* m_editorUI;
	PAD = mac 0x8, win 0x4;
	cocos2d::CCArray* m_undoObjects;
	cocos2d::CCArray* m_someArray;
	PAD = mac 0x16, win 0x8;
	int m_objectsRand1;
	int m_objectsRand2;
	int m_objects;
	DrawGridLayer* m_drawGridLayer;
	GJGameLevel* m_level;
	PlaybackMode m_playbackMode;
	PAD = win 0x14;
	GJGroundLayer* m_groundLayer;
}

class LevelInfoLayer : cocos2d::CCLayer, LevelDownloadDelegate, LevelUpdateDelegate, RateLevelDelegate, LikeItemDelegate, FLAlertLayerProtocol, LevelDeleteDelegate, NumberInputDelegate, SetIDPopupDelegate {
	static LevelInfoLayer* create(GJGameLevel* level) = mac 0x15f290, win 0x175d50;
	void onGarage(cocos2d::CCObject* pSender) = win 0x177c10;
	void onViewProfile(cocos2d::CCObject* pSender) = win 0x17ac90;
	void onLevelInfo(cocos2d::CCObject* pSender) = win 0x17acf0;
	void setupProgressBars() = win 0x177fc0;

	PAD = win 0x4;
	cocos2d::CCMenu* m_playBtnMenu;
	GJGameLevel* m_level;
	cocos2d::CCArray* m_unknown;
	CCMenuItemSpriteExtra* m_likeBtn;
	CCMenuItemSpriteExtra* m_starRateBtn;
	CCMenuItemSpriteExtra* m_demonRateBtn;
	PAD = win 0x4;
	CCMenuItemToggler* m_toggler;
	cocos2d::CCLabelBMFont* m_label0;
	cocos2d::CCLabelBMFont* m_label1;
	cocos2d::CCLabelBMFont* m_label2;
	cocos2d::CCLabelBMFont* m_label3;
	cocos2d::CCLabelBMFont* m_label4;
	cocos2d::CCLabelBMFont* m_label5;
	CCMenuItemSpriteExtra* m_cloneBtn;
	PAD = win 0x4;
}

class LevelLeaderboard : FLAlertLayer {
	void onChangeType(cocos2d::CCObject* pSender) = win 0x17d090;
	void onGarage(cocos2d::CCObject* pSender) = win 0x17d1b0;
	bool init(GJGameLevel* level, int type) = win 0x17c4f0;
	static LevelLeaderboard* create(GJGameLevel* level, LevelLeaderboardType leaderboardType) = win 0x17c440;
}

class LevelManagerDelegate {}

class LevelPage {
	PAD = win 0x124;
	GJGameLevel* m_level;
}

class LevelSearchLayer {
	static LevelSearchLayer* create() = win 0x17d9c0;
	GJSearchObject* getSearchObject(SearchType, gd::string) = win 0x1805f0;
	void onMoreOptions(cocos2d::CCObject*) = win 0x17f500;
	void onSearch(cocos2d::CCObject*) = win 0x180fc0;

	PAD = win 0xC;
	CCTextInputNode* m_searchInput;
}

class LevelSelectLayer : cocos2d::CCLayer {
	static LevelSelectLayer* create(int lvl) = win 0x185500;

	PAD = win 0x10;
	BoomScrollLayer* m_scrollLayer;
}

class LevelSettingsDelegate {}

class LevelSettingsLayer {
	static LevelSettingsLayer* create(LevelSettingsObject* levelSettings, LevelEditorLayer* editor) = win 0x170d90;
}

class LevelSettingsObject : cocos2d::CCNode {
	~LevelSettingsObject() = mac 0xa5650;
	virtual bool init() = mac 0xa5690;
	static LevelSettingsObject* create() = mac 0x92760;
	void objectFromDict(cocos2d::CCDictionary*) = mac 0xa5810;
	static LevelSettingsObject* objectFromString(gd::string) = mac 0x945a0;
	void setupColorsFromLegacyMode(cocos2d::CCDictionary*) = mac 0xa6a30;

	GJEffectManager* m_effectManager;
	// TODO: make enums for these
	int m_startMode;
	int m_startSpeed;
	bool m_startMini;
	bool m_startDual;
	bool m_twoPlayerMode;
	float m_songOffset;
	bool m_fadeIn;
	bool m_fadeOut;
	int m_backgroundIndex;
	int m_groundIndex;
	int m_fontIndex;
	PAD = win 0x4;
	GJGameLevel* m_level;
	gd::string m_unknownStr;
}

class LevelUploadDelegate {}

class LevelUpdateDelegate {
	virtual void levelUpdateFinished(GJGameLevel*, UpdateResponse) {}
	virtual void levelUpdateFailed(int) {}
}

class LikeItemDelegate {
	virtual void likedItem(LikeItemType, int, bool) {}
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

	static LoadingLayer* create(bool fromReload) = mac 0x1df1f0, win 0x18bfe0, ios 0x130278;
	bool init(bool fromReload) = mac 0x1df2f0, win 0x18c080;
	char const* getLoadingString() = win 0x18cf40;
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
	static LocalLevelManager* sharedState() = mac 0x35dd60;

	//PAD = win 0x1C;
	cocos2d::CCDictionary* m_loadData;
	cocos2d::CCDictionary* m_levelData;
	cocos2d::CCArray* m_localLevels;
}

class MapPackCell : TableViewCell {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
}

class MenuGameLayer {
	void resetPlayer() = mac 0x28fdc0;
	void update(float) = mac 0x28fa70;
}

class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol {
	~MenuLayer() = mac 0x1d1230;
	virtual bool init() = mac 0x1d14b0, win 0x1907b0;
	virtual void keyBackClicked() = mac 0x1d3160, win 0x191f90;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x1d33d0, win 0x1922c0;
	virtual void googlePlaySignedIn() = mac 0x1d2f30;
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
	MenuLayer* node() = win 0x190550;
}

class MessageListDelegate {}

class MoreSearchLayer : FLAlertLayer {
	static MoreSearchLayer* create() = win 0x182520;
	bool init() = win 0x1825c0;
}

class MoreOptionsLayer {
	static MoreOptionsLayer* create() = win 0x1de850;
	void addToggle(char const* name, char const* key, char const* info) = win 0x1df6b0;
	void onKeybindings(cocos2d::CCObject* pSender) = win 0x749d0;
}

class MoreVideoOptionsLayer : FLAlertLayer {
	static MoreVideoOptionsLayer* create() = mac 0x443c10;
	virtual bool init() = mac 0x444150;
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

	cocos2d::CCDictionary* m_unknownDict;
	cocos2d::CCArray* m_handlers;
	cocos2d::CCDictionary* m_songsDict;
	int m_unknown;
}

class NumberInputDelegate {
	virtual void numberInputClosed(NumberInputLayer*) {}
}

class NumberInputLayer {}

class OBB2D : cocos2d::CCNode {
	void calculateWithCenter(cocos2d::CCPoint, float, float, float) = mac 0x35a9c0;
	static OBB2D* create(cocos2d::CCPoint, float, float, float) = mac 0x35a890;
	cocos2d::CCRect getBoundingRect() = mac 0x35b2b0;
	void overlaps(OBB2D*) = mac 0x35b0a0;
	void overlaps1Way(OBB2D*) = mac 0x35b0d0;

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

class ObjectToolbox : cocos2d::CCNode {
	cocos2d::CCArray* allKeys() {
	    return m_frameToKeyDict->allKeys();
	}
	char const* frameToKey(char const* frame) {
	    return reinterpret_cast<cocos2d::CCString*>(m_frameToKeyDict->objectForKey(frame))->getCString();
	}
	char const* intKeyToFrame(int key) {
	    return reinterpret_cast<cocos2d::CCString*>(m_keyToFrameDict->objectForKey(key))->getCString();
	}
	char const* keyToFrame(char const* key) {
	    return intKeyToFrame(atoi(key));
	}
	void addObject(int id, char const* frame) {
	    m_frameToKeyDict->setObject(cocos2d::CCString::createWithFormat("%i", id), frame);
	    m_keyToFrameDict->setObject(cocos2d::CCString::create(frame), id);
	}

	virtual bool init() = mac 0x3b2d80;
	static ObjectToolbox* sharedState() = mac 0x3b2bc0, win 0x198a80;
	float gridNodeSizeForKey(int key) = win 0x1dc920;
	char const* perspectiveBlockFrame(int key) = win 0x1dcd00;

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
	virtual void loadListFinished(cocos2d::CCArray*, char const*) {}
	virtual void loadListFailed(char const*) {}
	virtual void setupPageInfo(gd::string, char const*) {}
}

class OptionsLayer : GJDropDownLayer, FLAlertLayerProtocol {
	static OptionsLayer* addToCurrentScene(bool noTransition) {
	    auto layer = OptionsLayer::create();
	    cocos2d::CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
	    layer->showLayer(noTransition);
	    return layer;
	}

	static OptionsLayer* create() = mac 0x43db60, win 0x1dd310, ios 0x86314;
}

class PauseLayer : CCBlockLayer {
	static PauseLayer* create(bool) = mac 0x20b1e0;
	void onEdit(cocos2d::CCObject*) = mac 0x20c630;
	void createToggleButton(cocos2d::SEL_MenuHandler callback, bool on, cocos2d::CCMenu* menu, gd::string caption, cocos2d::CCPoint pos) = win 0x1e5570;

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
	static bool isControllerConnected() = mac 0x27d1b0;
}

class PlayLayer : GJBaseGameLayer, CCCircleWaveDelegate, DialogDelegate {
	static PlayLayer* get() {
	    return GameManager::sharedState()->getPlayLayer();
	}

	void addCircle(CCCircleWave*) = mac 0x7e0f0;
	void addObject(GameObject*) = mac 0x70e50, win 0x2017e0;
	void addToGroupOld(GameObject*) = mac 0x77680;
	void addToSpeedObjects(GameObject*) = mac 0x7cfc0;
	void animateInDualGround(GameObject*, float, bool) = mac 0x7d710;
	void animateInGround(bool) = mac 0x7d9d0;
	void animateOutGround(bool) = mac 0x6f350;
	void animateOutGroundFinished() = mac 0x7de80;
	void applyEnterEffect(GameObject*) = mac 0x7c310;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0x7aa10;
	void cameraMoveX(float, float, float) = mac 0x7cbe0;
	void cameraMoveY(float, float, float) = mac 0x7cc60;
	void checkCollisions(PlayerObject*, float) = mac 0x78c90;
	void circleWaveWillBeRemoved(CCCircleWave*) = mac 0x7e110;
	void claimParticle(gd::string) = mac 0x76ba0;
	void clearPickedUpItems() = mac 0x7cfa0;
	void colorObject(int, cocos2d::ccColor3B) = mac 0x77810;
	void commitJumps() = mac 0x737e0;
	static PlayLayer* create(GJGameLevel*) = mac 0x6b590, win 0x1fb6d0;
	void createCheckpoint() = mac 0x7e470, win 0x20b050;
	void createObjectsFromSetup(gd::string) = mac 0x6d130;
	void createParticle(int, char const*, int, cocos2d::tCCPositionType) = mac 0x76800;
	void currencyWillExit(CurrencyRewardLayer*) = mac 0x7e070;
	void delayedResetLevel() = mac 0x7e050;
	void destroyPlayer(PlayerObject*, GameObject*) = mac 0x7ab80, win 0x20a1a0;
	void dialogClosed(DialogLayer*) = mac 0x7e0b0;
	virtual void draw() = mac 0x7d160, win 0x208870;
	void enterDualMode(GameObject*, bool) = mac 0x7d6a0;
	void exitAirMode() = mac 0x7dd40;
	void exitBirdMode(PlayerObject*) = mac 0x7dd80;
	void exitDartMode(PlayerObject*) = mac 0x7ddd0;
	void exitFlyMode(PlayerObject*) = mac 0x7dcf0;
	void exitRobotMode(PlayerObject*) = mac 0x7de20;
	void exitRollMode(PlayerObject*) = mac 0x7de60;
	void exitSpiderMode(PlayerObject*) = mac 0x7de40;
	void flipFinished() = mac 0x7e150;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0x7cd10;
	void flipObjects() = mac 0x76130;
	void fullReset() = mac 0x7f8e0;
	void getLastCheckpoint() = mac 0x7f840;
	void getMaxPortalY() = mac 0x7b4e0;
	void getMinPortalY() = mac 0x7b550;
	void getObjectsState() = mac 0x7e9d0;
	void getOtherPlayer(PlayerObject*) = mac 0x7dcc0;
	void getParticleKey(int, char const*, int, cocos2d::tCCPositionType) = mac 0x764d0;
	void getParticleKey2(gd::string) = mac 0x767b0;
	void getRelativeMod(cocos2d::CCPoint, float, float, float) = mac 0x7c2a0;
	void getTempMilliTime() = mac 0x778e0;
	void gravityEffectFinished() = mac 0x7cec0;
	void hasItem(int) = mac 0x7cee0;
	void hasUniqueCoin(GameObject*) = mac 0x77510;
	void incrementJumps() = mac 0x7ff40;
	bool init(GJGameLevel*) = mac 0x6b5f0, win 0x1fb780;
	void isFlipping() = mac 0x76100;
	void levelComplete() = mac 0x72b80, win 0x1fd3d0;
	void lightningFlash(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::ccColor3B, float, float, int, bool, float) = mac 0x75cc0;
	void lightningFlash(cocos2d::CCPoint, cocos2d::ccColor3B) = mac 0x75bf0;
	void loadDefaultColors() = mac 0x6ef30;
	void loadFromCheckpoint(CheckpointObject*) = mac 0x7f000;
	void loadLastCheckpoint() = mac 0x7efc0;
	void loadSavedObjectsState(gd::string) = mac 0x7f3d0;
	void markCheckpoint() = mac 0x7ef60, win 0x25fb60;
	void moveCameraToPos(cocos2d::CCPoint) = mac 0x7c980;
	virtual void onEnterTransitionDidFinish() = mac 0x806e0, win 0x20dbd0;
	virtual void onExit() = mac 0x80710, win 0x20d810;
	void onQuit() = mac 0x72710, win 0x20d810;
	void optimizeColorGroups() = mac 0x6dad0;
	void optimizeOpacityGroups() = mac 0x6dc20;
	void optimizeSaveRequiredGroups() = mac 0x6dd70;
	void pauseGame(bool) = mac 0x802d0;
	void pickupItem(GameObject*) = mac 0x7c1d0;
	void playAnimationCommand(int, int) = mac 0x75930;
	void playEndAnimationToPos(cocos2d::CCPoint) = mac 0x759a0;
	void playExitDualEffect(PlayerObject*) = mac 0x7d1d0;
	void playFlashEffect(float, int, float) = mac 0x75e50;
	void playGravityEffect(bool) = mac 0x7b5a0;
	void playSpeedParticle(float) = mac 0x77030;
	void playerWillSwitchMode(PlayerObject*, GameObject*) = mac 0x7b820;
	void prepareSpawnObjects() = mac 0x7fc00;
	void processItems() = mac 0x735c0;
	void processLoadedMoveActions() = mac 0x7a7c0;
	void recordAction(bool, PlayerObject*) = mac 0x7e190;
	void registerActiveObject(GameObject*) = mac 0x77620;
	void registerStateObject(GameObject*) = mac 0x777b0;
	void removeAllObjects() = mac 0x727b0;
	void removeFromGroupOld(GameObject*) = mac 0x77750;
	void removeLastCheckpoint() = mac 0x7f870, win 0x20b830;
	void removePlayer2() = mac 0x7d630;
	void resetLevel() = mac 0x71c50, win 0x20bf00;
	void resume() = mac 0x80480;
	void resumeAndRestart() = mac 0x80400;
	void saveRecordAction(bool, PlayerObject*) = mac 0x78750;
	static cocos2d::CCScene* scene(GJGameLevel*) = mac 0x6b500, win 0x1fb690;
	void setupLevelStart(LevelSettingsObject*) = mac 0x6f560, win 0x1fb780;
	void setupReplay(gd::string) = mac 0x7e1e0;
	void shakeCamera(float, float, float) = mac 0x744a0, win 0x1ff210;
	void shouldBlend(int) = mac 0x771b0;
	void showCompleteEffect() = mac 0x738e0;
	void showCompleteText() = mac 0x73be0;
	void showEndLayer() = mac 0x74450;
	void showHint() = mac 0x7deb0;
	void showNewBest(bool, int, int, bool, bool, bool) = mac 0x74580;
	void showRetryLayer() = mac 0x75ba0;
	void showTwoPlayerGuide() = mac 0x6df00;
	void sortGroups() = mac 0x6d9e0;
	void spawnCircle() = mac 0x73820;
	void spawnFirework() = mac 0x74200;
	void spawnParticle(char const*, int, cocos2d::tCCPositionType, cocos2d::CCPoint) = mac 0x76330;
	void spawnPlayer2() = mac 0x7d170;
	void startGame() = mac 0x726b0;
	void startMusic() = mac 0x72910;
	void startRecording() = mac 0x7fec0;
	void startRecordingDelayed() = mac 0x7fed0;
	void stopCameraShake() = mac 0x75900;
	void stopRecording() = mac 0x6d090;
	void storeCheckpoint(CheckpointObject*) = mac 0x7ef10;
	void switchToFlyMode(PlayerObject*, GameObject*, bool, int) = mac 0x7baf0;
	void switchToRobotMode(PlayerObject*, GameObject*, bool) = mac 0x7bc80;
	void switchToRollMode(PlayerObject*, GameObject*, bool) = mac 0x7bbe0;
	void switchToSpiderMode(PlayerObject*, GameObject*, bool) = mac 0x7bd20;
	void timeForXPos(float) = mac 0x7d120;
	void timeForXPos2(float, bool) = mac 0x293eb0, win 0x1fd3d0;
	void toggleBGEffectVisibility(bool) = mac 0x7fe80;
	void toggleDualMode(GameObject*, bool, PlayerObject*, bool) = mac 0x7bf90;
	void toggleFlipped(bool, bool) = mac 0x7bdc0;
	void toggleGhostEffect(int) = mac 0x7fe40;
	void toggleGlitter(bool) = mac 0x70e00;
	void togglePracticeMode(bool) = mac 0x7f9e0, win 0x20d0d0;
	void toggleProgressbar() = mac 0x6eeb0;
	void tryStartRecord() = mac 0x7fe00;
	void unclaimParticle(char const*, cocos2d::CCParticleSystemQuad*) = mac 0x76e00;
	void unregisterActiveObject(GameObject*) = mac 0x77660;
	void unregisterStateObject(GameObject*) = mac 0x777f0;
	virtual void update(float) = mac 0x77900, win 0x2029C0, ios 0xb2f08;
	void updateAttempts() = mac 0x7fcd0, win 0x20ced0;
	void updateCamera(float) = mac 0x6e2b0;
	virtual void updateColor(cocos2d::ccColor3B, float, int, bool, float, cocos2d::ccHSVValue, int, bool, int, EffectGameObject*) = mac 0x7c7f0;
	void updateDualGround(PlayerObject*, int, bool) = mac 0x7caa0;
	void updateEffectPositions() = mac 0x7a6d0;
	void updateLevelColors() = mac 0x6f1e0;
	void updateMoveObjectsLastPosition() = mac 0x7a720;
	void updateProgressbar() = mac 0x6ed70;
	void updateReplay(float) = mac 0x78b60, win 0x20af40;
	void updateTimeMod(float, bool) = mac 0x786f0;
	virtual void updateTweenAction(float, char const*) = mac 0x7ffb0, win 0x20d1f0;
	void updateVisibility() = mac 0x6fb90, win 0x205460;
	void vfDChk() = mac 0x7fcb0;
	virtual void visit() = mac 0x75ef0, win 0x200020;
	void visitWithColorFlash() = mac 0x761f0;
	void willSwitchToMode(int, PlayerObject*) = mac 0x7b9e0;
	void xPosForTime(float) = mac 0x7d140;
	~PlayLayer() = mac 0x6b090, win 0x1fafc0;

	float unused4c8;
	bool unused4cc;
	bool m_hasCheated;
	int m_dontSaveRand;
	int m_dontSaveSeed;
	int unknown4d8;
	bool unknown4dc;
	bool m_shouldSmoothCamera;
	float unused_4e0;
	cocos2d::CCObject* unknown4e8;
	float m_camera4f0;
	int unused4f4;
	float m_somegroup4f8;
	float m_groundRestriction;
	float m_ceilRestriction;
	bool unknown504;
	bool unknown505;
	float unknown508;
	float unknown50c;
	float unknown510;
	float unknown514;
	float unknown518;
	//PAD = mac 0x19, win 0x19;
	StartPosObject* m_startPos;
	CheckpointObject* unk330;
	EndPortalObject* m_endPortal;
	cocos2d::CCArray* m_checkpoints;
	cocos2d::CCArray* unk33C;
	cocos2d::CCArray* unk340;
	cocos2d::CCArray* unk344;
	cocos2d::CCSprite* unk348;
	float m_backgroundRepeat;
	bool unk350;
	cocos2d::CCArray* unk354;
	cocos2d::CCArray* unk358;
	cocos2d::CCArray* unk35C;
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
	cocos2d::CCParticleSystemQuad* unk370;
	cocos2d::CCDictionary* m_pickedUpItems;
	cocos2d::CCArray* m_circleWaves;
	cocos2d::CCArray* unk37C;
	AudioEffectsLayer* m_audioEffectsLayer;
	float unknown5c0;
	float unknown5c4;
	GJGroundLayer* m_bottomGround;
	GJGroundLayer* m_topGround;

	// this is literaly garbage data
	PAD = mac 0x8, win 0x8;

	bool m_isDead;
	bool m_startCameraAtCorner;
	bool m_cameraYLocked;
	bool m_cameraXLocked;
	bool unknown5e4;
	int m_ballFrameSeed;
	float unknown5ec;
	float unknown5f0;
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
	void* unknown680;
	int m_activeGravityEffects;
	int m_gravityEffectStatus; // ??
	cocos2d::CCArray* m_gravitySprites;
	bool unk428;
	bool m_shouldRecordActions;
	bool unk42A;
	bool m_isPaused;
	bool unk42C;
	bool m_isPlayer2Frozen;
	gd::string m_previousRecords;
	void* unknown6a8;
	double m_time;
	int unknown6b8;
	int unknown6bc;
	bool unk460;
	bool unk461;
	cocos2d::CCDictionary* unk464;
	gd::map<short, bool> unk468;
	bool m_collisionDisabled;
	bool unknown701;
	GameObject* m_latestVehicle;
	GameObject* m_dualObject;
	bool m_isFlipped;
	float m_mirrorTransition;
	UILayer* m_UILayer;
	GJGameLevel* m_level;
	cocos2d::CCPoint m_cameraPos;
	bool m_isTestMode;
	bool m_isPracticeMode;
	bool unk496;
	bool unk497;
	cocos2d::CCArray* unk498;
	bool unk49C;
	cocos2d::CCPoint unk4A0;
	int m_currentAttempt;
	int m_jumpCount;
	bool unk4B0;
	float m_totalTime;
	int m_attemptJumpCount;
	bool unk4BC;
	bool m_hasLevelCompleteMenu;
	bool m_hasCompletedLevel;
	bool unk4BF;
	int m_lastDeathPercent;
	bool unk4C4;
	PAD = win 0xb;
	bool unk4D0;
	bool unk4D1;
	cocos2d::CCArray* unk4D4;
	cocos2d::CCDictionary* unk4D8;
	PAD = win 0x4;
	double unk4E0;
	double unk4E8;
	PAD = win 0xc;
	float unk4FC;
	int unk500;
	PAD = win 0x4;
	double unk508;
	PAD = win 0x8;
	double unk518;
	double unk520;
	PAD = win 0x4;
	bool m_hasGlitter;
	bool m_isBgEffectOff;
	bool unk52F;
	GameObject* unk530;
	bool unk534;
	bool unk535;
	bool m_disableGravityEffect;
}

class PlayerCheckpoint : cocos2d::CCNode {
	~PlayerCheckpoint() = mac 0x80760;
	virtual bool init() = mac 0x807a0;
	static PlayerCheckpoint* create() = mac 0x7e8c0;

	cocos2d::CCPoint m_position;
	float m_yAccel;
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
	~PlayerObject() = mac 0x217220;
	virtual void update(float) = mac 0x218bf0;
	virtual void setScaleX(float) = mac 0x22e7f0;
	virtual void setScaleY(float) = mac 0x22e830;
	virtual void setScale(float) = mac 0x22e870;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x22c8b0;
	virtual void setVisible(bool) = mac 0x22e8b0, win 0x1fa860;
	virtual void setRotation(float) = mac 0x22e6e0;
	virtual void setOpacity(unsigned char) = mac 0x22d400;
	virtual void setColor(cocos2d::ccColor3B const&) = mac 0x22cdf0;
	virtual void setFlipX(bool) = mac 0x22e720, win 0x1fa690;
	virtual void setFlipY(bool) = mac 0x22e7b0, win 0x1fa740;
	virtual void resetObject() = mac 0x223170, win 0x1eecd0;
	virtual void getRealPosition() = mac 0x22d5f0, win 0x1f7e20;
	virtual void getOrientedBox() = mac 0x22dee0, win 0x1f95d0;
	virtual void animationFinished(char const*) = mac 0x22e9d0;
	void activateStreak() = mac 0x21aef0, win 0x1f9080;
	void addAllParticles() = mac 0x2189b0;
	void addToTouchedRings(GameObject*) = mac 0x22b800;
	void boostPlayer(float) = mac 0x21d6b0;
	void bumpPlayer(float, int) = mac 0x22d890;
	void buttonDown(PlayerButton) = mac 0x22b7e0;
	void checkSnapJumpToObject(GameObject*) = mac 0x2217f0;
	void collidedWithObject(float, GameObject*) = mac 0x21d880;
	void collidedWithObject(float, GameObject*, cocos2d::CCRect) = mac 0x21f0b0;
	void collidedWithSlope(float, GameObject*, bool) = mac 0x21d8d0;
	void convertToClosestRotation(float) = mac 0x21c860;
	void copyAttributes(PlayerObject*) = mac 0x22dc70;
	static PlayerObject* create(int, int, cocos2d::CCLayer*) = mac 0x217260;
	void deactivateParticle() = mac 0x21a540;
	void deactivateStreak(bool) = mac 0x218b30;
	void fadeOutStreak2(float) = mac 0x225890, win 0x1f9110;
	void flashPlayer(float, float, cocos2d::ccColor3B, cocos2d::ccColor3B) = mac 0x221c80;
	void flipGravity(bool, bool) = mac 0x21c090, win 0x1f59d0;
	void flipMod() = mac 0x21a4c0;
	void getActiveMode() = mac 0x22b950;
	void getModifiedSlopeYVel() = mac 0x21bff0;
	void getOldPosition(float) = mac 0x21a830;
	void getSecondColor() = mac 0x22cee0;
	void gravityDown() = mac 0x22e930;
	void gravityUp() = mac 0x22e900;
	void hardFlipGravity() = mac 0x22b860;
	void hitGround(bool) = mac 0x220a30;
	void incrementJumps() = mac 0x21c050, win 0x1e9a20;
	bool init(int, int, cocos2d::CCLayer*) = mac 0x2172e0;
	void isBoostValid(float) = mac 0x21d650;
	void isFlying() = mac 0x21a4e0;
	void isSafeFlip(float) = mac 0x2209f0;
	void isSafeMode(float) = mac 0x2209b0;
	void isSafeSpiderFlip(float) = mac 0x221be0;
	void levelFlipFinished() = mac 0x21b060;
	void levelFlipping() = mac 0x21a510;
	void levelWillFlip() = mac 0x21b020;
	void loadFromCheckpoint(PlayerCheckpoint*) = mac 0x22e420;
	void lockPlayer() = mac 0x22d680;
	void logValues() = mac 0x221220;
	void modeDidChange() = mac 0x22bfd0;
	void placeStreakPoint() = mac 0x21af90;
	void playBurstEffect() = mac 0x21c780, win 0x1f6790;
	void playDeathEffect() = mac 0x225930, win 0x2efbe0;
	void playDynamicSpiderRun() = mac 0x222ec0;
	void playerDestroyed(bool) = mac 0x2256d0, win 0x1efaa0;
	bool playerIsFalling() = mac 0x21c730, win 0x1f5d60;
	void playerTeleported() = mac 0x22b840;
	void playingEndEffect() = mac 0x22d7e0;
	void postCollision(float) = mac 0x21cd10;
	void preCollision() = mac 0x21ccc0;
	void preSlopeCollision(float, GameObject*) = mac 0x21ec80;
	void propellPlayer(float) = mac 0x22d8e0;
	void pushButton(int) = mac 0x22aa00, win 0x1f4e40;
	void pushDown() = mac 0x22dbd0;
	void pushPlayer(float) = mac 0x22dbb0;
	void releaseButton(int) = mac 0x22b6f0, win 0x1f4f70;
	void removeAllParticles() = mac 0x218ac0;
	void removePendingCheckpoint() = mac 0x2237b0;
	void resetAllParticles() = mac 0x21adb0;
	void resetCollisionLog() = mac 0x21cc20;
	void resetPlayerIcon() = mac 0x22be00;
	void resetStateVariables() = mac 0x223760;
	void resetStreak() = mac 0x21ae10;
	void ringJump(GameObject*) = mac 0x22abf0, win 0x1f4ff0;
	void runBallRotation(float) = mac 0x21ca10;
	void runBallRotation2() = mac 0x21cb10;
	void runNormalRotation() = mac 0x21c960, win 0x1e9c50;
	void runRotateAction(bool) = mac 0x21c570;
	void saveToCheckpoint(PlayerCheckpoint*) = mac 0x22e2f0;
	void setSecondColor(cocos2d::ccColor3B const&) = mac 0x219610;
	void setupStreak() = mac 0x218720;
	void spawnCircle() = mac 0x225480;
	void spawnCircle2() = mac 0x2252a0;
	void spawnDualCircle() = mac 0x2255c0;
	void spawnFromPlayer(PlayerObject*) = mac 0x22dde0;
	void spawnPortalCircle(cocos2d::ccColor3B, float) = mac 0x225350;
	void spawnScaleCircle() = mac 0x2251b0;
	void specialGroundHit() = mac 0x22dbf0;
	void speedDown() = mac 0x22e970;
	void speedUp() = mac 0x22e950;
	void spiderTestJump(bool) = mac 0x21b160, win 0x1ed360;
	void startDashing(GameObject*) = mac 0x221d70;
	void stopBurstEffect() = mac 0x22c680;
	void stopDashing() = mac 0x222990;
	void stopRotation(bool) = mac 0x21c830;
	void storeCollision(bool, int) = mac 0x21cc60;
	void switchedToMode(GameObjectType) = mac 0x22b9a0;
	void testForMoving(float, GameObject*) = mac 0x21eb70;
	void toggleBirdMode(bool) = mac 0x224070, win 0x1f6050;
	void toggleDartMode(bool) = mac 0x2243f0, win 0x1f62c0;
	void toggleFlyMode(bool) = mac 0x223820, win 0x1f5e40;
	void toggleGhostEffect(GhostType) = mac 0x225000, win 0x1f8930;
	void togglePlayerScale(bool) = mac 0x224bd0, win 0x1f9640;
	void toggleRobotMode(bool) = mac 0x223c70, win 0x1f6a10;
	void toggleRollMode(bool) = mac 0x223b20, win 0x1f68e0;
	void toggleSpiderMode(bool) = mac 0x224830, win 0x1f94d1;
	void toggleVisibility(bool) = mac 0x21abf0;
	void touchedObject(GameObject*) = mac 0x22e660;
	void tryPlaceCheckpoint() = mac 0x21a950;
	void updateCheckpointMode(bool) = mac 0x218980;
	void updateCheckpointTest() = mac 0x21a890;
	void updateCollide(bool, int) = mac 0x220f10;
	void updateCollideBottom(float, int) = mac 0x221790;
	void updateCollideTop(float, int) = mac 0x221c20;
	void updateDashAnimation() = mac 0x21a570;
	void updateDashArt() = mac 0x222520;
	void updateGlowColor() = mac 0x22cf10;
	void updateJump(float) = mac 0x219680;
	void updateJumpVariables() = mac 0x21a740;
	void updatePlayerBirdFrame(int) = mac 0x22bfe0;
	void updatePlayerDartFrame(int) = mac 0x22c260;
	void updatePlayerFrame(int) = mac 0x22c470;
	void updatePlayerGlow() = mac 0x22bc50;
	void updatePlayerRobotFrame(int) = mac 0x22d620;
	void updatePlayerRollFrame(int) = mac 0x22c6a0;
	void updatePlayerScale() = mac 0x22b8b0;
	void updatePlayerShipFrame(int) = mac 0x22ba40;
	void updatePlayerSpiderFrame(int) = mac 0x22d650;
	void updatePlayerSpriteExtra(gd::string) = mac 0x218440;
	void updateRobotAnimationSpeed() = mac 0x22df40;
	void updateRotation(float) = mac 0x2214b0;
	void updateRotation(float, float) = mac 0x221230;
	void updateShipRotation(float) = mac 0x221310;
	void updateShipSpriteExtra(gd::string) = mac 0x218510;
	void updateSlopeRotation(float) = mac 0x221030;
	void updateSlopeYVelocity(float) = mac 0x22e920;
	void updateSpecial(float) = mac 0x21a790;
	void updateStateVariables() = mac 0x21a770;
	void updateTimeMod(float) = mac 0x2185e0, win 0x1f94e0;
	void usingWallLimitedMode() = mac 0x22df00;
	void yStartDown() = mac 0x22e9b0;
	void yStartUp() = mac 0x22e990;
	void runRotateAction() = win 0x1e9bf0;
	void runBallRotation() = win 0x1e9d10;

	// HardStreak* waveStreak = mac 0x600;
	// double speed = mac 0x608;
	// double gravity = mac 0x618;
	// bool inPlayLayer = mac 0x62c;
	// GJRobotSprite* robotSprite = mac 0x6a8;
	// GJSpiderSprite* spiderSprite = mac 0x6b0;
	// bool isHolding = mac 0x745;
	// bool hasJustHeld = mac 0x746;
	// double yAccel = mac 0x760;
	// bool isShip = mac 0x770;
	// bool isBird = mac 0x771;
	// bool isBall = mac 0x772;
	// bool isDart = mac 0x773;
	// bool isRobot = mac 0x774;
	// bool isSpider = mac 0x775;
	// bool upsideDown = mac 0x776;
	// bool dead = mac 0x777;
	// bool onGround = mac 0x778;
	// float vehicleSize = mac 0x77c;
	// cocos2d::CCPoint lastPortalLocation = mac 0x78c;
	// bool isSliding = mac 0x7a0;
	// bool isRising = mac 0x7a1;
	// cocos2d::CCPoint lastHitGround = mac 0x7a4;
	// GameObject* lastPortal = mac 0x7b8;
	// cocos2d::ccColor3B col1 = mac 0x7c2;
	// cocos2d::ccColor3B col2 = mac 0x7c5;
	// float xPos = mac 0x7c8;
	// float yPos = mac 0x7cc;
	PAD = mac 0x18, win 0x14;
	bool m_unk480;
	cocos2d::CCNode* m_unk484;
	cocos2d::CCDictionary* m_collisionLog;
	cocos2d::CCDictionary* m_collisionLog1;
	PAD = mac 0x38, win 0x20;
	bool m_unk4B0;
	cocos2d::CCSprite* m_unk4B4;
	PAD = mac 0x1c, win 0x1c;
	bool m_unk4D4;
	cocos2d::CCArray* m_particleSystems;
	bool m_unk4DC;
	bool m_isHidden;
	int m_hasGhostTrail;
	GhostTrailEffect* m_ghostTrail;
	cocos2d::CCSprite* m_iconSprite;
	cocos2d::CCSprite* m_iconSpriteSecondary;
	cocos2d::CCSprite* m_iconSpriteWhitener;
	cocos2d::CCSprite* m_iconGlow;
	cocos2d::CCSprite* m_vehicleSprite;
	cocos2d::CCSprite* m_vehicleSpriteSecondary;
	cocos2d::CCSprite* m_unk500;
	cocos2d::CCSprite* m_vehicleSpriteWhitener;
	cocos2d::CCSprite* m_vehicleGlow;
	cocos2d::CCSprite* m_unk50C;
	cocos2d::CCMotionStreak* m_regularTrail;
	HardStreak* m_waveTrail;
	double m_xAccel;
	double m_jumpAccel;
	double m_gravity;
	PAD = mac 0x108;
	bool m_unk538;
	bool m_unk539;
	bool m_unk53A;
	bool m_unk53B;
	bool m_isInPlayLayer;
	bool m_unk53D;
	bool m_unk53E;
	bool m_unk53F;
	PAD = win 0x10;
	double m_lastJumpTime;
	double m_unk558;
	PAD = win 0x24;
	float m_unk584;
	PAD = win 0x14;
	GameObject* m_unk59C;
	PAD = win 0x8;
	GJRobotSprite* m_robot;
	GJSpiderSprite* m_spider;
	bool m_unk5B0;
	cocos2d::CCParticleSystemQuad* m_unk5B4;
	cocos2d::CCParticleSystemQuad* m_unk5B8;
	cocos2d::CCParticleSystemQuad* m_unk5BC;
	cocos2d::CCParticleSystemQuad* m_unk5C0;
	cocos2d::CCParticleSystemQuad* m_unk5C4;
	cocos2d::CCParticleSystemQuad* m_unk5C8;
	cocos2d::CCParticleSystemQuad* m_unk5CC;
	PAD = win 0x4;
	cocos2d::CCParticleSystemQuad* m_unk5D4;
	cocos2d::CCParticleSystemQuad* m_unk5D8;
	PAD = win 0x20;
	// int m_streakID;
	// float m_wellIdk;
	// PAD = win 0x10;
	bool m_unk5FC;
	bool m_unk5FD;
	bool m_unk5FE;
	PAD = win 0x11;
	bool m_unk610;
	bool m_isHolding;
	bool m_hasJustHeld;
	bool m_isHolding2;
	bool m_hasJustHeld2;
	int m_unk618;
	float m_unk61C;
	int m_unk620;
	bool m_canRobotJump;
	double m_yAccel;
	bool m_unk630;
	bool m_unk631;
	float m_unk634;
	bool m_isShip;
	bool m_isBird;
	bool m_isBall;
	bool m_isDart;
	bool m_isRobot;
	bool m_isSpider;
	bool m_isUpsideDown;
	bool m_isOnGround;
	bool m_isDashing;
	float m_vehicleSize;
	float m_playerSpeed;
	cocos2d::CCPoint m_unk64C;
	cocos2d::CCPoint m_lastPortalPos;
	cocos2d::CCLayer* m_unk65C;
	bool m_isSliding;
	bool m_isRising;
	bool m_unk662;
	cocos2d::CCPoint m_lastGroundedPos;
	cocos2d::CCArray* m_touchingRings;
	GameObject* m_lastActivatedPortal;
	bool m_unk674;
	PAD = win 0x4;
	cocos2d::CCPoint m_position;
	bool m_unk684;
	bool m_unk685;
	double m_unk688;
	PAD = win 0x8;
	float m_groundHeight;
	float m_unk69C;
	PAD = win 0x4;
	float m_unk6A4[200];
	PAD = win 0x1c;
}

class PointNode : cocos2d::CCObject {
	static PointNode* create(cocos2d::CCPoint point) = win 0x14ec80;

	cocos2d::CCPoint m_point;
}

class ProfilePage : FLAlertLayer, FLAlertLayerProtocol, LevelCommentDelegate, CommentUploadDelegate, UserInfoDelegate, UploadActionDelegate, UploadPopupDelegate, LeaderboardManagerDelegate {
	static ProfilePage* create(int accountID, bool idk) = mac 0x45eed0, win 0x20ee50;
	void getUserInfoFailed(int) = win 0x2133e0;
	bool init(int accountID, bool idk) = win 0x20ef00;
	void onMyLevels(cocos2d::CCObject*) = win 0x211bb0;
	void onUpdate(cocos2d::CCObject*) = win 0x20fa20;
	void loadPageFromUserInfo(GJUserScore* score) = win 0x210040;

	GJUserScore* m_score;
	int m_accountID;
	PAD = win 0x38, android 0x24;
	cocos2d::CCArray* m_buttons;
}

class PulseEffectAction : cocos2d::CCNode {
	static PulseEffectAction* createFromString(gd::string) = mac 0x179e90;
	void getSaveString() = mac 0x17a850;

	int group = mac 0x130;
}

class RateLevelDelegate {
	virtual void rateLevelClosed() {}
}

class RetryLevelLayer {
	static RetryLevelLayer* create() = mac 0x28dd60;
}

class ScrollingLayer : cocos2d::CCLayerColor {
	static ScrollingLayer* create(cocos2d::CCSize, cocos2d::CCPoint, float) = mac 0x41a900;
}

class SelectArtLayer {
	static SelectArtLayer* create(SelectArtType type) = win 0x174b00;
}

class SetGroupIDLayer : FLAlertLayer, TextInputDelegate {
	void onNextGroupID1(cocos2d::CCObject*) = mac 0x1967a0;
	void textChanged(CCTextInputNode*) = mac 0x197af0;
	void updateGroupIDLabel() = mac 0x197260, win 0x22e450;
	~SetGroupIDLayer() = mac 0x194410;
	void updateEditorLayerID() = win 0x22e0b0;
	void updateEditorLayerID2() = win 0x22e110;
	void updateZOrder() = win 0x22e3d0;

	GameObject* m_obj;
	cocos2d::CCArray* m_objs;
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
	static SetIDLayer* create(GameObject*) = mac 0x168f20;
}

class SetIDPopup {}

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
	static SetupOpacityPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x32b70;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x340a0;
	void textChanged(CCTextInputNode*) = mac 0x34a60;
	void updateTargetID() = mac 0x34760;
}

class SetupPickupTriggerPopup : FLAlertLayer {
	static SetupPickupTriggerPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x35e70;
	void onItemIDArrow(cocos2d::CCObject*) = mac 0x37100;
	void onNextItemID(cocos2d::CCObject*) = mac 0x37260;
	void textChanged(CCTextInputNode*) = mac 0x37ca0;
	void updateItemID() = mac 0x37ab0;

	PAD = win 0xc;
	CCTextInputNode* m_countInput;
}

class SetupPulsePopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
	virtual void colorValueChanged(cocos2d::ccColor3B color) = win 0x242660;

	bool init(EffectGameObject* triggerObj, cocos2d::CCArray* triggerObjs) = win 0x23e980;
	void updateColorValue() = win 0x2426b0;
	void onSelectPulseMode(cocos2d::CCObject*) = win 0x241420;
	void updatePulseMode() = win 0x242cf0;


	cocos2d::extension::CCControlColourPicker* m_colorPicker;
	PAD = win 0x30;
	cocos2d::CCSprite* m_currentColorSpr;
	cocos2d::CCSprite* m_prevColorSpr;
	PAD = win 0x64;
	int m_pulseMode;
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
	void setSecondColor(cocos2d::ccColor3B const& color) {
	    
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
	void setFrames(char const* firstLayer, char const* secondLayer, char const* birdDome, char const* outlineSprite, char const* detailSprite) = win 0x12c9e0;
	void setColor(cocos2d::ccColor3B const& color) = mac 0x1bc9b0, win 0x12c410;
	void setOpacity(unsigned char opacity) = win 0x12cb90;

	cocos2d::CCSprite* m_firstLayer;
	cocos2d::CCSprite* m_secondLayer;
	cocos2d::CCSprite* m_birdDome;
	cocos2d::CCSprite* m_outlineSprite;
	cocos2d::CCSprite* m_detailSprite;
	GJRobotSprite* m_robotSprite;
	GJSpiderSprite* m_spiderSprite;
	PAD = win 0x4;
	bool m_hasGlowOutline;
}

class Slider : cocos2d::CCLayer {
	void setValue(float val) {
	    this->m_touchLogic->getThumb()->setValue(val);
	}
	void setBarVisibility(bool v) {
	    this->m_sliderBar->setVisible(v);
	}
	static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler click, float scale) {
	    return create(target, click, nullptr, "slidergroove.png", "sliderthumb.png", "sliderthumbsel.png", scale);
	}

	float getValue() = mac 0x18e0c0, win 0x2e970;
	void updateBar() = mac 0x18d970, win 0x2ea10;
	static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler click, char const* unused, char const* grooveSpr, char const* thumbSpr, char const* thumbSprSel, float scale) = mac 0x18dd80, win 0x2e6e0, ios 0x2113f4;

	SliderTouchLogic* m_touchLogic;
	cocos2d::CCSprite* m_sliderBar;
	cocos2d::CCSprite* m_groove;
	float m_unknown;
	float m_height;
}

class SliderThumb : cocos2d::CCMenuItemImage {
	void setValue(float val) = mac 0x18ce80, win 0x2e1b0, ios 0x210db4;
	float getValue() {
		return (m_fScaleX * this->m_length * .5f +
				(m_vertical ?
					this->getPositionY() : 
					this->getPositionX())
			) / (m_fScaleX * this->m_length);
	}

	float m_length;
	bool m_vertical;
}

class SliderTouchLogic : cocos2d::CCMenu {
	SliderThumb* getThumb() const { return m_thumb; }

	PAD = win 0x4;
	float m_length;
	SliderThumb* m_thumb;
	Slider* m_slider;
	bool m_unknown;
	PAD = win 0x8;
	bool m_vertical;
}

class SongCell : TableViewCell {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
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
	~SongInfoObject() = mac 0x2f2040;
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x2f2c70;
	virtual bool canEncode() = mac 0x2f2da0;

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

class StartPosObject {}

class StatsCell {
	void updateBGColor(unsigned int index) = win 0x59cf0;
	void draw() = mac 0x11bf80, win 0x59d40;
}

class TableView : CCScrollLayerExt, CCScrollLayerExtDelegate {
	inline TableView() {}
	inline TableView(cocos2d::CCRect rect) : CCScrollLayerExt(rect) {}

	static TableView* create(TableViewDelegate*, TableViewDataSource*, cocos2d::CCRect) = mac 0x37eb30, win 0x30ed0;
	void reloadData() = mac 0x37f970, win 0x317e0;

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

	inline TableViewCell(char const* p0, float p1, float p2) : m_unknownString(p0), m_width(p1), m_height(p2) {
		m_backgroundLayer = cocos2d::CCLayerColor::create(cocos2d::ccc4(0,0,0,0), m_width, m_height);
		addChild(m_backgroundLayer, -1);
		m_mainLayer = cocos2d::CCLayer::create();
		addChild(m_mainLayer);
		// = mac 0x383de0, win 0x32e70;
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
	PAD = win 0x4;
	TeleportPortalObject* m_orangePortal;
	bool m_unk470;
	float m_unk474;
	bool m_unk478;
}

class TextAlertPopup {
	static TextAlertPopup* create(gd::string const& text, float time, float scale) = win 0x1450b0;
}

class TextArea : cocos2d::CCSprite {
	// https://www.youtube.com/watch?v=1LVW7IUyKMg
	TextArea() = mac 0x19fba0, win 0x33110, ios 0x92c28;
	inline TextArea(TextArea const&) : m_fontFile() {}
	inline ~TextArea() {}
	virtual void draw() = mac 0x19f890;
	virtual void setOpacity(unsigned char) = mac 0x19f760;
	bool init(gd::string str, char const* font, float width, float height, cocos2d::CCPoint anchor, float scale, bool disableColor) = mac 0x19ec70, win 0x33370, ios 0x92444;
	static TextArea* create(gd::string const& str, char const* font, float width, float height, cocos2d::CCPoint const& anchor, float scale, bool disableColor) {
		auto ret = new TextArea();
	    if (ret->init(str, font, width, height, anchor, scale, disableColor)) {
	        ret->autorelease();
	        return ret;
	    }
	    CC_SAFE_DELETE(ret);
	    return nullptr;
	}
	void colorAllCharactersTo(cocos2d::ccColor3B color) = win 0x33830;
	void setString(gd::string str) = mac 0x19eda0, win 0x33480;

	bool m_disableColor;			// 0x1e4
	MultilineBitmapFont* m_label;	// 0x1e8
	float m_width;					// 0x1ec
	int m_unknown;					// 0x1f0
	gd::string m_fontFile;			// 0x1f4
	float m_height;					// 0x20c
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
	static TopArtistsLayer* create() = mac 0x192a90;
	void setupLeaderboard(cocos2d::CCArray*) = mac 0x193420;
	virtual bool init() = mac 0x192c30;
	void loadPage(int) = mac 0x193b60;
	void setupPageInfo(gd::string, char const*) = mac 0x193730;

	cocos2d::CCNode* unknown = mac 0x220;
	GJCommentListLayer* commentLayer = mac 0x260;
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
	void disableMenu() = mac 0x280960;
	void enableMenu() = mac 0x280940;
	void pCommand(cocos2d::CCNode*) = mac 0x280830;
	void toggleCheckpointsMenu(bool) = mac 0x280430;
	void onCheck(cocos2d::CCObject* pSender) = mac 0x2803e0, win 0x25fb60;
	void onDeleteCheck(cocos2d::CCObject* pSender) = mac 0x280410, win 0x25fc90;
	void onPause(cocos2d::CCObject* pSender) = mac 0x2803c0, win 0x25fad0;
	virtual void keyDown(cocos2d::enumKeyCodes key) = mac 0x280470, win 0x25f890;
	virtual void keyUp(cocos2d::enumKeyCodes key) = mac 0x280600, win 0x25fa10;
	UILayer() = win 0x25f230;
	~UILayer() = win 0x25fef0;

	void* m_keyboardDelegateVtable; // rob is silly so he subclasses cckeyboarddelegate twice
	PAD = mac 0x8, win 0x4, android 0x4;
	cocos2d::CCMenu* m_checkPointMenu;
}

class UndoObject : cocos2d::CCObject {
	~UndoObject() = mac 0xa2fd0;
	static UndoObject* create(GameObject*, UndoCommand) = mac 0x94ea0, win 0x16bc20;
	static UndoObject* createWithArray(cocos2d::CCArray*, UndoCommand) = mac 0x96ee0, win 0x16bee0;
	static UndoObject* createWithTransformObjects(cocos2d::CCArray* pObjects, UndoCommand nCommand) = win 0x16bcd0;

	GameObject* m_gameObject;
	UndoCommand m_command;
	cocos2d::CCArray* m_objects;
	bool m_redo;
}

class UploadActionDelegate {
	virtual void uploadActionFinished(int, int) {}
	virtual void uploadActionFailed(int, int) {}
}

class UploadMessageDelegate {}

class UploadPopupDelegate {
	virtual void onClosePopup() {}
}

class UserInfoDelegate {}

class VideoOptionsLayer : FLAlertLayer {
	PAD = win 0x18;
	cocos2d::CCArray* m_resolutions;
	PAD = win 0x4;
	int m_currentResolution;
}
