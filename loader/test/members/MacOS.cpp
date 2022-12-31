#include "Common.hpp"

#ifdef GEODE_IS_MACOS

// PlayerObject
GEODE_MEMBER_CHECK(PlayerObject, m_collisionLog, 0x518);
GEODE_MEMBER_CHECK(PlayerObject, m_particleSystems, 0x590);
GEODE_MEMBER_CHECK(PlayerObject, m_waveTrail, 0x600);
GEODE_MEMBER_CHECK(PlayerObject, m_lastCollidedSolid, 0x690);
GEODE_MEMBER_CHECK(PlayerObject, m_playerColor1, 0x7c2);

// EditorUI
GEODE_MEMBER_CHECK(EditorUI, m_buttonBar, 0x198);
GEODE_MEMBER_CHECK(EditorUI, m_selectedObject, 0x440);

// LevelEditorLayer
GEODE_MEMBER_CHECK(LevelEditorLayer, m_editorUI, 0x5d8);
GEODE_MEMBER_CHECK(LevelEditorLayer, m_level, 0x618);

// GameManager
GEODE_MEMBER_CHECK(GameManager, m_playLayer, 0x180);
GEODE_MEMBER_CHECK(GameManager, m_levelEditorLayer, 0x188);
// GEODE_MEMBER_CHECK(GameManager, m_clickedGarage, 0x2a2); // todo: find why this is not 0x2a2

#endif