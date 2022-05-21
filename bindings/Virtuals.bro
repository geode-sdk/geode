class ArtistCell : TableViewCell {
	virtual bool init() = mac 0x11c7c0, ios 0xa5588;
	virtual void draw() = mac 0x11c980, ios 0xa56fc;
}

class ButtonPage : cocos2d::CCLayer {
}

class FontObject : cocos2d::CCObject {
}

class GJItemIcon : cocos2d::CCSprite {
	virtual void setOpacity(unsigned char) = mac 0x1bd9d0, ios 0x227a30;
}

class GJUserCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	virtual bool init() = mac 0x11e180, ios 0xa6374;
	virtual void draw() = mac 0x11e790, ios 0xa69ac;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x11e860, ios 0xa6a5c;
	virtual void uploadActionFinished(int, int) = mac 0x11eae0, ios 0xa6ca8;
	virtual void uploadActionFailed(int, int) = mac 0x11ecb0, ios 0xa6e90;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x11edb0, ios 0xa6f50;
}

class GameObject : CCSpritePlus {
	virtual void update(float) = mac 0x2fbb90, ios 0x171f20;
	virtual void setScaleX(float) = mac 0x335b00, ios 0x194d78;
	virtual void setScaleY(float) = mac 0x335b90, ios 0x194e10;
	virtual void setScale(float) = mac 0x335c20, ios 0x194ea8;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x335850, ios 0x194abc;
	virtual void setVisible(bool) = mac 0x336280, ios 0x195354;
	virtual void setRotation(float) = mac 0x335970, ios 0x194bc4;
	virtual void setOpacity(unsigned char) = mac 0x335f10, ios 0x1950a0;
	virtual bool initWithTexture(cocos2d::CCTexture2D*) = mac 0x2f56a0, ios 0x16ea20;
	virtual void setChildColor(cocos2d::ccColor3B const&) = mac 0x341f20, ios 0x19c354;
	virtual void setFlipX(bool) = mac 0x335a60, ios 0x194ca8;
	virtual void setFlipY(bool) = mac 0x335ab0, ios 0x194d10;
	virtual void customSetup() = mac 0x2fbba0, ios 0x171f24;
	virtual void setupCustomSprites() = mac 0x307f60, ios 0x179358;
	virtual void addMainSpriteToParent(bool) = mac 0x33a5b0, ios 0x198cf8;
	virtual void resetObject() = mac 0x2fa620, ios 0x170f64;
	virtual void triggerObject(GJBaseGameLayer*) = mac 0x2fa8f0, ios 0x171158;
	virtual void activateObject() = mac 0x2faf60, ios 0x17178c;
	virtual void deactivateObject(bool) = mac 0x2fb8f0, ios 0x171cb8;
	virtual void getObjectRect() = mac 0x3352b0, ios 0x194644;
	virtual void getObjectRect(float, float) = mac 0x3352d0, ios 0x194668;
	virtual void getObjectRect2(float, float) = mac 0x3354e0, ios 0x194818;
	virtual void getObjectTextureRect() = mac 0x3355b0, ios 0x1948c8;
	virtual void getRealPosition() = mac 0x335750, ios 0x194a2c;
	virtual void setStartPos(cocos2d::CCPoint) = mac 0x2fa520, ios 0x170e70;
	virtual void updateStartValues() = mac 0x2fa800, ios 0x1710dc;
	virtual void customObjectSetup(gd::map<gd::string, gd::string>&) = mac 0xdc1a0;
	virtual gd::string getSaveString() = mac 0x33d3d0, ios 0x19b07c;
	virtual void isFlipX() = mac 0x335a40, ios 0x194c98;
	virtual void isFlipY() = mac 0x335a50, ios 0x194ca0;
	virtual void setRScaleX(float) = mac 0x335cb0, ios 0x194f40;
	virtual void setRScaleY(float) = mac 0x335d60, ios 0x194f64;
	virtual void setRScale(float) = mac 0x335e10, ios 0x194f88;
	virtual void getRScaleX() = mac 0x335e50, ios 0x194fcc;
	virtual void getRScaleY() = mac 0x335e80, ios 0x19500c;
	virtual void calculateSpawnXPos() = mac 0x336970, ios 0x195914;
	virtual void triggerActivated(float) = mac 0x336990, ios 0x195920;
	virtual void powerOnObject() = mac 0x3369a0, ios 0x19592c;
	virtual void powerOffObject() = mac 0x3369c0, ios 0x195944;
	virtual void setObjectColor(cocos2d::ccColor3B const&) = mac 0x341c90, ios 0x19c150;
	virtual void setGlowColor(cocos2d::ccColor3B const&) = mac 0x341ed0, ios 0x19c2f8;
	virtual void getOrientedBox() = mac 0x342ad0, ios 0x19c9d8;
	virtual void addToGroup(int) = mac 0x33ad00, ios 0x199130;
	virtual void removeFromGroup(int) = mac 0x33ada0, ios 0x1991c0;
	virtual void spawnXPosition() = mac 0xdc1b0, ios 0x152788;
	virtual void getObjectRectDirty() const = mac 0xdc1d0, ios 0x1527a8;
	virtual void setObjectRectDirty(bool) = mac 0xdc1e0, ios 0x1527b0;
	virtual void getOrientedRectDirty() const = mac 0xdc1f0, ios 0x1527b8;
	virtual void setOrientedRectDirty(bool) = mac 0xdc200, ios 0x1527c0;
	virtual void getType() const = mac 0xdc210, ios 0x1527c8;
	virtual void setType(GameObjectType) = mac 0xdc220, ios 0x1527d0;
	virtual void getStartPos() const = mac 0xdc230, ios 0x1527d8;
}

class GaragePage : cocos2d::CCLayer, ListButtonBarDelegate {
	virtual void listButtonBarSwitchedPage(ListButtonBar*, int) = mac 0x1bc2f0, ios 0x2266d0;
}

class HardStreak : cocos2d::CCDrawNode {
	virtual bool init() = mac 0x5c090, ios 0x3746c;
}

class PauseLayer : CCBlockLayer {
	virtual void keyBackClicked() = mac 0x20cd80, ios 0xc4efc;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x20cc80, ios 0xc4e28;
	virtual void customSetup() = mac 0x20b300, ios 0xc3628;
	virtual void keyUp(cocos2d::enumKeyCodes) = mac 0x20cd60, ios 0xc4ef4;
}

class PriceLabel : cocos2d::CCNode {
}

class RingObject : EffectGameObject {
	virtual void setScale(float) = mac 0xc6fb0, ios 0x276210;
	virtual void setRotation(float) = mac 0xc7010, ios 0x276260;
	virtual void resetObject() = mac 0xc6f90, ios 0x2761e4;
	virtual void customObjectSetup(gd::map<gd::string, gd::string>&) = mac 0xc7040, ios 0x276290;
	virtual gd::string getSaveString() = mac 0xc73a0, ios 0x27638c;
	virtual void setRScale(float) = mac 0xc6fe0, ios 0x276238;
	virtual void triggerActivated(float) = mac 0xc6e10, ios 0x276094;
	virtual void powerOnObject() = mac 0xc6e20, ios 0x2760a0;
	virtual void powerOffObject() = mac 0xc6f70, ios 0x2761d4;
}

class SetIDLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x169670, ios 0x2dcd70;
}

class SetIDPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x155310, ios 0x20d4b8;
	virtual void show() = mac 0x1552c0, ios 0x20d46c;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x155140, ios 0x20d3ac;
	virtual void textChanged(CCTextInputNode*) = mac 0x155180, ios 0x20d3c8;
	virtual void valueChanged() = mac 0x1552b0, ios 0x20d468;
}

class ShardsPage : FLAlertLayer {
	virtual bool init() = mac 0x27d610, ios 0x286c5c;
	virtual void registerWithTouchDispatcher() = mac 0x27fc40, ios 0x288b8c;
	virtual void keyBackClicked() = mac 0x27fbd0, ios 0x288b10;
	virtual void show() = mac 0x27fa30, ios 0x288994;
}

class SongObject : cocos2d::CCObject {
}

class SongsLayer : GJDropDownLayer {
	virtual void customSetup() = mac 0x29ec80, ios 0x253a5c;
}

class StatsLayer : GJDropDownLayer {
	virtual void customSetup() = mac 0x59df0, ios 0x164a34;
}

class UndoObject : cocos2d::CCObject {
}

class AppDelegate : cocos2d::CCSceneDelegate {
	virtual void applicationDidFinishLaunching() = mac 0x3aa900, ios 0x1a23f8;
	virtual void applicationDidEnterBackground() = mac 0x3aabe0, ios 0x1a267c;
	virtual void applicationWillEnterForeground() = mac 0x3aac80, ios 0x1a2710;
	virtual void applicationWillBecomeActive();
	virtual void applicationWillResignActive();
	virtual void trySaveGame() = mac 0x3aaf10, ios 0x1a28f0;
	virtual void willSwitchToScene(cocos2d::CCScene*) = mac 0x3aaf40, ios 0x1a28cc;
}

class CCCountdown : cocos2d::CCSprite {
	virtual bool init() = mac 0x468870;
	virtual void setOpacity(unsigned char) = mac 0x468c70;
}

class CCIndexPath : cocos2d::CCObject {
}

class CCMoveCNode : cocos2d::CCNode {
	virtual bool init() = mac 0x18b3d0, ios 0x14c3c;
}

class ColorAction : cocos2d::CCNode {
}

class CommentCell : TableViewCell, LikeItemDelegate, FLAlertLayerProtocol {
	virtual bool init() = mac 0x11ce80, ios 0xa5878;
	virtual void draw() = mac 0x11d1f0, ios 0xa5bb4;
	virtual void likedItem(LikeItemType, int, bool) = mac 0x11d2c0, ios 0xa5c64;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x11d4b0, ios 0xa5dac;
}

class DialogLayer : cocos2d::CCLayerColor, TextAreaDelegate {
	virtual void onEnter() = mac 0x205900, ios 0x46738;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205790, ios 0x465e8;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205820, ios 0x4667c;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2057e0, ios 0x46644;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205840, ios 0x46684;
	virtual void registerWithTouchDispatcher() = mac 0x205890, ios 0x466c8;
	virtual void keyBackClicked() = mac 0x2056a0, ios 0x465dc;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x205ce0, ios 0x46a80;
	virtual void fadeInTextFinished(TextArea*) = mac 0x205930, ios 0x46770;
}

class GJGameLevel : cocos2d::CCNode {
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x2dd1c0, ios 0x708a0;
	virtual bool canEncode() = mac 0x2ddae0, ios 0x71134;
	virtual bool init() = mac 0x2db310, ios 0x6f180;
}

class GJListLayer : cocos2d::CCLayerColor {
}

class GJScoreCell : TableViewCell, FLAlertLayerProtocol {
	virtual bool init() = mac 0x11d710, ios 0xa5e90;
	virtual void draw() = mac 0x11d9b0, ios 0xa60a4;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x11d8e0, ios 0xa604c;
}

class GJShopLayer : cocos2d::CCLayer, GJPurchaseDelegate {
	virtual void keyBackClicked() = mac 0x1a2370, ios 0xcb010;
	virtual void didPurchaseItem(GJStoreItem*) = mac 0x1a1ae0, ios 0xca8d8;
}

class GJStoreItem : cocos2d::CCNode {
}

class GJUserScore : cocos2d::CCNode {
	virtual bool init() = mac 0x2de5a0, ios 0x71a38;
}

class GJWorldNode : cocos2d::CCNode {
}

class GameManager : GManager {
	virtual void update(float) = mac 0x1d0270, ios 0x23ad30;
	virtual bool init() = mac 0x1c2ec0, ios 0x232218;
	virtual void encodeDataTo(DS_Dictionary*) = mac 0x1cfd90, ios 0x23a88c;
	virtual void dataLoaded(DS_Dictionary*) = mac 0x1ce7f0, ios 0x2396d8;
	virtual void firstLoad() = mac 0x1cf6a0, ios 0x23a2c4;
}

class MapPackCell : TableViewCell {
	virtual bool init() = mac 0x11a9c0, ios 0xa3f5c;
	virtual void draw() = mac 0x11ae80, ios 0xa43c8;
}

class ProfilePage : FLAlertLayer, FLAlertLayerProtocol, LevelCommentDelegate, CommentUploadDelegate, UserInfoDelegate, UploadActionDelegate, UploadPopupDelegate, LeaderboardManagerDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x464a60, ios 0xfeca0;
	virtual void keyBackClicked() = mac 0x4637e0, ios 0xfdca0;
	virtual void show() = mac 0x4639c0, ios 0xfde5c;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x463340, ios 0xfd7f4;
	virtual void updateUserScoreFinished() = mac 0x463850, ios 0xfdd1c;
	virtual void updateUserScoreFailed() = mac 0x463920, ios 0xfddb4;
	virtual void getUserInfoFinished(GJUserScore*) = mac 0x463cd0, ios 0xfe00c;
	virtual void getUserInfoFailed(int) = mac 0x463fb0, ios 0xfe324;
	virtual void userInfoChanged(GJUserScore*) = mac 0x464070, ios 0xfe398;
	virtual void loadCommentsFinished(cocos2d::CCArray*, char const*) = mac 0x464250, ios 0xfe594;
	virtual void loadCommentsFailed(char const*) = mac 0x464340, ios 0xfe618;
	virtual void setupPageInfo(gd::string, char const*) = mac 0x464410, ios 0xfe688;
	virtual void commentUploadFinished(int) = mac 0x464770, ios 0xfea14;
	virtual void commentUploadFailed(int, CommentError) = mac 0x464800, ios 0xfea6c;
	virtual void commentDeleteFailed(int, int) = mac 0x4648e0, ios 0xfeb44;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x463510, ios 0xfd9b4;
	virtual void uploadActionFinished(int, int) = mac 0x463570, ios 0xfda24;
	virtual void uploadActionFailed(int, int) = mac 0x463710, ios 0xfdbf4;
}

class RewardsPage : FLAlertLayer, FLAlertLayerProtocol, GJRewardDelegate {
	virtual bool init() = mac 0xf3800, ios 0x123118;
	virtual void registerWithTouchDispatcher() = mac 0xf5040, ios 0x124460;
	virtual void keyBackClicked() = mac 0xf4fd0, ios 0x1243e4;
	virtual void show() = mac 0xf4e70, ios 0x12429c;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0xf4fb0, ios 0x1243dc;
	virtual void rewardsStatusFinished(int) = mac 0xf4ae0, ios 0x124050;
	virtual void rewardsStatusFailed() = mac 0xf4d70, ios 0x124208;
}

class SecretLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol {
	virtual bool init() = mac 0x363340, ios 0x29f518;
	virtual void keyBackClicked() = mac 0x368450, ios 0x2a3110;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x366d30, ios 0x2a20a0;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x366f30, ios 0x2a2188;
	virtual void textChanged(CCTextInputNode*) = mac 0x367050, ios 0x2a2208;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x366d10, ios 0x2a2098;
}

class SliderThumb : cocos2d::CCMenuItemImage {
}

class SpeedObject : cocos2d::CCNode {
}

class StatsObject : cocos2d::CCObject {
}

class UploadPopup : FLAlertLayer, LevelUploadDelegate {
	virtual void keyBackClicked() = mac 0x12c600, ios 0x1af630;
	virtual void show() = mac 0x12c5a0, ios 0x1af5d0;
	virtual void levelUploadFinished(GJGameLevel*) = mac 0x12c080, ios 0x1af168;
	virtual void levelUploadFailed(GJGameLevel*) = mac 0x12c340, ios 0x1af3bc;
}

class AccountLayer : GJDropDownLayer, GJAccountDelegate, GJAccountBackupDelegate, GJAccountSyncDelegate, FLAlertLayerProtocol {
	virtual void customSetup() = mac 0x81c20, ios 0xdd4d0;
	virtual void layerHidden() = mac 0x83950, ios 0xdec24;
	virtual bool backupAccountFinished() = mac 0x830e0, ios 0xde5d0;
	virtual bool backupAccountFailed(BackupAccountError) = mac 0x832d0, ios 0xde748;
	virtual bool syncAccountFinished() = mac 0x83490, ios 0xde8b0;
	virtual bool syncAccountFailed(BackupAccountError) = mac 0x83630, ios 0xdea1c;
	virtual bool accountStatusChanged() = mac 0x830b0, ios 0xde5bc;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x83830, ios 0xdeb88;
}

class BoomListView : cocos2d::CCLayer, TableViewDelegate, TableViewDataSource {
	virtual void draw() = mac 0x18f790, ios 0x12cc7c;
	virtual void setupList() = mac 0x18ef90, ios 0x12c710;
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) = mac 0x18f030, ios 0x12c798;
	virtual void cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f070, ios 0x12c7a8;
	virtual void didSelectRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f090, ios 0x12c7b8;
	virtual void numberOfRowsInSection(unsigned int, TableView*) = mac 0x18f0b0, ios 0x12c7c0;
	virtual void numberOfSectionsInTableView(TableView*) = mac 0x18f0e0, ios 0x12c7d0;
	virtual void cellForRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f100, ios 0x12c7e0;
	virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&) = mac 0x18f770, ios 0x12cc74;
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) = mac 0x18f050, ios 0x12c7a0;
	virtual void getListCell(char const*) = mac 0x18f200, ios 0x12c86c;
	virtual void loadCell(TableViewCell*, int) = mac 0x18f4a0, ios 0x12c968;
}

class ButtonSprite : cocos2d::CCSprite {
}

class CCBlockLayer : cocos2d::CCLayerColor {
	virtual bool init() = mac 0x2a59c0, ios 0x45490;
	virtual void draw() = mac 0x2a5c20, ios 0x456c4;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2a5c50, ios 0x45700;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2a5cb0, ios 0x45720;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2a5c70, ios 0x45710;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2a5c90, ios 0x45718;
	virtual void registerWithTouchDispatcher() = mac 0x2a5ad0, ios 0x45594;
	virtual void keyBackClicked() = mac 0x2a5b10, ios 0x455cc;
	virtual void customSetup() = mac 0x2a5a70, ios 0x45550;
	virtual void enterLayer() = mac 0x2a5aa0, ios 0x4555c;
	virtual void exitLayer() = mac 0x2a5b40, ios 0x455e8;
	virtual void showLayer(bool) = mac 0x2a5b90, ios 0x45648;
	virtual void hideLayer(bool) = mac 0x2a5ba0, ios 0x45654;
	virtual void layerVisible() = mac 0x2a5bc0, ios 0x45664;
	virtual void layerHidden() = mac 0x2a5be0, ios 0x45674;
	virtual void enterAnimFinished() = mac 0x2a5bb0, ios 0x45660;
	virtual void disableUI() = mac 0x2a5a80, ios 0x45554;
	virtual void enableUI() = mac 0x2a5a90, ios 0x45558;
}

class CCCircleWave : cocos2d::CCNode {
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0xbd600, ios 0xe1e0c;
	virtual void removeMeAndCleanup() = mac 0xbdac0, ios 0xe2310;
	virtual void draw() = mac 0xbd960, ios 0xe21c8;
	virtual void updateTweenAction(float, char const*) = mac 0xbd710, ios 0xe1f44;
}

class CCLightFlash : cocos2d::CCNode {
	virtual bool init() = mac 0x2958f0, ios 0x1fdd0;
}

class CCLightStrip : cocos2d::CCNode {
	virtual void draw() = mac 0x296410, ios 0x206dc;
	virtual void updateTweenAction(float, char const*) = mac 0x2965a0, ios 0x20834;
}

class CCSpritePart : CCSpritePlus {
}

class CCSpritePlus : cocos2d::CCSprite {
	virtual void setScaleX(float) = mac 0x248950, ios 0x11925c;
	virtual void setScaleY(float) = mac 0x2489d0, ios 0x1192d8;
	virtual void setScale(float) = mac 0x248a50, ios 0x119354;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x2486b0, ios 0x119014;
	virtual void setRotation(float) = mac 0x248730, ios 0x119088;
	virtual bool initWithTexture(cocos2d::CCTexture2D*) = mac 0x248690, ios 0x119000;
	virtual bool initWithSpriteFrameName(char const*) = mac 0x248670, ios 0x118ff4;
	virtual void setFlipX(bool) = mac 0x2487b0, ios 0x1190fc;
	virtual void setFlipY(bool) = mac 0x248880, ios 0x1191ac;
}

class CreatorLayer : cocos2d::CCLayer, cocos2d::CCSceneTransitionDelegate, DialogDelegate {
	virtual bool init() = mac 0x141c10, ios 0x733b4;
	virtual void keyBackClicked() = mac 0x143950, ios 0x74b20;
	virtual void sceneWillResume() = mac 0x1438f0, ios 0x74af0;
	virtual void dialogClosed(DialogLayer*) = mac 0x143830, ios 0x74a84;
}

class DelaySection : cocos2d::CCObject {
}

class DialogObject : cocos2d::CCObject {
}

class FLAlertLayer : cocos2d::CCLayerColor {
	virtual void onEnter() = mac 0x25f350, ios 0x1ff1fc;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25ee40, ios 0x1fee48;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25f0a0, ios 0x1fefb4;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25ef60, ios 0x1fef00;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25f020, ios 0x1fef74;
	virtual void registerWithTouchDispatcher() = mac 0x25f2e0, ios 0x1ff18c;
	virtual void keyBackClicked() = mac 0x25ed90, ios 0x1fede4;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x25ece0, ios 0x1fedb4;
	virtual void show() = mac 0x25f120, ios 0x1feff4;
}

class GJHttpResult : cocos2d::CCNode {
}

class GJRewardItem : cocos2d::CCObject {
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x4d7f0, ios 0x252d08;
	virtual bool canEncode() = mac 0x4d850, ios 0x252d74;
}

class GauntletNode : cocos2d::CCNode {
}

class LoadingLayer : cocos2d::CCLayer {
}

class OptionsLayer : GJDropDownLayer, FLAlertLayerProtocol {
	virtual void customSetup() = mac 0x43dc70, ios 0x86434;
	virtual void layerHidden() = mac 0x43f200, ios 0x875e8;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x43f270, ios 0x87674;
}

class PlayerObject : GameObject {
	virtual void update(float) = mac 0x218bf0, ios 0x1416fc;
	virtual void setScaleX(float) = mac 0x22e7f0, ios 0x152374;
	virtual void setScaleY(float) = mac 0x22e830, ios 0x1523c4;
	virtual void setScale(float) = mac 0x22e870, ios 0x152414;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x22c8b0, ios 0x150b14;
	virtual void setVisible(bool) = mac 0x22e8b0, ios 0x152464;
	virtual void setRotation(float) = mac 0x22e6e0, ios 0x152270;
	virtual void setOpacity(unsigned char) = mac 0x22d400, ios 0x1514e8;
	virtual void setColor(cocos2d::ccColor3B const&) = mac 0x22cdf0, ios 0x150fb0;
	virtual void setFlipX(bool) = mac 0x22e720, ios 0x1522c0;
	virtual void setFlipY(bool) = mac 0x22e7b0, ios 0x152338;
	virtual void resetObject() = mac 0x223170, ios 0x148dec;
	virtual void getRealPosition() = mac 0x22d5f0, ios 0x1515ec;
	virtual void getOrientedBox() = mac 0x22dee0, ios 0x151c70;
	virtual void animationFinished(char const*) = mac 0x22e9d0, ios 0x1524b8;
}

class SearchButton : cocos2d::CCSprite {
}

class SecretLayer2 : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DialogDelegate {
	virtual bool init() = mac 0x25fe70, ios 0x227d08;
	virtual void onExit() = mac 0x266b30, ios 0x22d6bc;
	virtual void keyBackClicked() = mac 0x266a30, ios 0x22d6b0;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x265880, ios 0x22c684;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x265a80, ios 0x22c76c;
	virtual void textChanged(CCTextInputNode*) = mac 0x265ba0, ios 0x22c7ec;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x265860, ios 0x22c67c;
	virtual void dialogClosed(DialogLayer*) = mac 0x264880, ios 0x22b3f0;
}

class SecretLayer3 : cocos2d::CCLayer, DialogDelegate {
	virtual bool init() = mac 0x267040, ios 0x22d8d4;
	virtual void onExit() = mac 0x26c270, ios 0x231e5c;
	virtual void keyBackClicked() = mac 0x26c240, ios 0x231e50;
	virtual void dialogClosed(DialogLayer*) = mac 0x26bf10, ios 0x231bdc;
}

class SecretLayer4 : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DialogDelegate {
	virtual bool init() = mac 0x1ed640, ios 0x25dd84;
	virtual void onExit() = mac 0x1f2220, ios 0x261068;
	virtual void keyBackClicked() = mac 0x1f2120, ios 0x26105c;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x1f0ff0, ios 0x260558;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x1f11f0, ios 0x260640;
	virtual void textChanged(CCTextInputNode*) = mac 0x1f1310, ios 0x2606c0;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x1f0fd0, ios 0x260550;
	virtual void dialogClosed(DialogLayer*) = mac 0x1f0a90, ios 0x25fc10;
}

class SetTextPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x157050, ios 0x20e89c;
	virtual void show() = mac 0x157000, ios 0x20e850;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x156ef0, ios 0x20e7ac;
	virtual void textChanged(CCTextInputNode*) = mac 0x156f30, ios 0x20e7c8;
}

class SimpleObject : cocos2d::CCObject {
}

class SimplePlayer : cocos2d::CCSprite {
	virtual void setOpacity(unsigned char) = mac 0x1bcc40, ios 0x22705c;
	virtual void setColor(cocos2d::ccColor3B const&) = mac 0x1bc9b0, ios 0x226dd8;
}

class SlideInLayer : cocos2d::CCLayerColor {
	virtual bool init() = mac 0x138cf0;
	virtual void draw() = mac 0x1394a0;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1394d0;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x139530;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1394f0;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x139510;
	virtual void registerWithTouchDispatcher() = mac 0x139190;
	virtual void keyBackClicked() = mac 0x1391d0;
	virtual void customSetup() = mac 0x139110;
	virtual void enterLayer() = mac 0x139160;
	virtual void exitLayer(cocos2d::CCObject*) = mac 0x139210;
	virtual void showLayer(bool) = mac 0x139270;
	virtual void hideLayer(bool) = mac 0x139360;
	virtual void layerVisible() = mac 0x139440;
	virtual void layerHidden() = mac 0x139460;
	virtual void enterAnimFinished() = mac 0x139430;
	virtual void disableUI() = mac 0x139120;
	virtual void enableUI() = mac 0x139140;
}

class SupportLayer : GJDropDownLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate {
	virtual void customSetup() = mac 0x1d90b0, ios 0xf5ce0;
	virtual void uploadActionFinished(int, int) = mac 0x1d9de0, ios 0xf6934;
	virtual void uploadActionFailed(int, int) = mac 0x1d9f00, ios 0xf6a5c;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x1d9d90, ios 0xf68e4;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x1da170, ios 0xf6ca4;
}

class BonusDropdown : cocos2d::CCNode {
}

class BoomListLayer : cocos2d::CCLayerColor {
}

class CCAlertCircle : cocos2d::CCNode {
	virtual bool init() = mac 0xbdea0;
	virtual void draw() = mac 0xbdf40;
}

class CCCircleAlert : CCCircleWave {
}

class ChallengeNode : cocos2d::CCNode {
}

class DrawGridLayer : cocos2d::CCLayer {
	virtual void update(float) = mac 0xa3b30, ios 0x26da24;
	virtual void draw() = mac 0xa3c40, ios 0x26dae4;
}

class EditButtonBar : cocos2d::CCNode {
}

class EndLevelLayer : GJDropDownLayer {
	virtual void keyBackClicked();
	virtual void keyDown(cocos2d::enumKeyCodes);
	virtual void customSetup();
	virtual void showLayer(bool);
	virtual void enterAnimFinished();
	virtual void keyUp(cocos2d::enumKeyCodes);
}

class ExtendedLayer : cocos2d::CCLayer {
	virtual bool init() = mac 0x1e41e0, ios 0xbd198;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x1e41f0, ios 0xbd19c;
}

class GJChestSprite : cocos2d::CCSprite {
	virtual void setColor(cocos2d::ccColor3B const&) = mac 0xf8270, ios 0x1272f4;
}

class GJGarageLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, GameRateDelegate, ListButtonBarDelegate, DialogDelegate {
	virtual bool init() = mac 0x1b4980, ios 0x22045c;
	virtual void keyBackClicked() = mac 0x1bb630, ios 0x225d04;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x1b7dc0, ios 0x222fc0;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x1b8040, ios 0x223110;
	virtual void textChanged(CCTextInputNode*) = mac 0x1b82c0, ios 0x2232cc;
	virtual void listButtonBarSwitchedPage(ListButtonBar*, int) = mac 0x1bad10, ios 0x2256b4;
	virtual void updateRate() = mac 0x1bb420, ios 0x225b74;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x1bb050, ios 0x225948;
	virtual void dialogClosed(DialogLayer*) = mac 0x1bb160, ios 0x225a44;
}

class GJGroundLayer : cocos2d::CCLayer {
	virtual void draw() = mac 0x356a60, ios 0x1fbf4;
	virtual void showGround() = mac 0x356920, ios 0x1fab0;
	virtual void fadeInGround(float) = mac 0x356930, ios 0x1fabc;
	virtual void fadeOutGround(float) = mac 0x356a30, ios 0x1fbc8;
}

class GJMessageCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	virtual bool init() = mac 0x11fc00, ios 0xa78d8;
	virtual void draw() = mac 0x11fef0, ios 0xa7b30;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x11ffc0, ios 0xa7be0;
	virtual void uploadActionFinished(int, int) = mac 0x120180, ios 0xa7ce8;
	virtual void uploadActionFailed(int, int) = mac 0x120310, ios 0xa7ea8;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x120410, ios 0xa7f68;
}

class GJRequestCell : TableViewCell, FLAlertLayerProtocol, UploadPopupDelegate, UploadActionDelegate {
	virtual bool init() = mac 0x11f080, ios 0xa709c;
	virtual void draw() = mac 0x11f390, ios 0xa7308;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x11f460, ios 0xa73b8;
	virtual void uploadActionFinished(int, int) = mac 0x11f640, ios 0xa74d4;
	virtual void uploadActionFailed(int, int) = mac 0x11f800, ios 0xa76b0;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x11f930, ios 0xa778c;
}

class GJRobotSprite : CCAnimatedSprite {
	virtual bool init() = mac 0x34ad50, ios 0x161634;
	virtual void setOpacity(unsigned char) = mac 0x34bcc0, ios 0x1625d4;
	virtual void hideSecondary() = mac 0x34c3b0, ios 0x162ce4;
}

class GJSongBrowser : GJDropDownLayer, FLAlertLayerProtocol {
	virtual void customSetup() = mac 0x368ab0, ios 0x19dd04;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x3690a0, ios 0x19e2a0;
}

class GJSpriteColor : cocos2d::CCNode {
	virtual bool init() = mac 0x343c90, ios 0x19d6a0;
}

class GJUserMessage : cocos2d::CCNode {
	virtual bool init() = mac 0x2debf0, ios 0x71da0;
}

class GauntletLayer : cocos2d::CCLayer, LevelManagerDelegate {
	virtual void keyBackClicked() = mac 0x1d5e20, ios 0x15716c;
	virtual void loadLevelsFinished(cocos2d::CCArray*, char const*) = mac 0x1d5410, ios 0x156864;
	virtual void loadLevelsFailed(char const*) = mac 0x1d55d0, ios 0x156a48;
}

class LikeItemLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x35ff20, ios 0x168bcc;
}

class ListButtonBar : cocos2d::CCNode {
}

class LoadingCircle : cocos2d::CCLayerColor {
	virtual bool init() = mac 0x277f00, ios 0x27f280;
	virtual void draw() = mac 0x278170, ios 0x27f4e4;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2781d0;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2781e0;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2781f0;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x278200;
	virtual void registerWithTouchDispatcher() = mac 0x278190, ios 0x27f4f4;
}

class MenuGameLayer : cocos2d::CCLayer {
	virtual void update(float) = mac 0x28fa70, ios 0x27fea0;
	virtual bool init() = mac 0x28f150, ios 0x27f654;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x290650, ios 0x280898;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x290770, ios 0x2809b0;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x290790, ios 0x2809b8;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2907b0, ios 0x2809c0;
	virtual void registerWithTouchDispatcher() = mac 0x2907f0, ios 0x2809dc;
}

class MyLevelsLayer : cocos2d::CCLayer {
	virtual bool init() = mac 0x35470, ios 0x81b90;
	virtual void keyBackClicked() = mac 0x35b90, ios 0x8222c;
}

class ObjectDecoder : cocos2d::CCNode {
	virtual bool init() = mac 0x437f80, ios 0x1589dc;
}

class ObjectManager : cocos2d::CCNode {
	virtual bool init() = mac 0x43b280, ios 0x168d48;
}

class ObjectToolbox : cocos2d::CCNode {
	virtual bool init() = mac 0x3b2d80, ios 0x1b092c;
}

class SongInfoLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x36a980, ios 0x28e514;
}

class TableViewCell : cocos2d::CCLayer {
}

class TutorialLayer : FLAlertLayer {
	virtual bool init() = mac 0x286e00, ios 0x11e860;
	virtual void keyBackClicked() = mac 0x2878d0, ios 0x11f240;
}

class TutorialPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x344ae0;
	virtual void show() = mac 0x344bb0;
}

class AchievementBar : cocos2d::CCNodeRGBA {
	virtual void setOpacity(unsigned char) = mac 0x37b9b0, ios 0x1a5fc4;
}

class CCContentLayer : cocos2d::CCLayerColor {
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x464c60, ios 0xf5440;
}

class CCCounterLabel : cocos2d::CCLabelBMFont {
}

class ChallengesPage : FLAlertLayer, FLAlertLayerProtocol, GJChallengeDelegate, CurrencyRewardDelegate {
	virtual bool init() = mac 0x1db520, ios 0x20e64;
	virtual void registerWithTouchDispatcher() = mac 0x1dcc50, ios 0x22014;
	virtual void keyBackClicked() = mac 0x1dcbc0, ios 0x21fc0;
	virtual void show() = mac 0x1dca10, ios 0x21e3c;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x1dcba0, ios 0x21fb8;
	virtual void challengeStatusFinished() = mac 0x1dc520, ios 0x21ad0;
	virtual void challengeStatusFailed() = mac 0x1dc830, ios 0x21cd4;
	virtual void currencyWillExit(CurrencyRewardLayer*) = mac 0x1dcdb0, ios 0x22174;
}

class ColoredSection : cocos2d::CCObject {
}

class CreateMenuItem : CCMenuItemSpriteExtra {
}

class CurrencySprite : CCSpritePlus {
}

class CustomListView : BoomListView {
	virtual void setupList() = mac 0x116e70, ios 0xa1014;
	virtual void getListCell(char const*) = mac 0x10d560, ios 0x99240;
	virtual void loadCell(TableViewCell*, int) = mac 0x10e610, ios 0x99750;
}

class CustomSongCell : TableViewCell {
	virtual bool init() = mac 0x11c440, ios 0xa53a0;
	virtual void draw() = mac 0x11c5c0, ios 0xa548c;
}

class DailyLevelNode : cocos2d::CCNode, FLAlertLayerProtocol {
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x10b750, ios 0x11c304;
}

class DailyLevelPage : FLAlertLayer, FLAlertLayerProtocol, GJDailyLevelDelegate, LevelDownloadDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x10a6f0, ios 0x11b424;
	virtual void keyBackClicked() = mac 0x10a660, ios 0x11b3d0;
	virtual void show() = mac 0x10a4b0, ios 0x11b24c;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x10a640, ios 0x11b3c8;
	virtual void dailyStatusFinished(bool) = mac 0x109be0, ios 0x11ab98;
	virtual void dailyStatusFailed(bool) = mac 0x10a090, ios 0x11aee8;
	virtual void levelDownloadFinished(GJGameLevel*) = mac 0x10a1b0, ios 0x11affc;
	virtual void levelDownloadFailed(int) = mac 0x10a280, ios 0x11b074;
}

class DialogDelegate {
	virtual void dialogClosed(DialogLayer*);
}

class EditLevelLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, SetIDPopupDelegate {
	virtual void keyBackClicked() = mac 0xe6670, ios 0x85ff8;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0xe66a0, ios 0x86004;
	virtual void setIDPopupClosed(SetIDPopup*, int) = mac 0xe6560, ios 0x85f6c;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0xe5e70, ios 0x85a88;
	virtual void textInputOpened(CCTextInputNode*) = mac 0xe5540, ios 0x85374;
	virtual void textInputClosed(CCTextInputNode*) = mac 0xe51e0, ios 0x850b0;
	virtual void textChanged(CCTextInputNode*) = mac 0xe57a0, ios 0x85558;
	virtual void uploadActionFinished(int, int) = mac 0xe6740, ios 0x86074;
	virtual void uploadActionFailed(int, int) = mac 0xe68c0, ios 0x861e0;
	virtual void onClosePopup(UploadActionPopup*) = mac 0xe66f0, ios 0x86024;
}

class GJMessagePopup : FLAlertLayer, UploadActionDelegate, UploadPopupDelegate, FLAlertLayerProtocol, DownloadMessageDelegate {
	virtual void keyBackClicked() = mac 0x1504a0, ios 0x2096b0;
	virtual void downloadMessageFinished(GJUserMessage*) = mac 0x150510, ios 0x20972c;
	virtual void downloadMessageFailed(int) = mac 0x1505e0, ios 0x2097a0;
	virtual void uploadActionFinished(int, int) = mac 0x150680, ios 0x209858;
	virtual void uploadActionFailed(int, int) = mac 0x150820, ios 0x209a14;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x150900, ios 0x209ad8;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x150a00, ios 0x209b70;
}

class GJOptionsLayer : FLAlertLayer {
	virtual bool init() = mac 0x145c60, ios 0x200ce8;
	virtual void keyBackClicked() = mac 0x147190, ios 0x2023a8;
	virtual void setupOptions() = mac 0x1464d0, ios 0x201554;
	virtual void onClose(cocos2d::CCObject*) = mac 0x147160, ios 0x20236c;
}

class GJRewardObject : cocos2d::CCObject {
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x4d170, ios 0x252930;
	virtual bool canEncode() = mac 0x4d1f0, ios 0x2529b0;
}

class GJScaleControl : cocos2d::CCLayer {
	virtual bool init() = mac 0x31b30, ios 0x2c4d5c;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x31d30, ios 0x2c4f08;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x31e60, ios 0x2c4fb8;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x31fb0, ios 0x2c50b4;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x32060, ios 0x2c5128;
}

class GJSearchObject : cocos2d::CCNode {
}

class GJSpiderSprite : GJRobotSprite {
	virtual bool init() = mac 0x34c700, ios 0x162e74;
}

class GameObjectCopy : cocos2d::CCObject {
}

class HSVWidgetPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x2371e0, ios 0x98420;
}

class InstantSection : cocos2d::CCObject {
}

class LevelInfoLayer : cocos2d::CCLayer, LevelDownloadDelegate, LevelUpdateDelegate, RateLevelDelegate, LikeItemDelegate, FLAlertLayerProtocol, LevelDeleteDelegate, NumberInputDelegate, SetIDPopupDelegate {
	virtual void keyBackClicked() = mac 0x166160, ios 0x1e358;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x166190, ios 0x1e364;
	virtual void numberInputClosed(NumberInputLayer*) = mac 0x1657a0, ios 0x1dc18;
	virtual void levelDownloadFinished(GJGameLevel*) = mac 0x164c00, ios 0x1d3c0;
	virtual void levelDownloadFailed(int) = mac 0x164d50, ios 0x1d4f4;
	virtual void levelUpdateFinished(GJGameLevel*, UpdateResponse) = mac 0x164e60, ios 0x1d5dc;
	virtual void levelUpdateFailed(int) = mac 0x165010, ios 0x1d76c;
	virtual void levelDeleteFinished(int) = mac 0x165120, ios 0x1d858;
	virtual void levelDeleteFailed(int) = mac 0x165230, ios 0x1d950;
	virtual void rateLevelClosed() = mac 0x165e60, ios 0x1e18c;
	virtual void likedItem(LikeItemType, int, bool) = mac 0x165e90, ios 0x1e198;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x165f90, ios 0x1e214;
	virtual void setIDPopupClosed(SetIDPopup*, int) = mac 0x163e60, ios 0x1c95c;
}

class ListButtonPage : cocos2d::CCLayer {
}

class RateDemonLayer : FLAlertLayer, UploadPopupDelegate, UploadActionDelegate {
	virtual void keyBackClicked() = mac 0x12ec20, ios 0x2e2820;
	virtual void uploadActionFinished(int, int) = mac 0x12e960, ios 0x2e2668;
	virtual void uploadActionFailed(int, int) = mac 0x12ea40, ios 0x2e2714;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x12eb40, ios 0x2e27d8;
}

class RateLevelLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x2e2250, ios 0x384d8;
}

class RateStarsLayer : FLAlertLayer, UploadPopupDelegate, UploadActionDelegate {
	virtual void keyBackClicked() = mac 0x136ec0, ios 0x3f5d8;
	virtual void uploadActionFinished(int, int) = mac 0x136c10, ios 0x3f430;
	virtual void uploadActionFailed(int, int) = mac 0x136cf0, ios 0x3f4d4;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x136de0, ios 0x3f590;
}

class ScrollingLayer : cocos2d::CCLayerColor {
	virtual void draw() = mac 0x41abc0, ios 0x119758;
	virtual void visit() = mac 0x41acb0, ios 0x119814;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x41ae10, ios 0x1198a8;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x41afb0, ios 0x119968;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x41b0d0, ios 0x119a64;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x41b110, ios 0x119a9c;
}

class SelectArtLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0xad440, ios 0x275020;
}

class SetFolderPopup : SetIDPopup, SetTextPopupDelegate {
	virtual void valueChanged() = mac 0x155d40, ios 0x20da70;
	virtual void setTextPopupClosed(SetTextPopup*, gd::string) = mac 0x1561b0, ios 0x20ddb0;
}

class SetItemIDLayer : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x5bb90, ios 0x2d5bec;
	virtual void show() = mac 0x5bb40, ios 0x2d5ba0;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x5b9e0, ios 0x2d5b04;
	virtual void textChanged(CCTextInputNode*) = mac 0x5ba20, ios 0x2d5b20;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x5bca0, ios 0x2d5bf8;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x5bd60, ios 0x2d5c64;
}

class SongInfoObject : cocos2d::CCNode {
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x2f2c70, ios 0xd4a5c;
	virtual bool canEncode() = mac 0x2f2da0, ios 0xd4b8c;
}

class StartPosObject : EffectGameObject {
	virtual bool init() = mac 0xda8a0, ios 0x27e060;
	virtual gd::string getSaveString() = mac 0xda960, ios 0x27e0f0;
}

class TextAlertPopup : cocos2d::CCNode {
}

class WorldLevelPage : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x204300;
	virtual void show() = mac 0x2041c0;
}

class AchievementCell : TableViewCell {
	virtual bool init() = mac 0x117730, ios 0xa1650;
	virtual void draw() = mac 0x117740, ios 0xa16c8;
}

class BitmapFontCache : cocos2d::CCObject {
}

class BoomScrollLayer : cocos2d::CCLayer {
	virtual void visit() = mac 0x1e2f80, ios 0xbc5d4;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e3c40, ios 0xbccec;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e3d90, ios 0xbcdf8;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e3ff0, ios 0xbd000;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e3b40, ios 0xbcca0;
	virtual void registerWithTouchDispatcher() = mac 0x1e3970, ios 0xbcb30;
}

class CCNodeContainer : cocos2d::CCNode {
	virtual bool init() = mac 0xba950, ios 0x13aa24;
	virtual void visit() = mac 0xba960, ios 0x13aa28;
}

class CCSpriteWithHue : cocos2d::CCSprite {
	virtual void draw() = mac 0x35a400, ios 0x104384;
	virtual bool initWithTexture(cocos2d::CCTexture2D*) = mac 0x359d10, ios 0x103ff8;
	virtual bool initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&) = mac 0x359d80, ios 0x104074;
	virtual bool initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool) = mac 0x359b80, ios 0x103ebc;
	virtual bool initWithSpriteFrame(cocos2d::CCSpriteFrame*) = mac 0x359da0, ios 0x104084;
	virtual void updateColor() = mac 0x359f70, ios 0x104148;
}

class CCTextInputNode : cocos2d::CCLayer, cocos2d::CCIMEDelegate, cocos2d::CCTextFieldDelegate {
	virtual void visit() = mac 0x5d380, ios 0xe279c;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ec80, ios 0xe3e0c;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5eea0, ios 0xe3f90;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ee60, ios 0xe3f80;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ee80, ios 0xe3f88;
	virtual void registerWithTouchDispatcher() = mac 0x5eec0, ios 0xe3f98;
	virtual void textChanged() = mac 0x5dd70, ios 0xe2fb4;
	virtual void onClickTrackNode(bool) = mac 0x5dd40, ios 0xe2f88;
	virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5dad0, ios 0xe2da0;
	virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5dc20, ios 0xe2ef8;
	virtual void onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int) = mac 0x5de50, ios 0xe30c0;
	virtual void onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF*) = mac 0x5e2c0, ios 0xe34e0;
	virtual void onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF*) = mac 0x5e610, ios 0xe373c;
}

class CustomSongLayer : FLAlertLayer, FLAlertLayerProtocol, TextInputDelegate, GJDropDownLayerDelegate {
	virtual void keyBackClicked() = mac 0xf21c0, ios 0xc683c;
	virtual void show() = mac 0xf22a0, ios 0xc6848;
	virtual void textChanged(CCTextInputNode*) = mac 0xf1fb0, ios 0xc66e8;
	virtual void textInputOpened(CCTextInputNode*) = mac 0xf1f10, ios 0xc66b0;
	virtual void textInputClosed(CCTextInputNode*) = mac 0xf1e70, ios 0xc6678;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0xf1fd0, ios 0xc6708;
	virtual void textInputReturn(CCTextInputNode*) = mac 0xf2090, ios 0xc6774;
	virtual void dropDownLayerWillClose(GJDropDownLayer*) = mac 0xf2140, ios 0xc67d4;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0xf1be0, ios 0xc64b0;
}

class EndPortalObject : GameObject {
	virtual bool init() = mac 0x1da980, ios 0x2c62c4;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x1dae70, ios 0x2c6734;
	virtual void setVisible(bool) = mac 0x1daf30, ios 0x2c67dc;
	virtual void triggerObject(GJBaseGameLayer*) = mac 0x1dadc0, ios 0x2c669c;
	virtual void calculateSpawnXPos() = mac 0x1dae50, ios 0x2c671c;
}

class FMODAudioEngine : cocos2d::CCNode {
	virtual void update(float) = mac 0x20f1d0, ios 0xc2690;
}

class FileSaveManager : GManager {
	virtual bool init() = mac 0x6360;
	virtual void firstLoad() = mac 0x6390;
}

class GJBaseGameLayer : cocos2d::CCLayer, TriggerEffectDelegate {
	virtual bool init() = mac 0xafc90, ios 0x1338e8;
	virtual void objectsCollided(int, int) = mac 0xb6d90, ios 0x137ff0;
	virtual void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int) = mac 0xb73a0, ios 0x1384a8;
	virtual void updateColor(cocos2d::ccColor3B, float, int, bool, float, cocos2d::ccHSVValue, int, bool, int, EffectGameObject*) = mac 0xb7420, ios 0x138530;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0xba990;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0xba9a0;
	virtual void toggleGroupTriggered(int, bool) = mac 0xb75a0, ios 0x138674;
	virtual void spawnGroup(int) = mac 0xb7050, ios 0x138210;
	virtual void addToSection(GameObject*) = mac 0xb7b70, ios 0x138ad0;
	virtual void addToGroup(GameObject*, int, bool) = mac 0xb77f0, ios 0x138880;
	virtual void removeFromGroup(GameObject*, int) = mac 0xb7a60, ios 0x138a64;
}

class GJChallengeItem : cocos2d::CCObject {
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x4df60, ios 0x25331c;
	virtual bool canEncode() = mac 0x4e020, ios 0x2533f8;
}

class GJDropDownLayer : cocos2d::CCLayerColor {
	virtual void draw() = mac 0x352910, ios 0x29827c;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x352940, ios 0x2982b8;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x3529a0, ios 0x2982d8;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x352960, ios 0x2982c8;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x352980, ios 0x2982d0;
	virtual void registerWithTouchDispatcher() = mac 0x3525f0, ios 0x297f60;
	virtual void keyBackClicked() = mac 0x352630, ios 0x297f98;
	virtual void customSetup() = mac 0x352570, ios 0x297efc;
	virtual void enterLayer() = mac 0x3525c0, ios 0x297f28;
	virtual void exitLayer(cocos2d::CCObject*) = mac 0x352670, ios 0x297fbc;
	virtual void showLayer(bool) = mac 0x3526c0, ios 0x29801c;
	virtual void hideLayer(bool) = mac 0x3527b0, ios 0x29811c;
	virtual void layerVisible() = mac 0x3528b0, ios 0x29821c;
	virtual void layerHidden() = mac 0x3528d0, ios 0x29822c;
	virtual void enterAnimFinished() = mac 0x3528a0, ios 0x298218;
	virtual void disableUI() = mac 0x352580, ios 0x297f00;
	virtual void enableUI() = mac 0x3525a0, ios 0x297f14;
}

class GJEffectManager : cocos2d::CCNode {
	virtual bool init() = mac 0x180230, ios 0xdf5c;
}

class GJFriendRequest : cocos2d::CCNode {
	virtual bool init() = mac 0x2dea00, ios 0x71c5c;
}

class GJObjectDecoder : cocos2d::CCNode, ObjectDecoderDelegate {
	virtual bool init() = mac 0x41e780, ios 0x13f9f8;
	virtual void getDecodedObject(int, DS_Dictionary*) = mac 0x41e790, ios 0x13f9fc;
}

class InfoAlertButton : CCMenuItemSpriteExtra {
	virtual void activate() = mac 0x2ecd70, ios 0xe0914;
}

class InheritanceNode : cocos2d::CCObject {
}

class LabelGameObject : EffectGameObject {
	virtual bool init() = mac 0xdb990, ios 0x27ea8c;
	virtual void setOpacity(unsigned char) = mac 0xdba40, ios 0x27eb28;
	virtual void setupCustomSprites() = mac 0xdb9b0, ios 0x27ea98;
	virtual void addMainSpriteToParent(bool) = mac 0xdbce0, ios 0x27ed40;
	virtual void customObjectSetup(gd::map<gd::string, gd::string>&) = mac 0xdbbe0, ios 0x27ec6c;
	virtual gd::string getSaveString() = mac 0xdbd50, ios 0x27eda4;
	virtual void setObjectColor(cocos2d::ccColor3B const&) = mac 0xdbca0, ios 0x27ed00;
}

class MoreSearchLayer : FLAlertLayer, TextInputDelegate {
	virtual bool init() = mac 0x3896b0, ios 0x34f48;
	virtual void keyBackClicked() = mac 0x38b820, ios 0x36c2c;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x38b6b0, ios 0x36b60;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x38b770, ios 0x36bcc;
}

class RetryLevelLayer : GJDropDownLayer {
	virtual void keyBackClicked() = mac 0x28ed10, ios 0x116b68;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x28ec30, ios 0x116b18;
	virtual void customSetup() = mac 0x28de80, ios 0x115d48;
	virtual void showLayer(bool) = mac 0x28eaf0, ios 0x1169bc;
	virtual void enterAnimFinished() = mac 0x28ebf0, ios 0x116ad4;
	virtual void keyUp(cocos2d::enumKeyCodes) = mac 0x28ecf0, ios 0x116b60;
}

class SelectFontLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x1445f0, ios 0x1ffcc8;
}

class SetGroupIDLayer : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x198050, ios 0x26940;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x197ab0, ios 0x26488;
	virtual void textChanged(CCTextInputNode*) = mac 0x197af0, ios 0x264a4;
}

class SetupPulsePopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
	virtual void keyBackClicked() = mac 0x1ec840, ios 0x29d680;
	virtual void show() = mac 0x1ec870, ios 0x29d68c;
	virtual void colorValueChanged(cocos2d::ccColor3B) = mac 0x1ec680, ios 0x29d574;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x1ec900, ios 0x29d70c;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x1ec920, ios 0x29d714;
	virtual void textChanged(CCTextInputNode*) = mac 0x1ec960, ios 0x29d730;
	virtual void colorSelectClosed(GJSpecialColorSelect*, int) = mac 0x1ebf20, ios 0x29cf68;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x1ecf20, ios 0x29d9c8;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x1ecfe0, ios 0x29da34;
}

class SetupShakePopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x3b00d0, ios 0x2906d4;
	virtual void show() = mac 0x3b0080, ios 0x290688;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x3afc80, ios 0x290454;
	virtual void textChanged(CCTextInputNode*) = mac 0x3afcc0, ios 0x290470;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x3b0260, ios 0x2906e0;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x3b0320, ios 0x29074c;
}

class SetupSpawnPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x13bc50, ios 0x94ff0;
	virtual void show() = mac 0x13bc00, ios 0x94fa4;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x13b950, ios 0x94e70;
	virtual void textChanged(CCTextInputNode*) = mac 0x13b990, ios 0x94e8c;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x13bda0, ios 0x94ffc;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x13be60, ios 0x95068;
}

class ShareLevelLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x12b830, ios 0x1aea74;
}

class TopArtistsLayer : FLAlertLayer, OnlineListDelegate {
	virtual bool init() = mac 0x192c30, ios 0x104f34;
	virtual void registerWithTouchDispatcher() = mac 0x1935b0, ios 0x1058b0;
	virtual void keyBackClicked() = mac 0x193540, ios 0x105834;
	virtual void show() = mac 0x193e40, ios 0x106198;
	virtual void loadListFinished(cocos2d::CCArray*, char const*) = mac 0x193600, ios 0x1058e8;
	virtual void loadListFailed(char const*) = mac 0x1936f0, ios 0x105958;
	virtual void setupPageInfo(gd::string, char const*) = mac 0x193730, ios 0x105980;
}

class AccountHelpLayer : GJDropDownLayer, GJAccountDelegate, FLAlertLayerProtocol {
	virtual void customSetup() = mac 0x83e80, ios 0xdee48;
	virtual void layerHidden() = mac 0x84b60, ios 0xdf960;
	virtual bool accountStatusChanged() = mac 0x84a20, ios 0xdf8ac;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x84a50, ios 0xdf8b8;
}

class CCAnimatedSprite : cocos2d::CCSprite {
	virtual void setOpacity(unsigned char) = mac 0x1a6af0, ios 0x21c8ec;
	virtual void animationFinished(char const*) = mac 0x1a6ad0, ios 0x21c8d4;
	virtual void animationFinishedO(cocos2d::CCObject*) = mac 0x1a6aa0, ios 0x21c890;
}

class CCPartAnimSprite : cocos2d::CCSprite {
	virtual void setScaleX(float) = mac 0x1bff50, ios 0x3dd94;
	virtual void setScaleY(float) = mac 0x1bff70, ios 0x3ddbc;
	virtual void setScale(float) = mac 0x1bff30, ios 0x3dd6c;
	virtual void setOpacity(unsigned char) = mac 0x1c01d0, ios 0x3dfa8;
	virtual void setBlendFunc(cocos2d::ccBlendFunc) = mac 0x1bff90, ios 0x3dde4;
	virtual void setDisplayFrame(cocos2d::CCSpriteFrame*) = mac 0x1bfa40, ios 0x3d8d4;
	virtual bool isFrameDisplayed(cocos2d::CCSpriteFrame*) = mac 0x1bfc20, ios 0x3da6c;
	virtual cocos2d::CCSpriteFrame* displayFrame() = mac 0x1bfc40, ios 0x3da84;
}

class CCScrollLayerExt : cocos2d::CCLayer {
	virtual void visit() = mac 0x236550, ios 0x220018;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x235ef0, ios 0x21fa58;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x236300, ios 0x21fdf0;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x236020, ios 0x21fb64;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2362a0, ios 0x21fd94;
	virtual void registerWithTouchDispatcher() = mac 0x235eb0, ios 0x21fa20;
	virtual void preVisitWithClippingRect(cocos2d::CCRect) = mac 0x2366a0, ios 0x220154;
	virtual void postVisit() = mac 0x236720, ios 0x2201e8;
}

