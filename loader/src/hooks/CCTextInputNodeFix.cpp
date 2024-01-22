// FIXME: This fix ends up breaking some of the vanilla text inputs.
#if 0

using namespace geode::prelude;

// rob only uses `CCTextInputNode`s in mostly-flat hierarchies, which still
// happen to work with the weird vanilla code. this fix makes it work even in
// deep hierarchies, because the vanilla code uses `getParent` and manually
// calculates the child location in the world space based on that rather than
// using `convertToNodeSpace`.

struct CCTextInputNodeFix : Modify<CCTextInputNodeFix, CCTextInputNode> {
	bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
		auto const touchPos = touch->getLocation();
		auto const size = this->getContentSize();
		auto const pos = this->convertToNodeSpace(touchPos) + m_textField->getAnchorPoint() * size;

		if (pos.x < 0 || pos.x > size.width || pos.y < 0 || pos.y > size.height)
			return false;
		if (m_delegate && !m_delegate->allowTextInput(this))
			return false;

		this->onClickTrackNode(true);

		return true;
	}
};
#endif

#include <Geode/Geode.hpp>

// This doesn't actually add any IDs since those aren't needed for 
// CCTextInputNode where everything is accessible through members. 
// This is to fix the effects of the epic mistake of Cocos2d inventing 
// ignoreAnchorPointForPosition which causes the content size of 
// text input nodes to be way off

#include <Geode/modify/CCTextInputNode.hpp>

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
