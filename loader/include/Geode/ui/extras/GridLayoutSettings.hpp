#include "../Layout.hpp"

namespace geode {

class GEODE_DLL GridLayoutSettings : public Popup<GridLayout*, cocos2d::CCNode*> {
protected:
	GridLayout* m_layout;
	cocos2d::CCNode* m_node;

	cocos2d::CCLabelBMFont* m_gapXLabel;
	cocos2d::CCLabelBMFont* m_gapYLabel;
	CCMenuItemToggler* m_expandAxisToggler;
	TextInput* m_maxCountInput;
	CCMenuItemToggler* m_autoAdjustSizeToggler;
	CCMenuItemToggler* m_reverseExpandAxisToggler;
	CCMenuItemToggler* m_reverseCrossExpandAxisToggler;


	bool setup(GridLayout* layout, cocos2d::CCNode* node) override;
	

	void onChangeGap(cocos2d::CCObject* sender);

	void onMaxCountArrow(cocos2d::CCObject* sender);

	void onMaxCountInfo(cocos2d::CCObject* sender);

	void onApply(cocos2d::CCObject* sender);
	
public:
	static GridLayoutSettings* create(GridLayout* layout, cocos2d::CCNode* node);
};

}