class CheckpointObject : cocos2d::CCNode {
	virtual bool init() = mac 0x80920, ios 0xb98c8;
}

class ColorSelectPopup : FLAlertLayer, cocos2d::extension::ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
	virtual void keyBackClicked() = mac 0x423540, ios 0x2dbcb4;
	virtual void show() = mac 0x423570, ios 0x2dbcc0;
	virtual void colorValueChanged(cocos2d::ccColor3B) = mac 0x423320, ios 0x2dbb8c;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x423820, ios 0x2dbf2c;
	virtual void textChanged(CCTextInputNode*) = mac 0x423860, ios 0x2dbf48;
	virtual void colorSelectClosed(GJSpecialColorSelect*, int) = mac 0x423c80, ios 0x2dc1c4;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x423fa0, ios 0x2dc460;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x424060, ios 0x2dc4cc;
}

class CustomSongWidget : cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {
	virtual void loadSongInfoFinished(SongInfoObject*) = mac 0x37e400, ios 0x979d0;
	virtual void loadSongInfoFailed(int, GJSongError) = mac 0x37e5a0, ios 0x97a7c;
	virtual void downloadSongFinished(SongInfoObject*) = mac 0x37e7b0, ios 0x97bec;
	virtual void downloadSongFailed(int, GJSongError) = mac 0x37e8c0, ios 0x97c8c;
	virtual void songStateChanged() = mac 0x37e390, ios 0x97984;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x37ea00, ios 0x97d08;
}

class DelayedSpawnNode : cocos2d::CCObject {
}

class EditorPauseLayer : CCBlockLayer, FLAlertLayerProtocol {
	virtual void keyBackClicked() = mac 0x13f320, ios 0x282efc;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x13f3a0, ios 0x282f7c;
	virtual void customSetup() = mac 0x13cc00, ios 0x281134;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x13f1b0, ios 0x282e24;
}

class EffectGameObject : GameObject {
	virtual void customSetup() = mac 0xca370, ios 0x278c98;
	virtual void triggerObject(GJBaseGameLayer*) = mac 0xc9870, ios 0x278380;
	virtual void customObjectSetup(gd::map<gd::string, gd::string>&) = mac 0xca420, ios 0x278d40;
	virtual gd::string getSaveString() = mac 0xcd7e0, ios 0x27b49c;
	virtual void triggerActivated(float) = mac 0xca310, ios 0x278c34;
	virtual void spawnXPosition() = mac 0xca2d0, ios 0x278bfc;
}

class FollowRewardPage : FLAlertLayer, FLAlertLayerProtocol, GameRateDelegate {
	virtual bool init() = mac 0x22f4a0, ios 0x2e08e8;
	virtual void registerWithTouchDispatcher() = mac 0x230eb0, ios 0x2e1d2c;
	virtual void keyBackClicked() = mac 0x230e40, ios 0x2e1cb0;
	virtual void show() = mac 0x230c10, ios 0x2e1ad0;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x230da0, ios 0x2e1c4c;
	virtual void updateRate() = mac 0x230950, ios 0x2e18b8;
}

class GJAccountManager : cocos2d::CCNode {
	virtual bool init() = mac 0x879d0, ios 0x284c04;
}

class GJFlyGroundLayer : GJGroundLayer {
	virtual bool init() = mac 0x356c00;
}

class GJLevelScoreCell : TableViewCell {
	virtual bool init() = mac 0x11dbb0, ios 0xa61a0;
	virtual void draw() = mac 0x11dc00, ios 0xa61e4;
}

class GJMoreGamesLayer : GJDropDownLayer {
	virtual void customSetup() = mac 0x43ae60, ios 0x3a02c;
}

class GJRewardDelegate {
	virtual void rewardsStatusFinished(int);
	virtual void rewardsStatusFailed();
}

class GJUnlockableItem : cocos2d::CCObject {
}

class GameLevelManager : cocos2d::CCNode {
	virtual bool init() = mac 0x2b7ba0, ios 0x518b8;
}

class GameRateDelegate {
	virtual void updateRate();
}

class GameSoundManager : cocos2d::CCNode {
	virtual bool init() = mac 0x3611d0, ios 0x1a2a60;
}

class GameStatsManager : cocos2d::CCNode {
	virtual bool init() = mac 0x39330, ios 0x2433d0;
}

class GhostTrailEffect : cocos2d::CCNode {
	virtual bool init() = mac 0x360730, ios 0x213688;
	virtual void draw() = mac 0x360d20, ios 0x213c00;
}

class KeybindingsLayer : FLAlertLayer {
	virtual bool init() = mac 0x375890;
	virtual void keyBackClicked() = mac 0x376f30;
}

class LevelEditorLayer : GJBaseGameLayer, LevelSettingsDelegate {
	virtual void update(float) = mac 0xa1b70, ios 0x26c164;
	virtual void draw() = mac 0xa2a70, ios 0x26cc8c;
	virtual void updateColor(cocos2d::ccColor3B, float, int, bool, float, cocos2d::ccHSVValue, int, bool, int, EffectGameObject*) = mac 0x9c200, ios 0x268d08;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0xa04e0, ios 0x26b22c;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0x9c590, ios 0x268f44;
	virtual void addToGroup(GameObject*, int, bool) = mac 0x9dab0, ios 0x2691fc;
	virtual void removeFromGroup(GameObject*, int) = mac 0x9db60, ios 0x26928c;
	virtual void timeForXPos(float) = mac 0x9c7d0, ios 0x269130;
	virtual void xPosForTime(float) = mac 0x9c800, ios 0x269148;
	virtual void levelSettingsUpdated() = mac 0x93f30, ios 0x2639e0;
}

class LevelLeaderboard : FLAlertLayer, LeaderboardManagerDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x20e7a0, ios 0x16cd70;
	virtual void keyBackClicked() = mac 0x20e730, ios 0x16ccf4;
	virtual void show() = mac 0x20ec40, ios 0x16d1d0;
	virtual void loadLeaderboardFinished(cocos2d::CCArray*, char const*) = mac 0x20e980, ios 0x16cfc4;
	virtual void loadLeaderboardFailed(char const*) = mac 0x20ead0, ios 0x16d064;
	virtual void updateUserScoreFinished() = mac 0x20e660, ios 0x16cc8c;
	virtual void updateUserScoreFailed() = mac 0x20e690, ios 0x16cc98;
}

class LevelSearchLayer : cocos2d::CCLayer, TextInputDelegate, FLAlertLayerProtocol, DemonFilterDelegate {
	virtual bool init() = mac 0x384770, ios 0x31358;
	virtual void keyBackClicked() = mac 0x3891f0, ios 0x34c14;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x3885f0, ios 0x343e0;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x3887f0, ios 0x34538;
	virtual void textChanged(CCTextInputNode*) = mac 0x388910, ios 0x345b8;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x388580, ios 0x34348;
	virtual void demonFilterSelectClosed(int) = mac 0x388040, ios 0x34024;
}

class LevelSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate, DynamicScrollDelegate {
	virtual void keyBackClicked() = mac 0x23a5e0, ios 0x2c896c;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x23a680, ios 0x2c8978;
	virtual void updatePageWithObject(cocos2d::CCObject*, cocos2d::CCObject*) = mac 0x2390a0, ios 0x2c794c;
	virtual void scrollLayerMoved(cocos2d::CCPoint) = mac 0x23a100, ios 0x2c862c;
}

class LikeItemDelegate {
	virtual void likedItem(LikeItemType, int, bool);
}

class MoreOptionsLayer : FLAlertLayer, TextInputDelegate, GooglePlayDelegate {
	virtual bool init() = mac 0x43f470, ios 0x8779c;
	virtual void keyBackClicked() = mac 0x441f50, ios 0x8a104;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x4420a0, ios 0x8a110;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x442160, ios 0x8a17c;
	virtual void googlePlaySignedIn() = mac 0x442210, ios 0x8a1dc;
}

class NumberInputLayer : FLAlertLayer {
	virtual bool init() = mac 0x255ff0, ios 0x952f4;
	virtual void registerWithTouchDispatcher() = mac 0x256a70, ios 0x95bcc;
	virtual void keyBackClicked() = mac 0x2569c0, ios 0x95b24;
}

class PlayerCheckpoint : cocos2d::CCNode {
	virtual bool init() = mac 0x807a0, ios 0xb97dc;
}

class SetTargetIDLayer : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x15b990, ios 0x13c724;
	virtual void show() = mac 0x15b940, ios 0x13c6d8;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x15b680, ios 0x13c560;
	virtual void textChanged(CCTextInputNode*) = mac 0x15b6c0, ios 0x13c57c;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x15baa0, ios 0x13c730;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x15bb60, ios 0x13c79c;
}

class SetupRotatePopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x2f47c0, ios 0x1a190c;
	virtual void show() = mac 0x2f4770, ios 0x1a18c0;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x2f45d0, ios 0x1a17d8;
	virtual void textChanged(CCTextInputNode*) = mac 0x2f4610, ios 0x1a17f4;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x2f48b0, ios 0x1a1918;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x2f4970, ios 0x1a1984;
}

class SliderTouchLogic : cocos2d::CCMenu {
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x18d630, ios 0x21113c;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x18d760, ios 0x2111c0;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x18d730, ios 0x2111b4;
	virtual void registerWithTouchDispatcher() = mac 0x18da90, ios 0x211338;
}

class SongOptionsLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0xf33c0, ios 0xc7640;
}

class TextAreaDelegate {
	virtual void fadeInTextFinished(TextArea*);
}

class UserInfoDelegate {
	virtual void getUserInfoFinished(GJUserScore*);
	virtual void getUserInfoFailed(int);
	virtual void userInfoChanged(GJUserScore*);
}

class UserListDelegate {
	virtual void getUserListFinished(cocos2d::CCArray*, UserListType);
	virtual void getUserListFailed(UserListType, GJErrorCode);
	virtual void userListChanged(cocos2d::CCArray*, UserListType);
	virtual void forceReloadList(UserListType);
}

class WorldSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate {
	virtual void onExit() = mac 0x201320;
	virtual void keyBackClicked() = mac 0x201250;
	virtual void scrollLayerWillScrollToPage(BoomScrollLayer*, int) = mac 0x201140;
	virtual void scrollLayerScrolledToPage(BoomScrollLayer*, int) = mac 0x201030;
	virtual void scrollLayerMoved(cocos2d::CCPoint) = mac 0x201370;
}

class AccountLoginLayer : FLAlertLayer, TextInputDelegate, GJAccountLoginDelegate, FLAlertLayerProtocol {
	virtual void registerWithTouchDispatcher() = mac 0x25d7e0, ios 0x434d0;
	virtual void keyBackClicked() = mac 0x25d6a0, ios 0x43380;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x25d4d0, ios 0x43288;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x25d9c0, ios 0x43650;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x25d820, ios 0x43508;
	virtual void textChanged(CCTextInputNode*) = mac 0x25d9e0, ios 0x43658;
	virtual bool loginAccountFinished(int, int) = mac 0x25ce20, ios 0x42d90;
	virtual bool loginAccountFailed(AccountError) = mac 0x25d190, ios 0x42fc4;
}

class AchievementsLayer : GJDropDownLayer {
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x1be380, ios 0x2de1a0;
	virtual void customSetup() = mac 0x1bdea0, ios 0x2dda20;
}

class AudioEffectsLayer : cocos2d::CCLayerColor {
	virtual void draw() = mac 0x2722a0, ios 0x2a5980;
	virtual void updateTweenAction(float, char const*) = mac 0x272170, ios 0x2a5834;
}

class CCMenuItemToggler : cocos2d::CCMenuItem {
	virtual void activate() = mac 0x38bc0, ios 0xf59ac;
	virtual void selected() = mac 0x38b80, ios 0xf596c;
	virtual void unselected() = mac 0x38c00, ios 0xf59ec;
	virtual void setEnabled(bool) = mac 0x38c40, ios 0xf5a2c;
}

class ColorActionSprite : cocos2d::CCNode {
	virtual bool init() = mac 0x175370, ios 0x9b6c;
}

class DungeonBarsSprite : cocos2d::CCNode {
	virtual bool init() = mac 0x26c2b0, ios 0x231e8c;
	virtual void visit() = mac 0x26c330, ios 0x231f0c;
}

class EditTriggersPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x1458a0, ios 0x200b4c;
	virtual void show() = mac 0x145850, ios 0x200b00;
}

class GJAccountDelegate {
	virtual bool accountStatusChanged();
}

class GJColorSetupLayer : FLAlertLayer, ColorSelectDelegate, FLAlertLayerProtocol {
	virtual void keyBackClicked() = mac 0xf0210, ios 0xf4fd0;
	virtual void colorSelectClosed(cocos2d::CCNode*) = mac 0xf01e0, ios 0xf4fc4;
}

class GJRotationControl : cocos2d::CCLayer {
	virtual bool init() = mac 0x31510, ios 0x2c4914;
	virtual void draw() = mac 0x31ac0, ios 0x2c4ce8;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x31790, ios 0x2c4b20;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x318b0, ios 0x2c4c1c;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x31990, ios 0x2c4c98;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x31a80, ios 0x2c4ccc;
}

class GooglePlayManager : cocos2d::CCNode {
	virtual bool init() = mac 0x246130, ios 0x26a84;
}

class LeaderboardsLayer : cocos2d::CCLayer, LeaderboardManagerDelegate {
	virtual void keyBackClicked() = mac 0x2a11c0, ios 0x29796c;
	virtual void updateUserScoreFinished() = mac 0x2a07c0, ios 0x29712c;
	virtual void updateUserScoreFailed() = mac 0x2a0820, ios 0x297160;
	virtual void loadLeaderboardFinished(cocos2d::CCArray*, char const*) = mac 0x2a08c0, ios 0x2971bc;
	virtual void loadLeaderboardFailed(char const*) = mac 0x2a0fa0, ios 0x2976e4;
}

class LevelBrowserLayer : cocos2d::CCLayer, LevelManagerDelegate, FLAlertLayerProtocol, SetIDPopupDelegate, SetTextPopupDelegate {
	virtual void onEnter() = mac 0x255b70, ios 0x2d4ab8;
	virtual void onEnterTransitionDidFinish() = mac 0x255be0, ios 0x2d4b20;
	virtual void keyBackClicked() = mac 0x255630, ios 0x2d46c4;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x255680, ios 0x2d4710;
	virtual void loadLevelsFinished(cocos2d::CCArray*, char const*) = mac 0x254e10, ios 0x2d3fdc;
	virtual void loadLevelsFailed(char const*) = mac 0x254f60, ios 0x2d4080;
	virtual void setupPageInfo(gd::string, char const*) = mac 0x255050, ios 0x2d40fc;
	virtual void setTextPopupClosed(SetTextPopup*, gd::string) = mac 0x255760, ios 0x2d47d0;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x255840, ios 0x2d4858;
	virtual void setIDPopupClosed(SetIDPopup*, int) = mac 0x2554f0, ios 0x2d45c0;
}

class LevelFeatureLayer : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x297a00;
}

class LocalLevelManager : GManager {
	virtual bool init() = mac 0x35dfc0, ios 0x127888;
	virtual void encodeDataTo(DS_Dictionary*) = mac 0x35ed60, ios 0x128534;
	virtual void dataLoaded(DS_Dictionary*) = mac 0x35eda0, ios 0x128574;
	virtual void firstLoad() = mac 0x35ed10, ios 0x12850c;
}

class PromoInterstitial : FLAlertLayer {
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x169e80;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x16a0c0;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x169fa0;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x16a040;
	virtual void keyBackClicked() = mac 0x169cf0;
	virtual void show() = mac 0x169cc0;
}

class PulseEffectAction : cocos2d::CCNode {
}

class PurchaseItemPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x1a2fc0, ios 0xcb994;
}

class RateLevelDelegate {
	virtual void rateLevelClosed();
}

class RewardUnlockLayer : FLAlertLayer, CurrencyRewardDelegate {
	virtual void keyBackClicked() = mac 0xf81b0, ios 0x127258;
	virtual void currencyWillExit(CurrencyRewardLayer*) = mac 0xf8170, ios 0x127230;
}

class SecretNumberLayer : cocos2d::CCLayer {
	virtual bool init() = mac 0x266c30, ios 0x22d758;
}

class SelectArtDelegate {
	virtual void selectArtClosed(SelectArtLayer*);
}

class SetupOpacityPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x34bf0, ios 0x18144;
	virtual void show() = mac 0x34ba0, ios 0x180f8;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x34a20, ios 0x1805c;
	virtual void textChanged(CCTextInputNode*) = mac 0x34a60, ios 0x18078;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x34e50, ios 0x1822c;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x34f10, ios 0x18298;
}

class ShareCommentLayer : FLAlertLayer, TextInputDelegate, UploadActionDelegate, UploadPopupDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x350590, ios 0x12f45c;
	virtual void keyBackClicked() = mac 0x3504e0, ios 0x12f3b4;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x350690, ios 0x12f520;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x3505d0, ios 0x12f494;
	virtual void textChanged(CCTextInputNode*) = mac 0x3507b0, ios 0x12f64c;
	virtual void uploadActionFinished(int, int) = mac 0x3509f0, ios 0x12f7dc;
	virtual void uploadActionFailed(int, int) = mac 0x350ad0, ios 0x12f888;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x350ba0, ios 0x12f934;
}

class SpriteDescription : cocos2d::CCObject {
}

class TableViewDelegate {
	virtual void willTweenToIndexPath(CCIndexPath&, TableViewCell*, TableView*) = mac 0x120460;
	virtual void didEndTweenToIndexPath(CCIndexPath&, TableView*) = mac 0x120470;
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*);
	virtual void TableViewDidDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) = mac 0x120480;
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*);
}

class TextInputDelegate {
	virtual void textChanged(CCTextInputNode*) = mac 0x1573f0;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x6200;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x157400;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0xe1810;
	virtual void textInputReturn(CCTextInputNode*) = mac 0xe1820;
	virtual bool allowTextInput(CCTextInputNode*) = mac 0x6210, ios 0x182f8;
}

class TouchToggleAction : cocos2d::CCNode {
}

class UploadActionPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x14ca10, ios 0x2065f8;
}

class VideoOptionsLayer : FLAlertLayer {
	virtual bool init() = mac 0x442460, ios 0x8a2d0;
	virtual void keyBackClicked() = mac 0x443f50, ios 0x8b670;
}

class AchievementManager : cocos2d::CCNode {
	virtual bool init() = mac 0x4244c0, ios 0x74d04;
}

class AnimatedGameObject : GameObject, AnimatedSpriteDelegate, SpritePartDelegate {
	virtual void setOpacity(unsigned char) = mac 0xc8640, ios 0x2770fc;
	virtual void resetObject() = mac 0xc9720, ios 0x2782a4;
	virtual void activateObject() = mac 0xc84d0, ios 0x276fec;
	virtual void deactivateObject(bool) = mac 0xc85e0, ios 0x2770ac;
	virtual void setObjectColor(cocos2d::ccColor3B const&) = mac 0xc8720, ios 0x2771c8;
	virtual void animationFinished(char const*) = mac 0xc8750, ios 0x2771fc;
	virtual cocos2d::CCSpriteFrame* displayFrameChanged(cocos2d::CCObject*, gd::string) = mac 0xc9160, ios 0x277c58;
}

class AnimatedShopKeeper : CCAnimatedSprite {
	virtual void animationFinished(char const*) = mac 0x1a3130, ios 0xcbaf8;
}

class ColorChannelSprite : cocos2d::CCSprite {
	virtual bool init() = mac 0x16deb0, ios 0x6e04;
}

class ColorSetupDelegate {
	virtual void colorSetupClosed(int);
}

class ConfigureHSVWidget : cocos2d::CCNode {
}

class CountTriggerAction : cocos2d::CCNode {
}

class EditorOptionsLayer : GJOptionsLayer {
	virtual bool init() = mac 0x147420, ios 0x2024e4;
	virtual void setupOptions() = mac 0x147440, ios 0x2024f0;
	virtual void onClose(cocos2d::CCObject*) = mac 0x147c30, ios 0x202d00;
}

class FriendRequestPopup : FLAlertLayer, UploadActionDelegate, UploadPopupDelegate, FLAlertLayerProtocol {
	virtual void keyBackClicked() = mac 0x14e2a0, ios 0x207b2c;
	virtual void uploadActionFinished(int, int) = mac 0x14e390, ios 0x207b38;
	virtual void uploadActionFailed(int, int) = mac 0x14e560, ios 0x207d08;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x14e640, ios 0x207dc8;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x14e7c0, ios 0x207e40;
}

class FriendsProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, UserListDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x3aa6e0, ios 0x133634;
	virtual void keyBackClicked() = mac 0x3aa5e0, ios 0x133628;
	virtual void getUserListFinished(cocos2d::CCArray*, UserListType) = mac 0x3aa240, ios 0x1333c4;
	virtual void getUserListFailed(UserListType, GJErrorCode) = mac 0x3aa390, ios 0x1334d8;
	virtual void userListChanged(cocos2d::CCArray*, UserListType) = mac 0x3aa4a0, ios 0x13357c;
	virtual void forceReloadList(UserListType) = mac 0x3aa4e0, ios 0x133598;
}

class GJCommentListLayer : cocos2d::CCLayerColor {
}

class GJMoveCommandLayer : FLAlertLayer, TextInputDelegate, ConfigureValuePopupDelegate {
	virtual void keyBackClicked() = mac 0x374970, ios 0x1ad330;
	virtual void valuePopupClosed(ConfigureValuePopup*, float) = mac 0x373c20, ios 0x1ac9e0;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x374430, ios 0x1ad03c;
	virtual void textChanged(CCTextInputNode*) = mac 0x374470, ios 0x1ad058;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x374b50, ios 0x1ad33c;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x374c10, ios 0x1ad3a8;
}

class GJPurchaseDelegate {
	virtual void didPurchaseItem(GJStoreItem*);
}

class GameEffectsManager : cocos2d::CCNode {
}

class GooglePlayDelegate {
	virtual void googlePlaySignedIn();
}

class GroupCommandObject : cocos2d::CCNode {
	virtual bool init() = mac 0x16e590, ios 0x7430;
	virtual void updateTweenAction(float, char const*) = mac 0x16ead0, ios 0x7878;
}

class KeybindingsManager : cocos2d::CCNode {
	virtual bool init() = mac 0x289a00, ios 0x253ddc;
}

class LevelSettingsLayer : FLAlertLayer, ColorSelectDelegate, SelectArtDelegate, FLAlertLayerProtocol, CustomSongLayerDelegate {
	virtual void registerWithTouchDispatcher() = mac 0xac070, ios 0x27421c;
	virtual void keyBackClicked() = mac 0xac7b0, ios 0x27461c;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0xac260, ios 0x27432c;
	virtual void colorSelectClosed(cocos2d::CCNode*) = mac 0xac100, ios 0x274274;
	virtual void customSongLayerClosed() = mac 0xac340, ios 0x2743ac;
	virtual void selectArtClosed(SelectArtLayer*) = mac 0xac5e0, ios 0x274498;
}

class OnlineListDelegate {
	virtual void loadListFinished(cocos2d::CCArray*, char const*);
	virtual void loadListFailed(char const*);
	virtual void setupPageInfo(gd::string, char const*);
}

class SetIDPopupDelegate {
	virtual void setIDPopupClosed(SetIDPopup*, int);
}

class SpawnTriggerAction : cocos2d::CCNode {
}

class SpritePartDelegate {
	virtual cocos2d::CCSpriteFrame* displayFrameChanged(cocos2d::CCObject*, gd::string);
}

class AchievementNotifier : cocos2d::CCNode {
	virtual bool init() = mac 0x464e90, ios 0x13f488;
}

class CCAnimateFrameCache : cocos2d::CCObject {
}

class CollisionBlockPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x1317e0, ios 0x44cd0;
	virtual void show() = mac 0x131790, ios 0x44c84;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x131630, ios 0x44be8;
	virtual void textChanged(CCTextInputNode*) = mac 0x131670, ios 0x44c04;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x1318f0, ios 0x44cdc;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x1319b0, ios 0x44d48;
}

class ColorSelectDelegate {
	virtual void colorSelectClosed(cocos2d::CCNode*);
}

class CommunityCreditNode : cocos2d::CCNode {
}

class ConfigureValuePopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x382c00, ios 0xcda2c;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x382a90, ios 0xcd928;
	virtual void textChanged(CCTextInputNode*) = mac 0x382ad0, ios 0xcd944;
}

class CurrencyRewardLayer : cocos2d::CCLayer {
	virtual void update(float) = mac 0x44a5c0, ios 0x23e138;
}

class DemonFilterDelegate {
	virtual void demonFilterSelectClosed(int);
}

class EditGameObjectPopup : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x154070, ios 0x20c5d8;
	virtual void show() = mac 0x154020, ios 0x20c58c;
}

class FRequestProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, FriendRequestDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x437300, ios 0x1182ac;
	virtual void keyBackClicked() = mac 0x4372d0, ios 0x1182a0;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x437200, ios 0x1181a4;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x437340, ios 0x1182e4;
	virtual void uploadActionFinished(int, int) = mac 0x4373a0, ios 0x118354;
	virtual void uploadActionFailed(int, int) = mac 0x437500, ios 0x1184f0;
	virtual void loadFRequestsFinished(cocos2d::CCArray*, char const*) = mac 0x437840, ios 0x118860;
	virtual void loadFRequestsFailed(char const*, GJErrorCode) = mac 0x437930, ios 0x1188e4;
	virtual void setupPageInfo(gd::string, char const*) = mac 0x437ab0, ios 0x1189b8;
	virtual void forceReloadRequests(bool) = mac 0x437a60, ios 0x118984;
}

class GJChallengeDelegate {
	virtual void challengeStatusFinished();
	virtual void challengeStatusFailed();
}

class GJWriteMessagePopup : FLAlertLayer, TextInputDelegate, UploadMessageDelegate, UploadPopupDelegate, FLAlertLayerProtocol {
	virtual void registerWithTouchDispatcher() = mac 0x1527b0, ios 0x20b2dc;
	virtual void keyBackClicked() = mac 0x152780, ios 0x20b2c8;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x152830, ios 0x20b330;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x1527f0, ios 0x20b314;
	virtual void textChanged(CCTextInputNode*) = mac 0x152990, ios 0x20b43c;
	virtual void uploadMessageFinished(int) = mac 0x152fc0, ios 0x20ba34;
	virtual void uploadMessageFailed(int) = mac 0x1530a0, ios 0x20bab0;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x153170, ios 0x20bb5c;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x1532b0, ios 0x20bc1c;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x153390, ios 0x20bca4;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x153450, ios 0x20bd10;
}

class GauntletSelectLayer : cocos2d::CCLayer, BoomScrollLayerDelegate, LevelManagerDelegate {
	virtual void onExit() = mac 0x2eb490, ios 0x12176c;
	virtual void keyBackClicked() = mac 0x2eb370, ios 0x121720;
	virtual void scrollLayerWillScrollToPage(BoomScrollLayer*, int) = mac 0x2eb260, ios 0x121714;
	virtual void scrollLayerScrolledToPage(BoomScrollLayer*, int) = mac 0x2eb150, ios 0x121708;
	virtual void loadLevelsFinished(cocos2d::CCArray*, char const*) = mac 0x2ea630, ios 0x120df0;
	virtual void loadLevelsFailed(char const*) = mac 0x2ea820, ios 0x120fac;
}

class GravityEffectSprite : cocos2d::CCSprite {
	virtual bool init() = mac 0x80b20, ios 0xb99b8;
	virtual void draw() = mac 0x80d80, ios 0xb9be8;
}

class LevelDeleteDelegate {
	virtual void levelDeleteFinished(int);
	virtual void levelDeleteFailed(int);
}

class LevelSettingsObject : cocos2d::CCNode {
	virtual bool init() = mac 0xa5690, ios 0x26ebb8;
}

class LevelUpdateDelegate {
	virtual void levelUpdateFinished(GJGameLevel*, UpdateResponse);
	virtual void levelUpdateFailed(int);
}

class LevelUploadDelegate {
	virtual void levelUploadFinished(GJGameLevel*);
	virtual void levelUploadFailed(GJGameLevel*);
}

