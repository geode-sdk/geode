#include "Common.hpp"

#ifdef GEODE_IS_WINDOWS

GEODE_MEMBER_CHECK(cocos2d::CCApplication, m_fVsyncInterval, 0x8c);
GEODE_MEMBER_CHECK(AppDelegate, m_runningScene, 0xa0);

GEODE_MEMBER_CHECK(GameObject, m_effectPlistName, 0x278);
GEODE_MEMBER_CHECK(GameObject, m_effectManager, 0x454);
GEODE_MEMBER_CHECK(GameObject, m_firstPosition, 0x424);

GEODE_MEMBER_CHECK(PlayerObject, m_unk484, 0x484);
GEODE_MEMBER_CHECK(PlayerObject, m_ghostTrail, 0x4E4);
GEODE_MEMBER_CHECK(PlayerObject, m_waveTrail, 0x514);
GEODE_MEMBER_CHECK(PlayerObject, m_isDart, 0x63b);
GEODE_MEMBER_CHECK(PlayerObject, m_isDashing, 0x641);

GEODE_MEMBER_CHECK(EditorUI, m_buttonBar, 0x134);
GEODE_MEMBER_CHECK(EditorUI, m_hideableUIElementArray, 0x13c);
GEODE_MEMBER_CHECK(EditorUI, m_gridSize, 0x140);
GEODE_MEMBER_CHECK(EditorUI, m_objectInfoLabel, 0x168);
GEODE_MEMBER_CHECK(EditorUI, m_scaleControl, 0x17c);
GEODE_MEMBER_CHECK(EditorUI, m_selectedObject, 0x2c4);

GEODE_MEMBER_CHECK(LevelEditorLayer, m_editorUI, 0x380);
GEODE_MEMBER_CHECK(LevelEditorLayer, m_level, 0x3a8);

GEODE_MEMBER_CHECK(GameManager, m_valueKeeper, 0x110);
GEODE_MEMBER_CHECK(GameManager, m_loaded, 0x148);
GEODE_MEMBER_CHECK(GameManager, m_playLayer, 0x164);
GEODE_MEMBER_CHECK(GameManager, m_levelEditorLayer, 0x168);
GEODE_MEMBER_CHECK(GameManager, m_playerUDID, 0x180);
GEODE_MEMBER_CHECK(GameManager, m_backgroundMusicVolume, 0x1c0);
GEODE_MEMBER_CHECK(GameManager, m_sceneEnum, 0x1dc);
GEODE_MEMBER_CHECK(GameManager, m_ratePower, 0x2d0);

GEODE_MEMBER_CHECK(PlayLayer, m_startPos, 0x32c);
GEODE_MEMBER_CHECK(PlayLayer, m_screenRingObjects, 0x36c);
GEODE_MEMBER_CHECK(PlayLayer, m_bottomGround, 0x38c);
GEODE_MEMBER_CHECK(PlayLayer, m_topGround, 0x390);
GEODE_MEMBER_CHECK(PlayLayer, m_isDead, 0x39c);
GEODE_MEMBER_CHECK(PlayLayer, m_levelLength, 0x3b4);
GEODE_MEMBER_CHECK(PlayLayer, m_attemptLabel, 0x3bc);
GEODE_MEMBER_CHECK(PlayLayer, m_sliderGrooveSprite, 0x404);
GEODE_MEMBER_CHECK(PlayLayer, m_gravitySprites, 0x424);
GEODE_MEMBER_CHECK(PlayLayer, m_level, 0x488);
GEODE_MEMBER_CHECK(PlayLayer, m_isTestMode, 0x494);
GEODE_MEMBER_CHECK(PlayLayer, m_antiCheatObject, 0x530);
GEODE_MEMBER_CHECK(PlayLayer, m_disableGravityEffect, 0x536);

GEODE_MEMBER_CHECK(GJBaseGameLayer, m_boundingBox, 0x120);
GEODE_MEMBER_CHECK(GJBaseGameLayer, m_player1, 0x224);
GEODE_MEMBER_CHECK(GJBaseGameLayer, m_groups, 0x274);
GEODE_MEMBER_CHECK(GJBaseGameLayer, m_attemptClickCount, 0x2b4);
static_assert(sizeof(GJBaseGameLayer) == 0x2cc);

GEODE_MEMBER_CHECK(AudioEffectsLayer, m_unk19C, 0x19c);

#endif
