class AccountHelpLayer : GJDropDownLayer, GJAccountDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void accountStatusChanged();
	void create();
	void customSetup();
	void doUnlink();
	void exitLayer();
	void layerHidden();
	void onAccountManagement(cocos2d::CCObject*);
	void onReLogin(cocos2d::CCObject*);
	void onUnlink(cocos2d::CCObject*);
	void updatePage();
	void verifyUnlink();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
}
class AccountLayer : GJDropDownLayer, GJAccountDelegate, GJAccountBackupDelegate, GJAccountSyncDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void accountStatusChanged();
	void backupAccountFailed(BackupAccountError);
	void backupAccountFinished();
	void create();
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void customSetup();
	void doBackup();
	void doSync();
	void exitLayer();
	void hideLoadingUI();
	void layerHidden();
	void onBackup(cocos2d::CCObject*);
	void onHelp(cocos2d::CCObject*);
	void onLogin(cocos2d::CCObject*);
	void onMore(cocos2d::CCObject*);
	void onRegister(cocos2d::CCObject*);
	void onSync(cocos2d::CCObject*);
	void showLoadingUI();
	void syncAccountFailed(BackupAccountError);
	void syncAccountFinished();
	void toggleUI(bool);
	void updatePage(bool);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void backupAccountFailed(BackupAccountError);
	virtual void syncAccountFailed(BackupAccountError);
}
class AccountLoginLayer : FLAlertLayer, TextInputDelegate, GJAccountLoginDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(gd::string);
	void createTextBackground(cocos2d::CCPoint, char const*, cocos2d::CCSize);
	void createTextInput(cocos2d::CCPoint, cocos2d::CCSize, char const*, int);
	void createTextLabel(cocos2d::CCPoint, char const*, cocos2d::CCSize);
	void disableNodes();
	void hideLoadingUI();
	void init(gd::string);
	void keyBackClicked();
	void loginAccountFailed(AccountError);
	void loginAccountFinished(int, int);
	void onClose(cocos2d::CCObject*);
	void onForgotPassword(cocos2d::CCObject*);
	void onForgotUser(cocos2d::CCObject*);
	void onSubmit(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void resetLabel(int);
	void resetLabels();
	void showLoadingUI();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void toggleUI(bool);
	void updateLabel(AccountError);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void loginAccountFailed(AccountError);
	virtual void loginAccountFinished(int,int);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
}
class AccountRegisterLayer : FLAlertLayer, TextInputDelegate, GJAccountRegisterDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void allowTextInput(CCTextInputNode*);
	void create();
	void createTextBackground(cocos2d::CCPoint, cocos2d::CCSize);
	void createTextInput(cocos2d::CCPoint, cocos2d::CCSize, gd::string, int);
	void createTextLabel(cocos2d::CCPoint, gd::string, cocos2d::CCSize);
	void disableNodes();
	void hideLoadingUI();
	void init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSubmit(cocos2d::CCObject*);
	void registerAccountFailed(AccountError);
	void registerAccountFinished();
	void registerWithTouchDispatcher();
	void resetLabel(int);
	void resetLabels();
	void showLoadingUI();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void toggleUI(bool);
	void updateLabel(AccountError);
	void validEmail(gd::string);
	void validPassword(gd::string);
	void validUser(gd::string);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void allowTextInput(CCTextInputNode *);
	virtual void registerAccountFailed(AccountError);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class AchievementBar : cocos2d::CCNodeRGBA {
	void create(char const*, char const*, char const*, bool);
	void init(char const*, char const*, char const*, bool);
	void setOpacity(unsigned char);
	void show();
	virtual void setOpacity(uchar);
}
class AchievementCell : TableViewCell {
	void draw();
	void init();
	void loadFromDict(cocos2d::CCDictionary*);
	void updateBGColor(int);
}
class AchievementManager : cocos2d::CCNode {
	void achievementForUnlock(int, UnlockType);
	void areAchievementsEarned(cocos2d::CCArray*);
	void checkAchFromUnlock(char const*);
	void dataLoaded(DS_Dictionary*);
	void encodeDataTo(DS_Dictionary*);
	void firstSetup();
	void getAchievementRewardDict();
	void getAchievementsWithID(char const*);
	void getAllAchievements();
	void getAllAchievementsSorted();
	void init();
	void isAchievementEarned(char const*);
	void notifyAchievement(char const*, char const*, char const*);
	void notifyAchievementWithID(char const*);
	void percentForAchievement(char const*);
	void percentageForCount(int, int);
	void reportAchievementWithID(char const*, int, bool);
	void reportPlatformAchievementWithID(char const*, int);
	void resetAchievement(char const*);
	void resetAchievements();
	void setReportedAchievements(cocos2d::CCDictionary*);
	void setup();
	void sharedState();
	void storeAchievementUnlocks();
}
class AchievementNotifier : cocos2d::CCNode {
	void achievementDisplayFinished();
	void init();
	void notifyAchievement(char const*, char const*, char const*, bool);
	void sharedState();
	void showNextAchievement();
	void willSwitchToScene(cocos2d::CCScene*);
}
class AchievementsLayer : GJDropDownLayer {
	void create();
	void customSetup();
	void keyDown(cocos2d::enumKeyCodes);
	void loadPage(int);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void setupLevelBrowser(cocos2d::CCArray*);
	void setupPageInfo(int, int, int);
	virtual void keyDown(cocos2d::enumKeyCodes);
}
class AnimatedGameObject : GameObject, AnimatedSpriteDelegate, SpritePartDelegate {
	void activateObject();
	void animationFinished(char const*);
	void animationForID(int, int);
	void create(int);
	void deactivateObject(bool);
	void displayFrameChanged(cocos2d::CCObject*, gd::string);
	void getTweenTime(int, int);
	void init(int);
	void playAnimation(int);
	void resetObject();
	void setObjectColor(cocos2d::_ccColor3B const&);
	void setOpacity(unsigned char);
	void setupAnimatedSize(int);
	void setupChildSprites();
	void updateChildSpriteColor(cocos2d::_ccColor3B);
	virtual void animationFinished(char const*);
	virtual void deactivateObject(bool);
	virtual void displayFrameChanged(cocos2d::CCObject *,gd::string);
	virtual void setObjectColor(cocos2d::_ccColor3B const&);
	virtual void setOpacity(uchar);
}
class AnimatedShopKeeper : CCAnimatedSprite {
	void animationFinished(char const*);
	void create(ShopType);
	void init(ShopType);
	void startAnimating();
	virtual void animationFinished(char const*);
}
class AnimatedSpriteDelegate {
	void animationFinished(char const*);
	virtual void animationFinished(char const*);
}
class AppDelegate : cocos2d::CCSceneDelegate {
	void applicationDidEnterBackground();
	void applicationDidFinishLaunching();
	void applicationWillBecomeActive();
	void applicationWillEnterForeground();
	void applicationWillResignActive();
	void bgScale();
	void checkSound();
	void get();
	void hideLoadingCircle();
	void loadingIsFinished();
	void musicTest();
	void pauseGame();
	void platformShutdown();
	void resumeSound();
	void setIdleTimerDisabled(bool);
	void setupGLView();
	void showLoadingCircle(bool, bool, bool);
	void trySaveGame();
	void willSwitchToScene(cocos2d::CCScene*);
	virtual void willSwitchToScene(cocos2d::CCScene *);
}
class ArtistCell : TableViewCell {
	void draw();
	void init();
	void loadFromObject(SongInfoObject*);
	void onNewgrounds(cocos2d::CCObject*);
	void onYouTube(cocos2d::CCObject*);
	void updateBGColor(int);
}
class AudioEffectsLayer : cocos2d::CCLayerColor {
	void audioStep(float);
	void create(gd::string);
	void draw();
	void getBGSquare();
	void goingDown();
	void init(gd::string);
	void resetAudioVars();
	void triggerEffect(float);
	void updateTweenAction(float, char const*);
	virtual void updateTweenAction(float,char const*);
}
class BitmapFontCache : cocos2d::CCObject {
	void fontWithConfigFile(char const*, float);
	void init();
	void purgeSharedFontCache();
	void sharedFontCache();
}
class BonusDropdown : cocos2d::CCNode {
	void create(gd::string, int);
	void init(gd::string, int);
	void show();
}
class BoomListLayer : cocos2d::CCLayerColor {
	void create(BoomListView*, char const*);
	void init(BoomListView*, char const*);
}
class BoomListView : cocos2d::CCLayer, TableViewDelegate, TableViewDataSource {
	void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&);
	void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*);
	void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*);
	void cellForRowAtIndexPath(CCIndexPath&, TableView*);
	void cellHeightForRowAtIndexPath(CCIndexPath&, TableView*);
	void create(cocos2d::CCArray*, float, float, int, BoomListType);
	void didSelectRowAtIndexPath(CCIndexPath&, TableView*);
	void draw();
	void getListCell(char const*);
	void init(cocos2d::CCArray*, float, float, int, BoomListType);
	void loadCell(TableViewCell*, int);
	void numberOfRowsInSection(unsigned int, TableView*);
	void numberOfSectionsInTableView(TableView*);
	void setupList();
	virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView *,TableViewCellEditingStyle,CCIndexPath &);
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath &,TableViewCell *,TableView *);
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath &,TableViewCell *,TableView *);
	virtual void cellForRowAtIndexPath(CCIndexPath &,TableView *);
	virtual void cellHeightForRowAtIndexPath(CCIndexPath &,TableView *);
	virtual void didSelectRowAtIndexPath(CCIndexPath &,TableView *);
	virtual void getListCell(char const*);
	virtual void loadCell(TableViewCell *,int);
	virtual void numberOfRowsInSection(uint,TableView *);
	virtual void numberOfSectionsInTableView(TableView *);
}
class BoomScrollLayer : cocos2d::CCLayer {
	void addPage(cocos2d::CCLayer*);
	void addPage(cocos2d::CCLayer*, int);
	void cancelAndStoleTouch(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void claimTouch(cocos2d::CCTouch*);
	void create(cocos2d::CCArray*, int, bool);
	void create(cocos2d::CCArray*, int, bool, cocos2d::CCArray*, DynamicScrollDelegate*);
	void getPage(int);
	void getRelativePageForNum(int);
	void getRelativePosForPage(int);
	void getTotalPages();
	void init(cocos2d::CCArray*, int, bool, cocos2d::CCArray*, DynamicScrollDelegate*);
	void instantMoveToPage(int);
	void moveToPage(int);
	void moveToPageEnded();
	void pageNumberForPosition(cocos2d::CCPoint);
	void positionForPageWithNumber(int);
	void postVisit();
	void preVisitWithClippingRect(cocos2d::CCRect);
	void quickUpdate();
	void registerWithTouchDispatcher();
	void removePage(cocos2d::CCLayer*);
	void removePageWithNumber(int);
	void repositionPagesLooped();
	void selectPage(int);
	void setDotScale(float);
	void setPagesIndicatorPosition(cocos2d::CCPoint);
	void setupDynamicScrolling(cocos2d::CCArray*, DynamicScrollDelegate*);
	void togglePageIndicators(bool);
	void updateDots(float);
	void updatePages();
	void visit();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
}
class BoomScrollLayerDelegate {
	void scrollLayerMoved(cocos2d::CCPoint);
	void scrollLayerScrolledToPage(BoomScrollLayer*, int);
	void scrollLayerScrollingStarted(BoomScrollLayer*);
	void scrollLayerWillScrollToPage(BoomScrollLayer*, int);
	virtual void scrollLayerMoved(cocos2d::CCPoint);
	virtual void scrollLayerScrolledToPage(BoomScrollLayer *,int);
	virtual void scrollLayerScrollingStarted(BoomScrollLayer *);
	virtual void scrollLayerWillScrollToPage(BoomScrollLayer *,int);
}
class ButtonPage : cocos2d::CCLayer {
	void create(cocos2d::CCArray*, cocos2d::CCPoint, int, float);
	void init(cocos2d::CCArray*, cocos2d::CCPoint, int, float);
}
class ButtonSprite : cocos2d::CCSprite {
	void create(char const*);
	void create(char const*, float);
	void create(char const*, int, int, float, bool);
	void create(char const*, int, int, float, bool, char const*, char const*);
	void create(char const*, int, int, float, bool, char const*, char const*, float);
	void create(cocos2d::CCSprite*);
	void create(cocos2d::CCSprite*, int, int, float, float, bool);
	void create(cocos2d::CCSprite*, int, int, float, float, bool, char const*, bool);
	void init(char const*, int, int, float, bool, char const*, char const*, float);
	void init(cocos2d::CCSprite*, int, int, float, float, bool, char const*, bool);
	void setColor(cocos2d::_ccColor3B);
	void setString(char const*);
	void updateBGImage(char const*);
	void updateSpriteBGSize();
	void updateSpriteOffset(cocos2d::CCPoint);
}
class CCAlertCircle : cocos2d::CCNode {
	void create();
	void draw();
	void init();
}
class CCAnimatedSprite : cocos2d::CCSprite {
	void animationFinished(char const*);
	void animationFinishedO(cocos2d::CCObject*);
	void cleanupSprite();
	void createWithType(char const*);
	void initWithType(char const*);
	void loadType(char const*);
	void runAnimation(gd::string);
	void runAnimationForced(gd::string);
	void setColor(cocos2d::_ccColor3B);
	void setOpacity(unsigned char);
	void stopTween();
	void switchToMode(spriteMode);
	void tweenToAnimation(gd::string, float);
	void tweenToAnimationFinished();
	void willPlayAnimation();
	virtual void animationFinished(char const*);
	virtual void animationFinishedO(cocos2d::CCObject *);
	virtual void setOpacity(uchar);
}
class CCAnimateFrameCache : cocos2d::CCObject {
	void addCustomSpriteFramesWithFile(char const*);
	void addDict(DS_Dictionary*, char const*);
	void addDict(cocos2d::CCDictionary*, char const*);
	void addSpriteFramesWithFile(char const*);
	void init();
	void purgeSharedSpriteFrameCache();
	void removeSpriteFrames();
	void sharedSpriteFrameCache();
	void spriteFrameByName(char const*);
}
class CCBlockLayer : cocos2d::CCLayerColor {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void customSetup();
	void disableUI();
	void draw();
	void enableUI();
	void enterAnimFinished();
	void enterLayer();
	void exitLayer();
	void hideLayer(bool);
	void init();
	void keyBackClicked();
	void layerHidden();
	void layerVisible();
	void registerWithTouchDispatcher();
	void showLayer(bool);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void hideLayer(bool);
	virtual void showLayer(bool);
}
class CCCircleAlert : CCCircleWave {
	void create(float, float, float);
	void init(float, float, float);
}
class CCCircleWave : cocos2d::CCNode {
	void baseSetup(float);
	void create(float, float, float, bool);
	void create(float, float, float, bool, bool);
	void draw();
	void followObject(cocos2d::CCNode*, bool);
	void init(float, float, float, bool, bool);
	void removeMeAndCleanup();
	void setPosition(cocos2d::CCPoint const&);
	void updatePosition(float);
	void updateTweenAction(float, char const*);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void updateTweenAction(float,char const*);
}
class CCCircleWaveDelegate {
	void circleWaveWillBeRemoved(CCCircleWave*);
	virtual void circleWaveWillBeRemoved(CCCircleWave *);
}
class CCContentLayer : cocos2d::CCLayerColor {
	void create(cocos2d::_ccColor4B const&, float, float);
	void setPosition(cocos2d::CCPoint const&);
	virtual void setPosition(cocos2d::CCPoint const&);
}
class CCContentManager : cocos2d::CCObject {
	void addDict(char const*, bool);
	void addDictDS(char const*);
	void clearCache();
	void init();
	void sharedManager();
}
class CCCountdown : cocos2d::CCSprite {
	void create();
	void init();
	void lapFinished();
	void setOpacity(unsigned char);
	void startTimerWithSeconds(float, void (cocos2d::CCObject::*)(), cocos2d::CCNode*);
	virtual void setOpacity(uchar);
}
class CCCounterLabel : cocos2d::CCLabelBMFont {
	void calculateStepSize();
	void create(int, char const*, FormatterType);
	void disableCounter();
	void enableCounter();
	void fastUpdateCounter();
	void getTargetCount();
	void init(int, char const*, FormatterType);
	void setTargetCount(int);
	void setupFormatter(FormatterType);
	void updateCounter(float);
	void updateString();
}
class CCIndexPath : cocos2d::CCObject {
	void CCIndexPathWithSectionRow(int, int);
	void operator=(CCIndexPath const&);
}
class CCLightFlash : cocos2d::CCNode {
	void cleanupFlash();
	void create();
	void fadeAndRemove();
	void init();
	void playEffect(cocos2d::CCPoint, cocos2d::_ccColor3B, float, float, float, float, float, float, float, float, float, float, float, float, float, float, int, bool, bool, float);
	void removeLights();
	void showFlash();
}
class CCLightStrip : cocos2d::CCNode {
	void create(float, float, float, float, float);
	void draw();
	void init(float, float, float, float, float);
	void updateTweenAction(float, char const*);
	virtual void updateTweenAction(float,char const*);
}
class CCMenuItemSpriteExtra : cocos2d::CCMenuItemSprite {
	void activate();
	void create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void init(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void selected();
	void setSizeMult(float);
	void unselected();
	void useAnimationType(MenuAnimationType);
}
class CCMenuItemToggler : cocos2d::CCMenuItem {
	void activate();
	void activeItem();
	void create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void init(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void normalTouch(cocos2d::CCObject*);
	void selected();
	void selectedTouch(cocos2d::CCObject*);
	void setEnabled(bool);
	void setSizeMult(float);
	void toggle(bool);
	void unselected();
	virtual void setEnabled(bool);
}
class CCMoveCNode : cocos2d::CCNode {
	void create();
	void init();
}
class CCNodeContainer : cocos2d::CCNode {
	void create();
	void init();
	void visit();
}
class CCPartAnimSprite : cocos2d::CCSprite {
	void changeTextureOfID(char const*, char const*);
	void countParts();
	void createWithAnimDesc(char const*, cocos2d::CCTexture2D*);
	void dirtify();
	void displayFrame();
	void getSpriteForKey(char const*);
	void initWithAnimDesc(char const*, cocos2d::CCTexture2D*);
	void isFrameDisplayed(cocos2d::CCSpriteFrame*);
	void setBlendFunc(cocos2d::_ccBlendFunc);
	void setColor(cocos2d::_ccColor3B);
	void setDisplayFrame(cocos2d::CCSpriteFrame*);
	void setFlipX(bool);
	void setFlipY(bool);
	void setOpacity(unsigned char);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void transformSprite(frameValues);
	void tweenSpriteTo(frameValues, float);
	void tweenToFrame(cocos2d::CCSpriteFrame*, float);
	virtual void isFrameDisplayed(cocos2d::CCSpriteFrame *);
	virtual void setBlendFunc(cocos2d::_ccBlendFunc);
	virtual void setDisplayFrame(cocos2d::CCSpriteFrame *);
	virtual void setOpacity(uchar);
	virtual void setScale(float);
	virtual void setScaleX(float);
	virtual void setScaleY(float);
}
class CCScrollLayerExt : cocos2d::CCLayer {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void constraintContent();
	void getMaxY();
	void getMinY();
	void moveToTop();
	void moveToTopWithOffset(float);
	void postVisit();
	void preVisitWithClippingRect(cocos2d::CCRect);
	void registerWithTouchDispatcher();
	void scrollLayer(float);
	void scrollingEnd();
	void setContentLayerSize(cocos2d::CCSize);
	void setContentOffset(cocos2d::CCPoint, bool);
	void updateIndicators(float);
	void visit();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void preVisitWithClippingRect(cocos2d::CCRect);
}
class CCScrollLayerExtDelegate {
	void scrllViewWillBeginDecelerating(CCScrollLayerExt*);
	void scrollViewDidEndDecelerating(CCScrollLayerExt*);
	void scrollViewDidEndMoving(CCScrollLayerExt*);
	void scrollViewTouchBegin(CCScrollLayerExt*);
	void scrollViewTouchEnd(CCScrollLayerExt*);
	void scrollViewTouchMoving(CCScrollLayerExt*);
	virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt *);
	virtual void scrollViewDidEndDecelerating(CCScrollLayerExt *);
	virtual void scrollViewDidEndMoving(CCScrollLayerExt *);
	virtual void scrollViewTouchBegin(CCScrollLayerExt *);
	virtual void scrollViewTouchEnd(CCScrollLayerExt *);
	virtual void scrollViewTouchMoving(CCScrollLayerExt *);
}
class CCSpritePart : CCSpritePlus {
	void create(cocos2d::CCTexture2D*);
	void createWithSpriteFrameName(char const*);
	void frameChanged(gd::string);
	void getBeingUsed();
	void hideInactive();
	void resetTextureRect();
	void setBeingUsed(bool);
	void updateDisplayFrame(char const*);
}
class CCSpritePlus : cocos2d::CCSprite {
	void addFollower(cocos2d::CCNode*);
	void createWithSpriteFrame(cocos2d::CCSpriteFrame*);
	void createWithSpriteFrameName(char const*);
	void followSprite(CCSpritePlus*);
	void getFollower();
	void initWithSpriteFrameName(char const*);
	void initWithTexture(cocos2d::CCTexture2D*);
	void removeFollower(cocos2d::CCNode*);
	void setFlipX(bool);
	void setFlipY(bool);
	void setPosition(cocos2d::CCPoint const&);
	void setRotation(float);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void stopFollow();
	virtual void initWithSpriteFrameName(char const*);
	virtual void initWithTexture(cocos2d::CCTexture2D *);
	virtual void setFlipX(bool);
	virtual void setFlipY(bool);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void setRotation(float);
	virtual void setScale(float);
	virtual void setScaleX(float);
	virtual void setScaleY(float);
}
class CCSpriteWithHue : cocos2d::CCSprite {
	void create(gd::string const&);
	void create(gd::string const&, cocos2d::CCRect const&);
	void createWithSpriteFrame(cocos2d::CCSpriteFrame*);
	void createWithSpriteFrameName(gd::string const&);
	void createWithTexture(cocos2d::CCTexture2D*);
	void createWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool);
	void draw();
	void getAlpha();
	void getHue();
	void getUniformLocations();
	void initShader();
	void initWithSpriteFrame(cocos2d::CCSpriteFrame*);
	void initWithTexture(cocos2d::CCTexture2D*);
	void initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	void initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool);
	void setHue(float);
	void setHueDegrees(float);
	void setupDefaultSettings();
	void shaderBody();
	void updateColor();
	void updateColorMatrix();
	void updateHue(float);
	virtual void initWithSpriteFrame(cocos2d::CCSpriteFrame *);
	virtual void initWithTexture(cocos2d::CCTexture2D *);
	virtual void initWithTexture(cocos2d::CCTexture2D *,cocos2d::CCRect const&);
	virtual void initWithTexture(cocos2d::CCTexture2D *,cocos2d::CCRect const&,bool);
}
class CCTextInputNode : cocos2d::CCLayer, cocos2d::CCIMEDelegate, cocos2d::CCTextFieldDelegate {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(float, float, char const*, char const*, int, char const*);
	void forceOffset();
	void getString();
	void init(float, float, char const*, char const*, int, char const*);
	void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	void onClickTrackNode(bool);
	void onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF*);
	void onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF*);
	void onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int);
	void refreshLabel();
	void registerWithTouchDispatcher();
	void setAllowedChars(gd::string);
	void setLabelNormalColor(cocos2d::_ccColor3B);
	void setLabelPlaceholderColor(cocos2d::_ccColor3B);
	void setLabelPlaceholderScale(float);
	void setMaxLabelScale(float);
	void setMaxLabelWidth(float);
	void setString(gd::string);
	void textChanged();
	void updateBlinkLabel();
	void updateLabel(gd::string);
	void visit();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo &);
	virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo &);
	virtual void onClickTrackNode(bool);
	virtual void onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *);
	virtual void onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *);
	virtual void onTextFieldInsertText(cocos2d::CCTextFieldTTF *,char const*,int);
}
class ChallengeNode : cocos2d::CCNode {
	void create(GJChallengeItem*, ChallengesPage*, bool);
	void init(GJChallengeItem*, ChallengesPage*, bool);
	void onClaimReward(cocos2d::CCObject*);
	void updateTimeLabel(gd::string);
}
class ChallengesPage : FLAlertLayer, FLAlertLayerProtocol, GJChallengeDelegate, CurrencyRewardDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void challengeStatusFailed();
	void challengeStatusFinished();
	void claimItem(ChallengeNode*, GJChallengeItem*, cocos2d::CCPoint);
	void create();
	void createChallengeNode(int, bool, float, bool);
	void currencyWillExit(CurrencyRewardLayer*);
	void exitNodeAtSlot(int, float);
	void init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void show();
	void tryGetChallenges();
	void updateDots();
	void updateTimers(float);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void currencyWillExit(CurrencyRewardLayer *);
}
class CheckpointObject : cocos2d::CCNode {
	void create();
	void getColorAction(ColorAction*, cocos2d::_ccColor3B);
	void getObject();
	void init();
	void setObject(GameObject*);
}
class CollisionBlockPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onDynamicBlock(cocos2d::CCObject*);
	void onItemIDArrow(cocos2d::CCObject*);
	void onNextItemID(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateEditorLabel();
	void updateItemID();
	void updateTextInputLabel();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class CollisionTriggerAction : cocos2d::CCNode {
	void create(int, int, int, bool, bool);
	void createFromString(gd::string);
	void getSaveString();
	void init(int, int, int, bool, bool);
}
class ColorAction : cocos2d::CCNode {
	void create(cocos2d::_ccColor3B, bool, int);
	void create(cocos2d::_ccColor3B, cocos2d::_ccColor3B, float, double, bool);
	void create(cocos2d::_ccColor3B, cocos2d::_ccColor3B, float, double, bool, int, float, float);
	void create();
	void getCopy();
	void getSaveString();
	void init(cocos2d::_ccColor3B, cocos2d::_ccColor3B, float, double, bool, int, float, float);
	void resetAction();
	void setupFromDict(cocos2d::CCDictionary*);
	void setupFromString(gd::string);
	void step(float);
	void updateCustomColor(cocos2d::_ccColor3B, cocos2d::_ccColor3B);
}
class ColorActionSprite : cocos2d::CCNode {
	void create();
	void init();
}
class ColorChannelSprite : cocos2d::CCSprite {
	void create();
	void init();
	void updateBlending(bool);
	void updateCopyLabel(int, bool);
	void updateOpacity(float);
	void updateValues(ColorAction*);
}
class ColoredSection : cocos2d::CCObject {
	void create(cocos2d::_ccColor3B, int, int);
	void init(cocos2d::_ccColor3B, int, int);
}
class ColorSelectDelegate {
	void colorSelectClosed(cocos2d::CCNode*);
	virtual void colorSelectClosed(cocos2d::CCNode *);
}
class ColorSelectLiveOverlay : FLAlertLayer {
	void closeColorSelect(cocos2d::CCObject*);
	void colorValueChanged(cocos2d::_ccColor3B);
	void create(ColorAction*, ColorAction*, EffectGameObject*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void createWithActions(ColorAction*, ColorAction*);
	void createWithObject(EffectGameObject*);
	void determineStartValues();
	void getColorValue();
	void init(ColorAction*, ColorAction*, EffectGameObject*);
	void keyBackClicked();
	void onSelectTab(cocos2d::CCObject*);
	void selectColor(cocos2d::_ccColor3B);
	void show();
	void sliderChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void toggleControls(bool);
	void updateColorLabel();
	void updateColorValue();
	void updateOpacity();
}
class ColorSelectPopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
	void closeColorSelect(cocos2d::CCObject*);
	void colorSelectClosed(GJSpecialColorSelect*, int);
	void colorValueChanged(cocos2d::_ccColor3B);
	void create(ColorAction*);
	void create(EffectGameObject*, cocos2d::CCArray*);
	void create(EffectGameObject*, cocos2d::CCArray*, ColorAction*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void getColorValue();
	void init(EffectGameObject*, cocos2d::CCArray*, ColorAction*);
	void keyBackClicked();
	void onCopy(cocos2d::CCObject*);
	void onCopyOpacity(cocos2d::CCObject*);
	void onDefault(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onPaste(cocos2d::CCObject*);
	void onPlayerColor1(cocos2d::CCObject*);
	void onPlayerColor2(cocos2d::CCObject*);
	void onSelectSpecialColor(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTintGround(cocos2d::CCObject*);
	void onToggleHSVMode(cocos2d::CCObject*);
	void onToggleTintMode(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void onUpdateCopyColor(cocos2d::CCObject*);
	void onUpdateCustomColor(cocos2d::CCObject*);
	void selectColor(cocos2d::_ccColor3B);
	void show();
	void sliderChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateColorValue();
	void updateCopyColor();
	void updateCopyColorTextInputLabel();
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
	virtual void colorSelectClosed(GJSpecialColorSelect *,int);
	virtual void colorValueChanged(cocos2d::_ccColor3B);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class ColorSetupDelegate {
	void colorSetupClosed(int);
	virtual void colorSetupClosed(int);
}
class CommentCell : TableViewCell, LikeItemDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void draw();
	void incrementDislikes();
	void incrementLikes();
	void init();
	void likedItem(LikeItemType, int, bool);
	void loadFromComment(GJComment*);
	void onConfirmDelete(cocos2d::CCObject*);
	void onDelete();
	void onGoToLevel(cocos2d::CCObject*);
	void onLike(cocos2d::CCObject*);
	void onUndelete();
	void onUnhide(cocos2d::CCObject*);
	void onViewProfile(cocos2d::CCObject*);
	void updateBGColor(int);
	void updateLabelValues();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void likedItem(LikeItemType,int,bool);
}
class CommentUploadDelegate {
	void commentDeleteFailed(int, int);
	void commentUploadFailed(int, CommentError);
	void commentUploadFinished(int);
	virtual void commentDeleteFailed(int,int);
	virtual void commentUploadFailed(int,CommentError);
	virtual void commentUploadFinished(int);
}
class CommunityCreditNode : cocos2d::CCNode {
	void create(int, int, int, gd::string);
	void init(int, int, int, gd::string);
}
class CommunityCreditsPage : FLAlertLayer {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void goToPage(int);
	void init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSwitchPage(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void show();
}
class ConfigureHSVWidget : cocos2d::CCNode {
	void create(cocos2d::_ccHSVValue, bool);
	void init(cocos2d::_ccHSVValue, bool);
	void onResetHSV(cocos2d::CCObject*);
	void onToggleSConst(cocos2d::CCObject*);
	void onToggleVConst(cocos2d::CCObject*);
	void sliderChanged(cocos2d::CCObject*);
	void updateLabels();
}
class ConfigureValuePopup : FLAlertLayer, TextInputDelegate {
	void create(ConfigureValuePopupDelegate*, float, float, float, gd::string, gd::string);
	void init(ConfigureValuePopupDelegate*, float, float, float, gd::string, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void sliderChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void updateTextInputLabel();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
}
class ConfigureValuePopupDelegate {
	void valuePopupClosed(ConfigureValuePopup*, float);
	virtual void valuePopupClosed(ConfigureValuePopup *,float);
}
class CountTriggerAction : cocos2d::CCNode {
	void create(int, int, bool, bool);
	void createFromString(gd::string);
	void getSaveString();
	void init(int, int, bool, bool);
}
class CreateGuidelinesLayer : FLAlertLayer, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(LevelSettingsObject*);
	void doClearGuidelines();
	void getMergedRecordString(gd::string, gd::string);
	void init(LevelSettingsObject*);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void keyUp(cocos2d::enumKeyCodes);
	void onClearGuidelines(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onRecord(cocos2d::CCObject*);
	void onStop(cocos2d::CCObject*);
	void registerTouch();
	void registerWithTouchDispatcher();
	void toggleItems(bool);
	void update(float);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void keyUp(cocos2d::enumKeyCodes);
	virtual void update(float);
}
class CreateMenuItem : CCMenuItemSpriteExtra {
	void create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void init(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
}
class CreatorLayer : cocos2d::CCLayer, cocos2d::CCSceneTransitionDelegate, DialogDelegate {
	void canPlayOnlineLevels();
	void create();
	void dialogClosed(DialogLayer*);
	void init();
	void keyBackClicked();
	void onBack(cocos2d::CCObject*);
	void onChallenge(cocos2d::CCObject*);
	void onDailyLevel(cocos2d::CCObject*);
	void onFameLevels(cocos2d::CCObject*);
	void onFeaturedLevels(cocos2d::CCObject*);
	void onGauntlets(cocos2d::CCObject*);
	void onLeaderboards(cocos2d::CCObject*);
	void onMapPacks(cocos2d::CCObject*);
	void onMyLevels(cocos2d::CCObject*);
	void onOnlineLevels(cocos2d::CCObject*);
	void onOnlyFullVersion(cocos2d::CCObject*);
	void onSavedLevels(cocos2d::CCObject*);
	void onSecretVault(cocos2d::CCObject*);
	void onTreasureRoom(cocos2d::CCObject*);
	void onWeeklyLevel(cocos2d::CCObject*);
	void scene();
	void sceneWillResume();
	virtual void dialogClosed(DialogLayer *);
}
class CurrencyRewardDelegate {
	void currencyWillExit(CurrencyRewardLayer*);
	virtual void currencyWillExit(CurrencyRewardLayer *);
}
class CurrencyRewardLayer : cocos2d::CCLayer {
	void create(int, int, int, CurrencySpriteType, int, CurrencySpriteType, int, cocos2d::CCPoint, CurrencyRewardType, float);
	void createObjects(CurrencySpriteType, int, cocos2d::CCPoint, float);
	void createObjectsFull(CurrencySpriteType, int, cocos2d::CCSprite*, cocos2d::CCPoint, float);
	void createUnlockObject(cocos2d::CCSprite*, cocos2d::CCPoint, float);
	void incrementCount(int);
	void incrementDiamondsCount(int);
	void incrementSpecialCount1(int);
	void incrementSpecialCount2(int);
	void incrementStarsCount(int);
	void init(int, int, int, CurrencySpriteType, int, CurrencySpriteType, int, cocos2d::CCPoint, CurrencyRewardType, float);
	void pulseSprite(cocos2d::CCSprite*);
	void update(float);
	virtual void update(float);
}
class CurrencySprite : CCSpritePlus {
	void create(CurrencySpriteType, bool);
	void createWithSprite(cocos2d::CCSprite*);
	void init(CurrencySpriteType, bool);
	void initWithSprite(cocos2d::CCSprite*);
	void rewardToSpriteType(int);
	void spriteTypeToStat(CurrencySpriteType);
}
class CustomizeObjectLayer : FLAlertLayer, TextInputDelegate, HSVWidgetPopupDelegate, ColorSelectDelegate, ColorSetupDelegate {
	void colorSelectClosed(cocos2d::CCNode*);
	void colorSetupClosed(int);
	void create(GameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void determineStartValues();
	void getActiveMode(bool);
	void getButtonByTag(int);
	void getHSV();
	void highlightSelected(ButtonSprite*);
	void hsvPopupClosed(HSVWidgetPopup*, cocos2d::_ccHSVValue);
	void init(GameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onBreakApart(cocos2d::CCObject*);
	void onBrowse(cocos2d::CCObject*);
	void onClear(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onCopy(cocos2d::CCObject*);
	void onEditColor(cocos2d::CCObject*);
	void onHSV(cocos2d::CCObject*);
	void onNextColorChannel(cocos2d::CCObject*);
	void onPaste(cocos2d::CCObject*);
	void onSelectColor(cocos2d::CCObject*);
	void onSelectMode(cocos2d::CCObject*);
	void onToggleGlow(cocos2d::CCObject*);
	void onUpdateCustomColor(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void toggleVisible();
	void updateChannelLabel(int);
	void updateColorSprite();
	void updateCurrentSelection();
	void updateCustomColorLabels();
	void updateHSVButtons();
	void updateSelected(int);
	virtual void colorSelectClosed(cocos2d::CCNode *);
	virtual void colorSetupClosed(int);
	virtual void hsvPopupClosed(HSVWidgetPopup *,cocos2d::_ccHSVValue);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
}
class CustomListView : BoomListView {
	void create(cocos2d::CCArray*, float, float, int, BoomListType);
	void getListCell(char const*);
	void loadCell(TableViewCell*, int);
	void reloadAll();
	void setupList();
	virtual void getListCell(char const*);
	virtual void loadCell(TableViewCell *,int);
}
class CustomSongCell : TableViewCell {
	void draw();
	void init();
	void loadFromObject(SongInfoObject*);
	void onDelete(cocos2d::CCObject*);
	void updateBGColor(int);
}
class CustomSongLayer : FLAlertLayer, FLAlertLayerProtocol, TextInputDelegate, GJDropDownLayerDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(LevelSettingsObject*);
	void dropDownLayerWillClose(GJDropDownLayer*);
	void init(LevelSettingsObject*);
	void keyBackClicked();
	void onArtists(cocos2d::CCObject*);
	void onChanges(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onCreateLines(cocos2d::CCObject*);
	void onGuidelines(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onNewgrounds(cocos2d::CCObject*);
	void onOptions(cocos2d::CCObject*);
	void onSearch(cocos2d::CCObject*);
	void onSongBrowser(cocos2d::CCObject*);
	void onSupporter(cocos2d::CCObject*);
	void show();
	void showNewgroundsMessage();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void dropDownLayerWillClose(GJDropDownLayer *);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class CustomSongLayerDelegate {
	void customSongLayerClosed();
}
class CustomSongWidget : cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(SongInfoObject*, LevelSettingsObject*, bool, bool, bool, bool, bool);
	void downloadFailed();
	void downloadSongFailed(int, GJSongError);
	void downloadSongFinished(SongInfoObject*);
	void getSongInfoIfUnloaded();
	void init(SongInfoObject*, LevelSettingsObject*, bool, bool, bool, bool, bool);
	void loadSongInfoFailed(int, GJSongError);
	void loadSongInfoFinished(SongInfoObject*);
	void onCancelDownload(cocos2d::CCObject*);
	void onDownload(cocos2d::CCObject*);
	void onGetSongInfo(cocos2d::CCObject*);
	void onMore(cocos2d::CCObject*);
	void onPlayback(cocos2d::CCObject*);
	void onSelect(cocos2d::CCObject*);
	void setSongObject(SongInfoObject*);
	void songStateChanged();
	void startDownload();
	void startMonitorDownload();
	void toggleUpdateButton(bool);
	void updateDownloadProgress(float);
	void updateError(GJSongError);
	void updatePlaybackBtn();
	void updateProgressBar(int);
	void updateSongInfo();
	void updateSongObject(SongInfoObject*);
	void verifySongID(int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void downloadSongFailed(int,GJSongError);
	virtual void downloadSongFinished(SongInfoObject *);
	virtual void loadSongInfoFailed(int,GJSongError);
	virtual void loadSongInfoFinished(SongInfoObject *);
}
class DailyLevelNode : cocos2d::CCNode, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(GJGameLevel*, DailyLevelPage*, bool);
	void init(GJGameLevel*, DailyLevelPage*, bool);
	void onClaimReward(cocos2d::CCObject*);
	void onSkipLevel(cocos2d::CCObject*);
	void showSkipButton();
	void updateTimeLabel(gd::string);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
}
class DailyLevelPage : FLAlertLayer, FLAlertLayerProtocol, GJDailyLevelDelegate, LevelDownloadDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void claimLevelReward(DailyLevelNode*, GJGameLevel*, cocos2d::CCPoint);
	void create(bool);
	void createDailyNode(GJGameLevel*, bool, float, bool);
	void createNodeIfLoaded();
	void dailyStatusFailed(bool);
	void dailyStatusFinished(bool);
	void downloadAndCreateNode();
	void exitDailyNode(DailyLevelNode*, float);
	void getDailyTime();
	void getDailyTimeString(int);
	void init(bool);
	void keyBackClicked();
	void levelDownloadFailed(int);
	void levelDownloadFinished(GJGameLevel*);
	void onClose(cocos2d::CCObject*);
	void refreshDailyPage();
	void registerWithTouchDispatcher();
	void show();
	void skipDailyLevel(DailyLevelNode*, GJGameLevel*);
	void tryGetDailyStatus();
	void updateTimers(float);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void dailyStatusFailed(bool);
	virtual void dailyStatusFinished(bool);
	virtual void levelDownloadFailed(int);
	virtual void levelDownloadFinished(GJGameLevel *);
}
class DelayedSpawnNode : cocos2d::CCObject {
	void create();
	void init();
}
class DelaySection : cocos2d::CCObject {
	void create(int, float);
	void init(int, float);
}
class DemonFilterDelegate {
	void demonFilterSelectClosed(int);
	virtual void demonFilterSelectClosed(int);
}
class DemonFilterSelectLayer : FLAlertLayer {
	void create();
	void init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void selectRating(cocos2d::CCObject*);
}
class DialogDelegate {
	void dialogClosed(DialogLayer*);
	virtual void dialogClosed(DialogLayer *);
}
class DialogLayer : cocos2d::CCLayerColor, TextAreaDelegate {
	void animateIn(DialogAnimationType);
	void animateInDialog();
	void animateInRandomSide();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(DialogObject*, int);
	void createDialogLayer(DialogObject*, cocos2d::CCArray*, int);
	void createWithObjects(cocos2d::CCArray*, int);
	void displayDialogObject(DialogObject*);
	void displayNextObject();
	void fadeInTextFinished(TextArea*);
	void finishCurrentAnimation();
	void handleDialogTap();
	void init(DialogObject*, cocos2d::CCArray*, int);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void onClose();
	void onEnter();
	void registerWithTouchDispatcher();
	void updateChatPlacement(DialogChatPlacement);
	void updateNavButtonFrame();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void fadeInTextFinished(TextArea *);
	virtual void keyDown(cocos2d::enumKeyCodes);
}
class DialogObject : cocos2d::CCObject {
	void create(gd::string, gd::string, int, float, bool, cocos2d::_ccColor3B);
	void init(gd::string, gd::string, int, float, bool, cocos2d::_ccColor3B);
}
class DownloadMessageDelegate {
	void downloadMessageFailed(int);
	void downloadMessageFinished(GJUserMessage*);
	virtual void downloadMessageFailed(int);
	virtual void downloadMessageFinished(GJUserMessage *);
}
class DrawGridLayer : cocos2d::CCLayer {
	void addPlayer2Point(cocos2d::CCPoint, bool);
	void addPlayerPoint(cocos2d::CCPoint);
	void addToEffects(EffectGameObject*);
	void addToGuides(GameObject*);
	void addToSpeedObjects(GameObject*);
	void clearPlayerPoints();
	void create(cocos2d::CCNode*, LevelEditorLayer*);
	void draw();
	void getPortalMinMax(GameObject*);
	void init(cocos2d::CCNode*, LevelEditorLayer*);
	void loadTimeMarkers(gd::string);
	void removeFromEffects(EffectGameObject*);
	void removeFromGuides(GameObject*);
	void removeFromSpeedObjects(GameObject*);
	void sortSpeedObjects();
	void timeForXPos(float);
	void update(float);
	void updateMusicGuideTime(float);
	void updateTimeMarkers();
	void xPosForTime(float);
	virtual void update(float);
}
class DungeonBarsSprite : cocos2d::CCNode {
	void animateOutBars();
	void create();
	void init();
	void visit();
}
class DynamicScrollDelegate {
}
class EditButtonBar : cocos2d::CCNode {
	void create(cocos2d::CCArray*, cocos2d::CCPoint, int, bool, int, int);
	void getPage();
	void goToPage(int);
	void init(cocos2d::CCArray*, cocos2d::CCPoint, int, bool, int, int);
	void loadFromItems(cocos2d::CCArray*, int, int, bool);
	void onLeft(cocos2d::CCObject*);
	void onRight(cocos2d::CCObject*);
	void reloadItems(int, int);
}
class EditGameObjectPopup : FLAlertLayer {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onAllowMultiActivate(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void show();
}
class EditLevelLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, SetIDPopupDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void closeTextInputs();
	void confirmClone(cocos2d::CCObject*);
	void confirmDelete(cocos2d::CCObject*);
	void confirmMoveToTop(cocos2d::CCObject*);
	void create(GJGameLevel*);
	void init(GJGameLevel*);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void onBack(cocos2d::CCObject*);
	void onClone();
	void onClosePopup(UploadActionPopup*);
	void onDelete();
	void onEdit(cocos2d::CCObject*);
	void onHelp(cocos2d::CCObject*);
	void onLevelInfo(cocos2d::CCObject*);
	void onLowDetailMode(cocos2d::CCObject*);
	void onMoveToTop();
	void onPlay(cocos2d::CCObject*);
	void onSetFolder(cocos2d::CCObject*);
	void onShare(cocos2d::CCObject*);
	void onTest(cocos2d::CCObject*);
	void onUpdateDescription(cocos2d::CCObject*);
	void playStep2();
	void playStep3();
	void scene(GJGameLevel*);
	void setIDPopupClosed(SetIDPopup*, int);
	void setupLevelInfo();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void updateDescText(char const*);
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	void verifyLevelName();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void setIDPopupClosed(SetIDPopup *,int);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class EditorOptionsLayer : GJOptionsLayer {
	void create();
	void init();
	void onButtonRows(cocos2d::CCObject*);
	void onButtonsPerRow(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void setupOptions();
	virtual void onClose(cocos2d::CCObject *);
}
class EditorPauseLayer : CCBlockLayer, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(LevelEditorLayer*);
	void customSetup();
	void doResetUnused();
	void init(LevelEditorLayer*);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void onAlignX(cocos2d::CCObject*);
	void onAlignY(cocos2d::CCObject*);
	void onBuildHelper(cocos2d::CCObject*);
	void onCreateBase(cocos2d::CCObject*);
	void onCreateEdges(cocos2d::CCObject*);
	void onCreateOutline(cocos2d::CCObject*);
	void onExitEditor(cocos2d::CCObject*);
	void onExitNoSave(cocos2d::CCObject*);
	void onHelp(cocos2d::CCObject*);
	void onKeybindings(cocos2d::CCObject*);
	void onOptions(cocos2d::CCObject*);
	void onResetUnusedColors(cocos2d::CCObject*);
	void onResume(cocos2d::CCObject*);
	void onSave(cocos2d::CCObject*);
	void onSaveAndExit(cocos2d::CCObject*);
	void onSaveAndPlay(cocos2d::CCObject*);
	void onSaveAndTest(cocos2d::CCObject*);
	void onSelectAll(cocos2d::CCObject*);
	void onSelectAllLeft(cocos2d::CCObject*);
	void onSelectAllRight(cocos2d::CCObject*);
	void onSong(cocos2d::CCObject*);
	void onUnlockAllLayers(cocos2d::CCObject*);
	void playStep2();
	void playStep3();
	void saveLevel();
	void toggleDebugDraw(cocos2d::CCObject*);
	void toggleEditorBackground(cocos2d::CCObject*);
	void toggleEditorColorMode(cocos2d::CCObject*);
	void toggleEditorGrid(cocos2d::CCObject*);
	void toggleEditorGround(cocos2d::CCObject*);
	void toggleEffectDuration(cocos2d::CCObject*);
	void toggleEffectLines(cocos2d::CCObject*);
	void toggleFollowPlayer(cocos2d::CCObject*);
	void toggleGridOnTop(cocos2d::CCObject*);
	void toggleIgnoreDamage(cocos2d::CCObject*);
	void togglePlaytestMusic(cocos2d::CCObject*);
	void toggleSelectFilter(cocos2d::CCObject*);
	void toggleShowObjectInfo(cocos2d::CCObject*);
	void uncheckAllPortals(cocos2d::CCObject*);
	void updateSongButton();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void keyDown(cocos2d::enumKeyCodes);
}
class EditorUI : cocos2d::CCLayer, FLAlertLayerProtocol, ColorSelectDelegate, GJRotationControlDelegate, GJScaleControlDelegate, MusicDownloadDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void activateRotationControl(cocos2d::CCObject*);
	void activateScaleControl(cocos2d::CCObject*);
	void addSnapPosition(cocos2d::CCPoint);
	void alignObjects(cocos2d::CCArray*, bool);
	void angleChangeBegin();
	void angleChangeEnded();
	void angleChanged(float);
	void applyOffset(GameObject*);
	void applySpecialOffset(cocos2d::CCPoint, GameObject*, cocos2d::CCPoint);
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void changeSelectedObjects(cocos2d::CCArray*, bool);
	void clickOnPosition(cocos2d::CCPoint);
	void closeLiveColorSelect();
	void colorSelectClosed(cocos2d::_ccColor3B);
	void colorSelectClosed(cocos2d::CCNode*);
	void constrainGameLayerPosition(float, float);
	void constrainGameLayerPosition();
	void copyObjects(cocos2d::CCArray*);
	void create(LevelEditorLayer*);
	void createCustomItems();
	void createEdgeForObject(GameObject*, int);
	void createGlow();
	void createMoveMenu();
	void createRockBase();
	void createRockEdges();
	void createRockOutline();
	void createUndoSelectObject(bool);
	void deactivateRotationControl();
	void deactivateScaleControl();
	void deleteObject(GameObject*, bool);
	void deselectAll();
	void deselectObject(GameObject*);
	void deselectObject();
	void deselectTargetPortals();
	void disableButton(CreateMenuItem*);
	void draw();
	void dynamicGroupUpdate(bool);
	void edgeForObject(int, int);
	void editButton2Usable();
	void editButtonUsable();
	void editColor(cocos2d::CCObject*);
	void editGroup(cocos2d::CCObject*);
	void editObject(cocos2d::CCObject*);
	void editObject2(cocos2d::CCObject*);
	void editorLayerForArray(cocos2d::CCArray*, bool);
	void enableButton(CreateMenuItem*);
	void findSnapObject(cocos2d::CCPoint, float);
	void findSnapObject(cocos2d::CCArray*, float);
	void flipObjectsX(cocos2d::CCArray*);
	void flipObjectsY(cocos2d::CCArray*);
	void getButton(char const*, int, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*);
	void getCreateBtn(int, int);
	void getCreateMenuItemButton(cocos2d::CCSprite*, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float, int, cocos2d::CCPoint);
	void getCycledObject(cocos2d::CCArray*, bool);
	void getGridSnappedPos(cocos2d::CCPoint);
	void getGroupCenter(cocos2d::CCArray*, bool);
	void getLimitedPosition(cocos2d::CCPoint);
	void getModeBtn(char const*, int);
	void getRelativeOffset(GameObject*);
	void getSelectedObjects();
	void getSimpleButton(gd::string, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*);
	void getSpriteButton(char const*, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float);
	void getSpriteButton(char const*, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float, int, cocos2d::CCPoint);
	void getSpriteButton(cocos2d::CCSprite*, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float, int, cocos2d::CCPoint);
	void getTouchPoint(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void getXMin(int);
	void init(LevelEditorLayer*);
	void isLiveColorSelectTrigger(GameObject*);
	void isSpecialSnapObject(int);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void keyUp(cocos2d::enumKeyCodes);
	void liveEditColorUsable();
	void menuItemFromObjectString(gd::string, int);
	void moveForCommand(EditCommand);
	void moveGamelayer(cocos2d::CCPoint);
	void moveObject(GameObject*, cocos2d::CCPoint);
	void moveObjectCall(EditCommand);
	void moveObjectCall(cocos2d::CCObject*);
	void offsetForKey(int);
	void onCopy(cocos2d::CCObject*);
	void onCopyState(cocos2d::CCObject*);
	void onCreate();
	void onCreateButton(cocos2d::CCObject*);
	void onCreateObject(int);
	void onDelete(cocos2d::CCObject*);
	void onDeleteAll(cocos2d::CCObject*);
	void onDeleteCustomItem(cocos2d::CCObject*);
	void onDeleteInfo(cocos2d::CCObject*);
	void onDeleteSelected(cocos2d::CCObject*);
	void onDeleteSelectedType(cocos2d::CCObject*);
	void onDeleteStartPos(cocos2d::CCObject*);
	void onDeselectAll(cocos2d::CCObject*);
	void onDuplicate(cocos2d::CCObject*);
	void onGoToBaseLayer(cocos2d::CCObject*);
	void onGoToLayer(cocos2d::CCObject*);
	void onGroupDown(cocos2d::CCObject*);
	void onGroupSticky(cocos2d::CCObject*);
	void onGroupUp(cocos2d::CCObject*);
	void onLockLayer(cocos2d::CCObject*);
	void onNewCustomItem(cocos2d::CCObject*);
	void onPaste(cocos2d::CCObject*);
	void onPasteColor(cocos2d::CCObject*);
	void onPasteState(cocos2d::CCObject*);
	void onPause(cocos2d::CCObject*);
	void onPlayback(cocos2d::CCObject*);
	void onPlaytest(cocos2d::CCObject*);
	void onSelectBuildTab(cocos2d::CCObject*);
	void onSettings(cocos2d::CCObject*);
	void onStopPlaytest(cocos2d::CCObject*);
	void onToggleGuide(cocos2d::CCObject*);
	void onUngroupSticky(cocos2d::CCObject*);
	void onUpdateDeleteFilter(cocos2d::CCObject*);
	void orderDownCustomItem(cocos2d::CCObject*);
	void orderUpCustomItem(cocos2d::CCObject*);
	void pasteObjects(gd::string);
	void playerTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void playerTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void playtestStopped();
	void positionIsInSnapped(cocos2d::CCPoint);
	void positionWithoutOffset(GameObject*);
	void processSelectObjects(cocos2d::CCArray*);
	void recreateButtonTabs();
	void redoLastAction(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void reloadCustomItems();
	void removeOffset(GameObject*);
	void replaceGroupID(GameObject*, int, int);
	void repositionObjectsToCenter(cocos2d::CCArray*, cocos2d::CCPoint, bool);
	void resetUI();
	void rotateObjects(cocos2d::CCArray*, float, cocos2d::CCPoint);
	void rotationforCommand(EditCommand);
	void scaleChangeBegin();
	void scaleChangeEnded();
	void scaleChanged(float);
	void scaleObjects(cocos2d::CCArray*, float, cocos2d::CCPoint);
	void scrollWheel(float, float);
	void selectAll();
	void selectAllWithDirection(bool);
	void selectBuildTab(int);
	void selectObject(GameObject*, bool);
	void selectObjects(cocos2d::CCArray*, bool);
	void selectObjectsInRect(cocos2d::CCRect);
	void setStoredUndoObject(UndoObject*);
	void setupCreateMenu();
	void setupDeleteMenu();
	void setupEditMenu();
	void setupTransformControl();
	void shouldDeleteObject(GameObject*);
	void shouldSnap(GameObject*);
	void showDeleteConfirmation();
	void showLiveColorSelectForMode(int);
	void showMaxBasicError();
	void showMaxCoinError();
	void showMaxError();
	void showUI(bool);
	void sliderChanged(cocos2d::CCObject*);
	void songStateChanged();
	void toggleDuplicateButton();
	void toggleEditObjectButton();
	void toggleEnableRotate(cocos2d::CCObject*);
	void toggleFreeMove(cocos2d::CCObject*);
	void toggleGuideButton();
	void toggleMode(cocos2d::CCObject*);
	void toggleObjectInfoLabel();
	void toggleSnap(cocos2d::CCObject*);
	void toggleSpecialEditButtons();
	void toggleStickyControls(bool);
	void toggleSwipe(cocos2d::CCObject*);
	void transformObject(GameObject*, EditCommand, bool);
	void transformObjectCall(EditCommand);
	void transformObjectCall(cocos2d::CCObject*);
	void triggerSwipeMode();
	void tryUpdateTimeMarkers();
	void undoLastAction(cocos2d::CCObject*);
	void updateButtons();
	void updateCreateMenu(bool);
	void updateDeleteButtons();
	void updateDeleteMenu();
	void updateEditMenu();
	void updateGridNodeSize(int);
	void updateGridNodeSize();
	void updateGroupIDBtn2();
	void updateGroupIDLabel();
	void updateObjectInfoLabel();
	void updatePlaybackBtn();
	void updateScaleControl();
	void updateSlider();
	void updateSpecialUIElements();
	void updateZoom(float);
	void valueFromXPos(float);
	void xPosFromValue(float);
	void zoomGameLayer(bool);
	void zoomIn(cocos2d::CCObject*);
	void zoomOut(cocos2d::CCObject*);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void angleChanged(float);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void colorSelectClosed(cocos2d::CCNode *);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void keyUp(cocos2d::enumKeyCodes);
	virtual void scaleChanged(float);
	virtual void scrollWheel(float,float);
}
class EditTriggersPopup : FLAlertLayer {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTouchTriggered();
}
class EffectGameObject : GameObject {
	void create(char const*);
	void customObjectSetup(gd::map<gd::string, gd::string, gd::less<gd::string >, gd::allocator<gd::pair<gd::string const, gd::string > > >&);
	void customSetup();
	void getSaveString();
	void getTargetColorIndex();
	void init(char const*);
	void resetSpawnTrigger();
	void spawnXPosition();
	void triggerActivated(float);
	void triggerObject(GJBaseGameLayer*);
	void updateSpecialColor();
	virtual void customObjectSetup(gd::map<gd::string,gd::string,gd::less<gd::string>,gd::allocator<gd::pair<gd::string const,gd::string>>> &);
	virtual void triggerActivated(float);
	virtual void triggerObject(GJBaseGameLayer *);
}
class EndLevelLayer : GJDropDownLayer {
	void coinEnterFinished(cocos2d::CCPoint);
	void coinEnterFinishedO(cocos2d::CCObject*);
	void create();
	void currencyEnterFinished();
	void customSetup();
	void diamondEnterFinished();
	void enterAnimFinished();
	void getCoinString();
	void getEndText();
	void goEdit();
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void keyUp(cocos2d::enumKeyCodes);
	void onEdit(cocos2d::CCObject*);
	void onEveryplay(cocos2d::CCObject*);
	void onMenu(cocos2d::CCObject*);
	void onReplay(cocos2d::CCObject*);
	void playCoinEffect(float);
	void playCurrencyEffect(float);
	void playDiamondEffect(float);
	void playEndEffect();
	void playStarEffect(float);
	void showLayer(bool);
	void starEnterFinished();
	void tryShowBanner(float);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void keyUp(cocos2d::enumKeyCodes);
	virtual void showLayer(bool);
}
class EndPortalObject : GameObject {
	void calculateSpawnXPos();
	void create();
	void init();
	void setPosition(cocos2d::CCPoint const&);
	void setVisible(bool);
	void triggerObject(GJBaseGameLayer*);
	void updateColors(cocos2d::_ccColor3B);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void setVisible(bool);
	virtual void triggerObject(GJBaseGameLayer *);
}
class ExtendedLayer : cocos2d::CCLayer {
	void create();
	void init();
	void setPosition(cocos2d::CCPoint const&);
	virtual void setPosition(cocos2d::CCPoint const&);
}
class FileSaveManager : GManager {
	void firstLoad();
	void getStoreData();
	void init();
	void loadDataFromFile(char const*);
	void sharedState();
}
class FLAlertLayer : cocos2d::CCLayerColor {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*);
	void create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float);
	void create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float);
	void init(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void onBtn1(cocos2d::CCObject*);
	void onBtn2(cocos2d::CCObject*);
	void onEnter();
	void registerWithTouchDispatcher();
	void show();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void keyDown(cocos2d::enumKeyCodes);
}
class FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
}
class FMODAudioEngine : cocos2d::CCNode {
	void disableMetering();
	void enableMetering();
	void fadeBackgroundMusic(bool, float);
	void getBackgroundMusicTime();
	void getBackgroundMusicVolume();
	void getEffectsVolume();
	void isBackgroundMusicPlaying(gd::string);
	void isBackgroundMusicPlaying();
	void pauseAllEffects();
	void pauseBackgroundMusic();
	void pauseEffect(unsigned int);
	void playBackgroundMusic(gd::string, bool, bool);
	void playEffect(gd::string, bool, float, float, float);
	void preloadBackgroundMusic(gd::string);
	void preloadEffect(gd::string);
	void printResult(FMOD_RESULT);
	void resumeAllEffects();
	void resumeBackgroundMusic();
	void resumeEffect(unsigned int);
	void rewindBackgroundMusic();
	void setBackgroundMusicTime(float);
	void setBackgroundMusicVolume(float);
	void setEffectsVolume(float);
	void setupAudioEngine();
	void sharedEngine();
	void start();
	void stop();
	void stopAllEffects();
	void stopBackgroundMusic(bool);
	void stopEffect(unsigned int);
	void unloadEffect(gd::string);
	void update(float);
	void willPlayBackgroundMusic();
	virtual void update(float);
}
class FMODSound : cocos2d::CCNode {
	void create(FMOD::Sound*);
	void init(FMOD::Sound*);
}
class FollowRewardPage : FLAlertLayer, FLAlertLayerProtocol, GameRateDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSpecialItem(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void show();
	void switchToOpenedState(CCMenuItemSpriteExtra*);
	void updateRate();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
}
class FontObject : cocos2d::CCObject {
	void createWithConfigFile(char const*, float);
	void getFontWidth(int);
	void initWithConfigFile(char const*, float);
	void parseConfigFile(char const*, float);
}
class FRequestProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, FriendRequestDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(bool);
	void deleteSelected();
	void forceReloadRequests(bool);
	void init(bool);
	void isCorrect(char const*);
	void keyBackClicked();
	void loadFRequestsFailed(char const*, GJErrorCode);
	void loadFRequestsFinished(cocos2d::CCArray*, char const*);
	void loadPage(int);
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onDeleteSelected(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onSentRequests(cocos2d::CCObject*);
	void onToggleAllObjects(cocos2d::CCObject*);
	void onUpdate(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void setupCommentsBrowser(cocos2d::CCArray*);
	void setupPageInfo(gd::string, char const*);
	void untoggleAll();
	void updateLevelsLabel();
	void updatePageArrows();
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void forceReloadRequests(bool);
	virtual void loadFRequestsFailed(char const*,GJErrorCode);
	virtual void loadFRequestsFinished(cocos2d::CCArray *,char const*);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void setupPageInfo(gd::string,char const*);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class FriendRequestDelegate {
	void forceReloadRequests(bool);
	void loadFRequestsFailed(char const*, GJErrorCode);
	void loadFRequestsFinished(cocos2d::CCArray*, char const*);
	void setupPageInfo(gd::string, char const*);
	virtual void forceReloadRequests(bool);
	virtual void loadFRequestsFailed(char const*,GJErrorCode);
	virtual void loadFRequestsFinished(cocos2d::CCArray *,char const*);
	virtual void setupPageInfo(gd::string,char const*);
}
class FriendRequestPopup : FLAlertLayer, UploadActionDelegate, UploadPopupDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void blockUser();
	void create(GJFriendRequest*);
	void init(GJFriendRequest*);
	void keyBackClicked();
	void loadFromGJFriendRequest(GJFriendRequest*);
	void onAccept(cocos2d::CCObject*);
	void onBlock(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onRemove(cocos2d::CCObject*);
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class FriendsProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, UserListDelegate {
	void create(UserListType);
	void forceReloadList(UserListType);
	void getUserListFailed(UserListType, GJErrorCode);
	void getUserListFinished(cocos2d::CCArray*, UserListType);
	void init(UserListType);
	void keyBackClicked();
	void onBlocked(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onUpdate(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void setupUsersBrowser(cocos2d::CCArray*, UserListType);
	void userListChanged(cocos2d::CCArray*, UserListType);
	virtual void forceReloadList(UserListType);
	virtual void getUserListFailed(UserListType,GJErrorCode);
	virtual void getUserListFinished(cocos2d::CCArray *,UserListType);
	virtual void userListChanged(cocos2d::CCArray *,UserListType);
}
class GameCell : TableViewCell {
	void draw();
	void init();
	void loadFromString(gd::string);
	void onTouch(cocos2d::CCObject*);
	void updateBGColor(int);
}
class GameEffectsManager : cocos2d::CCNode {
	void addParticleEffect(cocos2d::CCParticleSystemQuad*, int);
	void create(PlayLayer*);
	void init(PlayLayer*);
	void scaleParticle(cocos2d::CCParticleSystemQuad*, float);
}
class GameLevelManager : cocos2d::CCNode {
	void ProcessHttpRequest(gd::string, gd::string, gd::string, GJHttpType);
	void acceptFriendRequest(int, int);
	void accountIDForUserID(int);
	void addDLToActive(char const*);
	void areGauntletsLoaded();
	void banUser(int);
	void blockUser(int);
	void cleanupDailyLevels();
	void createAndGetAccountComments(gd::string, int);
	void createAndGetCommentsFull(gd::string, int, bool);
	void createAndGetLevelComments(gd::string, int);
	void createAndGetLevels(gd::string);
	void createAndGetMapPacks(gd::string);
	void createAndGetScores(gd::string, GJScoreType);
	void createNewLevel();
	void createPageInfo(int, int, int);
	void dataLoaded(DS_Dictionary*);
	void deleteAccountComment(int);
	void deleteComment(int, CommentType, int);
	void deleteFriendRequests(int, cocos2d::CCArray*, bool);
	void deleteLevel(GJGameLevel*);
	void deleteLevelComment(int, int);
	void deleteSentFriendRequest(int);
	void deleteServerLevel(int);
	void deleteUserMessages(GJUserMessage*, cocos2d::CCArray*, bool);
	void downloadLevel(int, bool);
	void downloadUserMessage(int, bool);
	void encodeDataTo(DS_Dictionary*);
	void firstSetup();
	void followUser(int);
	void friendRequestFromAccountID(int);
	void friendRequestWasRemoved(int, bool);
	void getAccountCommentKey(int, int);
	void getAccountComments(int, int, int);
	void getActiveDailyID(bool);
	void getAllUsedSongIDs();
	void getBasePostString(bool);
	void getBasePostStringFull();
	void getBoolForKey(char const*);
	void getCommentKey(int, int, int, bool);
	void getCompletedLevels(bool);
	void getDailyID(bool);
	void getDailyTimer(bool);
	void getDeleteCommentKey(int, int, int);
	void getDeleteMessageKey(int, bool);
	void getDescKey(int);
	void getDiffKey(int);
	void getDiffVal(int);
	void getDifficultyStr(bool, bool, bool, bool, bool, bool, bool, bool);
	void getFolderName(int, bool);
	void getFriendRequestKey(bool, int);
	void getFriendRequests(bool, int, int);
	void getGJChallenges();
	void getGJDailyLevelState(bool);
	void getGJRewards(int);
	void getGJUserInfo(int);
	void getGauntletKey(int);
	void getGauntletLevels(int);
	void getGauntletSearchKey(int);
	void getGauntlets();
	void getHighestLevelOrder();
	void getIntForKey(char const*);
	void getLeaderboardScores(char const*);
	void getLenKey(int);
	void getLenVal(int);
	void getLengthStr(bool, bool, bool, bool, bool);
	void getLevelComments(int, int, int, int, bool);
	void getLevelDownloadKey(int, bool);
	void getLevelKey(int);
	void getLevelLeaderboard(GJGameLevel*, LevelLeaderboardType);
	void getLevelLeaderboardKey(int, LevelLeaderboardType);
	void getLevelSaveData();
	void getLikeAccountItemKey(LikeItemType, int, bool, int);
	void getLikeItemKey(LikeItemType, int, bool, int);
	void getLocalLevel(int);
	void getLowestLevelOrder();
	void getMainLevel(int, bool);
	void getMapPackKey(int);
	void getMapPacks(GJSearchObject*);
	void getMessageKey(int);
	void getMessagesKey(bool, int);
	void getNextLevelName(gd::string);
	void getOnlineLevels(GJSearchObject*);
	void getPageInfo(char const*);
	void getPostCommentKey(int);
	void getRateStarsKey(int);
	void getReportKey(int);
	void getSavedDailyLevel(int);
	void getSavedDailyLevelFromLevelID(int);
	void getSavedGauntlet(int);
	void getSavedGauntletLevel(int);
	void getSavedLevel(int);
	void getSavedLevel(GJGameLevel*);
	void getSavedLevels(bool, int);
	void getSavedMapPack(int);
	void getSearchScene(char const*);
	void getSplitIntFromKey(char const*, int);
	void getStoredLevelComments(char const*);
	void getStoredOnlineLevels(char const*);
	void getStoredUserList(UserListType);
	void getStoredUserMessage(int);
	void getStoredUserMessageReply(int);
	void getTimeLeft(char const*, float);
	void getTopArtists(int, int);
	void getTopArtistsKey(int);
	void getUploadMessageKey(int);
	void getUserInfoKey(int);
	void getUserList(UserListType);
	void getUserMessages(bool, int, int);
	void getUsers(GJSearchObject*);
	void gotoLevelPage(GJGameLevel*);
	void handleIt(bool, gd::string, gd::string, GJHttpType);
	void handleItDelayed(bool, gd::string, gd::string, GJHttpType);
	void handleItND(cocos2d::CCNode*, void*);
	void hasDailyStateBeenLoaded(bool);
	void hasDownloadedLevel(int);
	void hasLikedAccountItem(LikeItemType, int, bool, int);
	void hasLikedItem(LikeItemType, int, bool, int);
	void hasLikedItemFullCheck(LikeItemType, int, int);
	void hasRatedDemon(int);
	void hasRatedLevelStars(int);
	void hasReportedLevel(int);
	void init();
	void invalidateMessages(bool, bool);
	void invalidateRequests(bool, bool);
	void invalidateUserList(UserListType, bool);
	void isDLActive(char const*);
	void isFollowingUser(int);
	void isTimeValid(char const*, float);
	void isUpdateValid(int);
	void itemIDFromLikeKey(char const*);
	void keyHasTimer(char const*);
	void levelIDFromCommentKey(char const*);
	void levelIDFromPostCommentKey(char const*);
	void likeFromLikeKey(char const*);
	void likeItem(LikeItemType, int, bool, int);
	void limitSavedLevels();
	void makeTimeStamp(char const*);
	void markItemAsLiked(LikeItemType, int, bool, int);
	void markLevelAsDownloaded(int);
	void markLevelAsRatedDemon(int);
	void markLevelAsRatedStars(int);
	void markLevelAsReported(int);
	void messageWasRemoved(int, bool);
	void onAcceptFriendRequestCompleted(gd::string, gd::string);
	void onBanUserCompleted(gd::string, gd::string);
	void onBlockUserCompleted(gd::string, gd::string);
	void onDeleteCommentCompleted(gd::string, gd::string);
	void onDeleteFriendRequestCompleted(gd::string, gd::string);
	void onDeleteServerLevelCompleted(gd::string, gd::string);
	void onDeleteUserMessagesCompleted(gd::string, gd::string);
	void onDownloadLevelCompleted(gd::string, gd::string);
	void onDownloadUserMessageCompleted(gd::string, gd::string);
	void onGetAccountCommentsCompleted(gd::string, gd::string);
	void onGetFriendRequestsCompleted(gd::string, gd::string);
	void onGetGJChallengesCompleted(gd::string, gd::string);
	void onGetGJRewardsCompleted(gd::string, gd::string);
	void onGetGJUserInfoCompleted(gd::string, gd::string);
	void onGetGauntletsCompleted(gd::string, gd::string);
	void onGetLeaderboardScoresCompleted(gd::string, gd::string);
	void onGetLevelCommentsCompleted(gd::string, gd::string);
	void onGetLevelLeaderboardCompleted(gd::string, gd::string);
	void onGetLevelSaveDataCompleted(gd::string, gd::string);
	void onGetMapPacksCompleted(gd::string, gd::string);
	void onGetOnlineLevelsCompleted(gd::string, gd::string);
	void onGetTopArtistsCompleted(gd::string, gd::string);
	void onGetUserListCompleted(gd::string, gd::string);
	void onGetUserMessagesCompleted(gd::string, gd::string);
	void onGetUsersCompleted(gd::string, gd::string);
	void onLikeItemCompleted(gd::string, gd::string);
	void onProcessHttpRequestCompleted(cocos2d::extension::CCHttpClient*, cocos2d::extension::CCHttpResponse*);
	void onRateDemonCompleted(gd::string, gd::string);
	void onRateStarsCompleted(gd::string, gd::string);
	void onReadFriendRequestCompleted(gd::string, gd::string);
	void onRemoveFriendCompleted(gd::string, gd::string);
	void onReportLevelCompleted(gd::string, gd::string);
	void onRequestUserAccessCompleted(gd::string, gd::string);
	void onRestoreItemsCompleted(gd::string, gd::string);
	void onSetLevelFeaturedCompleted(gd::string, gd::string);
	void onSetLevelStarsCompleted(gd::string, gd::string);
	void onSubmitUserInfoCompleted(gd::string, gd::string);
	void onSuggestLevelStarsCompleted(gd::string, gd::string);
	void onUnblockUserCompleted(gd::string, gd::string);
	void onUpdateDescriptionCompleted(gd::string, gd::string);
	void onUpdateLevelCompleted(gd::string, gd::string);
	void onUpdateUserScoreCompleted(gd::string, gd::string);
	void onUploadCommentCompleted(gd::string, gd::string);
	void onUploadFriendRequestCompleted(gd::string, gd::string);
	void onUploadLevelCompleted(gd::string, gd::string);
	void onUploadUserMessageCompleted(gd::string, gd::string);
	void ongetGJDailyLevelStateCompleted(gd::string, gd::string);
	void pageFromCommentKey(char const*);
	void parseRestoreData(gd::string);
	void processOnDownloadLevelCompleted(gd::string, gd::string, bool);
	void purgeUnusedLevels();
	void rateDemon(int, int, bool);
	void rateStars(int, int);
	void readFriendRequest(int);
	void removeDLFromActive(char const*);
	void removeDelimiterChars(gd::string, bool);
	void removeFriend(int);
	void removeLevelDownloadedKeysFromDict(cocos2d::CCDictionary*);
	void removeUserFromList(int, UserListType);
	void reportLevel(int);
	void requestUserAccess();
	void resetAccountComments(int);
	void resetAllTimers();
	void resetCommentTimersForLevelID(int, bool);
	void resetDailyLevelState(bool);
	void resetGauntlets();
	void resetStoredUserInfo(int);
	void resetStoredUserList(UserListType);
	void resetTimerForKey(char const*);
	void responseToDict(gd::string, bool);
	void restoreItems();
	void saveFetchedLevels(cocos2d::CCArray*);
	void saveFetchedMapPacks(cocos2d::CCArray*);
	void saveGauntlet(GJMapPack*);
	void saveLevel(GJGameLevel*);
	void saveMapPack(GJMapPack*);
	void setBoolForKey(bool, char const*);
	void setDiffVal(int, bool);
	void setDownloadedLevelsDict(cocos2d::CCDictionary*);
	void setFolderName(int, gd::string, bool);
	void setFolderNamesCreatedDict(cocos2d::CCDictionary*);
	void setFolderNamesDict(cocos2d::CCDictionary*);
	void setFollowedUsersDict(cocos2d::CCDictionary*);
	void setIntForKey(int, char const*);
	void setLenVal(int, bool);
	void setLevelFeatured(int, int, bool);
	void setLevelStars(int, int, bool);
	void setLikedItemsDict(cocos2d::CCDictionary*);
	void setMainLevels(cocos2d::CCDictionary*);
	void setRatedDemonsDict(cocos2d::CCDictionary*);
	void setRatedLevelsDict(cocos2d::CCDictionary*);
	void setReportedLevelsDict(cocos2d::CCDictionary*);
	void setSavedDailyLevelsDict(cocos2d::CCDictionary*);
	void setSavedGauntletLevelsDict(cocos2d::CCDictionary*);
	void setSavedLevelsDict(cocos2d::CCDictionary*);
	void setSettingsDict(cocos2d::CCDictionary*);
	void sharedState();
	void specialFromLikeKey(char const*);
	void storeCommentsResult(cocos2d::CCArray*, gd::string, char const*);
	void storeDailyLevelState(int, int, bool);
	void storeFriendRequest(GJFriendRequest*);
	void storeSearchResult(cocos2d::CCArray*, gd::string, char const*);
	void storeUserInfo(GJUserScore*);
	void storeUserMessage(GJUserMessage*);
	void storeUserMessageReply(int, GJUserMessage*);
	void storeUserName(int, int, gd::string);
	void storeUserNames(gd::string);
	void submitUserInfo();
	void suggestLevelStars(int, int, bool);
	void typeFromCommentKey(char const*);
	void typeFromLikeKey(char const*);
	void unblockUser(int);
	void unfollowUser(int);
	void updateDescription(int, gd::string);
	void updateLevel(GJGameLevel*);
	void updateLevelOrders();
	void updateLevelRewards(GJGameLevel*);
	void updateUserScore();
	void updateUsernames();
	void uploadAccountComment(gd::string);
	void uploadComment(gd::string, CommentType, int, int);
	void uploadFriendRequest(int, gd::string);
	void uploadLevel(GJGameLevel*);
	void uploadLevelComment(int, gd::string, int);
	void uploadUserMessage(int, gd::string, gd::string);
	void userIDForAccountID(int);
	void userInfoForAccountID(int);
	void userNameForUserID(int);
	void verifyLevelState(GJGameLevel*);
}
class GameManager : GManager {
	void accountStatusChanged();
	void addNewCustomObject(gd::string);
	void applicationDidEnterBackground();
	void applicationWillEnterForeground();
	void canShowRewardedVideoAd();
	void checkSteamAchievementUnlock();
	void checkUsedIcons();
	void claimItemsResponse(gd::string);
	void colorForIdx(int);
	void colorForPos(int);
	void colorKey(int, bool);
	void completedAchievement(char const*);
	void dataLoaded(DS_Dictionary*);
	void didExitPlayscene();
	void doQuickSave();
	void encodeDataTo(DS_Dictionary*);
	void eventUnlockFeature(char const*);
	void fadeInMusic(char const*);
	void firstLoad();
	void followTwitter();
	void generateSecretNumber();
	void getBGTexture(int);
	void getFontFile(int);
	void getFontTexture(int);
	void getGTexture(int);
	void getGameVariable(char const*);
	void getIntGameVariable(char const*);
	void getNextUniqueObjectKey();
	void getNextUsedKey(int, bool);
	void getOrderedCustomObjectKeys();
	void getPlayerUDID();
	void getUGV(char const*);
	void hasRatingPower();
	void hasShownAdRecently();
	void iconKey(int, IconType);
	void iconTypeToUnlockType(IconType);
	void init();
	void isColorUnlocked(int, bool);
	void isIconUnlocked(int, IconType);
	void itemPurchased(char const*);
	void levelIsPremium(int, int);
	void likeFacebook();
	void loadBackground(int);
	void loadDeathEffect(int);
	void loadFont(int);
	void loadGround(int);
	void loadVideoSettings();
	void lockColor(int, bool);
	void lockIcon(int, IconType);
	void openEditorGuide();
	void rateGame();
	void recountUserStats(gd::string);
	void reloadAll(bool, bool, bool);
	void reloadAllStep2();
	void reloadAllStep3();
	void reloadAllStep4();
	void reloadAllStep5();
	void removeCustomObject(int);
	void reorderKey(int, bool);
	void reportAchievementWithID(char const*, int, bool);
	void reportPercentageForLevel(int, int, bool);
	void resetAchievement(char const*);
	void resetAllIcons();
	void resetCoinUnlocks();
	void resolutionForKey(int);
	void returnToLastScene(GJGameLevel*);
	void rewardedVideoAdFinished(int);
	void rewardedVideoHidden();
	void rewardedVideoHiddenDelayed();
	void safePopScene();
	void setCustomObjectDict(cocos2d::CCDictionary*);
	void setGameVariable(char const*, bool);
	void setHasRatingPower(int);
	void setIntGameVariable(char const*, int);
	void setPlayerBall(int);
	void setPlayerBird(int);
	void setPlayerColor(int);
	void setPlayerColor2(int);
	void setPlayerDart(int);
	void setPlayerDeathEffect(int);
	void setPlayerFrame(int);
	void setPlayerRobot(int);
	void setPlayerShip(int);
	void setPlayerSpider(int);
	void setPlayerStreak(int);
	void setPlayerUserID(int);
	void setUGV(char const*, bool);
	void setUnlockValueKeeper(cocos2d::CCDictionary*);
	void setValueKeeper(cocos2d::CCDictionary*);
	void sharedState();
	void shortenAdTimer(float);
	void shouldShowInterstitial();
	void showInterstitial();
	void showInterstitialForced();
	void showMainMenuAd();
	void startUpdate();
	void stringForCustomObject(int);
	void subYouTube();
	void switchCustomObjects(int, int);
	void switchScreenMode(bool, bool);
	void syncPlatformAchievements();
	void toggleGameVariable(char const*);
	void tryCacheAd();
	void tryResetAdTimer();
	void unloadBackground();
	void unlockColor(int, bool);
	void unlockIcon(int, IconType);
	void unlockTypeToIconType(int);
	void unlockedPremium();
	void update(float);
	void updateMusic();
	void verifyAchievementUnlocks();
	void verifyCoinUnlocks();
	void verifyStarUnlocks();
	void verifySyncedCoins();
	void videoAdHidden();
	void videoAdShowed();
	virtual void dataLoaded(DS_Dictionary *);
	virtual void encodeDataTo(DS_Dictionary *);
	virtual void update(float);
}
class GameObject : CCSpritePlus {
	void activateObject();
	void activatedByPlayer(GameObject*);
	void addColorSprite();
	void addColorSpriteToParent(bool);
	void addCustomBlackChild(gd::string, float, bool);
	void addCustomChild(gd::string, cocos2d::CCPoint, int);
	void addCustomColorChild(gd::string);
	void addEmptyGlow();
	void addGlow();
	void addInternalChild(cocos2d::CCSprite*, gd::string, cocos2d::CCPoint, int);
	void addInternalCustomColorChild(gd::string, cocos2d::CCPoint, int);
	void addInternalGlowChild(gd::string, cocos2d::CCPoint);
	void addMainSpriteToParent(bool);
	void addNewSlope01(bool);
	void addNewSlope01Glow(bool);
	void addNewSlope02(bool);
	void addNewSlope02Glow(bool);
	void addToColorGroup(int);
	void addToGroup(int);
	void addToOpacityGroup(int);
	void addToTempOffset(float, float);
	void belongsToGroup(int);
	void calculateOrientedBox();
	void calculateSpawnXPos();
	void canAllowMultiActivate();
	void canChangeCustomColor();
	void canChangeMainColor();
	void canChangeSecondaryColor();
	void canRotateFree();
	void colorForMode(int, bool);
	void commonSetup();
	void copyGroups(GameObject*);
	void createAndAddParticle(int, char const*, int, cocos2d::tCCPositionType);
	void createColorGroupContainer(int);
	void createGlow(gd::string);
	void createGroupContainer(int);
	void createOpacityGroupContainer(int);
	void createRotateAction(float, int);
	void createSpriteColor(int);
	void createWithFrame(char const*);
	void createWithKey(int);
	void createWithTexture(cocos2d::CCTexture2D*);
	void customObjectSetup(gd::map<gd::string, gd::string, gd::less<gd::string >, gd::allocator<gd::pair<gd::string const, gd::string > > >&);
	void customScaleMod(char const*);
	void customSetup();
	void deactivateObject(bool);
	void deselectObject();
	void destroyObject();
	void determineSlopeDirection();
	void disableObject();
	void dontCountTowardsLimit();
	void duplicateAttributes(GameObject*);
	void duplicateColorMode(GameObject*);
	void duplicateValues(GameObject*);
	void editorColorForCustomMode(int);
	void editorColorForMode(int);
	void getActiveColorForMode(int, bool);
	void getBallFrame(int);
	void getBoxOffset();
	void getColorFrame(gd::string);
	void getColorIndex();
	void getDidUpdateLastPosition();
	void getGlowFrame(gd::string);
	void getGroupDisabled();
	void getGroupID(int);
	void getGroupString();
	void getLastPosition();
	void getMainColor();
	void getMainColorMode();
	void getObjectRadius();
	void getObjectRect(float, float);
	void getObjectRect();
	void getObjectRect2(float, float);
	void getObjectRectDirty();
	void getObjectTextureRect();
	void getObjectZLayer();
	void getObjectZOrder();
	void getOrientedBox();
	void getOrientedRectDirty();
	void getOuterObjectRect();
	void getParentMode();
	void getRScaleX();
	void getRScaleY();
	void getRealPosition();
	void getRelativeSpriteColor(int);
	void getSaveString();
	void getSecondaryColor();
	void getSecondaryColorMode();
	void getSectionIdx();
	void getStartPos();
	void getType();
	void getUpdateLastPos();
	void groupColor(cocos2d::_ccColor3B const&, bool);
	void groupOpacityMod();
	void groupWasDisabled();
	void groupWasEnabled();
	void hasBeenActivated();
	void hasBeenActivatedByPlayer(GameObject*);
	void hasSecondaryColor();
	void ignoreEditorDuration();
	void ignoreEnter();
	void ignoreFade();
	void init(char const*);
	void initWithTexture(cocos2d::CCTexture2D*);
	void isBasicTrigger();
	void isColorObject();
	void isColorTrigger();
	void isEditorSpawnableTrigger();
	void isFacingDown();
	void isFlipX();
	void isFlipY();
	void isSpawnableTrigger();
	void isSpecialObject();
	void isStoppableTrigger();
	void isTrigger();
	void loadGroupsFromString(gd::string);
	void objectFromString(gd::string, bool);
	void opacityModForMode(int, bool);
	void parentForZLayer(int, bool, int);
	void perspectiveColorFrame(char const*, int);
	void perspectiveFrame(char const*, int);
	void playShineEffect();
	void powerOffObject();
	void powerOnObject();
	void quickUpdatePosition();
	void quickUpdatePosition2();
	void removeColorSprite();
	void removeFromGroup(int);
	void removeGlow();
	void reorderColorSprite();
	void resetGroupDisabled();
	void resetMainColorMode();
	void resetObject();
	void resetSecondaryColorMode();
	void saveActiveColors();
	void selectObject(cocos2d::_ccColor3B);
	void setChildColor(cocos2d::_ccColor3B const&);
	void setDefaultMainColorMode(int);
	void setDefaultSecondaryColorMode(int);
	void setDidUpdateLastPosition(bool const&);
	void setFlipX(bool);
	void setFlipY(bool);
	void setGlowColor(cocos2d::_ccColor3B const&);
	void setGlowOpacity(unsigned char);
	void setLastPosition(cocos2d::CCPoint const&);
	void setMainColorMode(int);
	void setMyAction(cocos2d::CCAction*);
	void setObjectColor(cocos2d::_ccColor3B const&);
	void setObjectRectDirty(bool);
	void setOpacity(unsigned char);
	void setOrientedRectDirty(bool);
	void setPosition(cocos2d::CCPoint const&);
	void setRScale(float);
	void setRScaleX(float);
	void setRScaleY(float);
	void setRotation(float);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void setSecondaryColorMode(int);
	void setSectionIdx(int const&);
	void setStartPos(cocos2d::CCPoint);
	void setTempOffset(float, float);
	void setType(GameObjectType);
	void setUpdateLastPos(bool const&);
	void setVisible(bool);
	void setupCoinAnimation();
	void setupCoinArt();
	void setupColorSprite(int, bool);
	void setupCustomSprites();
	void setupSpriteSize();
	void shouldBlendColor(GJSpriteColor*, bool);
	void shouldNotSave();
	void slopeFloorTop();
	void slopeWallLeft();
	void slopeYPos(float);
	void slopeYPos(cocos2d::CCRect);
	void slopeYPos(GameObject*);
	void spawnXPosition();
	void triggerActivated(float);
	void triggerObject(GJBaseGameLayer*);
	void update(float);
	void updateCustomScale(float);
	void updateMainColor(cocos2d::_ccColor3B const&);
	void updateMainColor();
	void updateObjectEditorColor();
	void updateOrientedBox();
	void updateParticleColor(cocos2d::_ccColor3B const&);
	void updateSecondaryColor(cocos2d::_ccColor3B const&);
	void updateSecondaryColor();
	void updateStartPos();
	void updateStartValues();
	void updateState();
	void updateSyncedAnimation(float);
	void updateTextObject(gd::string, bool);
	void waitingForActivation();
	virtual void addMainSpriteToParent(bool);
	virtual void addToGroup(int);
	virtual void customObjectSetup(gd::map<gd::string,gd::string,gd::less<gd::string>,gd::allocator<gd::pair<gd::string const,gd::string>>> &);
	virtual void deactivateObject(bool);
	virtual void getObjectRect(float,float);
	virtual void getObjectRect2(float,float);
	virtual void initWithTexture(cocos2d::CCTexture2D *);
	virtual void removeFromGroup(int);
	virtual void setChildColor(cocos2d::_ccColor3B const&);
	virtual void setFlipX(bool);
	virtual void setFlipY(bool);
	virtual void setGlowColor(cocos2d::_ccColor3B const&);
	virtual void setObjectColor(cocos2d::_ccColor3B const&);
	virtual void setObjectRectDirty(bool);
	virtual void setOpacity(uchar);
	virtual void setOrientedRectDirty(bool);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void setRScale(float);
	virtual void setRScaleX(float);
	virtual void setRScaleY(float);
	virtual void setRotation(float);
	virtual void setScale(float);
	virtual void setScaleX(float);
	virtual void setScaleY(float);
	virtual void setStartPos(cocos2d::CCPoint);
	virtual void setType(GameObjectType);
	virtual void setVisible(bool);
	virtual void triggerActivated(float);
	virtual void triggerObject(GJBaseGameLayer *);
	virtual void update(float);
}
class GameObjectCopy : cocos2d::CCObject {
	void create(GameObject*);
	void init(GameObject*);
	void resetObject();
}
class GameRateDelegate {
	void updateRate();
}
class GameSoundManager : cocos2d::CCNode {
	void asynchronousSetup();
	void disableMetering();
	void enableMetering();
	void fadeInMusic(bool);
	void fadeOutMusic(bool);
	void getMeteringValue();
	void init();
	void isLoopedSoundPlaying(gd::string);
	void pauseAllLoopedSounds();
	void pauseLoopedSound(gd::string);
	void playBackgroundMusic(gd::string, bool, bool);
	void playEffect(gd::string, float, float, float);
	void playLoopedSound(gd::string, gd::string, float, float, bool, bool, bool);
	void playUniqueEffect(gd::string);
	void playUniqueEffect(gd::string, float, float, float);
	void preload();
	void removeLoopedSound(gd::string);
	void resetUniqueEffects();
	void resumeAllLoopedSounds();
	void resumeSound();
	void setup();
	void sharedManager();
	void stopAllLoopedSounds();
	void stopBackgroundMusic();
	void stopLoopedSound(gd::string, bool);
	void updateLoopedVolume(gd::string, float);
	void updateMetering(float);
}
class GameStatsManager : cocos2d::CCNode {
	void addStoreItem(int, int, int, int, ShopType);
	void areChallengesLoaded();
	void areRewardsLoaded();
	void awardCurrencyForLevel(GJGameLevel*);
	void awardDiamondsForLevel(GJGameLevel*);
	void awardSecretKey();
	void checkAchievement(char const*);
	void checkCoinAchievement(GJGameLevel*);
	void checkCoinsForLevel(GJGameLevel*);
	void collectReward(GJRewardType, GJRewardItem*);
	void collectVideoReward(int);
	void completedChallenge(GJChallengeItem*);
	void completedDailyLevel(GJGameLevel*);
	void completedDemonLevel(GJGameLevel*);
	void completedLevel(GJGameLevel*);
	void completedMapPack(GJMapPack*);
	void completedStarLevel(GJGameLevel*);
	void createSecretChestItems();
	void createSecretChestRewards();
	void createSpecialChestItems();
	void createStoreItems();
	void dataLoaded(DS_Dictionary*);
	void encodeDataTo(DS_Dictionary*);
	void firstSetup();
	void generateItemUnlockableData();
	void getAwardedCurrencyForLevel(GJGameLevel*);
	void getAwardedDiamondsForLevel(GJGameLevel*);
	void getBaseCurrency(int, bool, int);
	void getBaseCurrencyForLevel(GJGameLevel*);
	void getBaseDiamonds(int);
	void getBonusDiamonds(int);
	void getBonusShardCount();
	void getChallenge(int);
	void getChallengeKey(GJChallengeItem*);
	void getCollectedCoinsForLevel(GJGameLevel*);
	void getCompletedMapPacks();
	void getCurrencyKey(GJGameLevel*);
	void getDailyLevelKey(int);
	void getDemonLevelKey(GJGameLevel*);
	void getGauntletRewardKey(int);
	void getItemKey(int, UnlockType);
	void getItemUnlockState(int, UnlockType);
	void getLevelKey(int, bool, bool, bool);
	void getLevelKey(GJGameLevel*);
	void getLiteAchievements();
	void getMapPackKey(int);
	void getQueuedChallenge(int);
	void getRewardForSecretChest(int);
	void getRewardForSpecialChest(gd::string);
	void getRewardItem(GJRewardType);
	void getRewardKey(GJRewardType, int);
	void getSecondaryQueuedChallenge(int);
	void getSecretChestForItem(int, UnlockType);
	void getSecretCoinKey(char const*);
	void getSpecialChestKeyForItem(int, UnlockType);
	void getStarLevelKey(GJGameLevel*);
	void getStat(char const*);
	void getStoreItem(int);
	void getStoreItem(int, int);
	void getStoreItemKey(int, int);
	void getTotalCollectedCurrency();
	void getTotalCollectedDiamonds();
	void hasCompletedChallenge(GJChallengeItem*);
	void hasCompletedDailyLevel(int);
	void hasCompletedDemonLevel(GJGameLevel*);
	void hasCompletedGauntletLevel(int);
	void hasCompletedLevel(GJGameLevel*);
	void hasCompletedMainLevel(int);
	void hasCompletedMapPack(int);
	void hasCompletedOnlineLevel(int);
	void hasCompletedStarLevel(GJGameLevel*);
	void hasPendingUserCoin(char const*);
	void hasRewardBeenCollected(GJRewardType, int);
	void hasSecretCoin(char const*);
	void hasUserCoin(char const*);
	void incrementChallenge(GJChallengeType, int);
	void incrementStat(char const*);
	void incrementStat(char const*, int);
	void init();
	void isItemUnlocked(UnlockType, int);
	void isLiteUnlockable(gd::string);
	void isSecretChestUnlocked(int);
	void isSecretCoin(gd::string);
	void isSecretCoinValid(gd::string);
	void isSpecialChestUnlocked(gd::string);
	void isStoreItemUnlocked(int);
	void logCoins();
	void markLevelAsCompletedAndClaimed(GJGameLevel*);
	void postLoadGameStats();
	void preProcessReward(GJRewardItem*);
	void preSaveGameStats();
	void processChallengeQueue(int);
	void purchaseItem(int);
	void recountSpecialStats();
	void recountUserCoins(bool);
	void registerRewardsFromItem(GJRewardItem*);
	void removeChallenge(int);
	void removeErrorFromSpecialChests();
	void removeQueuedChallenge(int);
	void removeQueuedSecondaryChallenge(int);
	void resetChallengeTimer();
	void resetPreSync();
	void resetSpecialStatAchievements();
	void resetUserCoins();
	void restorePostSync();
	void setAwardedBonusKeys(int);
	void setChallengeItemDict(cocos2d::CCDictionary*);
	void setChallengeItemQueuedDict(cocos2d::CCDictionary*);
	void setCollectedRewards(cocos2d::CCDictionary*);
	void setCollectedVideoRewards(cocos2d::CCDictionary*);
	void setCollectedWeeklyLevelRewards(cocos2d::CCDictionary*);
	void setCompletedLevels(cocos2d::CCDictionary*);
	void setCurrencyAwardDict(cocos2d::CCDictionary*);
	void setCurrencyDailyLevelDict(cocos2d::CCDictionary*);
	void setCurrencyDailyLevelStarsDict(cocos2d::CCDictionary*);
	void setCurrencyGauntletLevelDict(cocos2d::CCDictionary*);
	void setCurrencyLevelDict(cocos2d::CCDictionary*);
	void setCurrencyLevelStarsDict(cocos2d::CCDictionary*);
	void setCurrencyMainLevelDict(cocos2d::CCDictionary*);
	void setCurrencyUnlockDict(cocos2d::CCDictionary*);
	void setDiamondAwardDict(cocos2d::CCDictionary*);
	void setDiamondsDailyLevelDict(cocos2d::CCDictionary*);
	void setDiamondsLevelDict(cocos2d::CCDictionary*);
	void setPendingUserCoinsDict(cocos2d::CCDictionary*);
	void setRandomContainer(cocos2d::CCDictionary*);
	void setRewardItemDict(cocos2d::CCDictionary*);
	void setStarsForMapPack(int, int);
	void setStat(char const*, int);
	void setStatIfHigher(char const*, int);
	void setUnlockedSecretChests(cocos2d::CCDictionary*);
	void setUnlockedSpecialChests(cocos2d::CCDictionary*);
	void setUserCoinsDict(cocos2d::CCDictionary*);
	void setValueDict(cocos2d::CCDictionary*);
	void sharedState();
	void shouldAwardSecretKey();
	void starsForMapPack(int);
	void storeChallenge(int, GJChallengeItem*);
	void storeChallengeTime(int);
	void storePendingUserCoin(char const*);
	void storeQueuedChallenge(int, GJChallengeItem*);
	void storeRewardState(GJRewardType, int, int, gd::string);
	void storeSecondaryQueuedChallenge(int, GJChallengeItem*);
	void storeSecretCoin(char const*);
	void storeUserCoin(char const*);
	void tempClear();
	void uncompleteLevel(GJGameLevel*);
	void unlockSecretChest(int);
	void unlockSpecialChest(gd::string);
	void verifyUserCoins();
}
class GaragePage : cocos2d::CCLayer, ListButtonBarDelegate {
	void create(IconType, GJGarageLayer*, cocos2d::SEL_MenuHandler);
	void init(IconType, GJGarageLayer*, cocos2d::SEL_MenuHandler);
	void listButtonBarSwitchedPage(ListButtonBar*, int);
	void onGlow(cocos2d::CCObject*);
	void onSelect(cocos2d::CCObject*);
	void updateSelect(cocos2d::CCNode*);
	virtual void listButtonBarSwitchedPage(ListButtonBar *,int);
}
class GauntletLayer : cocos2d::CCLayer, LevelManagerDelegate {
	void create(GauntletType);
	void init(GauntletType);
	void keyBackClicked();
	void loadLevelsFailed(char const*);
	void loadLevelsFinished(cocos2d::CCArray*, char const*);
	void onBack(cocos2d::CCObject*);
	void onLevel(cocos2d::CCObject*);
	void scene(GauntletType);
	void setupGauntlet(cocos2d::CCArray*);
	void unlockActiveItem();
	virtual void loadLevelsFailed(char const*);
	virtual void loadLevelsFinished(cocos2d::CCArray *,char const*);
}
class GauntletNode : cocos2d::CCNode {
	void create(GJMapPack*);
	void frameForType(GauntletType);
	void init(GJMapPack*);
	void nameForType(GauntletType);
	void onClaimReward();
}
class GauntletSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate, LevelManagerDelegate {
	void create(int);
	void goToPage(int, bool);
	void init(int);
	void keyBackClicked();
	void loadLevelsFailed(char const*);
	void loadLevelsFinished(cocos2d::CCArray*, char const*);
	void onBack(cocos2d::CCObject*);
	void onExit();
	void onNext(cocos2d::CCObject*);
	void onPlay(cocos2d::CCObject*);
	void onPrev(cocos2d::CCObject*);
	void onRefresh(cocos2d::CCObject*);
	void scene(int);
	void scrollLayerScrolledToPage(BoomScrollLayer*, int);
	void scrollLayerWillScrollToPage(BoomScrollLayer*, int);
	void setupGauntlets();
	void updateArrows();
	virtual void loadLevelsFailed(char const*);
	virtual void loadLevelsFinished(cocos2d::CCArray *,char const*);
	virtual void scrollLayerScrolledToPage(BoomScrollLayer *,int);
	virtual void scrollLayerWillScrollToPage(BoomScrollLayer *,int);
}
class GhostTrailEffect : cocos2d::CCNode {
	void create();
	void doBlendAdditive();
	void draw();
	void init();
	void runWithTarget(cocos2d::CCSprite*, float, float, float, float, bool);
	void stopTrail();
	void trailSnapshot(float);
}
class GJAccountBackupDelegate {
	void backupAccountFailed(BackupAccountError);
	void backupAccountFinished();
	virtual void backupAccountFailed(BackupAccountError);
}
class GJAccountDelegate {
	void accountStatusChanged();
}
class GJAccountLoginDelegate {
	void loginAccountFailed(AccountError);
	void loginAccountFinished(int, int);
	virtual void loginAccountFailed(AccountError);
	virtual void loginAccountFinished(int,int);
}
class GJAccountManager : cocos2d::CCNode {
	void ProcessHttpRequest(gd::string, gd::string, gd::string, GJHttpType);
	void addDLToActive(char const*);
	void addDLToActive(char const*, cocos2d::CCObject*);
	void backupAccount(gd::string);
	void dataLoaded(DS_Dictionary*);
	void encodeDataTo(DS_Dictionary*);
	void firstSetup();
	void getAccountBackupURL();
	void getAccountSyncURL();
	void getDLObject(char const*);
	void getGJP();
	void handleIt(bool, gd::string, gd::string, GJHttpType);
	void handleItDelayed(bool, gd::string, gd::string, GJHttpType);
	void handleItND(cocos2d::CCNode*, void*);
	void init();
	void isDLActive(char const*);
	void linkToAccount(gd::string, gd::string, int, int);
	void loginAccount(gd::string, gd::string);
	void onBackupAccountCompleted(gd::string, gd::string);
	void onGetAccountBackupURLCompleted(gd::string, gd::string);
	void onGetAccountSyncURLCompleted(gd::string, gd::string);
	void onLoginAccountCompleted(gd::string, gd::string);
	void onProcessHttpRequestCompleted(cocos2d::extension::CCHttpClient*, cocos2d::extension::CCHttpResponse*);
	void onRegisterAccountCompleted(gd::string, gd::string);
	void onSyncAccountCompleted(gd::string, gd::string);
	void onUpdateAccountSettingsCompleted(gd::string, gd::string);
	void registerAccount(gd::string, gd::string, gd::string);
	void removeDLFromActive(char const*);
	void sharedState();
	void syncAccount(gd::string);
	void unlinkFromAccount();
	void updateAccountSettings(int, int, int, gd::string, gd::string, gd::string);
}
class GJAccountRegisterDelegate {
	void registerAccountFailed(AccountError);
	void registerAccountFinished();
	virtual void registerAccountFailed(AccountError);
}
class GJAccountSettingsDelegate {
	void updateSettingsFailed();
	void updateSettingsFinished();
}
class GJAccountSettingsLayer : FLAlertLayer, TextInputDelegate {
	void create(int);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, float, float);
	void init(int);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onCommentSetting(cocos2d::CCObject*);
	void onFriendRequests(cocos2d::CCObject*);
	void onMessageSetting(cocos2d::CCObject*);
	void onUpdate(cocos2d::CCObject*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateScoreValues();
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class GJAccountSyncDelegate {
	void syncAccountFailed(BackupAccountError);
	void syncAccountFinished();
	virtual void syncAccountFailed(BackupAccountError);
}
class GJBaseGameLayer : cocos2d::CCLayer, TriggerEffectDelegate {
	void addObjectCounter(LabelGameObject*, int);
	void addToGroup(GameObject*, int, bool);
	void addToGroups(GameObject*, bool);
	void addToSection(GameObject*);
	void atlasValue(int);
	void bumpPlayer(PlayerObject*, GameObject*);
	void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*);
	void calculateOpacityValues(EffectGameObject*, EffectGameObject*, float, GJEffectManager*);
	void checkSpawnObjects();
	void collectItem(int, int);
	void collectedObject(EffectGameObject*);
	void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int);
	void createTextLayers();
	void damagingObjectsInRect(cocos2d::CCRect);
	void enableHighCapacityMode();
	void flipGravity(PlayerObject*, bool, bool);
	void getCapacityString();
	void getGroundHeightForMode(int);
	void getGroup(int);
	void getMoveDeltaForObjects(int, int);
	void getOptimizedGroup(int);
	void getStaticGroup(int);
	void init();
	void isGroupDisabledForObject(GameObject*);
	void isGroupDisabledForObjectFull(GameObject*, cocos2d::CCArray*);
	void loadUpToPosition(float);
	void objectIntersectsCircle(GameObject*, GameObject*);
	void objectTriggered(EffectGameObject*);
	void objectsCollided(int, int);
	void optimizeMoveGroups();
	void parentForZLayer(int, bool, int);
	void playerTouchedRing(PlayerObject*, GameObject*);
	void processColorObject(EffectGameObject*, int, cocos2d::CCDictionary*, float, GJEffectManager*);
	void processFollowActions();
	void processMoveActions();
	void processMoveActionsStep(float);
	void processOpacityObject(EffectGameObject*, cocos2d::CCDictionary*, float, GJEffectManager*);
	void processPlayerFollowActions(float);
	void processRotationActions();
	void pushButton(int, bool);
	void rectIntersectsCircle(cocos2d::CCRect, cocos2d::CCPoint, float);
	void refreshCounterLabels();
	void releaseButton(int, bool);
	void removeFromGroup(GameObject*, int);
	void removeFromGroups(GameObject*);
	void removeObjectFromSection(GameObject*);
	void reorderObjectSection(GameObject*);
	void resetGroupCounters(bool);
	void resetMoveOptimizedValue();
	void sectionForPos(float);
	void setupLayers();
	void shouldExitHackedLevel();
	void spawnGroup(int);
	void spawnGroupTriggered(int, float, int);
	void staticObjectsInRect(cocos2d::CCRect);
	void stopTriggersInGroup(int);
	void testInstantCountTrigger(int, int, int, bool, int);
	void toggleGroup(int, bool);
	void toggleGroupTriggered(int, bool);
	void togglePlayerVisibility(bool);
	void triggerMoveCommand(EffectGameObject*);
	void updateCollisionBlocks();
	void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*);
	void updateCounters(int, int);
	void updateDisabledObjectsLastPos(cocos2d::CCArray*);
	void updateLayerCapacity(gd::string);
	void updateLegacyLayerCapacity(int, int, int, int);
	void updateOBB2(cocos2d::CCRect);
	void updateQueuedLabels();
	virtual void addToGroup(GameObject *,int,bool);
	virtual void addToSection(GameObject *);
	virtual void calculateColorValues(EffectGameObject *,EffectGameObject *,int,float,ColorActionSprite *,GJEffectManager *);
	virtual void createMoveCommand(cocos2d::CCPoint,int,float,int,float,bool,bool,int);
	virtual void flipGravity(PlayerObject *,bool,bool);
	virtual void objectsCollided(int,int);
	virtual void removeFromGroup(GameObject *,int);
	virtual void spawnGroup(int);
	virtual void toggleGroupTriggered(int,bool);
	virtual void updateColor(cocos2d::_ccColor3B,float,int,bool,float,cocos2d::_ccHSVValue,int,bool,int,EffectGameObject *);
}
class GJChallengeDelegate {
	void challengeStatusFailed();
	void challengeStatusFinished();
}
class GJChallengeItem : cocos2d::CCObject {
	void canEncode();
	void create(GJChallengeType, int, int, int, gd::string);
	void create();
	void createFromString(gd::string);
	void createWithCoder(DS_Dictionary*);
	void dataLoaded(DS_Dictionary*);
	void encodeWithCoder(DS_Dictionary*);
	void incrementCount(int);
	void init(GJChallengeType, int, int, int, gd::string);
	void setCount(int);
	virtual void encodeWithCoder(DS_Dictionary *);
}
class GJChestSprite : cocos2d::CCSprite {
	void create(int);
	void init(int);
	void setColor(cocos2d::_ccColor3B const&);
	void switchToState(ChestSpriteState, bool);
	virtual void setColor(cocos2d::_ccColor3B const&);
}
class GJColorSetupLayer : FLAlertLayer, ColorSelectDelegate, FLAlertLayerProtocol {
	void colorSelectClosed(cocos2d::CCNode*);
	void create(LevelSettingsObject*);
	void init(LevelSettingsObject*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onColor(cocos2d::CCObject*);
	void onPage(cocos2d::CCObject*);
	void showPage(int);
	void updateSpriteColor(ColorChannelSprite*, cocos2d::CCLabelBMFont*, int);
	void updateSpriteColors();
	virtual void colorSelectClosed(cocos2d::CCNode *);
}
class GJComment : cocos2d::CCNode {
	void create(cocos2d::CCDictionary*);
	void create();
	void init();
}
class GJCommentListLayer : cocos2d::CCLayerColor {
	void create(BoomListView*, char const*, cocos2d::_ccColor4B, float, float, bool);
	void init(BoomListView*, char const*, cocos2d::_ccColor4B, float, float, bool);
}
class GJDailyLevelDelegate {
	void dailyStatusFailed(bool);
	void dailyStatusFinished(bool);
	virtual void dailyStatusFailed(bool);
	virtual void dailyStatusFinished(bool);
}
class GJDropDownLayer : cocos2d::CCLayerColor {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(char const*);
	void create(char const*, float);
	void customSetup();
	void disableUI();
	void draw();
	void enableUI();
	void enterAnimFinished();
	void enterLayer();
	void exitLayer(cocos2d::CCObject*);
	void hideLayer(bool);
	void init(char const*);
	void init(char const*, float);
	void keyBackClicked();
	void layerHidden();
	void layerVisible();
	void registerWithTouchDispatcher();
	void showLayer(bool);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void exitLayer(cocos2d::CCObject *);
	virtual void hideLayer(bool);
	virtual void showLayer(bool);
}
class GJDropDownLayerDelegate {
	void dropDownLayerWillClose(GJDropDownLayer*);
	virtual void dropDownLayerWillClose(GJDropDownLayer *);
}
class GJEffectManager : cocos2d::CCNode {
	void activeColorForIndex(int);
	void activeOpacityForIndex(int);
	void addAllInheritedColorActions(cocos2d::CCArray*);
	void addGroupPulseEffect(PulseEffectAction*);
	void calculateBaseActiveColors();
	void calculateInheritedColor(int, ColorAction*);
	void calculateLightBGColor(cocos2d::_ccColor3B);
	void colorActionChanged(ColorAction*);
	void colorExists(int);
	void colorForEffect(cocos2d::_ccColor3B, cocos2d::_ccHSVValue);
	void colorForGroupID(int, cocos2d::_ccColor3B const&, bool);
	void colorForIndex(int);
	void colorForPulseEffect(cocos2d::_ccColor3B const&, PulseEffectAction*);
	void countChangedForItem(int);
	void countForItem(int);
	void create();
	void createFollowCommand(float, float, float, int, int, bool, int);
	void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int);
	void createPlayerFollowCommand(float, float, int, float, float, int, int);
	void createRotateCommand(int, float, int, int, int, float, bool, int);
	void decrementCountForItem(int);
	void getAllColorActions();
	void getAllColorSprites();
	void getColorAction(int);
	void getColorSprite(int);
	void getCurrentStateString();
	void getLoadedMoveOffset();
	void getMixedColor(cocos2d::_ccColor3B, cocos2d::_ccColor3B, float);
	void getOpacityActionForGroup(int);
	void getSaveString();
	void handleObjectCollision(bool, int, int);
	void hasActiveDualTouch();
	void hasBeenTriggered(int);
	void hasPulseEffectForGroupID(int);
	void incrementCountForItem(int);
	void init();
	void isGroupEnabled(int);
	void keyForGroupIDColor(int, cocos2d::_ccColor3B const&, bool);
	void loadState(gd::string);
	void objectsCollided(int, int);
	void opacityForIndex(int);
	void opacityModForGroup(int);
	void playerButton(bool, bool);
	void playerDied();
	void postCollisionCheck();
	void preCollisionCheck();
	void prepareMoveActions(float, bool);
	void processColors();
	void processCopyColorPulseActions();
	void processInheritedColors();
	void processPulseActions();
	void registerCollisionTrigger(int, int, int, bool, bool, int);
	void removeAllPulseActions();
	void removeColorAction(int);
	void reset();
	void resetColorCache();
	void resetEffects();
	void resetMoveActions();
	void resetToggledGroups();
	void resetTriggeredIDs();
	void runCountTrigger(int, int, bool, int, bool, int);
	void runDeathTrigger(int, bool, int);
	void runOpacityActionOnGroup(int, float, float, int);
	void runPulseEffect(int, bool, float, float, float, PulseEffectType, cocos2d::_ccColor3B, cocos2d::_ccHSVValue, int, bool, bool, bool, int);
	void runTouchTriggerCommand(int, bool, TouchTriggerType, bool, int);
	void setColorAction(ColorAction*, int);
	void setFollowing(int, int, bool);
	void setupFromString(gd::string);
	void shouldBlend(int);
	void spawnGroup(int, float, int);
	void stopActionsForTrigger(EffectGameObject*);
	void stopMoveActionsForGroup(int);
	void storeTriggeredID(int);
	void toggleGroup(int, bool);
	void traverseInheritanceChain(InheritanceNode*);
	void updateActiveOpacityEffects();
	void updateColorAction(ColorAction*);
	void updateColorEffects(float);
	void updateColors(cocos2d::_ccColor3B, cocos2d::_ccColor3B);
	void updateEffects(float);
	void updateOpacityAction(OpacityEffectAction*);
	void updateOpacityEffects(float);
	void updatePulseEffects(float);
	void updateSpawnTriggers(float);
	void wasFollowing(int, int);
	void wouldCreateLoop(InheritanceNode*, int);
}
class GJFlyGroundLayer : GJGroundLayer {
	void create();
	void init();
}
class GJFollowCommandLayer : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onFollowRotation(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void onUpdateGroupID(cocos2d::CCObject*);
	void onUpdateGroupID2(cocos2d::CCObject*);
	void sliderChanged(cocos2d::CCObject*);
	void sliderXModChanged(cocos2d::CCObject*);
	void sliderYModChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateDurLabel(bool);
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
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class GJFriendRequest : cocos2d::CCNode {
	void create(cocos2d::CCDictionary*);
	void create();
	void init();
}
class GJGameLevel : cocos2d::CCNode {
	void areCoinsVerified();
	void canEncode();
	void create(cocos2d::CCDictionary*, bool);
	void create();
	void createWithCoder(DS_Dictionary*);
	void dataLoaded(DS_Dictionary*);
	void demonIconForDifficulty(DemonDifficultyType);
	void encodeWithCoder(DS_Dictionary*);
	void getAudioFileName();
	void getAverageDifficulty();
	void getCoinKey(int);
	void getLastBuildPageForTab(int);
	void getLengthKey(int);
	void getLevelName();
	void getNormalPercent();
	void getSongName();
	void getUnpackedLevelDescription();
	void handleStatsConflict(GJGameLevel*);
	void init();
	void lengthKeyToString(int);
	void levelWasAltered();
	void levelWasSubmitted();
	void savePercentage(int, bool, int, int, bool);
	void setAccountID(int);
	void setAttemptTime(int);
	void setAttempts(int);
	void setCapacityString(gd::string);
	void setClicks(int);
	void setCoinsVerified(int);
	void setDailyID(int);
	void setDemon(int);
	void setJumps(int);
	void setLastBuildPageForTab(int, int);
	void setLastBuildSave(cocos2d::CCDictionary*);
	void setLevelID(int);
	void setLevelName(gd::string);
	void setNewNormalPercent(int);
	void setNewNormalPercent2(int);
	void setNormalPercent(int);
	void setPassword(int);
	void setStars(int);
	void unverifyCoins();
	virtual void encodeWithCoder(DS_Dictionary *);
}
class GJGarageLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, GameRateDelegate, ListButtonBarDelegate, DialogDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void achievementForUnlock(int, UnlockType);
	void checkBall(int);
	void checkBird(int);
	void checkColor(int, bool);
	void checkDart(int);
	void checkDeathEffect(int);
	void checkIcon(int);
	void checkRobot(int);
	void checkShip(int);
	void checkSpecial(int);
	void checkSpider(int);
	void descriptionForUnlock(int, UnlockType);
	void dialogClosed(DialogLayer*);
	void getLockFrame(int, UnlockType);
	void init();
	void keyBackClicked();
	void listButtonBarSwitchedPage(ListButtonBar*, int);
	void node();
	void onBack(cocos2d::CCObject*);
	void onBallIcon(cocos2d::CCObject*);
	void onBirdIcon(cocos2d::CCObject*);
	void onDartIcon(cocos2d::CCObject*);
	void onDeathEffect(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onKeys(cocos2d::CCObject*);
	void onPlayerColor1(cocos2d::CCObject*);
	void onPlayerColor2(cocos2d::CCObject*);
	void onPlayerIcon(cocos2d::CCObject*);
	void onRobotIcon(cocos2d::CCObject*);
	void onSelectTab(cocos2d::CCObject*);
	void onShards(cocos2d::CCObject*);
	void onShipIcon(cocos2d::CCObject*);
	void onShop(cocos2d::CCObject*);
	void onSpecialIcon(cocos2d::CCObject*);
	void onSpiderIcon(cocos2d::CCObject*);
	void playRainbowEffect();
	void playShadowEffect();
	void scene();
	void selectPage(IconType);
	void setupColorSelect();
	void setupIconSelect();
	void showUnlockPopup(int, UnlockType);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void toggleGlow();
	void updateColorSelect(cocos2d::CCNode*, bool);
	void updatePlayerColors();
	void updatePlayerName(char const*);
	void updatePlayerSelect(cocos2d::CCNode*);
	void updateRate();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void dialogClosed(DialogLayer *);
	virtual void listButtonBarSwitchedPage(ListButtonBar *,int);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
}
class GJGroundLayer : cocos2d::CCLayer {
	void create(int, int);
	void createLine(int);
	void deactivateGround();
	void draw();
	void fadeInFinished();
	void fadeInGround(float);
	void fadeOutGround(float);
	void getGroundY();
	void hideShadows();
	void init(int, int);
	void loadGroundSprites(int, bool);
	void positionGround(float);
	void showGround();
	void updateGround01Color(cocos2d::_ccColor3B);
	void updateGround02Color(cocos2d::_ccColor3B);
	void updateGroundPos(cocos2d::CCPoint);
	void updateGroundWidth();
	void updateLineBlend(bool);
	virtual void fadeInGround(float);
	virtual void fadeOutGround(float);
}
class GJHttpResult : cocos2d::CCNode {
	void create(bool, gd::string, gd::string, GJHttpType);
	void init(bool, gd::string, gd::string, GJHttpType);
}
class GJItemIcon : cocos2d::CCSprite {
	void create(UnlockType, int, cocos2d::_ccColor3B, cocos2d::_ccColor3B, bool, bool, bool, cocos2d::_ccColor3B);
	void createBrowserItem(UnlockType, int);
	void createStoreItem(UnlockType, int, bool, cocos2d::_ccColor3B);
	void init(UnlockType, int, cocos2d::_ccColor3B, cocos2d::_ccColor3B, bool, bool, bool, cocos2d::_ccColor3B);
	void setOpacity(unsigned char);
	virtual void setOpacity(uchar);
}
class GJLevelScoreCell : TableViewCell {
	void draw();
	void init();
	void loadFromScore(GJUserScore*);
	void onViewProfile(cocos2d::CCObject*);
	void updateBGColor(int);
}
class GJListLayer : cocos2d::CCLayerColor {
	void create(BoomListView*, char const*, cocos2d::_ccColor4B, float, float);
	void init(BoomListView*, char const*, cocos2d::_ccColor4B, float, float);
}
class GJMapPack : cocos2d::CCNode {
	void completedMaps();
	void create(cocos2d::CCDictionary*);
	void create();
	void hasCompletedMapPack();
	void init();
	void parsePackColors(gd::string, gd::string);
	void parsePackLevels(gd::string);
	void totalMaps();
}
class GJMessageCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void draw();
	void init();
	void loadFromMessage(GJUserMessage*);
	void markAsRead();
	void onClosePopup(UploadActionPopup*);
	void onDeleteMessage(cocos2d::CCObject*);
	void onToggle(cocos2d::CCObject*);
	void onViewMessage(cocos2d::CCObject*);
	void onViewProfile(cocos2d::CCObject*);
	void updateBGColor(int);
	void updateToggle();
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class GJMessagePopup : FLAlertLayer, UploadActionDelegate, UploadPopupDelegate, FLAlertLayerProtocol, DownloadMessageDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void blockUser();
	void create(GJUserMessage*);
	void downloadMessageFailed(int);
	void downloadMessageFinished(GJUserMessage*);
	void init(GJUserMessage*);
	void keyBackClicked();
	void loadFromGJMessage(GJUserMessage*);
	void onBlock(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onRemove(cocos2d::CCObject*);
	void onReply(cocos2d::CCObject*);
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void downloadMessageFailed(int);
	virtual void downloadMessageFinished(GJUserMessage *);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class GJMoreGamesLayer : GJDropDownLayer {
	void create();
	void customSetup();
	void getMoreGamesList();
}
class GJMoveCommandLayer : FLAlertLayer, TextInputDelegate, ConfigureValuePopupDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onEasing(cocos2d::CCObject*);
	void onEasingRate(cocos2d::CCObject*);
	void onLockPlayerX(cocos2d::CCObject*);
	void onLockPlayerY(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetMode(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void onUpdateGroupID(cocos2d::CCObject*);
	void onUpdateGroupID2(cocos2d::CCObject*);
	void onUseMoveTarget(cocos2d::CCObject*);
	void posFromSliderValue(float);
	void sliderChanged(cocos2d::CCObject*);
	void sliderValueFromPos(int);
	void sliderXChanged(cocos2d::CCObject*);
	void sliderYChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void toggleEasingRateVisibility();
	void updateDurLabel(bool);
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
	void valuePopupClosed(ConfigureValuePopup*, float);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
	virtual void valuePopupClosed(ConfigureValuePopup *,float);
}
class GJObjectDecoder : cocos2d::CCNode, ObjectDecoderDelegate {
	void getDecodedObject(int, DS_Dictionary*);
	void init();
	void sharedDecoder();
	virtual void getDecodedObject(int,DS_Dictionary *);
}
class GJOptionsLayer : FLAlertLayer {
	void addToggle(char const*, char const*, char const*);
	void countForPage(int);
	void create();
	void goToPage(int);
	void incrementCountForPage(int);
	void infoKey(int);
	void init();
	void keyBackClicked();
	void layerForPage(int);
	void layerKey(int);
	void nextPosition(int);
	void objectKey(int);
	void objectsForPage(int);
	void onClose(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onToggle(cocos2d::CCObject*);
	void pageKey(int);
	void setupOptions();
	virtual void onClose(cocos2d::CCObject *);
}
class GJPFollowCommandLayer : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void onUpdateGroupID(cocos2d::CCObject*);
	void sliderChanged(cocos2d::CCObject*);
	void sliderXModChanged(cocos2d::CCObject*);
	void sliderYModChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateDurLabel(bool);
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
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class GJPurchaseDelegate {
	void didPurchaseItem(GJStoreItem*);
	virtual void didPurchaseItem(GJStoreItem *);
}
class GJRequestCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void draw();
	void init();
	void loadFromScore(GJUserScore*);
	void markAsRead();
	void onClosePopup(UploadActionPopup*);
	void onDeleteRequest(cocos2d::CCObject*);
	void onToggle(cocos2d::CCObject*);
	void onViewFriendRequest(cocos2d::CCObject*);
	void onViewProfile(cocos2d::CCObject*);
	void updateBGColor(int);
	void updateToggle();
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class GJRewardDelegate {
	void rewardsStatusFailed();
	void rewardsStatusFinished(int);
	virtual void rewardsStatusFinished(int);
}
class GJRewardItem : cocos2d::CCObject {
	void canEncode();
	void create(int, int, gd::string);
	void create();
	void createSpecial(GJRewardType, int, int, SpecialRewardItem, int, SpecialRewardItem, int, int, int);
	void createWithCoder(DS_Dictionary*);
	void createWithObject(GJRewardType, GJRewardObject*);
	void createWithObjects(GJRewardType, cocos2d::CCArray*);
	void dataLoaded(DS_Dictionary*);
	void encodeWithCoder(DS_Dictionary*);
	void getNextShardType(SpecialRewardItem);
	void getRandomNonMaxShardType();
	void getRandomShardType();
	void getRewardCount(SpecialRewardItem);
	void getRewardObjectForType(SpecialRewardItem);
	void init(int, int, gd::string);
	void isShardType(SpecialRewardItem);
	void rewardItemToStat(SpecialRewardItem);
	void setObjects(cocos2d::CCArray*);
	virtual void encodeWithCoder(DS_Dictionary *);
}
class GJRewardObject : cocos2d::CCObject {
	void canEncode();
	void create(SpecialRewardItem, int, int);
	void create();
	void createItemUnlock(UnlockType, int);
	void createWithCoder(DS_Dictionary*);
	void dataLoaded(DS_Dictionary*);
	void encodeWithCoder(DS_Dictionary*);
	void init(SpecialRewardItem, int, int);
	void isSpecialType();
	virtual void encodeWithCoder(DS_Dictionary *);
}
class GJRobotSprite : CCAnimatedSprite {
	void create();
	void hideGlow();
	void hideSecondary();
	void init(gd::string);
	void init();
	void setOpacity(unsigned char);
	void showGlow();
	void updateColor01(cocos2d::_ccColor3B);
	void updateColor02(cocos2d::_ccColor3B);
	void updateColors();
	void updateFrame(int);
	void updateGlowColor(cocos2d::_ccColor3B, bool);
	virtual void setOpacity(uchar);
}
class GJRotateCommandLayer : FLAlertLayer, TextInputDelegate, ConfigureValuePopupDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onEasing(cocos2d::CCObject*);
	void onEasingRate(cocos2d::CCObject*);
	void onLockRotation(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void onUpdateGroupID(cocos2d::CCObject*);
	void onUpdateGroupID2(cocos2d::CCObject*);
	void sliderChanged(cocos2d::CCObject*);
	void sliderDegreesChanged(cocos2d::CCObject*);
	void sliderTimesChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void toggleEasingRateVisibility();
	void updateCommandDegrees();
	void updateCommandTimes();
	void updateDegreesLabel();
	void updateDurLabel(bool);
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
	void valuePopupClosed(ConfigureValuePopup*, float);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
	virtual void valuePopupClosed(ConfigureValuePopup *,float);
}
class GJRotationControl : cocos2d::CCLayer {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void draw();
	void finishTouch();
	void init();
	void setAngle(float);
	void updateSliderPosition(cocos2d::CCPoint);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
}
class GJRotationControlDelegate {
	void angleChangeBegin();
	void angleChangeEnded();
	void angleChanged(float);
	virtual void angleChanged(float);
}
class GJScaleControl : cocos2d::CCLayer {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void finishTouch();
	void init();
	void loadValues(GameObject*, cocos2d::CCArray*);
	void scaleFromValue(float);
	void sliderChanged(cocos2d::CCObject*);
	void updateLabel(float);
	void valueFromScale(float);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
}
class GJScaleControlDelegate {
	void scaleChangeBegin();
	void scaleChangeEnded();
	void scaleChanged(float);
	virtual void scaleChanged(float);
}
class GJScoreCell : TableViewCell, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void draw();
	void init();
	void loadFromScore(GJUserScore*);
	void onBan(cocos2d::CCObject*);
	void onCheck(cocos2d::CCObject*);
	void onMoreLevels(cocos2d::CCObject*);
	void onViewProfile(cocos2d::CCObject*);
	void updateBGColor(int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
}
class GJSearchObject : cocos2d::CCNode {
	void create(SearchType);
	void create(SearchType, gd::string);
	void create(SearchType, gd::string, gd::string, gd::string, int, bool, bool, bool, int, bool, bool, bool, bool, bool, bool, bool, bool, int, int);
	void createFromKey(char const*);
	void getKey();
	void getNextPageKey();
	void getNextPageObject();
	void getPageObject(int);
	void getPrevPageObject();
	void getSearchKey(SearchType, gd::string, gd::string, gd::string, int, bool, bool, bool, int, bool, bool, bool, bool, bool, bool, bool, bool, int, int);
	void getString();
	void init(SearchType, gd::string, gd::string, gd::string, int, bool, bool, bool, int, bool, bool, bool, bool, bool, bool, bool, bool, int, int);
}
class GJShopLayer : cocos2d::CCLayer, GJPurchaseDelegate {
	void create(ShopType);
	void didPurchaseItem(GJStoreItem*);
	void exitVideoAdItems();
	void init(ShopType);
	void keyBackClicked();
	void onBack(cocos2d::CCObject*);
	void onCommunityCredits(cocos2d::CCObject*);
	void onPlushies(cocos2d::CCObject*);
	void onSelectItem(cocos2d::CCObject*);
	void onVideoAd(cocos2d::CCObject*);
	void scene(ShopType);
	void showVideoAdReward(int);
	void updateCurrencyCounter();
	virtual void didPurchaseItem(GJStoreItem *);
}
class GJSongBrowser : GJDropDownLayer, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(LevelSettingsObject*);
	void customSetup();
	void init(LevelSettingsObject*);
	void loadPage(int);
	void onDeleteAll(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void setupPageInfo(int, int, int);
	void setupSongBrowser(cocos2d::CCArray*);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
}
class GJSpecialColorSelect : FLAlertLayer {
	void create(int, GJSpecialColorSelectDelegate*, bool);
	void getButtonByTag(int);
	void highlightSelected(ButtonSprite*);
	void init(int, GJSpecialColorSelectDelegate*, bool);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSelectColor(cocos2d::CCObject*);
	void textForColorIdx(int);
}
class GJSpecialColorSelectDelegate {
	void colorSelectClosed(GJSpecialColorSelect*, int);
	virtual void colorSelectClosed(GJSpecialColorSelect *,int);
}
class GJSpiderSprite : GJRobotSprite {
	void create();
	void init();
}
class GJSpriteColor : cocos2d::CCNode {
	void create();
	void getColorMode();
	void init();
	void resetCustomColorMode();
	void setCustomColorMode(int);
	void setDefaultColorMode(int);
}
class GJStoreItem : cocos2d::CCNode {
	void create(int, int, int, int, ShopType);
	void init(int, int, int, int, ShopType);
}
class GJUnlockableItem : cocos2d::CCObject {
	void create();
	void init();
}
class GJUserCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void draw();
	void init();
	void loadFromScore(GJUserScore*);
	void onCancelFriendRequest(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onRemoveFriend(cocos2d::CCObject*);
	void onSendMessage(cocos2d::CCObject*);
	void onUnblockUser(cocos2d::CCObject*);
	void onViewFriendRequest(cocos2d::CCObject*);
	void onViewProfile(cocos2d::CCObject*);
	void updateBGColor(int);
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class GJUserMessage : cocos2d::CCNode {
	void create(cocos2d::CCDictionary*);
	void create();
	void init();
}
class GJUserScore : cocos2d::CCNode {
	void create(cocos2d::CCDictionary*);
	void create();
	void init();
	void isCurrentUser();
	void mergeWithScore(GJUserScore*);
}
class GJWorldNode : cocos2d::CCNode {
	void addDotsToLevel(int, bool);
	void create(int, WorldSelectLayer*);
	void dotPositionForLevel(int, int);
	void init(int, WorldSelectLayer*);
	void onLevel(cocos2d::CCObject*);
	void playStep1();
	void playStep2();
	void playStep3();
	void positionForLevelButton(int);
	void unlockActiveItem();
}
class GJWriteMessagePopup : FLAlertLayer, TextInputDelegate, UploadMessageDelegate, UploadPopupDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void closeMessagePopup(bool);
	void create(int, int);
	void init(int, int);
	void keyBackClicked();
	void onClearBody(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onSend(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateBody(gd::string);
	void updateCharCountLabel(int);
	void updateSubject(gd::string);
	void updateText(gd::string, int);
	void uploadMessageFailed(int);
	void uploadMessageFinished(int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
	virtual void uploadMessageFailed(int);
	virtual void uploadMessageFinished(int);
}
class GManager : cocos2d::CCNode {
	void dataLoaded(DS_Dictionary*);
	void encodeDataTo(DS_Dictionary*);
	void firstLoad();
	void getCompressedSaveString();
	void getSaveString();
	void init();
	void load();
	void loadDataFromFile(gd::string);
	void loadFromCompressedString(gd::string);
	void loadFromString(gd::string);
	void save();
	void saveData(DS_Dictionary*, gd::string);
	void saveGMTo(gd::string);
	void setup();
	void tryLoadData(DS_Dictionary*, gd::string);
	virtual void dataLoaded(DS_Dictionary *);
	virtual void encodeDataTo(DS_Dictionary *);
}
class GooglePlayDelegate {
	void googlePlaySignedIn();
}
class GooglePlayManager : cocos2d::CCNode {
	void googlePlaySignedIn();
	void init();
	void sharedState();
}
class GravityEffectSprite : cocos2d::CCSprite {
	void create();
	void draw();
	void init();
	void updateSpritesColor(cocos2d::_ccColor3B);
}
class GroupCommandObject : cocos2d::CCNode {
	void create();
	void createFromString(gd::string);
	void easeToText(int);
	void getEasedAction(cocos2d::CCActionInterval*, int, float);
	void getSaveString();
	void init();
	void resetDelta(bool);
	void runFollowCommand(float, float, float, bool);
	void runMoveCommand(cocos2d::CCPoint, float, int, float, bool, bool);
	void runPlayerFollowCommand(float, float, int, float, float);
	void runRotateCommand(float, float, int, float, bool);
	void step(float);
	void updateTweenAction(float, char const*);
	virtual void updateTweenAction(float,char const*);
}
class HardStreak : cocos2d::CCDrawNode {
	void addPoint(cocos2d::CCPoint);
	void clearBehindXPos(float);
	void create();
	void firstSetup();
	void init();
	void normalizeAngle(double);
	void quadCornerOffset(cocos2d::CCPoint, cocos2d::CCPoint, float);
	void reset();
	void resumeStroke();
	void stopStroke();
	void updateStroke(float);
}
class HSVWidgetPopup : FLAlertLayer {
	void create(cocos2d::_ccHSVValue, HSVWidgetPopupDelegate*, gd::string);
	void init(cocos2d::_ccHSVValue, HSVWidgetPopupDelegate*, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
}
class HSVWidgetPopupDelegate {
	void hsvPopupClosed(HSVWidgetPopup*, cocos2d::_ccHSVValue);
	virtual void hsvPopupClosed(HSVWidgetPopup *,cocos2d::_ccHSVValue);
}
class InfoAlertButton : CCMenuItemSpriteExtra {
	void activate();
	void create(gd::string, gd::string, float);
	void init(gd::string, gd::string, float);
}
class InfoLayer : FLAlertLayer, LevelCommentDelegate, CommentUploadDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void commentUploadFailed(int, CommentError);
	void commentUploadFinished(int);
	void confirmReport(cocos2d::CCObject*);
	void create(GJGameLevel*, GJUserScore*);
	void getSpriteButton(char const*, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float, cocos2d::CCPoint);
	void init(GJGameLevel*, GJUserScore*);
	void isCorrect(char const*);
	void keyBackClicked();
	void loadCommentsFailed(char const*);
	void loadCommentsFinished(cocos2d::CCArray*, char const*);
	void loadPage(int, bool);
	void onClose(cocos2d::CCObject*);
	void onComment(cocos2d::CCObject*);
	void onCopyLevelID(cocos2d::CCObject*);
	void onGetComments(cocos2d::CCObject*);
	void onLevelInfo(cocos2d::CCObject*);
	void onMore(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onOriginal(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onRefreshComments(cocos2d::CCObject*);
	void onSimilar(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void reloadWindow();
	void setupCommentsBrowser(cocos2d::CCArray*);
	void setupLevelInfo();
	void setupPageInfo(gd::string, char const*);
	void show();
	void toggleCommentMode(cocos2d::CCObject*);
	void toggleExtendedMode(cocos2d::CCObject*);
	void toggleSmallCommentMode(cocos2d::CCObject*);
	void updateCommentModeButtons();
	void updateLevelsLabel();
	void updateUserScoreFinished();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void commentUploadFailed(int,CommentError);
	virtual void commentUploadFinished(int);
	virtual void loadCommentsFailed(char const*);
	virtual void loadCommentsFinished(cocos2d::CCArray *,char const*);
	virtual void setupPageInfo(gd::string,char const*);
}
class InheritanceNode : cocos2d::CCObject {
	void create(int, InheritanceNode*);
	void init(int, InheritanceNode*);
}
class InstantSection : cocos2d::CCObject {
	void create(int, int);
	void init(int, int);
}
class KeybindingsLayer : FLAlertLayer {
	void addKeyPair(char const*, char const*);
	void countForPage(int);
	void create();
	void goToPage(int);
	void incrementCountForPage(int);
	void infoKey(int);
	void init();
	void keyBackClicked();
	void layerForPage(int);
	void layerKey(int);
	void nextPosition(int);
	void objectKey(int);
	void objectsForPage(int);
	void onClose(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onToggle(cocos2d::CCObject*);
	void pageKey(int);
}
class KeybindingsManager : cocos2d::CCNode {
	void commandForKey(cocos2d::enumKeyCodes, GJKeyGroup, bool, bool, bool);
	void commandForKeyMods(cocos2d::enumKeyCodes, GJKeyGroup);
	void commandForKeyNoMods(cocos2d::enumKeyCodes, GJKeyGroup);
	void commandToKeyForGroup(GJKeyGroup);
	void dataLoaded(DS_Dictionary*);
	void encodeDataTo(DS_Dictionary*);
	void firstSetup();
	void groupForCommand(GJKeyCommand);
	void init();
	void keyForCommand(GJKeyCommand);
	void keyToCommandForGroup(GJKeyGroup);
	void setCommandToKeyDict(cocos2d::CCDictionary*);
	void setKeyToCommandDict(cocos2d::CCDictionary*);
	void sharedState();
}
class KeysLayer : cocos2d::CCLayer, FLAlertLayerProtocol, DialogDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void dialogClosed(DialogLayer*);
	void goToPage(int);
	void init();
	void keyBackClicked();
	void onBack(cocos2d::CCObject*);
	void onSelectItem(cocos2d::CCObject*);
	void onShop(cocos2d::CCObject*);
	void onShop3(cocos2d::CCObject*);
	void onSpecialItem(cocos2d::CCObject*);
	void onSwitchPage(cocos2d::CCObject*);
	void scene();
	void switchToOpenedState(CCMenuItemSpriteExtra*);
	void updateUnlockedLabel();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void dialogClosed(DialogLayer *);
}
class LabelGameObject : EffectGameObject {
	void addMainSpriteToParent(bool);
	void create();
	void customObjectSetup(gd::map<gd::string, gd::string, gd::less<gd::string >, gd::allocator<gd::pair<gd::string const, gd::string > > >&);
	void getSaveString();
	void init();
	void queueUpdateLabel(gd::string);
	void setObjectColor(cocos2d::_ccColor3B const&);
	void setOpacity(unsigned char);
	void setupCustomSprites();
	void updateLabel(gd::string);
	void updateLabelIfDirty();
	virtual void addMainSpriteToParent(bool);
	virtual void customObjectSetup(gd::map<gd::string,gd::string,gd::less<gd::string>,gd::allocator<gd::pair<gd::string const,gd::string>>> &);
	virtual void setObjectColor(cocos2d::_ccColor3B const&);
	virtual void setOpacity(uchar);
}
class LeaderboardManagerDelegate {
	void loadLeaderboardFailed(char const*);
	void loadLeaderboardFinished(cocos2d::CCArray*, char const*);
	void updateUserScoreFailed();
	void updateUserScoreFinished();
	virtual void loadLeaderboardFailed(char const*);
	virtual void loadLeaderboardFinished(cocos2d::CCArray *,char const*);
}
class LeaderboardsLayer : cocos2d::CCLayer, LeaderboardManagerDelegate {
	void create(LeaderboardState);
	void init(LeaderboardState);
	void isCorrect(char const*);
	void keyBackClicked();
	void loadLeaderboardFailed(char const*);
	void loadLeaderboardFinished(cocos2d::CCArray*, char const*);
	void onBack(cocos2d::CCObject*);
	void onCreators(cocos2d::CCObject*);
	void onGlobal(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onTop(cocos2d::CCObject*);
	void onWeek(cocos2d::CCObject*);
	void refreshTabs();
	void scene(LeaderboardState);
	void selectLeaderboard(LeaderboardState);
	void setupLevelBrowser(cocos2d::CCArray*);
	void setupTabs();
	void toggleTabButtons();
	void updateUserScoreFailed();
	void updateUserScoreFinished();
	virtual void loadLeaderboardFailed(char const*);
	virtual void loadLeaderboardFinished(cocos2d::CCArray *,char const*);
}
class LevelBrowserLayer : cocos2d::CCLayer, LevelManagerDelegate, FLAlertLayerProtocol, SetIDPopupDelegate, SetTextPopupDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(GJSearchObject*);
	void getLevelMatchingSearch(cocos2d::CCArray*, gd::string);
	void init(GJSearchObject*);
	void isCorrect(char const*);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void loadLevelsFailed(char const*);
	void loadLevelsFinished(cocos2d::CCArray*, char const*);
	void loadPage(GJSearchObject*);
	void onBack(cocos2d::CCObject*);
	void onClearSearch(cocos2d::CCObject*);
	void onDeleteAll(cocos2d::CCObject*);
	void onEnter();
	void onEnterTransitionDidFinish();
	void onFavorites(cocos2d::CCObject*);
	void onGoToFolder(cocos2d::CCObject*);
	void onGoToLastPage(cocos2d::CCObject*);
	void onGoToPage(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onMyOnlineLevels(cocos2d::CCObject*);
	void onNew(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onRefresh(cocos2d::CCObject*);
	void onRemoveAllFavorites(cocos2d::CCObject*);
	void onSaved(cocos2d::CCObject*);
	void onSearch(cocos2d::CCObject*);
	void scene(GJSearchObject*);
	void setIDPopupClosed(SetIDPopup*, int);
	void setSearchObject(GJSearchObject*);
	void setTextPopupClosed(SetTextPopup*, gd::string);
	void setupLevelBrowser(cocos2d::CCArray*);
	void setupPageInfo(gd::string, char const*);
	void updateLevelsLabel();
	void updatePageLabel();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void loadLevelsFailed(char const*);
	virtual void loadLevelsFinished(cocos2d::CCArray *,char const*);
	virtual void setIDPopupClosed(SetIDPopup *,int);
	virtual void setTextPopupClosed(SetTextPopup *,gd::string);
	virtual void setupPageInfo(gd::string,char const*);
}
class LevelCell : TableViewCell {
	void create(float, float);
	void draw();
	void init();
	void loadCustomLevelCell();
	void loadFromLevel(GJGameLevel*);
	void loadLocalLevelCell();
	void onClick(cocos2d::CCObject*);
	void onViewProfile(cocos2d::CCObject*);
	void updateBGColor(int);
}
class LevelCommentDelegate {
	void loadCommentsFailed(char const*);
	void loadCommentsFinished(cocos2d::CCArray*, char const*);
	void setupPageInfo(gd::string, char const*);
	void updateUserScoreFinished();
	virtual void loadCommentsFailed(char const*);
	virtual void loadCommentsFinished(cocos2d::CCArray *,char const*);
	virtual void setupPageInfo(gd::string,char const*);
}
class LevelDeleteDelegate {
	void levelDeleteFailed(int);
	void levelDeleteFinished(int);
	virtual void levelDeleteFailed(int);
	virtual void levelDeleteFinished(int);
}
class LevelDownloadDelegate {
	void levelDownloadFailed(int);
	void levelDownloadFinished(GJGameLevel*);
	virtual void levelDownloadFailed(int);
	virtual void levelDownloadFinished(GJGameLevel *);
}
class LevelEditorLayer : GJBaseGameLayer, LevelSettingsDelegate {
	void activateTriggerEffect(EffectGameObject*, float, float, float);
	void addObjectFromString(gd::string);
	void addObjectsRecursiveFromArray(cocos2d::CCArray*, int);
	void addRelatedStickyObjects(cocos2d::CCArray*);
	void addSpecial(GameObject*);
	void addToGroup(GameObject*, int, bool);
	void addToRedoList(UndoObject*);
	void addToUndoList(UndoObject*, bool);
	void animateInDualGround(GameObject*, float, bool);
	void assignNewStickyGroups(cocos2d::CCArray*);
	void breakApartTextObject(GameObject*);
	void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*);
	void canPasteState();
	void checkCollisions(PlayerObject*, float);
	void copyObjectState(GameObject*);
	void create(GJGameLevel*);
	void createBackground();
	void createGroundLayer();
	void createObject(int, cocos2d::CCPoint, bool);
	void createObjectsFromSetup(gd::string);
	void createObjectsFromString(gd::string, bool);
	void dirtifyTriggers();
	void draw();
	void enterDualMode(GameObject*, bool);
	void fastUpdateDisabledGroups();
	void flipGravity(PlayerObject*, bool, bool);
	void fullUpdateDisabledGroups();
	void getAllObjects();
	void getGridPos(cocos2d::CCPoint);
	void getLastObjectX();
	void getLevelString();
	void getNextColorChannel();
	void getNextFreeBlockID(cocos2d::CCArray*);
	void getNextFreeGroupID(cocos2d::CCArray*);
	void getNextFreeItemID(cocos2d::CCArray*);
	void getObjectRect(GameObject*, bool);
	void getOtherPlayer(PlayerObject*);
	void getRelativeOffset(GameObject*);
	void getSectionCount();
	void getStickyGroup(int);
	void getTriggerGroup(int);
	void groupStickyObjects(cocos2d::CCArray*);
	void handleAction(bool, cocos2d::CCArray*);
	void hasAction(bool);
	void init(GJGameLevel*);
	void isLayerLocked(int);
	void levelSettingsUpdated();
	void objectAtPosition(cocos2d::CCPoint);
	void objectMoved(GameObject*);
	void objectsAtPosition(cocos2d::CCPoint);
	void objectsInRect(cocos2d::CCRect, bool);
	void onPausePlaytest();
	void onPlaytest();
	void onResumePlaytest();
	void onStopPlaytest();
	void pasteAtributeState(GameObject*, cocos2d::CCArray*);
	void pasteColorState(GameObject*, cocos2d::CCArray*);
	void playMusic();
	void playerWillSwitchMode(PlayerObject*, GameObject*);
	void processLoadedMoveActions();
	void processMoveActionsStepOld();
	void recreateGroups();
	void redoLastAction();
	void removeAllObjects();
	void removeAllObjectsOfType(int);
	void removeBackground();
	void removeFromGroup(GameObject*, int);
	void removeGroundLayer();
	void removeObject(GameObject*, bool);
	void removePlayer2();
	void removeSpecial(GameObject*);
	void resetEffectTriggerOptim(GameObject*, cocos2d::CCArray*);
	void resetMovingObjects();
	void resetObjectVector();
	void resetToggledGroups();
	void resetToggledGroupsAndObjects();
	void resetUnusedColorChannels();
	void rotationForSlopeNearObject(GameObject*);
	void runColorEffect(EffectGameObject*, int, float, float, bool);
	void scene(GJGameLevel*);
	void setObjectCount(int);
	void setStartPosObject(StartPosObject*);
	void setupLevelStart(LevelSettingsObject*);
	void shouldBlend(int);
	void sortBatchnodeChildren(float);
	void sortStickyGroups();
	void spawnGroupPreview(int, float, float, float, float, float);
	void spawnPlayer2();
	void stopTriggersInGroup(int, float);
	void switchToFlyMode(PlayerObject*, GameObject*, bool, int);
	void switchToRobotMode(PlayerObject*, GameObject*, bool);
	void switchToRollMode(PlayerObject*, GameObject*, bool);
	void switchToSpiderMode(PlayerObject*, GameObject*, bool);
	void timeForXPos(float);
	void toggleBackground(bool);
	void toggleDualMode(GameObject*, bool, PlayerObject*, bool);
	void toggleGrid(bool);
	void toggleGround(bool);
	void toggleGroupPreview(int, bool);
	void toggleLockActiveLayer();
	void transferDefaultColors(GJEffectManager*, GJEffectManager*);
	void triggerFollowCommand(EffectGameObject*);
	void triggerPlayerFollowCommand(EffectGameObject*);
	void triggerRotateCommand(EffectGameObject*);
	void typeExistsAtPosition(int, cocos2d::CCPoint, bool, bool, float);
	void undoLastAction();
	void ungroupStickyObjects(cocos2d::CCArray*);
	void unlockAllLayers();
	void update(float);
	void updateBGAndGColors();
	void updateBlendValues();
	void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*);
	void updateDualGround(PlayerObject*, int, bool);
	void updateEditorMode();
	void updateGameObjects();
	void updateGameObjectsNew();
	void updateGround(float);
	void updateGroundWidth();
	void updateLevelColors();
	void updateLevelFont(int);
	void updateOptions();
	void updateTimeMod(float, bool);
	void updateToggledGroups();
	void updateVisibility(float);
	void validGroup(GameObject*, bool);
	void willSwitchToMode(int, PlayerObject*);
	void xPosForTime(float);
	virtual void addToGroup(GameObject *,int,bool);
	virtual void calculateColorValues(EffectGameObject *,EffectGameObject *,int,float,ColorActionSprite *,GJEffectManager *);
	virtual void flipGravity(PlayerObject *,bool,bool);
	virtual void removeFromGroup(GameObject *,int);
	virtual void timeForXPos(float);
	virtual void update(float);
	virtual void updateColor(cocos2d::_ccColor3B,float,int,bool,float,cocos2d::_ccHSVValue,int,bool,int,EffectGameObject *);
	virtual void xPosForTime(float);
}
class LevelFeatureLayer : FLAlertLayer {
	void create(int);
	void init(int);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onDown(cocos2d::CCObject*);
	void onDown2(cocos2d::CCObject*);
	void onRemoveValues(cocos2d::CCObject*);
	void onSetEpicOnly(cocos2d::CCObject*);
	void onSetFeatured(cocos2d::CCObject*);
	void onToggleEpic(cocos2d::CCObject*);
	void onUp(cocos2d::CCObject*);
	void onUp2(cocos2d::CCObject*);
	void updateStars();
}
class LevelInfoLayer : cocos2d::CCLayer, LevelDownloadDelegate, LevelUpdateDelegate, RateLevelDelegate, LikeItemDelegate, FLAlertLayerProtocol, LevelDeleteDelegate, NumberInputDelegate, SetIDPopupDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void confirmClone(cocos2d::CCObject*);
	void confirmDelete(cocos2d::CCObject*);
	void confirmMoveToBottom(cocos2d::CCObject*);
	void confirmMoveToTop(cocos2d::CCObject*);
	void confirmOwnerDelete(cocos2d::CCObject*);
	void create(GJGameLevel*);
	void downloadLevel();
	void incrementDislikes();
	void incrementLikes();
	void init(GJGameLevel*);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void levelDeleteFailed(int);
	void levelDeleteFinished(int);
	void levelDownloadFailed(int);
	void levelDownloadFinished(GJGameLevel*);
	void levelUpdateFailed(int);
	void levelUpdateFinished(GJGameLevel*, UpdateResponse);
	void likedItem(LikeItemType, int, bool);
	void numberInputClosed(NumberInputLayer*);
	void onBack(cocos2d::CCObject*);
	void onClone(cocos2d::CCObject*);
	void onDelete(cocos2d::CCObject*);
	void onFavorite(cocos2d::CCObject*);
	void onFeatured(cocos2d::CCObject*);
	void onGarage(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onLevelInfo(cocos2d::CCObject*);
	void onLevelLeaderboard(cocos2d::CCObject*);
	void onLike(cocos2d::CCObject*);
	void onLowDetailMode(cocos2d::CCObject*);
	void onOwnerDelete(cocos2d::CCObject*);
	void onPlay(cocos2d::CCObject*);
	void onPlayReplay(cocos2d::CCObject*);
	void onRate(cocos2d::CCObject*);
	void onRateDemon(cocos2d::CCObject*);
	void onRateStars(cocos2d::CCObject*);
	void onRateStarsMod(cocos2d::CCObject*);
	void onSetFolder(cocos2d::CCObject*);
	void onUpdate(cocos2d::CCObject*);
	void onViewProfile(cocos2d::CCObject*);
	void playStep2();
	void playStep3();
	void rateLevelClosed();
	void scene(GJGameLevel*);
	void setIDPopupClosed(SetIDPopup*, int);
	void setupLevelInfo();
	void setupProgressBars();
	void shouldDownloadLevel();
	void showSongWarning();
	void showUpdateAlert(UpdateResponse);
	void tryCloneLevel(cocos2d::CCObject*);
	void tryShowAd();
	void updateLabelValues();
	void updateSideButtons();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void levelDeleteFailed(int);
	virtual void levelDeleteFinished(int);
	virtual void levelDownloadFailed(int);
	virtual void levelDownloadFinished(GJGameLevel *);
	virtual void levelUpdateFailed(int);
	virtual void levelUpdateFinished(GJGameLevel *,UpdateResponse);
	virtual void likedItem(LikeItemType,int,bool);
	virtual void numberInputClosed(NumberInputLayer *);
	virtual void setIDPopupClosed(SetIDPopup *,int);
}
class LevelLeaderboard : FLAlertLayer, LeaderboardManagerDelegate {
	void create(GJGameLevel*, LevelLeaderboardType);
	void init(GJGameLevel*, LevelLeaderboardType);
	void isCorrect(char const*);
	void keyBackClicked();
	void loadLeaderboardFailed(char const*);
	void loadLeaderboardFinished(cocos2d::CCArray*, char const*);
	void loadScores();
	void onChangeType(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onUpdate(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void setupLeaderboard(cocos2d::CCArray*);
	void show();
	void updateUserScoreFailed();
	void updateUserScoreFinished();
	virtual void loadLeaderboardFailed(char const*);
	virtual void loadLeaderboardFinished(cocos2d::CCArray *,char const*);
}
class LevelManagerDelegate {
	void loadLevelsFailed(char const*);
	void loadLevelsFinished(cocos2d::CCArray*, char const*);
	void setupPageInfo(gd::string, char const*);
	virtual void loadLevelsFailed(char const*);
	virtual void loadLevelsFinished(cocos2d::CCArray *,char const*);
	virtual void setupPageInfo(gd::string,char const*);
}
class LevelPage : cocos2d::CCLayer, DialogDelegate {
	void addSecretCoin();
	void addSecretDoor();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(GJGameLevel*);
	void dialogClosed(DialogLayer*);
	void init(GJGameLevel*);
	void onInfo(cocos2d::CCObject*);
	void onPlay(cocos2d::CCObject*);
	void onSecretDoor(cocos2d::CCObject*);
	void playCoinEffect();
	void playStep2();
	void playStep3();
	void registerWithTouchDispatcher();
	void updateDynamicPage(GJGameLevel*);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void dialogClosed(DialogLayer *);
}
class LevelSearchLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DemonFilterDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void checkDiff(int);
	void checkTime(int);
	void clearFilters();
	void confirmClearFilters(cocos2d::CCObject*);
	void create();
	void demonFilterSelectClosed(int);
	void getDiffKey(int);
	void getLevelLenKey();
	void getSearchDiffKey();
	void getSearchObject(SearchType, gd::string);
	void getTimeKey(int);
	void init();
	void keyBackClicked();
	void onBack(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onFollowed(cocos2d::CCObject*);
	void onFriends(cocos2d::CCObject*);
	void onLatestStars(cocos2d::CCObject*);
	void onMagic(cocos2d::CCObject*);
	void onMoreOptions(cocos2d::CCObject*);
	void onMostDownloaded(cocos2d::CCObject*);
	void onMostLikes(cocos2d::CCObject*);
	void onMostRecent(cocos2d::CCObject*);
	void onSearch(cocos2d::CCObject*);
	void onSearchUser(cocos2d::CCObject*);
	void onSpecialDemon(cocos2d::CCObject*);
	void onStarAward(cocos2d::CCObject*);
	void onTrending(cocos2d::CCObject*);
	void scene();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void toggleDifficulty(cocos2d::CCObject*);
	void toggleDifficultyNum(int, bool);
	void toggleStar(cocos2d::CCObject*);
	void toggleTime(cocos2d::CCObject*);
	void toggleTimeNum(int, bool);
	void updateSearchLabel(char const*);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void demonFilterSelectClosed(int);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
}
class LevelSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate, DynamicScrollDelegate {
	void colorForPage(int);
	void create(int);
	void getColorValue(int, int, float);
	void init(int);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void onBack(cocos2d::CCObject*);
	void onDownload(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onNext(cocos2d::CCObject*);
	void onPlay(cocos2d::CCObject*);
	void onPrev(cocos2d::CCObject*);
	void scene(int);
	void scrollLayerMoved(cocos2d::CCPoint);
	void tryShowAd();
	void updatePageWithObject(cocos2d::CCObject*, cocos2d::CCObject*);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void scrollLayerMoved(cocos2d::CCPoint);
	virtual void updatePageWithObject(cocos2d::CCObject *,cocos2d::CCObject *);
}
class LevelSettingsDelegate {
	void levelSettingsUpdated();
}
class LevelSettingsLayer : FLAlertLayer, ColorSelectDelegate, SelectArtDelegate, FLAlertLayerProtocol, CustomSongLayerDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void audioNext(cocos2d::CCObject*);
	void audioPrevious(cocos2d::CCObject*);
	void colorSelectClosed(cocos2d::CCNode*);
	void create(LevelSettingsObject*, LevelEditorLayer*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void customSongLayerClosed();
	void init(LevelSettingsObject*, LevelEditorLayer*);
	void keyBackClicked();
	void on3DLine(cocos2d::CCObject*);
	void onBG(cocos2d::CCObject*);
	void onBGArt(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onCol(cocos2d::CCObject*);
	void onDualMode(cocos2d::CCObject*);
	void onG(cocos2d::CCObject*);
	void onG2(cocos2d::CCObject*);
	void onGArt(cocos2d::CCObject*);
	void onGravityFlipped(cocos2d::CCObject*);
	void onLine(cocos2d::CCObject*);
	void onLiveEdit(cocos2d::CCObject*);
	void onMiniMode(cocos2d::CCObject*);
	void onObj(cocos2d::CCObject*);
	void onOpenCustomSong(cocos2d::CCObject*);
	void onSelectFont(cocos2d::CCObject*);
	void onSelectMode(cocos2d::CCObject*);
	void onSelectSpeed(cocos2d::CCObject*);
	void onSongMode(cocos2d::CCObject*);
	void onTwoPlayerMode(cocos2d::CCObject*);
	void onTwoPlayerModeInfo(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void selectArtClosed(SelectArtLayer*);
	void selectSong(int);
	void showCustomSongSelect();
	void showPicker(int, ColorAction*);
	void updateAudioLabel();
	void updateColorSprite(ColorChannelSprite*);
	void updateColorSprites();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void colorSelectClosed(cocos2d::CCNode *);
	virtual void selectArtClosed(SelectArtLayer *);
}
class LevelSettingsObject : cocos2d::CCNode {
	void create();
	void getSaveString();
	void init();
	void objectFromDict(cocos2d::CCDictionary*);
	void objectFromString(gd::string);
	void offsetMusic();
	void setupColorsFromLegacyMode(cocos2d::CCDictionary*);
}
class LevelUpdateDelegate {
	void levelUpdateFailed(int);
	void levelUpdateFinished(GJGameLevel*, UpdateResponse);
	virtual void levelUpdateFailed(int);
	virtual void levelUpdateFinished(GJGameLevel *,UpdateResponse);
}
class LevelUploadDelegate {
	void levelUploadFailed(GJGameLevel*);
	void levelUploadFinished(GJGameLevel*);
	virtual void levelUploadFailed(GJGameLevel *);
	virtual void levelUploadFinished(GJGameLevel *);
}
class LikeItemDelegate {
	void likedItem(LikeItemType, int, bool);
	virtual void likedItem(LikeItemType,int,bool);
}
class LikeItemLayer : FLAlertLayer {
	void create(LikeItemType, int, int);
	void init(LikeItemType, int, int);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onDislike(cocos2d::CCObject*);
	void onLike(cocos2d::CCObject*);
	void triggerLike(bool);
}
class ListButtonBar : cocos2d::CCNode {
	void create(cocos2d::CCArray*, cocos2d::CCPoint, int, int, float, float, float, float, int);
	void getPage();
	void goToPage(int);
	void init(cocos2d::CCArray*, cocos2d::CCPoint, int, int, float, float, float, float, int);
	void onLeft(cocos2d::CCObject*);
	void onRight(cocos2d::CCObject*);
}
class ListButtonBarDelegate {
	void listButtonBarSwitchedPage(ListButtonBar*, int);
	virtual void listButtonBarSwitchedPage(ListButtonBar *,int);
}
class ListButtonPage : cocos2d::CCLayer {
	void create(cocos2d::CCArray*, cocos2d::CCPoint, int, int, float, float, float);
	void init(cocos2d::CCArray*, cocos2d::CCPoint, int, int, float, float, float);
}
class ListCell : TableViewCell {
	void draw();
	void init();
	void loadFromObject(cocos2d::CCObject*, int, int, int);
	void updateBGColor(int);
}
class LoadingCircle : cocos2d::CCLayerColor {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void draw();
	void fadeAndRemove();
	void init();
	void registerWithTouchDispatcher();
	void show();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
}
class LoadingLayer : cocos2d::CCLayer {
	void create(bool);
	void getLoadingString();
	void init(bool);
	void loadAssets();
	void loadingFinished();
	void scene(bool);
	void updateProgress(int);
}
class LocalLevelManager : GManager {
	void dataLoaded(DS_Dictionary*);
	void encodeDataTo(DS_Dictionary*);
	void firstLoad();
	void getAllLevelsInDict();
	void getAllLevelsWithName(gd::string);
	void getCreatedLevels(int);
	void getLevelsInNameGroups();
	void getLoadString(int);
	void getMainLevelString(int);
	void init();
	void markLevelsAsUnmodified();
	void moveLevelToTop(GJGameLevel*);
	void reorderLevels();
	void setLocalLevels(cocos2d::CCArray*);
	void sharedState();
	void updateLevelOrder();
	void updateLevelRevision();
	virtual void dataLoaded(DS_Dictionary *);
	virtual void encodeDataTo(DS_Dictionary *);
}
class MapPackCell : TableViewCell {
	void draw();
	void init();
	void loadFromMapPack(GJMapPack*);
	void onClaimReward(cocos2d::CCObject*);
	void onClick(cocos2d::CCObject*);
	void playCompleteEffect();
	void reloadCell();
	void updateBGColor(int);
}
class MenuGameLayer : cocos2d::CCLayer {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void destroyPlayer();
	void getBGColor(int);
	void init();
	void registerWithTouchDispatcher();
	void resetPlayer();
	void tryJump(float);
	void update(float);
	void updateColor(float);
	void updateColors();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void update(float);
}
class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol, GooglePlayDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void endGame();
	void googlePlaySignedIn();
	void init();
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void node();
	void onAchievements(cocos2d::CCObject*);
	void onCreator(cocos2d::CCObject*);
	void onDaily(cocos2d::CCObject*);
	void onEveryplay(cocos2d::CCObject*);
	void onFacebook(cocos2d::CCObject*);
	void onFreeLevels(cocos2d::CCObject*);
	void onFullVersion(cocos2d::CCObject*);
	void onGameCenter(cocos2d::CCObject*);
	void onGarage(cocos2d::CCObject*);
	void onGooglePlayGames(cocos2d::CCObject*);
	void onMoreGames(cocos2d::CCObject*);
	void onMyProfile(cocos2d::CCObject*);
	void onNewgrounds(cocos2d::CCObject*);
	void onOptions(cocos2d::CCObject*);
	void onOptionsInstant();
	void onPlay(cocos2d::CCObject*);
	void onQuit(cocos2d::CCObject*);
	void onRobTop(cocos2d::CCObject*);
	void onStats(cocos2d::CCObject*);
	void onTrailer(cocos2d::CCObject*);
	void onTwitter(cocos2d::CCObject*);
	void onYouTube(cocos2d::CCObject*);
	void openOptions(bool);
	void scene(bool);
	void showGCQuestion();
	void showMeltdownPromo();
	void syncPlatformAchievements(float);
	void tryShowAd(float);
	void updateUserProfileButton();
	void willClose();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void keyDown(cocos2d::enumKeyCodes);
}
class MessageListDelegate {
	void forceReloadMessages(bool);
	void loadMessagesFailed(char const*, GJErrorCode);
	void loadMessagesFinished(cocos2d::CCArray*, char const*);
	void setupPageInfo(gd::string, char const*);
	virtual void forceReloadMessages(bool);
	virtual void loadMessagesFailed(char const*,GJErrorCode);
	virtual void loadMessagesFinished(cocos2d::CCArray *,char const*);
	virtual void setupPageInfo(gd::string,char const*);
}
class MessagesProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, MessageListDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(bool);
	void deleteSelected();
	void forceReloadMessages(bool);
	void init(bool);
	void isCorrect(char const*);
	void keyBackClicked();
	void loadMessagesFailed(char const*, GJErrorCode);
	void loadMessagesFinished(cocos2d::CCArray*, char const*);
	void loadPage(int);
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onDeleteSelected(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onSentMessages(cocos2d::CCObject*);
	void onToggleAllObjects(cocos2d::CCObject*);
	void onUpdate(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void setupCommentsBrowser(cocos2d::CCArray*);
	void setupPageInfo(gd::string, char const*);
	void untoggleAll();
	void updateLevelsLabel();
	void updatePageArrows();
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void forceReloadMessages(bool);
	virtual void loadMessagesFailed(char const*,GJErrorCode);
	virtual void loadMessagesFinished(cocos2d::CCArray *,char const*);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void setupPageInfo(gd::string,char const*);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class MoreOptionsLayer : FLAlertLayer, TextInputDelegate, GooglePlayDelegate {
	void addToggle(char const*, char const*, char const*);
	void countForPage(int);
	void create();
	void goToPage(int);
	void googlePlaySignedIn();
	void incrementCountForPage(int);
	void infoKey(int);
	void init();
	void keyBackClicked();
	void layerForPage(int);
	void layerKey(int);
	void nextPosition(int);
	void objectKey(int);
	void objectsForPage(int);
	void onClose(cocos2d::CCObject*);
	void onGPSignIn(cocos2d::CCObject*);
	void onGPSignOut(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onKeybindings(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onParental(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onSongBrowser(cocos2d::CCObject*);
	void onToggle(cocos2d::CCObject*);
	void pageKey(int);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void toggleGP();
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class MoreSearchLayer : FLAlertLayer, TextInputDelegate {
	void audioNext(cocos2d::CCObject*);
	void audioPrevious(cocos2d::CCObject*);
	void create();
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onCoins(cocos2d::CCObject*);
	void onCompleted(cocos2d::CCObject*);
	void onEpic(cocos2d::CCObject*);
	void onFeatured(cocos2d::CCObject*);
	void onFollowed(cocos2d::CCObject*);
	void onFriends(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onNoStar(cocos2d::CCObject*);
	void onOriginal(cocos2d::CCObject*);
	void onSongFilter(cocos2d::CCObject*);
	void onSongMode(cocos2d::CCObject*);
	void onTwoPlayer(cocos2d::CCObject*);
	void onUncompleted(cocos2d::CCObject*);
	void selectSong(int);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void toggleSongNodes(bool, bool);
	void updateAudioLabel();
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class MoreVideoOptionsLayer : FLAlertLayer {
	void addToggle(char const*, char const*, char const*);
	void countForPage(int);
	void create();
	void goToPage(int);
	void incrementCountForPage(int);
	void infoKey(int);
	void init();
	void keyBackClicked();
	void layerForPage(int);
	void layerKey(int);
	void nextPosition(int);
	void objectKey(int);
	void objectsForPage(int);
	void onClose(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onToggle(cocos2d::CCObject*);
	void pageKey(int);
}
class MultilineBitmapFont : cocos2d::CCSprite {
	void createWithFont(char const*, gd::string, float, float, cocos2d::CCPoint, int, bool);
	void initWithFont(char const*, gd::string, float, float, cocos2d::CCPoint, int, bool);
	void moveSpecialDescriptors(int, int);
	void readColorInfo(gd::string);
	void setOpacity(unsigned char);
	void stringWithMaxWidth(gd::string, float, float);
	virtual void setOpacity(uchar);
}
class MusicDelegateHandler : cocos2d::CCNode {
	void create(MusicDownloadDelegate*);
	void init(MusicDownloadDelegate*);
}
class MusicDownloadDelegate {
	void downloadSongFailed(int, GJSongError);
	void downloadSongFinished(SongInfoObject*);
	void loadSongInfoFailed(int, GJSongError);
	void loadSongInfoFinished(SongInfoObject*);
	void songStateChanged();
	virtual void downloadSongFailed(int,GJSongError);
	virtual void downloadSongFinished(SongInfoObject *);
	virtual void loadSongInfoFailed(int,GJSongError);
	virtual void loadSongInfoFinished(SongInfoObject *);
}
class MusicDownloadManager : cocos2d::CCNode, PlatformDownloadDelegate {
	void ProcessHttpRequest(gd::string, gd::string, gd::string, GJHttpType);
	void addDLToActive(char const*);
	void addDLToActive(char const*, cocos2d::CCObject*);
	void addMusicDownloadDelegate(MusicDownloadDelegate*);
	void addSongObjectFromString(gd::string);
	void clearSong(int);
	void clearUnusedSongs();
	void createSongsInfo(gd::string);
	void dataLoaded(DS_Dictionary*);
	void deleteSong(int);
	void downloadSong(int);
	void downloadSongFailed(int, GJSongError);
	void downloadSongFinished(SongInfoObject*);
	void encodeDataTo(DS_Dictionary*);
	void firstSetup();
	void getAllSongs();
	void getDLObject(char const*);
	void getDownloadProgress(int);
	void getDownloadedSongs();
	void getSongDownloadKey(int);
	void getSongInfo(int, bool);
	void getSongInfoKey(int);
	void getSongInfoObject(int);
	void getSongPriority();
	void handleIt(bool, gd::string, gd::string, GJHttpType);
	void handleItDelayed(bool, gd::string, gd::string, GJHttpType);
	void handleItND(cocos2d::CCNode*, void*);
	void incrementPriorityForSong(int);
	void init();
	void isDLActive(char const*);
	void isRunningActionForSongID(int);
	void isSongDownloaded(int);
	void limitDownloadedSongs();
	void loadSongInfoFailed(int, GJSongError);
	void loadSongInfoFinished(SongInfoObject*);
	void onDownloadSongCompleted(cocos2d::extension::CCHttpClient*, cocos2d::extension::CCHttpResponse*);
	void onGetSongInfoCompleted(gd::string, gd::string);
	void onProcessHttpRequestCompleted(cocos2d::extension::CCHttpClient*, cocos2d::extension::CCHttpResponse*);
	void pathForSong(int);
	void removeDLFromActive(char const*);
	void removeMusicDownloadDelegate(MusicDownloadDelegate*);
	void responseToDict(gd::string, char const*);
	void setDownloadedSongsDict(cocos2d::CCDictionary*);
	void sharedState();
	void showTOS(FLAlertLayerProtocol*);
	void songStateChanged();
	void stopDownload(int);
}
class MyLevelsLayer : cocos2d::CCLayer {
	void create();
	void init();
	void keyBackClicked();
	void onBack(cocos2d::CCObject*);
	void onNew(cocos2d::CCObject*);
	void scene();
	void setupLevelBrowser();
}
class NodePoint : cocos2d::CCObject {
	void create(cocos2d::CCPoint);
	void init(cocos2d::CCPoint);
}
class NumberInputDelegate {
	void numberInputClosed(NumberInputLayer*);
	virtual void numberInputClosed(NumberInputLayer *);
}
class NumberInputLayer : FLAlertLayer {
	void create();
	void deleteLast();
	void init();
	void inputNumber(int);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onDone(cocos2d::CCObject*);
	void onNumber(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void updateNumberState();
}
class OBB2D : cocos2d::CCNode {
	void calculateWithCenter(cocos2d::CCPoint, float, float, float);
	void computeAxes();
	void create(cocos2d::CCPoint, float, float, float);
	void getBoundingRect();
	void init(cocos2d::CCPoint, float, float, float);
	void orderCorners();
	void overlaps(OBB2D*);
	void overlaps1Way(OBB2D*);
}
class ObjectDecoder : cocos2d::CCNode {
	void getDecodedObject(int, DS_Dictionary*);
	void init();
	void sharedDecoder();
}
class ObjectDecoderDelegate {
	void getDecodedObject(int, DS_Dictionary*);
	virtual void getDecodedObject(int,DS_Dictionary *);
}
class ObjectManager : cocos2d::CCNode {
	void animLoaded(char const*);
	void getDefinition(char const*);
	void getGlobalAnimCopy(char const*);
	void init();
	void instance();
	void loadCopiedAnimations();
	void loadCopiedSets();
	void purgeObjectManager();
	void replaceAllOccurencesOfString(cocos2d::CCString*, cocos2d::CCString*, cocos2d::CCDictionary*);
	void setLoaded(char const*);
	void setup();
}
class ObjectToolbox : cocos2d::CCNode {
	void allKeys();
	void frameToKey(char const*);
	void gridNodeSizeForKey(int);
	void init();
	void intKeyToFrame(int);
	void keyToFrame(char const*);
	void perspectiveBlockFrame(int);
	void sharedState();
}
class OnlineListDelegate {
	void loadListFailed(char const*);
	void loadListFinished(cocos2d::CCArray*, char const*);
	void setupPageInfo(gd::string, char const*);
	virtual void loadListFailed(char const*);
	virtual void loadListFinished(cocos2d::CCArray *,char const*);
	virtual void setupPageInfo(gd::string,char const*);
}
class OpacityEffectAction : cocos2d::CCNode {
	void create(float, float, float, int);
	void createFromString(gd::string);
	void getSaveString();
	void init(float, float, float, int);
	void step(float);
}
class OptionsLayer : GJDropDownLayer, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void customSetup();
	void exitLayer();
	void layerHidden();
	void musicSliderChanged(cocos2d::CCObject*);
	void onAccount(cocos2d::CCObject*);
	void onHelp(cocos2d::CCObject*);
	void onOptions(cocos2d::CCObject*);
	void onProgressBar(cocos2d::CCObject*);
	void onRate(cocos2d::CCObject*);
	void onRecordReplays(cocos2d::CCObject*);
	void onSecretVault(cocos2d::CCObject*);
	void onSoundtracks(cocos2d::CCObject*);
	void onSupport(cocos2d::CCObject*);
	void onVideo(cocos2d::CCObject*);
	void sfxSliderChanged(cocos2d::CCObject*);
	void tryEnableRecord();
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
}
class ParentalOptionsLayer : FLAlertLayer {
	void addToggle(char const*, char const*, char const*);
	void countForPage(int);
	void create();
	void goToPage(int);
	void incrementCountForPage(int);
	void infoKey(int);
	void init();
	void keyBackClicked();
	void layerForPage(int);
	void layerKey(int);
	void nextPosition(int);
	void objectKey(int);
	void objectsForPage(int);
	void onClose(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onToggle(cocos2d::CCObject*);
	void pageKey(int);
}
class PauseLayer : CCBlockLayer {
	void create(bool);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void customSetup();
	void goEdit();
	void init(bool);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void keyUp(cocos2d::enumKeyCodes);
	void musicSliderChanged(cocos2d::CCObject*);
	void onAutoCheckpoints(cocos2d::CCObject*);
	void onAutoRetry(cocos2d::CCObject*);
	void onEdit(cocos2d::CCObject*);
	void onHelp(cocos2d::CCObject*);
	void onNormalMode(cocos2d::CCObject*);
	void onPracticeMode(cocos2d::CCObject*);
	void onProgressBar(cocos2d::CCObject*);
	void onQuit(cocos2d::CCObject*);
	void onRecordReplays(cocos2d::CCObject*);
	void onRestart(cocos2d::CCObject*);
	void onResume(cocos2d::CCObject*);
	void onTime(cocos2d::CCObject*);
	void setupProgressBars();
	void sfxSliderChanged(cocos2d::CCObject*);
	void tryShowBanner(float);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void keyUp(cocos2d::enumKeyCodes);
}
class PlatformDownloadDelegate {
	void downloadFailed(char const*);
	void downloadFinished(char const*);
	virtual void downloadFailed(char const*);
	virtual void downloadFinished(char const*);
}
class PlayerCheckpoint : cocos2d::CCNode {
	void create();
	void init();
}
class PlayerObject : GameObject {
	void activateStreak();
	void addAllParticles();
	void addToTouchedRings(GameObject*);
	void animationFinished(char const*);
	void boostPlayer(float);
	void bumpPlayer(float, int);
	void buttonDown(PlayerButton);
	void checkSnapJumpToObject(GameObject*);
	void collidedWithObject(float, GameObject*);
	void collidedWithObject(float, GameObject*, cocos2d::CCRect);
	void collidedWithSlope(float, GameObject*, bool);
	void convertToClosestRotation(float);
	void copyAttributes(PlayerObject*);
	void create(int, int, cocos2d::CCLayer*);
	void deactivateParticle();
	void deactivateStreak(bool);
	void fadeOutStreak2(float);
	void flashPlayer(float, float, cocos2d::_ccColor3B, cocos2d::_ccColor3B);
	void flipGravity(bool, bool);
	void flipMod();
	void getActiveMode();
	void getModifiedSlopeYVel();
	void getOldPosition(float);
	void getOrientedBox();
	void getRealPosition();
	void getSecondColor();
	void gravityDown();
	void gravityUp();
	void hardFlipGravity();
	void hitGround(bool);
	void incrementJumps();
	void init(int, int, cocos2d::CCLayer*);
	void isBoostValid(float);
	void isFlying();
	void isSafeFlip(float);
	void isSafeMode(float);
	void isSafeSpiderFlip(float);
	void levelFlipFinished();
	void levelFlipping();
	void levelWillFlip();
	void loadFromCheckpoint(PlayerCheckpoint*);
	void lockPlayer();
	void logValues();
	void modeDidChange();
	void placeStreakPoint();
	void playBurstEffect();
	void playDeathEffect();
	void playDynamicSpiderRun();
	void playerDestroyed(bool);
	void playerIsFalling();
	void playerTeleported();
	void playingEndEffect();
	void postCollision(float);
	void preCollision();
	void preSlopeCollision(float, GameObject*);
	void propellPlayer(float);
	void pushButton(PlayerButton);
	void pushDown();
	void pushPlayer(float);
	void releaseButton(PlayerButton);
	void removeAllParticles();
	void removePendingCheckpoint();
	void resetAllParticles();
	void resetCollisionLog();
	void resetObject();
	void resetPlayerIcon();
	void resetStateVariables();
	void resetStreak();
	void resetTouchedRings();
	void ringJump(GameObject*);
	void runBallRotation(float);
	void runBallRotation2();
	void runNormalRotation();
	void runRotateAction(bool);
	void saveToCheckpoint(PlayerCheckpoint*);
	void setColor(cocos2d::_ccColor3B const&);
	void setFlipX(bool);
	void setFlipY(bool);
	void setOpacity(unsigned char);
	void setPortalP(cocos2d::CCPoint);
	void setPosition(cocos2d::CCPoint const&);
	void setRotation(float);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void setSecondColor(cocos2d::_ccColor3B const&);
	void setVisible(bool);
	void setupStreak();
	void spawnCircle();
	void spawnCircle2();
	void spawnDualCircle();
	void spawnFromPlayer(PlayerObject*);
	void spawnPortalCircle(cocos2d::_ccColor3B, float);
	void spawnScaleCircle();
	void specialGroundHit();
	void speedDown();
	void speedUp();
	void spiderTestJump(bool);
	void startDashing(GameObject*);
	void stopBurstEffect();
	void stopDashing();
	void stopRotation(bool);
	void storeCollision(bool, int);
	void switchedToMode(GameObjectType);
	void testForMoving(float, GameObject*);
	void toggleBirdMode(bool);
	void toggleDartMode(bool);
	void toggleFlyMode(bool);
	void toggleGhostEffect(GhostType);
	void togglePlayerScale(bool);
	void toggleRobotMode(bool);
	void toggleRollMode(bool);
	void toggleSpiderMode(bool);
	void toggleVisibility(bool);
	void touchedObject(GameObject*);
	void tryPlaceCheckpoint();
	void update(float);
	void updateCheckpointMode(bool);
	void updateCheckpointTest();
	void updateCollide(bool, int);
	void updateCollideBottom(float, int);
	void updateCollideTop(float, int);
	void updateDashAnimation();
	void updateDashArt();
	void updateGlowColor();
	void updateJump(float);
	void updateJumpVariables();
	void updatePlayerBirdFrame(int);
	void updatePlayerDartFrame(int);
	void updatePlayerFrame(int);
	void updatePlayerGlow();
	void updatePlayerRobotFrame(int);
	void updatePlayerRollFrame(int);
	void updatePlayerScale();
	void updatePlayerShipFrame(int);
	void updatePlayerSpiderFrame(int);
	void updatePlayerSpriteExtra(gd::string);
	void updateRobotAnimationSpeed();
	void updateRotation(float);
	void updateRotation(float, float);
	void updateShipRotation(float);
	void updateShipSpriteExtra(gd::string);
	void updateSlopeRotation(float);
	void updateSlopeYVelocity(float);
	void updateSpecial(float);
	void updateStateVariables();
	void updateTimeMod(float);
	void usingWallLimitedMode();
	void yStartDown();
	void yStartUp();
	virtual void animationFinished(char const*);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setFlipX(bool);
	virtual void setFlipY(bool);
	virtual void setOpacity(uchar);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void setRotation(float);
	virtual void setScale(float);
	virtual void setScaleX(float);
	virtual void setScaleY(float);
	virtual void setVisible(bool);
	virtual void update(float);
}
class PlayLayer : GJBaseGameLayer, CCCircleWaveDelegate, CurrencyRewardDelegate, DialogDelegate {
	void addCircle(CCCircleWave*);
	void addObject(GameObject*);
	void addToGroupOld(GameObject*);
	void addToSpeedObjects(GameObject*);
	void animateInDualGround(GameObject*, float, bool);
	void animateInGround(bool);
	void animateOutGround(bool);
	void animateOutGroundFinished();
	void applyEnterEffect(GameObject*);
	void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*);
	void cameraMoveX(float, float, float);
	void cameraMoveY(float, float, float);
	void checkCollisions(PlayerObject*, float);
	void circleWaveWillBeRemoved(CCCircleWave*);
	void claimParticle(gd::string);
	void clearPickedUpItems();
	void colorObject(int, cocos2d::_ccColor3B);
	void commitJumps();
	void create(GJGameLevel*);
	void createCheckpoint();
	void createObjectsFromSetup(gd::string);
	void createParticle(int, char const*, int, cocos2d::tCCPositionType);
	void currencyWillExit(CurrencyRewardLayer*);
	void delayedResetLevel();
	void destroyPlayer(PlayerObject*, GameObject*);
	void dialogClosed(DialogLayer*);
	void draw();
	void enterDualMode(GameObject*, bool);
	void exitAirMode();
	void exitBirdMode(PlayerObject*);
	void exitDartMode(PlayerObject*);
	void exitFlyMode(PlayerObject*);
	void exitRobotMode(PlayerObject*);
	void exitRollMode(PlayerObject*);
	void exitSpiderMode(PlayerObject*);
	void flipFinished();
	void flipGravity(PlayerObject*, bool, bool);
	void flipObjects();
	void fullReset();
	void getLastCheckpoint();
	void getMaxPortalY();
	void getMinPortalY();
	void getObjectsState();
	void getOtherPlayer(PlayerObject*);
	void getParticleKey(int, char const*, int, cocos2d::tCCPositionType);
	void getParticleKey2(gd::string);
	void getRelativeMod(cocos2d::CCPoint, float, float, float);
	void getTempMilliTime();
	void gravityEffectFinished();
	void hasItem(int);
	void hasUniqueCoin(GameObject*);
	void incrementJumps();
	void init(GJGameLevel*);
	void isFlipping();
	void levelComplete();
	void lightningFlash(cocos2d::CCPoint, cocos2d::_ccColor3B);
	void lightningFlash(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::_ccColor3B, float, float, int, bool, float);
	void loadDefaultColors();
	void loadFromCheckpoint(CheckpointObject*);
	void loadLastCheckpoint();
	void loadSavedObjectsState(gd::string);
	void markCheckpoint();
	void moveCameraToPos(cocos2d::CCPoint);
	void onEnterTransitionDidFinish();
	void onExit();
	void onQuit();
	void optimizeColorGroups();
	void optimizeOpacityGroups();
	void optimizeSaveRequiredGroups();
	void pauseGame(bool);
	void pickupItem(GameObject*);
	void playAnimationCommand(int, int);
	void playEndAnimationToPos(cocos2d::CCPoint);
	void playExitDualEffect(PlayerObject*);
	void playFlashEffect(float, int, float);
	void playGravityEffect(bool);
	void playSpeedParticle(float);
	void playerWillSwitchMode(PlayerObject*, GameObject*);
	void prepareSpawnObjects();
	void processItems();
	void processLoadedMoveActions();
	void recordAction(bool, PlayerObject*);
	void registerActiveObject(GameObject*);
	void registerStateObject(GameObject*);
	void removeAllObjects();
	void removeFromGroupOld(GameObject*);
	void removeLastCheckpoint();
	void removePlayer2();
	void resetLevel();
	void resume();
	void resumeAndRestart();
	void saveRecordAction(bool, PlayerObject*);
	void scene(GJGameLevel*);
	void setStartPosObject(StartPosObject*);
	void setupLevelStart(LevelSettingsObject*);
	void setupReplay(gd::string);
	void shakeCamera(float, float, float);
	void shouldBlend(int);
	void showCompleteEffect();
	void showCompleteText();
	void showEndLayer();
	void showHint();
	void showNewBest(bool, int, int, bool, bool, bool);
	void showRetryLayer();
	void showTwoPlayerGuide();
	void sortGroups();
	void spawnCircle();
	void spawnFirework();
	void spawnParticle(char const*, int, cocos2d::tCCPositionType, cocos2d::CCPoint);
	void spawnPlayer2();
	void startGame();
	void startMusic();
	void startRecording();
	void startRecordingDelayed();
	void stopCameraShake();
	void stopRecording();
	void storeCheckpoint(CheckpointObject*);
	void switchToFlyMode(PlayerObject*, GameObject*, bool, int);
	void switchToRobotMode(PlayerObject*, GameObject*, bool);
	void switchToRollMode(PlayerObject*, GameObject*, bool);
	void switchToSpiderMode(PlayerObject*, GameObject*, bool);
	void timeForXPos(float);
	void timeForXPos2(float, bool);
	void toggleBGEffectVisibility(bool);
	void toggleDualMode(GameObject*, bool, PlayerObject*, bool);
	void toggleFlipped(bool, bool);
	void toggleGhostEffect(int);
	void toggleGlitter(bool);
	void togglePracticeMode(bool);
	void toggleProgressbar();
	void tryStartRecord();
	void unclaimParticle(char const*, cocos2d::CCParticleSystemQuad*);
	void unregisterActiveObject(GameObject*);
	void unregisterStateObject(GameObject*);
	void update(float);
	void updateAttempts();
	void updateCamera(float);
	void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*);
	void updateDualGround(PlayerObject*, int, bool);
	void updateEffectPositions();
	void updateLevelColors();
	void updateMoveObjectsLastPosition();
	void updateProgressbar();
	void updateReplay(float);
	void updateTimeMod(float, bool);
	void updateTweenAction(float, char const*);
	void updateVisibility();
	void vfDChk();
	void visit();
	void visitWithColorFlash();
	void willSwitchToMode(int, PlayerObject*);
	void xPosForTime(float);
	virtual void calculateColorValues(EffectGameObject *,EffectGameObject *,int,float,ColorActionSprite *,GJEffectManager *);
	virtual void circleWaveWillBeRemoved(CCCircleWave *);
	virtual void currencyWillExit(CurrencyRewardLayer *);
	virtual void dialogClosed(DialogLayer *);
	virtual void flipGravity(PlayerObject *,bool,bool);
	virtual void timeForXPos(float);
	virtual void update(float);
	virtual void updateColor(cocos2d::_ccColor3B,float,int,bool,float,cocos2d::_ccHSVValue,int,bool,int,EffectGameObject *);
	virtual void updateTweenAction(float,char const*);
	virtual void xPosForTime(float);
}
class PointNode : cocos2d::CCObject {
	void create(cocos2d::CCPoint);
	void init(cocos2d::CCPoint);
}
class PriceLabel : cocos2d::CCNode {
	void create(int);
	void init(int);
	void setColor(cocos2d::_ccColor3B);
	void setOpacity(float);
	void setPrice(int);
}
class ProfilePage : FLAlertLayer, FLAlertLayerProtocol, LevelCommentDelegate, CommentUploadDelegate, UserInfoDelegate, UploadActionDelegate, UploadPopupDelegate, LeaderboardManagerDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void blockUser();
	void commentDeleteFailed(int, int);
	void commentUploadFailed(int, CommentError);
	void commentUploadFinished(int);
	void create(int, bool);
	void getUserInfoFailed(int);
	void getUserInfoFinished(GJUserScore*);
	void init(int, bool);
	void isCorrect(char const*);
	void keyBackClicked();
	void loadCommentsFailed(char const*);
	void loadCommentsFinished(cocos2d::CCArray*, char const*);
	void loadPage(int);
	void loadPageFromUserInfo(GJUserScore*);
	void onBlockUser(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onComment(cocos2d::CCObject*);
	void onCommentHistory(cocos2d::CCObject*);
	void onFollow(cocos2d::CCObject*);
	void onFriend(cocos2d::CCObject*);
	void onFriends(cocos2d::CCObject*);
	void onMessages(cocos2d::CCObject*);
	void onMyLevels(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void onRequests(cocos2d::CCObject*);
	void onSendMessage(cocos2d::CCObject*);
	void onSettings(cocos2d::CCObject*);
	void onTwitch(cocos2d::CCObject*);
	void onTwitter(cocos2d::CCObject*);
	void onUpdate(cocos2d::CCObject*);
	void onYouTube(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void setupComments();
	void setupCommentsBrowser(cocos2d::CCArray*);
	void setupPageInfo(gd::string, char const*);
	void show();
	void showNoAccountError();
	void toggleMainPageVisibility(bool);
	void updateLevelsLabel();
	void updatePageArrows();
	void updateUserScoreFailed();
	void updateUserScoreFinished();
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	void userInfoChanged(GJUserScore*);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void commentDeleteFailed(int,int);
	virtual void commentUploadFailed(int,CommentError);
	virtual void commentUploadFinished(int);
	virtual void getUserInfoFailed(int);
	virtual void getUserInfoFinished(GJUserScore *);
	virtual void loadCommentsFailed(char const*);
	virtual void loadCommentsFinished(cocos2d::CCArray *,char const*);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void setupPageInfo(gd::string,char const*);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
	virtual void userInfoChanged(GJUserScore *);
}
class PromoInterstitial : FLAlertLayer {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(bool);
	void init(bool);
	void keyBackClicked();
	void onClick(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void setup();
	void show();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
}
class pugi::xml_tree_walker {
	void begin(pugi::xml_node&);
	void depth();
	void end(pugi::xml_node&);
	void xml_tree_walker();
	virtual void begin(pugi::xml_node &);
	virtual void end(pugi::xml_node &);
}
class pugi::xml_writer {
}
class pugi::xml_writer_file : pugi::xml_writer {
	void write(void const*, unsigned int);
	void xml_writer_file(void*);
	virtual void write(void const*,uint);
}
class pugi::xml_writer_stream : pugi::xml_writer {
	void write(void const*, unsigned int);
	void xml_writer_stream(gd::basic_ostream<char, gd::char_traits<char> >&);
	void xml_writer_stream(gd::basic_ostream<wchar_t, gd::char_traits<wchar_t> >&);
	virtual void write(void const*,uint);
}
class PulseEffectAction : cocos2d::CCNode {
	void create(float, float, float, PulseEffectType, int, cocos2d::_ccColor3B, cocos2d::_ccHSVValue, int, bool, bool);
	void createFromString(gd::string);
	void getSaveString();
	void init(float, float, float, PulseEffectType, int, cocos2d::_ccColor3B, cocos2d::_ccHSVValue, int, bool, bool);
	void isFinished();
	void step(float);
	void valueForDelta(float, float, float, float);
}
class PurchaseItemPopup : FLAlertLayer {
	void create(GJStoreItem*);
	void init(GJStoreItem*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onPurchase(cocos2d::CCObject*);
}
class RateDemonLayer : FLAlertLayer, UploadPopupDelegate, UploadActionDelegate {
	void create(int);
	void init(int);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onRate(cocos2d::CCObject*);
	void selectRating(cocos2d::CCObject*);
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class RateLevelDelegate {
	void rateLevelClosed();
}
class RateLevelLayer : FLAlertLayer {
	void create(int);
	void init(int);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onRate(cocos2d::CCObject*);
	void selectRating(cocos2d::CCObject*);
}
class RateStarsLayer : FLAlertLayer, UploadPopupDelegate, UploadActionDelegate {
	void create(int, bool);
	void getStarsButton(int, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float);
	void init(int, bool);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onRate(cocos2d::CCObject*);
	void onToggleCoins(cocos2d::CCObject*);
	void onToggleFeature(cocos2d::CCObject*);
	void selectRating(cocos2d::CCObject*);
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class RetryLevelLayer : GJDropDownLayer {
	void create();
	void customSetup();
	void enterAnimFinished();
	void getEndText();
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void keyUp(cocos2d::enumKeyCodes);
	void onEveryplay(cocos2d::CCObject*);
	void onMenu(cocos2d::CCObject*);
	void onReplay(cocos2d::CCObject*);
	void setupLastProgress();
	void showLayer(bool);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void keyUp(cocos2d::enumKeyCodes);
	virtual void showLayer(bool);
}
class RewardsPage : FLAlertLayer, FLAlertLayerProtocol, GJRewardDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void getRewardFrame(int, int);
	void init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onFreeStuff(cocos2d::CCObject*);
	void onReward(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void rewardsStatusFailed();
	void rewardsStatusFinished(int);
	void show();
	void tryGetRewards();
	void unlockLayerClosed(RewardUnlockLayer*);
	void updateTimers(float);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void rewardsStatusFinished(int);
}
class RewardUnlockLayer : FLAlertLayer, CurrencyRewardDelegate {
	void connectionTimeout();
	void create(int, RewardsPage*);
	void currencyWillExit(CurrencyRewardLayer*);
	void init(int, RewardsPage*);
	void keyBackClicked();
	void labelEnterFinishedO(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void playDropSound();
	void playLabelEffect(int, int, cocos2d::CCSprite*, cocos2d::CCPoint, float);
	void playRewardEffect();
	void readyToCollect(GJRewardItem*);
	void showCloseButton();
	void showCollectReward(GJRewardItem*);
	void step2();
	void step3();
	virtual void currencyWillExit(CurrencyRewardLayer *);
}
class RingObject : EffectGameObject {
	void create(char const*);
	void customObjectSetup(gd::map<gd::string, gd::string, gd::less<gd::string >, gd::allocator<gd::pair<gd::string const, gd::string > > >&);
	void getSaveString();
	void init(char const*);
	void powerOffObject();
	void powerOnObject();
	void resetObject();
	void setRScale(float);
	void setRotation(float);
	void setScale(float);
	void spawnCircle();
	void triggerActivated(float);
	virtual void customObjectSetup(gd::map<gd::string,gd::string,gd::less<gd::string>,gd::allocator<gd::pair<gd::string const,gd::string>>> &);
	virtual void setRScale(float);
	virtual void setRotation(float);
	virtual void setScale(float);
	virtual void triggerActivated(float);
}
class ScrollingLayer : cocos2d::CCLayerColor {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(cocos2d::CCSize, cocos2d::CCPoint, float);
	void draw();
	void getViewRect();
	void init(cocos2d::CCSize, cocos2d::CCPoint, float);
	void setStartOffset(cocos2d::CCPoint);
	void visit();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
}
class SearchButton : cocos2d::CCSprite {
	void create(char const*, char const*, float, char const*);
	void init(char const*, char const*, float, char const*);
}
class SecretLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void getBasicMessage();
	void getMessage();
	void getThreadMessage();
	void init();
	void keyBackClicked();
	void nodeWithTag(int);
	void onBack(cocos2d::CCObject*);
	void onSubmit(cocos2d::CCObject*);
	void playCoinEffect();
	void scene();
	void selectAThread();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void updateMessageLabel(gd::string);
	void updateSearchLabel(char const*);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
}
class SecretLayer2 : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DialogDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void dialogClosed(DialogLayer*);
	void getBasicMessage();
	void getErrorMessage();
	void getMessage();
	void getThreadMessage();
	void init();
	void keyBackClicked();
	void nodeWithTag(int);
	void onBack(cocos2d::CCObject*);
	void onDoor(cocos2d::CCObject*);
	void onExit();
	void onSecretLevel(cocos2d::CCObject*);
	void onSubmit(cocos2d::CCObject*);
	void playCoinEffect();
	void scene();
	void selectAThread();
	void showCompletedLevel();
	void showSecretLevel();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void updateMessageLabel(gd::string);
	void updateSearchLabel(char const*);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void dialogClosed(DialogLayer *);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
}
class SecretLayer3 : cocos2d::CCLayer, DialogDelegate {
	void animateEyes();
	void create();
	void dialogClosed(DialogLayer*);
	void firstInteractionStep1();
	void firstInteractionStep2();
	void firstInteractionStep3();
	void firstInteractionStep4();
	void init();
	void keyBackClicked();
	void onBack(cocos2d::CCObject*);
	void onChest01(cocos2d::CCObject*);
	void onChest02(cocos2d::CCObject*);
	void onClick(cocos2d::CCObject*);
	void onExit();
	void revealStep1();
	void revealStep2();
	void revealStep3();
	void revealStep4();
	void revealStep5();
	void scene();
	void showUnlockDialog();
	virtual void dialogClosed(DialogLayer *);
}
class SecretLayer4 : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DialogDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void dialogClosed(DialogLayer*);
	void getBasicMessage();
	void getErrorMessage();
	void getMessage();
	void getThreadMessage();
	void init();
	void keyBackClicked();
	void nodeWithTag(int);
	void onBack(cocos2d::CCObject*);
	void onChest01(cocos2d::CCObject*);
	void onExit();
	void onSubmit(cocos2d::CCObject*);
	void playCoinEffect();
	void scene();
	void selectAThread();
	void showDialog(int);
	void showFirstDialog();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void updateMessageLabel(gd::string);
	void updateSearchLabel(char const*);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void dialogClosed(DialogLayer *);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
}
class SecretNumberLayer : cocos2d::CCLayer {
	void create();
	void init();
	void playNumberEffect(int);
}
class SelectArtDelegate {
	void selectArtClosed(SelectArtLayer*);
	virtual void selectArtClosed(SelectArtLayer *);
}
class SelectArtLayer : FLAlertLayer {
	void create(SelectArtType);
	void init(SelectArtType);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSelectCustom(cocos2d::CCObject*);
	void selectArt(cocos2d::CCObject*);
	void updateSelectedCustom(int);
}
class SelectFontLayer : FLAlertLayer {
	void create(LevelEditorLayer*);
	void init(LevelEditorLayer*);
	void keyBackClicked();
	void onChangeFont(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void updateFontLabel();
}
class SetFolderPopup : SetIDPopup, SetTextPopupDelegate {
	void create(int, bool, gd::string);
	void init(int, bool, gd::string);
	void onSetFolderName(cocos2d::CCObject*);
	void setTextPopupClosed(SetTextPopup*, gd::string);
	void valueChanged();
	virtual void setTextPopupClosed(SetTextPopup *,gd::string);
}
class SetGroupIDLayer : FLAlertLayer, TextInputDelegate {
	void addGroupID(int);
	void callRemoveFromGroup(float);
	void create(GameObject*, cocos2d::CCArray*);
	void determineStartValues();
	void init(GameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onAddGroup(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onCopy(cocos2d::CCObject*);
	void onDontEnter(cocos2d::CCObject*);
	void onDontFade(cocos2d::CCObject*);
	void onEditorLayer(cocos2d::CCObject*);
	void onEditorLayer2(cocos2d::CCObject*);
	void onGroupID(cocos2d::CCObject*);
	void onHighDetail(cocos2d::CCObject*);
	void onNextGroupID1(cocos2d::CCObject*);
	void onPaste(cocos2d::CCObject*);
	void onRemoveFromGroup(cocos2d::CCObject*);
	void onSmoothEase(cocos2d::CCObject*);
	void onToggleGroupParent(cocos2d::CCObject*);
	void onZLayer(cocos2d::CCObject*);
	void onZLayerShift(cocos2d::CCObject*);
	void onZOrder(cocos2d::CCObject*);
	void removeGroupID(int);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void updateEditorLabel();
	void updateEditorLabel2();
	void updateEditorLayerID();
	void updateEditorLayerID2();
	void updateGroupIDButtons();
	void updateGroupIDLabel();
	void updateZLayerButtons();
	void updateZOrder();
	void updateZOrderLabel();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
}
class SetIDLayer : FLAlertLayer {
	void create(GameObject*);
	void init(GameObject*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onDown(cocos2d::CCObject*);
	void onUp(cocos2d::CCObject*);
	void updateID();
}
class SetIDPopup : FLAlertLayer, TextInputDelegate {
	void create(int, int, int, gd::string, gd::string, bool, int);
	void init(int, int, int, gd::string, gd::string, bool, int);
	void keyBackClicked();
	void onCancel(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onItemIDArrow(cocos2d::CCObject*);
	void onResetValue(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void updateTextInputLabel();
	void valueChanged();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
}
class SetIDPopupDelegate {
	void setIDPopupClosed(SetIDPopup*, int);
	virtual void setIDPopupClosed(SetIDPopup *,int);
}
class SetItemIDLayer : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onItemIDArrow(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateEditorLabel();
	void updateItemID();
	void updateTextInputLabel();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetTargetIDLayer : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*, gd::string);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetIDArrow(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetID();
	void updateTextInputLabel();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetTextPopup : FLAlertLayer, TextInputDelegate {
	void create(gd::string, gd::string, int, gd::string, gd::string, bool);
	void init(gd::string, gd::string, int, gd::string, gd::string, bool);
	void keyBackClicked();
	void onCancel(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onResetValue(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void updateTextInputLabel();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
}
class SetTextPopupDelegate {
	void setTextPopupClosed(SetTextPopup*, gd::string);
	virtual void setTextPopupClosed(SetTextPopup *,gd::string);
}
class SetupAnimationPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onAnimationIDArrow(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetIDArrow(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateAnimationID();
	void updateAnimationTextInputLabel();
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetID();
	void updateTextInputLabel();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupAnimSettingsPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onUseAnimOffset(cocos2d::CCObject*);
	void show();
	void sliderChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateAnimSpeedMod();
	void updateRotationInputLabel();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupCollisionTriggerPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onActivateOnExit(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onEnableGroup(cocos2d::CCObject*);
	void onItemID2Arrow(cocos2d::CCObject*);
	void onItemIDArrow(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetIDArrow(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateEditorLabel();
	void updateItemID();
	void updateItemID2();
	void updateItemID2InputLabel();
	void updateItemIDInputLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetID();
	void updateTargetIDInputLabel();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupCountTriggerPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onEnableGroup(cocos2d::CCObject*);
	void onItemIDArrow(cocos2d::CCObject*);
	void onMultiActivate(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetCountArrow(cocos2d::CCObject*);
	void onTargetIDArrow(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateCountTextInputLabel();
	void updateEditorLabel();
	void updateItemID();
	void updateItemIDInputLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetCount();
	void updateTargetID();
	void updateTargetIDInputLabel();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupInstantCountPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onCountType(cocos2d::CCObject*);
	void onEnableGroup(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetCountArrow(cocos2d::CCObject*);
	void onTargetID2Arrow(cocos2d::CCObject*);
	void onTargetIDArrow(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateCountTextInputLabel();
	void updateEditorLabel();
	void updateItemID();
	void updateItemIDInputLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetCount();
	void updateTargetID();
	void updateTargetIDInputLabel();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupInteractObjectPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onEnableGroup(cocos2d::CCObject*);
	void onItemIDArrow(cocos2d::CCObject*);
	void onNextItemID(cocos2d::CCObject*);
	void onSubstractCount(cocos2d::CCObject*);
	void onTargetIDArrow(cocos2d::CCObject*);
	void onToggleItem(cocos2d::CCObject*);
	void onToggleTrigger(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateItemID();
	void updateItemIDInputLabel();
	void updatePickupType();
	void updateState();
	void updateTargetID();
	void updateTextInputLabel();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupObjectTogglePopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onEnableGroup(cocos2d::CCObject*);
	void onMultiActivate(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetIDArrow(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetID();
	void updateTextInputLabel();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupOpacityPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetIDArrow(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void sliderChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateDurLabel();
	void updateDuration();
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateOpacity();
	void updateOpacityLabel();
	void updateSpawnedByTrigger();
	void updateTargetID();
	void updateTextInputLabel();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupPickupTriggerPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onItemIDArrow(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onNextItemID(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetCountArrow(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateCountTextInputLabel();
	void updateEditorLabel();
	void updateItemID();
	void updateItemIDInputLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetCount();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupPulsePopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
	void closeColorSelect(cocos2d::CCObject*);
	void colorSelectClosed(GJSpecialColorSelect*, int);
	void colorValueChanged(cocos2d::_ccColor3B);
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	void getColorValue();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onCopy(cocos2d::CCObject*);
	void onExclusive(cocos2d::CCObject*);
	void onGroupMainOnly(cocos2d::CCObject*);
	void onGroupSecondaryOnly(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onPaste(cocos2d::CCObject*);
	void onSelectPulseMode(cocos2d::CCObject*);
	void onSelectSpecialColor(cocos2d::CCObject*);
	void onSelectSpecialTargetID(cocos2d::CCObject*);
	void onSelectTargetMode(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void onUpdateCopyColor(cocos2d::CCObject*);
	void onUpdateCustomColor(cocos2d::CCObject*);
	void selectColor(cocos2d::_ccColor3B);
	void show();
	void sliderChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateColorValue();
	void updateCopyColor();
	void updateCopyColorTextInputLabel();
	void updateEditorLabel();
	void updateFadeInLabel(bool);
	void updateFadeInTime();
	void updateFadeOutLabel(bool);
	void updateFadeOutTime();
	void updateGroupMainOnly();
	void updateGroupSecondaryOnly();
	void updateHSVValue();
	void updateHoldLabel(bool);
	void updateHoldTime();
	void updateMultiTriggerBtn();
	void updatePulseMode();
	void updatePulseTargetType();
	void updateSpawnedByTrigger();
	void updateTargetID();
	void updateTextInputLabel();
	void updateTouchTriggered();
	virtual void colorSelectClosed(GJSpecialColorSelect *,int);
	virtual void colorValueChanged(cocos2d::_ccColor3B);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupRotatePopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onChangeState(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void show();
	void sliderChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateCustomRotation();
	void updateDisableRotation();
	void updateRotationInputLabel();
	void updateState();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupShakePopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void sliderChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateDurLabel(bool);
	void updateDuration();
	void updateInterval();
	void updateIntervalLabel(bool);
	void updateMultiTriggerBtn();
	void updateShake();
	void updateShakeLabel(bool);
	void updateSpawnedByTrigger();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupSpawnPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onEditorDisable(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetIDArrow(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void sliderChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateDelay();
	void updateDelayLabel(bool);
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetID();
	void updateTextInputLabel();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class SetupTouchTogglePopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void init(EffectGameObject*, cocos2d::CCArray*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onDualTouchMode(cocos2d::CCObject*);
	void onHoldMode(cocos2d::CCObject*);
	void onMultiTrigger(cocos2d::CCObject*);
	void onSpawnedByTrigger(cocos2d::CCObject*);
	void onTargetIDArrow(cocos2d::CCObject*);
	void onTouchMode(cocos2d::CCObject*);
	void onTouchTriggered(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateEditorLabel();
	void updateMultiTriggerBtn();
	void updateSpawnedByTrigger();
	void updateTargetID();
	void updateTextInputLabel();
	void updateTouchTriggered();
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class ShardsPage : FLAlertLayer {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void goToPage(int);
	void init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSwitchPage(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void show();
}
class ShareCommentLayer : FLAlertLayer, TextInputDelegate, UploadActionDelegate, UploadPopupDelegate {
	void create(gd::string, int, CommentType, int);
	void init(gd::string, int, CommentType, int);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onPercent(cocos2d::CCObject*);
	void onShare(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void updateCharCountLabel();
	void updateDescText(char const*);
	void updatePercentLabel();
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class ShareLevelLayer : FLAlertLayer {
	void create(GJGameLevel*);
	void getStarsButton(int, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float);
	void init(GJGameLevel*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSettings(cocos2d::CCObject*);
	void onShare(cocos2d::CCObject*);
	void selectRating(cocos2d::CCObject*);
	void setupStars();
}
class ShareLevelSettingsLayer : FLAlertLayer, NumberInputDelegate {
	void create(GJGameLevel*);
	void init(GJGameLevel*);
	void keyBackClicked();
	void numberInputClosed(NumberInputLayer*);
	void onClose(cocos2d::CCObject*);
	void onCopyable(cocos2d::CCObject*);
	void onEditPassword(cocos2d::CCObject*);
	void onPassword(cocos2d::CCObject*);
	void onUnlisted(cocos2d::CCObject*);
	void updateSettingsState();
	virtual void numberInputClosed(NumberInputLayer *);
}
class SimpleObject : cocos2d::CCObject {
	void create();
	void init();
}
class SimplePlayer : cocos2d::CCSprite {
	void create(int);
	void init(int);
	void setColor(cocos2d::_ccColor3B const&);
	void setFrames(char const*, char const*, char const*, char const*, char const*);
	void setOpacity(unsigned char);
	void setSecondColor(cocos2d::_ccColor3B const&);
	void updateColors();
	void updatePlayerFrame(int, IconType);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setOpacity(uchar);
}
class SlideInLayer : cocos2d::CCLayerColor {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void customSetup();
	void disableUI();
	void draw();
	void enableUI();
	void enterAnimFinished();
	void enterLayer();
	void exitLayer(cocos2d::CCObject*);
	void hideLayer(bool);
	void init();
	void keyBackClicked();
	void layerHidden();
	void layerVisible();
	void registerWithTouchDispatcher();
	void showLayer(bool);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void exitLayer(cocos2d::CCObject *);
	virtual void hideLayer(bool);
	virtual void showLayer(bool);
}
class Slider : cocos2d::CCLayer {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler);
	void create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, float);
	void create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float);
	void disableTouch();
	void getLiveDragging();
	void getThumb();
	void getValue();
	void hideGroove(bool);
	void init(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float);
	void setBarVisibility(bool);
	void setLiveDragging(bool);
	void setMaxOffset(float);
	void setRotated(bool);
	void setValue(float);
	void updateBar();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
}
class SliderThumb : cocos2d::CCMenuItemImage {
	void create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*);
	void getValue();
	void init(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*);
	void setMaxOffset(float);
	void setRotated(bool);
	void setValue(float);
}
class SliderTouchLogic : cocos2d::CCMenu {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, float);
	void init(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, float);
	void registerWithTouchDispatcher();
	void setMaxOffset(float);
	void setRotated(bool);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
}
class SongCell : TableViewCell {
	void draw();
	void init();
	void loadFromObject(SongObject*);
	void onClick(cocos2d::CCObject*);
	void updateBGColor(int);
}
class SongInfoLayer : FLAlertLayer {
	void create(int);
	void create(gd::string, gd::string, gd::string, gd::string, gd::string, gd::string);
	void init(gd::string, gd::string, gd::string, gd::string, gd::string, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onDownload(cocos2d::CCObject*);
	void onFB(cocos2d::CCObject*);
	void onNG(cocos2d::CCObject*);
	void onYT(cocos2d::CCObject*);
}
class SongInfoObject : cocos2d::CCNode {
	void canEncode();
	void create(int);
	void create(int, gd::string, gd::string, int, float, gd::string, gd::string, gd::string, int);
	void create(cocos2d::CCDictionary*);
	void createWithCoder(DS_Dictionary*);
	void encodeWithCoder(DS_Dictionary*);
	void init(int, gd::string, gd::string, int, float, gd::string, gd::string, gd::string, int);
	virtual void encodeWithCoder(DS_Dictionary *);
}
class SongObject : cocos2d::CCObject {
	void create(int);
	void init(int);
}
class SongOptionsLayer : FLAlertLayer {
	void create(LevelSettingsObject*);
	void init(LevelSettingsObject*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onFadeIn(cocos2d::CCObject*);
	void onFadeOut(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onPlayback(cocos2d::CCObject*);
	void updatePlaybackBtn();
}
class SongsLayer : GJDropDownLayer {
	void create();
	void customSetup();
}
class SpawnTriggerAction : cocos2d::CCNode {
	void create(float, int);
	void createFromString(gd::string);
	void getSaveString();
	void init(float, int);
	void isFinished();
	void step(float);
}
class SpeedObject : cocos2d::CCNode {
	void create(GameObject*, int, float);
	void init(GameObject*, int, float);
}
class SpriteAnimationManager : cocos2d::CCNode {
	void animationFinished();
	void callAnimationFinished();
	void createAnimations(gd::string);
	void createWithOwner(CCAnimatedSprite*, gd::string);
	void doCleanup();
	void executeAnimation(gd::string);
	void finishAnimation(gd::string);
	void getAnimType(gd::string);
	void getPrio(gd::string);
	void initWithOwner(CCAnimatedSprite*, gd::string);
	void loadAnimations(gd::string);
	void offsetCurrentAnimation(float);
	void overridePrio();
	void playSound(gd::string);
	void playSoundForAnimation(gd::string);
	void queueAnimation(gd::string);
	void resetAnimState();
	void runAnimation(gd::string);
	void runQueuedAnimation();
	void stopAnimations();
	void storeAnimation(cocos2d::CCAnimate*, cocos2d::CCAnimate*, gd::string, int, spriteMode, cocos2d::CCSpriteFrame*);
	void storeSoundForAnimation(cocos2d::CCString*, gd::string, float);
	void switchToFirstFrameOfAnimation(gd::string);
	void updateAnimationSpeed(float);
}
class SpriteDescription : cocos2d::CCObject {
	void createDescription(DS_Dictionary*);
	void createDescription(cocos2d::CCDictionary*);
	void getTransformValues(frameValues*);
	void initDescription(DS_Dictionary*);
	void initDescription(cocos2d::CCDictionary*);
}
class SpritePartDelegate {
	void displayFrameChanged(cocos2d::CCObject*, gd::string);
	virtual void displayFrameChanged(cocos2d::CCObject *,gd::string);
}
class StartPosObject : EffectGameObject {
	void create();
	void getSaveString();
	void init();
	void setSettings(LevelSettingsObject*);
}
class StatsCell : TableViewCell {
	void draw();
	void getTitleFromKey(char const*);
	void init();
	void loadFromObject(StatsObject*);
	void updateBGColor(int);
}
class StatsLayer : GJDropDownLayer {
	void create();
	void customSetup();
}
class StatsObject : cocos2d::CCObject {
	void create(char const*, int);
	void init(char const*, int);
}
class SupportLayer : GJDropDownLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void customSetup();
	void exitLayer();
	void onClosePopup(UploadActionPopup*);
	void onCocos2d(cocos2d::CCObject*);
	void onEmail(cocos2d::CCObject*);
	void onFAQ(cocos2d::CCObject*);
	void onGetReward(cocos2d::CCObject*);
	void onLowDetail(cocos2d::CCObject*);
	void onRequestAccess(cocos2d::CCObject*);
	void onRestore(cocos2d::CCObject*);
	void onRobTop(cocos2d::CCObject*);
	void sendSupportMail();
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void FLAlert_Clicked(FLAlertLayer *,bool);
	virtual void onClosePopup(UploadActionPopup *);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class TableView : CCScrollLayerExt, CCScrollLayerExtDelegate {
	void cancelAndStoleTouch(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void cellForRowAtIndexPath(CCIndexPath&);
	void cellForTouch(cocos2d::CCTouch*);
	void checkBoundaryOfCell(TableViewCell*);
	void checkBoundaryOfCell(cocos2d::CCPoint&, float);
	void checkBoundaryOfContent(float);
	void checkFirstCell(TableViewCell*);
	void checkLastCell(TableViewCell*);
	void claimTouch(cocos2d::CCTouch*);
	void create(TableViewDelegate*, TableViewDataSource*, cocos2d::CCRect);
	void deleteTableViewCell(TableViewCell*);
	void dequeueReusableCellWithIdentifier(char const*);
	void initTableViewCells();
	void isDuplicateInVisibleCellArray(CCIndexPath*);
	void isDuplicateIndexPath(CCIndexPath&);
	void onEnter();
	void onExit();
	void registerWithTouchDispatcher();
	void reloadData();
	void removeIndexPathFromPathAddedArray(CCIndexPath&);
	void scrllViewWillBeginDecelerating(CCScrollLayerExt*);
	void scrollToIndexPath(CCIndexPath&);
	void scrollViewDidEndDecelerating(CCScrollLayerExt*);
	void scrollViewDidEndMoving(CCScrollLayerExt*);
	void scrollViewTouchMoving(CCScrollLayerExt*);
	void scrollWheel(float, float);
	void touchFinish(cocos2d::CCTouch*);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt *);
	virtual void scrollViewDidEndDecelerating(CCScrollLayerExt *);
	virtual void scrollViewDidEndMoving(CCScrollLayerExt *);
	virtual void scrollViewTouchMoving(CCScrollLayerExt *);
	virtual void scrollWheel(float,float);
}
class TableViewCell : cocos2d::CCLayer {
	void updateVisibility();
}
class TableViewDataSource {
	void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&);
	void numberOfSectionsInTableView(TableView*);
	virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView *,TableViewCellEditingStyle,CCIndexPath &);
	virtual void numberOfSectionsInTableView(TableView *);
}
class TableViewDelegate {
	void TableViewDidDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*);
	void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*);
	void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*);
	void didEndTweenToIndexPath(CCIndexPath&, TableView*);
	void willTweenToIndexPath(CCIndexPath&, TableViewCell*, TableView*);
	virtual void TableViewDidDisplayCellForRowAtIndexPath(CCIndexPath &,TableViewCell *,TableView *);
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath &,TableViewCell *,TableView *);
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath &,TableViewCell *,TableView *);
	virtual void didEndTweenToIndexPath(CCIndexPath &,TableView *);
	virtual void willTweenToIndexPath(CCIndexPath &,TableViewCell *,TableView *);
}
class TeleportPortalObject : GameObject {
	void addToGroup(int);
	void addToGroup2(int);
	void create(char const*);
	void customObjectSetup(gd::map<gd::string, gd::string, gd::less<gd::string >, gd::allocator<gd::pair<gd::string const, gd::string > > >&);
	void getSaveString();
	void getTeleportXOff(cocos2d::CCNode*);
	void init(char const*);
	void removeFromGroup(int);
	void removeFromGroup2(int);
	void setPosition(cocos2d::CCPoint const&);
	void setPositionOverride(cocos2d::CCPoint);
	void setRotation(float);
	void setRotation2(float);
	void setStartPos(cocos2d::CCPoint);
	void setStartPosOverride(cocos2d::CCPoint);
	virtual void addToGroup(int);
	virtual void addToGroup2(int);
	virtual void customObjectSetup(gd::map<gd::string,gd::string,gd::less<gd::string>,gd::allocator<gd::pair<gd::string const,gd::string>>> &);
	virtual void removeFromGroup(int);
	virtual void removeFromGroup2(int);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void setRotation(float);
	virtual void setRotation2(float);
	virtual void setStartPos(cocos2d::CCPoint);
}
class TextAlertPopup : cocos2d::CCNode {
	void create(gd::string, float, float);
	void init(gd::string, float, float);
}
class TextArea : cocos2d::CCSprite {
	void colorAllCharactersTo(cocos2d::_ccColor3B);
	void create(gd::string, char const*, float, float, cocos2d::CCPoint, float, bool);
	void draw();
	void fadeIn(float, bool);
	void fadeInCharacters(float, float);
	void fadeOut(float);
	void fadeOutAndRemove();
	void finishFade();
	void hideAll();
	void init(gd::string, char const*, float, float, cocos2d::CCPoint, float, bool);
	void setIgnoreColorCode(bool);
	void setOpacity(unsigned char);
	void setString(gd::string);
	void showAll();
	void stopAllCharacterActions();
	virtual void setOpacity(uchar);
}
class TextAreaDelegate {
	void fadeInTextFinished(TextArea*);
	virtual void fadeInTextFinished(TextArea *);
}
class TextInputDelegate {
	void allowTextInput(CCTextInputNode*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	virtual void allowTextInput(CCTextInputNode *);
	virtual void textChanged(CCTextInputNode *);
	virtual void textInputClosed(CCTextInputNode *);
	virtual void textInputOpened(CCTextInputNode *);
	virtual void textInputReturn(CCTextInputNode *);
	virtual void textInputShouldOffset(CCTextInputNode *,float);
}
class tinyxml2::MemPool {
}
class tinyxml2::MemPoolT<36> : tinyxml2::MemPool {
	void Alloc();
	void Free(void*);
	void ItemSize();
	void SetTracked();
	virtual void Free(void *);
}
class tinyxml2::MemPoolT<44> : tinyxml2::MemPool {
	void Alloc();
	void Free(void*);
	void ItemSize();
	void SetTracked();
	virtual void Free(void *);
}
class tinyxml2::MemPoolT<48> : tinyxml2::MemPool {
	void Alloc();
	void Free(void*);
	void ItemSize();
	void SetTracked();
	virtual void Free(void *);
}
class tinyxml2::MemPoolT<52> : tinyxml2::MemPool {
	void Alloc();
	void Free(void*);
	void ItemSize();
	void SetTracked();
	virtual void Free(void *);
}
class tinyxml2::XMLAttribute {
	void ParseDeep(char*, bool);
	void QueryBoolValue(bool*);
	void QueryDoubleValue(double*);
	void QueryFloatValue(float*);
	void QueryIntValue(int*);
	void QueryUnsignedValue(unsigned int*);
	void SetAttribute(bool);
	void SetAttribute(double);
	void SetAttribute(float);
	void SetAttribute(int);
	void SetAttribute(unsigned int);
	void SetAttribute(char const*);
	void SetName(char const*);
}
class tinyxml2::XMLComment : tinyxml2::XMLNode {
	void Accept(tinyxml2::XMLVisitor*);
	void ParseDeep(char*, tinyxml2::StrPair*);
	void ShallowClone(tinyxml2::XMLDocument*);
	void ShallowEqual(tinyxml2::XMLNode const*);
	void ToComment();
	void ToComment();
	void XMLComment(tinyxml2::XMLDocument*);
	virtual void Accept(tinyxml2::XMLVisitor *);
	virtual void ParseDeep(char *,tinyxml2::StrPair *);
	virtual void ShallowClone(tinyxml2::XMLDocument *);
	virtual void ShallowEqual(tinyxml2::XMLNode const*);
}
class tinyxml2::XMLDeclaration : tinyxml2::XMLNode {
	void Accept(tinyxml2::XMLVisitor*);
	void ParseDeep(char*, tinyxml2::StrPair*);
	void ShallowClone(tinyxml2::XMLDocument*);
	void ShallowEqual(tinyxml2::XMLNode const*);
	void ToDeclaration();
	void ToDeclaration();
	void XMLDeclaration(tinyxml2::XMLDocument*);
	virtual void Accept(tinyxml2::XMLVisitor *);
	virtual void ParseDeep(char *,tinyxml2::StrPair *);
	virtual void ShallowClone(tinyxml2::XMLDocument *);
	virtual void ShallowEqual(tinyxml2::XMLNode const*);
}
class tinyxml2::XMLDocument : tinyxml2::XMLNode {
	void Accept(tinyxml2::XMLVisitor*);
	void Identify(char*, tinyxml2::XMLNode**);
	void InitDocument();
	void LoadFile(__sFILE*);
	void LoadFile(char const*);
	void NewComment(char const*);
	void NewDeclaration(char const*);
	void NewElement(char const*);
	void NewText(char const*);
	void NewUnknown(char const*);
	void Parse(char const*, unsigned int);
	void Print(tinyxml2::XMLPrinter*);
	void PrintError();
	void SaveFile(__sFILE*, bool);
	void SaveFile(char const*, bool);
	void SetError(tinyxml2::XMLError, char const*, char const*);
	void ShallowClone(tinyxml2::XMLDocument*);
	void ShallowEqual(tinyxml2::XMLNode const*);
	void ToDocument();
	void ToDocument();
	void XMLDocument(bool, tinyxml2::Whitespace);
	virtual void Accept(tinyxml2::XMLVisitor *);
	virtual void ShallowClone(tinyxml2::XMLDocument*);
	virtual void ShallowEqual(tinyxml2::XMLNode const*);
}
class tinyxml2::XMLElement : tinyxml2::XMLNode {
	void Accept(tinyxml2::XMLVisitor*);
	void Attribute(char const*, char const*);
	void DeleteAttribute(char const*);
	void FindAttribute(char const*);
	void FindAttribute(char const*);
	void FindOrCreateAttribute(char const*);
	void GetText();
	void ParseAttributes(char*);
	void ParseDeep(char*, tinyxml2::StrPair*);
	void QueryBoolText(bool*);
	void QueryDoubleText(double*);
	void QueryFloatText(float*);
	void QueryIntText(int*);
	void QueryUnsignedText(unsigned int*);
	void ShallowClone(tinyxml2::XMLDocument*);
	void ShallowEqual(tinyxml2::XMLNode const*);
	void ToElement();
	void ToElement();
	void XMLElement(tinyxml2::XMLDocument*);
	virtual void Accept(tinyxml2::XMLVisitor *);
	virtual void ParseDeep(char *,tinyxml2::StrPair *);
	virtual void ShallowClone(tinyxml2::XMLDocument *);
	virtual void ShallowEqual(tinyxml2::XMLNode const*);
}
class tinyxml2::XMLNode {
	void DeleteChild(tinyxml2::XMLNode*);
	void DeleteChildren();
	void FirstChildElement(char const*);
	void InsertAfterChild(tinyxml2::XMLNode*, tinyxml2::XMLNode*);
	void InsertEndChild(tinyxml2::XMLNode*);
	void InsertFirstChild(tinyxml2::XMLNode*);
	void LastChildElement(char const*);
	void NextSiblingElement(char const*);
	void ParseDeep(char*, tinyxml2::StrPair*);
	void PreviousSiblingElement(char const*);
	void SetValue(char const*, bool);
	void ToComment();
	void ToComment();
	void ToDeclaration();
	void ToDeclaration();
	void ToDocument();
	void ToDocument();
	void ToElement();
	void ToElement();
	void ToText();
	void ToText();
	void ToUnknown();
	void ToUnknown();
	void Unlink(tinyxml2::XMLNode*);
	void XMLNode(tinyxml2::XMLDocument*);
	virtual void ParseDeep(char *,tinyxml2::StrPair *);
}
class tinyxml2::XMLPrinter : tinyxml2::XMLVisitor {
	void CloseElement();
	void OpenElement(char const*);
	void Print(char const*, ...);
	void PrintSpace(int);
	void PrintString(char const*, bool);
	void PushAttribute(char const*, bool);
	void PushAttribute(char const*, double);
	void PushAttribute(char const*, int);
	void PushAttribute(char const*, unsigned int);
	void PushAttribute(char const*, char const*);
	void PushComment(char const*);
	void PushDeclaration(char const*);
	void PushHeader(bool, bool);
	void PushText(bool);
	void PushText(double);
	void PushText(float);
	void PushText(int);
	void PushText(unsigned int);
	void PushText(char const*, bool);
	void PushUnknown(char const*);
	void SealElement();
	void Visit(tinyxml2::XMLComment const&);
	void Visit(tinyxml2::XMLUnknown const&);
	void Visit(tinyxml2::XMLDeclaration const&);
	void Visit(tinyxml2::XMLText const&);
	void VisitEnter(tinyxml2::XMLElement const&, tinyxml2::XMLAttribute const*);
	void VisitEnter(tinyxml2::XMLDocument const&);
	void VisitExit(tinyxml2::XMLElement const&);
	void VisitExit(tinyxml2::XMLDocument const&);
	void XMLPrinter(__sFILE*, bool);
	virtual void Visit(tinyxml2::XMLDeclaration const&);
	virtual void Visit(tinyxml2::XMLText const&);
	virtual void Visit(tinyxml2::XMLComment const&);
	virtual void Visit(tinyxml2::XMLUnknown const&);
	virtual void VisitEnter(tinyxml2::XMLDocument const&);
	virtual void VisitEnter(tinyxml2::XMLElement const&,tinyxml2::XMLAttribute const*);
	virtual void VisitExit(tinyxml2::XMLDocument const&);
	virtual void VisitExit(tinyxml2::XMLElement const&);
}
class tinyxml2::XMLText : tinyxml2::XMLNode {
	void Accept(tinyxml2::XMLVisitor*);
	void ParseDeep(char*, tinyxml2::StrPair*);
	void ShallowClone(tinyxml2::XMLDocument*);
	void ShallowEqual(tinyxml2::XMLNode const*);
	void ToText();
	void ToText();
	virtual void Accept(tinyxml2::XMLVisitor *);
	virtual void ParseDeep(char *,tinyxml2::StrPair *);
	virtual void ShallowClone(tinyxml2::XMLDocument *);
	virtual void ShallowEqual(tinyxml2::XMLNode const*);
}
class tinyxml2::XMLUnknown : tinyxml2::XMLNode {
	void Accept(tinyxml2::XMLVisitor*);
	void ParseDeep(char*, tinyxml2::StrPair*);
	void ShallowClone(tinyxml2::XMLDocument*);
	void ShallowEqual(tinyxml2::XMLNode const*);
	void ToUnknown();
	void ToUnknown();
	void XMLUnknown(tinyxml2::XMLDocument*);
	virtual void Accept(tinyxml2::XMLVisitor *);
	virtual void ParseDeep(char *,tinyxml2::StrPair *);
	virtual void ShallowClone(tinyxml2::XMLDocument *);
	virtual void ShallowEqual(tinyxml2::XMLNode const*);
}
class tinyxml2::XMLVisitor {
	void Visit(tinyxml2::XMLComment const&);
	void Visit(tinyxml2::XMLUnknown const&);
	void Visit(tinyxml2::XMLDeclaration const&);
	void Visit(tinyxml2::XMLText const&);
	void VisitEnter(tinyxml2::XMLElement const&, tinyxml2::XMLAttribute const*);
	void VisitEnter(tinyxml2::XMLDocument const&);
	void VisitExit(tinyxml2::XMLElement const&);
	void VisitExit(tinyxml2::XMLDocument const&);
	virtual void Visit(tinyxml2::XMLDeclaration const&);
	virtual void Visit(tinyxml2::XMLText const&);
	virtual void Visit(tinyxml2::XMLComment const&);
	virtual void Visit(tinyxml2::XMLUnknown const&);
	virtual void VisitEnter(tinyxml2::XMLDocument const&);
	virtual void VisitEnter(tinyxml2::XMLElement const&,tinyxml2::XMLAttribute const*);
	virtual void VisitExit(tinyxml2::XMLDocument const&);
	virtual void VisitExit(tinyxml2::XMLElement const&);
}
class ToggleTriggerAction : cocos2d::CCNode {
	void create(int, bool);
	void createFromString(gd::string);
	void getSaveString();
	void init(int, bool);
}
class TopArtistsLayer : FLAlertLayer, OnlineListDelegate {
	void create();
	void init();
	void isCorrect(char const*);
	void keyBackClicked();
	void loadListFailed(char const*);
	void loadListFinished(cocos2d::CCArray*, char const*);
	void loadPage(int);
	void onClose(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void setupLeaderboard(cocos2d::CCArray*);
	void setupPageInfo(gd::string, char const*);
	void show();
	void updateLevelsLabel();
	virtual void loadListFailed(char const*);
	virtual void loadListFinished(cocos2d::CCArray *,char const*);
	virtual void setupPageInfo(gd::string,char const*);
}
class TouchToggleAction : cocos2d::CCNode {
	void create(int, bool, TouchTriggerType);
	void createFromString(gd::string);
	void getSaveString();
	void init(int, bool, TouchTriggerType);
}
class TriggerEffectDelegate {
}
class TutorialLayer : FLAlertLayer {
	void create();
	void init();
	void keyBackClicked();
	void loadPage(int);
	void onClose(cocos2d::CCObject*);
	void onNext(cocos2d::CCObject*);
	void removeTutorialTexture();
}
class TutorialPopup : FLAlertLayer {
	void animateMenu();
	void closeTutorial(cocos2d::CCObject*);
	void create(gd::string);
	void init(gd::string);
	void keyBackClicked();
	void registerForCallback(cocos2d::SEL_MenuHandler, cocos2d::CCNode*);
	void show();
}
class UILayer : cocos2d::CCLayerColor, cocos2d::CCKeyboardDelegate {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void disableMenu();
	void draw();
	void enableMenu();
	void init();
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void keyUp(cocos2d::enumKeyCodes);
	void onCheck(cocos2d::CCObject*);
	void onDeleteCheck(cocos2d::CCObject*);
	void onPause(cocos2d::CCObject*);
	void pCommand(cocos2d::CCNode*);
	void registerWithTouchDispatcher();
	void toggleCheckpointsMenu(bool);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void keyUp(cocos2d::enumKeyCodes);
}
class UndoObject : cocos2d::CCObject {
	void create(GameObject*, UndoCommand);
	void createWithArray(cocos2d::CCArray*, UndoCommand);
	void createWithTransformObjects(cocos2d::CCArray*, UndoCommand);
	void init(GameObject*, UndoCommand);
	void init(cocos2d::CCArray*, UndoCommand);
	void initWithTransformObjects(cocos2d::CCArray*, UndoCommand);
	void setObjects(cocos2d::CCArray*);
}
class UpdateAccountSettingsPopup : FLAlertLayer, GJAccountSettingsDelegate {
	void create(GJAccountSettingsLayer*, int, int, int, gd::string, gd::string, gd::string);
	void init(GJAccountSettingsLayer*, int, int, int, gd::string, gd::string, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void updateSettingsFailed();
	void updateSettingsFinished();
}
class UploadActionDelegate {
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
	virtual void uploadActionFailed(int,int);
	virtual void uploadActionFinished(int,int);
}
class UploadActionPopup : FLAlertLayer {
	void closePopup();
	void create(UploadPopupDelegate*, gd::string);
	void init(UploadPopupDelegate*, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void showFailMessage(gd::string);
	void showSuccessMessage(gd::string);
}
class UploadMessageDelegate {
	void uploadMessageFailed(int);
	void uploadMessageFinished(int);
	virtual void uploadMessageFailed(int);
	virtual void uploadMessageFinished(int);
}
class UploadPopup : FLAlertLayer, LevelUploadDelegate {
	void create(GJGameLevel*);
	void init(GJGameLevel*);
	void keyBackClicked();
	void levelUploadFailed(GJGameLevel*);
	void levelUploadFinished(GJGameLevel*);
	void onBack(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onReturnToLevel(cocos2d::CCObject*);
	void show();
	virtual void levelUploadFailed(GJGameLevel *);
	virtual void levelUploadFinished(GJGameLevel *);
}
class UploadPopupDelegate {
	void onClosePopup(UploadActionPopup*);
	virtual void onClosePopup(UploadActionPopup *);
}
class UserInfoDelegate {
	void getUserInfoFailed(int);
	void getUserInfoFinished(GJUserScore*);
	void userInfoChanged(GJUserScore*);
	virtual void getUserInfoFailed(int);
	virtual void getUserInfoFinished(GJUserScore *);
	virtual void userInfoChanged(GJUserScore *);
}
class UserListDelegate {
	void forceReloadList(UserListType);
	void getUserListFailed(UserListType, GJErrorCode);
	void getUserListFinished(cocos2d::CCArray*, UserListType);
	void userListChanged(cocos2d::CCArray*, UserListType);
	virtual void forceReloadList(UserListType);
	virtual void getUserListFailed(UserListType,GJErrorCode);
	virtual void getUserListFinished(cocos2d::CCArray *,UserListType);
	virtual void userListChanged(cocos2d::CCArray *,UserListType);
}
class VideoOptionsLayer : FLAlertLayer {
	void create();
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, float, float, bool);
	void init();
	void keyBackClicked();
	void onAdvanced(cocos2d::CCObject*);
	void onApply(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onFullscreen(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onResolutionNext(cocos2d::CCObject*);
	void onResolutionPrev(cocos2d::CCObject*);
	void onTextureQualityNext(cocos2d::CCObject*);
	void onTextureQualityPrev(cocos2d::CCObject*);
	void toggleResolution();
	void updateResolution(int);
	void updateTextureQuality(int);
}
class WorldLevelPage : FLAlertLayer {
	void create(GJGameLevel*, GJWorldNode*);
	void init(GJGameLevel*, GJWorldNode*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onInfo(cocos2d::CCObject*);
	void onPlay(cocos2d::CCObject*);
	void onSong(cocos2d::CCObject*);
	void show();
}
class WorldSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate {
	void animateInActiveIsland();
	void colorForPage(int);
	void create(int);
	void getColorValue(int, int, float);
	void goToPage(int, bool);
	void init(int);
	void keyBackClicked();
	void onBack(cocos2d::CCObject*);
	void onExit();
	void onGarage(cocos2d::CCObject*);
	void onNext(cocos2d::CCObject*);
	void onPrev(cocos2d::CCObject*);
	void scene(int);
	void scrollLayerMoved(cocos2d::CCPoint);
	void scrollLayerScrolledToPage(BoomScrollLayer*, int);
	void scrollLayerWillScrollToPage(BoomScrollLayer*, int);
	void setupWorlds();
	void showCompleteDialog();
	void tryShowAd();
	void unblockButtons();
	void updateArrows();
	virtual void scrollLayerMoved(cocos2d::CCPoint);
	virtual void scrollLayerScrolledToPage(BoomScrollLayer *,int);
	virtual void scrollLayerWillScrollToPage(BoomScrollLayer *,int);
}
class xml_string_writer : pugi::xml_writer {
	void write(void const*, unsigned int);
	virtual void write(void const*,uint);
}
class cocos2d::CCAccelAmplitude : cocos2d::CCActionInterval {
	void create(cocos2d::CCAction*, float);
	void initWithAction(cocos2d::CCAction*, float);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCAccelDeccelAmplitude : cocos2d::CCActionInterval {
	void create(cocos2d::CCAction*, float);
	void initWithAction(cocos2d::CCAction*, float);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCAccelerometerDelegate {
	void didAccelerate(cocos2d::CCAcceleration*);
	virtual void didAccelerate(cocos2d::CCAcceleration *);
}
class cocos2d::CCAction : cocos2d::CCObject {
	void CCAction();
	void copyWithZone(cocos2d::CCZone*);
	void create();
	void description();
	void isDone();
	void setTag(int);
	void startWithTarget(cocos2d::CCNode*);
	void step(float);
	void stop();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void setTag(int);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void step(float);
	virtual void update(float);
}
class cocos2d::CCActionCamera : cocos2d::CCActionInterval {
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	virtual void startWithTarget(cocos2d::CCNode *);
}
class cocos2d::CCActionEase : cocos2d::CCActionInterval {
	void CCActionEase();
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void getInnerAction();
	void initWithAction(cocos2d::CCActionInterval*);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void stop();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCActionInstant : cocos2d::CCFiniteTimeAction {
	void CCActionInstant();
	void copyWithZone(cocos2d::CCZone*);
	void isDone();
	void reverse();
	void step(float);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void step(float);
	virtual void update(float);
}
class cocos2d::CCActionInterval : cocos2d::CCFiniteTimeAction {
	void CCActionInterval();
	void copyWithZone(cocos2d::CCZone*);
	void create(float);
	void getAmplitudeRate();
	void initWithDuration(float);
	void isDone();
	void reverse();
	void setAmplitudeRate(float);
	void startWithTarget(cocos2d::CCNode*);
	void step(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void step(float);
}
class cocos2d::CCActionManager : cocos2d::CCObject {
	void CCActionManager();
	void actionAllocWithHashElement(cocos2d::_hashElement*);
	void addAction(cocos2d::CCAction*, cocos2d::CCNode*, bool);
	void deleteHashElement(cocos2d::_hashElement*);
	void getActionByTag(unsigned int, cocos2d::CCObject*);
	void numberOfRunningActionsInTarget(cocos2d::CCObject*);
	void pauseAllRunningActions();
	void pauseTarget(cocos2d::CCObject*);
	void removeAction(cocos2d::CCAction*);
	void removeActionAtIndex(unsigned int, cocos2d::_hashElement*);
	void removeActionByTag(unsigned int, cocos2d::CCObject*);
	void removeAllActions();
	void removeAllActionsFromTarget(cocos2d::CCObject*);
	void resumeTarget(cocos2d::CCObject*);
	void resumeTargets(cocos2d::CCSet*);
	void update(float);
	virtual void update(float);
}
class cocos2d::CCActionTween : cocos2d::CCActionInterval {
	void create(float, char const*, float, float);
	void initWithDuration(float, char const*, float, float);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCActionTweenDelegate {
}
class cocos2d::CCAnimate : cocos2d::CCActionInterval {
	void CCAnimate();
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCAnimation*);
	void initWithAnimation(cocos2d::CCAnimation*);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void stop();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCAnimation : cocos2d::CCObject {
	void CCAnimation();
	void addSpriteFrame(cocos2d::CCSpriteFrame*);
	void addSpriteFrameWithFileName(char const*);
	void addSpriteFrameWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCArray*, float, unsigned int);
	void create();
	void createWithSpriteFrames(cocos2d::CCArray*, float);
	void getDuration();
	void init();
	void initWithAnimationFrames(cocos2d::CCArray*, float, unsigned int);
	void initWithSpriteFrames(cocos2d::CCArray*, float);
	void setFrames(cocos2d::CCArray*);
	virtual void copyWithZone(cocos2d::CCZone *);
}
class cocos2d::CCAnimationCache : cocos2d::CCObject {
	void CCAnimationCache();
	void addAnimation(cocos2d::CCAnimation*, char const*);
	void addAnimationsWithDictionary(cocos2d::CCDictionary*, char const*);
	void addAnimationsWithFile(char const*);
	void animationByName(char const*);
	void init();
	void parseVersion1(cocos2d::CCDictionary*);
	void parseVersion2(cocos2d::CCDictionary*);
	void purgeSharedAnimationCache();
	void removeAnimationByName(char const*);
	void sharedAnimationCache();
}
class cocos2d::CCAnimationFrame : cocos2d::CCObject {
	void CCAnimationFrame();
	void copyWithZone(cocos2d::CCZone*);
	void initWithSpriteFrame(cocos2d::CCSpriteFrame*, float, cocos2d::CCDictionary*);
	virtual void copyWithZone(cocos2d::CCZone *);
}
class cocos2d::CCApplication : cocos2d::CCApplicationProtocol {
	void CCApplication();
	void getCurrentLanguage();
	void getTargetPlatform();
	void openURL(char const*);
	void run();
	void setAnimationInterval(double);
	void sharedApplication();
	virtual void openURL(char const*);
	virtual void setAnimationInterval(double);
}
class cocos2d::CCApplicationProtocol {
	void applicationWillBecomeActive();
	void applicationWillResignActive();
	void gameDidSave();
	void openURL(char const*);
	void trySaveGame();
	virtual void openURL(char const*);
}
class cocos2d::CCArray : cocos2d::CCObject {
	void CCArray(unsigned int);
	void CCArray();
	void acceptVisitor(cocos2d::CCDataVisitor&);
	void addObject(cocos2d::CCObject*);
	void addObjectNew(cocos2d::CCObject*);
	void addObjectsFromArray(cocos2d::CCArray*);
	void capacity();
	void containsObject(cocos2d::CCObject*);
	void copyWithZone(cocos2d::CCZone*);
	void count();
	void create(cocos2d::CCObject*, ...);
	void create();
	void createWithArray(cocos2d::CCArray*);
	void createWithCapacity(unsigned int);
	void createWithContentsOfFile(char const*);
	void createWithContentsOfFileThreadSafe(char const*);
	void createWithObject(cocos2d::CCObject*);
	void exchangeObject(cocos2d::CCObject*, cocos2d::CCObject*);
	void exchangeObjectAtIndex(unsigned int, unsigned int);
	void fastRemoveObject(cocos2d::CCObject*);
	void fastRemoveObjectAtIndex(unsigned int);
	void fastRemoveObjectAtIndexNew(unsigned int);
	void indexOfObject(cocos2d::CCObject*);
	void init();
	void initWithArray(cocos2d::CCArray*);
	void initWithCapacity(unsigned int);
	void initWithObject(cocos2d::CCObject*);
	void initWithObjects(cocos2d::CCObject*, ...);
	void insertObject(cocos2d::CCObject*, unsigned int);
	void isEqualToArray(cocos2d::CCArray*);
	void lastObject();
	void objectAtIndex(unsigned int);
	void randomObject();
	void reduceMemoryFootprint();
	void removeAllObjects();
	void removeLastObject(bool);
	void removeObject(cocos2d::CCObject*, bool);
	void removeObjectAtIndex(unsigned int, bool);
	void removeObjectsInArray(cocos2d::CCArray*);
	void replaceObjectAtIndex(unsigned int, cocos2d::CCObject*, bool);
	void reverseObjects();
	void stringAtIndex(unsigned int);
	virtual void acceptVisitor(cocos2d::CCDataVisitor &);
	virtual void copyWithZone(cocos2d::CCZone *);
}
class cocos2d::CCAtlasNode : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	void CCAtlasNode();
	void calculateMaxItems();
	void create(char const*, unsigned int, unsigned int, unsigned int);
	void draw();
	void getBlendFunc();
	void getColor();
	void getQuadsToDraw();
	void getTexture();
	void getTextureAtlas();
	void initWithTexture(cocos2d::CCTexture2D*, unsigned int, unsigned int, unsigned int);
	void initWithTileFile(char const*, unsigned int, unsigned int, unsigned int);
	void isOpacityModifyRGB();
	void setBlendFunc(cocos2d::_ccBlendFunc);
	void setColor(cocos2d::_ccColor3B const&);
	void setIgnoreContentScaleFactor(bool);
	void setOpacity(unsigned char);
	void setOpacityModifyRGB(bool);
	void setQuadsToDraw(unsigned int);
	void setTexture(cocos2d::CCTexture2D*);
	void setTextureAtlas(cocos2d::CCTextureAtlas*);
	void updateAtlasValues();
	void updateBlendFunc();
	void updateOpacityModifyRGB();
	virtual void setBlendFunc(cocos2d::_ccBlendFunc);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setOpacity(uchar);
	virtual void setOpacityModifyRGB(bool);
	virtual void setQuadsToDraw(uint);
	virtual void setTexture(cocos2d::CCTexture2D *);
	virtual void setTextureAtlas(cocos2d::CCTextureAtlas *);
}
class cocos2d::CCAutoreleasePool : cocos2d::CCObject {
	void CCAutoreleasePool();
	void addObject(cocos2d::CCObject*);
	void clear();
	void removeObject(cocos2d::CCObject*);
}
class cocos2d::CCBezierBy : cocos2d::CCActionInterval {
	void CCBezierBy();
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::_ccBezierConfig const&);
	void initWithDuration(float, cocos2d::_ccBezierConfig const&);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCBezierTo : cocos2d::CCBezierBy {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::_ccBezierConfig const&);
	void initWithDuration(float, cocos2d::_ccBezierConfig const&);
	void startWithTarget(cocos2d::CCNode*);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
}
class cocos2d::CCBlendProtocol {
}
class cocos2d::CCBlink : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, unsigned int);
	void initWithDuration(float, unsigned int);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void stop();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCBMFontConfiguration : cocos2d::CCObject {
	void CCBMFontConfiguration();
	void create(char const*);
	void description();
	void getCharacterSet();
	void initWithFNTfile(char const*);
	void parseCharacterDefinition(gd::string, cocos2d::_BMFontDef*);
	void parseCommonArguments(gd::string);
	void parseConfigFile(char const*);
	void parseImageFileName(gd::string, char const*);
	void parseInfoArguments(gd::string);
	void parseKerningEntry(gd::string);
	void purgeFontDefDictionary();
	void purgeKerningDictionary();
}
class cocos2d::CCBool : cocos2d::CCObject {
	void acceptVisitor(cocos2d::CCDataVisitor&);
	void create(bool);
	virtual void acceptVisitor(cocos2d::CCDataVisitor &);
}
class cocos2d::CCCallFunc : cocos2d::CCActionInstant {
	void CCCallFunc();
	void copyWithZone(cocos2d::CCZone*);
	void create(int);
	void create(cocos2d::CCObject*, void (cocos2d::CCObject::*)());
	void execute();
	void initWithTarget(cocos2d::CCObject*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithTarget(cocos2d::CCObject *);
	virtual void update(float);
}
class cocos2d::CCCallFuncN : cocos2d::CCCallFunc, cocos2d::TypeInfo {
	void copyWithZone(cocos2d::CCZone*);
	void create(int);
	void create(cocos2d::CCObject*, cocos2d::SEL_CallFuncN);
	void execute();
	void getClassTypeInfo();
	void getClassTypeInfo();
	void initWithTarget(cocos2d::CCObject*, cocos2d::SEL_CallFuncN);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithTarget(cocos2d::CCObject *,void (cocos2d::CCObject::*)(cocos2d::CCNode *));
}
class cocos2d::CCCallFuncND : cocos2d::CCCallFuncN {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCObject*, cocos2d::SEL_CallFuncND, void*);
	void execute();
	void getClassTypeInfo();
	void getClassTypeInfo();
	void initWithTarget(cocos2d::CCObject*, cocos2d::SEL_CallFuncND, void*);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithTarget(cocos2d::CCObject *,void (cocos2d::CCObject::*)(cocos2d::CCNode *,void *),void *);
}
class cocos2d::CCCallFuncO : cocos2d::CCCallFunc, cocos2d::TypeInfo {
	void CCCallFuncO();
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCObject*);
	void execute();
	void getClassTypeInfo();
	void getClassTypeInfo();
	void initWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCObject*);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithTarget(cocos2d::CCObject *,void (cocos2d::CCObject::*)(cocos2d::CCObject *),cocos2d::CCObject *);
}
class cocos2d::CCCamera : cocos2d::CCObject {
	void CCCamera();
	void description();
	void getCenterXYZ(float*, float*, float*);
	void getEyeXYZ(float*, float*, float*);
	void getUpXYZ(float*, float*, float*);
	void getZEye();
	void init();
	void locate();
	void restore();
	void setCenterXYZ(float, float, float);
	void setEyeXYZ(float, float, float);
	void setUpXYZ(float, float, float);
}
class cocos2d::CCCardinalSplineBy : cocos2d::CCCardinalSplineTo {
	void CCCardinalSplineBy();
	void create(float, cocos2d::CCPointArray*, float);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void updatePosition(cocos2d::CCPoint&);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void updatePosition(cocos2d::CCPoint &);
}
class cocos2d::CCCardinalSplineTo : cocos2d::CCActionInterval {
	void CCCardinalSplineTo();
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCPointArray*, float);
	void initWithDuration(float, cocos2d::CCPointArray*, float);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	void updatePosition(cocos2d::CCPoint&);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
	virtual void updatePosition(cocos2d::CCPoint &);
}
class cocos2d::CCCatmullRomBy : cocos2d::CCCardinalSplineBy {
	void create(float, cocos2d::CCPointArray*);
	void initWithDuration(float, cocos2d::CCPointArray*);
}
class cocos2d::CCCatmullRomTo : cocos2d::CCCardinalSplineTo {
	void create(float, cocos2d::CCPointArray*);
	void initWithDuration(float, cocos2d::CCPointArray*);
}
class cocos2d::CCClippingNode : cocos2d::CCNode {
	void CCClippingNode();
	void create(cocos2d::CCNode*);
	void create();
	void getAlphaThreshold();
	void getStencil();
	void init(cocos2d::CCNode*);
	void init();
	void isInverted();
	void onEnter();
	void onEnterTransitionDidFinish();
	void onExit();
	void onExitTransitionDidStart();
	void setAlphaThreshold(float);
	void setInverted(bool);
	void setStencil(cocos2d::CCNode*);
	void visit();
	virtual void init(cocos2d::CCNode *);
}
class cocos2d::CCComponent : cocos2d::CCObject {
	void CCComponent();
	void create();
	void getName();
	void getOwner();
	void init();
	void isEnabled();
	void onEnter();
	void onExit();
	void serialize(void*);
	void setEnabled(bool);
	void setName(char const*);
	void setOwner(cocos2d::CCNode*);
	void update(float);
	virtual void serialize(void *);
	virtual void setEnabled(bool);
	virtual void update(float);
}
class cocos2d::CCComponentContainer {
	void CCComponentContainer(cocos2d::CCNode*);
	void add(cocos2d::CCComponent*);
	void alloc();
	void get(char const*);
	void isEmpty();
	void remove(char const*);
	void remove(cocos2d::CCComponent*);
	void removeAll();
	void visit(float);
	virtual void add(cocos2d::CCComponent *);
	virtual void get(char const*);
	virtual void remove(char const*);
	virtual void remove(cocos2d::CCComponent *);
	virtual void visit(float);
}
class cocos2d::CCConfiguration : cocos2d::CCObject {
	void CCConfiguration();
	void checkForGLExtension(gd::string const&);
	void dumpInfo();
	void gatherGPUInfo();
	void getBool(char const*, bool);
	void getCString(char const*, char const*);
	void getMaxModelviewStackDepth();
	void getMaxTextureSize();
	void getMaxTextureUnits();
	void getNumber(char const*, double);
	void getObject(char const*);
	void init();
	void loadConfigFile(char const*);
	void purgeConfiguration();
	void setObject(char const*, cocos2d::CCObject*);
	void sharedConfiguration();
	void supportsBGRA8888();
	void supportsDiscardFramebuffer();
	void supportsNPOT();
	void supportsPVRTC();
	void supportsShareableVAO();
}
class cocos2d::CCCopying {
	void copyWithZone(cocos2d::CCZone*);
	virtual void copyWithZone(cocos2d::CCZone *);
}
class cocos2d::CCDataVisitor {
	void visit(cocos2d::CCDictionary const*);
	void visit(cocos2d::CCSet const*);
	void visit(cocos2d::CCBool const*);
	void visit(cocos2d::CCArray const*);
	void visit(cocos2d::CCFloat const*);
	void visit(cocos2d::CCDouble const*);
	void visit(cocos2d::CCString const*);
	void visit(cocos2d::CCInteger const*);
	virtual void visit(cocos2d::CCBool const*);
	virtual void visit(cocos2d::CCInteger const*);
	virtual void visit(cocos2d::CCFloat const*);
	virtual void visit(cocos2d::CCDouble const*);
	virtual void visit(cocos2d::CCString const*);
	virtual void visit(cocos2d::CCArray const*);
	virtual void visit(cocos2d::CCDictionary const*);
	virtual void visit(cocos2d::CCSet const*);
}
class cocos2d::CCDeccelAmplitude : cocos2d::CCActionInterval {
	void create(cocos2d::CCAction*, float);
	void initWithAction(cocos2d::CCAction*, float);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCDelayTime : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCDictionary : cocos2d::CCObject {
	void CCDictionary();
	void acceptVisitor(cocos2d::CCDataVisitor&);
	void allKeys();
	void allKeysForObject(cocos2d::CCObject*);
	void charForKey(gd::string const&);
	void copyWithZone(cocos2d::CCZone*);
	void count();
	void create();
	void createWithContentsOfFile(char const*);
	void createWithContentsOfFileThreadSafe(char const*);
	void createWithDictionary(cocos2d::CCDictionary*);
	void getFirstKey();
	void objectForKey(int);
	void objectForKey(gd::string const&);
	void randomObject();
	void removeAllObjects();
	void removeObjectForElememt(cocos2d::CCDictElement*);
	void removeObjectForKey(int);
	void removeObjectForKey(gd::string const&);
	void removeObjectsForKeys(cocos2d::CCArray*);
	void setObject(cocos2d::CCObject*, int);
	void setObject(cocos2d::CCObject*, gd::string const&);
	void setObjectUnSafe(cocos2d::CCObject*, int);
	void setObjectUnSafe(cocos2d::CCObject*, gd::string const&);
	void valueForKey(int);
	void valueForKey(gd::string const&);
	void writeToFile(char const*);
	virtual void acceptVisitor(cocos2d::CCDataVisitor &);
	virtual void copyWithZone(cocos2d::CCZone *);
}
class cocos2d::CCDictMaker : cocos2d::CCSAXDelegator {
	void CCDictMaker();
	void endElement(void*, char const*);
	void startElement(void*, char const*, char const**);
	void textHandler(void*, char const*, int);
	virtual void endElement(void *,char const*);
	virtual void startElement(void *,char const*,char const**);
	virtual void textHandler(void *,char const*,int);
}
class cocos2d::CCDirector : cocos2d::CCObject, cocos2d::TypeInfo {
	void CCDirector();
	void calculateDeltaTime();
	void calculateMPF();
	void checkSceneReference();
	void convertToGL(cocos2d::CCPoint const&);
	void convertToUI(cocos2d::CCPoint const&);
	void createStatsLabel();
	void drawScene();
	void end();
	void getAccelerometer();
	void getActionManager();
	void getClassTypeInfo();
	void getClassTypeInfo();
	void getContentScaleFactor();
	void getDelegate();
	void getDeltaTime();
	void getFPSImageData(unsigned char**, unsigned int*);
	void getKeyboardDispatcher();
	void getKeypadDispatcher();
	void getMouseDispatcher();
	void getNotificationNode();
	void getSceneDelegate();
	void getScheduler();
	void getScreenBottom();
	void getScreenLeft();
	void getScreenRight();
	void getScreenScaleFactor();
	void getScreenScaleFactorH();
	void getScreenScaleFactorMax();
	void getScreenScaleFactorW();
	void getScreenTop();
	void getTouchDispatcher();
	void getVisibleOrigin();
	void getVisibleSize();
	void getWinSize();
	void getWinSizeInPixels();
	void getZEye();
	void init();
	void levelForSceneInStack(cocos2d::CCScene*);
	void pause();
	void popScene();
	void popSceneWithTransition(float, cocos2d::PopTransition);
	void popToRootScene();
	void popToSceneInStack(cocos2d::CCScene*);
	void popToSceneStackLevel(int);
	void purgeCachedData();
	void purgeDirector();
	void pushScene(cocos2d::CCScene*);
	void removeStatsLabel();
	void replaceScene(cocos2d::CCScene*);
	void resetSmoothFixCounter();
	void reshapeProjection(cocos2d::CCSize const&);
	void resume();
	void runWithScene(cocos2d::CCScene*);
	void sceneCount();
	void setAccelerometer(cocos2d::CCAccelerometer*);
	void setActionManager(cocos2d::CCActionManager*);
	void setAlphaBlending(bool);
	void setContentScaleFactor(float);
	void setDefaultValues();
	void setDelegate(cocos2d::CCDirectorDelegate*);
	void setDeltaTime(float);
	void setDepthTest(bool);
	void setGLDefaultValues();
	void setKeyboardDispatcher(cocos2d::CCKeyboardDispatcher*);
	void setKeypadDispatcher(cocos2d::CCKeypadDispatcher*);
	void setMouseDispatcher(cocos2d::CCMouseDispatcher*);
	void setNextDeltaTimeZero(bool);
	void setNextScene();
	void setNotificationNode(cocos2d::CCNode*);
	void setOpenGLView(cocos2d::CCEGLView*);
	void setProjection(cocos2d::ccDirectorProjection);
	void setSceneDelegate(cocos2d::CCSceneDelegate*);
	void setScheduler(cocos2d::CCScheduler*);
	void setTouchDispatcher(cocos2d::CCTouchDispatcher*);
	void setViewport();
	void setupScreenScale(cocos2d::CCSize, cocos2d::CCSize, cocos2d::TextureQuality);
	void sharedDirector();
	void showStats();
	void updateContentScale(cocos2d::TextureQuality);
	void updateScreenScale(cocos2d::CCSize);
	void willSwitchToScene(cocos2d::CCScene*);
	virtual void setAccelerometer(cocos2d::CCAccelerometer *);
	virtual void setActionManager(cocos2d::CCActionManager *);
	virtual void setKeyboardDispatcher(cocos2d::CCKeyboardDispatcher *);
	virtual void setKeypadDispatcher(cocos2d::CCKeypadDispatcher *);
	virtual void setMouseDispatcher(cocos2d::CCMouseDispatcher *);
	virtual void setSceneDelegate(cocos2d::CCSceneDelegate *);
	virtual void setScheduler(cocos2d::CCScheduler *);
	virtual void setTouchDispatcher(cocos2d::CCTouchDispatcher *);
}
class cocos2d::CCDisplayLinkDirector : cocos2d::CCDirector {
	void mainLoop();
	void setAnimationInterval(double);
	void startAnimation();
	void stopAnimation();
	virtual void setAnimationInterval(double);
}
class cocos2d::CCDrawNode : cocos2d::CCNodeRGBA {
	void CCDrawNode();
	void clear();
	void create();
	void draw();
	void drawDot(cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&);
	void drawPolygon(cocos2d::CCPoint*, unsigned int, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&);
	void drawSegment(cocos2d::CCPoint const&, cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&);
	void ensureCapacity(unsigned int);
	void getBlendFunc();
	void init();
	void listenBackToForeground(cocos2d::CCObject*);
	void render();
	void setBlendFunc(cocos2d::_ccBlendFunc const&);
}
class cocos2d::CCEaseBackIn : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseBackInOut : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseBackOut : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseBounce : cocos2d::CCActionEase {
	void bounceTime(float);
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	virtual void copyWithZone(cocos2d::CCZone *);
}
class cocos2d::CCEaseBounceIn : cocos2d::CCEaseBounce {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseBounceInOut : cocos2d::CCEaseBounce {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseBounceOut : cocos2d::CCEaseBounce {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseElastic : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void create(cocos2d::CCActionInterval*, float);
	void initWithAction(cocos2d::CCActionInterval*, float);
	void reverse();
	virtual void copyWithZone(cocos2d::CCZone *);
}
class cocos2d::CCEaseElasticIn : cocos2d::CCEaseElastic {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void create(cocos2d::CCActionInterval*, float);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseElasticInOut : cocos2d::CCEaseElastic {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void create(cocos2d::CCActionInterval*, float);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseElasticOut : cocos2d::CCEaseElastic {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void create(cocos2d::CCActionInterval*, float);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseExponentialIn : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseExponentialInOut : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseExponentialOut : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseIn : cocos2d::CCEaseRateAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*, float);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseInOut : cocos2d::CCEaseRateAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*, float);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseOut : cocos2d::CCEaseRateAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*, float);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseRateAction : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*, float);
	void initWithAction(cocos2d::CCActionInterval*, float);
	void reverse();
	virtual void copyWithZone(cocos2d::CCZone *);
}
class cocos2d::CCEaseSineIn : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseSineInOut : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEaseSineOut : cocos2d::CCActionEase {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCEGLView : cocos2d::CCEGLViewProtocol {
	void CCEGLView();
	void end();
	void isOpenGLReady();
	void setIMEKeyboardState(bool);
	void sharedOpenGLView();
	void swapBuffers();
	virtual void otocol::handleTouchesBegin(int,int *,float *,float *);
	virtual void otocol::handleTouchesCancel(int,int *,float *,float *);
	virtual void otocol::handleTouchesEnd(int,int *,float *,float *);
	virtual void otocol::handleTouchesMove(int,int *,float *,float *);
	virtual void otocol::setDesignResolutionSize(float,float,ResolutionPolicy);
	virtual void otocol::setFrameSize(float,float);
	virtual void otocol::setScissorInPoints(float,float,float,float);
	virtual void otocol::setTouchDelegate(cocos2d::EGLTouchDelegate *);
	virtual void otocol::setViewName(char const*);
	virtual void otocol::setViewPortInPoints(float,float,float,float);
	virtual void setIMEKeyboardState(bool);
}
class cocos2d::CCEGLViewProtocol {
	void CCEGLViewProtocol();
	void getDesignResolutionSize();
	void getFrameSize();
	void getScaleX();
	void getScaleY();
	void getScissorRect();
	void getSetOfTouchesEndOrCancel(cocos2d::CCSet&, int, int*, float*, float*);
	void getViewName();
	void getViewPortRect();
	void getVisibleOrigin();
	void getVisibleSize();
	void handleTouchesBegin(int, int*, float*, float*);
	void handleTouchesCancel(int, int*, float*, float*);
	void handleTouchesEnd(int, int*, float*, float*);
	void handleTouchesMove(int, int*, float*, float*);
	void isScissorEnabled();
	void pollInputEvents();
	void setDesignResolutionSize(float, float, ResolutionPolicy);
	void setFrameSize(float, float);
	void setScissorInPoints(float, float, float, float);
	void setTouchDelegate(cocos2d::EGLTouchDelegate*);
	void setViewName(char const*);
	void setViewPortInPoints(float, float, float, float);
	void updateDesignResolutionSize();
	virtual void handleTouchesBegin(int,int *,float *,float *);
	virtual void handleTouchesCancel(int,int *,float *,float *);
	virtual void handleTouchesEnd(int,int *,float *,float *);
	virtual void handleTouchesMove(int,int *,float *,float *);
	virtual void setDesignResolutionSize(float,float,ResolutionPolicy);
	virtual void setFrameSize(float,float);
	virtual void setScissorInPoints(float,float,float,float);
	virtual void setTouchDelegate(cocos2d::EGLTouchDelegate *);
	virtual void setViewName(char const*);
	virtual void setViewPortInPoints(float,float,float,float);
}
class cocos2d::CCFadeIn : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCFadeOut : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCFadeOutBLTiles : cocos2d::CCFadeOutTRTiles {
	void create(float, cocos2d::CCSize const&);
	void testFunc(cocos2d::CCSize const&, float);
	virtual void testFunc(cocos2d::CCSize const&,float);
}
class cocos2d::CCFadeOutDownTiles : cocos2d::CCFadeOutUpTiles {
	void create(float, cocos2d::CCSize const&);
	void testFunc(cocos2d::CCSize const&, float);
	virtual void testFunc(cocos2d::CCSize const&,float);
}
class cocos2d::CCFadeOutTRTiles : cocos2d::CCTiledGrid3DAction {
	void create(float, cocos2d::CCSize const&);
	void testFunc(cocos2d::CCSize const&, float);
	void transformTile(cocos2d::CCPoint const&, float);
	void turnOffTile(cocos2d::CCPoint const&);
	void turnOnTile(cocos2d::CCPoint const&);
	void update(float);
	virtual void testFunc(cocos2d::CCSize const&,float);
	virtual void transformTile(cocos2d::CCPoint const&,float);
	virtual void update(float);
}
class cocos2d::CCFadeOutUpTiles : cocos2d::CCFadeOutTRTiles {
	void create(float, cocos2d::CCSize const&);
	void testFunc(cocos2d::CCSize const&, float);
	void transformTile(cocos2d::CCPoint const&, float);
	virtual void testFunc(cocos2d::CCSize const&,float);
	virtual void transformTile(cocos2d::CCPoint const&,float);
}
class cocos2d::CCFadeTo : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, unsigned char);
	void initWithDuration(float, unsigned char);
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCFileUtils : cocos2d::TypeInfo {
	void CCFileUtils();
	void addSearchPath(char const*);
	void addSearchResolutionsOrder(char const*);
	void addSuffix(gd::string, gd::string);
	void createCCArrayWithContentsOfFile(gd::string const&);
	void createCCDictionaryWithContentsOfFile(gd::string const&);
	void fullPathForFilename(char const*, bool);
	void fullPathFromRelativeFile(char const*, char const*);
	void getClassTypeInfo();
	void getClassTypeInfo();
	void getFileData(char const*, char const*, unsigned long*);
	void getFileDataFromZip(char const*, char const*, unsigned long*);
	void getFullPathForDirectoryAndFilename(gd::string const&, gd::string const&);
	void getNewFilename(char const*);
	void getPathForFilename(gd::string const&, gd::string const&, gd::string const&);
	void getSearchPaths();
	void getSearchResolutionsOrder();
	void getWritablePath2();
	void init();
	void isAbsolutePath(gd::string const&);
	void isPopupNotify();
	void loadFilenameLookupDictionaryFromFile(char const*);
	void purgeCachedEntries();
	void purgeFileUtils();
	void removeAllPaths();
	void removeFullPath(char const*);
	void removeSearchPath(char const*);
	void setFilenameLookupDictionary(cocos2d::CCDictionary*);
	void setPopupNotify(bool);
	void setSearchPaths(gd::vector<gd::string, gd::allocator<gd::string > > const&);
	void setSearchResolutionsOrder(gd::vector<gd::string, gd::allocator<gd::string > > const&);
	void sharedFileUtils();
	void shouldUseHD();
	void writeToFile(cocos2d::CCDictionary*, gd::string const&);
	virtual void addSearchPath(char const*);
	virtual void addSearchResolutionsOrder(char const*);
	virtual void addSuffix(gd::string,gd::string);
	virtual void createCCArrayWithContentsOfFile(gd::string const&);
	virtual void createCCDictionaryWithContentsOfFile(gd::string const&);
	virtual void fullPathForFilename(char const*,bool);
	virtual void fullPathFromRelativeFile(char const*,char const*);
	virtual void getFileData(char const*,char const*,ulong *);
	virtual void getFileDataFromZip(char const*,char const*,ulong *);
	virtual void getFullPathForDirectoryAndFilename(gd::string const&,gd::string const&);
	virtual void getNewFilename(char const*);
	virtual void getPathForFilename(gd::string const&,gd::string const&,gd::string const&);
	virtual void isAbsolutePath(gd::string const&);
	virtual void loadFilenameLookupDictionaryFromFile(char const*);
	virtual void removeFullPath(char const*);
	virtual void removeSearchPath(char const*);
	virtual void setFilenameLookupDictionary(cocos2d::CCDictionary *);
	virtual void setPopupNotify(bool);
	virtual void setSearchPaths(gd::vector<gd::string,gd::allocator<gd::string>> const&);
	virtual void setSearchResolutionsOrder(gd::vector<gd::string,gd::allocator<gd::string>> const&);
	virtual void writeToFile(cocos2d::CCDictionary *,gd::string const&);
}
class cocos2d::CCFileUtilsAndroid : cocos2d::CCFileUtils {
	void CCFileUtilsAndroid();
	void doGetFileData(char const*, char const*, unsigned long*, bool);
	void getFileData(char const*, char const*, unsigned long*);
	void getFileDataForAsync(char const*, char const*, unsigned long*);
	void getWritablePath();
	void init();
	void isAbsolutePath(gd::string const&);
	void isFileExist(gd::string const&);
	virtual void getFileData(char const*,char const*,ulong *);
	virtual void isAbsolutePath(gd::string const&);
	virtual void isFileExist(gd::string const&);
}
class cocos2d::CCFiniteTimeAction : cocos2d::CCAction {
	void reverse();
}
class cocos2d::CCFlipX : cocos2d::CCActionInstant {
	void copyWithZone(cocos2d::CCZone*);
	void create(bool);
	void initWithFlipX(bool);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCFlipX3D : cocos2d::CCGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float);
	void initWithDuration(float);
	void initWithSize(cocos2d::CCSize const&, float);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float);
	virtual void initWithSize(cocos2d::CCSize const&,float);
	virtual void update(float);
}
class cocos2d::CCFlipY : cocos2d::CCActionInstant {
	void copyWithZone(cocos2d::CCZone*);
	void create(bool);
	void initWithFlipY(bool);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCFlipY3D : cocos2d::CCFlipX3D {
	void copyWithZone(cocos2d::CCZone*);
	void create(float);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCFollow : cocos2d::CCAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCNode*, cocos2d::CCRect const&);
	void initWithTarget(cocos2d::CCNode*, cocos2d::CCRect const&);
	void isDone();
	void step(float);
	void stop();
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void step(float);
}
class cocos2d::CCGLProgram : cocos2d::CCObject {
	void CCGLProgram();
	void addAttribute(char const*, unsigned int);
	void compileShader(unsigned int*, unsigned int, char const*);
	void description();
	void fragmentShaderLog();
	void getUniformLocationForName(char const*);
	void initWithVertexShaderByteArray(char const*, char const*);
	void initWithVertexShaderFilename(char const*, char const*);
	void link();
	void logForOpenGLObject(unsigned int, void (*)(unsigned int, unsigned int, int*), void (*)(unsigned int, int, int*, char*));
	void programLog();
	void reset();
	void setUniformLocationWith1f(int, float);
	void setUniformLocationWith1i(int, int);
	void setUniformLocationWith2f(int, float, float);
	void setUniformLocationWith2fv(int, float*, unsigned int);
	void setUniformLocationWith2i(int, int, int);
	void setUniformLocationWith2iv(int, int*, unsigned int);
	void setUniformLocationWith3f(int, float, float, float);
	void setUniformLocationWith3fv(int, float*, unsigned int);
	void setUniformLocationWith3i(int, int, int, int);
	void setUniformLocationWith3iv(int, int*, unsigned int);
	void setUniformLocationWith4f(int, float, float, float, float);
	void setUniformLocationWith4fv(int, float*, unsigned int);
	void setUniformLocationWith4i(int, int, int, int, int);
	void setUniformLocationWith4iv(int, int*, unsigned int);
	void setUniformLocationWithMatrix3fv(int, float*, unsigned int);
	void setUniformLocationWithMatrix4fv(int, float*, unsigned int);
	void setUniformsForBuiltins();
	void updateUniformLocation(int, void*, unsigned int);
	void updateUniforms();
	void use();
	void vertexShaderLog();
}
class cocos2d::CCGrabber : cocos2d::CCObject {
	void CCGrabber();
	void afterRender(cocos2d::CCTexture2D*);
	void beforeRender(cocos2d::CCTexture2D*);
	void grab(cocos2d::CCTexture2D*);
}
class cocos2d::CCGrid3D : cocos2d::CCGridBase {
	void CCGrid3D();
	void blit();
	void calculateVertexPoints();
	void create(cocos2d::CCSize const&);
	void create(cocos2d::CCSize const&, cocos2d::CCTexture2D*, bool);
	void originalVertex(cocos2d::CCPoint const&);
	void reuse();
	void setVertex(cocos2d::CCPoint const&, cocos2d::_ccVertex3F const&);
	void vertex(cocos2d::CCPoint const&);
}
class cocos2d::CCGrid3DAction : cocos2d::CCGridAction {
	void CCGrid3DAction();
	void getGrid();
	void originalVertex(cocos2d::CCPoint const&);
	void setVertex(cocos2d::CCPoint const&, cocos2d::_ccVertex3F const&);
	void vertex(cocos2d::CCPoint const&);
}
class cocos2d::CCGridAction : cocos2d::CCActionInterval {
	void CCGridAction();
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&);
	void getGrid();
	void initWithDuration(float, cocos2d::CCSize const&);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float,cocos2d::CCSize const&);
	virtual void startWithTarget(cocos2d::CCNode *);
}
class cocos2d::CCGridBase : cocos2d::CCObject {
	void CCGridBase();
	void afterDraw(cocos2d::CCNode*);
	void beforeDraw();
	void blit();
	void calculateVertexPoints();
	void create(cocos2d::CCSize const&);
	void create(cocos2d::CCSize const&, cocos2d::CCTexture2D*, bool);
	void initWithSize(cocos2d::CCSize const&);
	void initWithSize(cocos2d::CCSize const&, cocos2d::CCTexture2D*, bool);
	void reuse();
	void set2DProjection();
	void setActive(bool);
	void setTextureFlipped(bool);
}
class cocos2d::CCHide : cocos2d::CCActionInstant {
	void copyWithZone(cocos2d::CCZone*);
	void create();
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCImage : cocos2d::CCObject {
	void CCImage();
	void _initWithJpgData(void*, int);
	void _initWithPngData(void*, int);
	void _initWithRawData(void*, int, int, int, int, bool);
	void _initWithTiffData(void*, int);
	void _initWithWebpData(void*, int);
	void _saveImageToJPG(char const*);
	void _saveImageToPNG(char const*, bool);
	void initWithImageData(void*, int, cocos2d::CCImage::EImageFormat, int, int, int);
	void initWithImageFile(char const*, cocos2d::CCImage::EImageFormat);
	void initWithImageFileThreadSafe(char const*, cocos2d::CCImage::EImageFormat);
	void initWithString(char const*, int, int, cocos2d::CCImage::ETextAlign, char const*, int);
	void initWithStringShadowStroke(char const*, int, int, cocos2d::CCImage::ETextAlign, char const*, int, float, float, float, bool, float, float, float, float, bool, float, float, float, float);
	void saveToFile(char const*, bool);
}
class cocos2d::CCIMEDelegate {
	void CCIMEDelegate();
	void attachWithIME();
	void canAttachWithIME();
	void canDetachWithIME();
	void deleteBackward();
	void detachWithIME();
	void didAttachWithIME();
	void didDetachWithIME();
	void getContentText();
	void insertText(char const*, int);
	void keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	void keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	virtual void insertText(char const*,int);
	virtual void keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo &);
	virtual void keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo &);
	virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo &);
	virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo &);
}
class cocos2d::CCInteger : cocos2d::CCObject {
	void acceptVisitor(cocos2d::CCDataVisitor&);
	void create(int);
	virtual void acceptVisitor(cocos2d::CCDataVisitor &);
}
class cocos2d::CCJumpBy : cocos2d::CCActionInterval {
	void CCJumpBy();
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCPoint const&, float, unsigned int);
	void initWithDuration(float, cocos2d::CCPoint const&, float, unsigned int);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCJumpTiles3D : cocos2d::CCTiledGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, unsigned int, float);
	void initWithDuration(float, cocos2d::CCSize const&, unsigned int, float);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCJumpTo : cocos2d::CCJumpBy {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCPoint const&, float, int);
	void startWithTarget(cocos2d::CCNode*);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
}
class cocos2d::CCKeyboardDelegate {
	void keyDown(cocos2d::enumKeyCodes);
	void keyUp(cocos2d::enumKeyCodes);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void keyUp(cocos2d::enumKeyCodes);
}
class cocos2d::CCKeyboardDispatcher : cocos2d::CCObject {
	void CCKeyboardDispatcher();
	void addDelegate(cocos2d::CCKeyboardDelegate*);
	void convertKeyCode(cocos2d::enumKeyCodes);
	void dispatchKeyboardMSG(cocos2d::enumKeyCodes, bool);
	void forceAddDelegate(cocos2d::CCKeyboardDelegate*);
	void forceRemoveDelegate(cocos2d::CCKeyboardDelegate*);
	void keyToString(cocos2d::enumKeyCodes);
	void removeDelegate(cocos2d::CCKeyboardDelegate*);
	void updateModifierKeys(bool, bool, bool, bool);
}
class cocos2d::CCKeyboardHandler : cocos2d::CCObject {
	void getDelegate();
	void handlerWithDelegate(cocos2d::CCKeyboardDelegate*);
	void initWithDelegate(cocos2d::CCKeyboardDelegate*);
	void setDelegate(cocos2d::CCKeyboardDelegate*);
	virtual void initWithDelegate(cocos2d::CCKeyboardDelegate *);
}
class cocos2d::CCKeypadDelegate {
	void keyBackClicked();
	void keyMenuClicked();
}
class cocos2d::CCKeypadDispatcher : cocos2d::CCObject {
	void CCKeypadDispatcher();
	void addDelegate(cocos2d::CCKeypadDelegate*);
	void dispatchKeypadMSG(cocos2d::ccKeypadMSGType);
	void forceAddDelegate(cocos2d::CCKeypadDelegate*);
	void forceRemoveDelegate(cocos2d::CCKeypadDelegate*);
	void removeDelegate(cocos2d::CCKeypadDelegate*);
}
class cocos2d::CCKeypadHandler : cocos2d::CCObject {
	void getDelegate();
	void handlerWithDelegate(cocos2d::CCKeypadDelegate*);
	void initWithDelegate(cocos2d::CCKeypadDelegate*);
	void setDelegate(cocos2d::CCKeypadDelegate*);
	virtual void initWithDelegate(cocos2d::CCKeypadDelegate *);
}
class cocos2d::CCLabelAtlas : cocos2d::CCAtlasNode, cocos2d::CCLabelProtocol {
	void create(char const*, char const*);
	void create(char const*, char const*, unsigned int, unsigned int, unsigned int);
	void getString();
	void initWithString(char const*, cocos2d::CCTexture2D*, unsigned int, unsigned int, unsigned int);
	void initWithString(char const*, char const*);
	void initWithString(char const*, char const*, unsigned int, unsigned int, unsigned int);
	void setString(char const*);
	void updateAtlasValues();
	virtual void setString(char const*);
}
class cocos2d::CCLabelBMFont : cocos2d::CCSpriteBatchNode, cocos2d::CCLabelProtocol, cocos2d::CCRGBAProtocol {
	void CCLabelBMFont();
	void create(char const*, char const*);
	void create(char const*, char const*, float);
	void create(char const*, char const*, float, cocos2d::CCTextAlignment);
	void create(char const*, char const*, float, cocos2d::CCTextAlignment, cocos2d::CCPoint);
	void create();
	void createBatched(char const*, char const*, cocos2d::CCArray*);
	void createFontChars();
	void getColor();
	void getConfiguration();
	void getDisplayedColor();
	void getDisplayedOpacity();
	void getFntFile();
	void getLetterPosXLeft(cocos2d::CCSprite*);
	void getLetterPosXRight(cocos2d::CCSprite*);
	void getOpacity();
	void getString();
	void init();
	void initWithString(char const*, char const*, float, cocos2d::CCTextAlignment, cocos2d::CCPoint);
	void isCascadeColorEnabled();
	void isCascadeOpacityEnabled();
	void isOpacityModifyRGB();
	void kerningAmountForFirst(unsigned short, unsigned short);
	void limitLabelWidth(float, float, float);
	void purgeCachedData();
	void setAlignment(cocos2d::CCTextAlignment);
	void setAnchorPoint(cocos2d::CCPoint const&);
	void setCString(char const*);
	void setCascadeColorEnabled(bool);
	void setCascadeOpacityEnabled(bool);
	void setColor(cocos2d::_ccColor3B const&);
	void setFntFile(char const*);
	void setLineBreakWithoutSpace(bool);
	void setOpacity(unsigned char);
	void setOpacityModifyRGB(bool);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void setString(char const*);
	void setString(char const*, bool);
	void setString(unsigned short*, bool);
	void setWidth(float);
	void updateDisplayedColor(cocos2d::_ccColor3B const&);
	void updateDisplayedOpacity(unsigned char);
	void updateLabel();
	virtual void setAlignment(cocos2d::CCTextAlignment);
	virtual void setAnchorPoint(cocos2d::CCPoint const&);
	virtual void setCString(char const*);
	virtual void setCascadeColorEnabled(bool);
	virtual void setCascadeOpacityEnabled(bool);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setLineBreakWithoutSpace(bool);
	virtual void setOpacity(uchar);
	virtual void setOpacityModifyRGB(bool);
	virtual void setScale(float);
	virtual void setScaleX(float);
	virtual void setScaleY(float);
	virtual void setString(char const*);
	virtual void setString(char const*,bool);
	virtual void setString(ushort *,bool);
	virtual void setWidth(float);
	virtual void updateDisplayedColor(cocos2d::_ccColor3B const&);
	virtual void updateDisplayedOpacity(uchar);
}
class cocos2d::CCLabelProtocol {
}
class cocos2d::CCLabelTTF : cocos2d::CCSprite, cocos2d::CCLabelProtocol {
	void CCLabelTTF();
	void _prepareTextDefinition(bool);
	void _updateWithTextDefinition(cocos2d::_ccFontDefinition&, bool);
	void create(char const*, char const*, float);
	void create(char const*, char const*, float, cocos2d::CCSize const&, cocos2d::CCTextAlignment);
	void create(char const*, char const*, float, cocos2d::CCSize const&, cocos2d::CCTextAlignment, cocos2d::CCVerticalTextAlignment);
	void create();
	void createWithFontDefinition(char const*, cocos2d::_ccFontDefinition&);
	void description();
	void disableShadow(bool);
	void disableStroke(bool);
	void enableShadow(cocos2d::CCSize const&, float, float, bool);
	void enableStroke(cocos2d::_ccColor3B const&, float, bool);
	void getDimensions();
	void getFontName();
	void getFontSize();
	void getHorizontalAlignment();
	void getString();
	void getTextDefinition();
	void getVerticalAlignment();
	void init();
	void initWithString(char const*, char const*, float);
	void initWithString(char const*, char const*, float, cocos2d::CCSize const&, cocos2d::CCTextAlignment);
	void initWithString(char const*, char const*, float, cocos2d::CCSize const&, cocos2d::CCTextAlignment, cocos2d::CCVerticalTextAlignment);
	void initWithStringAndTextDefinition(char const*, cocos2d::_ccFontDefinition&);
	void setDimensions(cocos2d::CCSize const&);
	void setFontFillColor(cocos2d::_ccColor3B const&, bool);
	void setFontName(char const*);
	void setFontSize(float);
	void setHorizontalAlignment(cocos2d::CCTextAlignment);
	void setString(char const*);
	void setTextDefinition(cocos2d::_ccFontDefinition*);
	void setVerticalAlignment(cocos2d::CCVerticalTextAlignment);
	void updateTexture();
	virtual void setString(char const*);
}
class cocos2d::CCLayer : cocos2d::CCNode, cocos2d::CCTouchDelegate, cocos2d::CCAccelerometerDelegate, cocos2d::CCKeypadDelegate, cocos2d::CCKeyboardDelegate, cocos2d::CCMouseDelegate {
	void CCLayer();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*);
	void ccTouchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*);
	void ccTouchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*);
	void ccTouchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*);
	void create();
	void didAccelerate(cocos2d::CCAcceleration*);
	void excuteScriptTouchHandler(int, cocos2d::CCSet*);
	void excuteScriptTouchHandler(int, cocos2d::CCTouch*);
	void getTouchMode();
	void getTouchPriority();
	void init();
	void isAccelerometerEnabled();
	void isKeyboardEnabled();
	void isKeypadEnabled();
	void isMouseEnabled();
	void isTouchEnabled();
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void keyMenuClicked();
	void onEnter();
	void onEnterTransitionDidFinish();
	void onExit();
	void registerScriptAccelerateHandler(int);
	void registerScriptKeypadHandler(int);
	void registerScriptTouchHandler(int, bool, int, bool);
	void registerWithTouchDispatcher();
	void setAccelerometerEnabled(bool);
	void setAccelerometerInterval(double);
	void setKeyboardEnabled(bool);
	void setKeypadEnabled(bool);
	void setMouseEnabled(bool);
	void setTouchEnabled(bool);
	void setTouchMode(cocos2d::ccTouchesMode);
	void setTouchPriority(int);
	void unregisterScriptAccelerateHandler();
	void unregisterScriptKeypadHandler();
	void unregisterScriptTouchHandler();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchesBegan(cocos2d::CCSet *,cocos2d::CCEvent *);
	virtual void ccTouchesCancelled(cocos2d::CCSet *,cocos2d::CCEvent *);
	virtual void ccTouchesEnded(cocos2d::CCSet *,cocos2d::CCEvent *);
	virtual void ccTouchesMoved(cocos2d::CCSet *,cocos2d::CCEvent *);
	virtual void didAccelerate(cocos2d::CCAcceleration *);
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void registerScriptTouchHandler(int,bool,int,bool);
	virtual void setAccelerometerEnabled(bool);
	virtual void setAccelerometerInterval(double);
	virtual void setKeyboardEnabled(bool);
	virtual void setKeypadEnabled(bool);
	virtual void setMouseEnabled(bool);
	virtual void setTouchEnabled(bool);
	virtual void setTouchMode(cocos2d::ccTouchesMode);
	virtual void setTouchPriority(int);
}
class cocos2d::CCLayerColor : cocos2d::CCLayerRGBA, cocos2d::CCBlendProtocol {
	void CCLayerColor();
	void changeHeight(float);
	void changeWidth(float);
	void changeWidthAndHeight(float, float);
	void create(cocos2d::_ccColor4B const&);
	void create(cocos2d::_ccColor4B const&, float, float);
	void create();
	void draw();
	void getBlendFunc();
	void init();
	void initWithColor(cocos2d::_ccColor4B const&);
	void initWithColor(cocos2d::_ccColor4B const&, float, float);
	void setBlendFunc(cocos2d::_ccBlendFunc);
	void setColor(cocos2d::_ccColor3B const&);
	void setContentSize(cocos2d::CCSize const&);
	void setOpacity(unsigned char);
	void updateColor();
	virtual void initWithColor(cocos2d::_ccColor4B const&,float,float);
	virtual void initWithColor(cocos2d::_ccColor4B const&);
	virtual void setBlendFunc(cocos2d::_ccBlendFunc);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setContentSize(cocos2d::CCSize const&);
	virtual void setOpacity(uchar);
}
class cocos2d::CCLayerGradient : cocos2d::CCLayerColor {
	void CCLayerGradient();
	void create(cocos2d::_ccColor4B const&, cocos2d::_ccColor4B const&);
	void create(cocos2d::_ccColor4B const&, cocos2d::_ccColor4B const&, cocos2d::CCPoint const&);
	void create();
	void getEndColor();
	void getEndOpacity();
	void getStartColor();
	void getStartOpacity();
	void getVector();
	void init();
	void initWithColor(cocos2d::_ccColor4B const&, cocos2d::_ccColor4B const&);
	void initWithColor(cocos2d::_ccColor4B const&, cocos2d::_ccColor4B const&, cocos2d::CCPoint const&);
	void isCompressedInterpolation();
	void setCompressedInterpolation(bool);
	void setEndColor(cocos2d::_ccColor3B const&);
	void setEndOpacity(unsigned char);
	void setStartColor(cocos2d::_ccColor3B const&);
	void setStartOpacity(unsigned char);
	void setVector(cocos2d::CCPoint const&);
	void updateColor();
	virtual void initWithColor(cocos2d::_ccColor4B const&,cocos2d::_ccColor4B const&);
	virtual void initWithColor(cocos2d::_ccColor4B const&,cocos2d::_ccColor4B const&,cocos2d::CCPoint const&);
	virtual void setCompressedInterpolation(bool);
	virtual void setEndColor(cocos2d::_ccColor3B const&);
	virtual void setEndOpacity(uchar);
	virtual void setStartColor(cocos2d::_ccColor3B const&);
	virtual void setStartOpacity(uchar);
	virtual void setVector(cocos2d::CCPoint const&);
}
class cocos2d::CCLayerMultiplex : cocos2d::CCLayer {
	void CCLayerMultiplex();
	void addLayer(cocos2d::CCLayer*);
	void create(cocos2d::CCLayer*, ...);
	void create();
	void createWithArray(cocos2d::CCArray*);
	void createWithLayer(cocos2d::CCLayer*);
	void initWithArray(cocos2d::CCArray*);
	void initWithLayers(cocos2d::CCLayer*, gd::__va_list);
	void switchTo(unsigned int);
	void switchToAndReleaseMe(unsigned int);
}
class cocos2d::CCLayerRGBA : cocos2d::CCLayer, cocos2d::CCRGBAProtocol {
	void CCLayerRGBA();
	void getColor();
	void getDisplayedColor();
	void getDisplayedOpacity();
	void getOpacity();
	void init();
	void isCascadeColorEnabled();
	void isCascadeOpacityEnabled();
	void isOpacityModifyRGB();
	void setCascadeColorEnabled(bool);
	void setCascadeOpacityEnabled(bool);
	void setColor(cocos2d::_ccColor3B const&);
	void setOpacity(unsigned char);
	void setOpacityModifyRGB(bool);
	void updateDisplayedColor(cocos2d::_ccColor3B const&);
	void updateDisplayedOpacity(unsigned char);
	virtual void setCascadeColorEnabled(bool);
	virtual void setCascadeOpacityEnabled(bool);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setOpacity(uchar);
	virtual void setOpacityModifyRGB(bool);
	virtual void updateDisplayedColor(cocos2d::_ccColor3B const&);
	virtual void updateDisplayedOpacity(uchar);
}
class cocos2d::CCLens3D : cocos2d::CCGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, cocos2d::CCPoint const&, float);
	void initWithDuration(float, cocos2d::CCSize const&, cocos2d::CCPoint const&, float);
	void setPosition(cocos2d::CCPoint const&);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCLightning : cocos2d::CCNode, cocos2d::CCRGBAProtocol {
	void CCLightning();
	void draw();
	void getColor();
	void getDisplayedColor();
	void getDisplayedOpacity();
	void getOpacity();
	void initWithStrikePoint(cocos2d::CCPoint);
	void initWithStrikePoint(cocos2d::CCPoint, cocos2d::CCPoint, float);
	void isCascadeColorEnabled();
	void isCascadeOpacityEnabled();
	void isOpacityModifyRGB();
	void lightningWithStrikePoint(cocos2d::CCPoint);
	void lightningWithStrikePoint(cocos2d::CCPoint, cocos2d::CCPoint, float);
	void setCascadeColorEnabled(bool);
	void setCascadeOpacityEnabled(bool);
	void setColor(cocos2d::_ccColor3B const&);
	void setOpacity(unsigned char);
	void setOpacityModifyRGB(bool);
	void strike();
	void strikeFinished();
	void strikeRandom();
	void strikeWithSeed(unsigned long);
	void updateDisplayedColor(cocos2d::_ccColor3B const&);
	void updateDisplayedOpacity(unsigned char);
	virtual void setCascadeColorEnabled(bool);
	virtual void setCascadeOpacityEnabled(bool);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setOpacity(uchar);
	virtual void setOpacityModifyRGB(bool);
	virtual void updateDisplayedColor(cocos2d::_ccColor3B const&);
	virtual void updateDisplayedOpacity(uchar);
}
class cocos2d::CCLiquid : cocos2d::CCGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, unsigned int, float);
	void initWithDuration(float, cocos2d::CCSize const&, unsigned int, float);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCMenu : cocos2d::CCLayerRGBA {
	void addChild(cocos2d::CCNode*);
	void addChild(cocos2d::CCNode*, int);
	void addChild(cocos2d::CCNode*, int, int);
	void alignItemsHorizontally();
	void alignItemsHorizontallyWithPadding(float);
	void alignItemsInColumns(unsigned int, gd::__va_list);
	void alignItemsInColumns(unsigned int, ...);
	void alignItemsInColumnsWithArray(cocos2d::CCArray*);
	void alignItemsInRows(unsigned int, gd::__va_list);
	void alignItemsInRows(unsigned int, ...);
	void alignItemsInRowsWithArray(cocos2d::CCArray*);
	void alignItemsVertically();
	void alignItemsVerticallyWithPadding(float);
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(cocos2d::CCMenuItem*, ...);
	void create();
	void createWithArray(cocos2d::CCArray*);
	void createWithItem(cocos2d::CCMenuItem*);
	void createWithItems(cocos2d::CCMenuItem*, gd::__va_list);
	void init();
	void initWithArray(cocos2d::CCArray*);
	void isEnabled();
	void isOpacityModifyRGB();
	void itemForTouch(cocos2d::CCTouch*);
	void onExit();
	void registerWithTouchDispatcher();
	void removeChild(cocos2d::CCNode*, bool);
	void setEnabled(bool);
	void setHandlerPriority(int);
	void setOpacityModifyRGB(bool);
	virtual void addChild(cocos2d::CCNode *);
	virtual void addChild(cocos2d::CCNode *,int);
	virtual void addChild(cocos2d::CCNode *,int,int);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void removeChild(cocos2d::CCNode *,bool);
	virtual void setEnabled(bool);
	virtual void setOpacityModifyRGB(bool);
}
class cocos2d::CCMenuItem : cocos2d::CCNodeRGBA {
	void activate();
	void create(cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void create();
	void initWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void isEnabled();
	void isSelected();
	void rect();
	void registerScriptTapHandler(int);
	void selected();
	void setEnabled(bool);
	void setTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void unregisterScriptTapHandler();
	void unselected();
	virtual void registerScriptTapHandler(int);
	virtual void setEnabled(bool);
}
class cocos2d::CCMenuItemAtlasFont : cocos2d::CCMenuItemLabel {
	void create(char const*, char const*, int, int, char);
	void create(char const*, char const*, int, int, char, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void initWithString(char const*, char const*, int, int, char, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
}
class cocos2d::CCMenuItemFont : cocos2d::CCMenuItemLabel {
	void create(char const*);
	void create(char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void fontName();
	void fontNameObj();
	void fontSize();
	void fontSizeObj();
	void initWithString(char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void recreateLabel();
	void setFontName(char const*);
	void setFontNameObj(char const*);
	void setFontSize(unsigned int);
	void setFontSizeObj(unsigned int);
}
class cocos2d::CCMenuItemImage : cocos2d::CCMenuItemSprite {
	void CCMenuItemImage();
	void create(char const*, char const*);
	void create(char const*, char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void create(char const*, char const*, char const*);
	void create(char const*, char const*, char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void create();
	void init();
	void initWithNormalImage(char const*, char const*, char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void setDisabledSpriteFrame(cocos2d::CCSpriteFrame*);
	void setNormalSpriteFrame(cocos2d::CCSpriteFrame*);
	void setSelectedSpriteFrame(cocos2d::CCSpriteFrame*);
}
class cocos2d::CCMenuItemLabel : cocos2d::CCMenuItem {
	void CCMenuItemLabel();
	void activate();
	void create(cocos2d::CCNode*);
	void create(cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void getDisabledColor();
	void getLabel();
	void initWithLabel(cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void selected();
	void setDisabledColor(cocos2d::_ccColor3B const&);
	void setEnabled(bool);
	void setLabel(cocos2d::CCNode*);
	void setString(char const*);
	void unselected();
	virtual void setDisabledColor(cocos2d::_ccColor3B const&);
	virtual void setEnabled(bool);
	virtual void setLabel(cocos2d::CCNode *);
}
class cocos2d::CCMenuItemSprite : cocos2d::CCMenuItem {
	void create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*);
	void create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void getDisabledImage();
	void getNormalImage();
	void getSelectedImage();
	void initWithNormalSprite(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void selected();
	void setDisabledImage(cocos2d::CCNode*);
	void setEnabled(bool);
	void setNormalImage(cocos2d::CCNode*);
	void setSelectedImage(cocos2d::CCNode*);
	void unselected();
	void updateImagesVisibility();
	virtual void setDisabledImage(cocos2d::CCNode *);
	virtual void setEnabled(bool);
	virtual void setNormalImage(cocos2d::CCNode *);
	virtual void setSelectedImage(cocos2d::CCNode *);
}
class cocos2d::CCMenuItemToggle : cocos2d::CCMenuItem {
	void CCMenuItemToggle();
	void activate();
	void addSubItem(cocos2d::CCMenuItem*);
	void create(cocos2d::CCMenuItem*);
	void create();
	void createWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCMenuItem*, ...);
	void createWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCArray*);
	void getSelectedIndex();
	void getSubItems();
	void initWithItem(cocos2d::CCMenuItem*);
	void initWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCMenuItem*, gd::__va_list);
	void selected();
	void selectedItem();
	void setEnabled(bool);
	void setSelectedIndex(unsigned int);
	void setSubItems(cocos2d::CCArray*);
	void unselected();
	virtual void setEnabled(bool);
	virtual void setSelectedIndex(uint);
	virtual void setSubItems(cocos2d::CCArray *);
}
class cocos2d::CCMotionStreak : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	void CCMotionStreak();
	void create(float, float, float, cocos2d::_ccColor3B const&, char const*);
	void create(float, float, float, cocos2d::_ccColor3B const&, cocos2d::CCTexture2D*);
	void draw();
	void enableRepeatMode(float);
	void getBlendFunc();
	void getOpacity();
	void getTexture();
	void initWithFade(float, float, float, cocos2d::_ccColor3B const&, char const*);
	void initWithFade(float, float, float, cocos2d::_ccColor3B const&, cocos2d::CCTexture2D*);
	void isOpacityModifyRGB();
	void reset();
	void resumeStroke();
	void setBlendFunc(cocos2d::_ccBlendFunc);
	void setOpacity(unsigned char);
	void setOpacityModifyRGB(bool);
	void setPosition(cocos2d::CCPoint const&);
	void setStroke(float);
	void setTexture(cocos2d::CCTexture2D*);
	void stopStroke();
	void tintWithColor(cocos2d::_ccColor3B);
	void update(float);
	virtual void setBlendFunc(cocos2d::_ccBlendFunc);
	virtual void setOpacity(uchar);
	virtual void setOpacityModifyRGB(bool);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void setTexture(cocos2d::CCTexture2D *);
	virtual void update(float);
}
class cocos2d::CCMouseDelegate {
	void rightKeyDown();
	void rightKeyUp();
	void scrollWheel(float, float);
	virtual void scrollWheel(float,float);
}
class cocos2d::CCMouseDispatcher : cocos2d::CCObject {
	void CCMouseDispatcher();
	void addDelegate(cocos2d::CCMouseDelegate*);
	void dispatchScrollMSG(float, float);
	void forceAddDelegate(cocos2d::CCMouseDelegate*);
	void forceRemoveDelegate(cocos2d::CCMouseDelegate*);
	void removeDelegate(cocos2d::CCMouseDelegate*);
}
class cocos2d::CCMouseHandler : cocos2d::CCObject {
	void getDelegate();
	void handlerWithDelegate(cocos2d::CCMouseDelegate*);
	void initWithDelegate(cocos2d::CCMouseDelegate*);
	void setDelegate(cocos2d::CCMouseDelegate*);
	virtual void initWithDelegate(cocos2d::CCMouseDelegate *);
}
class cocos2d::CCMoveBy : cocos2d::CCActionInterval {
	void CCMoveBy();
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCPoint const&);
	void initWithDuration(float, cocos2d::CCPoint const&);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCMoveTo : cocos2d::CCMoveBy {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCPoint const&);
	void initWithDuration(float, cocos2d::CCPoint const&);
	void startWithTarget(cocos2d::CCNode*);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
}
class cocos2d::CCNode : cocos2d::CCObject {
	void CCNode();
	void _setZOrder(int);
	void addChild(cocos2d::CCNode*);
	void addChild(cocos2d::CCNode*, int);
	void addChild(cocos2d::CCNode*, int, int);
	void addComponent(cocos2d::CCComponent*);
	void boundingBox();
	void childrenAlloc();
	void cleanup();
	void convertToNodeSpace(cocos2d::CCPoint const&);
	void convertToNodeSpaceAR(cocos2d::CCPoint const&);
	void convertToWindowSpace(cocos2d::CCPoint const&);
	void convertToWorldSpace(cocos2d::CCPoint const&);
	void convertToWorldSpaceAR(cocos2d::CCPoint const&);
	void convertTouchToNodeSpace(cocos2d::CCTouch*);
	void convertTouchToNodeSpaceAR(cocos2d::CCTouch*);
	void create();
	void description();
	void detachChild(cocos2d::CCNode*, bool);
	void draw();
	void getActionByTag(int);
	void getActionManager();
	void getAnchorPoint();
	void getAnchorPointInPoints();
	void getCamera();
	void getChildByTag(int);
	void getChildren();
	void getChildrenCount();
	void getComponent(char const*);
	void getContentSize();
	void getGLServerState();
	void getGrid();
	void getOrderOfArrival();
	void getParent();
	void getPosition(float*, float*);
	void getPosition();
	void getPositionX();
	void getPositionY();
	void getRotation();
	void getRotationX();
	void getRotationY();
	void getScale();
	void getScaleX();
	void getScaleY();
	void getScaledContentSize();
	void getScheduler();
	void getShaderProgram();
	void getSkewX();
	void getSkewY();
	void getUserData();
	void getUserObject();
	void getVertexZ();
	void getZOrder();
	void ignoreAnchorPointForPosition(bool);
	void init();
	void insertChild(cocos2d::CCNode*, int);
	void isIgnoreAnchorPointForPosition();
	void isRunning();
	void isVisible();
	void nodeToParentTransform();
	void nodeToWorldTransform();
	void numberOfRunningActions();
	void onEnter();
	void onEnterTransitionDidFinish();
	void onExit();
	void onExitTransitionDidStart();
	void parentToNodeTransform();
	void pauseSchedulerAndActions();
	void registerScriptHandler(int);
	void removeAllChildren();
	void removeAllChildrenWithCleanup(bool);
	void removeAllComponents();
	void removeChild(cocos2d::CCNode*);
	void removeChild(cocos2d::CCNode*, bool);
	void removeChildByTag(int);
	void removeChildByTag(int, bool);
	void removeComponent(char const*);
	void removeComponent(cocos2d::CCComponent*);
	void removeFromParent();
	void removeFromParentAndCleanup(bool);
	void removeMeAndCleanup();
	void reorderChild(cocos2d::CCNode*, int);
	void resumeSchedulerAndActions();
	void runAction(cocos2d::CCAction*);
	void schedule(cocos2d::SEL_SCHEDULE);
	void schedule(cocos2d::SEL_SCHEDULE, float);
	void schedule(cocos2d::SEL_SCHEDULE, float, unsigned int, float);
	void scheduleOnce(cocos2d::SEL_SCHEDULE, float);
	void scheduleUpdate();
	void scheduleUpdateWithPriority(int);
	void scheduleUpdateWithPriorityLua(int, int);
	void setActionManager(cocos2d::CCActionManager*);
	void setAdditionalTransform(cocos2d::CCAffineTransform const&);
	void setAnchorPoint(cocos2d::CCPoint const&);
	void setContentSize(cocos2d::CCSize const&);
	void setGLServerState(cocos2d::ccGLServerState);
	void setGrid(cocos2d::CCGridBase*);
	void setOrderOfArrival(unsigned int);
	void setParent(cocos2d::CCNode*);
	void setPosition(float, float);
	void setPosition(cocos2d::CCPoint const&);
	void setPositionX(float);
	void setPositionY(float);
	void setRotation(float);
	void setRotationX(float);
	void setRotationY(float);
	void setScale(float);
	void setScale(float, float);
	void setScaleX(float);
	void setScaleY(float);
	void setScheduler(cocos2d::CCScheduler*);
	void setShaderProgram(cocos2d::CCGLProgram*);
	void setSkewX(float);
	void setSkewY(float);
	void setUserData(void*);
	void setUserObject(cocos2d::CCObject*);
	void setVertexZ(float);
	void setVisible(bool);
	void setZOrder(int);
	void sortAllChildren();
	void stopAction(cocos2d::CCAction*);
	void stopActionByTag(int);
	void stopAllActions();
	void transform();
	void transformAncestors();
	void unregisterScriptHandler();
	void unschedule(cocos2d::SEL_SCHEDULE);
	void unscheduleAllSelectors();
	void unscheduleUpdate();
	void update(float);
	void updateTransform();
	void updateTweenAction(float, char const*);
	void visit();
	void worldToNodeTransform();
	virtual void _setZOrder(int);
	virtual void addChild(cocos2d::CCNode*);
	virtual void addChild(cocos2d::CCNode*,int);
	virtual void addChild(cocos2d::CCNode*,int,int);
	virtual void addComponent(cocos2d::CCComponent *);
	virtual void getChildByTag(int);
	virtual void getPosition(float *,float *);
	virtual void ignoreAnchorPointForPosition(bool);
	virtual void registerScriptHandler(int);
	virtual void removeAllChildrenWithCleanup(bool);
	virtual void removeChild(cocos2d::CCNode*);
	virtual void removeChild(cocos2d::CCNode*,bool);
	virtual void removeChildByTag(int);
	virtual void removeChildByTag(int,bool);
	virtual void removeComponent(char const*);
	virtual void removeComponent(cocos2d::CCComponent *);
	virtual void removeFromParentAndCleanup(bool);
	virtual void reorderChild(cocos2d::CCNode*,int);
	virtual void setActionManager(cocos2d::CCActionManager *);
	virtual void setAnchorPoint(cocos2d::CCPoint const&);
	virtual void setContentSize(cocos2d::CCSize const&);
	virtual void setGLServerState(cocos2d::ccGLServerState);
	virtual void setGrid(cocos2d::CCGridBase *);
	virtual void setOrderOfArrival(uint);
	virtual void setParent(cocos2d::CCNode*);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void setPosition(float,float);
	virtual void setPositionX(float);
	virtual void setPositionY(float);
	virtual void setRotation(float);
	virtual void setRotationX(float);
	virtual void setRotationY(float);
	virtual void setScale(float);
	virtual void setScale(float,float);
	virtual void setScaleX(float);
	virtual void setScaleY(float);
	virtual void setScheduler(cocos2d::CCScheduler *);
	virtual void setShaderProgram(cocos2d::CCGLProgram *);
	virtual void setSkewX(float);
	virtual void setSkewY(float);
	virtual void setUserData(void *);
	virtual void setUserObject(cocos2d::CCObject *);
	virtual void setVertexZ(float);
	virtual void setVisible(bool);
	virtual void setZOrder(int);
	virtual void update(float);
	virtual void updateTweenAction(float,char const*);
}
class cocos2d::CCNodeRGBA : cocos2d::CCNode, cocos2d::CCRGBAProtocol {
	void CCNodeRGBA();
	void create();
	void getColor();
	void getDisplayedColor();
	void getDisplayedOpacity();
	void getOpacity();
	void init();
	void isCascadeColorEnabled();
	void isCascadeOpacityEnabled();
	void isOpacityModifyRGB();
	void setCascadeColorEnabled(bool);
	void setCascadeOpacityEnabled(bool);
	void setCascadeOpacityEnabled(bool);
	void setColor(cocos2d::_ccColor3B const&);
	void setColor(cocos2d::_ccColor3B const&);
	void setOpacity(unsigned char);
	void setOpacity(uchar);
	void setOpacityModifyRGB(bool);
	void updateDisplayedColor(cocos2d::_ccColor3B const&);
	void updateDisplayedOpacity(unsigned char);
	void updateDisplayedOpacity(uchar);
	virtual void setCascadeColorEnabled(bool);
	virtual void setOpacityModifyRGB(bool);
	virtual void updateDisplayedColor(cocos2d::_ccColor3B const&);
}
class cocos2d::CCNotificationCenter : cocos2d::CCObject {
	void CCNotificationCenter();
	void addObserver(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, char const*, cocos2d::CCObject*);
	void getObserverHandlerByName(char const*);
	void observerExisted(cocos2d::CCObject*, char const*);
	void postNotification(char const*);
	void postNotification(char const*, cocos2d::CCObject*);
	void purgeNotificationCenter();
	void registerScriptObserver(cocos2d::CCObject*, int, char const*);
	void removeAllObservers(cocos2d::CCObject*);
	void removeObserver(cocos2d::CCObject*, char const*);
	void sharedNotificationCenter();
	void unregisterScriptObserver(cocos2d::CCObject*, char const*);
}
class cocos2d::CCNotificationObserver : cocos2d::CCObject {
	void CCNotificationObserver(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, char const*, cocos2d::CCObject*);
	void getHandler();
	void getName();
	void getObject();
	void getSelector();
	void getTarget();
	void performSelector(cocos2d::CCObject*);
	void setHandler(int);
	virtual void setHandler(int);
}
class cocos2d::CCObject : cocos2d::CCCopying {
	void CCObject();
	void acceptVisitor(cocos2d::CCDataVisitor&);
	void autorelease();
	void canEncode();
	void copy();
	void createWithCoder(DS_Dictionary*);
	void encodeWithCoder(DS_Dictionary*);
	void getTag();
	void isEqual(cocos2d::CCObject const*);
	void isSingleReference();
	void release();
	void retain();
	void retainCount();
	void setTag(int);
	void update(float);
	virtual void acceptVisitor(cocos2d::CCDataVisitor &);
	virtual void encodeWithCoder(DS_Dictionary *);
	virtual void isEqual(cocos2d::CCObject const*);
	virtual void setTag(int);
	virtual void update(float);
}
class cocos2d::CCOrbitCamera : cocos2d::CCActionCamera {
	void CCOrbitCamera();
	void copyWithZone(cocos2d::CCZone*);
	void create(float, float, float, float, float, float, float);
	void initWithDuration(float, float, float, float, float, float, float);
	void sphericalRadius(float*, float*, float*);
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCPageTurn3D : cocos2d::CCGrid3DAction {
	void create(float, cocos2d::CCSize const&);
	void update(float);
	virtual void update(float);
}
class cocos2d::CCParallaxNode : cocos2d::CCNode {
	void CCParallaxNode();
	void absolutePosition();
	void addChild(cocos2d::CCNode*, unsigned int, int);
	void addChild(cocos2d::CCNode*, unsigned int, cocos2d::CCPoint const&, cocos2d::CCPoint const&);
	void create();
	void removeAllChildrenWithCleanup(bool);
	void removeChild(cocos2d::CCNode*, bool);
	void visit();
	virtual void addChild(cocos2d::CCNode *,uint,cocos2d::CCPoint const&,cocos2d::CCPoint const&);
	virtual void addChild(cocos2d::CCNode *,uint,int);
	virtual void removeAllChildrenWithCleanup(bool);
	virtual void removeChild(cocos2d::CCNode *,bool);
}
class cocos2d::CCParticleBatchNode : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	void CCParticleBatchNode();
	void addChild(cocos2d::CCNode*);
	void addChild(cocos2d::CCNode*, int);
	void addChild(cocos2d::CCNode*, int, int);
	void addChildHelper(cocos2d::CCParticleSystem*, int, int);
	void create(char const*, unsigned int);
	void createWithTexture(cocos2d::CCTexture2D*, unsigned int);
	void disableParticle(unsigned int);
	void draw();
	void getBlendFunc();
	void getCurrentIndex(unsigned int*, unsigned int*, cocos2d::CCNode*, int);
	void getTexture();
	void increaseAtlasCapacityTo(unsigned int);
	void initWithFile(char const*, unsigned int);
	void initWithTexture(cocos2d::CCTexture2D*, unsigned int);
	void insertChild(cocos2d::CCParticleSystem*, unsigned int);
	void removeAllChildrenWithCleanup(bool);
	void removeChild(cocos2d::CCNode*, bool);
	void removeChildAtIndex(unsigned int, bool);
	void reorderChild(cocos2d::CCNode*, int);
	void searchNewPositionInChildrenForZ(int);
	void setBlendFunc(cocos2d::_ccBlendFunc);
	void setTexture(cocos2d::CCTexture2D*);
	void updateAllAtlasIndexes();
	void updateBlendFunc();
	void visit();
	virtual void addChild(cocos2d::CCNode *);
	virtual void addChild(cocos2d::CCNode *,int);
	virtual void addChild(cocos2d::CCNode *,int,int);
	virtual void removeAllChildrenWithCleanup(bool);
	virtual void removeChild(cocos2d::CCNode *,bool);
	virtual void reorderChild(cocos2d::CCNode *,int);
	virtual void setBlendFunc(cocos2d::_ccBlendFunc);
	virtual void setTexture(cocos2d::CCTexture2D *);
}
class cocos2d::CCParticleExplosion : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleFire : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleFireworks : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleFlower : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleGalaxy : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleMeteor : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleRain : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleSmoke : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleSnow : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleSpiral : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleSun : cocos2d::CCParticleSystemQuad {
	void create();
	void createWithTotalParticles(unsigned int);
	void init();
	void initWithTotalParticles(unsigned int);
	virtual void initWithTotalParticles(uint);
}
class cocos2d::CCParticleSystem : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	void CCParticleSystem();
	void addParticle();
	void create(char const*);
	void createWithTotalParticles(unsigned int);
	void getAngle();
	void getAngleVar();
	void getBatchNode();
	void getBlendFunc();
	void getDuration();
	void getEmissionRate();
	void getEmitterMode();
	void getEndColor();
	void getEndColorVar();
	void getEndRadius();
	void getEndRadiusVar();
	void getEndSize();
	void getEndSizeVar();
	void getEndSpin();
	void getEndSpinVar();
	void getGravity();
	void getLife();
	void getLifeVar();
	void getOpacityModifyRGB();
	void getParticleCount();
	void getPosVar();
	void getPositionType();
	void getRadialAccel();
	void getRadialAccelVar();
	void getRotatePerSecond();
	void getRotatePerSecondVar();
	void getRotationIsDir();
	void getSourcePosition();
	void getSpeed();
	void getSpeedVar();
	void getStartColor();
	void getStartColorVar();
	void getStartRadius();
	void getStartRadiusVar();
	void getStartSize();
	void getStartSizeVar();
	void getStartSpin();
	void getStartSpinVar();
	void getTangentialAccel();
	void getTangentialAccelVar();
	void getTexture();
	void getTotalParticles();
	void init();
	void initParticle(cocos2d::sCCParticle*);
	void initWithDictionary(cocos2d::CCDictionary*);
	void initWithDictionary(cocos2d::CCDictionary*, char const*);
	void initWithFile(char const*);
	void initWithTotalParticles(unsigned int);
	void isActive();
	void isAutoRemoveOnFinish();
	void isBlendAdditive();
	void isFull();
	void loadDefaults();
	void loadScaledDefaults(float);
	void postStep();
	void resetSystem();
	void resumeSystem();
	void saveDefaults();
	void setAngle(float);
	void setAngleVar(float);
	void setAutoRemoveOnFinish(bool);
	void setBatchNode(cocos2d::CCParticleBatchNode*);
	void setBlendAdditive(bool);
	void setBlendFunc(cocos2d::_ccBlendFunc);
	void setDuration(float);
	void setEmissionRate(float);
	void setEmitterMode(int);
	void setEndColor(cocos2d::_ccColor4F const&);
	void setEndColorVar(cocos2d::_ccColor4F const&);
	void setEndRadius(float);
	void setEndRadiusVar(float);
	void setEndSize(float);
	void setEndSizeVar(float);
	void setEndSpin(float);
	void setEndSpinVar(float);
	void setGravity(cocos2d::CCPoint const&);
	void setLife(float);
	void setLifeVar(float);
	void setOpacityModifyRGB(bool);
	void setPosVar(cocos2d::CCPoint const&);
	void setPositionType(cocos2d::tCCPositionType);
	void setRadialAccel(float);
	void setRadialAccelVar(float);
	void setRotatePerSecond(float);
	void setRotatePerSecondVar(float);
	void setRotation(float);
	void setRotationIsDir(bool);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void setSourcePosition(cocos2d::CCPoint const&);
	void setSpeed(float);
	void setSpeedVar(float);
	void setStartColor(cocos2d::_ccColor4F const&);
	void setStartColorVar(cocos2d::_ccColor4F const&);
	void setStartRadius(float);
	void setStartRadiusVar(float);
	void setStartSize(float);
	void setStartSizeVar(float);
	void setStartSpin(float);
	void setStartSpinVar(float);
	void setTangentialAccel(float);
	void setTangentialAccelVar(float);
	void setTexture(cocos2d::CCTexture2D*);
	void setTotalParticles(unsigned int);
	void stopSystem();
	void update(float);
	void updateBlendFunc();
	void updateQuadWithParticle(cocos2d::sCCParticle*, cocos2d::CCPoint const&);
	void updateWithNoTime();
	virtual void initWithTotalParticles(uint);
	virtual void setAngle(float);
	virtual void setAngleVar(float);
	virtual void setAutoRemoveOnFinish(bool);
	virtual void setBatchNode(cocos2d::CCParticleBatchNode *);
	virtual void setBlendAdditive(bool);
	virtual void setBlendFunc(cocos2d::_ccBlendFunc);
	virtual void setDuration(float);
	virtual void setEmissionRate(float);
	virtual void setEmitterMode(int);
	virtual void setEndColor(cocos2d::_ccColor4F const&);
	virtual void setEndColorVar(cocos2d::_ccColor4F const&);
	virtual void setEndRadius(float);
	virtual void setEndRadiusVar(float);
	virtual void setEndSize(float);
	virtual void setEndSizeVar(float);
	virtual void setEndSpin(float);
	virtual void setEndSpinVar(float);
	virtual void setGravity(cocos2d::CCPoint const&);
	virtual void setLife(float);
	virtual void setLifeVar(float);
	virtual void setOpacityModifyRGB(bool);
	virtual void setPosVar(cocos2d::CCPoint const&);
	virtual void setPositionType(cocos2d::tCCPositionType);
	virtual void setRadialAccel(float);
	virtual void setRadialAccelVar(float);
	virtual void setRotatePerSecond(float);
	virtual void setRotatePerSecondVar(float);
	virtual void setRotation(float);
	virtual void setRotationIsDir(bool);
	virtual void setScale(float);
	virtual void setScaleX(float);
	virtual void setScaleY(float);
	virtual void setSourcePosition(cocos2d::CCPoint const&);
	virtual void setSpeed(float);
	virtual void setSpeedVar(float);
	virtual void setStartColor(cocos2d::_ccColor4F const&);
	virtual void setStartColorVar(cocos2d::_ccColor4F const&);
	virtual void setStartRadius(float);
	virtual void setStartRadiusVar(float);
	virtual void setStartSize(float);
	virtual void setStartSizeVar(float);
	virtual void setStartSpin(float);
	virtual void setStartSpinVar(float);
	virtual void setTangentialAccel(float);
	virtual void setTangentialAccelVar(float);
	virtual void setTexture(cocos2d::CCTexture2D *);
	virtual void setTotalParticles(uint);
	virtual void update(float);
	virtual void updateQuadWithParticle(cocos2d::sCCParticle *,cocos2d::CCPoint const&);
}
class cocos2d::CCParticleSystemQuad : cocos2d::CCParticleSystem {
	void CCParticleSystemQuad();
	void allocMemory();
	void create(char const*);
	void create();
	void createWithTotalParticles(unsigned int);
	void draw();
	void initIndices();
	void initTexCoordsWithRect(cocos2d::CCRect const&);
	void initWithTotalParticles(unsigned int);
	void listenBackToForeground(cocos2d::CCObject*);
	void postStep();
	void setBatchNode(cocos2d::CCParticleBatchNode*);
	void setDisplayFrame(cocos2d::CCSpriteFrame*);
	void setTexture(cocos2d::CCTexture2D*);
	void setTextureWithRect(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	void setTotalParticles(unsigned int);
	void setupVBO();
	void updateQuadWithParticle(cocos2d::sCCParticle*, cocos2d::CCPoint const&);
	virtual void initWithTotalParticles(uint);
	virtual void setBatchNode(cocos2d::CCParticleBatchNode *);
	virtual void setTexture(cocos2d::CCTexture2D *);
	virtual void setTotalParticles(uint);
	virtual void updateQuadWithParticle(cocos2d::sCCParticle *,cocos2d::CCPoint const&);
}
class cocos2d::CCPlace : cocos2d::CCActionInstant {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCPoint const&);
	void initWithPosition(cocos2d::CCPoint const&);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCPointArray : cocos2d::CCObject {
	void CCPointArray();
	void addControlPoint(cocos2d::CCPoint);
	void copyWithZone(cocos2d::CCZone*);
	void count();
	void create(unsigned int);
	void getControlPointAtIndex(unsigned int);
	void getControlPoints();
	void initWithCapacity(unsigned int);
	void insertControlPoint(cocos2d::CCPoint&, unsigned int);
	void removeControlPointAtIndex(unsigned int);
	void replaceControlPoint(cocos2d::CCPoint&, unsigned int);
	void reverse();
	void reverseInline();
	void setControlPoints(gd::vector<cocos2d::CCPoint*, gd::allocator<cocos2d::CCPoint*> >*);
	virtual void copyWithZone(cocos2d::CCZone *);
}
class cocos2d::CCPointObject {
	void pointWithCCPoint(cocos2d::CCPoint, cocos2d::CCPoint);
}
class cocos2d::CCPrettyPrinter : cocos2d::CCDataVisitor {
	void CCPrettyPrinter(int);
	void clear();
	void getResult();
	void setIndentLevel(int);
	void visit(cocos2d::CCDictionary const*);
	void visit(cocos2d::CCSet const*);
	void visit(cocos2d::CCBool const*);
	void visit(cocos2d::CCArray const*);
	void visit(cocos2d::CCFloat const*);
	void visit(cocos2d::CCDouble const*);
	void visit(cocos2d::CCString const*);
	void visit(cocos2d::CCInteger const*);
	void visitObject(cocos2d::CCObject const*);
	virtual void visit(cocos2d::CCBool const*);
	virtual void visit(cocos2d::CCInteger const*);
	virtual void visit(cocos2d::CCFloat const*);
	virtual void visit(cocos2d::CCDouble const*);
	virtual void visit(cocos2d::CCString const*);
	virtual void visit(cocos2d::CCArray const*);
	virtual void visit(cocos2d::CCDictionary const*);
	virtual void visit(cocos2d::CCSet const*);
	virtual void visitObject(cocos2d::CCObject const*);
}
class cocos2d::CCProfiler : cocos2d::CCObject {
	void createAndAddTimerWithName(char const*);
	void displayTimers();
	void init();
	void releaseAllTimers();
	void releaseTimer(char const*);
	void sharedProfiler();
}
class cocos2d::CCProfilingTimer : cocos2d::CCObject {
	void CCProfilingTimer();
	void description();
	void initWithName(char const*);
	void reset();
}
class cocos2d::CCProgressFromTo : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, float, float);
	void initWithDuration(float, float, float);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCProgressTimer : cocos2d::CCNodeRGBA {
	void CCProgressTimer();
	void boundaryTexCoord(char);
	void create(cocos2d::CCSprite*);
	void draw();
	void getColor();
	void getMidpoint();
	void getOpacity();
	void initWithSprite(cocos2d::CCSprite*);
	void setAnchorPoint(cocos2d::CCPoint);
	void setColor(cocos2d::_ccColor3B const&);
	void setMidpoint(cocos2d::CCPoint);
	void setOpacity(unsigned char);
	void setPercentage(float);
	void setReverseProgress(bool);
	void setSprite(cocos2d::CCSprite*);
	void setType(cocos2d::CCProgressTimerType);
	void textureCoordFromAlphaPoint(cocos2d::CCPoint);
	void updateBar();
	void updateColor();
	void updateProgress();
	void updateRadial();
	void vertexFromAlphaPoint(cocos2d::CCPoint);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setMidpoint(cocos2d::CCPoint);
	virtual void setOpacity(uchar);
}
class cocos2d::CCProgressTo : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, float);
	void initWithDuration(float, float);
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCRemoveSelf : cocos2d::CCActionInstant {
	void copyWithZone(cocos2d::CCZone*);
	void create(bool);
	void init(bool);
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCRenderTexture : cocos2d::CCNode {
	void CCRenderTexture();
	void begin();
	void beginWithClear(float, float, float, float);
	void beginWithClear(float, float, float, float, float);
	void beginWithClear(float, float, float, float, float, int);
	void beginWithClear(float, float, float, float, float, int, unsigned int);
	void clear(float, float, float, float);
	void clearDepth(float);
	void clearStencil(int);
	void create(int, int);
	void create(int, int, cocos2d::CCTexture2DPixelFormat);
	void create(int, int, cocos2d::CCTexture2DPixelFormat, unsigned int);
	void draw();
	void end();
	void getClearColor();
	void getClearDepth();
	void getClearFlags();
	void getClearStencil();
	void getSprite();
	void initWithWidthAndHeight(int, int, cocos2d::CCTexture2DPixelFormat);
	void initWithWidthAndHeight(int, int, cocos2d::CCTexture2DPixelFormat, unsigned int);
	void isAutoDraw();
	void listenToBackground(cocos2d::CCObject*);
	void listenToForeground(cocos2d::CCObject*);
	void newCCImage(bool);
	void saveToFile(char const*);
	void saveToFile(char const*, cocos2d::eImageFormat);
	void setAutoDraw(bool);
	void setClearColor(cocos2d::_ccColor4F const&);
	void setClearDepth(float);
	void setClearFlags(unsigned int);
	void setClearStencil(float);
	void setSprite(cocos2d::CCSprite*);
	void visit();
	virtual void setSprite(cocos2d::CCSprite *);
}
class cocos2d::CCRepeat : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCFiniteTimeAction*, unsigned int);
	void initWithAction(cocos2d::CCFiniteTimeAction*, unsigned int);
	void isDone();
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void stop();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCRepeatForever : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*);
	void initWithAction(cocos2d::CCActionInterval*);
	void isDone();
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void step(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void step(float);
}
class cocos2d::CCReuseGrid : cocos2d::CCActionInstant {
	void create(int);
	void initWithTimes(int);
	void startWithTarget(cocos2d::CCNode*);
	virtual void startWithTarget(cocos2d::CCNode *);
}
class cocos2d::CCReverseTime : cocos2d::CCActionInterval {
	void CCReverseTime();
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCFiniteTimeAction*);
	void initWithAction(cocos2d::CCFiniteTimeAction*);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void stop();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCRGBAProtocol {
}
class cocos2d::CCRipple3D : cocos2d::CCGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, cocos2d::CCPoint const&, float, unsigned int, float);
	void initWithDuration(float, cocos2d::CCSize const&, cocos2d::CCPoint const&, float, unsigned int, float);
	void setPosition(cocos2d::CCPoint const&);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCRotateBy : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, float);
	void create(float, float, float);
	void initWithDuration(float, float);
	void initWithDuration(float, float, float);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCRotateTo : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, float);
	void create(float, float, float);
	void initWithDuration(float, float);
	void initWithDuration(float, float, float);
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float,float,float);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCSAXDelegator {
}
class cocos2d::CCScaleBy : cocos2d::CCScaleTo {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, float);
	void create(float, float, float);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
}
class cocos2d::CCScaleTo : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, float);
	void create(float, float, float);
	void initWithDuration(float, float);
	void initWithDuration(float, float, float);
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCScene : cocos2d::CCNode {
	void CCScene();
	void create();
	void getHighestChildZ();
	void init();
}
class cocos2d::CCSceneDelegate {
	void willSwitchToScene(cocos2d::CCScene*);
}
class cocos2d::CCSceneTransitionDelegate {
	void sceneWillResume();
}
class cocos2d::CCScheduler : cocos2d::CCObject {
	void CCScheduler();
	void appendIn(cocos2d::_listEntry**, cocos2d::CCObject*, bool);
	void isTargetPaused(cocos2d::CCObject*);
	void pauseAllTargets();
	void pauseAllTargetsWithMinPriority(int);
	void pauseTarget(cocos2d::CCObject*);
	void priorityIn(cocos2d::_listEntry**, cocos2d::CCObject*, int, bool);
	void removeHashElement(cocos2d::_hashSelectorEntry*);
	void removeUpdateFromHash(cocos2d::_listEntry*);
	void resumeTarget(cocos2d::CCObject*);
	void resumeTargets(cocos2d::CCSet*);
	void scheduleScriptFunc(unsigned int, float, bool);
	void scheduleSelector(cocos2d::SEL_SCHEDULE, cocos2d::CCObject*, float, bool);
	void scheduleSelector(cocos2d::SEL_SCHEDULE, cocos2d::CCObject*, float, unsigned int, float, bool);
	void scheduleUpdateForTarget(cocos2d::CCObject*, int, bool);
	void unscheduleAll();
	void unscheduleAllForTarget(cocos2d::CCObject*);
	void unscheduleAllWithMinPriority(int);
	void unscheduleScriptEntry(unsigned int);
	void unscheduleSelector(cocos2d::SEL_SCHEDULE, cocos2d::CCObject*);
	void unscheduleUpdateForTarget(cocos2d::CCObject const*);
	void update(float);
	virtual void update(float);
}
class cocos2d::CCSchedulerScriptHandlerEntry : cocos2d::CCScriptHandlerEntry {
	void create(int, float, bool);
	void init(float, bool);
}
class cocos2d::CCScriptHandlerEntry : cocos2d::CCObject {
	void CCScriptHandlerEntry(int);
	void create(int);
}
class cocos2d::CCScrollLayer : cocos2d::CCLayer {
	void CCScrollLayer();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void initWithLayers(cocos2d::CCArray*, int);
	void nodeWithLayers(cocos2d::CCArray*, int);
	void setMaximumScrollHeight(float);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
}
class cocos2d::CCSequence : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCFiniteTimeAction*, ...);
	void create(cocos2d::CCArray*);
	void createWithTwoActions(cocos2d::CCFiniteTimeAction*, cocos2d::CCFiniteTimeAction*);
	void createWithVariableList(cocos2d::CCFiniteTimeAction*, gd::__va_list);
	void initWithTwoActions(cocos2d::CCFiniteTimeAction*, cocos2d::CCFiniteTimeAction*);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void stop();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCSet : cocos2d::CCObject {
	void CCSet(cocos2d::CCSet const&);
	void CCSet();
	void acceptVisitor(cocos2d::CCDataVisitor&);
	void addObject(cocos2d::CCObject*);
	void anyObject();
	void begin();
	void containsObject(cocos2d::CCObject*);
	void copy();
	void count();
	void create();
	void end();
	void mutableCopy();
	void removeAllObjects();
	void removeObject(cocos2d::CCObject*);
	virtual void acceptVisitor(cocos2d::CCDataVisitor &);
}
class cocos2d::CCShaderCache : cocos2d::CCObject {
	void CCShaderCache();
	void addProgram(cocos2d::CCGLProgram*, char const*);
	void init();
	void loadDefaultShader(cocos2d::CCGLProgram*, int);
	void loadDefaultShaders();
	void programForKey(char const*);
	void purgeSharedShaderCache();
	void reloadDefaultShaders();
	void sharedShaderCache();
}
class cocos2d::CCShaky3D : cocos2d::CCGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, int, bool);
	void initWithDuration(float, cocos2d::CCSize const&, int, bool);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCShakyTiles3D : cocos2d::CCTiledGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, int, bool);
	void initWithDuration(float, cocos2d::CCSize const&, int, bool);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float,cocos2d::CCSize const&,int,bool);
	virtual void update(float);
}
class cocos2d::CCShatteredTiles3D : cocos2d::CCTiledGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, int, bool);
	void initWithDuration(float, cocos2d::CCSize const&, int, bool);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float,cocos2d::CCSize const&,int,bool);
	virtual void update(float);
}
class cocos2d::CCShow : cocos2d::CCActionInstant {
	void copyWithZone(cocos2d::CCZone*);
	void create();
	void reverse();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCShuffleTiles : cocos2d::CCTiledGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, unsigned int);
	void getDelta(cocos2d::CCSize const&);
	void initWithDuration(float, cocos2d::CCSize const&, unsigned int);
	void placeTile(cocos2d::CCPoint const&, cocos2d::Tile*);
	void shuffle(unsigned int*, unsigned int);
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float,cocos2d::CCSize const&,uint);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCSkewBy : cocos2d::CCSkewTo {
	void create(float, float, float);
	void initWithDuration(float, float, float);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	virtual void initWithDuration(float,float,float);
	virtual void startWithTarget(cocos2d::CCNode *);
}
class cocos2d::CCSkewTo : cocos2d::CCActionInterval {
	void CCSkewTo();
	void copyWithZone(cocos2d::CCZone*);
	void create(float, float, float);
	void initWithDuration(float, float, float);
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float,float,float);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCSpawn : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCFiniteTimeAction*, ...);
	void create(cocos2d::CCArray*);
	void createWithTwoActions(cocos2d::CCFiniteTimeAction*, cocos2d::CCFiniteTimeAction*);
	void createWithVariableList(cocos2d::CCFiniteTimeAction*, gd::__va_list);
	void initWithTwoActions(cocos2d::CCFiniteTimeAction*, cocos2d::CCFiniteTimeAction*);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void stop();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCSpeed : cocos2d::CCAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCActionInterval*, float);
	void initWithAction(cocos2d::CCActionInterval*, float);
	void isDone();
	void reverse();
	void setInnerAction(cocos2d::CCActionInterval*);
	void startWithTarget(cocos2d::CCNode*);
	void step(float);
	void stop();
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void step(float);
}
class cocos2d::CCSplitCols : cocos2d::CCTiledGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, unsigned int);
	void initWithDuration(float, unsigned int);
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float,uint);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCSplitRows : cocos2d::CCTiledGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, unsigned int);
	void initWithDuration(float, unsigned int);
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float,uint);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCSprite : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	void CCSprite();
	void addChild(cocos2d::CCNode*);
	void addChild(cocos2d::CCNode*, int);
	void addChild(cocos2d::CCNode*, int, int);
	void create(char const*);
	void create(char const*, cocos2d::CCRect const&);
	void create();
	void createWithSpriteFrame(cocos2d::CCSpriteFrame*);
	void createWithSpriteFrameName(char const*);
	void createWithTexture(cocos2d::CCTexture2D*);
	void createWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	void displayFrame();
	void draw();
	void getBatchNode();
	void getBlendFunc();
	void getTexture();
	void ignoreAnchorPointForPosition(bool);
	void init();
	void initWithFile(char const*);
	void initWithFile(char const*, cocos2d::CCRect const&);
	void initWithSpriteFrame(cocos2d::CCSpriteFrame*);
	void initWithSpriteFrameName(char const*);
	void initWithTexture(cocos2d::CCTexture2D*);
	void initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	void initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool);
	void isDirty();
	void isFlipX();
	void isFlipY();
	void isFrameDisplayed(cocos2d::CCSpriteFrame*);
	void isOpacityModifyRGB();
	void refreshTextureRect();
	void removeAllChildrenWithCleanup(bool);
	void removeChild(cocos2d::CCNode*, bool);
	void reorderChild(cocos2d::CCNode*, int);
	void setAnchorPoint(cocos2d::CCPoint const&);
	void setBatchNode(cocos2d::CCSpriteBatchNode*);
	void setBlendFunc(cocos2d::_ccBlendFunc);
	void setChildColor(cocos2d::_ccColor3B const&);
	void setChildOpacity(unsigned char);
	void setColor(cocos2d::_ccColor3B const&);
	void setDirty(bool);
	void setDirtyRecursively(bool);
	void setDisplayFrame(cocos2d::CCSpriteFrame*);
	void setDisplayFrameWithAnimationName(char const*, int);
	void setFlipX(bool);
	void setFlipY(bool);
	void setOpacity(unsigned char);
	void setOpacityModifyRGB(bool);
	void setPosition(cocos2d::CCPoint const&);
	void setReorderChildDirtyRecursively();
	void setRotation(float);
	void setRotationX(float);
	void setRotationY(float);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void setSkewX(float);
	void setSkewY(float);
	void setTexture(cocos2d::CCTexture2D*);
	void setTextureCoords(cocos2d::CCRect const&);
	void setTextureRect(cocos2d::CCRect const&);
	void setTextureRect(cocos2d::CCRect const&, bool, cocos2d::CCSize const&);
	void setVertexRect(cocos2d::CCRect const&);
	void setVertexZ(float);
	void setVisible(bool);
	void sortAllChildren();
	void updateBlendFunc();
	void updateColor();
	void updateDisplayedColor(cocos2d::_ccColor3B const&);
	void updateDisplayedOpacity(unsigned char);
	void updateTransform();
	virtual void addChild(cocos2d::CCNode *);
	virtual void addChild(cocos2d::CCNode *,int);
	virtual void addChild(cocos2d::CCNode *,int,int);
	virtual void ignoreAnchorPointForPosition(bool);
	virtual void initWithFile(char const*);
	virtual void initWithFile(char const*,cocos2d::CCRect const&);
	virtual void initWithSpriteFrame(cocos2d::CCSpriteFrame *);
	virtual void initWithSpriteFrameName(char const*);
	virtual void initWithTexture(cocos2d::CCTexture2D *);
	virtual void initWithTexture(cocos2d::CCTexture2D *,cocos2d::CCRect const&);
	virtual void initWithTexture(cocos2d::CCTexture2D *,cocos2d::CCRect const&,bool);
	virtual void isFrameDisplayed(cocos2d::CCSpriteFrame *);
	virtual void removeAllChildrenWithCleanup(bool);
	virtual void removeChild(cocos2d::CCNode *,bool);
	virtual void reorderChild(cocos2d::CCNode *,int);
	virtual void setAnchorPoint(cocos2d::CCPoint const&);
	virtual void setBatchNode(cocos2d::CCSpriteBatchNode *);
	virtual void setBlendFunc(cocos2d::_ccBlendFunc);
	virtual void setChildColor(cocos2d::_ccColor3B const&);
	virtual void setChildOpacity(uchar);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setDirty(bool);
	virtual void setDirtyRecursively(bool);
	virtual void setDisplayFrame(cocos2d::CCSpriteFrame *);
	virtual void setDisplayFrameWithAnimationName(char const*,int);
	virtual void setOpacity(uchar);
	virtual void setOpacityModifyRGB(bool);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void setRotation(float);
	virtual void setRotationX(float);
	virtual void setRotationY(float);
	virtual void setScale(float);
	virtual void setScaleX(float);
	virtual void setScaleY(float);
	virtual void setSkewX(float);
	virtual void setSkewY(float);
	virtual void setTexture(cocos2d::CCTexture2D *);
	virtual void setTextureCoords(cocos2d::CCRect const&);
	virtual void setTextureRect(cocos2d::CCRect const&);
	virtual void setTextureRect(cocos2d::CCRect const&,bool,cocos2d::CCSize const&);
	virtual void setVertexRect(cocos2d::CCRect const&);
	virtual void setVertexZ(float);
	virtual void setVisible(bool);
	virtual void updateDisplayedColor(cocos2d::_ccColor3B const&);
	virtual void updateDisplayedOpacity(uchar);
}
class cocos2d::CCSpriteBatchNode : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	void CCSpriteBatchNode();
	void addChild(cocos2d::CCNode*);
	void addChild(cocos2d::CCNode*, int);
	void addChild(cocos2d::CCNode*, int, int);
	void addSpriteWithoutQuad(cocos2d::CCSprite*, unsigned int, int);
	void appendChild(cocos2d::CCSprite*);
	void atlasIndexForChild(cocos2d::CCSprite*, int);
	void create(char const*, unsigned int);
	void createWithTexture(cocos2d::CCTexture2D*);
	void createWithTexture(cocos2d::CCTexture2D*, unsigned int);
	void draw();
	void getAtlasCapacity();
	void getBlendFunc();
	void getTexture();
	void getUsedAtlasCapacity();
	void highestAtlasIndexInChild(cocos2d::CCSprite*);
	void increaseAtlasCapacity(unsigned int);
	void increaseAtlasCapacity();
	void init();
	void initWithFile(char const*, unsigned int);
	void initWithTexture(cocos2d::CCTexture2D*, unsigned int);
	void insertChild(cocos2d::CCSprite*, unsigned int);
	void insertQuadFromSprite(cocos2d::CCSprite*, unsigned int);
	void lowestAtlasIndexInChild(cocos2d::CCSprite*);
	void manualSortAllChildren();
	void rebuildIndexInOrder(cocos2d::CCSprite*, unsigned int);
	void removeAllChildrenWithCleanup(bool);
	void removeChild(cocos2d::CCNode*, bool);
	void removeChildAtIndex(unsigned int, bool);
	void removeSpriteFromAtlas(cocos2d::CCSprite*);
	void reorderBatch(bool);
	void reorderChild(cocos2d::CCNode*, int);
	void setBlendFunc(cocos2d::_ccBlendFunc);
	void setTexture(cocos2d::CCTexture2D*);
	void sortAllChildren();
	void swap(int, int);
	void updateAtlasIndex(cocos2d::CCSprite*, int*);
	void updateBlendFunc();
	void updateQuadFromSprite(cocos2d::CCSprite*, unsigned int);
	void visit();
	virtual void addChild(cocos2d::CCNode *);
	virtual void addChild(cocos2d::CCNode *,int);
	virtual void addChild(cocos2d::CCNode *,int,int);
	virtual void removeAllChildrenWithCleanup(bool);
	virtual void removeChild(cocos2d::CCNode *,bool);
	virtual void reorderChild(cocos2d::CCNode *,int);
	virtual void setBlendFunc(cocos2d::_ccBlendFunc);
	virtual void setTexture(cocos2d::CCTexture2D *);
}
class cocos2d::CCSpriteExtra : cocos2d::CCSprite {
}
class cocos2d::CCSpriteFrame : cocos2d::CCObject {
	void CCSpriteFrame();
	void copyWithZone(cocos2d::CCZone*);
	void create(char const*, cocos2d::CCRect const&);
	void create(char const*, cocos2d::CCRect const&, bool, cocos2d::CCPoint const&, cocos2d::CCSize const&);
	void createWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	void createWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool, cocos2d::CCPoint const&, cocos2d::CCSize const&);
	void getOffset();
	void getOffsetInPixels();
	void getTexture();
	void initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	void initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool, cocos2d::CCPoint const&, cocos2d::CCSize const&);
	void initWithTextureFilename(char const*, cocos2d::CCRect const&);
	void initWithTextureFilename(char const*, cocos2d::CCRect const&, bool, cocos2d::CCPoint const&, cocos2d::CCSize const&);
	void setOffset(cocos2d::CCPoint const&);
	void setOffsetInPixels(cocos2d::CCPoint const&);
	void setRect(cocos2d::CCRect const&);
	void setRectInPixels(cocos2d::CCRect const&);
	void setTexture(cocos2d::CCTexture2D*);
	virtual void copyWithZone(cocos2d::CCZone *);
}
class cocos2d::CCSpriteFrameCache : cocos2d::CCObject {
	void addSpriteFrame(cocos2d::CCSpriteFrame*, char const*);
	void addSpriteFramesWithDictionary(cocos2d::CCDictionary*, cocos2d::CCTexture2D*);
	void addSpriteFramesWithFile(char const*);
	void addSpriteFramesWithFile(char const*, cocos2d::CCTexture2D*);
	void addSpriteFramesWithFile(char const*, char const*);
	void init();
	void purgeSharedSpriteFrameCache();
	void removeSpriteFrameByName(char const*);
	void removeSpriteFrames();
	void removeSpriteFramesFromDictionary(cocos2d::CCDictionary*);
	void removeSpriteFramesFromFile(char const*);
	void removeSpriteFramesFromTexture(cocos2d::CCTexture2D*);
	void removeUnusedSpriteFrames();
	void sharedSpriteFrameCache();
	void spriteFrameByName(char const*);
}
class cocos2d::CCStandardTouchHandler : cocos2d::CCTouchHandler {
	void handlerWithDelegate(cocos2d::CCTouchDelegate*, int);
	void initWithDelegate(cocos2d::CCTouchDelegate*, int);
	virtual void initWithDelegate(cocos2d::CCTouchDelegate *,int);
}
class cocos2d::CCStopGrid : cocos2d::CCActionInstant {
	void create();
	void startWithTarget(cocos2d::CCNode*);
	virtual void startWithTarget(cocos2d::CCNode *);
}
class cocos2d::CCString : cocos2d::CCObject {
	void CCString(char const*);
	void CCString(cocos2d::CCString const&);
	void CCString(gd::string const&);
	void CCString();
	void acceptVisitor(cocos2d::CCDataVisitor&);
	void boolValue();
	void compare(char const*);
	void copyWithZone(cocos2d::CCZone*);
	void create(gd::string const&);
	void createWithContentsOfFile(char const*);
	void createWithData(unsigned char const*, unsigned long);
	void createWithFormat(char const*, ...);
	void doubleValue();
	void floatValue();
	void getCString();
	void initWithFormat(char const*, ...);
	void initWithFormatAndValist(char const*, gd::__va_list);
	void intValue();
	void isEqual(cocos2d::CCObject const*);
	void length();
	void operator=(cocos2d::CCString const&);
	void uintValue();
	virtual void acceptVisitor(cocos2d::CCDataVisitor &);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void isEqual(cocos2d::CCObject const*);
}
class cocos2d::CCTargetedAction : cocos2d::CCActionInterval {
	void CCTargetedAction();
	void copyWithZone(cocos2d::CCZone*);
	void create(cocos2d::CCNode*, cocos2d::CCFiniteTimeAction*);
	void initWithTarget(cocos2d::CCNode*, cocos2d::CCFiniteTimeAction*);
	void startWithTarget(cocos2d::CCNode*);
	void stop();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCTargetedTouchHandler : cocos2d::CCTouchHandler {
	void getClaimedTouches();
	void handlerWithDelegate(cocos2d::CCTouchDelegate*, int, bool);
	void initWithDelegate(cocos2d::CCTouchDelegate*, int, bool);
	void isSwallowsTouches();
	void setSwallowsTouches(bool);
}
class cocos2d::CCTextFieldDelegate {
	void onDraw(cocos2d::CCTextFieldTTF*);
	void onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF*);
	void onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF*, char const*, int);
	void onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF*);
	void onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int);
	void textChanged();
	virtual void onDraw(cocos2d::CCTextFieldTTF *);
	virtual void onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *);
	virtual void onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *,char const*,int);
	virtual void onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *);
	virtual void onTextFieldInsertText(cocos2d::CCTextFieldTTF *,char const*,int);
}
class cocos2d::CCTextFieldTTF : cocos2d::CCLabelTTF, cocos2d::CCIMEDelegate {
	void CCTextFieldTTF();
	void attachWithIME();
	void canAttachWithIME();
	void canDetachWithIME();
	void deleteBackward();
	void detachWithIME();
	void draw();
	void getColorSpaceHolder();
	void getContentText();
	void getPlaceHolder();
	void getString();
	void initWithPlaceHolder(char const*, cocos2d::CCSize const&, cocos2d::CCTextAlignment, char const*, float);
	void initWithPlaceHolder(char const*, char const*, float);
	void insertText(char const*, int);
	void isSecureTextEntry();
	void setColorSpaceHolder(cocos2d::_ccColor3B const&);
	void setPlaceHolder(char const*);
	void setSecureTextEntry(bool);
	void setString(char const*);
	void textFieldWithPlaceHolder(char const*, cocos2d::CCSize const&, cocos2d::CCTextAlignment, char const*, float);
	void textFieldWithPlaceHolder(char const*, char const*, float);
	virtual void insertText(char const*,int);
	virtual void setColorSpaceHolder(cocos2d::_ccColor3B const&);
	virtual void setPlaceHolder(char const*);
	virtual void setSecureTextEntry(bool);
	virtual void setString(char const*);
}
class cocos2d::CCTexture2D : cocos2d::CCObject {
	void CCTexture2D();
	void PVRImagesHavePremultipliedAlpha(bool);
	void bitsPerPixelForFormat(cocos2d::CCTexture2DPixelFormat);
	void bitsPerPixelForFormat();
	void defaultAlphaPixelFormat();
	void description();
	void drawAtPoint(cocos2d::CCPoint const&);
	void drawInRect(cocos2d::CCRect const&);
	void generateMipmap();
	void getContentSize();
	void getContentSizeInPixels();
	void getMaxS();
	void getMaxT();
	void getName();
	void getPixelFormat();
	void getPixelsHigh();
	void getPixelsWide();
	void getShaderProgram();
	void hasMipmaps();
	void hasPremultipliedAlpha();
	void initPremultipliedATextureWithImage(cocos2d::CCImage*, unsigned int, unsigned int);
	void initWithData(void const*, cocos2d::CCTexture2DPixelFormat, unsigned int, unsigned int, cocos2d::CCSize const&);
	void initWithETCFile(char const*);
	void initWithImage(cocos2d::CCImage*);
	void initWithPVRFile(char const*);
	void initWithString(char const*, cocos2d::_ccFontDefinition*);
	void initWithString(char const*, char const*, float);
	void initWithString(char const*, char const*, float, cocos2d::CCSize const&, cocos2d::CCTextAlignment, cocos2d::CCVerticalTextAlignment);
	void keepData(void*, unsigned int);
	void releaseData(void*);
	void releaseGLTexture();
	void setAliasTexParameters();
	void setAntiAliasTexParameters();
	void setDefaultAlphaPixelFormat(cocos2d::CCTexture2DPixelFormat);
	void setMaxS(float);
	void setMaxT(float);
	void setShaderProgram(cocos2d::CCGLProgram*);
	void setTexParameters(cocos2d::_ccTexParams*);
	void stringForFormat();
	virtual void setMaxS(float);
	virtual void setMaxT(float);
	virtual void setShaderProgram(cocos2d::CCGLProgram *);
}
class cocos2d::CCTextureAtlas : cocos2d::CCObject {
	void CCTextureAtlas();
	void create(char const*, unsigned int);
	void createWithTexture(cocos2d::CCTexture2D*, unsigned int);
	void description();
	void drawNumberOfQuads(unsigned int);
	void drawNumberOfQuads(unsigned int, unsigned int);
	void drawQuads();
	void fillWithEmptyQuadsFromIndex(unsigned int, unsigned int);
	void getCapacity();
	void getQuads();
	void getTexture();
	void getTotalQuads();
	void increaseTotalQuadsWith(unsigned int);
	void initWithFile(char const*, unsigned int);
	void initWithTexture(cocos2d::CCTexture2D*, unsigned int);
	void insertQuad(cocos2d::_ccV3F_C4B_T2F_Quad*, unsigned int);
	void insertQuadFromIndex(unsigned int, unsigned int);
	void insertQuads(cocos2d::_ccV3F_C4B_T2F_Quad*, unsigned int, unsigned int);
	void listenBackToForeground(cocos2d::CCObject*);
	void mapBuffers();
	void moveQuadsFromIndex(unsigned int, unsigned int);
	void moveQuadsFromIndex(unsigned int, unsigned int, unsigned int);
	void removeAllQuads();
	void removeQuadAtIndex(unsigned int);
	void removeQuadsAtIndex(unsigned int, unsigned int);
	void resizeCapacity(unsigned int);
	void setQuads(cocos2d::_ccV3F_C4B_T2F_Quad*);
	void setTexture(cocos2d::CCTexture2D*);
	void setupIndices();
	void setupVBO();
	void updateQuad(cocos2d::_ccV3F_C4B_T2F_Quad*, unsigned int);
	virtual void setQuads(cocos2d::_ccV3F_C4B_T2F_Quad *);
	virtual void setTexture(cocos2d::CCTexture2D *);
}
class cocos2d::CCTextureCache : cocos2d::CCObject {
	void CCTextureCache();
	void addETCImage(char const*);
	void addImage(char const*, bool);
	void addImageAsync(char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void addImageAsyncCallBack(float);
	void addPVRImage(char const*);
	void addUIImage(cocos2d::CCImage*, char const*);
	void description();
	void dumpCachedTextureInfo();
	void purgeSharedTextureCache();
	void reloadAllTextures();
	void reloadTexture(char const*);
	void removeAllTextures();
	void removeTexture(cocos2d::CCTexture2D*);
	void removeTextureForKey(char const*);
	void removeUnusedTextures();
	void sharedTextureCache();
	void snapshotTextures();
	void textureForKey(char const*);
}
class cocos2d::CCTextureETC : cocos2d::CCObject {
	void CCTextureETC();
	void getHeight();
	void getName();
	void getWidth();
	void initWithFile(char const*);
	void loadTexture(char const*);
}
class cocos2d::CCTextureProtocol : cocos2d::CCBlendProtocol {
}
class cocos2d::CCTexturePVR : cocos2d::CCObject {
	void CCTexturePVR();
	void create(char const*);
	void createGLTexture();
	void initWithContentsOfFile(char const*);
	void unpackPVRv2Data(unsigned char*, unsigned int);
	void unpackPVRv3Data(unsigned char*, unsigned int);
}
class cocos2d::CCTiledGrid3D : cocos2d::CCGridBase {
	void CCTiledGrid3D();
	void blit();
	void calculateVertexPoints();
	void create(cocos2d::CCSize const&);
	void create(cocos2d::CCSize const&, cocos2d::CCTexture2D*, bool);
	void originalTile(cocos2d::CCPoint const&);
	void reuse();
	void setTile(cocos2d::CCPoint const&, cocos2d::_ccQuad3 const&);
	void tile(cocos2d::CCPoint const&);
}
class cocos2d::CCTiledGrid3DAction : cocos2d::CCGridAction {
	void CCTiledGrid3DAction();
	void create(float, cocos2d::CCSize const&);
	void getGrid();
	void originalTile(cocos2d::CCPoint const&);
	void setTile(cocos2d::CCPoint const&, cocos2d::_ccQuad3 const&);
	void tile(cocos2d::CCPoint const&);
}
class cocos2d::CCTileMapAtlas : cocos2d::CCAtlasNode {
	void CCTileMapAtlas();
	void calculateItemsToRender();
	void create(char const*, char const*, int, int);
	void getTGAInfo();
	void initWithTileFile(char const*, char const*, int, int);
	void loadTGAfile(char const*);
	void releaseMap();
	void setTGAInfo(cocos2d::sImageTGA*);
	void setTile(cocos2d::_ccColor3B const&, cocos2d::CCPoint const&);
	void tileAt(cocos2d::CCPoint const&);
	void updateAtlasValueAt(cocos2d::CCPoint const&, cocos2d::_ccColor3B const&, unsigned int);
	void updateAtlasValues();
	virtual void setTGAInfo(cocos2d::sImageTGA *);
}
class cocos2d::CCTimer : cocos2d::CCObject {
	void CCTimer();
	void getInterval();
	void getSelector();
	void initWithScriptHandler(int, float);
	void initWithTarget(cocos2d::CCObject*, cocos2d::SEL_SCHEDULE);
	void initWithTarget(cocos2d::CCObject*, cocos2d::SEL_SCHEDULE, float, unsigned int, float);
	void setInterval(float);
	void timerWithScriptHandler(int, float);
	void timerWithTarget(cocos2d::CCObject*, cocos2d::SEL_SCHEDULE);
	void timerWithTarget(cocos2d::CCObject*, cocos2d::SEL_SCHEDULE, float);
	void update(float);
	virtual void update(float);
}
class cocos2d::CCTintBy : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, short, short, short);
	void initWithDuration(float, short, short, short);
	void reverse();
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCTintTo : cocos2d::CCActionInterval {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, unsigned char, unsigned char, unsigned char);
	void initWithDuration(float, unsigned char, unsigned char, unsigned char);
	void startWithTarget(cocos2d::CCNode*);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCTMXLayer : cocos2d::CCSpriteBatchNode {
	void CCTMXLayer();
	void addChild(cocos2d::CCNode*, int, int);
	void appendTileForGID(unsigned int, cocos2d::CCPoint const&);
	void atlasIndexForExistantZ(unsigned int);
	void atlasIndexForNewZ(int);
	void calculateLayerOffset(cocos2d::CCPoint const&);
	void create(cocos2d::CCTMXTilesetInfo*, cocos2d::CCTMXLayerInfo*, cocos2d::CCTMXMapInfo*);
	void getProperties();
	void getTileSet();
	void initWithTilesetInfo(cocos2d::CCTMXTilesetInfo*, cocos2d::CCTMXLayerInfo*, cocos2d::CCTMXMapInfo*);
	void insertTileForGID(unsigned int, cocos2d::CCPoint const&);
	void parseInternalProperties();
	void positionAt(cocos2d::CCPoint const&);
	void positionForHexAt(cocos2d::CCPoint const&);
	void positionForIsoAt(cocos2d::CCPoint const&);
	void positionForOrthoAt(cocos2d::CCPoint const&);
	void propertyNamed(char const*);
	void releaseMap();
	void removeChild(cocos2d::CCNode*, bool);
	void removeTileAt(cocos2d::CCPoint const&);
	void reusedTileWithRect(cocos2d::CCRect);
	void setProperties(cocos2d::CCDictionary*);
	void setTileGID(unsigned int, cocos2d::CCPoint const&);
	void setTileGID(unsigned int, cocos2d::CCPoint const&, cocos2d::ccTMXTileFlags_);
	void setTileSet(cocos2d::CCTMXTilesetInfo*);
	void setupTileSprite(cocos2d::CCSprite*, cocos2d::CCPoint, unsigned int);
	void setupTiles();
	void tileAt(cocos2d::CCPoint const&);
	void tileGIDAt(cocos2d::CCPoint const&);
	void tileGIDAt(cocos2d::CCPoint const&, cocos2d::ccTMXTileFlags_*);
	void updateTileForGID(unsigned int, cocos2d::CCPoint const&);
	void vertexZForPos(cocos2d::CCPoint const&);
	virtual void addChild(cocos2d::CCNode *,int,int);
	virtual void removeChild(cocos2d::CCNode *,bool);
	virtual void setProperties(cocos2d::CCDictionary *);
	virtual void setTileSet(cocos2d::CCTMXTilesetInfo *);
}
class cocos2d::CCTMXLayerInfo : cocos2d::CCObject {
	void CCTMXLayerInfo();
	void getProperties();
	void setProperties(cocos2d::CCDictionary*);
	virtual void setProperties(cocos2d::CCDictionary *);
}
class cocos2d::CCTMXMapInfo : cocos2d::CCObject, cocos2d::CCSAXDelegator {
	void CCTMXMapInfo();
	void endElement(void*, char const*);
	void formatWithTMXFile(char const*);
	void formatWithXML(char const*, char const*);
	void getLayers();
	void getObjectGroups();
	void getProperties();
	void getTileProperties();
	void getTilesets();
	void initWithTMXFile(char const*);
	void initWithXML(char const*, char const*);
	void internalInit(char const*, char const*);
	void parseXMLFile(char const*);
	void parseXMLString(char const*);
	void setLayers(cocos2d::CCArray*);
	void setObjectGroups(cocos2d::CCArray*);
	void setProperties(cocos2d::CCDictionary*);
	void setTileProperties(cocos2d::CCDictionary*);
	void setTilesets(cocos2d::CCArray*);
	void startElement(void*, char const*, char const**);
	void textHandler(void*, char const*, int);
	virtual void endElement(void *,char const*);
	virtual void setLayers(cocos2d::CCArray *);
	virtual void setObjectGroups(cocos2d::CCArray *);
	virtual void setProperties(cocos2d::CCDictionary *);
	virtual void setTilesets(cocos2d::CCArray *);
	virtual void startElement(void *,char const*,char const**);
	virtual void textHandler(void *,char const*,int);
}
class cocos2d::CCTMXObjectGroup : cocos2d::CCObject {
	void CCTMXObjectGroup();
	void getObjects();
	void getProperties();
	void objectNamed(char const*);
	void propertyNamed(char const*);
	void setObjects(cocos2d::CCArray*);
	void setProperties(cocos2d::CCDictionary*);
	virtual void setObjects(cocos2d::CCArray *);
	virtual void setProperties(cocos2d::CCDictionary *);
}
class cocos2d::CCTMXTiledMap : cocos2d::CCNode {
	void CCTMXTiledMap();
	void buildWithMapInfo(cocos2d::CCTMXMapInfo*);
	void create(char const*);
	void createWithXML(char const*, char const*);
	void getObjectGroups();
	void getProperties();
	void initWithTMXFile(char const*);
	void initWithXML(char const*, char const*);
	void layerNamed(char const*);
	void objectGroupNamed(char const*);
	void parseLayer(cocos2d::CCTMXLayerInfo*, cocos2d::CCTMXMapInfo*);
	void propertiesForGID(int);
	void propertyNamed(char const*);
	void setObjectGroups(cocos2d::CCArray*);
	void setProperties(cocos2d::CCDictionary*);
	void tilesetForLayer(cocos2d::CCTMXLayerInfo*, cocos2d::CCTMXMapInfo*);
	virtual void setObjectGroups(cocos2d::CCArray *);
	virtual void setProperties(cocos2d::CCDictionary *);
}
class cocos2d::CCTMXTilesetInfo : cocos2d::CCObject {
	void CCTMXTilesetInfo();
	void rectForGID(unsigned int);
}
class cocos2d::CCToggleVisibility : cocos2d::CCActionInstant {
	void copyWithZone(cocos2d::CCZone*);
	void create();
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCTouch : cocos2d::CCObject {
	void getDelta();
	void getLocation();
	void getLocationInView();
	void getPreviousLocation();
	void getPreviousLocationInView();
	void getStartLocation();
	void getStartLocationInView();
}
class cocos2d::CCTouchDelegate {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*);
	void ccTouchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*);
	void ccTouchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*);
	void ccTouchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchesBegan(cocos2d::CCSet *,cocos2d::CCEvent *);
	virtual void ccTouchesCancelled(cocos2d::CCSet *,cocos2d::CCEvent *);
	virtual void ccTouchesEnded(cocos2d::CCSet *,cocos2d::CCEvent *);
	virtual void ccTouchesMoved(cocos2d::CCSet *,cocos2d::CCEvent *);
}
class cocos2d::CCTouchDispatcher : cocos2d::CCObject, cocos2d::EGLTouchDelegate {
	void addStandardDelegate(cocos2d::CCTouchDelegate*, int);
	void addTargetedDelegate(cocos2d::CCTouchDelegate*, int, bool);
	void decrementForcePrio(int);
	void findHandler(cocos2d::CCTouchDelegate*);
	void findHandler(cocos2d::CCArray*, cocos2d::CCTouchDelegate*);
	void forceAddHandler(cocos2d::CCTouchHandler*, cocos2d::CCArray*);
	void forceRemoveAllDelegates();
	void forceRemoveDelegate(cocos2d::CCTouchDelegate*);
	void incrementForcePrio(int);
	void init();
	void isDispatchEvents();
	void rearrangeHandlers(cocos2d::CCArray*);
	void removeAllDelegates();
	void removeDelegate(cocos2d::CCTouchDelegate*);
	void setDispatchEvents(bool);
	void setPriority(int, cocos2d::CCTouchDelegate*);
	void touches(cocos2d::CCSet*, cocos2d::CCEvent*, unsigned int);
	void touchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*);
	void touchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*);
	void touchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*);
	void touchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*);
	virtual void touchesBegan(cocos2d::CCSet *,cocos2d::CCEvent *);
	virtual void touchesCancelled(cocos2d::CCSet *,cocos2d::CCEvent *);
	virtual void touchesEnded(cocos2d::CCSet *,cocos2d::CCEvent *);
	virtual void touchesMoved(cocos2d::CCSet *,cocos2d::CCEvent *);
}
class cocos2d::CCTouchHandler : cocos2d::CCObject {
	void getDelegate();
	void getEnabledSelectors();
	void getPriority();
	void handlerWithDelegate(cocos2d::CCTouchDelegate*, int);
	void initWithDelegate(cocos2d::CCTouchDelegate*, int);
	void setDelegate(cocos2d::CCTouchDelegate*);
	void setEnalbedSelectors(int);
	void setPriority(int);
	virtual void initWithDelegate(cocos2d::CCTouchDelegate *,int);
}
class cocos2d::CCTouchScriptHandlerEntry : cocos2d::CCScriptHandlerEntry {
	void create(int, bool, int, bool);
	void init(bool, int, bool);
}
class cocos2d::CCTransitionCrossFade : cocos2d::CCTransitionScene {
	void CCTransitionCrossFade();
	void create(float, cocos2d::CCScene*);
	void draw();
	void onEnter();
	void onExit();
}
class cocos2d::CCTransitionEaseScene {
}
class cocos2d::CCTransitionFade : cocos2d::CCTransitionScene {
	void CCTransitionFade();
	void create(float, cocos2d::CCScene*);
	void create(float, cocos2d::CCScene*, cocos2d::_ccColor3B const&);
	void initWithDuration(float, cocos2d::CCScene*);
	void initWithDuration(float, cocos2d::CCScene*, cocos2d::_ccColor3B const&);
	void onEnter();
	void onExit();
	virtual void initWithDuration(float,cocos2d::CCScene *);
	virtual void initWithDuration(float,cocos2d::CCScene *,cocos2d::_ccColor3B const&);
}
class cocos2d::CCTransitionFadeBL : cocos2d::CCTransitionFadeTR {
	void CCTransitionFadeBL();
	void actionWithSize(cocos2d::CCSize const&);
	void create(float, cocos2d::CCScene*);
	virtual void actionWithSize(cocos2d::CCSize const&);
}
class cocos2d::CCTransitionFadeDown : cocos2d::CCTransitionFadeTR {
	void CCTransitionFadeDown();
	void actionWithSize(cocos2d::CCSize const&);
	void create(float, cocos2d::CCScene*);
	virtual void actionWithSize(cocos2d::CCSize const&);
}
class cocos2d::CCTransitionFadeTR : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	void CCTransitionFadeTR();
	void actionWithSize(cocos2d::CCSize const&);
	void create(float, cocos2d::CCScene*);
	void easeActionWithAction(cocos2d::CCActionInterval*);
	void onEnter();
	void sceneOrder();
	virtual void actionWithSize(cocos2d::CCSize const&);
	virtual void easeActionWithAction(cocos2d::CCActionInterval *);
}
class cocos2d::CCTransitionFadeUp : cocos2d::CCTransitionFadeTR {
	void CCTransitionFadeUp();
	void actionWithSize(cocos2d::CCSize const&);
	void create(float, cocos2d::CCScene*);
	virtual void actionWithSize(cocos2d::CCSize const&);
}
class cocos2d::CCTransitionFlipAngular : cocos2d::CCTransitionSceneOriented {
	void CCTransitionFlipAngular();
	void create(float, cocos2d::CCScene*);
	void create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	void onEnter();
}
class cocos2d::CCTransitionFlipX : cocos2d::CCTransitionSceneOriented {
	void CCTransitionFlipX();
	void create(float, cocos2d::CCScene*);
	void create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	void onEnter();
}
class cocos2d::CCTransitionFlipY : cocos2d::CCTransitionSceneOriented {
	void CCTransitionFlipY();
	void create(float, cocos2d::CCScene*);
	void create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	void onEnter();
}
class cocos2d::CCTransitionJumpZoom : cocos2d::CCTransitionScene {
	void CCTransitionJumpZoom();
	void create(float, cocos2d::CCScene*);
	void onEnter();
}
class cocos2d::CCTransitionMoveInB : cocos2d::CCTransitionMoveInL {
	void CCTransitionMoveInB();
	void create(float, cocos2d::CCScene*);
	void initScenes();
}
class cocos2d::CCTransitionMoveInL : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	void CCTransitionMoveInL();
	void action();
	void create(float, cocos2d::CCScene*);
	void easeActionWithAction(cocos2d::CCActionInterval*);
	void initScenes();
	void onEnter();
	virtual void easeActionWithAction(cocos2d::CCActionInterval *);
}
class cocos2d::CCTransitionMoveInR : cocos2d::CCTransitionMoveInL {
	void CCTransitionMoveInR();
	void create(float, cocos2d::CCScene*);
	void initScenes();
}
class cocos2d::CCTransitionMoveInT : cocos2d::CCTransitionMoveInL {
	void CCTransitionMoveInT();
	void create(float, cocos2d::CCScene*);
	void initScenes();
}
class cocos2d::CCTransitionPageTurn : cocos2d::CCTransitionScene {
	void CCTransitionPageTurn();
	void actionWithSize(cocos2d::CCSize const&);
	void create(float, cocos2d::CCScene*, bool);
	void initWithDuration(float, cocos2d::CCScene*, bool);
	void onEnter();
	void sceneOrder();
	virtual void initWithDuration(float,cocos2d::CCScene *,bool);
}
class cocos2d::CCTransitionProgress : cocos2d::CCTransitionScene {
	void CCTransitionProgress();
	void create(float, cocos2d::CCScene*);
	void onEnter();
	void onExit();
	void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
	void sceneOrder();
	void setupTransition();
	virtual void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture *);
}
class cocos2d::CCTransitionProgressHorizontal : cocos2d::CCTransitionProgress {
	void create(float, cocos2d::CCScene*);
	void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
	virtual void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture *);
}
class cocos2d::CCTransitionProgressInOut : cocos2d::CCTransitionProgress {
	void create(float, cocos2d::CCScene*);
	void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
	void sceneOrder();
	void setupTransition();
	virtual void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture *);
}
class cocos2d::CCTransitionProgressOutIn : cocos2d::CCTransitionProgress {
	void create(float, cocos2d::CCScene*);
	void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
	virtual void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture *);
}
class cocos2d::CCTransitionProgressRadialCCW : cocos2d::CCTransitionProgress {
	void create(float, cocos2d::CCScene*);
	void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
	virtual void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture *);
}
class cocos2d::CCTransitionProgressRadialCW : cocos2d::CCTransitionProgress {
	void create(float, cocos2d::CCScene*);
	void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
	virtual void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture *);
}
class cocos2d::CCTransitionProgressVertical : cocos2d::CCTransitionProgress {
	void create(float, cocos2d::CCScene*);
	void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
	virtual void progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture *);
}
class cocos2d::CCTransitionRotoZoom : cocos2d::CCTransitionScene {
	void CCTransitionRotoZoom();
	void create(float, cocos2d::CCScene*);
	void onEnter();
}
class cocos2d::CCTransitionScene : cocos2d::CCScene {
	void CCTransitionScene();
	void cleanup();
	void create(float, cocos2d::CCScene*);
	void draw();
	void finish();
	void hideOutShowIn();
	void initWithDuration(float, cocos2d::CCScene*);
	void onEnter();
	void onExit();
	void sceneOrder();
	void setNewScene(float);
	virtual void initWithDuration(float,cocos2d::CCScene *);
}
class cocos2d::CCTransitionSceneOriented : cocos2d::CCTransitionScene {
	void CCTransitionSceneOriented();
	void create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	void initWithDuration(float, cocos2d::CCScene*, cocos2d::tOrientation);
	virtual void initWithDuration(float,cocos2d::CCScene *,cocos2d::tOrientation);
}
class cocos2d::CCTransitionShrinkGrow : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	void CCTransitionShrinkGrow();
	void create(float, cocos2d::CCScene*);
	void easeActionWithAction(cocos2d::CCActionInterval*);
	void onEnter();
	virtual void easeActionWithAction(cocos2d::CCActionInterval *);
}
class cocos2d::CCTransitionSlideInB : cocos2d::CCTransitionSlideInL {
	void CCTransitionSlideInB();
	void action();
	void create(float, cocos2d::CCScene*);
	void initScenes();
	void sceneOrder();
}
class cocos2d::CCTransitionSlideInL : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	void CCTransitionSlideInL();
	void action();
	void create(float, cocos2d::CCScene*);
	void easeActionWithAction(cocos2d::CCActionInterval*);
	void initScenes();
	void onEnter();
	void sceneOrder();
	virtual void easeActionWithAction(cocos2d::CCActionInterval *);
}
class cocos2d::CCTransitionSlideInR : cocos2d::CCTransitionSlideInL {
	void CCTransitionSlideInR();
	void action();
	void create(float, cocos2d::CCScene*);
	void initScenes();
	void sceneOrder();
}
class cocos2d::CCTransitionSlideInT : cocos2d::CCTransitionSlideInL {
	void CCTransitionSlideInT();
	void action();
	void create(float, cocos2d::CCScene*);
	void initScenes();
	void sceneOrder();
}
class cocos2d::CCTransitionSplitCols : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	void CCTransitionSplitCols();
	void action();
	void create(float, cocos2d::CCScene*);
	void easeActionWithAction(cocos2d::CCActionInterval*);
	void onEnter();
	virtual void easeActionWithAction(cocos2d::CCActionInterval *);
}
class cocos2d::CCTransitionSplitRows : cocos2d::CCTransitionSplitCols {
	void CCTransitionSplitRows();
	void action();
	void create(float, cocos2d::CCScene*);
}
class cocos2d::CCTransitionTurnOffTiles : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	void CCTransitionTurnOffTiles();
	void create(float, cocos2d::CCScene*);
	void easeActionWithAction(cocos2d::CCActionInterval*);
	void onEnter();
	void sceneOrder();
	virtual void easeActionWithAction(cocos2d::CCActionInterval *);
}
class cocos2d::CCTransitionZoomFlipAngular : cocos2d::CCTransitionSceneOriented {
	void CCTransitionZoomFlipAngular();
	void create(float, cocos2d::CCScene*);
	void create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	void onEnter();
}
class cocos2d::CCTransitionZoomFlipX : cocos2d::CCTransitionSceneOriented {
	void CCTransitionZoomFlipX();
	void create(float, cocos2d::CCScene*);
	void create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	void onEnter();
}
class cocos2d::CCTransitionZoomFlipY : cocos2d::CCTransitionSceneOriented {
	void CCTransitionZoomFlipY();
	void create(float, cocos2d::CCScene*);
	void create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	void onEnter();
}
class cocos2d::CCTurnOffTiles : cocos2d::CCTiledGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&);
	void create(float, cocos2d::CCSize const&, unsigned int);
	void initWithDuration(float, cocos2d::CCSize const&, unsigned int);
	void shuffle(unsigned int*, unsigned int);
	void startWithTarget(cocos2d::CCNode*);
	void turnOffTile(cocos2d::CCPoint const&);
	void turnOnTile(cocos2d::CCPoint const&);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float,cocos2d::CCSize const&,uint);
	virtual void startWithTarget(cocos2d::CCNode *);
	virtual void update(float);
}
class cocos2d::CCTwirl : cocos2d::CCGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, cocos2d::CCPoint, unsigned int, float);
	void initWithDuration(float, cocos2d::CCSize const&, cocos2d::CCPoint, unsigned int, float);
	void setPosition(cocos2d::CCPoint const&);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCWaves : cocos2d::CCGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, unsigned int, float, bool, bool);
	void initWithDuration(float, cocos2d::CCSize const&, unsigned int, float, bool, bool);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCWaves3D : cocos2d::CCGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, unsigned int, float);
	void initWithDuration(float, cocos2d::CCSize const&, unsigned int, float);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void update(float);
}
class cocos2d::CCWavesTiles3D : cocos2d::CCTiledGrid3DAction {
	void copyWithZone(cocos2d::CCZone*);
	void create(float, cocos2d::CCSize const&, unsigned int, float);
	void initWithDuration(float, cocos2d::CCSize const&, unsigned int, float);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void initWithDuration(float,cocos2d::CCSize const&,uint,float);
	virtual void update(float);
}
class cocos2d::EGLTouchDelegate {
}
class cocos2d::extension::AssetsManager {
	void AssetsManager(char const*, char const*, char const*);
	void Helper::Helper();
	void Helper::handleUpdateSucceed(cocos2d::extension::AssetsManager::_Message*);
	void Helper::sendMessage(cocos2d::extension::AssetsManager::_Message*);
	void Helper::update(float);
	void Helper::~Helper();
	void checkStoragePath();
	void checkUpdate();
	void createDirectory(char const*);
	void deleteVersion();
	void downLoad();
	void getConnectionTimeout();
	void getPackageUrl();
	void getStoragePath();
	void getVersion();
	void getVersionFileUrl();
	void sendErrorMessage(cocos2d::extension::AssetsManager::ErrorCode);
	void setConnectionTimeout(unsigned int);
	void setDelegate(cocos2d::extension::AssetsManagerDelegateProtocol*);
	void setPackageUrl(char const*);
	void setSearchPath();
	void setStoragePath(char const*);
	void setVersionFileUrl(char const*);
	void uncompress();
	void update();
}
class cocos2d::extension::AssetsManager::Helper : cocos2d::CCObject {
	void Helper();
	void handleUpdateSucceed(cocos2d::extension::AssetsManager::_Message*);
	void sendMessage(cocos2d::extension::AssetsManager::_Message*);
	void update(float);
	virtual void update(float);
}
class cocos2d::extension::CCArrayForObjectSorting : cocos2d::CCArray {
	void indexOfSortedObject(cocos2d::extension::CCSortableObject*);
	void insertSortedObject(cocos2d::extension::CCSortableObject*);
	void objectWithObjectID(unsigned int);
	void removeSortedObject(cocos2d::extension::CCSortableObject*);
	void setObjectID_ofSortedObject(unsigned int, cocos2d::extension::CCSortableObject*);
}
class cocos2d::extension::CCColor3bObject : cocos2d::CCObject {
}
class cocos2d::extension::CCControl : cocos2d::CCLayerRGBA {
	void CCControl();
	void addHandleOfControlEvent(int, unsigned int);
	void addTargetWithActionForControlEvent(cocos2d::CCObject*, cocos2d::extension::SEL_EventHandler, unsigned int);
	void addTargetWithActionForControlEvents(cocos2d::CCObject*, cocos2d::extension::SEL_EventHandler, unsigned int);
	void create();
	void dispatchListforControlEvent(unsigned int);
	void getHandleOfControlEvent(unsigned int);
	void getTouchLocation(cocos2d::CCTouch*);
	void hasVisibleParents();
	void init();
	void isEnabled();
	void isHighlighted();
	void isOpacityModifyRGB();
	void isSelected();
	void isTouchInside(cocos2d::CCTouch*);
	void needsLayout();
	void onEnter();
	void onExit();
	void registerWithTouchDispatcher();
	void removeHandleOfControlEvent(unsigned int);
	void removeTargetWithActionForControlEvent(cocos2d::CCObject*, cocos2d::extension::SEL_EventHandler, unsigned int);
	void removeTargetWithActionForControlEvents(cocos2d::CCObject*, cocos2d::extension::SEL_EventHandler, unsigned int);
	void sendActionsForControlEvents(unsigned int);
	void setEnabled(bool);
	void setHighlighted(bool);
	void setOpacityModifyRGB(bool);
	void setSelected(bool);
	virtual void addTargetWithActionForControlEvents(cocos2d::CCObject *,void (cocos2d::CCObject::*)(cocos2d::CCObject *,uint),uint);
	virtual void getTouchLocation(cocos2d::CCTouch *);
	virtual void isTouchInside(cocos2d::CCTouch *);
	virtual void removeTargetWithActionForControlEvents(cocos2d::CCObject *,void (cocos2d::CCObject::*)(cocos2d::CCObject *,uint),uint);
	virtual void sendActionsForControlEvents(uint);
	virtual void setEnabled(bool);
	virtual void setHighlighted(bool);
	virtual void setOpacityModifyRGB(bool);
	virtual void setSelected(bool);
}
class cocos2d::extension::CCControlButton : cocos2d::extension::CCControl {
	void CCControlButton();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(cocos2d::extension::CCScale9Sprite*);
	void create(cocos2d::CCNode*, cocos2d::extension::CCScale9Sprite*);
	void create(gd::string, char const*, float);
	void create();
	void doesAdjustBackgroundImage();
	void getBackgroundSpriteForState(unsigned int);
	void getColor();
	void getLabelAnchorPoint();
	void getOpacity();
	void getPreferredSize();
	void getTitleBMFontForState(unsigned int);
	void getTitleColorForState(unsigned int);
	void getTitleForState(unsigned int);
	void getTitleLabelForState(unsigned int);
	void getTitleTTFForState(unsigned int);
	void getTitleTTFSizeForState(unsigned int);
	void getZoomOnTouchDown();
	void init();
	void initWithBackgroundSprite(cocos2d::extension::CCScale9Sprite*);
	void initWithLabelAndBackgroundSprite(cocos2d::CCNode*, cocos2d::extension::CCScale9Sprite*);
	void initWithTitleAndFontNameAndFontSize(gd::string, char const*, float);
	void needsLayout();
	void setAdjustBackgroundImage(bool);
	void setBackgroundSprite(cocos2d::extension::CCScale9Sprite*);
	void setBackgroundSpriteForState(cocos2d::extension::CCScale9Sprite*, unsigned int);
	void setBackgroundSpriteFrameForState(cocos2d::CCSpriteFrame*, unsigned int);
	void setColor(cocos2d::_ccColor3B const&);
	void setEnabled(bool);
	void setHighlighted(bool);
	void setLabelAnchorPoint(cocos2d::CCPoint);
	void setMargins(int, int);
	void setOpacity(unsigned char);
	void setPreferredSize(cocos2d::CCSize);
	void setSelected(bool);
	void setTitleBMFontForState(char const*, unsigned int);
	void setTitleColorForState(cocos2d::_ccColor3B, unsigned int);
	void setTitleForState(cocos2d::CCString*, unsigned int);
	void setTitleLabel(cocos2d::CCNode*);
	void setTitleLabelForState(cocos2d::CCNode*, unsigned int);
	void setTitleTTFForState(char const*, unsigned int);
	void setTitleTTFSizeForState(float, unsigned int);
	void setZoomOnTouchDown(bool);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void getBackgroundSpriteForState(uint);
	virtual void getTitleBMFontForState(uint);
	virtual void getTitleColorForState(uint);
	virtual void getTitleForState(uint);
	virtual void getTitleLabelForState(uint);
	virtual void getTitleTTFForState(uint);
	virtual void getTitleTTFSizeForState(uint);
	virtual void initWithBackgroundSprite(cocos2d::extension::CCScale9Sprite *);
	virtual void initWithLabelAndBackgroundSprite(cocos2d::CCNode *,cocos2d::extension::CCScale9Sprite *);
	virtual void initWithTitleAndFontNameAndFontSize(gd::string,char const*,float);
	virtual void setBackgroundSpriteForState(cocos2d::extension::CCScale9Sprite *,uint);
	virtual void setBackgroundSpriteFrameForState(cocos2d::CCSpriteFrame *,uint);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setEnabled(bool);
	virtual void setHighlighted(bool);
	virtual void setLabelAnchorPoint(cocos2d::CCPoint);
	virtual void setMargins(int,int);
	virtual void setOpacity(uchar);
	virtual void setPreferredSize(cocos2d::CCSize);
	virtual void setSelected(bool);
	virtual void setTitleBMFontForState(char const*,uint);
	virtual void setTitleColorForState(cocos2d::_ccColor3B,uint);
	virtual void setTitleForState(cocos2d::CCString *,uint);
	virtual void setTitleLabelForState(cocos2d::CCNode *,uint);
	virtual void setTitleTTFForState(char const*,uint);
	virtual void setTitleTTFSizeForState(float,uint);
	virtual void setZoomOnTouchDown(bool);
}
class cocos2d::extension::CCControlColourPicker : cocos2d::extension::CCControl {
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void colourPicker();
	void colourSliderValueChanged(cocos2d::CCObject*, unsigned int);
	void create();
	void hueSliderValueChanged(cocos2d::CCObject*, unsigned int);
	void init();
	void setColorValue(cocos2d::_ccColor3B const&);
	void updateControlPicker();
	void updateHueAndControlPicker();
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void setColorValue(cocos2d::_ccColor3B const&);
}
class cocos2d::extension::CCControlHuePicker : cocos2d::extension::CCControl {
	void CCControlHuePicker();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void checkSliderPosition(cocos2d::CCPoint, bool);
	void create(cocos2d::CCNode*, cocos2d::CCPoint);
	void initWithTargetAndPos(cocos2d::CCNode*, cocos2d::CCPoint);
	void registerWithTouchDispatcher();
	void setEnabled(bool);
	void setHue(float);
	void setHuePercentage(float);
	void updateSliderPosition(cocos2d::CCPoint);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void initWithTargetAndPos(cocos2d::CCNode *,cocos2d::CCPoint);
	virtual void setEnabled(bool);
	virtual void setHue(float);
	virtual void setHuePercentage(float);
}
class cocos2d::extension::CCControlPotentiometer : cocos2d::extension::CCControl {
	void CCControlPotentiometer();
	void angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::CCPoint);
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(char const*, char const*, char const*);
	void distanceBetweenPointAndPoint(cocos2d::CCPoint, cocos2d::CCPoint);
	void getMaximumValue();
	void getMinimumValue();
	void getValue();
	void initWithTrackSprite_ProgressTimer_ThumbSprite(cocos2d::CCSprite*, cocos2d::CCProgressTimer*, cocos2d::CCSprite*);
	void isTouchInside(cocos2d::CCTouch*);
	void potentiometerBegan(cocos2d::CCPoint);
	void potentiometerEnded(cocos2d::CCPoint);
	void potentiometerMoved(cocos2d::CCPoint);
	void setEnabled(bool);
	void setMaximumValue(float);
	void setMinimumValue(float);
	void setValue(float);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void isTouchInside(cocos2d::CCTouch *);
	virtual void setEnabled(bool);
}
class cocos2d::extension::CCControlSaturationBrightnessPicker : cocos2d::extension::CCControl {
	void CCControlSaturationBrightnessPicker();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void checkSliderPosition(cocos2d::CCPoint, bool);
	void create(cocos2d::CCNode*, cocos2d::CCPoint);
	void initWithTargetAndPos(cocos2d::CCNode*, cocos2d::CCPoint);
	void registerWithTouchDispatcher();
	void setEnabled(bool);
	void updateDraggerWithHSV(cocos2d::extension::HSV);
	void updateSliderPosition(cocos2d::CCPoint);
	void updateWithHSV(cocos2d::extension::HSV);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void initWithTargetAndPos(cocos2d::CCNode *,cocos2d::CCPoint);
	virtual void setEnabled(bool);
	virtual void updateDraggerWithHSV(cocos2d::extension::HSV);
	virtual void updateWithHSV(cocos2d::extension::HSV);
}
class cocos2d::extension::CCControlSlider : cocos2d::extension::CCControl {
	void CCControlSlider();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(char const*, char const*, char const*);
	void create(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*);
	void initWithSprites(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*);
	void isTouchInside(cocos2d::CCTouch*);
	void locationFromTouch(cocos2d::CCTouch*);
	void needsLayout();
	void setEnabled(bool);
	void setMaximumValue(float);
	void setMinimumValue(float);
	void setValue(float);
	void sliderBegan(cocos2d::CCPoint);
	void sliderEnded(cocos2d::CCPoint);
	void sliderMoved(cocos2d::CCPoint);
	void valueForLocation(cocos2d::CCPoint);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void initWithSprites(cocos2d::CCSprite *,cocos2d::CCSprite *,cocos2d::CCSprite *);
	virtual void isTouchInside(cocos2d::CCTouch *);
	virtual void setEnabled(bool);
	virtual void setMaximumValue(float);
	virtual void setMinimumValue(float);
	virtual void setValue(float);
}
class cocos2d::extension::CCControlStepper : cocos2d::extension::CCControl {
	void CCControlStepper();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(cocos2d::CCSprite*, cocos2d::CCSprite*);
	void getValue();
	void initWithMinusSpriteAndPlusSprite(cocos2d::CCSprite*, cocos2d::CCSprite*);
	void isContinuous();
	void setMaximumValue(double);
	void setMinimumValue(double);
	void setStepValue(double);
	void setValue(double);
	void setValueWithSendingEvent(double, bool);
	void setWraps(bool);
	void startAutorepeat();
	void stopAutorepeat();
	void update(float);
	void updateLayoutUsingTouchLocation(cocos2d::CCPoint);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void setMaximumValue(double);
	virtual void setMinimumValue(double);
	virtual void setStepValue(double);
	virtual void setValue(double);
	virtual void setValueWithSendingEvent(double,bool);
	virtual void setWraps(bool);
	virtual void update(float);
}
class cocos2d::extension::CCControlSwitch : cocos2d::extension::CCControl {
	void CCControlSwitch();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*);
	void create(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCLabelTTF*, cocos2d::CCLabelTTF*);
	void initWithMaskSprite(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*);
	void initWithMaskSprite(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCLabelTTF*, cocos2d::CCLabelTTF*);
	void locationFromTouch(cocos2d::CCTouch*);
	void setEnabled(bool);
	void setOn(bool);
	void setOn(bool, bool);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void setEnabled(bool);
}
class cocos2d::extension::CCControlSwitchSprite : cocos2d::CCSprite, cocos2d::CCActionTweenDelegate {
	void CCControlSwitchSprite();
	void draw();
	void initWithMaskSprite(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCLabelTTF*, cocos2d::CCLabelTTF*);
	void needsLayout();
	void offSideWidth();
	void onSideWidth();
	void setSliderXPosition(float);
	void updateTweenAction(float, char const*);
	virtual void updateTweenAction(float,char const*);
}
class cocos2d::extension::CCEditBox : cocos2d::extension::CCControlButton, cocos2d::CCIMEDelegate {
	void CCEditBox();
	void create(cocos2d::CCSize const&, cocos2d::extension::CCScale9Sprite*, cocos2d::extension::CCScale9Sprite*, cocos2d::extension::CCScale9Sprite*);
	void getDelegate();
	void getMaxLength();
	void getPlaceHolder();
	void getText();
	void initWithSizeAndBackgroundSprite(cocos2d::CCSize const&, cocos2d::extension::CCScale9Sprite*);
	void keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	void keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	void onEnter();
	void onExit();
	void registerScriptEditBoxHandler(int);
	void setAnchorPoint(cocos2d::CCPoint const&);
	void setContentSize(cocos2d::CCSize const&);
	void setDelegate(cocos2d::extension::CCEditBoxDelegate*);
	void setFont(char const*, int);
	void setFontColor(cocos2d::_ccColor3B const&);
	void setFontName(char const*);
	void setFontSize(int);
	void setInputFlag(cocos2d::extension::EditBoxInputFlag);
	void setInputMode(cocos2d::extension::EditBoxInputMode);
	void setMaxLength(int);
	void setPlaceHolder(char const*);
	void setPlaceholderFont(char const*, int);
	void setPlaceholderFontColor(cocos2d::_ccColor3B const&);
	void setPlaceholderFontName(char const*);
	void setPlaceholderFontSize(int);
	void setPosition(cocos2d::CCPoint const&);
	void setReturnType(cocos2d::extension::KeyboardReturnType);
	void setText(char const*);
	void setVisible(bool);
	void touchDownAction(cocos2d::CCObject*, unsigned int);
	void unregisterScriptEditBoxHandler();
	void visit();
	virtual void keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo &);
	virtual void keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo &);
	virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo &);
	virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo &);
	virtual void setAnchorPoint(cocos2d::CCPoint const&);
	virtual void setContentSize(cocos2d::CCSize const&);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void setVisible(bool);
}
class cocos2d::extension::CCEditBoxImpl {
}
class cocos2d::extension::CCEditBoxImplAndroid : cocos2d::extension::CCEditBoxImpl {
	void CCEditBoxImplAndroid(cocos2d::extension::CCEditBox*);
	void closeKeyboard();
	void doAnimationWhenKeyboardMove(float, float);
	void getMaxLength();
	void getText();
	void initWithSize(cocos2d::CCSize const&);
	void isEditing();
	void onEnter();
	void openKeyboard();
	void setAnchorPoint(cocos2d::CCPoint const&);
	void setContentSize(cocos2d::CCSize const&);
	void setFont(char const*, int);
	void setFontColor(cocos2d::_ccColor3B const&);
	void setInputFlag(cocos2d::extension::EditBoxInputFlag);
	void setInputMode(cocos2d::extension::EditBoxInputMode);
	void setMaxLength(int);
	void setPlaceHolder(char const*);
	void setPlaceholderFont(char const*, int);
	void setPlaceholderFontColor(cocos2d::_ccColor3B const&);
	void setPosition(cocos2d::CCPoint const&);
	void setReturnType(cocos2d::extension::KeyboardReturnType);
	void setText(char const*);
	void setVisible(bool);
	void visit();
	virtual void doAnimationWhenKeyboardMove(float,float);
	virtual void initWithSize(cocos2d::CCSize const&);
	virtual void setAnchorPoint(cocos2d::CCPoint const&);
	virtual void setContentSize(cocos2d::CCSize const&);
	virtual void setFont(char const*,int);
	virtual void setFontColor(cocos2d::_ccColor3B const&);
	virtual void setInputFlag(cocos2d::extension::EditBoxInputFlag);
	virtual void setInputMode(cocos2d::extension::EditBoxInputMode);
	virtual void setMaxLength(int);
	virtual void setPlaceHolder(char const*);
	virtual void setPlaceholderFont(char const*,int);
	virtual void setPlaceholderFontColor(cocos2d::_ccColor3B const&);
	virtual void setPosition(cocos2d::CCPoint const&);
	virtual void setReturnType(cocos2d::extension::KeyboardReturnType);
	virtual void setText(char const*);
	virtual void setVisible(bool);
}
class cocos2d::extension::CCHttpClient : cocos2d::CCObject {
	void CCHttpClient();
	void destroyInstance();
	void dispatchResponseCallbacks(float);
	void getInstance();
	void lazyInitThreadSemphore();
	void send(cocos2d::extension::CCHttpRequest*);
}
class cocos2d::extension::CCHttpRequest : cocos2d::CCObject {
	void CCHttpRequest();
}
class cocos2d::extension::CCHttpResponse : cocos2d::CCObject {
	void CCHttpResponse(cocos2d::extension::CCHttpRequest*);
}
class cocos2d::extension::CCInvocation : cocos2d::CCObject {
	void CCInvocation(cocos2d::CCObject*, cocos2d::extension::SEL_EventHandler, unsigned int);
	void create(cocos2d::CCObject*, cocos2d::extension::SEL_EventHandler, unsigned int);
	void invoke(cocos2d::CCObject*);
}
class cocos2d::extension::CCScale9Sprite : cocos2d::CCNodeRGBA {
	void CCScale9Sprite();
	void create(cocos2d::CCRect, char const*);
	void create(char const*);
	void create(char const*, cocos2d::CCRect);
	void create(char const*, cocos2d::CCRect, cocos2d::CCRect);
	void create();
	void createWithSpriteFrame(cocos2d::CCSpriteFrame*);
	void createWithSpriteFrame(cocos2d::CCSpriteFrame*, cocos2d::CCRect);
	void createWithSpriteFrameName(char const*);
	void createWithSpriteFrameName(char const*, cocos2d::CCRect);
	void getCapInsets();
	void getColor();
	void getInsetBottom();
	void getInsetLeft();
	void getInsetRight();
	void getInsetTop();
	void getOpacity();
	void getPreferredSize();
	void init();
	void initWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, bool, cocos2d::CCRect);
	void initWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, cocos2d::CCRect);
	void initWithFile(cocos2d::CCRect, char const*);
	void initWithFile(char const*);
	void initWithFile(char const*, cocos2d::CCRect);
	void initWithFile(char const*, cocos2d::CCRect, cocos2d::CCRect);
	void initWithSpriteFrame(cocos2d::CCSpriteFrame*);
	void initWithSpriteFrame(cocos2d::CCSpriteFrame*, cocos2d::CCRect);
	void initWithSpriteFrameName(char const*);
	void initWithSpriteFrameName(char const*, cocos2d::CCRect);
	void isOpacityModifyRGB();
	void resizableSpriteWithCapInsets(cocos2d::CCRect);
	void setBlendAdditive(bool);
	void setCapInsets(cocos2d::CCRect);
	void setColor(cocos2d::_ccColor3B const&);
	void setContentSize(cocos2d::CCSize const&);
	void setInsetBottom(float);
	void setInsetLeft(float);
	void setInsetRight(float);
	void setInsetTop(float);
	void setOpacity(unsigned char);
	void setOpacityModifyRGB(bool);
	void setPreferredSize(cocos2d::CCSize);
	void setSpriteFrame(cocos2d::CCSpriteFrame*);
	void updateCapInset();
	void updateDisplayedColor(cocos2d::_ccColor3B const&);
	void updateDisplayedOpacity(unsigned char);
	void updatePositions();
	void updateWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, bool, cocos2d::CCRect);
	void visit();
	virtual void initWithBatchNode(cocos2d::CCSpriteBatchNode *,cocos2d::CCRect,bool,cocos2d::CCRect);
	virtual void initWithBatchNode(cocos2d::CCSpriteBatchNode *,cocos2d::CCRect,cocos2d::CCRect);
	virtual void initWithFile(char const*,cocos2d::CCRect,cocos2d::CCRect);
	virtual void initWithFile(char const*,cocos2d::CCRect);
	virtual void initWithFile(cocos2d::CCRect,char const*);
	virtual void initWithFile(char const*);
	virtual void initWithSpriteFrame(cocos2d::CCSpriteFrame *,cocos2d::CCRect);
	virtual void initWithSpriteFrame(cocos2d::CCSpriteFrame *);
	virtual void initWithSpriteFrameName(char const*,cocos2d::CCRect);
	virtual void initWithSpriteFrameName(char const*);
	virtual void setCapInsets(cocos2d::CCRect);
	virtual void setColor(cocos2d::_ccColor3B const&);
	virtual void setContentSize(cocos2d::CCSize const&);
	virtual void setInsetBottom(float);
	virtual void setInsetLeft(float);
	virtual void setInsetRight(float);
	virtual void setInsetTop(float);
	virtual void setOpacity(uchar);
	virtual void setOpacityModifyRGB(bool);
	virtual void setPreferredSize(cocos2d::CCSize);
	virtual void setSpriteFrame(cocos2d::CCSpriteFrame *);
	virtual void updateDisplayedColor(cocos2d::_ccColor3B const&);
	virtual void updateDisplayedOpacity(uchar);
	virtual void updateWithBatchNode(cocos2d::CCSpriteBatchNode *,cocos2d::CCRect,bool,cocos2d::CCRect);
}
class cocos2d::extension::CCScrollView : cocos2d::CCLayer {
	void CCScrollView();
	void addChild(cocos2d::CCNode*);
	void addChild(cocos2d::CCNode*, int);
	void addChild(cocos2d::CCNode*, int, int);
	void afterDraw();
	void beforeDraw();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(cocos2d::CCSize, cocos2d::CCNode*);
	void create();
	void deaccelerateScrolling(float);
	void getContainer();
	void getContentOffset();
	void getContentSize();
	void getScriptHandler(int);
	void getViewRect();
	void getZoomScale();
	void init();
	void initWithViewSize(cocos2d::CCSize, cocos2d::CCNode*);
	void isNodeVisible(cocos2d::CCNode*);
	void maxContainerOffset();
	void minContainerOffset();
	void pause(cocos2d::CCObject*);
	void performedAnimatedScroll(float);
	void registerScriptHandler(int, int);
	void registerWithTouchDispatcher();
	void relocateContainer(bool);
	void resume(cocos2d::CCObject*);
	void setContainer(cocos2d::CCNode*);
	void setContentOffset(cocos2d::CCPoint, bool);
	void setContentOffsetInDuration(cocos2d::CCPoint, float);
	void setContentSize(cocos2d::CCSize const&);
	void setDirection(cocos2d::extension::CCScrollViewDirection);
	void setTouchEnabled(bool);
	void setViewSize(cocos2d::CCSize);
	void setZoomScale(float);
	void setZoomScale(float, bool);
	void setZoomScaleInDuration(float, float);
	void stoppedAnimatedScroll(cocos2d::CCNode*);
	void unregisterScriptHandler(int);
	void updateInset();
	void visit();
	virtual void addChild(cocos2d::CCNode *);
	virtual void addChild(cocos2d::CCNode *,int);
	virtual void addChild(cocos2d::CCNode *,int,int);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void setContentSize(cocos2d::CCSize const&);
	virtual void setDirection(cocos2d::extension::CCScrollViewDirection);
	virtual void setTouchEnabled(bool);
}
class cocos2d::extension::CCScrollViewDelegate {
}
class cocos2d::extension::CCSortableObject {
}
class cocos2d::extension::CCSortedObject : cocos2d::CCObject, cocos2d::extension::CCSortableObject {
	void getObjectID();
	void setObjectID(unsigned int);
	virtual void setObjectID(uint);
}
class cocos2d::extension::CCTableView : cocos2d::extension::CCScrollView, cocos2d::extension::CCScrollViewDelegate {
	void CCTableView();
	void __indexFromOffset(cocos2d::CCPoint);
	void __offsetFromIndex(unsigned int);
	void _addCellIfNecessary(cocos2d::extension::CCTableViewCell*);
	void _indexFromOffset(cocos2d::CCPoint);
	void _moveCellOutOfSight(cocos2d::extension::CCTableViewCell*);
	void _offsetFromIndex(unsigned int);
	void _setIndexForCell(unsigned int, cocos2d::extension::CCTableViewCell*);
	void _updateCellPositions();
	void _updateContentSize();
	void ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void cellAtIndex(unsigned int);
	void create(cocos2d::extension::CCTableViewDataSource*, cocos2d::CCSize);
	void create(cocos2d::extension::CCTableViewDataSource*, cocos2d::CCSize, cocos2d::CCNode*);
	void dequeueCell();
	void getVerticalFillOrder();
	void initWithViewSize(cocos2d::CCSize, cocos2d::CCNode*);
	void insertCellAtIndex(unsigned int);
	void reloadData();
	void removeCellAtIndex(unsigned int);
	void scrollViewDidScroll(cocos2d::extension::CCScrollView*);
	void scrollViewDidZoom(cocos2d::extension::CCScrollView*);
	void setVerticalFillOrder(cocos2d::extension::CCTableViewVerticalFillOrder);
	void unregisterAllScriptHandler();
	void updateCellAtIndex(unsigned int);
	virtual void ccTouchBegan(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchCancelled(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchEnded(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void ccTouchMoved(cocos2d::CCTouch *,cocos2d::CCEvent *);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView *);
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView *);
}
class cocos2d::extension::CCTableViewCell : cocos2d::CCNode, cocos2d::extension::CCSortableObject {
	void getIdx();
	void getObjectID();
	void reset();
	void setIdx(unsigned int);
	void setObjectID(unsigned int);
	virtual void setObjectID(uint);
}
class cocos2d::extension::ColorPickerDelegate {
	void colorValueChanged(cocos2d::_ccColor3B);
	virtual void colorValueChanged(cocos2d::_ccColor3B);
}
class cocos2d::extension::WebSocket {
	void WebSocket();
	void close();
	void getReadyState();
	void init(cocos2d::extension::WebSocket::Delegate const&, gd::string const&, gd::vector<gd::string, gd::allocator<gd::string > > const*);
	void onSocketCallback(lws*, lws_callback_reasons, void*, void*, unsigned int);
	void onSubThreadEnded();
	void onSubThreadLoop();
	void onSubThreadStarted();
	void onUIThreadReceiveMessage(cocos2d::extension::WsMessage*);
	void send(unsigned char const*, unsigned int);
	void send(gd::string const&);
	virtual void onUIThreadReceiveMessage(cocos2d::extension::WsMessage *);
}
class cocos2d::extension::WsThreadHelper : cocos2d::CCObject {
	void WsThreadHelper();
	void createThread(cocos2d::extension::WebSocket const&);
	void joinSubThread();
	void quitSubThread();
	void sendMessageToSubThread(cocos2d::extension::WsMessage*);
	void sendMessageToUIThread(cocos2d::extension::WsMessage*);
	void update(float);
	void wsThreadEntryFunc(void*);
	virtual void update(float);
}
class cocos2d::ExtraAction : cocos2d::CCFiniteTimeAction {
	void copyWithZone(cocos2d::CCZone*);
	void create();
	void reverse();
	void step(float);
	void update(float);
	virtual void copyWithZone(cocos2d::CCZone *);
	virtual void step(float);
	virtual void update(float);
}
class cocos2d::TypeInfo {
}
class cocos2d::XmlSaxHander : tinyxml2::XMLVisitor {
	void Visit(tinyxml2::XMLUnknown const&);
	void Visit(tinyxml2::XMLText const&);
	void VisitEnter(tinyxml2::XMLElement const&, tinyxml2::XMLAttribute const*);
	void VisitExit(tinyxml2::XMLElement const&);
	virtual void Visit(tinyxml2::XMLText const&);
	virtual void Visit(tinyxml2::XMLUnknown const&);
	virtual void VisitEnter(tinyxml2::XMLElement const&,tinyxml2::XMLAttribute const*);
	virtual void VisitExit(tinyxml2::XMLElement const&);
}
class cocos2d::ZipFile {
	void ZipFile(gd::string const&, gd::string const&);
	void fileExists(gd::string const&);
	void getFileData(gd::string const&, unsigned long*);
	void getFileData(gd::string const&, unsigned long*, cocos2d::ZipFilePrivate*);
	void setFilter(gd::string const&);
	void setFilter(gd::string const&, cocos2d::ZipFilePrivate*);
}