class MessageListDelegate {
	virtual void loadMessagesFinished(cocos2d::CCArray*, char const*);
	virtual void loadMessagesFailed(char const*, GJErrorCode);
	virtual void forceReloadMessages(bool);
	virtual void setupPageInfo(gd::string, char const*);
}

class MessagesProfilePage : FLAlertLayer, FLAlertLayerProtocol, UploadActionDelegate, UploadPopupDelegate, MessageListDelegate {
	virtual void registerWithTouchDispatcher() = mac 0x107eb0, ios 0x12b1a8;
	virtual void keyBackClicked() = mac 0x107e80, ios 0x12b19c;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x107b20, ios 0x12adf4;
	virtual void onClosePopup(UploadActionPopup*) = mac 0x107b90, ios 0x12ae84;
	virtual void uploadActionFinished(int, int) = mac 0x107bf0, ios 0x12aef4;
	virtual void uploadActionFailed(int, int) = mac 0x107d50, ios 0x12b090;
	virtual void loadMessagesFinished(cocos2d::CCArray*, char const*) = mac 0x108150, ios 0x12b4a0;
	virtual void loadMessagesFailed(char const*, GJErrorCode) = mac 0x108280, ios 0x12b5b4;
	virtual void forceReloadMessages(bool) = mac 0x1083b0, ios 0x12b654;
	virtual void setupPageInfo(gd::string, char const*) = mac 0x108400, ios 0x12b688;
}

class MultilineBitmapFont : cocos2d::CCSprite {
	virtual void setOpacity(unsigned char) = mac 0x24f030, ios 0x158460;
}

class NumberInputDelegate {
	virtual void numberInputClosed(NumberInputLayer*);
}

class OpacityEffectAction : cocos2d::CCNode {
}

class SetupAnimationPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x20ade0, ios 0x2a4efc;
	virtual void show() = mac 0x20ad90, ios 0x2a4eb0;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x20aaf0, ios 0x2a4dbc;
	virtual void textChanged(CCTextInputNode*) = mac 0x20ab30, ios 0x2a4dd8;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x20af30, ios 0x2a4f08;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x20aff0, ios 0x2a4f74;
}

class TableViewDataSource {
	virtual void numberOfSectionsInTableView(TableView*);
	virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&);
}

class ToggleTriggerAction : cocos2d::CCNode {
}

class UploadPopupDelegate {
	virtual void onClosePopup(UploadActionPopup*) = mac 0x3aa740;
}

class AccountRegisterLayer : FLAlertLayer, TextInputDelegate, GJAccountRegisterDelegate, FLAlertLayerProtocol {
	virtual bool init() = mac 0x2575d0, ios 0x3f908;
	virtual void registerWithTouchDispatcher() = mac 0x25a910, ios 0x415f8;
	virtual void keyBackClicked() = mac 0x25a730, ios 0x414b4;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x25a380, ios 0x41350;
	virtual void textInputOpened(CCTextInputNode*) = mac 0x25b420, ios 0x41c94;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x25a950, ios 0x41630;
	virtual void textChanged(CCTextInputNode*) = mac 0x25b440, ios 0x41c9c;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x25b460, ios 0x41ca4;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x25b520, ios 0x41d10;
	virtual bool allowTextInput(CCTextInputNode*) = mac 0x25b5d0, ios 0x41d70;
	virtual void registerAccountFinished() = mac 0x259bf0, ios 0x40d64;
	virtual void registerAccountFailed(AccountError) = mac 0x259d70, ios 0x40f00;
}

class CCCircleWaveDelegate {
	virtual void circleWaveWillBeRemoved(CCCircleWave*);
}

class CommunityCreditsPage : FLAlertLayer {
	virtual bool init() = mac 0x19b2b0, ios 0xff360;
	virtual void registerWithTouchDispatcher() = mac 0x19e9f0, ios 0x102184;
	virtual void keyBackClicked() = mac 0x19e980, ios 0x102108;
	virtual void show() = mac 0x19e7e0, ios 0x101f8c;
}

class CustomizeObjectLayer : FLAlertLayer, TextInputDelegate, HSVWidgetPopupDelegate, ColorSelectDelegate, ColorSetupDelegate {
	virtual void keyBackClicked() = mac 0xe16c0, ios 0x3105c;
	virtual void textInputOpened(CCTextInputNode*) = mac 0xe1280, ios 0x30d30;
	virtual void textInputClosed(CCTextInputNode*) = mac 0xe1430, ios 0x30e64;
	virtual void textChanged(CCTextInputNode*) = mac 0xe1470, ios 0x30e80;
	virtual void hsvPopupClosed(HSVWidgetPopup*, cocos2d::ccHSVValue) = mac 0xe1050, ios 0x30b38;
	virtual void colorSelectClosed(cocos2d::CCNode*) = mac 0xe0c60, ios 0x309a0;
	virtual void colorSetupClosed(int) = mac 0xe0850, ios 0x306c8;
}

class FLAlertLayerProtocol {
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0xf02c0;
}

class GJDailyLevelDelegate {
	virtual void dailyStatusFinished(bool);
	virtual void dailyStatusFailed(bool);
}

class GJFollowCommandLayer : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x16da20, ios 0xc223c;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x16d440, ios 0xc1f88;
	virtual void textChanged(CCTextInputNode*) = mac 0x16d480, ios 0xc1fa4;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x16dc00, ios 0xc2248;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x16dcc0, ios 0xc22b4;
}

class GJRotateCommandLayer : FLAlertLayer, TextInputDelegate, ConfigureValuePopupDelegate {
	virtual void keyBackClicked() = mac 0x5e30, ios 0x2ce328;
	virtual void valuePopupClosed(ConfigureValuePopup*, float) = mac 0x5160, ios 0x2cdaf4;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x5890, ios 0x2ce090;
	virtual void textChanged(CCTextInputNode*) = mac 0x58d0, ios 0x2ce0ac;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x6010, ios 0x2ce334;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x60d0, ios 0x2ce3a0;
}

class GJSpecialColorSelect : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x383ca0, ios 0x114094;
}

class LevelCommentDelegate {
	virtual void loadCommentsFinished(cocos2d::CCArray*, char const*);
	virtual void loadCommentsFailed(char const*);
	virtual void updateUserScoreFinished();
	virtual void setupPageInfo(gd::string, char const*);
}

class LevelManagerDelegate {
	virtual void loadLevelsFinished(cocos2d::CCArray*, char const*);
	virtual void loadLevelsFailed(char const*);
	virtual void setupPageInfo(gd::string, char const*) = mac 0x1d5ec0;
}

class MusicDelegateHandler : cocos2d::CCNode {
}

class MusicDownloadManager : cocos2d::CCNode, PlatformDownloadDelegate {
	virtual bool init() = mac 0x2ef100, ios 0xd1670;
}

class ParentalOptionsLayer : FLAlertLayer {
	virtual bool init() = mac 0x445da0, ios 0x8d484;
	virtual void keyBackClicked() = mac 0x4472d0, ios 0x8e8b4;
}

class SetTextPopupDelegate {
	virtual void setTextPopupClosed(SetTextPopup*, gd::string);
}

class TeleportPortalObject : GameObject {
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0xdad00, ios 0x27e3e4;
	virtual void setRotation(float) = mac 0xdadb0, ios 0x27e48c;
	virtual void setStartPos(cocos2d::CCPoint) = mac 0xdab50, ios 0x27e254;
	virtual void customObjectSetup(gd::map<gd::string, gd::string>&) = mac 0xdb640, ios 0x27e858;
	virtual gd::string getSaveString() = mac 0xdaf00, ios 0x27e5d0;
	virtual void addToGroup(int) = mac 0xdae40, ios 0x27e508;
	virtual void removeFromGroup(int) = mac 0xdaea0, ios 0x27e56c;
	virtual void setRotation2(float) = mac 0xdae10, ios 0x27e4fc;
	virtual void addToGroup2(int) = mac 0xdae20, ios 0x27e500;
	virtual void removeFromGroup2(int) = mac 0xdae30, ios 0x27e504;
}

class UploadActionDelegate {
	virtual void uploadActionFinished(int, int) = mac 0x3aa720;
	virtual void uploadActionFailed(int, int) = mac 0x3aa730;
}

class CCMenuItemSpriteExtra : cocos2d::CCMenuItemSprite {
	virtual void activate() = mac 0x125730, ios 0x13cbd8;
	virtual void selected() = mac 0x125840, ios 0x13cd28;
	virtual void unselected() = mac 0x125a70, ios 0x13cf64;
}

class CommentUploadDelegate {
	virtual void commentUploadFinished(int);
	virtual void commentUploadFailed(int, CommentError);
	virtual void commentDeleteFailed(int, int) = mac 0x45ac60;
}

class CreateGuidelinesLayer : FLAlertLayer, FLAlertLayerProtocol {
	virtual void update(float) = mac 0x299c50, ios 0x3c6a8;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x299dd0, ios 0x3c840;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x299f50, ios 0x3c8e4;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x299f70, ios 0x3c8ec;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x29a030, ios 0x3c9b4;
	virtual void registerWithTouchDispatcher() = mac 0x29a070, ios 0x3c9d0;
	virtual void keyBackClicked() = mac 0x299ce0, ios 0x3c758;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x29a0b0, ios 0x3ca08;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x299c70, ios 0x3c6b8;
	virtual void keyUp(cocos2d::enumKeyCodes) = mac 0x29a180, ios 0x3ca3c;
}

class DynamicScrollDelegate {
}

class FriendRequestDelegate {
	virtual void loadFRequestsFinished(cocos2d::CCArray*, char const*);
	virtual void loadFRequestsFailed(char const*, GJErrorCode);
	virtual void setupPageInfo(gd::string, char const*);
	virtual void forceReloadRequests(bool);
}

class GJAccountSyncDelegate {
	virtual void syncAccountFinished();
	virtual void syncAccountFailed(BackupAccountError);
}

class GJPFollowCommandLayer : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x1b2590, ios 0xd7c18;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x1b1d80, ios 0xd7840;
	virtual void textChanged(CCTextInputNode*) = mac 0x1b1dc0, ios 0xd785c;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x1b27a0, ios 0xd7c24;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x1b2860, ios 0xd7c90;
}

class LevelDownloadDelegate {
	virtual void levelDownloadFinished(GJGameLevel*);
	virtual void levelDownloadFailed(int);
}

class LevelSettingsDelegate {
	virtual void levelSettingsUpdated();
}

class ListButtonBarDelegate {
	virtual void listButtonBarSwitchedPage(ListButtonBar*, int);
}

class MoreVideoOptionsLayer : FLAlertLayer {
	virtual bool init() = mac 0x444150, ios 0x8b7e0;
	virtual void keyBackClicked() = mac 0x445ba0, ios 0x8d314;
}

class MusicDownloadDelegate {
	virtual void loadSongInfoFinished(SongInfoObject*) = mac 0x320a0;
	virtual void loadSongInfoFailed(int, GJSongError) = mac 0x320b0;
	virtual void downloadSongFinished(SongInfoObject*) = mac 0x320c0;
	virtual void downloadSongFailed(int, GJSongError) = mac 0x320d0;
	virtual void songStateChanged();
}

class ObjectDecoderDelegate {
	virtual void getDecodedObject(int, DS_Dictionary*);
}

class SetupTouchTogglePopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x159850, ios 0x167b34;
	virtual void show() = mac 0x159800, ios 0x167ae8;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x159660, ios 0x167a30;
	virtual void textChanged(CCTextInputNode*) = mac 0x1596a0, ios 0x167a4c;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x159960, ios 0x167b40;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x159a20, ios 0x167bac;
}

class TriggerEffectDelegate {
}

class UploadMessageDelegate {
	virtual void uploadMessageFinished(int);
	virtual void uploadMessageFailed(int);
}

class AnimatedSpriteDelegate {
	virtual void animationFinished(char const*);
}

class CollisionTriggerAction : cocos2d::CCNode {
}

class ColorSelectLiveOverlay : FLAlertLayer {
	virtual void keyBackClicked() = mac 0x2e4550, ios 0x92180;
	virtual void show() = mac 0x2e4620, ios 0x9218c;
}

class CurrencyRewardDelegate {
	virtual void currencyWillExit(CurrencyRewardLayer*);
}

class DemonFilterSelectLayer : FLAlertLayer {
	virtual bool init() = mac 0x38b9d0, ios 0x36d20;
	virtual void keyBackClicked() = mac 0x38c060, ios 0x372b4;
}

class GJAccountLoginDelegate {
	virtual bool loginAccountFinished(int, int);
	virtual bool loginAccountFailed(AccountError);
}

class GJAccountSettingsLayer : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x14b200, ios 0x205460;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x14abe0, ios 0x2050d0;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x14aca0, ios 0x20513c;
}

class GJScaleControlDelegate {
	virtual void scaleChanged(float);
	virtual void scaleChangeBegin();
	virtual void scaleChangeEnded();
}

class HSVWidgetPopupDelegate {
	virtual void hsvPopupClosed(HSVWidgetPopup*, cocos2d::ccHSVValue);
}

class SetupAnimSettingsPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x599f0, ios 0x210a38;
	virtual void show() = mac 0x599a0, ios 0x2109ec;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x597b0, ios 0x2108d4;
	virtual void textChanged(CCTextInputNode*) = mac 0x597f0, ios 0x2108f0;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x59ae0, ios 0x210a44;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x59ba0, ios 0x210ab0;
}

class SetupCountTriggerPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x15ec70, ios 0x908e0;
	virtual void show() = mac 0x15ec20, ios 0x90894;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x15e960, ios 0x90744;
	virtual void textChanged(CCTextInputNode*) = mac 0x15e9a0, ios 0x90760;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x15ede0, ios 0x908ec;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x15eea0, ios 0x90958;
}

class SetupInstantCountPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x355600, ios 0x11e57c;
	virtual void show() = mac 0x3555b0, ios 0x11e530;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x355230, ios 0x11e3e0;
	virtual void textChanged(CCTextInputNode*) = mac 0x355270, ios 0x11e3fc;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x355770, ios 0x11e588;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x355830, ios 0x11e5f4;
}

class SetupObjectTogglePopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x1c27f0, ios 0x103c00;
	virtual void show() = mac 0x1c27a0, ios 0x103bb4;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x1c2620, ios 0x103b18;
	virtual void textChanged(CCTextInputNode*) = mac 0x1c2660, ios 0x103b34;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x1c2900, ios 0x103c0c;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x1c29c0, ios 0x103c78;
}

class SpriteAnimationManager : cocos2d::CCNode {
}

class BoomScrollLayerDelegate {
	virtual void scrollLayerScrollingStarted(BoomScrollLayer*) = mac 0x204370;
	virtual void scrollLayerScrolledToPage(BoomScrollLayer*, int) = mac 0x23cf70;
	virtual void scrollLayerMoved(cocos2d::CCPoint) = mac 0x2eca30;
	virtual void scrollLayerWillScrollToPage(BoomScrollLayer*, int) = mac 0x23cf80;
}

class CustomSongLayerDelegate {
	virtual void customSongLayerClosed();
}

class DownloadMessageDelegate {
	virtual void downloadMessageFinished(GJUserMessage*);
	virtual void downloadMessageFailed(int);
}

class GJAccountBackupDelegate {
	virtual bool backupAccountFinished();
	virtual bool backupAccountFailed(BackupAccountError);
}

class GJDropDownLayerDelegate {
	virtual void dropDownLayerWillClose(GJDropDownLayer*);
}

class SetupPickupTriggerPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x37f00, ios 0x28fec;
	virtual void show() = mac 0x37eb0, ios 0x28fa0;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x37c60, ios 0x28e90;
	virtual void textChanged(CCTextInputNode*) = mac 0x37ca0, ios 0x28eac;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x38050, ios 0x28ff8;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x38110, ios 0x29064;
}

class ShareLevelSettingsLayer : FLAlertLayer, NumberInputDelegate {
	virtual void keyBackClicked() = mac 0x12db50, ios 0x1b07b0;
	virtual void numberInputClosed(NumberInputLayer*) = mac 0x12da50, ios 0x1b06e0;
}

class CCScrollLayerExtDelegate {
	virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt*);
	virtual void scrollViewDidEndDecelerating(CCScrollLayerExt*);
	virtual void scrollViewTouchMoving(CCScrollLayerExt*);
	virtual void scrollViewDidEndMoving(CCScrollLayerExt*);
	virtual void scrollViewTouchBegin(CCScrollLayerExt*) = mac 0x381920;
	virtual void scrollViewTouchEnd(CCScrollLayerExt*) = mac 0x381930;
}

class PlatformDownloadDelegate {
	virtual void downloadFinished(char const*) = mac 0x2f2db0;
	virtual void downloadFailed(char const*) = mac 0x2f2dc0;
}

class SetupInteractObjectPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x29c550, ios 0x2d0358;
	virtual void show() = mac 0x29c500, ios 0x2d030c;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x29c270, ios 0x2d021c;
	virtual void textChanged(CCTextInputNode*) = mac 0x29c2b0, ios 0x2d0238;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x29c6a0, ios 0x2d0364;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x29c760, ios 0x2d03d0;
}

class GJAccountRegisterDelegate {
	virtual void registerAccountFinished();
	virtual void registerAccountFailed(AccountError);
}

class GJAccountSettingsDelegate {
	virtual void updateSettingsFinished();
	virtual void updateSettingsFailed();
}

class GJRotationControlDelegate {
	virtual void angleChanged(float);
	virtual void angleChangeBegin();
	virtual void angleChangeEnded();
}

class LeaderboardManagerDelegate {
	virtual void updateUserScoreFinished();
	virtual void updateUserScoreFailed();
	virtual void loadLeaderboardFinished(cocos2d::CCArray*, char const*) = mac 0x464aa0;
	virtual void loadLeaderboardFailed(char const*) = mac 0x464ab0;
}

class SetupCollisionTriggerPopup : FLAlertLayer, TextInputDelegate {
	virtual void keyBackClicked() = mac 0x1d8880, ios 0x154ccc;
	virtual void show() = mac 0x1d8830, ios 0x154c80;
	virtual void textInputClosed(CCTextInputNode*) = mac 0x1d8490, ios 0x154b30;
	virtual void textChanged(CCTextInputNode*) = mac 0x1d84d0, ios 0x154b4c;
	virtual void textInputShouldOffset(CCTextInputNode*, float) = mac 0x1d89f0, ios 0x154cd8;
	virtual void textInputReturn(CCTextInputNode*) = mac 0x1d8ab0, ios 0x154d44;
}

class UpdateAccountSettingsPopup : FLAlertLayer, GJAccountSettingsDelegate {
	virtual void keyBackClicked() = mac 0x14bcb0, ios 0x205cac;
	virtual void updateSettingsFinished() = mac 0x14ba40, ios 0x205aac;
	virtual void updateSettingsFailed() = mac 0x14bb80, ios 0x205bc0;
}

class ConfigureValuePopupDelegate {
	virtual void valuePopupClosed(ConfigureValuePopup*, float);
}

class GJSpecialColorSelectDelegate {
	virtual void colorSelectClosed(GJSpecialColorSelect*, int);
}

class OBB2D : cocos2d::CCNode {
}

class Slider : cocos2d::CCLayer {
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x18e2e0, ios 0x211700;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x18e360, ios 0x211740;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x18e320, ios 0x211720;
}

class UILayer : cocos2d::CCLayerColor {
	virtual bool init() = mac 0x27fe40, ios 0x2b994;
	virtual void draw() = mac 0x280980, ios 0x2c094;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x280990, ios 0x2c098;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x280ad0, ios 0x2c1b8;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x280af0, ios 0x2c1c0;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x280bd0, ios 0x2c234;
	virtual void registerWithTouchDispatcher() = mac 0x280c10, ios 0x2c250;
	virtual void keyBackClicked() = mac 0x2808e0, ios 0x2c02c;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x280470, ios 0x2be24;
	virtual void keyUp(cocos2d::enumKeyCodes) = mac 0x280600, ios 0x2bf60;
}

class EditorUI : cocos2d::CCLayer, FLAlertLayerProtocol, ColorSelectDelegate, GJRotationControlDelegate, GJScaleControlDelegate, MusicDownloadDelegate {
	virtual void draw() = mac 0x2e170, ios 0x2c2990;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2ed60, ios 0x2c314c;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2f3d0, ios 0x2c3654;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2fb00, ios 0x2c3ab8;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x30710, ios 0x2c41f0;
	virtual void registerWithTouchDispatcher() = mac 0x30750, ios 0x2c420c;
	virtual void keyBackClicked() = mac 0x24c50, ios 0x2bdbac;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x30790, ios 0x2c4244;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x1f590, ios 0x2b9794;
	virtual void songStateChanged() = mac 0x24760, ios 0x2bd7e4;
	virtual void colorSelectClosed(cocos2d::CCNode*) = mac 0x29940, ios 0x2c0e70;
	virtual void keyUp(cocos2d::enumKeyCodes) = mac 0x312b0, ios 0x2c482c;
	virtual void scrollWheel(float, float) = mac 0x31370, ios 0x2c4884;
	virtual void angleChangeBegin() = mac 0x2e260, ios 0x2c2a74;
	virtual void angleChangeEnded() = mac 0x2e320, ios 0x2c2acc;
	virtual void angleChanged(float) = mac 0x2e3a0, ios 0x2c2ad8;
	virtual void scaleChangeBegin() = mac 0x25120, ios 0x2bdf38;
	virtual void scaleChangeEnded() = mac 0x251e0, ios 0x2bdf90;
	virtual void scaleChanged(float) = mac 0x25260, ios 0x2bdf9c;
}

class GManager : cocos2d::CCNode {
	virtual bool init() = mac 0x26ee00, ios 0x165418;
	virtual void setup() = mac 0x26ee20, ios 0x165428;
	virtual void encodeDataTo(DS_Dictionary*) = mac 0x26f5b0, ios 0x165964;
	virtual void dataLoaded(DS_Dictionary*) = mac 0x26fb70, ios 0x165f28;
	virtual void firstLoad() = mac 0x26fb80, ios 0x165f2c;
}

class GameCell : TableViewCell {
	virtual bool init() = mac 0x18f980, ios 0x12cd0c;
	virtual void draw() = mac 0x18fa40, ios 0x12cd50;
}

class ListCell : TableViewCell {
	virtual bool init() = mac 0x18f7a0, ios 0x12cc80;
	virtual void draw() = mac 0x18f840, ios 0x12cc88;
}

class SongCell : TableViewCell {
	virtual bool init() = mac 0x11c200, ios 0xa5274;
	virtual void draw() = mac 0x11c240, ios 0xa52a4;
}

class TextArea : cocos2d::CCSprite {
	virtual void draw() = mac 0x19f890, ios 0x92998;
	virtual void setOpacity(unsigned char) = mac 0x19f760, ios 0x92874;
}

class FMODSound : cocos2d::CCNode {
}

class GJComment : cocos2d::CCNode {
	virtual bool init() = mac 0x2dfec0, ios 0x728b0;
}

class GJMapPack : cocos2d::CCNode {
	virtual bool init() = mac 0x2de0e0, ios 0x71740;
}

class InfoLayer : FLAlertLayer, LevelCommentDelegate, CommentUploadDelegate, FLAlertLayerProtocol {
	virtual void registerWithTouchDispatcher() = mac 0x45a070, ios 0x25caa0;
	virtual void keyBackClicked() = mac 0x45a000, ios 0x25ca24;
	virtual void show() = mac 0x45ab80, ios 0x25d430;
	virtual void loadCommentsFinished(cocos2d::CCArray*, char const*) = mac 0x45a1b0, ios 0x25cbdc;
	virtual void loadCommentsFailed(char const*) = mac 0x45a270, ios 0x25cc4c;
	virtual void setupPageInfo(gd::string, char const*) = mac 0x45a320, ios 0x25cca8;
	virtual void commentUploadFinished(int) = mac 0x45a890, ios 0x25d1a4;
	virtual void commentUploadFailed(int, CommentError) = mac 0x45a960, ios 0x25d21c;
	virtual void updateUserScoreFinished() = mac 0x45a720, ios 0x25d0f4;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x459d50, ios 0x25c8a0;
}

class KeysLayer : cocos2d::CCLayer, FLAlertLayerProtocol, DialogDelegate {
	virtual bool init() = mac 0x44d340, ios 0x254898;
	virtual void keyBackClicked() = mac 0x453210, ios 0x259580;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x4532e0, ios 0x25958c;
	virtual void dialogClosed(DialogLayer*) = mac 0x453090, ios 0x2593b8;
}

class LevelCell : TableViewCell {
	virtual bool init() = mac 0x117a40, ios 0xa1864;
	virtual void draw() = mac 0x11a4d0, ios 0xa3d68;
}

class LevelPage : cocos2d::CCLayer, DialogDelegate {
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x23cd90, ios 0x2cab10;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x23ceb0, ios 0x2cac0c;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x23ced0, ios 0x2cac14;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x23cef0, ios 0x2cac1c;
	virtual void registerWithTouchDispatcher() = mac 0x23cf30, ios 0x2cac38;
	virtual void dialogClosed(DialogLayer*) = mac 0x23cad0, ios 0x2ca934;
}

class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol, GooglePlayDelegate {
	virtual bool init() = mac 0x1d14b0, ios 0x19e6d4;
	virtual void keyBackClicked() = mac 0x1d3160, ios 0x1a0024;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x1d33d0, ios 0x1a0354;
	virtual void googlePlaySignedIn() = mac 0x1d2f30, ios 0x19fe90;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x1d3190, ios 0x1a010c;
}

class NodePoint : cocos2d::CCObject {
}

class PlayLayer : GJBaseGameLayer, CCCircleWaveDelegate, CurrencyRewardDelegate, DialogDelegate {
	virtual void update(float) = mac 0x77900, ios 0xb2f08;
	virtual void onEnterTransitionDidFinish() = mac 0x806e0, ios 0xb9724;
	virtual void onExit() = mac 0x80710, ios 0xb9754;
	virtual void draw() = mac 0x7d160, ios 0xb7260;
	virtual void visit() = mac 0x75ef0, ios 0xb1930;
	virtual void updateTweenAction(float, char const*) = mac 0x7ffb0, ios 0xb90bc;
	virtual void updateColor(cocos2d::ccColor3B, float, int, bool, float, cocos2d::ccHSVValue, int, bool, int, EffectGameObject*) = mac 0x7c7f0, ios 0xb6b1c;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0x7cd10, ios 0xb6e84;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0x7aa10, ios 0xb51b8;
	virtual void timeForXPos(float) = mac 0x7d120, ios 0xb7238;
	virtual void xPosForTime(float) = mac 0x7d140, ios 0xb724c;
	virtual void currencyWillExit(CurrencyRewardLayer*) = mac 0x7e070, ios 0xb7da4;
	virtual void circleWaveWillBeRemoved(CCCircleWave*) = mac 0x7e110, ios 0xb7df4;
	virtual void dialogClosed(DialogLayer*) = mac 0x7e0b0, ios 0xb7dc8;
}

class PointNode : cocos2d::CCObject {
}

class StatsCell : TableViewCell {
	virtual bool init() = mac 0x11b100, ios 0xa45f0;
	virtual void draw() = mac 0x11bf80, ios 0xa510c;
}

class TableView : CCScrollLayerExt, CCScrollLayerExtDelegate {
	virtual void onEnter() = mac 0x37ff30, ios 0x21dcac;
	virtual void onExit() = mac 0x37ff40, ios 0x21dcb0;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x380120, ios 0x21de24;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x380be0, ios 0x21e5e8;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x3809a0, ios 0x21e46c;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x380b20, ios 0x21e580;
	virtual void registerWithTouchDispatcher() = mac 0x37ff50, ios 0x21dcb4;
	virtual void scrollWheel(float, float) = mac 0x380cd0, ios 0x21e6b4;
	virtual void scrllViewWillBeginDecelerating(CCScrollLayerExt*) = mac 0x3818a0, ios 0x21efd4;
	virtual void scrollViewDidEndDecelerating(CCScrollLayerExt*) = mac 0x3818c0, ios 0x21efdc;
	virtual void scrollViewTouchMoving(CCScrollLayerExt*) = mac 0x3818e0, ios 0x21efe4;
	virtual void scrollViewDidEndMoving(CCScrollLayerExt*) = mac 0x381900, ios 0x21efec;
}

