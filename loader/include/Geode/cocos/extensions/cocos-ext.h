#ifndef __COCOS2D_EXT_H__
#define __COCOS2D_EXT_H__

#include "ExtensionMacros.h"
#include "../include/ccMacros.h"

#include "GUI/CCControlExtension/CCControlExtensions.h"
#include "GUI/CCScrollView/CCScrollView.h"
#include "GUI/CCScrollView/CCTableView.h"
#include "GUI/CCEditBox/CCEditBox.h"

#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"


// Physics integration
#if CC_ENABLE_CHIPMUNK_INTEGRATION || CC_ENABLE_BOX2D_INTEGRATION
#include "physics_nodes/CCPhysicsDebugNode.h"
#include "physics_nodes/CCPhysicsSprite.h"
#endif

#ifndef RT_ADD
#include "spine/spine-cocos2dx.h"
#endif

#endif /* __COCOS2D_EXT_H__ */
