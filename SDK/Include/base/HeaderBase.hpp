#pragma once 

#ifdef _MSC_VER
#pragma warning(disable: 4251) // dll-interface
#endif

#include <Base.hpp>
#include <MacroBase.hpp>
#include <PlatformBase.hpp>

#include <cocos2d.h>
#include <fmod.hpp>
#include <cocos-ext.h>
#include <unordered_map>
// #include <Geode.hpp>

inline std::unordered_map<void*, bool> destructorLock;

#define GM (GameManager::sharedState())
#define LEL (GM->_editorLayer())
#define PL (GM->_playLayer())
#define GJBL (LEL ? static_cast<GJBaseGameLayer*>(LEL) : static_cast<GJBaseGameLayer*>(PL))
#define WINSIZE (CCDirector::sharedDirector()->getWinSize())

struct GameModes {
	bool cube;
	bool ship;
	bool ufo;
	bool ball;
	bool wave;
	bool robot;
	bool spider;
};

struct LevelDifficulty {
	int32_t denominator;
	int32_t numerator;  
};

#define CLASSPARAM(type, getter, offset)        		\
	inline type& _##getter() {                      	\
		return *((type*)((uintptr_t)this + offset));	\
	}

#define STRUCTPARAM(type, getter, offset)       		\
	inline type _##getter() {                       	\
		return *(type*)((uintptr_t)this+offset);		\
	}

//thanks pie
enum SearchType {
    kGJSearchTypeSearch = 0,
    kGJSearchTypeDownloaded = 1,
    kGJSearchTypeMostLiked = 2,
    kGJSearchTypeTrending = 3,
    kGJSearchTypeRecent = 4,
    kGJSearchTypeUsersLevels = 5,
    kGJSearchTypeFeatured = 6,
    kGJSearchTypeMagic = 7,
    kGJSearchTypeSends = 8,
    kGJSearchTypeMapPack = 9,
    kGJSearchTypeMapPackOnClick = 10,
    kGJSearchTypeAwarded = 11,
    kGJSearchTypeFollowed = 12,
    kGJSearchTypeFriends = 13,
    kGJSearchTypeUsers = 14,
    kGJSearchTypeLikedGDW = 15,
    kGJSearchTypeHallOfFame = 16,
    kGJSearchTypeFeaturedGDW = 17,
    kGJSearchTypeSimilar = 18,
    kGJSearchTypeMyLevels = 98,
    kGJSearchTypeSavedLevels = 99,
    kGJSearchTypeFavouriteLevels = 100
};

// jesus fucking christ (painfully written by @hjfod)
enum GameObjectType {
	kGameObjectTypeSolid = 0,
	kGameObjectTypeHazard = 2,
	kGameObjectTypeInverseGravityPortal = 3,
	kGameObjectTypeNormalGravityPortal = 4,
	kGameObjectTypeShipPortal = 5,
	kGameObjectTypeCubePortal = 6,
	kGameObjectTypeDecoration = 7,
	kGameObjectTypeYellowJumpPad = 8,
	kGameObjectTypePinkJumpPad = 9,
	kGameObjectTypeGravityPad = 10,
	kGameObjectTypeYellowJumpRing = 11,
	kGameObjectTypePinkJumpRing = 12,
	kGameObjectTypeGravityRing = 13,
	kGameObjectTypeInverseMirrorPortal = 14,
	kGameObjectTypeNormalMirrorPortal = 15,
	kGameObjectTypeBallPortal = 16,
	kGameObjectTypeRegularSizePortal = 17,
	kGameObjectTypeMiniSizePortal = 18,
	kGameObjectTypeUfoPortal = 19,
	kGameObjectTypeModifier = 20,
	// kGameObjectTypeTest = 21,
	kGameObjectTypeSecretCoin = 22,
	kGameObjectTypeDualPortal = 23,
	kGameObjectTypeSoloPortal = 24,
	kGameObjectTypeSlope = 25,
	kGameObjectTypeWavePortal = 26,
	kGameObjectTypeRobotPortal = 27,
	kGameObjectTypeTeleportPortal = 28,
	kGameObjectTypeGreenRing = 29,
	kGameObjectTypeCollectible = 30,
	kGameObjectTypeUserCoin = 31,
	kGameObjectTypeDropRing = 32,
	kGameObjectTypeSpiderPortal = 33,
	kGameObjectTypeRedJumpPad = 34,
	kGameObjectTypeRedJumpRing = 35,
	kGameObjectTypeCustomRing = 36,
	kGameObjectTypeDashRing = 37,
	kGameObjectTypeGravityDashRing = 38,
	kGameObjectTypeCollisionObject = 39,
	kGameObjectTypeSpecial = 40,
};

