#include "Common.hpp"

#ifdef GEODE_IS_ANDROID32

using namespace geode::prelude;

// Add known android struct members here

GEODE_MEMBER_CHECK(CCMenuItemSprite, m_pfnSelector, 0x11c);

GEODE_SIZE_CHECK(CCObject, 0x34);
GEODE_SIZE_CHECK(CCNode, 0x108);
GEODE_SIZE_CHECK(CCNodeRGBA, 0x118);
GEODE_SIZE_CHECK(CCLayer, 0x13c);
GEODE_SIZE_CHECK(CCLayerRGBA, 0x14c);
GEODE_SIZE_CHECK(CCLayerColor, 0x1b8);
GEODE_SIZE_CHECK(CCSprite, 0x1fc);

GEODE_SIZE_CHECK(CCMenu, 0x154);
GEODE_SIZE_CHECK(CCMenuItem, 0x12c);
GEODE_SIZE_CHECK(CCMenuItemSprite, 0x138);

GEODE_SIZE_CHECK(CCScale9Sprite, 0x1a4);

GEODE_SIZE_CHECK(FLAlertLayer, 0x1f0);
GEODE_SIZE_CHECK(BoomListView, 0x164);
GEODE_SIZE_CHECK(CustomListView, 0x168);
GEODE_SIZE_CHECK(CCMenuItemSpriteExtra, 0x174);
GEODE_SIZE_CHECK(LoadingLayer, 0x15c);
GEODE_SIZE_CHECK(GJDropDownLayer, 0x1e4);
GEODE_SIZE_CHECK(TableViewCell, 0x19c);
GEODE_SIZE_CHECK(Slider, 0x158);
GEODE_SIZE_CHECK(SliderTouchLogic, 0x174);
GEODE_SIZE_CHECK(CCScrollLayerExt, 0x184);
GEODE_SIZE_CHECK(TableView, 0x1dc);
GEODE_SIZE_CHECK(CCTextInputNode, 0x1a0);
GEODE_SIZE_CHECK(CCTouchDispatcher, 0x6c);
GEODE_SIZE_CHECK(CCSpritePlus, 0x208);

// GEODE_MEMBER_CHECK(CCTouchDispatcher, m_forcePrio, 0x60);

GEODE_MEMBER_CHECK(CCTextInputNode, m_maxLabelWidth, 0x164);
GEODE_MEMBER_CHECK(CCTextInputNode, m_textField, 0x17c);
GEODE_MEMBER_CHECK(FLAlertLayer, m_buttonMenu, 0x1b8);
GEODE_MEMBER_CHECK(FLAlertLayer, m_mainLayer, 0x1c8);
GEODE_MEMBER_CHECK(TableView, m_cellArray, 0x1b0);
GEODE_MEMBER_CHECK(TableViewCell, m_backgroundLayer, 0x190);
GEODE_SIZE_CHECK(CCBlockLayer, 0x1bc);


GEODE_SIZE_CHECK(GameObject, 0x488);
GEODE_SIZE_CHECK(EnhancedGameObject, 0x4f0);
GEODE_SIZE_CHECK(EffectGameObject, 0x668);
GEODE_SIZE_CHECK(TextGameObject, 0x490);
GEODE_SIZE_CHECK(SmartGameObject, 0x490);
GEODE_SIZE_CHECK(ParticleGameObject, 0x618);
GEODE_SIZE_CHECK(SpecialAnimGameObject, 0x4f0);
GEODE_SIZE_CHECK(RingObject, 0x668);
GEODE_SIZE_CHECK(StartPosObject, 0x668);
GEODE_SIZE_CHECK(LabelGameObject, 0x690);
GEODE_SIZE_CHECK(TeleportPortalObject, 0x6a0);
// GEODE_SIZE_CHECK(EventLinkTrigger, 0x688);

GEODE_SIZE_CHECK(GJBaseGameLayer, 0x2d40);
GEODE_MEMBER_CHECK(GJBaseGameLayer, m_level, 0x5f0);
GEODE_MEMBER_CHECK(GJBaseGameLayer, m_levelSettings, 0x88c);

GEODE_MEMBER_CHECK(LevelEditorLayer, m_coinCount, 0x2da8);
GEODE_MEMBER_CHECK(LevelEditorLayer, m_undoObjects, 0x2dec);

GEODE_MEMBER_CHECK(GameManager, m_gameLayer, 0x174);

GEODE_MEMBER_CHECK(EditorUI, m_selectedObjects, 0x260);
GEODE_MEMBER_CHECK(EditorUI, m_selectedObject, 0x370);
GEODE_MEMBER_CHECK(EditorUI, m_editButtonBar, 0x22c);

GEODE_SIZE_CHECK(SetupTriggerPopup, 0x2a8);
GEODE_MEMBER_CHECK(ColorSelectPopup, m_colorAction, 0x2f0);


// GEODE_MEMBER_CHECK(CCNode, m_pUserObject, 0xcc);

// needed classes are ones in the ids folder and some generic ones (i think they are already done though so only ids)

// GEODE_MEMBER_CHECK(GameManager, m_playLayer, 0x138);
// GEODE_MEMBER_CHECK(GameManager, m_levelEditorLayer, 0x13c);
// GEODE_MEMBER_CHECK(GameManager, m_canGetLevelSaveData, 0x28c);

// static_assert(sizeof(GJBaseGameLayer) == 0x2cc);

// GEODE_MEMBER_CHECK(PlayLayer, unknown4e8, 0x2e8);
// GEODE_MEMBER_CHECK(PlayLayer, m_endPortal, 0x324);
// GEODE_MEMBER_CHECK(PlayLayer, m_bottomGround, 0x37c);
// GEODE_MEMBER_CHECK(PlayLayer, m_topGround, 0x380);
// GEODE_MEMBER_CHECK(PlayLayer, m_level, 0x470);
// GEODE_MEMBER_CHECK(PlayLayer, m_shouldTryToKick, 0x4e0);

// static_assert(sizeof(GameObject) == 0x42c);

// GEODE_MEMBER_CHECK(GameStatsManager, m_dailyChests, 0x110);
// GEODE_MEMBER_CHECK(GameStatsManager, m_completedLevels, 0x164);

// GEODE_MEMBER_CHECK(DailyLevelPage, m_weekly, 0x1ed);

// GEODE_MEMBER_CHECK(TeleportPortalObject, m_orangePortal, 0x430);

// GEODE_MEMBER_CHECK(EditorUI, m_rotationControl, 0x16c);
// GEODE_MEMBER_CHECK(EditorUI, m_updateTimeMarkers, 0x1a4);
// GEODE_MEMBER_CHECK(EditorUI, m_selectedObjects, 0x1bc);
// GEODE_MEMBER_CHECK(EditorUI, m_selectedObject, 0x2c4);

// GEODE_MEMBER_CHECK(MoreSearchLayer, m_enterSongID, 0x1E4);
// GEODE_MEMBER_CHECK(MoreSearchLayer, m_songLeftBtn, 0x1D4);

#endif