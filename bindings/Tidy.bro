class AccountHelpLayer : GJDropDownLayer, GJAccountDelegate, FLAlertLayerProtocol {
	virtual void customSetup() = mac 0x83e80, ios 0xdee48;
	virtual void layerHidden() = mac 0x84b60, ios 0xdf960;
	virtual bool accountStatusChanged() = mac 0x84a20, ios 0xdf8ac;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x84a50, ios 0xdf8b8;
	void create();
	void doUnlink();
	void exitLayer();
	void onAccountManagement(cocos2d::CCObject* p0);
	void onReLogin(cocos2d::CCObject* p0);
	void onUnlink(cocos2d::CCObject* p0);
	void updatePage();
	void verifyUnlink();
	~AccountHelpLayer() = mac 0x83b50, ios 0xdedfc;
}

class AccountLayer : GJDropDownLayer, GJAccountDelegate, GJAccountBackupDelegate, GJAccountSyncDelegate, FLAlertLayerProtocol {
	virtual void customSetup() = mac 0x81c20, ios 0xdd4d0;
	virtual void layerHidden() = mac 0x83950, ios 0xdec24;
	virtual bool backupAccountFinished() = mac 0x830e0, ios 0xde5d0;
	virtual bool backupAccountFailed(BackupAccountError p0) = mac 0x832d0, ios 0xde748;
	virtual bool syncAccountFinished() = mac 0x83490, ios 0xde8b0;
	virtual bool syncAccountFailed(BackupAccountError p0) = mac 0x83630, ios 0xdea1c;
	virtual bool accountStatusChanged() = mac 0x830b0, ios 0xde5bc;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x83830, ios 0xdeb88;
	void create();
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4);
	void doBackup();
	void doSync();
	void exitLayer();
	void hideLoadingUI();
	void onBackup(cocos2d::CCObject* p0);
	void onHelp(cocos2d::CCObject* p0);
	void onLogin(cocos2d::CCObject* p0);
	void onMore(cocos2d::CCObject* p0);
	void onRegister(cocos2d::CCObject* p0);
	void onSync(cocos2d::CCObject* p0);
	void showLoadingUI();
	void toggleUI(bool p0);
	void updatePage(bool p0);
	~AccountLayer() = mac 0x81b80, ios 0xdd484;
}

class AccountLoginLayer : FLAlertLayer, TextInputDelegate, GJAccountLoginDelegate, FLAlertLayerProtocol {
	virtual void registerWithTouchDispatcher() = mac 0x25d7e0, ios 0x434d0;
	virtual void keyBackClicked() = mac 0x25d6a0, ios 0x43380;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x25d4d0, ios 0x43288;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0x25d9c0, ios 0x43650;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x25d820, ios 0x43508;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x25d9e0, ios 0x43658;
	virtual bool loginAccountFinished(int p0, int p1) = mac 0x25ce20, ios 0x42d90;
	virtual bool loginAccountFailed(AccountError p0) = mac 0x25d190, ios 0x42fc4;
	void create(gd::string p0);
	void createTextBackground(cocos2d::CCPoint p0, char const* p1, cocos2d::CCSize p2);
	void createTextInput(cocos2d::CCPoint p0, cocos2d::CCSize p1, char const* p2, int p3);
	void createTextLabel(cocos2d::CCPoint p0, char const* p1, cocos2d::CCSize p2);
	void disableNodes();
	void hideLoadingUI();
	bool init(gd::string p0);
	void onClose(cocos2d::CCObject* p0);
	void onForgotPassword(cocos2d::CCObject* p0);
	void onForgotUser(cocos2d::CCObject* p0);
	void onSubmit(cocos2d::CCObject* p0);
	void resetLabel(int p0);
	void resetLabels();
	void showLoadingUI();
	void toggleUI(bool p0);
	void updateLabel(AccountError p0);
	~AccountLoginLayer() = mac 0x25b7e0, ios 0x41ea0;
}

class AccountRegisterLayer : FLAlertLayer, TextInputDelegate, GJAccountRegisterDelegate, FLAlertLayerProtocol {
	virtual bool init() = mac 0x2575d0, ios 0x3f908;
	virtual void registerWithTouchDispatcher() = mac 0x25a910, ios 0x415f8;
	virtual void keyBackClicked() = mac 0x25a730, ios 0x414b4;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x25a380, ios 0x41350;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0x25b420, ios 0x41c94;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x25a950, ios 0x41630;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x25b440, ios 0x41c9c;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x25b460, ios 0x41ca4;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x25b520, ios 0x41d10;
	virtual bool allowTextInput(CCTextInputNode* p0) = mac 0x25b5d0, ios 0x41d70;
	virtual void registerAccountFinished() = mac 0x259bf0, ios 0x40d64;
	virtual void registerAccountFailed(AccountError p0) = mac 0x259d70, ios 0x40f00;
	void create();
	void createTextBackground(cocos2d::CCPoint p0, cocos2d::CCSize p1);
	void createTextInput(cocos2d::CCPoint p0, cocos2d::CCSize p1, gd::string p2, int p3);
	void createTextLabel(cocos2d::CCPoint p0, gd::string p1, cocos2d::CCSize p2);
	void disableNodes();
	void hideLoadingUI();
	void onClose(cocos2d::CCObject* p0);
	void onSubmit(cocos2d::CCObject* p0);
	void resetLabel(int p0);
	void resetLabels();
	void showLoadingUI();
	void toggleUI(bool p0);
	void updateLabel(AccountError p0);
	void validEmail(gd::string p0);
	void validPassword(gd::string p0);
	void validUser(gd::string p0);
	~AccountRegisterLayer() = mac 0x257330, ios 0x3f7d0;
}

class AchievementBar : cocos2d::CCNodeRGBA {
	static AchievementBar* create(char const* title, char const* desc, char const* icon, bool quest) = mac 0x379f80, win 0x3b120, ios 0x1a4784;
	virtual void setOpacity(unsigned char p0) = mac 0x37b9b0, ios 0x1a5fc4;
	bool init(char const* p0, char const* p1, char const* p2, bool p3);
	void show();
	~AchievementBar() = mac 0x379f40, ios 0x1a4748;

	PAD = win 0x24;
}

class AchievementCell : TableViewCell {
	void loadFromDict(cocos2d::CCDictionary* p0) = mac 0x10eaa0;
	virtual bool init() = mac 0x117730, ios 0xa1650;
	virtual void draw() = mac 0x117740, ios 0xa16c8;
	void updateBGColor(int p0);
	~AchievementCell() = mac 0x120490;
}

class AchievementManager : cocos2d::CCNode {
	void getAllAchievements() = mac 0x434d60;
	static AchievementManager* sharedState() = mac 0x424420, win 0x7b10;
	virtual bool init() = mac 0x4244c0, ios 0x74d04;
	void achievementForUnlock(int p0, UnlockType p1);
	void areAchievementsEarned(cocos2d::CCArray* p0);
	void checkAchFromUnlock(char const* p0);
	void dataLoaded(DS_Dictionary* p0);
	void encodeDataTo(DS_Dictionary* p0);
	void firstSetup();
	void getAchievementRewardDict();
	void getAchievementsWithID(char const* p0);
	void getAllAchievementsSorted();
	void isAchievementEarned(char const* p0);
	void notifyAchievement(char const* p0, char const* p1, char const* p2);
	void notifyAchievementWithID(char const* p0);
	void percentForAchievement(char const* p0);
	void percentageForCount(int p0, int p1);
	void reportAchievementWithID(char const* p0, int p1, bool p2);
	void reportPlatformAchievementWithID(char const* p0, int p1);
	void resetAchievement(char const* p0);
	void resetAchievements();
	void setReportedAchievements(cocos2d::CCDictionary* p0);
	void setup();
	void storeAchievementUnlocks();
	~AchievementManager() = mac 0x434b90, ios 0x80f0c;

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
	void willSwitchToScene(cocos2d::CCScene* p0) = mac 0x4650b0, win 0xfea0;
	void showNextAchievement() = mac 0x464f10, win 0xfd60, ios 0x13f508;
	virtual bool init() = mac 0x464e90, ios 0x13f488;
	void achievementDisplayFinished();
	~AchievementNotifier() = mac 0x4651c0, ios 0x13f764;

	cocos2d::CCScene* m_currentScene;
	cocos2d::CCArray* m_queue;
	AchievementBar* m_currentAchievement;
}

class AchievementsLayer : GJDropDownLayer {
	void customSetup() = mac 0x1bdea0, ios 0x2dda20;
	void loadPage(int p0) = mac 0x1be190, win 0x3c600;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x1be380, ios 0x2de1a0;
	void create();
	void onNextPage(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void setupLevelBrowser(cocos2d::CCArray* p0);
	void setupPageInfo(int p0, int p1, int p2);
	~AchievementsLayer() = mac 0x1bde00, ios 0x2dd9d4;
}

class AnimatedGameObject : GameObject, AnimatedSpriteDelegate, SpritePartDelegate {
	void playAnimation(int p0) = mac 0xc93d0;
	void updateChildSpriteColor(cocos2d::ccColor3B p0) = mac 0xc8450;
	virtual void setOpacity(unsigned char p0) = mac 0xc8640, ios 0x2770fc;
	virtual void resetObject() = mac 0xc9720, ios 0x2782a4;
	virtual void activateObject() = mac 0xc84d0, ios 0x276fec;
	virtual void deactivateObject(bool p0) = mac 0xc85e0, ios 0x2770ac;
	virtual void setObjectColor(cocos2d::ccColor3B const& p0) = mac 0xc8720, ios 0x2771c8;
	virtual void animationFinished(char const* p0) = mac 0xc8750, ios 0x2771fc;
	virtual cocos2d::CCSpriteFrame* displayFrameChanged(cocos2d::CCObject* p0, gd::string p1) = mac 0xc9160, ios 0x277c58;
	void animationForID(int p0, int p1);
	void create(int p0);
	void getTweenTime(int p0, int p1);
	bool init(int p0);
	void setupAnimatedSize(int p0);
	void setupChildSprites();
	~AnimatedGameObject() = mac 0xc79b0, ios 0x2765bc;
}

class AnimatedShopKeeper : CCAnimatedSprite {
	void animationFinished(char const* p0) {}

	static AnimatedShopKeeper* create(ShopType type) = win 0x14c4d0;
	void startAnimating() = win 0x14c690;
	bool init(ShopType p0);
	~AnimatedShopKeeper() = mac 0x1a3040, ios 0xcba10;

	float m_unknown1;
	float m_unknown2;
	bool m_unknown;
}

class AnimatedSpriteDelegate {
	virtual void animationFinished(char const* p0) {}
}

class AppDelegate : cocos2d::CCApplication, cocos2d::CCSceneDelegate {
	void bgScale() = mac 0x3aaab0;
	virtual bool applicationDidFinishLaunching() = mac 0x3aa900, win 0x3cbb0, ios 0x1a23f8;
	virtual void applicationDidEnterBackground() = mac 0x3aabe0, win 0x3cf40, ios 0x1a267c;
	virtual void applicationWillEnterForeground() = mac 0x3aac80, win 0x3d130, ios 0x1a2710;
	virtual void applicationWillBecomeActive() = mac 0x3aab30, win 0x3ce90;
	virtual void applicationWillResignActive() = mac 0x3aab50, win 0x3d3e0;
	virtual void trySaveGame() = mac 0x3aaf10, win 0x3d5e0, ios 0x1a28f0;
	virtual void willSwitchToScene(cocos2d::CCScene* p0) = mac 0x3aaf40, win 0x3d690, ios 0x1a28cc;
	static AppDelegate* get() = mac 0x3aab10;
	void checkSound();
	void hideLoadingCircle();
	void loadingIsFinished();
	void musicTest();
	void pauseGame();
	void platformShutdown();
	void resumeSound();
	void setIdleTimerDisabled(bool p0);
	void setupGLView();
	void showLoadingCircle(bool p0, bool p1, bool p2);
	~AppDelegate();

	cocos2d::CCScene* m_runningScene;
}

class ArtistCell : TableViewCell {
	void draw() = mac 0x11c980, ios 0xa56fc;
	virtual bool init() = mac 0x11c7c0, ios 0xa5588;
	void loadFromObject(SongInfoObject* p0) = mac 0x1118b0;
	void onNewgrounds(cocos2d::CCObject* p0) = mac 0x11c7e0;
	void updateBGColor(int p0) = mac 0x110460, win 0x5c6b0;
	void onYouTube(cocos2d::CCObject* p0);
	~ArtistCell() = mac 0x11c6a0, ios 0xa553c;

	void* m_idk;
}

class AudioEffectsLayer : cocos2d::CCLayerColor {
	void audioStep(float p0) = mac 0x271f40;
	static AudioEffectsLayer* create(gd::string p0) = mac 0x271a00;
	void resetAudioVars() = mac 0x271ee0;
	virtual void draw() = mac 0x2722a0, ios 0x2a5980;
	virtual void updateTweenAction(float p0, char const* p1) = mac 0x272170, ios 0x2a5834;
	void getBGSquare();
	void goingDown();
	bool init(gd::string p0);
	void triggerEffect(float p0);
	~AudioEffectsLayer() = mac 0x271750, ios 0x2a50cc;
}

class BitmapFontCache : cocos2d::CCObject {
	void fontWithConfigFile(char const* p0, float p1);
	bool init();
	void purgeSharedFontCache();
	void sharedFontCache();
	~BitmapFontCache() = mac 0xf86b0, ios 0x211aec;
}

class BonusDropdown : cocos2d::CCNode {
	void create(gd::string p0, int p1);
	bool init(gd::string p0, int p1);
	void show();
	~BonusDropdown();
}

class BoomListLayer : cocos2d::CCLayerColor {
	void create(BoomListView* p0, char const* p1);
	bool init(BoomListView* p0, char const* p1);
	~BoomListLayer() = mac 0x245f00;
}

class BoomListView : cocos2d::CCLayer, TableViewDelegate, TableViewDataSource {
	inline ~BoomListView() {
	    CC_SAFE_RELEASE(m_entries);
	}
	inline bool init(cocos2d::CCArray* entries, BoomListType type, float width, float height) {
		return this->init(entries, height, width, 0, type);
	}

	virtual void didSelectRowAtIndexPath(CCIndexPath& p0, TableView* p1) {}

	static BoomListView* create(cocos2d::CCArray* p0, float p1, float p2, int p3, BoomListType p4) = mac 0x18ecb0;
	bool init(cocos2d::CCArray* p0, float p1, float p2, int p3, BoomListType p4) = mac 0x18ee00, win 0x10c20, ios 0x12c5a8;
	virtual void draw() = mac 0x18f790, ios 0x12cc7c;
	virtual void setupList() = mac 0x18ef90, win 0x10dc0, ios 0x12c710;
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath& p0, TableViewCell* p1, TableView* p2) = mac 0x18f030, ios 0x12c798;
	virtual float cellHeightForRowAtIndexPath(CCIndexPath& p0, TableView* p1) = mac 0x18f070, win 0x10e50, ios 0x12c7a8;
	virtual int numberOfRowsInSection(unsigned int p0, TableView* p1) = mac 0x18f0b0, win 0x10e60, ios 0x12c7c0;
	virtual unsigned int numberOfSectionsInTableView(TableView* p0) = mac 0x18f0e0, win 0x10a70, ios 0x12c7d0;
	virtual TableViewCell* cellForRowAtIndexPath(CCIndexPath& p0, TableView* p1) = mac 0x18f100, win 0x10e70, ios 0x12c7e0;
	virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView* p0, TableViewCellEditingStyle p1, CCIndexPath& p2) = mac 0x18f770, ios 0x12cc74;
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath& p0, TableViewCell* p1, TableView* p2) = mac 0x18f050, ios 0x12c7a0;
	virtual TableViewCell* getListCell(char const* p0) = mac 0x18f200, win 0x10ed0, ios 0x12c86c;
	virtual void loadCell(TableViewCell* p0, unsigned int p1) = mac 0x18f4a0, win 0x10ff0;
	virtual void loadCell(TableViewCell* p0, int p1) = mac 0x18f4a0, ios 0x12c968;

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
	virtual void visit() = mac 0x1e2f80, ios 0xbc5d4;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e3c40, ios 0xbccec;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e3d90, ios 0xbcdf8;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e3ff0, ios 0xbd000;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e3b40, ios 0xbcca0;
	virtual void registerWithTouchDispatcher() = mac 0x1e3970, ios 0xbcb30;
	void addPage(cocos2d::CCLayer* p0);
	void addPage(cocos2d::CCLayer* p0, int p1);
	void cancelAndStoleTouch(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	void claimTouch(cocos2d::CCTouch* p0);
	void create(cocos2d::CCArray* p0, int p1, bool p2);
	void create(cocos2d::CCArray* p0, int p1, bool p2, cocos2d::CCArray* p3, DynamicScrollDelegate* p4);
	void getPage(int p0);
	void getRelativePageForNum(int p0);
	void getRelativePosForPage(int p0);
	void getTotalPages();
	bool init(cocos2d::CCArray* p0, int p1, bool p2, cocos2d::CCArray* p3, DynamicScrollDelegate* p4);
	void moveToPageEnded();
	void pageNumberForPosition(cocos2d::CCPoint p0);
	void positionForPageWithNumber(int p0);
	void postVisit();
	void preVisitWithClippingRect(cocos2d::CCRect p0);
	void quickUpdate();
	void removePage(cocos2d::CCLayer* p0);
	void removePageWithNumber(int p0);
	void repositionPagesLooped();
	void selectPage(int p0);
	void setDotScale(float p0);
	void setPagesIndicatorPosition(cocos2d::CCPoint p0);
	void setupDynamicScrolling(cocos2d::CCArray* p0, DynamicScrollDelegate* p1);
	void togglePageIndicators(bool p0);
	void updateDots(float p0);
	void updatePages();
	~BoomScrollLayer() = mac 0x1e2d40, ios 0xbc49c;

	cocos2d::CCArray* m_dots;
	PAD = win 0x40;
	ExtendedLayer* m_layer;
	PAD = win 0x48;
	int m_page;
}

class BoomScrollLayerDelegate {
	virtual void scrollLayerScrollingStarted(BoomScrollLayer* p0) {}
	virtual void scrollLayerScrolledToPage(BoomScrollLayer* p0, int p1) {}
	virtual void scrollLayerMoved(cocos2d::CCPoint p0) {}
	virtual void scrollLayerWillScrollToPage(BoomScrollLayer* p0, int p1) {}
}

class ButtonPage : cocos2d::CCLayer {
	void create(cocos2d::CCArray* p0, cocos2d::CCPoint p1, int p2, float p3);
	bool init(cocos2d::CCArray* p0, cocos2d::CCPoint p1, int p2, float p3);
	~ButtonPage();
}

class ButtonSprite : cocos2d::CCSprite {
	inline static ButtonSprite* create(char const* caption) {
		return ButtonSprite::create(caption, 0, 0, "goldFont.fnt", "GJ_button_01.png", .0f, 1.f);
	}
	inline static ButtonSprite* create(char const* caption, char const* font, char const* texture, float scale = 1.f) {
		return ButtonSprite::create(caption, 0, 0, font, texture, .0f, scale);
	}

	static ButtonSprite* create(char const* caption, int width, bool absolute, char const* font, char const* texture, float height, float scale) {
		return create(caption, width, 0, scale, absolute, font, texture, height);
	}

	static ButtonSprite* create(char const* caption, int width, int unknown, float scale, bool absoluteWidth, char const* font, char const* texture, float height) = mac 0x4fa90, win 0x137d0, ios 0x38c7c;
	static ButtonSprite* create(char const* p0, int p1, int p2, float p3, bool p4) = mac 0x4fa40;
	void updateBGImage(char const* p0) = mac 0x502d0, win 0x13af0;
	static ButtonSprite* create(char const* p0, float p1) = mac 0x4fa60;
	void setString(char const* txt) = win 0x14030;
	void updateSpriteBGSize(cocos2d::CCPoint const& offset) = win 0x13c00;
	void setColor(cocos2d::ccColor3B color) = mac 0x50410, win 0x143e0;
	void create(char const* p0);
	void create(char const* p0, int p1, int p2, float p3, bool p4, char const* p5, char const* p6);
	void create(cocos2d::CCSprite* p0);
	void create(cocos2d::CCSprite* p0, int p1, int p2, float p3, float p4, bool p5);
	void create(cocos2d::CCSprite* p0, int p1, int p2, float p3, float p4, bool p5, char const* p6, bool p7);
	bool init(char const* p0, int p1, int p2, float p3, bool p4, char const* p5, char const* p6, float p7);
	bool init(cocos2d::CCSprite* p0, int p1, int p2, float p3, float p4, bool p5, char const* p6, bool p7);
	void updateSpriteBGSize();
	void updateSpriteOffset(cocos2d::CCPoint p0);
	~ButtonSprite();

	PAD = win 0x18;
	cocos2d::CCLabelBMFont* m_label;
	cocos2d::CCSprite* m_subSprite;
	cocos2d::CCSprite* m_subBGSprite;
	cocos2d::extension::CCScale9Sprite* m_BGSprite;
	PAD = win 0x8;
	cocos2d::CCPoint m_spritePosition;
}

class CCAlertCircle : cocos2d::CCNode {
	virtual bool init() = mac 0xbdea0;
	virtual void draw() = mac 0xbdf40;
	void create();
	~CCAlertCircle() = mac 0xbe050;
}

class CCAnimateFrameCache : cocos2d::CCObject {
	static CCAnimateFrameCache* sharedSpriteFrameCache() = win 0x158f0;
	void addSpriteFramesWithFile(char const* file) = win 0x159b0;
	void addCustomSpriteFramesWithFile(char const* p0);
	void addDict(DS_Dictionary* p0, char const* p1);
	void addDict(cocos2d::CCDictionary* p0, char const* p1);
	bool init();
	void purgeSharedSpriteFrameCache();
	void removeSpriteFrames();
	void spriteFrameByName(char const* p0);
	~CCAnimateFrameCache() = mac 0x2e4f90, ios 0x114ef8;
}

class CCAnimatedSprite : cocos2d::CCSprite {
	void runAnimation(gd::string p0) = mac 0x1a6430, win 0x14f60;
	void tweenToAnimation(gd::string p0, float p1) = mac 0x1a65b0;
	static CCAnimatedSprite* create(char const* file) = win 0x14540;
	virtual void setOpacity(unsigned char p0) = mac 0x1a6af0, ios 0x21c8ec;
	virtual void animationFinished(char const* p0) = mac 0x1a6ad0, ios 0x21c8d4;
	virtual void animationFinishedO(cocos2d::CCObject* p0) = mac 0x1a6aa0, ios 0x21c890;
	void cleanupSprite();
	void createWithType(char const* p0);
	bool initWithType(char const* p0);
	void loadType(char const* p0);
	void runAnimationForced(gd::string p0);
	void setColor(cocos2d::ccColor3B p0);
	void stopTween();
	void switchToMode(spriteMode p0);
	void tweenToAnimationFinished();
	void willPlayAnimation();
	~CCAnimatedSprite() = mac 0x1a6db0, ios 0x21ca94;

	gd::string m_unknown1;
	gd::string m_unknown2;
	PAD = win 0x14;
	gd::string m_unknown3;
	PAD = win 0x4;
}

class CCBlockLayer : cocos2d::CCLayerColor {
	void disableUI() = mac 0x2a5a80, ios 0x45554;
	void draw() = mac 0x2a5c20, ios 0x456c4;
	void enableUI() = mac 0x2a5a90, ios 0x45558;
	void enterAnimFinished() = mac 0x2a5bb0, ios 0x45660;
	void enterLayer() = mac 0x2a5aa0, ios 0x4555c;
	void exitLayer() = mac 0x2a5b40, ios 0x455e8;
	void hideLayer(bool p0) = mac 0x2a5ba0, ios 0x45654;
	virtual bool init() = mac 0x2a59c0, ios 0x45490;
	void layerHidden() = mac 0x2a5be0, ios 0x45674;
	void layerVisible() = mac 0x2a5bc0, ios 0x45664;
	void registerWithTouchDispatcher() = mac 0x2a5ad0, ios 0x45594;
	void showLayer(bool p0) = mac 0x2a5b90, ios 0x45648;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2a5c50, ios 0x45700;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2a5cb0, ios 0x45720;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2a5c70, ios 0x45710;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2a5c90, ios 0x45718;
	virtual void keyBackClicked() = mac 0x2a5b10, ios 0x455cc;
	virtual void customSetup() = mac 0x2a5a70, ios 0x45550;
	void create();
	~CCBlockLayer() = mac 0x2a5620, ios 0x45444;

	bool m_unknown;
	bool m_unknown2;
}

class CCCircleAlert : CCCircleWave {
	void create(float p0, float p1, float p2);
	bool init(float p0, float p1, float p2);
	~CCCircleAlert();
}

class CCCircleWave : cocos2d::CCNode {
	static CCCircleWave* create(float p0, float p1, float p2, bool p3) = mac 0xbd270;
	static CCCircleWave* create(float p0, float p1, float p2, bool p3, bool p4) = mac 0xbd290, win 0x16c00;
	bool init(float p0, float p1, float p2, bool p3, bool p4) = mac 0xbd380;
	void followObject(cocos2d::CCNode* p0, bool p1) = mac 0xbd670, win 0x16f20;
	void updatePosition(float p0) = mac 0xbd630, win 0x16f00;
	void setPosition(cocos2d::CCPoint const& pos) = mac 0xbd600, win 0x16ed0, ios 0xe1e0c;
	void removeMeAndCleanup() = mac 0xbdac0, win 0x17280, ios 0xe2310;
	void draw() = mac 0xbd960, win 0x17100, ios 0xe21c8;
	void updateTweenAction(float dt, char const* key) = mac 0xbd710, win 0x16f90, ios 0xe1f44;
	void baseSetup(float p0);
	~CCCircleWave() = mac 0xbd1b0, ios 0xe1b50;

	cocos2d::CCArray* m_children;
	PAD = win 0x4;
	float m_currentRadius;
	float m_currentOpacity;
	cocos2d::ccColor3B color;
	cocos2d::CCPoint m_circleCenter;
	int m_filled;
	int m_lineWidth;
	float m_opacityMultiplier;
	bool m_blendFuncDirty;
	CCCircleWaveDelegate* delegate;
}

class CCCircleWaveDelegate {
	virtual void circleWaveWillBeRemoved(CCCircleWave* wave) {}
}

class CCContentLayer : cocos2d::CCLayerColor {
	static CCContentLayer* create(cocos2d::ccColor4B const& color, float width, float height) = win 0x172a0;
	virtual void setPosition(cocos2d::CCPoint const& pos) = mac 0x464c60, win 0x17400, ios 0xf5440;
	~CCContentLayer() = mac 0x464cd0;
}

class CCCountdown : cocos2d::CCSprite {
	virtual bool init() = mac 0x468870;
	virtual void setOpacity(unsigned char p0) = mac 0x468c70;
	void create();
	void lapFinished();
	void startTimerWithSeconds(float p0, cocos2d::SEL_CallFunc p1, cocos2d::CCNode* p2);
	~CCCountdown() = mac 0x468cf0;
}

class CCCounterLabel : cocos2d::CCLabelBMFont {
	void calculateStepSize();
	void create(int p0, char const* p1, FormatterType p2);
	void disableCounter();
	void enableCounter();
	void fastUpdateCounter();
	void getTargetCount();
	bool init(int p0, char const* p1, FormatterType p2);
	void setTargetCount(int p0);
	void setupFormatter(FormatterType p0);
	void updateCounter(float p0);
	void updateString();
	~CCCounterLabel() = mac 0x29eaa0, ios 0x238c8;
}

class CCIndexPath : cocos2d::CCObject {
	static CCIndexPath* create(unsigned int idk1, int idk2) = win 0x30e40;
	void CCIndexPathWithSectionRow(int p0, int p1);
	~CCIndexPath();

	int m_unknown1;
	int m_unknown2;
}

class CCLightFlash : cocos2d::CCNode {
	static CCLightFlash* create() = mac 0x295870;
	void playEffect(cocos2d::CCPoint p0, cocos2d::ccColor3B p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11, float p12, float p13, float p14, float p15, int p16, bool p17, bool p18, float p19) = mac 0x295900, win 0x179f0;
	virtual bool init() = mac 0x2958f0, ios 0x1fdd0;
	void cleanupFlash();
	void fadeAndRemove();
	void removeLights();
	void showFlash();
	~CCLightFlash() = mac 0x296210, ios 0x20584;
}

class CCLightStrip : cocos2d::CCNode {
	virtual void draw() = mac 0x296410, ios 0x206dc;
	virtual void updateTweenAction(float p0, char const* p1) = mac 0x2965a0, ios 0x20834;
	void create(float p0, float p1, float p2, float p3, float p4);
	bool init(float p0, float p1, float p2, float p3, float p4);
	~CCLightStrip() = mac 0x2968b0;
}

class CCMenuItemSpriteExtra : cocos2d::CCMenuItemSprite {
	inline static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) {
		return CCMenuItemSpriteExtra::create(sprite, nullptr, target, callback);
	}

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

	static CCMenuItemSpriteExtra* create(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3) = mac 0x1253c0, win 0x18ee0, ios 0xe0740;
	void setSizeMult(float p0) = mac 0x1255e0, win 0x19080;
	CCMenuItemSpriteExtra() = mac 0x32670, win 0x18db0;
	~CCMenuItemSpriteExtra() = mac 0x125c00, win 0x18eb0, ios 0x13d0c0;
	bool init(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3) = mac 0x125450, win 0x18fa0;
	bool init(cocos2d::CCNode* spr) = win 0x18fa0;
	void activate() = mac 0x125730, win 0x191c0, ios 0x13cbd8;
	void selected() = mac 0x125840, win 0x19270, ios 0x13cd28;
	void unselected() = mac 0x125a70, win 0x19430, ios 0x13cf64;

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
	void setSizeMult(float p0) = mac 0x38a40, win 0x19850;
	void toggle(bool p0) = mac 0x38950, win 0x199b0, ios 0xf5848;
	bool init(cocos2d::CCNode* off, cocos2d::CCNode* on, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handler) = mac 0x38500, win 0x196e0;
	void activate() = mac 0x38bc0, win 0x198d0, ios 0xf59ac;
	void selected() = mac 0x38b80, win 0x198a0, ios 0xf596c;
	void unselected() = mac 0x38c00, win 0x19900, ios 0xf59ec;
	void setEnabled(bool enabled) = mac 0x38c40, win 0x19930, ios 0xf5a2c;
	void activeItem();
	void normalTouch(cocos2d::CCObject* p0);
	void selectedTouch(cocos2d::CCObject* p0);
	~CCMenuItemToggler() = mac 0x38c90, ios 0xf5a74;

	CCMenuItemSpriteExtra* m_offButton;
	CCMenuItemSpriteExtra* m_onButton;
	bool m_toggled;
	bool m_notClickable;
}

class CCMoveCNode : cocos2d::CCNode {
	static CCMoveCNode* create() = mac 0x1842a0;
	virtual bool init() = mac 0x18b3d0, ios 0x14c3c;
	~CCMoveCNode() = mac 0x18b2c0, ios 0x14c00;
}

class CCNodeContainer : cocos2d::CCNode {
	static CCNodeContainer* create() = mac 0xb1090, win 0x112370;
	virtual bool init() = mac 0xba950, win 0x33b40, ios 0x13aa24;
	void visit() = mac 0xba960, win 0x112420, ios 0x13aa28;
	~CCNodeContainer() = mac 0xba9b0;
}

class CCPartAnimSprite : cocos2d::CCSprite {
	virtual void setScaleX(float p0) = mac 0x1bff50, ios 0x3dd94;
	virtual void setScaleY(float p0) = mac 0x1bff70, ios 0x3ddbc;
	virtual void setScale(float p0) = mac 0x1bff30, ios 0x3dd6c;
	virtual void setOpacity(unsigned char p0) = mac 0x1c01d0, ios 0x3dfa8;
	virtual void setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x1bff90, ios 0x3dde4;
	virtual void setDisplayFrame(cocos2d::CCSpriteFrame* p0) = mac 0x1bfa40, ios 0x3d8d4;
	virtual bool isFrameDisplayed(cocos2d::CCSpriteFrame* p0) = mac 0x1bfc20, ios 0x3da6c;
	virtual cocos2d::CCSpriteFrame* displayFrame() = mac 0x1bfc40, ios 0x3da84;
	void changeTextureOfID(char const* p0, char const* p1);
	void countParts();
	void createWithAnimDesc(char const* p0, cocos2d::CCTexture2D* p1);
	void dirtify();
	void getSpriteForKey(char const* p0);
	bool initWithAnimDesc(char const* p0, cocos2d::CCTexture2D* p1);
	void setBlendFunc(cocos2d::_ccBlendFunc p0);
	void setColor(cocos2d::ccColor3B p0);
	void setFlipX(bool p0);
	void setFlipY(bool p0);
	void transformSprite(frameValues p0);
	void tweenSpriteTo(frameValues p0, float p1);
	void tweenToFrame(cocos2d::CCSpriteFrame* p0, float p1);
	~CCPartAnimSprite() = mac 0x1c04c0, ios 0x3e100;
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

	virtual void visit() = mac 0x236550, win 0x1bed0, ios 0x220018;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x235ef0, win 0x1b9b0, ios 0x21fa58;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x236300, win 0x1bce0, ios 0x21fdf0;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x236020, win 0x1baa0, ios 0x21fb64;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2362a0, win 0x1bcc0, ios 0x21fd94;
	virtual void registerWithTouchDispatcher() = mac 0x235eb0, win 0x1b980, ios 0x21fa20;
	virtual void preVisitWithClippingRect(cocos2d::CCRect p0) = mac 0x2366a0, win 0x1c000, ios 0x220154;
	virtual void postVisit() = mac 0x236720, win 0x1c090, ios 0x2201e8;
	void moveToTop() = mac 0x235870, win 0x1b4a0;
	void moveToTopWithOffset(float p0) = mac 0x2357d0, win 0x1b420;
	CCScrollLayerExt(cocos2d::CCRect rect) = mac 0x235130, win 0x1b020, ios 0x21f05c;
	void scrollLayer(float scroll) = mac 0x236490, win 0x1be20;
	void constraintContent();
	void scrollingEnd();
	void setContentLayerSize(cocos2d::CCSize p0);
	void setContentOffset(cocos2d::CCPoint p0, bool p1);
	void updateIndicators(float p0);
	~CCScrollLayerExt() = mac 0x2359b0, win 0x1b520, ios 0x21f7ac;

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
	virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt* p0) {}
	virtual void scrollViewDidEndDecelerating(CCScrollLayerExt* p0) {}
	virtual void scrollViewTouchMoving(CCScrollLayerExt* p0) {}
	virtual void scrollViewDidEndMoving(CCScrollLayerExt* p0) {}
	virtual void scrollViewTouchBegin(CCScrollLayerExt* p0) {}
	virtual void scrollViewTouchEnd(CCScrollLayerExt* p0) {}
}

class CCSpritePart : CCSpritePlus {
	void create(cocos2d::CCTexture2D* p0);
	void createWithSpriteFrameName(char const* p0);
	void frameChanged(gd::string p0);
	void getBeingUsed();
	void hideInactive();
	void resetTextureRect();
	void setBeingUsed(bool p0);
	void updateDisplayFrame(char const* p0);
	~CCSpritePart();
}

class CCSpritePlus : cocos2d::CCSprite {
	bool initWithSpriteFrameName(char const* p0) = mac 0x248670, win 0x1c1e0, ios 0x118ff4;
	void setScaleX(float scale) = mac 0x248950, win 0x1c440, ios 0x11925c;
	void setScaleY(float scale) = mac 0x2489d0, win 0x1c4c0, ios 0x1192d8;
	void setScale(float scale) = mac 0x248a50, win 0x1c540, ios 0x119354;
	void setPosition(cocos2d::CCPoint const& pos) = mac 0x2486b0, win 0x1c220, ios 0x119014;
	void setRotation(float rotation) = mac 0x248730, win 0x1c280, ios 0x119088;
	bool initWithTexture(cocos2d::CCTexture2D* texture) = mac 0x248690, win 0x1c200, ios 0x119000;
	void setFlipX(bool flip) = mac 0x2487b0, win 0x1c300, ios 0x1190fc;
	void setFlipY(bool flip) = mac 0x248880, win 0x1c3a0, ios 0x1191ac;
	static CCSpritePlus* createWithSpriteFrame(cocos2d::CCSpriteFrame* frame) = win 0x1c130;
	void addFollower(cocos2d::CCNode* p0);
	void createWithSpriteFrameName(char const* p0);
	void followSprite(CCSpritePlus* p0);
	void getFollower();
	void removeFollower(cocos2d::CCNode* p0);
	void stopFollow();
	~CCSpritePlus() = mac 0x248400, ios 0x118ee8;

	cocos2d::CCArray* m_followers;
	CCSpritePlus* m_followingSprite;
	bool m_hasFollower;
	bool m_propagateScaleChanges;
	bool m_propagateFlipChanges;
}

class CCSpriteWithHue : cocos2d::CCSprite {
	virtual void draw() = mac 0x35a400, ios 0x104384;
	virtual bool initWithTexture(cocos2d::CCTexture2D* p0) = mac 0x359d10, ios 0x103ff8;
	virtual bool initWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1) = mac 0x359d80, ios 0x104074;
	virtual bool initWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1, bool p2) = mac 0x359b80, ios 0x103ebc;
	virtual bool initWithSpriteFrame(cocos2d::CCSpriteFrame* p0) = mac 0x359da0, ios 0x104084;
	virtual void updateColor() = mac 0x359f70, ios 0x104148;
	void create(gd::string const& p0);
	void create(gd::string const& p0, cocos2d::CCRect const& p1);
	void createWithSpriteFrame(cocos2d::CCSpriteFrame* p0);
	void createWithSpriteFrameName(gd::string const& p0);
	void createWithTexture(cocos2d::CCTexture2D* p0);
	void createWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1, bool p2);
	void getAlpha();
	void getHue();
	void getUniformLocations();
	bool initShader();
	void setHue(float p0);
	void setHueDegrees(float p0);
	void setupDefaultSettings();
	void shaderBody();
	void updateColorMatrix();
	void updateHue(float p0);
	~CCSpriteWithHue() = mac 0x35a7c0;
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

	void setString(gd::string text) = mac 0x5d3e0, win 0x21070;
	bool init(float width, float height, char const* caption, char const* thonburi, int maxCharCount, char const* font) = mac 0x5d180, win 0x20e50, ios 0xe261c;
	void refreshLabel() = mac 0x5d730, win 0x21330, ios 0xe2af4;
	void updateLabel(gd::string p0) = mac 0x5d4a0;
	virtual void registerWithTouchDispatcher() = mac 0x5eec0, win 0x220e0, ios 0xe3f98;
	virtual void visit() = mac 0x5d380, win 0x21000, ios 0xe279c;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x5ec80, win 0x21f20, ios 0xe3e0c;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x5ee80, ios 0xe3f88;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x5ee60, ios 0xe3f80;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x5eea0, ios 0xe3f90;
	virtual void textChanged() = mac 0x5dd70, win 0x216e0, ios 0xe2fb4;
	virtual void onClickTrackNode(bool p0) = mac 0x5dd40, win 0x216b0, ios 0xe2f88;
	virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& p0) = mac 0x5dad0, win 0x21580, ios 0xe2da0;
	virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& p0) = mac 0x5dc20, win 0x21650, ios 0xe2ef8;
	virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* p0, char const* p1, int p2) = mac 0x5de50, ios 0xe30c0;
	virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF* p0) = mac 0x5e2c0, win 0x21b10, ios 0xe34e0;
	virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF* p0) = mac 0x5e610, win 0x21d60, ios 0xe373c;
	void create(float p0, float p1, char const* p2, char const* p3, int p4, char const* p5);
	void setMaxLabelWidth(float p0);
	void updateBlinkLabel();

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

class ChallengeNode : cocos2d::CCNode {
	void create(GJChallengeItem* p0, ChallengesPage* p1, bool p2);
	bool init(GJChallengeItem* p0, ChallengesPage* p1, bool p2);
	void onClaimReward(cocos2d::CCObject* p0);
	void updateTimeLabel(gd::string p0);
	~ChallengeNode() = mac 0x1dce50, ios 0x221f8;
}

class ChallengesPage : FLAlertLayer, FLAlertLayerProtocol, GJChallengeDelegate, CurrencyRewardDelegate {
	static ChallengesPage* create() = win 0x3e050;
	virtual bool init() = mac 0x1db520, ios 0x20e64;
	virtual void registerWithTouchDispatcher() = mac 0x1dcc50, ios 0x22014;
	virtual void keyBackClicked() = mac 0x1dcbc0, ios 0x21fc0;
	virtual void show() = mac 0x1dca10, ios 0x21e3c;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x1dcba0, ios 0x21fb8;
	virtual void challengeStatusFinished() = mac 0x1dc520, ios 0x21ad0;
	virtual void challengeStatusFailed() = mac 0x1dc830, ios 0x21cd4;
	virtual void currencyWillExit(CurrencyRewardLayer* p0) = mac 0x1dcdb0, ios 0x22174;
	void claimItem(ChallengeNode* p0, GJChallengeItem* p1, cocos2d::CCPoint p2);
	void createChallengeNode(int p0, bool p1, float p2, bool p3);
	void exitNodeAtSlot(int p0, float p1);
	void onClose(cocos2d::CCObject* p0);
	void tryGetChallenges();
	void updateDots();
	void updateTimers(float p0);
	~ChallengesPage() = mac 0x1db2b0, ios 0x20d3c;
}

class CheckpointObject : cocos2d::CCNode {
	static CheckpointObject* create() = mac 0x7e7d0;
	void getObject() = mac 0x7ef50;
	virtual bool init() = mac 0x80920, ios 0xb98c8;
	void getColorAction(ColorAction* p0, cocos2d::ccColor3B p1);
	void setObject(GameObject* p0);
	~CheckpointObject() = mac 0x808e0, ios 0xb988c;

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

class CollisionBlockPopup : FLAlertLayer, TextInputDelegate {
	static CollisionBlockPopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x130010;
	void onNextItemID(cocos2d::CCObject* p0) = mac 0x130e60;
	virtual void keyBackClicked() = mac 0x1317e0, ios 0x44cd0;
	virtual void show() = mac 0x131790, ios 0x44c84;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x131630, ios 0x44be8;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x131670, ios 0x44c04;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x1318f0, ios 0x44cdc;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x1319b0, ios 0x44d48;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onDynamicBlock(cocos2d::CCObject* p0);
	void onItemIDArrow(cocos2d::CCObject* p0);
	void updateEditorLabel();
	void updateItemID();
	void updateTextInputLabel();
	~CollisionBlockPopup() = mac 0x12ff70, ios 0x43b38;
}

class CollisionTriggerAction : cocos2d::CCNode {
	static CollisionTriggerAction* createFromString(gd::string p0) = mac 0x176ee0;
	void create(int p0, int p1, int p2, bool p3, bool p4);
	void getSaveString();
	bool init(int p0, int p1, int p2, bool p3, bool p4);
	~CollisionTriggerAction();
}

class ColorAction : cocos2d::CCNode {
	void getSaveString() = mac 0x17d080;
	void setupFromDict(cocos2d::CCDictionary* p0) = mac 0x17f310;
	void setupFromString(gd::string p0) = mac 0x17f270;
	void create(cocos2d::ccColor3B p0, bool p1, int p2);
	void create(cocos2d::ccColor3B p0, cocos2d::ccColor3B p1, float p2, double p3, bool p4);
	void create(cocos2d::ccColor3B p0, cocos2d::ccColor3B p1, float p2, double p3, bool p4, int p5, float p6, float p7);
	void create();
	void getCopy();
	bool init(cocos2d::ccColor3B p0, cocos2d::ccColor3B p1, float p2, double p3, bool p4, int p5, float p6, float p7);
	void resetAction();
	void step(float p0);
	void updateCustomColor(cocos2d::ccColor3B p0, cocos2d::ccColor3B p1);
	~ColorAction();

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
	virtual bool init() = mac 0x175370, ios 0x9b6c;
	void create();
	~ColorActionSprite() = mac 0x18b420;

	float m_opacity;
	cocos2d::ccColor3B m_f0124;
	cocos2d::ccColor3B m_activeColor;
}

class ColorChannelSprite : cocos2d::CCSprite {
	void updateBlending(bool p0) = mac 0x16e1d0, win 0x114710;
	void updateCopyLabel(int p0, bool p1) = mac 0x16ded0, win 0x114440;
	void updateOpacity(float p0) = mac 0x16e080, win 0x1145c0;
	void updateValues(ColorAction* p0) = mac 0x16e2e0, win 0x114800;
	static ColorChannelSprite* create() = mac 0x16de00, win 0x114340;
	virtual bool init() = mac 0x16deb0, ios 0x6e04;
	~ColorChannelSprite() = mac 0x18b3e0;

	cocos2d::CCLabelBMFont* m_copyLabel;
	cocos2d::CCLabelBMFont* m_opacityLabel;
	cocos2d::CCLabelBMFont* m_blendingDot;
}

class ColorSelectDelegate {
	virtual void colorSelectClosed(cocos2d::CCNode* p0) {}
}

class ColorSelectLiveOverlay : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x2e4550, ios 0x92180;
	virtual void show() = mac 0x2e4620, ios 0x9218c;
	void closeColorSelect(cocos2d::CCObject* p0);
	void colorValueChanged(cocos2d::ccColor3B p0);
	void create(ColorAction* p0, ColorAction* p1, EffectGameObject* p2);
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4);
	void createWithActions(ColorAction* p0, ColorAction* p1);
	void createWithObject(EffectGameObject* p0);
	void determineStartValues();
	void getColorValue();
	bool init(ColorAction* p0, ColorAction* p1, EffectGameObject* p2);
	void onSelectTab(cocos2d::CCObject* p0);
	void selectColor(cocos2d::ccColor3B p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void textChanged(CCTextInputNode* p0);
	void textInputClosed(CCTextInputNode* p0);
	void toggleControls(bool p0);
	void updateColorLabel();
	void updateColorValue();
	void updateOpacity();
	~ColorSelectLiveOverlay() = mac 0x2e24c0, ios 0x90b90;
}

class ColorSelectPopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
	virtual void colorValueChanged(cocos2d::ccColor3B color) = mac 0x423320, win 0x46ee0, ios 0x2dbb8c;
	bool init(EffectGameObject* triggerObj, cocos2d::CCArray* triggerObjs, ColorAction* colorAction) = win 0x43ae0;
	void updateColorValue() = win 0x46f30;
	void updateCopyColorTextInputLabel() = win 0x479c0;
	virtual void keyBackClicked() = mac 0x423540, ios 0x2dbcb4;
	virtual void show() = mac 0x423570, ios 0x2dbcc0;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x423820, ios 0x2dbf2c;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x423860, ios 0x2dbf48;
	virtual void colorSelectClosed(GJSpecialColorSelect* p0, int p1) = mac 0x423c80, ios 0x2dc1c4;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x423fa0, ios 0x2dc460;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x424060, ios 0x2dc4cc;
	void closeColorSelect(cocos2d::CCObject* p0);
	void create(ColorAction* p0);
	void create(EffectGameObject* p0, cocos2d::CCArray* p1);
	void create(EffectGameObject* p0, cocos2d::CCArray* p1, ColorAction* p2);
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	void getColorValue();
	void onCopy(cocos2d::CCObject* p0);
	void onCopyOpacity(cocos2d::CCObject* p0);
	void onDefault(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onPaste(cocos2d::CCObject* p0);
	void onPlayerColor1(cocos2d::CCObject* p0);
	void onPlayerColor2(cocos2d::CCObject* p0);
	void onSelectSpecialColor(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTintGround(cocos2d::CCObject* p0);
	void onToggleHSVMode(cocos2d::CCObject* p0);
	void onToggleTintMode(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void onUpdateCopyColor(cocos2d::CCObject* p0);
	void onUpdateCustomColor(cocos2d::CCObject* p0);
	void selectColor(cocos2d::ccColor3B p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void updateCopyColor();
	void updateCustomColorIdx();
	void updateDurLabel();
	void updateDuration();
	void updateEditorLabel();
	void updateHSVMode();
	void updateHSVValue();
	void updateMultiTriggerBtn();
	void updateOpacity();
	void updateOpacityLabel();
	void updateSpawnedByTrigger();
	void updateTextInputLabel();
	void updateTouchTriggered();
	~ColorSelectPopup() = mac 0x41ead0, ios 0x2d816c;

	cocos2d::extension::CCControlColorPicker* m_colorPicker;
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

class ColorSetupDelegate {
	virtual void colorSetupClosed(int p0) {}
}

class ColoredSection : cocos2d::CCObject {
	void create(cocos2d::ccColor3B p0, int p1, int p2);
	bool init(cocos2d::ccColor3B p0, int p1, int p2);
	~ColoredSection();
}

class CommentCell : TableViewCell, LikeItemDelegate, FLAlertLayerProtocol {
	void loadFromComment(GJComment* p0) = mac 0x111c70, win 0x5f3d0;
	virtual bool init() = mac 0x11ce80, ios 0xa5878;
	virtual void draw() = mac 0x11d1f0, ios 0xa5bb4;
	virtual void likedItem(LikeItemType p0, int p1, bool p2) = mac 0x11d2c0, ios 0xa5c64;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x11d4b0, ios 0xa5dac;
	void incrementDislikes();
	void incrementLikes();
	void onConfirmDelete(cocos2d::CCObject* p0);
	void onDelete();
	void onGoToLevel(cocos2d::CCObject* p0);
	void onLike(cocos2d::CCObject* p0);
	void onUndelete();
	void onUnhide(cocos2d::CCObject* p0);
	void onViewProfile(cocos2d::CCObject* p0);
	void updateBGColor(int p0);
	void updateLabelValues();
	~CommentCell() = mac 0x11cb20, ios 0xa582c;

	PAD = win 0x4;
	cocos2d::CCSprite* m_iconSprite;
	cocos2d::CCLabelBMFont* m_likeLabel;
	GJComment* m_comment;
	PAD = win 0x4;
}

class CommentUploadDelegate {
	virtual void commentUploadFinished(int p0) {}
	virtual void commentUploadFailed(int p0, CommentError p1) {}
	virtual void commentDeleteFailed(int p0, int p1) {}
}

class CommunityCreditNode : cocos2d::CCNode {
	void create(int p0, int p1, int p2, gd::string p3);
	bool init(int p0, int p1, int p2, gd::string p3);
	~CommunityCreditNode() = mac 0x19ad00, ios 0xff070;
}

class CommunityCreditsPage : FLAlertLayer {
	virtual bool init() = mac 0x19b2b0, ios 0xff360;
	virtual void registerWithTouchDispatcher() = mac 0x19e9f0, ios 0x102184;
	virtual void keyBackClicked() = mac 0x19e980, ios 0x102108;
	virtual void show() = mac 0x19e7e0, ios 0x101f8c;
	void FLAlert_Clicked(FLAlertLayer* p0, bool p1);
	void create();
	void goToPage(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onSwitchPage(cocos2d::CCObject* p0);
	~CommunityCreditsPage() = mac 0x19b080, ios 0xff254;
}

class ConfigureHSVWidget : cocos2d::CCNode {
	void create(cocos2d::ccHSVValue p0, bool p1);
	bool init(cocos2d::ccHSVValue p0, bool p1);
	void onResetHSV(cocos2d::CCObject* p0);
	void onToggleSConst(cocos2d::CCObject* p0);
	void onToggleVConst(cocos2d::CCObject* p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void updateLabels();
	~ConfigureHSVWidget() = mac 0x2372d0, ios 0x9842c;

	cocos2d::CCLabelBMFont* m_hueLabel;
	cocos2d::CCLabelBMFont* m_saturationLabel;
	cocos2d::CCLabelBMFont* m_brightnessLabel;
	Slider* m_hueSlider;
	Slider* m_saturationSlider;
	Slider* m_brightnessSlider;
	cocos2d::ccHSVValue m_value;
}

class ConfigureValuePopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x382c00, ios 0xcda2c;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x382a90, ios 0xcd928;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x382ad0, ios 0xcd944;
	void create(ConfigureValuePopupDelegate* p0, float p1, float p2, float p3, gd::string p4, gd::string p5);
	bool init(ConfigureValuePopupDelegate* p0, float p1, float p2, float p3, gd::string p4, gd::string p5);
	void onClose(cocos2d::CCObject* p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void updateTextInputLabel();
	~ConfigureValuePopup() = mac 0x381ae0, ios 0xcce0c;
}

class ConfigureValuePopupDelegate {
	virtual void valuePopupClosed(ConfigureValuePopup* p0, float p1) {}
}

class CountTriggerAction : cocos2d::CCNode {
	static CountTriggerAction* createFromString(gd::string p0) = mac 0x1754f0;
	void create(int p0, int p1, bool p2, bool p3);
	void getSaveString();
	bool init(int p0, int p1, bool p2, bool p3);
	~CountTriggerAction();

	int m_previousCount;
	int m_targetCount;
	int m_targetID;
	bool m_activateGroup;
	bool multiActivate = mac 0x138;
}

class CreateGuidelinesLayer : FLAlertLayer, FLAlertLayerProtocol {
	void onStop(cocos2d::CCObject* pSender) = win 0x4d2c0;
	virtual void update(float p0) = mac 0x299c50, ios 0x3c6a8;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x299dd0, ios 0x3c840;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x299f50, ios 0x3c8e4;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x299f70, ios 0x3c8ec;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x29a030, ios 0x3c9b4;
	virtual void registerWithTouchDispatcher() = mac 0x29a070, ios 0x3c9d0;
	virtual void keyBackClicked() = mac 0x299ce0, ios 0x3c758;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x29a0b0, ios 0x3ca08;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x299c70, ios 0x3c6b8;
	virtual void keyUp(cocos2d::enumKeyCodes p0) = mac 0x29a180, ios 0x3ca3c;
	void create(LevelSettingsObject* p0);
	void doClearGuidelines();
	void getMergedRecordString(gd::string p0, gd::string p1);
	bool init(LevelSettingsObject* p0);
	void onClearGuidelines(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onRecord(cocos2d::CCObject* p0);
	void registerTouch();
	void toggleItems(bool p0);
	~CreateGuidelinesLayer() = mac 0x297c70, ios 0x3ac68;

	PAD = win 0x24;
	gd::string m_guidelineString;
}

class CreateMenuItem : CCMenuItemSpriteExtra {
	static CreateMenuItem* create(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3) = mac 0x1c580;
	bool init(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3);
	~CreateMenuItem();

	PAD = win 0x18;
	int m_objectID;
	int m_buildTabPage;
	int m_buildTab;
}

class CreatorLayer : cocos2d::CCLayer, cocos2d::CCSceneTransitionDelegate, DialogDelegate {
	void onBack(cocos2d::CCObject* p0) = win 0x4fae0;
	void onChallenge(cocos2d::CCObject* p0) = win 0x4f1b0;
	void onLeaderboards(cocos2d::CCObject* p0) = win 0x4ed20;
	void onMyLevels(cocos2d::CCObject* p0) = mac 0x142b70;
	void onSavedLevels(cocos2d::CCObject* p0) = mac 0x142860;
	virtual void sceneWillResume() = mac 0x1438f0, win 0x4fb50, ios 0x74af0;
	bool init() = mac 0x141c10, win 0x4de40, ios 0x733b4;
	static CreatorLayer* create() = win 0x4dda0;
	virtual void keyBackClicked() = mac 0x143950, ios 0x74b20;
	virtual void dialogClosed(DialogLayer* p0) = mac 0x143830, ios 0x74a84;
	void canPlayOnlineLevels();
	void onDailyLevel(cocos2d::CCObject* p0);
	void onFameLevels(cocos2d::CCObject* p0);
	void onFeaturedLevels(cocos2d::CCObject* p0);
	void onGauntlets(cocos2d::CCObject* p0);
	void onMapPacks(cocos2d::CCObject* p0);
	void onOnlineLevels(cocos2d::CCObject* p0);
	void onOnlyFullVersion(cocos2d::CCObject* p0);
	void onSecretVault(cocos2d::CCObject* p0);
	void onTreasureRoom(cocos2d::CCObject* p0);
	void onWeeklyLevel(cocos2d::CCObject* p0);
	void scene();
	~CreatorLayer() = mac 0x141940, ios 0x7325c;
}

class CurrencyRewardDelegate {
	virtual void currencyWillExit(CurrencyRewardLayer* p0) {}
}

class CurrencyRewardLayer : cocos2d::CCLayer {
	~CurrencyRewardLayer() = mac 0x447950, ios 0x23ba8c;
	virtual void update(float p0) = mac 0x44a5c0, ios 0x23e138;
	void create(int p0, int p1, int p2, CurrencySpriteType p3, int p4, CurrencySpriteType p5, int p6, cocos2d::CCPoint p7, CurrencyRewardType p8, float p9);
	void createObjects(CurrencySpriteType p0, int p1, cocos2d::CCPoint p2, float p3);
	void createObjectsFull(CurrencySpriteType p0, int p1, cocos2d::CCSprite* p2, cocos2d::CCPoint p3, float p4);
	void createUnlockObject(cocos2d::CCSprite* p0, cocos2d::CCPoint p1, float p2);
	void incrementCount(int p0);
	void incrementDiamondsCount(int p0);
	void incrementSpecialCount1(int p0);
	void incrementSpecialCount2(int p0);
	void incrementStarsCount(int p0);
	bool init(int p0, int p1, int p2, CurrencySpriteType p3, int p4, CurrencySpriteType p5, int p6, cocos2d::CCPoint p7, CurrencyRewardType p8, float p9);
	void pulseSprite(cocos2d::CCSprite* p0);
}

class CurrencySprite : CCSpritePlus {
	void create(CurrencySpriteType p0, bool p1);
	void createWithSprite(cocos2d::CCSprite* p0);
	bool init(CurrencySpriteType p0, bool p1);
	bool initWithSprite(cocos2d::CCSprite* p0);
	void rewardToSpriteType(int p0);
	void spriteTypeToStat(CurrencySpriteType p0);
	~CurrencySprite() = mac 0x44b5e0, ios 0x23ece4;
}

class CustomListView : BoomListView {
	inline static CustomListView* create(cocos2d::CCArray* entries, BoomListType type, float width, float height) {
		return CustomListView::create(entries, width, height, 0, type);
	}

	static CustomListView* create(cocos2d::CCArray* p0, float p1, float p2, int p3, BoomListType p4) = mac 0x10d410, win 0x57f90, ios 0x99154;
	virtual TableViewCell* getListCell(char const* p0) = mac 0x10d560, win 0x58050, ios 0x99240;
	virtual void loadCell(TableViewCell* p0, unsigned int p1) = mac 0x10e610, win 0x585c0;
	virtual void setupList() = mac 0x116e70, win 0x58870, ios 0xa1014;
	CustomListView() = win 0x57e60;
	virtual void loadCell(TableViewCell* p0, int p1) = mac 0x10e610, ios 0x99750;
	void reloadAll();
	~CustomListView() = mac 0x10d370, ios 0x99108;
}

class CustomSongCell : TableViewCell {
	void loadFromObject(SongInfoObject* p0) = mac 0x110220;
	virtual bool init() = mac 0x11c440, ios 0xa53a0;
	virtual void draw() = mac 0x11c5c0, ios 0xa548c;
	void onDelete(cocos2d::CCObject* p0);
	void updateBGColor(int p0);
	~CustomSongCell() = mac 0x11c320, ios 0xa5354;
}

class CustomSongLayer : FLAlertLayer, FLAlertLayerProtocol, TextInputDelegate, GJDropDownLayerDelegate {
	bool init(LevelSettingsObject* p0) = mac 0xf06f0;
	void onArtists(cocos2d::CCObject* p0) = mac 0xf1950;
	void onSongBrowser(cocos2d::CCObject* p0) = mac 0xf18a0;
	virtual void keyBackClicked() = mac 0xf21c0, ios 0xc683c;
	virtual void show() = mac 0xf22a0, ios 0xc6848;
	virtual void textChanged(CCTextInputNode* p0) = mac 0xf1fb0, ios 0xc66e8;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0xf1f10, ios 0xc66b0;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0xf1e70, ios 0xc6678;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0xf1fd0, ios 0xc6708;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0xf2090, ios 0xc6774;
	virtual void dropDownLayerWillClose(GJDropDownLayer* p0) = mac 0xf2140, ios 0xc67d4;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0xf1be0, ios 0xc64b0;
	void create(LevelSettingsObject* p0);
	void onChanges(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onCreateLines(cocos2d::CCObject* p0);
	void onGuidelines(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onNewgrounds(cocos2d::CCObject* p0);
	void onOptions(cocos2d::CCObject* p0);
	void onSearch(cocos2d::CCObject* p0);
	void onSupporter(cocos2d::CCObject* p0);
	void showNewgroundsMessage();
	~CustomSongLayer() = mac 0xf0490, ios 0xc5060;

	LevelSettingsObject* m_levelSettings;
	CCTextInputNode* m_songIDInput;
	CustomSongWidget* m_songWidget;
	LevelSettingsLayer* m_levelSettingsLayer;
}

class CustomSongLayerDelegate {
	virtual void customSongLayerClosed() {}
}

class CustomSongWidget : cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer* p0, bool p1) {}
	void loadSongInfoFinished(SongInfoObject* p0) {}

	void updateSongObject(SongInfoObject* song) = win 0x69280;
	virtual void loadSongInfoFailed(int p0, GJSongError p1) = mac 0x37e5a0, ios 0x97a7c;
	virtual void downloadSongFinished(SongInfoObject* p0) = mac 0x37e7b0, ios 0x97bec;
	virtual void downloadSongFailed(int p0, GJSongError p1) = mac 0x37e8c0, ios 0x97c8c;
	virtual void songStateChanged() = mac 0x37e390, ios 0x97984;
	void create(SongInfoObject* p0, LevelSettingsObject* p1, bool p2, bool p3, bool p4, bool p5, bool p6);
	void downloadFailed();
	void getSongInfoIfUnloaded();
	bool init(SongInfoObject* p0, LevelSettingsObject* p1, bool p2, bool p3, bool p4, bool p5, bool p6);
	void onCancelDownload(cocos2d::CCObject* p0);
	void onDownload(cocos2d::CCObject* p0);
	void onGetSongInfo(cocos2d::CCObject* p0);
	void onMore(cocos2d::CCObject* p0);
	void onPlayback(cocos2d::CCObject* p0);
	void onSelect(cocos2d::CCObject* p0);
	void setSongObject(SongInfoObject* p0);
	void startDownload();
	void startMonitorDownload();
	void toggleUpdateButton(bool p0);
	void updateDownloadProgress(float p0);
	void updateError(GJSongError p0);
	void updatePlaybackBtn();
	void updateProgressBar(int p0);
	void updateSongInfo();
	void verifySongID(int p0);
	~CustomSongWidget() = mac 0x37bb90, ios 0x95d78;

	SongInfoObject* m_songInfo;
	PAD = win 0x1C;
	CCMenuItemSpriteExtra* m_downloadBtn;
	PAD = win 0x30;
}

class CustomizeObjectLayer : FLAlertLayer, TextInputDelegate, HSVWidgetPopupDelegate, ColorSelectDelegate, ColorSetupDelegate {
	void onNextColorChannel(cocos2d::CCObject* pSender) = win 0x56c80;
	void onSelectColor(cocos2d::CCObject* pSender) = win 0x577b0;
	int getActiveMode(bool unknown) = win 0x57210;
	virtual void keyBackClicked() = mac 0xe16c0, ios 0x3105c;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0xe1280, ios 0x30d30;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0xe1430, ios 0x30e64;
	virtual void textChanged(CCTextInputNode* p0) = mac 0xe1470, ios 0x30e80;
	virtual void hsvPopupClosed(HSVWidgetPopup* p0, cocos2d::ccHSVValue p1) = mac 0xe1050, ios 0x30b38;
	virtual void colorSelectClosed(cocos2d::CCNode* p0) = mac 0xe0c60, ios 0x309a0;
	virtual void colorSetupClosed(int p0) = mac 0xe0850, ios 0x306c8;
	void create(GameObject* p0, cocos2d::CCArray* p1);
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4);
	void determineStartValues();
	void getButtonByTag(int p0);
	void getHSV();
	void highlightSelected(ButtonSprite* p0);
	bool init(GameObject* p0, cocos2d::CCArray* p1);
	void onBreakApart(cocos2d::CCObject* p0);
	void onBrowse(cocos2d::CCObject* p0);
	void onClear(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onCopy(cocos2d::CCObject* p0);
	void onEditColor(cocos2d::CCObject* p0);
	void onHSV(cocos2d::CCObject* p0);
	void onPaste(cocos2d::CCObject* p0);
	void onSelectMode(cocos2d::CCObject* p0);
	void onToggleGlow(cocos2d::CCObject* p0);
	void onUpdateCustomColor(cocos2d::CCObject* p0);
	void toggleVisible();
	void updateChannelLabel(int p0);
	void updateColorSprite();
	void updateCurrentSelection();
	void updateCustomColorLabels();
	void updateHSVButtons();
	void updateSelected(int p0);
	~CustomizeObjectLayer() = mac 0xdd2a0, ios 0x2db2c;

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

class DailyLevelNode : cocos2d::CCNode, FLAlertLayerProtocol {
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x10b750, ios 0x11c304;
	void create(GJGameLevel* p0, DailyLevelPage* p1, bool p2);
	bool init(GJGameLevel* p0, DailyLevelPage* p1, bool p2);
	void onClaimReward(cocos2d::CCObject* p0);
	void onSkipLevel(cocos2d::CCObject* p0);
	void showSkipButton();
	void updateTimeLabel(gd::string p0);
	~DailyLevelNode() = mac 0x10aaa0, ios 0x11b770;
}

class DailyLevelPage : FLAlertLayer, FLAlertLayerProtocol, GJDailyLevelDelegate, LevelDownloadDelegate {
	static DailyLevelPage* create(bool weekly) = win 0x6a860;
	bool init(bool weekly) = win 0x6a900;
	void updateTimers(float p0) = win 0x6bef0;
	virtual void registerWithTouchDispatcher() = mac 0x10a6f0, ios 0x11b424;
	virtual void keyBackClicked() = mac 0x10a660, ios 0x11b3d0;
	virtual void show() = mac 0x10a4b0, ios 0x11b24c;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x10a640, ios 0x11b3c8;
	virtual void dailyStatusFinished(bool p0) = mac 0x109be0, ios 0x11ab98;
	virtual void dailyStatusFailed(bool p0) = mac 0x10a090, ios 0x11aee8;
	virtual void levelDownloadFinished(GJGameLevel* p0) = mac 0x10a1b0, ios 0x11affc;
	virtual void levelDownloadFailed(int p0) = mac 0x10a280, ios 0x11b074;
	void claimLevelReward(DailyLevelNode* p0, GJGameLevel* p1, cocos2d::CCPoint p2);
	void createDailyNode(GJGameLevel* p0, bool p1, float p2, bool p3);
	void createNodeIfLoaded();
	void downloadAndCreateNode();
	void exitDailyNode(DailyLevelNode* p0, float p1);
	void getDailyTime();
	void getDailyTimeString(int p0);
	void onClose(cocos2d::CCObject* p0);
	void refreshDailyPage();
	void skipDailyLevel(DailyLevelNode* p0, GJGameLevel* p1);
	void tryGetDailyStatus();
	~DailyLevelPage() = mac 0x108a20, ios 0x119d40;

	PAD = win 0x21;
	bool m_weekly;
}

class DelaySection : cocos2d::CCObject {
	void create(int p0, float p1);
	bool init(int p0, float p1);
	~DelaySection();
}

class DelayedSpawnNode : cocos2d::CCObject {
	void create();
	bool init();
	~DelayedSpawnNode();
}

class DemonFilterDelegate {
	virtual void demonFilterSelectClosed(int p0) {}
}

class DemonFilterSelectLayer : FLAlertLayer {
	virtual bool init() = mac 0x38b9d0, ios 0x36d20;
	virtual void keyBackClicked() = mac 0x38c060, ios 0x372b4;
	void create();
	void onClose(cocos2d::CCObject* p0);
	void selectRating(cocos2d::CCObject* p0);
	~DemonFilterSelectLayer() = mac 0x38b930, ios 0x36cd4;
}

class DialogDelegate {
	virtual void dialogClosed(DialogLayer* p0) {}
}

class DialogLayer : cocos2d::CCLayerColor, TextAreaDelegate {
	static DialogLayer* create(DialogObject* dialog, int color) {
		return DialogLayer::createDialogLayer(dialog, nullptr, color);
	}
	static DialogLayer* createWithObjects(cocos2d::CCArray* dialogs, int color) {
		return DialogLayer::createDialogLayer(nullptr, dialogs, color);
	}

	static DialogLayer* createDialogLayer(DialogObject* p0, cocos2d::CCArray* p1, int p2) = mac 0x2047d0, win 0x6D470, ios 0x459d0;
	bool init(DialogLayer* p0, DialogObject* p1, cocos2d::CCArray* p2, int p3) = win 0x6D520;
	cocos2d::CCAction* animateIn(int location) = win 0x6E130;
	~DialogLayer() = mac 0x204720, ios 0x45984;
	virtual void onEnter() = mac 0x205900, ios 0x46738;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x205790, ios 0x465e8;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x205820, ios 0x4667c;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2057e0, ios 0x46644;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x205840, ios 0x46684;
	virtual void registerWithTouchDispatcher() = mac 0x205890, ios 0x466c8;
	virtual void keyBackClicked() = mac 0x2056a0, ios 0x465dc;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x205ce0, ios 0x46a80;
	virtual void fadeInTextFinished(TextArea* p0) = mac 0x205930, ios 0x46770;
	void animateIn(DialogAnimationType p0);
	void animateInDialog();
	void animateInRandomSide();
	void displayDialogObject(DialogObject* p0);
	void displayNextObject();
	void finishCurrentAnimation();
	void handleDialogTap();
	bool init(DialogObject* p0, cocos2d::CCArray* p1, int p2);
	void onClose();
	void updateChatPlacement(DialogChatPlacement p0);
	void updateNavButtonFrame();
}

class DialogObject : cocos2d::CCObject {
	static DialogObject* create(gd::string title, gd::string text, int portrait, float text_scale, bool is_unskippable, cocos2d::ccColor3B text_color) = win 0x6D160;
	bool init(DialogObject* p0, gd::string title, gd::string text, int portrait, float text_scale, bool is_unskippable, cocos2d::ccColor3B text_color) = win 0x6D2E0;
	bool init(gd::string p0, gd::string p1, int p2, float p3, bool p4, cocos2d::ccColor3B p5);
	~DialogObject();

	gd::string m_text;
	gd::string m_title;
	int m_dialogType;
	cocos2d::ccColor3B m_color;
	float m_textWidth;
	bool m_canSkip;
}

class DownloadMessageDelegate {
	virtual void downloadMessageFinished(GJUserMessage* p0) {}
	virtual void downloadMessageFailed(int p0) {}
}

class DrawGridLayer : cocos2d::CCLayer {
	inline using CCPointArray400 = cocos2d::CCPoint(*)[400];
	inline using CCPointArray200 = cocos2d::CCPoint(*)[200];

	void draw() = mac 0xa3c40, win 0x16ce90, ios 0x26dae4;
	virtual void update(float p0) = mac 0xa3b30, ios 0x26da24;
	void addPlayer2Point(cocos2d::CCPoint p0, bool p1);
	void addPlayerPoint(cocos2d::CCPoint p0);
	void addToEffects(EffectGameObject* p0);
	void addToGuides(GameObject* p0);
	void addToSpeedObjects(GameObject* p0);
	void clearPlayerPoints();
	void create(cocos2d::CCNode* p0, LevelEditorLayer* p1);
	void getPortalMinMax(GameObject* p0);
	bool init(cocos2d::CCNode* p0, LevelEditorLayer* p1);
	void loadTimeMarkers(gd::string p0);
	void removeFromEffects(EffectGameObject* p0);
	void removeFromGuides(GameObject* p0);
	void removeFromSpeedObjects(GameObject* p0);
	void sortSpeedObjects();
	void timeForXPos(float p0);
	void updateMusicGuideTime(float p0);
	void updateTimeMarkers();
	void xPosForTime(float p0);
	~DrawGridLayer() = mac 0xa3640, ios 0x26d5dc;

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

class DungeonBarsSprite : cocos2d::CCNode {
	virtual bool init() = mac 0x26c2b0, ios 0x231e8c;
	virtual void visit() = mac 0x26c330, ios 0x231f0c;
	void animateOutBars();
	void create();
	~DungeonBarsSprite() = mac 0x26c4c0, ios 0x14cb4;
}

class DynamicScrollDelegate {}

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
	void create(cocos2d::CCArray* p0, cocos2d::CCPoint p1, int p2, bool p3, int p4, int p5);
	void getPage();
	void goToPage(int p0);
	bool init(cocos2d::CCArray* p0, cocos2d::CCPoint p1, int p2, bool p3, int p4, int p5);
	void onLeft(cocos2d::CCObject* p0);
	void onRight(cocos2d::CCObject* p0);
	~EditButtonBar() = mac 0x350d80, ios 0x2dceb4;

	cocos2d::CCPoint m_position;
	int m_unknown;
	bool m_unknownBool;
	cocos2d::CCArray* m_buttonArray;
	BoomScrollLayer* m_scrollLayer;
	cocos2d::CCArray* m_pagesArray;
}

class EditGameObjectPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x154070, ios 0x20c5d8;
	virtual void show() = mac 0x154020, ios 0x20c58c;
	void create(EffectGameObject* p0, cocos2d::CCArray* p1);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onAllowMultiActivate(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	~EditGameObjectPopup() = mac 0x153600, ios 0x20be20;
}

class EditLevelLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, SetIDPopupDelegate {
	static void scene(GJGameLevel* level) {
	    auto pScene = cocos2d::CCScene::create();
	
	    pScene->addChild(reinterpret_cast<cocos2d::CCNode*>(EditLevelLayer::create(level)));
	
	    cocos2d::CCDirector::sharedDirector()->replaceScene(
	        cocos2d::CCTransitionFade::create(.5f, pScene)
	    );
	}

	static EditLevelLayer* create(GJGameLevel* level) = mac 0xe1e50, win 0x6f530, ios 0x82420;
	void onLevelInfo() = win 0x70660;
	virtual void keyBackClicked() = mac 0xe6670, ios 0x85ff8;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0xe66a0, ios 0x86004;
	virtual void setIDPopupClosed(SetIDPopup* p0, int p1) = mac 0xe6560, ios 0x85f6c;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0xe5e70, ios 0x85a88;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0xe5540, ios 0x85374;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0xe51e0, ios 0x850b0;
	virtual void textChanged(CCTextInputNode* p0) = mac 0xe57a0, ios 0x85558;
	virtual void uploadActionFinished(int p0, int p1) = mac 0xe6740, ios 0x86074;
	virtual void uploadActionFailed(int p0, int p1) = mac 0xe68c0, ios 0x861e0;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0xe66f0, ios 0x86024;
	void closeTextInputs();
	void confirmClone(cocos2d::CCObject* p0);
	void confirmDelete(cocos2d::CCObject* p0);
	void confirmMoveToTop(cocos2d::CCObject* p0);
	bool init(GJGameLevel* p0);
	void onBack(cocos2d::CCObject* p0);
	void onClone();
	void onDelete();
	void onEdit(cocos2d::CCObject* p0);
	void onHelp(cocos2d::CCObject* p0);
	void onLevelInfo(cocos2d::CCObject* p0);
	void onLowDetailMode(cocos2d::CCObject* p0);
	void onMoveToTop();
	void onPlay(cocos2d::CCObject* p0);
	void onSetFolder(cocos2d::CCObject* p0);
	void onShare(cocos2d::CCObject* p0);
	void onTest(cocos2d::CCObject* p0);
	void onUpdateDescription(cocos2d::CCObject* p0);
	void playStep2();
	void playStep3();
	void setupLevelInfo();
	void updateDescText(char const* p0);
	void verifyLevelName();
	~EditLevelLayer() = mac 0xe1d70, ios 0x82388;

	cocos2d::CCMenu* m_buttonMenu;
	GJGameLevel* m_level;
	TextArea* m_descriptionInput;
	cocos2d::CCArray* m_someArray;
	cocos2d::CCLabelBMFont* m_someFont;
}

class EditTriggersPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x1458a0, ios 0x200b4c;
	virtual void show() = mac 0x145850, ios 0x200b00;
	void create(EffectGameObject* p0, cocos2d::CCArray* p1);
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTouchTriggered();
	~EditTriggersPopup() = mac 0x1447b0, ios 0x1ffe2c;
}

class EditorOptionsLayer : GJOptionsLayer {
	void onButtonsPerRow(cocos2d::CCObject* p0) = mac 0x147b30;
	virtual bool init() = mac 0x147420, ios 0x2024e4;
	virtual void setupOptions() = mac 0x147440, ios 0x2024f0;
	virtual void onClose(cocos2d::CCObject* p0) = mac 0x147c30, ios 0x202d00;
	void create();
	void onButtonRows(cocos2d::CCObject* p0);
	~EditorOptionsLayer() = mac 0x1471e0, ios 0x2023cc;
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

	~EditorPauseLayer() = mac 0x13c3b0, ios 0x280b9c;
	virtual void keyBackClicked() = mac 0x13f320, win 0x758d0, ios 0x282efc;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x13f3a0, ios 0x282f7c;
	virtual void customSetup() = mac 0x13cc00, ios 0x281134;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x13f1b0, win 0x75780, ios 0x282e24;
	void saveLevel() = mac 0x13ebd0, win 0x75010;
	bool init(LevelEditorLayer* p0) = mac 0x13c7a0, win 0x730e0, ios 0x280cb8;
	void onExitEditor(cocos2d::CCObject* pSender) = win 0x75660;
	void playStep2() = win 0x75440;
	void onResume(cocos2d::CCObject* pSender) = win 0x74fe0;
	void onSaveAndPlay(cocos2d::CCObject* pSender) = win 0x753d0;
	void onSaveAndExit(cocos2d::CCObject* pSender) = win 0x75620;
	void onSave(cocos2d::CCObject* pSender) = win 0x755a0;
	void onExitNoSave(cocos2d::CCObject* pSender) = win 0x75700;
	void uncheckAllPortals(cocos2d::CCObject* pSender) = win 0x74760;
	EditorPauseLayer() = win 0x72f10;
	void doResetUnused();
	void onAlignX(cocos2d::CCObject* p0);
	void onAlignY(cocos2d::CCObject* p0);
	void onBuildHelper(cocos2d::CCObject* p0);
	void onCreateBase(cocos2d::CCObject* p0);
	void onCreateEdges(cocos2d::CCObject* p0);
	void onCreateOutline(cocos2d::CCObject* p0);
	void onHelp(cocos2d::CCObject* p0);
	void onKeybindings(cocos2d::CCObject* p0);
	void onOptions(cocos2d::CCObject* p0);
	void onResetUnusedColors(cocos2d::CCObject* p0);
	void onSaveAndTest(cocos2d::CCObject* p0);
	void onSelectAll(cocos2d::CCObject* p0);
	void onSelectAllLeft(cocos2d::CCObject* p0);
	void onSelectAllRight(cocos2d::CCObject* p0);
	void onSong(cocos2d::CCObject* p0);
	void onUnlockAllLayers(cocos2d::CCObject* p0);
	void playStep3();
	void toggleDebugDraw(cocos2d::CCObject* p0);
	void toggleEditorBackground(cocos2d::CCObject* p0);
	void toggleEditorColorMode(cocos2d::CCObject* p0);
	void toggleEditorGrid(cocos2d::CCObject* p0);
	void toggleEditorGround(cocos2d::CCObject* p0);
	void toggleEffectDuration(cocos2d::CCObject* p0);
	void toggleEffectLines(cocos2d::CCObject* p0);
	void toggleFollowPlayer(cocos2d::CCObject* p0);
	void toggleGridOnTop(cocos2d::CCObject* p0);
	void toggleIgnoreDamage(cocos2d::CCObject* p0);
	void togglePlaytestMusic(cocos2d::CCObject* p0);
	void toggleSelectFilter(cocos2d::CCObject* p0);
	void toggleShowObjectInfo(cocos2d::CCObject* p0);
	void updateSongButton();

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
	void create(LevelEditorLayer* p0) = mac 0x8a80;
	void deselectAll() = mac 0x1f300, win 0x86af0;
	void onDeselectAll(cocos2d::CCObject* p0) = mac 0x19cd0;
	void disableButton(CreateMenuItem* p0) = mac 0x1c0f0, win 0x78af0;
	void editButtonUsable() = mac 0x28f30;
	void editObject(cocos2d::CCObject* p0) = mac 0x195a0, win 0x8ca50;
	void enableButton(CreateMenuItem* p0) = mac 0x1bff0, win 0x78990;
	CCMenuItemSpriteExtra* getCreateBtn(int p0, int p1) = mac 0x1f6c0, win 0x85120;
	cocos2d::CCPoint getGroupCenter(cocos2d::CCArray* p0, bool p1) = mac 0x23470, win 0x8fc30;
	cocos2d::CCArray* getSelectedObjects() = mac 0x23f30, win 0x86900;
	bool init(LevelEditorLayer* p0) = mac 0x8ae0;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2ed60, ios 0x2c314c;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2f3d0, ios 0x2c3654;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2fb00, ios 0x2c3ab8;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x30790, win 0x91a30, ios 0x2c4244;
	void moveObject(GameObject* p0, cocos2d::CCPoint p1) = mac 0x24b10, win 0x8ddb0;
	void onDuplicate(cocos2d::CCObject* p0) = mac 0x18ba0, win 0x87d20;
	cocos2d::CCArray* pasteObjects(gd::string const& p0) = mac 0x232d0, win 0x88240;
	void playerTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2ebf0;
	void playtestStopped() = mac 0x24790;
	void redoLastAction(cocos2d::CCObject* p0) = mac 0xb8e0, win 0x870f0;
	void replaceGroupID(GameObject* p0, int p1, int p2) = mac 0x27470;
	void scaleChanged(float p0) = mac 0x25490, ios 0x2bdf9c;
	void scaleObjects(cocos2d::CCArray* p0, float p1, cocos2d::CCPoint p2) = mac 0x252e0, win 0x8f150;
	void selectObjects(cocos2d::CCArray* p0, bool p1) = mac 0x23940, win 0x864a0;
	void setupCreateMenu() = mac 0xcb50;
	void undoLastAction(cocos2d::CCObject* p0) = mac 0xb830, win 0x87070;
	void updateButtons() = mac 0x1a300, win 0x78280;
	void updateObjectInfoLabel() = mac 0x1cb10, win 0x793b0;
	void updateSlider() = mac 0x18a90, win 0x78f10;
	void updateZoom(float p0) = mac 0x248c0, win 0x878a0;
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
	virtual void keyUp(cocos2d::enumKeyCodes key) = mac 0x312b0, win 0x92180, ios 0x2c482c;
	virtual void draw() = mac 0x2e170, ios 0x2c2990;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x30710, ios 0x2c41f0;
	virtual void registerWithTouchDispatcher() = mac 0x30750, ios 0x2c420c;
	virtual void keyBackClicked() = mac 0x24c50, ios 0x2bdbac;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x1f590, ios 0x2b9794;
	virtual void songStateChanged() = mac 0x24760, ios 0x2bd7e4;
	virtual void colorSelectClosed(cocos2d::CCNode* p0) = mac 0x29940, ios 0x2c0e70;
	virtual void scrollWheel(float p0, float p1) = mac 0x31370, ios 0x2c4884;
	virtual void angleChangeBegin() = mac 0x2e260, ios 0x2c2a74;
	virtual void angleChangeEnded() = mac 0x2e320, ios 0x2c2acc;
	virtual void angleChanged(float p0) = mac 0x2e3a0, ios 0x2c2ad8;
	virtual void scaleChangeBegin() = mac 0x25120, ios 0x2bdf38;
	virtual void scaleChangeEnded() = mac 0x251e0, ios 0x2bdf90;
	void addSnapPosition(cocos2d::CCPoint p0);
	void applyOffset(GameObject* p0);
	void applySpecialOffset(cocos2d::CCPoint p0, GameObject* p1, cocos2d::CCPoint p2);
	void changeSelectedObjects(cocos2d::CCArray* p0, bool p1);
	void clickOnPosition(cocos2d::CCPoint p0);
	void closeLiveColorSelect();
	void colorSelectClosed(cocos2d::ccColor3B p0);
	void copyObjects(cocos2d::CCArray* p0);
	void createCustomItems();
	void createEdgeForObject(GameObject* p0, int p1);
	void createGlow();
	void createMoveMenu();
	void createUndoSelectObject(bool p0);
	void deactivateRotationControl();
	void deactivateScaleControl();
	void deleteObject(GameObject* p0, bool p1);
	void deselectObject(GameObject* p0);
	void deselectObject();
	void deselectTargetPortals();
	void edgeForObject(int p0, int p1);
	void editButton2Usable();
	void editorLayerForArray(cocos2d::CCArray* p0, bool p1);
	void findSnapObject(cocos2d::CCPoint p0, float p1);
	void findSnapObject(cocos2d::CCArray* p0, float p1);
	void flipObjectsX(cocos2d::CCArray* p0);
	void flipObjectsY(cocos2d::CCArray* p0);
	void getButton(char const* p0, int p1, cocos2d::SEL_MenuHandler p2, cocos2d::CCMenu* p3);
	void getCreateMenuItemButton(cocos2d::CCSprite* p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCMenu* p2, float p3, int p4, cocos2d::CCPoint p5);
	void getCycledObject(cocos2d::CCArray* p0, bool p1);
	void getGridSnappedPos(cocos2d::CCPoint p0);
	void getLimitedPosition(cocos2d::CCPoint p0);
	void getModeBtn(char const* p0, int p1);
	void getRelativeOffset(GameObject* p0);
	void getSimpleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCMenu* p2);
	void getSpriteButton(char const* p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCMenu* p2, float p3, int p4, cocos2d::CCPoint p5);
	void getSpriteButton(cocos2d::CCSprite* p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCMenu* p2, float p3, int p4, cocos2d::CCPoint p5);
	void getXMin(int p0);
	void isLiveColorSelectTrigger(GameObject* p0);
	void isSpecialSnapObject(int p0);
	void liveEditColorUsable();
	void menuItemFromObjectString(gd::string p0, int p1);
	void moveForCommand(EditCommand p0);
	void moveGamelayer(cocos2d::CCPoint p0);
	void moveObjectCall(cocos2d::CCObject* p0);
	void onCreate();
	void onCreateObject(int p0);
	void onDelete(cocos2d::CCObject* p0);
	void onDeleteAll(cocos2d::CCObject* p0);
	void onDeleteCustomItem(cocos2d::CCObject* p0);
	void onDeleteInfo(cocos2d::CCObject* p0);
	void onDeleteSelectedType(cocos2d::CCObject* p0);
	void onDeleteStartPos(cocos2d::CCObject* p0);
	void onLockLayer(cocos2d::CCObject* p0);
	void onNewCustomItem(cocos2d::CCObject* p0);
	void onToggleGuide(cocos2d::CCObject* p0);
	void onUpdateDeleteFilter(cocos2d::CCObject* p0);
	void orderDownCustomItem(cocos2d::CCObject* p0);
	void orderUpCustomItem(cocos2d::CCObject* p0);
	void pasteObjects(gd::string p0);
	void playerTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	void positionIsInSnapped(cocos2d::CCPoint p0);
	void positionWithoutOffset(GameObject* p0);
	void processSelectObjects(cocos2d::CCArray* p0);
	void recreateButtonTabs();
	void reloadCustomItems();
	void removeOffset(GameObject* p0);
	void repositionObjectsToCenter(cocos2d::CCArray* p0, cocos2d::CCPoint p1, bool p2);
	void resetUI();
	void rotationforCommand(EditCommand p0);
	void selectObjectsInRect(cocos2d::CCRect p0);
	void setStoredUndoObject(UndoObject* p0);
	void setupDeleteMenu();
	void setupEditMenu();
	void setupTransformControl();
	void shouldDeleteObject(GameObject* p0);
	void shouldSnap(GameObject* p0);
	void showDeleteConfirmation();
	void showLiveColorSelectForMode(int p0);
	void showMaxBasicError();
	void showMaxCoinError();
	void showMaxError();
	void sliderChanged(cocos2d::CCObject* p0);
	void toggleDuplicateButton();
	void toggleEditObjectButton();
	void toggleGuideButton();
	void toggleObjectInfoLabel();
	void toggleSpecialEditButtons();
	void toggleStickyControls(bool p0);
	void transformObject(GameObject* p0, EditCommand p1, bool p2);
	void transformObjectCall(cocos2d::CCObject* p0);
	void triggerSwipeMode();
	void tryUpdateTimeMarkers();
	void updateDeleteButtons();
	void updateEditMenu();
	void updateGridNodeSize(int p0);
	void updateGroupIDBtn2();
	void updateGroupIDLabel();
	void updatePlaybackBtn();
	void updateScaleControl();
	void valueFromXPos(float p0);
	void xPosFromValue(float p0);
	void zoomGameLayer(bool p0);
	~EditorUI() = mac 0x89e0, ios 0x2a5c1c;

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

	static EffectGameObject* create(char const* p0) = mac 0xc9790, win 0x253c30;
	void getTargetColorIndex() = mac 0xca1f0;
	virtual void triggerObject(GJBaseGameLayer* p0) = mac 0xc9870, win 0x253d60, ios 0x278380;
	gd::string getSaveString() = mac 0xcd7e0, win 0x257560, ios 0x27b49c;
	virtual void customSetup() = mac 0xca370, ios 0x278c98;
	virtual void customObjectSetup(gd::map<gd::string, gd::string>& p0) = mac 0xca420, ios 0x278d40;
	virtual void triggerActivated(float p0) = mac 0xca310, ios 0x278c34;
	virtual void spawnXPosition() = mac 0xca2d0, ios 0x278bfc;
	bool init(char const* p0);
	void resetSpawnTrigger();
	void updateSpecialColor();
	~EffectGameObject() = mac 0xc9750, ios 0x2782ac;

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

class EndLevelLayer : GJDropDownLayer {
	static EndLevelLayer* create() = mac 0x2787d0;
	virtual void keyBackClicked();
	virtual void keyDown(cocos2d::enumKeyCodes p0);
	virtual void customSetup();
	virtual void showLayer(bool p0);
	virtual void enterAnimFinished();
	virtual void keyUp(cocos2d::enumKeyCodes p0);
	void coinEnterFinished(cocos2d::CCPoint p0);
	void coinEnterFinishedO(cocos2d::CCObject* p0);
	void currencyEnterFinished();
	void diamondEnterFinished();
	void getCoinString();
	void getEndText();
	void goEdit();
	void onEdit(cocos2d::CCObject* p0);
	void onEveryplay(cocos2d::CCObject* p0);
	void onMenu(cocos2d::CCObject* p0);
	void onReplay(cocos2d::CCObject* p0);
	void playCoinEffect(float p0);
	void playCurrencyEffect(float p0);
	void playDiamondEffect(float p0);
	void playEndEffect();
	void playStarEffect(float p0);
	void starEnterFinished();
	void tryShowBanner(float p0);
	~EndLevelLayer();
}

class EndPortalObject : GameObject {
	static EndPortalObject* create() = mac 0x1da8f0;
	void updateColors(cocos2d::ccColor3B p0) = mac 0x1dacb0;
	virtual bool init() = mac 0x1da980, ios 0x2c62c4;
	virtual void setPosition(cocos2d::CCPoint const& p0) = mac 0x1dae70, ios 0x2c6734;
	virtual void setVisible(bool p0) = mac 0x1daf30, ios 0x2c67dc;
	virtual void triggerObject(GJBaseGameLayer* p0) = mac 0x1dadc0, ios 0x2c669c;
	virtual void calculateSpawnXPos() = mac 0x1dae50, ios 0x2c671c;
	~EndPortalObject() = mac 0x1db080, ios 0x27ef7c;
}

class ExtendedLayer : cocos2d::CCLayer {
	virtual bool init() = mac 0x1e41e0, ios 0xbd198;
	virtual void setPosition(cocos2d::CCPoint const& p0) = mac 0x1e41f0, ios 0xbd19c;
	void create();
	~ExtendedLayer() = mac 0x1e4250;
}

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

	virtual void onEnter() = mac 0x25f350, win 0x23750, ios 0x1ff1fc;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x25ee40, win 0x233c0, ios 0x1fee48;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x25f0a0, win 0x23510, ios 0x1fefb4;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x25ef60, win 0x23450, ios 0x1fef00;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x25f020, win 0x234c0, ios 0x1fef74;
	virtual void registerWithTouchDispatcher() = mac 0x25f2e0, win 0x236f0, ios 0x1ff18c;
	virtual void keyBackClicked() = mac 0x25ed90, win 0x232c0, ios 0x1fede4;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x25ece0, win 0x23250, ios 0x1fedb4;
	virtual void show() = mac 0x25f120, win 0x23560, ios 0x1feff4;
	bool init(FLAlertLayerProtocol* p0, char const* p1, gd::string p2, char const* p3, char const* p4, float p5, bool p6, float p7) = mac 0x25e1b0;
	static FLAlertLayer* create(FLAlertLayerProtocol* p0, char const* p1, gd::string p2, char const* p3, char const* p4) = mac 0x25de00, win 0x22680;
	static FLAlertLayer* create(FLAlertLayerProtocol* p0, char const* p1, gd::string p2, char const* p3, char const* p4, float p5) = mac 0x25e0e0, win 0x22730, ios 0x1fe374;
	static FLAlertLayer* create(FLAlertLayerProtocol* p0, char const* p1, gd::string p2, char const* p3, char const* p4, float p5, bool p6, float p7) = mac 0x25dec0, win 0x227e0;
	void onBtn1(cocos2d::CCObject* p0) = mac 0x25ec20, win 0x23340;
	void onBtn2(cocos2d::CCObject* p0) = mac 0x25ec80, win 0x23380;

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
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) {}
}

class FMODAudioEngine : cocos2d::CCNode {
	static FMODAudioEngine* sharedEngine() = mac 0x20ef80, win 0x239f0;
	void preloadEffect(gd::string filename) = win 0x24240;
	bool isBackgroundMusicPlaying() = win 0x24050;
	bool isBackgroundMusicPlaying(gd::string path) = win 0x24080;
	void playBackgroundMusic(gd::string path, bool fade, bool paused) = win 0x23d80;
	virtual void update(float p0) = mac 0x20f1d0, ios 0xc2690;
	void disableMetering();
	void enableMetering();
	void fadeBackgroundMusic(bool p0, float p1);
	void getBackgroundMusicTime();
	void getBackgroundMusicVolume();
	void getEffectsVolume();
	void pauseAllEffects();
	void pauseBackgroundMusic();
	void pauseEffect(unsigned int p0);
	void playEffect(gd::string p0, bool p1, float p2, float p3, float p4);
	void preloadBackgroundMusic(gd::string p0);
	void printResult(FMOD_RESULT p0);
	void resumeAllEffects();
	void resumeBackgroundMusic();
	void resumeEffect(unsigned int p0);
	void rewindBackgroundMusic();
	void setBackgroundMusicTime(float p0);
	void setBackgroundMusicVolume(float p0);
	void setEffectsVolume(float p0);
	void setupAudioEngine();
	void start();
	void stop();
	void stopAllEffects();
	void stopBackgroundMusic(bool p0);
	void stopEffect(unsigned int p0);
	void unloadEffect(gd::string p0);
	void willPlayBackgroundMusic();
	~FMODAudioEngine() = mac 0x20ef40, ios 0xc2494;

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
	bool init(FMOD::Sound* p0);
	~FMODSound() = mac 0x210550, ios 0xc347c;

	FMOD::Sound* m_sound;
}

class FRequestProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, FriendRequestDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x437300, ios 0x1182ac;
	virtual void keyBackClicked() = mac 0x4372d0, ios 0x1182a0;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x437200, ios 0x1181a4;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x437340, ios 0x1182e4;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x4373a0, ios 0x118354;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x437500, ios 0x1184f0;
	virtual void loadFRequestsFinished(cocos2d::CCArray* p0, char const* p1) = mac 0x437840, ios 0x118860;
	virtual void loadFRequestsFailed(char const* p0, GJErrorCode p1) = mac 0x437930, ios 0x1188e4;
	virtual void setupPageInfo(gd::string p0, char const* p1) = mac 0x437ab0, ios 0x1189b8;
	virtual void forceReloadRequests(bool p0) = mac 0x437a60, ios 0x118984;
	void create(bool p0);
	void deleteSelected();
	bool init(bool p0);
	void isCorrect(char const* p0);
	void loadPage(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onDeleteSelected(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void onSentRequests(cocos2d::CCObject* p0);
	void onToggleAllObjects(cocos2d::CCObject* p0);
	void onUpdate(cocos2d::CCObject* p0);
	void setupCommentsBrowser(cocos2d::CCArray* p0);
	void untoggleAll();
	void updateLevelsLabel();
	void updatePageArrows();
	~FRequestProfilePage() = mac 0x435980, ios 0x116cfc;
}

class FileSaveManager : GManager {
	virtual bool init() = mac 0x6360;
	virtual void firstLoad() = mac 0x6390;
	void getStoreData();
	void loadDataFromFile(char const* p0);
	void sharedState();
	~FileSaveManager() = mac 0x68d0;
}

class FollowRewardPage : FLAlertLayer, FLAlertLayerProtocol, GameRateDelegate {
	virtual bool init() = mac 0x22f4a0, ios 0x2e08e8;
	virtual void registerWithTouchDispatcher() = mac 0x230eb0, ios 0x2e1d2c;
	virtual void keyBackClicked() = mac 0x230e40, ios 0x2e1cb0;
	virtual void show() = mac 0x230c10, ios 0x2e1ad0;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x230da0, ios 0x2e1c4c;
	virtual void updateRate() = mac 0x230950, ios 0x2e18b8;
	void create();
	void onClose(cocos2d::CCObject* p0);
	void onSpecialItem(cocos2d::CCObject* p0);
	void switchToOpenedState(CCMenuItemSpriteExtra* p0);
	~FollowRewardPage() = mac 0x22f270, ios 0x2e07dc;
}

class FontObject : cocos2d::CCObject {
	void createWithConfigFile(char const* p0, float p1);
	void getFontWidth(int p0);
	bool initWithConfigFile(char const* p0, float p1);
	void parseConfigFile(char const* p0, float p1);
	~FontObject();
}

class FriendRequestDelegate {
	virtual void loadFRequestsFinished(cocos2d::CCArray* p0, char const* p1) {}
	virtual void loadFRequestsFailed(char const* p0, GJErrorCode p1) {}
	virtual void setupPageInfo(gd::string p0, char const* p1) {}
	virtual void forceReloadRequests(bool p0) {}
}

class FriendRequestPopup : FLAlertLayer, UploadActionDelegate, UploadPopupDelegate, FLAlertLayerProtocol {
	virtual void keyBackClicked() = mac 0x14e2a0, ios 0x207b2c;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x14e390, ios 0x207b38;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x14e560, ios 0x207d08;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x14e640, ios 0x207dc8;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x14e7c0, ios 0x207e40;
	void blockUser();
	void create(GJFriendRequest* p0);
	bool init(GJFriendRequest* p0);
	void loadFromGJFriendRequest(GJFriendRequest* p0);
	void onAccept(cocos2d::CCObject* p0);
	void onBlock(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onRemove(cocos2d::CCObject* p0);
	~FriendRequestPopup() = mac 0x14cc00, ios 0x2066d4;
}

class FriendsProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, UserListDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x3aa6e0, ios 0x133634;
	virtual void keyBackClicked() = mac 0x3aa5e0, ios 0x133628;
	virtual void getUserListFinished(cocos2d::CCArray* p0, UserListType p1) = mac 0x3aa240, ios 0x1333c4;
	virtual void getUserListFailed(UserListType p0, GJErrorCode p1) = mac 0x3aa390, ios 0x1334d8;
	virtual void userListChanged(cocos2d::CCArray* p0, UserListType p1) = mac 0x3aa4a0, ios 0x13357c;
	virtual void forceReloadList(UserListType p0) = mac 0x3aa4e0, ios 0x133598;
	void create(UserListType p0);
	bool init(UserListType p0);
	void onBlocked(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onUpdate(cocos2d::CCObject* p0);
	void setupUsersBrowser(cocos2d::CCArray* p0, UserListType p1);
	~FriendsProfilePage() = mac 0x3a94d0, ios 0x13289c;
}

class GJAccountBackupDelegate {
	virtual bool backupAccountFailed(BackupAccountError p0) {
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
	virtual bool loginAccountFailed(AccountError p0) {
		return false;
	}
	virtual bool loginAccountFinished(int p0, int p1) {
		return false;
	}
}

class GJAccountManager : cocos2d::CCNode {
	static GJAccountManager* sharedState() = win 0x107d50;
	virtual bool init() = mac 0x879d0, ios 0x284c04;
	void ProcessHttpRequest(gd::string p0, gd::string p1, gd::string p2, GJHttpType p3);
	void addDLToActive(char const* p0);
	void addDLToActive(char const* p0, cocos2d::CCObject* p1);
	void backupAccount(gd::string p0);
	void dataLoaded(DS_Dictionary* p0);
	void encodeDataTo(DS_Dictionary* p0);
	void firstSetup();
	void getAccountBackupURL();
	void getAccountSyncURL();
	void getDLObject(char const* p0);
	void getGJP();
	void handleIt(bool p0, gd::string p1, gd::string p2, GJHttpType p3);
	void handleItDelayed(bool p0, gd::string p1, gd::string p2, GJHttpType p3);
	void handleItND(cocos2d::CCNode* p0, void* p1);
	void isDLActive(char const* p0);
	void linkToAccount(gd::string p0, gd::string p1, int p2, int p3);
	void loginAccount(gd::string p0, gd::string p1);
	void onBackupAccountCompleted(gd::string p0, gd::string p1);
	void onGetAccountBackupURLCompleted(gd::string p0, gd::string p1);
	void onGetAccountSyncURLCompleted(gd::string p0, gd::string p1);
	void onLoginAccountCompleted(gd::string p0, gd::string p1);
	void onProcessHttpRequestCompleted(cocos2d::extension::CCHttpClient* p0, cocos2d::extension::CCHttpResponse* p1);
	void onRegisterAccountCompleted(gd::string p0, gd::string p1);
	void onSyncAccountCompleted(gd::string p0, gd::string p1);
	void onUpdateAccountSettingsCompleted(gd::string p0, gd::string p1);
	void registerAccount(gd::string p0, gd::string p1, gd::string p2);
	void removeDLFromActive(char const* p0);
	void syncAccount(gd::string p0);
	void unlinkFromAccount();
	void updateAccountSettings(int p0, int p1, int p2, gd::string p3, gd::string p4, gd::string p5);
	~GJAccountManager() = mac 0x87990, ios 0x284bc8;

	PAD = win 0x4;
	gd::string m_password;
	gd::string m_username;
	int m_accountID;
}

class GJAccountRegisterDelegate {
	virtual void registerAccountFinished() {}
	virtual void registerAccountFailed(AccountError p0) {}
}

class GJAccountSettingsDelegate {
	virtual void updateSettingsFinished() {}
	virtual void updateSettingsFailed() {}
}

class GJAccountSettingsLayer : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x14b200, ios 0x205460;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x14abe0, ios 0x2050d0;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x14aca0, ios 0x20513c;
	void create(int p0);
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, float p5, float p6);
	bool init(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onCommentSetting(cocos2d::CCObject* p0);
	void onFriendRequests(cocos2d::CCObject* p0);
	void onMessageSetting(cocos2d::CCObject* p0);
	void onUpdate(cocos2d::CCObject* p0);
	void updateScoreValues();
	~GJAccountSettingsLayer() = mac 0x148760, ios 0x20349c;
}

class GJAccountSyncDelegate {
	virtual bool syncAccountFailed(BackupAccountError p0) {
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

	virtual void objectsCollided(int p0, int p1) = mac 0xb6d90, ios 0x137ff0;
	virtual void createMoveCommand(cocos2d::CCPoint p0, int p1, float p2, int p3, float p4, bool p5, bool p6, int p7) = mac 0xb73a0, ios 0x1384a8;
	virtual void updateColor(cocos2d::ccColor3B p0, float p1, int p2, bool p3, float p4, cocos2d::ccHSVValue p5, int p6, bool p7, int p8, EffectGameObject* p9) = mac 0xb7420, ios 0x138530;
	virtual void flipGravity(PlayerObject* p0, bool p1, bool p2) = mac 0xba990;
	virtual void calculateColorValues(EffectGameObject* p0, EffectGameObject* p1, int p2, float p3, ColorActionSprite* p4, GJEffectManager* p5) = mac 0xba9a0;
	virtual void toggleGroupTriggered(int p0, bool p1) = mac 0xb75a0, win 0x10f890, ios 0x138674;
	virtual void spawnGroup(int p0) = mac 0xb7050, win 0x10f400, ios 0x138210;
	virtual void addToSection(GameObject* p0) = mac 0xb7b70, ios 0x138ad0;
	virtual void addToGroup(GameObject* p0, int p1, bool p2) = mac 0xb77f0, ios 0x138880;
	virtual void removeFromGroup(GameObject* p0, int p1) = mac 0xb7a60, ios 0x138a64;
	virtual bool init() = mac 0xafc90, ios 0x1338e8;
	void addObjectCounter(LabelGameObject* p0, int p1) = mac 0xb9eb0;
	void addToGroups(GameObject* p0, bool p1) = mac 0xb7780;
	void atlasValue(int p0) = mac 0xb21e0;
	void bumpPlayer(PlayerObject* p0, GameObject* p1) = mac 0xb6860;
	void calculateOpacityValues(EffectGameObject* p0, EffectGameObject* p1, float p2, GJEffectManager* p3) = mac 0xb5be0;
	void checkSpawnObjects() = mac 0xb6f90;
	void collectItem(int p0, int p1) = mac 0xb9e20;
	void collectedObject(EffectGameObject* p0) = mac 0xb9b60;
	void createTextLayers() = mac 0xb5260;
	cocos2d::CCArray* damagingObjectsInRect(cocos2d::CCRect p0) = mac 0xb6140;
	void enableHighCapacityMode() = mac 0xb11e0;
	void getCapacityString() = mac 0xb2210, win 0x10c9b0;
	void getGroundHeightForMode(int p0) = mac 0xb6630;
	void getGroup(int p0) = mac 0xb6f20;
	void getMoveDeltaForObjects(int p0, int p1) = mac 0xb6db0;
	void getOptimizedGroup(int p0) = mac 0xb7940;
	void getStaticGroup(int p0) = mac 0xb79a0;
	void isGroupDisabledForObject(GameObject* p0) = mac 0xb5cc0;
	void isGroupDisabledForObjectFull(GameObject* p0, cocos2d::CCArray* p1) = mac 0xb5de0;
	void loadUpToPosition(float p0) = mac 0xba680;
	void objectIntersectsCircle(GameObject* p0, GameObject* p1) = mac 0xb66e0;
	void objectTriggered(EffectGameObject* p0) = mac 0xb71b0;
	void optimizeMoveGroups() = mac 0xb96c0;
	void parentForZLayer(int p0, bool p1, int p2) = mac 0xb55d0;
	void playerTouchedRing(PlayerObject* p0, GameObject* p1) = mac 0xb69e0;
	void processColorObject(EffectGameObject* p0, int p1, cocos2d::CCDictionary* p2, float p3, GJEffectManager* p4) = mac 0xb5a90;
	void processFollowActions() = mac 0xb8fd0;
	void processMoveActions() = mac 0xb86c0;
	void processMoveActionsStep(float p0) = mac 0xb7ea0;
	void processOpacityObject(EffectGameObject* p0, cocos2d::CCDictionary* p1, float p2, GJEffectManager* p3) = mac 0xb5ae0;
	void processPlayerFollowActions(float p0) = mac 0xb8b50;
	void processRotationActions() = mac 0xb7fd0;
	void pushButton(int p0, bool p1) = mac 0xb9920, win 0x111500;
	void rectIntersectsCircle(cocos2d::CCRect p0, cocos2d::CCPoint p1, float p2) = mac 0xb6470;
	void refreshCounterLabels() = mac 0xb9fc0;
	void releaseButton(int p0, bool p1) = mac 0xb9a00, win 0x111660;
	void removeFromGroups(GameObject* p0) = mac 0xb7a00;
	void removeObjectFromSection(GameObject* p0) = mac 0xb7e00;
	void reorderObjectSection(GameObject* p0) = mac 0xb7cb0;
	void resetGroupCounters(bool p0) = mac 0xba300;
	void resetMoveOptimizedValue() = mac 0xb9670;
	int sectionForPos(float p0) = mac 0xb6120;
	void setupLayers() = mac 0xaffe0;
	void shouldExitHackedLevel() = mac 0xb1100;
	void spawnGroupTriggered(int p0, float p1, int p2) = mac 0xb7020;
	cocos2d::CCArray* staticObjectsInRect(cocos2d::CCRect p0) = mac 0xb5f90;
	void testInstantCountTrigger(int p0, int p1, int p2, bool p3, int p4) = mac 0xb9ae0;
	void toggleGroup(int p0, bool p1) = mac 0xb75f0;
	void togglePlayerVisibility(bool p0) = mac 0xba910;
	void triggerMoveCommand(EffectGameObject* p0) = mac 0xb7290;
	void updateCollisionBlocks() = mac 0xb6a30;
	void updateCounters(int p0, int p1) = mac 0xb9bc0;
	void updateDisabledObjectsLastPos(cocos2d::CCArray* p0) = mac 0xb95b0;
	void updateLayerCapacity(gd::string p0) = mac 0xb1680;
	void updateLegacyLayerCapacity(int p0, int p1, int p2, int p3) = mac 0xb1590;
	void updateOBB2(cocos2d::CCRect p0) = mac 0xb63f0;
	void updateQueuedLabels() = mac 0xb9f30;
	~GJBaseGameLayer() = mac 0xaf990, ios 0x13389c;
	void stopTriggersInGroup(int p0);

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

class GJChallengeDelegate {
	virtual void challengeStatusFinished() {}
	virtual void challengeStatusFailed() {}
}

class GJChallengeItem : cocos2d::CCObject {
	virtual void encodeWithCoder(DS_Dictionary* p0) = mac 0x4df60, ios 0x25331c;
	virtual bool canEncode() = mac 0x4e020, ios 0x2533f8;
	void create(GJChallengeType p0, int p1, int p2, int p3, gd::string p4);
	void create();
	void createFromString(gd::string p0);
	void createWithCoder(DS_Dictionary* p0);
	void dataLoaded(DS_Dictionary* p0);
	void incrementCount(int p0);
	bool init(GJChallengeType p0, int p1, int p2, int p3, gd::string p4);
	void setCount(int p0);
	~GJChallengeItem() = mac 0x4e1b0, ios 0x253530;

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

class GJChestSprite : cocos2d::CCSprite {
	virtual void setColor(cocos2d::ccColor3B const& p0) = mac 0xf8270, ios 0x1272f4;
	void create(int p0);
	bool init(int p0);
	void switchToState(ChestSpriteState p0, bool p1);
	~GJChestSprite() = mac 0xf81e0, ios 0x127260;
}

class GJColorSetupLayer : FLAlertLayer, ColorSelectDelegate, FLAlertLayerProtocol {
	virtual void keyBackClicked() = mac 0xf0210, ios 0xf4fd0;
	virtual void colorSelectClosed(cocos2d::CCNode* p0) = mac 0xf01e0, ios 0xf4fc4;
	void create(LevelSettingsObject* p0);
	bool init(LevelSettingsObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onColor(cocos2d::CCObject* p0);
	void onPage(cocos2d::CCObject* p0);
	void showPage(int p0);
	void updateSpriteColor(ColorChannelSprite* p0, cocos2d::CCLabelBMFont* p1, int p2);
	void updateSpriteColors();
	~GJColorSetupLayer() = mac 0xef350, ios 0xf4438;
}

class GJComment : cocos2d::CCNode {
	~GJComment() = mac 0x2dfd70, ios 0x72798;
	virtual bool init() = mac 0x2dfec0, ios 0x728b0;
	static GJComment* create(cocos2d::CCDictionary* dict) = win 0xc3740;
	void create();

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
	~GJCommentListLayer() = mac 0x1482a0, ios 0x203334;
	static GJCommentListLayer* create(BoomListView* p0, char const* p1, cocos2d::ccColor4B p2, float p3, float p4, bool p5) = mac 0x147d00;
	bool init(BoomListView* p0, char const* p1, cocos2d::ccColor4B p2, float p3, float p4, bool p5);

	BoomListView* m_list;
}

class GJDailyLevelDelegate {
	virtual void dailyStatusFinished(bool p0) {}
	virtual void dailyStatusFailed(bool p0) {}
}

class GJDropDownLayer : cocos2d::CCLayerColor {
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

	virtual void customSetup() {}
	virtual void enterAnimFinished() {}
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

	virtual void enterLayer() = mac 0x3525c0, win 0x16970, ios 0x297f28;
	virtual void exitLayer(cocos2d::CCObject* p0) = mac 0x352670, win 0x113980, ios 0x297fbc;
	virtual void showLayer(bool p0) = mac 0x3526c0, win 0x1139c0, ios 0x29801c;
	virtual void hideLayer(bool p0) = mac 0x3527b0, win 0x113a90, ios 0x29811c;
	virtual void layerVisible() = mac 0x3528b0, win 0x16a40, ios 0x29821c;
	virtual void layerHidden() = mac 0x3528d0, win 0x113b60, ios 0x29822c;
	virtual void disableUI() = mac 0x352580, win 0x113920, ios 0x297f00;
	virtual void enableUI() = mac 0x3525a0, win 0x113940, ios 0x297f14;
	virtual void draw() = mac 0x352910, win 0x16a80, ios 0x29827c;
	bool init(char const* title, float height) = mac 0x352100, win 0x113530, ios 0x297afc;
	virtual void registerWithTouchDispatcher() = mac 0x3525f0, win 0x16990, ios 0x297f60;
	virtual void keyBackClicked() = mac 0x352630, win 0x113960, ios 0x297f98;
	void create(char const* p0);
	bool init(char const* p0);
	~GJDropDownLayer() = mac 0x351d00, ios 0x297ab0;

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
	virtual void dropDownLayerWillClose(GJDropDownLayer* p0) {}
}

class GJEffectManager : cocos2d::CCNode {
	virtual bool init() = mac 0x180230, ios 0xdf5c;
	void activeColorForIndex(int p0) = mac 0x180cb0;
	void activeOpacityForIndex(int p0) = mac 0x180e10;
	void addAllInheritedColorActions(cocos2d::CCArray* p0) = mac 0x1817a0;
	void addGroupPulseEffect(PulseEffectAction* p0) = mac 0x184c10;
	void calculateBaseActiveColors() = mac 0x180f70;
	void calculateInheritedColor(int p0, ColorAction* p1) = mac 0x1818f0;
	void calculateLightBGColor(cocos2d::ccColor3B p0) = mac 0x185b90;
	void colorActionChanged(ColorAction* p0) = mac 0x181dc0;
	void colorExists(int p0) = mac 0x181da0;
	void colorForEffect(cocos2d::ccColor3B p0, cocos2d::ccHSVValue p1) = mac 0x182650;
	cocos2d::ccColor3B colorForGroupID(int p0, cocos2d::ccColor3B const& p1, bool p2) = mac 0x184f90;
	void colorForIndex(int p0) = mac 0x180ad0;
	void colorForPulseEffect(cocos2d::ccColor3B const& p0, PulseEffectAction* p1) = mac 0x181bf0;
	void countChangedForItem(int p0) = mac 0x185a40;
	void countForItem(int p0) = mac 0x185a10;
	static GJEffectManager* create() = mac 0x1800f0;
	void createFollowCommand(float p0, float p1, float p2, int p3, int p4, bool p5, int p6) = mac 0x182ed0;
	void createMoveCommand(cocos2d::CCPoint p0, int p1, float p2, int p3, float p4, bool p5, bool p6, int p7) = mac 0x182cc0;
	void createPlayerFollowCommand(float p0, float p1, int p2, float p3, float p4, int p5, int p6) = mac 0x182fe0;
	void createRotateCommand(int p0, float p1, int p2, int p3, int p4, float p5, bool p6, int p7) = mac 0x182df0;
	void getAllColorActions() = mac 0x180980;
	void getAllColorSprites() = mac 0x1809e0;
	cocos2d::ccColor3B const& getColorAction(int p0) = mac 0x180b00, win 0x11cde0;
	cocos2d::ccColor3B const& getColorSprite(int p0) = mac 0x180d00;
	void getCurrentStateString() = mac 0x1867e0;
	void getLoadedMoveOffset() = mac 0x184390;
	void getMixedColor(cocos2d::ccColor3B p0, cocos2d::ccColor3B p1, float p2) = mac 0x185d30;
	uint8_t getOpacityActionForGroup(int p0) = mac 0x1845b0;
	void getSaveString() = mac 0x185e90;
	void handleObjectCollision(bool p0, int p1, int p2) = mac 0x1828f0;
	void hasActiveDualTouch() = mac 0x185540;
	void hasBeenTriggered(int p0) = mac 0x1853b0;
	void hasPulseEffectForGroupID(int p0) = mac 0x184f60;
	bool isGroupEnabled(int p0) = mac 0x1853d0;
	void keyForGroupIDColor(int p0, cocos2d::ccColor3B const& p1, bool p2) = mac 0x184c90;
	void loadState(gd::string p0) = mac 0x188db0;
	void objectsCollided(int p0, int p1) = mac 0x182a00;
	void opacityForIndex(int p0) = mac 0x180c80;
	float opacityModForGroup(int p0) = mac 0x184740;
	void playerButton(bool p0, bool p1) = mac 0x1855a0;
	void playerDied() = mac 0x185860;
	void postCollisionCheck() = mac 0x182720;
	void preCollisionCheck() = mac 0x182680;
	void prepareMoveActions(float p0, bool p1) = mac 0x183660;
	void processColors() = mac 0x180e70;
	void processCopyColorPulseActions() = mac 0x181530;
	void processInheritedColors() = mac 0x181190;
	void processPulseActions() = mac 0x181040;
	void registerCollisionTrigger(int p0, int p1, int p2, bool p3, bool p4, int p5) = mac 0x182b70;
	void removeAllPulseActions() = mac 0x1825e0;
	void removeColorAction(int p0) = mac 0x181d60;
	void reset() = mac 0x180690;
	void resetColorCache() = mac 0x185280;
	void resetEffects() = mac 0x1807d0;
	void resetMoveActions() = mac 0x180940;
	void resetToggledGroups() = mac 0x1853f0;
	void resetTriggeredIDs() = mac 0x182630;
	void runCountTrigger(int p0, int p1, bool p2, int p3, bool p4, int p5) = mac 0x1858d0;
	void runDeathTrigger(int p0, bool p1, int p2) = mac 0x1857a0;
	OpacityEffectAction* runOpacityActionOnGroup(int p0, float p1, float p2, int p3) = mac 0x1845d0;
	void runPulseEffect(int p0, bool p1, float p2, float p3, float p4, PulseEffectType p5, cocos2d::ccColor3B p6, cocos2d::ccHSVValue p7, int p8, bool p9, bool p10, bool p11, int p12) = mac 0x184890;
	void runTouchTriggerCommand(int p0, bool p1, TouchTriggerType p2, bool p3, int p4) = mac 0x185460;
	void setColorAction(ColorAction* p0, int p1) = mac 0x181d00;
	void setFollowing(int p0, int p1, bool p2) = mac 0x185e00;
	void setupFromString(gd::string p0) = mac 0x186290;
	void shouldBlend(int p0) = mac 0x180e40;
	void spawnGroup(int p0, float p1, int p2) = mac 0x1852a0;
	void stopActionsForTrigger(EffectGameObject* p0) = mac 0x183150;
	void stopMoveActionsForGroup(int p0) = mac 0x1830e0;
	void storeTriggeredID(int p0) = mac 0x185380;
	void toggleGroup(int p0, bool p1) = mac 0x182c80;
	void traverseInheritanceChain(InheritanceNode* p0) = mac 0x181850;
	void updateActiveOpacityEffects() = mac 0x1847e0;
	void updateColorAction(ColorAction* p0) = mac 0x184560;
	void updateColorEffects(float p0) = mac 0x181f40;
	void updateColors(cocos2d::ccColor3B p0, cocos2d::ccColor3B p1) = mac 0x180a40;
	void updateEffects(float p0) = mac 0x181df0;
	void updateOpacityAction(OpacityEffectAction* p0) = mac 0x184780;
	void updateOpacityEffects(float p0) = mac 0x1823e0, win 0x11e490;
	void updatePulseEffects(float p0) = mac 0x182130;
	void updateSpawnTriggers(float p0) = mac 0x182510;
	void wasFollowing(int p0, int p1) = mac 0x185e60;
	void wouldCreateLoop(InheritanceNode* p0, int p1) = mac 0x181820;
	~GJEffectManager() = mac 0x17fe00, ios 0xde6c;
	void decrementCountForItem(int p0);
	void incrementCountForItem(int p0);

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

class GJFlyGroundLayer : GJGroundLayer {
	virtual bool init() = mac 0x356c00;
	void create();
	~GJFlyGroundLayer() = mac 0x356cd0;
}

class GJFollowCommandLayer : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x16da20, ios 0xc223c;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x16d440, ios 0xc1f88;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x16d480, ios 0xc1fa4;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x16dc00, ios 0xc2248;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x16dcc0, ios 0xc22b4;
	void create(EffectGameObject* p0, cocos2d::CCArray* p1);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onFollowRotation(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void onUpdateGroupID(cocos2d::CCObject* p0);
	void onUpdateGroupID2(cocos2d::CCObject* p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void sliderXModChanged(cocos2d::CCObject* p0);
	void sliderYModChanged(cocos2d::CCObject* p0);
	void updateDurLabel(bool p0);
	void updateDuration();
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetGroupID();
	void updateTargetGroupID2();
	void updateTextInputLabel();
	void updateTextInputLabel2();
	void updateTouchTriggered();
	void updateXMod();
	void updateXModLabel();
	void updateYMod();
	void updateYModLabel();
	~GJFollowCommandLayer() = mac 0x16a4b0, ios 0xbf98c;
}

class GJFriendRequest : cocos2d::CCNode {
	virtual bool init() = mac 0x2dea00, ios 0x71c5c;
	void create(cocos2d::CCDictionary* p0);
	void create();
	~GJFriendRequest() = mac 0x2de830, ios 0x71b8c;
}

class GJGameLevel : cocos2d::CCNode {
	static GJGameLevel* createWithCoder(DS_Dictionary* dict) {
	    auto ret = GJGameLevel::create();
	    ret->dataLoaded(dict);
	    return ret;
	}

	~GJGameLevel() = mac 0x2db2d0, ios 0x6f144;
	virtual void encodeWithCoder(DS_Dictionary* p0) = mac 0x2dd1c0, ios 0x708a0;
	virtual bool canEncode() = mac 0x2ddae0, ios 0x71134;
	virtual bool init() = mac 0x2db310, ios 0x6f180;
	static GJGameLevel* create() = mac 0x2b83e0, win 0xbd2b0, ios 0x51fe8;
	gd::string getAudioFileName() = mac 0x2dbe70, win 0xbdc70;
	const char* getCoinKey(int p0) = mac 0x2ce360, win 0xbda50;
	void getLengthKey(int p0) = mac 0x2dbba0;
	void getNormalPercent() = mac 0x2b8b20;
	void levelWasAltered() = mac 0x2db530, win 0xbd550;
	void savePercentage(int p0, bool p1, int p2, int p3, bool p4) = mac 0x2db700;
	void dataLoaded(DS_Dictionary* dict) = mac 0x2922f0, win 0xbded0, ios 0x6fca4;
	GJDifficulty getAverageDifficulty() = win 0xbd9b0;
	gd::string getUnpackedLevelDescription() = win 0xbf890;
	void areCoinsVerified();
	void create(cocos2d::CCDictionary* p0, bool p1);
	void demonIconForDifficulty(DemonDifficultyType p0);
	void getLastBuildPageForTab(int p0);
	void getLevelName();
	void getSongName();
	void handleStatsConflict(GJGameLevel* p0);
	void lengthKeyToString(int p0);
	void levelWasSubmitted();
	void setAccountID(int p0);
	void setAttemptTime(int p0);
	void setAttempts(int p0);
	void setCapacityString(gd::string p0);
	void setClicks(int p0);
	void setCoinsVerified(int p0);
	void setDailyID(int p0);
	void setDemon(int p0);
	void setJumps(int p0);
	void setLastBuildPageForTab(int p0, int p1);
	void setLastBuildSave(cocos2d::CCDictionary* p0);
	void setLevelID(int p0);
	void setLevelName(gd::string p0);
	void setNewNormalPercent(int p0);
	void setNewNormalPercent2(int p0);
	void setNormalPercent(int p0);
	void setPassword(int p0);
	void setStars(int p0);
	void unverifyCoins();

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

	void onPlayerColor1(cocos2d::CCObject* p0) = mac 0x1ba640, win 0x129470, ios 0x22531c;
	void onPlayerColor2(cocos2d::CCObject* p0) = mac 0x1ba8c0, win 0x129590, ios 0x225408;
	static GJGarageLayer* create() = win 0x125220;
	virtual bool init() = mac 0x1b4980, win 0x1255d0, ios 0x22045c;
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
	virtual void keyBackClicked() = mac 0x1bb630, ios 0x225d04;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0x1b7dc0, ios 0x222fc0;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x1b8040, ios 0x223110;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x1b82c0, ios 0x2232cc;
	virtual void listButtonBarSwitchedPage(ListButtonBar* p0, int p1) = mac 0x1bad10, ios 0x2256b4;
	virtual void updateRate() = mac 0x1bb420, ios 0x225b74;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x1bb050, ios 0x225948;
	virtual void dialogClosed(DialogLayer* p0) = mac 0x1bb160, ios 0x225a44;
	void achievementForUnlock(int p0, UnlockType p1);
	void checkBall(int p0);
	void checkBird(int p0);
	void checkColor(int p0, bool p1);
	void checkDart(int p0);
	void checkDeathEffect(int p0);
	void checkIcon(int p0);
	void checkRobot(int p0);
	void checkShip(int p0);
	void checkSpecial(int p0);
	void checkSpider(int p0);
	void descriptionForUnlock(int p0, UnlockType p1);
	void getLockFrame(int p0, UnlockType p1);
	void node();
	void onDeathEffect(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onKeys(cocos2d::CCObject* p0);
	void onSpecialIcon(cocos2d::CCObject* p0);
	void playRainbowEffect();
	void playShadowEffect();
	void scene();
	void selectPage(IconType p0);
	void setupColorSelect();
	void setupIconSelect();
	void showUnlockPopup(int p0, UnlockType p1);
	void toggleGlow();
	void updateColorSelect(cocos2d::CCNode* p0, bool p1);
	void updatePlayerColors();
	void updatePlayerName(char const* p0);
	void updatePlayerSelect(cocos2d::CCNode* p0);
	~GJGarageLayer() = mac 0x1b4430, ios 0x2202f4;

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

class GJGroundLayer : cocos2d::CCLayer {
	void updateGroundWidth() = win 0x12dda0;
	virtual void draw() = mac 0x356a60, ios 0x1fbf4;
	virtual void showGround() = mac 0x356920, ios 0x1fab0;
	virtual void fadeInGround(float p0) = mac 0x356930, ios 0x1fabc;
	virtual void fadeOutGround(float p0) = mac 0x356a30, ios 0x1fbc8;
	void create(int p0, int p1);
	void createLine(int p0);
	void deactivateGround();
	void fadeInFinished();
	void getGroundY();
	void hideShadows();
	bool init(int p0, int p1);
	void loadGroundSprites(int p0, bool p1);
	void positionGround(float p0);
	void updateGround01Color(cocos2d::ccColor3B p0);
	void updateGround02Color(cocos2d::ccColor3B p0);
	void updateGroundPos(cocos2d::CCPoint p0);
	void updateLineBlend(bool p0);
	~GJGroundLayer() = mac 0x356c30;
}

class GJHttpResult : cocos2d::CCNode {
	void create(bool p0, gd::string p1, gd::string p2, GJHttpType p3);
	bool init(bool p0, gd::string p1, gd::string p2, GJHttpType p3);
	~GJHttpResult() = mac 0x2dff80;
}

class GJItemIcon : cocos2d::CCSprite {
	GJItemIcon* createBrowserIcon(UnlockType _type, int _id) {
	    return GJItemIcon::create(_type, _id,
	        { 0xaf, 0xaf, 0xaf }, { 0xff, 0xff, 0xff },
	        false, true, true,
	        { 0xff, 0xff, 0xff }
	    );
	}

	static GJItemIcon* create(UnlockType _type, int _id, cocos2d::ccColor3B _col1, cocos2d::ccColor3B _col2, bool _un0, bool _un1, bool _un2, cocos2d::ccColor3B _col3) = mac 0x1bb330, win 0x12cbf0, ios 0x227188;
	virtual void setOpacity(unsigned char p0) = mac 0x1bd9d0, ios 0x227a30;
	void createBrowserItem(UnlockType p0, int p1);
	void createStoreItem(UnlockType p0, int p1, bool p2, cocos2d::ccColor3B p3);
	bool init(UnlockType p0, int p1, cocos2d::ccColor3B p2, cocos2d::ccColor3B p3, bool p4, bool p5, bool p6, cocos2d::ccColor3B p7);
	~GJItemIcon() = mac 0x1bcd10, ios 0x22712c;
}

class GJLevelScoreCell : TableViewCell {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
	virtual bool init() = mac 0x11dbb0, ios 0xa61a0;
	virtual void draw() = mac 0x11dc00, ios 0xa61e4;
	void loadFromScore(GJUserScore* p0);
	void onViewProfile(cocos2d::CCObject* p0);
	void updateBGColor(int p0);
	~GJLevelScoreCell() = mac 0x11da90, ios 0xa6154;
}

class GJListLayer : cocos2d::CCLayerColor {
	~GJListLayer() = mac 0x344350, ios 0x3e78c;
	static GJListLayer* create(cocos2d::CCObject* target, char const* title, cocos2d::ccColor4B color, float width, float height) = mac 0x343e10, win 0x12e000;
	void create(BoomListView* p0, char const* p1, cocos2d::ccColor4B p2, float p3, float p4);
	bool init(BoomListView* p0, char const* p1, cocos2d::ccColor4B p2, float p3, float p4);

	BoomListView* m_listView;
}

class GJMapPack : cocos2d::CCNode {
	virtual bool init() = mac 0x2de0e0, ios 0x71740;
	void completedMaps();
	void create(cocos2d::CCDictionary* p0);
	void create();
	void hasCompletedMapPack();
	void parsePackColors(gd::string p0, gd::string p1);
	void parsePackLevels(gd::string p0);
	void totalMaps();
	~GJMapPack() = mac 0x2ddd50, ios 0x712e0;

	cocos2d::CCArray* m_levels;
	int m_packID;
	GJDifficulty m_difficulty;
	int m_stars;
	int m_coins;
	gd::string m_packName;
	gd::string m_levelStrings;
	cocos2d::ccColor3B m_textColor;
	cocos2d::ccColor3B m_barColor;
	int m_MId;
	bool m_isGauntlet;
}

class GJMessageCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
	virtual bool init() = mac 0x11fc00, ios 0xa78d8;
	virtual void draw() = mac 0x11fef0, ios 0xa7b30;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x11ffc0, ios 0xa7be0;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x120180, ios 0xa7ce8;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x120310, ios 0xa7ea8;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x120410, ios 0xa7f68;
	void loadFromMessage(GJUserMessage* p0);
	void markAsRead();
	void onDeleteMessage(cocos2d::CCObject* p0);
	void onToggle(cocos2d::CCObject* p0);
	void onViewMessage(cocos2d::CCObject* p0);
	void onViewProfile(cocos2d::CCObject* p0);
	void updateBGColor(int p0);
	void updateToggle();
	~GJMessageCell() = mac 0x11fa90, ios 0xa788c;
}

class GJMessagePopup : FLAlertLayer, UploadActionDelegate, UploadPopupDelegate, FLAlertLayerProtocol, DownloadMessageDelegate {
	virtual void keyBackClicked() = mac 0x1504a0, ios 0x2096b0;
	virtual void downloadMessageFinished(GJUserMessage* p0) = mac 0x150510, ios 0x20972c;
	virtual void downloadMessageFailed(int p0) = mac 0x1505e0, ios 0x2097a0;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x150680, ios 0x209858;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x150820, ios 0x209a14;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x150900, ios 0x209ad8;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x150a00, ios 0x209b70;
	void blockUser();
	void create(GJUserMessage* p0);
	bool init(GJUserMessage* p0);
	void loadFromGJMessage(GJUserMessage* p0);
	void onBlock(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onRemove(cocos2d::CCObject* p0);
	void onReply(cocos2d::CCObject* p0);
	~GJMessagePopup() = mac 0x14e9a0, ios 0x207fc4;
}

class GJMoreGamesLayer : GJDropDownLayer {
	virtual void customSetup() = mac 0x43ae60, ios 0x3a02c;
	void create();
	void getMoreGamesList();
	~GJMoreGamesLayer() = mac 0x43a790, ios 0x398bc;
}

class GJMoveCommandLayer : FLAlertLayer, TextInputDelegate, ConfigureValuePopupDelegate {
	virtual void keyBackClicked() = mac 0x374970, ios 0x1ad330;
	virtual void valuePopupClosed(ConfigureValuePopup* p0, float p1) = mac 0x373c20, ios 0x1ac9e0;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x374430, ios 0x1ad03c;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x374470, ios 0x1ad058;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x374b50, ios 0x1ad33c;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x374c10, ios 0x1ad3a8;
	void create(EffectGameObject* p0, cocos2d::CCArray* p1);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onEasing(cocos2d::CCObject* p0);
	void onEasingRate(cocos2d::CCObject* p0);
	void onLockPlayerX(cocos2d::CCObject* p0);
	void onLockPlayerY(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTargetMode(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void onUpdateGroupID(cocos2d::CCObject* p0);
	void onUpdateGroupID2(cocos2d::CCObject* p0);
	void onUseMoveTarget(cocos2d::CCObject* p0);
	void posFromSliderValue(float p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void sliderValueFromPos(int p0);
	void sliderXChanged(cocos2d::CCObject* p0);
	void sliderYChanged(cocos2d::CCObject* p0);
	void toggleEasingRateVisibility();
	void updateDurLabel(bool p0);
	void updateDuration();
	void updateEasingLabel();
	void updateEasingRateLabel();
	void updateEditorLabel();
	void updateMoveCommandEasing();
	void updateMoveCommandEasingRate();
	void updateMoveCommandPosX();
	void updateMoveCommandPosY();
	void updateMoveTargetElements();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetGroupID();
	void updateTargetGroupID2();
	void updateTextInputLabel();
	void updateTextInputLabel2();
	void updateTouchTriggered();
	void updateValueXLabel();
	void updateValueYLabel();
	~GJMoveCommandLayer() = mac 0x36f800, ios 0x1a9214;
}

class GJObjectDecoder : cocos2d::CCNode, ObjectDecoderDelegate {
	virtual bool init() = mac 0x41e780, ios 0x13f9f8;
	virtual void getDecodedObject(int p0, DS_Dictionary* p1) = mac 0x41e790, ios 0x13f9fc;
	void sharedDecoder();
	~GJObjectDecoder() = mac 0x41e890;
}

class GJOptionsLayer : FLAlertLayer {
	virtual bool init() = mac 0x145c60, ios 0x200ce8;
	virtual void keyBackClicked() = mac 0x147190, ios 0x2023a8;
	virtual void setupOptions() = mac 0x1464d0, ios 0x201554;
	virtual void onClose(cocos2d::CCObject* p0) = mac 0x147160, ios 0x20236c;
	void addToggle(char const* p0, char const* p1, char const* p2);
	void countForPage(int p0);
	void create();
	void goToPage(int p0);
	void incrementCountForPage(int p0);
	void infoKey(int p0);
	void layerForPage(int p0);
	void layerKey(int p0);
	void nextPosition(int p0);
	void objectKey(int p0);
	void objectsForPage(int p0);
	void onInfo(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void onToggle(cocos2d::CCObject* p0);
	void pageKey(int p0);
	~GJOptionsLayer() = mac 0x145a40, ios 0x200c9c;
}

class GJPFollowCommandLayer : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x1b2590, ios 0xd7c18;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x1b1d80, ios 0xd7840;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x1b1dc0, ios 0xd785c;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x1b27a0, ios 0xd7c24;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x1b2860, ios 0xd7c90;
	void create(EffectGameObject* p0, cocos2d::CCArray* p1);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void onUpdateGroupID(cocos2d::CCObject* p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void sliderXModChanged(cocos2d::CCObject* p0);
	void sliderYModChanged(cocos2d::CCObject* p0);
	void updateDurLabel(bool p0);
	void updateDuration();
	void updateEditorLabel();
	void updateMaxSpeed();
	void updateMaxSpeedLabel();
	void updateMultiTriggerBtn();
	void updateOffsetLabel();
	void updatePlayerOffset();
	void updateSpawnedByTrigger();
	void updateTargetGroupID();
	void updateTextInputLabel();
	void updateTouchTriggered();
	void updateXMod();
	void updateXModLabel();
	void updateYMod();
	void updateYModLabel();
	~GJPFollowCommandLayer() = mac 0x1ae980, ios 0xd4cb8;
}

class GJPurchaseDelegate {
	virtual void didPurchaseItem(GJStoreItem* p0) {}
}

class GJRequestCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
	virtual bool init() = mac 0x11f080, ios 0xa709c;
	virtual void draw() = mac 0x11f390, ios 0xa7308;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x11f460, ios 0xa73b8;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x11f640, ios 0xa74d4;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x11f800, ios 0xa76b0;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x11f930, ios 0xa778c;
	void loadFromScore(GJUserScore* p0);
	void markAsRead();
	void onDeleteRequest(cocos2d::CCObject* p0);
	void onToggle(cocos2d::CCObject* p0);
	void onViewFriendRequest(cocos2d::CCObject* p0);
	void onViewProfile(cocos2d::CCObject* p0);
	void updateBGColor(int p0);
	void updateToggle();
	~GJRequestCell() = mac 0x11ef10, ios 0xa7050;
}

class GJRewardDelegate {
	virtual void rewardsStatusFinished(int p0) {}
	virtual void rewardsStatusFailed() {}
}

class GJRewardItem : cocos2d::CCObject {
	virtual void encodeWithCoder(DS_Dictionary* p0) = mac 0x4d7f0, ios 0x252d08;
	virtual bool canEncode() = mac 0x4d850, ios 0x252d74;
	void create(int p0, int p1, gd::string p2);
	void create();
	void createSpecial(GJRewardType p0, int p1, int p2, SpecialRewardItem p3, int p4, SpecialRewardItem p5, int p6, int p7, int p8);
	void createWithCoder(DS_Dictionary* p0);
	void createWithObject(GJRewardType p0, GJRewardObject* p1);
	void createWithObjects(GJRewardType p0, cocos2d::CCArray* p1);
	void dataLoaded(DS_Dictionary* p0);
	void getNextShardType(SpecialRewardItem p0);
	void getRandomNonMaxShardType();
	void getRandomShardType();
	void getRewardCount(SpecialRewardItem p0);
	void getRewardObjectForType(SpecialRewardItem p0);
	bool init(int p0, int p1, gd::string p2);
	void isShardType(SpecialRewardItem p0);
	void rewardItemToStat(SpecialRewardItem p0);
	void setObjects(cocos2d::CCArray* p0);
	~GJRewardItem() = mac 0x4d250, ios 0x252a0c;
}

class GJRewardObject : cocos2d::CCObject {
	virtual void encodeWithCoder(DS_Dictionary* p0) = mac 0x4d170, ios 0x252930;
	virtual bool canEncode() = mac 0x4d1f0, ios 0x2529b0;
	void create(SpecialRewardItem p0, int p1, int p2);
	void create();
	void createItemUnlock(UnlockType p0, int p1);
	void createWithCoder(DS_Dictionary* p0);
	void dataLoaded(DS_Dictionary* p0);
	bool init(SpecialRewardItem p0, int p1, int p2);
	void isSpecialType();
	~GJRewardObject() = mac 0x4e170, ios 0x14dc4;

	SpecialRewardItem m_specialRewardItem;
	UnlockType m_unlockType;
	int m_itemID;
	int m_total;
}

class GJRobotSprite : CCAnimatedSprite {
	~GJRobotSprite() = mac 0x34aaf0, ios 0x161510;
	virtual bool init() = mac 0x34ad50, ios 0x161634;
	virtual void setOpacity(unsigned char p0) = mac 0x34bcc0, ios 0x1625d4;
	virtual void hideSecondary() = mac 0x34c3b0, ios 0x162ce4;
	static GJRobotSprite* create() = mac 0x34ac00;
	void updateColor02(cocos2d::ccColor3B p0) = mac 0x34bbd0;
	void updateFrame(int p0) = mac 0x34bdd0;
	void hideGlow() = mac 0x34b860;
	bool init(gd::string p0);
	void showGlow();
	void updateColor01(cocos2d::ccColor3B p0);
	void updateColors();
	void updateGlowColor(cocos2d::ccColor3B p0, bool p1);

	PAD = win 0x8;
	cocos2d::ccColor3B m_secondaryColor;
}

class GJRotateCommandLayer : FLAlertLayer, TextInputDelegate, ConfigureValuePopupDelegate {
	virtual void keyBackClicked() = mac 0x5e30, ios 0x2ce328;
	virtual void valuePopupClosed(ConfigureValuePopup* p0, float p1) = mac 0x5160, ios 0x2cdaf4;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x5890, ios 0x2ce090;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x58d0, ios 0x2ce0ac;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x6010, ios 0x2ce334;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x60d0, ios 0x2ce3a0;
	void create(EffectGameObject* p0, cocos2d::CCArray* p1);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onEasing(cocos2d::CCObject* p0);
	void onEasingRate(cocos2d::CCObject* p0);
	void onLockRotation(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void onUpdateGroupID(cocos2d::CCObject* p0);
	void onUpdateGroupID2(cocos2d::CCObject* p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void sliderDegreesChanged(cocos2d::CCObject* p0);
	void sliderTimesChanged(cocos2d::CCObject* p0);
	void toggleEasingRateVisibility();
	void updateCommandDegrees();
	void updateCommandTimes();
	void updateDegreesLabel();
	void updateDurLabel(bool p0);
	void updateDuration();
	void updateEasingLabel();
	void updateEasingRateLabel();
	void updateEditorLabel();
	void updateMoveCommandEasing();
	void updateMoveCommandEasingRate();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetGroupID();
	void updateTargetGroupID2();
	void updateTextInputLabel();
	void updateTextInputLabel2();
	void updateTimesLabel();
	void updateTouchTriggered();
	~GJRotateCommandLayer() = mac 0x1b00, ios 0x2cae28;
}

class GJRotationControl : cocos2d::CCLayer {
	void setAngle(float angle) {
	    this->m_sliderPosition = cocos2d::CCPointMake(sinf(angle) * 60.0f, cosf(angle) * 60.0f);
	    this->m_angle = angle;
	
	    this->m_sliderThumb->setPosition(this->m_sliderPosition);
	}

	void updateSliderPosition(cocos2d::CCPoint const& pos) = win 0x94020;
	virtual bool init() = mac 0x31510, ios 0x2c4914;
	virtual void draw() = mac 0x31ac0, ios 0x2c4ce8;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x31790, ios 0x2c4b20;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x318b0, ios 0x2c4c1c;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x31990, ios 0x2c4c98;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x31a80, ios 0x2c4ccc;
	void create();
	void finishTouch();
	void updateSliderPosition(cocos2d::CCPoint p0);
	~GJRotationControl() = mac 0x320e0;

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
	virtual void angleChanged(float p0) {}
}

class GJScaleControl : cocos2d::CCLayer {
	void updateLabel(float value) = win 0x94990;
	void loadValues(GameObject* obj, cocos2d::CCArray* objs) = win 0x94590;
	virtual bool init() = mac 0x31b30, ios 0x2c4d5c;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x31d30, ios 0x2c4f08;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x31e60, ios 0x2c4fb8;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x31fb0, ios 0x2c50b4;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x32060, ios 0x2c5128;
	void create();
	void finishTouch();
	void scaleFromValue(float p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void valueFromScale(float p0);
	~GJScaleControl() = mac 0x32180, ios 0x1fc80;

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
	virtual void scaleChanged(float p0) {}
}

class GJScoreCell : TableViewCell, FLAlertLayerProtocol {
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x11d8e0, win 0x624a0, ios 0xa604c;
	void loadFromScore(GJUserScore* score) = win 0x61440;
	void onViewProfile(cocos2d::CCObject* pSender) = win 0x62380;
	void updateBGColor(unsigned int index) = win 0x5c6b0;
	virtual bool init() = mac 0x11d710, ios 0xa5e90;
	virtual void draw() = mac 0x11d9b0, ios 0xa60a4;
	void onBan(cocos2d::CCObject* p0);
	void onCheck(cocos2d::CCObject* p0);
	void onMoreLevels(cocos2d::CCObject* p0);
	void updateBGColor(int p0);
	~GJScoreCell() = mac 0x11d5e0, ios 0xa5e44;
}

class GJSearchObject : cocos2d::CCNode {
	SearchType getType() {
	    return this->m_searchType;
	}

	static GJSearchObject* create(SearchType nID) = win 0xc2b90;
	static GJSearchObject* create(SearchType nID, gd::string str) = win 0xc2c80;
	void create(SearchType p0, gd::string p1, gd::string p2, gd::string p3, int p4, bool p5, bool p6, bool p7, int p8, bool p9, bool p10, bool p11, bool p12, bool p13, bool p14, bool p15, bool p16, int p17, int p18);
	void createFromKey(char const* p0);
	void getKey();
	void getNextPageKey();
	void getNextPageObject();
	void getPageObject(int p0);
	void getPrevPageObject();
	void getSearchKey(SearchType p0, gd::string p1, gd::string p2, gd::string p3, int p4, bool p5, bool p6, bool p7, int p8, bool p9, bool p10, bool p11, bool p12, bool p13, bool p14, bool p15, bool p16, int p17, int p18);
	void getString();
	bool init(SearchType p0, gd::string p1, gd::string p2, gd::string p3, int p4, bool p5, bool p6, bool p7, int p8, bool p9, bool p10, bool p11, bool p12, bool p13, bool p14, bool p15, bool p16, int p17, int p18);
	~GJSearchObject() = mac 0x2decb0, ios 0x71e04;

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

class GJShopLayer : cocos2d::CCLayer, GJPurchaseDelegate {
	virtual void keyBackClicked() = mac 0x1a2370, ios 0xcb010;
	virtual void didPurchaseItem(GJStoreItem* p0) = mac 0x1a1ae0, ios 0xca8d8;
	void create(ShopType p0);
	void exitVideoAdItems();
	bool init(ShopType p0);
	void onBack(cocos2d::CCObject* p0);
	void onCommunityCredits(cocos2d::CCObject* p0);
	void onPlushies(cocos2d::CCObject* p0);
	void onSelectItem(cocos2d::CCObject* p0);
	void onVideoAd(cocos2d::CCObject* p0);
	void scene(ShopType p0);
	void showVideoAdReward(int p0);
	void updateCurrencyCounter();
	~GJShopLayer() = mac 0x19fe10, ios 0xc92c8;
}

class GJSongBrowser : GJDropDownLayer, FLAlertLayerProtocol {
	virtual void customSetup() = mac 0x368ab0, ios 0x19dd04;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x3690a0, ios 0x19e2a0;
	void create(LevelSettingsObject* p0);
	bool init(LevelSettingsObject* p0);
	void loadPage(int p0);
	void onDeleteAll(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void setupPageInfo(int p0, int p1, int p2);
	void setupSongBrowser(cocos2d::CCArray* p0);
	~GJSongBrowser() = mac 0x368780, ios 0x19db2c;
}

class GJSpecialColorSelect : FLAlertLayer {
	static char const* textForColorIdx(int id) = mac 0x383a50, win 0x14e1d0, ios 0x113f6c;
	virtual void keyBackClicked() = mac 0x383ca0, ios 0x114094;
	void create(int p0, GJSpecialColorSelectDelegate* p1, bool p2);
	void getButtonByTag(int p0);
	void highlightSelected(ButtonSprite* p0);
	bool init(int p0, GJSpecialColorSelectDelegate* p1, bool p2);
	void onClose(cocos2d::CCObject* p0);
	void onSelectColor(cocos2d::CCObject* p0);
	~GJSpecialColorSelect() = mac 0x3830f0, ios 0x1137ec;
}

class GJSpecialColorSelectDelegate {
	virtual void colorSelectClosed(GJSpecialColorSelect* p0, int p1) {}
}

class GJSpiderSprite : GJRobotSprite {
	~GJSpiderSprite() = mac 0x34c4a0, ios 0x162d50;
	virtual bool init() = mac 0x34c700, ios 0x162e74;
	static GJSpiderSprite* create() = mac 0x34c5b0;
}

class GJSpriteColor : cocos2d::CCNode {
	virtual bool init() = mac 0x343c90, ios 0x19d6a0;
	void create();
	void getColorMode();
	void resetCustomColorMode();
	void setCustomColorMode(int p0);
	void setDefaultColorMode(int p0);
	~GJSpriteColor() = mac 0x343d40;

	int m_colorID;
	int m_defaultColorID;
	float m_unk_0F4;
	cocos2d::ccHSVValue m_hsv;
	bool unk_108;
	float unk_10C;
	bool unk_110;
}

class GJStoreItem : cocos2d::CCNode {
	void create(int p0, int p1, int p2, int p3, ShopType p4);
	bool init(int p0, int p1, int p2, int p3, ShopType p4);
	~GJStoreItem();
}

class GJUnlockableItem : cocos2d::CCObject {
	void create();
	bool init();
	~GJUnlockableItem();
}

class GJUserCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
	virtual bool init() = mac 0x11e180, ios 0xa6374;
	virtual void draw() = mac 0x11e790, ios 0xa69ac;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x11e860, ios 0xa6a5c;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x11eae0, ios 0xa6ca8;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x11ecb0, ios 0xa6e90;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x11edb0, ios 0xa6f50;
	void loadFromScore(GJUserScore* p0);
	void onCancelFriendRequest(cocos2d::CCObject* p0);
	void onRemoveFriend(cocos2d::CCObject* p0);
	void onSendMessage(cocos2d::CCObject* p0);
	void onUnblockUser(cocos2d::CCObject* p0);
	void onViewFriendRequest(cocos2d::CCObject* p0);
	void onViewProfile(cocos2d::CCObject* p0);
	void updateBGColor(int p0);
	~GJUserCell() = mac 0x11ddc0, ios 0xa6328;
}

class GJUserMessage : cocos2d::CCNode {
	virtual bool init() = mac 0x2debf0, ios 0x71da0;
	void create(cocos2d::CCDictionary* p0);
	void create();
	~GJUserMessage() = mac 0x2deaf0, ios 0x71cd0;
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
	static GJUserScore* create(cocos2d::CCDictionary* p0) = win 0xc0750;
	virtual bool init() = mac 0x2de5a0, ios 0x71a38;
	void isCurrentUser();
	void mergeWithScore(GJUserScore* p0);
	~GJUserScore() = mac 0x2de3f0, ios 0x71914;

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

class GJWorldNode : cocos2d::CCNode {
	void addDotsToLevel(int p0, bool p1);
	void create(int p0, WorldSelectLayer* p1);
	void dotPositionForLevel(int p0, int p1);
	bool init(int p0, WorldSelectLayer* p1);
	void onLevel(cocos2d::CCObject* p0);
	void playStep1();
	void playStep2();
	void playStep3();
	void positionForLevelButton(int p0);
	void unlockActiveItem();
	~GJWorldNode() = mac 0x2016b0;
}

class GJWriteMessagePopup : FLAlertLayer, TextInputDelegate, UploadMessageDelegate, UploadPopupDelegate, FLAlertLayerProtocol {
	virtual void registerWithTouchDispatcher() = mac 0x1527b0, ios 0x20b2dc;
	virtual void keyBackClicked() = mac 0x152780, ios 0x20b2c8;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0x152830, ios 0x20b330;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x1527f0, ios 0x20b314;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x152990, ios 0x20b43c;
	virtual void uploadMessageFinished(int p0) = mac 0x152fc0, ios 0x20ba34;
	virtual void uploadMessageFailed(int p0) = mac 0x1530a0, ios 0x20bab0;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x153170, ios 0x20bb5c;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x1532b0, ios 0x20bc1c;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x153390, ios 0x20bca4;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x153450, ios 0x20bd10;
	void closeMessagePopup(bool p0);
	void create(int p0, int p1);
	bool init(int p0, int p1);
	void onClearBody(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onSend(cocos2d::CCObject* p0);
	void updateBody(gd::string p0);
	void updateCharCountLabel(int p0);
	void updateSubject(gd::string p0);
	void updateText(gd::string p0, int p1);
	~GJWriteMessagePopup() = mac 0x150d50, ios 0x209d90;
}

class GManager : cocos2d::CCNode {
	virtual void setup() {}
	void encodeDataTo(DS_Dictionary* data) {}
	void dataLoaded(DS_Dictionary* data) {}
	void firstLoad() {}

	void save() = mac 0x26f300, win 0x29250;
	void saveData(DS_Dictionary* p0, gd::string p1) = mac 0x26f4b0;
	void saveGMTo(gd::string p0) = mac 0x26f3b0;
	virtual bool init() = mac 0x26ee00, ios 0x165418;
	void getCompressedSaveString();
	void getSaveString();
	void load();
	void loadDataFromFile(gd::string p0);
	void loadFromCompressedString(gd::string p0);
	void loadFromString(gd::string p0);
	void tryLoadData(DS_Dictionary* p0, gd::string p1);
	~GManager() = mac 0x26fb90, ios 0x165f30;

	gd::string m_fileName;
	bool m_setup;
	bool m_saved;
	bool m_quickSave;
}

class GameCell : TableViewCell {
	virtual bool init() = mac 0x18f980, ios 0x12cd0c;
	virtual void draw() = mac 0x18fa40, ios 0x12cd50;
	void loadFromString(gd::string p0);
	void onTouch(cocos2d::CCObject* p0);
	void updateBGColor(int p0);
	~GameCell() = mac 0x18fb70, ios 0x12ce3c;
}

class GameEffectsManager : cocos2d::CCNode {
	void addParticleEffect(cocos2d::CCParticleSystemQuad* p0, int p1);
	void create(PlayLayer* p0);
	bool init(PlayLayer* p0);
	void scaleParticle(cocos2d::CCParticleSystemQuad* p0, float p1);
	~GameEffectsManager() = mac 0xe6a70;
}

class GameLevelManager : cocos2d::CCNode {
	cocos2d::CCArray* createAndGetScores(gd::string p0, GJScoreType p1) = win 0xa2780;
	GJGameLevel* createNewLevel() = mac 0x2b8180, win 0xa0db0;
	static GameLevelManager* sharedState() = mac 0x2a8340, win 0x9f860;
	void getCompletedLevels(bool newFilter) = win 0xa2d20;
	void getGJUserInfo(int p0) = win 0xb00b0;
	void getOnlineLevels(GJSearchObject* p0) = win 0xa7bc0;
	void getPageInfo(char const* p0) = mac 0x2c0050;
	cocos2d::CCArray* getSavedLevels(bool favorite, int levelFolder) = win 0xa2960;
	cocos2d::CCArray* getStoredOnlineLevels(char const* p0) = mac 0x2bfe80, win 0xa3a90;
	void getTopArtists(int p0, int p1) = mac 0x2ce3d0;
	void getTopArtistsKey(int p0) = mac 0x2ce7a0;
	void makeTimeStamp(char const* p0) = mac 0x2bfd90;
	GJGameLevel* getMainLevel(int id, bool unk) = win 0xa0940;
	cocos2d::CCDictionary* responseToDict(gd::string response, bool comment) = win 0xbba50;
	void storeUserNames(gd::string p0) = win 0xa1840;
	gd::string userNameForUserID(int id) = win 0xa1c20;
	void updateUserScore() = win 0xada60;
	virtual bool init() = mac 0x2b7ba0, ios 0x518b8;
	void ProcessHttpRequest(gd::string p0, gd::string p1, gd::string p2, GJHttpType p3);
	void acceptFriendRequest(int p0, int p1);
	void accountIDForUserID(int p0);
	void addDLToActive(char const* p0);
	void areGauntletsLoaded();
	void banUser(int p0);
	void blockUser(int p0);
	void cleanupDailyLevels();
	void createAndGetAccountComments(gd::string p0, int p1);
	void createAndGetCommentsFull(gd::string p0, int p1, bool p2);
	void createAndGetLevelComments(gd::string p0, int p1);
	void createAndGetLevels(gd::string p0);
	void createAndGetMapPacks(gd::string p0);
	void createPageInfo(int p0, int p1, int p2);
	void dataLoaded(DS_Dictionary* p0);
	void deleteAccountComment(int p0);
	void deleteComment(int p0, CommentType p1, int p2);
	void deleteFriendRequests(int p0, cocos2d::CCArray* p1, bool p2);
	void deleteLevel(GJGameLevel* p0);
	void deleteLevelComment(int p0, int p1);
	void deleteSentFriendRequest(int p0);
	void deleteServerLevel(int p0);
	void deleteUserMessages(GJUserMessage* p0, cocos2d::CCArray* p1, bool p2);
	void downloadLevel(int p0, bool p1);
	void downloadUserMessage(int p0, bool p1);
	void encodeDataTo(DS_Dictionary* p0);
	void firstSetup();
	void followUser(int p0);
	void friendRequestFromAccountID(int p0);
	void friendRequestWasRemoved(int p0, bool p1);
	void getAccountCommentKey(int p0, int p1);
	void getAccountComments(int p0, int p1, int p2);
	void getActiveDailyID(bool p0);
	void getAllUsedSongIDs();
	void getBasePostString(bool p0);
	void getBasePostStringFull();
	void getBoolForKey(char const* p0);
	void getCommentKey(int p0, int p1, int p2, bool p3);
	void getDailyID(bool p0);
	void getDailyTimer(bool p0);
	void getDeleteCommentKey(int p0, int p1, int p2);
	void getDeleteMessageKey(int p0, bool p1);
	void getDescKey(int p0);
	void getDiffKey(int p0);
	void getDiffVal(int p0);
	void getDifficultyStr(bool p0, bool p1, bool p2, bool p3, bool p4, bool p5, bool p6, bool p7);
	void getFolderName(int p0, bool p1);
	void getFriendRequestKey(bool p0, int p1);
	void getFriendRequests(bool p0, int p1, int p2);
	void getGJChallenges();
	void getGJDailyLevelState(bool p0);
	void getGJRewards(int p0);
	void getGauntletKey(int p0);
	void getGauntletLevels(int p0);
	void getGauntletSearchKey(int p0);
	void getGauntlets();
	void getHighestLevelOrder();
	void getIntForKey(char const* p0);
	void getLeaderboardScores(char const* p0);
	void getLenKey(int p0);
	void getLenVal(int p0);
	void getLengthStr(bool p0, bool p1, bool p2, bool p3, bool p4);
	void getLevelComments(int p0, int p1, int p2, int p3, bool p4);
	void getLevelDownloadKey(int p0, bool p1);
	void getLevelKey(int p0);
	void getLevelLeaderboard(GJGameLevel* p0, LevelLeaderboardType p1);
	void getLevelLeaderboardKey(int p0, LevelLeaderboardType p1);
	void getLevelSaveData();
	void getLikeAccountItemKey(LikeItemType p0, int p1, bool p2, int p3);
	void getLikeItemKey(LikeItemType p0, int p1, bool p2, int p3);
	void getLocalLevel(int p0);
	void getLowestLevelOrder();
	void getMapPackKey(int p0);
	void getMapPacks(GJSearchObject* p0);
	void getMessageKey(int p0);
	void getMessagesKey(bool p0, int p1);
	void getNextLevelName(gd::string p0);
	void getPostCommentKey(int p0);
	void getRateStarsKey(int p0);
	void getReportKey(int p0);
	void getSavedDailyLevel(int p0);
	void getSavedDailyLevelFromLevelID(int p0);
	void getSavedGauntlet(int p0);
	void getSavedGauntletLevel(int p0);
	void getSavedLevel(int p0);
	void getSavedLevel(GJGameLevel* p0);
	void getSavedMapPack(int p0);
	void getSearchScene(char const* p0);
	void getSplitIntFromKey(char const* p0, int p1);
	void getStoredLevelComments(char const* p0);
	void getStoredUserList(UserListType p0);
	void getStoredUserMessage(int p0);
	void getStoredUserMessageReply(int p0);
	void getTimeLeft(char const* p0, float p1);
	void getUploadMessageKey(int p0);
	void getUserInfoKey(int p0);
	void getUserList(UserListType p0);
	void getUserMessages(bool p0, int p1, int p2);
	void getUsers(GJSearchObject* p0);
	void gotoLevelPage(GJGameLevel* p0);
	void handleIt(bool p0, gd::string p1, gd::string p2, GJHttpType p3);
	void handleItDelayed(bool p0, gd::string p1, gd::string p2, GJHttpType p3);
	void handleItND(cocos2d::CCNode* p0, void* p1);
	void hasDailyStateBeenLoaded(bool p0);
	void hasDownloadedLevel(int p0);
	void hasLikedAccountItem(LikeItemType p0, int p1, bool p2, int p3);
	void hasLikedItem(LikeItemType p0, int p1, bool p2, int p3);
	void hasLikedItemFullCheck(LikeItemType p0, int p1, int p2);
	void hasRatedDemon(int p0);
	void hasRatedLevelStars(int p0);
	void hasReportedLevel(int p0);
	void invalidateMessages(bool p0, bool p1);
	void invalidateRequests(bool p0, bool p1);
	void invalidateUserList(UserListType p0, bool p1);
	void isDLActive(char const* p0);
	void isFollowingUser(int p0);
	void isTimeValid(char const* p0, float p1);
	void isUpdateValid(int p0);
	void itemIDFromLikeKey(char const* p0);
	void keyHasTimer(char const* p0);
	void levelIDFromCommentKey(char const* p0);
	void levelIDFromPostCommentKey(char const* p0);
	void likeFromLikeKey(char const* p0);
	void likeItem(LikeItemType p0, int p1, bool p2, int p3);
	void limitSavedLevels();
	void markItemAsLiked(LikeItemType p0, int p1, bool p2, int p3);
	void markLevelAsDownloaded(int p0);
	void markLevelAsRatedDemon(int p0);
	void markLevelAsRatedStars(int p0);
	void markLevelAsReported(int p0);
	void messageWasRemoved(int p0, bool p1);
	void onAcceptFriendRequestCompleted(gd::string p0, gd::string p1);
	void onBanUserCompleted(gd::string p0, gd::string p1);
	void onBlockUserCompleted(gd::string p0, gd::string p1);
	void onDeleteCommentCompleted(gd::string p0, gd::string p1);
	void onDeleteFriendRequestCompleted(gd::string p0, gd::string p1);
	void onDeleteServerLevelCompleted(gd::string p0, gd::string p1);
	void onDeleteUserMessagesCompleted(gd::string p0, gd::string p1);
	void onDownloadLevelCompleted(gd::string p0, gd::string p1);
	void onDownloadUserMessageCompleted(gd::string p0, gd::string p1);
	void onGetAccountCommentsCompleted(gd::string p0, gd::string p1);
	void onGetFriendRequestsCompleted(gd::string p0, gd::string p1);
	void onGetGJChallengesCompleted(gd::string p0, gd::string p1);
	void onGetGJRewardsCompleted(gd::string p0, gd::string p1);
	void onGetGJUserInfoCompleted(gd::string p0, gd::string p1);
	void onGetGauntletsCompleted(gd::string p0, gd::string p1);
	void onGetLeaderboardScoresCompleted(gd::string p0, gd::string p1);
	void onGetLevelCommentsCompleted(gd::string p0, gd::string p1);
	void onGetLevelLeaderboardCompleted(gd::string p0, gd::string p1);
	void onGetLevelSaveDataCompleted(gd::string p0, gd::string p1);
	void onGetMapPacksCompleted(gd::string p0, gd::string p1);
	void onGetOnlineLevelsCompleted(gd::string p0, gd::string p1);
	void onGetTopArtistsCompleted(gd::string p0, gd::string p1);
	void onGetUserListCompleted(gd::string p0, gd::string p1);
	void onGetUserMessagesCompleted(gd::string p0, gd::string p1);
	void onGetUsersCompleted(gd::string p0, gd::string p1);
	void onLikeItemCompleted(gd::string p0, gd::string p1);
	void onProcessHttpRequestCompleted(cocos2d::extension::CCHttpClient* p0, cocos2d::extension::CCHttpResponse* p1);
	void onRateDemonCompleted(gd::string p0, gd::string p1);
	void onRateStarsCompleted(gd::string p0, gd::string p1);
	void onReadFriendRequestCompleted(gd::string p0, gd::string p1);
	void onRemoveFriendCompleted(gd::string p0, gd::string p1);
	void onReportLevelCompleted(gd::string p0, gd::string p1);
	void onRequestUserAccessCompleted(gd::string p0, gd::string p1);
	void onRestoreItemsCompleted(gd::string p0, gd::string p1);
	void onSetLevelFeaturedCompleted(gd::string p0, gd::string p1);
	void onSetLevelStarsCompleted(gd::string p0, gd::string p1);
	void onSubmitUserInfoCompleted(gd::string p0, gd::string p1);
	void onSuggestLevelStarsCompleted(gd::string p0, gd::string p1);
	void onUnblockUserCompleted(gd::string p0, gd::string p1);
	void onUpdateDescriptionCompleted(gd::string p0, gd::string p1);
	void onUpdateLevelCompleted(gd::string p0, gd::string p1);
	void onUpdateUserScoreCompleted(gd::string p0, gd::string p1);
	void onUploadCommentCompleted(gd::string p0, gd::string p1);
	void onUploadFriendRequestCompleted(gd::string p0, gd::string p1);
	void onUploadLevelCompleted(gd::string p0, gd::string p1);
	void onUploadUserMessageCompleted(gd::string p0, gd::string p1);
	void ongetGJDailyLevelStateCompleted(gd::string p0, gd::string p1);
	void pageFromCommentKey(char const* p0);
	void parseRestoreData(gd::string p0);
	void processOnDownloadLevelCompleted(gd::string p0, gd::string p1, bool p2);
	void purgeUnusedLevels();
	void rateDemon(int p0, int p1, bool p2);
	void rateStars(int p0, int p1);
	void readFriendRequest(int p0);
	void removeDLFromActive(char const* p0);
	void removeDelimiterChars(gd::string p0, bool p1);
	void removeFriend(int p0);
	void removeLevelDownloadedKeysFromDict(cocos2d::CCDictionary* p0);
	void removeUserFromList(int p0, UserListType p1);
	void reportLevel(int p0);
	void requestUserAccess();
	void resetAccountComments(int p0);
	void resetAllTimers();
	void resetCommentTimersForLevelID(int p0, bool p1);
	void resetDailyLevelState(bool p0);
	void resetGauntlets();
	void resetStoredUserInfo(int p0);
	void resetStoredUserList(UserListType p0);
	void resetTimerForKey(char const* p0);
	void restoreItems();
	void saveFetchedLevels(cocos2d::CCArray* p0);
	void saveFetchedMapPacks(cocos2d::CCArray* p0);
	void saveGauntlet(GJMapPack* p0);
	void saveLevel(GJGameLevel* p0);
	void saveMapPack(GJMapPack* p0);
	void setBoolForKey(bool p0, char const* p1);
	void setDiffVal(int p0, bool p1);
	void setDownloadedLevelsDict(cocos2d::CCDictionary* p0);
	void setFolderName(int p0, gd::string p1, bool p2);
	void setFolderNamesCreatedDict(cocos2d::CCDictionary* p0);
	void setFolderNamesDict(cocos2d::CCDictionary* p0);
	void setFollowedUsersDict(cocos2d::CCDictionary* p0);
	void setIntForKey(int p0, char const* p1);
	void setLenVal(int p0, bool p1);
	void setLevelFeatured(int p0, int p1, bool p2);
	void setLevelStars(int p0, int p1, bool p2);
	void setLikedItemsDict(cocos2d::CCDictionary* p0);
	void setMainLevels(cocos2d::CCDictionary* p0);
	void setRatedDemonsDict(cocos2d::CCDictionary* p0);
	void setRatedLevelsDict(cocos2d::CCDictionary* p0);
	void setReportedLevelsDict(cocos2d::CCDictionary* p0);
	void setSavedDailyLevelsDict(cocos2d::CCDictionary* p0);
	void setSavedGauntletLevelsDict(cocos2d::CCDictionary* p0);
	void setSavedLevelsDict(cocos2d::CCDictionary* p0);
	void setSettingsDict(cocos2d::CCDictionary* p0);
	void specialFromLikeKey(char const* p0);
	void storeCommentsResult(cocos2d::CCArray* p0, gd::string p1, char const* p2);
	void storeDailyLevelState(int p0, int p1, bool p2);
	void storeFriendRequest(GJFriendRequest* p0);
	void storeSearchResult(cocos2d::CCArray* p0, gd::string p1, char const* p2);
	void storeUserInfo(GJUserScore* p0);
	void storeUserMessage(GJUserMessage* p0);
	void storeUserMessageReply(int p0, GJUserMessage* p1);
	void storeUserName(int p0, int p1, gd::string p2);
	void submitUserInfo();
	void suggestLevelStars(int p0, int p1, bool p2);
	void typeFromCommentKey(char const* p0);
	void typeFromLikeKey(char const* p0);
	void unblockUser(int p0);
	void unfollowUser(int p0);
	void updateDescription(int p0, gd::string p1);
	void updateLevel(GJGameLevel* p0);
	void updateLevelOrders();
	void updateLevelRewards(GJGameLevel* p0);
	void updateUsernames();
	void uploadAccountComment(gd::string p0);
	void uploadComment(gd::string p0, CommentType p1, int p2, int p3);
	void uploadFriendRequest(int p0, gd::string p1);
	void uploadLevel(GJGameLevel* p0);
	void uploadLevelComment(int p0, gd::string p1, int p2);
	void uploadUserMessage(int p0, gd::string p1, gd::string p2);
	void userIDForAccountID(int p0);
	void userInfoForAccountID(int p0);
	void verifyLevelState(GJGameLevel* p0);
	~GameLevelManager() = mac 0x2b7b60, ios 0x5187c;

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
	cocos2d::ccColor3B colorForIdx(int p0) = mac 0x1cbc80, win 0xc8d10, ios 0x237488;
	void didExitPlayscene() = mac 0x1d0230;
	void doQuickSave() = mac 0x1d0200;
	void fadeInMusic(char const* p0) = mac 0x1c2ff0, win 0xc4bd0;
	void getBGTexture(int p0) = mac 0x1cca00;
	void getFontFile(int p0) = mac 0x1cc5f0;
	bool getGameVariable(char const* p0) = mac 0x1cccd0, win 0xc9d30;
	int getIntGameVariable(char const* p0) = mac 0x1cd1d0, win 0xca330;
	bool getUGV(char const* p0) = mac 0x1ccfa0, win 0xca0d0;
	void loadDeathEffect(int p0) = mac 0x1cc690;
	void loadFont(int p0) = mac 0x1cc550;
	void reloadAll(bool p0, bool p1, bool p2) = mac 0x1d08a0, win 0xce950;
	void reloadAllStep2() = mac 0x1d0940, win 0xce9e0, ios 0x23b1f4;
	void reloadAllStep5() = mac 0x1d0b00;
	void reportPercentageForLevel(int p0, int p1, bool p2) = mac 0x1c5b00;
	void setGameVariable(char const* p0, bool p1) = mac 0x1cca80, win 0xc9b50;
	void setIntGameVariable(char const* p0, int p1) = mac 0x1cd0e0, win 0xca230;
	void setUGV(char const* p0, bool p1) = mac 0x1cce50;
	static GameManager* sharedState() = mac 0x1c2b30, win 0xc4a50, ios 0x2321b8;
	~GameManager() = mac 0x1d0e00, ios 0x23b79c;
	void getGTexture(int p0) = mac 0x1cca40;
	virtual bool init() = mac 0x1c2ec0, ios 0x232218;
	void reportAchievementWithID(char const* p0, int p1, bool p2) = mac 0x1c6460;
	cocos2d::CCSize* resolutionForKey(cocos2d::CCSize* p0, int p1) = mac 0x1d0b40, win 0xceca0;
	void update(float p0) = mac 0x1d0270, ios 0x23ad30;
	bool isColorUnlocked(int _id, bool _type) = mac 0x1c3b90, win 0xc53f0;
	bool isIconUnlocked(int _id, IconType _type) = mac 0x1c35b0, win 0xc4fc0;
	void toggleGameVariable(char const* key) = win 0xc9e90;
	static void returnToLastScene(GJGameLevel* level) = win 0xce6a0;
	virtual void encodeDataTo(DS_Dictionary* p0) = mac 0x1cfd90, ios 0x23a88c;
	virtual void dataLoaded(DS_Dictionary* p0) = mac 0x1ce7f0, ios 0x2396d8;
	virtual void firstLoad() = mac 0x1cf6a0, ios 0x23a2c4;
	void addNewCustomObject(gd::string p0);
	void applicationDidEnterBackground();
	void applicationWillEnterForeground();
	void canShowRewardedVideoAd();
	void checkSteamAchievementUnlock();
	void checkUsedIcons();
	void claimItemsResponse(gd::string p0);
	void colorForPos(int p0);
	void colorKey(int p0, bool p1);
	void completedAchievement(char const* p0);
	void eventUnlockFeature(char const* p0);
	void followTwitter();
	void generateSecretNumber();
	void getFontTexture(int p0);
	void getNextUniqueObjectKey();
	void getNextUsedKey(int p0, bool p1);
	void getOrderedCustomObjectKeys();
	void getPlayerUDID();
	void hasRatingPower();
	void hasShownAdRecently();
	void iconKey(int p0, IconType p1);
	void iconTypeToUnlockType(IconType p0);
	void itemPurchased(char const* p0);
	void levelIsPremium(int p0, int p1);
	void likeFacebook();
	void loadBackground(int p0);
	void loadGround(int p0);
	void loadVideoSettings();
	void lockColor(int p0, bool p1);
	void lockIcon(int p0, IconType p1);
	void openEditorGuide();
	void rateGame();
	void recountUserStats(gd::string p0);
	void reloadAllStep3();
	void reloadAllStep4();
	void removeCustomObject(int p0);
	void reorderKey(int p0, bool p1);
	void resetAchievement(char const* p0);
	void resetAllIcons();
	void resetCoinUnlocks();
	void resolutionForKey(int p0);
	void rewardedVideoAdFinished(int p0);
	void rewardedVideoHidden();
	void rewardedVideoHiddenDelayed();
	void safePopScene();
	void setCustomObjectDict(cocos2d::CCDictionary* p0);
	void setHasRatingPower(int p0);
	void setPlayerUserID(int p0);
	void setUnlockValueKeeper(cocos2d::CCDictionary* p0);
	void setValueKeeper(cocos2d::CCDictionary* p0);
	void shortenAdTimer(float p0);
	void shouldShowInterstitial();
	void showInterstitial();
	void showInterstitialForced();
	void showMainMenuAd();
	void startUpdate();
	void stringForCustomObject(int p0);
	void subYouTube();
	void switchCustomObjects(int p0, int p1);
	void switchScreenMode(bool p0, bool p1);
	void syncPlatformAchievements();
	void tryCacheAd();
	void tryResetAdTimer();
	void unloadBackground();
	void unlockColor(int p0, bool p1);
	void unlockIcon(int p0, IconType p1);
	void unlockTypeToIconType(int p0);
	void unlockedPremium();
	void updateMusic();
	void verifyAchievementUnlocks();
	void verifyCoinUnlocks();
	void verifyStarUnlocks();
	void verifySyncedCoins();
	void videoAdHidden();
	void videoAdShowed();

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
	PAD = mac 0x4, win 0x4;
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
	int m_ratePowerSeed;
	int m_ratePowerRand;
	int m_ratePower;
	bool m_canGetLevelSaveData;
	int m_resolution;
	cocos2d::TextureQuality m_quality;
}

class GameObject : CCSpritePlus {
	// it is a short array with size 10 but pointer to arrays are weird
	inline using GroupArrayType = short*; 

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

	~GameObject() = mac 0x2f4ca0, ios 0x16e52c;
	virtual void update(float p0) = mac 0x2fbb90, ios 0x171f20;
	virtual void setScaleX(float p0) = mac 0x335b00, win 0xe5050, ios 0x194d78;
	virtual void setScaleY(float p0) = mac 0x335b90, win 0xe50e0, ios 0x194e10;
	virtual void setScale(float p0) = mac 0x335c20, win 0xe5170, ios 0x194ea8;
	virtual void setPosition(cocos2d::CCPoint const& p0) = mac 0x335850, win 0xe4de0, ios 0x194abc;
	virtual void setVisible(bool p0) = mac 0x336280, win 0xe57c0, ios 0x195354;
	virtual void setRotation(float p0) = mac 0x335970, win 0xe4ed0, ios 0x194bc4;
	virtual void setOpacity(unsigned char p0) = mac 0x335f10, win 0xe53c0, ios 0x1950a0;
	virtual bool initWithTexture(cocos2d::CCTexture2D* p0) = mac 0x2f56a0, win 0xcfa80, ios 0x16ea20;
	virtual void setChildColor(cocos2d::ccColor3B const& p0) = mac 0x341f20, win 0xee900, ios 0x19c354;
	virtual void setFlipX(bool p0) = mac 0x335a60, ios 0x194ca8;
	virtual void setFlipY(bool p0) = mac 0x335ab0, ios 0x194d10;
	virtual void customSetup() = mac 0x2fbba0, ios 0x171f24;
	virtual void setupCustomSprites() = mac 0x307f60, win 0xd7d50, ios 0x179358;
	virtual void addMainSpriteToParent(bool p0) = mac 0x33a5b0, win 0xeb250, ios 0x198cf8;
	virtual void resetObject() = mac 0x2fa620, win 0xd1470, ios 0x170f64;
	virtual void triggerObject(GJBaseGameLayer* p0) = mac 0x2fa8f0, ios 0x171158;
	virtual void activateObject() = mac 0x2faf60, win 0xEF0E0, ios 0x17178c;
	virtual void deactivateObject(bool p0) = mac 0x2fb8f0, win 0xd19b0, ios 0x171cb8;
	virtual cocos2d::CCRect* getObjectRect() = mac 0x3352b0, win 0xe4a40, ios 0x194644;
	virtual cocos2d::CCRect* getObjectRect(float p0, float p1) = mac 0x3352d0, win 0xe4a70, ios 0x194668;
	virtual cocos2d::CCRect* getObjectRect2(float p0, float p1) = mac 0x3354e0, win 0xe4b90, ios 0x194818;
	virtual cocos2d::CCRect* getObjectTextureRect() = mac 0x3355b0, win 0xe4c40, ios 0x1948c8;
	virtual void getRealPosition() = mac 0x335750, win 0xe4d90, ios 0x194a2c;
	virtual void setStartPos(cocos2d::CCPoint p0) = mac 0x2fa520, win 0xd1390, ios 0x170e70;
	virtual void updateStartValues() = mac 0x2fa800, win 0xd1610, ios 0x1710dc;
	virtual void customObjectSetup() = mac 0xdc1a0;
	virtual gd::string getSaveString() = mac 0x33d3d0, win 0xed0c0, ios 0x19b07c;
	virtual bool isFlipX() = mac 0x335a40, win 0xe4fb0, ios 0x194c98;
	virtual bool isFlipY() = mac 0x335a50, win 0xe4fc0, ios 0x194ca0;
	virtual void setRScaleX(float p0) = mac 0x335cb0, win 0xe5200, ios 0x194f40;
	virtual void setRScaleY(float p0) = mac 0x335d60, win 0xe5240, ios 0x194f64;
	virtual void setRScale(float p0) = mac 0x335e10, win 0xe5280, ios 0x194f88;
	virtual void getRScaleX() = mac 0x335e50, ios 0x194fcc;
	virtual void getRScaleY() = mac 0x335e80, win 0xe5300, ios 0x19500c;
	virtual void calculateSpawnXPos() = mac 0x336970, win 0xe5d20, ios 0x195914;
	virtual void triggerActivated(float p0) = mac 0x336990, ios 0x195920;
	virtual void powerOnObject() = mac 0x3369a0, win 0xe5d40, ios 0x19592c;
	virtual void powerOffObject() = mac 0x3369c0, win 0xe5d60, ios 0x195944;
	virtual void setObjectColor(cocos2d::ccColor3B const& p0) = mac 0x341c90, win 0xee620, ios 0x19c150;
	virtual void setGlowColor(cocos2d::ccColor3B const& p0) = mac 0x341ed0, win 0xee8c0, ios 0x19c2f8;
	virtual void getOrientedBox() = mac 0x342ad0, win 0xef160, ios 0x19c9d8;
	virtual void addToGroup(int p0) = mac 0x33ad00, win 0xeb8d0, ios 0x199130;
	virtual void removeFromGroup(int p0) = mac 0x33ada0, win 0xeb930, ios 0x1991c0;
	virtual void spawnXPosition() = mac 0xdc1b0, ios 0x152788;
	virtual void getObjectRectDirty() const = mac 0xdc1d0, ios 0x1527a8;
	virtual void setObjectRectDirty(bool p0) = mac 0xdc1e0, ios 0x1527b0;
	virtual void getOrientedRectDirty() const = mac 0xdc1f0, ios 0x1527b8;
	virtual void setOrientedRectDirty(bool p0) = mac 0xdc200, ios 0x1527c0;
	virtual GameObjectType getType() const = mac 0xdc210, win 0x989e0, ios 0x1527c8;
	virtual void setType(GameObjectType p0) = mac 0xdc220, win 0x989f0, ios 0x1527d0;
	virtual void getStartPos() const = mac 0xdc230, win 0x98a00, ios 0x1527d8;
	void activatedByPlayer(GameObject* p0) = mac 0x342a20;
	void addColorSprite() = mac 0x2f7fe0;
	void addColorSpriteToParent(bool p0) = mac 0x2fb470;
	void addToTempOffset(float p0, float p1) = mac 0x335700;
	void calculateOrientedBox() = mac 0x342b20;
	void canChangeCustomColor() = mac 0x342db0;
	void colorForMode(int p0, bool p1) = mac 0x343460;
	void commonSetup() = mac 0x2f5570, win 0xcfac0;
	void copyGroups(GameObject* p0) = mac 0x33ae30;
	static GameObject* createWithFrame(char const* p0) = mac 0x2f5490, win 0xcf8f0;
	static GameObject* createWithKey(int p0) = mac 0x2f4ce0, win 0xcf4f0;
	void destroyObject() = mac 0x336a00;
	void determineSlopeDirection() = mac 0x33a9e0;
	void getActiveColorForMode(int p0, bool p1) = mac 0x343860;
	void getBallFrame(int p0) = mac 0x341bf0;
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
	void hasBeenActivatedByPlayer(GameObject* p0) = mac 0x342a50;
	void hasSecondaryColor() = mac 0x342f80;
	void ignoreEnter() = mac 0x3352a0;
	void ignoreFade() = mac 0x335290;
	void isBasicTrigger() = mac 0x343d10;
	void isColorTrigger() = mac 0x343b40;
	void isSpawnableTrigger() = mac 0x343a60;
	void isSpecialObject() = mac 0x343c40;
	static GameObject* objectFromString(gd::string p0, bool p1) = mac 0x33b720, win 0xebe50;
	void playShineEffect() = mac 0x2fa9d0;
	void quickUpdatePosition() = mac 0x335790;
	void removeGlow() = mac 0x2f7f70;
	void resetGroupDisabled() = mac 0x2fa7e0;
	void saveActiveColors() = mac 0x33d250;
	void selectObject(const cocos2d::ccColor3B& p0) = mac 0x341f90, win 0xee960;
	void setDefaultMainColorMode(int p0) = mac 0x304fc0;
	void setDidUpdateLastPosition(bool const& p0) = mac 0x343a30;
	void setGlowOpacity(unsigned char p0) = mac 0x336200;
	void setLastPosition(cocos2d::CCPoint const& p0) = mac 0x3439e0;
	void setMainColorMode(int p0) = mac 0x342e70;
	void setSectionIdx(int const& p0) = mac 0x343a10;
	void setupCoinArt() = mac 0x337dd0;
	void slopeFloorTop() = mac 0x342800;
	void slopeWallLeft() = mac 0x3427e0;
	void updateCustomScale(float p0) = mac 0x335eb0, win 0xe5340;
	void updateMainColor() = mac 0x343340;
	void updateOrientedBox() = mac 0x342b50;
	void updateSecondaryColor() = mac 0x343740;
	void updateStartPos() = mac 0x2fa590;
	void updateState() = mac 0x3369e0;
	void updateSyncedAnimation(float p0) = mac 0x337f00;
	void deselectObject() = win 0xeee50;
	cocos2d::CCRepeatForever* createRotateAction(float f, int n) = win 0xe49b0;
	void setMyAction(cocos2d::CCAction* pAction) = win 0xd1b90;
	virtual void customObjectSetup(gd::map<gd::string, gd::string>& p0) = mac 0xdc1a0;
	void addCustomBlackChild(gd::string p0, float p1, bool p2);
	void addCustomChild(gd::string p0, cocos2d::CCPoint p1, int p2);
	void addCustomColorChild(gd::string p0);
	void addEmptyGlow();
	void addGlow();
	void addInternalChild(cocos2d::CCSprite* p0, gd::string p1, cocos2d::CCPoint p2, int p3);
	void addInternalCustomColorChild(gd::string p0, cocos2d::CCPoint p1, int p2);
	void addInternalGlowChild(gd::string p0, cocos2d::CCPoint p1);
	void addNewSlope01(bool p0);
	void addNewSlope01Glow(bool p0);
	void addNewSlope02(bool p0);
	void addNewSlope02Glow(bool p0);
	void addToColorGroup(int p0);
	void addToOpacityGroup(int p0);
	void belongsToGroup(int p0);
	void canAllowMultiActivate();
	void canChangeMainColor();
	void canChangeSecondaryColor();
	void canRotateFree();
	void createAndAddParticle(int p0, char const* p1, int p2, cocos2d::tCCPositionType p3);
	void createColorGroupContainer(int p0);
	void createGlow(gd::string p0);
	void createGroupContainer(int p0);
	void createOpacityGroupContainer(int p0);
	void createSpriteColor(int p0);
	void createWithTexture(cocos2d::CCTexture2D* p0);
	void customScaleMod(char const* p0);
	void disableObject();
	void dontCountTowardsLimit();
	void duplicateAttributes(GameObject* p0);
	void duplicateColorMode(GameObject* p0);
	void duplicateValues(GameObject* p0);
	void editorColorForCustomMode(int p0);
	void editorColorForMode(int p0);
	void getColorFrame(gd::string p0);
	void getGlowFrame(gd::string p0);
	void getGroupDisabled();
	void getGroupString();
	void getMainColor();
	void getObjectZLayer();
	void getOuterObjectRect();
	void getParentMode();
	void getRelativeSpriteColor(int p0);
	void getSecondaryColor();
	void getUpdateLastPos();
	void groupColor(cocos2d::ccColor3B const& p0, bool p1);
	void groupOpacityMod();
	void ignoreEditorDuration();
	bool init(char const* p0);
	void isColorObject();
	void isEditorSpawnableTrigger();
	void isFacingDown();
	void isStoppableTrigger();
	void isTrigger();
	void loadGroupsFromString(gd::string p0);
	void opacityModForMode(int p0, bool p1);
	void parentForZLayer(int p0, bool p1, int p2);
	void perspectiveColorFrame(char const* p0, int p1);
	void perspectiveFrame(char const* p0, int p1);
	void quickUpdatePosition2();
	void removeColorSprite();
	void reorderColorSprite();
	void resetMainColorMode();
	void resetSecondaryColorMode();
	void selectObject(cocos2d::ccColor3B p0);
	void setDefaultSecondaryColorMode(int p0);
	void setSecondaryColorMode(int p0);
	void setTempOffset(float p0, float p1);
	void setUpdateLastPos(bool const& p0);
	void setupCoinAnimation();
	void setupColorSprite(int p0, bool p1);
	void setupSpriteSize();
	void shouldBlendColor(GJSpriteColor* p0, bool p1);
	void shouldNotSave();
	void slopeYPos(float p0);
	void slopeYPos(cocos2d::CCRect p0);
	void slopeYPos(GameObject* p0);
	void updateMainColor(cocos2d::ccColor3B const& p0);
	void updateObjectEditorColor();
	void updateParticleColor(cocos2d::ccColor3B const& p0);
	void updateSecondaryColor(cocos2d::ccColor3B const& p0);
	void updateTextObject(gd::string p0, bool p1);
	void waitingForActivation();

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
	bool m_colorOnTop;
	//GJSpriteColor* m_mainColorMode;
	//GJSpriteColor* m_secondaryColorMode;
	//bool m_col1;
	//bool m_col2;
	int m_baseColorID;
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
	bool m_isPortal;
	bool m_lockColorAsChild;
	bool m_customAudioScale;
	int m_minAudioScale;
	int m_maxAudioScale;
	bool m_unkParticleSystem2;
	int m_secretCoinID;
	int m_unkUnusedSaveStringKey53;
	bool m_invisibleMode;
	bool m_glowUserBackgroundColor;
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
	GroupArrayType m_groups;
	short m_groupCount;
	GroupArrayType m_pulseGroups;
	short m_pulseGroupCount;
	GroupArrayType m_alphaGroups;
	short m_alphaGroupCount;
	int m_editorLayer;
	int m_editorLayer2;
	int m_unk414;
	PAD = mac 0xc, win 0xc;
	cocos2d::CCPoint m_firstPosition;
	PAD = mac 0x1c, win 0x1c;
	bool m_highDetail;
	ColorActionSprite* m_colorActionSpriteBase;
	ColorActionSprite* m_colorActionSpriteDetail;
	GJEffectManager* m_effectManager;
	PAD = mac 0x10, win 0x10;
}

class GameObjectCopy : cocos2d::CCObject {
	~GameObjectCopy() = mac 0xa3290, ios 0x26d34c;
	static GameObjectCopy* create(GameObject* p0) = mac 0x975a0;
	void resetObject() = mac 0x976a0;
	bool init(GameObject* p0);
}

class GameRateDelegate {
	virtual void updateRate() {}
}

class GameSoundManager : cocos2d::CCNode {
	void disableMetering() = mac 0x362d80;
	void enableMetering() = mac 0x362d00;
	void getMeteringValue() = mac 0x362db0;
	void playBackgroundMusic(gd::string p0, bool p1, bool p2) = mac 0x362070;
	void playEffect(gd::string p0, float p1, float p2, float p3) = mac 0x3623d0, win 0x25450;
	void stopBackgroundMusic() = mac 0x362130;
	~GameSoundManager() = mac 0x362c00, ios 0x1a353c;
	static GameSoundManager* sharedManager() = mac 0x3610f0, win 0x24800;
	virtual bool init() = mac 0x3611d0, ios 0x1a2a60;
	void asynchronousSetup();
	void fadeInMusic(bool p0);
	void fadeOutMusic(bool p0);
	void isLoopedSoundPlaying(gd::string p0);
	void pauseAllLoopedSounds();
	void pauseLoopedSound(gd::string p0);
	void playLoopedSound(gd::string p0, gd::string p1, float p2, float p3, bool p4, bool p5, bool p6);
	void playUniqueEffect(gd::string p0);
	void playUniqueEffect(gd::string p0, float p1, float p2, float p3);
	void preload();
	void removeLoopedSound(gd::string p0);
	void resetUniqueEffects();
	void resumeAllLoopedSounds();
	void resumeSound();
	void setup();
	void stopAllLoopedSounds();
	void stopLoopedSound(gd::string p0, bool p1);
	void updateLoopedVolume(gd::string p0, float p1);
	void updateMetering(float p0);

	cocos2d::CCDictionary* m_dictionary1;
	cocos2d::CCDictionary* m_dictionary2;
	PAD = win 0xc;
	bool m_preloaded;
	PAD = win 0x4;
	gd::string m_filePath;
}

class GameStatsManager : cocos2d::CCNode {
	void awardCurrencyForLevel(GJGameLevel* p0) = mac 0x43600;
	void awardDiamondsForLevel(GJGameLevel* p0) = mac 0x43c60;
	void awardSecretKey() = mac 0x4b1e0;
	int getAwardedCurrencyForLevel(GJGameLevel* p0) = win 0xf83e0;
	int getBaseCurrencyForLevel(GJGameLevel* p0) = win 0xf8530;
	char const* getCoinKey(int p0) = win 0xbda50;
	GJChallengeItem* getChallenge(int id) = win 0xa2fb0;
	void getSecretCoinKey(char const* p0) = mac 0x429f0;
	void getStat(char const* p0) = mac 0x3d310;
	void hasPendingUserCoin(char const* p0) = mac 0x42730;
	void hasSecretCoin(char const* p0) = mac 0x40730;
	void hasUserCoin(char const* p0) = mac 0x427e0;
	void incrementChallenge(GJChallengeType type, int count) = win 0xf9ae0;
	void incrementStat(char const* p0) = mac 0x3d6d0;
	void incrementStat(char const* p0, int p1) = mac 0x3d6e0;
	static GameStatsManager* sharedState() = mac 0x38f20, win 0xf1e50;
	void storePendingUserCoin(char const* p0) = mac 0x42940;
	void storeSecretCoin(char const* p0) = mac 0x42a10;
	void storeUserCoin(char const* p0) = mac 0x42890;
	virtual bool init() = mac 0x39330, ios 0x2433d0;
	void addStoreItem(int p0, int p1, int p2, int p3, ShopType p4);
	void areChallengesLoaded();
	void areRewardsLoaded();
	void checkAchievement(char const* p0);
	void checkCoinAchievement(GJGameLevel* p0);
	void checkCoinsForLevel(GJGameLevel* p0);
	void collectReward(GJRewardType p0, GJRewardItem* p1);
	void collectVideoReward(int p0);
	void completedChallenge(GJChallengeItem* p0);
	void completedDailyLevel(GJGameLevel* p0);
	void completedDemonLevel(GJGameLevel* p0);
	void completedLevel(GJGameLevel* p0);
	void completedMapPack(GJMapPack* p0);
	void completedStarLevel(GJGameLevel* p0);
	void createSecretChestItems();
	void createSecretChestRewards();
	void createSpecialChestItems();
	void createStoreItems();
	void dataLoaded(DS_Dictionary* p0);
	void encodeDataTo(DS_Dictionary* p0);
	void firstSetup();
	void generateItemUnlockableData();
	void getAwardedDiamondsForLevel(GJGameLevel* p0);
	void getBaseCurrency(int p0, bool p1, int p2);
	void getBaseDiamonds(int p0);
	void getBonusDiamonds(int p0);
	void getBonusShardCount();
	void getChallengeKey(GJChallengeItem* p0);
	void getCollectedCoinsForLevel(GJGameLevel* p0);
	void getCompletedMapPacks();
	void getCurrencyKey(GJGameLevel* p0);
	void getDailyLevelKey(int p0);
	void getDemonLevelKey(GJGameLevel* p0);
	void getGauntletRewardKey(int p0);
	void getItemKey(int p0, UnlockType p1);
	void getItemUnlockState(int p0, UnlockType p1);
	void getLevelKey(int p0, bool p1, bool p2, bool p3);
	void getLevelKey(GJGameLevel* p0);
	void getLiteAchievements();
	void getMapPackKey(int p0);
	void getQueuedChallenge(int p0);
	void getRewardForSecretChest(int p0);
	void getRewardForSpecialChest(gd::string p0);
	void getRewardItem(GJRewardType p0);
	void getRewardKey(GJRewardType p0, int p1);
	void getSecondaryQueuedChallenge(int p0);
	void getSecretChestForItem(int p0, UnlockType p1);
	void getSpecialChestKeyForItem(int p0, UnlockType p1);
	void getStarLevelKey(GJGameLevel* p0);
	void getStoreItem(int p0);
	void getStoreItem(int p0, int p1);
	void getStoreItemKey(int p0, int p1);
	void getTotalCollectedCurrency();
	void getTotalCollectedDiamonds();
	void hasCompletedChallenge(GJChallengeItem* p0);
	void hasCompletedDailyLevel(int p0);
	void hasCompletedDemonLevel(GJGameLevel* p0);
	void hasCompletedGauntletLevel(int p0);
	void hasCompletedLevel(GJGameLevel* p0);
	void hasCompletedMainLevel(int p0);
	void hasCompletedMapPack(int p0);
	void hasCompletedOnlineLevel(int p0);
	void hasCompletedStarLevel(GJGameLevel* p0);
	void hasRewardBeenCollected(GJRewardType p0, int p1);
	void isItemUnlocked(UnlockType p0, int p1);
	void isLiteUnlockable(gd::string p0);
	void isSecretChestUnlocked(int p0);
	void isSecretCoin(gd::string p0);
	void isSecretCoinValid(gd::string p0);
	void isSpecialChestUnlocked(gd::string p0);
	void isStoreItemUnlocked(int p0);
	void logCoins();
	void markLevelAsCompletedAndClaimed(GJGameLevel* p0);
	void postLoadGameStats();
	void preProcessReward(GJRewardItem* p0);
	void preSaveGameStats();
	void processChallengeQueue(int p0);
	void purchaseItem(int p0);
	void recountSpecialStats();
	void recountUserCoins(bool p0);
	void registerRewardsFromItem(GJRewardItem* p0);
	void removeChallenge(int p0);
	void removeErrorFromSpecialChests();
	void removeQueuedChallenge(int p0);
	void removeQueuedSecondaryChallenge(int p0);
	void resetChallengeTimer();
	void resetPreSync();
	void resetSpecialStatAchievements();
	void resetUserCoins();
	void restorePostSync();
	void setAwardedBonusKeys(int p0);
	void setChallengeItemDict(cocos2d::CCDictionary* p0);
	void setChallengeItemQueuedDict(cocos2d::CCDictionary* p0);
	void setCollectedRewards(cocos2d::CCDictionary* p0);
	void setCollectedVideoRewards(cocos2d::CCDictionary* p0);
	void setCollectedWeeklyLevelRewards(cocos2d::CCDictionary* p0);
	void setCompletedLevels(cocos2d::CCDictionary* p0);
	void setCurrencyAwardDict(cocos2d::CCDictionary* p0);
	void setCurrencyDailyLevelDict(cocos2d::CCDictionary* p0);
	void setCurrencyDailyLevelStarsDict(cocos2d::CCDictionary* p0);
	void setCurrencyGauntletLevelDict(cocos2d::CCDictionary* p0);
	void setCurrencyLevelDict(cocos2d::CCDictionary* p0);
	void setCurrencyLevelStarsDict(cocos2d::CCDictionary* p0);
	void setCurrencyMainLevelDict(cocos2d::CCDictionary* p0);
	void setCurrencyUnlockDict(cocos2d::CCDictionary* p0);
	void setDiamondAwardDict(cocos2d::CCDictionary* p0);
	void setDiamondsDailyLevelDict(cocos2d::CCDictionary* p0);
	void setDiamondsLevelDict(cocos2d::CCDictionary* p0);
	void setPendingUserCoinsDict(cocos2d::CCDictionary* p0);
	void setRandomContainer(cocos2d::CCDictionary* p0);
	void setRewardItemDict(cocos2d::CCDictionary* p0);
	void setStarsForMapPack(int p0, int p1);
	void setStat(char const* p0, int p1);
	void setStatIfHigher(char const* p0, int p1);
	void setUnlockedSecretChests(cocos2d::CCDictionary* p0);
	void setUnlockedSpecialChests(cocos2d::CCDictionary* p0);
	void setUserCoinsDict(cocos2d::CCDictionary* p0);
	void setValueDict(cocos2d::CCDictionary* p0);
	void shouldAwardSecretKey();
	void starsForMapPack(int p0);
	void storeChallenge(int p0, GJChallengeItem* p1);
	void storeChallengeTime(int p0);
	void storeQueuedChallenge(int p0, GJChallengeItem* p1);
	void storeRewardState(GJRewardType p0, int p1, int p2, gd::string p3);
	void storeSecondaryQueuedChallenge(int p0, GJChallengeItem* p1);
	void tempClear();
	void uncompleteLevel(GJGameLevel* p0);
	void unlockSecretChest(int p0);
	void unlockSpecialChest(gd::string p0);
	void verifyUserCoins();
	~GameStatsManager() = mac 0x392f0, ios 0x243394;

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

	void listButtonBarSwitchedPage(ListButtonBar* bar, int idk) = mac 0x1bc2f0, win 0x12bb40, ios 0x2266d0;
	GaragePage() = win 0x1252f0;
	bool init(IconType type, GJGarageLayer* pGarage, cocos2d::SEL_MenuHandler pSelectCallback) = mac 0x1bb710, win 0x12af70, ios 0x225d5c;
	void onGlow(cocos2d::CCObject* p0);
	void onSelect(cocos2d::CCObject* p0);
	void updateSelect(cocos2d::CCNode* p0);
	~GaragePage() = mac 0x1bb670, ios 0x225d10;

	GJGarageLayer* m_garage;
	cocos2d::SEL_MenuHandler m_handler;
	cocos2d::CCSprite* m_selectSprite;
	CCMenuItemSpriteExtra* m_unkNode0x12c;
	IconType m_type;
	int m_unknown;
	PAD = win 0x4;
}

class GauntletLayer : cocos2d::CCLayer, LevelManagerDelegate {
	virtual void keyBackClicked() = mac 0x1d5e20, ios 0x15716c;
	virtual void loadLevelsFinished(cocos2d::CCArray* p0, char const* p1) = mac 0x1d5410, ios 0x156864;
	virtual void loadLevelsFailed(char const* p0) = mac 0x1d55d0, ios 0x156a48;
	void create(GauntletType p0);
	bool init(GauntletType p0);
	void onBack(cocos2d::CCObject* p0);
	void onLevel(cocos2d::CCObject* p0);
	void scene(GauntletType p0);
	void setupGauntlet(cocos2d::CCArray* p0);
	void unlockActiveItem();
	~GauntletLayer() = mac 0x1d36c0, ios 0x154f40;
}

class GauntletNode : cocos2d::CCNode {
	void create(GJMapPack* p0);
	void frameForType(GauntletType p0);
	bool init(GJMapPack* p0);
	void nameForType(GauntletType p0);
	void onClaimReward();
	~GauntletNode() = mac 0x2eb4f0, ios 0x1217bc;
}

class GauntletSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate, LevelManagerDelegate {
	static GauntletSelectLayer* create() = win 0x105120;
	virtual void onExit() = mac 0x2eb490, ios 0x12176c;
	virtual void keyBackClicked() = mac 0x2eb370, ios 0x121720;
	virtual void scrollLayerWillScrollToPage(BoomScrollLayer* p0, int p1) = mac 0x2eb260, ios 0x121714;
	virtual void scrollLayerScrolledToPage(BoomScrollLayer* p0, int p1) = mac 0x2eb150, ios 0x121708;
	virtual void loadLevelsFinished(cocos2d::CCArray* p0, char const* p1) = mac 0x2ea630, ios 0x120df0;
	virtual void loadLevelsFailed(char const* p0) = mac 0x2ea820, ios 0x120fac;
	void create(int p0);
	void goToPage(int p0, bool p1);
	bool init(int p0);
	void onBack(cocos2d::CCObject* p0);
	void onNext(cocos2d::CCObject* p0);
	void onPlay(cocos2d::CCObject* p0);
	void onPrev(cocos2d::CCObject* p0);
	void onRefresh(cocos2d::CCObject* p0);
	void scene(int p0);
	void setupGauntlets();
	void updateArrows();
	~GauntletSelectLayer() = mac 0x2e91e0, ios 0x11fd44;
}

class GhostTrailEffect : cocos2d::CCNode {
	virtual bool init() = mac 0x360730, ios 0x213688;
	virtual void draw() = mac 0x360d20, ios 0x213c00;
	void create();
	void doBlendAdditive();
	void runWithTarget(cocos2d::CCSprite* p0, float p1, float p2, float p3, float p4, bool p5);
	void stopTrail();
	void trailSnapshot(float p0);
	~GhostTrailEffect() = mac 0x360600, ios 0x2135d8;
}

class GooglePlayDelegate {
	virtual void googlePlaySignedIn() {}
}

class GooglePlayManager : cocos2d::CCNode {
	virtual bool init() = mac 0x246130, ios 0x26a84;
	void googlePlaySignedIn();
	void sharedState();
	~GooglePlayManager() = mac 0x246050, ios 0x269b4;
}

class GravityEffectSprite : cocos2d::CCSprite {
	virtual bool init() = mac 0x80b20, ios 0xb99b8;
	virtual void draw() = mac 0x80d80, ios 0xb9be8;
	void create();
	void updateSpritesColor(cocos2d::ccColor3B p0);
	~GravityEffectSprite() = mac 0x80d90;
}

class GroupCommandObject : cocos2d::CCNode {
	virtual bool init() = mac 0x16e590, ios 0x7430;
	virtual void updateTweenAction(float p0, char const* p1) = mac 0x16ead0, ios 0x7878;
	void create();
	void createFromString(gd::string p0);
	void easeToText(int p0);
	void getEasedAction(cocos2d::CCActionInterval* p0, int p1, float p2);
	void getSaveString();
	void resetDelta(bool p0);
	void runFollowCommand(float p0, float p1, float p2, bool p3);
	void runMoveCommand(cocos2d::CCPoint p0, float p1, int p2, float p3, bool p4, bool p5);
	void runPlayerFollowCommand(float p0, float p1, int p2, float p3, float p4);
	void runRotateCommand(float p0, float p1, int p2, float p3, bool p4);
	void step(float p0);
	~GroupCommandObject() = mac 0x16e450, ios 0x7380;
}

class HSVWidgetPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x2371e0, ios 0x98420;
	void create(cocos2d::ccHSVValue p0, HSVWidgetPopupDelegate* p1, gd::string p2);
	bool init(cocos2d::ccHSVValue p0, HSVWidgetPopupDelegate* p1, gd::string p2);
	void onClose(cocos2d::CCObject* p0);
	~HSVWidgetPopup() = mac 0x236890, ios 0x97e6c;

	ConfigureHSVWidget* m_configureWidget;
	HSVWidgetPopupDelegate* m_delegate;
}

class HSVWidgetPopupDelegate {
	virtual void hsvPopupClosed(HSVWidgetPopup* p0, cocos2d::ccHSVValue p1) {}
}

class HardStreak : cocos2d::CCDrawNode {
	~HardStreak() = mac 0x5bf00, ios 0x373bc;
	virtual bool init() = mac 0x5c090, ios 0x3746c;
	void addPoint(cocos2d::CCPoint p0) = mac 0x5c950;
	void clearBehindXPos(float p0) = mac 0x5cb40;
	static HardStreak* create() = mac 0x5bfd0;
	void firstSetup() = mac 0x5c160;
	double normalizeAngle(double p0) = mac 0x5cbe0;
	cocos2d::CCPoint quadCornerOffset(cocos2d::CCPoint p0, cocos2d::CCPoint p1, float p2) = mac 0x5c990;
	void reset() = mac 0x5c930;
	void resumeStroke() = mac 0x5c210;
	void stopStroke() = mac 0x5c8f0;
	void updateStroke(float p0) = mac 0x5c240;

	cocos2d::CCArray* m_pointsArr;
	cocos2d::CCPoint m_currentPoint;
	float m_waveSize;
	float m_pulseSize;
	bool m_isSolid;
}

class InfoAlertButton : CCMenuItemSpriteExtra {
	bool init(gd::string const& title, gd::string const& text, float scale) = win 0x14ef50;
	void activate() = mac 0x2ecd70, win 0x14f050, ios 0xe0914;
	InfoAlertButton() = win 0x14ef50;
	static InfoAlertButton* create(gd::string const& title, gd::string const& text, float scale) = win 0x14ed20;
	void create(gd::string p0, gd::string p1, float p2);
	bool init(gd::string p0, gd::string p1, float p2);
	~InfoAlertButton() = mac 0x2ece40, ios 0xe0a40;
}

class InfoLayer : FLAlertLayer, LevelCommentDelegate, CommentUploadDelegate, FLAlertLayerProtocol {
	bool init(GJGameLevel* level, GJUserScore* score) = win 0x14f5a0;
	void setupCommentsBrowser(cocos2d::CCArray* comments) = win 0x152270;
	void onMore(cocos2d::CCObject* pSender) = win 0x151500;
	void onLevelInfo(cocos2d::CCObject* pSender) = win 0x151850;
	void loadPage(int page) = win 0x151e70;
	static InfoLayer* create(GJGameLevel* level, GJUserScore* score) = win 0x14f4f0;
	virtual void registerWithTouchDispatcher() = mac 0x45a070, ios 0x25caa0;
	virtual void keyBackClicked() = mac 0x45a000, ios 0x25ca24;
	virtual void show() = mac 0x45ab80, ios 0x25d430;
	virtual void loadCommentsFinished(cocos2d::CCArray* p0, char const* p1) = mac 0x45a1b0, ios 0x25cbdc;
	virtual void loadCommentsFailed(char const* p0) = mac 0x45a270, ios 0x25cc4c;
	virtual void setupPageInfo(gd::string p0, char const* p1) = mac 0x45a320, ios 0x25cca8;
	virtual void commentUploadFinished(int p0) = mac 0x45a890, ios 0x25d1a4;
	virtual void commentUploadFailed(int p0, CommentError p1) = mac 0x45a960, ios 0x25d21c;
	virtual void updateUserScoreFinished() = mac 0x45a720, ios 0x25d0f4;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x459d50, ios 0x25c8a0;
	void confirmReport(cocos2d::CCObject* p0);
	void getSpriteButton(char const* p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCMenu* p2, float p3, cocos2d::CCPoint p4);
	void isCorrect(char const* p0);
	void loadPage(int p0, bool p1);
	void onClose(cocos2d::CCObject* p0);
	void onComment(cocos2d::CCObject* p0);
	void onCopyLevelID(cocos2d::CCObject* p0);
	void onGetComments(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onOriginal(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void onRefreshComments(cocos2d::CCObject* p0);
	void onSimilar(cocos2d::CCObject* p0);
	void reloadWindow();
	void setupLevelInfo();
	void toggleCommentMode(cocos2d::CCObject* p0);
	void toggleExtendedMode(cocos2d::CCObject* p0);
	void toggleSmallCommentMode(cocos2d::CCObject* p0);
	void updateCommentModeButtons();
	void updateLevelsLabel();
	~InfoLayer() = mac 0x456560, ios 0x2597f0;

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

class InheritanceNode : cocos2d::CCObject {
	void create(int p0, InheritanceNode* p1);
	bool init(int p0, InheritanceNode* p1);
	~InheritanceNode();
}

class InstantSection : cocos2d::CCObject {
	void create(int p0, int p1);
	bool init(int p0, int p1);
	~InstantSection();
}

class KeybindingsLayer : FLAlertLayer {
	void onClose(cocos2d::CCObject* pSender) = win 0x49c60;
	void onPrevPage(cocos2d::CCObject* pSender) = win 0x153cd0;
	void onNextPage(cocos2d::CCObject* pSender) = win 0x153cc0;
	void goToPage(int page) = win 0x153ce0;
	virtual bool init() = mac 0x375890;
	virtual void keyBackClicked() = mac 0x376f30;
	void addKeyPair(char const* p0, char const* p1);
	void countForPage(int p0);
	void create();
	void incrementCountForPage(int p0);
	void infoKey(int p0);
	void layerForPage(int p0);
	void layerKey(int p0);
	void nextPosition(int p0);
	void objectKey(int p0);
	void objectsForPage(int p0);
	void onInfo(cocos2d::CCObject* p0);
	void onToggle(cocos2d::CCObject* p0);
	void pageKey(int p0);
	~KeybindingsLayer() = mac 0x375460;

	int m_currentPage;
	int m_itemCount;
	int m_pageCount;
	cocos2d::CCDictionary* m_pages;
	cocos2d::CCDictionary* m_unused;
	cocos2d::CCNode* m_leftArrow;
	cocos2d::CCNode* m_rightArrow;
}

class KeybindingsManager : cocos2d::CCNode {
	virtual bool init() = mac 0x289a00, ios 0x253ddc;
	void commandForKey(cocos2d::enumKeyCodes p0, GJKeyGroup p1, bool p2, bool p3, bool p4);
	void commandForKeyMods(cocos2d::enumKeyCodes p0, GJKeyGroup p1);
	void commandForKeyNoMods(cocos2d::enumKeyCodes p0, GJKeyGroup p1);
	void commandToKeyForGroup(GJKeyGroup p0);
	void dataLoaded(DS_Dictionary* p0);
	void encodeDataTo(DS_Dictionary* p0);
	void firstSetup();
	void groupForCommand(GJKeyCommand p0);
	void keyForCommand(GJKeyCommand p0);
	void keyToCommandForGroup(GJKeyGroup p0);
	void setCommandToKeyDict(cocos2d::CCDictionary* p0);
	void setKeyToCommandDict(cocos2d::CCDictionary* p0);
	void sharedState();
	~KeybindingsManager() = mac 0x289910, ios 0x253da0;
}

class KeysLayer : cocos2d::CCLayer, FLAlertLayerProtocol, DialogDelegate {
	virtual bool init() = mac 0x44d340, ios 0x254898;
	virtual void keyBackClicked() = mac 0x453210, ios 0x259580;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x4532e0, ios 0x25958c;
	virtual void dialogClosed(DialogLayer* p0) = mac 0x453090, ios 0x2593b8;
	void create();
	void goToPage(int p0);
	void onBack(cocos2d::CCObject* p0);
	void onSelectItem(cocos2d::CCObject* p0);
	void onShop(cocos2d::CCObject* p0);
	void onShop3(cocos2d::CCObject* p0);
	void onSpecialItem(cocos2d::CCObject* p0);
	void onSwitchPage(cocos2d::CCObject* p0);
	void scene();
	void switchToOpenedState(CCMenuItemSpriteExtra* p0);
	void updateUnlockedLabel();
	~KeysLayer() = mac 0x44ce00, ios 0x254744;
}

class LabelGameObject : EffectGameObject {
	virtual bool init() = mac 0x2f5520, ios 0x27ea8c;
	void setObjectColor(cocos2d::ccColor3B const& p0) = mac 0xdbca0, ios 0x27ed00;
	virtual void setOpacity(unsigned char p0) = mac 0xdba40, ios 0x27eb28;
	virtual void setupCustomSprites() = mac 0xdb9b0, ios 0x27ea98;
	virtual void addMainSpriteToParent(bool p0) = mac 0xdbce0, ios 0x27ed40;
	virtual void customObjectSetup(gd::map<gd::string, gd::string>& p0) = mac 0xdbbe0, ios 0x27ec6c;
	virtual gd::string getSaveString() = mac 0xdbd50, ios 0x27eda4;
	void create();
	void queueUpdateLabel(gd::string p0);
	void updateLabel(gd::string p0);
	void updateLabelIfDirty();
	~LabelGameObject() = mac 0xdb7e0, ios 0x27e9a8;
}

class LeaderboardManagerDelegate {
	virtual void updateUserScoreFinished() {}
	virtual void updateUserScoreFailed() {}
	virtual void loadLeaderboardFinished(cocos2d::CCArray* p0, char const* p1) {}
	virtual void loadLeaderboardFailed(char const* p0) {}
}

class LeaderboardsLayer : cocos2d::CCLayer, LeaderboardManagerDelegate {
	static LeaderboardsLayer* create(LeaderboardState state) = win 0x158710;
	bool init(LeaderboardState state) = win 0x1587b0;
	virtual void keyBackClicked() = mac 0x2a11c0, ios 0x29796c;
	virtual void updateUserScoreFinished() = mac 0x2a07c0, ios 0x29712c;
	virtual void updateUserScoreFailed() = mac 0x2a0820, ios 0x297160;
	virtual void loadLeaderboardFinished(cocos2d::CCArray* p0, char const* p1) = mac 0x2a08c0, ios 0x2971bc;
	virtual void loadLeaderboardFailed(char const* p0) = mac 0x2a0fa0, ios 0x2976e4;
	void isCorrect(char const* p0);
	void onBack(cocos2d::CCObject* p0);
	void onCreators(cocos2d::CCObject* p0);
	void onGlobal(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onTop(cocos2d::CCObject* p0);
	void onWeek(cocos2d::CCObject* p0);
	void refreshTabs();
	void scene(LeaderboardState p0);
	void selectLeaderboard(LeaderboardState p0);
	void setupLevelBrowser(cocos2d::CCArray* p0);
	void setupTabs();
	void toggleTabButtons();
	~LeaderboardsLayer() = mac 0x29f190, ios 0x296058;
}

class LevelBrowserLayer : cocos2d::CCLayer, LevelManagerDelegate, FLAlertLayerProtocol, SetIDPopupDelegate, SetTextPopupDelegate {
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
	virtual void onEnter() = mac 0x255b70, ios 0x2d4ab8;
	virtual void onEnterTransitionDidFinish() = mac 0x255be0, ios 0x2d4b20;
	virtual void keyBackClicked() = mac 0x255630, ios 0x2d46c4;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x255680, ios 0x2d4710;
	virtual void loadLevelsFinished(cocos2d::CCArray* p0, char const* p1) = mac 0x254e10, ios 0x2d3fdc;
	virtual void loadLevelsFailed(char const* p0) = mac 0x254f60, ios 0x2d4080;
	virtual void setupPageInfo(gd::string p0, char const* p1) = mac 0x255050, ios 0x2d40fc;
	virtual void setTextPopupClosed(SetTextPopup* p0, gd::string p1) = mac 0x255760, ios 0x2d47d0;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x255840, ios 0x2d4858;
	virtual void setIDPopupClosed(SetIDPopup* p0, int p1) = mac 0x2554f0, ios 0x2d45c0;
	void getLevelMatchingSearch(cocos2d::CCArray* p0, gd::string p1);
	void isCorrect(char const* p0);
	void onBack(cocos2d::CCObject* p0);
	void onClearSearch(cocos2d::CCObject* p0);
	void onDeleteAll(cocos2d::CCObject* p0);
	void onFavorites(cocos2d::CCObject* p0);
	void onGoToFolder(cocos2d::CCObject* p0);
	void onGoToLastPage(cocos2d::CCObject* p0);
	void onGoToPage(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onMyOnlineLevels(cocos2d::CCObject* p0);
	void onNew(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void onRefresh(cocos2d::CCObject* p0);
	void onRemoveAllFavorites(cocos2d::CCObject* p0);
	void onSaved(cocos2d::CCObject* p0);
	void onSearch(cocos2d::CCObject* p0);
	void setSearchObject(GJSearchObject* p0);
	void updatePageLabel();
	~LevelBrowserLayer() = mac 0x251130, ios 0x2d0968;

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
	void onViewProfile(cocos2d::CCObject* p0) = win 0x5c790;
	void loadCustomLevelCell() = win 0x5a020;
	void updateBGColor(unsigned int index) = win 0x5c6b0;
	virtual bool init() = mac 0x117a40, ios 0xa1864;
	virtual void draw() = mac 0x11a4d0, ios 0xa3d68;
	void create(float p0, float p1);
	void loadFromLevel(GJGameLevel* p0);
	void loadLocalLevelCell();
	void onClick(cocos2d::CCObject* p0);
	void updateBGColor(int p0);
	~LevelCell() = mac 0x117820, ios 0xa1778;

	CCMenuItemSpriteExtra* m_button;
	GJGameLevel* m_level;
	bool m_cellDrawn;
}

class LevelCommentDelegate {
	virtual void loadCommentsFinished(cocos2d::CCArray* p0, char const* p1) {}
	virtual void loadCommentsFailed(char const* p0) {}
	virtual void updateUserScoreFinished() {}
	virtual void setupPageInfo(gd::string p0, char const* p1) {}
}

class LevelDeleteDelegate {
	virtual void levelDeleteFinished(int p0) {}
	virtual void levelDeleteFailed(int p0) {}
}

class LevelDownloadDelegate {
	virtual void levelDownloadFinished(GJGameLevel* p0) {}
	virtual void levelDownloadFailed(int p0) {}
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

	~LevelEditorLayer() = mac 0x90e80, ios 0x261588;
	virtual void update(float p0) = mac 0xa1b70, win 0x26a660, ios 0x26c164;
	virtual void draw() = mac 0xa2a70, ios 0x26cc8c;
	virtual void updateColor(cocos2d::ccColor3B p0, float p1, int p2, bool p3, float p4, cocos2d::ccHSVValue p5, int p6, bool p7, int p8, EffectGameObject* p9) = mac 0x9c200, ios 0x268d08;
	virtual void flipGravity(PlayerObject* p0, bool p1, bool p2) = mac 0xa04e0, ios 0x26b22c;
	virtual void calculateColorValues(EffectGameObject* p0, EffectGameObject* p1, int p2, float p3, ColorActionSprite* p4, GJEffectManager* p5) = mac 0x9c590, ios 0x268f44;
	virtual void addToGroup(GameObject* p0, int p1, bool p2) = mac 0x9dab0, ios 0x2691fc;
	virtual void removeFromGroup(GameObject* p0, int p1) = mac 0x9db60, ios 0x26928c;
	virtual void timeForXPos(float p0) = mac 0x9c7d0, ios 0x269130;
	virtual void xPosForTime(float p0) = mac 0x9c800, ios 0x269148;
	virtual void levelSettingsUpdated() = mac 0x93f30, ios 0x2639e0;
	static LevelEditorLayer* create(GJGameLevel* level) = mac 0x90fb0, win 0x15ed60, ios 0x261628;
	void activateTriggerEffect(EffectGameObject* p0, float p1, float p2, float p3) = mac 0x9b520;
	GameObject* addObjectFromString(gd::string p0) = mac 0x94640, win 0x160c80;
	void addSpecial(GameObject* p0) = mac 0x94f30, win 0x162650;
	void addToRedoList(UndoObject* p0) = mac 0x96f80;
	void addToUndoList(UndoObject* p0, bool p1) = mac 0x94e20;
	void animateInDualGround(GameObject* p0, float p1, bool p2) = mac 0xa2780;
	void checkCollisions(PlayerObject* p0, float p1) = mac 0x9e620;
	void createBackground() = mac 0x929f0;
	void createGroundLayer() = mac 0x92840;
	GameObject* createObject(int p0, cocos2d::CCPoint p1, bool p2) = mac 0x957c0, win 0x160d70;
	void createObjectsFromSetup(gd::string p0) = mac 0x92230;
	void createObjectsFromString(gd::string p0, bool p1) = mac 0x94730;
	void getLastObjectX() = mac 0x9c860, win 0x167290;
	void getLevelString() = mac 0x97790, win 0x162480;
	void getNextColorChannel() = mac 0x9a610;
	void getNextFreeBlockID(cocos2d::CCArray* p0) = mac 0x9a4e0;
	int getNextFreeGroupID(cocos2d::CCArray* p0) = mac 0x9a1b0, win 0x164ae0;
	void getNextFreeItemID(cocos2d::CCArray* p0) = mac 0x9a390;
	void getObjectRect(GameObject* p0, bool p1) = mac 0x96240, win 0x1616b0;
	void getRelativeOffset(GameObject* p0) = mac 0x96840;
	void handleAction(bool p0, cocos2d::CCArray* p1) = mac 0x97020;
	bool init(GJGameLevel* p0) = mac 0x91010;
	void objectAtPosition(cocos2d::CCPoint p0) = mac 0x960c0, win 0x161300;
	void objectMoved(GameObject* p0) = mac 0x999f0;
	void objectsInRect(cocos2d::CCRect p0, bool p1) = mac 0x95e60;
	void onPlaytest() = mac 0xa06b0;
	void onStopPlaytest() = mac 0xa1780;
	void pasteAttributeState(GameObject* obj, cocos2d::CCArray* objs) = win 0x16b740;
	void playMusic() = mac 0xa13c0;
	void recreateGroups() = mac 0x9dbf0;
	void redoLastAction() = mac 0x97750;
	void removeAllObjects() = mac 0x93d80, win 0x161d60;
	void removeAllObjectsOfType(int p0) = mac 0x96d40;
	void removeObject(GameObject* p0, bool p1) = mac 0x96890, win 0x161cb0;
	void removeSpecial(GameObject* p0) = mac 0x969c0;
	void resetMovingObjects() = mac 0x9ddc0;
	void resetObjectVector() = mac 0x9c4b0;
	void resetToggledGroups() = mac 0x9aa70;
	void resetToggledGroupsAndObjects() = mac 0x9c3c0;
	void resetUnusedColorChannels() = mac 0x9a870;
	void rotationForSlopeNearObject(GameObject* p0) = mac 0x95cd0;
	void runColorEffect(EffectGameObject* p0, int p1, float p2, float p3, bool p4) = mac 0x9bd30;
	void setupLevelStart(LevelSettingsObject* p0) = mac 0xa0ca0;
	void sortStickyGroups() = mac 0x92b10;
	void stopTriggersInGroup(int p0, float p1) = mac 0x9c030;
	void toggleDualMode(GameObject* p0, bool p1, PlayerObject* p2, bool p3) = mac 0xa0200;
	void toggleGroupPreview(int p0, bool p1) = mac 0x9bea0;
	void transferDefaultColors(GJEffectManager* p0, GJEffectManager* p1) = mac 0x9ab50;
	void undoLastAction() = mac 0x97770;
	void updateBGAndGColors() = mac 0x9b9b0;
	void updateBlendValues() = mac 0x9bc60;
	void updateDualGround(PlayerObject* p0, int p1, bool p2) = mac 0xa1a60;
	void updateEditorMode() = mac 0x93b50, win 0x1652b0;
	void updateGameObjectsNew() = mac 0x9adc0;
	void updateGround(float p0) = mac 0x93a60;
	void updateGroundWidth() = mac 0x92af0;
	void updateOptions() = mac 0x91ed0, win 0x15fcc0;
	void updateToggledGroups() = mac 0x9bb10;
	void updateVisibility(float p0) = mac 0x92c70;
	void addObjectsRecursiveFromArray(cocos2d::CCArray* p0, int p1);
	void addRelatedStickyObjects(cocos2d::CCArray* p0);
	void assignNewStickyGroups(cocos2d::CCArray* p0);
	void breakApartTextObject(GameObject* p0);
	void canPasteState();
	void copyObjectState(GameObject* p0);
	void dirtifyTriggers();
	void enterDualMode(GameObject* p0, bool p1);
	void fastUpdateDisabledGroups();
	void fullUpdateDisabledGroups();
	void getAllObjects();
	void getGridPos(cocos2d::CCPoint p0);
	void getOtherPlayer(PlayerObject* p0);
	void getSectionCount();
	void getStickyGroup(int p0);
	void getTriggerGroup(int p0);
	void groupStickyObjects(cocos2d::CCArray* p0);
	void hasAction(bool p0);
	void isLayerLocked(int p0);
	void objectsAtPosition(cocos2d::CCPoint p0);
	void onPausePlaytest();
	void onResumePlaytest();
	void pasteAtributeState(GameObject* p0, cocos2d::CCArray* p1);
	void pasteColorState(GameObject* p0, cocos2d::CCArray* p1);
	void playerWillSwitchMode(PlayerObject* p0, GameObject* p1);
	void processLoadedMoveActions();
	void processMoveActionsStepOld();
	void removeBackground();
	void removeGroundLayer();
	void removePlayer2();
	void resetEffectTriggerOptim(GameObject* p0, cocos2d::CCArray* p1);
	void setObjectCount(int p0);
	void setStartPosObject(StartPosObject* p0);
	void shouldBlend(int p0);
	void sortBatchnodeChildren(float p0);
	void spawnGroupPreview(int p0, float p1, float p2, float p3, float p4, float p5);
	void spawnPlayer2();
	void switchToFlyMode(PlayerObject* p0, GameObject* p1, bool p2, int p3);
	void switchToRobotMode(PlayerObject* p0, GameObject* p1, bool p2);
	void switchToRollMode(PlayerObject* p0, GameObject* p1, bool p2);
	void switchToSpiderMode(PlayerObject* p0, GameObject* p1, bool p2);
	void toggleBackground(bool p0);
	void toggleGrid(bool p0);
	void toggleGround(bool p0);
	void toggleLockActiveLayer();
	void triggerFollowCommand(EffectGameObject* p0);
	void triggerPlayerFollowCommand(EffectGameObject* p0);
	void triggerRotateCommand(EffectGameObject* p0);
	void typeExistsAtPosition(int p0, cocos2d::CCPoint p1, bool p2, bool p3, float p4);
	void ungroupStickyObjects(cocos2d::CCArray* p0);
	void unlockAllLayers();
	void updateGameObjects();
	void updateLevelColors();
	void updateLevelFont(int p0);
	void updateTimeMod(float p0, bool p1);
	void validGroup(GameObject* p0, bool p1);
	void willSwitchToMode(int p0, PlayerObject* p1);

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

class LevelFeatureLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x297a00;
	void create(int p0);
	bool init(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onDown(cocos2d::CCObject* p0);
	void onDown2(cocos2d::CCObject* p0);
	void onRemoveValues(cocos2d::CCObject* p0);
	void onSetEpicOnly(cocos2d::CCObject* p0);
	void onSetFeatured(cocos2d::CCObject* p0);
	void onToggleEpic(cocos2d::CCObject* p0);
	void onUp(cocos2d::CCObject* p0);
	void onUp2(cocos2d::CCObject* p0);
	void updateStars();
	~LevelFeatureLayer() = mac 0x296c30;
}

class LevelInfoLayer : cocos2d::CCLayer, LevelDownloadDelegate, LevelUpdateDelegate, RateLevelDelegate, LikeItemDelegate, FLAlertLayerProtocol, LevelDeleteDelegate, NumberInputDelegate, SetIDPopupDelegate {
	static LevelInfoLayer* create(GJGameLevel* level) = mac 0x15f290, win 0x175d50;
	void onGarage(cocos2d::CCObject* pSender) = win 0x177c10;
	void onViewProfile(cocos2d::CCObject* pSender) = win 0x17ac90;
	void onLevelInfo(cocos2d::CCObject* pSender) = win 0x17acf0;
	void setupProgressBars() = win 0x177fc0;
	virtual void keyBackClicked() = mac 0x166160, ios 0x1e358;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x166190, ios 0x1e364;
	virtual void numberInputClosed(NumberInputLayer* p0) = mac 0x1657a0, ios 0x1dc18;
	virtual void levelDownloadFinished(GJGameLevel* p0) = mac 0x164c00, ios 0x1d3c0;
	virtual void levelDownloadFailed(int p0) = mac 0x164d50, ios 0x1d4f4;
	virtual void levelUpdateFinished(GJGameLevel* p0, UpdateResponse p1) = mac 0x164e60, ios 0x1d5dc;
	virtual void levelUpdateFailed(int p0) = mac 0x165010, ios 0x1d76c;
	virtual void levelDeleteFinished(int p0) = mac 0x165120, ios 0x1d858;
	virtual void levelDeleteFailed(int p0) = mac 0x165230, ios 0x1d950;
	virtual void rateLevelClosed() = mac 0x165e60, ios 0x1e18c;
	virtual void likedItem(LikeItemType p0, int p1, bool p2) = mac 0x165e90, ios 0x1e198;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x165f90, ios 0x1e214;
	virtual void setIDPopupClosed(SetIDPopup* p0, int p1) = mac 0x163e60, ios 0x1c95c;
	void confirmClone(cocos2d::CCObject* p0);
	void confirmDelete(cocos2d::CCObject* p0);
	void confirmMoveToBottom(cocos2d::CCObject* p0);
	void confirmMoveToTop(cocos2d::CCObject* p0);
	void confirmOwnerDelete(cocos2d::CCObject* p0);
	void downloadLevel();
	void incrementDislikes();
	void incrementLikes();
	bool init(GJGameLevel* p0);
	void onBack(cocos2d::CCObject* p0);
	void onClone(cocos2d::CCObject* p0);
	void onDelete(cocos2d::CCObject* p0);
	void onFavorite(cocos2d::CCObject* p0);
	void onFeatured(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onLevelLeaderboard(cocos2d::CCObject* p0);
	void onLike(cocos2d::CCObject* p0);
	void onLowDetailMode(cocos2d::CCObject* p0);
	void onOwnerDelete(cocos2d::CCObject* p0);
	void onPlay(cocos2d::CCObject* p0);
	void onPlayReplay(cocos2d::CCObject* p0);
	void onRate(cocos2d::CCObject* p0);
	void onRateDemon(cocos2d::CCObject* p0);
	void onRateStars(cocos2d::CCObject* p0);
	void onRateStarsMod(cocos2d::CCObject* p0);
	void onSetFolder(cocos2d::CCObject* p0);
	void onUpdate(cocos2d::CCObject* p0);
	void playStep2();
	void playStep3();
	void scene(GJGameLevel* p0);
	void setupLevelInfo();
	void shouldDownloadLevel();
	void showSongWarning();
	void showUpdateAlert(UpdateResponse p0);
	void tryCloneLevel(cocos2d::CCObject* p0);
	void tryShowAd();
	void updateLabelValues();
	void updateSideButtons();
	~LevelInfoLayer() = mac 0x15f1b0, ios 0x18480;

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

class LevelLeaderboard : FLAlertLayer, LeaderboardManagerDelegate {
	void onChangeType(cocos2d::CCObject* pSender) = win 0x17d090;
	void onGarage(cocos2d::CCObject* pSender) = win 0x17d1b0;
	bool init(GJGameLevel* level, int type) = win 0x17c4f0;
	static LevelLeaderboard* create(GJGameLevel* level, LevelLeaderboardType leaderboardType) = win 0x17c440;
	virtual void registerWithTouchDispatcher() = mac 0x20e7a0, ios 0x16cd70;
	virtual void keyBackClicked() = mac 0x20e730, ios 0x16ccf4;
	virtual void show() = mac 0x20ec40, ios 0x16d1d0;
	virtual void loadLeaderboardFinished(cocos2d::CCArray* p0, char const* p1) = mac 0x20e980, ios 0x16cfc4;
	virtual void loadLeaderboardFailed(char const* p0) = mac 0x20ead0, ios 0x16d064;
	virtual void updateUserScoreFinished() = mac 0x20e660, ios 0x16cc8c;
	virtual void updateUserScoreFailed() = mac 0x20e690, ios 0x16cc98;
	bool init(GJGameLevel* p0, LevelLeaderboardType p1);
	void isCorrect(char const* p0);
	void loadScores();
	void onClose(cocos2d::CCObject* p0);
	void onUpdate(cocos2d::CCObject* p0);
	void setupLeaderboard(cocos2d::CCArray* p0);
	~LevelLeaderboard() = mac 0x20d4b0, ios 0x16bc7c;
}

class LevelManagerDelegate {
	virtual void loadLevelsFinished(cocos2d::CCArray* p0, char const* p1) {}
	virtual void loadLevelsFailed(char const* p0) {}
	virtual void setupPageInfo(gd::string p0, char const* p1) {}
}

class LevelPage : cocos2d::CCLayer, DialogDelegate {
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x23cd90, ios 0x2cab10;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x23ceb0, ios 0x2cac0c;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x23ced0, ios 0x2cac14;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x23cef0, ios 0x2cac1c;
	virtual void registerWithTouchDispatcher() = mac 0x23cf30, ios 0x2cac38;
	virtual void dialogClosed(DialogLayer* p0) = mac 0x23cad0, ios 0x2ca934;
	void addSecretCoin();
	void addSecretDoor();
	void create(GJGameLevel* p0);
	bool init(GJGameLevel* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onPlay(cocos2d::CCObject* p0);
	void onSecretDoor(cocos2d::CCObject* p0);
	void playCoinEffect();
	void playStep2();
	void playStep3();
	void updateDynamicPage(GJGameLevel* p0);
	~LevelPage() = mac 0x23aeb0, ios 0x2c9174;

	PAD = win 0x124;
	GJGameLevel* m_level;
}

class LevelSearchLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DemonFilterDelegate {
	static LevelSearchLayer* create() = win 0x17d9c0;
	GJSearchObject* getSearchObject(SearchType p0, gd::string p1) = win 0x1805f0;
	void onMoreOptions(cocos2d::CCObject* p0) = win 0x17f500;
	void onSearch(cocos2d::CCObject* p0) = win 0x180fc0;
	virtual bool init() = mac 0x384770, ios 0x31358;
	virtual void keyBackClicked() = mac 0x3891f0, ios 0x34c14;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0x3885f0, ios 0x343e0;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x3887f0, ios 0x34538;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x388910, ios 0x345b8;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x388580, ios 0x34348;
	virtual void demonFilterSelectClosed(int p0) = mac 0x388040, ios 0x34024;
	void checkDiff(int p0);
	void checkTime(int p0);
	void clearFilters();
	void confirmClearFilters(cocos2d::CCObject* p0);
	void getDiffKey(int p0);
	void getLevelLenKey();
	void getSearchDiffKey();
	void getTimeKey(int p0);
	void onBack(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onFollowed(cocos2d::CCObject* p0);
	void onFriends(cocos2d::CCObject* p0);
	void onLatestStars(cocos2d::CCObject* p0);
	void onMagic(cocos2d::CCObject* p0);
	void onMostDownloaded(cocos2d::CCObject* p0);
	void onMostLikes(cocos2d::CCObject* p0);
	void onMostRecent(cocos2d::CCObject* p0);
	void onSearchUser(cocos2d::CCObject* p0);
	void onSpecialDemon(cocos2d::CCObject* p0);
	void onStarAward(cocos2d::CCObject* p0);
	void onTrending(cocos2d::CCObject* p0);
	void scene();
	void toggleDifficulty(cocos2d::CCObject* p0);
	void toggleDifficultyNum(int p0, bool p1);
	void toggleStar(cocos2d::CCObject* p0);
	void toggleTime(cocos2d::CCObject* p0);
	void toggleTimeNum(int p0, bool p1);
	void updateSearchLabel(char const* p0);
	~LevelSearchLayer() = mac 0x384430, ios 0x3120c;

	PAD = win 0xC;
	CCTextInputNode* m_searchInput;
}

class LevelSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate, DynamicScrollDelegate {
	static LevelSelectLayer* create(int lvl) = win 0x185500;
	virtual void keyBackClicked() = mac 0x23a5e0, ios 0x2c896c;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x23a680, ios 0x2c8978;
	virtual void updatePageWithObject(cocos2d::CCObject* p0, cocos2d::CCObject* p1) = mac 0x2390a0, ios 0x2c794c;
	virtual void scrollLayerMoved(cocos2d::CCPoint p0) = mac 0x23a100, ios 0x2c862c;
	void colorForPage(int p0);
	void getColorValue(int p0, int p1, float p2);
	bool init(int p0);
	void onBack(cocos2d::CCObject* p0);
	void onDownload(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onNext(cocos2d::CCObject* p0);
	void onPlay(cocos2d::CCObject* p0);
	void onPrev(cocos2d::CCObject* p0);
	void scene(int p0);
	void tryShowAd();
	~LevelSelectLayer() = mac 0x2381d0, ios 0x2c6df4;

	PAD = win 0x10;
	BoomScrollLayer* m_scrollLayer;
}

class LevelSettingsDelegate {
	virtual void levelSettingsUpdated() {}
}

class LevelSettingsLayer : FLAlertLayer, ColorSelectDelegate, SelectArtDelegate, FLAlertLayerProtocol, CustomSongLayerDelegate {
	static LevelSettingsLayer* create(LevelSettingsObject* levelSettings, LevelEditorLayer* editor) = win 0x170d90;
	virtual void registerWithTouchDispatcher() = mac 0xac070, ios 0x27421c;
	virtual void keyBackClicked() = mac 0xac7b0, ios 0x27461c;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0xac260, ios 0x27432c;
	virtual void colorSelectClosed(cocos2d::CCNode* p0) = mac 0xac100, ios 0x274274;
	virtual void customSongLayerClosed() = mac 0xac340, ios 0x2743ac;
	virtual void selectArtClosed(SelectArtLayer* p0) = mac 0xac5e0, ios 0x274498;
	void audioNext(cocos2d::CCObject* p0);
	void audioPrevious(cocos2d::CCObject* p0);
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4);
	bool init(LevelSettingsObject* p0, LevelEditorLayer* p1);
	void on3DLine(cocos2d::CCObject* p0);
	void onBG(cocos2d::CCObject* p0);
	void onBGArt(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onCol(cocos2d::CCObject* p0);
	void onDualMode(cocos2d::CCObject* p0);
	void onG(cocos2d::CCObject* p0);
	void onG2(cocos2d::CCObject* p0);
	void onGArt(cocos2d::CCObject* p0);
	void onGravityFlipped(cocos2d::CCObject* p0);
	void onLine(cocos2d::CCObject* p0);
	void onLiveEdit(cocos2d::CCObject* p0);
	void onMiniMode(cocos2d::CCObject* p0);
	void onObj(cocos2d::CCObject* p0);
	void onOpenCustomSong(cocos2d::CCObject* p0);
	void onSelectFont(cocos2d::CCObject* p0);
	void onSelectMode(cocos2d::CCObject* p0);
	void onSelectSpeed(cocos2d::CCObject* p0);
	void onSongMode(cocos2d::CCObject* p0);
	void onTwoPlayerMode(cocos2d::CCObject* p0);
	void onTwoPlayerModeInfo(cocos2d::CCObject* p0);
	void selectSong(int p0);
	void showCustomSongSelect();
	void showPicker(int p0, ColorAction* p1);
	void updateAudioLabel();
	void updateColorSprite(ColorChannelSprite* p0);
	void updateColorSprites();
	~LevelSettingsLayer() = mac 0xa7b90, ios 0x270930;
}

class LevelSettingsObject : cocos2d::CCNode {
	~LevelSettingsObject() = mac 0xa5650, ios 0x26eb7c;
	virtual bool init() = mac 0xa5690, ios 0x26ebb8;
	static LevelSettingsObject* create() = mac 0x92760;
	void objectFromDict(cocos2d::CCDictionary* p0) = mac 0xa5810;
	static LevelSettingsObject* objectFromString(gd::string p0) = mac 0x945a0;
	void setupColorsFromLegacyMode(cocos2d::CCDictionary* p0) = mac 0xa6a30;
	void getSaveString();
	void offsetMusic();

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

class LevelUpdateDelegate {
	virtual void levelUpdateFinished(GJGameLevel* p0, UpdateResponse p1) {}
	virtual void levelUpdateFailed(int p0) {}
}

class LevelUploadDelegate {
	virtual void levelUploadFinished(GJGameLevel* p0) {}
	virtual void levelUploadFailed(GJGameLevel* p0) {}
}

class LikeItemDelegate {
	virtual void likedItem(LikeItemType p0, int p1, bool p2) {}
}

class LikeItemLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x35ff20, ios 0x168bcc;
	void create(LikeItemType p0, int p1, int p2);
	bool init(LikeItemType p0, int p1, int p2);
	void onClose(cocos2d::CCObject* p0);
	void onDislike(cocos2d::CCObject* p0);
	void onLike(cocos2d::CCObject* p0);
	void triggerLike(bool p0);
	~LikeItemLayer() = mac 0x35f4c0, ios 0x1685a8;
}

class ListButtonBar : cocos2d::CCNode {
	void create(cocos2d::CCArray* p0, cocos2d::CCPoint p1, int p2, int p3, float p4, float p5, float p6, float p7, int p8);
	void getPage();
	void goToPage(int p0);
	bool init(cocos2d::CCArray* p0, cocos2d::CCPoint p1, int p2, int p3, float p4, float p5, float p6, float p7, int p8);
	void onLeft(cocos2d::CCObject* p0);
	void onRight(cocos2d::CCObject* p0);
	~ListButtonBar() = mac 0x210720, ios 0xdfaec;

	BoomScrollLayer* m_scrollLayer;
}

class ListButtonBarDelegate {
	virtual void listButtonBarSwitchedPage(ListButtonBar* p0, int p1) {}
}

class ListButtonPage : cocos2d::CCLayer {
	void create(cocos2d::CCArray* p0, cocos2d::CCPoint p1, int p2, int p3, float p4, float p5, float p6);
	bool init(cocos2d::CCArray* p0, cocos2d::CCPoint p1, int p2, int p3, float p4, float p5, float p6);
	~ListButtonPage();
}

class ListCell : TableViewCell {
	virtual bool init() = mac 0x18f7a0, ios 0x12cc80;
	virtual void draw() = mac 0x18f840, ios 0x12cc88;
	void loadFromObject(cocos2d::CCObject* p0, int p1, int p2, int p3);
	void updateBGColor(int p0);
	~ListCell() = mac 0x18fad0, ios 0xa7fb8;
}

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
	virtual bool init() = mac 0x277f00, ios 0x27f280;
	virtual void draw() = mac 0x278170, ios 0x27f4e4;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2781d0;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2781e0;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2781f0;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x278200;
	virtual void registerWithTouchDispatcher() = mac 0x278190, ios 0x27f4f4;
	~LoadingCircle() = mac 0x277e60, ios 0x27f234;

	cocos2d::CCSprite* m_sprite;
	cocos2d::CCLayer* m_parentLayer;
	bool m_fade;
}

class LoadingLayer : cocos2d::CCLayer {
	void setFromRefresh(bool value) {
	    m_fromRefresh = value;
	}
	static cocos2d::CCScene* scene(bool fromReload) {
		auto scene = cocos2d::CCScene::create();
		scene->addChild(LoadingLayer::create(fromReload));
		return scene;
	}

	static LoadingLayer* create(bool fromReload) = mac 0x1df1f0, win 0x18bfe0, ios 0x130278;
	bool init(bool fromReload) = mac 0x1df2f0, win 0x18c080;
	char const* getLoadingString() = win 0x18cf40;
	void loadAssets() = mac 0x1dfb20, win 0x18c8e0, ios 0x130278;
	void loadingFinished() = win 0x18c790;
	void updateProgress(int p0);
	~LoadingLayer() = mac 0x1e0330, ios 0x130928;

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
	virtual bool init() = mac 0x35dfc0, ios 0x127888;
	virtual void encodeDataTo(DS_Dictionary* p0) = mac 0x35ed60, ios 0x128534;
	virtual void dataLoaded(DS_Dictionary* p0) = mac 0x35eda0, ios 0x128574;
	virtual void firstLoad() = mac 0x35ed10, ios 0x12850c;
	void getAllLevelsInDict();
	void getAllLevelsWithName(gd::string p0);
	void getCreatedLevels(int p0);
	void getLevelsInNameGroups();
	void getLoadString(int p0);
	void getMainLevelString(int p0);
	void markLevelsAsUnmodified();
	void moveLevelToTop(GJGameLevel* p0);
	void reorderLevels();
	void setLocalLevels(cocos2d::CCArray* p0);
	void updateLevelOrder();
	void updateLevelRevision();
	~LocalLevelManager() = mac 0x35df80, ios 0x12784c;

	cocos2d::CCDictionary* m_loadData;
	cocos2d::CCDictionary* m_levelData;
	cocos2d::CCArray* m_localLevels;
}

class MapPackCell : TableViewCell {
	void updateBGColor(unsigned int index) = win 0x5c6b0;
	virtual bool init() = mac 0x11a9c0, ios 0xa3f5c;
	virtual void draw() = mac 0x11ae80, ios 0xa43c8;
	void loadFromMapPack(GJMapPack* p0);
	void onClaimReward(cocos2d::CCObject* p0);
	void onClick(cocos2d::CCObject* p0);
	void playCompleteEffect();
	void reloadCell();
	void updateBGColor(int p0);
	~MapPackCell() = mac 0x11a670, ios 0xa3ea0;
}

class MenuGameLayer : cocos2d::CCLayer {
	void resetPlayer() = mac 0x28fdc0;
	void update(float p0) = mac 0x28fa70, ios 0x27fea0;
	virtual bool init() = mac 0x28f150, ios 0x27f654;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x290650, ios 0x280898;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x290770, ios 0x2809b0;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x290790, ios 0x2809b8;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2907b0, ios 0x2809c0;
	virtual void registerWithTouchDispatcher() = mac 0x2907f0, ios 0x2809dc;
	void create();
	void destroyPlayer();
	void getBGColor(int p0);
	void tryJump(float p0);
	void updateColor(float p0);
	void updateColors();
	~MenuGameLayer() = mac 0x28ef90, ios 0x27f594;
}

class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol, GooglePlayDelegate {
	~MenuLayer() = mac 0x1d1230, ios 0x19e530;
	virtual bool init() = mac 0x1d14b0, win 0x1907b0, ios 0x19e6d4;
	virtual void keyBackClicked() = mac 0x1d3160, win 0x191f90, ios 0x1a0024;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x1d33d0, win 0x1922c0, ios 0x1a0354;
	virtual void googlePlaySignedIn() = mac 0x1d2f30, ios 0x19fe90;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x1d3190, win 0x192150, ios 0x1a010c;
	void onMoreGames(cocos2d::CCObject* p0) = mac 0x1d2ad0, win 0x1919c0, ios 0x19fbb4;
	void onGarage(cocos2d::CCObject* p0) = win 0x191dc0;
	void onQuit(cocos2d::CCObject* p0) = mac 0x1d2b40, win 0x191fa0;
	void onMyProfile(cocos2d::CCObject* p0) = mac 0x1d2c20, win 0x1e0950;
	void onPlay(cocos2d::CCObject* p0) = win 0x191b50;
	void onCreator(cocos2d::CCObject* p0) = win 0x191cd0;
	void onRobTop(cocos2d::CCObject* p0) = win 0x191940;
	void onNewgrounds(cocos2d::CCObject* p0) = win 0x191e90;
	void onDaily(cocos2d::CCObject* p0) = win 0x1916e0;
	void onOptions(cocos2d::CCObject* p0) = win 0x191c30;
	void onAchievements(cocos2d::CCObject* p0) = win 0x191af0;
	void onStats(cocos2d::CCObject* p0) = win 0x191ca0;
	void onFacebook(cocos2d::CCObject* p0) = win 0x191960;
	void onTwitter(cocos2d::CCObject* p0) = win 0x191980;
	void onYouTube(cocos2d::CCObject* p0) = win 0x1919A0;
	static cocos2d::CCScene* scene(bool p0) = mac 0x1d12d0, win 0x190720, ios 0x19e57c;
	MenuLayer* node() = win 0x190550;
	void endGame();
	void onEveryplay(cocos2d::CCObject* p0);
	void onFreeLevels(cocos2d::CCObject* p0);
	void onFullVersion(cocos2d::CCObject* p0);
	void onGameCenter(cocos2d::CCObject* p0);
	void onGooglePlayGames(cocos2d::CCObject* p0);
	void onOptionsInstant();
	void onTrailer(cocos2d::CCObject* p0);
	void openOptions(bool p0);
	void showGCQuestion();
	void showMeltdownPromo();
	void syncPlatformAchievements(float p0);
	void tryShowAd(float p0);
	void updateUserProfileButton();
	void willClose();
}

class MessageListDelegate {
	virtual void loadMessagesFinished(cocos2d::CCArray* p0, char const* p1) {}
	virtual void loadMessagesFailed(char const* p0, GJErrorCode p1) {}
	virtual void forceReloadMessages(bool p0) {}
	virtual void setupPageInfo(gd::string p0, char const* p1) {}
}

class MessagesProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, MessageListDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x107eb0, ios 0x12b1a8;
	virtual void keyBackClicked() = mac 0x107e80, ios 0x12b19c;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x107b20, ios 0x12adf4;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x107b90, ios 0x12ae84;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x107bf0, ios 0x12aef4;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x107d50, ios 0x12b090;
	virtual void loadMessagesFinished(cocos2d::CCArray* p0, char const* p1) = mac 0x108150, ios 0x12b4a0;
	virtual void loadMessagesFailed(char const* p0, GJErrorCode p1) = mac 0x108280, ios 0x12b5b4;
	virtual void forceReloadMessages(bool p0) = mac 0x1083b0, ios 0x12b654;
	virtual void setupPageInfo(gd::string p0, char const* p1) = mac 0x108400, ios 0x12b688;
	void create(bool p0);
	void deleteSelected();
	bool init(bool p0);
	void isCorrect(char const* p0);
	void loadPage(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onDeleteSelected(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void onSentMessages(cocos2d::CCObject* p0);
	void onToggleAllObjects(cocos2d::CCObject* p0);
	void onUpdate(cocos2d::CCObject* p0);
	void setupCommentsBrowser(cocos2d::CCArray* p0);
	void untoggleAll();
	void updateLevelsLabel();
	void updatePageArrows();
	~MessagesProfilePage() = mac 0x106360, ios 0x1299d4;
}

class MoreOptionsLayer : FLAlertLayer, TextInputDelegate, GooglePlayDelegate {
	static MoreOptionsLayer* create() = win 0x1de850;
	void addToggle(char const* name, char const* key, char const* info) = win 0x1df6b0;
	void onKeybindings(cocos2d::CCObject* pSender) = win 0x749d0;
	virtual bool init() = mac 0x43f470, ios 0x8779c;
	virtual void keyBackClicked() = mac 0x441f50, ios 0x8a104;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x4420a0, ios 0x8a110;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x442160, ios 0x8a17c;
	virtual void googlePlaySignedIn() = mac 0x442210, ios 0x8a1dc;
	void countForPage(int p0);
	void goToPage(int p0);
	void incrementCountForPage(int p0);
	void infoKey(int p0);
	void layerForPage(int p0);
	void layerKey(int p0);
	void nextPosition(int p0);
	void objectKey(int p0);
	void objectsForPage(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onGPSignIn(cocos2d::CCObject* p0);
	void onGPSignOut(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onParental(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void onSongBrowser(cocos2d::CCObject* p0);
	void onToggle(cocos2d::CCObject* p0);
	void pageKey(int p0);
	void toggleGP();
	~MoreOptionsLayer() = mac 0x43f3d0, ios 0x87750;
}

class MoreSearchLayer : FLAlertLayer, TextInputDelegate {
	static MoreSearchLayer* create() = win 0x182520;
	bool init() = mac 0x3896b0, win 0x1825c0, ios 0x34f48;
	virtual void keyBackClicked() = mac 0x38b820, ios 0x36c2c;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x38b6b0, ios 0x36b60;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x38b770, ios 0x36bcc;
	void audioNext(cocos2d::CCObject* p0);
	void audioPrevious(cocos2d::CCObject* p0);
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4);
	void onClose(cocos2d::CCObject* p0);
	void onCoins(cocos2d::CCObject* p0);
	void onCompleted(cocos2d::CCObject* p0);
	void onEpic(cocos2d::CCObject* p0);
	void onFeatured(cocos2d::CCObject* p0);
	void onFollowed(cocos2d::CCObject* p0);
	void onFriends(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onNoStar(cocos2d::CCObject* p0);
	void onOriginal(cocos2d::CCObject* p0);
	void onSongFilter(cocos2d::CCObject* p0);
	void onSongMode(cocos2d::CCObject* p0);
	void onTwoPlayer(cocos2d::CCObject* p0);
	void onUncompleted(cocos2d::CCObject* p0);
	void selectSong(int p0);
	void toggleSongNodes(bool p0, bool p1);
	void updateAudioLabel();
	~MoreSearchLayer() = mac 0x389610, ios 0x34efc;
}

class MoreVideoOptionsLayer : FLAlertLayer {
	static MoreVideoOptionsLayer* create() = mac 0x443c10;
	virtual bool init() = mac 0x444150, ios 0x8b7e0;
	void onClose(cocos2d::CCObject* p0) = mac 0x444aa0;
	virtual void keyBackClicked() = mac 0x445ba0, ios 0x8d314;
	void addToggle(char const* p0, char const* p1, char const* p2);
	void countForPage(int p0);
	void goToPage(int p0);
	void incrementCountForPage(int p0);
	void infoKey(int p0);
	void layerForPage(int p0);
	void layerKey(int p0);
	void nextPosition(int p0);
	void objectKey(int p0);
	void objectsForPage(int p0);
	void onInfo(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void onToggle(cocos2d::CCObject* p0);
	void pageKey(int p0);
	~MoreVideoOptionsLayer() = mac 0x4440b0, ios 0x8b794;
}

class MultilineBitmapFont : cocos2d::CCSprite {
	virtual void setOpacity(unsigned char p0) = mac 0x24f030, ios 0x158460;
	void createWithFont(char const* p0, gd::string p1, float p2, float p3, cocos2d::CCPoint p4, int p5, bool p6);
	bool initWithFont(char const* p0, gd::string p1, float p2, float p3, cocos2d::CCPoint p4, int p5, bool p6);
	void moveSpecialDescriptors(int p0, int p1);
	void readColorInfo(gd::string p0);
	void stringWithMaxWidth(gd::string p0, float p1, float p2);
	~MultilineBitmapFont() = mac 0x24f430, ios 0x158798;
}

class MusicDelegateHandler : cocos2d::CCNode {
	void create(MusicDownloadDelegate* p0);
	bool init(MusicDownloadDelegate* p0);
	~MusicDelegateHandler() = mac 0x2ee460, ios 0xd0fdc;
}

class MusicDownloadDelegate {
	virtual void downloadSongFailed(int p0, GJSongError p1) {}
	virtual void downloadSongFinished(SongInfoObject* p0) {}
	virtual void loadSongInfoFailed(int p0, GJSongError p1) {}
	virtual void loadSongInfoFinished(SongInfoObject* p0) {}
	virtual void songStateChanged() {}
}

class MusicDownloadManager : cocos2d::CCNode, PlatformDownloadDelegate {
	void incrementPriorityForSong(int p0) = mac 0x2ef750;
	static MusicDownloadManager* sharedState() = mac 0x2ee4c0, win 0x1945b0;
	gd::string pathForSong(int id) = mac 0x2f1140, win 0x1960e0;
	cocos2d::CCArray* getDownloadedSongs() = win 0x195640;
	void songStateChanged() = win 0x194d90;
	virtual bool init() = mac 0x2ef100, ios 0xd1670;
	void ProcessHttpRequest(gd::string p0, gd::string p1, gd::string p2, GJHttpType p3);
	void addDLToActive(char const* p0);
	void addDLToActive(char const* p0, cocos2d::CCObject* p1);
	void addMusicDownloadDelegate(MusicDownloadDelegate* p0);
	void addSongObjectFromString(gd::string p0);
	void clearSong(int p0);
	void clearUnusedSongs();
	void createSongsInfo(gd::string p0);
	void dataLoaded(DS_Dictionary* p0);
	void deleteSong(int p0);
	void downloadSong(int p0);
	void downloadSongFailed(int p0, GJSongError p1);
	void downloadSongFinished(SongInfoObject* p0);
	void encodeDataTo(DS_Dictionary* p0);
	void firstSetup();
	void getAllSongs();
	void getDLObject(char const* p0);
	void getDownloadProgress(int p0);
	void getSongDownloadKey(int p0);
	void getSongInfo(int p0, bool p1);
	void getSongInfoKey(int p0);
	void getSongInfoObject(int p0);
	void getSongPriority();
	void handleIt(bool p0, gd::string p1, gd::string p2, GJHttpType p3);
	void handleItDelayed(bool p0, gd::string p1, gd::string p2, GJHttpType p3);
	void handleItND(cocos2d::CCNode* p0, void* p1);
	void isDLActive(char const* p0);
	void isRunningActionForSongID(int p0);
	void isSongDownloaded(int p0);
	void limitDownloadedSongs();
	void loadSongInfoFailed(int p0, GJSongError p1);
	void loadSongInfoFinished(SongInfoObject* p0);
	void onDownloadSongCompleted(cocos2d::extension::CCHttpClient* p0, cocos2d::extension::CCHttpResponse* p1);
	void onGetSongInfoCompleted(gd::string p0, gd::string p1);
	void onProcessHttpRequestCompleted(cocos2d::extension::CCHttpClient* p0, cocos2d::extension::CCHttpResponse* p1);
	void removeDLFromActive(char const* p0);
	void removeMusicDownloadDelegate(MusicDownloadDelegate* p0);
	void responseToDict(gd::string p0, char const* p1);
	void setDownloadedSongsDict(cocos2d::CCDictionary* p0);
	void showTOS(FLAlertLayerProtocol* p0);
	void stopDownload(int p0);
	~MusicDownloadManager() = mac 0x2ef0c0, ios 0xd1634;

	cocos2d::CCDictionary* m_unknownDict;
	cocos2d::CCArray* m_handlers;
	cocos2d::CCDictionary* m_songsDict;
	int m_unknown;
}

class MyLevelsLayer : cocos2d::CCLayer {
	virtual bool init() = mac 0x35470, ios 0x81b90;
	virtual void keyBackClicked() = mac 0x35b90, ios 0x8222c;
	void create();
	void onBack(cocos2d::CCObject* p0);
	void onNew(cocos2d::CCObject* p0);
	void scene();
	void setupLevelBrowser();
	~MyLevelsLayer() = mac 0x350d0, ios 0x819d0;
}

class NodePoint : cocos2d::CCObject {
	void create(cocos2d::CCPoint p0);
	bool init(cocos2d::CCPoint p0);
	~NodePoint();
}

class NumberInputDelegate {
	virtual void numberInputClosed(NumberInputLayer* p0) {}
}

class NumberInputLayer : FLAlertLayer {
	virtual bool init() = mac 0x255ff0, ios 0x952f4;
	virtual void registerWithTouchDispatcher() = mac 0x256a70, ios 0x95bcc;
	virtual void keyBackClicked() = mac 0x2569c0, ios 0x95b24;
	void create();
	void deleteLast();
	void inputNumber(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onDone(cocos2d::CCObject* p0);
	void onNumber(cocos2d::CCObject* p0);
	void updateNumberState();
	~NumberInputLayer() = mac 0x255db0, ios 0x951e0;
}

class OBB2D : cocos2d::CCNode {
	void calculateWithCenter(cocos2d::CCPoint p0, float p1, float p2, float p3) = mac 0x35a9c0;
	static OBB2D* create(cocos2d::CCPoint p0, float p1, float p2, float p3) = mac 0x35a890;
	cocos2d::CCRect getBoundingRect() = mac 0x35b2b0;
	void overlaps(OBB2D* p0) = mac 0x35b0a0;
	void overlaps1Way(OBB2D* p0) = mac 0x35b0d0;
	void computeAxes();
	bool init(cocos2d::CCPoint p0, float p1, float p2, float p3);
	void orderCorners();
	~OBB2D();

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

class ObjectDecoder : cocos2d::CCNode {
	virtual bool init() = mac 0x437f80, ios 0x1589dc;
	void getDecodedObject(int p0, DS_Dictionary* p1);
	void sharedDecoder();
	~ObjectDecoder() = mac 0x437fb0;
}

class ObjectDecoderDelegate {
	virtual void getDecodedObject(int p0, DS_Dictionary* p1) {}
}

class ObjectManager : cocos2d::CCNode {
	static ObjectManager* instance() = win 0x2c2c0;
	void setup() = win 0x2c3b0;
	virtual bool init() = mac 0x43b280, ios 0x168d48;
	void animLoaded(char const* p0);
	void getDefinition(char const* p0);
	void getGlobalAnimCopy(char const* p0);
	void loadCopiedAnimations();
	void loadCopiedSets();
	void purgeObjectManager();
	void replaceAllOccurencesOfString(cocos2d::CCString* p0, cocos2d::CCString* p1, cocos2d::CCDictionary* p2);
	void setLoaded(char const* p0);
	~ObjectManager() = mac 0x43c970, ios 0x16a85c;
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

	virtual bool init() = mac 0x3b2d80, ios 0x1b092c;
	static ObjectToolbox* sharedState() = mac 0x3b2bc0, win 0x198a80;
	float gridNodeSizeForKey(int key) = win 0x1dc920;
	char const* perspectiveBlockFrame(int key) = win 0x1dcd00;
	~ObjectToolbox() = mac 0x3b2ca0, ios 0x1b08f0;

	cocos2d::CCDictionary* m_frameToKeyDict;
	cocos2d::CCDictionary* m_keyToFrameDict;
}

class OnlineListDelegate {
	virtual void loadListFinished(cocos2d::CCArray* p0, char const* p1) {}
	virtual void loadListFailed(char const* p0) {}
	virtual void setupPageInfo(gd::string p0, char const* p1) {}
}

class OpacityEffectAction : cocos2d::CCNode {
	static OpacityEffectAction* create(float p0, float p1, float p2, int p3) = mac 0x1789f0;
	static OpacityEffectAction* createFromString(gd::string p0) = mac 0x178c10;
	bool init(float p0, float p1, float p2, int p3) = mac 0x178b00;
	void step(float p0) = mac 0x178b90;
	void getSaveString();
	~OpacityEffectAction();

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

class OptionsLayer : GJDropDownLayer, FLAlertLayerProtocol {
	static OptionsLayer* addToCurrentScene(bool noTransition) {
	    auto layer = OptionsLayer::create();
	    cocos2d::CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
	    layer->showLayer(noTransition);
	    return layer;
	}

	static OptionsLayer* create() = mac 0x43db60, win 0x1dd310, ios 0x86314;
	virtual void customSetup() = mac 0x43dc70, ios 0x86434;
	virtual void layerHidden() = mac 0x43f200, ios 0x875e8;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x43f270, ios 0x87674;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4);
	void exitLayer();
	void musicSliderChanged(cocos2d::CCObject* p0);
	void onAccount(cocos2d::CCObject* p0);
	void onHelp(cocos2d::CCObject* p0);
	void onOptions(cocos2d::CCObject* p0);
	void onProgressBar(cocos2d::CCObject* p0);
	void onRate(cocos2d::CCObject* p0);
	void onRecordReplays(cocos2d::CCObject* p0);
	void onSecretVault(cocos2d::CCObject* p0);
	void onSoundtracks(cocos2d::CCObject* p0);
	void onSupport(cocos2d::CCObject* p0);
	void onVideo(cocos2d::CCObject* p0);
	void sfxSliderChanged(cocos2d::CCObject* p0);
	void tryEnableRecord();
	~OptionsLayer() = mac 0x43dbd0, ios 0x863e8;
}

class ParentalOptionsLayer : FLAlertLayer {
	virtual bool init() = mac 0x445da0, ios 0x8d484;
	virtual void keyBackClicked() = mac 0x4472d0, ios 0x8e8b4;
	void addToggle(char const* p0, char const* p1, char const* p2);
	void countForPage(int p0);
	void create();
	void goToPage(int p0);
	void incrementCountForPage(int p0);
	void infoKey(int p0);
	void layerForPage(int p0);
	void layerKey(int p0);
	void nextPosition(int p0);
	void objectKey(int p0);
	void objectsForPage(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void onToggle(cocos2d::CCObject* p0);
	void pageKey(int p0);
	~ParentalOptionsLayer() = mac 0x445d00, ios 0x8d438;
}

class PauseLayer : CCBlockLayer {
	static PauseLayer* create(bool p0) = mac 0x20b1e0;
	void onEdit(cocos2d::CCObject* p0) = mac 0x20c630;
	void createToggleButton(cocos2d::SEL_MenuHandler callback, bool on, cocos2d::CCMenu* menu, gd::string caption, cocos2d::CCPoint pos) = win 0x1e5570;
	virtual void keyBackClicked() = mac 0x20cd80, ios 0xc4efc;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x20cc80, ios 0xc4e28;
	virtual void customSetup() = mac 0x20b300, ios 0xc3628;
	virtual void keyUp(cocos2d::enumKeyCodes p0) = mac 0x20cd60, ios 0xc4ef4;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4);
	void goEdit();
	bool init(bool p0);
	void musicSliderChanged(cocos2d::CCObject* p0);
	void onAutoCheckpoints(cocos2d::CCObject* p0);
	void onAutoRetry(cocos2d::CCObject* p0);
	void onHelp(cocos2d::CCObject* p0);
	void onNormalMode(cocos2d::CCObject* p0);
	void onPracticeMode(cocos2d::CCObject* p0);
	void onProgressBar(cocos2d::CCObject* p0);
	void onQuit(cocos2d::CCObject* p0);
	void onRecordReplays(cocos2d::CCObject* p0);
	void onRestart(cocos2d::CCObject* p0);
	void onResume(cocos2d::CCObject* p0);
	void onTime(cocos2d::CCObject* p0);
	void setupProgressBars();
	void sfxSliderChanged(cocos2d::CCObject* p0);
	void tryShowBanner(float p0);
	~PauseLayer() = mac 0x20b140, ios 0xc3520;

	bool m_unknown;
	bool m_unknown2;
}

class PlatformDownloadDelegate {
	virtual void downloadFinished(char const* p0) {}
	virtual void downloadFailed(char const* p0) {}
}

class PlatformToolbox {
	static void hideCursor() = mac 0x27c340;
	static void showCursor() = mac 0x27c360;
	static bool isControllerConnected() = mac 0x27d1b0;
}

class PlayLayer : GJBaseGameLayer, CCCircleWaveDelegate, DialogDelegate, CurrencyRewardDelegate {
	static PlayLayer* get() {
	    return GameManager::sharedState()->getPlayLayer();
	}

	void addCircle(CCCircleWave* p0) = mac 0x7e0f0;
	void addObject(GameObject* p0) = mac 0x70e50, win 0x2017e0;
	void addToGroupOld(GameObject* p0) = mac 0x77680;
	void addToSpeedObjects(GameObject* p0) = mac 0x7cfc0;
	void animateInDualGround(GameObject* p0, float p1, bool p2) = mac 0x7d710;
	void animateInGround(bool p0) = mac 0x7d9d0;
	void animateOutGround(bool p0) = mac 0x6f350;
	void animateOutGroundFinished() = mac 0x7de80;
	void applyEnterEffect(GameObject* p0) = mac 0x7c310;
	virtual void calculateColorValues(EffectGameObject* p0, EffectGameObject* p1, int p2, float p3, ColorActionSprite* p4, GJEffectManager* p5) = mac 0x7aa10, ios 0xb51b8;
	void cameraMoveX(float p0, float p1, float p2) = mac 0x7cbe0;
	void cameraMoveY(float p0, float p1, float p2) = mac 0x7cc60;
	void checkCollisions(PlayerObject* p0, float p1) = mac 0x78c90;
	void circleWaveWillBeRemoved(CCCircleWave* p0) = mac 0x7e110, ios 0xb7df4;
	void claimParticle(gd::string p0) = mac 0x76ba0;
	void clearPickedUpItems() = mac 0x7cfa0;
	void colorObject(int p0, cocos2d::ccColor3B p1) = mac 0x77810;
	void commitJumps() = mac 0x737e0;
	static PlayLayer* create(GJGameLevel* p0) = mac 0x6b590, win 0x1fb6d0;
	void createCheckpoint() = mac 0x7e470, win 0x20b050;
	void createObjectsFromSetup(gd::string p0) = mac 0x6d130;
	void createParticle(int p0, char const* p1, int p2, cocos2d::tCCPositionType p3) = mac 0x76800;
	void currencyWillExit(CurrencyRewardLayer* p0) = mac 0x7e070, ios 0xb7da4;
	void delayedResetLevel() = mac 0x7e050;
	void destroyPlayer(PlayerObject* p0, GameObject* p1) = mac 0x7ab80, win 0x20a1a0;
	void dialogClosed(DialogLayer* p0) = mac 0x7e0b0, ios 0xb7dc8;
	virtual void draw() = mac 0x7d160, win 0x208870, ios 0xb7260;
	void enterDualMode(GameObject* p0, bool p1) = mac 0x7d6a0;
	void exitAirMode() = mac 0x7dd40;
	void exitBirdMode(PlayerObject* p0) = mac 0x7dd80;
	void exitDartMode(PlayerObject* p0) = mac 0x7ddd0;
	void exitFlyMode(PlayerObject* p0) = mac 0x7dcf0;
	void exitRobotMode(PlayerObject* p0) = mac 0x7de20;
	void exitRollMode(PlayerObject* p0) = mac 0x7de60;
	void exitSpiderMode(PlayerObject* p0) = mac 0x7de40;
	void flipFinished() = mac 0x7e150;
	virtual void flipGravity(PlayerObject* p0, bool p1, bool p2) = mac 0x7cd10, ios 0xb6e84;
	void flipObjects() = mac 0x76130;
	void fullReset() = mac 0x7f8e0;
	void getLastCheckpoint() = mac 0x7f840;
	void getMaxPortalY() = mac 0x7b4e0;
	void getMinPortalY() = mac 0x7b550;
	void getObjectsState() = mac 0x7e9d0;
	void getOtherPlayer(PlayerObject* p0) = mac 0x7dcc0;
	void getParticleKey(int p0, char const* p1, int p2, cocos2d::tCCPositionType p3) = mac 0x764d0;
	void getParticleKey2(gd::string p0) = mac 0x767b0;
	void getRelativeMod(cocos2d::CCPoint p0, float p1, float p2, float p3) = mac 0x7c2a0;
	void getTempMilliTime() = mac 0x778e0;
	void gravityEffectFinished() = mac 0x7cec0;
	void hasItem(int p0) = mac 0x7cee0;
	void hasUniqueCoin(GameObject* p0) = mac 0x77510;
	void incrementJumps() = mac 0x7ff40;
	bool init(GJGameLevel* p0) = mac 0x6b5f0, win 0x1fb780;
	void isFlipping() = mac 0x76100;
	void levelComplete() = mac 0x72b80, win 0x1fd3d0;
	void lightningFlash(cocos2d::CCPoint p0, cocos2d::CCPoint p1, cocos2d::ccColor3B p2, float p3, float p4, int p5, bool p6, float p7) = mac 0x75cc0;
	void lightningFlash(cocos2d::CCPoint p0, cocos2d::ccColor3B p1) = mac 0x75bf0;
	void loadDefaultColors() = mac 0x6ef30;
	void loadFromCheckpoint(CheckpointObject* p0) = mac 0x7f000;
	void loadLastCheckpoint() = mac 0x7efc0;
	void loadSavedObjectsState(gd::string p0) = mac 0x7f3d0;
	void markCheckpoint() = mac 0x7ef60, win 0x25fb60;
	void moveCameraToPos(cocos2d::CCPoint p0) = mac 0x7c980;
	virtual void onEnterTransitionDidFinish() = mac 0x806e0, win 0x20dbd0, ios 0xb9724;
	virtual void onExit() = mac 0x80710, win 0x20d810, ios 0xb9754;
	void onQuit() = mac 0x72710, win 0x20d810;
	void optimizeColorGroups() = mac 0x6dad0;
	void optimizeOpacityGroups() = mac 0x6dc20;
	void optimizeSaveRequiredGroups() = mac 0x6dd70;
	void pauseGame(bool p0) = mac 0x802d0;
	void pickupItem(GameObject* p0) = mac 0x7c1d0;
	void playAnimationCommand(int p0, int p1) = mac 0x75930;
	void playEndAnimationToPos(cocos2d::CCPoint p0) = mac 0x759a0;
	void playExitDualEffect(PlayerObject* p0) = mac 0x7d1d0;
	void playFlashEffect(float p0, int p1, float p2) = mac 0x75e50;
	void playGravityEffect(bool p0) = mac 0x7b5a0;
	void playSpeedParticle(float p0) = mac 0x77030;
	void playerWillSwitchMode(PlayerObject* p0, GameObject* p1) = mac 0x7b820;
	void prepareSpawnObjects() = mac 0x7fc00;
	void processItems() = mac 0x735c0;
	void processLoadedMoveActions() = mac 0x7a7c0;
	void recordAction(bool p0, PlayerObject* p1) = mac 0x7e190;
	void registerActiveObject(GameObject* p0) = mac 0x77620;
	void registerStateObject(GameObject* p0) = mac 0x777b0;
	void removeAllObjects() = mac 0x727b0;
	void removeFromGroupOld(GameObject* p0) = mac 0x77750;
	void removeLastCheckpoint() = mac 0x7f870, win 0x20b830;
	void removePlayer2() = mac 0x7d630;
	void resetLevel() = mac 0x71c50, win 0x20bf00;
	void resume() = mac 0x80480;
	void resumeAndRestart() = mac 0x80400;
	void saveRecordAction(bool p0, PlayerObject* p1) = mac 0x78750;
	static cocos2d::CCScene* scene(GJGameLevel* p0) = mac 0x6b500, win 0x1fb690;
	void setupLevelStart(LevelSettingsObject* p0) = mac 0x6f560, win 0x1fb780;
	void setupReplay(gd::string p0) = mac 0x7e1e0;
	void shakeCamera(float p0, float p1, float p2) = mac 0x744a0, win 0x1ff210;
	void shouldBlend(int p0) = mac 0x771b0;
	void showCompleteEffect() = mac 0x738e0;
	void showCompleteText() = mac 0x73be0;
	void showEndLayer() = mac 0x74450;
	void showHint() = mac 0x7deb0;
	void showNewBest(bool p0, int p1, int p2, bool p3, bool p4, bool p5) = mac 0x74580;
	void showRetryLayer() = mac 0x75ba0;
	void showTwoPlayerGuide() = mac 0x6df00;
	void sortGroups() = mac 0x6d9e0;
	void spawnCircle() = mac 0x73820;
	void spawnFirework() = mac 0x74200;
	void spawnParticle(char const* p0, int p1, cocos2d::tCCPositionType p2, cocos2d::CCPoint p3) = mac 0x76330;
	void spawnPlayer2() = mac 0x7d170;
	void startGame() = mac 0x726b0;
	void startMusic() = mac 0x72910;
	void startRecording() = mac 0x7fec0;
	void startRecordingDelayed() = mac 0x7fed0;
	void stopCameraShake() = mac 0x75900;
	void stopRecording() = mac 0x6d090;
	void storeCheckpoint(CheckpointObject* p0) = mac 0x7ef10;
	void switchToFlyMode(PlayerObject* p0, GameObject* p1, bool p2, int p3) = mac 0x7baf0;
	void switchToRobotMode(PlayerObject* p0, GameObject* p1, bool p2) = mac 0x7bc80;
	void switchToRollMode(PlayerObject* p0, GameObject* p1, bool p2) = mac 0x7bbe0;
	void switchToSpiderMode(PlayerObject* p0, GameObject* p1, bool p2) = mac 0x7bd20;
	void timeForXPos(float p0) = mac 0x7d120, ios 0xb7238;
	void timeForXPos2(float p0, bool p1) = mac 0x293eb0, win 0x1fd3d0;
	void toggleBGEffectVisibility(bool p0) = mac 0x7fe80;
	void toggleDualMode(GameObject* p0, bool p1, PlayerObject* p2, bool p3) = mac 0x7bf90;
	void toggleFlipped(bool p0, bool p1) = mac 0x7bdc0;
	void toggleGhostEffect(int p0) = mac 0x7fe40;
	void toggleGlitter(bool p0) = mac 0x70e00;
	void togglePracticeMode(bool p0) = mac 0x7f9e0, win 0x20d0d0;
	void toggleProgressbar() = mac 0x6eeb0;
	void tryStartRecord() = mac 0x7fe00;
	void unclaimParticle(char const* p0, cocos2d::CCParticleSystemQuad* p1) = mac 0x76e00;
	void unregisterActiveObject(GameObject* p0) = mac 0x77660;
	void unregisterStateObject(GameObject* p0) = mac 0x777f0;
	virtual void update(float p0) = mac 0x77900, win 0x2029C0, ios 0xb2f08;
	void updateAttempts() = mac 0x7fcd0, win 0x20ced0;
	void updateCamera(float p0) = mac 0x6e2b0;
	virtual void updateColor(cocos2d::ccColor3B p0, float p1, int p2, bool p3, float p4, cocos2d::ccHSVValue p5, int p6, bool p7, int p8, EffectGameObject* p9) = mac 0x7c7f0, ios 0xb6b1c;
	void updateDualGround(PlayerObject* p0, int p1, bool p2) = mac 0x7caa0;
	void updateEffectPositions() = mac 0x7a6d0;
	void updateLevelColors() = mac 0x6f1e0;
	void updateMoveObjectsLastPosition() = mac 0x7a720;
	void updateProgressbar() = mac 0x6ed70;
	void updateReplay(float p0) = mac 0x78b60, win 0x20af40;
	void updateTimeMod(float p0, bool p1) = mac 0x786f0;
	virtual void updateTweenAction(float p0, char const* p1) = mac 0x7ffb0, win 0x20d1f0, ios 0xb90bc;
	void updateVisibility() = mac 0x6fb90, win 0x205460;
	void vfDChk() = mac 0x7fcb0;
	virtual void visit() = mac 0x75ef0, win 0x200020, ios 0xb1930;
	void visitWithColorFlash() = mac 0x761f0;
	void willSwitchToMode(int p0, PlayerObject* p1) = mac 0x7b9e0;
	void xPosForTime(float p0) = mac 0x7d140, ios 0xb724c;
	~PlayLayer() = mac 0x6b090, win 0x1fafc0, ios 0xa990c;
	void setStartPosObject(StartPosObject* p0);

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
	int m_gravityEffectStatus;
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
	~PlayerCheckpoint() = mac 0x80760, ios 0xb97a0;
	virtual bool init() = mac 0x807a0, ios 0xb97dc;
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
	~PlayerObject() = mac 0x217220, ios 0x13fcec;
	virtual void update(float p0) = mac 0x218bf0, ios 0x1416fc;
	virtual void setScaleX(float p0) = mac 0x22e7f0, ios 0x152374;
	virtual void setScaleY(float p0) = mac 0x22e830, ios 0x1523c4;
	virtual void setScale(float p0) = mac 0x22e870, ios 0x152414;
	virtual void setPosition(cocos2d::CCPoint const& p0) = mac 0x22c8b0, ios 0x150b14;
	virtual void setVisible(bool p0) = mac 0x22e8b0, win 0x1fa860, ios 0x152464;
	virtual void setRotation(float p0) = mac 0x22e6e0, ios 0x152270;
	virtual void setOpacity(unsigned char p0) = mac 0x22d400, ios 0x1514e8;
	virtual void setColor(cocos2d::ccColor3B const& p0) = mac 0x22cdf0, ios 0x150fb0;
	virtual void setFlipX(bool p0) = mac 0x22e720, win 0x1fa690, ios 0x1522c0;
	virtual void setFlipY(bool p0) = mac 0x22e7b0, win 0x1fa740, ios 0x152338;
	virtual void resetObject() = mac 0x223170, win 0x1eecd0, ios 0x148dec;
	virtual void getRealPosition() = mac 0x22d5f0, win 0x1f7e20, ios 0x1515ec;
	virtual void getOrientedBox() = mac 0x22dee0, win 0x1f95d0, ios 0x151c70;
	virtual void animationFinished(char const* p0) = mac 0x22e9d0, ios 0x1524b8;
	void activateStreak() = mac 0x21aef0, win 0x1f9080;
	void addAllParticles() = mac 0x2189b0;
	void addToTouchedRings(GameObject* p0) = mac 0x22b800;
	void boostPlayer(float p0) = mac 0x21d6b0;
	void bumpPlayer(float p0, int p1) = mac 0x22d890;
	void buttonDown(PlayerButton p0) = mac 0x22b7e0;
	void checkSnapJumpToObject(GameObject* p0) = mac 0x2217f0;
	void collidedWithObject(float p0, GameObject* p1) = mac 0x21d880;
	void collidedWithObject(float p0, GameObject* p1, cocos2d::CCRect p2) = mac 0x21f0b0;
	void collidedWithSlope(float p0, GameObject* p1, bool p2) = mac 0x21d8d0;
	void convertToClosestRotation(float p0) = mac 0x21c860;
	void copyAttributes(PlayerObject* p0) = mac 0x22dc70;
	static PlayerObject* create(int p0, int p1, cocos2d::CCLayer* p2) = mac 0x217260;
	void deactivateParticle() = mac 0x21a540;
	void deactivateStreak(bool p0) = mac 0x218b30;
	void fadeOutStreak2(float p0) = mac 0x225890, win 0x1f9110;
	void flashPlayer(float p0, float p1, cocos2d::ccColor3B p2, cocos2d::ccColor3B p3) = mac 0x221c80;
	void flipGravity(bool p0, bool p1) = mac 0x21c090, win 0x1f59d0;
	void flipMod() = mac 0x21a4c0;
	void getActiveMode() = mac 0x22b950;
	void getModifiedSlopeYVel() = mac 0x21bff0;
	void getOldPosition(float p0) = mac 0x21a830;
	void getSecondColor() = mac 0x22cee0;
	void gravityDown() = mac 0x22e930;
	void gravityUp() = mac 0x22e900;
	void hardFlipGravity() = mac 0x22b860;
	void hitGround(bool p0) = mac 0x220a30;
	void incrementJumps() = mac 0x21c050, win 0x1e9a20;
	bool init(int p0, int p1, cocos2d::CCLayer* p2) = mac 0x2172e0;
	void isBoostValid(float p0) = mac 0x21d650;
	void isFlying() = mac 0x21a4e0;
	void isSafeFlip(float p0) = mac 0x2209f0;
	void isSafeMode(float p0) = mac 0x2209b0;
	void isSafeSpiderFlip(float p0) = mac 0x221be0;
	void levelFlipFinished() = mac 0x21b060;
	void levelFlipping() = mac 0x21a510;
	void levelWillFlip() = mac 0x21b020;
	void loadFromCheckpoint(PlayerCheckpoint* p0) = mac 0x22e420;
	void lockPlayer() = mac 0x22d680;
	void logValues() = mac 0x221220;
	void modeDidChange() = mac 0x22bfd0;
	void placeStreakPoint() = mac 0x21af90;
	void playBurstEffect() = mac 0x21c780, win 0x1f6790;
	void playDeathEffect() = mac 0x225930, win 0x2efbe0;
	void playDynamicSpiderRun() = mac 0x222ec0;
	void playerDestroyed(bool p0) = mac 0x2256d0, win 0x1efaa0;
	bool playerIsFalling() = mac 0x21c730, win 0x1f5d60;
	void playerTeleported() = mac 0x22b840;
	void playingEndEffect() = mac 0x22d7e0;
	void postCollision(float p0) = mac 0x21cd10;
	void preCollision() = mac 0x21ccc0;
	void preSlopeCollision(float p0, GameObject* p1) = mac 0x21ec80;
	void propellPlayer(float p0) = mac 0x22d8e0;
	void pushButton(int p0) = mac 0x22aa00, win 0x1f4e40;
	void pushDown() = mac 0x22dbd0;
	void pushPlayer(float p0) = mac 0x22dbb0;
	void releaseButton(int p0) = mac 0x22b6f0, win 0x1f4f70;
	void removeAllParticles() = mac 0x218ac0;
	void removePendingCheckpoint() = mac 0x2237b0;
	void resetAllParticles() = mac 0x21adb0;
	void resetCollisionLog() = mac 0x21cc20;
	void resetPlayerIcon() = mac 0x22be00;
	void resetStateVariables() = mac 0x223760;
	void resetStreak() = mac 0x21ae10;
	void ringJump(GameObject* p0) = mac 0x22abf0, win 0x1f4ff0;
	void runBallRotation(float p0) = mac 0x21ca10;
	void runBallRotation2() = mac 0x21cb10;
	void runNormalRotation() = mac 0x21c960, win 0x1e9c50;
	void runRotateAction(bool p0) = mac 0x21c570;
	void saveToCheckpoint(PlayerCheckpoint* p0) = mac 0x22e2f0;
	void setSecondColor(cocos2d::ccColor3B const& p0) = mac 0x219610;
	void setupStreak() = mac 0x218720;
	void spawnCircle() = mac 0x225480;
	void spawnCircle2() = mac 0x2252a0;
	void spawnDualCircle() = mac 0x2255c0;
	void spawnFromPlayer(PlayerObject* p0) = mac 0x22dde0;
	void spawnPortalCircle(cocos2d::ccColor3B p0, float p1) = mac 0x225350;
	void spawnScaleCircle() = mac 0x2251b0;
	void specialGroundHit() = mac 0x22dbf0;
	void speedDown() = mac 0x22e970;
	void speedUp() = mac 0x22e950;
	void spiderTestJump(bool p0) = mac 0x21b160, win 0x1ed360;
	void startDashing(GameObject* p0) = mac 0x221d70;
	void stopBurstEffect() = mac 0x22c680;
	void stopDashing() = mac 0x222990;
	void stopRotation(bool p0) = mac 0x21c830;
	void storeCollision(bool p0, int p1) = mac 0x21cc60;
	void switchedToMode(GameObjectType p0) = mac 0x22b9a0;
	void testForMoving(float p0, GameObject* p1) = mac 0x21eb70;
	void toggleBirdMode(bool p0) = mac 0x224070, win 0x1f6050;
	void toggleDartMode(bool p0) = mac 0x2243f0, win 0x1f62c0;
	void toggleFlyMode(bool p0) = mac 0x223820, win 0x1f5e40;
	void toggleGhostEffect(GhostType p0) = mac 0x225000, win 0x1f8930;
	void togglePlayerScale(bool p0) = mac 0x224bd0, win 0x1f9640;
	void toggleRobotMode(bool p0) = mac 0x223c70, win 0x1f6a10;
	void toggleRollMode(bool p0) = mac 0x223b20, win 0x1f68e0;
	void toggleSpiderMode(bool p0) = mac 0x224830, win 0x1f94d1;
	void toggleVisibility(bool p0) = mac 0x21abf0;
	void touchedObject(GameObject* p0) = mac 0x22e660;
	void tryPlaceCheckpoint() = mac 0x21a950;
	void updateCheckpointMode(bool p0) = mac 0x218980;
	void updateCheckpointTest() = mac 0x21a890;
	void updateCollide(bool p0, int p1) = mac 0x220f10;
	void updateCollideBottom(float p0, int p1) = mac 0x221790;
	void updateCollideTop(float p0, int p1) = mac 0x221c20;
	void updateDashAnimation() = mac 0x21a570;
	void updateDashArt() = mac 0x222520;
	void updateGlowColor() = mac 0x22cf10;
	void updateJump(float p0) = mac 0x219680;
	void updateJumpVariables() = mac 0x21a740;
	void updatePlayerBirdFrame(int p0) = mac 0x22bfe0;
	void updatePlayerDartFrame(int p0) = mac 0x22c260;
	void updatePlayerFrame(int p0) = mac 0x22c470;
	void updatePlayerGlow() = mac 0x22bc50;
	void updatePlayerRobotFrame(int p0) = mac 0x22d620;
	void updatePlayerRollFrame(int p0) = mac 0x22c6a0;
	void updatePlayerScale() = mac 0x22b8b0;
	void updatePlayerShipFrame(int p0) = mac 0x22ba40;
	void updatePlayerSpiderFrame(int p0) = mac 0x22d650;
	void updatePlayerSpriteExtra(gd::string p0) = mac 0x218440;
	void updateRobotAnimationSpeed() = mac 0x22df40;
	void updateRotation(float p0) = mac 0x2214b0;
	void updateRotation(float p0, float p1) = mac 0x221230;
	void updateShipRotation(float p0) = mac 0x221310;
	void updateShipSpriteExtra(gd::string p0) = mac 0x218510;
	void updateSlopeRotation(float p0) = mac 0x221030;
	void updateSlopeYVelocity(float p0) = mac 0x22e920;
	void updateSpecial(float p0) = mac 0x21a790;
	void updateStateVariables() = mac 0x21a770;
	void updateTimeMod(float p0) = mac 0x2185e0, win 0x1f94e0;
	void usingWallLimitedMode() = mac 0x22df00;
	void yStartDown() = mac 0x22e9b0;
	void yStartUp() = mac 0x22e990;
	void runRotateAction() = win 0x1e9bf0;
	void runBallRotation() = win 0x1e9d10;
	void pushButton(PlayerButton p0);
	void releaseButton(PlayerButton p0);
	void resetTouchedRings();
	void setPortalP(cocos2d::CCPoint p0);

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
	bool init(cocos2d::CCPoint p0);
	~PointNode();

	cocos2d::CCPoint m_point;
}

class PriceLabel : cocos2d::CCNode {
	void create(int p0);
	bool init(int p0);
	void setColor(cocos2d::ccColor3B p0);
	void setOpacity(float p0);
	void setPrice(int p0);
	~PriceLabel();
}

class ProfilePage : FLAlertLayer, FLAlertLayerProtocol, LevelCommentDelegate, CommentUploadDelegate, UserInfoDelegate, UploadActionDelegate, UploadPopupDelegate, LeaderboardManagerDelegate {
	static ProfilePage* create(int accountID, bool idk) = mac 0x45eed0, win 0x20ee50;
	void getUserInfoFailed(int p0) = mac 0x463fb0, win 0x2133e0, ios 0xfe324;
	bool init(int accountID, bool idk) = win 0x20ef00;
	void onMyLevels(cocos2d::CCObject* p0) = win 0x211bb0;
	void onUpdate(cocos2d::CCObject* p0) = win 0x20fa20;
	void loadPageFromUserInfo(GJUserScore* score) = win 0x210040;
	virtual void registerWithTouchDispatcher() = mac 0x464a60, ios 0xfeca0;
	virtual void keyBackClicked() = mac 0x4637e0, ios 0xfdca0;
	virtual void show() = mac 0x4639c0, ios 0xfde5c;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x463340, ios 0xfd7f4;
	virtual void updateUserScoreFinished() = mac 0x463850, ios 0xfdd1c;
	virtual void updateUserScoreFailed() = mac 0x463920, ios 0xfddb4;
	virtual void getUserInfoFinished(GJUserScore* p0) = mac 0x463cd0, ios 0xfe00c;
	virtual void userInfoChanged(GJUserScore* p0) = mac 0x464070, ios 0xfe398;
	virtual void loadCommentsFinished(cocos2d::CCArray* p0, char const* p1) = mac 0x464250, ios 0xfe594;
	virtual void loadCommentsFailed(char const* p0) = mac 0x464340, ios 0xfe618;
	virtual void setupPageInfo(gd::string p0, char const* p1) = mac 0x464410, ios 0xfe688;
	virtual void commentUploadFinished(int p0) = mac 0x464770, ios 0xfea14;
	virtual void commentUploadFailed(int p0, CommentError p1) = mac 0x464800, ios 0xfea6c;
	virtual void commentDeleteFailed(int p0, int p1) = mac 0x4648e0, ios 0xfeb44;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x463510, ios 0xfd9b4;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x463570, ios 0xfda24;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x463710, ios 0xfdbf4;
	void blockUser();
	void isCorrect(char const* p0);
	void loadPage(int p0);
	void onBlockUser(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onComment(cocos2d::CCObject* p0);
	void onCommentHistory(cocos2d::CCObject* p0);
	void onFollow(cocos2d::CCObject* p0);
	void onFriend(cocos2d::CCObject* p0);
	void onFriends(cocos2d::CCObject* p0);
	void onMessages(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void onRequests(cocos2d::CCObject* p0);
	void onSendMessage(cocos2d::CCObject* p0);
	void onSettings(cocos2d::CCObject* p0);
	void onTwitch(cocos2d::CCObject* p0);
	void onTwitter(cocos2d::CCObject* p0);
	void onYouTube(cocos2d::CCObject* p0);
	void setupComments();
	void setupCommentsBrowser(cocos2d::CCArray* p0);
	void showNoAccountError();
	void toggleMainPageVisibility(bool p0);
	void updateLevelsLabel();
	void updatePageArrows();
	~ProfilePage() = mac 0x45ee30, ios 0xf9d70;

	GJUserScore* m_score;
	int m_accountID;
	PAD = win 0x38, android 0x24;
	cocos2d::CCArray* m_buttons;
}

class PromoInterstitial : FLAlertLayer {
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x169e80;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x16a0c0;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x169fa0;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x16a040;
	virtual void keyBackClicked() = mac 0x169cf0;
	virtual void show() = mac 0x169cc0;
	void create(bool p0);
	bool init(bool p0);
	void onClick(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void setup();
	~PromoInterstitial() = mac 0x16a270;
}

class PulseEffectAction : cocos2d::CCNode {
	static PulseEffectAction* createFromString(gd::string p0) = mac 0x179e90;
	void getSaveString() = mac 0x17a850;
	void create(float p0, float p1, float p2, PulseEffectType p3, int p4, cocos2d::ccColor3B p5, cocos2d::ccHSVValue p6, int p7, bool p8, bool p9);
	bool init(float p0, float p1, float p2, PulseEffectType p3, int p4, cocos2d::ccColor3B p5, cocos2d::ccHSVValue p6, int p7, bool p8, bool p9);
	void isFinished();
	void step(float p0);
	void valueForDelta(float p0, float p1, float p2, float p3);
	~PulseEffectAction();

	int group = mac 0x130;
}

class PurchaseItemPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x1a2fc0, ios 0xcb994;
	void create(GJStoreItem* p0);
	bool init(GJStoreItem* p0);
	void onClose(cocos2d::CCObject* p0);
	void onPurchase(cocos2d::CCObject* p0);
	~PurchaseItemPopup() = mac 0x1a2460, ios 0xcb0a0;
}

class RateDemonLayer : FLAlertLayer, UploadPopupDelegate, UploadActionDelegate {
	virtual void keyBackClicked() = mac 0x12ec20, ios 0x2e2820;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x12e960, ios 0x2e2668;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x12ea40, ios 0x2e2714;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x12eb40, ios 0x2e27d8;
	void create(int p0);
	bool init(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onRate(cocos2d::CCObject* p0);
	void selectRating(cocos2d::CCObject* p0);
	~RateDemonLayer() = mac 0x12dfc0, ios 0x2e1e94;
}

class RateLevelDelegate {
	virtual void rateLevelClosed() {}
}

class RateLevelLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x2e2250, ios 0x384d8;
	void create(int p0);
	bool init(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onRate(cocos2d::CCObject* p0);
	void selectRating(cocos2d::CCObject* p0);
	~RateLevelLayer() = mac 0x2e1a60, ios 0x37e64;
}

class RateStarsLayer : FLAlertLayer, UploadPopupDelegate, UploadActionDelegate {
	virtual void keyBackClicked() = mac 0x136ec0, ios 0x3f5d8;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x136c10, ios 0x3f430;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x136cf0, ios 0x3f4d4;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x136de0, ios 0x3f590;
	void create(int p0, bool p1);
	void getStarsButton(int p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCMenu* p2, float p3);
	bool init(int p0, bool p1);
	void onClose(cocos2d::CCObject* p0);
	void onRate(cocos2d::CCObject* p0);
	void onToggleCoins(cocos2d::CCObject* p0);
	void onToggleFeature(cocos2d::CCObject* p0);
	void selectRating(cocos2d::CCObject* p0);
	~RateStarsLayer() = mac 0x135db0, ios 0x3e908;
}

class RetryLevelLayer : GJDropDownLayer {
	static RetryLevelLayer* create() = mac 0x28dd60;
	virtual void keyBackClicked() = mac 0x28ed10, ios 0x116b68;
	virtual void keyDown(cocos2d::enumKeyCodes p0) = mac 0x28ec30, ios 0x116b18;
	virtual void customSetup() = mac 0x28de80, ios 0x115d48;
	virtual void showLayer(bool p0) = mac 0x28eaf0, ios 0x1169bc;
	virtual void enterAnimFinished() = mac 0x28ebf0, ios 0x116ad4;
	virtual void keyUp(cocos2d::enumKeyCodes p0) = mac 0x28ecf0, ios 0x116b60;
	void getEndText();
	void onEveryplay(cocos2d::CCObject* p0);
	void onMenu(cocos2d::CCObject* p0);
	void onReplay(cocos2d::CCObject* p0);
	void setupLastProgress();
	~RetryLevelLayer() = mac 0x28dde0, ios 0x115cfc;
}

class RewardUnlockLayer : FLAlertLayer, CurrencyRewardDelegate {
	virtual void keyBackClicked() = mac 0xf81b0, ios 0x127258;
	virtual void currencyWillExit(CurrencyRewardLayer* p0) = mac 0xf8170, ios 0x127230;
	void connectionTimeout();
	void create(int p0, RewardsPage* p1);
	bool init(int p0, RewardsPage* p1);
	void labelEnterFinishedO(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void playDropSound();
	void playLabelEffect(int p0, int p1, cocos2d::CCSprite* p2, cocos2d::CCPoint p3, float p4);
	void playRewardEffect();
	void readyToCollect(GJRewardItem* p0);
	void showCloseButton();
	void showCollectReward(GJRewardItem* p0);
	void step2();
	void step3();
	~RewardUnlockLayer() = mac 0xf51a0, ios 0x124548;
}

class RewardsPage : FLAlertLayer, FLAlertLayerProtocol, GJRewardDelegate {
	virtual bool init() = mac 0xf3800, ios 0x123118;
	virtual void registerWithTouchDispatcher() = mac 0xf5040, ios 0x124460;
	virtual void keyBackClicked() = mac 0xf4fd0, ios 0x1243e4;
	virtual void show() = mac 0xf4e70, ios 0x12429c;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0xf4fb0, ios 0x1243dc;
	virtual void rewardsStatusFinished(int p0) = mac 0xf4ae0, ios 0x124050;
	virtual void rewardsStatusFailed() = mac 0xf4d70, ios 0x124208;
	void create();
	void getRewardFrame(int p0, int p1);
	void onClose(cocos2d::CCObject* p0);
	void onFreeStuff(cocos2d::CCObject* p0);
	void onReward(cocos2d::CCObject* p0);
	void tryGetRewards();
	void unlockLayerClosed(RewardUnlockLayer* p0);
	void updateTimers(float p0);
	~RewardsPage() = mac 0xf35a0, ios 0x122ff8;
}

class RingObject : EffectGameObject {
	virtual void setScale(float p0) = mac 0xc6fb0, ios 0x276210;
	virtual void setRotation(float p0) = mac 0xc7010, ios 0x276260;
	virtual void resetObject() = mac 0xc6f90, ios 0x2761e4;
	virtual void customObjectSetup(gd::map<gd::string, gd::string>& p0) = mac 0xc7040, ios 0x276290;
	virtual gd::string getSaveString() = mac 0xc73a0, ios 0x27638c;
	virtual void setRScale(float p0) = mac 0xc6fe0, ios 0x276238;
	virtual void triggerActivated(float p0) = mac 0xc6e10, ios 0x276094;
	virtual void powerOnObject() = mac 0xc6e20, ios 0x2760a0;
	virtual void powerOffObject() = mac 0xc6f70, ios 0x2761d4;
	void create(char const* p0);
	bool init(char const* p0);
	void spawnCircle();
	~RingObject() = mac 0xdc250;
}

class ScrollingLayer : cocos2d::CCLayerColor {
	static ScrollingLayer* create(cocos2d::CCSize p0, cocos2d::CCPoint p1, float p2) = mac 0x41a900;
	virtual void draw() = mac 0x41abc0, ios 0x119758;
	virtual void visit() = mac 0x41acb0, ios 0x119814;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x41ae10, ios 0x1198a8;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x41afb0, ios 0x119968;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x41b0d0, ios 0x119a64;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x41b110, ios 0x119a9c;
	void getViewRect();
	bool init(cocos2d::CCSize p0, cocos2d::CCPoint p1, float p2);
	void setStartOffset(cocos2d::CCPoint p0);
	~ScrollingLayer() = mac 0x41b150, ios 0xf54ac;
}

class SearchButton : cocos2d::CCSprite {
	void create(char const* p0, char const* p1, float p2, char const* p3);
	bool init(char const* p0, char const* p1, float p2, char const* p3);
	~SearchButton();
}

class SecretLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol {
	virtual bool init() = mac 0x363340, ios 0x29f518;
	virtual void keyBackClicked() = mac 0x368450, ios 0x2a3110;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0x366d30, ios 0x2a20a0;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x366f30, ios 0x2a2188;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x367050, ios 0x2a2208;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x366d10, ios 0x2a2098;
	void create();
	void getBasicMessage();
	void getMessage();
	void getThreadMessage();
	void nodeWithTag(int p0);
	void onBack(cocos2d::CCObject* p0);
	void onSubmit(cocos2d::CCObject* p0);
	void playCoinEffect();
	void scene();
	void selectAThread();
	void updateMessageLabel(gd::string p0);
	void updateSearchLabel(char const* p0);
	~SecretLayer() = mac 0x362f30, ios 0x29f3bc;
}

class SecretLayer2 : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DialogDelegate {
	virtual bool init() = mac 0x25fe70, ios 0x227d08;
	virtual void onExit() = mac 0x266b30, ios 0x22d6bc;
	virtual void keyBackClicked() = mac 0x266a30, ios 0x22d6b0;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0x265880, ios 0x22c684;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x265a80, ios 0x22c76c;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x265ba0, ios 0x22c7ec;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x265860, ios 0x22c67c;
	virtual void dialogClosed(DialogLayer* p0) = mac 0x264880, ios 0x22b3f0;
	void create();
	void getBasicMessage();
	void getErrorMessage();
	void getMessage();
	void getThreadMessage();
	void nodeWithTag(int p0);
	void onBack(cocos2d::CCObject* p0);
	void onDoor(cocos2d::CCObject* p0);
	void onSecretLevel(cocos2d::CCObject* p0);
	void onSubmit(cocos2d::CCObject* p0);
	void playCoinEffect();
	void scene();
	void selectAThread();
	void showCompletedLevel();
	void showSecretLevel();
	void updateMessageLabel(gd::string p0);
	void updateSearchLabel(char const* p0);
	~SecretLayer2() = mac 0x25f9e0, ios 0x227c04;
}

class SecretLayer3 : cocos2d::CCLayer, DialogDelegate {
	virtual bool init() = mac 0x267040, ios 0x22d8d4;
	virtual void onExit() = mac 0x26c270, ios 0x231e5c;
	virtual void keyBackClicked() = mac 0x26c240, ios 0x231e50;
	virtual void dialogClosed(DialogLayer* p0) = mac 0x26bf10, ios 0x231bdc;
	void animateEyes();
	void create();
	void firstInteractionStep1();
	void firstInteractionStep2();
	void firstInteractionStep3();
	void firstInteractionStep4();
	void onBack(cocos2d::CCObject* p0);
	void onChest01(cocos2d::CCObject* p0);
	void onChest02(cocos2d::CCObject* p0);
	void onClick(cocos2d::CCObject* p0);
	void revealStep1();
	void revealStep2();
	void revealStep3();
	void revealStep4();
	void revealStep5();
	void scene();
	void showUnlockDialog();
	~SecretLayer3() = mac 0x266cf0, ios 0x22d7d8;
}

class SecretLayer4 : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DialogDelegate {
	virtual bool init() = mac 0x1ed640, ios 0x25dd84;
	virtual void onExit() = mac 0x1f2220, ios 0x261068;
	virtual void keyBackClicked() = mac 0x1f2120, ios 0x26105c;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0x1f0ff0, ios 0x260558;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x1f11f0, ios 0x260640;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x1f1310, ios 0x2606c0;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x1f0fd0, ios 0x260550;
	virtual void dialogClosed(DialogLayer* p0) = mac 0x1f0a90, ios 0x25fc10;
	void create();
	void getBasicMessage();
	void getErrorMessage();
	void getMessage();
	void getThreadMessage();
	void nodeWithTag(int p0);
	void onBack(cocos2d::CCObject* p0);
	void onChest01(cocos2d::CCObject* p0);
	void onSubmit(cocos2d::CCObject* p0);
	void playCoinEffect();
	void scene();
	void selectAThread();
	void showDialog(int p0);
	void showFirstDialog();
	void updateMessageLabel(gd::string p0);
	void updateSearchLabel(char const* p0);
	~SecretLayer4() = mac 0x1ed200, ios 0x25dc80;
}

class SecretNumberLayer : cocos2d::CCLayer {
	virtual bool init() = mac 0x266c30, ios 0x22d758;
	void create();
	void playNumberEffect(int p0);
	~SecretNumberLayer() = mac 0x266b90, ios 0x22d70c;
}

class SelectArtDelegate {
	virtual void selectArtClosed(SelectArtLayer* p0) {}
}

class SelectArtLayer : FLAlertLayer {
	static SelectArtLayer* create(SelectArtType type) = win 0x174b00;
	virtual void keyBackClicked() = mac 0xad440, ios 0x275020;
	bool init(SelectArtType p0);
	void onClose(cocos2d::CCObject* p0);
	void onSelectCustom(cocos2d::CCObject* p0);
	void selectArt(cocos2d::CCObject* p0);
	void updateSelectedCustom(int p0);
	~SelectArtLayer() = mac 0xac9c0, ios 0x2746d0;
}

class SelectFontLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x1445f0, ios 0x1ffcc8;
	void create(LevelEditorLayer* p0);
	bool init(LevelEditorLayer* p0);
	void onChangeFont(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void updateFontLabel();
	~SelectFontLayer() = mac 0x143b80, ios 0x1ff404;
}

class SetFolderPopup : SetIDPopup, SetTextPopupDelegate {
	virtual void valueChanged() = mac 0x155d40, ios 0x20da70;
	virtual void setTextPopupClosed(SetTextPopup* p0, gd::string p1) = mac 0x1561b0, ios 0x20ddb0;
	void create(int p0, bool p1, gd::string p2);
	bool init(int p0, bool p1, gd::string p2);
	void onSetFolderName(cocos2d::CCObject* p0);
	~SetFolderPopup() = mac 0x155480, ios 0x20d4c4;
}

class SetGroupIDLayer : FLAlertLayer, TextInputDelegate {
	void onNextGroupID1(cocos2d::CCObject* p0) = mac 0x1967a0;
	void textChanged(CCTextInputNode* p0) = mac 0x197af0, ios 0x264a4;
	void updateGroupIDLabel() = mac 0x197260, win 0x22e450;
	~SetGroupIDLayer() = mac 0x194410, ios 0x23a4c;
	void updateEditorLayerID() = win 0x22e0b0;
	void updateEditorLayerID2() = win 0x22e110;
	void updateZOrder() = win 0x22e3d0;
	virtual void keyBackClicked() = mac 0x198050, ios 0x26940;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x197ab0, ios 0x26488;
	void addGroupID(int p0);
	void callRemoveFromGroup(float p0);
	void create(GameObject* p0, cocos2d::CCArray* p1);
	void determineStartValues();
	bool init(GameObject* p0, cocos2d::CCArray* p1);
	void onAddGroup(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onCopy(cocos2d::CCObject* p0);
	void onDontEnter(cocos2d::CCObject* p0);
	void onDontFade(cocos2d::CCObject* p0);
	void onEditorLayer(cocos2d::CCObject* p0);
	void onEditorLayer2(cocos2d::CCObject* p0);
	void onGroupID(cocos2d::CCObject* p0);
	void onHighDetail(cocos2d::CCObject* p0);
	void onPaste(cocos2d::CCObject* p0);
	void onRemoveFromGroup(cocos2d::CCObject* p0);
	void onSmoothEase(cocos2d::CCObject* p0);
	void onToggleGroupParent(cocos2d::CCObject* p0);
	void onZLayer(cocos2d::CCObject* p0);
	void onZLayerShift(cocos2d::CCObject* p0);
	void onZOrder(cocos2d::CCObject* p0);
	void removeGroupID(int p0);
	void updateEditorLabel();
	void updateEditorLabel2();
	void updateGroupIDButtons();
	void updateZLayerButtons();
	void updateZOrderLabel();

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

class SetIDLayer : FLAlertLayer {
	static SetIDLayer* create(GameObject* p0) = mac 0x168f20;
	virtual void keyBackClicked() = mac 0x169670, ios 0x2dcd70;
	bool init(GameObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onDown(cocos2d::CCObject* p0);
	void onUp(cocos2d::CCObject* p0);
	void updateID();
	~SetIDLayer() = mac 0x168c80, ios 0x2dc714;
}

class SetIDPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x155310, ios 0x20d4b8;
	virtual void show() = mac 0x1552c0, ios 0x20d46c;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x155140, ios 0x20d3ac;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x155180, ios 0x20d3c8;
	virtual void valueChanged() = mac 0x1552b0, ios 0x20d468;
	void create(int p0, int p1, int p2, gd::string p3, gd::string p4, bool p5, int p6);
	bool init(int p0, int p1, int p2, gd::string p3, gd::string p4, bool p5, int p6);
	void onCancel(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onItemIDArrow(cocos2d::CCObject* p0);
	void onResetValue(cocos2d::CCObject* p0);
	void updateTextInputLabel();
	~SetIDPopup() = mac 0x154210, ios 0x20c720;
}

class SetIDPopupDelegate {
	virtual void setIDPopupClosed(SetIDPopup* p0, int p1) {}
}

class SetItemIDLayer : FLAlertLayer, TextInputDelegate {
	static SetItemIDLayer* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x5a830;
	virtual void keyBackClicked() = mac 0x5bb90, ios 0x2d5bec;
	virtual void show() = mac 0x5bb40, ios 0x2d5ba0;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x5b9e0, ios 0x2d5b04;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x5ba20, ios 0x2d5b20;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x5bca0, ios 0x2d5bf8;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x5bd60, ios 0x2d5c64;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onItemIDArrow(cocos2d::CCObject* p0);
	void updateEditorLabel();
	void updateItemID();
	void updateTextInputLabel();
	~SetItemIDLayer() = mac 0x5a790, ios 0x2d4cac;
}

class SetTargetIDLayer : FLAlertLayer, TextInputDelegate {
	static SetTargetIDLayer* create(EffectGameObject* p0, cocos2d::CCArray* p1, gd::string p2) = mac 0x159d20;
	void onTargetIDArrow(cocos2d::CCObject* p0) = mac 0x15aed0;
	void textChanged(CCTextInputNode* p0) = mac 0x15b6c0, ios 0x13c57c;
	void updateTargetID() = mac 0x15b4a0;
	virtual void keyBackClicked() = mac 0x15b990, ios 0x13c724;
	virtual void show() = mac 0x15b940, ios 0x13c6d8;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x15b680, ios 0x13c560;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x15baa0, ios 0x13c730;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x15bb60, ios 0x13c79c;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1, gd::string p2);
	void onClose(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTextInputLabel();
	void updateTouchTriggered();
	~SetTargetIDLayer() = mac 0x159c80, ios 0x13b2ac;
}

class SetTextPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x157050, ios 0x20e89c;
	virtual void show() = mac 0x157000, ios 0x20e850;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x156ef0, ios 0x20e7ac;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x156f30, ios 0x20e7c8;
	void create(gd::string p0, gd::string p1, int p2, gd::string p3, gd::string p4, bool p5);
	bool init(gd::string p0, gd::string p1, int p2, gd::string p3, gd::string p4, bool p5);
	void onCancel(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onResetValue(cocos2d::CCObject* p0);
	void updateTextInputLabel();
	~SetTextPopup() = mac 0x1563f0, ios 0x20df10;
}

class SetTextPopupDelegate {
	virtual void setTextPopupClosed(SetTextPopup* p0, gd::string p1) {}
}

class SetupAnimSettingsPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x599f0, ios 0x210a38;
	virtual void show() = mac 0x599a0, ios 0x2109ec;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x597b0, ios 0x2108d4;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x597f0, ios 0x2108f0;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x59ae0, ios 0x210a44;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x59ba0, ios 0x210ab0;
	void create(EffectGameObject* p0, cocos2d::CCArray* p1);
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onUseAnimOffset(cocos2d::CCObject* p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void updateAnimSpeedMod();
	void updateRotationInputLabel();
	~SetupAnimSettingsPopup() = mac 0x585c0, ios 0x20f9e8;
}

class SetupAnimationPopup : FLAlertLayer, TextInputDelegate {
	static SetupAnimationPopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x208b70;
	void onTargetIDArrow(cocos2d::CCObject* p0) = mac 0x209fc0;
	void textChanged(CCTextInputNode* p0) = mac 0x20ab30, ios 0x2a4dd8;
	void updateTargetID() = mac 0x20a910;
	virtual void keyBackClicked() = mac 0x20ade0, ios 0x2a4efc;
	virtual void show() = mac 0x20ad90, ios 0x2a4eb0;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x20aaf0, ios 0x2a4dbc;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x20af30, ios 0x2a4f08;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x20aff0, ios 0x2a4f74;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onAnimationIDArrow(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void updateAnimationID();
	void updateAnimationTextInputLabel();
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTextInputLabel();
	void updateTouchTriggered();
	~SetupAnimationPopup() = mac 0x208ad0, ios 0x2a3380;
}

class SetupCollisionTriggerPopup : FLAlertLayer, TextInputDelegate {
	static SetupCollisionTriggerPopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x1d6120;
	void onTargetIDArrow(cocos2d::CCObject* p0) = mac 0x1d77b0;
	void textChanged(CCTextInputNode* p0) = mac 0x1d84d0, ios 0x154b4c;
	void updateTargetID() = mac 0x1d82b0;
	virtual void keyBackClicked() = mac 0x1d8880, ios 0x154ccc;
	virtual void show() = mac 0x1d8830, ios 0x154c80;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x1d8490, ios 0x154b30;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x1d89f0, ios 0x154cd8;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x1d8ab0, ios 0x154d44;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onActivateOnExit(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onEnableGroup(cocos2d::CCObject* p0);
	void onItemID2Arrow(cocos2d::CCObject* p0);
	void onItemIDArrow(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void updateEditorLabel();
	void updateItemID();
	void updateItemID2();
	void updateItemID2InputLabel();
	void updateItemIDInputLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetIDInputLabel();
	void updateTouchTriggered();
	~SetupCollisionTriggerPopup() = mac 0x1d6080, ios 0x152e2c;
}

class SetupCountTriggerPopup : FLAlertLayer, TextInputDelegate {
	static SetupCountTriggerPopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x15c6c0;
	void onTargetIDArrow(cocos2d::CCObject* p0) = mac 0x15dd40;
	void textChanged(CCTextInputNode* p0) = mac 0x15e9a0, ios 0x90760;
	void updateTargetID() = mac 0x15e8a0;
	virtual void keyBackClicked() = mac 0x15ec70, ios 0x908e0;
	virtual void show() = mac 0x15ec20, ios 0x90894;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x15e960, ios 0x90744;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x15ede0, ios 0x908ec;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x15eea0, ios 0x90958;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onEnableGroup(cocos2d::CCObject* p0);
	void onItemIDArrow(cocos2d::CCObject* p0);
	void onMultiActivate(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTargetCountArrow(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void updateCountTextInputLabel();
	void updateEditorLabel();
	void updateItemID();
	void updateItemIDInputLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetCount();
	void updateTargetIDInputLabel();
	void updateTouchTriggered();
	~SetupCountTriggerPopup() = mac 0x15c620, ios 0x8ea5c;
}

class SetupInstantCountPopup : FLAlertLayer, TextInputDelegate {
	static SetupInstantCountPopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x352c10;
	void onTargetIDArrow(cocos2d::CCObject* p0) = mac 0x354520;
	void textChanged(CCTextInputNode* p0) = mac 0x355270, ios 0x11e3fc;
	void updateTargetID() = mac 0x355170;
	virtual void keyBackClicked() = mac 0x355600, ios 0x11e57c;
	virtual void show() = mac 0x3555b0, ios 0x11e530;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x355230, ios 0x11e3e0;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x355770, ios 0x11e588;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x355830, ios 0x11e5f4;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onCountType(cocos2d::CCObject* p0);
	void onEnableGroup(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTargetCountArrow(cocos2d::CCObject* p0);
	void onTargetID2Arrow(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void updateCountTextInputLabel();
	void updateEditorLabel();
	void updateItemID();
	void updateItemIDInputLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetCount();
	void updateTargetIDInputLabel();
	void updateTouchTriggered();
	~SetupInstantCountPopup() = mac 0x352b70, ios 0x11c4d0;
}

class SetupInteractObjectPopup : FLAlertLayer, TextInputDelegate {
	static SetupInteractObjectPopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x29a400;
	void onTargetIDArrow(cocos2d::CCObject* p0) = mac 0x29bbc0;
	void textChanged(CCTextInputNode* p0) = mac 0x29c2b0, ios 0x2d0238;
	void updateTargetID() = mac 0x29c120;
	virtual void keyBackClicked() = mac 0x29c550, ios 0x2d0358;
	virtual void show() = mac 0x29c500, ios 0x2d030c;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x29c270, ios 0x2d021c;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x29c6a0, ios 0x2d0364;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x29c760, ios 0x2d03d0;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onEnableGroup(cocos2d::CCObject* p0);
	void onItemIDArrow(cocos2d::CCObject* p0);
	void onNextItemID(cocos2d::CCObject* p0);
	void onSubstractCount(cocos2d::CCObject* p0);
	void onToggleItem(cocos2d::CCObject* p0);
	void onToggleTrigger(cocos2d::CCObject* p0);
	void updateItemID();
	void updateItemIDInputLabel();
	void updatePickupType();
	void updateState();
	void updateTextInputLabel();
	~SetupInteractObjectPopup() = mac 0x29a360, ios 0x2ce8dc;
}

class SetupObjectTogglePopup : FLAlertLayer, TextInputDelegate {
	static SetupObjectTogglePopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x1c0860;
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x1c0a40;
	void onTargetIDArrow(cocos2d::CCObject* p0) = mac 0x1c1c40;
	void textChanged(CCTextInputNode* p0) = mac 0x1c2660, ios 0x103b34;
	void updateTargetID() = mac 0x1c2440;
	virtual void keyBackClicked() = mac 0x1c27f0, ios 0x103c00;
	virtual void show() = mac 0x1c27a0, ios 0x103bb4;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x1c2620, ios 0x103b18;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x1c2900, ios 0x103c0c;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x1c29c0, ios 0x103c78;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	void onClose(cocos2d::CCObject* p0);
	void onEnableGroup(cocos2d::CCObject* p0);
	void onMultiActivate(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTextInputLabel();
	void updateTouchTriggered();
	~SetupObjectTogglePopup() = mac 0x1c07c0, ios 0x1022e8;
}

class SetupOpacityPopup : FLAlertLayer, TextInputDelegate {
	static SetupOpacityPopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x32b70;
	void onTargetIDArrow(cocos2d::CCObject* p0) = mac 0x340a0;
	void textChanged(CCTextInputNode* p0) = mac 0x34a60, ios 0x18078;
	void updateTargetID() = mac 0x34760;
	virtual void keyBackClicked() = mac 0x34bf0, ios 0x18144;
	virtual void show() = mac 0x34ba0, ios 0x180f8;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x34a20, ios 0x1805c;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x34e50, ios 0x1822c;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x34f10, ios 0x18298;
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void updateDurLabel();
	void updateDuration();
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateOpacity();
	void updateOpacityLabel();
	void updateSpawnedByTrigger();
	void updateTextInputLabel();
	void updateTouchTriggered();
	~SetupOpacityPopup() = mac 0x32ad0, ios 0x1688c;
}

class SetupPickupTriggerPopup : FLAlertLayer, TextInputDelegate {
	static SetupPickupTriggerPopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x35e70;
	void onItemIDArrow(cocos2d::CCObject* p0) = mac 0x37100;
	void onNextItemID(cocos2d::CCObject* p0) = mac 0x37260;
	void textChanged(CCTextInputNode* p0) = mac 0x37ca0, ios 0x28eac;
	void updateItemID() = mac 0x37ab0;
	virtual void keyBackClicked() = mac 0x37f00, ios 0x28fec;
	virtual void show() = mac 0x37eb0, ios 0x28fa0;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x37c60, ios 0x28e90;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x38050, ios 0x28ff8;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x38110, ios 0x29064;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTargetCountArrow(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void updateCountTextInputLabel();
	void updateEditorLabel();
	void updateItemIDInputLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetCount();
	void updateTouchTriggered();
	~SetupPickupTriggerPopup() = mac 0x35dd0, ios 0x2759c;

	PAD = win 0xc;
	CCTextInputNode* m_countInput;
}

class SetupPulsePopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
	virtual void colorValueChanged(cocos2d::ccColor3B color) = mac 0x1ec680, win 0x242660, ios 0x29d574;
	bool init(EffectGameObject* triggerObj, cocos2d::CCArray* triggerObjs) = win 0x23e980;
	void updateColorValue() = win 0x2426b0;
	void onSelectPulseMode(cocos2d::CCObject* p0) = win 0x241420;
	void updatePulseMode() = win 0x242cf0;
	virtual void keyBackClicked() = mac 0x1ec840, ios 0x29d680;
	virtual void show() = mac 0x1ec870, ios 0x29d68c;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0x1ec900, ios 0x29d70c;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x1ec920, ios 0x29d714;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x1ec960, ios 0x29d730;
	virtual void colorSelectClosed(GJSpecialColorSelect* p0, int p1) = mac 0x1ebf20, ios 0x29cf68;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x1ecf20, ios 0x29d9c8;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x1ecfe0, ios 0x29da34;
	void closeColorSelect(cocos2d::CCObject* p0);
	void create(EffectGameObject* p0, cocos2d::CCArray* p1);
	void determineStartValues();
	void getColorValue();
	void onCopy(cocos2d::CCObject* p0);
	void onExclusive(cocos2d::CCObject* p0);
	void onGroupMainOnly(cocos2d::CCObject* p0);
	void onGroupSecondaryOnly(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onPaste(cocos2d::CCObject* p0);
	void onSelectSpecialColor(cocos2d::CCObject* p0);
	void onSelectSpecialTargetID(cocos2d::CCObject* p0);
	void onSelectTargetMode(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void onUpdateCopyColor(cocos2d::CCObject* p0);
	void onUpdateCustomColor(cocos2d::CCObject* p0);
	void selectColor(cocos2d::ccColor3B p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void updateCopyColor();
	void updateCopyColorTextInputLabel();
	void updateEditorLabel();
	void updateFadeInLabel(bool p0);
	void updateFadeInTime();
	void updateFadeOutLabel(bool p0);
	void updateFadeOutTime();
	void updateGroupMainOnly();
	void updateGroupSecondaryOnly();
	void updateHSVValue();
	void updateHoldLabel(bool p0);
	void updateHoldTime();
	void updateMultiTriggerBtn();
	void updatePulseTargetType();
	void updateSpawnedByTrigger();
	void updateTargetID();
	void updateTextInputLabel();
	void updateTouchTriggered();
	~SetupPulsePopup() = mac 0x1e6ca0, ios 0x298dc4;

	cocos2d::extension::CCControlColorPicker* m_colorPicker;
	PAD = win 0x30;
	cocos2d::CCSprite* m_currentColorSpr;
	cocos2d::CCSprite* m_prevColorSpr;
	PAD = win 0x64;
	int m_pulseMode;
}

class SetupRotatePopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x2f47c0, ios 0x1a190c;
	virtual void show() = mac 0x2f4770, ios 0x1a18c0;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x2f45d0, ios 0x1a17d8;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x2f4610, ios 0x1a17f4;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x2f48b0, ios 0x1a1918;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x2f4970, ios 0x1a1984;
	void create(EffectGameObject* p0, cocos2d::CCArray* p1);
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onChangeState(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void updateCustomRotation();
	void updateDisableRotation();
	void updateRotationInputLabel();
	void updateState();
	~SetupRotatePopup() = mac 0x2f2f80, ios 0x1a056c;
}

class SetupShakePopup : FLAlertLayer, TextInputDelegate {
	static SetupShakePopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x3adc00;
	virtual void keyBackClicked() = mac 0x3b00d0, ios 0x2906d4;
	virtual void show() = mac 0x3b0080, ios 0x290688;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x3afc80, ios 0x290454;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x3afcc0, ios 0x290470;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x3b0260, ios 0x2906e0;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x3b0320, ios 0x29074c;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void updateDurLabel(bool p0);
	void updateDuration();
	void updateInterval();
	void updateIntervalLabel(bool p0);
	void updateMultiTriggerBtn();
	void updateShake();
	void updateShakeLabel(bool p0);
	void updateSpawnedByTrigger();
	void updateTouchTriggered();
	~SetupShakePopup() = mac 0x3adb60, ios 0x28e91c;
}

class SetupSpawnPopup : FLAlertLayer, TextInputDelegate {
	static SetupSpawnPopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x139790;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5) = mac 0x13b0e0;
	void onTargetIDArrow(cocos2d::CCObject* p0) = mac 0x13ad80;
	void textChanged(CCTextInputNode* p0) = mac 0x13b990, ios 0x94e8c;
	void updateTargetID() = mac 0x13b770;
	virtual void keyBackClicked() = mac 0x13bc50, ios 0x94ff0;
	virtual void show() = mac 0x13bc00, ios 0x94fa4;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x13b950, ios 0x94e70;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x13bda0, ios 0x94ffc;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x13be60, ios 0x95068;
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onEditorDisable(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void sliderChanged(cocos2d::CCObject* p0);
	void updateDelay();
	void updateDelayLabel(bool p0);
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTextInputLabel();
	void updateTouchTriggered();
	~SetupSpawnPopup() = mac 0x1396f0, ios 0x932b8;

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

class SetupTouchTogglePopup : FLAlertLayer, TextInputDelegate {
	static SetupTouchTogglePopup* create(EffectGameObject* p0, cocos2d::CCArray* p1) = mac 0x1576a0;
	void onTargetIDArrow(cocos2d::CCObject* p0) = mac 0x158b60;
	void textChanged(CCTextInputNode* p0) = mac 0x1596a0, ios 0x167a4c;
	void updateTargetID() = mac 0x159480;
	virtual void keyBackClicked() = mac 0x159850, ios 0x167b34;
	virtual void show() = mac 0x159800, ios 0x167ae8;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x159660, ios 0x167a30;
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) = mac 0x159960, ios 0x167b40;
	virtual void textInputReturn(CCTextInputNode* p0) = mac 0x159a20, ios 0x167bac;
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void determineStartValues();
	bool init(EffectGameObject* p0, cocos2d::CCArray* p1);
	void onClose(cocos2d::CCObject* p0);
	void onDualTouchMode(cocos2d::CCObject* p0);
	void onHoldMode(cocos2d::CCObject* p0);
	void onMultiTrigger(cocos2d::CCObject* p0);
	void onSpawnedByTrigger(cocos2d::CCObject* p0);
	void onTouchMode(cocos2d::CCObject* p0);
	void onTouchTriggered(cocos2d::CCObject* p0);
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTextInputLabel();
	void updateTouchTriggered();
	~SetupTouchTogglePopup() = mac 0x157600, ios 0x1660f8;
}

class ShardsPage : FLAlertLayer {
	virtual bool init() = mac 0x27d610, ios 0x286c5c;
	virtual void registerWithTouchDispatcher() = mac 0x27fc40, ios 0x288b8c;
	virtual void keyBackClicked() = mac 0x27fbd0, ios 0x288b10;
	virtual void show() = mac 0x27fa30, ios 0x288994;
	void FLAlert_Clicked(FLAlertLayer* p0, bool p1);
	void create();
	void goToPage(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onSwitchPage(cocos2d::CCObject* p0);
	~ShardsPage() = mac 0x27d3e0, ios 0x286b50;
}

class ShareCommentLayer : FLAlertLayer, TextInputDelegate, UploadActionDelegate, UploadPopupDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x350590, ios 0x12f45c;
	virtual void keyBackClicked() = mac 0x3504e0, ios 0x12f3b4;
	virtual void textInputOpened(CCTextInputNode* p0) = mac 0x350690, ios 0x12f520;
	virtual void textInputClosed(CCTextInputNode* p0) = mac 0x3505d0, ios 0x12f494;
	virtual void textChanged(CCTextInputNode* p0) = mac 0x3507b0, ios 0x12f64c;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x3509f0, ios 0x12f7dc;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x350ad0, ios 0x12f888;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x350ba0, ios 0x12f934;
	void create(gd::string p0, int p1, CommentType p2, int p3);
	bool init(gd::string p0, int p1, CommentType p2, int p3);
	void onClose(cocos2d::CCObject* p0);
	void onPercent(cocos2d::CCObject* p0);
	void onShare(cocos2d::CCObject* p0);
	void updateCharCountLabel();
	void updateDescText(char const* p0);
	void updatePercentLabel();
	~ShareCommentLayer() = mac 0x34eb90, ios 0x12e034;
}

class ShareLevelLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x12b830, ios 0x1aea74;
	void create(GJGameLevel* p0);
	void getStarsButton(int p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCMenu* p2, float p3);
	bool init(GJGameLevel* p0);
	void onClose(cocos2d::CCObject* p0);
	void onSettings(cocos2d::CCObject* p0);
	void onShare(cocos2d::CCObject* p0);
	void selectRating(cocos2d::CCObject* p0);
	void setupStars();
	~ShareLevelLayer() = mac 0x129e50, ios 0x1ad5a8;
}

class ShareLevelSettingsLayer : FLAlertLayer, NumberInputDelegate {
	virtual void keyBackClicked() = mac 0x12db50, ios 0x1b07b0;
	virtual void numberInputClosed(NumberInputLayer* p0) = mac 0x12da50, ios 0x1b06e0;
	void create(GJGameLevel* p0);
	bool init(GJGameLevel* p0);
	void onClose(cocos2d::CCObject* p0);
	void onCopyable(cocos2d::CCObject* p0);
	void onEditPassword(cocos2d::CCObject* p0);
	void onPassword(cocos2d::CCObject* p0);
	void onUnlisted(cocos2d::CCObject* p0);
	void updateSettingsState();
	~ShareLevelSettingsLayer() = mac 0x12c700, ios 0x1af6d0;
}

class SimpleObject : cocos2d::CCObject {
	void create();
	bool init();
	~SimpleObject();
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
	void setColor(cocos2d::ccColor3B const& color) = mac 0x1bc9b0, win 0x12c410, ios 0x226dd8;
	void setOpacity(unsigned char opacity) = mac 0x1bcc40, win 0x12cb90, ios 0x22705c;
	~SimplePlayer() = mac 0x1bdb10, ios 0x14c40;

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

class SlideInLayer : cocos2d::CCLayerColor {
	virtual bool init() = mac 0x138cf0;
	virtual void draw() = mac 0x1394a0;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1394d0;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x139530;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1394f0;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x139510;
	virtual void registerWithTouchDispatcher() = mac 0x139190;
	virtual void keyBackClicked() = mac 0x1391d0;
	virtual void customSetup() = mac 0x139110;
	virtual void enterLayer() = mac 0x139160;
	virtual void exitLayer(cocos2d::CCObject* p0) = mac 0x139210;
	virtual void showLayer(bool p0) = mac 0x139270;
	virtual void hideLayer(bool p0) = mac 0x139360;
	virtual void layerVisible() = mac 0x139440;
	virtual void layerHidden() = mac 0x139460;
	virtual void enterAnimFinished() = mac 0x139430;
	virtual void disableUI() = mac 0x139120;
	virtual void enableUI() = mac 0x139140;
	void create();
	~SlideInLayer() = mac 0x1388e0;
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
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x18e2e0, ios 0x211700;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x18e360, ios 0x211740;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x18e320, ios 0x211720;
	void create(cocos2d::CCNode* p0, cocos2d::SEL_MenuHandler p1);
	void disableTouch();
	void getLiveDragging();
	void getThumb();
	void hideGroove(bool p0);
	bool init(cocos2d::CCNode* p0, cocos2d::SEL_MenuHandler p1, char const* p2, char const* p3, char const* p4, char const* p5, float p6);
	void setLiveDragging(bool p0);
	void setMaxOffset(float p0);
	void setRotated(bool p0);
	~Slider() = mac 0x18e5f0;

	SliderTouchLogic* m_touchLogic;
	cocos2d::CCSprite* m_sliderBar;
	cocos2d::CCSprite* m_groove;
	float m_unknown;
	float m_height;
}

class SliderThumb : cocos2d::CCMenuItemImage {
	float getValue() {
		return (m_fScaleX * this->m_length * .5f +
				(m_vertical ?
					this->getPositionY() : 
					this->getPositionX())
			) / (m_fScaleX * this->m_length);
	}

	void setValue(float val) = mac 0x18ce80, win 0x2e1b0, ios 0x210db4;
	void create(cocos2d::CCNode* p0, cocos2d::SEL_MenuHandler p1, char const* p2, char const* p3);
	bool init(cocos2d::CCNode* p0, cocos2d::SEL_MenuHandler p1, char const* p2, char const* p3);
	void setMaxOffset(float p0);
	void setRotated(bool p0);
	~SliderThumb();

	float m_length;
	bool m_vertical;
}

class SliderTouchLogic : cocos2d::CCMenu {
	SliderThumb* getThumb() const { return m_thumb; }

	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x18d630, ios 0x21113c;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x18d760, ios 0x2111c0;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x18d730, ios 0x2111b4;
	virtual void registerWithTouchDispatcher() = mac 0x18da90, ios 0x211338;
	void create(cocos2d::CCNode* p0, cocos2d::SEL_MenuHandler p1, char const* p2, char const* p3, float p4);
	bool init(cocos2d::CCNode* p0, cocos2d::SEL_MenuHandler p1, char const* p2, char const* p3, float p4);
	void setMaxOffset(float p0);
	void setRotated(bool p0);
	~SliderTouchLogic() = mac 0x18e690, ios 0x1326f4;

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
	virtual bool init() = mac 0x11c200, ios 0xa5274;
	virtual void draw() = mac 0x11c240, ios 0xa52a4;
	void loadFromObject(SongObject* p0);
	void onClick(cocos2d::CCObject* p0);
	void updateBGColor(int p0);
	~SongCell() = mac 0x11c0e0, ios 0xa5228;
}

class SongInfoLayer : FLAlertLayer {
	static SongInfoLayer* create(int songID) = win 0x250520;
	static SongInfoLayer* create(gd::string songName, gd::string artistName, gd::string downloadLink, gd::string artistNG, gd::string artistYT, gd::string artistFB) = win 0x250830;
	virtual void keyBackClicked() = mac 0x36a980, ios 0x28e514;
	bool init(gd::string p0, gd::string p1, gd::string p2, gd::string p3, gd::string p4, gd::string p5);
	void onClose(cocos2d::CCObject* p0);
	void onDownload(cocos2d::CCObject* p0);
	void onFB(cocos2d::CCObject* p0);
	void onNG(cocos2d::CCObject* p0);
	void onYT(cocos2d::CCObject* p0);
	~SongInfoLayer() = mac 0x369630, ios 0x28d6cc;

	gd::string m_downloadLink;
	gd::string m_artistNewgrounds;
	gd::string m_artistYoutube;
	gd::string m_artistFacebook;
}

class SongInfoObject : cocos2d::CCNode {
	~SongInfoObject() = mac 0x2f2040, ios 0xd42cc;
	virtual void encodeWithCoder(DS_Dictionary* p0) = mac 0x2f2c70, ios 0xd4a5c;
	virtual bool canEncode() = mac 0x2f2da0, ios 0xd4b8c;
	void create(int p0);
	void create(int p0, gd::string p1, gd::string p2, int p3, float p4, gd::string p5, gd::string p6, gd::string p7, int p8);
	void create(cocos2d::CCDictionary* p0);
	void createWithCoder(DS_Dictionary* p0);
	bool init(int p0, gd::string p1, gd::string p2, int p3, float p4, gd::string p5, gd::string p6, gd::string p7, int p8);

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

class SongObject : cocos2d::CCObject {
	void create(int p0);
	bool init(int p0);
	~SongObject();
}

class SongOptionsLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0xf33c0, ios 0xc7640;
	void create(LevelSettingsObject* p0);
	bool init(LevelSettingsObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onFadeIn(cocos2d::CCObject* p0);
	void onFadeOut(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onPlayback(cocos2d::CCObject* p0);
	void updatePlaybackBtn();
	~SongOptionsLayer() = mac 0xf2460, ios 0xc69c4;
}

class SongsLayer : GJDropDownLayer {
	virtual void customSetup() = mac 0x29ec80, ios 0x253a5c;
	void create();
	~SongsLayer() = mac 0x29ebe0, ios 0x253a10;
}

class SpawnTriggerAction : cocos2d::CCNode {
	static SpawnTriggerAction* createFromString(gd::string p0) = mac 0x17bf50;
	void create(float p0, int p1);
	void getSaveString();
	bool init(float p0, int p1);
	void isFinished();
	void step(float p0);
	~SpawnTriggerAction();

	bool m_timerEnded;
	float m_delay;
	float m_timer;
	int m_group;
	int m_uuid;
}

class SpeedObject : cocos2d::CCNode {
	void create(GameObject* p0, int p1, float p2);
	bool init(GameObject* p0, int p1, float p2);
	~SpeedObject() = mac 0x80a00, ios 0xb9920;
}

class SpriteAnimationManager : cocos2d::CCNode {
	void animationFinished();
	void callAnimationFinished();
	void createAnimations(gd::string p0);
	void createWithOwner(CCAnimatedSprite* p0, gd::string p1);
	void doCleanup();
	void executeAnimation(gd::string p0);
	void finishAnimation(gd::string p0);
	void getAnimType(gd::string p0);
	void getPrio(gd::string p0);
	bool initWithOwner(CCAnimatedSprite* p0, gd::string p1);
	void loadAnimations(gd::string p0);
	void offsetCurrentAnimation(float p0);
	void overridePrio();
	void playSound(gd::string p0);
	void playSoundForAnimation(gd::string p0);
	void queueAnimation(gd::string p0);
	void resetAnimState();
	void runAnimation(gd::string p0);
	void runQueuedAnimation();
	void stopAnimations();
	void storeAnimation(cocos2d::CCAnimate* p0, cocos2d::CCAnimate* p1, gd::string p2, int p3, spriteMode p4, cocos2d::CCSpriteFrame* p5);
	void storeSoundForAnimation(cocos2d::CCString* p0, gd::string p1, float p2);
	void switchToFirstFrameOfAnimation(gd::string p0);
	void updateAnimationSpeed(float p0);
	~SpriteAnimationManager() = mac 0x41e630, ios 0x241ee8;
}

class SpriteDescription : cocos2d::CCObject {
	void createDescription(DS_Dictionary* p0);
	void createDescription(cocos2d::CCDictionary* p0);
	void getTransformValues(frameValues* p0);
	bool initDescription(DS_Dictionary* p0);
	bool initDescription(cocos2d::CCDictionary* p0);
	~SpriteDescription() = mac 0x2e6c00, ios 0x115ab8;
}

class SpritePartDelegate {
	virtual cocos2d::CCSpriteFrame* displayFrameChanged(cocos2d::CCObject* p0, gd::string p1) {}
}

class StartPosObject : EffectGameObject {
	virtual bool init() = mac 0xda8a0, ios 0x27e060;
	virtual gd::string getSaveString() = mac 0xda960, ios 0x27e0f0;
	void create();
	void setSettings(LevelSettingsObject* p0);
	~StartPosObject() = mac 0xda6d0, ios 0x27df94;
}

class StatsCell : TableViewCell {
	void updateBGColor(unsigned int index) = win 0x59cf0;
	void draw() = mac 0x11bf80, win 0x59d40, ios 0xa510c;
	virtual bool init() = mac 0x11b100, ios 0xa45f0;
	void getTitleFromKey(char const* p0);
	void loadFromObject(StatsObject* p0);
	void updateBGColor(int p0);
	~StatsCell() = mac 0x120530;
}

class StatsLayer : GJDropDownLayer {
	virtual void customSetup() = mac 0x59df0, ios 0x164a34;
	void create();
	~StatsLayer() = mac 0x59d50, ios 0x1649e8;
}

class StatsObject : cocos2d::CCObject {
	void create(char const* p0, int p1);
	bool init(char const* p0, int p1);
	~StatsObject();
}

class SupportLayer : GJDropDownLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate {
	virtual void customSetup() = mac 0x1d90b0, ios 0xf5ce0;
	virtual void uploadActionFinished(int p0, int p1) = mac 0x1d9de0, ios 0xf6934;
	virtual void uploadActionFailed(int p0, int p1) = mac 0x1d9f00, ios 0xf6a5c;
	virtual void onClosePopup(UploadActionPopup* p0) = mac 0x1d9d90, ios 0xf68e4;
	virtual void FLAlert_Clicked(FLAlertLayer* p0, bool p1) = mac 0x1da170, ios 0xf6ca4;
	void create();
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, cocos2d::CCArray* p5);
	void exitLayer();
	void onCocos2d(cocos2d::CCObject* p0);
	void onEmail(cocos2d::CCObject* p0);
	void onFAQ(cocos2d::CCObject* p0);
	void onGetReward(cocos2d::CCObject* p0);
	void onLowDetail(cocos2d::CCObject* p0);
	void onRequestAccess(cocos2d::CCObject* p0);
	void onRestore(cocos2d::CCObject* p0);
	void onRobTop(cocos2d::CCObject* p0);
	void sendSupportMail();
	~SupportLayer() = mac 0x1d8d60, ios 0xf5c94;
}

class TableView : CCScrollLayerExt, CCScrollLayerExtDelegate {
	inline TableView() {}
	inline TableView(cocos2d::CCRect rect) : CCScrollLayerExt(rect) {}

	static TableView* create(TableViewDelegate* p0, TableViewDataSource* p1, cocos2d::CCRect p2) = mac 0x37eb30, win 0x30ed0;
	void reloadData() = mac 0x37f970, win 0x317e0;
	virtual void onEnter() = mac 0x37ff30, ios 0x21dcac;
	virtual void onExit() = mac 0x37ff40, ios 0x21dcb0;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x380120, ios 0x21de24;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x380be0, ios 0x21e5e8;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x3809a0, ios 0x21e46c;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x380b20, ios 0x21e580;
	virtual void registerWithTouchDispatcher() = mac 0x37ff50, ios 0x21dcb4;
	virtual void scrollWheel(float p0, float p1) = mac 0x380cd0, ios 0x21e6b4;
	virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt* p0) = mac 0x3818a0, ios 0x21efd4;
	virtual void scrollViewDidEndDecelerating(CCScrollLayerExt* p0) = mac 0x3818c0, ios 0x21efdc;
	virtual void scrollViewTouchMoving(CCScrollLayerExt* p0) = mac 0x3818e0, ios 0x21efe4;
	virtual void scrollViewDidEndMoving(CCScrollLayerExt* p0) = mac 0x381900, ios 0x21efec;
	void cancelAndStoleTouch(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	void cellForRowAtIndexPath(CCIndexPath& p0);
	void cellForTouch(cocos2d::CCTouch* p0);
	void checkBoundaryOfCell(TableViewCell* p0);
	void checkBoundaryOfCell(cocos2d::CCPoint& p0, float p1);
	void checkBoundaryOfContent(float p0);
	void checkFirstCell(TableViewCell* p0);
	void checkLastCell(TableViewCell* p0);
	void claimTouch(cocos2d::CCTouch* p0);
	void deleteTableViewCell(TableViewCell* p0);
	void dequeueReusableCellWithIdentifier(char const* p0);
	bool initTableViewCells();
	void isDuplicateInVisibleCellArray(CCIndexPath* p0);
	void isDuplicateIndexPath(CCIndexPath& p0);
	void removeIndexPathFromPathAddedArray(CCIndexPath& p0);
	void scrollToIndexPath(CCIndexPath& p0);
	void touchFinish(cocos2d::CCTouch* p0);
	~TableView() = mac 0x37eeb0, ios 0x21d1b0;

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

	void updateVisibility();

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
	virtual int numberOfRowsInSection(unsigned int p0, TableView* p1) { return 0; }
	virtual unsigned int numberOfSectionsInTableView(TableView* p0) { return 0; }
	virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView* p0, TableViewCellEditingStyle p1, CCIndexPath& p2) {}
	virtual TableViewCell* cellForRowAtIndexPath(CCIndexPath& p0, TableView* p1) { return nullptr; }
}

class TableViewDelegate {
	virtual void willTweenToIndexPath(CCIndexPath& p0, TableViewCell* p1, TableView* p2) {}
	virtual void didEndTweenToIndexPath(CCIndexPath& p0, TableView* p1) {}
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath& p0, TableViewCell* p1, TableView* p2) {}
	virtual void TableViewDidDisplayCellForRowAtIndexPath(CCIndexPath& p0, TableViewCell* p1, TableView* p2) {}
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath& p0, TableViewCell* p1, TableView* p2) {}
	virtual float cellHeightForRowAtIndexPath(CCIndexPath& p0, TableView* p1) { return 0.0; }
	virtual void didSelectRowAtIndexPath(CCIndexPath& p0, TableView* p1) {}
}

class TeleportPortalObject : GameObject {
	virtual void setPosition(cocos2d::CCPoint const& p0) = mac 0xdad00, ios 0x27e3e4;
	virtual void setRotation(float p0) = mac 0xdadb0, ios 0x27e48c;
	virtual void setStartPos(cocos2d::CCPoint p0) = mac 0xdab50, ios 0x27e254;
	virtual void customObjectSetup(gd::map<gd::string, gd::string>& p0) = mac 0xdb640, ios 0x27e858;
	virtual gd::string getSaveString() = mac 0xdaf00, ios 0x27e5d0;
	virtual void addToGroup(int p0) = mac 0xdae40, ios 0x27e508;
	virtual void removeFromGroup(int p0) = mac 0xdaea0, ios 0x27e56c;
	virtual void setRotation2(float p0) = mac 0xdae10, ios 0x27e4fc;
	virtual void addToGroup2(int p0) = mac 0xdae20, ios 0x27e500;
	virtual void removeFromGroup2(int p0) = mac 0xdae30, ios 0x27e504;
	void create(char const* p0);
	void getTeleportXOff(cocos2d::CCNode* p0);
	bool init(char const* p0);
	void setPositionOverride(cocos2d::CCPoint p0);
	void setStartPosOverride(cocos2d::CCPoint p0);
	~TeleportPortalObject() = mac 0xdc290;

	PAD = win 0x4;
	TeleportPortalObject* m_orangePortal;
	bool m_unk470;
	float m_unk474;
	bool m_unk478;
}

class TextAlertPopup : cocos2d::CCNode {
	static TextAlertPopup* create(gd::string const& text, float time, float scale) = win 0x1450b0;
	void create(gd::string p0, float p1, float p2);
	bool init(gd::string p0, float p1, float p2);
	~TextAlertPopup();
}

class TextArea : cocos2d::CCSprite {
	inline TextArea(TextArea const&) : m_fontFile() {}
	inline ~TextArea() {}

	TextArea() = mac 0x19fba0, win 0x33110, ios 0x92c28;
	virtual void draw() = mac 0x19f890, ios 0x92998;
	virtual void setOpacity(unsigned char p0) = mac 0x19f760, ios 0x92874;
	bool init(gd::string str, char const* font, float width, float height, cocos2d::CCPoint anchor, float scale, bool disableColor) = mac 0x19ec70, win 0x33370, ios 0x92444;
	static TextArea* create(gd::string const& str, char const* font, float width, float height, cocos2d::CCPoint const& anchor, float scale, bool disableColor) = mac 0x19eb40, win 0x33270;
	void colorAllCharactersTo(cocos2d::ccColor3B color) = win 0x33830;
	void setString(gd::string str) = mac 0x19eda0, win 0x33480;
	void create(gd::string p0, char const* p1, float p2, float p3, cocos2d::CCPoint p4, float p5, bool p6);
	void fadeIn(float p0, bool p1);
	void fadeInCharacters(float p0, float p1);
	void fadeOut(float p0);
	void fadeOutAndRemove();
	void finishFade();
	void hideAll();
	void setIgnoreColorCode(bool p0);
	void showAll();
	void stopAllCharacterActions();

	bool m_disableColor;
	MultilineBitmapFont* m_label;
	float m_width;
	int m_unknown;
	gd::string m_fontFile;
	float m_height;
}

class TextAreaDelegate {
	virtual void fadeInTextFinished(TextArea* p0) {}
}

class TextInputDelegate {
	virtual void textChanged(CCTextInputNode* p0) {}
	virtual void textInputOpened(CCTextInputNode* p0) {}
	virtual void textInputClosed(CCTextInputNode* p0) {}
	virtual void textInputShouldOffset(CCTextInputNode* p0, float p1) {}
	virtual void textInputReturn(CCTextInputNode* p0) {}
	virtual bool allowTextInput(CCTextInputNode* p0) {return true;}
}

class ToggleTriggerAction : cocos2d::CCNode {
	static ToggleTriggerAction* createFromString(gd::string p0) = mac 0x1765e0;
	void create(int p0, bool p1);
	void getSaveString();
	bool init(int p0, bool p1);
	~ToggleTriggerAction();
}

class TopArtistsLayer : FLAlertLayer, OnlineListDelegate {
	static TopArtistsLayer* create() = mac 0x192a90;
	void setupLeaderboard(cocos2d::CCArray* p0) = mac 0x193420;
	virtual bool init() = mac 0x192c30, ios 0x104f34;
	void loadPage(int p0) = mac 0x193b60;
	void setupPageInfo(gd::string p0, char const* p1) = mac 0x193730, ios 0x105980;
	virtual void registerWithTouchDispatcher() = mac 0x1935b0, ios 0x1058b0;
	virtual void keyBackClicked() = mac 0x193540, ios 0x105834;
	virtual void show() = mac 0x193e40, ios 0x106198;
	virtual void loadListFinished(cocos2d::CCArray* p0, char const* p1) = mac 0x193600, ios 0x1058e8;
	virtual void loadListFailed(char const* p0) = mac 0x1936f0, ios 0x105958;
	void isCorrect(char const* p0);
	void onClose(cocos2d::CCObject* p0);
	void onNextPage(cocos2d::CCObject* p0);
	void onPrevPage(cocos2d::CCObject* p0);
	void updateLevelsLabel();
	~TopArtistsLayer() = mac 0x1929f0, ios 0x104e24;

	cocos2d::CCNode* unknown = mac 0x220;
	GJCommentListLayer* commentLayer = mac 0x260;
}

class TouchToggleAction : cocos2d::CCNode {
	static TouchToggleAction* createFromString(gd::string p0) = mac 0x177e10;
	void create(int p0, bool p1, TouchTriggerType p2);
	void getSaveString();
	bool init(int p0, bool p1, TouchTriggerType p2);
	~TouchToggleAction();
}

class TriggerEffectDelegate {
	virtual void toggleGroupTriggered(int p0, bool p1) {}
	virtual void spawnGroup(int p0) {}
}

class TutorialLayer : FLAlertLayer {
	virtual bool init() = mac 0x286e00, ios 0x11e860;
	virtual void keyBackClicked() = mac 0x2878d0, ios 0x11f240;
	void create();
	void loadPage(int p0);
	void onClose(cocos2d::CCObject* p0);
	void onNext(cocos2d::CCObject* p0);
	void removeTutorialTexture();
	~TutorialLayer() = mac 0x286bd0, ios 0x11e758;
}

class TutorialPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x344ae0;
	virtual void show() = mac 0x344bb0;
	void animateMenu();
	void closeTutorial(cocos2d::CCObject* p0);
	void create(gd::string p0);
	bool init(gd::string p0);
	void registerForCallback(cocos2d::SEL_MenuHandler p0, cocos2d::CCNode* p1);
	~TutorialPopup() = mac 0x344de0;
}

class UILayer : cocos2d::CCLayerColor {
	static UILayer* create() = mac 0x27fd10, win 0x25f310;
	void disableMenu() = mac 0x280960;
	void enableMenu() = mac 0x280940;
	void pCommand(cocos2d::CCNode* p0) = mac 0x280830;
	void toggleCheckpointsMenu(bool p0) = mac 0x280430;
	void onCheck(cocos2d::CCObject* pSender) = mac 0x2803e0, win 0x25fb60;
	void onDeleteCheck(cocos2d::CCObject* pSender) = mac 0x280410, win 0x25fc90;
	void onPause(cocos2d::CCObject* pSender) = mac 0x2803c0, win 0x25fad0;
	virtual void keyDown(cocos2d::enumKeyCodes key) = mac 0x280470, win 0x25f890, ios 0x2be24;
	virtual void keyUp(cocos2d::enumKeyCodes key) = mac 0x280600, win 0x25fa10, ios 0x2bf60;
	UILayer() = win 0x25f230;
	~UILayer() = mac 0x280c60, win 0x25fef0, ios 0x2c288;
	virtual bool init() = mac 0x27fe40, ios 0x2b994;
	virtual void draw() = mac 0x280980, ios 0x2c094;
	virtual bool ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x280990, ios 0x2c098;
	virtual void ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x280ad0, ios 0x2c1b8;
	virtual void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x280af0, ios 0x2c1c0;
	virtual void ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x280bd0, ios 0x2c234;
	virtual void registerWithTouchDispatcher() = mac 0x280c10, ios 0x2c250;
	virtual void keyBackClicked() = mac 0x2808e0, ios 0x2c02c;

	void* m_keyboardDelegateVtable; // rob is silly so he subclasses cckeyboarddelegate twice
	PAD = mac 0x8, win 0x4, android 0x4;
	cocos2d::CCMenu* m_checkPointMenu;
}

class UndoObject : cocos2d::CCObject {
	~UndoObject() = mac 0xa2fd0, ios 0x26d124;
	static UndoObject* create(GameObject* p0, UndoCommand p1) = mac 0x94ea0, win 0x16bc20;
	static UndoObject* createWithArray(cocos2d::CCArray* p0, UndoCommand p1) = mac 0x96ee0, win 0x16bee0;
	static UndoObject* createWithTransformObjects(cocos2d::CCArray* pObjects, UndoCommand nCommand) = win 0x16bcd0;
	bool init(GameObject* p0, UndoCommand p1);
	bool init(cocos2d::CCArray* p0, UndoCommand p1);
	bool initWithTransformObjects(cocos2d::CCArray* p0, UndoCommand p1);
	void setObjects(cocos2d::CCArray* p0);

	GameObject* m_gameObject;
	UndoCommand m_command;
	cocos2d::CCArray* m_objects;
	bool m_redo;
}

class UpdateAccountSettingsPopup : FLAlertLayer, GJAccountSettingsDelegate {
	virtual void keyBackClicked() = mac 0x14bcb0, ios 0x205cac;
	virtual void updateSettingsFinished() = mac 0x14ba40, ios 0x205aac;
	virtual void updateSettingsFailed() = mac 0x14bb80, ios 0x205bc0;
	void create(GJAccountSettingsLayer* p0, int p1, int p2, int p3, gd::string p4, gd::string p5, gd::string p6);
	bool init(GJAccountSettingsLayer* p0, int p1, int p2, int p3, gd::string p4, gd::string p5, gd::string p6);
	void onClose(cocos2d::CCObject* p0);
	~UpdateAccountSettingsPopup() = mac 0x14b330, ios 0x205520;
}

class UploadActionDelegate {
	virtual void uploadActionFinished(int p0, int p1) {}
	virtual void uploadActionFailed(int p0, int p1) {}
}

class UploadActionPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x14ca10, ios 0x2065f8;
	void closePopup();
	void create(UploadPopupDelegate* p0, gd::string p1);
	bool init(UploadPopupDelegate* p0, gd::string p1);
	void onClose(cocos2d::CCObject* p0);
	void showFailMessage(gd::string p0);
	void showSuccessMessage(gd::string p0);
	~UploadActionPopup() = mac 0x14be50, ios 0x205d48;
}

class UploadMessageDelegate {
	virtual void uploadMessageFinished(int p0) {}
	virtual void uploadMessageFailed(int p0);
}

class UploadPopup : FLAlertLayer, LevelUploadDelegate {
	virtual void keyBackClicked() = mac 0x12c600, ios 0x1af630;
	virtual void show() = mac 0x12c5a0, ios 0x1af5d0;
	virtual void levelUploadFinished(GJGameLevel* p0) = mac 0x12c080, ios 0x1af168;
	virtual void levelUploadFailed(GJGameLevel* p0) = mac 0x12c340, ios 0x1af3bc;
	void create(GJGameLevel* p0);
	bool init(GJGameLevel* p0);
	void onBack(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onReturnToLevel(cocos2d::CCObject* p0);
	~UploadPopup() = mac 0x12b9c0, ios 0x1aebbc;
}

class UploadPopupDelegate {
	virtual void onClosePopup() {}

	virtual void onClosePopup(UploadActionPopup* p0) {}
}

class UserInfoDelegate {
	virtual void getUserInfoFinished(GJUserScore* p0) {}
	virtual void getUserInfoFailed(int p0) {}
	virtual void userInfoChanged(GJUserScore* p0) {}
}

class UserListDelegate {
	virtual void getUserListFinished(cocos2d::CCArray* p0, UserListType p1) {}
	virtual void getUserListFailed(UserListType p0, GJErrorCode p1) {}
	virtual void userListChanged(cocos2d::CCArray* p0, UserListType p1) {}
	virtual void forceReloadList(UserListType p0) {}
}

class VideoOptionsLayer : FLAlertLayer {
	virtual bool init() = mac 0x442460, ios 0x8a2d0;
	virtual void keyBackClicked() = mac 0x443f50, ios 0x8b670;
	void create();
	void createToggleButton(gd::string p0, cocos2d::SEL_MenuHandler p1, bool p2, cocos2d::CCMenu* p3, cocos2d::CCPoint p4, float p5, float p6, bool p7);
	void onAdvanced(cocos2d::CCObject* p0);
	void onApply(cocos2d::CCObject* p0);
	void onClose(cocos2d::CCObject* p0);
	void onFullscreen(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onResolutionNext(cocos2d::CCObject* p0);
	void onResolutionPrev(cocos2d::CCObject* p0);
	void onTextureQualityNext(cocos2d::CCObject* p0);
	void onTextureQualityPrev(cocos2d::CCObject* p0);
	void toggleResolution();
	void updateResolution(int p0);
	void updateTextureQuality(int p0);
	~VideoOptionsLayer() = mac 0x4423c0, ios 0x8a284;

	PAD = win 0x18;
	cocos2d::CCArray* m_resolutions;
	PAD = win 0x4;
	int m_currentResolution;
}

class WorldLevelPage : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x204300;
	virtual void show() = mac 0x2041c0;
	void create(GJGameLevel* p0, GJWorldNode* p1);
	bool init(GJGameLevel* p0, GJWorldNode* p1);
	void onClose(cocos2d::CCObject* p0);
	void onInfo(cocos2d::CCObject* p0);
	void onPlay(cocos2d::CCObject* p0);
	void onSong(cocos2d::CCObject* p0);
	~WorldLevelPage() = mac 0x202e70;
}

class WorldSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate {
	virtual void onExit() = mac 0x201320;
	virtual void keyBackClicked() = mac 0x201250;
	virtual void scrollLayerWillScrollToPage(BoomScrollLayer* p0, int p1) = mac 0x201140;
	virtual void scrollLayerScrolledToPage(BoomScrollLayer* p0, int p1) = mac 0x201030;
	virtual void scrollLayerMoved(cocos2d::CCPoint p0) = mac 0x201370;
	void animateInActiveIsland();
	void colorForPage(int p0);
	void create(int p0);
	void getColorValue(int p0, int p1, float p2);
	void goToPage(int p0, bool p1);
	bool init(int p0);
	void onBack(cocos2d::CCObject* p0);
	void onGarage(cocos2d::CCObject* p0);
	void onNext(cocos2d::CCObject* p0);
	void onPrev(cocos2d::CCObject* p0);
	void scene(int p0);
	void setupWorlds();
	void showCompleteDialog();
	void tryShowAd();
	void unblockButtons();
	void updateArrows();
	~WorldSelectLayer() = mac 0x1ff360;
}

class cocos2d {
	static auto ccGLBlendFunc(GLenum p0, GLenum p1) = mac 0x1ae560;
	static auto ccDrawSolidRect(cocos2d::CCPoint p0, cocos2d::CCPoint p1, cocos2d::_ccColor4F p2) = mac 0xecf00;
}

class cocos2d::CCAccelAmplitude : cocos2d::CCActionInterval {
	virtual auto update(float p0) = mac 0xeea70;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xeea40;
	virtual auto reverse() = mac 0xeeac0;
	auto create(cocos2d::CCAction* p0, float p1);
	auto initWithAction(cocos2d::CCAction* p0, float p1);
}

class cocos2d::CCAccelDeccelAmplitude : cocos2d::CCActionInterval {
	virtual auto update(float p0) = mac 0xee6f0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xee6c0;
	virtual auto reverse() = mac 0xee750;
	auto create(cocos2d::CCAction* p0, float p1);
	auto initWithAction(cocos2d::CCAction* p0, float p1);
}

class cocos2d::CCAccelerometerDelegate {
	virtual auto didAccelerate(cocos2d::CCAcceleration* p0);
}

class cocos2d::CCAction : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x35b890, ios 0x19d964;
	virtual auto setTag(int p0);
	virtual auto isDone() = mac 0x35b870, ios 0x19d958;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x35b850, ios 0x19d948;
	virtual auto stop() = mac 0x35b860, ios 0x19d950;
	virtual auto step(float p0) = mac 0x35b880, ios 0x19d960;
	auto CCAction();
	auto create();
	auto description();
	~CCAction() = mac 0x35b6b0, ios 0x19d838;
}

class cocos2d::CCActionCamera : cocos2d::CCActionInterval {
	virtual auto startWithTarget(cocos2d::CCNode* p0);
	virtual auto reverse() = mac 0x1b2a10;
}

class cocos2d::CCActionEase : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a15e0, ios 0x1a658c;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x2a1580, ios 0x1a652c;
	virtual auto stop() = mac 0x2a15b0, ios 0x1a655c;
	virtual auto reverse() = mac 0x2a1600, ios 0x1a659c;
	virtual auto getInnerAction() = mac 0x2a1620, ios 0x1a65bc;
	auto CCActionEase();
	auto create(cocos2d::CCActionInterval* p0);
	auto initWithAction(cocos2d::CCActionInterval* p0);
	~CCActionEase() = mac 0x2a14d0, ios 0x1a64f0;
}

class cocos2d::CCActionInstant : cocos2d::CCFiniteTimeAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x4542f0, ios 0x112518;
	virtual auto isDone() = mac 0x4542c0, ios 0x112500;
	virtual auto step(float p0) = mac 0x4542d0, ios 0x112508;
	virtual auto reverse() = mac 0x454300, ios 0x11251c;
	auto CCActionInstant();
}

class cocos2d::CCActionInterval : cocos2d::CCFiniteTimeAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto isDone() = mac 0x1f2640, ios 0xf04a0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f2700, ios 0xf0530;
	virtual auto step(float p0) = mac 0x1f2660, ios 0xf04b0;
	virtual auto reverse() = mac 0x1f2720, ios 0xf055c;
	auto CCActionInterval();
	auto create(float p0);
	auto getAmplitudeRate();
	auto initWithDuration(float p0);
	auto setAmplitudeRate(float p0);
}

class cocos2d::CCActionManager : cocos2d::CCObject {
	auto addAction(cocos2d::CCAction* p0, cocos2d::CCNode* p1, bool p2) = mac 0x10bed0;
	virtual auto update(float p0) = mac 0x10c9a0, ios 0x213454;
	auto CCActionManager();
	auto actionAllocWithHashElement(cocos2d::_hashElement* p0);
	auto deleteHashElement(cocos2d::_hashElement* p0);
	auto getActionByTag(unsigned int p0, cocos2d::CCObject* p1);
	auto numberOfRunningActionsInTarget(cocos2d::CCObject* p0);
	auto pauseAllRunningActions();
	auto pauseTarget(cocos2d::CCObject* p0);
	auto removeAction(cocos2d::CCAction* p0);
	auto removeActionAtIndex(unsigned int p0, cocos2d::_hashElement* p1);
	auto removeActionByTag(unsigned int p0, cocos2d::CCObject* p1);
	auto removeAllActions();
	auto removeAllActionsFromTarget(cocos2d::CCObject* p0);
	auto resumeTarget(cocos2d::CCObject* p0);
	auto resumeTargets(cocos2d::CCSet* p0);
	~CCActionManager() = mac 0x10b980, ios 0x21264c;
}

class cocos2d::CCActionTween : cocos2d::CCActionInterval {
	static cocos2d::CCActionTween* create(float p0, char const* p1, float p2, float p3) = mac 0x447590;
	virtual auto update(float p0) = mac 0x447720, ios 0x28e6fc;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x4476f0, ios 0x28e6d0;
	virtual auto reverse() = mac 0x447760, ios 0x28e734;
	auto initWithDuration(float p0, char const* p1, float p2, float p3);
}

class cocos2d::CCActionTweenDelegate {
	~CCActionTweenDelegate();
}

class cocos2d::CCAnimate : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCAnimate* create(cocos2d::CCAnimation* p0) = mac 0x1f8fc0;
	virtual auto update(float p0) = mac 0x1f9610, ios 0xf3c04;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f9570, ios 0xf3b58;
	virtual auto stop() = mac 0x1f95d0, ios 0xf3bc0;
	virtual auto reverse() = mac 0x1f9840, ios 0xf3e04;
	auto CCAnimate();
	auto initWithAnimation(cocos2d::CCAnimation* p0);
}

class cocos2d::CCAnimation : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static auto createWithSpriteFrames(cocos2d::CCArray* p0, float p1) = mac 0x140df0;
	virtual auto getDuration() = mac 0x141460, ios 0x104c2c;
	auto CCAnimation();
	auto addSpriteFrame(cocos2d::CCSpriteFrame* p0);
	auto addSpriteFrameWithFileName(char const* p0);
	auto addSpriteFrameWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1);
	auto create(cocos2d::CCArray* p0, float p1, unsigned int p2);
	auto create();
	auto init();
	auto initWithAnimationFrames(cocos2d::CCArray* p0, float p1, unsigned int p2);
	auto initWithSpriteFrames(cocos2d::CCArray* p0, float p1);
	auto setFrames(cocos2d::CCArray* p0);
	~CCAnimation() = mac 0x141220, ios 0x104bf0;
}

class cocos2d::CCAnimationCache : cocos2d::CCObject {
	auto CCAnimationCache();
	auto addAnimation(cocos2d::CCAnimation* p0, char const* p1);
	auto addAnimationsWithDictionary(cocos2d::CCDictionary* p0, char const* p1);
	auto addAnimationsWithFile(char const* p0);
	auto animationByName(char const* p0);
	auto init();
	auto parseVersion1(cocos2d::CCDictionary* p0);
	auto parseVersion2(cocos2d::CCDictionary* p0);
	auto purgeSharedAnimationCache();
	auto removeAnimationByName(char const* p0);
	auto sharedAnimationCache();
	~CCAnimationCache() = mac 0x244bd0, ios 0x12dc48;
}

class cocos2d::CCAnimationFrame : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	auto CCAnimationFrame();
	auto initWithSpriteFrame(cocos2d::CCSpriteFrame* p0, float p1, cocos2d::CCDictionary* p2);
	~CCAnimationFrame() = mac 0x140ab0, ios 0x104740;
}

class cocos2d::CCApplication : cocos2d::CCApplicationProtocol {
	virtual auto getCurrentLanguage() = mac 0x1a3f40, ios 0x10e508;
	virtual auto getTargetPlatform() = mac 0x1a3f20, ios 0x10e57c;
	virtual auto openURL(char const* p0) = mac 0x1a4550, ios 0x10e7a4;
	virtual auto setAnimationInterval(double p0) = mac 0x1a3ee0, ios 0x10e494;
	static auto sharedApplication() = mac 0x1a3f30;
	~CCApplication() = mac 0x1a3d10, ios 0x10e384;
	auto CCApplication();
	auto run();
}

class cocos2d::CCApplicationProtocol {
	virtual auto applicationWillBecomeActive();
	virtual auto applicationWillResignActive();
	virtual auto trySaveGame();
	virtual auto gameDidSave();
	virtual auto openURL(char const* p0);
	~CCApplicationProtocol();
}

class cocos2d::CCArray : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	auto addObject(cocos2d::CCObject* p0) = mac 0x419f90, ios 0x16504c;
	auto addObjectNew(cocos2d::CCObject* p0) = mac 0x41a450;
	auto addObjectsFromArray(cocos2d::CCArray* p0) = mac 0x41a2d0;
	auto containsObject(cocos2d::CCObject* p0) const = mac 0x41a3e0;
	auto count() const = mac 0x41a2f0, ios 0x1650e8;
	static cocos2d::CCArray* create() = mac 0x419cb0, ios 0x164ec8;
	static auto createWithObject(cocos2d::CCObject* p0) = mac 0x419d50;
	auto fastRemoveObject(cocos2d::CCObject* p0) = mac 0x41a520;
	auto fastRemoveObjectAtIndex(unsigned int p0) = mac 0x41a500;
	auto fastRemoveObjectAtIndexNew(unsigned int p0) = mac 0x41a510;
	auto lastObject() = mac 0x41a360;
	auto objectAtIndex(unsigned int p0) = mac 0x41a340, ios 0x16510c;
	auto removeAllObjects() = mac 0x41a4f0, ios 0x1651f0;
	auto removeLastObject(bool p0) = mac 0x41a470;
	auto removeObject(cocos2d::CCObject* p0, bool p1) = mac 0x41a490;
	auto removeObjectAtIndex(unsigned int p0, bool p1) = mac 0x41a4b0;
	auto stringAtIndex(unsigned int p0) = mac 0x41a320;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0) = mac 0x41a880, ios 0x165398;
	auto CCArray(unsigned int p0);
	auto CCArray();
	auto capacity();
	auto createWithArray(cocos2d::CCArray* p0);
	auto createWithCapacity(unsigned int p0);
	auto createWithContentsOfFile(char const* p0);
	auto createWithContentsOfFileThreadSafe(char const* p0);
	auto exchangeObject(cocos2d::CCObject* p0, cocos2d::CCObject* p1);
	auto exchangeObjectAtIndex(unsigned int p0, unsigned int p1);
	auto indexOfObject(cocos2d::CCObject* p0);
	auto init();
	auto initWithArray(cocos2d::CCArray* p0);
	auto initWithCapacity(unsigned int p0);
	auto initWithObject(cocos2d::CCObject* p0);
	auto insertObject(cocos2d::CCObject* p0, unsigned int p1);
	auto isEqualToArray(cocos2d::CCArray* p0);
	auto randomObject();
	auto reduceMemoryFootprint();
	auto removeObjectsInArray(cocos2d::CCArray* p0);
	auto replaceObjectAtIndex(unsigned int p0, cocos2d::CCObject* p1, bool p2);
	auto reverseObjects();
	~CCArray() = mac 0x41a6c0, ios 0x165290;
}

class cocos2d::CCAtlasNode : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	virtual auto draw() = mac 0x1a4f70;
	virtual auto setOpacity(unsigned char p0) = mac 0x1a51c0;
	virtual auto getColor() = mac 0x1a5060;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x1a50b0;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x1a5240;
	virtual auto isOpacityModifyRGB() = mac 0x1a52f0;
	virtual auto getTextureAtlas() = mac 0x1a54a0;
	virtual auto setTextureAtlas(cocos2d::CCTextureAtlas* p0) = mac 0x1a5460;
	virtual auto getBlendFunc() = mac 0x1a5320;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x1a5340;
	virtual auto getQuadsToDraw() = mac 0x1a54b0;
	virtual auto setQuadsToDraw(unsigned int p0) = mac 0x1a54c0;
	virtual auto updateAtlasValues() = mac 0x1a4f60;
	virtual auto getTexture() = mac 0x1a5430;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x1a5360;
	auto CCAtlasNode();
	auto calculateMaxItems();
	auto create(char const* p0, unsigned int p1, unsigned int p2, unsigned int p3);
	auto initWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1, unsigned int p2, unsigned int p3);
	auto initWithTileFile(char const* p0, unsigned int p1, unsigned int p2, unsigned int p3);
	auto setIgnoreContentScaleFactor(bool p0);
	auto updateBlendFunc();
	auto updateOpacityModifyRGB();
	~CCAtlasNode() = mac 0x1a4a30;
}

class cocos2d::CCAutoreleasePool : cocos2d::CCObject {
	auto CCAutoreleasePool();
	auto addObject(cocos2d::CCObject* p0);
	auto clear();
	auto removeObject(cocos2d::CCObject* p0);
	~CCAutoreleasePool() = mac 0x214100, ios 0x114a1c;
}

class cocos2d::CCBMFontConfiguration : cocos2d::CCObject {
	static cocos2d::CCBMFontConfiguration* create(char const* p0) = mac 0x3450f0;
	auto CCBMFontConfiguration();
	auto description();
	auto getCharacterSet();
	auto initWithFNTfile(char const* p0);
	auto parseCharacterDefinition(gd::string p0, cocos2d::_BMFontDef* p1);
	auto parseCommonArguments(gd::string p0);
	auto parseConfigFile(char const* p0);
	auto parseImageFileName(gd::string p0, char const* p1);
	auto parseInfoArguments(gd::string p0);
	auto parseKerningEntry(gd::string p0);
	auto purgeFontDefDictionary();
	auto purgeKerningDictionary();
	~CCBMFontConfiguration() = mac 0x346130, ios 0x217f2c;
}

class cocos2d::CCBezierBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f6960, ios 0xf21ec;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f67a0, ios 0xf2088;
	virtual auto reverse() = mac 0x1f6b40, ios 0xf2368;
	auto CCBezierBy();
	auto create(float p0, cocos2d::_ccBezierConfig const& p1);
	auto initWithDuration(float p0, cocos2d::_ccBezierConfig const& p1);
}

class cocos2d::CCBezierTo : cocos2d::CCBezierBy {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCBezierTo* create(float p0, cocos2d::_ccBezierConfig const& p1) = mac 0x1f6c10;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f6f30, ios 0xf2690;
	auto initWithDuration(float p0, cocos2d::_ccBezierConfig const& p1);
}

class cocos2d::CCBlendProtocol {}

class cocos2d::CCBlink : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f79f0, ios 0xf2c3c;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f78b0, ios 0xf2b08;
	virtual auto stop() = mac 0x1f7880, ios 0xf2ad4;
	virtual auto reverse() = mac 0x1f7a70, ios 0xf2cc8;
	auto create(float p0, unsigned int p1);
	auto initWithDuration(float p0, unsigned int p1);
}

class cocos2d::CCBool : cocos2d::CCObject {
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0);
	auto create(bool p0);
	~CCBool();
}

class cocos2d::CCCallFunc : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static auto create(cocos2d::CCObject* p0, cocos2d::SEL_CallFunc p1) = mac 0x454d90;
	virtual auto update(float p0) = mac 0x455120, ios 0x112a2c;
	virtual auto initWithTarget(cocos2d::CCObject* p0) = mac 0x454eb0, ios 0x112838;
	virtual auto execute() = mac 0x455130, ios 0x112a38;
	auto CCCallFunc();
	auto create(int p0);
	~CCCallFunc() = mac 0x454f50, ios 0x1128ec;
}

class cocos2d::CCCallFuncN : cocos2d::CCCallFunc, cocos2d::TypeInfo {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto execute() = mac 0x455190;
	virtual auto getClassTypeInfo();
	virtual auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_CallFuncN p1) = mac 0x455330;
	auto create(int p0);
	auto create(cocos2d::CCObject* p0, cocos2d::SEL_CallFuncN p1);
}

class cocos2d::CCCallFuncND : cocos2d::CCCallFuncN {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto getClassTypeInfo();
	virtual auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_CallFuncND p1, void* p2) = mac 0x455560;
	virtual auto execute() = mac 0x4556e0;
	auto create(cocos2d::CCObject* p0, cocos2d::SEL_CallFuncND p1, void* p2);
}

class cocos2d::CCCallFuncO : cocos2d::CCCallFunc, cocos2d::TypeInfo {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static auto create(cocos2d::CCObject* p0, cocos2d::SEL_CallFuncO p1, cocos2d::CCObject* p2) = mac 0x455940;
	virtual auto execute() = mac 0x455910, ios 0x112b40;
	virtual auto getClassTypeInfo();
	virtual auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCObject* p2) = mac 0x455a20, ios 0x112c38;
	auto CCCallFuncO();
	auto create(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCObject* p2);
}

class cocos2d::CCCamera : cocos2d::CCObject {
	auto CCCamera();
	auto description();
	auto getCenterXYZ(float* p0, float* p1, float* p2);
	auto getEyeXYZ(float* p0, float* p1, float* p2);
	auto getUpXYZ(float* p0, float* p1, float* p2);
	auto getZEye();
	auto init();
	auto locate();
	auto restore();
	auto setCenterXYZ(float p0, float p1, float p2);
	auto setEyeXYZ(float p0, float p1, float p2);
	auto setUpXYZ(float p0, float p1, float p2);
	~CCCamera() = mac 0x2e0980, ios 0xfee84;
}

class cocos2d::CCCardinalSplineBy : cocos2d::CCCardinalSplineTo {
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1684b0;
	virtual auto reverse() = mac 0x168160;
	virtual auto updatePosition(cocos2d::CCPoint& p0) = mac 0x168110;
	auto CCCardinalSplineBy();
	auto create(float p0, cocos2d::CCPointArray* p1, float p2);
}

class cocos2d::CCCardinalSplineTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x167c40;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x167aa0;
	virtual auto reverse() = mac 0x167f60;
	virtual auto updatePosition(cocos2d::CCPoint& p0) = mac 0x167f30;
	auto CCCardinalSplineTo();
	auto create(float p0, cocos2d::CCPointArray* p1, float p2);
	auto initWithDuration(float p0, cocos2d::CCPointArray* p1, float p2);
	~CCCardinalSplineTo() = mac 0x167960;
}

class cocos2d::CCCatmullRomBy : cocos2d::CCCardinalSplineBy {
	auto create(float p0, cocos2d::CCPointArray* p1);
	auto initWithDuration(float p0, cocos2d::CCPointArray* p1);
	~CCCatmullRomBy();
}

class cocos2d::CCCatmullRomTo : cocos2d::CCCardinalSplineTo {
	auto create(float p0, cocos2d::CCPointArray* p1);
	auto initWithDuration(float p0, cocos2d::CCPointArray* p1);
	~CCCatmullRomTo();
}

class cocos2d::CCClippingNode : cocos2d::CCNode {
	static cocos2d::CCClippingNode* create() = mac 0x4192a0;
	static cocos2d::CCClippingNode* create(cocos2d::CCNode* p0) = mac 0x419330;
	auto getAlphaThreshold() const = mac 0x419a10;
	auto getStencil() const = mac 0x4199c0;
	auto isInverted() const = mac 0x419a30;
	auto onEnter() = mac 0x419470;
	auto onEnterTransitionDidFinish() = mac 0x4194a0;
	auto onExit() = mac 0x419500;
	auto onExitTransitionDidStart() = mac 0x4194d0;
	auto setAlphaThreshold(float p0) = mac 0x419a20;
	auto setInverted(bool p0) = mac 0x419a40;
	auto setStencil(cocos2d::CCNode* p0) = mac 0x4199d0;
	auto visit() = mac 0x419530;
	virtual auto init() = mac 0x4193e0;
	virtual auto init(cocos2d::CCNode* p0) = mac 0x419400;
	auto CCClippingNode();
	~CCClippingNode() = mac 0x4191e0;
}

class cocos2d::CCComponent : cocos2d::CCObject {
	virtual auto update(float p0) = mac 0x5a510;
	virtual auto init() = mac 0x5a4e0;
	virtual auto onEnter() = mac 0x5a4f0;
	virtual auto onExit() = mac 0x5a500;
	virtual auto serialize(void* p0) = mac 0x5a520;
	virtual auto isEnabled() const = mac 0x5a600;
	virtual auto setEnabled(bool p0) = mac 0x5a610;
	auto CCComponent();
	auto create();
	auto getName();
	auto getOwner();
	auto setName(char const* p0);
	auto setOwner(cocos2d::CCNode* p0);
	~CCComponent() = mac 0x5a420;
}

class cocos2d::CCComponentContainer {
	virtual auto get(char const* p0) const = mac 0x1663a0, ios 0xdc854;
	virtual auto add(cocos2d::CCComponent* p0) = mac 0x166470, ios 0xdc970;
	virtual auto remove(char const* p0) = mac 0x1665f0, ios 0xdcb74;
	virtual auto remove(cocos2d::CCComponent* p0) = mac 0x1668d0, ios 0xdce54;
	virtual auto removeAll() = mac 0x166ab0, ios 0xdd00c;
	virtual auto visit(float p0) = mac 0x166cb0, ios 0xdd1b8;
	auto CCComponentContainer(cocos2d::CCNode* p0);
	auto alloc();
	auto isEmpty();
	~CCComponentContainer();
}

class cocos2d::CCConfiguration : cocos2d::CCObject {
	auto CCConfiguration();
	auto checkForGLExtension(gd::string const& p0);
	auto dumpInfo();
	auto gatherGPUInfo();
	auto getBool(char const* p0, bool p1);
	auto getCString(char const* p0, char const* p1);
	auto getMaxModelviewStackDepth();
	auto getMaxTextureSize();
	auto getMaxTextureUnits();
	auto getNumber(char const* p0, double p1);
	auto getObject(char const* p0);
	auto init();
	auto loadConfigFile(char const* p0);
	auto purgeConfiguration();
	auto setObject(char const* p0, cocos2d::CCObject* p1);
	auto sharedConfiguration();
	auto supportsBGRA8888();
	auto supportsDiscardFramebuffer();
	auto supportsNPOT();
	auto supportsPVRTC();
	auto supportsShareableVAO();
	~CCConfiguration() = mac 0x2a6c60, ios 0x242258;
}

class cocos2d::CCCopying {
	auto copyWithZone(cocos2d::CCZone* p0);
}

class cocos2d::CCDataVisitor {
	virtual auto visit(cocos2d::CCBool const* p0);
	virtual auto visit(cocos2d::CCInteger const* p0);
	virtual auto visit(cocos2d::CCFloat const* p0);
	virtual auto visit(cocos2d::CCDouble const* p0);
	virtual auto visit(cocos2d::CCString const* p0);
	virtual auto visit(cocos2d::CCArray const* p0);
	virtual auto visit(cocos2d::CCDictionary const* p0);
	virtual auto visit(cocos2d::CCSet const* p0);
	~CCDataVisitor();
}

class cocos2d::CCDeccelAmplitude : cocos2d::CCActionInterval {
	virtual auto update(float p0) = mac 0xeedb0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xeed80;
	virtual auto reverse() = mac 0xeee10;
	auto create(cocos2d::CCAction* p0, float p1);
	auto initWithAction(cocos2d::CCAction* p0, float p1);
}

class cocos2d::CCDelayTime : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCDelayTime* create(float p0) = mac 0x1f4380;
	virtual auto update(float p0) = mac 0x1f8af0, ios 0xf36a4;
	virtual auto reverse() = mac 0x1f8b00, ios 0xf36a8;
}

class cocos2d::CCDictionary : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	auto allKeys() = mac 0x190450, ios 0x2de774;
	auto count() = mac 0x190430;
	static cocos2d::CCDictionary* create() = mac 0x192650;
	auto objectForKey(intptr_t p0) = mac 0x190bb0, ios 0x2decc0;
	auto objectForKey(gd::string const& p0) = mac 0x190870, ios 0x2de988;
	auto removeAllObjects() = mac 0x190220;
	auto removeObjectForKey(intptr_t p0) = mac 0x1921d0;
	auto setObject(cocos2d::CCObject* p0, intptr_t p1) = mac 0x191790, ios 0x2df734;
	auto setObject(cocos2d::CCObject* p0, gd::string const& p1) = mac 0x190dc0, ios 0x2dee7c;
	auto valueForKey(intptr_t p0) = mac 0x190cf0;
	auto valueForKey(gd::string const& p0) = mac 0x1907a0;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0) = mac 0x192790, ios 0x2e066c;
	auto CCDictionary();
	auto allKeysForObject(cocos2d::CCObject* p0);
	auto charForKey(gd::string const& p0);
	auto createWithContentsOfFile(char const* p0);
	auto createWithContentsOfFileThreadSafe(char const* p0);
	auto createWithDictionary(cocos2d::CCDictionary* p0);
	auto getFirstKey();
	auto objectForKey(int p0);
	auto randomObject();
	auto removeObjectForElememt(cocos2d::CCDictElement* p0);
	auto removeObjectForKey(int p0);
	auto removeObjectForKey(gd::string const& p0);
	auto removeObjectsForKeys(cocos2d::CCArray* p0);
	auto setObject(cocos2d::CCObject* p0, int p1);
	auto setObjectUnSafe(cocos2d::CCObject* p0, int p1);
	auto setObjectUnSafe(cocos2d::CCObject* p0, gd::string const& p1);
	auto valueForKey(int p0);
	auto writeToFile(char const* p0);
	~CCDictionary() = mac 0x190390, ios 0x2de71c;
}

class cocos2d::CCDirector : cocos2d::CCObject, cocos2d::TypeInfo {
	~CCDirector() = mac 0x2493a0, ios 0xec4e4;
	virtual auto init() = mac 0x248df0, ios 0xec064;
	virtual auto getScheduler() = mac 0x24af00, ios 0xed6bc;
	virtual auto setScheduler(cocos2d::CCScheduler* p0) = mac 0x24aec0, ios 0xed674;
	virtual auto getActionManager() = mac 0x24af50, ios 0xed70c;
	virtual auto setActionManager(cocos2d::CCActionManager* p0) = mac 0x24af10, ios 0xed6c4;
	virtual auto getTouchDispatcher() = mac 0x24afa0, ios 0xed75c;
	virtual auto setTouchDispatcher(cocos2d::CCTouchDispatcher* p0) = mac 0x24af60, ios 0xed714;
	virtual auto getKeypadDispatcher() = mac 0x24b090, ios 0xed828;
	virtual auto setKeypadDispatcher(cocos2d::CCKeypadDispatcher* p0) = mac 0x24b050, ios 0xed7ec;
	virtual auto getKeyboardDispatcher() = mac 0x24aff0, ios 0xed7a0;
	virtual auto setKeyboardDispatcher(cocos2d::CCKeyboardDispatcher* p0) = mac 0x24afb0, ios 0xed764;
	virtual auto getMouseDispatcher() = mac 0x24b040, ios 0xed7e4;
	virtual auto setMouseDispatcher(cocos2d::CCMouseDispatcher* p0) = mac 0x24b000, ios 0xed7a8;
	virtual auto getAccelerometer() = mac 0x24b0e0, ios 0xed894;
	virtual auto setAccelerometer(cocos2d::CCAccelerometer* p0) = mac 0x24b0a0, ios 0xed830;
	virtual auto getDeltaTime() = mac 0x249bd0, ios 0xecbc8;
	virtual auto getSceneDelegate() = mac 0x24b320, ios 0xedaa8;
	virtual auto setSceneDelegate(cocos2d::CCSceneDelegate* p0) = mac 0x24b330, ios 0xedab0;
	auto getWinSize() = mac 0x24a0f0, ios 0xece34;
	auto pushScene(cocos2d::CCScene* p0) = mac 0x24a620;
	auto replaceScene(cocos2d::CCScene* p0) = mac 0x24a6d0, ios 0xed08c;
	auto resetSmoothFixCounter() = mac 0x249bc0;
	static auto sharedDirector() = mac 0x248cb0, ios 0xebf84;
	auto calculateDeltaTime() = mac 0x2497a0;
	auto calculateMPF() = mac 0x19eac0;
	auto convertToGL(cocos2d::CCPoint const& p0) = mac 0x24a210;
	auto drawScene() = mac 0x249690;
	auto setNextScene() = mac 0x2498d0;
	auto showStats() = mac 0x2499e0;
	auto updateContentScale(cocos2d::TextureQuality p0) = mac 0x249ff0;
	auto popSceneWithTransition(float p0, cocos2d::PopTransition p1) = mac 0x24a8b0;
	virtual auto getClassTypeInfo();
	auto CCDirector();
	auto checkSceneReference();
	auto convertToUI(cocos2d::CCPoint const& p0);
	auto createStatsLabel();
	auto end();
	auto getContentScaleFactor();
	auto getDelegate();
	auto getFPSImageData(unsigned char** p0, unsigned int* p1);
	auto getNotificationNode();
	auto getScreenBottom();
	auto getScreenLeft();
	auto getScreenRight();
	auto getScreenScaleFactor();
	auto getScreenScaleFactorH();
	auto getScreenScaleFactorMax();
	auto getScreenScaleFactorW();
	auto getScreenTop();
	auto getVisibleOrigin();
	auto getVisibleSize();
	auto getWinSizeInPixels();
	auto getZEye();
	auto levelForSceneInStack(cocos2d::CCScene* p0);
	auto pause();
	auto popScene();
	auto popToRootScene();
	auto popToSceneInStack(cocos2d::CCScene* p0);
	auto popToSceneStackLevel(int p0);
	auto purgeCachedData();
	auto purgeDirector();
	auto removeStatsLabel();
	auto reshapeProjection(cocos2d::CCSize const& p0);
	auto resume();
	auto runWithScene(cocos2d::CCScene* p0);
	auto sceneCount();
	auto setAlphaBlending(bool p0);
	auto setContentScaleFactor(float p0);
	auto setDefaultValues();
	auto setDelegate(cocos2d::CCDirectorDelegate* p0);
	auto setDeltaTime(float p0);
	auto setDepthTest(bool p0);
	auto setGLDefaultValues();
	auto setNextDeltaTimeZero(bool p0);
	auto setNotificationNode(cocos2d::CCNode* p0);
	auto setOpenGLView(cocos2d::CCEGLView* p0);
	auto setProjection(cocos2d::ccDirectorProjection p0);
	auto setViewport();
	auto setupScreenScale(cocos2d::CCSize p0, cocos2d::CCSize p1, cocos2d::TextureQuality p2);
	auto updateScreenScale(cocos2d::CCSize p0);
	auto willSwitchToScene(cocos2d::CCScene* p0);
}

class cocos2d::CCDisplayLinkDirector : cocos2d::CCDirector {
	virtual auto setAnimationInterval(double p0) = mac 0x24b180, ios 0xed918;
	virtual auto stopAnimation() = mac 0x24b170, ios 0xed90c;
	virtual auto startAnimation() = mac 0x24b0f0, ios 0xed89c;
	virtual auto mainLoop() = mac 0x24b130, ios 0xed8d8;
}

class cocos2d::CCDrawNode : cocos2d::CCNodeRGBA {
	auto clear() = mac 0x379e80;
	static cocos2d::CCDrawNode* create() = mac 0x378d00;
	auto drawPolygon(cocos2d::CCPoint* p0, unsigned int p1, cocos2d::_ccColor4F const& p2, float p3, cocos2d::_ccColor4F const& p4) = mac 0x3797f0;
	auto drawSegment(cocos2d::CCPoint const& p0, cocos2d::CCPoint const& p1, float p2, cocos2d::_ccColor4F const& p3) = mac 0x3792d0;
	auto getBlendFunc() const = mac 0x379ea0;
	auto init() = mac 0x378e00, ios 0x26d00;
	auto setBlendFunc(cocos2d::ccBlendFunc const& p0) = mac 0x379eb0;
	auto draw() = mac 0x379020, ios 0x26eb4;
	~CCDrawNode() = mac 0x378cc0, ios 0x26bc8;
	auto CCDrawNode();
	auto drawDot(cocos2d::CCPoint const& p0, float p1, cocos2d::_ccColor4F const& p2);
	auto ensureCapacity(unsigned int p0);
	auto listenBackToForeground(cocos2d::CCObject* p0);
	auto render();
}

class cocos2d::CCEGLView : cocos2d::CCEGLViewProtocol {
	virtual auto swapBuffers() = mac 0x295510, ios 0xbad54;
	void updateWindow(int width, int height);
	void toggleFullScreen(bool fullscreen);
	void pollEvents();
	void onGLFWCharCallback(GLFWwindow* window, unsigned int entered);
	void onGLFWCursorEnterFunCallback(GLFWwindow* window, int entered);
	void onGLFWDeviceChangeFunCallback(GLFWwindow* window);
	void onGLFWError(int code, const char* description);
	void onGLFWframebuffersize(GLFWwindow* window, int width, int height);
	void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y);
	void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods);
	void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void onGLFWWindowIconifyFunCallback(GLFWwindow* window, int iconified);
	void onGLFWWindowPosCallback(GLFWwindow* window, int x, int y);
	void onGLFWWindowSizeFunCallback(GLFWwindow* window, int width, int height);
	virtual auto end() = mac 0x2954a0, ios 0xbad14;
	virtual auto isOpenGLReady() = mac 0x295470, ios 0xbac80;
	virtual auto setIMEKeyboardState(bool p0) = mac 0x295550, ios 0xbad80;
	virtual auto setViewPortInPoints(float p0, float p1, float p2, float p3) = mac 0x295490, ios 0x20efb4;
	virtual auto setScissorInPoints(float p0, float p1, float p2, float p3) = mac 0x295730, ios 0x20efe8;
	auto CCEGLView();
	auto sharedOpenGLView();
	~CCEGLView();
}

class cocos2d::CCEGLViewProtocol {
	auto getViewPortRect() const = mac 0x29e2f0;
	virtual auto getFrameSize() const = mac 0x29d950, ios 0x20eee4;
	virtual auto setFrameSize(float p0, float p1) = mac 0x29d960, ios 0x20eeec;
	virtual auto getVisibleSize() const = mac 0x29d9a0, ios 0x20ef30;
	virtual auto getVisibleOrigin() const = mac 0x29d9f0, ios 0x20ef60;
	virtual auto setDesignResolutionSize(float p0, float p1, ResolutionPolicy p2) = mac 0x29d7c0, ios 0x20edb0;
	virtual auto getDesignResolutionSize() const = mac 0x29d940, ios 0x20eedc;
	virtual auto setTouchDelegate(cocos2d::EGLTouchDelegate* p0) = mac 0x29da60, ios 0x20efac;
	virtual auto setViewPortInPoints(float p0, float p1, float p2, float p3) = mac 0x29da70, ios 0x20efb4;
	virtual auto setScissorInPoints(float p0, float p1, float p2, float p3) = mac 0x29dab0, ios 0x20efe8;
	virtual auto isScissorEnabled() = mac 0x29daf0, ios 0x20f01c;
	virtual auto getScissorRect() = mac 0x29db10, ios 0x20f03c;
	virtual auto setViewName(char const* p0) = mac 0x29dba0, ios 0x20f0d4;
	virtual auto handleTouchesBegin(int p0, int* p1, float* p2, float* p3) = mac 0x29dbd0, ios 0x20f0f0;
	virtual auto handleTouchesMove(int p0, int* p1, float* p2, float* p3) = mac 0x29deb0, ios 0x20f350;
	virtual auto handleTouchesEnd(int p0, int* p1, float* p2, float* p3) = mac 0x29e1d0, ios 0x20f5cc;
	virtual auto handleTouchesCancel(int p0, int* p1, float* p2, float* p3) = mac 0x29e260, ios 0x20f66c;
	virtual auto pollInputEvents() = mac 0x29e320, ios 0x20f724;
	auto CCEGLViewProtocol();
	auto getScaleX();
	auto getScaleY();
	auto getSetOfTouchesEndOrCancel(cocos2d::CCSet& p0, int p1, int* p2, float* p3, float* p4);
	auto getViewName();
	auto updateDesignResolutionSize();
	~CCEGLViewProtocol();
}

class cocos2d::CCEaseBackIn : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a42a0, ios 0x1a85a4;
	virtual auto reverse() = mac 0x2a42d0, ios 0x1a85d4;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBackInOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a4670, ios 0x1a8914;
	virtual auto reverse() = mac 0x2a4700, ios 0x1a89a0;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBackOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a4480, ios 0x1a8754;
	virtual auto reverse() = mac 0x2a44c0, ios 0x1a8794;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBounce : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto reverse() = mac 0x2a3890;
	auto bounceTime(float p0);
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBounceIn : cocos2d::CCEaseBounce {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a3a40, ios 0x1a804c;
	virtual auto reverse() = mac 0x2a3b20, ios 0x1a808c;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBounceInOut : cocos2d::CCEaseBounce {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a3f40, ios 0x1a83b8;
	virtual auto reverse() = mac 0x2a40f0, ios 0x1a8424;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBounceOut : cocos2d::CCEaseBounce {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a3cd0, ios 0x1a820c;
	virtual auto reverse() = mac 0x2a3d90, ios 0x1a8238;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseElastic : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto reverse() = mac 0x2a2dd0;
	auto create(cocos2d::CCActionInterval* p0);
	auto create(cocos2d::CCActionInterval* p0, float p1);
	auto initWithAction(cocos2d::CCActionInterval* p0, float p1);
}

class cocos2d::CCEaseElasticIn : cocos2d::CCEaseElastic {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2fb0, ios 0x1a7878;
	virtual auto reverse() = mac 0x2a3050, ios 0x1a7904;
	auto create(cocos2d::CCActionInterval* p0);
	auto create(cocos2d::CCActionInterval* p0, float p1);
}

class cocos2d::CCEaseElasticInOut : cocos2d::CCEaseElastic {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a34f0, ios 0x1a7d00;
	virtual auto reverse() = mac 0x2a3620, ios 0x1a7e00;
	auto create(cocos2d::CCActionInterval* p0);
	auto create(cocos2d::CCActionInterval* p0, float p1);
}

class cocos2d::CCEaseElasticOut : cocos2d::CCEaseElastic {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCEaseElasticOut* create(cocos2d::CCActionInterval* p0, float p1) = mac 0x2a3080;
	virtual auto update(float p0) = mac 0x2a3250, ios 0x1a7abc;
	virtual auto reverse() = mac 0x2a32f0, ios 0x1a7b54;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseExponentialIn : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2160, ios 0x1a6d3c;
	virtual auto reverse() = mac 0x2a21b0, ios 0x1a6da4;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseExponentialInOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2560, ios 0x1a7100;
	virtual auto reverse() = mac 0x2a25d0, ios 0x1a716c;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseExponentialOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2360, ios 0x1a6f24;
	virtual auto reverse() = mac 0x2a23b0, ios 0x1a6f80;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseIn : cocos2d::CCEaseRateAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCEaseIn* create(cocos2d::CCActionInterval* p0, float p1) = mac 0x2a1960;
	virtual auto update(float p0) = mac 0x2a1b10, ios 0x1a6740;
	virtual auto reverse() = mac 0x2a1b40, ios 0x1a6780;
}

class cocos2d::CCEaseInOut : cocos2d::CCEaseRateAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCEaseInOut* create(cocos2d::CCActionInterval* p0, float p1) = mac 0x2a1d80;
	virtual auto update(float p0) = mac 0x2a1f30, ios 0x1a6b30;
	virtual auto reverse() = mac 0x2a1fa0, ios 0x1a6bac;
}

class cocos2d::CCEaseOut : cocos2d::CCEaseRateAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCEaseOut* create(cocos2d::CCActionInterval* p0, float p1) = mac 0x2a1b70;
	virtual auto update(float p0) = mac 0x2a1d20, ios 0x1a6934;
	virtual auto reverse() = mac 0x2a1d50, ios 0x1a697c;
}

class cocos2d::CCEaseRateAction : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto reverse() = mac 0x2a1930;
	auto create(cocos2d::CCActionInterval* p0, float p1);
	auto initWithAction(cocos2d::CCActionInterval* p0, float p1);
	~CCEaseRateAction() = mac 0x2a1880;
}

class cocos2d::CCEaseSineIn : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2780, ios 0x1a72ec;
	virtual auto reverse() = mac 0x2a27c0, ios 0x1a733c;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseSineInOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2b50, ios 0x1a7684;
	virtual auto reverse() = mac 0x2a2b90, ios 0x1a76dc;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseSineOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2970, ios 0x1a74bc;
	virtual auto reverse() = mac 0x2a29a0, ios 0x1a7504;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCFadeIn : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f7c80, ios 0xf2e34;
	virtual auto reverse() = mac 0x1f7ce0, ios 0xf2e98;
	auto create(float p0);
}

class cocos2d::CCFadeOut : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCFadeOut* create(float p0) = mac 0x1f7d80;
	virtual auto update(float p0) = mac 0x1f7ee0, ios 0xf3000;
	virtual auto reverse() = mac 0x1f7f50, ios 0xf306c;
}

class cocos2d::CCFadeOutBLTiles : cocos2d::CCFadeOutTRTiles {
	virtual auto testFunc(cocos2d::CCSize const& p0, float p1) = mac 0x45cf10;
	auto create(float p0, cocos2d::CCSize const& p1);
}

class cocos2d::CCFadeOutDownTiles : cocos2d::CCFadeOutUpTiles {
	virtual auto testFunc(cocos2d::CCSize const& p0, float p1) = mac 0x45d280;
	auto create(float p0, cocos2d::CCSize const& p1);
}

class cocos2d::CCFadeOutTRTiles : cocos2d::CCTiledGrid3DAction {
	virtual auto update(float p0) = mac 0x45ccc0;
	virtual auto testFunc(cocos2d::CCSize const& p0, float p1) = mac 0x45cad0;
	virtual auto transformTile(cocos2d::CCPoint const& p0, float p1) = mac 0x45cbc0;
	auto create(float p0, cocos2d::CCSize const& p1);
	auto turnOffTile(cocos2d::CCPoint const& p0);
	auto turnOnTile(cocos2d::CCPoint const& p0);
}

class cocos2d::CCFadeOutUpTiles : cocos2d::CCFadeOutTRTiles {
	virtual auto testFunc(cocos2d::CCSize const& p0, float p1) = mac 0x45d070;
	virtual auto transformTile(cocos2d::CCPoint const& p0, float p1) = mac 0x45d0f0;
	auto create(float p0, cocos2d::CCSize const& p1);
}

class cocos2d::CCFadeTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCFadeTo* create(float p0, unsigned char p1) = mac 0x1f7ff0;
	virtual auto update(float p0) = mac 0x1f8220, ios 0xf3274;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f81c0, ios 0xf3210;
	auto initWithDuration(float p0, unsigned char p1);
}

class cocos2d::CCFileUtils : cocos2d::TypeInfo {
	static cocos2d::CCFileUtils* sharedFileUtils() = mac 0x377030, ios 0x159450;
	virtual auto getClassTypeInfo();
	virtual auto purgeCachedEntries() = mac 0x23f100, ios 0xcdd6c;
	virtual auto getFileData(char const* p0, char const* p1, unsigned long* p2) = mac 0x23f140, ios 0xcdd9c;
	virtual auto getFileDataFromZip(char const* p0, char const* p1, unsigned long* p2) = mac 0x23f2e0, ios 0xcdf0c;
	virtual auto fullPathForFilename(char const* p0, bool p1) = mac 0x23f940, ios 0xce604;
	virtual auto removeFullPath(char const* p0) = mac 0x240710, ios 0xcf170;
	virtual auto loadFilenameLookupDictionaryFromFile(char const* p0) = mac 0x2416f0, ios 0xd0084;
	virtual auto setFilenameLookupDictionary(cocos2d::CCDictionary* p0) = mac 0x241680, ios 0xd001c;
	virtual auto fullPathFromRelativeFile(char const* p0, char const* p1) = mac 0x2408c0, ios 0xcf270;
	virtual auto setSearchResolutionsOrder(gd::vector<gd::string>const& p0) = mac 0x240ac0, ios 0xcf43c;
	virtual auto addSearchResolutionsOrder(char const* p0) = mac 0x240d30, ios 0xcf658;
	virtual auto getSearchResolutionsOrder() = mac 0x240df0, ios 0xcf75c;
	virtual auto setSearchPaths(gd::vector<gd::string>const& p0) = mac 0x240e10, ios 0xcf76c;
	virtual auto addSearchPath(char const* p0) = mac 0x241180, ios 0xcfb6c;
	virtual auto removeSearchPath(char const* p0) = mac 0x241390, ios 0xcfd50;
	virtual auto getSearchPaths() = mac 0x240e00, ios 0xcf764;
	virtual auto getWritablePath2() = mac 0x241a90, ios 0xd03b0;
	virtual auto isAbsolutePath(gd::string const& p0) = mac 0x2419c0, ios 0xd02ac;
	virtual auto setPopupNotify(bool p0) = mac 0x2419d0, ios 0xd02cc;
	virtual auto isPopupNotify() = mac 0x2419e0, ios 0xd02d8;
	virtual auto init() = mac 0x23f010, ios 0xcdcb4;
	virtual auto getNewFilename(char const* p0) = mac 0x23f3f0, ios 0xce02c;
	virtual auto shouldUseHD() = mac 0x23f4e0, ios 0xce1a0;
	virtual auto addSuffix(gd::string p0, gd::string p1) = mac 0x23f510, ios 0xce1c4;
	virtual auto getPathForFilename(gd::string const& p0, gd::string const& p1, gd::string const& p2) = mac 0x23f650, ios 0xce3d8;
	virtual auto getFullPathForDirectoryAndFilename(gd::string const& p0, gd::string const& p1) = mac 0x241900, ios 0xd023c;
	virtual auto createCCDictionaryWithContentsOfFile(gd::string const& p0) = mac 0x23ec30, ios 0xcdb08;
	virtual auto writeToFile(cocos2d::CCDictionary* p0, gd::string const& p1) = mac 0x23ec40, ios 0xcdb10;
	virtual auto createCCArrayWithContentsOfFile(gd::string const& p0) = mac 0x23ec50, ios 0xcdb18;
	auto CCFileUtils();
	auto purgeFileUtils();
	auto removeAllPaths();
	~CCFileUtils() = mac 0x23efd0, ios 0xcdc78;
}

class cocos2d::CCFiniteTimeAction : cocos2d::CCAction {
	virtual auto reverse();
	~CCFiniteTimeAction();
}

class cocos2d::CCFlipX : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x454910;
	virtual auto reverse() = mac 0x454930;
	auto create(bool p0);
	auto initWithFlipX(bool p0);
}

class cocos2d::CCFlipX3D : cocos2d::CCGrid3DAction {
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0);
	auto initWithDuration(float p0);
	auto initWithSize(cocos2d::CCSize const& p0, float p1);
	auto update(float p0);
}

class cocos2d::CCFlipY : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x454ab0;
	virtual auto reverse() = mac 0x454ad0;
	auto create(bool p0);
	auto initWithFlipY(bool p0);
}

class cocos2d::CCFlipY3D : cocos2d::CCFlipX3D {
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0);
	auto update(float p0);
}

class cocos2d::CCFollow : cocos2d::CCAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto isDone() = mac 0x35c2e0;
	virtual auto stop() = mac 0x35c300;
	virtual auto step(float p0) = mac 0x35c1f0;
	auto create(cocos2d::CCNode* p0, cocos2d::CCRect const& p1);
	auto initWithTarget(cocos2d::CCNode* p0, cocos2d::CCRect const& p1);
}

class cocos2d::CCGLProgram : cocos2d::CCObject {
	auto CCGLProgram();
	auto addAttribute(char const* p0, unsigned int p1);
	auto compileShader(unsigned int* p0, unsigned int p1, char const* p2);
	auto description();
	auto fragmentShaderLog();
	auto getUniformLocationForName(char const* p0);
	auto initWithVertexShaderByteArray(char const* p0, char const* p1);
	auto initWithVertexShaderFilename(char const* p0, char const* p1);
	auto link();
	auto programLog();
	auto reset();
	auto setUniformLocationWith1f(int p0, float p1);
	auto setUniformLocationWith1i(int p0, int p1);
	auto setUniformLocationWith2f(int p0, float p1, float p2);
	auto setUniformLocationWith2fv(int p0, float* p1, unsigned int p2);
	auto setUniformLocationWith2i(int p0, int p1, int p2);
	auto setUniformLocationWith2iv(int p0, int* p1, unsigned int p2);
	auto setUniformLocationWith3f(int p0, float p1, float p2, float p3);
	auto setUniformLocationWith3fv(int p0, float* p1, unsigned int p2);
	auto setUniformLocationWith3i(int p0, int p1, int p2, int p3);
	auto setUniformLocationWith3iv(int p0, int* p1, unsigned int p2);
	auto setUniformLocationWith4f(int p0, float p1, float p2, float p3, float p4);
	auto setUniformLocationWith4fv(int p0, float* p1, unsigned int p2);
	auto setUniformLocationWith4i(int p0, int p1, int p2, int p3, int p4);
	auto setUniformLocationWith4iv(int p0, int* p1, unsigned int p2);
	auto setUniformLocationWithMatrix3fv(int p0, float* p1, unsigned int p2);
	auto setUniformLocationWithMatrix4fv(int p0, float* p1, unsigned int p2);
	auto setUniformsForBuiltins();
	auto updateUniformLocation(int p0, void* p1, unsigned int p2);
	auto updateUniforms();
	auto use();
	auto vertexShaderLog();
	~CCGLProgram() = mac 0x231950, ios 0xbeadc;
}

class cocos2d::CCGrabber : cocos2d::CCObject {
	auto CCGrabber();
	auto afterRender(cocos2d::CCTexture2D* p0);
	auto beforeRender(cocos2d::CCTexture2D* p0);
	auto grab(cocos2d::CCTexture2D* p0);
	~CCGrabber() = mac 0x360fe0;
}

class cocos2d::CCGrid3D : cocos2d::CCGridBase {
	virtual auto blit() = mac 0x2884b0;
	virtual auto reuse() = mac 0x288da0;
	virtual auto calculateVertexPoints() = mac 0x288550;
	auto CCGrid3D();
	auto create(cocos2d::CCSize const& p0);
	auto create(cocos2d::CCSize const& p0, cocos2d::CCTexture2D* p1, bool p2);
	auto originalVertex(cocos2d::CCPoint const& p0);
	auto setVertex(cocos2d::CCPoint const& p0, cocos2d::_ccVertex3F const& p1);
	auto vertex(cocos2d::CCPoint const& p0);
	~CCGrid3D() = mac 0x288470;
}

class cocos2d::CCGrid3DAction : cocos2d::CCGridAction {
	virtual auto getGrid() = mac 0xee2b0;
	auto CCGrid3DAction();
	auto originalVertex(cocos2d::CCPoint const& p0);
	auto setVertex(cocos2d::CCPoint const& p0, cocos2d::_ccVertex3F const& p1);
	auto vertex(cocos2d::CCPoint const& p0);
}

class cocos2d::CCGridAction : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xee0d0;
	virtual auto reverse() = mac 0xee1a0;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1) = mac 0xee0a0;
	virtual auto getGrid() = mac 0xee190;
	auto CCGridAction();
	auto create(float p0, cocos2d::CCSize const& p1);
}

class cocos2d::CCGridBase : cocos2d::CCObject {
	virtual auto blit() = mac 0x2880e0;
	virtual auto reuse() = mac 0x2880f0;
	virtual auto calculateVertexPoints() = mac 0x288100;
	auto CCGridBase();
	auto afterDraw(cocos2d::CCNode* p0);
	auto beforeDraw();
	auto create(cocos2d::CCSize const& p0);
	auto create(cocos2d::CCSize const& p0, cocos2d::CCTexture2D* p1, bool p2);
	auto initWithSize(cocos2d::CCSize const& p0);
	auto initWithSize(cocos2d::CCSize const& p0, cocos2d::CCTexture2D* p1, bool p2);
	auto set2DProjection();
	auto setActive(bool p0);
	auto setTextureFlipped(bool p0);
	~CCGridBase() = mac 0x287e30;
}

class cocos2d::CCHide : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCHide* create() = mac 0x4543e0;
	virtual auto update(float p0);
	virtual auto reverse();
}

class cocos2d::CCIMEDelegate {
	inline cocos2d::CCIMEDelegate::~CCIMEDelegate() {
	    CCIMEDispatcher::sharedDispatcher()->removeDelegate(this);
	}
	inline cocos2d::CCIMEDelegate::CCIMEDelegate() {
		CCIMEDispatcher::sharedDispatcher()->addDelegate(this);
	}

	virtual auto attachWithIME() = mac 0x2776a0, ios 0x12d3d4;
	virtual auto detachWithIME() = mac 0x277880, ios 0x12d4e8;
	virtual auto canAttachWithIME();
	virtual auto didAttachWithIME();
	virtual auto canDetachWithIME();
	virtual auto didDetachWithIME();
	virtual auto insertText(char const* p0, int p1);
	virtual auto deleteBackward();
	virtual auto getContentText();
	virtual auto keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& p0);
	virtual auto keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo& p0);
	virtual auto keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& p0);
	virtual auto keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo& p0);
	auto CCIMEDelegate();
	~CCIMEDelegate();
}

class cocos2d::CCIMEDispatcher {
	static auto sharedDispatcher() = mac 0x2773f0, ios 0x12d170;
	auto addDelegate(cocos2d::CCIMEDelegate* p0) = mac 0x277480, ios 0x12d204;
	auto removeDelegate(cocos2d::CCIMEDelegate* p0) = mac 0x2775f0, ios 0x12d2c4;
}

class cocos2d::CCImage : cocos2d::CCObject {
	CCImage() = mac 0x24fa00;
	~CCImage() = mac 0x24fa80, ios 0x1e598;
	auto initWithImageData(void* p0, int p1, cocos2d::CCImage::EImageFormat p2, int p3, int p4, int p5) = mac 0x24fcb0;
	auto _initWithJpgData(void* p0, int p1);
	auto _initWithPngData(void* p0, int p1);
	auto _initWithRawData(void* p0, int p1, int p2, int p3, int p4, bool p5);
	auto _initWithTiffData(void* p0, int p1);
	auto _initWithWebpData(void* p0, int p1);
	auto _saveImageToJPG(char const* p0);
	auto _saveImageToPNG(char const* p0, bool p1);
	auto initWithImageFile(char const* p0, cocos2d::CCImage::EImageFormat p1);
	auto initWithImageFileThreadSafe(char const* p0, cocos2d::CCImage::EImageFormat p1);
	auto initWithString(char const* p0, int p1, int p2, cocos2d::CCImage::ETextAlign p3, char const* p4, int p5);
	auto initWithStringShadowStroke(char const* p0, int p1, int p2, cocos2d::CCImage::ETextAlign p3, char const* p4, int p5, float p6, float p7, float p8, bool p9, float p10, float p11, float p12, float p13, bool p14, float p15, float p16, float p17, float p18);
	auto saveToFile(char const* p0, bool p1);
}

class cocos2d::CCInteger : cocos2d::CCObject {
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0);
	auto create(int p0);
	~CCInteger();
}

class cocos2d::CCJumpBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f6160;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f6110;
	virtual auto reverse() = mac 0x1f6290;
	auto CCJumpBy();
	auto create(float p0, cocos2d::CCPoint const& p1, float p2, unsigned int p3);
	auto initWithDuration(float p0, cocos2d::CCPoint const& p1, float p2, unsigned int p3);
}

class cocos2d::CCJumpTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual auto update(float p0) = mac 0x45df60;
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
}

class cocos2d::CCJumpTo : cocos2d::CCJumpBy {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f6570;
	auto create(float p0, cocos2d::CCPoint const& p1, float p2, int p3);
}

class cocos2d::CCKeyboardDelegate {
	virtual auto keyDown(cocos2d::enumKeyCodes p0);
	virtual auto keyUp(cocos2d::enumKeyCodes p0);
}

class cocos2d::CCKeyboardDispatcher : cocos2d::CCObject {
	auto dispatchKeyboardMSG(cocos2d::enumKeyCodes p0, bool p1) = mac 0xe8190;
	auto keyToString(cocos2d::enumKeyCodes p0) = mac 0xe8450;
	auto CCKeyboardDispatcher();
	auto addDelegate(cocos2d::CCKeyboardDelegate* p0);
	auto convertKeyCode(cocos2d::enumKeyCodes p0);
	auto forceAddDelegate(cocos2d::CCKeyboardDelegate* p0);
	auto forceRemoveDelegate(cocos2d::CCKeyboardDelegate* p0);
	auto removeDelegate(cocos2d::CCKeyboardDelegate* p0);
	auto updateModifierKeys(bool p0, bool p1, bool p2, bool p3);
	~CCKeyboardDispatcher() = mac 0xe7f90, ios 0x15d8ac;
}

class cocos2d::CCKeyboardHandler : cocos2d::CCObject {
	static cocos2d::CCKeyboardHandler* handlerWithDelegate(cocos2d::CCKeyboardDelegate* p0) = mac 0x242030;
	virtual auto initWithDelegate(cocos2d::CCKeyboardDelegate* p0) = mac 0x241ff0, ios 0x13f8b8;
	~CCKeyboardHandler() = mac 0x241e90, ios 0x13f87c;
	auto getDelegate();
	auto setDelegate(cocos2d::CCKeyboardDelegate* p0);
}

class cocos2d::CCKeypadDelegate {
	virtual auto keyBackClicked();
	virtual auto keyMenuClicked();
}

class cocos2d::CCKeypadDispatcher : cocos2d::CCObject {
	auto CCKeypadDispatcher();
	auto addDelegate(cocos2d::CCKeypadDelegate* p0);
	auto dispatchKeypadMSG(cocos2d::ccKeypadMSGType p0);
	auto forceAddDelegate(cocos2d::CCKeypadDelegate* p0);
	auto forceRemoveDelegate(cocos2d::CCKeypadDelegate* p0);
	auto removeDelegate(cocos2d::CCKeypadDelegate* p0);
	~CCKeypadDispatcher() = mac 0x360110, ios 0x1120e0;
}

class cocos2d::CCKeypadHandler : cocos2d::CCObject {
	static cocos2d::CCKeypadHandler* handlerWithDelegate(cocos2d::CCKeypadDelegate* p0) = mac 0x1ff2d0;
	virtual auto initWithDelegate(cocos2d::CCKeypadDelegate* p0) = mac 0x1ff290, ios 0x69;
	~CCKeypadHandler() = mac 0x1ff130, ios 0x2c530;
	cocos2d::CCKeypadDelegate* getDelegate() = mac 0x1ff0b0;
	auto setDelegate(cocos2d::CCKeypadDelegate* p0);
}

class cocos2d::CCLabelAtlas : cocos2d::CCAtlasNode, cocos2d::CCLabelProtocol {
	virtual auto updateAtlasValues() = mac 0x43d590;
	virtual auto setString(char const* p0) = mac 0x43d840;
	virtual auto getString() = mac 0x43d930;
	auto create(char const* p0, char const* p1);
	auto create(char const* p0, char const* p1, unsigned int p2, unsigned int p3, unsigned int p4);
	auto initWithString(char const* p0, cocos2d::CCTexture2D* p1, unsigned int p2, unsigned int p3, unsigned int p4);
	auto initWithString(char const* p0, char const* p1);
	auto initWithString(char const* p0, char const* p1, unsigned int p2, unsigned int p3, unsigned int p4);
}

class cocos2d::CCLabelBMFont : cocos2d::CCSpriteBatchNode, cocos2d::CCLabelProtocol, cocos2d::CCRGBAProtocol {
	static cocos2d::CCLabelBMFont* create(char const* p0, char const* p1) = mac 0x347660;
	auto limitLabelWidth(float p0, float p1, float p2) = mac 0x34a6e0, ios 0x21b740;
	auto setScale(float p0) = mac 0x34a5d0, ios 0x21b6bc;
	auto setString(char const* p0, bool p1) = mac 0x3489e0, ios 0x21a42c;
	auto setAnchorPoint(cocos2d::CCPoint const& p0) = mac 0x349440, ios 0x21aa7c;
	virtual auto init() = mac 0x347b10, ios 0x2198e0;
	virtual auto setScaleX(float p0) = mac 0x34a5b0, ios 0x21b6e8;
	virtual auto setScaleY(float p0) = mac 0x34a5d0, ios 0x21b714;
	virtual auto setString(char const* p0) = mac 0x348990, ios 0x21a408;
	virtual auto getString() = mac 0x348bf0, ios 0x21a60c;
	virtual auto setCString(char const* p0) = mac 0x348c10, ios 0x21a63c;
	virtual auto updateLabel() = mac 0x349480, ios 0x21aad0;
	virtual auto setAlignment(cocos2d::CCTextAlignment p0) = mac 0x34a530, ios 0x21b68c;
	virtual auto setWidth(float p0) = mac 0x34a550, ios 0x21b69c;
	virtual auto setLineBreakWithoutSpace(bool p0) = mac 0x34a570, ios 0x21b6ac;
	virtual auto isOpacityModifyRGB() = mac 0x3490e0, ios 0x21a8c4;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x348f70, ios 0x21a80c;
	virtual auto getOpacity() = mac 0x348df0, ios 0x21a748;
	virtual auto getDisplayedOpacity() = mac 0x348e10, ios 0x21a758;
	virtual auto setOpacity(unsigned char p0) = mac 0x348e30, ios 0x21a768;
	virtual auto updateDisplayedOpacity(unsigned char p0) = mac 0x349100, ios 0x21a8d4;
	virtual auto isCascadeOpacityEnabled() = mac 0x349400, ios 0x21aa5c;
	virtual auto setCascadeOpacityEnabled(bool p0) = mac 0x349420, ios 0x21aa6c;
	virtual auto getColor() = mac 0x348c30, ios 0x21a648;
	virtual auto getDisplayedColor() = mac 0x348c50, ios 0x21a658;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x348c70, ios 0x21a668;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const& p0) = mac 0x349210, ios 0x21a968;
	virtual auto isCascadeColorEnabled() = mac 0x3493c0, ios 0x21aa3c;
	virtual auto setCascadeColorEnabled(bool p0) = mac 0x3493e0, ios 0x21aa4c;
	virtual auto setString(unsigned short* p0, bool p1) = mac 0x348a60, ios 0x21a4b4;
	auto CCLabelBMFont();
	auto create(char const* p0, char const* p1, float p2);
	auto create(char const* p0, char const* p1, float p2, cocos2d::CCTextAlignment p3);
	auto create(char const* p0, char const* p1, float p2, cocos2d::CCTextAlignment p3, cocos2d::CCPoint p4);
	auto create();
	auto createBatched(char const* p0, char const* p1, cocos2d::CCArray* p2);
	auto createFontChars();
	auto getConfiguration();
	auto getFntFile();
	auto getLetterPosXLeft(cocos2d::CCSprite* p0);
	auto getLetterPosXRight(cocos2d::CCSprite* p0);
	auto initWithString(char const* p0, char const* p1, float p2, cocos2d::CCTextAlignment p3, cocos2d::CCPoint p4);
	auto kerningAmountForFirst(unsigned short p0, unsigned short p1);
	auto purgeCachedData();
	auto setFntFile(char const* p0);
	~CCLabelBMFont() = mac 0x347e80, ios 0x219afc;
}

class cocos2d::CCLabelProtocol {}

class cocos2d::CCLabelTTF : cocos2d::CCSprite, cocos2d::CCLabelProtocol {
	static cocos2d::CCLabelTTF* create() = mac 0x1fa7e0;
	static cocos2d::CCLabelTTF* create(char const* p0, char const* p1, float p2) = mac 0x1fa840;
	auto updateTexture() = mac 0x1fadc0;
	virtual auto setString(char const* p0) = mac 0x1fad70, ios 0x3a558;
	virtual auto init() = mac 0x1fac60, ios 0x3a51c;
	virtual auto getString() = mac 0x1faf70, ios 0x3a6b4;
	auto CCLabelTTF();
	auto _prepareTextDefinition(bool p0);
	auto _updateWithTextDefinition(cocos2d::_ccFontDefinition& p0, bool p1);
	auto create(char const* p0, char const* p1, float p2, cocos2d::CCSize const& p3, cocos2d::CCTextAlignment p4);
	auto create(char const* p0, char const* p1, float p2, cocos2d::CCSize const& p3, cocos2d::CCTextAlignment p4, cocos2d::CCVerticalTextAlignment p5);
	auto createWithFontDefinition(char const* p0, cocos2d::_ccFontDefinition& p1);
	auto description();
	auto disableShadow(bool p0);
	auto disableStroke(bool p0);
	auto enableShadow(cocos2d::CCSize const& p0, float p1, float p2, bool p3);
	auto enableStroke(cocos2d::ccColor3B const& p0, float p1, bool p2);
	auto getDimensions();
	auto getFontName();
	auto getFontSize();
	auto getHorizontalAlignment();
	auto getTextDefinition();
	auto getVerticalAlignment();
	auto initWithString(char const* p0, char const* p1, float p2);
	auto initWithString(char const* p0, char const* p1, float p2, cocos2d::CCSize const& p3, cocos2d::CCTextAlignment p4);
	auto initWithString(char const* p0, char const* p1, float p2, cocos2d::CCSize const& p3, cocos2d::CCTextAlignment p4, cocos2d::CCVerticalTextAlignment p5);
	auto initWithStringAndTextDefinition(char const* p0, cocos2d::_ccFontDefinition& p1);
	auto setDimensions(cocos2d::CCSize const& p0);
	auto setFontFillColor(cocos2d::ccColor3B const& p0, bool p1);
	auto setFontName(char const* p0);
	auto setFontSize(float p0);
	auto setHorizontalAlignment(cocos2d::CCTextAlignment p0);
	auto setTextDefinition(cocos2d::_ccFontDefinition* p0);
	auto setVerticalAlignment(cocos2d::CCVerticalTextAlignment p0);
	~CCLabelTTF() = mac 0x1fa620, ios 0x3a388;
}

class cocos2d::CCLayer : cocos2d::CCNode, cocos2d::CCTouchDelegate, cocos2d::CCAccelerometerDelegate, cocos2d::CCKeypadDelegate, cocos2d::CCKeyboardDelegate, cocos2d::CCMouseDelegate {
	CCLayer() = mac 0x2725b0, ios 0xc7708;
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2734d0, ios 0xc810c;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x273650, ios 0xc82ac;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2735d0, ios 0xc820c;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x273550, ios 0xc816c;
	virtual auto ccTouchesBegan(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x2736d0, ios 0xc834c;
	virtual auto ccTouchesCancelled(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x273850, ios 0xc852c;
	virtual auto ccTouchesEnded(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x2737d0, ios 0xc848c;
	virtual auto ccTouchesMoved(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x273750, ios 0xc83ec;
	static cocos2d::CCLayer* create() = mac 0x272a00;
	virtual auto didAccelerate(cocos2d::CCAcceleration* p0) = mac 0x272ea0, ios 0xc7c5c;
	virtual auto getTouchMode() = mac 0x272e10, ios 0xc7b9c;
	virtual auto getTouchPriority() = mac 0x272e00, ios 0xc7b94;
	virtual auto init() = mac 0x2729a0, ios 0xc7894;
	virtual auto isAccelerometerEnabled() = mac 0x272e20, ios 0xc7ba4;
	virtual auto isKeyboardEnabled() = mac 0x273010, ios 0xc7d70;
	virtual auto isKeypadEnabled() = mac 0x272f70, ios 0xc7cf4;
	virtual auto isMouseEnabled() = mac 0x273090, ios 0xc7de0;
	virtual auto isTouchEnabled() = mac 0x272ce0, ios 0xc7a54;
	virtual auto keyBackClicked() = mac 0x273160, ios 0xc7e50;
	virtual auto keyDown(cocos2d::enumKeyCodes p0) = mac 0x273280, ios 0xc7efc;
	virtual auto keyMenuClicked() = mac 0x273200, ios 0xc7eac;
	virtual auto onEnter() = mac 0x273300, ios 0xc7f38;
	virtual auto onEnterTransitionDidFinish() = mac 0x273490, ios 0xc80cc;
	virtual auto onExit() = mac 0x2733c0, ios 0xc8004;
	virtual auto registerScriptTouchHandler(int p0, bool p1, int p2, bool p3) = mac 0x272bd0, ios 0xc79c8;
	virtual auto registerWithTouchDispatcher() = mac 0x272b40, ios 0xc7958;
	virtual auto setAccelerometerEnabled(bool p0) = mac 0x272e30, ios 0xc7bac;
	virtual auto setAccelerometerInterval(double p0) = mac 0x272e70, ios 0xc7c10;
	virtual auto setKeyboardEnabled(bool p0) = mac 0x273020, ios 0xc7d78;
	virtual auto setKeypadEnabled(bool p0) = mac 0x272f80, ios 0xc7cfc;
	virtual auto setMouseEnabled(bool p0) = mac 0x2730a0, ios 0xc7de8;
	virtual auto setTouchEnabled(bool p0) = mac 0x272cf0, ios 0xc7a5c;
	virtual auto setTouchMode(cocos2d::ccTouchesMode p0) = mac 0x272d60, ios 0xc7acc;
	virtual auto setTouchPriority(int p0) = mac 0x272db0, ios 0xc7b30;
	virtual auto unregisterScriptTouchHandler() = mac 0x272c30, ios 0xc7a28;
	~CCLayer() = mac 0x2727b0, ios 0xc7848;
	auto excuteScriptTouchHandler(int p0, cocos2d::CCSet* p1);
	auto excuteScriptTouchHandler(int p0, cocos2d::CCTouch* p1);
	auto registerScriptAccelerateHandler(int p0);
	auto registerScriptKeypadHandler(int p0);
	auto unregisterScriptAccelerateHandler();
	auto unregisterScriptKeypadHandler();
}

class cocos2d::CCLayerColor : cocos2d::CCLayerRGBA, cocos2d::CCBlendProtocol {
	CCLayerColor() = mac 0x274320, ios 0xc8aec;
	static cocos2d::CCLayerColor* create(cocos2d::ccColor4B const& p0, float p1, float p2) = mac 0x2745e0;
	auto draw() = mac 0x274b50, ios 0xc8fe0;
	auto getBlendFunc() = mac 0x274480, ios 0xc8bcc;
	auto init() = mac 0x274800, ios 0xc8de8;
	auto initWithColor(cocos2d::ccColor4B const& p0) = mac 0x2749a0, ios 0xc8f14;
	auto initWithColor(cocos2d::ccColor4B const& p0, float p1, float p2) = mac 0x274850, ios 0xc8e34;
	auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x2744a0, ios 0xc8bdc;
	auto setColor(cocos2d::ccColor3B const& p0) = mac 0x274c20, ios 0xc90ac;
	auto setContentSize(cocos2d::CCSize const& p0) = mac 0x2749f0, ios 0xc8f64;
	auto setOpacity(unsigned char p0) = mac 0x274db0, ios 0xc9108;
	auto updateColor() = mac 0x274ae0, ios 0xc8f80;
	~CCLayerColor() = mac 0x2743d0, ios 0x2743e0;
	auto changeHeight(float p0);
	auto changeWidth(float p0);
	auto changeWidthAndHeight(float p0, float p1);
	auto create(cocos2d::ccColor4B const& p0);
	auto create();
}

class cocos2d::CCLayerGradient : cocos2d::CCLayerColor {
	virtual auto init() = mac 0x275280;
	virtual auto updateColor() = mac 0x2753c0;
	virtual auto initWithColor(cocos2d::ccColor4B const& p0, cocos2d::ccColor4B const& p1) = mac 0x2752b0;
	virtual auto initWithColor(cocos2d::ccColor4B const& p0, cocos2d::ccColor4B const& p1, cocos2d::CCPoint const& p2) = mac 0x275310;
	virtual auto getStartColor() = mac 0x275610;
	virtual auto setStartColor(cocos2d::ccColor3B const& p0) = mac 0x275620;
	virtual auto getEndColor() = mac 0x275670;
	virtual auto setEndColor(cocos2d::ccColor3B const& p0) = mac 0x275640;
	virtual auto getStartOpacity() = mac 0x2756a0;
	virtual auto setStartOpacity(unsigned char p0) = mac 0x275680;
	virtual auto getEndOpacity() = mac 0x2756d0;
	virtual auto setEndOpacity(unsigned char p0) = mac 0x2756b0;
	virtual auto getVector() = mac 0x275710;
	virtual auto setVector(cocos2d::CCPoint const& p0) = mac 0x2756e0;
	virtual auto setCompressedInterpolation(bool p0) = mac 0x275730;
	virtual auto isCompressedInterpolation() = mac 0x275720;
	auto CCLayerGradient();
	auto create(cocos2d::ccColor4B const& p0, cocos2d::ccColor4B const& p1);
	auto create(cocos2d::ccColor4B const& p0, cocos2d::ccColor4B const& p1, cocos2d::CCPoint const& p2);
	auto create();
}

class cocos2d::CCLayerMultiplex : cocos2d::CCLayer {
	auto CCLayerMultiplex();
	auto addLayer(cocos2d::CCLayer* p0);
	auto create();
	auto createWithArray(cocos2d::CCArray* p0);
	auto createWithLayer(cocos2d::CCLayer* p0);
	auto initWithArray(cocos2d::CCArray* p0);
	auto switchTo(unsigned int p0);
	auto switchToAndReleaseMe(unsigned int p0);
	~CCLayerMultiplex() = mac 0x275980;
}

class cocos2d::CCLayerRGBA : cocos2d::CCLayer, cocos2d::CCRGBAProtocol {
	CCLayerRGBA() = mac 0x2738d0, ios 0xc85cc;
	virtual auto init() = mac 0x273b40, ios 0xc8de8;
	virtual auto getOpacity() = mac 0x273be0, ios 0xc8698;
	virtual auto getDisplayedOpacity() = mac 0x273c00, ios 0xc8724;
	virtual auto setOpacity(unsigned char p0) = mac 0x273c20, ios 0xc8734;
	virtual auto updateDisplayedOpacity(unsigned char p0) = mac 0x273f20, ios 0xc88d8;
	virtual auto isCascadeOpacityEnabled() = mac 0x2741f0, ios 0xc8aac;
	virtual auto setCascadeOpacityEnabled(bool p0) = mac 0x274210, ios 0xc8abc;
	virtual auto getColor() = mac 0x273d60, ios 0xc87d8;
	virtual auto getDisplayedColor() = mac 0x273d80, ios 0xc87e8;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x273da0, ios 0xc87f8;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const& p0) = mac 0x2740b0, ios 0xc89a4;
	virtual auto isCascadeColorEnabled() = mac 0x274230, ios 0xc8acc;
	virtual auto setCascadeColorEnabled(bool p0) = mac 0x274250, ios 0xc8adc;
	~CCLayerRGBA() = mac 0x273aa0, ios 0xc77b0;
	virtual auto setOpacityModifyRGB(bool p0);
	virtual auto isOpacityModifyRGB();
}

class cocos2d::CCLens3D : cocos2d::CCGrid3DAction {
	virtual auto update(float p0) = mac 0x128140;
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint const& p2, float p3);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint const& p2, float p3);
	auto setPosition(cocos2d::CCPoint const& p0);
}

class cocos2d::CCLiquid : cocos2d::CCGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x128ed0;
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
}

class cocos2d::CCMenu : cocos2d::CCLayerRGBA {
	auto alignItemsHorizontallyWithPadding(float p0) = mac 0x4393e0, ios 0x132508;
	auto alignItemsVerticallyWithPadding(float p0) = mac 0x439190;
	static cocos2d::CCMenu* create() = mac 0x438720, ios 0x131ba4;
	static cocos2d::CCMenu* createWithArray(cocos2d::CCArray* p0) = mac 0x4387e0;
	static cocos2d::CCMenu* createWithItem(cocos2d::CCMenuItem* p0) = mac 0x438b80;
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0x438ba0, ios 0x131ec8;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0x438bb0, ios 0x131ecc;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x438bc0, ios 0x131ed0;
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x438d20, ios 0x131fe8;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x4390b0, ios 0x1322ac;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x438fd0, ios 0x1321bc;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x439050, ios 0x132244;
	virtual auto init() = mac 0x438b90, ios 0x131ec0;
	virtual auto registerWithTouchDispatcher() = mac 0x438cd0, ios 0x131f8c;
	virtual auto onExit() = mac 0x438bd0, ios 0x131ed4;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x438c20, ios 0x15e630;
	auto initWithArray(cocos2d::CCArray* p0) = mac 0x4389f0, ios 0x131d04;
	virtual auto setOpacityModifyRGB(bool p0);
	virtual auto isOpacityModifyRGB();
	virtual auto isEnabled();
	virtual auto setEnabled(bool p0);
	auto alignItemsHorizontally();
	auto alignItemsInColumnsWithArray(cocos2d::CCArray* p0);
	auto alignItemsInRowsWithArray(cocos2d::CCArray* p0);
	auto alignItemsVertically();
	auto itemForTouch(cocos2d::CCTouch* p0);
	auto setHandlerPriority(int p0);
}

class cocos2d::CCMenuItem : cocos2d::CCNodeRGBA {
	~CCMenuItem() = mac 0x1fb8e0, ios 0x2cdf4;
	virtual auto activate() = mac 0x1fba70, ios 0x2ceb0;
	virtual auto selected() = mac 0x1fb9e0, ios 0x2ce2e;
	virtual auto unselected() = mac 0x1fb9f0, ios 0x2ce38;
	virtual auto registerScriptTapHandler(int p0) = mac 0x1fba00, ios 0x2ce40;
	virtual auto unregisterScriptTapHandler() = mac 0x1fba30, ios 0x2ce70;
	virtual auto isEnabled() = mac 0x1fbaf0, ios 0x2cf34;
	virtual auto setEnabled(bool p0) = mac 0x1fbae0, ios 0x2cf2c;
	virtual auto isSelected() = mac 0x1fbb50, ios 0x2cf60;
	auto setTarget(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1) = mac 0x1fbb60;
	auto rect() = mac 0x1fbb00, ios 0x2cf3c;
	auto create(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1);
	auto create();
	auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1);
}

class cocos2d::CCMenuItemAtlasFont : cocos2d::CCMenuItemLabel {
	auto create(char const* p0, char const* p1, int p2, int p3, char p4);
	auto create(char const* p0, char const* p1, int p2, int p3, char p4, cocos2d::CCObject* p5, cocos2d::SEL_MenuHandler p6);
	auto initWithString(char const* p0, char const* p1, int p2, int p3, char p4, cocos2d::CCObject* p5, cocos2d::SEL_MenuHandler p6);
	~CCMenuItemAtlasFont();
}

class cocos2d::CCMenuItemFont : cocos2d::CCMenuItemLabel {
	auto create(char const* p0);
	auto create(char const* p0, cocos2d::CCObject* p1, cocos2d::SEL_MenuHandler p2);
	auto fontName();
	auto fontNameObj();
	auto fontSize();
	auto fontSizeObj();
	auto initWithString(char const* p0, cocos2d::CCObject* p1, cocos2d::SEL_MenuHandler p2);
	auto recreateLabel();
	auto setFontName(char const* p0);
	auto setFontNameObj(char const* p0);
	auto setFontSize(unsigned int p0);
	auto setFontSizeObj(unsigned int p0);
	~CCMenuItemFont();
}

class cocos2d::CCMenuItemImage : cocos2d::CCMenuItemSprite {
	virtual auto init() = mac 0x1fd750;
	auto CCMenuItemImage();
	auto create(char const* p0, char const* p1);
	auto create(char const* p0, char const* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3);
	auto create(char const* p0, char const* p1, char const* p2);
	auto create(char const* p0, char const* p1, char const* p2, cocos2d::CCObject* p3, cocos2d::SEL_MenuHandler p4);
	auto create();
	auto initWithNormalImage(char const* p0, char const* p1, char const* p2, cocos2d::CCObject* p3, cocos2d::SEL_MenuHandler p4);
	auto setDisabledSpriteFrame(cocos2d::CCSpriteFrame* p0);
	auto setNormalSpriteFrame(cocos2d::CCSpriteFrame* p0);
	auto setSelectedSpriteFrame(cocos2d::CCSpriteFrame* p0);
}

class cocos2d::CCMenuItemLabel : cocos2d::CCMenuItem {
	~CCMenuItemLabel() = mac 0x1fc0d0;
	virtual auto activate() = mac 0x1fc240;
	virtual auto selected() = mac 0x1fc2e0;
	virtual auto unselected() = mac 0x1fc380;
	virtual auto setEnabled(bool p0) = mac 0x1fc3f0;
	virtual auto getDisabledColor() = mac 0x1fbb80;
	virtual auto setDisabledColor(cocos2d::ccColor3B const& p0) = mac 0x1fbb90;
	virtual auto getLabel() = mac 0x1fbbb0;
	virtual auto setLabel(cocos2d::CCNode* p0) = mac 0x1fbbc0;
	auto CCMenuItemLabel();
	auto create(cocos2d::CCNode* p0);
	auto create(cocos2d::CCNode* p0, cocos2d::CCObject* p1, cocos2d::SEL_MenuHandler p2);
	auto initWithLabel(cocos2d::CCNode* p0, cocos2d::CCObject* p1, cocos2d::SEL_MenuHandler p2);
	auto setString(char const* p0);
}

class cocos2d::CCMenuItemSprite : cocos2d::CCMenuItem {
	virtual auto selected() = mac 0x1fd3f0, ios 0x02d2cc;
	virtual auto unselected() = mac 0x1fd470, ios 0x2d34c;
	virtual auto setEnabled(bool p0) = mac 0x1fd4e0, ios 0x2d3bc;
	virtual auto getNormalImage() = mac 0x1fcee0, ios 0x2cf68;
	virtual auto setNormalImage(cocos2d::CCNode* p0) = mac 0x1fcef0, ios 0x2cf70;
	virtual auto getSelectedImage() = mac 0x1fcfc0, ios 0x2d044;
	virtual auto setSelectedImage(cocos2d::CCNode* p0) = mac 0x1fcfd0, ios 0x2d04;
	virtual auto getDisabledImage() = mac 0x1fd070, ios 0x2d0fc;
	virtual auto setDisabledImage(cocos2d::CCNode* p0) = mac 0x1fd080, ios 0x2d104;
	virtual auto updateImagesVisibility() = mac 0x1fd510, ios 0x2d3dc;
	bool initWithNormalSprite(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCNode* p2, cocos2d::CCObject* p3, cocos2d::SEL_MenuHandler p4) = mac 0x1fd2f0;
	static auto create(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCNode* p2) = mac 0x1fd120;
	static auto create(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCNode* p2, cocos2d::CCObject* p3, cocos2d::SEL_MenuHandler p4) = mac 0x1fd140;
	static auto create(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3) = mac 0x1fd2d0;
}

class cocos2d::CCMenuItemToggle : cocos2d::CCMenuItem {
	virtual auto activate() = mac 0x1fe950;
	virtual auto selected() = mac 0x1fe8f0;
	virtual auto unselected() = mac 0x1fe920;
	virtual auto setEnabled(bool p0) = mac 0x1fea00;
	virtual auto getSelectedIndex() = mac 0x1fe8e0;
	virtual auto setSelectedIndex(unsigned int p0) = mac 0x1fe7e0;
	virtual auto getSubItems() = mac 0x1fde50;
	virtual auto setSubItems(cocos2d::CCArray* p0) = mac 0x1fde10;
	auto CCMenuItemToggle();
	auto addSubItem(cocos2d::CCMenuItem* p0);
	auto create(cocos2d::CCMenuItem* p0);
	auto create();
	auto createWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCArray* p2);
	auto initWithItem(cocos2d::CCMenuItem* p0);
	auto selectedItem();
	~CCMenuItemToggle() = mac 0x1fe7a0;
}

class cocos2d::CCMotionStreak : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	auto reset() = mac 0x2ee190;
	auto resumeStroke() = mac 0x2edb30;
	auto stopStroke() = mac 0x2edb20;
	bool initWithFade(float fade, float minSeg, float stroke, cocos2d::ccColor3B const& color, cocos2d::CCTexture2D* texture) = mac 0x2ed6f0;
	virtual auto update(float p0) = mac 0x2edb40, ios 0xee07c;
	virtual auto setPosition(cocos2d::CCPoint const& p0) = mac 0x2ed8b0, ios 0xedf5c;
	virtual auto draw() = mac 0x2ee1b0, ios 0xee574;
	virtual auto getOpacity() = mac 0x2eda60, ios 0xedff4;
	virtual auto setOpacity(unsigned char p0) = mac 0x2eda40, ios 0xedfec;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x2eda80, ios 0xee004;
	virtual auto isOpacityModifyRGB() = mac 0x2edaa0, ios 0xee00c;
	virtual auto getTexture() = mac 0x2ed950, ios 0xedf6c;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x2ed970, ios 0xedf7c;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x2eda00, ios 0xedfcc;
	virtual auto getBlendFunc() = mac 0x2eda20, ios 0xedfdc;
	auto CCMotionStreak();
	auto create(float p0, float p1, float p2, cocos2d::ccColor3B const& p3, char const* p4);
	auto create(float p0, float p1, float p2, cocos2d::ccColor3B const& p3, cocos2d::CCTexture2D* p4);
	auto enableRepeatMode(float p0);
	auto initWithFade(float p0, float p1, float p2, cocos2d::ccColor3B const& p3, char const* p4);
	auto setStroke(float p0);
	auto tintWithColor(cocos2d::ccColor3B p0);
	~CCMotionStreak() = mac 0x2ed330, ios 0xedce8;
}

class cocos2d::CCMouseDelegate {
	virtual auto rightKeyDown();
	virtual auto rightKeyUp();
	virtual auto scrollWheel(float p0, float p1);
}

class cocos2d::CCMouseDispatcher : cocos2d::CCObject {
	bool dispatchScrollMSG(float x, float y);
	auto removeDelegate(cocos2d::CCMouseDelegate* delegate);
	auto CCMouseDispatcher();
	auto addDelegate(cocos2d::CCMouseDelegate* p0);
	auto forceAddDelegate(cocos2d::CCMouseDelegate* p0);
	auto forceRemoveDelegate(cocos2d::CCMouseDelegate* p0);
	~CCMouseDispatcher() = mac 0x2e8d40, ios 0x21cc20;
}

class cocos2d::CCMouseHandler : cocos2d::CCObject {
	static cocos2d::CCMouseHandler* handlerWithDelegate(cocos2d::CCMouseDelegate* p0) = mac 0x12ef80;
	virtual auto initWithDelegate(cocos2d::CCMouseDelegate* p0) = mac 0x12ef40, ios 0x43798;
	~CCMouseHandler() = mac 0x12ede0, ios 0x4375c;
	auto getDelegate();
	auto setDelegate(cocos2d::CCMouseDelegate* p0);
}

class cocos2d::CCMoveBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCMoveBy* create(float p0, cocos2d::CCPoint const& p1) = mac 0x1f50e0;
	virtual auto update(float p0) = mac 0x1f5400, ios 0xf1c24;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f5360, ios 0xf1b8c;
	virtual auto reverse() = mac 0x1f53b0, ios 0xf1be0;
	auto CCMoveBy();
	auto initWithDuration(float p0, cocos2d::CCPoint const& p1);
}

class cocos2d::CCMoveTo : cocos2d::CCMoveBy {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCMoveTo* create(float p0, cocos2d::CCPoint const& p1) = mac 0x1f54d0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f5740, ios 0xf1ef8;
	auto initWithDuration(float p0, cocos2d::CCPoint const& p1);
}

class cocos2d::CCNode : cocos2d::CCObject {
	CCNode() = mac 0x122550;
	virtual auto _setZOrder(int p0) = mac 0x122990, ios 0x15dd7c;
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0x1233d0, ios 0x15e5d4;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0x1233b0, ios 0x15e5c4;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x1232a0, ios 0x15e4e4;
	virtual auto addComponent(cocos2d::CCComponent* p0) = mac 0x124a40, ios 0x15f6a4;
	virtual auto cleanup() = mac 0x123100, ios 0x15e3a4;
	virtual auto convertToNodeSpace(cocos2d::CCPoint const& p0) = mac 0x124750, ios 0x15f55c;
	virtual auto convertToWorldSpace(cocos2d::CCPoint const& p0) = mac 0x124790;
	static cocos2d::CCNode* create() = mac 0x1230a0;
	virtual auto draw() = mac 0x123840, ios 0x15e974;
	virtual auto getActionByTag(int p0) = mac 0x123ee0;
	virtual auto getActionManager() = mac 0x123e50, ios 0x15ef54;
	virtual auto getAnchorPoint() = mac 0x122d80, ios 0x15e090;
	virtual auto getAnchorPointInPoints() = mac 0x122d70, ios 0x15e088;
	virtual auto getCamera() = mac 0x122cb0, ios 0x15dfe4;
	virtual auto getChildByTag(int p0) = mac 0x123220, ios 0x15e460;
	virtual auto getChildren() = mac 0x122c80, ios 0x15dfcc;
	virtual auto getChildrenCount() const = mac 0x122c90, ios 0x15dfd4;
	virtual auto getContentSize() const = mac 0x122e00, ios 0x15e118;
	virtual auto getGLServerState() = mac 0x122f90, ios 0x15e248;
	virtual auto getGrid() = mac 0x122d00, ios 0x15e034;
	virtual auto getOrderOfArrival() = mac 0x122f50, ios 0x15e228;
	virtual auto getParent() = mac 0x122ed0, ios 0x15e1e0;
	virtual auto getPosition() = mac 0x122b60, ios 0x15debc;
	virtual auto getPosition(float* p0, float* p1) = mac 0x122b90, ios 0x15dedc;
	virtual auto getPositionX() = mac 0x122be0, ios 0x15df30;
	virtual auto getPositionY() = mac 0x122bf0, ios 0x15df38;
	virtual auto getRotation() = mac 0x122a00, ios 0x15dde4;
	virtual auto getRotationX() = mac 0x122a50, ios 0x15de1c;
	virtual auto getRotationY() = mac 0x122a80, ios 0x15de38;
	virtual auto getScale() = mac 0x122ab0, ios 0x15de54;
	virtual auto getScaleX() = mac 0x122b00, ios 0x15de84;
	virtual auto getScaleY() = mac 0x122b30, ios 0x15dea0;
	virtual auto getScaledContentSize() = mac 0x122e10, ios 0x15e120;
	virtual auto getScheduler() = mac 0x123f70, ios 0x15f008;
	virtual auto getShaderProgram() = mac 0x122f70, ios 0x15e238;
	virtual auto getSkewX() = mac 0x122920, ios 0x15dd3c;
	virtual auto getSkewY() = mac 0x122950, ios 0x15dd58;
	virtual auto getUserData() = mac 0x122f30, ios 0x15e218;
	virtual auto getUserObject() = mac 0x122f80, ios 0x15e240;
	virtual auto getVertexZ() = mac 0x1229e0, ios 0x15ddd4;
	virtual auto getZOrder() = mac 0x122980, ios 0x15dd74;
	virtual auto ignoreAnchorPointForPosition(bool p0) = mac 0x122f00, ios 0x15e1f8;
	virtual auto init() = mac 0x122910, ios 0x15dd34;
	virtual auto isIgnoreAnchorPointForPosition() = mac 0x122ef0, ios 0x15e1f0;
	virtual auto isRunning() = mac 0x122ec0, ios 0x15e1d8;
	virtual auto isVisible() = mac 0x122d50, ios 0x15e078;
	virtual auto nodeToParentTransform() = mac 0x124210, ios 0x15f1a4;
	virtual auto nodeToWorldTransform() = mac 0x124670, ios 0x15f490;
	virtual auto onEnter() = mac 0x123a90, ios 0x15eb94;
	virtual auto onEnterTransitionDidFinish() = mac 0x123b90, ios 0x15ec94;
	virtual auto onExit() = mac 0x123ca0, ios 0x15edb0;
	virtual auto onExitTransitionDidStart() = mac 0x123c00, ios 0x15ed18;
	virtual auto parentToNodeTransform() = mac 0x1245d0, ios 0x15f410;
	virtual auto pauseSchedulerAndActions() = mac 0x123d60;
	virtual auto registerScriptHandler(int p0) = mac 0x123d90, ios 0x15ee94;
	virtual auto removeAllChildren() = mac 0x123600, ios 0x15e788;
	virtual auto removeAllChildrenWithCleanup(bool p0) = mac 0x123620, ios 0x15e798;
	virtual auto removeAllComponents() = mac 0x124aa0, ios 0x15f6d4;
	virtual auto removeChild(cocos2d::CCNode* p0) = mac 0x123460, ios 0x15e630;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x123480, ios 0x15e640;
	virtual auto removeChildByTag(int p0) = mac 0x1235a0, ios 0x15e724;
	virtual auto removeChildByTag(int p0, bool p1) = mac 0x1235c0, ios 0x15e734;
	virtual auto removeComponent(char const* p0) = mac 0x124a60, ios 0x15f6b4;
	virtual auto removeComponent(cocos2d::CCComponent* p0) = mac 0x124a80, ios 0x15f6c4;
	virtual auto removeFromParent() = mac 0x1233f0, ios 0x15e5e8;
	virtual auto removeFromParentAndCleanup(bool p0) = mac 0x123410, ios 0x15e5f8;
	virtual auto removeMeAndCleanup() = mac 0x123440, ios 0x15e620;
	virtual auto reorderChild(cocos2d::CCNode* p0, int p1) = mac 0x123760, ios 0x15e87c;
	virtual auto resumeSchedulerAndActions() = mac 0x123b60;
	virtual auto runAction(cocos2d::CCAction* p0) = mac 0x123e60;
	virtual auto schedule(cocos2d::SEL_SCHEDULE p0) = mac 0x1240b0;
	virtual auto schedule(cocos2d::SEL_SCHEDULE p0, float p1) = mac 0x124120;
	virtual auto scheduleUpdate() = mac 0x123f80;
	virtual auto setActionManager(cocos2d::CCActionManager* p0) = mac 0x123e00, ios 0x15ef04;
	virtual auto setAnchorPoint(cocos2d::CCPoint const& p0) = mac 0x122d90, ios 0x15e098;
	virtual auto setContentSize(cocos2d::CCSize const& p0) = mac 0x122e50, ios 0x15e158;
	virtual auto setGLServerState(cocos2d::ccGLServerState p0) = mac 0x122fa0, ios 0x15e250;
	virtual auto setGrid(cocos2d::CCGridBase* p0) = mac 0x122d10, ios 0x15e03c;
	virtual auto setOrderOfArrival(unsigned int p0) = mac 0x122f60, ios 0x15e230;
	virtual auto setParent(cocos2d::CCNode* p0) = mac 0x122ee0, ios 0x15e1e8;
	virtual auto setPosition(cocos2d::CCPoint const& p0) = mac 0x122b70, ios 0x15dec4;
	virtual auto setPosition(float p0, float p1) = mac 0x122ba0, ios 0x15def0;
	virtual auto setPositionX(float p0) = mac 0x122c00, ios 0x15df40;
	virtual auto setPositionY(float p0) = mac 0x122c40, ios 0x15df84;
	virtual auto setRotation(float p0) = mac 0x122a10, ios 0x15ddec;
	virtual auto setRotationX(float p0) = mac 0x122a60, ios 0x15de24;
	virtual auto setRotationY(float p0) = mac 0x122a90, ios 0x15de40;
	virtual auto setScale(float p0) = mac 0x122ac0, ios 0x15de5c;
	virtual auto setScale(float p0, float p1) = mac 0x122ae0, ios 0x15de70;
	virtual auto setScaleX(float p0) = mac 0x122b10, ios 0x15de8c;
	virtual auto setScaleY(float p0) = mac 0x122b40, ios 0x15dea8;
	virtual auto setScheduler(cocos2d::CCScheduler* p0) = mac 0x123f20, ios 0x15efb8;
	virtual auto setShaderProgram(cocos2d::CCGLProgram* p0) = mac 0x122ff0, ios 0x15e294;
	virtual auto setSkewX(float p0) = mac 0x122930, ios 0x15dd44;
	virtual auto setSkewY(float p0) = mac 0x122960, ios 0x15dd60;
	virtual auto setUserData(void* p0) = mac 0x122f40, ios 0x15e220;
	virtual auto setUserObject(cocos2d::CCObject* p0) = mac 0x122fb0, ios 0x15e258;
	virtual auto setVertexZ(float p0) = mac 0x1229f0, ios 0x15dddc;
	virtual auto setVisible(bool p0) = mac 0x122d60, ios 0x15e080;
	virtual auto setZOrder(int p0) = mac 0x1229a0, ios 0x15dd84;
	virtual auto sortAllChildren() = mac 0x1237b0, ios 0x15e8d4;
	virtual auto stopActionByTag(int p0) = mac 0x123ec0;
	virtual auto stopAllActions() = mac 0x123190;
	virtual auto unregisterScriptHandler() = mac 0x123dc0, ios 0x15eec4;
	virtual auto unschedule(cocos2d::SEL_SCHEDULE p0) = mac 0x124180;
	virtual auto unscheduleAllSelectors() = mac 0x1231b0;
	virtual auto unscheduleUpdate() = mac 0x124060;
	virtual auto update(float p0) = mac 0x1241a0, ios 0x15f124;
	virtual auto updateTransform() = mac 0x1249d0, ios 0x15f648;
	virtual auto updateTweenAction(float p0, char const* p1) = mac 0x1249c0, ios 0x15f644;
	virtual auto visit() = mac 0x123850, ios 0x15e978;
	virtual auto worldToNodeTransform() = mac 0x124710, ios 0x15f51c;
	~CCNode() = mac 0x122750, ios 0x6c98;
	auto boundingBox();
	auto childrenAlloc();
	auto convertToNodeSpaceAR(cocos2d::CCPoint const& p0);
	auto convertToWindowSpace(cocos2d::CCPoint const& p0);
	auto convertToWorldSpaceAR(cocos2d::CCPoint const& p0);
	auto convertTouchToNodeSpace(cocos2d::CCTouch* p0);
	auto convertTouchToNodeSpaceAR(cocos2d::CCTouch* p0);
	auto description();
	auto detachChild(cocos2d::CCNode* p0, bool p1);
	auto getComponent(char const* p0);
	auto insertChild(cocos2d::CCNode* p0, int p1);
	auto numberOfRunningActions();
	auto schedule(cocos2d::SEL_SCHEDULE p0, float p1, unsigned int p2, float p3);
	auto scheduleOnce(cocos2d::SEL_SCHEDULE p0, float p1);
	auto scheduleUpdateWithPriority(int p0);
	auto scheduleUpdateWithPriorityLua(int p0, int p1);
	auto setAdditionalTransform(cocos2d::CCAffineTransform const& p0);
	auto stopAction(cocos2d::CCAction* p0);
	auto transform();
	auto transformAncestors();
}

class cocos2d::CCNodeRGBA : cocos2d::CCNode, cocos2d::CCRGBAProtocol {
	CCNodeRGBA() = mac 0x124b30;
	~CCNodeRGBA() = mac 0x124bb0, ios 0x15f748;
	virtual auto init() = mac 0x124bf0, ios 0x15f780;
	virtual auto getOpacity() = mac 0x124cd0, ios 0x15f7b8;
	virtual auto getDisplayedOpacity() = mac 0x124cf0, ios 0x15f7c8;
	virtual auto setOpacity(unsigned char p0) = mac 0x124d10, ios 0x15f7d8;
	virtual auto updateDisplayedOpacity(unsigned char p0) = mac 0x124e50, ios 0x15f87c;
	virtual auto isCascadeOpacityEnabled() = mac 0x124fe0, ios 0x15f948;
	virtual auto setCascadeOpacityEnabled(bool p0) = mac 0x125000, ios 0x15f958;
	virtual auto getColor() = mac 0x125020, ios 0x15f968;
	virtual auto getDisplayedColor() = mac 0x125040, ios 0x15f978;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x125060, ios 0x15f9888;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const& p0) = mac 0x1251e0, ios 0x15fa68;
	virtual auto isCascadeColorEnabled() = mac 0x125320, ios 0x15fb70;
	virtual auto setCascadeColorEnabled(bool p0) = mac 0x125340, ios 0x15fb80;
	virtual auto setOpacityModifyRGB(bool p0);
	virtual auto isOpacityModifyRGB();
	auto create();
	auto setOpacity(uchar p0);
	auto updateDisplayedOpacity(uchar p0);
}

class cocos2d::CCNotificationCenter : cocos2d::CCObject {
	auto CCNotificationCenter();
	auto addObserver(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1, char const* p2, cocos2d::CCObject* p3);
	auto getObserverHandlerByName(char const* p0);
	auto observerExisted(cocos2d::CCObject* p0, char const* p1);
	auto postNotification(char const* p0);
	auto postNotification(char const* p0, cocos2d::CCObject* p1);
	auto purgeNotificationCenter();
	auto registerScriptObserver(cocos2d::CCObject* p0, int p1, char const* p2);
	auto removeAllObservers(cocos2d::CCObject* p0);
	auto removeObserver(cocos2d::CCObject* p0, char const* p1);
	auto sharedNotificationCenter();
	auto unregisterScriptObserver(cocos2d::CCObject* p0, char const* p1);
	~CCNotificationCenter() = mac 0x131c00;
}

class cocos2d::CCNotificationObserver : cocos2d::CCObject {
	virtual auto getTarget() = mac 0x1326e0;
	virtual auto getSelector() = mac 0x1326f0;
	virtual auto getName() = mac 0x132700;
	virtual auto getObject() = mac 0x132710;
	virtual auto getHandler() = mac 0x132720;
	virtual auto setHandler(int p0) = mac 0x132730;
	auto CCNotificationObserver(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1, char const* p2, cocos2d::CCObject* p3);
	auto performSelector(cocos2d::CCObject* p0);
	~CCNotificationObserver() = mac 0x132640;
}

class cocos2d::CCObject : cocos2d::CCCopying {
	CCObject() = mac 0x250ca0, ios 0x43864;
	auto acceptVisitor(cocos2d::CCDataVisitor& p0) = mac 0x250f30, ios 0x439f0;
	auto autorelease() = mac 0x250ed0, ios 0x439b8;
	auto canEncode() = mac 0x250f90, ios 0x43a24;
	auto encodeWithCoder(DS_Dictionary* p0) = mac 0x250f70, ios 0x43a18;
	virtual auto getTag() const = mac 0x250f50, ios 0x43a08;
	auto isEqual(cocos2d::CCObject const* p0) = mac 0x250f20, ios 0x439e4;
	auto release() = mac 0x250ea0, ios 0x43984;
	auto retain() = mac 0x250ec0, ios 0x439a8;
	virtual auto setTag(int p0) = mac 0x250f60, ios 0x43a10;
	~CCObject() = mac 0x250d20, ios 0x6ac0;
	virtual auto update(float p0);
	auto copy();
	auto createWithCoder(DS_Dictionary* p0);
	auto isSingleReference();
	auto retainCount();
}

class cocos2d::CCOrbitCamera : cocos2d::CCActionCamera {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1b30c0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1b2de0;
	auto CCOrbitCamera();
	auto create(float p0, float p1, float p2, float p3, float p4, float p5, float p6);
	auto initWithDuration(float p0, float p1, float p2, float p3, float p4, float p5, float p6);
	auto sphericalRadius(float* p0, float* p1, float* p2);
}

class cocos2d::CCPageTurn3D : cocos2d::CCGrid3DAction {
	virtual auto update(float p0) = mac 0x194130;
	auto create(float p0, cocos2d::CCSize const& p1);
}

class cocos2d::CCParallaxNode : cocos2d::CCNode {
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x2712c0;
	virtual auto removeAllChildrenWithCleanup(bool p0) = mac 0x271350;
	virtual auto visit() = mac 0x271410;
	virtual auto addChild(cocos2d::CCNode* p0, unsigned int p1, cocos2d::CCPoint const& p2, cocos2d::CCPoint const& p3) = mac 0x271100;
	virtual auto addChild(cocos2d::CCNode* p0, unsigned int p1, int p2) = mac 0x2710f0;
	auto CCParallaxNode();
	auto absolutePosition();
	auto create();
	~CCParallaxNode() = mac 0x270fe0;
}

class cocos2d::CCParticleBatchNode : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0x206360;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0x206370;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x206380;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x206bd0;
	virtual auto removeAllChildrenWithCleanup(bool p0) = mac 0x206d10;
	virtual auto reorderChild(cocos2d::CCNode* p0, int p1) = mac 0x2067c0;
	virtual auto draw() = mac 0x206da0;
	virtual auto visit() = mac 0x2062e0;
	virtual auto getTexture() = mac 0x206f90;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x206ee0;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x206fc0;
	virtual auto getBlendFunc() = mac 0x206fe0;
	auto CCParticleBatchNode();
	auto addChildHelper(cocos2d::CCParticleSystem* p0, int p1, int p2);
	auto create(char const* p0, unsigned int p1);
	auto createWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1);
	auto disableParticle(unsigned int p0);
	auto getCurrentIndex(unsigned int* p0, unsigned int* p1, cocos2d::CCNode* p2, int p3);
	auto increaseAtlasCapacityTo(unsigned int p0);
	auto initWithFile(char const* p0, unsigned int p1);
	auto initWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1);
	auto insertChild(cocos2d::CCParticleSystem* p0, unsigned int p1);
	auto removeChildAtIndex(unsigned int p0, bool p1);
	auto searchNewPositionInChildrenForZ(int p0);
	auto updateAllAtlasIndexes();
	auto updateBlendFunc();
	~CCParticleBatchNode() = mac 0x205ff0;
}

class cocos2d::CCParticleExplosion : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24cad0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleFire : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24b570;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleFireworks : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24b910;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleFlower : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24c1a0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleGalaxy : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24be90;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleMeteor : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24c4c0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleRain : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24d3f0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleSmoke : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24cdd0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleSnow : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24d0d0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleSpiral : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24c7e0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleSun : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24bbb0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleSystem : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	auto resetSystem() = mac 0x46bd50;
	auto resumeSystem() = mac 0x46bd40;
	auto stopSystem() = mac 0x46bd10;
	virtual auto update(float p0) = mac 0x46be50, ios 0xf8fc4;
	virtual auto init() = mac 0x4692b0, ios 0xf7040;
	virtual auto setScaleX(float p0) = mac 0x46cdf0, ios 0xf99b8;
	virtual auto setScaleY(float p0) = mac 0x46ce10, ios 0xf99c4;
	virtual auto setScale(float p0) = mac 0x46cdb0, ios 0xf99a0;
	virtual auto setRotation(float p0) = mac 0x46cdd0, ios 0xf99ac;
	virtual auto getBatchNode() = mac 0x46ccd0, ios 0xf995c;
	virtual auto setBatchNode(cocos2d::CCParticleBatchNode* p0) = mac 0x46cce0, ios 0xf9964;
	virtual auto getParticleCount() = mac 0x46c8a0, ios 0xf9734;
	virtual auto getDuration() = mac 0x46c8b0, ios 0xf973c;
	virtual auto setDuration(float p0) = mac 0x46c8c0, ios 0xf9744;
	virtual auto getSourcePosition() = mac 0x46c8d0, ios 0xf974c;
	virtual auto setSourcePosition(cocos2d::CCPoint const& p0) = mac 0x46c8e0, ios 0xf9754;
	virtual auto getPosVar() = mac 0x46c900, ios 0xf975c;
	virtual auto setPosVar(cocos2d::CCPoint const& p0) = mac 0x46c910, ios 0xf9764;
	virtual auto getLife() = mac 0x46c930, ios 0xf976c;
	virtual auto setLife(float p0) = mac 0x46c940, ios 0xf9774;
	virtual auto getLifeVar() = mac 0x46c950, ios 0xf977c;
	virtual auto setLifeVar(float p0) = mac 0x46c960, ios 0xf9784;
	virtual auto getAngle() = mac 0x46c970, ios 0xf978c;
	virtual auto setAngle(float p0) = mac 0x46c980, ios 0xf9794;
	virtual auto getAngleVar() = mac 0x46c990, ios 0xf979c;
	virtual auto setAngleVar(float p0) = mac 0x46c9a0, ios 0xf97a4;
	virtual auto getGravity() = mac 0x46c780, ios 0xf96a4;
	virtual auto setGravity(cocos2d::CCPoint const& p0) = mac 0x46c760, ios 0xf969c;
	virtual auto getSpeed() = mac 0x46c7a0, ios 0xf96b4;
	virtual auto setSpeed(float p0) = mac 0x46c790, ios 0xf96ac;
	virtual auto getSpeedVar() = mac 0x46c7c0, ios 0xf96c4;
	virtual auto setSpeedVar(float p0) = mac 0x46c7b0, ios 0xf96bc;
	virtual auto getTangentialAccel() = mac 0x46c6d0, ios 0xf9654;
	virtual auto setTangentialAccel(float p0) = mac 0x46c6c0, ios 0xf964c;
	virtual auto getTangentialAccelVar() = mac 0x46c6f0, ios 0xf9664;
	virtual auto setTangentialAccelVar(float p0) = mac 0x46c6e0, ios 0xf965c;
	virtual auto getRadialAccel() = mac 0x46c710, ios 0xf9674;
	virtual auto setRadialAccel(float p0) = mac 0x46c700, ios 0xf966c;
	virtual auto getRadialAccelVar() = mac 0x46c730, ios 0xf9684;
	virtual auto setRadialAccelVar(float p0) = mac 0x46c720, ios 0xf967c;
	virtual auto getRotationIsDir() = mac 0x46c750, ios 0xf9694;
	virtual auto setRotationIsDir(bool p0) = mac 0x46c740, ios 0xf968c;
	virtual auto getStartRadius() = mac 0x46c7e0, ios 0xf96d4;
	virtual auto setStartRadius(float p0) = mac 0x46c7d0, ios 0xf96cc;
	virtual auto getStartRadiusVar() = mac 0x46c800, ios 0xf96e4;
	virtual auto setStartRadiusVar(float p0) = mac 0x46c7f0, ios 0xf96dc;
	virtual auto getEndRadius() = mac 0x46c820, ios 0xf96f4;
	virtual auto setEndRadius(float p0) = mac 0x46c810, ios 0xf96ec;
	virtual auto getEndRadiusVar() = mac 0x46c840, ios 0xf9704;
	virtual auto setEndRadiusVar(float p0) = mac 0x46c830, ios 0xf96fc;
	virtual auto getRotatePerSecond() = mac 0x46c860, ios 0xf9714;
	virtual auto setRotatePerSecond(float p0) = mac 0x46c850, ios 0xf970c;
	virtual auto getRotatePerSecondVar() = mac 0x46c880, ios 0xf9724;
	virtual auto setRotatePerSecondVar(float p0) = mac 0x46c870, ios 0xf971c;
	virtual auto isActive() = mac 0x46c890, ios 0xf972c;
	virtual auto isBlendAdditive() = mac 0x46c6a0, ios 0xf9628;
	virtual auto setBlendAdditive(bool p0) = mac 0x46c640, ios 0xf95d4;
	virtual auto getStartSize() = mac 0x46c9b0, ios 0xf97ac;
	virtual auto setStartSize(float p0) = mac 0x46c9c0, ios 0xf97b4;
	virtual auto getStartSizeVar() = mac 0x46c9d0, ios 0xf97bc;
	virtual auto setStartSizeVar(float p0) = mac 0x46c9e0, ios 0xf97c4;
	virtual auto getEndSize() = mac 0x46c9f0, ios 0xf97cc;
	virtual auto setEndSize(float p0) = mac 0x46ca00, ios 0xf97d4;
	virtual auto getEndSizeVar() = mac 0x46ca10, ios 0xf97dc;
	virtual auto setEndSizeVar(float p0) = mac 0x46ca20, ios 0xf97e4;
	virtual auto getStartColor() = mac 0x46ca30, ios 0xf97ec;
	virtual auto setStartColor(cocos2d::_ccColor4F const& p0) = mac 0x46ca40, ios 0xf97f4;
	virtual auto getStartColorVar() = mac 0x46ca60, ios 0xf9800;
	virtual auto setStartColorVar(cocos2d::_ccColor4F const& p0) = mac 0x46ca70, ios 0xf9808;
	virtual auto getEndColor() = mac 0x46ca90, ios 0xf9814;
	virtual auto setEndColor(cocos2d::_ccColor4F const& p0) = mac 0x46caa0, ios 0xf981c;
	virtual auto getEndColorVar() = mac 0x46cac0, ios 0xf9828;
	virtual auto setEndColorVar(cocos2d::_ccColor4F const& p0) = mac 0x46cad0, ios 0xf9830;
	virtual auto getStartSpin() = mac 0x46caf0, ios 0xf983c;
	virtual auto setStartSpin(float p0) = mac 0x46cb00, ios 0xf9844;
	virtual auto getStartSpinVar() = mac 0x46cb10, ios 0xf984c;
	virtual auto setStartSpinVar(float p0) = mac 0x46cb20, ios 0xf9854;
	virtual auto getEndSpin() = mac 0x46cb30, ios 0xf985c;
	virtual auto setEndSpin(float p0) = mac 0x46cb40, ios 0xf9864;
	virtual auto getEndSpinVar() = mac 0x46cb50, ios 0xf986c;
	virtual auto setEndSpinVar(float p0) = mac 0x46cb60, ios 0xf9874;
	virtual auto getEmissionRate() = mac 0x46cb70, ios 0xf987c;
	virtual auto setEmissionRate(float p0) = mac 0x46cb80, ios 0xf9884;
	virtual auto getTotalParticles() = mac 0x46cb90, ios 0xf988c;
	virtual auto setTotalParticles(unsigned int p0) = mac 0x46cba0, ios 0xf9894;
	virtual auto getTexture() = mac 0x46c620, ios 0xf95c4;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x46c4f0, ios 0xf94f4;
	virtual auto getBlendFunc() = mac 0x46cbb0, ios 0xf989c;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x46cbd0, ios 0xf98ac;
	virtual auto getOpacityModifyRGB() = mac 0x46cc50, ios 0xf991c;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x46cc60, ios 0xf9924;
	virtual auto getPositionType() = mac 0x46cc70, ios 0xf992c;
	virtual auto setPositionType(cocos2d::tCCPositionType p0) = mac 0x46cc80, ios 0xf9934;
	virtual auto isAutoRemoveOnFinish() = mac 0x46cc90, ios 0xf993c;
	virtual auto setAutoRemoveOnFinish(bool p0) = mac 0x46cca0, ios 0xf9944;
	virtual auto getEmitterMode() = mac 0x46ccb0, ios 0xf994c;
	virtual auto setEmitterMode(int p0) = mac 0x46ccc0, ios 0xf9954;
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x46b130, ios 0xf87f4;
	virtual auto updateQuadWithParticle(cocos2d::sCCParticle* p0, cocos2d::CCPoint const& p1) = mac 0x46c4d0, ios 0xf94ec;
	virtual auto postStep() = mac 0x46c4e0, ios 0xf94f0;
	virtual auto updateWithNoTime() = mac 0x46c4b0, ios 0xf94dc;
	virtual auto updateBlendFunc() = mac 0x46c5b0, ios 0xf955c;
	auto CCParticleSystem();
	auto addParticle();
	auto create(char const* p0);
	auto createWithTotalParticles(unsigned int p0);
	auto initParticle(cocos2d::sCCParticle* p0);
	auto initWithDictionary(cocos2d::CCDictionary* p0);
	auto initWithDictionary(cocos2d::CCDictionary* p0, char const* p1);
	auto initWithFile(char const* p0);
	auto isFull();
	auto loadDefaults();
	auto loadScaledDefaults(float p0);
	auto saveDefaults();
	~CCParticleSystem() = mac 0x46b390, ios 0xf8948;
}

class cocos2d::CCParticleSystemQuad : cocos2d::CCParticleSystem {
	static cocos2d::CCParticleSystemQuad* create(char const* p0) = mac 0x36b000;
	virtual auto draw() = mac 0x36b850, ios 0x16e080;
	virtual auto setBatchNode(cocos2d::CCParticleBatchNode* p0) = mac 0x36bb70, ios 0x16e294;
	virtual auto setTotalParticles(unsigned int p0) = mac 0x36b910, ios 0x16e144;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x36b3c0, ios 0x16ddd4;
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x36aa80, ios 0x16d7b4;
	virtual auto updateQuadWithParticle(cocos2d::sCCParticle* p0, cocos2d::CCPoint const& p1) = mac 0x36b570, ios 0x16de3c;
	virtual auto postStep() = mac 0x36b800, ios 0x16e034;
	auto CCParticleSystemQuad();
	auto allocMemory();
	auto create();
	auto createWithTotalParticles(unsigned int p0);
	auto initIndices();
	auto initTexCoordsWithRect(cocos2d::CCRect const& p0);
	auto listenBackToForeground(cocos2d::CCObject* p0);
	auto setDisplayFrame(cocos2d::CCSpriteFrame* p0);
	auto setTextureWithRect(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1);
	auto setupVBO();
	~CCParticleSystemQuad() = mac 0x36afc0, ios 0x16db40;
}

class cocos2d::CCPlace : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x454d70;
	auto create(cocos2d::CCPoint const& p0);
	auto initWithPosition(cocos2d::CCPoint const& p0);
}

class cocos2d::CCPointArray : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	auto CCPointArray();
	auto addControlPoint(cocos2d::CCPoint p0);
	auto count();
	auto create(unsigned int p0);
	auto getControlPointAtIndex(unsigned int p0);
	auto getControlPoints();
	auto initWithCapacity(unsigned int p0);
	auto insertControlPoint(cocos2d::CCPoint& p0, unsigned int p1);
	auto removeControlPointAtIndex(unsigned int p0);
	auto replaceControlPoint(cocos2d::CCPoint& p0, unsigned int p1);
	auto reverse();
	auto reverseInline();
	auto setControlPoints(gd::vector<cocos2d::CCPoint*>* p0);
	~CCPointArray() = mac 0x167090;
}

class cocos2d::CCPoolManager {
	auto pop() = mac 0x214620;
	static cocos2d::CCPoolManager* sharedPoolManager() = mac 0x2142c0;
}

class cocos2d::CCPrettyPrinter : cocos2d::CCDataVisitor {
	virtual auto visitObject(cocos2d::CCObject const* p0) = mac 0x2763e0, ios 0x16b074;
	virtual auto visit(cocos2d::CCBool const* p0) = mac 0x276470, ios 0x16b0f4;
	virtual auto visit(cocos2d::CCInteger const* p0) = mac 0x276510, ios 0x16b18c;
	virtual auto visit(cocos2d::CCFloat const* p0) = mac 0x2765a0, ios 0x16b210;
	virtual auto visit(cocos2d::CCDouble const* p0) = mac 0x276630, ios 0x16b298;
	virtual auto visit(cocos2d::CCString const* p0) = mac 0x2766c0, ios 0x16b31c;
	virtual auto visit(cocos2d::CCArray const* p0) = mac 0x276700, ios 0x16b348;
	virtual auto visit(cocos2d::CCDictionary const* p0) = mac 0x276b50, ios 0x16b644;
	virtual auto visit(cocos2d::CCSet const* p0) = mac 0x276f20, ios 0x16b8d0;
	virtual auto clear() = mac 0x2763a0, ios 0x16b044;
	virtual auto getResult() = mac 0x2763c0, ios 0x16b068;
	auto CCPrettyPrinter(int p0);
	auto setIndentLevel(int p0);
	~CCPrettyPrinter();
}

class cocos2d::CCProfiler : cocos2d::CCObject {
	auto createAndAddTimerWithName(char const* p0);
	auto displayTimers();
	auto init();
	auto releaseAllTimers();
	auto releaseTimer(char const* p0);
	auto sharedProfiler();
	~CCProfiler() = mac 0x28a2e0;
}

class cocos2d::CCProfilingTimer : cocos2d::CCObject {
	auto CCProfilingTimer();
	auto description();
	auto initWithName(char const* p0);
	auto reset();
	~CCProfilingTimer() = mac 0x28a470;
}

class cocos2d::CCProgressFromTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x23d650;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x23d640;
	virtual auto reverse() = mac 0x23d590;
	auto create(float p0, float p1, float p2);
	auto initWithDuration(float p0, float p1, float p2);
}

class cocos2d::CCProgressTimer : cocos2d::CCNodeRGBA {
	virtual auto draw() = mac 0x36e8f0;
	virtual auto setOpacity(unsigned char p0) = mac 0x36c720;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x36c590;
	virtual auto getColor() const = mac 0x36c700;
	virtual auto getOpacity() const = mac 0x36c820;
	virtual auto getMidpoint() = mac 0x36e7c0;
	virtual auto setMidpoint(cocos2d::CCPoint p0) = mac 0x36e7e0;
	auto CCProgressTimer();
	auto boundaryTexCoord(char p0);
	auto create(cocos2d::CCSprite* p0);
	auto initWithSprite(cocos2d::CCSprite* p0);
	auto setAnchorPoint(cocos2d::CCPoint p0);
	auto setPercentage(float p0);
	auto setReverseProgress(bool p0);
	auto setSprite(cocos2d::CCSprite* p0);
	auto setType(cocos2d::CCProgressTimerType p0);
	auto textureCoordFromAlphaPoint(cocos2d::CCPoint p0);
	auto updateBar();
	auto updateColor();
	auto updateProgress();
	auto updateRadial();
	auto vertexFromAlphaPoint(cocos2d::CCPoint p0);
	~CCProgressTimer() = mac 0x36c3b0;
}

class cocos2d::CCProgressTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x23d380;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x23d340;
	auto create(float p0, float p1);
	auto initWithDuration(float p0, float p1);
}

class cocos2d::CCRGBAProtocol {}

class cocos2d::CCRemoveSelf : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x454770;
	virtual auto reverse() = mac 0x454790;
	auto create(bool p0);
	auto init(bool p0);
}

class cocos2d::CCRenderTexture : cocos2d::CCNode {
	auto begin() = mac 0x35ce10;
	auto end() = mac 0x35d2c0;
	static cocos2d::CCRenderTexture* create(int p0, int p1, cocos2d::CCTexture2DPixelFormat p2) = mac 0x35c720;
	auto newCCImage(bool p0) = mac 0x35d7d0;
	virtual auto draw() = mac 0x35d570;
	virtual auto visit() = mac 0x35d4e0;
	virtual auto getSprite() = mac 0x35c620;
	virtual auto setSprite(cocos2d::CCSprite* p0) = mac 0x35c630;
	auto CCRenderTexture();
	auto beginWithClear(float p0, float p1, float p2, float p3);
	auto beginWithClear(float p0, float p1, float p2, float p3, float p4);
	auto beginWithClear(float p0, float p1, float p2, float p3, float p4, int p5);
	auto beginWithClear(float p0, float p1, float p2, float p3, float p4, int p5, unsigned int p6);
	auto clear(float p0, float p1, float p2, float p3);
	auto clearDepth(float p0);
	auto clearStencil(int p0);
	auto create(int p0, int p1);
	auto create(int p0, int p1, cocos2d::CCTexture2DPixelFormat p2, unsigned int p3);
	auto getClearColor();
	auto getClearDepth();
	auto getClearFlags();
	auto getClearStencil();
	auto initWithWidthAndHeight(int p0, int p1, cocos2d::CCTexture2DPixelFormat p2);
	auto initWithWidthAndHeight(int p0, int p1, cocos2d::CCTexture2DPixelFormat p2, unsigned int p3);
	auto isAutoDraw();
	auto listenToBackground(cocos2d::CCObject* p0);
	auto listenToForeground(cocos2d::CCObject* p0);
	auto saveToFile(char const* p0);
	auto saveToFile(char const* p0, cocos2d::eImageFormat p1);
	auto setAutoDraw(bool p0);
	auto setClearColor(cocos2d::_ccColor4F const& p0);
	auto setClearDepth(float p0);
	auto setClearFlags(unsigned int p0);
	auto setClearStencil(float p0);
	~CCRenderTexture() = mac 0x35c5c0;
}

class cocos2d::CCRepeat : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f36b0;
	virtual auto isDone() = mac 0x1f37e0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f3630;
	virtual auto stop() = mac 0x1f3680;
	virtual auto reverse() = mac 0x1f37f0;
	auto create(cocos2d::CCFiniteTimeAction* p0, unsigned int p1);
	auto initWithAction(cocos2d::CCFiniteTimeAction* p0, unsigned int p1);
}

class cocos2d::CCRepeatForever : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCRepeatForever* create(cocos2d::CCActionInterval* p0) = mac 0x1f3920;
	virtual auto isDone() = mac 0x1f3b40, ios 0xf0d70;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f3a90, ios 0xf0c98;
	virtual auto step(float p0) = mac 0x1f3ad0, ios 0xf0cd8;
	virtual auto reverse() = mac 0x1f3b50, ios 0xf0d78;
	auto initWithAction(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCReuseGrid : cocos2d::CCActionInstant {
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xef010;
	auto create(int p0);
	auto initWithTimes(int p0);
}

class cocos2d::CCReverseTime : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f8f70;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f8f00;
	virtual auto stop() = mac 0x1f8f40;
	virtual auto reverse() = mac 0x1f8fa0;
	auto CCReverseTime();
	auto create(cocos2d::CCFiniteTimeAction* p0);
	auto initWithAction(cocos2d::CCFiniteTimeAction* p0);
}

class cocos2d::CCRipple3D : cocos2d::CCGrid3DAction {
	virtual auto update(float p0) = mac 0x1286c0;
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint const& p2, float p3, unsigned int p4, float p5);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint const& p2, float p3, unsigned int p4, float p5);
	auto setPosition(cocos2d::CCPoint const& p0);
}

class cocos2d::CCRotateBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCRotateBy* create(float p0, float p1) = mac 0x1f4c50;
	virtual auto update(float p0) = mac 0x1f4f90, ios 0xf18e0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f4f40, ios 0xf1888;
	virtual auto reverse() = mac 0x1f5000, ios 0xf1964;
	auto create(float p0, float p1, float p2);
	auto initWithDuration(float p0, float p1);
	auto initWithDuration(float p0, float p1, float p2);
}

class cocos2d::CCRotateTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f4be0, ios 0xf15c0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f4aa0, ios 0xf14b8;
	virtual auto initWithDuration(float p0, float p1, float p2) = mac 0x1f4980, ios 0xf13a8;
	auto create(float p0, float p1);
	auto create(float p0, float p1, float p2);
	auto initWithDuration(float p0, float p1);
}

class cocos2d::CCSAXDelegator {}

class cocos2d::CCScaleBy : cocos2d::CCScaleTo {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f7640;
	virtual auto reverse() = mac 0x1f76b0;
	auto create(float p0, float p1);
	auto create(float p0, float p1, float p2);
}

class cocos2d::CCScaleTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCScaleTo* create(float p0, float p1) = mac 0x1f6ff0;
	static cocos2d::CCScaleTo* create(float p0, float p1, float p2) = mac 0x1f70f0;
	virtual auto update(float p0) = mac 0x1f7360, ios 0xf29c0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f72f0, ios 0xf2954;
	auto initWithDuration(float p0, float p1);
	auto initWithDuration(float p0, float p1, float p2);
}

class cocos2d::CCScene : cocos2d::CCNode {
	static cocos2d::CCScene* create() = mac 0x13c140, ios 0x163070;
	auto getHighestChildZ() = mac 0x13c200, ios 0x1630e4;
	virtual auto init() = mac 0x13c0f0, ios 0x163024;
	auto CCScene();
	~CCScene() = mac 0x13c0b0, ios 0x162fe8;
}

class cocos2d::CCSceneDelegate {
	virtual auto willSwitchToScene(cocos2d::CCScene* p0);
}

class cocos2d::CCSceneTransitionDelegate {
	virtual auto sceneWillResume();
}

class cocos2d::CCScheduler : cocos2d::CCObject {
	auto scheduleSelector(cocos2d::SEL_SCHEDULE p0, cocos2d::CCObject* p1, float p2, unsigned int p3, float p4, bool p5) = mac 0x242b20;
	auto scheduleUpdateForTarget(cocos2d::CCObject* p0, int p1, bool p2) = mac 0x2438d0;
	auto unscheduleAllForTarget(cocos2d::CCObject* p0) = mac 0x243e40;
	virtual auto update(float delta) = mac 0x2446d0, ios 0x10df78;
	auto CCScheduler();
	auto appendIn(cocos2d::_listEntry** p0, cocos2d::CCObject* p1, bool p2);
	auto isTargetPaused(cocos2d::CCObject* p0);
	auto pauseAllTargets();
	auto pauseAllTargetsWithMinPriority(int p0);
	auto pauseTarget(cocos2d::CCObject* p0);
	auto priorityIn(cocos2d::_listEntry** p0, cocos2d::CCObject* p1, int p2, bool p3);
	auto removeHashElement(cocos2d::_hashSelectorEntry* p0);
	auto removeUpdateFromHash(cocos2d::_listEntry* p0);
	auto resumeTarget(cocos2d::CCObject* p0);
	auto resumeTargets(cocos2d::CCSet* p0);
	auto scheduleScriptFunc(unsigned int p0, float p1, bool p2);
	auto scheduleSelector(cocos2d::SEL_SCHEDULE p0, cocos2d::CCObject* p1, float p2, bool p3);
	auto unscheduleAll();
	auto unscheduleAllWithMinPriority(int p0);
	auto unscheduleScriptEntry(unsigned int p0);
	auto unscheduleSelector(cocos2d::SEL_SCHEDULE p0, cocos2d::CCObject* p1);
	auto unscheduleUpdateForTarget(cocos2d::CCObject const* p0);
	~CCScheduler() = mac 0x242910, ios 0x10c4f4;
}

class cocos2d::CCSchedulerScriptHandlerEntry : cocos2d::CCScriptHandlerEntry {
	auto create(int p0, float p1, bool p2);
	auto init(float p0, bool p1);
	~CCSchedulerScriptHandlerEntry() = mac 0x1985f0;
}

class cocos2d::CCScriptHandlerEntry : cocos2d::CCObject {
	auto CCScriptHandlerEntry(int p0);
	auto create(int p0);
	~CCScriptHandlerEntry() = mac 0x198280, ios 0x1a8edc;
}

class cocos2d::CCSequence : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static auto createWithVariableList(cocos2d::CCFiniteTimeAction* p0, va_list p1) = mac 0x1f2910;
	virtual auto update(float p0);
	virtual auto startWithTarget(cocos2d::CCNode* p0);
	virtual auto stop();
	virtual auto reverse();
	auto create(cocos2d::CCArray* p0);
	auto createWithTwoActions(cocos2d::CCFiniteTimeAction* p0, cocos2d::CCFiniteTimeAction* p1);
	auto initWithTwoActions(cocos2d::CCFiniteTimeAction* p0, cocos2d::CCFiniteTimeAction* p1);
}

class cocos2d::CCSet : cocos2d::CCObject {
	CCSet() = mac 0x45ad80, ios 0x10e870;
	~CCSet() = mac 0x45b050, ios 0x10ebcc;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0) = mac 0x45b090, ios 0x10ec04;
	auto anyObject() = mac 0x45b410;
	auto CCSet(cocos2d::CCSet const& p0);
	auto addObject(cocos2d::CCObject* p0);
	auto begin();
	auto containsObject(cocos2d::CCObject* p0);
	auto copy();
	auto count();
	auto create();
	auto end();
	auto mutableCopy();
	auto removeAllObjects();
	auto removeObject(cocos2d::CCObject* p0);
}

class cocos2d::CCShaderCache : cocos2d::CCObject {
	auto CCShaderCache();
	auto addProgram(cocos2d::CCGLProgram* p0, char const* p1);
	auto init();
	auto loadDefaultShader(cocos2d::CCGLProgram* p0, int p1);
	auto loadDefaultShaders();
	auto programForKey(char const* p0);
	auto purgeSharedShaderCache();
	auto reloadDefaultShaders();
	auto sharedShaderCache();
	~CCShaderCache() = mac 0xe6ee0, ios 0xba20c;
}

class cocos2d::CCShaky3D : cocos2d::CCGrid3DAction {
	virtual auto update(float p0) = mac 0x128ae0;
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0, cocos2d::CCSize const& p1, int p2, bool p3);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, int p2, bool p3);
}

class cocos2d::CCShakyTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45b900;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1, int p2, bool p3) = mac 0x45b7b0;
	auto create(float p0, cocos2d::CCSize const& p1, int p2, bool p3);
}

class cocos2d::CCShatteredTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45bdc0;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1, int p2, bool p3) = mac 0x45bc70;
	auto create(float p0, cocos2d::CCSize const& p1, int p2, bool p3);
}

class cocos2d::CCShow : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0);
	virtual auto reverse();
	auto create();
}

class cocos2d::CCShuffleTiles : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45c830;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x45c590;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2) = mac 0x45c140;
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2);
	auto getDelta(cocos2d::CCSize const& p0);
	auto placeTile(cocos2d::CCPoint const& p0, cocos2d::Tile* p1);
	auto shuffle(unsigned int* p0, unsigned int p1);
}

class cocos2d::CCSkewBy : cocos2d::CCSkewTo {
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f5d00;
	virtual auto reverse() = mac 0x1f5d40;
	virtual auto initWithDuration(float p0, float p1, float p2) = mac 0x1f5cb0;
	auto create(float p0, float p1, float p2);
}

class cocos2d::CCSkewTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f5b20;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f59f0;
	virtual auto initWithDuration(float p0, float p1, float p2) = mac 0x1f58d0;
	auto CCSkewTo();
	auto create(float p0, float p1, float p2);
}

class cocos2d::CCSpawn : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f46c0, ios 0xf1258;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f4650, ios 0xf11c4;
	virtual auto stop() = mac 0x1f4690, ios 0xf1218;
	virtual auto reverse() = mac 0x1f4710, ios 0xf12bc;
	auto create(cocos2d::CCArray* p0);
	auto createWithTwoActions(cocos2d::CCFiniteTimeAction* p0, cocos2d::CCFiniteTimeAction* p1);
	auto initWithTwoActions(cocos2d::CCFiniteTimeAction* p0, cocos2d::CCFiniteTimeAction* p1);
}

class cocos2d::CCSpeed : cocos2d::CCAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto isDone() = mac 0x35bc50;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x35bbe0;
	virtual auto stop() = mac 0x35bc00;
	virtual auto step(float p0) = mac 0x35bc30;
	virtual auto reverse() = mac 0x35bc60;
	auto create(cocos2d::CCActionInterval* p0, float p1);
	auto initWithAction(cocos2d::CCActionInterval* p0, float p1);
	auto setInnerAction(cocos2d::CCActionInterval* p0);
	~CCSpeed() = mac 0x35b910;
}

class cocos2d::CCSplitCols : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45e790;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x45e750;
	virtual auto initWithDuration(float p0, unsigned int p1) = mac 0x45e5d0;
	auto create(float p0, unsigned int p1);
}

class cocos2d::CCSplitRows : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45e3e0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x45e3a0;
	virtual auto initWithDuration(float p0, unsigned int p1) = mac 0x45e220;
	auto create(float p0, unsigned int p1);
}

class cocos2d::CCSprite : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	~CCSprite() = mac 0x133430, ios 0x15b92c;
	virtual auto init() = mac 0x132ef0, ios 0x15b488;
	virtual auto setVertexZ(float p0) = mac 0x134a80, ios 0x15ccc8;
	virtual auto setScaleX(float p0) = mac 0x134900, ios 0x15cb3c;
	virtual auto setScaleY(float p0) = mac 0x134980, ios 0x15cbbc;
	virtual auto setScale(float p0) = mac 0x134a00, ios 0x15cc3c;
	virtual auto setPosition(cocos2d::CCPoint const& p0) = mac 0x134650, ios 0x15c878;
	virtual auto setSkewX(float p0) = mac 0x134820, ios 0x15ca5c;
	virtual auto setSkewY(float p0) = mac 0x134890, ios 0x15cacc;
	virtual auto setAnchorPoint(cocos2d::CCPoint const& p0) = mac 0x134af0, ios 0x15cd38;
	virtual auto setVisible(bool p0) = mac 0x134b70, ios 0x15cdac;
	virtual auto setRotation(float p0) = mac 0x1346d0, ios 0x15c90c;
	virtual auto setRotationX(float p0) = mac 0x134740, ios 0x15c97c;
	virtual auto setRotationY(float p0) = mac 0x1347b0, ios 0x15c9ec;
	virtual auto ignoreAnchorPointForPosition(bool p0) = mac 0x134b60, ios 0x15cda8;
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0x134190, ios 0x15c34c;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0x1341a0, ios 0x15c350;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x1341b0, ios 0x15c354;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x134300, ios 0x15c4cc;
	virtual auto removeAllChildrenWithCleanup(bool p0) = mac 0x134340, ios 0x15c514;
	virtual auto reorderChild(cocos2d::CCNode* p0, int p1) = mac 0x134270, ios 0x15c438;
	virtual auto sortAllChildren() = mac 0x1343f0, ios 0x15c5cc;
	virtual auto draw() = mac 0x134070, ios 0x15c240;
	virtual auto updateTransform() = mac 0x133b70, ios 0x15be0c;
	virtual auto setOpacity(unsigned char p0) = mac 0x134da0, ios 0x15cf48;
	virtual auto updateDisplayedOpacity(unsigned char p0) = mac 0x1354c0, ios 0x15d228;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x134ff0, ios 0x15d088;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const& p0) = mac 0x135370, ios 0x15d1dc;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x135200, ios 0x15d198;
	virtual auto isOpacityModifyRGB() = mac 0x135350, ios 0x15d1cc;
	virtual auto initWithTexture(cocos2d::CCTexture2D* p0) = mac 0x133110, ios 0x15b608;
	virtual auto initWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1) = mac 0x1330f0, ios 0x15b5f8;
	virtual auto initWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1, bool p2) = mac 0x132f10, ios 0x15b4a0;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame* p0) = mac 0x133270, ios 0x15b78c;
	virtual auto initWithSpriteFrameName(char const* p0) = mac 0x1332c0, ios 0x15b7f0;
	virtual auto initWithFile(char const* p0) = mac 0x133180, ios 0x15b684;
	virtual auto initWithFile(char const* p0, cocos2d::CCRect const& p1) = mac 0x133210, ios 0x15b720;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x135a90, ios 0x15d610;
	virtual auto getTexture() = mac 0x135c00, ios 0x15d748;
	virtual auto setChildColor(cocos2d::ccColor3B const& p0) = mac 0x135160, ios 0x15d0f4;
	virtual auto setChildOpacity(unsigned char p0) = mac 0x134f50, ios 0x15cfe4;
	virtual auto getBatchNode() = mac 0x135910, ios 0x15d4e0;
	virtual auto setBatchNode(cocos2d::CCSpriteBatchNode* p0) = mac 0x135920, ios 0x15d4e8;
	virtual auto refreshTextureRect() = mac 0x133520, ios 0x15b964;
	virtual auto setTextureRect(cocos2d::CCRect const& p0) = mac 0x133560, ios 0x15b9a8;
	virtual auto setTextureRect(cocos2d::CCRect const& p0, bool p1, cocos2d::CCSize const& p2) = mac 0x133580, ios 0x15b9bc;
	virtual auto setVertexRect(cocos2d::CCRect const& p0) = mac 0x1338f0, ios 0x15bc5c;
	virtual auto setDisplayFrame(cocos2d::CCSpriteFrame* p0) = mac 0x135610, ios 0x15d274;
	virtual auto isFrameDisplayed(cocos2d::CCSpriteFrame* p0) = mac 0x1356d0, ios 0x15d344;
	virtual auto displayFrame() = mac 0x135760, ios 0x15d3d8;
	virtual auto setDisplayFrameWithAnimationName(char const* p0, int p1) = mac 0x135680, ios 0x15d2f0;
	virtual auto setTextureCoords(cocos2d::CCRect const& p0) = mac 0x133910, ios 0x15bc64;
	virtual auto updateBlendFunc() = mac 0x135a20, ios 0x15d5a0;
	virtual auto setReorderChildDirtyRecursively() = mac 0x134550, ios 0x15c764;
	virtual auto setDirtyRecursively(bool p0) = mac 0x1345b0, ios 0x15c7cc;
	CCSprite() = mac 0x133300, ios 0x15b82c;
	static cocos2d::CCSprite* create() = mac 0x132df0;
	static cocos2d::CCSprite* create(char const* p0) = mac 0x132a80;
	static cocos2d::CCSprite* createWithSpriteFrame(cocos2d::CCSpriteFrame* p0) = mac 0x132cb0;
	static cocos2d::CCSprite* createWithSpriteFrameName(char const* p0) = mac 0x132dc0, ios 0x15b3ec;
	static cocos2d::CCSprite* createWithTexture(cocos2d::CCTexture2D* p0) = mac 0x132790;
	static cocos2d::CCSprite* createWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1) = mac 0x132960;
	auto setFlipX(bool p0) = mac 0x134be0;
	auto setFlipY(bool p0) = mac 0x134c30;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0);
	virtual auto getBlendFunc();
	virtual auto isDirty();
	virtual auto setDirty(bool p0);
	auto create(char const* p0, cocos2d::CCRect const& p1);
	auto isFlipX();
	auto isFlipY();
	auto updateColor();
}

class cocos2d::CCSpriteBatchNode : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	static cocos2d::CCSpriteBatchNode* create(char const* p0, unsigned int p1) = mac 0xbb540;
	static auto createWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1) = mac 0xbb310;
	auto getUsedAtlasCapacity() = mac 0xbc6b0;
	auto increaseAtlasCapacity(unsigned int p0) = mac 0xbc670;
	virtual auto init() = mac 0xbb680, ios 0x130c20;
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0xbbb40, ios 0x131034;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0xbbb50, ios 0x131038;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0xbb930, ios 0x130e0c;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0xbbbb0, ios 0x1310a4;
	virtual auto removeAllChildrenWithCleanup(bool p0) = mac 0xbbe30, ios 0x1312d8;
	virtual auto reorderChild(cocos2d::CCNode* p0, int p1) = mac 0xbbb60, ios 0x13103c;
	virtual auto sortAllChildren() = mac 0xbbee0, ios 0x13137c;
	virtual auto draw() = mac 0xbc540, ios 0x13191c;
	virtual auto visit() = mac 0xbb880, ios 0x130d54;
	virtual auto getTexture() = mac 0xbcd70, ios 0x131a70;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0xbcda0, ios 0x131a90;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0xbcd30, ios 0x131a50;
	virtual auto getBlendFunc() = mac 0xbcd50, ios 0x131a60;
	auto CCSpriteBatchNode();
	auto addSpriteWithoutQuad(cocos2d::CCSprite* p0, unsigned int p1, int p2);
	auto appendChild(cocos2d::CCSprite* p0);
	auto atlasIndexForChild(cocos2d::CCSprite* p0, int p1);
	auto createWithTexture(cocos2d::CCTexture2D* p0);
	auto getAtlasCapacity();
	auto highestAtlasIndexInChild(cocos2d::CCSprite* p0);
	auto increaseAtlasCapacity();
	auto initWithFile(char const* p0, unsigned int p1);
	auto initWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1);
	auto insertChild(cocos2d::CCSprite* p0, unsigned int p1);
	auto insertQuadFromSprite(cocos2d::CCSprite* p0, unsigned int p1);
	auto lowestAtlasIndexInChild(cocos2d::CCSprite* p0);
	auto manualSortAllChildren();
	auto rebuildIndexInOrder(cocos2d::CCSprite* p0, unsigned int p1);
	auto removeChildAtIndex(unsigned int p0, bool p1);
	auto removeSpriteFromAtlas(cocos2d::CCSprite* p0);
	auto reorderBatch(bool p0);
	auto swap(int p0, int p1);
	auto updateAtlasIndex(cocos2d::CCSprite* p0, int* p1);
	auto updateBlendFunc();
	auto updateQuadFromSprite(cocos2d::CCSprite* p0, unsigned int p1);
	~CCSpriteBatchNode() = mac 0xbb790, ios 0x130d18;
}

class cocos2d::CCSpriteFrame : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static auto createWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1, bool p2, cocos2d::CCPoint const& p3, cocos2d::CCSize const& p4) = mac 0x1ac7f0;
	static auto createWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1) = mac 0x1ac5c0;
	auto CCSpriteFrame();
	auto create(char const* p0, cocos2d::CCRect const& p1);
	auto create(char const* p0, cocos2d::CCRect const& p1, bool p2, cocos2d::CCPoint const& p3, cocos2d::CCSize const& p4);
	auto getOffset();
	auto getOffsetInPixels();
	auto getTexture();
	auto initWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1);
	auto initWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1, bool p2, cocos2d::CCPoint const& p3, cocos2d::CCSize const& p4);
	auto initWithTextureFilename(char const* p0, cocos2d::CCRect const& p1);
	auto initWithTextureFilename(char const* p0, cocos2d::CCRect const& p1, bool p2, cocos2d::CCPoint const& p3, cocos2d::CCSize const& p4);
	auto setOffset(cocos2d::CCPoint const& p0);
	auto setOffsetInPixels(cocos2d::CCPoint const& p0);
	auto setRect(cocos2d::CCRect const& p0);
	auto setRectInPixels(cocos2d::CCRect const& p0);
	auto setTexture(cocos2d::CCTexture2D* p0);
	~CCSpriteFrame() = mac 0x1ace20, ios 0x16ace8;
}

class cocos2d::CCSpriteFrameCache : cocos2d::CCObject {
	auto addSpriteFramesWithFile(char const* p0) = mac 0x199a10, ios 0x29e818;
	static cocos2d::CCSpriteFrameCache* sharedSpriteFrameCache() = mac 0x198970, ios 0x29dc4c;
	auto spriteFrameByName(char const* p0) = mac 0x19a7e0;
	auto addSpriteFrame(cocos2d::CCSpriteFrame* p0, char const* p1);
	auto addSpriteFramesWithDictionary(cocos2d::CCDictionary* p0, cocos2d::CCTexture2D* p1);
	auto addSpriteFramesWithFile(char const* p0, cocos2d::CCTexture2D* p1);
	auto addSpriteFramesWithFile(char const* p0, char const* p1);
	auto init();
	auto purgeSharedSpriteFrameCache();
	auto removeSpriteFrameByName(char const* p0);
	auto removeSpriteFrames();
	auto removeSpriteFramesFromDictionary(cocos2d::CCDictionary* p0);
	auto removeSpriteFramesFromFile(char const* p0);
	auto removeSpriteFramesFromTexture(cocos2d::CCTexture2D* p0);
	auto removeUnusedSpriteFrames();
	~CCSpriteFrameCache() = mac 0x198b50, ios 0x29dde0;
}

class cocos2d::CCStandardTouchHandler : cocos2d::CCTouchHandler {
	static cocos2d::CCStandardTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate* p0, int p1) = mac 0x247f30;
	virtual auto initWithDelegate(cocos2d::CCTouchDelegate* p0, int p1) = mac 0x247ed0, ios 0x69;
	~CCStandardTouchHandler() = mac 0x2482a0, ios 0x6d28;
}

class cocos2d::CCStopGrid : cocos2d::CCActionInstant {
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xeeee0;
	auto create();
}

class cocos2d::CCString : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto isEqual(cocos2d::CCObject const* p0) = mac 0x44c8f0, ios 0x1a1e6c;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0) = mac 0x44ccb0, ios 0x1a2168;
	auto boolValue() const = mac 0x44c810, ios 0x1a1d88;
	auto doubleValue() const = mac 0x44c7f0;
	auto floatValue() const = mac 0x44c7d0;
	auto getCString() const = mac 0x44c470, ios 0x1a1b7c;
	auto intValue() const = mac 0x44c780, ios 0x1a1ca8;
	auto CCString(char const* p0);
	auto CCString(cocos2d::CCString const& p0);
	auto CCString(gd::string const& p0);
	auto CCString();
	auto compare(char const* p0);
	auto create(gd::string const& p0);
	auto createWithContentsOfFile(char const* p0);
	auto createWithData(unsigned char const* p0, unsigned long p1);
	auto length();
	auto uintValue();
}

class cocos2d::CCTMXLayer : cocos2d::CCSpriteBatchNode {
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x1222b0;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x1222c0;
	virtual auto getTileSet() = mac 0x120d20;
	virtual auto setTileSet(cocos2d::CCTMXTilesetInfo* p0) = mac 0x120d30;
	virtual auto getProperties() = mac 0x1224a0;
	virtual auto setProperties(cocos2d::CCDictionary* p0) = mac 0x1224b0;
	auto CCTMXLayer();
	auto appendTileForGID(unsigned int p0, cocos2d::CCPoint const& p1);
	auto atlasIndexForExistantZ(unsigned int p0);
	auto atlasIndexForNewZ(int p0);
	auto calculateLayerOffset(cocos2d::CCPoint const& p0);
	auto create(cocos2d::CCTMXTilesetInfo* p0, cocos2d::CCTMXLayerInfo* p1, cocos2d::CCTMXMapInfo* p2);
	auto initWithTilesetInfo(cocos2d::CCTMXTilesetInfo* p0, cocos2d::CCTMXLayerInfo* p1, cocos2d::CCTMXMapInfo* p2);
	auto insertTileForGID(unsigned int p0, cocos2d::CCPoint const& p1);
	auto parseInternalProperties();
	auto positionAt(cocos2d::CCPoint const& p0);
	auto positionForHexAt(cocos2d::CCPoint const& p0);
	auto positionForIsoAt(cocos2d::CCPoint const& p0);
	auto positionForOrthoAt(cocos2d::CCPoint const& p0);
	auto propertyNamed(char const* p0);
	auto releaseMap();
	auto removeTileAt(cocos2d::CCPoint const& p0);
	auto reusedTileWithRect(cocos2d::CCRect p0);
	auto setTileGID(unsigned int p0, cocos2d::CCPoint const& p1);
	auto setTileGID(unsigned int p0, cocos2d::CCPoint const& p1, cocos2d::ccTMXTileFlags_ p2);
	auto setupTileSprite(cocos2d::CCSprite* p0, cocos2d::CCPoint p1, unsigned int p2);
	auto setupTiles();
	auto tileAt(cocos2d::CCPoint const& p0);
	auto tileGIDAt(cocos2d::CCPoint const& p0);
	auto tileGIDAt(cocos2d::CCPoint const& p0, cocos2d::ccTMXTileFlags_* p1);
	auto updateTileForGID(unsigned int p0, cocos2d::CCPoint const& p1);
	auto vertexZForPos(cocos2d::CCPoint const& p0);
	~CCTMXLayer() = mac 0x120ce0;
}

class cocos2d::CCTMXLayerInfo : cocos2d::CCObject {
	virtual auto getProperties() = mac 0x282900;
	virtual auto setProperties(cocos2d::CCDictionary* p0) = mac 0x282910;
	auto CCTMXLayerInfo();
	~CCTMXLayerInfo() = mac 0x2828c0;
}

class cocos2d::CCTMXMapInfo : cocos2d::CCObject, cocos2d::CCSAXDelegator {
	virtual auto getLayers() = mac 0x283640;
	virtual auto setLayers(cocos2d::CCArray* p0) = mac 0x283650;
	virtual auto getTilesets() = mac 0x283690;
	virtual auto setTilesets(cocos2d::CCArray* p0) = mac 0x2836a0;
	virtual auto getObjectGroups() = mac 0x2836e0;
	virtual auto setObjectGroups(cocos2d::CCArray* p0) = mac 0x2836f0;
	virtual auto getProperties() = mac 0x283730;
	virtual auto setProperties(cocos2d::CCDictionary* p0) = mac 0x283740;
	virtual auto startElement(void* p0, char const* p1, char const** p2) = mac 0x2837d0;
	virtual auto endElement(void* p0, char const* p1) = mac 0x286210;
	virtual auto textHandler(void* p0, char const* p1, int p2) = mac 0x286460;
	auto CCTMXMapInfo();
	auto formatWithTMXFile(char const* p0);
	auto formatWithXML(char const* p0, char const* p1);
	auto getTileProperties();
	auto initWithTMXFile(char const* p0);
	auto initWithXML(char const* p0, char const* p1);
	auto internalInit(char const* p0, char const* p1);
	auto parseXMLFile(char const* p0);
	auto parseXMLString(char const* p0);
	auto setTileProperties(cocos2d::CCDictionary* p0);
	~CCTMXMapInfo() = mac 0x283600;
}

class cocos2d::CCTMXObjectGroup : cocos2d::CCObject {
	virtual auto getProperties() = mac 0x25f800;
	virtual auto setProperties(cocos2d::CCDictionary* p0) = mac 0x25f810;
	virtual auto getObjects() = mac 0x25f850;
	virtual auto setObjects(cocos2d::CCArray* p0) = mac 0x25f860;
	auto CCTMXObjectGroup();
	auto objectNamed(char const* p0);
	auto propertyNamed(char const* p0);
	~CCTMXObjectGroup() = mac 0x25f5c0;
}

class cocos2d::CCTMXTiledMap : cocos2d::CCNode {
	virtual auto getObjectGroups() = mac 0x2a63e0;
	virtual auto setObjectGroups(cocos2d::CCArray* p0) = mac 0x2a63f0;
	virtual auto getProperties() = mac 0x2a6430;
	virtual auto setProperties(cocos2d::CCDictionary* p0) = mac 0x2a6440;
	auto CCTMXTiledMap();
	auto buildWithMapInfo(cocos2d::CCTMXMapInfo* p0);
	auto create(char const* p0);
	auto createWithXML(char const* p0, char const* p1);
	auto initWithTMXFile(char const* p0);
	auto initWithXML(char const* p0, char const* p1);
	auto layerNamed(char const* p0);
	auto objectGroupNamed(char const* p0);
	auto parseLayer(cocos2d::CCTMXLayerInfo* p0, cocos2d::CCTMXMapInfo* p1);
	auto propertiesForGID(int p0);
	auto propertyNamed(char const* p0);
	auto tilesetForLayer(cocos2d::CCTMXLayerInfo* p0, cocos2d::CCTMXMapInfo* p1);
	~CCTMXTiledMap() = mac 0x2a63a0;
}

class cocos2d::CCTMXTilesetInfo : cocos2d::CCObject {
	auto CCTMXTilesetInfo();
	auto rectForGID(unsigned int p0);
	~CCTMXTilesetInfo() = mac 0x282ad0;
}

class cocos2d::CCTargetedAction : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f9d40;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f9d00;
	virtual auto stop() = mac 0x1f9d30;
	auto CCTargetedAction();
	auto create(cocos2d::CCNode* p0, cocos2d::CCFiniteTimeAction* p1);
	auto initWithTarget(cocos2d::CCNode* p0, cocos2d::CCFiniteTimeAction* p1);
}

class cocos2d::CCTargetedTouchHandler : cocos2d::CCTouchHandler {
	static cocos2d::CCTargetedTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate* p0, int p1, bool p2) = mac 0x248010;
	auto initWithDelegate(cocos2d::CCTouchDelegate* p0, int p1, bool p2) = mac 0x2480f0, ios 0x69;
	~CCTargetedTouchHandler() = mac 0x248180, ios 0x6cec;
	auto getClaimedTouches();
	auto isSwallowsTouches();
	auto setSwallowsTouches(bool p0);
}

class cocos2d::CCTextFieldDelegate {
	virtual auto onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF* p0);
	virtual auto onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF* p0);
	virtual auto onTextFieldInsertText(cocos2d::CCTextFieldTTF* p0, char const* p1, int p2);
	virtual auto onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF* p0, char const* p1, int p2);
	virtual auto onDraw(cocos2d::CCTextFieldTTF* p0);
	virtual auto textChanged();
}

class cocos2d::CCTextFieldTTF : cocos2d::CCLabelTTF, cocos2d::CCIMEDelegate {
	virtual auto draw() = mac 0x126a00, ios 0x12931c;
	virtual auto setString(char const* p0) = mac 0x126ac0, ios 0x1293fc;
	virtual auto getString() = mac 0x126d00, ios 0x129654;
	virtual auto attachWithIME() = mac 0x126370, ios 0x128cd8;
	virtual auto detachWithIME() = mac 0x1263f0, ios 0x128d28;
	virtual auto getColorSpaceHolder() = mac 0x126a90, ios 0x1293e0;
	virtual auto setColorSpaceHolder(cocos2d::ccColor3B const& p0) = mac 0x126aa0, ios 0x1293e8;
	virtual auto setPlaceHolder(char const* p0) = mac 0x126d20, ios 0x12967c;
	virtual auto getPlaceHolder() = mac 0x126e10, ios 0x1297bc;
	virtual auto setSecureTextEntry(bool p0) = mac 0x126e20, ios 0x1297d0;
	virtual auto isSecureTextEntry() = mac 0x126e70, ios 0x129838;
	virtual auto canAttachWithIME() = mac 0x126470, ios 0x128d78;
	virtual auto canDetachWithIME() = mac 0x1264d0, ios 0x128de8;
	virtual auto insertText(char const* p0, int p1) = mac 0x126530, ios 0x128e58;
	virtual auto deleteBackward() = mac 0x126760, ios 0x1290ac;
	virtual auto getContentText() = mac 0x1269e0, ios 0x1292f4;
	auto CCTextFieldTTF();
	auto initWithPlaceHolder(char const* p0, cocos2d::CCSize const& p1, cocos2d::CCTextAlignment p2, char const* p3, float p4);
	auto initWithPlaceHolder(char const* p0, char const* p1, float p2);
	auto textFieldWithPlaceHolder(char const* p0, cocos2d::CCSize const& p1, cocos2d::CCTextAlignment p2, char const* p3, float p4);
	auto textFieldWithPlaceHolder(char const* p0, char const* p1, float p2);
	~CCTextFieldTTF() = mac 0x125ff0, ios 0x128bdc;
}

class cocos2d::CCTexture2D : cocos2d::CCObject {
	~CCTexture2D() = mac 0x246350, ios 0xbdccc;
	CCTexture2D() = mac 0x246280;
	auto getContentSize() = mac 0x246460, ios 0xbdd28;
	auto getMaxS() = mac 0x2464e0, ios 0xbdd80;
	auto getMaxT() = mac 0x246500, ios 0xbdd90;
	auto getName() = mac 0x246450, ios 0xbdd20;
	auto getPixelFormat() = mac 0x246420, ios 0xbdd08;
	auto getPixelsHigh() = mac 0x246440, ios 0xbdd18;
	auto getPixelsWide() = mac 0x246430, ios 0xbdd10;
	auto getShaderProgram() = mac 0x246520, ios 0xbdda0;
	auto initPremultipliedATextureWithImage(cocos2d::CCImage* p0, unsigned int p1, unsigned int p2) = mac 0x2469a0;
	auto initWithData(void const* p0, cocos2d::CCTexture2DPixelFormat p1, unsigned int p2, unsigned int p3, cocos2d::CCSize const& p4) = mac 0x2465d0;
	auto setAliasTexParameters() = mac 0x247a20;
	auto setAntiAliasTexParameters() = mac 0x247a80;
	auto setMaxS(float p0) = mac 0x2464f0, ios 0xbdd88;
	auto setMaxT(float p0) = mac 0x246510, ios 0xbdd98;
	auto setShaderProgram(cocos2d::CCGLProgram* p0) = mac 0x246530, ios 0xbdda8;
	auto initWithImage(cocos2d::CCImage* p0) = mac 0x246940;
	auto setTexParameters(cocos2d::_ccTexParams* p0) = mac 0x247980;
	auto PVRImagesHavePremultipliedAlpha(bool p0);
	auto bitsPerPixelForFormat(cocos2d::CCTexture2DPixelFormat p0);
	auto bitsPerPixelForFormat();
	auto defaultAlphaPixelFormat();
	auto description();
	auto drawAtPoint(cocos2d::CCPoint const& p0);
	auto drawInRect(cocos2d::CCRect const& p0);
	auto generateMipmap();
	auto getContentSizeInPixels();
	auto hasMipmaps();
	auto hasPremultipliedAlpha();
	auto initWithData(auto const* p0, cocos2d::CCTexture2DPixelFormat p1, unsigned int p2, unsigned int p3, cocos2d::CCSize const& p4);
	auto initWithETCFile(char const* p0);
	auto initWithPVRFile(char const* p0);
	auto initWithString(char const* p0, cocos2d::_ccFontDefinition* p1);
	auto initWithString(char const* p0, char const* p1, float p2);
	auto initWithString(char const* p0, char const* p1, float p2, cocos2d::CCSize const& p3, cocos2d::CCTextAlignment p4, cocos2d::CCVerticalTextAlignment p5);
	auto keepData(void* p0, unsigned int p1);
	auto releaseData(void* p0);
	auto releaseGLTexture();
	auto setDefaultAlphaPixelFormat(cocos2d::CCTexture2DPixelFormat p0);
	auto stringForFormat();
}

class cocos2d::CCTextureAtlas : cocos2d::CCObject {
	virtual auto getTotalQuads() = mac 0x26fe70, ios 0x1a3e94;
	virtual auto getCapacity() = mac 0x26fe80, ios 0x1a3e9c;
	virtual auto getTexture() = mac 0x26fe90, ios 0x1a3ea4;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x26fea0, ios 0x1a3eac;
	virtual auto getQuads() = mac 0x26fee0, ios 0x1a3ee8;
	virtual auto setQuads(cocos2d::_ccV3F_C4B_T2F_Quad* p0) = mac 0x26fef0, ios 0x1a3ef8;
	auto CCTextureAtlas();
	auto create(char const* p0, unsigned int p1);
	auto createWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1);
	auto description();
	auto drawNumberOfQuads(unsigned int p0);
	auto drawNumberOfQuads(unsigned int p0, unsigned int p1);
	auto drawQuads();
	auto fillWithEmptyQuadsFromIndex(unsigned int p0, unsigned int p1);
	auto increaseTotalQuadsWith(unsigned int p0);
	auto initWithFile(char const* p0, unsigned int p1);
	auto initWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1);
	auto insertQuad(cocos2d::_ccV3F_C4B_T2F_Quad* p0, unsigned int p1);
	auto insertQuadFromIndex(unsigned int p0, unsigned int p1);
	auto insertQuads(cocos2d::_ccV3F_C4B_T2F_Quad* p0, unsigned int p1, unsigned int p2);
	auto listenBackToForeground(cocos2d::CCObject* p0);
	auto mapBuffers();
	auto moveQuadsFromIndex(unsigned int p0, unsigned int p1);
	auto moveQuadsFromIndex(unsigned int p0, unsigned int p1, unsigned int p2);
	auto removeAllQuads();
	auto removeQuadAtIndex(unsigned int p0);
	auto removeQuadsAtIndex(unsigned int p0, unsigned int p1);
	auto resizeCapacity(unsigned int p0);
	auto setupIndices();
	auto setupVBO();
	auto updateQuad(cocos2d::_ccV3F_C4B_T2F_Quad* p0, unsigned int p1);
	~CCTextureAtlas() = mac 0x26fe30, ios 0x1a3e58;
}

class cocos2d::CCTextureCache : cocos2d::CCObject {
	auto addImage(char const* p0, bool p1) = mac 0x358120, ios 0xa8388;
	static cocos2d::CCTextureCache* sharedTextureCache() = mac 0x356e00, ios 0xa81ec;
	auto CCTextureCache();
	auto addETCImage(char const* p0);
	auto addImageAsync(char const* p0, cocos2d::CCObject* p1, cocos2d::SEL_MenuHandler p2);
	auto addImageAsyncCallBack(float p0);
	auto addPVRImage(char const* p0);
	auto addUIImage(cocos2d::CCImage* p0, char const* p1);
	auto description();
	auto dumpCachedTextureInfo();
	auto purgeSharedTextureCache();
	auto reloadAllTextures();
	auto reloadTexture(char const* p0);
	auto removeAllTextures();
	auto removeTexture(cocos2d::CCTexture2D* p0);
	auto removeTextureForKey(char const* p0);
	auto removeUnusedTextures();
	auto snapshotTextures();
	auto textureForKey(char const* p0);
	~CCTextureCache() = mac 0x356fe0, ios 0xa8320;
}

class cocos2d::CCTexturePVR : cocos2d::CCObject {
	~CCTexturePVR() = mac 0x12f100, ios 0x27568c;
}

class cocos2d::CCTextureProtocol : cocos2d::CCBlendProtocol {}

class cocos2d::CCTileMapAtlas : cocos2d::CCAtlasNode {
	virtual auto updateAtlasValues() = mac 0x105f00;
	virtual auto getTGAInfo() = mac 0x106120;
	virtual auto setTGAInfo(cocos2d::sImageTGA* p0) = mac 0x106110;
	auto CCTileMapAtlas();
	auto calculateItemsToRender();
	auto create(char const* p0, char const* p1, int p2, int p3);
	auto initWithTileFile(char const* p0, char const* p1, int p2, int p3);
	auto loadTGAfile(char const* p0);
	auto releaseMap();
	auto setTile(cocos2d::ccColor3B const& p0, cocos2d::CCPoint const& p1);
	auto tileAt(cocos2d::CCPoint const& p0);
	auto updateAtlasValueAt(cocos2d::CCPoint const& p0, cocos2d::ccColor3B const& p1, unsigned int p2);
	~CCTileMapAtlas() = mac 0x105970;
}

class cocos2d::CCTiledGrid3D : cocos2d::CCGridBase {
	virtual auto blit() = mac 0x2891c0;
	virtual auto reuse() = mac 0x289780;
	virtual auto calculateVertexPoints() = mac 0x289260;
	auto CCTiledGrid3D();
	auto create(cocos2d::CCSize const& p0);
	auto create(cocos2d::CCSize const& p0, cocos2d::CCTexture2D* p1, bool p2);
	auto originalTile(cocos2d::CCPoint const& p0);
	auto setTile(cocos2d::CCPoint const& p0, cocos2d::_ccQuad3 const& p1);
	auto tile(cocos2d::CCPoint const& p0);
	~CCTiledGrid3D() = mac 0x288fe0;
}

class cocos2d::CCTiledGrid3DAction : cocos2d::CCGridAction {
	virtual auto getGrid() = mac 0xee350;
	auto CCTiledGrid3DAction();
	auto create(float p0, cocos2d::CCSize const& p1);
	auto originalTile(cocos2d::CCPoint const& p0);
	auto setTile(cocos2d::CCPoint const& p0, cocos2d::_ccQuad3 const& p1);
	auto tile(cocos2d::CCPoint const& p0);
}

class cocos2d::CCTime {
	static auto gettimeofdayCocos2d(cocos2d::cc_timeval* p0, void* p1) = mac 0x19eac0;
}

class cocos2d::CCTimer : cocos2d::CCObject {
	virtual auto update(float p0) = mac 0x242440, ios 0x10c048;
	auto CCTimer();
	auto getInterval();
	auto getSelector();
	auto initWithScriptHandler(int p0, float p1);
	auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_SCHEDULE p1);
	auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_SCHEDULE p1, float p2, unsigned int p3, float p4);
	auto setInterval(float p0);
	auto timerWithScriptHandler(int p0, float p1);
	auto timerWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_SCHEDULE p1);
	auto timerWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_SCHEDULE p1, float p2);
}

class cocos2d::CCTintBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f8870;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f8800;
	virtual auto reverse() = mac 0x1f8950;
	auto create(float p0, short p1, short p2, short p3);
	auto initWithDuration(float p0, short p1, short p2, short p3);
}

class cocos2d::CCTintTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCTintTo* create(float p0, unsigned char p1, unsigned char p2, unsigned char p3) = mac 0x1f82a0;
	virtual auto update(float p0) = mac 0x1f8510, ios 0xf3518;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f84b0, ios 0xf34ac;
	auto initWithDuration(float p0, unsigned char p1, unsigned char p2, unsigned char p3);
}

class cocos2d::CCToggleVisibility : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0);
	auto create();
}

class cocos2d::CCTouch : cocos2d::CCObject {
	auto getDelta() const = mac 0x38340;
	auto getLocationInView() const = mac 0x38250;
	auto getPreviousLocationInView() const = mac 0x38270;
	auto getLocation() const = mac 0x382b0, ios 0x21ce78;
	auto getPreviousLocation();
	auto getStartLocation();
	auto getStartLocationInView();
	~CCTouch();
}

class cocos2d::CCTouchDelegate {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchesBegan(cocos2d::CCSet* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchesMoved(cocos2d::CCSet* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchesEnded(cocos2d::CCSet* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchesCancelled(cocos2d::CCSet* p0, cocos2d::CCEvent* p1);
	~CCTouchDelegate();
}

class cocos2d::CCTouchDispatcher : cocos2d::CCObject, cocos2d::EGLTouchDelegate {
	auto addTargetedDelegate(cocos2d::CCTouchDelegate* p0, int p1, bool p2) = mac 0x281180;
	auto decrementForcePrio(int p0) = mac 0x280f70, ios 0xcc280;
	auto incrementForcePrio(int p0) = mac 0x280f60, ios 0xcc268;
	auto touches(cocos2d::CCSet* p0, cocos2d::CCEvent* p1, unsigned int p2) = mac 0x281a60;
	auto removeDelegate(cocos2d::CCTouchDelegate* delegate);
	virtual auto touchesBegan(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x282050, ios 0xccbf4;
	virtual auto touchesMoved(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x282090, ios 0xccc20;
	virtual auto touchesEnded(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x2820d0, ios 0xccc4c;
	virtual auto touchesCancelled(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x282110, ios 0xccc78;
	auto addStandardDelegate(cocos2d::CCTouchDelegate* p0, int p1);
	auto findHandler(cocos2d::CCTouchDelegate* p0);
	auto findHandler(cocos2d::CCArray* p0, cocos2d::CCTouchDelegate* p1);
	auto forceAddHandler(cocos2d::CCTouchHandler* p0, cocos2d::CCArray* p1);
	auto forceRemoveAllDelegates();
	auto forceRemoveDelegate(cocos2d::CCTouchDelegate* p0);
	auto init();
	auto isDispatchEvents();
	auto rearrangeHandlers(cocos2d::CCArray* p0);
	auto removeAllDelegates();
	auto setDispatchEvents(bool p0);
	auto setPriority(int p0, cocos2d::CCTouchDelegate* p1);
	~CCTouchDispatcher() = mac 0x280ed0, ios 0xcc21c;
}

class cocos2d::CCTouchHandler : cocos2d::CCObject {
	virtual auto initWithDelegate(cocos2d::CCTouchDelegate* p0, int p1) = mac 0x247d10, ios 0x69f8;
	auto getPriority() = mac 0x247c20;
	~CCTouchHandler() = mac 0x247de0, ios 0x6ac0;
	auto getDelegate();
	auto getEnabledSelectors();
	auto handlerWithDelegate(cocos2d::CCTouchDelegate* p0, int p1);
	auto setDelegate(cocos2d::CCTouchDelegate* p0);
	auto setEnalbedSelectors(int p0);
	auto setPriority(int p0);
}

class cocos2d::CCTouchScriptHandlerEntry : cocos2d::CCScriptHandlerEntry {
	auto create(int p0, bool p1, int p2, bool p3);
	auto init(bool p0, int p1, bool p2);
	~CCTouchScriptHandlerEntry() = mac 0x1987d0, ios 0x1a9040;
}

class cocos2d::CCTransitionCrossFade : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x8ef30;
	virtual auto onExit() = mac 0x8f290;
	virtual auto draw() = mac 0x8ef20;
	auto CCTransitionCrossFade();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionCrossFade() = mac 0x8ed70;
}

class cocos2d::CCTransitionEaseScene {}

class cocos2d::CCTransitionFade : cocos2d::CCTransitionScene {
	static cocos2d::CCTransitionFade* create(float p0, cocos2d::CCScene* p1) = mac 0x8ea30, ios 0x12c244;
	virtual auto onEnter() = mac 0x8eb30, ios 0x12c2d0;
	virtual auto onExit() = mac 0x8ec60, ios 0x12c3d4;
	virtual auto initWithDuration(float p0, cocos2d::CCScene* p1) = mac 0x8eb10, ios 0x12c2a8;
	virtual auto initWithDuration(float p0, cocos2d::CCScene* p1, cocos2d::ccColor3B const& p2) = mac 0x8ea50, ios 0x12c260;
	auto CCTransitionFade();
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::ccColor3B const& p2);
	~CCTransitionFade() = mac 0x8e860, ios 0x12c164;
}

class cocos2d::CCTransitionFadeBL : cocos2d::CCTransitionFadeTR {
	virtual auto actionWithSize(cocos2d::CCSize const& p0) = mac 0x90380;
	auto CCTransitionFadeBL();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionFadeBL() = mac 0x901c0;
}

class cocos2d::CCTransitionFadeDown : cocos2d::CCTransitionFadeTR {
	virtual auto actionWithSize(cocos2d::CCSize const& p0) = mac 0x908c0;
	auto CCTransitionFadeDown();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionFadeDown() = mac 0x90700;
}

class cocos2d::CCTransitionFadeTR : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8ffc0;
	virtual auto sceneOrder() = mac 0x8ffb0;
	virtual auto actionWithSize(cocos2d::CCSize const& p0) = mac 0x900c0;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x900e0;
	auto CCTransitionFadeTR();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionFadeTR() = mac 0x8fdf0;
}

class cocos2d::CCTransitionFadeUp : cocos2d::CCTransitionFadeTR {
	virtual auto actionWithSize(cocos2d::CCSize const& p0) = mac 0x90620;
	auto CCTransitionFadeUp();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionFadeUp() = mac 0x90460;
}

class cocos2d::CCTransitionFlipAngular : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8d7a0;
	auto CCTransitionFlipAngular();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionFlipAngular() = mac 0x8d6d0;
}

class cocos2d::CCTransitionFlipX : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8cf90;
	auto CCTransitionFlipX();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionFlipX() = mac 0x8cec0;
}

class cocos2d::CCTransitionFlipY : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8d390;
	auto CCTransitionFlipY();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionFlipY() = mac 0x8d2c0;
}

class cocos2d::CCTransitionJumpZoom : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x8abf0;
	auto CCTransitionJumpZoom();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionJumpZoom() = mac 0x8aa40;
}

class cocos2d::CCTransitionMoveInB : cocos2d::CCTransitionMoveInL {
	virtual auto initScenes() = mac 0x8ba90;
	auto CCTransitionMoveInB();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionMoveInB() = mac 0x8b8d0;
}

class cocos2d::CCTransitionMoveInL : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8b0b0, ios 0x12bf40;
	virtual auto initScenes() = mac 0x8b1e0;
	virtual auto action() = mac 0x8b180, ios 0x12bfe4;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x8b1c0, ios 0x12c024;
	auto CCTransitionMoveInL();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionMoveInL() = mac 0x8aef0;
}

class cocos2d::CCTransitionMoveInR : cocos2d::CCTransitionMoveInL {
	virtual auto initScenes() = mac 0x8b4d0;
	auto CCTransitionMoveInR();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionMoveInR() = mac 0x8b310;
}

class cocos2d::CCTransitionMoveInT : cocos2d::CCTransitionMoveInL {
	virtual auto initScenes() = mac 0x8b7b0, ios 0x12c10c;
	auto CCTransitionMoveInT();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionMoveInT() = mac 0x8b5f0, ios 0x12c034;
}

class cocos2d::CCTransitionPageTurn : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x20d1d0;
	virtual auto sceneOrder() = mac 0x20d1b0;
	virtual auto initWithDuration(float p0, cocos2d::CCScene* p1, bool p2) = mac 0x20d190;
	auto CCTransitionPageTurn();
	auto actionWithSize(cocos2d::CCSize const& p0);
	auto create(float p0, cocos2d::CCScene* p1, bool p2);
	~CCTransitionPageTurn() = mac 0x20d0d0;
}

class cocos2d::CCTransitionProgress : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x137970;
	virtual auto onExit() = mac 0x137b30;
	virtual auto sceneOrder() = mac 0x137b60;
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x137ba0;
	virtual auto setupTransition() = mac 0x137b70;
	auto CCTransitionProgress();
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressHorizontal : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x137f50;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressInOut : cocos2d::CCTransitionProgress {
	virtual auto sceneOrder() = mac 0x1382d0;
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x138300;
	virtual auto setupTransition() = mac 0x1382e0;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressOutIn : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x1384c0;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressRadialCCW : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x137bb0;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressRadialCW : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x137dd0;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressVertical : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x138110;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionRotoZoom : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x8a7d0;
	auto CCTransitionRotoZoom();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionRotoZoom() = mac 0x8a700;
}

class cocos2d::CCTransitionScene : cocos2d::CCScene {
	virtual auto onEnter() = mac 0x8a1a0, ios 0x12be58;
	virtual auto onExit() = mac 0x8a1f0, ios 0x12beac;
	virtual auto cleanup() = mac 0x8a250, ios 0x12bf00;
	virtual auto draw() = mac 0x89f40, ios 0x12bc10;
	virtual auto initWithDuration(float p0, cocos2d::CCScene* p1) = mac 0x89ea0, ios 0x12bb80;
	virtual auto sceneOrder() = mac 0x89f30, ios 0x12bc04;
	auto CCTransitionScene();
	auto create(float p0, cocos2d::CCScene* p1);
	auto finish();
	auto hideOutShowIn();
	auto setNewScene(float p0);
	~CCTransitionScene() = mac 0x89cf0, ios 0x12bb44;
}

class cocos2d::CCTransitionSceneOriented : cocos2d::CCTransitionScene {
	virtual auto initWithDuration(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2) = mac 0x8a4e0;
	auto CCTransitionSceneOriented();
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionSceneOriented() = mac 0x8a330;
}

class cocos2d::CCTransitionShrinkGrow : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8cc50;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x8cde0;
	auto CCTransitionShrinkGrow();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionShrinkGrow() = mac 0x8ca90;
}

class cocos2d::CCTransitionSlideInB : cocos2d::CCTransitionSlideInL {
	virtual auto sceneOrder() = mac 0x8c8f0;
	virtual auto initScenes() = mac 0x8c900;
	virtual auto action() = mac 0x8c970;
	auto CCTransitionSlideInB();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSlideInB() = mac 0x8c730;
}

class cocos2d::CCTransitionSlideInL : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8bc90;
	virtual auto sceneOrder() = mac 0x8bd80;
	virtual auto initScenes() = mac 0x8bd90;
	virtual auto action() = mac 0x8be00;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x8be60;
	auto CCTransitionSlideInL();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSlideInL() = mac 0x8bbc0;
}

class cocos2d::CCTransitionSlideInR : cocos2d::CCTransitionSlideInL {
	virtual auto sceneOrder() = mac 0x8c210;
	virtual auto initScenes() = mac 0x8c220;
	virtual auto action() = mac 0x8c290;
	auto CCTransitionSlideInR();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSlideInR() = mac 0x8c050;
}

class cocos2d::CCTransitionSlideInT : cocos2d::CCTransitionSlideInL {
	virtual auto sceneOrder() = mac 0x8c580;
	virtual auto initScenes() = mac 0x8c590;
	virtual auto action() = mac 0x8c600;
	auto CCTransitionSlideInT();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSlideInT() = mac 0x8c3c0;
}

class cocos2d::CCTransitionSplitCols : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8f940;
	virtual auto action() = mac 0x8fa30;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x8fa50;
	auto CCTransitionSplitCols();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSplitCols() = mac 0x8f780;
}

class cocos2d::CCTransitionSplitRows : cocos2d::CCTransitionSplitCols {
	virtual auto action() = mac 0x8fc20;
	auto CCTransitionSplitRows();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSplitRows() = mac 0x8fb50;
}

class cocos2d::CCTransitionTurnOffTiles : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8f590;
	virtual auto sceneOrder() = mac 0x8f580;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x8f6a0;
	auto CCTransitionTurnOffTiles();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionTurnOffTiles() = mac 0x8f3c0;
}

class cocos2d::CCTransitionZoomFlipAngular : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8e4a0;
	auto CCTransitionZoomFlipAngular();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionZoomFlipAngular() = mac 0x8e3d0;
}

class cocos2d::CCTransitionZoomFlipX : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8dbb0;
	auto CCTransitionZoomFlipX();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionZoomFlipX() = mac 0x8dae0;
}

class cocos2d::CCTransitionZoomFlipY : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8e020;
	auto CCTransitionZoomFlipY();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionZoomFlipY() = mac 0x8df50;
}

class cocos2d::CCTurnOffTiles : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45d870;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x45d7c0;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2) = mac 0x45d4d0;
	auto create(float p0, cocos2d::CCSize const& p1);
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2);
	auto shuffle(unsigned int* p0, unsigned int p1);
	auto turnOffTile(cocos2d::CCPoint const& p0);
	auto turnOnTile(cocos2d::CCPoint const& p0);
}

class cocos2d::CCTwirl : cocos2d::CCGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x129850;
	auto create(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint p2, unsigned int p3, float p4);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint p2, unsigned int p3, float p4);
	auto setPosition(cocos2d::CCPoint const& p0);
}

class cocos2d::CCWaves : cocos2d::CCGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x129340;
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3, bool p4, bool p5);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3, bool p4, bool p5);
}

class cocos2d::CCWaves3D : cocos2d::CCGrid3DAction {
	virtual auto update(float p0) = mac 0x127170;
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
}

class cocos2d::CCWavesTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45dba0;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3) = mac 0x45da40;
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
}

class cocos2d::EGLTouchDelegate {
	~EGLTouchDelegate();
}

class cocos2d::TypeInfo {}

class cocos2d::ZipFile {
	auto ZipFile(gd::string const& p0, gd::string const& p1);
	auto fileExists(gd::string const& p0);
	auto getFileData(gd::string const& p0, unsigned long* p1);
	auto getFileData(gd::string const& p0, unsigned long* p1, cocos2d::ZipFilePrivate* p2);
	auto setFilter(gd::string const& p0);
	auto setFilter(gd::string const& p0, cocos2d::ZipFilePrivate* p1);
	~ZipFile();
}

class cocos2d::extension::CCArrayForObjectSorting : cocos2d::CCArray {
	~CCArrayForObjectSorting();
}

class cocos2d::extension::CCControl : cocos2d::CCLayerRGBA {
	virtual bool init() = mac 0x1a71c0, ios 0x10644c;
	~CCControl() = mac 0x1a7380, ios 0x1065a0;
	auto sendActionsForControlEvents(cocos2d::extension::CCControlEvent p0) = mac 0x1a7490;
	auto registerWithTouchDispatcher() = mac 0x1a7420, ios 0x1065ec;
	auto setOpacityModifyRGB(bool p0) = mac 0x1a7c10, ios 0x106aac;
	auto onExit() = mac 0x1a7480, ios 0x10664c;
	auto isTouchInside(cocos2d::CCTouch* p0) = mac 0x1a7de0, ios 0x106bb8;
	auto setEnabled(bool p0) = mac 0x1a7e60, ios 0x106c2c;
	auto onEnter() = mac 0x1a7470, ios 0x106648;
	auto isEnabled() = mac 0x1a7e90, ios 0x106c4c;
	auto setSelected(bool p0) = mac 0x1a7ea0, ios 0x106c54;
	auto isOpacityModifyRGB() = mac 0x1a7d70, ios 0x106b5c;
	auto setHighlighted(bool p0) = mac 0x1a7ed0, ios 0x106c6c;
	auto needsLayout() = mac 0x1a7e50, ios 0x106c28;
	auto getTouchLocation(cocos2d::CCTouch* p0) = mac 0x1a7d90, ios 0x106b6c;
	auto isHighlighted() = mac 0x1a7ef0, ios 0x106c7c;
	auto addTargetWithActionForControlEvents(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, cocos2d::extension::CCControlEvent p2) = mac 0x1a7820;
	auto removeTargetWithActionForControlEvents(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, cocos2d::extension::CCControlEvent p2) = mac 0x1a7950;
	auto isSelected() = mac 0x1a7ec0, ios 0x106c64;
	virtual auto sendActionsForControlEvents(unsigned int p0);
	virtual auto addTargetWithActionForControlEvents(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
	virtual auto removeTargetWithActionForControlEvents(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
	auto CCControl();
	auto addHandleOfControlEvent(int p0, unsigned int p1);
	auto addTargetWithActionForControlEvent(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
	auto create();
	auto dispatchListforControlEvent(unsigned int p0);
	auto getHandleOfControlEvent(unsigned int p0);
	auto hasVisibleParents();
	auto removeHandleOfControlEvent(unsigned int p0);
	auto removeTargetWithActionForControlEvent(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
}

class cocos2d::extension::CCControlButton : cocos2d::extension::CCControl {
	virtual auto init() = mac 0x1a8420;
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1a9c20;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1a9db0;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1a9ef0;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1aa1a0;
	virtual auto getOpacity() = mac 0x1aa090;
	virtual auto setOpacity(unsigned char p0) = mac 0x1a9fc0;
	virtual auto getColor() = mac 0x1aa180;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x1aa0b0;
	virtual auto setEnabled(bool p0) = mac 0x1a8b00;
	virtual auto setSelected(bool p0) = mac 0x1a8b30;
	virtual auto setHighlighted(bool p0) = mac 0x1a8b60;
	virtual auto needsLayout() = mac 0x1a9700;
	virtual auto getPreferredSize() = mac 0x1a8d20;
	virtual auto setPreferredSize(cocos2d::CCSize p0) = mac 0x1a8c60;
	virtual auto getZoomOnTouchDown() = mac 0x1a8c50;
	virtual auto setZoomOnTouchDown(bool p0) = mac 0x1a8c40;
	virtual auto getLabelAnchorPoint() = mac 0x1a8d50;
	virtual auto setLabelAnchorPoint(cocos2d::CCPoint p0) = mac 0x1a8d70;
	virtual auto setMargins(int p0, int p1) = mac 0x1a8ae0;
	virtual auto initWithLabelAndBackgroundSprite(cocos2d::CCNode* p0, cocos2d::extension::CCScale9Sprite* p1) = mac 0x1a8480;
	virtual auto initWithTitleAndFontNameAndFontSize(gd::string p0, char const* p1, float p2) = mac 0x1a88f0;
	virtual auto initWithBackgroundSprite(cocos2d::extension::CCScale9Sprite* p0) = mac 0x1a8a30;
	virtual auto getTitleForState(unsigned int p0) = mac 0x1a8db0;
	virtual auto setTitleForState(cocos2d::CCString* p0, unsigned int p1) = mac 0x1a8e90;
	virtual auto getTitleColorForState(unsigned int p0) = mac 0x1a8f00;
	virtual auto setTitleColorForState(cocos2d::ccColor3B p0, unsigned int p1) = mac 0x1a8f60;
	virtual auto getTitleLabelForState(unsigned int p0) = mac 0x1a9020;
	virtual auto setTitleLabelForState(cocos2d::CCNode* p0, unsigned int p1) = mac 0x1a9060;
	virtual auto setTitleTTFForState(char const* p0, unsigned int p1) = mac 0x1a9140;
	virtual auto getTitleTTFForState(unsigned int p0) = mac 0x1a9240;
	virtual auto setTitleTTFSizeForState(float p0, unsigned int p1) = mac 0x1a92b0;
	virtual auto getTitleTTFSizeForState(unsigned int p0) = mac 0x1a9330;
	virtual auto setTitleBMFontForState(char const* p0, unsigned int p1) = mac 0x1a93a0;
	virtual auto getTitleBMFontForState(unsigned int p0) = mac 0x1a9490;
	virtual auto getBackgroundSpriteForState(unsigned int p0) = mac 0x1a9500;
	virtual auto setBackgroundSpriteForState(cocos2d::extension::CCScale9Sprite* p0, unsigned int p1) = mac 0x1a9540;
	virtual auto setBackgroundSpriteFrameForState(cocos2d::CCSpriteFrame* p0, unsigned int p1) = mac 0x1a96d0;
	auto CCControlButton();
	auto create(cocos2d::extension::CCScale9Sprite* p0);
	auto create(cocos2d::CCNode* p0, cocos2d::extension::CCScale9Sprite* p1);
	auto create(gd::string p0, char const* p1, float p2);
	auto create();
	auto doesAdjustBackgroundImage();
	auto setAdjustBackgroundImage(bool p0);
	auto setBackgroundSprite(cocos2d::extension::CCScale9Sprite* p0);
	auto setTitleLabel(cocos2d::CCNode* p0);
	~CCControlButton() = mac 0x1a8380;
}

class cocos2d::extension::CCControlColorPicker : cocos2d::extension::CCControl {
	~CCControlColorPicker() = mac 0x1aae30;
	auto setColorValue(cocos2d::ccColor3B const& p0) = mac 0x1aac10;
	auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1aae10;
	auto init() = mac 0x1aa400;
	static auto colorPicker() = mac 0x1aaa30;
	auto colorSliderValueChanged(cocos2d::CCObject* p0, unsigned int p1);
	auto create();
	auto hueSliderValueChanged(cocos2d::CCObject* p0, unsigned int p1);
	auto updateControlPicker();
	auto updateHueAndControlPicker();
}

class cocos2d::extension::CCControlHuePicker : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1ac050, ios 0x1147a8;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1ac150, ios 0x114830;
	virtual auto registerWithTouchDispatcher() = mac 0x1abcd0, ios 0x1144c4;
	virtual auto setEnabled(bool p0) = mac 0x1abe70, ios 0x114624;
	virtual auto setHue(float p0) = mac 0x1abd40, ios 0x114534;
	virtual auto setHuePercentage(float p0) = mac 0x1abd70, ios 0x114550;
	virtual auto initWithTargetAndPos(cocos2d::CCNode* p0, cocos2d::CCPoint p1) = mac 0x1abb30, ios 0x1142ec;
	auto CCControlHuePicker();
	auto checkSliderPosition(cocos2d::CCPoint p0, bool p1);
	auto create(cocos2d::CCNode* p0, cocos2d::CCPoint p1);
	auto updateSliderPosition(cocos2d::CCPoint p0);
	~CCControlHuePicker() = mac 0x1ab960, ios 0x11421c;
}

class cocos2d::extension::CCControlPotentiometer : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1ade70;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1ae030;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1ae2a0;
	virtual auto setEnabled(bool p0) = mac 0x1adb80;
	virtual auto isTouchInside(cocos2d::CCTouch* p0) = mac 0x1add50;
	auto CCControlPotentiometer();
	auto angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint(cocos2d::CCPoint p0, cocos2d::CCPoint p1, cocos2d::CCPoint p2, cocos2d::CCPoint p3);
	auto create(char const* p0, char const* p1, char const* p2);
	auto distanceBetweenPointAndPoint(cocos2d::CCPoint p0, cocos2d::CCPoint p1);
	auto getMaximumValue();
	auto getMinimumValue();
	auto getValue();
	auto initWithTrackSprite_ProgressTimer_ThumbSprite(cocos2d::CCSprite* p0, cocos2d::CCProgressTimer* p1, cocos2d::CCSprite* p2);
	auto potentiometerBegan(cocos2d::CCPoint p0);
	auto potentiometerEnded(cocos2d::CCPoint p0);
	auto potentiometerMoved(cocos2d::CCPoint p0);
	auto setMaximumValue(float p0);
	auto setMinimumValue(float p0);
	auto setValue(float p0);
	~CCControlPotentiometer() = mac 0x1ad590;
}

class cocos2d::extension::CCControlSaturationBrightnessPicker : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1b4030, ios 0x11fb14;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1b4130, ios 0x11fb9c;
	virtual auto registerWithTouchDispatcher() = mac 0x1b3fc0, ios 0x11faa4;
	virtual auto setEnabled(bool p0) = mac 0x1b3b00, ios 0x11f6bc;
	virtual auto initWithTargetAndPos(cocos2d::CCNode* p0, cocos2d::CCPoint p1) = mac 0x1b3820, ios 0x11f4b8;
	virtual auto updateWithHSV(cocos2d::extension::HSV p0) = mac 0x1b3b50, ios 0x11f70c;
	virtual auto updateDraggerWithHSV(cocos2d::extension::HSV p0) = mac 0x1b3bf0, ios 0x11f77c;
	auto CCControlSaturationBrightnessPicker();
	auto checkSliderPosition(cocos2d::CCPoint p0, bool p1);
	auto create(cocos2d::CCNode* p0, cocos2d::CCPoint p1);
	auto updateSliderPosition(cocos2d::CCPoint p0);
	~CCControlSaturationBrightnessPicker() = mac 0x1b3550, ios 0x11f46c;
}

class cocos2d::extension::CCControlSlider : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1de890;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1deaf0;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1decc0;
	virtual auto setEnabled(bool p0) = mac 0x1de610;
	virtual auto needsLayout() = mac 0x1deea0;
	virtual auto isTouchInside(cocos2d::CCTouch* p0) = mac 0x1de750;
	virtual auto setValue(float p0) = mac 0x1de660;
	virtual auto setMinimumValue(float p0) = mac 0x1de6b0;
	virtual auto setMaximumValue(float p0) = mac 0x1de700;
	virtual auto initWithSprites(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2) = mac 0x1de320;
	auto CCControlSlider();
	auto create(char const* p0, char const* p1, char const* p2);
	auto create(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2);
	auto locationFromTouch(cocos2d::CCTouch* p0);
	auto sliderBegan(cocos2d::CCPoint p0);
	auto sliderEnded(cocos2d::CCPoint p0);
	auto sliderMoved(cocos2d::CCPoint p0);
	auto valueForLocation(cocos2d::CCPoint p0);
	~CCControlSlider() = mac 0x1de040;
}

class cocos2d::extension::CCControlStepper : cocos2d::extension::CCControl {
	virtual auto update(float p0) = mac 0x1e12a0;
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e1470;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e1620;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e1750;
	virtual auto setWraps(bool p0) = mac 0x1e0fe0;
	virtual auto setMinimumValue(double p0) = mac 0x1e1060;
	virtual auto setMaximumValue(double p0) = mac 0x1e1090;
	virtual auto setValue(double p0) = mac 0x1e10c0;
	virtual auto getValue() = mac 0x1e10e0;
	virtual auto setStepValue(double p0) = mac 0x1e10f0;
	virtual auto setValueWithSendingEvent(double p0, bool p1) = mac 0x1e1110;
	virtual auto isContinuous() = mac 0x1e1100;
	auto CCControlStepper();
	auto create(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1);
	auto initWithMinusSpriteAndPlusSprite(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1);
	auto startAutorepeat();
	auto stopAutorepeat();
	auto updateLayoutUsingTouchLocation(cocos2d::CCPoint p0);
	~CCControlStepper() = mac 0x1e0960;
}

class cocos2d::extension::CCControlSwitch : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e5bc0;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e5cb0;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e5d80;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e5ef0;
	virtual auto setEnabled(bool p0) = mac 0x1e5b30;
	auto CCControlSwitch();
	auto create(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2, cocos2d::CCSprite* p3);
	auto create(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2, cocos2d::CCSprite* p3, cocos2d::CCLabelTTF* p4, cocos2d::CCLabelTTF* p5);
	auto initWithMaskSprite(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2, cocos2d::CCSprite* p3);
	auto initWithMaskSprite(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2, cocos2d::CCSprite* p3, cocos2d::CCLabelTTF* p4, cocos2d::CCLabelTTF* p5);
	auto locationFromTouch(cocos2d::CCTouch* p0);
	auto setOn(bool p0);
	auto setOn(bool p0, bool p1);
	~CCControlSwitch() = mac 0x1e5330;
}

class cocos2d::extension::CCControlSwitchSprite : cocos2d::CCSprite, cocos2d::CCActionTweenDelegate {
	virtual auto draw() = mac 0x1e4f90;
	virtual auto updateTweenAction(float p0, char const* p1) = mac 0x1e4ed0;
	auto CCControlSwitchSprite();
	auto initWithMaskSprite(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2, cocos2d::CCSprite* p3, cocos2d::CCLabelTTF* p4, cocos2d::CCLabelTTF* p5);
	auto needsLayout();
	auto offSideWidth();
	auto onSideWidth();
	auto setSliderXPosition(float p0);
	~CCControlSwitchSprite() = mac 0x1e4760;
}

class cocos2d::extension::CCControlUtils {
	static cocos2d::extension::HSV HSVfromRGB(cocos2d::extension::RGBA p0) = mac 0x1e6750;
	static cocos2d::extension::RGBA RGBfromHSV(cocos2d::extension::HSV p0) = mac 0x1e6850;
}

class cocos2d::extension::CCEditBox : cocos2d::extension::CCControlButton, cocos2d::CCIMEDelegate {
	virtual auto setPosition(cocos2d::CCPoint const& p0) = mac 0x26d850;
	virtual auto setAnchorPoint(cocos2d::CCPoint const& p0) = mac 0x26d910;
	virtual auto setContentSize(cocos2d::CCSize const& p0) = mac 0x26d8d0;
	virtual auto setVisible(bool p0) = mac 0x26d890;
	virtual auto onEnter() = mac 0x26d980;
	virtual auto onExit() = mac 0x26d9b0;
	virtual auto visit() = mac 0x26d950;
	virtual auto keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& p0) = mac 0x26d9e0;
	virtual auto keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo& p0) = mac 0x26daf0;
	virtual auto keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& p0) = mac 0x26db10;
	virtual auto keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo& p0) = mac 0x26db80;
	auto CCEditBox();
	auto create(cocos2d::CCSize const& p0, cocos2d::extension::CCScale9Sprite* p1, cocos2d::extension::CCScale9Sprite* p2, cocos2d::extension::CCScale9Sprite* p3);
	auto getDelegate();
	auto getMaxLength();
	auto getPlaceHolder();
	auto getText();
	auto initWithSizeAndBackgroundSprite(cocos2d::CCSize const& p0, cocos2d::extension::CCScale9Sprite* p1);
	auto registerScriptEditBoxHandler(int p0);
	auto setDelegate(cocos2d::extension::CCEditBoxDelegate* p0);
	auto setFont(char const* p0, int p1);
	auto setFontColor(cocos2d::ccColor3B const& p0);
	auto setFontName(char const* p0);
	auto setFontSize(int p0);
	auto setInputFlag(cocos2d::extension::EditBoxInputFlag p0);
	auto setInputMode(cocos2d::extension::EditBoxInputMode p0);
	auto setMaxLength(int p0);
	auto setPlaceHolder(char const* p0);
	auto setPlaceholderFont(char const* p0, int p1);
	auto setPlaceholderFontColor(cocos2d::ccColor3B const& p0);
	auto setPlaceholderFontName(char const* p0);
	auto setPlaceholderFontSize(int p0);
	auto setReturnType(cocos2d::extension::KeyboardReturnType p0);
	auto setText(char const* p0);
	auto touchDownAction(cocos2d::CCObject* p0, unsigned int p1);
	auto unregisterScriptEditBoxHandler();
	~CCEditBox() = mac 0x26cfd0;
}

class cocos2d::extension::CCEditBoxImpl {
	~CCEditBoxImpl();
}

class cocos2d::extension::CCHttpClient : cocos2d::CCObject {
	auto CCHttpClient();
	auto destroyInstance();
	auto dispatchResponseCallbacks(float p0);
	auto getInstance();
	auto lazyInitThreadSemphore();
	auto send(cocos2d::extension::CCHttpRequest* p0);
	~CCHttpClient() = mac 0x23da60, ios 0x1638ec;
}

class cocos2d::extension::CCHttpRequest : cocos2d::CCObject {
	auto CCHttpRequest();
	~CCHttpRequest();
}

class cocos2d::extension::CCHttpResponse : cocos2d::CCObject {
	auto CCHttpResponse(cocos2d::extension::CCHttpRequest* p0);
	~CCHttpResponse();
}

class cocos2d::extension::CCInvocation : cocos2d::CCObject {
	auto CCInvocation(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
	auto create(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
	auto invoke(cocos2d::CCObject* p0);
	~CCInvocation();
}

class cocos2d::extension::CCScale9Sprite : cocos2d::CCNodeRGBA {
	CCScale9Sprite() = mac 0x211330;
	static cocos2d::extension::CCScale9Sprite* create(char const* p0) = mac 0x2130d0;
	static cocos2d::extension::CCScale9Sprite* create(char const* p0, cocos2d::CCRect p1) = mac 0x212ef0;
	~CCScale9Sprite() = mac 0x211590, ios 0x13d4b8;
	virtual auto init() = mac 0x2115d0, ios 0x13d4f4;
	virtual auto setContentSize(cocos2d::CCSize const& p0) = mac 0x2127c0, ios 0x13e400;
	virtual auto visit() = mac 0x213bf0, ios 0x13f144;
	virtual auto getOpacity() = mac 0x213f30, ios 0x13f30c;
	virtual auto setOpacity(unsigned char p0) = mac 0x213dd0, ios 0x13f250;
	virtual auto updateDisplayedOpacity(unsigned char p0) = mac 0x2139b0, ios 0x13ef98;
	virtual auto getColor() = mac 0x213db0, ios 0x13f240;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x213c20, ios 0x13f178;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const& p0) = mac 0x213a20, ios 0x13f004;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x213830, ios 0x13eed0;
	virtual auto isOpacityModifyRGB() = mac 0x213990, ios 0x13ef88;
	virtual auto getPreferredSize() = mac 0x213620, ios 0x13ed18;
	virtual auto setPreferredSize(cocos2d::CCSize p0) = mac 0x2135f0, ios 0x13ece4;
	virtual auto getCapInsets() = mac 0x2136e0, ios 0x13edc4;
	virtual auto setCapInsets(cocos2d::CCRect p0) = mac 0x213640, ios 0x13ed24;
	virtual auto getInsetLeft() = mac 0x213b30, ios 0x13f104;
	virtual auto setInsetLeft(float p0) = mac 0x213b70, ios 0x13f124;
	virtual auto getInsetTop() = mac 0x213b40, ios 0x13f10c;
	virtual auto setInsetTop(float p0) = mac 0x213b90, ios 0x13f12c;
	virtual auto getInsetRight() = mac 0x213b50, ios 0x13f114;
	virtual auto setInsetRight(float p0) = mac 0x213bb0, ios 0x13f134;
	virtual auto getInsetBottom() = mac 0x213b60, ios 0x13f11c;
	virtual auto setInsetBottom(float p0) = mac 0x213bd0, ios 0x13f13c;
	virtual auto initWithBatchNode(cocos2d::CCSpriteBatchNode* p0, cocos2d::CCRect p1, bool p2, cocos2d::CCRect p3) = mac 0x211690, ios 0x13d5c8;
	virtual auto initWithBatchNode(cocos2d::CCSpriteBatchNode* p0, cocos2d::CCRect p1, cocos2d::CCRect p2) = mac 0x211630, ios 0x13d55c;
	virtual auto initWithFile(char const* p0, cocos2d::CCRect p1, cocos2d::CCRect p2) = mac 0x212d50, ios 0x13e8e0;
	virtual auto initWithFile(char const* p0, cocos2d::CCRect p1) = mac 0x212e80, ios 0x13e964;
	virtual auto initWithFile(cocos2d::CCRect p0, char const* p1) = mac 0x212f80, ios 0x13ea6c;
	virtual auto initWithFile(char const* p0) = mac 0x213080, ios 0x13ead8;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame* p0, cocos2d::CCRect p1) = mac 0x213140, ios 0x13eb30;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame* p0) = mac 0x213250, ios 0x13ebbc;
	virtual auto initWithSpriteFrameName(char const* p0, cocos2d::CCRect p1) = mac 0x213310, ios 0x13ec14;
	virtual auto initWithSpriteFrameName(char const* p0) = mac 0x213410, ios 0x13ec8c;
	virtual auto updateWithBatchNode(cocos2d::CCSpriteBatchNode* p0, cocos2d::CCRect p1, bool p2, cocos2d::CCRect p3) = mac 0x211730, ios 0x13d674;
	virtual auto setSpriteFrame(cocos2d::CCSpriteFrame* p0) = mac 0x213a90, ios 0x13f070;
	auto create(cocos2d::CCRect p0, char const* p1);
	auto create(char const* p0, cocos2d::CCRect p1, cocos2d::CCRect p2);
	auto create();
	auto createWithSpriteFrame(cocos2d::CCSpriteFrame* p0);
	auto createWithSpriteFrame(cocos2d::CCSpriteFrame* p0, cocos2d::CCRect p1);
	auto createWithSpriteFrameName(char const* p0);
	auto createWithSpriteFrameName(char const* p0, cocos2d::CCRect p1);
	auto resizableSpriteWithCapInsets(cocos2d::CCRect p0);
	auto setBlendAdditive(bool p0);
	auto updateCapInset();
	auto updatePositions();
}

class cocos2d::extension::CCScrollView : cocos2d::CCLayer {
	CCScrollView() = mac 0x214800;
	~CCScrollView() = mac 0x214c30;
	virtual auto init() = mac 0x214fb0;
	virtual auto setContentSize(cocos2d::CCSize const& p0) = mac 0x215eb0;
	virtual auto getContentSize() const = mac 0x215e90;
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0x216160;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0x216120;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x216080;
	virtual auto visit() = mac 0x2164a0;
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x216620;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x216890;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x216c70;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x216d30;
	virtual auto registerWithTouchDispatcher() = mac 0x214ff0;
	virtual auto setTouchEnabled(bool p0) = mac 0x215250;
	virtual auto setDirection(cocos2d::extension::CCScrollViewDirection p0);
	auto afterDraw();
	auto beforeDraw();
	auto create(cocos2d::CCSize p0, cocos2d::CCNode* p1);
	auto create();
	auto deaccelerateScrolling(float p0);
	auto getContainer();
	auto getContentOffset();
	auto getScriptHandler(int p0);
	auto getViewRect();
	auto getZoomScale();
	auto initWithViewSize(cocos2d::CCSize p0, cocos2d::CCNode* p1);
	auto isNodeVisible(cocos2d::CCNode* p0);
	auto maxContainerOffset();
	auto minContainerOffset();
	auto pause(cocos2d::CCObject* p0);
	auto performedAnimatedScroll(float p0);
	auto registerScriptHandler(int p0, int p1);
	auto relocateContainer(bool p0);
	auto resume(cocos2d::CCObject* p0);
	auto setContainer(cocos2d::CCNode* p0);
	auto setContentOffset(cocos2d::CCPoint p0, bool p1);
	auto setContentOffsetInDuration(cocos2d::CCPoint p0, float p1);
	auto setViewSize(cocos2d::CCSize p0);
	auto setZoomScale(float p0);
	auto setZoomScale(float p0, bool p1);
	auto setZoomScaleInDuration(float p0, float p1);
	auto stoppedAnimatedScroll(cocos2d::CCNode* p0);
	auto unregisterScriptHandler(int p0);
	auto updateInset();
}

class cocos2d::extension::CCScrollViewDelegate {
	~CCScrollViewDelegate();
}

class cocos2d::extension::CCSortableObject {
	~CCSortableObject();
}

class cocos2d::extension::CCTableView : cocos2d::extension::CCScrollView, cocos2d::extension::CCScrollViewDelegate {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x234bf0;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x234da0;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x234ae0;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x234e50;
	virtual auto scrollViewDidScroll(cocos2d::extension::CCScrollView* p0) = mac 0x234830;
	virtual auto scrollViewDidZoom(cocos2d::extension::CCScrollView* p0);
	auto CCTableView();
	auto __indexFromOffset(cocos2d::CCPoint p0);
	auto __offsetFromIndex(unsigned int p0);
	auto _addCellIfNecessary(cocos2d::extension::CCTableViewCell* p0);
	auto _indexFromOffset(cocos2d::CCPoint p0);
	auto _moveCellOutOfSight(cocos2d::extension::CCTableViewCell* p0);
	auto _offsetFromIndex(unsigned int p0);
	auto _setIndexForCell(unsigned int p0, cocos2d::extension::CCTableViewCell* p1);
	auto _updateCellPositions();
	auto _updateContentSize();
	auto cellAtIndex(unsigned int p0);
	auto create(cocos2d::extension::CCTableViewDataSource* p0, cocos2d::CCSize p1);
	auto create(cocos2d::extension::CCTableViewDataSource* p0, cocos2d::CCSize p1, cocos2d::CCNode* p2);
	auto dequeueCell();
	auto getVerticalFillOrder();
	auto initWithViewSize(cocos2d::CCSize p0, cocos2d::CCNode* p1);
	auto insertCellAtIndex(unsigned int p0);
	auto reloadData();
	auto removeCellAtIndex(unsigned int p0);
	auto setVerticalFillOrder(cocos2d::extension::CCTableViewVerticalFillOrder p0);
	auto unregisterAllScriptHandler();
	auto updateCellAtIndex(unsigned int p0);
	~CCTableView() = mac 0x233a60;
}

class cocos2d::extension::CCTableViewCell : cocos2d::CCNode, cocos2d::extension::CCSortableObject {
	virtual auto setObjectID(unsigned int p0);
	virtual auto getObjectID();
	auto getIdx();
	auto reset();
	auto setIdx(unsigned int p0);
	~CCTableViewCell();
}

class cocos2d::extension::ColorPickerDelegate {
	virtual auto colorValueChanged(cocos2d::ccColor3B p0);
}

