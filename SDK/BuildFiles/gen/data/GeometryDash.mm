class AccountHelpLayer : GJDropDownLayer, GJAccountDelegate, FLAlertLayerProtocol {
	create();
	doUnlink();
	exitLayer();
	onAccountManagement(cocos2d::CCObject*);
	onReLogin(cocos2d::CCObject*);
	onUnlink(cocos2d::CCObject*);
	updatePage();
	verifyUnlink();
	virtual void FLAlert_Clicked(FLAlertLayer* ,bool);
	virtual bool accountStatusChanged(void);
	virtual void customSetup(void);
	virtual void layerHidden(void);
}

class AccountLayer : GJDropDownLayer, GJAccountDelegate, GJAccountBackupDelegate, GJAccountSyncDelegate, FLAlertLayerProtocol {
	create();
	createToggleButton(std::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	doBackup();
	doSync();
	exitLayer();
	hideLoadingUI();
	onBackup(cocos2d::CCObject*);
	onHelp(cocos2d::CCObject*);
	onLogin(cocos2d::CCObject*);
	onMore(cocos2d::CCObject*);
	onRegister(cocos2d::CCObject*);
	onSync(cocos2d::CCObject*);
	showLoadingUI();
	toggleUI(bool);
	updatePage(bool);
	virtual void FLAlert_Clicked(FLAlertLayer* ,bool);
	virtual bool accountStatusChanged(void);
	virtual bool backupAccountFailed(BackupAccountError);
	virtual bool backupAccountFinished(void);
	virtual void customSetup(void);
	virtual void layerHidden(void);
	virtual bool syncAccountFailed(BackupAccountError);
	virtual bool syncAccountFinished(void);
}

class AchievementCell {
	void loadFromDict(cocos2d::CCDictionary*) = mac 0x10eaa0;
}

class AchievementManager {
	void getAllAchievements() = mac 0x434d60;
	void sharedState() = mac 0x424420;
}

class AchievementNotifier : cocos2d::CCNode {
	void sharedState() = mac 0x464e00;
	void willSwitchToScene(cocos2d::CCScene*) = mac 0x4650b0;
}

class AchievementsLayer {
	void customSetup() = mac 0x1bdea0;
	void loadPage(int) = mac 0x1be190;
}

class AnimatedGameObject {
	void playAnimation(int) = mac 0xc93d0;
	void updateChildSpriteColor(cocos2d::_ccColor3B) = mac 0xc8450;
}

class AppDelegate : cocos2d::CCObject {
	void bgScale() = mac 0x3aaab0;
	virtual bool applicationDidFinishLaunching() = mac 0x3aa900;
	virtual void applicationDidEnterBackground() = mac 0x3aabe0;
	virtual void applicationWillEnterForeground() = mac 0x3aac80;
	virtual bool applicationWillBecomeActive() = mac 0x3aab30;
	virtual bool applicationWillResignActive() = mac 0x3aab50;
	virtual void trySaveGame() = mac 0x3aaf10;
	virtual void willSwitchToScene(cocos2d::CCScene*) = mac 0x3aaf40;
	static AppDelegate* get() = mac 0x3aab10;

	PAD = mac 0x8, win 0x4, android 0x4;
	cocos2d::CCScene* m_runningScene;
}

class ArtistCell : TableViewCell {
	// ArtistCell(char const*, float, float) = mac 0x11c740;
	void draw() = mac 0x11c980;
	bool init() = mac 0x11c7c0;
	void loadFromObject(SongInfoObject*) = mac 0x1118b0;
	void onNewgrounds(cocos2d::CCObject*) = mac 0x11c7e0;
	void updateBGColor(int) = mac 0x110460;
	void* m_idk;
}

class AudioEffectsLayer {
	void audioStep(float) = mac 0x271f40;
	static AudioEffectsLayer* create(gd::string) = mac 0x271a00;
	void resetAudioVars() = mac 0x271ee0;
}

class BoomListView : cocos2d::CCLayer, TableViewDataSource, TableViewDelegate {
	static BoomListView* create(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x18ecb0;
	bool init(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x18ee00;
	void draw() = mac 0x18f790;
	
	virtual void setupList() = mac 0x18ef90;
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) = mac 0x18f030;
	virtual void cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f070;
	virtual void didSelectRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f090;
	virtual int numberOfRowsInSection(unsigned int, TableView*) = mac 0x18f0b0;
	virtual void numberOfSectionsInTableView(TableView*) = mac 0x18f0e0;
	virtual void cellForRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f100;
	virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&) = mac 0x18f770;
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) = mac 0x18f050;
	virtual TableViewCell* getListCell(char const*) = mac 0x18f200;
	virtual void loadCell(TableViewCell*, int) = mac 0x18f4a0;

	TableView* m_tableView;
	cocos2d::CCArray* m_content;
	BoomListType m_type; 
	float m_width;
	float m_height;
	float m_cellHeight;
	int m_page;
}

class BoomScrollLayer {
	BoomScrollLayer() = mac 0x1e42f0;
}

class ButtonSprite : cocos2d::CCSprite {
	static ButtonSprite* create(char const*) = mac 0x4fa10;
	static ButtonSprite* create(char const*, int, int, float, bool) = mac 0x4fa40;
	void updateBGImage(char const*) = mac 0x502d0;
	static ButtonSprite* create(char const*, float) = mac 0x4fa60;
	static ButtonSprite* create(cocos2d::CCSprite*, int, int, float, float, bool, char const*, bool) = mac 0x4fa90;
}

class CCAnimatedSprite : cocos2d::CCSprite {
	void runAnimation(gd::string) = mac 0x1a6430;
	void tweenToAnimation(gd::string, float) = mac 0x1a65b0;
}

class CCBlockLayer {
	void disableUI() = mac 0x2a5a80;
	void draw() = mac 0x2a5c20;
	void enableUI() = mac 0x2a5a90;
	void enterAnimFinished() = mac 0x2a5bb0;
	void enterLayer() = mac 0x2a5aa0;
	void exitLayer() = mac 0x2a5b40;
	void hideLayer(bool) = mac 0x2a5ba0;
	bool init() = mac 0x2a59c0;
	void layerHidden() = mac 0x2a5be0;
	void layerVisible() = mac 0x2a5bc0;
	void registerWithTouchDispatcher() = mac 0x2a5ad0;
	void showLayer(bool) = mac 0x2a5b90;
}

class CCCircleWave : cocos2d::CCNode {
	static CCCircleWave* create(float, float, float, bool) = mac 0xbd270;
	static CCCircleWave* create(float, float, float, bool, bool) = mac 0xbd290;
	bool init(float, float, float, bool, bool) = mac 0xbd380;
	void followObject(cocos2d::CCNode*, bool) = mac 0xbd670;
	void updatePosition(float) = mac 0xbd630;
	cocos2d::_ccColor3B m_color = mac 0x134;
	CCCircleWaveDelegate* m_delegate = mac 0x150;
}

class CCCircleWaveDelegate {

}

class CCLightFlash {
	static CCLightFlash* create() = mac 0x295870;
	void playEffect(cocos2d::CCPoint, cocos2d::_ccColor3B, float, float, float, float, float, float, float, float, float, float, float, float, float, float, int, bool, bool, float) = mac 0x295900;
}

class CCMenuItemSpriteExtra : cocos2d::CCMenuItemSprite {
	static CCMenuItemSpriteExtra* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1253c0, win 0x18ee0;
	void setSizeMult(float) = mac 0x1255e0, win 0x19080;
	CCMenuItemSpriteExtra() = mac 0x32670;
	bool init(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x125450, win 0x18fa0;
}

class CCMenuItemToggler : cocos2d::CCMenuItem {
    static CCMenuItemToggler* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x38400, win 0x19600, ios 0xf5594;
    void setSizeMult(float) = mac 0x38a40;
    void toggle(bool) = mac 0x38950;

	CCMenuItemSpriteExtra* m_onButton;
	CCMenuItemSpriteExtra* m_offButton;
	bool m_toggled;
	bool m_notClickable;
}

class CCMoveCNode : cocos2d::CCNode {
	static CCMoveCNode* create() = mac 0x1842a0;
	bool init() = mac 0x18b3d0;
	~CCMoveCNode() = mac 0x18b2c0;
}

class CCNodeContainer : cocos2d::CCNode {
	static CCNodeContainer* create() = mac 0xb1090;
	bool init() = mac 0xba950;
	void visit() = mac 0xba960;
}

class CCScrollLayerExt : cocos2d::CCLayer {
	virtual ~CCScrollLayerExt() = mac 0x2359b0;
	virtual void visit() = mac 0x236550;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x235ef0;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x236300;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x236020;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2362a0;
	virtual void registerWithTouchDispatcher() = mac 0x235eb0;
	virtual void preVisitWithClippingRect(cocos2d::CCRect) = mac 0x2366a0;
	virtual void postVisit() = mac 0x236720;

	void moveToTop() = mac 0x235870;
	void moveToTopWithOffset(float) = mac 0x2357d0;
}

class CCScrollLayerExtDelegate {

}

class CCSpritePlus : cocos2d::CCSprite {
	bool initWithSpriteFrameName(char const*) = mac 0x248670;

	cocos2d::CCArray* m_followers;
	CCSpritePlus* m_followingSprite;
	bool m_hasFollower;
	bool m_propagateScaleChanges;
	bool m_propagateFlipChanges;
}

class CCTextInputNode : cocos2d::CCLayer, cocos2d::CCIMEDelegate, cocos2d::CCTextFieldDelegate {
	static CCTextInputNode* create(float, float, char const*, char const*, int, char const*) = mac 0x5cfb0;