class cocos2d::CCBezierBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f6960, ios 0xf21ec;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f67a0, ios 0xf2088;
	virtual auto reverse() = mac 0x1f6b40, ios 0xf2368;
}

class cocos2d::CCBezierTo : cocos2d::CCBezierBy {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f6f30, ios 0xf2690;
}

class cocos2d::CCCallFunc : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x455120, ios 0x112a2c;
	virtual auto initWithTarget(cocos2d::CCObject*) = mac 0x454eb0, ios 0x112838;
	virtual auto execute() = mac 0x455130, ios 0x112a38;
}

class cocos2d::CCDirector : cocos2d::CCObject, cocos2d::TypeInfo {
	virtual auto init() = mac 0x248df0, ios 0xec064;
	virtual auto getClassTypeInfo();
	virtual auto getScheduler() = mac 0x24af00, ios 0xed6bc;
	virtual auto setScheduler(cocos2d::CCScheduler*) = mac 0x24aec0, ios 0xed674;
	virtual auto getActionManager() = mac 0x24af50, ios 0xed70c;
	virtual auto setActionManager(cocos2d::CCActionManager*) = mac 0x24af10, ios 0xed6c4;
	virtual auto getTouchDispatcher() = mac 0x24afa0, ios 0xed75c;
	virtual auto setTouchDispatcher(cocos2d::CCTouchDispatcher*) = mac 0x24af60, ios 0xed714;
	virtual auto getKeypadDispatcher() = mac 0x24b090, ios 0xed828;
	virtual auto setKeypadDispatcher(cocos2d::CCKeypadDispatcher*) = mac 0x24b050, ios 0xed7ec;
	virtual auto getKeyboardDispatcher() = mac 0x24aff0, ios 0xed7a0;
	virtual auto setKeyboardDispatcher(cocos2d::CCKeyboardDispatcher*) = mac 0x24afb0, ios 0xed764;
	virtual auto getMouseDispatcher() = mac 0x24b040, ios 0xed7e4;
	virtual auto setMouseDispatcher(cocos2d::CCMouseDispatcher*) = mac 0x24b000, ios 0xed7a8;
	virtual auto getAccelerometer() = mac 0x24b0e0, ios 0xed894;
	virtual auto setAccelerometer(cocos2d::CCAccelerometer*) = mac 0x24b0a0, ios 0xed830;
	virtual auto getDeltaTime() = mac 0x249bd0, ios 0xecbc8;
	virtual auto getSceneDelegate() const = mac 0x24b320, ios 0xedaa8;
	virtual auto setSceneDelegate(cocos2d::CCSceneDelegate*) = mac 0x24b330, ios 0xedab0;
}

class cocos2d::CCDrawNode : cocos2d::CCNodeRGBA {
	virtual auto init() = mac 0x378e00, ios 0x26d00;
	virtual auto draw() = mac 0x379020, ios 0x26eb4;
}

class cocos2d::CCGridBase : cocos2d::CCObject {
	virtual auto blit() = mac 0x2880e0;
	virtual auto reuse() = mac 0x2880f0;
	virtual auto calculateVertexPoints() = mac 0x288100;
}

class cocos2d::CCLabelTTF : cocos2d::CCSprite, cocos2d::CCLabelProtocol {
	virtual auto init() = mac 0x1fac60, ios 0x3a51c;
	virtual auto setString(char const*) = mac 0x1fad70, ios 0x3a558;
	virtual auto getString() = mac 0x1faf70, ios 0x3a6b4;
}

class cocos2d::CCMenuItem : cocos2d::CCNodeRGBA {
	virtual auto activate() = mac 0x1fba70, ios 0x2ceb0;
	virtual auto selected() = mac 0x1fb9e0, ios 0x2ce2c;
	virtual auto unselected() = mac 0x1fb9f0, ios 0x2ce38;
	virtual auto registerScriptTapHandler(int) = mac 0x1fba00, ios 0x2ce40;
	virtual auto unregisterScriptTapHandler() = mac 0x1fba30, ios 0x2ce70;
	virtual auto isEnabled() = mac 0x1fbaf0, ios 0x2cf34;
	virtual auto setEnabled(bool) = mac 0x1fbae0, ios 0x2cf2c;
	virtual auto isSelected() = mac 0x1fbb50, ios 0x2cf60;
}

class cocos2d::CCNodeRGBA : cocos2d::CCNode, cocos2d::CCRGBAProtocol {
	virtual auto init() = mac 0x124bf0, ios 0x15f780;
	virtual auto getOpacity() = mac 0x124cd0, ios 0x15f7b8;
	virtual auto getDisplayedOpacity() = mac 0x124cf0, ios 0x15f7c8;
	virtual auto setOpacity(unsigned char) = mac 0x124d10, ios 0x15f7d8;
	virtual auto updateDisplayedOpacity(unsigned char) = mac 0x124e50, ios 0x15f87c;
	virtual auto isCascadeOpacityEnabled() = mac 0x124fe0, ios 0x15f948;
	virtual auto setCascadeOpacityEnabled(bool) = mac 0x125000, ios 0x15f958;
	virtual auto getColor() = mac 0x125020, ios 0x15f968;
	virtual auto getDisplayedColor() = mac 0x125040, ios 0x15f978;
	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x125060, ios 0x15f988;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const&) = mac 0x1251e0, ios 0x15fa68;
	virtual auto isCascadeColorEnabled() = mac 0x125320, ios 0x15fb70;
	virtual auto setCascadeColorEnabled(bool) = mac 0x125340, ios 0x15fb80;
	virtual auto setOpacityModifyRGB(bool);
	virtual auto isOpacityModifyRGB();
}

class cocos2d::CCProfiler : cocos2d::CCObject {
}

class cocos2d::CCRipple3D : cocos2d::CCGrid3DAction {
	virtual auto update(float) = mac 0x1286c0;
}

class cocos2d::CCRotateBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f4f90, ios 0xf18e0;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f4f40, ios 0xf1888;
	virtual auto reverse() = mac 0x1f5000, ios 0xf1964;
}

class cocos2d::CCRotateTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f4be0, ios 0xf15c0;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f4aa0, ios 0xf14b8;
	virtual auto initWithDuration(float, float, float) = mac 0x1f4980, ios 0xf13a8;
}

class cocos2d::CCSequence : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float);
	virtual auto startWithTarget(cocos2d::CCNode*);
	virtual auto stop();
	virtual auto reverse();
}

class cocos2d::CCStopGrid : cocos2d::CCActionInstant {
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0xeeee0;
}

class cocos2d::CCTMXLayer : cocos2d::CCSpriteBatchNode {
	virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x1222b0;
	virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0x1222c0;
	virtual auto getTileSet() = mac 0x120d20;
	virtual auto setTileSet(cocos2d::CCTMXTilesetInfo*) = mac 0x120d30;
	virtual auto getProperties() = mac 0x1224a0;
	virtual auto setProperties(cocos2d::CCDictionary*) = mac 0x1224b0;
}

class cocos2d::CCAnimation : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto getDuration() = mac 0x141460, ios 0x104c2c;
}

class cocos2d::CCAtlasNode : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	virtual auto draw() = mac 0x1a4f70;
	virtual auto setOpacity(unsigned char) = mac 0x1a51c0;
	virtual auto getColor() = mac 0x1a5060;
	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x1a50b0;
	virtual auto setOpacityModifyRGB(bool) = mac 0x1a5240;
	virtual auto isOpacityModifyRGB() = mac 0x1a52f0;
	virtual auto getTextureAtlas() = mac 0x1a54a0;
	virtual auto setTextureAtlas(cocos2d::CCTextureAtlas*) = mac 0x1a5460;
	virtual auto getBlendFunc() = mac 0x1a5320;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc) = mac 0x1a5340;
	virtual auto getQuadsToDraw() = mac 0x1a54b0;
	virtual auto setQuadsToDraw(unsigned int) = mac 0x1a54c0;
	virtual auto updateAtlasValues() = mac 0x1a4f60;
	virtual auto getTexture() = mac 0x1a5430;
	virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0x1a5360;
}

class cocos2d::CCCallFuncN : cocos2d::CCCallFunc, cocos2d::TypeInfo {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto execute() = mac 0x455190;
	virtual auto getClassTypeInfo();
	virtual auto initWithTarget(cocos2d::CCObject*, cocos2d::SEL_CallFuncN) = mac 0x455330;
}

class cocos2d::CCCallFuncO : cocos2d::CCCallFunc, cocos2d::TypeInfo {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto execute() = mac 0x455910, ios 0x112b40;
	virtual auto getClassTypeInfo();
	virtual auto initWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCObject*) = mac 0x455a20, ios 0x112c38;
}

class cocos2d::CCComponent : cocos2d::CCObject {
	virtual auto update(float) = mac 0x5a510;
	virtual auto init() = mac 0x5a4e0;
	virtual auto onEnter() = mac 0x5a4f0;
	virtual auto onExit() = mac 0x5a500;
	virtual auto serialize(void*) = mac 0x5a520;
	virtual auto isEnabled() const = mac 0x5a600;
	virtual auto setEnabled(bool) = mac 0x5a610;
}

class cocos2d::CCDelayTime : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f8af0, ios 0xf36a4;
	virtual auto reverse() = mac 0x1f8b00, ios 0xf36a8;
}

class cocos2d::CCEaseInOut : cocos2d::CCEaseRateAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a1f30, ios 0x1a6b30;
	virtual auto reverse() = mac 0x2a1fa0, ios 0x1a6bac;
}

class cocos2d::CCFileUtils : cocos2d::TypeInfo {
	virtual auto getClassTypeInfo();
	virtual auto purgeCachedEntries() = mac 0x23f100, ios 0xcdd6c;
	virtual auto getFileData(char const*, char const*, unsigned long*) = mac 0x23f140, ios 0xcdd9c;
	virtual auto getFileDataFromZip(char const*, char const*, unsigned long*) = mac 0x23f2e0, ios 0xcdf0c;
	virtual auto fullPathForFilename(char const*, bool) = mac 0x23f940, ios 0xce604;
	virtual auto removeFullPath(char const*) = mac 0x240710, ios 0xcf170;
	virtual auto loadFilenameLookupDictionaryFromFile(char const*) = mac 0x2416f0, ios 0xd0084;
	virtual auto setFilenameLookupDictionary(cocos2d::CCDictionary*) = mac 0x241680, ios 0xd001c;
	virtual auto fullPathFromRelativeFile(char const*, char const*) = mac 0x2408c0, ios 0xcf270;
	virtual auto setSearchResolutionsOrder(gd::vector<gd::string> const&) = mac 0x240ac0, ios 0xcf43c;
	virtual auto addSearchResolutionsOrder(char const*) = mac 0x240d30, ios 0xcf658;
	virtual auto getSearchResolutionsOrder() = mac 0x240df0, ios 0xcf75c;
	virtual auto setSearchPaths(gd::vector<gd::string> const&) = mac 0x240e10, ios 0xcf76c;
	virtual auto addSearchPath(char const*) = mac 0x241180, ios 0xcfb6c;
	virtual auto removeSearchPath(char const*) = mac 0x241390, ios 0xcfd50;
	virtual auto getSearchPaths() = mac 0x240e00, ios 0xcf764;
	virtual auto getWritablePath2() = mac 0x241a90, ios 0xd03b0;
	virtual auto isAbsolutePath(gd::string const&) = mac 0x2419c0, ios 0xd02ac;
	virtual auto setPopupNotify(bool) = mac 0x2419d0, ios 0xd02cc;
	virtual auto isPopupNotify() = mac 0x2419e0, ios 0xd02d8;
	virtual auto init() = mac 0x23f010, ios 0xcdcb4;
	virtual auto getNewFilename(char const*) = mac 0x23f3f0, ios 0xce02c;
	virtual auto shouldUseHD() = mac 0x23f4e0, ios 0xce1a0;
	virtual auto addSuffix(gd::string, gd::string) = mac 0x23f510, ios 0xce1c4;
	virtual auto getPathForFilename(gd::string const&, gd::string const&, gd::string const&) = mac 0x23f650, ios 0xce3d8;
	virtual auto getFullPathForDirectoryAndFilename(gd::string const&, gd::string const&) = mac 0x241900, ios 0xd023c;
	virtual auto createCCDictionaryWithContentsOfFile(gd::string const&) = mac 0x23ec30, ios 0xcdb08;
	virtual auto writeToFile(cocos2d::CCDictionary*, gd::string const&) = mac 0x23ec40, ios 0xcdb10;
	virtual auto createCCArrayWithContentsOfFile(gd::string const&) = mac 0x23ec50, ios 0xcdb18;
}

class cocos2d::CCGLProgram : cocos2d::CCObject {
}

class cocos2d::CCLayerRGBA : cocos2d::CCLayer, cocos2d::CCRGBAProtocol {
	virtual auto init() = mac 0x273b40, ios 0xc8698;
	virtual auto getOpacity() = mac 0x273be0, ios 0xc8714;
	virtual auto getDisplayedOpacity() = mac 0x273c00, ios 0xc8724;
	virtual auto setOpacity(unsigned char) = mac 0x273c20, ios 0xc8734;
	virtual auto updateDisplayedOpacity(unsigned char) = mac 0x273f20, ios 0xc88d8;
	virtual auto isCascadeOpacityEnabled() = mac 0x2741f0, ios 0xc8aac;
	virtual auto setCascadeOpacityEnabled(bool) = mac 0x274210, ios 0xc8abc;
	virtual auto getColor() = mac 0x273d60, ios 0xc87d8;
	virtual auto getDisplayedColor() = mac 0x273d80, ios 0xc87e8;
	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x273da0, ios 0xc87f8;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const&) = mac 0x2740b0, ios 0xc89a4;
	virtual auto isCascadeColorEnabled() = mac 0x274230, ios 0xc8acc;
	virtual auto setCascadeColorEnabled(bool) = mac 0x274250, ios 0xc8adc;
	virtual auto setOpacityModifyRGB(bool);
	virtual auto isOpacityModifyRGB();
}

// class cocos2d::CCLightning : cocos2d::CCNode, cocos2d::CCRGBAProtocol {
// 	virtual auto draw() = mac 0x34d200, ios 0x254424;
// 	virtual auto isOpacityModifyRGB() = mac 0x34d520, ios 0x2545b4;
// 	virtual auto setOpacityModifyRGB(bool) = mac 0x34d540, ios 0x2545c4;
// 	virtual auto getOpacity() = mac 0x34d450, ios 0x25453c;
// 	virtual auto getDisplayedOpacity() = mac 0x34d470, ios 0x25454c;
// 	virtual auto setOpacity(unsigned char) = mac 0x34d4a0, ios 0x254568;
// 	virtual auto updateDisplayedOpacity(unsigned char) = mac 0x34d560, ios 0x2545d4;
// 	virtual auto isCascadeOpacityEnabled() = mac 0x34d580, ios 0x2545e4;
// 	virtual auto setCascadeOpacityEnabled(bool) = mac 0x34d5a0, ios 0x2545f4;
// 	virtual auto getColor() = mac 0x34d4c0, ios 0x254578;
// 	virtual auto getDisplayedColor() = mac 0x34d5c0, ios 0x254604;
// 	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x34d4e0, ios 0x254588;
// 	virtual auto updateDisplayedColor(cocos2d::ccColor3B const&) = mac 0x34d5e0, ios 0x254614;
// 	virtual auto isCascadeColorEnabled() = mac 0x34d620, ios 0x25463c;
// 	virtual auto setCascadeColorEnabled(bool) = mac 0x34d640, ios 0x25464c;
// }

class cocos2d::CCReuseGrid : cocos2d::CCActionInstant {
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0xef010;
}

class cocos2d::CCScheduler : cocos2d::CCObject {
	virtual auto update(float) = mac 0x2446d0, ios 0x10df78;
}

class cocos2d::CCSplitCols : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x45e790;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x45e750;
	virtual auto initWithDuration(float, unsigned int) = mac 0x45e5d0;
}

class cocos2d::CCSplitRows : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x45e3e0;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x45e3a0;
	virtual auto initWithDuration(float, unsigned int) = mac 0x45e220;
}

class cocos2d::CCTexture2D : cocos2d::CCObject {
	virtual auto getName() = mac 0x246450, ios 0xbdd20;
	virtual auto getMaxS() = mac 0x2464e0, ios 0xbdd80;
	virtual auto setMaxS(float) = mac 0x2464f0, ios 0xbdd88;
	virtual auto getMaxT() = mac 0x246500, ios 0xbdd90;
	virtual auto setMaxT(float) = mac 0x246510, ios 0xbdd98;
	virtual auto getPixelFormat() = mac 0x246420, ios 0xbdd08;
	virtual auto getPixelsWide() = mac 0x246430, ios 0xbdd10;
	virtual auto getPixelsHigh() = mac 0x246440, ios 0xbdd18;
	virtual auto getContentSize() = mac 0x246460, ios 0xbdd28;
	virtual auto getShaderProgram() = mac 0x246520, ios 0xbdda0;
	virtual auto setShaderProgram(cocos2d::CCGLProgram*) = mac 0x246530, ios 0xbdda8;
}

class cocos2d::CCActionEase : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a15e0, ios 0x1a658c;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x2a1580, ios 0x1a652c;
	virtual auto stop() = mac 0x2a15b0, ios 0x1a655c;
	virtual auto reverse() = mac 0x2a1600, ios 0x1a659c;
	virtual auto getInnerAction() = mac 0x2a1620, ios 0x1a65bc;
}

class cocos2d::CCCallFuncND : cocos2d::CCCallFuncN {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto getClassTypeInfo();
	virtual auto initWithTarget(cocos2d::CCObject*, cocos2d::SEL_CallFuncND, void*) = mac 0x455560;
	virtual auto execute() = mac 0x4556e0;
}

class cocos2d::CCDictionary : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x192790, ios 0x2e066c;
}

class cocos2d::CCEaseBackIn : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a42a0, ios 0x1a85a4;
	virtual auto reverse() = mac 0x2a42d0, ios 0x1a85d4;
}

class cocos2d::CCEaseBounce : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto reverse() = mac 0x2a3890;
}

class cocos2d::CCEaseSineIn : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a2780, ios 0x1a72ec;
	virtual auto reverse() = mac 0x2a27c0, ios 0x1a733c;
}

class cocos2d::CCGridAction : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0xee0d0;
	virtual auto reverse() = mac 0xee1a0;
	virtual auto initWithDuration(float, cocos2d::CCSize const&) = mac 0xee0a0;
	virtual auto getGrid() = mac 0xee190;
}

class cocos2d::CCLabelAtlas : cocos2d::CCAtlasNode, cocos2d::CCLabelProtocol {
	virtual auto updateAtlasValues() = mac 0x43d590;
	virtual auto setString(char const*) = mac 0x43d840;
	virtual auto getString() = mac 0x43d930;
}

class cocos2d::CCLayerColor : cocos2d::CCLayerRGBA, cocos2d::CCBlendProtocol {
	virtual auto init() = mac 0x274800, ios 0xc8de8;
	virtual auto setContentSize(cocos2d::CCSize const&) = mac 0x2749f0, ios 0xc8f64;
	virtual auto draw() = mac 0x274b50, ios 0xc8fe0;
	virtual auto setOpacity(unsigned char) = mac 0x274db0, ios 0xc9108;
	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x274c20, ios 0xc90ac;
	virtual auto initWithColor(cocos2d::ccColor4B const&, float, float) = mac 0x274850, ios 0xc8e34;
	virtual auto initWithColor(cocos2d::ccColor4B const&) = mac 0x2749a0, ios 0xc8f14;
	virtual auto getBlendFunc() = mac 0x274480, ios 0xc8bcc;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc) = mac 0x2744a0, ios 0xc8bdc;
	virtual auto updateColor() = mac 0x274ae0, ios 0xc8f80;
}

class cocos2d::CCPageTurn3D : cocos2d::CCGrid3DAction {
	virtual auto update(float) = mac 0x194130;
}

class cocos2d::CCPointArray : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
}

class cocos2d::CCProgressTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x23d380;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x23d340;
}

class cocos2d::CCRemoveSelf : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x454770;
	virtual auto reverse() = mac 0x454790;
}

class cocos2d::CCTMXMapInfo : cocos2d::CCObject, cocos2d::CCSAXDelegator {
	virtual auto getLayers() = mac 0x283640;
	virtual auto setLayers(cocos2d::CCArray*) = mac 0x283650;
	virtual auto getTilesets() = mac 0x283690;
	virtual auto setTilesets(cocos2d::CCArray*) = mac 0x2836a0;
	virtual auto getObjectGroups() = mac 0x2836e0;
	virtual auto setObjectGroups(cocos2d::CCArray*) = mac 0x2836f0;
	virtual auto getProperties() = mac 0x283730;
	virtual auto setProperties(cocos2d::CCDictionary*) = mac 0x283740;
	virtual auto startElement(void*, char const*, char const**) = mac 0x2837d0;
	virtual auto endElement(void*, char const*) = mac 0x286210;
	virtual auto textHandler(void*, char const*, int) = mac 0x286460;
}

class cocos2d::CCActionTween : cocos2d::CCActionInterval {
	virtual auto update(float) = mac 0x447720, ios 0x28e6fc;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x4476f0, ios 0x28e6d0;
	virtual auto reverse() = mac 0x447760, ios 0x28e734;
}

class cocos2d::CCApplication : cocos2d::CCApplicationProtocol {
	virtual auto setAnimationInterval(double) = mac 0x1a3ee0, ios 0x10e508;
	virtual auto getCurrentLanguage() = mac 0x1a3f40, ios 0x10e57c;
	virtual auto getTargetPlatform() = mac 0x1a3f20, ios 0x10e7a4;
	virtual auto openURL(char const*) = mac 0x1a4550, ios 0x10e494;
}

class cocos2d::CCDataVisitor {
	virtual auto visit(cocos2d::CCBool const*);
	virtual auto visit(cocos2d::CCInteger const*);
	virtual auto visit(cocos2d::CCFloat const*);
	virtual auto visit(cocos2d::CCDouble const*);
	virtual auto visit(cocos2d::CCString const*);
	virtual auto visit(cocos2d::CCArray const*);
	virtual auto visit(cocos2d::CCDictionary const*);
	virtual auto visit(cocos2d::CCSet const*);
}

class cocos2d::CCEaseBackOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a4480, ios 0x1a8754;
	virtual auto reverse() = mac 0x2a44c0, ios 0x1a8794;
}

class cocos2d::CCEaseElastic : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto reverse() = mac 0x2a2dd0;
}

class cocos2d::CCEaseSineOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a2970, ios 0x1a74bc;
	virtual auto reverse() = mac 0x2a29a0, ios 0x1a7504;
}

class cocos2d::CCIMEDelegate {
	virtual auto attachWithIME() = mac 0x2776a0, ios 0x12d3d4;
	virtual auto detachWithIME() = mac 0x277880, ios 0x12d4e8;
	virtual auto canAttachWithIME();
	virtual auto didAttachWithIME();
	virtual auto canDetachWithIME();
	virtual auto didDetachWithIME();
	virtual auto insertText(char const*, int);
	virtual auto deleteBackward();
	virtual auto getContentText();
	virtual auto keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	virtual auto keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	virtual auto keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	virtual auto keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo&);
}

class cocos2d::CCJumpTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual auto update(float) = mac 0x45df60;
}

class cocos2d::CCLabelBMFont : cocos2d::CCSpriteBatchNode, cocos2d::CCLabelProtocol, cocos2d::CCRGBAProtocol {
	virtual auto init() = mac 0x347b10, ios 0x2198e0;
	virtual auto setScaleX(float) = mac 0x34a5b0, ios 0x21b6e8;
	virtual auto setScaleY(float) = mac 0x34a5d0, ios 0x21b714;
	virtual auto setScale(float) = mac 0x34a590, ios 0x21b6bc;
	virtual auto setAnchorPoint(cocos2d::CCPoint const&) = mac 0x349440, ios 0x21aa7c;
	virtual auto setString(char const*) = mac 0x348990, ios 0x21a408;
	virtual auto setString(char const*, bool) = mac 0x3489e0, ios 0x21a42c;
	virtual auto getString() = mac 0x348bf0, ios 0x21a60c;
	virtual auto setCString(char const*) = mac 0x348c10, ios 0x21a63c;
	virtual auto updateLabel() = mac 0x349480, ios 0x21aad0;
	virtual auto setAlignment(cocos2d::CCTextAlignment) = mac 0x34a530, ios 0x21b68c;
	virtual auto setWidth(float) = mac 0x34a550, ios 0x21b69c;
	virtual auto setLineBreakWithoutSpace(bool) = mac 0x34a570, ios 0x21b6ac;
	virtual auto isOpacityModifyRGB() = mac 0x3490e0, ios 0x21a8c4;
	virtual auto setOpacityModifyRGB(bool) = mac 0x348f70, ios 0x21a80c;
	virtual auto getOpacity() = mac 0x348df0, ios 0x21a748;
	virtual auto getDisplayedOpacity() = mac 0x348e10, ios 0x21a758;
	virtual auto setOpacity(unsigned char) = mac 0x348e30, ios 0x21a768;
	virtual auto updateDisplayedOpacity(unsigned char) = mac 0x349100, ios 0x21a8d4;
	virtual auto isCascadeOpacityEnabled() = mac 0x349400, ios 0x21aa5c;
	virtual auto setCascadeOpacityEnabled(bool) = mac 0x349420, ios 0x21aa6c;
	virtual auto getColor() = mac 0x348c30, ios 0x21a648;
	virtual auto getDisplayedColor() = mac 0x348c50, ios 0x21a658;
	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x348c70, ios 0x21a668;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const&) = mac 0x349210, ios 0x21a968;
	virtual auto isCascadeColorEnabled() = mac 0x3493c0, ios 0x21aa3c;
	virtual auto setCascadeColorEnabled(bool) = mac 0x3493e0, ios 0x21aa4c;
	virtual auto setString(unsigned short*, bool) = mac 0x348a60, ios 0x21a4b4;
}

class cocos2d::CCOrbitCamera : cocos2d::CCActionCamera {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1b30c0;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1b2de0;
}

class cocos2d::CCParticleSun : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24bbb0;
}

class cocos2d::CCReverseTime : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f8f70;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f8f00;
	virtual auto stop() = mac 0x1f8f40;
	virtual auto reverse() = mac 0x1f8fa0;
}

class cocos2d::CCShaderCache : cocos2d::CCObject {
}

class cocos2d::CCSpriteFrame : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
}

class cocos2d::CCTMXTiledMap : cocos2d::CCNode {
	virtual auto getObjectGroups() = mac 0x2a63e0;
	virtual auto setObjectGroups(cocos2d::CCArray*) = mac 0x2a63f0;
	virtual auto getProperties() = mac 0x2a6430;
	virtual auto setProperties(cocos2d::CCDictionary*) = mac 0x2a6440;
}

class cocos2d::CCTiledGrid3D : cocos2d::CCGridBase {
	virtual auto blit() = mac 0x2891c0;
	virtual auto reuse() = mac 0x289780;
	virtual auto calculateVertexPoints() = mac 0x289260;
}

class cocos2d::CCActionCamera : cocos2d::CCActionInterval {
	virtual auto startWithTarget(cocos2d::CCNode*);
	virtual auto reverse() = mac 0x1b2a10;
}

class cocos2d::CCCatmullRomBy : cocos2d::CCCardinalSplineBy {
}

class cocos2d::CCCatmullRomTo : cocos2d::CCCardinalSplineTo {
}

class cocos2d::CCClippingNode : cocos2d::CCNode {
	virtual auto init() = mac 0x4193e0;
	virtual auto onEnter() = mac 0x419470;
	virtual auto onEnterTransitionDidFinish() = mac 0x4194a0;
	virtual auto onExit() = mac 0x419500;
	virtual auto onExitTransitionDidStart() = mac 0x4194d0;
	virtual auto visit() = mac 0x419530;
	virtual auto init(cocos2d::CCNode*) = mac 0x419400;
}

class cocos2d::CCEaseBounceIn : cocos2d::CCEaseBounce {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a3a40, ios 0x1a804c;
	virtual auto reverse() = mac 0x2a3b20, ios 0x1a808c;
}

