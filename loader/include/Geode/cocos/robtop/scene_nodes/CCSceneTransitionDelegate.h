#ifndef __CCSCENE_TRANSITION_DELEGATE_H__
#define __CCSCENE_TRANSITION_DELEGATE_H__

#include "../../include/ccMacros.h"

RT_ADD(
	NS_CC_BEGIN

	class CC_DLL CCSceneTransitionDelegate {
	public:
		virtual void sceneWillResume(void) {}
	};

	NS_CC_END
)


#endif //__CCSCENE_TRANSITION_DELEGATE_H__