	gd::string getString() = mac 0x5d6f0;
	void refreshLabel() = mac 0x5d730;
	void setAllowedChars(gd::string) = mac 0x5d360;
	void setLabelNormalColor(cocos2d::_ccColor3B) = mac 0x5dab0;
	void setLabelPlaceholderColor(cocos2d::_ccColor3B) = mac 0x5da90;
	void setLabelPlaceholderScale(float) = mac 0x5da70;
	void setMaxLabelScale(float) = mac 0x5da30;
	void setMaxLabelWidth(float) = mac 0x5da50;
	void setString(gd::string) = mac 0x5d3e0;
	void updateLabel(gd::string) = mac 0x5d4a0;
	void forceOffset() = mac 0x5ec70;
	
	virtual void registerWithTouchDispatcher() = mac 0x5eec0;

	bool init(float, float, char const*, char const*, int, char const*) = mac 0x5d180;
	virtual ~CCTextInputNode() = mac 0x5ceb0;
	virtual void visit() = mac 0x5d380;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ec80;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ee80;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ee60;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5eea0;
	virtual void textChanged() = mac 0x5dd70;
	virtual void onClickTrackNode(bool) = mac 0x5dd40;
	virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5dad0;
	virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5dc20;
	virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int) = mac 0x5de50;
	virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF*) = mac 0x5e2c0;
	virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF*) = mac 0x5e610;

	void* m_unknown0;
	gd::string m_caption; // mac 0x188
	int m_unknown1;
	bool m_selected; // mac 0x194
	bool m_unknown2;
	gd::string m_allowedChars; // mac 0x198
	float m_maxLabelWidth;
	float m_maxLabelScale;
	float m_placeholderScale;
	cocos2d::ccColor3B m_placeholderColor;
	cocos2d::ccColor3B m_textColor;
	cocos2d::CCLabelBMFont* m_cursor;
	cocos2d::CCTextFieldTTF* m_textField; // mac 0x1c0
	TextInputDelegate* m_delegate; // TextInputDelegate vtable
	int m_maxLabelLength;
	cocos2d::CCLabelBMFont* m_placeholderLabel;
	bool m_unknown3;
	bool m_usePasswordChar;
	bool m_forceOffset;
}

class CheckpointObject {
	static CheckpointObject* create() = mac 0x7e7d0;
	void getObject() = mac 0x7ef50;
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
	cocos2d::_ccColor3B m_color = mac 0x12c;
}

class ColorActionSprite : cocos2d::CCNode {
	float m_opacity;
	cocos2d::_ccColor3B m_f0124;
	cocos2d::_ccColor3B m_activeColor;
}

class ColorChannelSprite {
	void updateBlending(bool) = mac 0x16e1d0;
	void updateCopyLabel(int, bool) = mac 0x16ded0;
	void updateOpacity(float) = mac 0x16e080;
	void updateValues(ColorAction*) = mac 0x16e2e0;
}

class CommentCell : TableViewCell {
	void loadFromComment(GJComment*) = mac 0x111c70;
}

class CountTriggerAction : cocos2d::CCNode {
	static CountTriggerAction* createFromString(gd::string) = mac 0x1754f0;

	int m_previousCount;
	int m_targetCount;
	int m_targetID;
	bool m_activateGroup;
	bool m_multiActivate = mac 0x138;
}

class CreateMenuItem : CCMenuItemSpriteExtra {
	static CreateMenuItem* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1c580;
}

class CreatorLayer : cocos2d::CCLayer {
	void onMyLevels(cocos2d::CCObject*) = mac 0x142b70;
	void onSavedLevels(cocos2d::CCObject*) = mac 0x142860;
}

class CurrencyRewardLayer {
	virtual ~CurrencyRewardLayer() = mac 0x447950;
	virtual void update(float) = mac 0x44a5c0;
}

class CustomListView : cocos2d::CCLayerColor {
	static CustomListView* create(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x10d410;
	void getListCell(char const*) = mac 0x10d560;
	void loadCell(TableViewCell*, int) = mac 0x10e610;
	void setupList() = mac 0x116e70;
}

class CustomSongCell : TableViewCell {
	void loadFromObject(SongInfoObject*) = mac 0x110220;
}

class CustomSongLayer {
	bool init(LevelSettingsObject*) = mac 0xf06f0;
	void onArtists(cocos2d::CCObject*) = mac 0xf1950;
	void onSongBrowser(cocos2d::CCObject*) = mac 0xf18a0;
}

class DialogLayer {
	virtual ~DialogLayer() = mac 0x204720;
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

class EditorOptionsLayer {
	void onButtonsPerRow(cocos2d::CCObject*) = mac 0x147b30;
}

class EditorPauseLayer : FLAlertLayer {
	virtual ~EditorPauseLayer() = mac 0x13c3b0;
	virtual void keyBackClicked() = mac 0x13f320;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x13f3a0;
	virtual void customSetup() = mac 0x13cc00;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x13f1b0;

	static EditorPauseLayer* create(LevelEditorLayer*) = mac 0x13c680;
	void saveLevel() = mac 0x13ebd0;
	bool init(LevelEditorLayer*) = mac 0x13c7a0;
}

class EditorUI : cocos2d::CCLayer {
	void constrainGameLayerPosition() = mac 0x1c6d0;
	void create(LevelEditorLayer*) = mac 0x8a80;
	void deselectAll() = mac 0x1f300;
	void onDeselectAll(cocos2d::CCObject*) = mac 0x19cd0;
	void disableButton(CreateMenuItem*) = mac 0x1c0f0;
	void editButtonUsable() = mac 0x28f30;
	void editObject(cocos2d::CCObject*) = mac 0x195a0;
	void enableButton(CreateMenuItem*) = mac 0x1bff0;
	CCMenuItemSpriteExtra* getCreateBtn(int, int) = mac 0x1f6c0;
	void getGroupCenter(cocos2d::CCArray*, bool) = mac 0x23470;
	cocos2d::CCArray* getSelectedObjects() = mac 0x23f30;
	void init(LevelEditorLayer*) = mac 0x8ae0;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2ed60;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2f3d0;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2fb00;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x30790;
	void moveObject(GameObject*, cocos2d::CCPoint) = mac 0x24b10;
	void onDuplicate(cocos2d::CCObject*) = mac 0x18ba0;
	void pasteObjects(gd::string) = mac 0x232d0;
	void playerTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2ebf0;
	void playtestStopped() = mac 0x24790;
	void redoLastAction(cocos2d::CCObject*) = mac 0xb8e0;
	void replaceGroupID(GameObject*, int, int) = mac 0x27470;
	void scaleChanged(float) = mac 0x25490;
	void scaleObjects(cocos2d::CCArray*, float, cocos2d::CCPoint) = mac 0x252e0;
	void selectObjects(cocos2d::CCArray*, bool) = mac 0x23940;
	void setupCreateMenu() = mac 0xcb50;
	void undoLastAction(cocos2d::CCObject*) = mac 0xb830;
	void updateButtons() = mac 0x1a300;
	void updateObjectInfoLabel() = mac 0x1cb10;
	void updateSlider() = mac 0x18a90;
	void updateZoom(float) = mac 0x248c0;

	LevelEditorLayer* m_editorLayer = mac 0x408;
	cocos2d::CCArray* m_editBars = mac 0x358;
	cocos2d::CCNode* m_locationSlider = mac 0x228;
	GameObject* m_lastSelectedObject = mac 0x440;
	gd::string m_clipboard = mac 0x458;
}

class EffectGameObject : GameObject {
	static EffectGameObject* create(char const*) = mac 0xc9790;
	void getTargetColorIndex() = mac 0xca1f0;
	virtual void triggerObject(GJBaseGameLayer*) = mac 0xc9870;

