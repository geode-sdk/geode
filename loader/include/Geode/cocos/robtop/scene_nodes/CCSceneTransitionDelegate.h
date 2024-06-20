#ifndef __CCSCENE_TRANSITION_DELEGATE_H__
#define __CCSCENE_TRANSITION_DELEGATE_H__

#include "../../include/ccMacros.h"

NS_CC_BEGIN

// @note RobTop Addition
class CC_DLL CCSceneTransitionDelegate {
public:
	virtual void sceneWillResume(void) {}
};

NS_CC_END


#endif //__CCSCENE_TRANSITION_DELEGATE_H__