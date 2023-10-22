#include <Geode/modify/CCTextInputNode.hpp>

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
