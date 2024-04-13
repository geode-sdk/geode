#pragma once

struct TodoReturnPlaceholder;

#ifdef GEODE_REVERT_TODO_RETURN
    using TodoReturn = void;
    #pragma message("Reverting TodoReturn to void. This behavior is deprecated and will be removed in a later update.")
#else
    #define GEODE_REVERT_TODO_RETURN 0
    using TodoReturn = TodoReturnPlaceholder;
#endif

// thanks pie
enum class SearchType {
    Search = 0,
    Downloaded = 1,
    MostLiked = 2,
    Trending = 3,
    Recent = 4,
    UsersLevels = 5,
    Featured = 6,
    Magic = 7,
    Sends = 8,
    MapPack = 9,
    MapPackOnClick = 10,
    Awarded = 11,
    Followed = 12,
    Friends = 13,
    Users = 14,
    LikedGDW = 15,
    HallOfFame = 16,
    FeaturedGDW = 17,
    Similar = 18,
    Type19 = 19,
    TopListsUnused = 20,
    DailySafe = 21,
    WeeklySafe = 22,
    EventSafe = 23,
    Reported = 24,
    LevelListsOnClick = 25,
    Type26 = 26,
    Sent = 27,
    MyLevels = 98,
    SavedLevels = 99,
    FavouriteLevels = 100,
    SmartTemplates = 101,
    MyLists = 102,
    FavouriteLists = 103
};

enum class GameObjectType {
    Solid = 0,
    Hazard = 2,
    InverseGravityPortal = 3,
    NormalGravityPortal = 4,
    ShipPortal = 5,
    CubePortal = 6,
    Decoration = 7,
    YellowJumpPad = 8,
    PinkJumpPad = 9,
    GravityPad = 10,
    YellowJumpRing = 11,
    PinkJumpRing = 12,
    GravityRing = 13,
    InverseMirrorPortal = 14,
    NormalMirrorPortal = 15,
    BallPortal = 16,
    RegularSizePortal = 17,
    MiniSizePortal = 18,
    UfoPortal = 19,
    Modifier = 20,
    Breakable = 21,
    SecretCoin = 22,
    DualPortal = 23,
    SoloPortal = 24,
    Slope = 25,
    WavePortal = 26,
    RobotPortal = 27,
    TeleportPortal = 28,
    GreenRing = 29,
    Collectible = 30,
    UserCoin = 31,
    DropRing = 32,
    SpiderPortal = 33,
    RedJumpPad = 34,
    RedJumpRing = 35,
    CustomRing = 36,
    DashRing = 37,
    GravityDashRing = 38,
    CollisionObject = 39,
    Special = 40,
    SwingPortal = 41,
    GravityTogglePortal = 42,
    SpiderOrb = 43,
    SpiderPad = 44,
    TeleportOrb = 46,
    AnimatedHazard = 47,
};

enum class GJGameEvent {
    None = 0,
    TinyLanding = 1,
    FeatherLanding = 2,
    SoftLanding = 3,
    NormalLanding = 4,
    HardLanding = 5,
    HitHead = 6,
    OrbTouched = 7,
    OrbActivated = 8,
    PadActivated = 9,
    GravityInverted = 10,
    GravityRestored = 11,
    NormalJump = 12,
    RobotBoostStart = 13,
    RobotBoostStop = 14,
    UFOJump = 15,
    ShipBoostStart = 16,
    ShipBoostEnd = 17,
    SpiderTeleport = 18,
    BallSwitch = 19,
    SwingSwitch = 20,
    WavePush = 21,
    WaveRelease = 22,
    DashStart = 23,
    DashStop = 24,
    Teleported = 25,
    PortalNormal = 26,
    PortalShip = 27,
    PortalBall = 28,
    PortalUFO = 29,
    PortalWave = 30,
    PortalRobot = 31,
    PortalSpider = 32,
    PortalSwing = 33,
    YellowOrb = 34,
    PinkOrb = 35,
    RedOrb = 36,
    GravityOrb = 37,
    GreenOrb = 38,
    DropOrb = 39,
    CustomOrb = 40,
    DashOrb = 41,
    GravityDashOrb = 42,
    SpiderOrb = 43,
    TeleportOrb = 44,
    YellowPad = 45,
    PinkPad = 46,
    RedPad = 47,
    GravityPad = 48,
    SpiderPad = 49,
    PortalGravityFlip = 50,
    PortalGravityNormal = 51,
    PortalGravityInvert = 52,
    PortalFlip = 53,
    PortalUnFlip = 54,
    PortalNormalScale = 55,
    PortalMiniScale = 56,
    PortalDualOn = 57,
    PortalDualOff = 58,
    PortalTeleport = 59,
    Checkpoint = 60,
    DestroyBlock = 61,
    UserCoin = 62,
    PickupItem = 63,
    CheckpointRespawn = 64,
    FallLow = 65,
    FallMed = 66,
    FallHigh = 67,
    FallVHigh = 68,
    JumpPush = 69,
    JumpRelease = 70,
    LeftPush = 71,
    LeftRelease = 72,
    RightPush = 73,
    RightRelease = 74,
    PlayerReversed = 75,
    FallSpeedLow = 76,
    FallSpeedMed = 77,
    FallSpeedHigh = 78
};