enum PulseEffectType {};
enum TouchTriggerType {};
enum PlayerButton {};
enum GhostType {};
enum TableViewCellEditingStyle {};
enum UserListType {};
enum GJErrorCode {};
enum AccountError {};
enum GJSongError {};
enum LikeItemType {};
enum GJStoreItem {};
enum CommentError {};
enum BackupAccountError {};

enum BoomListType {
	kBoomListTypeDefault 		= 0x0,
	kBoomListTypeUser 			= 0x2,
	kBoomListTypeStats 			= 0x3,
	kBoomListTypeAchievement    = 0x4,
	kBoomListTypeLevel 		    = 0x5,
	kBoomListTypeLevel2 	    = 0x6,
	kBoomListTypeComment 	    = 0x7,
	kBoomListTypeComment2 	    = 0x8,
	kBoomListTypeSong 		    = 0xb,
	kBoomListTypeScore 		    = 0xc,
	kBoomListTypeMapPack 	    = 0xd,
	kBoomListTypeCustomSong     = 0xe,
	kBoomListTypeComment3 	    = 0xf,
	kBoomListTypeUser2 		    = 0x10,
	kBoomListTypeRequest 	    = 0x11,
	kBoomListTypeMessage 	    = 0x12,
	kBoomListTypeLevelScore     = 0x13,
	kBoomListTypeArtist		    = 0x14,
};

enum MenuAnimationType {
	kMenuAnimationTypeScale	= 0,
	kMenuAnimationTypeMove 	= 1,
};

enum ShopType {
	kShopTypeNormal,
	kShopTypeSecret,
	kShopTypeCommunity
};

// Geode Addition
enum ZLayer {
	kZLayerB4 = -3,
	kZLayerB3 = -1,
	kZLayerB2 = 1,
	kZLayerB1 = 3,
	kZLayerDefault = 0,
	kZLayerT1 = 5,
	kZLayerT2 = 7,
	kZLayerT3 = 9,
};

enum UpdateResponse {
    kUpdateResponseUnknown,
    kUpdateResponseUpToDate,
    kUpdateResponseGameVerOutOfDate,
    kUpdateResponseUpdateSuccess,
};

enum UnlockType {
	kItemTypeCube = 0x1,
	kItemTypeCol1 = 0x2,
	kItemTypeCol2 = 0x3,
	kItemTypeShip = 0x4,
	kItemTypeBall = 0x5,
	kItemTypeBird = 0x6,
	kItemTypeDart = 0x7,
	kItemTypeRobot = 0x8,
	kItemTypeSpider = 0x9,
	kItemTypeStreak = 0xA,
	kItemTypeDeath = 0xB,
	kItemTypeGJItem = 0xC,
};

enum SpecialRewardItem {
	kSpecialRewardItemFireShard = 0x1,
	kSpecialRewardItemIceShard = 0x2,
	kSpecialRewardItemPoisonShard = 0x3,
	kSpecialRewardItemShadowShard = 0x4,
	kSpecialRewardItemLavaShard = 0x5,
	kSpecialRewardItemBonusKey = 0x6,
	kSpecialRewardItemOrbs = 0x7,
	kSpecialRewardItemDiamonds = 0x8,
	kSpecialRewardItemCustomItem = 0x9,
};  

enum EditCommand {
    kEditCommandSmallLeft   = 1,
    kEditCommandSmallRight  = 2,
    kEditCommandSmallUp     = 3,
    kEditCommandSmallDown   = 4,

    kEditCommandLeft        = 5,
    kEditCommandRight       = 6,
    kEditCommandUp          = 7,
    kEditCommandDown        = 8,

