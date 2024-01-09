#include "Common.hpp"

#if 0

using namespace cocos2d;

GEODE_SIZE_CHECK(CCNode, 0x108);
// GEODE_MEMBER_CHECK(CCNode, m_pUserObject, 0xcc);

GEODE_SIZE_CHECK(CCSprite, 0x1fc);

GEODE_SIZE_CHECK(CCLayer, 0x13c);

GEODE_SIZE_CHECK(CCLayerColor, 0x1b8);

// GEODE_SIZE_CHECK(CCLabelBMFont, 0x194);

GEODE_SIZE_CHECK(CCDirector, 0x130);
GEODE_MEMBER_CHECK(CCDirector, m_pobOpenGLView, 0x8c);
GEODE_MEMBER_CHECK(CCDirector, m_fFrameRate, 0xac);
GEODE_MEMBER_CHECK(CCDirector, m_pRunningScene, 0xcc);
GEODE_MEMBER_CHECK(CCDirector, m_pobScenesStack, 0xd8);
GEODE_MEMBER_CHECK(CCDirector, m_obWinSizeInPoints, 0xe8);


GEODE_MEMBER_CHECK(CCTextInputNode, m_allowedChars, 0x174);
GEODE_MEMBER_CHECK(CCTextInputNode, m_maxLabelWidth, 0x18c);
GEODE_MEMBER_CHECK(CCTextInputNode, m_maxLabelScale, 0x190);
GEODE_MEMBER_CHECK(CCTextInputNode, m_placeholderScale, 0x194);
GEODE_MEMBER_CHECK(CCTextInputNode, m_placeholderColor, 0x198);

GEODE_MEMBER_CHECK(GJListLayer, m_listView, 0x1b8);

GEODE_MEMBER_CHECK(BoomListView, m_tableView, 0x144);
GEODE_MEMBER_CHECK(BoomListView, m_entries, 0x148);

GEODE_MEMBER_CHECK(TableView, m_cellArray, 0x1b0);
GEODE_MEMBER_CHECK(CCScrollLayerExt, m_cutContent, 0x16c);

GEODE_SIZE_CHECK(SliderThumb, 0x140);
GEODE_SIZE_CHECK(Slider, 0x158);
GEODE_MEMBER_CHECK(Slider, m_touchLogic, 0x13c);
GEODE_SIZE_CHECK(SliderTouchLogic, 0x178);




#endif
