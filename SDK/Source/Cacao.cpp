// 
// Copyright camila314 & alk1m123 2022. 
//
#include <Cacao.hpp>
#include <stdexcept>
#include <set>
#include <iostream>

cocos2d::CCDestructor::~CCDestructor() {
	destructorLock.erase(this);
};

namespace Cacao {
using namespace cocos2d;

CCPoint anchorPosition(double x, double y, double ax, double ay) {
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	float axp = CCDirector::sharedDirector()->getScreenLeft() + winSize.width * ax;
	float ayp = CCDirector::sharedDirector()->getScreenBottom() + winSize.height * ay;
	
	CCPoint ccp;
	ccp.x = axp + x;
	ccp.y = ayp + y;
	return ccp;
}

CCPoint relativePosition(double x, double y) {
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	float xp = winSize.width * (x/100.);
	float yp = winSize.height * (y/100.);
	CCPoint ccp;
	ccp.x = xp;
	ccp.y = yp;
	return ccp;
}
CCPoint addedPosition(double x, double y) {
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	float xp = winSize.width/2 + x;
	float yp = winSize.height/2 + y;
	CCPoint ccp;
	ccp.x = xp;
	ccp.y = yp;
	return ccp;
}

CCSprite* spriteFromPng(unsigned char* img, int img_len) {
	auto image = new CCImage();
	image->initWithImageData((void*)img, img_len, CCImage::kFmtPng,1,1,1);

	auto text = new CCTexture2D();
	text->initWithImage(image);

	CCRect r(0, 0, image->getWidth(), image->getHeight());

	auto sprite = CCSprite::create();
	sprite->initWithTexture(text, r);

	sprite->setTexture(text);

	return sprite;
}

CCMenuItemToggler* createToggler(CCObject* parent, SEL_MenuHandler callback) {
	auto on = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	auto off = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
	return CCMenuItemToggler::create(off, on, parent, callback);
}

CCPoint touchToView(CCPoint p) {
	auto bar = cocos2d::CCDirector::sharedDirector()->getWinSize()/2;
	p = p - bar;

	Cacao::printGeometry(p);
	Cacao::printGeometry(bar);
	printf("----\n");

    return ccp(p.x, -p.y);
}

// void FLDialogDelegate::onSubmit(FLDialogHelper* dl, const std::string& text) {dl->close();}
// void FLDialogDelegate::onCancel(FLDialogHelper* dl) {dl->close();}
// void FLDialogDelegate::onShow(FLDialogHelper* dl) {}

// FLDialogHelper* FLDialogHelper::create(FLDialogDelegate* del, char const* title, char const* submit, char const* cancel, char const* placeholder) {
//     FLDialogHelper* pRet = new FLDialogHelper();
//     if (pRet->initWithStuff(del, title, submit, cancel, placeholder)) {
//         return pRet;
//     }
//     delete pRet;
//     return NULL;
// }
// // convenience functions
// FLDialogHelper* FLDialogHelper::create(FLDialogDelegate* del, char const* title, char const* submit, char const* cancel) {
//     return FLDialogHelper::create(del, title, submit, cancel, "Insert text");
// }
// FLDialogHelper* FLDialogHelper::create(FLDialogDelegate* del, char const* title, char const* submit) {
//     return FLDialogHelper::create(del, title, submit, "Cancel");
// }
// FLDialogHelper* FLDialogHelper::create(FLDialogDelegate* del, char const* title) {
//     return FLDialogHelper::create(del, title, "Submit");
// }
// FLDialogHelper* FLDialogHelper::create(FLDialogDelegate* del) {
//     return FLDialogHelper::create(del, "Cacao Prompt");
// }
// FLDialogHelper* FLDialogHelper::create() {
//     return FLDialogHelper::create(NULL);
// }

// void FLDialogHelper::close() {
//     alertLayer->keyBackClicked();
// }
// void FLDialogHelper::onSubmit(CCObject*) {
//     std::string text(textNode->getString());
//     del->onSubmit(this, text);
// }

// void FLDialogHelper::onCancel(CCObject*) {
//     del->onCancel(this);
// }

// bool FLDialogHelper::initWithStuff(FLDialogDelegate* delegate, char const* title, char const* submit, char const* cancel, char const* placeholder) {
//     if (delegate) {
//         del = delegate;
//     } else {
//         del = new FLDialogDelegate();
//     }


//     alertLayer = FLAlertLayer::create("ok","ok","ok");
//     mainLayer = alertLayer->m_mainLayer;

//     CCArray* ch = alertLayer->m_mainLayer->getChildren();

//     for (int i=0; i<ch->count(); i++) { 
//         // im so sorry for this code ik its bad.
//         // all you need to know is 0x682FC0 is the vtable address for CCMenu
//         // all im doing here is just making all of the FLALertLayer stuff invisible
//         auto ob = ch->objectAtIndex(i);
//         if ((getBase()+0x682FC0) == *((long*)ob)) { // if its a CCMenu
//             CCArray* ch2 = ((CCNode*)ob)->getChildren();
//             for (int i=0; i<ch2->count(); i++) {
//                 ((CCMenuItem*)ch2->objectAtIndex(i))->setVisible(false);
//             }
//         }  else {
//             ((CCNode*)ob)->setVisible(false);
//         }
//     }

//     CCSize wSize = CCDirector::sharedDirector()->getWinSize();

//         CCRect squareRect(0,0,80,80);
//         float squareHeight = 135.f;
//         float squareY = 80.f;
//         CCPoint squarePosition = {wSize.width/2.f, wSize.height/2.f + squareY};
//         CCPoint titlePosition = {squarePosition.x, (float)(squarePosition.y + squareHeight/2. - 16)};
//         CCPoint menuPosition = {squarePosition.x, squarePosition.y - 60};

//         auto label = CCLabelBMFont::create(title, "bigFont.fnt");
//         label->setPosition(titlePosition);
//         label->setScale(0.6);
//         mainLayer->addChild(label);

//         float textWidth = 360.0;
//         float textHeight = 50.0;

//         auto input = CCTextInputNode::create(textWidth, textHeight, placeholder, "Thonburi", 12, NULL);
//         input->setPosition(squarePosition);
//         mainLayer->addChild(input);
//         textNode = input;

//         CCPoint colOffset = {-100, -8};
//         auto lay = CCLayerColor::create(ccc4(230,230,230,150), 200, 16);
//         lay->setPosition(squarePosition + colOffset);
//         mainLayer->addChild(lay);

//         auto canc = ButtonSprite::create(cancel, 90, 0, 1, true);
//         auto menuCancel = CCMenuItemSpriteExtra::create(canc, canc, this, menu_selector(FLDialogHelper::onCancel));
//         menuCancel->setScale(0.8);

//         auto subm = ButtonSprite::create(submit, 90, 0, 1, true);
//         auto menuSubmit = CCMenuItemSpriteExtra::create(subm, subm, this, menu_selector(FLDialogHelper::onSubmit));
//         CCPoint offset(50, 0);
//         menuSubmit->setScale(0.8);

//         for (int i=0; i<ch->count(); i++) { // same with this
//             //for weird reasons the button only works if i add it to the existing CCMenu.
//             // so i am doing that here
//             auto ob = ch->objectAtIndex(i);
//             if ((getBase()+0x682FC0) == *((long*)ob)) { // if its a CCMenu
//                 CCMenu* m = (CCMenu*)ob;
//                 menuSubmit->setPosition(m->convertToNodeSpace(menuPosition + offset));
//                 menuCancel->setPosition(m->convertToNodeSpace(menuPosition - offset));

//                 m->addChild(menuSubmit);
//                 m->addChild(menuCancel);
//                 break;
//             }
//         }

//     return true;
// }
// void FLDialogHelper::show() {
//     del->onShow(this);
//     alertLayer->show();
// }

// void CacAlertLayer::keyBackClicked() {
//     onClose();
//     setKeypadEnabled(false);
//     setTouchEnabled(false);
//     removeFromParentAndCleanup(true);
// }

// bool CacAlertLayer::init(CCSize size) {
//     uint64_t ok1 = *((uint64_t*)this);
//     uint64_t ok2 = (*((uint64_t*)(ok1-8)))+16;

//     uint64_t n_typinfo = getBase()+0x65d870;
//     MemoryContainer(ok2, 8, reinterpret_cast<char*>(&n_typinfo)).enable();


//     if (CCLayerColor::initWithColor(ccc4(0,0,0,150))) { 

//         m_mainLayer = CCLayer::create();
//         m_buttonMenu = CCMenu::create();
//         addChild(m_mainLayer);
//         m_mainLayer->addChild(m_buttonMenu);

//         auto scale9 = extension::CCScale9Sprite::create("GJ_square01.png");
//         scale9->setContentSize(size);
//         scale9->setPosition(Cacao::relativePosition(50,50));
//         m_mainLayer->addChild(scale9, -2);


//         auto close = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");

//         m_closeButton = CCMenuItemSpriteExtra::create(close, NULL, this, menu_selector(CacAlertLayer::l_onClose));
//         m_closeButton->setPosition( - size.width / 2, size.height / 2 );
//         m_buttonMenu->addChild(m_closeButton);


//         m_title->limitLabelWidth(size.width*4, .75f, .2f);
//         m_title->setPosition(Cacao::addedPosition(0, size.height/2 - 25));
//         m_mainLayer->addChild(m_title);

//         alertInit();
//         return true;
//     }
//     return false;
// }

// void CacAlertLayer::show(void) {
//     //m_helper->m_noElasticity = m_noElasticity;
//     //m_helper->show();

//     registerWithTouchDispatcher();
//     CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);

//     setTouchEnabled(true);
//     setKeypadEnabled(true);

//     FLAlertLayer::show();
// }

// CacTextContainer* CacTextContainer::create(CCSize const& size, TextInputDelegate* delegate, char const* font) {
//     auto pRet = new CacTextContainer();
//     if (pRet && pRet->init(size, 24, delegate, font)) {
//         pRet->autorelease();
//         return pRet;
//     }
//     CC_SAFE_DELETE(pRet);
//     return NULL;
// }

// bool CacTextContainer::init(CCSize size, float fontSize, TextInputDelegate* delegate, char const* font) {
//     float clampMult = fmin(1, size.height/40.0);
//     if ((m_textInputNode = CCTextInputNode::create(size.width, size.height, "", "Thonburi", fontSize, font))) {
//         m_textInputNode->m_delegate = delegate;
//         m_textInputNode->setLabelPlaceholderColor(ccc3(120, 170, 240));
//         addChild(m_textInputNode,2);
//         m_textInputNode->setMaxLabelScale(size.height/50.);
//         m_box = extension::CCScale9Sprite::create("square02b_small.png");
//         m_box->setOpacity(100);
//         m_box->setColor(ccc3(0,0,0));
//         m_box->setContentSize(CCSizeMake((size.width+10)/clampMult, fmax(size.height-10,40)));
//         m_box->setScale(clampMult);
//         addChild(m_box, 1);
//         return true;
//     }
//     return false;
// }
}  // namespace Cacao