class cocos2d::CCGrid3DAction : cocos2d::CCGridAction {
	virtual auto getGrid() = mac 0xee2b0;
}

class cocos2d::CCMenuItemFont : cocos2d::CCMenuItemLabel {
}

class cocos2d::CCMotionStreak : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	virtual auto update(float) = mac 0x2edb40, ios 0xee07c;
	virtual auto setPosition(cocos2d::CCPoint const&) = mac 0x2ed8b0, ios 0xedf5c;
	virtual auto draw() = mac 0x2ee1b0, ios 0xee574;
	virtual auto getOpacity() = mac 0x2eda60, ios 0xedff4;
	virtual auto setOpacity(unsigned char) = mac 0x2eda40, ios 0xedfec;
	virtual auto setOpacityModifyRGB(bool) = mac 0x2eda80, ios 0xee004;
	virtual auto isOpacityModifyRGB() = mac 0x2edaa0, ios 0xee00c;
	virtual auto getTexture() = mac 0x2ed950, ios 0xedf6c;
	virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0x2ed970, ios 0xedf7c;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc) = mac 0x2eda00, ios 0xedfcc;
	virtual auto getBlendFunc() = mac 0x2eda20, ios 0xedfdc;
}

class cocos2d::CCMouseHandler : cocos2d::CCObject {
	virtual auto initWithDelegate(cocos2d::CCMouseDelegate*) = mac 0x12ef40, ios 0x43798;
}

class cocos2d::CCParallaxNode : cocos2d::CCNode {
	virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0x2712c0;
	virtual auto removeAllChildrenWithCleanup(bool) = mac 0x271350;
	virtual auto visit() = mac 0x271410;
	virtual auto addChild(cocos2d::CCNode*, unsigned int, cocos2d::CCPoint const&, cocos2d::CCPoint const&) = mac 0x271100;
	virtual auto addChild(cocos2d::CCNode*, unsigned int, int) = mac 0x2710f0;
}

class cocos2d::CCParticleFire : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24b570;
}

class cocos2d::CCParticleRain : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24d3f0;
}

class cocos2d::CCParticleSnow : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24d0d0;
}

class cocos2d::CCRGBAProtocol {
}

class cocos2d::CCSAXDelegator {
}

class cocos2d::CCShakyTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x45b900;
	virtual auto initWithDuration(float, cocos2d::CCSize const&, int, bool) = mac 0x45b7b0;
}

class cocos2d::CCShuffleTiles : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x45c830;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x45c590;
	virtual auto initWithDuration(float, cocos2d::CCSize const&, unsigned int) = mac 0x45c140;
}

class cocos2d::CCTMXLayerInfo : cocos2d::CCObject {
	virtual auto getProperties() = mac 0x282900;
	virtual auto setProperties(cocos2d::CCDictionary*) = mac 0x282910;
}

class cocos2d::CCTextFieldTTF : cocos2d::CCLabelTTF, cocos2d::CCIMEDelegate {
	virtual auto draw() = mac 0x126a00, ios 0x12931c;
	virtual auto setString(char const*) = mac 0x126ac0, ios 0x1293fc;
	virtual auto getString() = mac 0x126d00, ios 0x129654;
	virtual auto attachWithIME() = mac 0x126370, ios 0x128cd8;
	virtual auto detachWithIME() = mac 0x1263f0, ios 0x128d28;
	virtual auto getColorSpaceHolder() = mac 0x126a90, ios 0x1293e0;
	virtual auto setColorSpaceHolder(cocos2d::ccColor3B const&) = mac 0x126aa0, ios 0x1293e8;
	virtual auto setPlaceHolder(char const*) = mac 0x126d20, ios 0x12967c;
	virtual auto getPlaceHolder() = mac 0x126e10, ios 0x1297bc;
	virtual auto setSecureTextEntry(bool) = mac 0x126e20, ios 0x1297d0;
	virtual auto isSecureTextEntry() = mac 0x126e70, ios 0x129838;
	virtual auto canAttachWithIME() = mac 0x126470, ios 0x128d78;
	virtual auto canDetachWithIME() = mac 0x1264d0, ios 0x128de8;
	virtual auto insertText(char const*, int) = mac 0x126530, ios 0x128e58;
	virtual auto deleteBackward() = mac 0x126760, ios 0x1290ac;
	virtual auto getContentText() = mac 0x1269e0, ios 0x1292f4;
}

class cocos2d::CCTextureAtlas : cocos2d::CCObject {
	virtual auto getTotalQuads() = mac 0x26fe70, ios 0x1a3e94;
	virtual auto getCapacity() = mac 0x26fe80, ios 0x1a3e9c;
	virtual auto getTexture() = mac 0x26fe90, ios 0x1a3ea4;
	virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0x26fea0, ios 0x1a3eac;
	virtual auto getQuads() = mac 0x26fee0, ios 0x1a3ee8;
	virtual auto setQuads(cocos2d::_ccV3F_C4B_T2F_Quad*) = mac 0x26fef0, ios 0x1a3ef8;
}

class cocos2d::CCTextureCache : cocos2d::CCObject {
}

class cocos2d::CCTileMapAtlas : cocos2d::CCAtlasNode {
	virtual auto updateAtlasValues() = mac 0x105f00;
	virtual auto getTGAInfo() = mac 0x106120;
	virtual auto setTGAInfo(cocos2d::sImageTGA*) = mac 0x106110;
}

class cocos2d::CCTouchHandler : cocos2d::CCObject {
	virtual auto initWithDelegate(cocos2d::CCTouchDelegate*, int) = mac 0x247d10, ios 0x69f8;
}

class cocos2d::CCTurnOffTiles : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x45d870;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x45d7c0;
	virtual auto initWithDuration(float, cocos2d::CCSize const&, unsigned int) = mac 0x45d4d0;
}

class cocos2d::CCWavesTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x45dba0;
	virtual auto initWithDuration(float, cocos2d::CCSize const&, unsigned int, float) = mac 0x45da40;
}

class cocos2d::CCActionInstant : cocos2d::CCFiniteTimeAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x4542f0, ios 0x112518;
	virtual auto isDone() = mac 0x4542c0, ios 0x112500;
	virtual auto step(float) = mac 0x4542d0, ios 0x112508;
	virtual auto reverse() = mac 0x454300, ios 0x11251c;
}

class cocos2d::CCActionManager : cocos2d::CCObject {
	virtual auto update(float) = mac 0x10c9a0, ios 0x213454;
}

class cocos2d::CCBlendProtocol {
}

class cocos2d::CCConfiguration : cocos2d::CCObject {
}

class cocos2d::CCEaseBackInOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a4670, ios 0x1a8914;
	virtual auto reverse() = mac 0x2a4700, ios 0x1a89a0;
}

class cocos2d::CCEaseBounceOut : cocos2d::CCEaseBounce {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a3cd0, ios 0x1a820c;
	virtual auto reverse() = mac 0x2a3d90, ios 0x1a8238;
}

class cocos2d::CCEaseElasticIn : cocos2d::CCEaseElastic {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a2fb0, ios 0x1a7878;
	virtual auto reverse() = mac 0x2a3050, ios 0x1a7904;
}

class cocos2d::CCEaseSineInOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a2b50, ios 0x1a7684;
	virtual auto reverse() = mac 0x2a2b90, ios 0x1a76dc;
}

class cocos2d::CCKeypadHandler : cocos2d::CCObject {
	virtual auto initWithDelegate(cocos2d::CCKeypadDelegate*) = mac 0x1ff290, ios 0x2c5d8;
}

class cocos2d::CCLabelProtocol {
}

class cocos2d::CCLayerGradient : cocos2d::CCLayerColor {
	virtual auto init() = mac 0x275280;
	virtual auto updateColor() = mac 0x2753c0;
	virtual auto initWithColor(cocos2d::ccColor4B const&, cocos2d::ccColor4B const&) = mac 0x2752b0;
	virtual auto initWithColor(cocos2d::ccColor4B const&, cocos2d::ccColor4B const&, cocos2d::CCPoint const&) = mac 0x275310;
	virtual auto getStartColor() = mac 0x275610;
	virtual auto setStartColor(cocos2d::ccColor3B const&) = mac 0x275620;
	virtual auto getEndColor() = mac 0x275670;
	virtual auto setEndColor(cocos2d::ccColor3B const&) = mac 0x275640;
	virtual auto getStartOpacity() = mac 0x2756a0;
	virtual auto setStartOpacity(unsigned char) = mac 0x275680;
	virtual auto getEndOpacity() = mac 0x2756d0;
	virtual auto setEndOpacity(unsigned char) = mac 0x2756b0;
	virtual auto getVector() = mac 0x275710;
	virtual auto setVector(cocos2d::CCPoint const&) = mac 0x2756e0;
	virtual auto setCompressedInterpolation(bool) = mac 0x275730;
	virtual auto isCompressedInterpolation() = mac 0x275720;
}

class cocos2d::CCMenuItemImage : cocos2d::CCMenuItemSprite {
	virtual auto init() = mac 0x1fd750;
}

class cocos2d::CCMenuItemLabel : cocos2d::CCMenuItem {
	virtual auto activate() = mac 0x1fc240;
	virtual auto selected() = mac 0x1fc2e0;
	virtual auto unselected() = mac 0x1fc380;
	virtual auto setEnabled(bool) = mac 0x1fc3f0;
	virtual auto getDisabledColor() = mac 0x1fbb80;
	virtual auto setDisabledColor(cocos2d::ccColor3B const&);
	virtual auto getLabel() = mac 0x1fbbb0;
	virtual auto setLabel(cocos2d::CCNode*) = mac 0x1fbbc0;
}

class cocos2d::CCMouseDelegate {
	virtual auto rightKeyDown();
	virtual auto rightKeyUp();
	virtual auto scrollWheel(float, float);
}

class cocos2d::CCParticleSmoke : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24cdd0;
}

class cocos2d::CCPrettyPrinter : cocos2d::CCDataVisitor {
	virtual auto visitObject(cocos2d::CCObject const*) = mac 0x2763e0, ios 0x16b074;
	virtual auto visit(cocos2d::CCBool const*) = mac 0x276470, ios 0x16b0f4;
	virtual auto visit(cocos2d::CCInteger const*) = mac 0x276510, ios 0x16b18c;
	virtual auto visit(cocos2d::CCFloat const*) = mac 0x2765a0, ios 0x16b210;
	virtual auto visit(cocos2d::CCDouble const*) = mac 0x276630, ios 0x16b298;
	virtual auto visit(cocos2d::CCString const*) = mac 0x2766c0, ios 0x16b31c;
	virtual auto visit(cocos2d::CCArray const*) = mac 0x276700, ios 0x16b348;
	virtual auto visit(cocos2d::CCDictionary const*) = mac 0x276b50, ios 0x16b644;
	virtual auto visit(cocos2d::CCSet const*) = mac 0x276f20, ios 0x16b8d0;
	virtual auto clear() = mac 0x2763a0, ios 0x16b044;
	virtual auto getResult() = mac 0x2763c0, ios 0x16b068;
}

class cocos2d::CCProgressTimer : cocos2d::CCNodeRGBA {
	virtual auto draw() = mac 0x36e8f0;
	virtual auto setOpacity(unsigned char) = mac 0x36c720;
	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x36c590;
	virtual auto getColor() const = mac 0x36c700;
	virtual auto getOpacity() const = mac 0x36c820;
	virtual auto getMidpoint() = mac 0x36e7c0;
	virtual auto setMidpoint(cocos2d::CCPoint) = mac 0x36e7e0;
}

class cocos2d::CCRenderTexture : cocos2d::CCNode {
	virtual auto draw() = mac 0x35d570;
	virtual auto visit() = mac 0x35d4e0;
	virtual auto getSprite() = mac 0x35c620;
	virtual auto setSprite(cocos2d::CCSprite*) = mac 0x35c630;
}

class cocos2d::CCRepeatForever : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto isDone() = mac 0x1f3b40, ios 0xf0d70;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f3a90, ios 0xf0c98;
	virtual auto step(float) = mac 0x1f3ad0, ios 0xf0cd8;
	virtual auto reverse() = mac 0x1f3b50, ios 0xf0d78;
}

class cocos2d::CCSceneDelegate {
	virtual auto willSwitchToScene(cocos2d::CCScene*);
}

class cocos2d::CCTouchDelegate {
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	virtual auto ccTouchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*);
	virtual auto ccTouchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*);
	virtual auto ccTouchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*);
	virtual auto ccTouchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*);
}

class cocos2d::CCAccelAmplitude : cocos2d::CCActionInterval {
	virtual auto update(float) = mac 0xeea70;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0xeea40;
	virtual auto reverse() = mac 0xeeac0;
}

class cocos2d::CCActionInterval : cocos2d::CCFiniteTimeAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto isDone() = mac 0x1f2640, ios 0xf04a0;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f2700, ios 0xf0530;
	virtual auto step(float) = mac 0x1f2660, ios 0xf04b0;
	virtual auto reverse() = mac 0x1f2720, ios 0xf055c;
}

class cocos2d::CCAnimationCache : cocos2d::CCObject {
}

class cocos2d::CCAnimationFrame : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
}

class cocos2d::CCEaseElasticOut : cocos2d::CCEaseElastic {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a3250, ios 0x1a7abc;
	virtual auto reverse() = mac 0x2a32f0, ios 0x1a7b54;
}

class cocos2d::CCEaseRateAction : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto reverse() = mac 0x2a1930;
}

class cocos2d::CCFadeOutBLTiles : cocos2d::CCFadeOutTRTiles {
	virtual auto testFunc(cocos2d::CCSize const&, float) = mac 0x45cf10;
}

class cocos2d::CCFadeOutTRTiles : cocos2d::CCTiledGrid3DAction {
	virtual auto update(float) = mac 0x45ccc0;
	virtual auto testFunc(cocos2d::CCSize const&, float) = mac 0x45cad0;
	virtual auto transformTile(cocos2d::CCPoint const&, float) = mac 0x45cbc0;
}

class cocos2d::CCFadeOutUpTiles : cocos2d::CCFadeOutTRTiles {
	virtual auto testFunc(cocos2d::CCSize const&, float) = mac 0x45d070;
	virtual auto transformTile(cocos2d::CCPoint const&, float) = mac 0x45d0f0;
}

class cocos2d::CCKeypadDelegate {
	virtual auto keyBackClicked();
	virtual auto keyMenuClicked();
}

class cocos2d::CCLayerMultiplex : cocos2d::CCLayer {
}

class cocos2d::CCMenuItemSprite : cocos2d::CCMenuItem {
	virtual auto selected() = mac 0x1fd3f0;
	virtual auto unselected() = mac 0x1fd470;
	virtual auto setEnabled(bool) = mac 0x1fd4e0, ios 0x2d3bc;
	virtual auto getNormalImage() = mac 0x1fcee0, ios 0x2cf68;
	virtual auto setNormalImage(cocos2d::CCNode*) = mac 0x1fcef0, ios 0x2cf70;
	virtual auto getSelectedImage() = mac 0x1fcfc0, ios 0x2d044;
	virtual auto setSelectedImage(cocos2d::CCNode*) = mac 0x1fcfd0, ios 0x2d04c;
	virtual auto getDisabledImage() = mac 0x1fd070, ios 0x2d0fc;
	virtual auto setDisabledImage(cocos2d::CCNode*) = mac 0x1fd080, ios 0x2d104;
	virtual auto updateImagesVisibility() = mac 0x1fd510, ios 0x2d3dc;
}

class cocos2d::CCMenuItemToggle : cocos2d::CCMenuItem {
	virtual auto activate() = mac 0x1fe950;
	virtual auto selected() = mac 0x1fe8f0;
	virtual auto unselected() = mac 0x1fe920;
	virtual auto setEnabled(bool) = mac 0x1fea00;
	virtual auto getSelectedIndex() = mac 0x1fe8e0;
	virtual auto setSelectedIndex(unsigned int) = mac 0x1fe7e0;
	virtual auto getSubItems() = mac 0x1fde50;
	virtual auto setSubItems(cocos2d::CCArray*) = mac 0x1fde10;
}

class cocos2d::CCParticleFlower : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24c1a0;
}

class cocos2d::CCParticleGalaxy : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24be90;
}

class cocos2d::CCParticleMeteor : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24c4c0;
}

class cocos2d::CCParticleSpiral : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24c7e0;
}

class cocos2d::CCParticleSystem : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	virtual auto update(float) = mac 0x46be50, ios 0xf8fc4;
	virtual auto init() = mac 0x4692b0, ios 0xf7040;
	virtual auto setScaleX(float) = mac 0x46cdf0, ios 0xf99b8;
	virtual auto setScaleY(float) = mac 0x46ce10, ios 0xf99c4;
	virtual auto setScale(float) = mac 0x46cdb0, ios 0xf99a0;
	virtual auto setRotation(float) = mac 0x46cdd0, ios 0xf99ac;
	virtual auto getBatchNode() = mac 0x46ccd0, ios 0xf995c;
	virtual auto setBatchNode(cocos2d::CCParticleBatchNode*) = mac 0x46cce0, ios 0xf9964;
	virtual auto getParticleCount() = mac 0x46c8a0, ios 0xf9734;
	virtual auto getDuration() = mac 0x46c8b0, ios 0xf973c;
	virtual auto setDuration(float) = mac 0x46c8c0, ios 0xf9744;
	virtual auto getSourcePosition() = mac 0x46c8d0, ios 0xf974c;
	virtual auto setSourcePosition(cocos2d::CCPoint const&) = mac 0x46c8e0, ios 0xf9754;
	virtual auto getPosVar() = mac 0x46c900, ios 0xf975c;
	virtual auto setPosVar(cocos2d::CCPoint const&) = mac 0x46c910, ios 0xf9764;
	virtual auto getLife() = mac 0x46c930, ios 0xf976c;
	virtual auto setLife(float) = mac 0x46c940, ios 0xf9774;
	virtual auto getLifeVar() = mac 0x46c950, ios 0xf977c;
	virtual auto setLifeVar(float) = mac 0x46c960, ios 0xf9784;
	virtual auto getAngle() = mac 0x46c970, ios 0xf978c;
	virtual auto setAngle(float) = mac 0x46c980, ios 0xf9794;
	virtual auto getAngleVar() = mac 0x46c990, ios 0xf979c;
	virtual auto setAngleVar(float) = mac 0x46c9a0, ios 0xf97a4;
	virtual auto getGravity() = mac 0x46c780, ios 0xf96a4;
	virtual auto setGravity(cocos2d::CCPoint const&) = mac 0x46c760, ios 0xf969c;
	virtual auto getSpeed() = mac 0x46c7a0, ios 0xf96b4;
	virtual auto setSpeed(float) = mac 0x46c790, ios 0xf96ac;
	virtual auto getSpeedVar() = mac 0x46c7c0, ios 0xf96c4;
	virtual auto setSpeedVar(float) = mac 0x46c7b0, ios 0xf96bc;
	virtual auto getTangentialAccel() = mac 0x46c6d0, ios 0xf9654;
	virtual auto setTangentialAccel(float) = mac 0x46c6c0, ios 0xf964c;
	virtual auto getTangentialAccelVar() = mac 0x46c6f0, ios 0xf9664;
	virtual auto setTangentialAccelVar(float) = mac 0x46c6e0, ios 0xf965c;
	virtual auto getRadialAccel() = mac 0x46c710, ios 0xf9674;
	virtual auto setRadialAccel(float) = mac 0x46c700, ios 0xf966c;
	virtual auto getRadialAccelVar() = mac 0x46c730, ios 0xf9684;
	virtual auto setRadialAccelVar(float) = mac 0x46c720, ios 0xf967c;
	virtual auto getRotationIsDir() = mac 0x46c750, ios 0xf9694;
	virtual auto setRotationIsDir(bool) = mac 0x46c740, ios 0xf968c;
	virtual auto getStartRadius() = mac 0x46c7e0, ios 0xf96d4;
	virtual auto setStartRadius(float) = mac 0x46c7d0, ios 0xf96cc;
	virtual auto getStartRadiusVar() = mac 0x46c800, ios 0xf96e4;
	virtual auto setStartRadiusVar(float) = mac 0x46c7f0, ios 0xf96dc;
	virtual auto getEndRadius() = mac 0x46c820, ios 0xf96f4;
	virtual auto setEndRadius(float) = mac 0x46c810, ios 0xf96ec;
	virtual auto getEndRadiusVar() = mac 0x46c840, ios 0xf9704;
	virtual auto setEndRadiusVar(float) = mac 0x46c830, ios 0xf96fc;
	virtual auto getRotatePerSecond() = mac 0x46c860, ios 0xf9714;
	virtual auto setRotatePerSecond(float) = mac 0x46c850, ios 0xf970c;
	virtual auto getRotatePerSecondVar() = mac 0x46c880, ios 0xf9724;
	virtual auto setRotatePerSecondVar(float) = mac 0x46c870, ios 0xf971c;
	virtual auto isActive() = mac 0x46c890, ios 0xf972c;
	virtual auto isBlendAdditive() = mac 0x46c6a0, ios 0xf9628;
	virtual auto setBlendAdditive(bool) = mac 0x46c640, ios 0xf95d4;
	virtual auto getStartSize() = mac 0x46c9b0, ios 0xf97ac;
	virtual auto setStartSize(float) = mac 0x46c9c0, ios 0xf97b4;
	virtual auto getStartSizeVar() = mac 0x46c9d0, ios 0xf97bc;
	virtual auto setStartSizeVar(float) = mac 0x46c9e0, ios 0xf97c4;
	virtual auto getEndSize() = mac 0x46c9f0, ios 0xf97cc;
	virtual auto setEndSize(float) = mac 0x46ca00, ios 0xf97d4;
	virtual auto getEndSizeVar() = mac 0x46ca10, ios 0xf97dc;
	virtual auto setEndSizeVar(float) = mac 0x46ca20, ios 0xf97e4;
	virtual auto getStartColor() = mac 0x46ca30, ios 0xf97ec;
	virtual auto setStartColor(cocos2d::_ccColor4F const&) = mac 0x46ca40, ios 0xf97f4;
	virtual auto getStartColorVar() = mac 0x46ca60, ios 0xf9800;
	virtual auto setStartColorVar(cocos2d::_ccColor4F const&) = mac 0x46ca70, ios 0xf9808;
	virtual auto getEndColor() = mac 0x46ca90, ios 0xf9814;
	virtual auto setEndColor(cocos2d::_ccColor4F const&) = mac 0x46caa0, ios 0xf981c;
	virtual auto getEndColorVar() = mac 0x46cac0, ios 0xf9828;
	virtual auto setEndColorVar(cocos2d::_ccColor4F const&) = mac 0x46cad0, ios 0xf9830;
	virtual auto getStartSpin() = mac 0x46caf0, ios 0xf983c;
	virtual auto setStartSpin(float) = mac 0x46cb00, ios 0xf9844;
	virtual auto getStartSpinVar() = mac 0x46cb10, ios 0xf984c;
	virtual auto setStartSpinVar(float) = mac 0x46cb20, ios 0xf9854;
	virtual auto getEndSpin() = mac 0x46cb30, ios 0xf985c;
	virtual auto setEndSpin(float) = mac 0x46cb40, ios 0xf9864;
	virtual auto getEndSpinVar() = mac 0x46cb50, ios 0xf986c;
	virtual auto setEndSpinVar(float) = mac 0x46cb60, ios 0xf9874;
	virtual auto getEmissionRate() = mac 0x46cb70, ios 0xf987c;
	virtual auto setEmissionRate(float) = mac 0x46cb80, ios 0xf9884;
	virtual auto getTotalParticles() = mac 0x46cb90, ios 0xf988c;
	virtual auto setTotalParticles(unsigned int) = mac 0x46cba0, ios 0xf9894;
	virtual auto getTexture() = mac 0x46c620, ios 0xf95c4;
	virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0x46c4f0, ios 0xf94f4;
	virtual auto getBlendFunc() = mac 0x46cbb0, ios 0xf989c;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc) = mac 0x46cbd0, ios 0xf98ac;
	virtual auto getOpacityModifyRGB() = mac 0x46cc50, ios 0xf991c;
	virtual auto setOpacityModifyRGB(bool) = mac 0x46cc60, ios 0xf9924;
	virtual auto getPositionType() = mac 0x46cc70, ios 0xf992c;
	virtual auto setPositionType(cocos2d::tCCPositionType) = mac 0x46cc80, ios 0xf9934;
	virtual auto isAutoRemoveOnFinish() = mac 0x46cc90, ios 0xf993c;
	virtual auto setAutoRemoveOnFinish(bool) = mac 0x46cca0, ios 0xf9944;
	virtual auto getEmitterMode() = mac 0x46ccb0, ios 0xf994c;
	virtual auto setEmitterMode(int) = mac 0x46ccc0, ios 0xf9954;
	virtual auto initWithTotalParticles(unsigned int) = mac 0x46b130, ios 0xf87f4;
	virtual auto updateQuadWithParticle(cocos2d::sCCParticle*, cocos2d::CCPoint const&) = mac 0x46c4d0, ios 0xf94ec;
	virtual auto postStep() = mac 0x46c4e0, ios 0xf94f0;
	virtual auto updateWithNoTime() = mac 0x46c4b0, ios 0xf94dc;
	virtual auto updateBlendFunc() = mac 0x46c5b0, ios 0xf955c;
}

class cocos2d::CCProfilingTimer : cocos2d::CCObject {
}

class cocos2d::CCProgressFromTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x23d650;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x23d640;
	virtual auto reverse() = mac 0x23d590;
}

class cocos2d::CCTMXObjectGroup : cocos2d::CCObject {
	virtual auto getProperties() = mac 0x25f800;
	virtual auto setProperties(cocos2d::CCDictionary*) = mac 0x25f810;
	virtual auto getObjects() = mac 0x25f850;
	virtual auto setObjects(cocos2d::CCArray*) = mac 0x25f860;
}

class cocos2d::CCTMXTilesetInfo : cocos2d::CCObject {
}

class cocos2d::CCTargetedAction : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f9d40;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f9d00;
	virtual auto stop() = mac 0x1f9d30;
}

class cocos2d::CCTransitionFade : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x8eb30, ios 0x12c2d0;
	virtual auto onExit() = mac 0x8ec60, ios 0x12c3d4;
	virtual auto initWithDuration(float, cocos2d::CCScene*) = mac 0x8eb10, ios 0x12c2a8;
	virtual auto initWithDuration(float, cocos2d::CCScene*, cocos2d::ccColor3B const&) = mac 0x8ea50, ios 0x12c260;
}

class cocos2d::EGLTouchDelegate {
}

class cocos2d::CCAutoreleasePool : cocos2d::CCObject {
}

class cocos2d::CCDeccelAmplitude : cocos2d::CCActionInterval {
	virtual auto update(float) = mac 0xeedb0;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0xeed80;
	virtual auto reverse() = mac 0xeee10;
}

class cocos2d::CCEGLViewProtocol {
	virtual auto getFrameSize() const = mac 0x29d950, ios 0x20eee4;
	virtual auto setFrameSize(float, float) = mac 0x29d960, ios 0x20eeec;
	virtual auto getVisibleSize() const = mac 0x29d9a0, ios 0x20ef30;
	virtual auto getVisibleOrigin() const = mac 0x29d9f0, ios 0x20ef60;
	virtual auto setDesignResolutionSize(float, float, ResolutionPolicy) = mac 0x29d7c0, ios 0x20edb0;
	virtual auto getDesignResolutionSize() const = mac 0x29d940, ios 0x20eedc;
	virtual auto setTouchDelegate(cocos2d::EGLTouchDelegate*) = mac 0x29da60, ios 0x20efac;
	virtual auto setViewPortInPoints(float, float, float, float) = mac 0x29da70, ios 0x20efb4;
	virtual auto setScissorInPoints(float, float, float, float) = mac 0x29dab0, ios 0x20efe8;
	virtual auto isScissorEnabled() = mac 0x29daf0, ios 0x20f01c;
	virtual auto getScissorRect() = mac 0x29db10, ios 0x20f03c;
	virtual auto setViewName(char const*) = mac 0x29dba0, ios 0x20f0d4;
	virtual auto handleTouchesBegin(int, int*, float*, float*) = mac 0x29dbd0, ios 0x20f0f0;
	virtual auto handleTouchesMove(int, int*, float*, float*) = mac 0x29deb0, ios 0x20f350;
	virtual auto handleTouchesEnd(int, int*, float*, float*) = mac 0x29e1d0, ios 0x20f5cc;
	virtual auto handleTouchesCancel(int, int*, float*, float*) = mac 0x29e260, ios 0x20f66c;
	virtual auto pollInputEvents() = mac 0x29e320, ios 0x20f724;
}

