#include <Geode/modify/CCTextInputNode.hpp>

// This doesn't actually add any IDs since those aren't needed for 
// CCTextInputNode where everything is accessible through members. 
// This is to fix the effects of the epic mistake of Cocos2d inventing 
// ignoreAnchorPointForPosition which causes the content size of 
// text input nodes to be way off

struct $modify(CCTextInputNode) {
    bool init(float width, float height, const char* caption, const char* thonburi, int maxCharCount, const char* font) {
        if (!CCTextInputNode::init(width, height, caption, thonburi, maxCharCount, font))
            return false;
        
        this->ignoreAnchorPointForPosition(false);
        this->fixPosition();

        return true;
    }

    void fixPosition() {
        if (!m_bIgnoreAnchorPointForPosition && m_placeholderLabel) {
            this->setAnchorPoint(m_placeholderLabel->getAnchorPoint());
            m_placeholderLabel->setPosition(m_obContentSize * m_obAnchorPoint);
        }
    }

    void updateLabel(gd::string text) {
        CCTextInputNode::updateLabel(text);
        this->fixPosition();
    }
};
