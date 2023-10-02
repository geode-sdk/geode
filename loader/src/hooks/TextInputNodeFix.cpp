#include <Geode/modify/CCTextInputNode.hpp>

#ifdef GEODE_IS_ANDROID

using namespace geode::prelude;

struct TextNodeFix : Modify<TextNodeFix, CCTextInputNode> {
	bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* field, char const* text, int count) {
        auto change = count >= this->m_maxLabelLength ? 1 : 0;

        this->m_maxLabelLength += change;
		auto ret = CCTextInputNode::onTextFieldInsertText(field, text, count);
        this->m_maxLabelLength -= change;
        return ret;
	}
};

#endif