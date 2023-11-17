#pragma once

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
    ListsOnClick = 19,
    Lists = 20,
    DailyVault = 21,
    WeeklyVault = 22,
    EventVault = 23,
    Reported = 24,
    MyLevels = 98,
    SavedLevels = 99,
    FavouriteLevels = 100
};

// jesus fucking christ (painfully written by @hjfod)
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
};

enum class PulseEffectType {
};
enum class TouchTriggerType {
};
enum class PlayerButton {
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
    AccountComment = 3
};

enum class GJStoreItem {
};
enum class CommentError {
};
enum class BackupAccountError {
};

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
    Song = 0xb,
    Score = 0xc,
    MapPack = 0xd,
    CustomSong = 0xe,
    Comment3 = 0xf,
    User2 = 0x10,
    Request = 0x11,
    Message = 0x12,
    LevelScore = 0x13,
    Artist = 0x14,
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
    B4 = -3,
    B3 = -1,
    B2 = 1,
    B1 = 3,
    Default = 0,
    T1 = 5,
    T2 = 7,
    T3 = 9,
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

    FlipX = 17,
    FlipY = 18,
    RotateCW = 19,
    RotateCCW = 20,
    RotateCW45 = 21,
    RotateCCW45 = 22,
    RotateFree = 23,
    RotateSnap = 24,

    Scale = 25,
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
    Large = 0x2
};

enum class IconType {
    Cube = 0,
    Ship = 1,
    Ball = 2,
    Ufo = 3,
    Wave = 4,
    Robot = 5,
    Spider = 6,
    DeathEffect = 98,
    Special = 99,
};

enum class GJChallengeType {
    Unknown = 0,
    Orbs = 1,
    UserCoins = 2,
    Stars = 3
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