	int m_targetGroup = mac 0x4F8;
	bool m_activateGroup = mac 0x578;
	bool m_touchHoldMode = mac 0x579;
	int m_animationID = mac 0x584;
	float m_spawnDelay = mac 0x588;
	bool m_multiTrigger = mac 0x594;
	int m_targetCount = mac 0x598;
	int m_compareType = mac 0x5A0;
	int m_itemBlockBID = mac 0x5A8;
	int m_itemBlockID = mac 0x5B0;
}

class EndLevelLayer {
	static EndLevelLayer* create() = mac 0x2787d0;
}

class EndPortalObject : GameObject {
	static EndPortalObject* create() = mac 0x1da8f0;
	void updateColors(cocos2d::_ccColor3B) = mac 0x1dacb0;
}

class FLAlertLayer : cocos2d::CCLayerColor {
    virtual ~FLAlertLayer() =                                               mac 0x25db60;
    virtual void onEnter() =                                                mac 0x25f350;
    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) =       mac 0x25ee40;
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) =       mac 0x25f0a0;
    //virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) =       mac 0x25ef60;
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) =   mac 0x25f020;
    virtual void registerWithTouchDispatcher() =                            mac 0x25f2e0;
    virtual void keyBackClicked() =                                         mac 0x25ed90;
    virtual void keyDown(cocos2d::enumKeyCodes) =                           mac 0x25ece0;
    virtual void show() =                                                   mac 0x25f120, win 0x23560, ios 0x1feff4;

    bool init(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float) =                   mac 0x25e1b0;

    static FLAlertLayer* create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*) =                             , win 0x22680;
    [[mangle('_ZN12FLAlertLayer6createEP20FLAlertLayerProtocolPKcSsS3_S3_f')]]
    static FLAlertLayer* create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float) =              mac 0x25e0e0, win 0x22730, ios 0x1fe374;
    static FLAlertLayer* create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float) = mac 0x25dec0;
    
    inline static FLAlertLayer* create(char const* title, const gd::string &desc, char const* btn) {
        return FLAlertLayer::create(nullptr, title, desc, btn, nullptr, 300.0);
    }

    cocos2d::CCMenu* m_buttonMenu;                      // mac 0x1f8
    int m_controlConnected;                             // mac 0x200
    FLAlertLayerProtocol* m_alertProtocol;              // mac 0x208
    cocos2d::CCNode* m_scene;                           // mac 0x210
    bool m_reverseKeyBack;                              // mac 0x211
    cocos2d::ccColor3B m_color;                         // mac 0x212
    cocos2d::CCLayer* m_mainLayer;                      // mac 0x220
    int m_ZOrder;                                       // mac 0x228
    bool m_noElasticity;                                // mac 0x22c
    cocos2d::ccColor3B m_color2;                        // mac 0x230
    ButtonSprite* m_button1;                            // mac 0x238
    ButtonSprite* m_button2;                            // mac 0x240
    cocos2d::CCLayerColor* m_scrollingLayer;            // mac 0x248
    int m_joystickConnected;                            // mac 0x250
    bool m_containsBorder;                              // mac 0x251
    bool m_noAction;                                    // mac 0x252
}

class FLAlertLayerProtocol {
	inline virtual void FLAlert_Clicked(FLAlertLayer*, bool) {}
}

class GameToolbox {
	[[mangle('_ZN11GameToolbox18createToggleButtonESsMN7cocos2d8CCObjectEFvPS1_EbPNS0_6CCMenuENS0_7CCPointEPNS0_6CCNodeES9_PNS0_7CCArrayE')]]
	static CCMenuItemToggler createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCArray*) = mac 0x0;
	[[mangle('_ZN11GameToolbox18createToggleButtonESsMN7cocos2d8CCObjectEFvPS1_EbPNS0_6CCMenuENS0_7CCPointEPNS0_6CCNodeES9_fffS7_PKcbiPNS0_7CCArrayE')]]
	static CCMenuItemToggler createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCNode*, cocos2d::CCNode*, float, float, float, cocos2d::CCPoint, char const*, bool, int, cocos2d::CCArray*) = mac 0x0;
}

class GJAccountBackupDelegate {
	inline virtual bool backupAccountFailed(BackupAccountError) {
		return false;
	}
	inline virtual bool backupAccountFinished(void) {
		return false;
	}
}
class GJAccountDelegate {
	inline virtual bool accountStatusChanged(void) {
		return false;
	}
}
class GJAccountLoginDelegate {
	inline virtual bool loginAccountFailed(AccountError) {
		return false;
	}
	inline virtual bool loginAccountFinished(int,int) {
		return false;
	}
}

class GJAccountSyncDelegate {
	inline virtual bool syncAccountFailed(BackupAccountError) {
		return false;
	}
	inline virtual bool syncAccountFinished(void) {
		return false;
	}
}

class GJRotationControl : cocos2d::CCLayer {
	virtual bool init() = mac 0x31510, win 0x93f10;
	void updateSliderPosition(cocos2d::CCPoint) = mac 0x316a0, win 0x94020;
}

class GJBaseGameLayer : cocos2d::CCLayer {
	virtual void objectsCollided(int, int) = mac 0xb6d90;
	virtual void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int) = mac 0xb73a0;
	virtual void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*) = mac 0xb7420;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0xba990;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0xba9a0;
	virtual void toggleGroupTriggered(int, bool) = mac 0xb75a0;
	virtual void spawnGroup(int) = mac 0xb7050;
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
	void getCapacityString() = mac 0xb2210;
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
	void pushButton(int, bool) = mac 0xb9920;
	void rectIntersectsCircle(cocos2d::CCRect, cocos2d::CCPoint, float) = mac 0xb6470;
	void refreshCounterLabels() = mac 0xb9fc0;
	void releaseButton(int, bool) = mac 0xb9a00;
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

	GJEffectManager* m_effectManager = mac 0x180;
	cocos2d::CCLayer* m_objectLayer = mac 0x188;
	cocos2d::CCArray* m_objects = mac 0x3a0;
	cocos2d::CCArray* m_sections = mac 0x3a8;
	PlayerObject* m_player1 = mac 0x380;
	PlayerObject* m_player2 = mac 0x388;
	LevelSettingsObject* m_levelSettings = mac 0x390;
	cocos2d::CCDictionary* m_unknownDict = mac 0x398;
}

class GJComment {
	virtual ~GJComment() = mac 0x2dfd70;
	virtual bool init() = mac 0x2dfec0;
}

class GJCommentListLayer : cocos2d::CCLayerColor {
	virtual ~GJCommentListLayer() = mac 0x1482a0;
	static GJCommentListLayer* create(BoomListView*, char const*, cocos2d::_ccColor4B, float, float, bool) = mac 0x147d00;
}

class GJDropDownLayer : cocos2d::CCLayerColor {
	virtual void customSetup() = mac 0x352570;
	virtual void enterLayer() = mac 0x3525c0;
	virtual void exitLayer(cocos2d::CCObject*) = mac 0x352670;
	virtual void showLayer(bool) = mac 0x3526c0;
	virtual void hideLayer(bool) = mac 0x3527b0;
	virtual void layerVisible() = mac 0x3528b0;
	virtual void layerHidden() = mac 0x3528d0;
	virtual void enterAnimFinished() = mac 0x3528a0;
	virtual void disableUI() = mac 0x352580;
	virtual void enableUI() = mac 0x3525a0;
	virtual void draw() = mac 0x352910;
	bool init(char const*, float) = mac 0x352100;
	virtual void registerWithTouchDispatcher() = mac 0x3525f0;
	virtual void keyBackClicked() = mac 0x352630;

	static GJDropDownLayer* create(char const*) = mac 0x352530;

	cocos2d::CCPoint m_endPosition;
	cocos2d::CCPoint m_startPosition;
	cocos2d::CCMenu* m_buttonMenu;
	GJListLayer* m_listLayer;
	bool m_controllerEnabled;
	cocos2d::CCLayer* m_mainLayer;
	bool m_hidden;
	void* m_unknown;
}

class GJEffectManager : cocos2d::CCNode {
	virtual bool init() = mac 0x180230;

	void activeColorForIndex(int) = mac 0x180cb0;
	void activeOpacityForIndex(int) = mac 0x180e10;
	void addAllInheritedColorActions(cocos2d::CCArray*) = mac 0x1817a0;
	void addGroupPulseEffect(PulseEffectAction*) = mac 0x184c10;
	void calculateBaseActiveColors() = mac 0x180f70;
	void calculateInheritedColor(int, ColorAction*) = mac 0x1818f0;
	void calculateLightBGColor(cocos2d::_ccColor3B) = mac 0x185b90;
	void colorActionChanged(ColorAction*) = mac 0x181dc0;
	void colorExists(int) = mac 0x181da0;
	void colorForEffect(cocos2d::_ccColor3B, cocos2d::_ccHSVValue) = mac 0x182650;
	cocos2d::_ccColor3B colorForGroupID(int, cocos2d::_ccColor3B const&, bool) = mac 0x184f90;
	void colorForIndex(int) = mac 0x180ad0;
	void colorForPulseEffect(cocos2d::_ccColor3B const&, PulseEffectAction*) = mac 0x181bf0;
	void countChangedForItem(int) = mac 0x185a40;
	void countForItem(int) = mac 0x185a10;
	static GJEffectManager* create() = mac 0x1800f0;
	void createFollowCommand(float, float, float, int, int, bool, int) = mac 0x182ed0;
	void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int) = mac 0x182cc0;
	void createPlayerFollowCommand(float, float, int, float, float, int, int) = mac 0x182fe0;
	void createRotateCommand(int, float, int, int, int, float, bool, int) = mac 0x182df0;
	void getAllColorActions() = mac 0x180980;
	void getAllColorSprites() = mac 0x1809e0;
	const cocos2d::_ccColor3B& getColorAction(int) = mac 0x180b00;
	const cocos2d::_ccColor3B& getColorSprite(int) = mac 0x180d00;
	void getCurrentStateString() = mac 0x1867e0;
	void getLoadedMoveOffset() = mac 0x184390;
	void getMixedColor(cocos2d::_ccColor3B, cocos2d::_ccColor3B, float) = mac 0x185d30;
	uint8_t getOpacityActionForGroup(int) = mac 0x1845b0;
	void getSaveString() = mac 0x185e90;
	void handleObjectCollision(bool, int, int) = mac 0x1828f0;
	void hasActiveDualTouch() = mac 0x185540;
	void hasBeenTriggered(int) = mac 0x1853b0;
	void hasPulseEffectForGroupID(int) = mac 0x184f60;
	bool isGroupEnabled(int) = mac 0x1853d0;
	void keyForGroupIDColor(int, cocos2d::_ccColor3B const&, bool) = mac 0x184c90;
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
	void runPulseEffect(int, bool, float, float, float, PulseEffectType, cocos2d::_ccColor3B, cocos2d::_ccHSVValue, int, bool, bool, bool, int) = mac 0x184890;
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
	void updateColors(cocos2d::_ccColor3B, cocos2d::_ccColor3B) = mac 0x180a40;
	void updateEffects(float) = mac 0x181df0;
	void updateOpacityAction(OpacityEffectAction*) = mac 0x184780;
	void updateOpacityEffects(float) = mac 0x1823e0;
	void updatePulseEffects(float) = mac 0x182130;
	void updateSpawnTriggers(float) = mac 0x182510;
	void wasFollowing(int, int) = mac 0x185e60;
	void wouldCreateLoop(InheritanceNode*, int) = mac 0x181820;
	~GJEffectManager() = mac 0x17fe00;

	GJBaseGameLayer* m_gameLayer;
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

