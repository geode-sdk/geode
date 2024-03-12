#include <Geode/modify/CCTextInputNode.hpp>

#ifdef GEODE_IS_ANDROID

using namespace geode::prelude;

struct TextNodeFix : Modify<TextNodeFix, CCTextInputNode> {
    bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* field, const char* text, int count) override {
        if (count >= m_maxLabelLength) {
            ++m_maxLabelLength;
            bool ret = CCTextInputNode::onTextFieldInsertText(field, text, count);
            --m_maxLabelLength;
            return ret;
        } else {
            return CCTextInputNode::onTextFieldInsertText(field, text, count);
        }
    }
};

#endif