class cocos2d::CCEaseBounceInOut : cocos2d::CCEaseBounce {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a3f40, ios 0x1a83b8;
	virtual auto reverse() = mac 0x2a40f0, ios 0x1a8424;
}

class cocos2d::CCKeyboardHandler : cocos2d::CCObject {
	virtual auto initWithDelegate(cocos2d::CCKeyboardDelegate*) = mac 0x241ff0, ios 0x13f8b8;
}

class cocos2d::CCMouseDispatcher : cocos2d::CCObject {
}

class cocos2d::CCSpriteBatchNode : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	virtual auto init() = mac 0xbb680, ios 0x130c20;
	virtual auto addChild(cocos2d::CCNode*) = mac 0xbbb40, ios 0x131034;
	virtual auto addChild(cocos2d::CCNode*, int) = mac 0xbbb50, ios 0x131038;
	virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0xbb930, ios 0x130e0c;
	virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0xbbbb0, ios 0x1310a4;
	virtual auto removeAllChildrenWithCleanup(bool) = mac 0xbbe30, ios 0x1312d8;
	virtual auto reorderChild(cocos2d::CCNode*, int) = mac 0xbbb60, ios 0x13103c;
	virtual auto sortAllChildren() = mac 0xbbee0, ios 0x13137c;
	virtual auto draw() = mac 0xbc540, ios 0x13191c;
	virtual auto visit() = mac 0xbb880, ios 0x130d54;
	virtual auto getTexture() = mac 0xbcd70, ios 0x131a70;
	virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0xbcda0, ios 0x131a90;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc) = mac 0xbcd30, ios 0x131a50;
	virtual auto getBlendFunc() = mac 0xbcd50, ios 0x131a60;
}

class cocos2d::CCTextureProtocol : cocos2d::CCBlendProtocol {
}

class cocos2d::CCTouchDispatcher : cocos2d::CCObject, cocos2d::EGLTouchDelegate {
	virtual auto touchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x282050, ios 0xccbf4;
	virtual auto touchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x282090, ios 0xccc20;
	virtual auto touchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x2820d0, ios 0xccc4c;
	virtual auto touchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x282110, ios 0xccc78;
}

class cocos2d::CCTransitionFlipX : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8cf90;
}

class cocos2d::CCTransitionFlipY : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8d390;
}

class cocos2d::CCTransitionScene : cocos2d::CCScene {
	virtual auto onEnter() = mac 0x8a1a0, ios 0x12be58;
	virtual auto onExit() = mac 0x8a1f0, ios 0x12beac;
	virtual auto cleanup() = mac 0x8a250, ios 0x12bf00;
	virtual auto draw() = mac 0x89f40, ios 0x12bc10;
	virtual auto initWithDuration(float, cocos2d::CCScene*) = mac 0x89ea0, ios 0x12bb80;
	virtual auto sceneOrder() = mac 0x89f30, ios 0x12bc04;
}

class cocos2d::CCCardinalSplineBy : cocos2d::CCCardinalSplineTo {
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1684b0;
	virtual auto reverse() = mac 0x168160;
	virtual auto updatePosition(cocos2d::CCPoint&) = mac 0x168110;
}

class cocos2d::CCCardinalSplineTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x167c40;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x167aa0;
	virtual auto reverse() = mac 0x167f60;
	virtual auto updatePosition(cocos2d::CCPoint&) = mac 0x167f30;
}

class cocos2d::CCEaseElasticInOut : cocos2d::CCEaseElastic {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a34f0, ios 0x1a7d00;
	virtual auto reverse() = mac 0x2a3620, ios 0x1a7e00;
}

class cocos2d::CCFadeOutDownTiles : cocos2d::CCFadeOutUpTiles {
	virtual auto testFunc(cocos2d::CCSize const&, float) = mac 0x45d280;
}

class cocos2d::CCFiniteTimeAction : cocos2d::CCAction {
	virtual auto reverse();
}

class cocos2d::CCKeyboardDelegate {
	virtual auto keyDown(cocos2d::enumKeyCodes);
	virtual auto keyUp(cocos2d::enumKeyCodes);
}

class cocos2d::CCKeypadDispatcher : cocos2d::CCObject {
}

class cocos2d::CCShatteredTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x45bdc0;
	virtual auto initWithDuration(float, cocos2d::CCSize const&, int, bool) = mac 0x45bc70;
}

class cocos2d::CCSpriteFrameCache : cocos2d::CCObject {
}

class cocos2d::CCToggleVisibility : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float);
}

class cocos2d::CCTransitionFadeBL : cocos2d::CCTransitionFadeTR {
	virtual auto actionWithSize(cocos2d::CCSize const&) = mac 0x90380;
}

class cocos2d::CCTransitionFadeTR : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8ffc0;
	virtual auto sceneOrder() = mac 0x8ffb0;
	virtual auto actionWithSize(cocos2d::CCSize const&) = mac 0x900c0;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval*) = mac 0x900e0;
}

class cocos2d::CCTransitionFadeUp : cocos2d::CCTransitionFadeTR {
	virtual auto actionWithSize(cocos2d::CCSize const&) = mac 0x90620;
}

class cocos2d::CCEaseExponentialIn : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a2160, ios 0x1a6d3c;
	virtual auto reverse() = mac 0x2a21b0, ios 0x1a6da4;
}

class cocos2d::CCMenuItemAtlasFont : cocos2d::CCMenuItemLabel {
}

class cocos2d::CCParticleBatchNode : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	virtual auto addChild(cocos2d::CCNode*) = mac 0x206360;
	virtual auto addChild(cocos2d::CCNode*, int) = mac 0x206370;
	virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x206380;
	virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0x206bd0;
	virtual auto removeAllChildrenWithCleanup(bool) = mac 0x206d10;
	virtual auto reorderChild(cocos2d::CCNode*, int) = mac 0x2067c0;
	virtual auto draw() = mac 0x206da0;
	virtual auto visit() = mac 0x2062e0;
	virtual auto getTexture() = mac 0x206f90;
	virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0x206ee0;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc) = mac 0x206fc0;
	virtual auto getBlendFunc() = mac 0x206fe0;
}

class cocos2d::CCParticleExplosion : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24cad0;
}

class cocos2d::CCParticleFireworks : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int) = mac 0x24b910;
}

class cocos2d::CCTextFieldDelegate {
	virtual auto onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF*);
	virtual auto onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF*);
	virtual auto onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int);
	virtual auto onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF*, char const*, int);
	virtual auto onDraw(cocos2d::CCTextFieldTTF*);
	virtual auto textChanged();
}

class cocos2d::CCTiledGrid3DAction : cocos2d::CCGridAction {
	virtual auto getGrid() = mac 0xee350;
}

class cocos2d::CCTransitionMoveInB : cocos2d::CCTransitionMoveInL {
	virtual auto initScenes() = mac 0x8ba90;
}

class cocos2d::CCTransitionMoveInL : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8b0b0, ios 0x12bf40;
	virtual auto initScenes() = mac 0x8b1e0;
	virtual auto action() = mac 0x8b180, ios 0x12bfe4;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval*) = mac 0x8b1c0, ios 0x12c024;
}

class cocos2d::CCTransitionMoveInR : cocos2d::CCTransitionMoveInL {
	virtual auto initScenes() = mac 0x8b4d0;
}

class cocos2d::CCTransitionMoveInT : cocos2d::CCTransitionMoveInL {
	virtual auto initScenes() = mac 0x8b7b0, ios 0x12c10c;
}

class cocos2d::CCComponentContainer {
	virtual auto get(char const*) const = mac 0x1663a0, ios 0xdc854;
	virtual auto add(cocos2d::CCComponent*) = mac 0x166470, ios 0xdc970;
	virtual auto remove(char const*) = mac 0x1665f0, ios 0xdcb74;
	virtual auto remove(cocos2d::CCComponent*) = mac 0x1668d0, ios 0xdce54;
	virtual auto removeAll() = mac 0x166ab0, ios 0xdd00c;
	virtual auto visit(float) = mac 0x166cb0, ios 0xdd1b8;
}

class cocos2d::CCEaseExponentialOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a2360, ios 0x1a6f24;
	virtual auto reverse() = mac 0x2a23b0, ios 0x1a6f80;
}

class cocos2d::CCKeyboardDispatcher : cocos2d::CCObject {
}

class cocos2d::CCNotificationCenter : cocos2d::CCObject {
}

class cocos2d::CCParticleSystemQuad : cocos2d::CCParticleSystem {
	virtual auto draw() = mac 0x36b850, ios 0x16e080;
	virtual auto setBatchNode(cocos2d::CCParticleBatchNode*) = mac 0x36bb70, ios 0x16e294;
	virtual auto setTotalParticles(unsigned int) = mac 0x36b910, ios 0x16e144;
	virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0x36b3c0, ios 0x16ddd4;
	virtual auto initWithTotalParticles(unsigned int) = mac 0x36aa80, ios 0x16d7b4;
	virtual auto updateQuadWithParticle(cocos2d::sCCParticle*, cocos2d::CCPoint const&) = mac 0x36b570, ios 0x16de3c;
	virtual auto postStep() = mac 0x36b800, ios 0x16e034;
}

class cocos2d::CCScriptHandlerEntry : cocos2d::CCObject {
}

class cocos2d::CCTransitionFadeDown : cocos2d::CCTransitionFadeTR {
	virtual auto actionWithSize(cocos2d::CCSize const&) = mac 0x908c0;
}

class cocos2d::CCTransitionJumpZoom : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x8abf0;
}

class cocos2d::CCTransitionPageTurn : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x20d1d0;
	virtual auto sceneOrder() = mac 0x20d1b0;
	virtual auto initWithDuration(float, cocos2d::CCScene*, bool) = mac 0x20d190;
}

class cocos2d::CCTransitionProgress : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x137970;
	virtual auto onExit() = mac 0x137b30;
	virtual auto sceneOrder() = mac 0x137b60;
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*) = mac 0x137ba0;
	virtual auto setupTransition() = mac 0x137b70;
}

class cocos2d::CCTransitionRotoZoom : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x8a7d0;
}

class cocos2d::CCTransitionSlideInB : cocos2d::CCTransitionSlideInL {
	virtual auto sceneOrder() = mac 0x8c8f0;
	virtual auto initScenes() = mac 0x8c900;
	virtual auto action() = mac 0x8c970;
}

class cocos2d::CCTransitionSlideInL : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8bc90;
	virtual auto sceneOrder() = mac 0x8bd80;
	virtual auto initScenes() = mac 0x8bd90;
	virtual auto action() = mac 0x8be00;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval*) = mac 0x8be60;
}

class cocos2d::CCTransitionSlideInR : cocos2d::CCTransitionSlideInL {
	virtual auto sceneOrder() = mac 0x8c210;
	virtual auto initScenes() = mac 0x8c220;
	virtual auto action() = mac 0x8c290;
}

class cocos2d::CCTransitionSlideInT : cocos2d::CCTransitionSlideInL {
	virtual auto sceneOrder() = mac 0x8c580;
	virtual auto initScenes() = mac 0x8c590;
	virtual auto action() = mac 0x8c600;
}

class cocos2d::CCActionTweenDelegate {
}

class cocos2d::CCApplicationProtocol {
	virtual auto applicationWillBecomeActive();
	virtual auto applicationWillResignActive();
	virtual auto trySaveGame();
	virtual auto gameDidSave();
	virtual auto openURL(char const*);
}

class cocos2d::CCBMFontConfiguration : cocos2d::CCObject {
}

class cocos2d::CCDisplayLinkDirector : cocos2d::CCDirector {
	virtual auto setAnimationInterval(double) = mac 0x24b180, ios 0xed918;
	virtual auto stopAnimation() = mac 0x24b170, ios 0xed90c;
	virtual auto startAnimation() = mac 0x24b0f0, ios 0xed89c;
	virtual auto mainLoop() = mac 0x24b130, ios 0xed8d8;
}

class cocos2d::CCTransitionCrossFade : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x8ef30;
	virtual auto onExit() = mac 0x8f290;
	virtual auto draw() = mac 0x8ef20;
}

class cocos2d::CCTransitionEaseScene {
}

class cocos2d::CCTransitionSplitCols : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8f940;
	virtual auto action() = mac 0x8fa30;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval*) = mac 0x8fa50;
}

class cocos2d::CCTransitionSplitRows : cocos2d::CCTransitionSplitCols {
	virtual auto action() = mac 0x8fc20;
}

class cocos2d::CCTransitionZoomFlipX : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8dbb0;
}

class cocos2d::CCTransitionZoomFlipY : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8e020;
}

class cocos2d::CCAccelDeccelAmplitude : cocos2d::CCActionInterval {
	virtual auto update(float) = mac 0xee6f0;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0xee6c0;
	virtual auto reverse() = mac 0xee750;
}

class cocos2d::CCEaseExponentialInOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a2560, ios 0x1a7100;
	virtual auto reverse() = mac 0x2a25d0, ios 0x1a716c;
}

class cocos2d::CCNotificationObserver : cocos2d::CCObject {
	virtual auto getTarget() = mac 0x1326e0;
	virtual auto getSelector() = mac 0x1326f0;
	virtual auto getName() = mac 0x132700;
	virtual auto getObject() = mac 0x132710;
	virtual auto getHandler() = mac 0x132720;
	virtual auto setHandler(int) = mac 0x132730;
}

class cocos2d::CCStandardTouchHandler : cocos2d::CCTouchHandler {
	virtual auto initWithDelegate(cocos2d::CCTouchDelegate*, int) = mac 0x247ed0, ios 0x6afc;
}

class cocos2d::CCTargetedTouchHandler : cocos2d::CCTouchHandler {
}

class cocos2d::CCTransitionShrinkGrow : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8cc50;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval*) = mac 0x8cde0;
}

class cocos2d::CCAccelerometerDelegate {
	virtual auto didAccelerate(cocos2d::CCAcceleration*);
}

class cocos2d::CCTransitionFlipAngular : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8d7a0;
}

class cocos2d::CCTransitionTurnOffTiles : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8f590;
	virtual auto sceneOrder() = mac 0x8f580;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval*) = mac 0x8f6a0;
}

class cocos2d::CCSceneTransitionDelegate {
	virtual auto sceneWillResume();
}

class cocos2d::CCTouchScriptHandlerEntry : cocos2d::CCScriptHandlerEntry {
}

class cocos2d::CCTransitionProgressInOut : cocos2d::CCTransitionProgress {
	virtual auto sceneOrder() = mac 0x1382d0;
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*) = mac 0x138300;
	virtual auto setupTransition() = mac 0x1382e0;
}

class cocos2d::CCTransitionProgressOutIn : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*) = mac 0x1384c0;
}

class cocos2d::CCTransitionSceneOriented : cocos2d::CCTransitionScene {
	virtual auto initWithDuration(float, cocos2d::CCScene*, cocos2d::tOrientation) = mac 0x8a4e0;
}

class cocos2d::CCTransitionZoomFlipAngular : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8e4a0;
}

class cocos2d::CCTransitionProgressRadialCW : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*) = mac 0x137dd0;
}

class cocos2d::CCTransitionProgressVertical : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*) = mac 0x138110;
}

class cocos2d::CCSchedulerScriptHandlerEntry : cocos2d::CCScriptHandlerEntry {
}

class cocos2d::CCTransitionProgressRadialCCW : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*) = mac 0x137bb0;
}

class cocos2d::CCTransitionProgressHorizontal : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*) = mac 0x137f50;
}

class cocos2d::CCSet : cocos2d::CCObject {
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x45b090, ios 0x10ec04;
}

class cocos2d::CCBool : cocos2d::CCObject {
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&);
}

class cocos2d::CCHide : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float);
	virtual auto reverse();
}

class cocos2d::CCMenu : cocos2d::CCLayerRGBA {
	virtual auto init() = mac 0x438b90, ios 0x131ec0;
	virtual auto addChild(cocos2d::CCNode*) = mac 0x438ba0, ios 0x131ec8;
	virtual auto addChild(cocos2d::CCNode*, int) = mac 0x438bb0, ios 0x131ecc;
	virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x438bc0, ios 0x131ed0;
	virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0x438c20, ios 0x131f1c;
	virtual auto onExit() = mac 0x438bd0, ios 0x131ed4;
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x438d20, ios 0x131fe8;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x4390b0, ios 0x1322ac;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x438fd0, ios 0x1321bc;
	virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x439050, ios 0x132244;
	virtual auto registerWithTouchDispatcher() = mac 0x438cd0, ios 0x131f8c;
	virtual auto setOpacityModifyRGB(bool);
	virtual auto isOpacityModifyRGB();
	virtual auto isEnabled();
	virtual auto setEnabled(bool);
}

class cocos2d::CCNode : cocos2d::CCObject {
	virtual auto update(float) = mac 0x1241a0, ios 0x15f124;
	virtual auto init() = mac 0x122910, ios 0x15dd34;
	virtual auto setZOrder(int) = mac 0x1229a0, ios 0x15dd84;
	virtual auto _setZOrder(int) = mac 0x122990, ios 0x15dd7c;
	virtual auto getZOrder() = mac 0x122980, ios 0x15dd74;
	virtual auto setVertexZ(float) = mac 0x1229f0, ios 0x15dddc;
	virtual auto getVertexZ() = mac 0x1229e0, ios 0x15ddd4;
	virtual auto setScaleX(float) = mac 0x122b10, ios 0x15de8c;
	virtual auto getScaleX() = mac 0x122b00, ios 0x15de84;
	virtual auto setScaleY(float) = mac 0x122b40, ios 0x15dea8;
	virtual auto getScaleY() = mac 0x122b30, ios 0x15dea0;
	virtual auto setScale(float) = mac 0x122ac0, ios 0x15de5c;
	virtual auto getScale() = mac 0x122ab0, ios 0x15de54;
	virtual auto setScale(float, float) = mac 0x122ae0, ios 0x15de70;
	virtual auto setPosition(cocos2d::CCPoint const&) = mac 0x122b70, ios 0x15dec4;
	virtual auto getPosition() = mac 0x122b60, ios 0x15debc;
	virtual auto setPosition(float, float) = mac 0x122ba0, ios 0x15def0;
	virtual auto getPosition(float*, float*) = mac 0x122b90, ios 0x15dedc;
	virtual auto setPositionX(float) = mac 0x122c00, ios 0x15df40;
	virtual auto getPositionX() = mac 0x122be0, ios 0x15df30;
	virtual auto setPositionY(float) = mac 0x122c40, ios 0x15df84;
	virtual auto getPositionY() = mac 0x122bf0, ios 0x15df38;
	virtual auto setSkewX(float) = mac 0x122930, ios 0x15dd44;
	virtual auto getSkewX() = mac 0x122920, ios 0x15dd3c;
	virtual auto setSkewY(float) = mac 0x122960, ios 0x15dd60;
	virtual auto getSkewY() = mac 0x122950, ios 0x15dd58;
	virtual auto setAnchorPoint(cocos2d::CCPoint const&) = mac 0x122d90, ios 0x15e098;
	virtual auto getAnchorPoint() = mac 0x122d80, ios 0x15e090;
	virtual auto getAnchorPointInPoints() = mac 0x122d70, ios 0x15e088;
	virtual auto setContentSize(cocos2d::CCSize const&) = mac 0x122e50, ios 0x15e158;
	virtual auto getContentSize() const = mac 0x122e00, ios 0x15e118;
	virtual auto getScaledContentSize() = mac 0x122e10, ios 0x15e120;
	virtual auto setVisible(bool) = mac 0x122d60, ios 0x15e080;
	virtual auto isVisible() = mac 0x122d50, ios 0x15e078;
	virtual auto setRotation(float) = mac 0x122a10, ios 0x15ddec;
	virtual auto getRotation() = mac 0x122a00, ios 0x15dde4;
	virtual auto setRotationX(float) = mac 0x122a60, ios 0x15de24;
	virtual auto getRotationX() = mac 0x122a50, ios 0x15de1c;
	virtual auto setRotationY(float) = mac 0x122a90, ios 0x15de40;
	virtual auto getRotationY() = mac 0x122a80, ios 0x15de38;
	virtual auto setOrderOfArrival(unsigned int) = mac 0x122f60, ios 0x15e230;
	virtual auto getOrderOfArrival() = mac 0x122f50, ios 0x15e228;
	virtual auto setGLServerState(cocos2d::ccGLServerState) = mac 0x122fa0, ios 0x15e250;
	virtual auto getGLServerState() = mac 0x122f90, ios 0x15e248;
	virtual auto ignoreAnchorPointForPosition(bool) = mac 0x122f00, ios 0x15e1f8;
	virtual auto isIgnoreAnchorPointForPosition() = mac 0x122ef0, ios 0x15e1f0;
	virtual auto addChild(cocos2d::CCNode*) = mac 0x1233d0, ios 0x15e5d4;
	virtual auto addChild(cocos2d::CCNode*, int) = mac 0x1233b0, ios 0x15e5c4;
	virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x1232a0, ios 0x15e4e4;
	virtual auto getChildByTag(int) = mac 0x123220, ios 0x15e460;
	virtual auto getChildren() = mac 0x122c80, ios 0x15dfcc;
	virtual auto getChildrenCount() const = mac 0x122c90, ios 0x15dfd4;
	virtual auto setParent(cocos2d::CCNode*) = mac 0x122ee0, ios 0x15e1e8;
	virtual auto getParent() = mac 0x122ed0, ios 0x15e1e0;
	virtual auto removeFromParent() = mac 0x1233f0, ios 0x15e5e8;
	virtual auto removeFromParentAndCleanup(bool) = mac 0x123410, ios 0x15e5f8;
	virtual auto removeMeAndCleanup() = mac 0x123440, ios 0x15e620;
	virtual auto removeChild(cocos2d::CCNode*) = mac 0x123460, ios 0x15e630;
	virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0x123480, ios 0x15e640;
	virtual auto removeChildByTag(int) = mac 0x1235a0, ios 0x15e724;
	virtual auto removeChildByTag(int, bool) = mac 0x1235c0, ios 0x15e734;
	virtual auto removeAllChildren() = mac 0x123600, ios 0x15e788;
	virtual auto removeAllChildrenWithCleanup(bool) = mac 0x123620, ios 0x15e798;
	virtual auto reorderChild(cocos2d::CCNode*, int) = mac 0x123760, ios 0x15e87c;
	virtual auto sortAllChildren() = mac 0x1237b0, ios 0x15e8d4;
	virtual auto getGrid() = mac 0x122d00, ios 0x15e034;
	virtual auto setGrid(cocos2d::CCGridBase*) = mac 0x122d10, ios 0x15e03c;
	virtual auto getUserData() = mac 0x122f30, ios 0x15e218;
	virtual auto setUserData(void*) = mac 0x122f40, ios 0x15e220;
	virtual auto getUserObject() = mac 0x122f80, ios 0x15e240;
	virtual auto setUserObject(cocos2d::CCObject*) = mac 0x122fb0, ios 0x15e258;
	virtual auto getShaderProgram() = mac 0x122f70, ios 0x15e238;
	virtual auto setShaderProgram(cocos2d::CCGLProgram*) = mac 0x122ff0, ios 0x15e294;
	virtual auto getCamera() = mac 0x122cb0, ios 0x15dfe4;
	virtual auto isRunning() = mac 0x122ec0, ios 0x15e1d8;
	virtual auto registerScriptHandler(int) = mac 0x123d90, ios 0x15ee94;
	virtual auto unregisterScriptHandler() = mac 0x123dc0, ios 0x15eec4;
	virtual auto onEnter() = mac 0x123a90, ios 0x15eb94;
	virtual auto onEnterTransitionDidFinish() = mac 0x123b90, ios 0x15ec94;
	virtual auto onExit() = mac 0x123ca0, ios 0x15edb0;
	virtual auto onExitTransitionDidStart() = mac 0x123c00, ios 0x15ed18;
	virtual auto cleanup() = mac 0x123100, ios 0x15e3a4;
	virtual auto draw() = mac 0x123840, ios 0x15e974;
	virtual auto visit() = mac 0x123850, ios 0x15e978;
	virtual auto setActionManager(cocos2d::CCActionManager*) = mac 0x123e00, ios 0x15ef04;
	virtual auto getActionManager() = mac 0x123e50, ios 0x15ef54;
	virtual auto setScheduler(cocos2d::CCScheduler*) = mac 0x123f20, ios 0x15efb8;
	virtual auto getScheduler() = mac 0x123f70, ios 0x15f008;
	virtual auto updateTransform() = mac 0x1249d0, ios 0x15f648;
	virtual auto nodeToParentTransform() = mac 0x124210, ios 0x15f1a4;
	virtual auto parentToNodeTransform() = mac 0x1245d0, ios 0x15f410;
	virtual auto nodeToWorldTransform() = mac 0x124670, ios 0x15f490;
	virtual auto worldToNodeTransform() = mac 0x124710, ios 0x15f51c;
	virtual auto addComponent(cocos2d::CCComponent*) = mac 0x124a40, ios 0x15f6a4;
	virtual auto removeComponent(char const*) = mac 0x124a60, ios 0x15f6b4;
	virtual auto removeComponent(cocos2d::CCComponent*) = mac 0x124a80, ios 0x15f6c4;
	virtual auto removeAllComponents() = mac 0x124aa0, ios 0x15f6d4;
	virtual auto updateTweenAction(float, char const*) = mac 0x1249c0, ios 0x15f644;
}

class cocos2d::CCShow : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float);
	virtual auto reverse();
}

class cocos2d::CCArray : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x41a880, ios 0x165398;
}

class cocos2d::CCBlink : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f79f0, ios 0xf2c3c;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f78b0, ios 0xf2b08;
	virtual auto stop() = mac 0x1f7880, ios 0xf2ad4;
	virtual auto reverse() = mac 0x1f7a70, ios 0xf2cc8;
}

class cocos2d::CCFlipX : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x454910;
	virtual auto reverse() = mac 0x454930;
}

class cocos2d::CCFlipY : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x454ab0;
	virtual auto reverse() = mac 0x454ad0;
}

class cocos2d::CCImage : cocos2d::CCObject {
}

class cocos2d::CCLayer : cocos2d::CCNode, cocos2d::CCTouchDelegate, cocos2d::CCAccelerometerDelegate, cocos2d::CCKeypadDelegate, cocos2d::CCKeyboardDelegate, cocos2d::CCMouseDelegate {
	virtual auto init() = mac 0x2729a0, ios 0xc7894;
	virtual auto onEnter() = mac 0x273300, ios 0xc7f38;
	virtual auto onEnterTransitionDidFinish() = mac 0x273490, ios 0xc80cc;
	virtual auto onExit() = mac 0x2733c0, ios 0xc8004;
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2734d0, ios 0xc810c;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x273550, ios 0xc816c;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2735d0, ios 0xc820c;
	virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x273650, ios 0xc82ac;
	virtual auto ccTouchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x2736d0, ios 0xc834c;
	virtual auto ccTouchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x273750, ios 0xc83ec;
	virtual auto ccTouchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x2737d0, ios 0xc848c;
	virtual auto ccTouchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x273850, ios 0xc852c;
	virtual auto didAccelerate(cocos2d::CCAcceleration*) = mac 0x272ea0, ios 0xc7c5c;
	virtual auto registerWithTouchDispatcher() = mac 0x272b40, ios 0xc7958;
	virtual auto registerScriptTouchHandler(int, bool, int, bool) = mac 0x272bd0, ios 0xc79c8;
	virtual auto unregisterScriptTouchHandler() = mac 0x272c30, ios 0xc7a28;
	virtual auto isTouchEnabled() = mac 0x272ce0, ios 0xc7a54;
	virtual auto setTouchEnabled(bool) = mac 0x272cf0, ios 0xc7a5c;
	virtual auto setTouchMode(cocos2d::ccTouchesMode) = mac 0x272d60, ios 0xc7acc;
	virtual auto getTouchMode() = mac 0x272e10, ios 0xc7b9c;
	virtual auto setTouchPriority(int) = mac 0x272db0, ios 0xc7b30;
	virtual auto getTouchPriority() = mac 0x272e00, ios 0xc7b94;
	virtual auto isAccelerometerEnabled() = mac 0x272e20, ios 0xc7ba4;
	virtual auto setAccelerometerEnabled(bool) = mac 0x272e30, ios 0xc7bac;
	virtual auto setAccelerometerInterval(double) = mac 0x272e70, ios 0xc7c10;
	virtual auto isKeypadEnabled() = mac 0x272f70, ios 0xc7cf4;
	virtual auto setKeypadEnabled(bool) = mac 0x272f80, ios 0xc7cfc;
	virtual auto isKeyboardEnabled() = mac 0x273010, ios 0xc7d70;
	virtual auto setKeyboardEnabled(bool) = mac 0x273020, ios 0xc7d78;
	virtual auto isMouseEnabled() = mac 0x273090, ios 0xc7de0;
	virtual auto setMouseEnabled(bool) = mac 0x2730a0, ios 0xc7de8;
	virtual auto keyBackClicked() = mac 0x273160, ios 0xc7e50;
	virtual auto keyMenuClicked() = mac 0x273200, ios 0xc7eac;
	virtual auto keyDown(cocos2d::enumKeyCodes) = mac 0x273280, ios 0xc7efc;
}

