#ifndef __OBJECTDECODER_H__
#define __OBJECTDECODER_H__

#include "../../include/cocos2d.h"

// @note RobTop Addition
/*
// TODO: uncomment just before 3.3.1 releases, it requires a change in bindings/codegen as well
class CC_DLL ObjectDecoderDelegate {
public:
	virtual cocos2d::CCObject* getDecodedObject(int, DS_Dictionary*) { return nullptr; }
};
*/

// @note RobTop Addition
class CC_DLL ObjectDecoder : public cocos2d::CCNode {
public:
	ObjectDecoder();
	~ObjectDecoder() {}
	GEODE_CUSTOM_CONSTRUCTOR_BEGIN(ObjectDecoder)

	static ObjectDecoder* sharedDecoder();

	cocos2d::CCObject* getDecodedObject(int, DS_Dictionary*);

	virtual bool init();

public:
	ObjectDecoderDelegate* m_delegate;
};

#endif