class GJGameLevel {
	virtual ~GJGameLevel() = mac 0x2db2d0;
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x2dd1c0;
	virtual bool canEncode() = mac 0x2ddae0;
	virtual bool init() = mac 0x2db310;

	static GJGameLevel* create() = mac 0x2b83e0;
	void getAudioFileName() = mac 0x2dbe70;
	void getCoinKey(int) = mac 0x2ce360;
	void getLengthKey(int) = mac 0x2dbba0;
	void getNormalPercent() = mac 0x2b8b20;
	void levelWasAltered() = mac 0x2db530;
	void savePercentage(int, bool, int, int, bool) = mac 0x2db700;

	int m_levelId = mac 0x130;
	gd::string m_name = mac 0x138;
	gd::string m_levelString = mac 0x148;
	gd::string m_author = mac 0x150;
	int m_audioTrack = mac 0x18c;
	int m_songID = mac 0x190;
	int m_difficultyDenominator = mac 0x1ac;
	int m_difficultyNumerator = mac 0x1b0;  
	bool m_lowDetail = mac 0x1c5;
	int m_bestNormal = mac 0x214;
	int m_bestPractice = mac 0x238;
	int m_score = mac 0x248;
	int m_epic = mac 0x24c;
	int m_demon = mac 0x26c;
	int m_stars = mac 0x27c;
	OBB2D* m_hitbox = mac 0x2b0;
	bool m_official = mac 0x324;
}

class GJListLayer : cocos2d::CCLayerColor {
	virtual ~GJListLayer() = mac 0x344350;
}

class GJRobotSprite {
	virtual ~GJRobotSprite() = mac 0x34aaf0;
	virtual bool init() = mac 0x34ad50;
	virtual void setOpacity(unsigned char) = mac 0x34bcc0;
	virtual void hideSecondary() = mac 0x34c3b0;
	
	static GJRobotSprite* create() = mac 0x34ac00;
	void updateColor02(cocos2d::_ccColor3B) = mac 0x34bbd0;
	void updateFrame(int) = mac 0x34bdd0;
}

class GJSpecialColorSelectDelegate {

}

class GJSpiderSprite {
	virtual ~GJSpiderSprite() = mac 0x34c4a0;
	virtual bool init() = mac 0x34c700;

	static GJSpiderSprite* create() = mac 0x34c5b0;
}

class GManager : cocos2d::CCNode {
	inline virtual void setup() {}

	void save() = mac 0x26f300;
	void saveData(DS_Dictionary*, gd::string) = mac 0x26f4b0;
	void saveGMTo(gd::string) = mac 0x26f3b0;

	gd::string m_sFileName;
	bool m_bSetup;
	bool m_bSaved;
}

class GameLevelManager {
	GJGameLevel* createNewLevel() = mac 0x2b8180;
	static GameLevelManager* sharedState() = mac 0x2a8340;
	void getPageInfo(char const*) = mac 0x2c0050;
	cocos2d::CCArray* getStoredOnlineLevels(char const*) = mac 0x2bfe80;
	void getTopArtists(int, int) = mac 0x2ce3d0;
	void getTopArtistsKey(int) = mac 0x2ce7a0;
	void makeTimeStamp(char const*) = mac 0x2bfd90;

	cocos2d::CCDictionary* m_timerDict = mac 0x1e8;
}

class GameManager : cocos2d::CCNode {
    void accountStatusChanged() = mac 0x1cdad0;
    const cocos2d::_ccColor3B& colorForIdx(int) = mac 0x1cbc80, , ios 0x237488;
    void didExitPlayscene() = mac 0x1d0230;
    void doQuickSave() = mac 0x1d0200;
    void fadeInMusic(char const*) = mac 0x1c2ff0;
    void getBGTexture(int) = mac 0x1cca00;
    void getFontFile(int) = mac 0x1cc5f0;
    bool getGameVariable(char const*) = mac 0x1cccd0;
    int getIntGameVariable(char const*) = mac 0x1cd1d0;
    void getUGV(char const*) = mac 0x1ccfa0;
    void loadDeathEffect(int) = mac 0x1cc690;
    void loadFont(int) = mac 0x1cc550;
    void reloadAll(bool, bool, bool) = mac 0x1d08a0;
    void reloadAllStep5() = mac 0x1d0b00;
    void reportPercentageForLevel(int, int, bool) = mac 0x1c5b00;
    void setGameVariable(char const*, bool) = mac 0x1cca80;
    void setIntGameVariable(char const*, int) = mac 0x1cd0e0;
    void setUGV(char const*, bool) = mac 0x1cce50;
    static GameManager* sharedState() = mac 0x1c2b30, , ios 0x2321b8;
    ~GameManager() = mac 0x1d0e00;
    void getGTexture(int) = mac 0x1cca40;
    bool init() = mac 0x1c2ec0;
    void reportAchievementWithID(char const*, int, bool) = mac 0x1c6460;
    void resolutionForKey(int) = mac 0x1d0b40;
    void update(float) = mac 0x1d0270;

    PlayLayer* m_playLayer = mac 0x180;
    LevelEditorLayer* m_editorLayer = mac 0x188;
    int m_scene = mac 0x1f4;
    bool m_ldm = mac 0x2a1;
}

class GameObject : CCSpritePlus {
	virtual ~GameObject() = mac 0x2f4ca0;
	virtual void update(float) = mac 0x2fbb90;
	virtual void setScaleX(float) = mac 0x335b00;
	virtual void setScaleY(float) = mac 0x335b90;
	virtual void setScale(float) = mac 0x335c20;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x335850;
	virtual void setVisible(bool) = mac 0x336280;
	virtual void setRotation(float) = mac 0x335970;
	virtual void setOpacity(unsigned char) = mac 0x335f10;
	virtual bool initWithTexture(cocos2d::CCTexture2D*) = mac 0x2f56a0;
	virtual void setChildColor(cocos2d::_ccColor3B const&) = mac 0x341f20;
	virtual void setFlipX(bool) = mac 0x335a60;
	virtual void setFlipY(bool) = mac 0x335ab0;
	virtual void customSetup() = mac 0x2fbba0;
	virtual void setupCustomSprites() = mac 0x307f60;
	virtual void addMainSpriteToParent(bool) = mac 0x33a5b0;
	virtual void resetObject() = mac 0x2fa620;
	virtual void triggerObject(GJBaseGameLayer*) = mac 0x2fa8f0;
	virtual void activateObject() = mac 0x2faf60;
	virtual void deactivateObject(bool) = mac 0x2fb8f0;
	virtual cocos2d::CCRect* getObjectRect() = mac 0x3352b0;
	virtual cocos2d::CCRect* getObjectRect(float, float) = mac 0x3352d0;
	virtual cocos2d::CCRect* getObjectRect2(float, float) = mac 0x3354e0;
	virtual cocos2d::CCRect* getObjectTextureRect() = mac 0x3355b0;
	virtual void getRealPosition() = mac 0x335750;
	virtual void setStartPos(cocos2d::CCPoint) = mac 0x2fa520;
	virtual void updateStartValues() = mac 0x2fa800;
	virtual void customObjectSetup() = mac 0xdc1a0;
	virtual gd::string getSaveString() = mac 0x33d3d0;
	virtual bool isFlipX() = mac 0x335a40;
	virtual bool isFlipY() = mac 0x335a50;
	virtual void setRScaleX(float) = mac 0x335cb0;
	virtual void setRScaleY(float) = mac 0x335d60;
	virtual void setRScale(float) = mac 0x335e10;
	virtual void getRScaleX() = mac 0x335e50;
	virtual void getRScaleY() = mac 0x335e80;
	virtual void calculateSpawnXPos() = mac 0x336970;
	virtual void triggerActivated(float) = mac 0x336990;
	virtual void powerOnObject() = mac 0x3369a0;
	virtual void powerOffObject() = mac 0x3369c0;
	virtual void setObjectColor(cocos2d::_ccColor3B const&) = mac 0x341c90;
	virtual void setGlowColor(cocos2d::_ccColor3B const&) = mac 0x341ed0;
	virtual void getOrientedBox() = mac 0x342ad0;
	virtual void addToGroup(int) = mac 0x33ad00;
	virtual void removeFromGroup(int) = mac 0x33ada0;
	virtual void spawnXPosition() = mac 0xdc1b0;
	virtual void getObjectRectDirty() const = mac 0xdc1d0;
	virtual void setObjectRectDirty(bool) = mac 0xdc1e0;
	virtual void getOrientedRectDirty() const = mac 0xdc1f0;
	virtual void setOrientedRectDirty(bool) = mac 0xdc200;
	virtual GameObjectType getType() const = mac 0xdc210;
	virtual void setType(GameObjectType) = mac 0xdc220;
	virtual void getStartPos() const = mac 0xdc230;

