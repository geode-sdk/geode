#ifndef __OBJECTDECODER_H__
#define __OBJECTDECODER_H__

#include "../../include/cocos2d.h"

// @note RobTop Addition
class CC_DLL ObjectDecoder : public cocos2d::CCNode {
public:
	ObjectDecoder();
	~ObjectDecoder() {}
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(ObjectDecoder)

	static ObjectDecoder* sharedDecoder();

	cocos2d::CCObject* getDecodedObject(int, DS_Dictionary*);

	virtual bool init();
};

#endif
