// 
// Copyright camila314 & alk1m123 2022. 
//
#pragma once

#include <Cacao>
#include <map>
#include <vector>
#include <type_traits>

#define CAC_CREATE(cls, ...) static cls* create() {\
	auto pRet = new cls(); \
	if (pRet && pRet->init(__VA_ARGS__)) { \
		pRet->autorelease(); \
		return pRet; \
	} \
	CC_SAFE_DELETE(pRet); \
	return NULL;}

#define CAC_TYPEINFO(addr)

#define offsetOf(member) (uintptr_t)&member - (uintptr_t)this

namespace Cacao {
	using namespace cocos2d;

	inline void printGeometry(CCPoint p) {
		std::cout << "X: " << p.x << " Y: " << p.y << "\n";
	}
	inline void printGeometry(CCSize p) {
		std::cout << "Width: " << p.width << " Height: " << p.height << "\n";
	}
	inline void printGeometry(CCRect p) {
		std::cout << "X: " << p.origin.x << " Y: " << p.origin.y << " Width: " << p.size.width << " Height: " << p.size.height << "\n";
	}

	template <typename K>
	void scheduleFunction(K func) {
		GameManager::sharedState()->getScheduler()->scheduleSelector(reinterpret_cast<SEL_SCHEDULE&>(func), GameManager::sharedState(), 0.0, 0, 0.0, false);
	}
	template <typename K>
	void scheduleFunction(K func, float delay) {
		GameManager::sharedState()->getScheduler()->scheduleSelector(reinterpret_cast<SEL_SCHEDULE&>(func), GameManager::sharedState(), 0.0, 0, delay, false);
	}

	template <typename K>
	void repeatFunction(K func, int times) {
		GameManager::sharedState()->getScheduler()->scheduleSelector(reinterpret_cast<SEL_SCHEDULE&>(func), GameManager::sharedState(), 0.0, times, 0.0, false);
	}
	
	template <typename K>
	void repeatFunction(K func, int times, float delay) {
		GameManager::sharedState()->getScheduler()->scheduleSelector(reinterpret_cast<SEL_SCHEDULE&>(func), GameManager::sharedState(), delay, times, 0.0, false);
	}

	template <typename T>
	char const* typeinfo_name_for(T ptr) {
		long vtable = *(long*)(ptr);
		long typeinfo = *(long*)(vtable-8);
		char const* infoname = *(char const**)(typeinfo + 8);
		return infoname;
	}

	template <typename T>
	char const* typeinfoNameFor(T ptr) {
		long vtable = *(long*)(ptr);
		long typeinfo = *(long*)(vtable-8);
		char const* infoname = *(char const**)(typeinfo + 8);
		return infoname;
	}

	template <typename T>
	std::vector<T> ccToVec(CCArray* arr) {
		return std::vector<T>(reinterpret_cast<T*>(arr->data->arr), reinterpret_cast<T*>(arr->data->arr) + arr->data->num);
	}

	CCPoint anchorPosition(double x, double y, double ax, double ay);
	CCPoint relativePosition(double x, double y);
	CCPoint addedPosition(double x, double y);
	CCPoint touchToView(CCPoint x);
	CCSprite* spriteFromPng(unsigned char* img, int img_len);

	CCMenuItemToggler* createToggler(CCObject* parent, SEL_MenuHandler callback);

	// class FLDialogHelper;

	// class FLDialogDelegate {
	// protected:
	//     virtual void onSubmit(FLDialogHelper* dl, const std::string& text);
	//     virtual void onCancel(FLDialogHelper* dl);
	//     virtual void onShow(FLDialogHelper* dl);
	//     friend class FLDialogHelper;
	// };

	// class FLDialogHelper : public CCNode {
	//  public:
	//     static FLDialogHelper* create(FLDialogDelegate* del, char const* title, char const* submit, char const* cancel, char const* placeholder);
	//     // convenience functions
	//     static FLDialogHelper* create(FLDialogDelegate* del, char const* title, char const* submit, char const* cancel);
	//     static FLDialogHelper* create(FLDialogDelegate* del, char const* title, char const* submit);
	//     static FLDialogHelper* create(FLDialogDelegate* del, char const* title);
	//     static FLDialogHelper* create(FLDialogDelegate* del);
	//     static FLDialogHelper* create();
	//     void close();
	//     void show();

	//     bool initWithStuff(FLDialogDelegate* delegate, char const* title, char const* submit, char const* cancel, char const* placeholder);
	//  protected:
	//     FLAlertLayer* alertLayer;
	//     CCLayer* mainLayer;
	//     CCTextInputNode* textNode;
	//     FLDialogDelegate* del;
	//  private:
	//     void onSubmit(CCObject*);
	//     void onCancel(CCObject*);
	// };


	// class CacAlertLayer : public FLAlertLayer {
	//  public:
	//     CacAlertLayer() : 
	//         FLAlertLayer() {
	//             m_title = CCLabelBMFont::create("", "goldFont.fnt");
	//         }

	//     virtual void onClose() {}
	//     virtual bool alertInit() {return true;}
	//     virtual void keyBackClicked();
	//     virtual void show();
	//     inline void l_onClose(CCObject* sender) {keyBackClicked();}

	//     bool init(CCSize size);
	//     inline bool init() {return init(CCSize(300.0,200.0));}
	//     inline CacAlertLayer* title(char const* t) {m_title->setString(t, true);return this;}
	//  protected:
	//     CCMenuItemSpriteExtra* m_closeButton;
	//     CCLabelBMFont* m_title;
	// };

	// class CacTextContainer : public CCNode {
	//  public:
	//     static CacTextContainer* create(CCSize const& size, TextInputDelegate* delegate, char const* font);
	//     bool init(CCSize size, float fontSize, TextInputDelegate* delegate, char const* font);
	//     inline CCTextInputNode* textInputNode() {return m_textInputNode;}
	//     inline CacTextContainer* placeholder(char const* pholder) {m_textInputNode->m_placeholderLabel->setString(pholder);return this;}
	//     inline CacTextContainer* allowedChars(char const* filter) {m_textInputNode->setAllowedChars(std::string(filter));return this;}
	//     inline CacTextContainer* charLimit(int limit) {m_textInputNode->m_maxLabelLength = limit;return this;}
	//     inline CacTextContainer* text(char const* text) {m_textInputNode->setString(text);return this;}
	//     inline std::string text() {return m_textInputNode->getString();}
	//  protected:
	//     extension::CCScale9Sprite* m_box;
	//     CCTextInputNode* m_textInputNode;

	// };
}  // namespace Cacao