	void activatedByPlayer(GameObject*) = mac 0x342a20;
	void addColorSprite() = mac 0x2f7fe0;
	void addColorSpriteToParent(bool) = mac 0x2fb470;
	void addToTempOffset(float, float) = mac 0x335700;
	void calculateOrientedBox() = mac 0x342b20;
	void canChangeCustomColor() = mac 0x342db0;
	void colorForMode(int, bool) = mac 0x343460;
	void commonSetup() = mac 0x2f5570;
	void copyGroups(GameObject*) = mac 0x33ae30;
	static GameObject* createWithFrame(char const*) = mac 0x2f5490;
	static GameObject* createWithKey(int) = mac 0x2f4ce0;
	void destroyObject() = mac 0x336a00;
	void determineSlopeDirection() = mac 0x33a9e0;
	void getActiveColorForMode(int, bool) = mac 0x343860;
	void getBallFrame(int) = mac 0x341bf0;
	void getBoxOffset() = mac 0x3353d0;
	const cocos2d::_ccColor3B& getColorIndex() = mac 0x343b90;
	void getDidUpdateLastPosition() = mac 0x343a20;
	int getGroupID(int) = mac 0x33ae10;
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
	static GameObject* objectFromString(gd::string, bool) = mac 0x33b720;
	void playShineEffect() = mac 0x2fa9d0;
	void quickUpdatePosition() = mac 0x335790;
	void removeGlow() = mac 0x2f7f70;
	void resetGroupDisabled() = mac 0x2fa7e0;
	void saveActiveColors() = mac 0x33d250;
	void selectObject(cocos2d::_ccColor3B) = mac 0x341f90;
	void setDefaultMainColorMode(int) = mac 0x304fc0;
	void setDidUpdateLastPosition(bool const&) = mac 0x343a30;
	void setGlowOpacity(unsigned char) = mac 0x336200;
	void setLastPosition(cocos2d::CCPoint const&) = mac 0x3439e0;
	void setMainColorMode(int) = mac 0x342e70;
	void setSectionIdx(int const&) = mac 0x343a10;
	void setupCoinArt() = mac 0x337dd0;
	void slopeFloorTop() = mac 0x342800;
	void slopeWallLeft() = mac 0x3427e0;
	void updateCustomScale(float) = mac 0x335eb0;
	void updateMainColor() = mac 0x343340;
	void updateOrientedBox() = mac 0x342b50;
	void updateSecondaryColor() = mac 0x343740;
	void updateStartPos() = mac 0x2fa590;
	void updateState() = mac 0x3369e0;
	void updateSyncedAnimation(float) = mac 0x337f00;

	// float m_float250;
 //    float m_float254;
 //    float m_float258;
 //    float m_float25c;
 //    bool m_bool260;
 //    float m_animationSpeed;
 //    bool m_isAnimatedCircle;
 //    bool m_randomizeAnimationStart;
 //    float m_animationSpeed2; //what
 //    bool m_black;
 //    bool m_showSelection;
 //    bool m_blackOpacity;
 //    bool m_bool278;
 //    bool m_inEditorMode;
 //    bool m_toggledOff;
 //    bool m_colorOnTop;
 //    int m_int280;
 //    float m_float284;
 //    float m_relativeMovePos;
 //    float m_relativeRotation;
 //    bool m_isTrigger;
 //    bool m_flippedX;
 //    bool m_flippedY;
 //    cocos2d::CCSize m_origHitboxOffset;
 //    char m_unused0;
 //    cocos2d::CCSize m_adjustedHitboxOffset;
 //    OBB2D* m_orientedBox;
 //    bool m_useOrientedBox;
 //    cocos2d::CCSprite* m_glowSprite;
 //    bool m_inPlaylayer;
 //    cocos2d::CCAction* m_myAction; // rob named it not me
 //    bool m_dontRunAction;
 //    bool m_runActionOnObject;
 //    bool m_poweredOn;
 //    cocos2d::CCSize m_objectSize;
 //    bool m_modifierBlock;
 //    bool m_active;
 //    bool m_animationFinished;
 //    cocos2d::CCParticleSystemQuad* m_particleSystem;
 //    gd::string m_effectPlist;
 //    bool m_particleAdded;
 //    bool m_hasParticles;
 //    bool m_customRing;
 //    bool m_portalPosition;
 //    bool m_pulseWithMusic;
 //    cocos2d::CCRect m_textureRect;
 //    bool m_textureRectDirty;
 //    float m_textureRectCenterX;
 //    cocos2d::CCRect m_objectRect;
 //    bool m_objectRectDirty;
 //    bool m_orientdBoxDirty;
 //    bool m_activated;
 //    bool m_activatedByP2;
 //    bool m_hasAnimatedPart;
 //    int m_linkedGroup;
 //    bool m_isSpinning;
 //    float m_rotationSpeed;
 //    bool m_disableRotation;
 //    cocos2d::CCSprite* m_animatedPart;
 //    bool m_unknown358;
 //    float m_hitboxScale;
 //    bool m_onSide;
 //    cocos2d::CCSize m_unknown364;
 //    int m_uuid;
 //    GameObjectType m_objectType;
 //    int m_section;
 //    bool m_touchTriggered;
 //    bool m_spawnTriggered;
 //    cocos2d::CCPoint m_startPosition;
 //    gd::string m_textureName; //mac 0x388


	// mac 0x250
	bool m_unk3;
	bool m_isBlueMaybe;
	float m_unk2;
	float m_unk;
	float m_unk5;
	// mac 0x260
	float m_unk4;
	bool m_unk6;
	float m_animSpeed2;
	bool m_isEffectObject;
	bool m_randomisedAnimStart;
	float m_animSpeed;
	// mac 0x270
	bool m_blackChild;
	bool m_unkOutlineMaybe;
	float m_blackChildOpacity;
	bool m_unk8;
	bool m_inEditLayer;
	bool m_groupDisabled;
	bool m_colourOnTop;
	// mac 0x27c
	int m_mainColorID;
	int m_secondaryColorID;

	// mac 0x284
	bool m_col1;
	bool m_col2;

	float m_tempXOffset;
	float m_tempYOffset;
	// mac 0x290
	float m_unkRotationField;
	bool m_changeXOffset;
	bool m_isFlippedX;
	bool m_isFlippedY;
	// mac 0x298
	cocos2d::CCPoint m_boxOffset; 
	bool m_isOriented;
	cocos2d::CCPoint m_boxOffset2;
	// mac 0x2b0
	OBB2D* m_hitbox;
	bool m_oriented;
	// mac 0x2c0
	cocos2d::CCSprite* m_glowSprite;
	bool m_notEditor;
	// mac 0x2d0
	cocos2d::CCAction* m_myAction;
	bool m_unk7;
	bool m_runActionWithTag;
	bool m_objectPoweredOn;
	// mac 0x2e0
	cocos2d::CCSize m_objectSize;
	// mac 0x2f0
	bool m_trigger;	// mac 0x270
	bool m_active; 	// mac 0x271
	bool m_animationFinished;	// mac 0x272
	cocos2d::CCParticleSystemQuad* m_particleSystem;	// mac 0x274
	std::string m_effectPlistName; // mac 0x278
	bool m_particleAdded;	// mac 0x290
	bool m_hasParticles;	// mac 0x291
	bool m_unkCustomRing;	// mac 0x292
	cocos2d::CCPoint m_portalPosition;	// mac 0x294
	bool m_unkParticleSystem;	// mac 0x29c
	cocos2d::CCRect m_objectTextureRect;	// mac 0x2a0
	bool m_textureRectDirty;	// mac 0x2b0
	float m_rectXCenterMaybe;	// mac 0x2b4
	cocos2d::CCRect m_objectRect2; //mac 0x2B8
	bool m_isObjectRectDirty; //mac 0x2C8
	bool m_isOrientedRectDirty; //mac 0x2C9
	bool m_hasBeenActivated; //mac 0x2CA
	bool m_hasBeenActivatedP2; //mac 0x2CB

	int m_type = mac 0x370;
	int m_id = mac 0x3c4;
	cocos2d::CCPoint m_startPos = mac 0x37c;
	bool m_touchTriggered = mac 0x378;
	bool m_spawnTriggered = mac 0x379;
	int m_uuid = mac 0x36c;
	int m_colorID = mac 0x3bc;
	int m_zOrder = mac 0x42c;
	int m_unknownType = mac 0x3d4;
	int m_coinID = mac 0x3e8;
	float m_scale = mac 0x3c0;
	float m_multiScaleMultiplier = mac 0x44c;
}

class GameObjectCopy {
	virtual ~GameObjectCopy() = mac 0xa3290;

	static GameObjectCopy* create(GameObject*) = mac 0x975a0;
	void resetObject() = mac 0x976a0;
}

class GameSoundManager {
	void disableMetering() = mac 0x362d80;
	void enableMetering() = mac 0x362d00;
	void getMeteringValue() = mac 0x362db0;
	void playBackgroundMusic(gd::string, bool, bool) = mac 0x362070;
	void playEffect(gd::string, float, float, float) = mac 0x3623d0;
	void sharedManager() = mac 0x3610f0;
	void stopBackgroundMusic() = mac 0x362130;
	~GameSoundManager() = mac 0x362c00;
}

