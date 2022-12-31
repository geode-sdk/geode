#include "Common.hpp"

#ifdef GEODE_IS_MACOS

// PlayerObject
GEODE_MEMBER_CHECK(PlayerObject, m_collisionLog, 0x518);
GEODE_MEMBER_CHECK(PlayerObject, m_particleSystems, 0x590);
GEODE_MEMBER_CHECK(PlayerObject, m_waveTrail, 0x600);
GEODE_MEMBER_CHECK(PlayerObject, m_lastCollidedSolid, 0x690);
GEODE_MEMBER_CHECK(PlayerObject, m_playerColor1, 0x7c2);
#endif