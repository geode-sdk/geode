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
}
class AccountLoginLayer : FLAlertLayer, TextInputDelegate, GJAccountLoginDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(gd::string);
	void createTextBackground(cocos2d::CCPoint, char const*, cocos2d::CCSize);
	void createTextInput(cocos2d::CCPoint, cocos2d::CCSize, char const*, int);
	void createTextLabel(cocos2d::CCPoint, char const*, cocos2d::CCSize);
	void disableNodes();
	void hideLoadingUI();
	bool init(gd::string);
	void keyBackClicked();
	bool loginAccountFailed(AccountError);
	bool loginAccountFinished(int, int);
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
}
class AccountRegisterLayer : FLAlertLayer, TextInputDelegate, GJAccountRegisterDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	bool allowTextInput(CCTextInputNode*);
	void create();
	void createTextBackground(cocos2d::CCPoint, cocos2d::CCSize);
	void createTextInput(cocos2d::CCPoint, cocos2d::CCSize, gd::string, int);
	void createTextLabel(cocos2d::CCPoint, gd::string, cocos2d::CCSize);
	void disableNodes();
	void hideLoadingUI();
	bool init();
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
}
class AchievementBar : cocos2d::CCNodeRGBA {
	void create(char const*, char const*, char const*, bool);
	bool init(char const*, char const*, char const*, bool);
	void setOpacity(unsigned char);
	void show();
}
class AchievementCell : TableViewCell {
	void draw();
	bool init();
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
	bool init();
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
	bool init();
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
}
class AnimatedGameObject : GameObject, AnimatedSpriteDelegate, SpritePartDelegate {
	void activateObject();
	void animationFinished(char const*);
	void animationForID(int, int);
	void create(int);
	void deactivateObject(bool);
	cocos2d::CCSpriteFrame* displayFrameChanged(cocos2d::CCObject*, gd::string);
	void getTweenTime(int, int);
	bool init(int);
	void playAnimation(int);
	void resetObject();
	void setObjectColor(cocos2d::ccColor3B const&);
	void setOpacity(unsigned char);
	void setupAnimatedSize(int);
	void setupChildSprites();
	void updateChildSpriteColor(cocos2d::ccColor3B);
}
class AnimatedShopKeeper : CCAnimatedSprite {
	void animationFinished(char const*);
	void create(ShopType);
	bool init(ShopType);
	void startAnimating();
}
class AnimatedSpriteDelegate {
	void animationFinished(char const*);
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
}
class ArtistCell : TableViewCell {
	void draw();
	bool init();
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
	bool init(gd::string);
	void resetAudioVars();
	void triggerEffect(float);
	void updateTweenAction(float, char const*);
}
class BitmapFontCache : cocos2d::CCObject {
	void fontWithConfigFile(char const*, float);
	bool init();
	void purgeSharedFontCache();
	void sharedFontCache();
}
class BonusDropdown : cocos2d::CCNode {
	void create(gd::string, int);
	bool init(gd::string, int);
	void show();
}
class BoomListLayer : cocos2d::CCLayerColor {
	void create(BoomListView*, char const*);
	bool init(BoomListView*, char const*);
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
	bool init(cocos2d::CCArray*, float, float, int, BoomListType);
	void loadCell(TableViewCell*, int);
	void numberOfRowsInSection(unsigned int, TableView*);
	void numberOfSectionsInTableView(TableView*);
	void setupList();
}
class BoomScrollLayer : cocos2d::CCLayer {
	void addPage(cocos2d::CCLayer*);
	void addPage(cocos2d::CCLayer*, int);
	void cancelAndStoleTouch(cocos2d::CCTouch*, cocos2d::CCEvent*);
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
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
	bool init(cocos2d::CCArray*, int, bool, cocos2d::CCArray*, DynamicScrollDelegate*);
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
}
class BoomScrollLayerDelegate {
	void scrollLayerMoved(cocos2d::CCPoint);
	void scrollLayerScrolledToPage(BoomScrollLayer*, int);
	void scrollLayerScrollingStarted(BoomScrollLayer*);
	void scrollLayerWillScrollToPage(BoomScrollLayer*, int);
}
class ButtonPage : cocos2d::CCLayer {
	void create(cocos2d::CCArray*, cocos2d::CCPoint, int, float);
	bool init(cocos2d::CCArray*, cocos2d::CCPoint, int, float);
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
	bool init(char const*, int, int, float, bool, char const*, char const*, float);
	bool init(cocos2d::CCSprite*, int, int, float, float, bool, char const*, bool);
	void setColor(cocos2d::ccColor3B);
	void setString(char const*);
	void updateBGImage(char const*);
	void updateSpriteBGSize();
	void updateSpriteOffset(cocos2d::CCPoint);
}
class CCAlertCircle : cocos2d::CCNode {
	void create();
	void draw();
	bool init();
}
class CCAnimatedSprite : cocos2d::CCSprite {
	void animationFinished(char const*);
	void animationFinishedO(cocos2d::CCObject*);
	void cleanupSprite();
	void createWithType(char const*);
	bool initWithType(char const*);
	void loadType(char const*);
	void runAnimation(gd::string);
	void runAnimationForced(gd::string);
	void setColor(cocos2d::ccColor3B);
	void setOpacity(unsigned char);
	void stopTween();
	void switchToMode(spriteMode);
	void tweenToAnimation(gd::string, float);
	void tweenToAnimationFinished();
	void willPlayAnimation();
}
class CCAnimateFrameCache : cocos2d::CCObject {
	void addCustomSpriteFramesWithFile(char const*);
	void addDict(DS_Dictionary*, char const*);
	void addDict(cocos2d::CCDictionary*, char const*);
	void addSpriteFramesWithFile(char const*);
	bool init();
	void purgeSharedSpriteFrameCache();
	void removeSpriteFrames();
	void sharedSpriteFrameCache();
	void spriteFrameByName(char const*);
}
class CCBlockLayer : cocos2d::CCLayerColor {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
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
	bool init();
	void keyBackClicked();
	void layerHidden();
	void layerVisible();
	void registerWithTouchDispatcher();
	void showLayer(bool);
}
class CCCircleAlert : CCCircleWave {
	void create(float, float, float);
	bool init(float, float, float);
}
class CCCircleWave : cocos2d::CCNode {
	void baseSetup(float);
	void create(float, float, float, bool);
	void create(float, float, float, bool, bool);
	void draw();
	void followObject(cocos2d::CCNode*, bool);
	bool init(float, float, float, bool, bool);
	void removeMeAndCleanup();
	void setPosition(cocos2d::CCPoint const&);
	void updatePosition(float);
	void updateTweenAction(float, char const*);
}
class CCCircleWaveDelegate {
	void circleWaveWillBeRemoved(CCCircleWave*);
}
class CCContentLayer : cocos2d::CCLayerColor {
	void create(cocos2d::ccColor4B const&, float, float);
	void setPosition(cocos2d::CCPoint const&);
}
class CCCountdown : cocos2d::CCSprite {
	void create();
	bool init();
	void lapFinished();
	void setOpacity(unsigned char);
	void startTimerWithSeconds(float, cocos2d::SEL_CallFunc, cocos2d::CCNode*);
}
class CCCounterLabel : cocos2d::CCLabelBMFont {
	void calculateStepSize();
	void create(int, char const*, FormatterType);
	void disableCounter();
	void enableCounter();
	void fastUpdateCounter();
	void getTargetCount();
	bool init(int, char const*, FormatterType);
	void setTargetCount(int);
	void setupFormatter(FormatterType);
	void updateCounter(float);
	void updateString();
}
class CCIndexPath : cocos2d::CCObject {
	void CCIndexPathWithSectionRow(int, int);
}
class CCLightFlash : cocos2d::CCNode {
	void cleanupFlash();
	void create();
	void fadeAndRemove();
	bool init();
	void playEffect(cocos2d::CCPoint, cocos2d::ccColor3B, float, float, float, float, float, float, float, float, float, float, float, float, float, float, int, bool, bool, float);
	void removeLights();
	void showFlash();
}
class CCLightStrip : cocos2d::CCNode {
	void create(float, float, float, float, float);
	void draw();
	bool init(float, float, float, float, float);
	void updateTweenAction(float, char const*);
}
class CCMenuItemSpriteExtra : cocos2d::CCMenuItemSprite {
	void activate();
	void create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	bool init(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void selected();
	void setSizeMult(float);
	void unselected();
	void useAnimationType(MenuAnimationType);
}
class CCMenuItemToggler : cocos2d::CCMenuItem {
	void activate();
	void activeItem();
	void create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	bool init(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	void normalTouch(cocos2d::CCObject*);
	void selected();
	void selectedTouch(cocos2d::CCObject*);
	void setEnabled(bool);
	void setSizeMult(float);
	void toggle(bool);
	void unselected();
}
class CCMoveCNode : cocos2d::CCNode {
	void create();
	bool init();
}
class CCNodeContainer : cocos2d::CCNode {
	void create();
	bool init();
	void visit();
}
class CCPartAnimSprite : cocos2d::CCSprite {
	void changeTextureOfID(char const*, char const*);
	void countParts();
	void createWithAnimDesc(char const*, cocos2d::CCTexture2D*);
	void dirtify();
	cocos2d::CCSpriteFrame* displayFrame();
	void getSpriteForKey(char const*);
	bool initWithAnimDesc(char const*, cocos2d::CCTexture2D*);
	bool isFrameDisplayed(cocos2d::CCSpriteFrame*);
	void setBlendFunc(cocos2d::_ccBlendFunc);
	void setColor(cocos2d::ccColor3B);
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
}
class CCScrollLayerExt : cocos2d::CCLayer {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
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
}
class CCScrollLayerExtDelegate {
	void scrllViewWillBeginDecelerating(CCScrollLayerExt*);
	void scrollViewDidEndDecelerating(CCScrollLayerExt*);
	void scrollViewDidEndMoving(CCScrollLayerExt*);
	void scrollViewTouchBegin(CCScrollLayerExt*);
	void scrollViewTouchEnd(CCScrollLayerExt*);
	void scrollViewTouchMoving(CCScrollLayerExt*);
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
	bool initWithSpriteFrameName(char const*);
	bool initWithTexture(cocos2d::CCTexture2D*);
	void removeFollower(cocos2d::CCNode*);
	void setFlipX(bool);
	void setFlipY(bool);
	void setPosition(cocos2d::CCPoint const&);
	void setRotation(float);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void stopFollow();
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
	bool initShader();
	bool initWithSpriteFrame(cocos2d::CCSpriteFrame*);
	bool initWithTexture(cocos2d::CCTexture2D*);
	bool initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	bool initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool);
	void setHue(float);
	void setHueDegrees(float);
	void setupDefaultSettings();
	void shaderBody();
	void updateColor();
	void updateColorMatrix();
	void updateHue(float);
}
class CCTextInputNode : cocos2d::CCLayer, cocos2d::CCIMEDelegate, cocos2d::CCTextFieldDelegate {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(float, float, char const*, char const*, int, char const*);
	void forceOffset();
	void getString();
	bool init(float, float, char const*, char const*, int, char const*);
	void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	void onClickTrackNode(bool);
	void onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF*);
	void onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF*);
	void onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int);
	void refreshLabel();
	void registerWithTouchDispatcher();
	void setAllowedChars(gd::string);
	void setLabelNormalColor(cocos2d::ccColor3B);
	void setLabelPlaceholderColor(cocos2d::ccColor3B);
	void setLabelPlaceholderScale(float);
	void setMaxLabelScale(float);
	void setMaxLabelWidth(float);
	void setString(gd::string);
	void textChanged();
	void updateBlinkLabel();
	void updateLabel(gd::string);
	void visit();
}
class ChallengeNode : cocos2d::CCNode {
	void create(GJChallengeItem*, ChallengesPage*, bool);
	bool init(GJChallengeItem*, ChallengesPage*, bool);
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
	bool init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void show();
	void tryGetChallenges();
	void updateDots();
	void updateTimers(float);
}
class CheckpointObject : cocos2d::CCNode {
	void create();
	void getColorAction(ColorAction*, cocos2d::ccColor3B);
	void getObject();
	bool init();
	void setObject(GameObject*);
}
class CollisionBlockPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class CollisionTriggerAction : cocos2d::CCNode {
	void create(int, int, int, bool, bool);
	void createFromString(gd::string);
	void getSaveString();
	bool init(int, int, int, bool, bool);
}
class ColorAction : cocos2d::CCNode {
	void create(cocos2d::ccColor3B, bool, int);
	void create(cocos2d::ccColor3B, cocos2d::ccColor3B, float, double, bool);
	void create(cocos2d::ccColor3B, cocos2d::ccColor3B, float, double, bool, int, float, float);
	void create();
	void getCopy();
	void getSaveString();
	bool init(cocos2d::ccColor3B, cocos2d::ccColor3B, float, double, bool, int, float, float);
	void resetAction();
	void setupFromDict(cocos2d::CCDictionary*);
	void setupFromString(gd::string);
	void step(float);
	void updateCustomColor(cocos2d::ccColor3B, cocos2d::ccColor3B);
}
class ColorActionSprite : cocos2d::CCNode {
	void create();
	bool init();
}
class ColorChannelSprite : cocos2d::CCSprite {
	void create();
	bool init();
	void updateBlending(bool);
	void updateCopyLabel(int, bool);
	void updateOpacity(float);
	void updateValues(ColorAction*);
}
class ColoredSection : cocos2d::CCObject {
	void create(cocos2d::ccColor3B, int, int);
	bool init(cocos2d::ccColor3B, int, int);
}
class ColorSelectDelegate {
	void colorSelectClosed(cocos2d::CCNode*);
}
class ColorSelectLiveOverlay : FLAlertLayer {
	void closeColorSelect(cocos2d::CCObject*);
	void colorValueChanged(cocos2d::ccColor3B);
	void create(ColorAction*, ColorAction*, EffectGameObject*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void createWithActions(ColorAction*, ColorAction*);
	void createWithObject(EffectGameObject*);
	void determineStartValues();
	void getColorValue();
	bool init(ColorAction*, ColorAction*, EffectGameObject*);
	void keyBackClicked();
	void onSelectTab(cocos2d::CCObject*);
	void selectColor(cocos2d::ccColor3B);
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
	void colorValueChanged(cocos2d::ccColor3B);
	void create(ColorAction*);
	void create(EffectGameObject*, cocos2d::CCArray*);
	void create(EffectGameObject*, cocos2d::CCArray*, ColorAction*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	void getColorValue();
	bool init(EffectGameObject*, cocos2d::CCArray*, ColorAction*);
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
	void selectColor(cocos2d::ccColor3B);
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
}
class ColorSetupDelegate {
	void colorSetupClosed(int);
}
class CommentCell : TableViewCell, LikeItemDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void draw();
	void incrementDislikes();
	void incrementLikes();
	bool init();
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
}
class CommentUploadDelegate {
	void commentDeleteFailed(int, int);
	void commentUploadFailed(int, CommentError);
	void commentUploadFinished(int);
}
class CommunityCreditNode : cocos2d::CCNode {
	void create(int, int, int, gd::string);
	bool init(int, int, int, gd::string);
}
class CommunityCreditsPage : FLAlertLayer {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void goToPage(int);
	bool init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSwitchPage(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void show();
}
class ConfigureHSVWidget : cocos2d::CCNode {
	void create(cocos2d::ccHSVValue, bool);
	bool init(cocos2d::ccHSVValue, bool);
	void onResetHSV(cocos2d::CCObject*);
	void onToggleSConst(cocos2d::CCObject*);
	void onToggleVConst(cocos2d::CCObject*);
	void sliderChanged(cocos2d::CCObject*);
	void updateLabels();
}
class ConfigureValuePopup : FLAlertLayer, TextInputDelegate {
	void create(ConfigureValuePopupDelegate*, float, float, float, gd::string, gd::string);
	bool init(ConfigureValuePopupDelegate*, float, float, float, gd::string, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void sliderChanged(cocos2d::CCObject*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void updateTextInputLabel();
}
class ConfigureValuePopupDelegate {
	void valuePopupClosed(ConfigureValuePopup*, float);
}
class CountTriggerAction : cocos2d::CCNode {
	void create(int, int, bool, bool);
	void createFromString(gd::string);
	void getSaveString();
	bool init(int, int, bool, bool);
}
class CreateGuidelinesLayer : FLAlertLayer, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(LevelSettingsObject*);
	void doClearGuidelines();
	void getMergedRecordString(gd::string, gd::string);
	bool init(LevelSettingsObject*);
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
}
class CreateMenuItem : CCMenuItemSpriteExtra {
	void create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	bool init(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
}
class CreatorLayer : cocos2d::CCLayer, cocos2d::CCSceneTransitionDelegate, DialogDelegate {
	void canPlayOnlineLevels();
	void create();
	void dialogClosed(DialogLayer*);
	bool init();
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
}
class CurrencyRewardDelegate {
	void currencyWillExit(CurrencyRewardLayer*);
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
	bool init(int, int, int, CurrencySpriteType, int, CurrencySpriteType, int, cocos2d::CCPoint, CurrencyRewardType, float);
	void pulseSprite(cocos2d::CCSprite*);
	void update(float);
}
class CurrencySprite : CCSpritePlus {
	void create(CurrencySpriteType, bool);
	void createWithSprite(cocos2d::CCSprite*);
	bool init(CurrencySpriteType, bool);
	bool initWithSprite(cocos2d::CCSprite*);
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
	void hsvPopupClosed(HSVWidgetPopup*, cocos2d::ccHSVValue);
	bool init(GameObject*, cocos2d::CCArray*);
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
}
class CustomListView : BoomListView {
	void create(cocos2d::CCArray*, float, float, int, BoomListType);
	void getListCell(char const*);
	void loadCell(TableViewCell*, int);
	void reloadAll();
	void setupList();
}
class CustomSongCell : TableViewCell {
	void draw();
	bool init();
	void loadFromObject(SongInfoObject*);
	void onDelete(cocos2d::CCObject*);
	void updateBGColor(int);
}
class CustomSongLayer : FLAlertLayer, FLAlertLayerProtocol, TextInputDelegate, GJDropDownLayerDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(LevelSettingsObject*);
	void dropDownLayerWillClose(GJDropDownLayer*);
	bool init(LevelSettingsObject*);
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
	bool init(SongInfoObject*, LevelSettingsObject*, bool, bool, bool, bool, bool);
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
}
class DailyLevelNode : cocos2d::CCNode, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(GJGameLevel*, DailyLevelPage*, bool);
	bool init(GJGameLevel*, DailyLevelPage*, bool);
	void onClaimReward(cocos2d::CCObject*);
	void onSkipLevel(cocos2d::CCObject*);
	void showSkipButton();
	void updateTimeLabel(gd::string);
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
	bool init(bool);
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
}
class DelayedSpawnNode : cocos2d::CCObject {
	void create();
	bool init();
}
class DelaySection : cocos2d::CCObject {
	void create(int, float);
	bool init(int, float);
}
class DemonFilterDelegate {
	void demonFilterSelectClosed(int);
}
class DemonFilterSelectLayer : FLAlertLayer {
	void create();
	bool init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void selectRating(cocos2d::CCObject*);
}
class DialogDelegate {
	void dialogClosed(DialogLayer*);
}
class DialogLayer : cocos2d::CCLayerColor, TextAreaDelegate {
	void animateIn(DialogAnimationType);
	void animateInDialog();
	void animateInRandomSide();
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
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
	bool init(DialogObject*, cocos2d::CCArray*, int);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void onClose();
	void onEnter();
	void registerWithTouchDispatcher();
	void updateChatPlacement(DialogChatPlacement);
	void updateNavButtonFrame();
}
class DialogObject : cocos2d::CCObject {
	void create(gd::string, gd::string, int, float, bool, cocos2d::ccColor3B);
	bool init(gd::string, gd::string, int, float, bool, cocos2d::ccColor3B);
}
class DownloadMessageDelegate {
	void downloadMessageFailed(int);
	void downloadMessageFinished(GJUserMessage*);
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
	bool init(cocos2d::CCNode*, LevelEditorLayer*);
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
}
class DungeonBarsSprite : cocos2d::CCNode {
	void animateOutBars();
	void create();
	bool init();
	void visit();
}
class DynamicScrollDelegate {
}
class EditButtonBar : cocos2d::CCNode {
	void create(cocos2d::CCArray*, cocos2d::CCPoint, int, bool, int, int);
	void getPage();
	void goToPage(int);
	bool init(cocos2d::CCArray*, cocos2d::CCPoint, int, bool, int, int);
	void loadFromItems(cocos2d::CCArray*, int, int, bool);
	void onLeft(cocos2d::CCObject*);
	void onRight(cocos2d::CCObject*);
	void reloadItems(int, int);
}
class EditGameObjectPopup : FLAlertLayer {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
	bool init(GJGameLevel*);
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
}
class EditorOptionsLayer : GJOptionsLayer {
	void create();
	bool init();
	void onButtonRows(cocos2d::CCObject*);
	void onButtonsPerRow(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void setupOptions();
}
class EditorPauseLayer : CCBlockLayer, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(LevelEditorLayer*);
	void customSetup();
	void doResetUnused();
	bool init(LevelEditorLayer*);
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
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void changeSelectedObjects(cocos2d::CCArray*, bool);
	void clickOnPosition(cocos2d::CCPoint);
	void closeLiveColorSelect();
	void colorSelectClosed(cocos2d::ccColor3B);
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
	bool init(LevelEditorLayer*);
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
}
class EditTriggersPopup : FLAlertLayer {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
	void customObjectSetup(gd::map<gd::string, gd::string>&);
	void customSetup();
	void getSaveString();
	void getTargetColorIndex();
	bool init(char const*);
	void resetSpawnTrigger();
	void spawnXPosition();
	void triggerActivated(float);
	void triggerObject(GJBaseGameLayer*);
	void updateSpecialColor();
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
}
class EndPortalObject : GameObject {
	void calculateSpawnXPos();
	void create();
	bool init();
	void setPosition(cocos2d::CCPoint const&);
	void setVisible(bool);
	void triggerObject(GJBaseGameLayer*);
	void updateColors(cocos2d::ccColor3B);
}
class ExtendedLayer : cocos2d::CCLayer {
	void create();
	bool init();
	void setPosition(cocos2d::CCPoint const&);
}
class FileSaveManager : GManager {
	void firstLoad();
	void getStoreData();
	bool init();
	void loadDataFromFile(char const*);
	void sharedState();
}
class FLAlertLayer : cocos2d::CCLayerColor {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*);
	void create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float);
	void create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float);
	bool init(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float);
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void onBtn1(cocos2d::CCObject*);
	void onBtn2(cocos2d::CCObject*);
	void onEnter();
	void registerWithTouchDispatcher();
	void show();
}
class FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
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
}
class FMODSound : cocos2d::CCNode {
	void create(FMOD::Sound*);
	bool init(FMOD::Sound*);
}
class FollowRewardPage : FLAlertLayer, FLAlertLayerProtocol, GameRateDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	bool init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSpecialItem(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void show();
	void switchToOpenedState(CCMenuItemSpriteExtra*);
	void updateRate();
}
class FontObject : cocos2d::CCObject {
	void createWithConfigFile(char const*, float);
	void getFontWidth(int);
	bool initWithConfigFile(char const*, float);
	void parseConfigFile(char const*, float);
}
class FRequestProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, FriendRequestDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(bool);
	void deleteSelected();
	void forceReloadRequests(bool);
	bool init(bool);
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
}
class FriendRequestDelegate {
	void forceReloadRequests(bool);
	void loadFRequestsFailed(char const*, GJErrorCode);
	void loadFRequestsFinished(cocos2d::CCArray*, char const*);
	void setupPageInfo(gd::string, char const*);
}
class FriendRequestPopup : FLAlertLayer, UploadActionDelegate, UploadPopupDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void blockUser();
	void create(GJFriendRequest*);
	bool init(GJFriendRequest*);
	void keyBackClicked();
	void loadFromGJFriendRequest(GJFriendRequest*);
	void onAccept(cocos2d::CCObject*);
	void onBlock(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onRemove(cocos2d::CCObject*);
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
}
class FriendsProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, UserListDelegate {
	void create(UserListType);
	void forceReloadList(UserListType);
	void getUserListFailed(UserListType, GJErrorCode);
	void getUserListFinished(cocos2d::CCArray*, UserListType);
	bool init(UserListType);
	void keyBackClicked();
	void onBlocked(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onUpdate(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void setupUsersBrowser(cocos2d::CCArray*, UserListType);
	void userListChanged(cocos2d::CCArray*, UserListType);
}
class GameCell : TableViewCell {
	void draw();
	bool init();
	void loadFromString(gd::string);
	void onTouch(cocos2d::CCObject*);
	void updateBGColor(int);
}
class GameEffectsManager : cocos2d::CCNode {
	void addParticleEffect(cocos2d::CCParticleSystemQuad*, int);
	void create(PlayLayer*);
	bool init(PlayLayer*);
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
	bool init();
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
	bool init();
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
	void customObjectSetup(gd::map<gd::string, gd::string>&);
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
	void groupColor(cocos2d::ccColor3B const&, bool);
	void groupOpacityMod();
	void groupWasDisabled();
	void groupWasEnabled();
	void hasBeenActivated();
	void hasBeenActivatedByPlayer(GameObject*);
	void hasSecondaryColor();
	void ignoreEditorDuration();
	void ignoreEnter();
	void ignoreFade();
	bool init(char const*);
	bool initWithTexture(cocos2d::CCTexture2D*);
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
	void selectObject(cocos2d::ccColor3B);
	void setChildColor(cocos2d::ccColor3B const&);
	void setDefaultMainColorMode(int);
	void setDefaultSecondaryColorMode(int);
	void setDidUpdateLastPosition(bool const&);
	void setFlipX(bool);
	void setFlipY(bool);
	void setGlowColor(cocos2d::ccColor3B const&);
	void setGlowOpacity(unsigned char);
	void setLastPosition(cocos2d::CCPoint const&);
	void setMainColorMode(int);
	void setMyAction(cocos2d::CCAction*);
	void setObjectColor(cocos2d::ccColor3B const&);
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
	void updateMainColor(cocos2d::ccColor3B const&);
	void updateMainColor();
	void updateObjectEditorColor();
	void updateOrientedBox();
	void updateParticleColor(cocos2d::ccColor3B const&);
	void updateSecondaryColor(cocos2d::ccColor3B const&);
	void updateSecondaryColor();
	void updateStartPos();
	void updateStartValues();
	void updateState();
	void updateSyncedAnimation(float);
	void updateTextObject(gd::string, bool);
	void waitingForActivation();
}
class GameObjectCopy : cocos2d::CCObject {
	void create(GameObject*);
	bool init(GameObject*);
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
	bool init();
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
	bool init();
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
	bool init(IconType, GJGarageLayer*, cocos2d::SEL_MenuHandler);
	void listButtonBarSwitchedPage(ListButtonBar*, int);
	void onGlow(cocos2d::CCObject*);
	void onSelect(cocos2d::CCObject*);
	void updateSelect(cocos2d::CCNode*);
}
class GauntletLayer : cocos2d::CCLayer, LevelManagerDelegate {
	void create(GauntletType);
	bool init(GauntletType);
	void keyBackClicked();
	void loadLevelsFailed(char const*);
	void loadLevelsFinished(cocos2d::CCArray*, char const*);
	void onBack(cocos2d::CCObject*);
	void onLevel(cocos2d::CCObject*);
	void scene(GauntletType);
	void setupGauntlet(cocos2d::CCArray*);
	void unlockActiveItem();
}
class GauntletNode : cocos2d::CCNode {
	void create(GJMapPack*);
	void frameForType(GauntletType);
	bool init(GJMapPack*);
	void nameForType(GauntletType);
	void onClaimReward();
}
class GauntletSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate, LevelManagerDelegate {
	void create(int);
	void goToPage(int, bool);
	bool init(int);
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
}
class GhostTrailEffect : cocos2d::CCNode {
	void create();
	void doBlendAdditive();
	void draw();
	bool init();
	void runWithTarget(cocos2d::CCSprite*, float, float, float, float, bool);
	void stopTrail();
	void trailSnapshot(float);
}
class GJAccountBackupDelegate {
	void backupAccountFailed(BackupAccountError);
	void backupAccountFinished();
}
class GJAccountDelegate {
	void accountStatusChanged();
}
class GJAccountLoginDelegate {
	bool loginAccountFailed(AccountError);
	bool loginAccountFinished(int, int);
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
	bool init();
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
}
class GJAccountSettingsDelegate {
	void updateSettingsFailed();
	void updateSettingsFinished();
}
class GJAccountSettingsLayer : FLAlertLayer, TextInputDelegate {
	void create(int);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, float, float);
	bool init(int);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onCommentSetting(cocos2d::CCObject*);
	void onFriendRequests(cocos2d::CCObject*);
	void onMessageSetting(cocos2d::CCObject*);
	void onUpdate(cocos2d::CCObject*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
	void updateScoreValues();
}
class GJAccountSyncDelegate {
	void syncAccountFailed(BackupAccountError);
	void syncAccountFinished();
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
	bool init();
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
	void updateColor(cocos2d::ccColor3B, float, int, bool, float, cocos2d::ccHSVValue, int, bool, int, EffectGameObject*);
	void updateCounters(int, int);
	void updateDisabledObjectsLastPos(cocos2d::CCArray*);
	void updateLayerCapacity(gd::string);
	void updateLegacyLayerCapacity(int, int, int, int);
	void updateOBB2(cocos2d::CCRect);
	void updateQueuedLabels();
}
class GJChallengeDelegate {
	void challengeStatusFailed();
	void challengeStatusFinished();
}
class GJChallengeItem : cocos2d::CCObject {
	bool canEncode();
	void create(GJChallengeType, int, int, int, gd::string);
	void create();
	void createFromString(gd::string);
	void createWithCoder(DS_Dictionary*);
	void dataLoaded(DS_Dictionary*);
	void encodeWithCoder(DS_Dictionary*);
	void incrementCount(int);
	bool init(GJChallengeType, int, int, int, gd::string);
	void setCount(int);
}
class GJChestSprite : cocos2d::CCSprite {
	void create(int);
	bool init(int);
	void setColor(cocos2d::ccColor3B const&);
	void switchToState(ChestSpriteState, bool);
}
class GJColorSetupLayer : FLAlertLayer, ColorSelectDelegate, FLAlertLayerProtocol {
	void colorSelectClosed(cocos2d::CCNode*);
	void create(LevelSettingsObject*);
	bool init(LevelSettingsObject*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onColor(cocos2d::CCObject*);
	void onPage(cocos2d::CCObject*);
	void showPage(int);
	void updateSpriteColor(ColorChannelSprite*, cocos2d::CCLabelBMFont*, int);
	void updateSpriteColors();
}
class GJComment : cocos2d::CCNode {
	void create(cocos2d::CCDictionary*);
	void create();
	bool init();
}
class GJCommentListLayer : cocos2d::CCLayerColor {
	void create(BoomListView*, char const*, cocos2d::ccColor4B, float, float, bool);
	bool init(BoomListView*, char const*, cocos2d::ccColor4B, float, float, bool);
}
class GJDailyLevelDelegate {
	void dailyStatusFailed(bool);
	void dailyStatusFinished(bool);
}
class GJDropDownLayer : cocos2d::CCLayerColor {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
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
	bool init(char const*);
	bool init(char const*, float);
	void keyBackClicked();
	void layerHidden();
	void layerVisible();
	void registerWithTouchDispatcher();
	void showLayer(bool);
}
class GJDropDownLayerDelegate {
	void dropDownLayerWillClose(GJDropDownLayer*);
}
class GJEffectManager : cocos2d::CCNode {
	void activeColorForIndex(int);
	void activeOpacityForIndex(int);
	void addAllInheritedColorActions(cocos2d::CCArray*);
	void addGroupPulseEffect(PulseEffectAction*);
	void calculateBaseActiveColors();
	void calculateInheritedColor(int, ColorAction*);
	void calculateLightBGColor(cocos2d::ccColor3B);
	void colorActionChanged(ColorAction*);
	void colorExists(int);
	void colorForEffect(cocos2d::ccColor3B, cocos2d::ccHSVValue);
	void colorForGroupID(int, cocos2d::ccColor3B const&, bool);
	void colorForIndex(int);
	void colorForPulseEffect(cocos2d::ccColor3B const&, PulseEffectAction*);
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
	void getMixedColor(cocos2d::ccColor3B, cocos2d::ccColor3B, float);
	void getOpacityActionForGroup(int);
	void getSaveString();
	void handleObjectCollision(bool, int, int);
	void hasActiveDualTouch();
	void hasBeenTriggered(int);
	void hasPulseEffectForGroupID(int);
	void incrementCountForItem(int);
	bool init();
	void isGroupEnabled(int);
	void keyForGroupIDColor(int, cocos2d::ccColor3B const&, bool);
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
	void runPulseEffect(int, bool, float, float, float, PulseEffectType, cocos2d::ccColor3B, cocos2d::ccHSVValue, int, bool, bool, bool, int);
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
	void updateColors(cocos2d::ccColor3B, cocos2d::ccColor3B);
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
	bool init();
}
class GJFollowCommandLayer : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class GJFriendRequest : cocos2d::CCNode {
	void create(cocos2d::CCDictionary*);
	void create();
	bool init();
}
class GJGameLevel : cocos2d::CCNode {
	void areCoinsVerified();
	bool canEncode();
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
	bool init();
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
	bool init();
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
	bool init(int, int);
	void loadGroundSprites(int, bool);
	void positionGround(float);
	void showGround();
	void updateGround01Color(cocos2d::ccColor3B);
	void updateGround02Color(cocos2d::ccColor3B);
	void updateGroundPos(cocos2d::CCPoint);
	void updateGroundWidth();
	void updateLineBlend(bool);
}
class GJHttpResult : cocos2d::CCNode {
	void create(bool, gd::string, gd::string, GJHttpType);
	bool init(bool, gd::string, gd::string, GJHttpType);
}
class GJItemIcon : cocos2d::CCSprite {
	void create(UnlockType, int, cocos2d::ccColor3B, cocos2d::ccColor3B, bool, bool, bool, cocos2d::ccColor3B);
	void createBrowserItem(UnlockType, int);
	void createStoreItem(UnlockType, int, bool, cocos2d::ccColor3B);
	bool init(UnlockType, int, cocos2d::ccColor3B, cocos2d::ccColor3B, bool, bool, bool, cocos2d::ccColor3B);
	void setOpacity(unsigned char);
}
class GJLevelScoreCell : TableViewCell {
	void draw();
	bool init();
	void loadFromScore(GJUserScore*);
	void onViewProfile(cocos2d::CCObject*);
	void updateBGColor(int);
}
class GJListLayer : cocos2d::CCLayerColor {
	void create(BoomListView*, char const*, cocos2d::ccColor4B, float, float);
	bool init(BoomListView*, char const*, cocos2d::ccColor4B, float, float);
}
class GJMapPack : cocos2d::CCNode {
	void completedMaps();
	void create(cocos2d::CCDictionary*);
	void create();
	void hasCompletedMapPack();
	bool init();
	void parsePackColors(gd::string, gd::string);
	void parsePackLevels(gd::string);
	void totalMaps();
}
class GJMessageCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void draw();
	bool init();
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
}
class GJMessagePopup : FLAlertLayer, UploadActionDelegate, UploadPopupDelegate, FLAlertLayerProtocol, DownloadMessageDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void blockUser();
	void create(GJUserMessage*);
	void downloadMessageFailed(int);
	void downloadMessageFinished(GJUserMessage*);
	bool init(GJUserMessage*);
	void keyBackClicked();
	void loadFromGJMessage(GJUserMessage*);
	void onBlock(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onRemove(cocos2d::CCObject*);
	void onReply(cocos2d::CCObject*);
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
}
class GJMoreGamesLayer : GJDropDownLayer {
	void create();
	void customSetup();
	void getMoreGamesList();
}
class GJMoveCommandLayer : FLAlertLayer, TextInputDelegate, ConfigureValuePopupDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class GJObjectDecoder : cocos2d::CCNode, ObjectDecoderDelegate {
	void getDecodedObject(int, DS_Dictionary*);
	bool init();
	void sharedDecoder();
}
class GJOptionsLayer : FLAlertLayer {
	void addToggle(char const*, char const*, char const*);
	void countForPage(int);
	void create();
	void goToPage(int);
	void incrementCountForPage(int);
	void infoKey(int);
	bool init();
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
}
class GJPFollowCommandLayer : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class GJPurchaseDelegate {
	void didPurchaseItem(GJStoreItem*);
}
class GJRequestCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void draw();
	bool init();
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
}
class GJRewardDelegate {
	void rewardsStatusFailed();
	void rewardsStatusFinished(int);
}
class GJRewardItem : cocos2d::CCObject {
	bool canEncode();
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
	bool init(int, int, gd::string);
	void isShardType(SpecialRewardItem);
	void rewardItemToStat(SpecialRewardItem);
	void setObjects(cocos2d::CCArray*);
}
class GJRewardObject : cocos2d::CCObject {
	bool canEncode();
	void create(SpecialRewardItem, int, int);
	void create();
	void createItemUnlock(UnlockType, int);
	void createWithCoder(DS_Dictionary*);
	void dataLoaded(DS_Dictionary*);
	void encodeWithCoder(DS_Dictionary*);
	bool init(SpecialRewardItem, int, int);
	void isSpecialType();
}
class GJRobotSprite : CCAnimatedSprite {
	void create();
	void hideGlow();
	void hideSecondary();
	bool init(gd::string);
	bool init();
	void setOpacity(unsigned char);
	void showGlow();
	void updateColor01(cocos2d::ccColor3B);
	void updateColor02(cocos2d::ccColor3B);
	void updateColors();
	void updateFrame(int);
	void updateGlowColor(cocos2d::ccColor3B, bool);
}
class GJRotateCommandLayer : FLAlertLayer, TextInputDelegate, ConfigureValuePopupDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class GJRotationControl : cocos2d::CCLayer {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void draw();
	void finishTouch();
	bool init();
	void setAngle(float);
	void updateSliderPosition(cocos2d::CCPoint);
}
class GJRotationControlDelegate {
	void angleChangeBegin();
	void angleChangeEnded();
	void angleChanged(float);
}
class GJScaleControl : cocos2d::CCLayer {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void finishTouch();
	bool init();
	void loadValues(GameObject*, cocos2d::CCArray*);
	void scaleFromValue(float);
	void sliderChanged(cocos2d::CCObject*);
	void updateLabel(float);
	void valueFromScale(float);
}
class GJScaleControlDelegate {
	void scaleChangeBegin();
	void scaleChangeEnded();
	void scaleChanged(float);
}
class GJScoreCell : TableViewCell, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void draw();
	bool init();
	void loadFromScore(GJUserScore*);
	void onBan(cocos2d::CCObject*);
	void onCheck(cocos2d::CCObject*);
	void onMoreLevels(cocos2d::CCObject*);
	void onViewProfile(cocos2d::CCObject*);
	void updateBGColor(int);
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
	bool init(SearchType, gd::string, gd::string, gd::string, int, bool, bool, bool, int, bool, bool, bool, bool, bool, bool, bool, bool, int, int);
}
class GJShopLayer : cocos2d::CCLayer, GJPurchaseDelegate {
	void create(ShopType);
	void didPurchaseItem(GJStoreItem*);
	void exitVideoAdItems();
	bool init(ShopType);
	void keyBackClicked();
	void onBack(cocos2d::CCObject*);
	void onCommunityCredits(cocos2d::CCObject*);
	void onPlushies(cocos2d::CCObject*);
	void onSelectItem(cocos2d::CCObject*);
	void onVideoAd(cocos2d::CCObject*);
	void scene(ShopType);
	void showVideoAdReward(int);
	void updateCurrencyCounter();
}
class GJSongBrowser : GJDropDownLayer, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(LevelSettingsObject*);
	void customSetup();
	bool init(LevelSettingsObject*);
	void loadPage(int);
	void onDeleteAll(cocos2d::CCObject*);
	void onNextPage(cocos2d::CCObject*);
	void onPrevPage(cocos2d::CCObject*);
	void setupPageInfo(int, int, int);
	void setupSongBrowser(cocos2d::CCArray*);
}
class GJSpecialColorSelect : FLAlertLayer {
	void create(int, GJSpecialColorSelectDelegate*, bool);
	void getButtonByTag(int);
	void highlightSelected(ButtonSprite*);
	bool init(int, GJSpecialColorSelectDelegate*, bool);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSelectColor(cocos2d::CCObject*);
	void textForColorIdx(int);
}
class GJSpecialColorSelectDelegate {
	void colorSelectClosed(GJSpecialColorSelect*, int);
}
class GJSpiderSprite : GJRobotSprite {
	void create();
	bool init();
}
class GJSpriteColor : cocos2d::CCNode {
	void create();
	void getColorMode();
	bool init();
	void resetCustomColorMode();
	void setCustomColorMode(int);
	void setDefaultColorMode(int);
}
class GJStoreItem : cocos2d::CCNode {
	void create(int, int, int, int, ShopType);
	bool init(int, int, int, int, ShopType);
}
class GJUnlockableItem : cocos2d::CCObject {
	void create();
	bool init();
}
class GJUserCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void draw();
	bool init();
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
}
class GJUserMessage : cocos2d::CCNode {
	void create(cocos2d::CCDictionary*);
	void create();
	bool init();
}
class GJUserScore : cocos2d::CCNode {
	void create(cocos2d::CCDictionary*);
	void create();
	bool init();
	void isCurrentUser();
	void mergeWithScore(GJUserScore*);
}
class GJWorldNode : cocos2d::CCNode {
	void addDotsToLevel(int, bool);
	void create(int, WorldSelectLayer*);
	void dotPositionForLevel(int, int);
	bool init(int, WorldSelectLayer*);
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
	bool init(int, int);
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
}
class GManager : cocos2d::CCNode {
	void dataLoaded(DS_Dictionary*);
	void encodeDataTo(DS_Dictionary*);
	void firstLoad();
	void getCompressedSaveString();
	void getSaveString();
	bool init();
	void load();
	void loadDataFromFile(gd::string);
	void loadFromCompressedString(gd::string);
	void loadFromString(gd::string);
	void save();
	void saveData(DS_Dictionary*, gd::string);
	void saveGMTo(gd::string);
	void setup();
	void tryLoadData(DS_Dictionary*, gd::string);
}
class GooglePlayDelegate {
	void googlePlaySignedIn();
}
class GooglePlayManager : cocos2d::CCNode {
	void googlePlaySignedIn();
	bool init();
	void sharedState();
}
class GravityEffectSprite : cocos2d::CCSprite {
	void create();
	void draw();
	bool init();
	void updateSpritesColor(cocos2d::ccColor3B);
}
class GroupCommandObject : cocos2d::CCNode {
	void create();
	void createFromString(gd::string);
	void easeToText(int);
	void getEasedAction(cocos2d::CCActionInterval*, int, float);
	void getSaveString();
	bool init();
	void resetDelta(bool);
	void runFollowCommand(float, float, float, bool);
	void runMoveCommand(cocos2d::CCPoint, float, int, float, bool, bool);
	void runPlayerFollowCommand(float, float, int, float, float);
	void runRotateCommand(float, float, int, float, bool);
	void step(float);
	void updateTweenAction(float, char const*);
}
class HardStreak : cocos2d::CCDrawNode {
	void addPoint(cocos2d::CCPoint);
	void clearBehindXPos(float);
	void create();
	void firstSetup();
	bool init();
	void normalizeAngle(double);
	void quadCornerOffset(cocos2d::CCPoint, cocos2d::CCPoint, float);
	void reset();
	void resumeStroke();
	void stopStroke();
	void updateStroke(float);
}
class HSVWidgetPopup : FLAlertLayer {
	void create(cocos2d::ccHSVValue, HSVWidgetPopupDelegate*, gd::string);
	bool init(cocos2d::ccHSVValue, HSVWidgetPopupDelegate*, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
}
class HSVWidgetPopupDelegate {
	void hsvPopupClosed(HSVWidgetPopup*, cocos2d::ccHSVValue);
}
class InfoAlertButton : CCMenuItemSpriteExtra {
	void activate();
	void create(gd::string, gd::string, float);
	bool init(gd::string, gd::string, float);
}
class InfoLayer : FLAlertLayer, LevelCommentDelegate, CommentUploadDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void commentUploadFailed(int, CommentError);
	void commentUploadFinished(int);
	void confirmReport(cocos2d::CCObject*);
	void create(GJGameLevel*, GJUserScore*);
	void getSpriteButton(char const*, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float, cocos2d::CCPoint);
	bool init(GJGameLevel*, GJUserScore*);
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
}
class InheritanceNode : cocos2d::CCObject {
	void create(int, InheritanceNode*);
	bool init(int, InheritanceNode*);
}
class InstantSection : cocos2d::CCObject {
	void create(int, int);
	bool init(int, int);
}
class KeybindingsLayer : FLAlertLayer {
	void addKeyPair(char const*, char const*);
	void countForPage(int);
	void create();
	void goToPage(int);
	void incrementCountForPage(int);
	void infoKey(int);
	bool init();
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
	bool init();
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
	bool init();
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
}
class LabelGameObject : EffectGameObject {
	void addMainSpriteToParent(bool);
	void create();
	void customObjectSetup(gd::map<gd::string, gd::string>&);
	void getSaveString();
	bool init();
	void queueUpdateLabel(gd::string);
	void setObjectColor(cocos2d::ccColor3B const&);
	void setOpacity(unsigned char);
	void setupCustomSprites();
	void updateLabel(gd::string);
	void updateLabelIfDirty();
}
class LeaderboardManagerDelegate {
	void loadLeaderboardFailed(char const*);
	void loadLeaderboardFinished(cocos2d::CCArray*, char const*);
	void updateUserScoreFailed();
	void updateUserScoreFinished();
}
class LeaderboardsLayer : cocos2d::CCLayer, LeaderboardManagerDelegate {
	void create(LeaderboardState);
	bool init(LeaderboardState);
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
}
class LevelBrowserLayer : cocos2d::CCLayer, LevelManagerDelegate, FLAlertLayerProtocol, SetIDPopupDelegate, SetTextPopupDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(GJSearchObject*);
	void getLevelMatchingSearch(cocos2d::CCArray*, gd::string);
	bool init(GJSearchObject*);
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
}
class LevelCell : TableViewCell {
	void create(float, float);
	void draw();
	bool init();
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
}
class LevelDeleteDelegate {
	void levelDeleteFailed(int);
	void levelDeleteFinished(int);
}
class LevelDownloadDelegate {
	void levelDownloadFailed(int);
	void levelDownloadFinished(GJGameLevel*);
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
	bool init(GJGameLevel*);
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
	void updateColor(cocos2d::ccColor3B, float, int, bool, float, cocos2d::ccHSVValue, int, bool, int, EffectGameObject*);
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
}
class LevelFeatureLayer : FLAlertLayer {
	void create(int);
	bool init(int);
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
	bool init(GJGameLevel*);
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
}
class LevelLeaderboard : FLAlertLayer, LeaderboardManagerDelegate {
	void create(GJGameLevel*, LevelLeaderboardType);
	bool init(GJGameLevel*, LevelLeaderboardType);
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
}
class LevelManagerDelegate {
	void loadLevelsFailed(char const*);
	void loadLevelsFinished(cocos2d::CCArray*, char const*);
	void setupPageInfo(gd::string, char const*);
}
class LevelPage : cocos2d::CCLayer, DialogDelegate {
	void addSecretCoin();
	void addSecretDoor();
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(GJGameLevel*);
	void dialogClosed(DialogLayer*);
	bool init(GJGameLevel*);
	void onInfo(cocos2d::CCObject*);
	void onPlay(cocos2d::CCObject*);
	void onSecretDoor(cocos2d::CCObject*);
	void playCoinEffect();
	void playStep2();
	void playStep3();
	void registerWithTouchDispatcher();
	void updateDynamicPage(GJGameLevel*);
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
	bool init();
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
}
class LevelSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate, DynamicScrollDelegate {
	void colorForPage(int);
	void create(int);
	void getColorValue(int, int, float);
	bool init(int);
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
	bool init(LevelSettingsObject*, LevelEditorLayer*);
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
}
class LevelSettingsObject : cocos2d::CCNode {
	void create();
	void getSaveString();
	bool init();
	void objectFromDict(cocos2d::CCDictionary*);
	void objectFromString(gd::string);
	void offsetMusic();
	void setupColorsFromLegacyMode(cocos2d::CCDictionary*);
}
class LevelUpdateDelegate {
	void levelUpdateFailed(int);
	void levelUpdateFinished(GJGameLevel*, UpdateResponse);
}
class LevelUploadDelegate {
	void levelUploadFailed(GJGameLevel*);
	void levelUploadFinished(GJGameLevel*);
}
class LikeItemDelegate {
	void likedItem(LikeItemType, int, bool);
}
class LikeItemLayer : FLAlertLayer {
	void create(LikeItemType, int, int);
	bool init(LikeItemType, int, int);
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
	bool init(cocos2d::CCArray*, cocos2d::CCPoint, int, int, float, float, float, float, int);
	void onLeft(cocos2d::CCObject*);
	void onRight(cocos2d::CCObject*);
}
class ListButtonBarDelegate {
	void listButtonBarSwitchedPage(ListButtonBar*, int);
}
class ListButtonPage : cocos2d::CCLayer {
	void create(cocos2d::CCArray*, cocos2d::CCPoint, int, int, float, float, float);
	bool init(cocos2d::CCArray*, cocos2d::CCPoint, int, int, float, float, float);
}
class ListCell : TableViewCell {
	void draw();
	bool init();
	void loadFromObject(cocos2d::CCObject*, int, int, int);
	void updateBGColor(int);
}
class LoadingCircle : cocos2d::CCLayerColor {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void draw();
	void fadeAndRemove();
	bool init();
	void registerWithTouchDispatcher();
	void show();
}
class LoadingLayer : cocos2d::CCLayer {
	void create(bool);
	void getLoadingString();
	bool init(bool);
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
	bool init();
	void markLevelsAsUnmodified();
	void moveLevelToTop(GJGameLevel*);
	void reorderLevels();
	void setLocalLevels(cocos2d::CCArray*);
	void sharedState();
	void updateLevelOrder();
	void updateLevelRevision();
}
class MapPackCell : TableViewCell {
	void draw();
	bool init();
	void loadFromMapPack(GJMapPack*);
	void onClaimReward(cocos2d::CCObject*);
	void onClick(cocos2d::CCObject*);
	void playCompleteEffect();
	void reloadCell();
	void updateBGColor(int);
}
class MenuGameLayer : cocos2d::CCLayer {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void destroyPlayer();
	void getBGColor(int);
	bool init();
	void registerWithTouchDispatcher();
	void resetPlayer();
	void tryJump(float);
	void update(float);
	void updateColor(float);
	void updateColors();
}
class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol, GooglePlayDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void endGame();
	void googlePlaySignedIn();
	bool init();
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
}
class MessageListDelegate {
	void forceReloadMessages(bool);
	void loadMessagesFailed(char const*, GJErrorCode);
	void loadMessagesFinished(cocos2d::CCArray*, char const*);
	void setupPageInfo(gd::string, char const*);
}
class MessagesProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, MessageListDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create(bool);
	void deleteSelected();
	void forceReloadMessages(bool);
	bool init(bool);
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
}
class MoreOptionsLayer : FLAlertLayer, TextInputDelegate, GooglePlayDelegate {
	void addToggle(char const*, char const*, char const*);
	void countForPage(int);
	void create();
	void goToPage(int);
	void googlePlaySignedIn();
	void incrementCountForPage(int);
	void infoKey(int);
	bool init();
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
}
class MoreSearchLayer : FLAlertLayer, TextInputDelegate {
	void audioNext(cocos2d::CCObject*);
	void audioPrevious(cocos2d::CCObject*);
	void create();
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	bool init();
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
}
class MoreVideoOptionsLayer : FLAlertLayer {
	void addToggle(char const*, char const*, char const*);
	void countForPage(int);
	void create();
	void goToPage(int);
	void incrementCountForPage(int);
	void infoKey(int);
	bool init();
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
	bool initWithFont(char const*, gd::string, float, float, cocos2d::CCPoint, int, bool);
	void moveSpecialDescriptors(int, int);
	void readColorInfo(gd::string);
	void setOpacity(unsigned char);
	void stringWithMaxWidth(gd::string, float, float);
}
class MusicDelegateHandler : cocos2d::CCNode {
	void create(MusicDownloadDelegate*);
	bool init(MusicDownloadDelegate*);
}
class MusicDownloadDelegate {
	void downloadSongFailed(int, GJSongError);
	void downloadSongFinished(SongInfoObject*);
	void loadSongInfoFailed(int, GJSongError);
	void loadSongInfoFinished(SongInfoObject*);
	void songStateChanged();
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
	bool init();
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
	bool init();
	void keyBackClicked();
	void onBack(cocos2d::CCObject*);
	void onNew(cocos2d::CCObject*);
	void scene();
	void setupLevelBrowser();
}
class NodePoint : cocos2d::CCObject {
	void create(cocos2d::CCPoint);
	bool init(cocos2d::CCPoint);
}
class NumberInputDelegate {
	void numberInputClosed(NumberInputLayer*);
}
class NumberInputLayer : FLAlertLayer {
	void create();
	void deleteLast();
	bool init();
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
	bool init(cocos2d::CCPoint, float, float, float);
	void orderCorners();
	void overlaps(OBB2D*);
	void overlaps1Way(OBB2D*);
}
class ObjectDecoder : cocos2d::CCNode {
	void getDecodedObject(int, DS_Dictionary*);
	bool init();
	void sharedDecoder();
}
class ObjectDecoderDelegate {
	void getDecodedObject(int, DS_Dictionary*);
}
class ObjectManager : cocos2d::CCNode {
	void animLoaded(char const*);
	void getDefinition(char const*);
	void getGlobalAnimCopy(char const*);
	bool init();
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
	bool init();
	void intKeyToFrame(int);
	void keyToFrame(char const*);
	void perspectiveBlockFrame(int);
	void sharedState();
}
class OnlineListDelegate {
	void loadListFailed(char const*);
	void loadListFinished(cocos2d::CCArray*, char const*);
	void setupPageInfo(gd::string, char const*);
}
class OpacityEffectAction : cocos2d::CCNode {
	void create(float, float, float, int);
	void createFromString(gd::string);
	void getSaveString();
	bool init(float, float, float, int);
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
}
class ParentalOptionsLayer : FLAlertLayer {
	void addToggle(char const*, char const*, char const*);
	void countForPage(int);
	void create();
	void goToPage(int);
	void incrementCountForPage(int);
	void infoKey(int);
	bool init();
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
	bool init(bool);
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
}
class PlatformDownloadDelegate {
	void downloadFailed(char const*);
	void downloadFinished(char const*);
}
class PlayerCheckpoint : cocos2d::CCNode {
	void create();
	bool init();
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
	void flashPlayer(float, float, cocos2d::ccColor3B, cocos2d::ccColor3B);
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
	bool init(int, int, cocos2d::CCLayer*);
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
	void setColor(cocos2d::ccColor3B const&);
	void setFlipX(bool);
	void setFlipY(bool);
	void setOpacity(unsigned char);
	void setPortalP(cocos2d::CCPoint);
	void setPosition(cocos2d::CCPoint const&);
	void setRotation(float);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void setSecondColor(cocos2d::ccColor3B const&);
	void setVisible(bool);
	void setupStreak();
	void spawnCircle();
	void spawnCircle2();
	void spawnDualCircle();
	void spawnFromPlayer(PlayerObject*);
	void spawnPortalCircle(cocos2d::ccColor3B, float);
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
	void colorObject(int, cocos2d::ccColor3B);
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
	bool init(GJGameLevel*);
	void isFlipping();
	void levelComplete();
	void lightningFlash(cocos2d::CCPoint, cocos2d::ccColor3B);
	void lightningFlash(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::ccColor3B, float, float, int, bool, float);
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
	void updateColor(cocos2d::ccColor3B, float, int, bool, float, cocos2d::ccHSVValue, int, bool, int, EffectGameObject*);
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
}
class PointNode : cocos2d::CCObject {
	void create(cocos2d::CCPoint);
	bool init(cocos2d::CCPoint);
}
class PriceLabel : cocos2d::CCNode {
	void create(int);
	bool init(int);
	void setColor(cocos2d::ccColor3B);
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
	bool init(int, bool);
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
}
class PromoInterstitial : FLAlertLayer {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(bool);
	bool init(bool);
	void keyBackClicked();
	void onClick(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void setup();
	void show();
}
class PulseEffectAction : cocos2d::CCNode {
	void create(float, float, float, PulseEffectType, int, cocos2d::ccColor3B, cocos2d::ccHSVValue, int, bool, bool);
	void createFromString(gd::string);
	void getSaveString();
	bool init(float, float, float, PulseEffectType, int, cocos2d::ccColor3B, cocos2d::ccHSVValue, int, bool, bool);
	void isFinished();
	void step(float);
	void valueForDelta(float, float, float, float);
}
class PurchaseItemPopup : FLAlertLayer {
	void create(GJStoreItem*);
	bool init(GJStoreItem*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onPurchase(cocos2d::CCObject*);
}
class RateDemonLayer : FLAlertLayer, UploadPopupDelegate, UploadActionDelegate {
	void create(int);
	bool init(int);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onRate(cocos2d::CCObject*);
	void selectRating(cocos2d::CCObject*);
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
}
class RateLevelDelegate {
	void rateLevelClosed();
}
class RateLevelLayer : FLAlertLayer {
	void create(int);
	bool init(int);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onRate(cocos2d::CCObject*);
	void selectRating(cocos2d::CCObject*);
}
class RateStarsLayer : FLAlertLayer, UploadPopupDelegate, UploadActionDelegate {
	void create(int, bool);
	void getStarsButton(int, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float);
	bool init(int, bool);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onClosePopup(UploadActionPopup*);
	void onRate(cocos2d::CCObject*);
	void onToggleCoins(cocos2d::CCObject*);
	void onToggleFeature(cocos2d::CCObject*);
	void selectRating(cocos2d::CCObject*);
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
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
}
class RewardsPage : FLAlertLayer, FLAlertLayerProtocol, GJRewardDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void getRewardFrame(int, int);
	bool init();
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
}
class RewardUnlockLayer : FLAlertLayer, CurrencyRewardDelegate {
	void connectionTimeout();
	void create(int, RewardsPage*);
	void currencyWillExit(CurrencyRewardLayer*);
	bool init(int, RewardsPage*);
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
}
class RingObject : EffectGameObject {
	void create(char const*);
	void customObjectSetup(gd::map<gd::string, gd::string>&);
	void getSaveString();
	bool init(char const*);
	void powerOffObject();
	void powerOnObject();
	void resetObject();
	void setRScale(float);
	void setRotation(float);
	void setScale(float);
	void spawnCircle();
	void triggerActivated(float);
}
class ScrollingLayer : cocos2d::CCLayerColor {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(cocos2d::CCSize, cocos2d::CCPoint, float);
	void draw();
	void getViewRect();
	bool init(cocos2d::CCSize, cocos2d::CCPoint, float);
	void setStartOffset(cocos2d::CCPoint);
	void visit();
}
class SearchButton : cocos2d::CCSprite {
	void create(char const*, char const*, float, char const*);
	bool init(char const*, char const*, float, char const*);
}
class SecretLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void getBasicMessage();
	void getMessage();
	void getThreadMessage();
	bool init();
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
}
class SecretLayer2 : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DialogDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void dialogClosed(DialogLayer*);
	void getBasicMessage();
	void getErrorMessage();
	void getMessage();
	void getThreadMessage();
	bool init();
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
}
class SecretLayer3 : cocos2d::CCLayer, DialogDelegate {
	void animateEyes();
	void create();
	void dialogClosed(DialogLayer*);
	void firstInteractionStep1();
	void firstInteractionStep2();
	void firstInteractionStep3();
	void firstInteractionStep4();
	bool init();
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
}
class SecretLayer4 : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DialogDelegate {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void dialogClosed(DialogLayer*);
	void getBasicMessage();
	void getErrorMessage();
	void getMessage();
	void getThreadMessage();
	bool init();
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
}
class SecretNumberLayer : cocos2d::CCLayer {
	void create();
	bool init();
	void playNumberEffect(int);
}
class SelectArtDelegate {
	void selectArtClosed(SelectArtLayer*);
}
class SelectArtLayer : FLAlertLayer {
	void create(SelectArtType);
	bool init(SelectArtType);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSelectCustom(cocos2d::CCObject*);
	void selectArt(cocos2d::CCObject*);
	void updateSelectedCustom(int);
}
class SelectFontLayer : FLAlertLayer {
	void create(LevelEditorLayer*);
	bool init(LevelEditorLayer*);
	void keyBackClicked();
	void onChangeFont(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void updateFontLabel();
}
class SetFolderPopup : SetIDPopup, SetTextPopupDelegate {
	void create(int, bool, gd::string);
	bool init(int, bool, gd::string);
	void onSetFolderName(cocos2d::CCObject*);
	void setTextPopupClosed(SetTextPopup*, gd::string);
	void valueChanged();
}
class SetGroupIDLayer : FLAlertLayer, TextInputDelegate {
	void addGroupID(int);
	void callRemoveFromGroup(float);
	void create(GameObject*, cocos2d::CCArray*);
	void determineStartValues();
	bool init(GameObject*, cocos2d::CCArray*);
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
}
class SetIDLayer : FLAlertLayer {
	void create(GameObject*);
	bool init(GameObject*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onDown(cocos2d::CCObject*);
	void onUp(cocos2d::CCObject*);
	void updateID();
}
class SetIDPopup : FLAlertLayer, TextInputDelegate {
	void create(int, int, int, gd::string, gd::string, bool, int);
	bool init(int, int, int, gd::string, gd::string, bool, int);
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
}
class SetIDPopupDelegate {
	void setIDPopupClosed(SetIDPopup*, int);
}
class SetItemIDLayer : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetTargetIDLayer : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*, gd::string);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*, gd::string);
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
}
class SetTextPopup : FLAlertLayer, TextInputDelegate {
	void create(gd::string, gd::string, int, gd::string, gd::string, bool);
	bool init(gd::string, gd::string, int, gd::string, gd::string, bool);
	void keyBackClicked();
	void onCancel(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onResetValue(cocos2d::CCObject*);
	void show();
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void updateTextInputLabel();
}
class SetTextPopupDelegate {
	void setTextPopupClosed(SetTextPopup*, gd::string);
}
class SetupAnimationPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupAnimSettingsPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupCollisionTriggerPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupCountTriggerPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupInstantCountPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupInteractObjectPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupObjectTogglePopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupOpacityPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupPickupTriggerPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupPulsePopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
	void closeColorSelect(cocos2d::CCObject*);
	void colorSelectClosed(GJSpecialColorSelect*, int);
	void colorValueChanged(cocos2d::ccColor3B);
	void create(EffectGameObject*, cocos2d::CCArray*);
	void determineStartValues();
	void getColorValue();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
	void selectColor(cocos2d::ccColor3B);
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
}
class SetupRotatePopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupShakePopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupSpawnPopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class SetupTouchTogglePopup : FLAlertLayer, TextInputDelegate {
	void create(EffectGameObject*, cocos2d::CCArray*);
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*);
	void determineStartValues();
	bool init(EffectGameObject*, cocos2d::CCArray*);
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
}
class ShardsPage : FLAlertLayer {
	void FLAlert_Clicked(FLAlertLayer*, bool);
	void create();
	void goToPage(int);
	bool init();
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSwitchPage(cocos2d::CCObject*);
	void registerWithTouchDispatcher();
	void show();
}
class ShareCommentLayer : FLAlertLayer, TextInputDelegate, UploadActionDelegate, UploadPopupDelegate {
	void create(gd::string, int, CommentType, int);
	bool init(gd::string, int, CommentType, int);
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
}
class ShareLevelLayer : FLAlertLayer {
	void create(GJGameLevel*);
	void getStarsButton(int, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float);
	bool init(GJGameLevel*);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onSettings(cocos2d::CCObject*);
	void onShare(cocos2d::CCObject*);
	void selectRating(cocos2d::CCObject*);
	void setupStars();
}
class ShareLevelSettingsLayer : FLAlertLayer, NumberInputDelegate {
	void create(GJGameLevel*);
	bool init(GJGameLevel*);
	void keyBackClicked();
	void numberInputClosed(NumberInputLayer*);
	void onClose(cocos2d::CCObject*);
	void onCopyable(cocos2d::CCObject*);
	void onEditPassword(cocos2d::CCObject*);
	void onPassword(cocos2d::CCObject*);
	void onUnlisted(cocos2d::CCObject*);
	void updateSettingsState();
}
class SimpleObject : cocos2d::CCObject {
	void create();
	bool init();
}
class SimplePlayer : cocos2d::CCSprite {
	void create(int);
	bool init(int);
	void setColor(cocos2d::ccColor3B const&);
	void setFrames(char const*, char const*, char const*, char const*, char const*);
	void setOpacity(unsigned char);
	void setSecondColor(cocos2d::ccColor3B const&);
	void updateColors();
	void updatePlayerFrame(int, IconType);
}
class SlideInLayer : cocos2d::CCLayerColor {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
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
	bool init();
	void keyBackClicked();
	void layerHidden();
	void layerVisible();
	void registerWithTouchDispatcher();
	void showLayer(bool);
}
class Slider : cocos2d::CCLayer {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
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
	bool init(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float);
	void setBarVisibility(bool);
	void setLiveDragging(bool);
	void setMaxOffset(float);
	void setRotated(bool);
	void setValue(float);
	void updateBar();
}
class SliderThumb : cocos2d::CCMenuItemImage {
	void create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*);
	void getValue();
	bool init(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*);
	void setMaxOffset(float);
	void setRotated(bool);
	void setValue(float);
}
class SliderTouchLogic : cocos2d::CCMenu {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, float);
	bool init(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, float);
	void registerWithTouchDispatcher();
	void setMaxOffset(float);
	void setRotated(bool);
}
class SongCell : TableViewCell {
	void draw();
	bool init();
	void loadFromObject(SongObject*);
	void onClick(cocos2d::CCObject*);
	void updateBGColor(int);
}
class SongInfoLayer : FLAlertLayer {
	void create(int);
	void create(gd::string, gd::string, gd::string, gd::string, gd::string, gd::string);
	bool init(gd::string, gd::string, gd::string, gd::string, gd::string, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void onDownload(cocos2d::CCObject*);
	void onFB(cocos2d::CCObject*);
	void onNG(cocos2d::CCObject*);
	void onYT(cocos2d::CCObject*);
}
class SongInfoObject : cocos2d::CCNode {
	bool canEncode();
	void create(int);
	void create(int, gd::string, gd::string, int, float, gd::string, gd::string, gd::string, int);
	void create(cocos2d::CCDictionary*);
	void createWithCoder(DS_Dictionary*);
	void encodeWithCoder(DS_Dictionary*);
	bool init(int, gd::string, gd::string, int, float, gd::string, gd::string, gd::string, int);
}
class SongObject : cocos2d::CCObject {
	void create(int);
	bool init(int);
}
class SongOptionsLayer : FLAlertLayer {
	void create(LevelSettingsObject*);
	bool init(LevelSettingsObject*);
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
	bool init(float, int);
	void isFinished();
	void step(float);
}
class SpeedObject : cocos2d::CCNode {
	void create(GameObject*, int, float);
	bool init(GameObject*, int, float);
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
	bool initWithOwner(CCAnimatedSprite*, gd::string);
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
	bool initDescription(DS_Dictionary*);
	bool initDescription(cocos2d::CCDictionary*);
}
class SpritePartDelegate {
	cocos2d::CCSpriteFrame* displayFrameChanged(cocos2d::CCObject*, gd::string);
}
class StartPosObject : EffectGameObject {
	void create();
	void getSaveString();
	bool init();
	void setSettings(LevelSettingsObject*);
}
class StatsCell : TableViewCell {
	void draw();
	void getTitleFromKey(char const*);
	bool init();
	void loadFromObject(StatsObject*);
	void updateBGColor(int);
}
class StatsLayer : GJDropDownLayer {
	void create();
	void customSetup();
}
class StatsObject : cocos2d::CCObject {
	void create(char const*, int);
	bool init(char const*, int);
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
}
class TableView : CCScrollLayerExt, CCScrollLayerExtDelegate {
	void cancelAndStoleTouch(cocos2d::CCTouch*, cocos2d::CCEvent*);
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
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
	bool initTableViewCells();
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
}
class TableViewCell : cocos2d::CCLayer {
	void updateVisibility();
}
class TableViewDataSource {
	void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&);
	void numberOfSectionsInTableView(TableView*);
}
class TableViewDelegate {
	void TableViewDidDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*);
	void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*);
	void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*);
	void didEndTweenToIndexPath(CCIndexPath&, TableView*);
	void willTweenToIndexPath(CCIndexPath&, TableViewCell*, TableView*);
}
class TeleportPortalObject : GameObject {
	void addToGroup(int);
	void addToGroup2(int);
	void create(char const*);
	void customObjectSetup(gd::map<gd::string, gd::string>&);
	void getSaveString();
	void getTeleportXOff(cocos2d::CCNode*);
	bool init(char const*);
	void removeFromGroup(int);
	void removeFromGroup2(int);
	void setPosition(cocos2d::CCPoint const&);
	void setPositionOverride(cocos2d::CCPoint);
	void setRotation(float);
	void setRotation2(float);
	void setStartPos(cocos2d::CCPoint);
	void setStartPosOverride(cocos2d::CCPoint);
}
class TextAlertPopup : cocos2d::CCNode {
	void create(gd::string, float, float);
	bool init(gd::string, float, float);
}
class TextArea : cocos2d::CCSprite {
	void colorAllCharactersTo(cocos2d::ccColor3B);
	void create(gd::string, char const*, float, float, cocos2d::CCPoint, float, bool);
	void draw();
	void fadeIn(float, bool);
	void fadeInCharacters(float, float);
	void fadeOut(float);
	void fadeOutAndRemove();
	void finishFade();
	void hideAll();
	bool init(gd::string, char const*, float, float, cocos2d::CCPoint, float, bool);
	void setIgnoreColorCode(bool);
	void setOpacity(unsigned char);
	void setString(gd::string);
	void showAll();
	void stopAllCharacterActions();
}
class TextAreaDelegate {
	void fadeInTextFinished(TextArea*);
}
class TextInputDelegate {
	bool allowTextInput(CCTextInputNode*);
	void textChanged(CCTextInputNode*);
	void textInputClosed(CCTextInputNode*);
	void textInputOpened(CCTextInputNode*);
	void textInputReturn(CCTextInputNode*);
	void textInputShouldOffset(CCTextInputNode*, float);
}
class ToggleTriggerAction : cocos2d::CCNode {
	void create(int, bool);
	void createFromString(gd::string);
	void getSaveString();
	bool init(int, bool);
}
class TopArtistsLayer : FLAlertLayer, OnlineListDelegate {
	void create();
	bool init();
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
}
class TouchToggleAction : cocos2d::CCNode {
	void create(int, bool, TouchTriggerType);
	void createFromString(gd::string);
	void getSaveString();
	bool init(int, bool, TouchTriggerType);
}
class TriggerEffectDelegate {
}
class TutorialLayer : FLAlertLayer {
	void create();
	bool init();
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
	bool init(gd::string);
	void keyBackClicked();
	void registerForCallback(cocos2d::SEL_MenuHandler, cocos2d::CCNode*);
	void show();
}
class UILayer : cocos2d::CCLayerColor {
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	void create();
	void disableMenu();
	void draw();
	void enableMenu();
	bool init();
	void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes);
	void keyUp(cocos2d::enumKeyCodes);
	void onCheck(cocos2d::CCObject*);
	void onDeleteCheck(cocos2d::CCObject*);
	void onPause(cocos2d::CCObject*);
	void pCommand(cocos2d::CCNode*);
	void registerWithTouchDispatcher();
	void toggleCheckpointsMenu(bool);
}
class UndoObject : cocos2d::CCObject {
	void create(GameObject*, UndoCommand);
	void createWithArray(cocos2d::CCArray*, UndoCommand);
	void createWithTransformObjects(cocos2d::CCArray*, UndoCommand);
	bool init(GameObject*, UndoCommand);
	bool init(cocos2d::CCArray*, UndoCommand);
	bool initWithTransformObjects(cocos2d::CCArray*, UndoCommand);
	void setObjects(cocos2d::CCArray*);
}
class UpdateAccountSettingsPopup : FLAlertLayer, GJAccountSettingsDelegate {
	void create(GJAccountSettingsLayer*, int, int, int, gd::string, gd::string, gd::string);
	bool init(GJAccountSettingsLayer*, int, int, int, gd::string, gd::string, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void updateSettingsFailed();
	void updateSettingsFinished();
}
class UploadActionDelegate {
	void uploadActionFailed(int, int);
	void uploadActionFinished(int, int);
}
class UploadActionPopup : FLAlertLayer {
	void closePopup();
	void create(UploadPopupDelegate*, gd::string);
	bool init(UploadPopupDelegate*, gd::string);
	void keyBackClicked();
	void onClose(cocos2d::CCObject*);
	void showFailMessage(gd::string);
	void showSuccessMessage(gd::string);
}
class UploadMessageDelegate {
	void uploadMessageFailed(int);
	void uploadMessageFinished(int);
}
class UploadPopup : FLAlertLayer, LevelUploadDelegate {
	void create(GJGameLevel*);
	bool init(GJGameLevel*);
	void keyBackClicked();
	void levelUploadFailed(GJGameLevel*);
	void levelUploadFinished(GJGameLevel*);
	void onBack(cocos2d::CCObject*);
	void onClose(cocos2d::CCObject*);
	void onReturnToLevel(cocos2d::CCObject*);
	void show();
}
class UploadPopupDelegate {
	void onClosePopup(UploadActionPopup*);
}
class UserInfoDelegate {
	void getUserInfoFailed(int);
	void getUserInfoFinished(GJUserScore*);
	void userInfoChanged(GJUserScore*);
}
class UserListDelegate {
	void forceReloadList(UserListType);
	void getUserListFailed(UserListType, GJErrorCode);
	void getUserListFinished(cocos2d::CCArray*, UserListType);
	void userListChanged(cocos2d::CCArray*, UserListType);
}
class VideoOptionsLayer : FLAlertLayer {
	void create();
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, float, float, bool);
	bool init();
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
	bool init(GJGameLevel*, GJWorldNode*);
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
	bool init(int);
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
}