class cocos2d::CCPlace : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x454d70;
}

class cocos2d::CCScene : cocos2d::CCNode {
	virtual auto init() = mac 0x13c0f0, ios 0x163024;
}

class cocos2d::CCSpawn : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f46c0, ios 0xf1258;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f4650, ios 0xf11c4;
	virtual auto stop() = mac 0x1f4690, ios 0xf1218;
	virtual auto reverse() = mac 0x1f4710, ios 0xf12bc;
}

class cocos2d::CCSpeed : cocos2d::CCAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto isDone() = mac 0x35bc50;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x35bbe0;
	virtual auto stop() = mac 0x35bc00;
	virtual auto step(float) = mac 0x35bc30;
	virtual auto reverse() = mac 0x35bc60;
}

class cocos2d::CCTimer : cocos2d::CCObject {
	virtual auto update(float) = mac 0x242440, ios 0x10c048;
}

class cocos2d::CCTouch : cocos2d::CCObject {
}

class cocos2d::CCTwirl : cocos2d::CCGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x129850;
}

class cocos2d::CCWaves : cocos2d::CCGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x129340;
}

class cocos2d::ZipFile {
}

class cocos2d::CCAction : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x35b890, ios 0x19d964;
	virtual auto setTag(int);
	virtual auto isDone() = mac 0x35b870, ios 0x19d958;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x35b850, ios 0x19d948;
	virtual auto stop() = mac 0x35b860, ios 0x19d950;
	virtual auto step(float) = mac 0x35b880, ios 0x19d960;
}

class cocos2d::CCCamera : cocos2d::CCObject {
}

class cocos2d::CCEaseIn : cocos2d::CCEaseRateAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a1b10, ios 0x1a6740;
	virtual auto reverse() = mac 0x2a1b40, ios 0x1a6780;
}

class cocos2d::CCFadeIn : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f7c80, ios 0xf2e34;
	virtual auto reverse() = mac 0x1f7ce0, ios 0xf2e98;
}

class cocos2d::CCFadeTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f8220, ios 0xf3274;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f81c0, ios 0xf3210;
}

class cocos2d::CCFollow : cocos2d::CCAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto isDone() = mac 0x35c2e0;
	virtual auto stop() = mac 0x35c300;
	virtual auto step(float) = mac 0x35c1f0;
}

class cocos2d::CCGrid3D : cocos2d::CCGridBase {
	virtual auto blit() = mac 0x2884b0;
	virtual auto reuse() = mac 0x288da0;
	virtual auto calculateVertexPoints() = mac 0x288550;
}

class cocos2d::CCJumpBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f6160;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f6110;
	virtual auto reverse() = mac 0x1f6290;
}

class cocos2d::CCJumpTo : cocos2d::CCJumpBy {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f6570;
}

class cocos2d::CCLens3D : cocos2d::CCGrid3DAction {
	virtual auto update(float) = mac 0x128140;
}

class cocos2d::CCLiquid : cocos2d::CCGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x128ed0;
}

class cocos2d::CCMoveBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f5400, ios 0xf1c24;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f5360, ios 0xf1b8c;
	virtual auto reverse() = mac 0x1f53b0, ios 0xf1be0;
}

class cocos2d::CCMoveTo : cocos2d::CCMoveBy {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f5740, ios 0xf1ef8;
}

class cocos2d::CCObject : cocos2d::CCCopying {
	virtual auto isEqual(cocos2d::CCObject const*) = mac 0x250f20, ios 0x439e4;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x250f30, ios 0x439f0;
	virtual auto update(float);
	virtual auto encodeWithCoder(DS_Dictionary*) = mac 0x250f70, ios 0x43a18;
	virtual auto canEncode() = mac 0x250f90, ios 0x43a24;
	virtual auto getTag() const = mac 0x250f50, ios 0x43a08;
	virtual auto setTag(int) = mac 0x250f60, ios 0x43a10;
}

class cocos2d::CCRepeat : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f36b0;
	virtual auto isDone() = mac 0x1f37e0;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f3630;
	virtual auto stop() = mac 0x1f3680;
	virtual auto reverse() = mac 0x1f37f0;
}

class cocos2d::CCSkewBy : cocos2d::CCSkewTo {
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f5d00;
	virtual auto reverse() = mac 0x1f5d40;
	virtual auto initWithDuration(float, float, float) = mac 0x1f5cb0;
}

class cocos2d::CCSkewTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f5b20;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f59f0;
	virtual auto initWithDuration(float, float, float) = mac 0x1f58d0;
}

class cocos2d::CCSprite : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	virtual auto init() = mac 0x132ef0, ios 0x15b488;
	virtual auto setVertexZ(float) = mac 0x134a80, ios 0x15ccc8;
	virtual auto setScaleX(float) = mac 0x134900, ios 0x15cb3c;
	virtual auto setScaleY(float) = mac 0x134980, ios 0x15cbbc;
	virtual auto setScale(float) = mac 0x134a00, ios 0x15cc3c;
	virtual auto setPosition(cocos2d::CCPoint const&) = mac 0x134650, ios 0x15c878;
	virtual auto setSkewX(float) = mac 0x134820, ios 0x15ca5c;
	virtual auto setSkewY(float) = mac 0x134890, ios 0x15cacc;
	virtual auto setAnchorPoint(cocos2d::CCPoint const&) = mac 0x134af0, ios 0x15cd38;
	virtual auto setVisible(bool) = mac 0x134b70, ios 0x15cdac;
	virtual auto setRotation(float) = mac 0x1346d0, ios 0x15c90c;
	virtual auto setRotationX(float) = mac 0x134740, ios 0x15c97c;
	virtual auto setRotationY(float) = mac 0x1347b0, ios 0x15c9ec;
	virtual auto ignoreAnchorPointForPosition(bool) = mac 0x134b60, ios 0x15cda8;
	virtual auto addChild(cocos2d::CCNode*) = mac 0x134190, ios 0x15c34c;
	virtual auto addChild(cocos2d::CCNode*, int) = mac 0x1341a0, ios 0x15c350;
	virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x1341b0, ios 0x15c354;
	virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0x134300, ios 0x15c4cc;
	virtual auto removeAllChildrenWithCleanup(bool) = mac 0x134340, ios 0x15c514;
	virtual auto reorderChild(cocos2d::CCNode*, int) = mac 0x134270, ios 0x15c438;
	virtual auto sortAllChildren() = mac 0x1343f0, ios 0x15c5cc;
	virtual auto draw() = mac 0x134070, ios 0x15c240;
	virtual auto updateTransform() = mac 0x133b70, ios 0x15be0c;
	virtual auto setOpacity(unsigned char) = mac 0x134da0, ios 0x15cf48;
	virtual auto updateDisplayedOpacity(unsigned char) = mac 0x1354c0, ios 0x15d228;
	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x134ff0, ios 0x15d088;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const&) = mac 0x135370, ios 0x15d1dc;
	virtual auto setOpacityModifyRGB(bool) = mac 0x135200, ios 0x15d198;
	virtual auto isOpacityModifyRGB() = mac 0x135350, ios 0x15d1cc;
	virtual auto initWithTexture(cocos2d::CCTexture2D*) = mac 0x133110, ios 0x15b608;
	virtual auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&) = mac 0x1330f0, ios 0x15b5f8;
	virtual auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool) = mac 0x132f10, ios 0x15b4a0;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame*) = mac 0x133270, ios 0x15b78c;
	virtual auto initWithSpriteFrameName(char const*) = mac 0x1332c0, ios 0x15b7f0;
	virtual auto initWithFile(char const*) = mac 0x133180, ios 0x15b684;
	virtual auto initWithFile(char const*, cocos2d::CCRect const&) = mac 0x133210, ios 0x15b720;
	virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0x135a90, ios 0x15d610;
	virtual auto getTexture() = mac 0x135c00, ios 0x15d748;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc);
	virtual auto getBlendFunc();
	virtual auto setChildColor(cocos2d::ccColor3B const&) = mac 0x135160, ios 0x15d0f4;
	virtual auto setChildOpacity(unsigned char) = mac 0x134f50, ios 0x15cfe4;
	virtual auto getBatchNode() = mac 0x135910, ios 0x15d4e0;
	virtual auto setBatchNode(cocos2d::CCSpriteBatchNode*) = mac 0x135920, ios 0x15d4e8;
	virtual auto refreshTextureRect() = mac 0x133520, ios 0x15b964;
	virtual auto setTextureRect(cocos2d::CCRect const&) = mac 0x133560, ios 0x15b9a8;
	virtual auto setTextureRect(cocos2d::CCRect const&, bool, cocos2d::CCSize const&) = mac 0x133580, ios 0x15b9bc;
	virtual auto setVertexRect(cocos2d::CCRect const&) = mac 0x1338f0, ios 0x15bc5c;
	virtual auto setDisplayFrame(cocos2d::CCSpriteFrame*) = mac 0x135610, ios 0x15d274;
	virtual auto isFrameDisplayed(cocos2d::CCSpriteFrame*) = mac 0x1356d0, ios 0x15d344;
	virtual auto displayFrame() = mac 0x135760, ios 0x15d3d8;
	virtual auto setDisplayFrameWithAnimationName(char const*, int) = mac 0x135680, ios 0x15d2f0;
	virtual auto isDirty();
	virtual auto setDirty(bool);
	virtual auto setTextureCoords(cocos2d::CCRect const&) = mac 0x133910, ios 0x15bc64;
	virtual auto updateBlendFunc() = mac 0x135a20, ios 0x15d5a0;
	virtual auto setReorderChildDirtyRecursively() = mac 0x134550, ios 0x15c764;
	virtual auto setDirtyRecursively(bool) = mac 0x1345b0, ios 0x15c7cc;
}

class cocos2d::CCString : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto isEqual(cocos2d::CCObject const*) = mac 0x44c8f0, ios 0x1a1e6c;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x44ccb0, ios 0x1a2168;
}

class cocos2d::CCTintBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f8870;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f8800;
	virtual auto reverse() = mac 0x1f8950;
}

class cocos2d::CCTintTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f8510, ios 0xf3518;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f84b0, ios 0xf34ac;
}

class cocos2d::TypeInfo {
}

class cocos2d::CCAnimate : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f9610, ios 0xf3c04;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f9570, ios 0xf3b58;
	virtual auto stop() = mac 0x1f95d0, ios 0xf3bc0;
	virtual auto reverse() = mac 0x1f9840, ios 0xf3e04;
}

class cocos2d::CCEGLView : cocos2d::CCEGLViewProtocol {
	virtual auto end() = mac 0x2954a0, ios 0xbad14;
	virtual auto isOpenGLReady() = mac 0x295470, ios 0xbac80;
	virtual auto swapBuffers() = mac 0x295510, ios 0xbad54;
	virtual auto setIMEKeyboardState(bool) = mac 0x295550, ios 0xbad80;
	virtual auto setViewPortInPoints(float, float, float, float) = mac 0x295490, ios 0x20efb4;
	virtual auto setScissorInPoints(float, float, float, float) = mac 0x295730, ios 0x20efe8;
}

class cocos2d::CCEaseOut : cocos2d::CCEaseRateAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x2a1d20, ios 0x1a6934;
	virtual auto reverse() = mac 0x2a1d50, ios 0x1a697c;
}

class cocos2d::CCFadeOut : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f7ee0, ios 0xf3000;
	virtual auto reverse() = mac 0x1f7f50, ios 0xf306c;
}

class cocos2d::CCInteger : cocos2d::CCObject {
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&);
}

class cocos2d::CCScaleBy : cocos2d::CCScaleTo {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f7640;
	virtual auto reverse() = mac 0x1f76b0;
}

class cocos2d::CCScaleTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) { return nullptr; }
	virtual auto update(float) = mac 0x1f7360, ios 0xf29c0;
	virtual auto startWithTarget(cocos2d::CCNode*) = mac 0x1f72f0, ios 0xf2954;
}

class cocos2d::CCShaky3D : cocos2d::CCGrid3DAction {
	virtual auto update(float) = mac 0x128ae0;
}

class cocos2d::CCWaves3D : cocos2d::CCGrid3DAction {
	virtual auto update(float) = mac 0x127170;
}

class cocos2d::extension::CCTableView : cocos2d::extension::CCScrollView, cocos2d::extension::CCScrollViewDelegate {
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x234bf0;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x234da0;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x234ae0;
	virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x234e50;
	virtual auto scrollViewDidScroll(cocos2d::extension::CCScrollView*) = mac 0x234830;
	virtual auto scrollViewDidZoom(cocos2d::extension::CCScrollView*);
}

class cocos2d::extension::CCHttpClient : cocos2d::CCObject {
}

class cocos2d::extension::CCInvocation : cocos2d::CCObject {
}

class cocos2d::extension::CCScrollView : cocos2d::CCLayer {
	virtual auto init() = mac 0x214fb0;
	virtual auto setContentSize(cocos2d::CCSize const&) = mac 0x215eb0;
	virtual auto getContentSize() const = mac 0x215e90;
	virtual auto addChild(cocos2d::CCNode*) = mac 0x216160;
	virtual auto addChild(cocos2d::CCNode*, int) = mac 0x216120;
	virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x216080;
	virtual auto visit() = mac 0x2164a0;
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x216620;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x216890;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x216c70;
	virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x216d30;
	virtual auto registerWithTouchDispatcher() = mac 0x214ff0;
	virtual auto setTouchEnabled(bool) = mac 0x215250;
	virtual auto setDirection(cocos2d::extension::CCScrollViewDirection);
}

class cocos2d::extension::CCEditBoxImpl {
}

class cocos2d::extension::CCHttpRequest : cocos2d::CCObject {
}

class cocos2d::extension::CCHttpResponse : cocos2d::CCObject {
}

class cocos2d::extension::CCScale9Sprite : cocos2d::CCNodeRGBA {
	virtual auto init() = mac 0x2115d0, ios 0x13d4f4;
	virtual auto setContentSize(cocos2d::CCSize const&) = mac 0x2127c0, ios 0x13e400;
	virtual auto visit() = mac 0x213bf0, ios 0x13f144;
	virtual auto getOpacity() = mac 0x213f30, ios 0x13f30c;
	virtual auto setOpacity(unsigned char) = mac 0x213dd0, ios 0x13f250;
	virtual auto updateDisplayedOpacity(unsigned char) = mac 0x2139b0, ios 0x13ef98;
	virtual auto getColor() = mac 0x213db0, ios 0x13f240;
	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x213c20, ios 0x13f178;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const&) = mac 0x213a20, ios 0x13f004;
	virtual auto setOpacityModifyRGB(bool) = mac 0x213830, ios 0x13eed0;
	virtual auto isOpacityModifyRGB() = mac 0x213990, ios 0x13ef88;
	virtual auto getPreferredSize() = mac 0x213620, ios 0x13ed18;
	virtual auto setPreferredSize(cocos2d::CCSize) = mac 0x2135f0, ios 0x13ece4;
	virtual auto getCapInsets() = mac 0x2136e0, ios 0x13edc4;
	virtual auto setCapInsets(cocos2d::CCRect) = mac 0x213640, ios 0x13ed24;
	virtual auto getInsetLeft() = mac 0x213b30, ios 0x13f104;
	virtual auto setInsetLeft(float) = mac 0x213b70, ios 0x13f124;
	virtual auto getInsetTop() = mac 0x213b40, ios 0x13f10c;
	virtual auto setInsetTop(float) = mac 0x213b90, ios 0x13f12c;
	virtual auto getInsetRight() = mac 0x213b50, ios 0x13f114;
	virtual auto setInsetRight(float) = mac 0x213bb0, ios 0x13f134;
	virtual auto getInsetBottom() = mac 0x213b60, ios 0x13f11c;
	virtual auto setInsetBottom(float) = mac 0x213bd0, ios 0x13f13c;
	virtual auto initWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, bool, cocos2d::CCRect) = mac 0x211690, ios 0x13d5c8;
	virtual auto initWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, cocos2d::CCRect) = mac 0x211630, ios 0x13d55c;
	virtual auto initWithFile(char const*, cocos2d::CCRect, cocos2d::CCRect) = mac 0x212d50, ios 0x13e8e0;
	virtual auto initWithFile(char const*, cocos2d::CCRect) = mac 0x212e80, ios 0x13e964;
	virtual auto initWithFile(cocos2d::CCRect, char const*) = mac 0x212f80, ios 0x13ea6c;
	virtual auto initWithFile(char const*) = mac 0x213080, ios 0x13ead8;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame*, cocos2d::CCRect) = mac 0x213140, ios 0x13eb30;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame*) = mac 0x213250, ios 0x13ebbc;
	virtual auto initWithSpriteFrameName(char const*, cocos2d::CCRect) = mac 0x213310, ios 0x13ec14;
	virtual auto initWithSpriteFrameName(char const*) = mac 0x213410, ios 0x13ec8c;
	virtual auto updateWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, bool, cocos2d::CCRect) = mac 0x211730, ios 0x13d674;
	virtual auto setSpriteFrame(cocos2d::CCSpriteFrame*) = mac 0x213a90, ios 0x13f070;
}

class cocos2d::extension::CCControlButton : cocos2d::extension::CCControl {
	virtual auto init() = mac 0x1a8420;
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1a9c20;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1a9db0;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1a9ef0;
	virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1aa1a0;
	virtual auto getOpacity() = mac 0x1aa090;
	virtual auto setOpacity(unsigned char) = mac 0x1a9fc0;
	virtual auto getColor() = mac 0x1aa180;
	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x1aa0b0;
	virtual auto setEnabled(bool) = mac 0x1a8b00;
	virtual auto setSelected(bool) = mac 0x1a8b30;
	virtual auto setHighlighted(bool) = mac 0x1a8b60;
	virtual auto needsLayout() = mac 0x1a9700;
	virtual auto getPreferredSize() = mac 0x1a8d20;
	virtual auto setPreferredSize(cocos2d::CCSize) = mac 0x1a8c60;
	virtual auto getZoomOnTouchDown() = mac 0x1a8c50;
	virtual auto setZoomOnTouchDown(bool) = mac 0x1a8c40;
	virtual auto getLabelAnchorPoint() = mac 0x1a8d50;
	virtual auto setLabelAnchorPoint(cocos2d::CCPoint) = mac 0x1a8d70;
	virtual auto setMargins(int, int) = mac 0x1a8ae0;
	virtual auto initWithLabelAndBackgroundSprite(cocos2d::CCNode*, cocos2d::extension::CCScale9Sprite*) = mac 0x1a8480;
	virtual auto initWithTitleAndFontNameAndFontSize(gd::string, char const*, float) = mac 0x1a88f0;
	virtual auto initWithBackgroundSprite(cocos2d::extension::CCScale9Sprite*) = mac 0x1a8a30;
	virtual auto getTitleForState(unsigned int) = mac 0x1a8db0;
	virtual auto setTitleForState(cocos2d::CCString*, unsigned int) = mac 0x1a8e90;
	virtual auto getTitleColorForState(unsigned int) = mac 0x1a8f00;
	virtual auto setTitleColorForState(cocos2d::ccColor3B, unsigned int) = mac 0x1a8f60;
	virtual auto getTitleLabelForState(unsigned int) = mac 0x1a9020;
	virtual auto setTitleLabelForState(cocos2d::CCNode*, unsigned int) = mac 0x1a9060;
	virtual auto setTitleTTFForState(char const*, unsigned int) = mac 0x1a9140;
	virtual auto getTitleTTFForState(unsigned int) = mac 0x1a9240;
	virtual auto setTitleTTFSizeForState(float, unsigned int) = mac 0x1a92b0;
	virtual auto getTitleTTFSizeForState(unsigned int) = mac 0x1a9330;
	virtual auto setTitleBMFontForState(char const*, unsigned int) = mac 0x1a93a0;
	virtual auto getTitleBMFontForState(unsigned int) = mac 0x1a9490;
	virtual auto getBackgroundSpriteForState(unsigned int) = mac 0x1a9500;
	virtual auto setBackgroundSpriteForState(cocos2d::extension::CCScale9Sprite*, unsigned int) = mac 0x1a9540;
	virtual auto setBackgroundSpriteFrameForState(cocos2d::CCSpriteFrame*, unsigned int) = mac 0x1a96d0;
}

class cocos2d::extension::CCControlSlider : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1de890;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1deaf0;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1decc0;
	virtual auto setEnabled(bool) = mac 0x1de610;
	virtual auto needsLayout() = mac 0x1deea0;
	virtual auto isTouchInside(cocos2d::CCTouch*) = mac 0x1de750;
	virtual auto setValue(float) = mac 0x1de660;
	virtual auto setMinimumValue(float) = mac 0x1de6b0;
	virtual auto setMaximumValue(float) = mac 0x1de700;
	virtual auto initWithSprites(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*) = mac 0x1de320;
}

class cocos2d::extension::CCControlSwitch : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e5bc0;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e5cb0;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e5d80;
	virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e5ef0;
	virtual auto setEnabled(bool) = mac 0x1e5b30;
}

class cocos2d::extension::CCTableViewCell : cocos2d::CCNode, cocos2d::extension::CCSortableObject {
	virtual auto setObjectID(unsigned int);
	virtual auto getObjectID();
}

class cocos2d::extension::CCControlStepper : cocos2d::extension::CCControl {
	virtual auto update(float) = mac 0x1e12a0;
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e1470;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e1620;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1e1750;
	virtual auto setWraps(bool) = mac 0x1e0fe0;
	virtual auto setMinimumValue(double) = mac 0x1e1060;
	virtual auto setMaximumValue(double) = mac 0x1e1090;
	virtual auto setValue(double) = mac 0x1e10c0;
	virtual auto getValue() = mac 0x1e10e0;
	virtual auto setStepValue(double) = mac 0x1e10f0;
	virtual auto setValueWithSendingEvent(double, bool) = mac 0x1e1110;
	virtual auto isContinuous() = mac 0x1e1100;
}

class cocos2d::extension::CCControlHuePicker : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1ac050, ios 0x1147a8;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1ac150, ios 0x114830;
	virtual auto registerWithTouchDispatcher() = mac 0x1abcd0, ios 0x1144c4;
	virtual auto setEnabled(bool) = mac 0x1abe70, ios 0x114624;
	virtual auto setHue(float) = mac 0x1abd40, ios 0x114534;
	virtual auto setHuePercentage(float) = mac 0x1abd70, ios 0x114550;
	virtual auto initWithTargetAndPos(cocos2d::CCNode*, cocos2d::CCPoint) = mac 0x1abb30, ios 0x1142ec;
}

class cocos2d::extension::ColorPickerDelegate {
	virtual auto colorValueChanged(cocos2d::ccColor3B);
}

class cocos2d::extension::CCScrollViewDelegate {
}

class cocos2d::extension::CCControlColourPicker : cocos2d::extension::CCControl {
	virtual auto init();
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	virtual auto setColorValue(cocos2d::ccColor3B const&);
}

class cocos2d::extension::CCControlSwitchSprite : cocos2d::CCSprite, cocos2d::CCActionTweenDelegate {
	virtual auto draw() = mac 0x1e4f90;
	virtual auto updateTweenAction(float, char const*) = mac 0x1e4ed0;
}

class cocos2d::extension::CCControlPotentiometer : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1ade70;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1ae030;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1ae2a0;
	virtual auto setEnabled(bool) = mac 0x1adb80;
	virtual auto isTouchInside(cocos2d::CCTouch*) = mac 0x1add50;
}

class cocos2d::extension::CCArrayForObjectSorting : cocos2d::CCArray {
}

class cocos2d::extension::CCControlSaturationBrightnessPicker : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1b4030, ios 0x11fb14;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1b4130, ios 0x11fb9c;
	virtual auto registerWithTouchDispatcher() = mac 0x1b3fc0, ios 0x11faa4;
	virtual auto setEnabled(bool) = mac 0x1b3b00, ios 0x11f6bc;
	virtual auto initWithTargetAndPos(cocos2d::CCNode*, cocos2d::CCPoint) = mac 0x1b3820, ios 0x11f4b8;
	virtual auto updateWithHSV(cocos2d::extension::HSV) = mac 0x1b3b50, ios 0x11f70c;
	virtual auto updateDraggerWithHSV(cocos2d::extension::HSV) = mac 0x1b3bf0, ios 0x11f77c;
}

class cocos2d::extension::CCControl : cocos2d::CCLayerRGBA {
	virtual auto init() = mac 0x1a71c0, ios 0x10644c;
	virtual auto onEnter() = mac 0x1a7470, ios 0x106648;
	virtual auto onExit() = mac 0x1a7480, ios 0x10664c;
	virtual auto registerWithTouchDispatcher() = mac 0x1a7420, ios 0x1065ec;
	virtual auto setOpacityModifyRGB(bool) = mac 0x1a7c10, ios 0x106aac;
	virtual auto isOpacityModifyRGB() = mac 0x1a7d70, ios 0x106b5c;
	virtual auto setEnabled(bool) = mac 0x1a7e60, ios 0x106c2c;
	virtual auto isEnabled() = mac 0x1a7e90, ios 0x106c4c;
	virtual auto setSelected(bool) = mac 0x1a7ea0, ios 0x106c54;
	virtual auto isSelected() = mac 0x1a7ec0, ios 0x106c64;
	virtual auto setHighlighted(bool) = mac 0x1a7ed0, ios 0x106c6c;
	virtual auto isHighlighted() = mac 0x1a7ef0, ios 0x106c7c;
	virtual auto needsLayout() = mac 0x1a7e50, ios 0x106c28;
	virtual auto sendActionsForControlEvents(unsigned int);
	virtual auto addTargetWithActionForControlEvents(cocos2d::CCObject*, cocos2d::extension::SEL_CCControlHandler, unsigned int);
	virtual auto removeTargetWithActionForControlEvents(cocos2d::CCObject*, cocos2d::extension::SEL_CCControlHandler, unsigned int);
	virtual auto getTouchLocation(cocos2d::CCTouch*) = mac 0x1a7d90, ios 0x106b6c;
	virtual auto isTouchInside(cocos2d::CCTouch*) = mac 0x1a7de0, ios 0x106bb8;
}

class cocos2d::extension::CCEditBox : cocos2d::extension::CCControlButton, cocos2d::CCIMEDelegate {
	virtual auto setPosition(cocos2d::CCPoint const&) = mac 0x26d850;
	virtual auto setAnchorPoint(cocos2d::CCPoint const&) = mac 0x26d910;
	virtual auto setContentSize(cocos2d::CCSize const&) = mac 0x26d8d0;
	virtual auto setVisible(bool) = mac 0x26d890;
	virtual auto onEnter() = mac 0x26d980;
	virtual auto onExit() = mac 0x26d9b0;
	virtual auto visit() = mac 0x26d950;
	virtual auto keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x26d9e0;
	virtual auto keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x26daf0;
	virtual auto keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x26db10;
	virtual auto keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x26db80;
}