    kEditCommandBigLeft     = 9,
    kEditCommandBigRight    = 10,
    kEditCommandBigUp       = 11,
    kEditCommandBigDown     = 12,

    kEditCommandTinyLeft    = 13,
    kEditCommandTinyRight   = 14,
    kEditCommandTinyUp      = 15,
    kEditCommandTinyDown    = 16,

    kEditCommandFlipX       = 17,
    kEditCommandFlipY       = 18,
    kEditCommandRotateCW    = 19,
    kEditCommandRotateCCW   = 20,
    kEditCommandRotateCW45  = 21,
    kEditCommandRotateCCW45 = 22,
    kEditCommandRotateFree  = 23,
    kEditCommandRotateSnap  = 24,
    
    kEditCommandScale       = 25,
};

// Geode Addition
enum PlaybackMode {
	kPlaybackModeNot        = 0,
	kPlaybackModePlaying    = 1,
	kPlaybackModePaused     = 2,
};

enum SelectArtType {
    kSelectArtTypeBackground    = 0,
    kSelectArtTypeGround        = 1,
};

enum UndoCommand {
    kUndoCommandDelete = 1,
    kUndoCommandNew = 2,
    kUndoCommandPaste = 3,
    kUndoCommandDeleteMulti = 4,
    kUndoCommandTransform = 5,
    kUndoCommandSelect = 6,
};

enum EasingType {
    kEasingTypeNone = 0,
    kEasingTypeEaseInOut = 1,
    kEasingTypeEaseIn = 2,
    kEasingTypeEaseOut = 3,
    kEasingTypeElasticInOut = 4,
    kEasingTypeElasticIn = 5,
    kEasingTypeElasticOut = 6,
    kEasingTypeBounceInOut = 7,
    kEasingTypeBounceIn = 8,
    kEasingTypeBounceOut = 9,
    kEasingTypeExponentialInOut = 10,
    kEasingTypeExponentialIn = 11,
    kEasingTypeExponentialOut = 12,
    kEasingTypeSineInOut = 13,
    kEasingTypeSineIn = 14,
    kEasingTypeSineOut = 15,
    kEasingTypeBackInOut = 16,
    kEasingTypeBackIn = 17,
    kEasingTypeBackOut = 18,
};

enum GJDifficulty {
	kGJDifficultyAuto = 0,
	kGJDifficultyEasy = 1,
	kGJDifficultyNormal = 2,
	kGJDifficultyHard = 3,
	kGJDifficultyHarder = 4,
	kGJDifficultyInsane = 5,
	kGJDifficultyDemon = 6,
	kGJDifficultyDemonEasy = 7,
	kGJDifficultyDemonMedium = 8,
	kGJDifficultyDemonInsane = 9,
	kGJDifficultyDemonExtreme = 10
};

enum GJLevelType {
	kGJLevelTypeLocal = 1,
	kGJLevelTypeEditor = 2,
	kGJLevelTypeSaved = 3
};

enum IconType {
	kIconTypeCube           = 0,
	kIconTypeShip           = 1,
	kIconTypeBall           = 2,
	kIconTypeUfo            = 3,
	kIconTypeWave           = 4,
	kIconTypeRobot          = 5,
	kIconTypeSpider         = 6,
	kIconTypeDeathEffect    = 98,
	kIconTypeSpecial        = 99,
};

// Geode Addition
enum ComparisonType {
    kComparisonTypeEquals = 0,
    kComparisonTypeLarger = 1,
    kComparisonTypeSmaller = 2,
};

// Geode Addition
enum MoveTargetType {
    kMoveTargetTypeBoth = 0,
    kMoveTargetTypeXOnly = 1,
    kMoveTargetTypeYOnly = 2,
};

// Geode Addition
enum TouchToggleMode {
    kTouchToggleModeNormal = 0,
    kTouchToggleModeToggleOn = 1,
    kTouchToggleModeToggleOff = 2,
};

// Geode Addition
enum LeaderboardState {
    kLeaderboardStateTop100     = 1,
    kLeaderboardStateGlobal     = 2,
    kLeaderboardStateCreator    = 3,
    kLeaderboardStateFriends    = 4,
};