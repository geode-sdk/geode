#ifndef __COCOS2D_EXT_H__
#define __COCOS2D_EXT_H__

#include "extensions/ExtensionMacros.h"
#include "include/ccMacros.h"

#include "extensions/GUI/CCControlExtension/CCControlExtensions.h"
#include "extensions/GUI/CCScrollView/CCScrollView.h"
#include "extensions/GUI/CCScrollView/CCTableView.h"
#include "extensions/GUI/CCEditBox/CCEditBox.h"

#include "extensions/network/HttpRequest.h"
#include "extensions/network/HttpResponse.h"
#include "extensions/network/HttpClient.h"


// Physics integration
#if CC_ENABLE_CHIPMUNK_INTEGRATION || CC_ENABLE_BOX2D_INTEGRATION
#include "extensions/physics_nodes/CCPhysicsDebugNode.h"
#include "extensions/physics_nodes/CCPhysicsSprite.h"
#endif

#endif /* __COCOS2D_EXT_H__ */
