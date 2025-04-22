#ifndef __OBJECTDECODER_H__
#define __OBJECTDECODER_H__

#include "../../include/cocos2d.h"

// @note RobTop Addition
class CC_DLL ObjectDecoderDelegate {
public:
	virtual cocos2d::CCObject* getDecodedObject(int objectType, DS_Dictionary* data) { return nullptr; }
};

// @note RobTop Addition
class CC_DLL ObjectDecoder : public cocos2d::CCNode {
public:
	ObjectDecoder() : m_delegate(nullptr) {}
	~ObjectDecoder() {}
	GEODE_CUSTOM_CONSTRUCTOR_COCOS(ObjectDecoder, cocos2d::CCNode);

	static ObjectDecoder* sharedDecoder();

	cocos2d::CCObject* getDecodedObject(int, DS_Dictionary*);

	virtual bool init();

public:
	ObjectDecoderDelegate* m_delegate;
};

#endif