enum class PulseEffectType {
};
enum class TouchTriggerType {
};
enum class PlayerButton {
    Jump = 1,
    Left = 2,
    Right = 3,
};
enum class GhostType {
};
enum class TableViewCellEditingStyle {
};
enum class UserListType {
    Friends = 0,
    Blocked = 1,
};
enum class GJErrorCode {
};
enum class AccountError {
};
enum class GJSongError {
};
enum class LikeItemType {
    Unknown = 0,
    Level = 1,
    Comment = 2,
    AccountComment = 3,
    LevelList = 4
};

enum class CommentError {
};
enum class BackupAccountError {
};
enum class GJMusicAction {};
enum class CellAction {};
enum class GJActionCommand {};
enum class DifficultyIconType {};
enum class GauntletType {};
enum class GJMPErrorCode {};
enum class GJTimedLevelType {
    Daily = 0,
    Weekly = 1,
    Event = 2
};
enum class SongSelectType {};
enum class AudioTargetType {};
enum class FMODReverbPreset {};
enum class DemonDifficultyType {};
enum class PlayerCollisionDirection {};
enum class ChestSpriteState {};
enum class FormatterType {};
enum class AudioModType {};
enum class FMODQueuedMusic {};
enum class GJAreaActionType {};
enum class SFXTriggerState {};
enum class SongTriggerState {};
enum class GJSmartDirection {};
enum class SmartBlockType {};
enum class TouchTriggerControl {};
enum class SmartPrefabResult {};
enum class AudioSortType {};
enum class spriteMode {};
enum class GJAssetType {};
enum class CommentKeyType {
    Level = 0,
    User = 1,
    LevelList = 2
};
enum class LevelLeaderboardMode {
    Time = 0,
    Points = 1
};
enum class StatKey {};
enum class TextStyleType {};
enum class InputValueType {};
enum class GJInputStyle {};
enum class GJDifficultyName {};
enum class GJFeatureState {};
enum class GJKeyGroup {};
enum class GJKeyCommand {};
enum class SelectSettingType {};
enum class gjParticleValue {};
enum class ColorSelectType {};
enum class AudioGuidelinesType {};
enum class SmartBrowseFilter {};
enum class GJUITouchEvent {};
enum class ObjectScaleType {};
enum class SavedActiveObjectState {};
enum class SavedSpecialObjectState {};
enum class SavedObjectStateRef {};

// Thanks cocoa!
#ifdef GEODE_IS_MACOS
    #undef CommentType
#endif

enum class CommentType {
    Level = 0,
    Account = 1,
};

enum class BoomListType {
    Default = 0x0,
    User = 0x2,
    Stats = 0x3,
    Achievement = 0x4,
    Level = 0x5,
    Level2 = 0x6,
    Comment = 0x7,
    Comment2 = 0x8,
    Comment3 = 0x9,
    Song = 0xc,
    Score = 0xd,
    MapPack = 0xe,
    CustomSong = 0xf,
    Comment4 = 0x10,
    User2 = 0x11,
    Request = 0x12,
    Message = 0x13,
    LevelScore = 0x14,
    Artist = 0x15,
    SmartTemplate = 0x16,
    SFX = 0x17,
    SFX2 = 0x18,
    CustomMusic = 0x19,
    Options = 0x1a,
    LevelList = 0x1b,
    Level3 = 0x1c,
    LevelList2 = 0x1d,
    LevelList3 = 0x1e,
    Level4 = 0x1f,
    LocalLevelScore = 0x21,
    URL = 0x22,
};

enum class CurrencySpriteType {
    // todo
};

enum class CurrencyRewardType {
    // todo
};

enum class MenuAnimationType {
    Scale = 0,
    Move = 1,
};

enum class ShopType {
    Normal,
    Secret,
    Community
};

// Geode Addition
enum class ZLayer {
    B5 = -5,
    B4 = -3,
    B3 = -1,
    B2 = 1,
    B1 = 3,
    Default = 0,
    T1 = 5,
    T2 = 7,
    T3 = 9,
    T4 = 11,
};

enum class UpdateResponse {
    Unknown,
    UpToDate,
    GameVerOutOfDate,
    UpdateSuccess,
};

enum class UnlockType {
    Cube = 0x1,
    Col1 = 0x2,
    Col2 = 0x3,
    Ship = 0x4,
    Ball = 0x5,
    Bird = 0x6,
    Dart = 0x7,
    Robot = 0x8,
    Spider = 0x9,
    Streak = 0xA,
    Death = 0xB,
    GJItem = 0xC,
    Swing = 0xD,
    Jetpack = 0xE,
    ShipFire = 0xF
};

