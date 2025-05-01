#include <Geode/modify/CCTextInputNode.hpp>

#ifdef GEODE_IS_ANDROID

using namespace geode::prelude;

// TODO: commenting cause cctextinputnode is changed enough that i'm not sure how it works now

// struct TextNodeFix : Modify<TextNodeFix, CCTextInputNode> {
// 	bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* field, char const* text, int count) {
//         auto change = count >= m_maxLabelLength ? 1 : 0;

//         m_maxLabelLength += change;
// 		auto ret = CCTextInputNode::onTextFieldInsertText(field, text, count);
//         m_maxLabelLength -= change;
//         return ret;
// 	}
// };

#endif