class GameStatsManager {
	void awardCurrencyForLevel(GJGameLevel*) = mac 0x43600;
	void awardDiamondsForLevel(GJGameLevel*) = mac 0x43c60;
	void awardSecretKey() = mac 0x4b1e0;
	void getSecretCoinKey(char const*) = mac 0x429f0;
	void getStat(char const*) = mac 0x3d310;
	void hasPendingUserCoin(char const*) = mac 0x42730;
	void hasSecretCoin(char const*) = mac 0x40730;
	void hasUserCoin(char const*) = mac 0x427e0;
	void incrementStat(char const*) = mac 0x3d6d0;
	void incrementStat(char const*, int) = mac 0x3d6e0;
	void sharedState() = mac 0x38f20;
	void storePendingUserCoin(char const*) = mac 0x42940;
	void storeSecretCoin(char const*) = mac 0x42a10;
	void storeUserCoin(char const*) = mac 0x42890;
}

class GJGarageLayer {
	void onPlayerColor1(cocos2d::CCObject*) = mac 0x1ba640, , ios 0x22531c;
	void onPlayerColor2(cocos2d::CCObject*) = mac 0x1ba8c0, , ios 0x225408;
}

class HardStreak : cocos2d::CCDrawNode {
	virtual ~HardStreak() = mac 0x5bf00;
	virtual bool init() = mac 0x5c090;
	
	void addPoint(cocos2d::CCPoint) = mac 0x5c950;
	void reset() = mac 0x5c930;
	void resumeStroke() = mac 0x5c210;
	void stopStroke() = mac 0x5c8f0;

	cocos2d::CCArray* m_pointsArr; // mac 0x0160
	cocos2d::CCPoint m_currentPoint; // mac 0x0168
	float m_waveSize; // mac 0x0170
	float m_pulseSize; // mac 0x0174
	bool m_isSolid; // mac 0x0178
}

class InheritanceNode : cocos2d::CCObject {

}

class LabelGameObject : GameObject {
	bool init() = mac 0x2f5520;
	void setObjectColor(cocos2d::_ccColor3B const&) = mac 0xdbca0;
}

class LevelEditorLayer : GJBaseGameLayer {
	virtual ~LevelEditorLayer() = mac 0x90e80;
	virtual void update(float) = mac 0xa1b70;
	virtual void draw() = mac 0xa2a70;
	virtual void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*) = mac 0x9c200;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0xa04e0;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0x9c590;
	virtual void addToGroup(GameObject*, int, bool) = mac 0x9dab0;
	virtual void removeFromGroup(GameObject*, int) = mac 0x9db60;
	virtual void timeForXPos(float) = mac 0x9c7d0;
	virtual void xPosForTime(float) = mac 0x9c800;
	virtual void levelSettingsUpdated() = mac 0x93f30;

	void activateTriggerEffect(EffectGameObject*, float, float, float) = mac 0x9b520;
	GameObject* addObjectFromString(gd::string) = mac 0x94640;
	void addSpecial(GameObject*) = mac 0x94f30;
	void addToRedoList(UndoObject*) = mac 0x96f80;
	void addToUndoList(UndoObject*, bool) = mac 0x94e20;
	void animateInDualGround(GameObject*, float, bool) = mac 0xa2780;
	void checkCollisions(PlayerObject*, float) = mac 0x9e620;
	static LevelEditorLayer* create(GJGameLevel*) = mac 0x90fb0;
	void createBackground() = mac 0x929f0;
	void createGroundLayer() = mac 0x92840;
	GameObject* createObject(int, cocos2d::CCPoint, bool) = mac 0x957c0;
	void createObjectsFromSetup(gd::string) = mac 0x92230;
	void createObjectsFromString(gd::string, bool) = mac 0x94730;
	void getLastObjectX() = mac 0x9c860;
	void getLevelString() = mac 0x97790;
	void getNextColorChannel() = mac 0x9a610;
	void getNextFreeBlockID(cocos2d::CCArray*) = mac 0x9a4e0;
	int getNextFreeGroupID(cocos2d::CCArray*) = mac 0x9a1b0;
	void getNextFreeItemID(cocos2d::CCArray*) = mac 0x9a390;
	void getObjectRect(GameObject*, bool) = mac 0x96240;
	void getRelativeOffset(GameObject*) = mac 0x96840;
	void handleAction(bool, cocos2d::CCArray*) = mac 0x97020;
	bool init(GJGameLevel*) = mac 0x91010;
	void objectAtPosition(cocos2d::CCPoint) = mac 0x960c0;
	void objectMoved(GameObject*) = mac 0x999f0;
	void objectsInRect(cocos2d::CCRect, bool) = mac 0x95e60;
	void onPlaytest() = mac 0xa06b0;
	void onStopPlaytest() = mac 0xa1780;
	void playMusic() = mac 0xa13c0;
	void recreateGroups() = mac 0x9dbf0;
	void redoLastAction() = mac 0x97750;
	void removeAllObjects() = mac 0x93d80;
	void removeAllObjectsOfType(int) = mac 0x96d40;
	void removeObject(GameObject*, bool) = mac 0x96890;
	void removeSpecial(GameObject*) = mac 0x969c0;
	void resetMovingObjects() = mac 0x9ddc0;
	void resetObjectVector() = mac 0x9c4b0;
	void resetToggledGroups() = mac 0x9aa70;
	void resetToggledGroupsAndObjects() = mac 0x9c3c0;
	void resetUnusedColorChannels() = mac 0x9a870;
	void rotationForSlopeNearObject(GameObject*) = mac 0x95cd0;
	void runColorEffect(EffectGameObject*, int, float, float, bool) = mac 0x9bd30;
	void scene(GJGameLevel*) = mac 0x90f20;
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
	void updateEditorMode() = mac 0x93b50;
	void updateGameObjectsNew() = mac 0x9adc0;
	void updateGround(float) = mac 0x93a60;
	void updateGroundWidth() = mac 0x92af0;
	void updateOptions() = mac 0x91ed0;
	void updateToggledGroups() = mac 0x9bb10;
	void updateVisibility(float) = mac 0x92c70;

	cocos2d::CCArray* m_objects = mac 0x3a0;
	EditorUI* m_editorUI = mac 0x5d8;
}

class LevelSettingsObject : cocos2d::CCNode {
	virtual ~LevelSettingsObject() = mac 0xa5650;
	virtual bool init() = mac 0xa5690;

	static LevelSettingsObject* create() = mac 0x92760;
	void objectFromDict(cocos2d::CCDictionary*) = mac 0xa5810;
	static LevelSettingsObject* objectFromString(gd::string) = mac 0x945a0;
	void setupColorsFromLegacyMode(cocos2d::CCDictionary*) = mac 0xa6a30;

	GJGameLevel* m_level = mac 0x150;
	GJEffectManager* m_effectManager = mac 0x120;
	int m_fontType = mac 0x144;
	bool m_is2Player = mac 0x132;
}

class LocalLevelManager {
	static LocalLevelManager* sharedState() = mac 0x35dd60;
}

class MenuGameLayer {
	void resetPlayer() = mac 0x28fdc0;
	void update(float) = mac 0x28fa70;
}

class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol {
	virtual ~MenuLayer() =                                  mac 0x1d1230;
	virtual bool init() =                                   mac 0x1d14b0, win 0x1907b0;
	virtual void keyBackClicked() =                         mac 0x1d3160, win 0x191f90;
	virtual void keyDown(cocos2d::enumKeyCodes) =           mac 0x1d33d0, win 0x1922c0;
	virtual void googlePlaySignedIn() =                     mac 0x1d2f30;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) =     mac 0x1d3190;
}

class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol {
	onGarage(cocos2d::CCObject*);
    void onMoreGames(cocos2d::CCObject*) =                  mac 0x1d2ad0, win 0x1919c0, ios 0x19fbb4;
    void onQuit(cocos2d::CCObject*) =                       mac 0x1d2b40;
    static cocos2d::CCScene* scene(bool) =                  mac 0x1d12d0, win 0x190720;
}

class MoreVideoOptionsLayer : FLAlertLayer {
	static MoreVideoOptionsLayer* create() = mac 0x443c10;
	bool init() = mac 0x444150;
}

class MusicDownloadManager {
	void incrementPriorityForSong(int) = mac 0x2ef750;
	static MusicDownloadManager* sharedState() = mac 0x2ee4c0;
}

class OBB2D : cocos2d::CCNode  {
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

class ObjectToolbox {
	bool init() = mac 0x3b2d80;
	char const* intKeyToFrame(int) = mac 0x4173b0;
	static ObjectToolbox* sharedState() = mac 0x3b2bc0;