enum class SpecialRewardItem {
    FireShard = 0x1,
    IceShard = 0x2,
    PoisonShard = 0x3,
    ShadowShard = 0x4,
    LavaShard = 0x5,
    BonusKey = 0x6,
    Orbs = 0x7,
    Diamonds = 0x8,
    CustomItem = 0x9,
    EarthShard = 0xA,
    BloodShard = 0xB,
    MetalShard = 0xC,
    LightShard = 0xD,
    SoulShard = 0xE
};

enum class EditCommand {
    SmallLeft = 1,
    SmallRight = 2,
    SmallUp = 3,
    SmallDown = 4,

    Left = 5,
    Right = 6,
    Up = 7,
    Down = 8,

    BigLeft = 9,
    BigRight = 10,
    BigUp = 11,
    BigDown = 12,

    TinyLeft = 13,
    TinyRight = 14,
    TinyUp = 15,
    TinyDown = 16,

    HalfLeft = 17,
    HalfRight = 18,
    HalfUp = 19,
    HalfDown = 20,

    FlipX = 21,
    FlipY = 22,
    RotateCW = 23,
    RotateCCW = 24,
    RotateCW45 = 25,
    RotateCCW45 = 26,
    RotateFree = 27,
    RotateSnap = 28,

    Scale = 29,
    ScaleXY = 30,
    Skew = 31
};

// Geode Addition
enum class PlaybackMode {
    Not = 0,
    Playing = 1,
    Paused = 2,
};

enum class SelectArtType {
    Background = 0,
    Ground = 1,
};

enum class UndoCommand {
    Delete = 1,
    New = 2,
    Paste = 3,
    DeleteMulti = 4,
    Transform = 5,
    Select = 6,
};

enum class EasingType {
    None = 0,
    EaseInOut = 1,
    EaseIn = 2,
    EaseOut = 3,
    ElasticInOut = 4,
    ElasticIn = 5,
    ElasticOut = 6,
    BounceInOut = 7,
    BounceIn = 8,
    BounceOut = 9,
    ExponentialInOut = 10,
    ExponentialIn = 11,
    ExponentialOut = 12,
    SineInOut = 13,
    SineIn = 14,
    SineOut = 15,
    BackInOut = 16,
    BackIn = 17,
    BackOut = 18,
};

enum class GJDifficulty {
    Auto = 0,
    Easy = 1,
    Normal = 2,
    Hard = 3,
    Harder = 4,
    Insane = 5,
    Demon = 6,
    DemonEasy = 7,
    DemonMedium = 8,
    DemonInsane = 9,
    DemonExtreme = 10
};

enum class GJLevelType {
    Local = 1,
    Editor = 2,
    Saved = 3
};

enum class GJRewardType
{
    Unknown = 0x0,
    Small = 0x1,
    Large = 0x2,
    SmallTreasure = 0x3,
    LargeTreasure = 0x4,
    Key10Treasure = 0x5,
    Key25Treasure = 0x6,
    Key50Treasure = 0x7,
    Key100Treasure = 0x8
};

enum class IconType {
    Cube = 0,
    Ship = 1,
    Ball = 2,
    Ufo = 3,
    Wave = 4,
    Robot = 5,
    Spider = 6,
    Swing = 7,
    Jetpack = 8,
    DeathEffect = 98,
    Special = 99,
    ShipFire = 101,
};

enum class GJChallengeType {
    Unknown = 0,
    Orbs = 1,
    UserCoins = 2,
    Stars = 3,
    Moons = 4,
};

enum class GJScoreType {
    Unknown = 0,
    Creator = 1
};

enum class LevelLeaderboardType {
    Friends = 0,
    Global = 1,
    Weekly = 2
};

enum class GJHttpType {

};

enum class DialogChatPlacement {
    Center = 0,
    Top = 1,
    Bottom = 2,
};

enum class DialogAnimationType {
    Instant = 0,
    FromCenter = 1,
    FromLeft = 2,
    FromRight = 3,
    FromTop = 4,
    // a 5th type is defined which acts exactly the same as FromTop
};

// Geode Addition
enum class ComparisonType {
    Equals = 0,
    Larger = 1,
    Smaller = 2,
};

// Geode Addition
enum class MoveTargetType {
    Both = 0,
    XOnly = 1,
    YOnly = 2,
};

// Geode Addition
enum class TouchToggleMode {
    Normal = 0,
    ToggleOn = 1,
    ToggleOff = 2,
};

// Geode Addition
enum class LeaderboardState {
    Default = 0,
    Top100 = 1,
    Global = 2,
    Creator = 3,
    Friends = 4,
};

// Wylie Addition (https://github.com/Wyliemaster/GD-Decompiled/blob/main/GD/code/headers/Layers/LevelSettingsLayer.h)
enum class Speed {
	Normal = 0,
	Slow = 1,
	Fast = 2,
	Faster = 3,
	Fastest = 4,
};
