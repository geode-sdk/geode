#ifndef __CCSCENE_TRANSITION_DELEGATE_H__
#define __CCSCENE_TRANSITION_DELEGATE_H__

#include "../../include/ccMacros.h"

RT_ADD(
	NS_CC_BEGIN

	class CC_DLL CCSceneTransitionDelegate
	{
	public:
		CCSceneTransitionDelegate(const CCSceneTransitionDelegate&);
		CCSceneTransitionDelegate();

		CCSceneTransitionDelegate& operator=(const CCSceneTransitionDelegate&);

		virtual void sceneWillResume(void);
	};

	NS_CC_END
)


#endif //__CCSCENE_TRANSITION_DELEGATE_H__