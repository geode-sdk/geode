#include "Common.hpp"

#ifdef GEODE_IS_WINDOWS

using namespace cocos2d;

GEODE_SIZE_CHECK(CCNode, 0x108);
// GEODE_MEMBER_CHECK(CCNode, m_pUserObject, 0xcc);

GEODE_SIZE_CHECK(CCSprite, 0x1fc);

GEODE_SIZE_CHECK(CCLayer, 0x13c);

GEODE_SIZE_CHECK(CCLayerColor, 0x1b8);

// GEODE_SIZE_CHECK(CCLabelBMFont, 0x194);

GEODE_MEMBER_CHECK(CCTextInputNode, m_allowedChars, 0x174);
GEODE_MEMBER_CHECK(CCTextInputNode, m_maxLabelWidth, 0x18c);
GEODE_MEMBER_CHECK(CCTextInputNode, m_maxLabelScale, 0x190);
GEODE_MEMBER_CHECK(CCTextInputNode, m_placeholderScale, 0x194);
GEODE_MEMBER_CHECK(CCTextInputNode, m_placeholderColor, 0x198);


#endif