	cocos2d::CCDictionary* m_strKeyObjects = mac 0x120;
	cocos2d::CCDictionary* m_intKeyObjects = mac 0x128;
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

class PauseLayer : FLAlertLayer {
	static PauseLayer* create(bool) = mac 0x20b1e0;
	void onEdit(cocos2d::CCObject*) = mac 0x20c630;
}

class PlatformToolbox {
	void hideCursor() = mac 0x27c340;
	void showCursor() = mac 0x27c360;
}

class PlayLayer : GJBaseGameLayer, CCCircleWaveDelegate {
	void addCircle(CCCircleWave*) = mac 0x7e0f0;
	void addObject(GameObject*) = mac 0x70e50;
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
	void colorObject(int, cocos2d::_ccColor3B) = mac 0x77810;
	void commitJumps() = mac 0x737e0;
	static PlayLayer* create(GJGameLevel*) = mac 0x6b590;
	void createCheckpoint() = mac 0x7e470;
	void createObjectsFromSetup(gd::string) = mac 0x6d130;
	void createParticle(int, char const*, int, cocos2d::tCCPositionType) = mac 0x76800;
	void currencyWillExit(CurrencyRewardLayer*) = mac 0x7e070;
	void delayedResetLevel() = mac 0x7e050;
	void destroyPlayer(PlayerObject*, GameObject*) = mac 0x7ab80;
	void dialogClosed(DialogLayer*) = mac 0x7e0b0;
	virtual void draw() = mac 0x7d160;
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
	bool init(GJGameLevel*) = mac 0x6b5f0;
	void isFlipping() = mac 0x76100;
	void levelComplete() = mac 0x72b80;
	void lightningFlash(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::_ccColor3B, float, float, int, bool, float) = mac 0x75cc0;
	void lightningFlash(cocos2d::CCPoint, cocos2d::_ccColor3B) = mac 0x75bf0;
	void loadDefaultColors() = mac 0x6ef30;
	void loadFromCheckpoint(CheckpointObject*) = mac 0x7f000;
	void loadLastCheckpoint() = mac 0x7efc0;
	void loadSavedObjectsState(gd::string) = mac 0x7f3d0;
	void markCheckpoint() = mac 0x7ef60;
	void moveCameraToPos(cocos2d::CCPoint) = mac 0x7c980;
	virtual void onEnterTransitionDidFinish() = mac 0x806e0;
	virtual void onExit() = mac 0x80710;
	void onQuit() = mac 0x72710;
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
	void removeLastCheckpoint() = mac 0x7f870;
	void removePlayer2() = mac 0x7d630;
	void resetLevel() = mac 0x71c50;
	void resume() = mac 0x80480;
	void resumeAndRestart() = mac 0x80400;
	void saveRecordAction(bool, PlayerObject*) = mac 0x78750;
	void scene(GJGameLevel*) = mac 0x6b500;
	void setupLevelStart(LevelSettingsObject*) = mac 0x6f560;
	void setupReplay(gd::string) = mac 0x7e1e0;
	void shakeCamera(float, float, float) = mac 0x744a0;
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
	void timeForXPos2(float, bool) = mac 0x293eb0;
	void toggleBGEffectVisibility(bool) = mac 0x7fe80;
	void toggleDualMode(GameObject*, bool, PlayerObject*, bool) = mac 0x7bf90;
	void toggleFlipped(bool, bool) = mac 0x7bdc0;
	void toggleGhostEffect(int) = mac 0x7fe40;
	void toggleGlitter(bool) = mac 0x70e00;
	void togglePracticeMode(bool) = mac 0x7f9e0;
	void toggleProgressbar() = mac 0x6eeb0;
	void tryStartRecord() = mac 0x7fe00;
	void unclaimParticle(char const*, cocos2d::CCParticleSystemQuad*) = mac 0x76e00;
	void unregisterActiveObject(GameObject*) = mac 0x77660;
	void unregisterStateObject(GameObject*) = mac 0x777f0;
	virtual void update(float) = mac 0x77900;
	void updateAttempts() = mac 0x7fcd0;
	void updateCamera(float) = mac 0x6e2b0;
	virtual void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*) = mac 0x7c7f0;
	void updateDualGround(PlayerObject*, int, bool) = mac 0x7caa0;
	void updateEffectPositions() = mac 0x7a6d0;
	void updateLevelColors() = mac 0x6f1e0;
	void updateMoveObjectsLastPosition() = mac 0x7a720;
	void updateProgressbar() = mac 0x6ed70;
	void updateReplay(float) = mac 0x78b60;
	void updateTimeMod(float, bool) = mac 0x786f0;
	virtual void updateTweenAction(float, char const*) = mac 0x7ffb0;
	void updateVisibility() = mac 0x6fb90;
	void vfDChk() = mac 0x7fcb0;
	virtual void visit() = mac 0x75ef0;
	void visitWithColorFlash() = mac 0x761f0;
	void willSwitchToMode(int, PlayerObject*) = mac 0x7b9e0;
	void xPosForTime(float) = mac 0x7d140;
	~PlayLayer() = mac 0x6b090;

	bool m_gameStarted = mac 0x4dc;
	EndPortalObject* m_endPortal = mac 0x530;
	float m_length = mac 0x5f8;
	float m_trueLength = mac 0x5fc;
	UILayer* m_uiLayer = mac 0x720;
	GJGameLevel* m_level = mac 0x728;
	int m_attempt = mac 0x754;
	bool m_testMode = mac 0x738;
	bool m_practiceMode = mac 0x739;
	float m_time = mac 0x760;
	bool m_cube = mac 0x76f;
	bool m_ship = mac 0x770;
	bool m_ufo = mac 0x771;
	bool m_ball = mac 0x772;
	bool m_wave = mac 0x773;
	bool m_robot = mac 0x774;
	bool m_spider = mac 0x775;
}

class PlayerCheckpoint {
	virtual ~PlayerCheckpoint() = mac 0x80760;
	virtual bool init() = mac 0x807a0;

	static PlayerCheckpoint* create() = mac 0x7e8c0;
}

class PlayerObject : GameObject {
	virtual ~PlayerObject() = mac 0x217220;
	virtual void update(float) = mac 0x218bf0;
	virtual void setScaleX(float) = mac 0x22e7f0;
	virtual void setScaleY(float) = mac 0x22e830;
	virtual void setScale(float) = mac 0x22e870;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x22c8b0;
	virtual void setVisible(bool) = mac 0x22e8b0;
	virtual void setRotation(float) = mac 0x22e6e0;
	virtual void setOpacity(unsigned char) = mac 0x22d400;
	virtual void setColor(cocos2d::_ccColor3B const&) = mac 0x22cdf0;
	virtual void setFlipX(bool) = mac 0x22e720;
	virtual void setFlipY(bool) = mac 0x22e7b0;
	virtual void resetObject() = mac 0x223170;
	virtual void getRealPosition() = mac 0x22d5f0;
	virtual void getOrientedBox() = mac 0x22dee0;
	virtual void animationFinished(char const*) = mac 0x22e9d0;

	void activateStreak() = mac 0x21aef0;
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
	void fadeOutStreak2(float) = mac 0x225890;
	void flashPlayer(float, float, cocos2d::_ccColor3B, cocos2d::_ccColor3B) = mac 0x221c80;
	void flipGravity(bool, bool) = mac 0x21c090;
	void flipMod() = mac 0x21a4c0;
	void getActiveMode() = mac 0x22b950;
	void getModifiedSlopeYVel() = mac 0x21bff0;
	void getOldPosition(float) = mac 0x21a830;
	void getSecondColor() = mac 0x22cee0;
	void gravityDown() = mac 0x22e930;
	void gravityUp() = mac 0x22e900;
	void hardFlipGravity() = mac 0x22b860;
	void hitGround(bool) = mac 0x220a30;
	void incrementJumps() = mac 0x21c050;
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
	void playBurstEffect() = mac 0x21c780;
	void playDeathEffect() = mac 0x225930;
	void playDynamicSpiderRun() = mac 0x222ec0;
	void playerDestroyed(bool) = mac 0x2256d0;
	void playerIsFalling() = mac 0x21c730;
	void playerTeleported() = mac 0x22b840;
	void playingEndEffect() = mac 0x22d7e0;
	void postCollision(float) = mac 0x21cd10;
	void preCollision() = mac 0x21ccc0;
	void preSlopeCollision(float, GameObject*) = mac 0x21ec80;
	void propellPlayer(float) = mac 0x22d8e0;
	void pushButton(PlayerButton) = mac 0x22aa00;
	void pushDown() = mac 0x22dbd0;
	void pushPlayer(float) = mac 0x22dbb0;
	void releaseButton(PlayerButton) = mac 0x22b6f0;
	void removeAllParticles() = mac 0x218ac0;
	void removePendingCheckpoint() = mac 0x2237b0;
	void resetAllParticles() = mac 0x21adb0;
	void resetCollisionLog() = mac 0x21cc20;
	void resetPlayerIcon() = mac 0x22be00;
	void resetStateVariables() = mac 0x223760;
	void resetStreak() = mac 0x21ae10;
	void ringJump(GameObject*) = mac 0x22abf0;
	void runBallRotation(float) = mac 0x21ca10;
	void runBallRotation2() = mac 0x21cb10;
	void runNormalRotation() = mac 0x21c960;
	void runRotateAction(bool) = mac 0x21c570;
	void saveToCheckpoint(PlayerCheckpoint*) = mac 0x22e2f0;
	void setSecondColor(cocos2d::_ccColor3B const&) = mac 0x219610;
	void setupStreak() = mac 0x218720;
	void spawnCircle() = mac 0x225480;
	void spawnCircle2() = mac 0x2252a0;
	void spawnDualCircle() = mac 0x2255c0;
	void spawnFromPlayer(PlayerObject*) = mac 0x22dde0;
	void spawnPortalCircle(cocos2d::_ccColor3B, float) = mac 0x225350;
	void spawnScaleCircle() = mac 0x2251b0;
	void specialGroundHit() = mac 0x22dbf0;
	void speedDown() = mac 0x22e970;
	void speedUp() = mac 0x22e950;
	void spiderTestJump(bool) = mac 0x21b160;
	void startDashing(GameObject*) = mac 0x221d70;
	void stopBurstEffect() = mac 0x22c680;
	void stopDashing() = mac 0x222990;
	void stopRotation(bool) = mac 0x21c830;
	void storeCollision(bool, int) = mac 0x21cc60;
	void switchedToMode(GameObjectType) = mac 0x22b9a0;
	void testForMoving(float, GameObject*) = mac 0x21eb70;
	void toggleBirdMode(bool) = mac 0x224070;
	void toggleDartMode(bool) = mac 0x2243f0;
	void toggleFlyMode(bool) = mac 0x223820;
	void toggleGhostEffect(GhostType) = mac 0x225000;
	void togglePlayerScale(bool) = mac 0x224bd0;
	void toggleRobotMode(bool) = mac 0x223c70;
	void toggleRollMode(bool) = mac 0x223b20;
	void toggleSpiderMode(bool) = mac 0x224830;
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
	void updateTimeMod(float) = mac 0x2185e0;
	void usingWallLimitedMode() = mac 0x22df00;
	void yStartDown() = mac 0x22e9b0;
	void yStartUp() = mac 0x22e990;

