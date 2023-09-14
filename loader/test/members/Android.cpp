#include "Common.hpp"

#ifdef GEODE_IS_ANDROID

// Add known android struct members here

// needed classes are ones in the ids folder and some generic ones (i think they are already done though so only ids)

GEODE_MEMBER_CHECK(GameManager, m_playLayer, 0x138);
GEODE_MEMBER_CHECK(GameManager, m_levelEditorLayer, 0x13c);

GEODE_MEMBER_CHECK(PlayLayer, m_level, 0x470);

#endif