	HardStreak* m_waveStreak = mac 0x600;
	double m_speed = mac 0x608;
	double m_gravity = mac 0x618;
	bool m_inPlayLayer = mac 0x62c;
	GJRobotSprite* m_robotSprite = mac 0x6a8;
	GJSpiderSprite* m_spiderSprite = mac 0x6b0;
	bool m_isHolding = mac 0x745;
	bool m_hasJustHeld = mac 0x746;
	double m_yAccel = mac 0x760;
	bool m_isShip = mac 0x770;
	bool m_isBird = mac 0x771;
	bool m_isBall = mac 0x772;
	bool m_isDart = mac 0x773;
	bool m_isRobot = mac 0x774;
	bool m_isSpider = mac 0x775;
	bool m_upsideDown = mac 0x776;
	bool m_dead = mac 0x777;
	bool m_onGround = mac 0x778;
	float m_vehicleSize = mac 0x77c;
	cocos2d::CCPoint m_lastPortalLocation = mac 0x78c;
	bool m_isSliding = mac 0x7a0;
	bool m_isRising = mac 0x7a1;
	cocos2d::CCPoint m_lastHitGround = mac 0x7a4;
	GameObject* m_lastPortal = mac 0x7b8;
	cocos2d::_ccColor3B m_pCol1 = mac 0x7c2;
	cocos2d::_ccColor3B m_pCol2 = mac 0x7c5;
	float m_xPos = mac 0x7c8;
	float m_yPos = mac 0x7cc;
}

class PulseEffectAction : cocos2d::CCNode {
	static PulseEffectAction* createFromString(gd::string) = mac 0x179e90;
	void getSaveString() = mac 0x17a850;
	int m_group = mac 0x130;
}

class RetryLevelLayer {
	static RetryLevelLayer* create() = mac 0x28dd60;
}

class ScrollingLayer : cocos2d::CCLayerColor {
	static ScrollingLayer* create(cocos2d::CCSize, cocos2d::CCPoint, float) = mac 0x41a900;
}

class SetGroupIDLayer {
	void onNextGroupID1(cocos2d::CCObject*) = mac 0x1967a0;
	void textChanged(CCTextInputNode*) = mac 0x197af0;
	void updateGroupIDLabel() = mac 0x197260;
	~SetGroupIDLayer() = mac 0x194410;
}

class SetIDLayer {
	static SetIDLayer* create(GameObject*) = mac 0x168f20;
}

class SetIDPopup {
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

	EffectGameObject* object = mac 0x258;
	CCTextInputNode* numInput = mac 0x290;
}

class SetupTouchTogglePopup : FLAlertLayer {
	static SetupTouchTogglePopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x1576a0;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x158b60;
	void textChanged(CCTextInputNode*) = mac 0x1596a0;
	void updateTargetID() = mac 0x159480;
}

class SimplePlayer {
	static SimplePlayer* create(int) = mac 0x1b6140;
	void setSecondColor(cocos2d::_ccColor3B const&) = mac 0x1bace0;
	void updateColors() = mac 0x1ba1f0;
	void updatePlayerFrame(int, IconType) = mac 0x1b62f0;
}

class Slider : cocos2d::CCLayer {
	static Slider* create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, char const*, char const*, char const*, char const*, float) = mac 0x18dd80;
	static Slider* create(cocos2d::CCNode*, cocos2d::SEL_MenuHandler, float) = mac 0x18dc40;
	void getValue() = mac 0x18e0c0;
	void setBarVisibility(bool) = mac 0x18e280;
	void setValue(float) = mac 0x18e170;
}

class SliderThumb {
	void getValue() = mac 0x18ce80;
}

class SongInfoObject : cocos2d::CCNode {
	virtual ~SongInfoObject() = mac 0x2f2040;
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x2f2c70;
	virtual bool canEncode() = mac 0x2f2da0;

	int m_unknown0;
	gd::string m_unknown1;
	gd::string m_artist;
	gd::string m_unknown2;
	gd::string m_youtube;
	gd::string m_unknown3;
	int m_unknown4;
	float m_unknown5;
	unsigned short m_pad;
	unsigned short m_metadata;
	int m_uuid;
}

class SpawnTriggerAction : cocos2d::CCNode {
	static SpawnTriggerAction* createFromString(gd::string) = mac 0x17bf50;
	bool m_timerEnded;
	float m_delay;
	float m_timer;
	int m_group;
	int m_uuid;
}

class TableView : CCScrollLayerExt, CCScrollLayerExtDelegate {
	static TableView* create(TableViewDelegate*, TableViewDataSource*, cocos2d::CCRect) = mac 0x37eb30;
	void reloadData() = mac 0x37f970;

	float m_unknown = mac 0x1c8;
}

class TableViewCell : cocos2d::CCLayer {
	TableViewCell(char const*, float, float) = mac 0x383de0;
	// default constructor
	inline TableViewCell() {}
	PAD = mac 0x50;
	float m_parentHeight;
	float m_height;
	cocos2d::CCLayerColor* m_backgroundLayer;
	cocos2d::CCLayer* m_mainLayer;
}

class TableViewDataSource {
	inline virtual void willTweenToIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	inline virtual void didEndTweenToIndexPath(CCIndexPath&, TableView*) {}
	inline virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	inline virtual void TableViewDidDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	inline virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	inline virtual void cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) {}
	inline virtual void didSelectRowAtIndexPath(CCIndexPath&, TableView*) {}
}

class TableViewDelegate {
	inline virtual int numberOfRowsInSection(unsigned int, TableView*) {return 0;}
	inline virtual void numberOfSectionsInTableView(TableView*) {}
	inline virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&) {}
	inline virtual void cellForRowAtIndexPath(CCIndexPath&, TableView*) {}
}

class TextArea : ButtonSprite {
	virtual ~TextArea() = mac 0x19faa0;
	virtual void draw() = mac 0x19f890;
	virtual void setOpacity(unsigned char) = mac 0x19f760;

	static TextArea* create(gd::string, char const*, float, float, cocos2d::CCPoint, float, bool) = mac 0x19eb40;
}

class TextInputDelegate {
	inline virtual void textChanged(CCTextInputNode*) {}
	inline virtual void textInputOpened(CCTextInputNode*) {}
	inline virtual void textInputClosed(CCTextInputNode*) {}
	inline virtual void textInputShouldOffset(CCTextInputNode*, float) {}
	inline virtual void textInputReturn(CCTextInputNode*) {}
	inline virtual bool allowTextInput(CCTextInputNode*) {return true;}
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

	cocos2d::CCNode* m_unknown = mac 0x220;
	GJCommentListLayer* m_commentLayer = mac 0x260;
}

class TouchToggleAction : cocos2d::CCNode {
	static TouchToggleAction* createFromString(gd::string) = mac 0x177e10;
}

class UILayer {
	static UILayer* create() = mac 0x27fd10;
	void disableMenu() = mac 0x280960;
	void enableMenu() = mac 0x280940;
	void pCommand(cocos2d::CCNode*) = mac 0x280830;
	void toggleCheckpointsMenu(bool) = mac 0x280430;
}

class FMODAudioEngine {
	static FMODAudioEngine* sharedEngine() = mac 0x20ef80;
	cocos2d::CCDictionary* m_pDictionary;
	std::string m_sFilePath;
	float m_fBackgroundMusicVolume;
	float m_fEffectsVolume;
	float m_fPulse1;
	float m_fPulse2;
	float m_fPulse3;
	int m_nPulseCounter;
	bool m_bMetering;
	bool m_bFading;
	bool m_bFadeIn;
	float m_fFadeInDuration;
	FMOD::System* m_pSystem;
	FMOD::Sound* m_pSound;
	FMOD::Channel* m_pCurrentSoundChannel;
	FMOD::Channel* m_pGlobalChannel;
	FMOD::DSP* m_pDSP;
	FMOD_RESULT m_eLastResult;
	int m_nVersion;
	void* m_pExtraDriverData;
	int m_nMusicOffset;

}

class UndoCommand {
}

class UndoObject {
	virtual ~UndoObject() = mac 0xa2fd0;

	static UndoObject* create(GameObject*, UndoCommand) = mac 0x94ea0;
	static UndoObject* createWithArray(cocos2d::CCArray*, UndoCommand) = mac 0x96ee0;
}
