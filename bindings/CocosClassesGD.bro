
class cocos2d::CCAccelAmplitude : cocos2d::CCActionInterval {
	auto create(cocos2d::CCAction*, float);
	auto initWithAction(cocos2d::CCAction*, float);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCAccelDeccelAmplitude : cocos2d::CCActionInterval {
	auto create(cocos2d::CCAction*, float);
	auto initWithAction(cocos2d::CCAction*, float);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCAccelerometerDelegate {
	auto didAccelerate(cocos2d::CCAcceleration*);
}
class cocos2d::CCAction : cocos2d::CCObject {
	auto CCAction();
	auto copyWithZone(cocos2d::CCZone*);
	auto create();
	auto description();
	auto isDone();
	auto setTag(int);
	auto startWithTarget(cocos2d::CCNode*);
	auto step(float);
	auto stop();
	auto update(float);
}
class cocos2d::CCActionCamera : cocos2d::CCActionInterval {
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
}
class cocos2d::CCActionEase : cocos2d::CCActionInterval {
	auto CCActionEase();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto getInnerAction();
	auto initWithAction(cocos2d::CCActionInterval*);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto stop();
	auto update(float);
}
class cocos2d::CCActionInstant : cocos2d::CCFiniteTimeAction {
	auto CCActionInstant();
	auto copyWithZone(cocos2d::CCZone*);
	auto isDone();
	auto reverse();
	auto step(float);
	auto update(float);
}
class cocos2d::CCActionInterval : cocos2d::CCFiniteTimeAction {
	auto CCActionInterval();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float);
	auto getAmplitudeRate();
	auto initWithDuration(float);
	auto isDone();
	auto reverse();
	auto setAmplitudeRate(float);
	auto startWithTarget(cocos2d::CCNode*);
	auto step(float);
}
class cocos2d::CCActionManager : cocos2d::CCObject {
	auto CCActionManager();
	auto actionAllocWithHashElement(cocos2d::_hashElement*);
	auto addAction(cocos2d::CCAction*, cocos2d::CCNode*, bool);
	auto deleteHashElement(cocos2d::_hashElement*);
	auto getActionByTag(unsigned int, cocos2d::CCObject*);
	auto numberOfRunningActionsInTarget(cocos2d::CCObject*);
	auto pauseAllRunningActions();
	auto pauseTarget(cocos2d::CCObject*);
	auto removeAction(cocos2d::CCAction*);
	auto removeActionAtIndex(unsigned int, cocos2d::_hashElement*);
	auto removeActionByTag(unsigned int, cocos2d::CCObject*);
	auto removeAllActions();
	auto removeAllActionsFromTarget(cocos2d::CCObject*);
	auto resumeTarget(cocos2d::CCObject*);
	auto resumeTargets(cocos2d::CCSet*);
	auto update(float);
}
class cocos2d::CCActionTween : cocos2d::CCActionInterval {
	auto create(float, char const*, float, float);
	auto initWithDuration(float, char const*, float, float);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCActionTweenDelegate {
}
class cocos2d::CCAnimate : cocos2d::CCActionInterval {
	auto CCAnimate();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCAnimation*);
	auto initWithAnimation(cocos2d::CCAnimation*);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto stop();
	auto update(float);
}
class cocos2d::CCAnimation : cocos2d::CCObject {
	auto CCAnimation();
	auto addSpriteFrame(cocos2d::CCSpriteFrame*);
	auto addSpriteFrameWithFileName(char const*);
	auto addSpriteFrameWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCArray*, float, unsigned int);
	auto create();
	auto createWithSpriteFrames(cocos2d::CCArray*, float);
	auto getDuration();
	auto init();
	auto initWithAnimationFrames(cocos2d::CCArray*, float, unsigned int);
	auto initWithSpriteFrames(cocos2d::CCArray*, float);
	auto setFrames(cocos2d::CCArray*);
}
class cocos2d::CCAnimationCache : cocos2d::CCObject {
	auto CCAnimationCache();
	auto addAnimation(cocos2d::CCAnimation*, char const*);
	auto addAnimationsWithDictionary(cocos2d::CCDictionary*, char const*);
	auto addAnimationsWithFile(char const*);
	auto animationByName(char const*);
	auto init();
	auto parseVersion1(cocos2d::CCDictionary*);
	auto parseVersion2(cocos2d::CCDictionary*);
	auto purgeSharedAnimationCache();
	auto removeAnimationByName(char const*);
	auto sharedAnimationCache();
}
class cocos2d::CCAnimationFrame : cocos2d::CCObject {
	auto CCAnimationFrame();
	auto copyWithZone(cocos2d::CCZone*);
	auto initWithSpriteFrame(cocos2d::CCSpriteFrame*, float, cocos2d::CCDictionary*);
}
class cocos2d::CCApplication : cocos2d::CCApplicationProtocol {
	auto CCApplication();
	auto getCurrentLanguage();
	auto getTargetPlatform();
	auto openURL(char const*);
	auto run();
	auto setAnimationInterval(double);
	auto sharedApplication();
}
class cocos2d::CCApplicationProtocol {
	auto applicationWillBecomeActive();
	auto applicationWillResignActive();
	auto gameDidSave();
	auto openURL(char const*);
	auto trySaveGame();
}
class cocos2d::CCArray : cocos2d::CCObject {
	auto CCArray(unsigned int);
	auto CCArray();
	auto acceptVisitor(cocos2d::CCDataVisitor&);
	auto addObject(cocos2d::CCObject*);
	auto addObjectNew(cocos2d::CCObject*);
	auto addObjectsFromArray(cocos2d::CCArray*);
	auto capacity();
	auto containsObject(cocos2d::CCObject*);
	auto copyWithZone(cocos2d::CCZone*);
	auto count();
	// auto create(cocos2d::CCObject*, ...);
	auto create();
	auto createWithArray(cocos2d::CCArray*);
	auto createWithCapacity(unsigned int);
	auto createWithContentsOfFile(char const*);
	auto createWithContentsOfFileThreadSafe(char const*);
	auto createWithObject(cocos2d::CCObject*);
	auto exchangeObject(cocos2d::CCObject*, cocos2d::CCObject*);
	auto exchangeObjectAtIndex(unsigned int, unsigned int);
	auto fastRemoveObject(cocos2d::CCObject*);
	auto fastRemoveObjectAtIndex(unsigned int);
	auto fastRemoveObjectAtIndexNew(unsigned int);
	auto indexOfObject(cocos2d::CCObject*);
	auto init();
	auto initWithArray(cocos2d::CCArray*);
	auto initWithCapacity(unsigned int);
	auto initWithObject(cocos2d::CCObject*);
	// auto initWithObjects(cocos2d::CCObject*, ...);
	auto insertObject(cocos2d::CCObject*, unsigned int);
	auto isEqualToArray(cocos2d::CCArray*);
	auto lastObject();
	auto objectAtIndex(unsigned int);
	auto randomObject();
	auto reduceMemoryFootprint();
	auto removeAllObjects();
	auto removeLastObject(bool);
	auto removeObject(cocos2d::CCObject*, bool);
	auto removeObjectAtIndex(unsigned int, bool);
	auto removeObjectsInArray(cocos2d::CCArray*);
	auto replaceObjectAtIndex(unsigned int, cocos2d::CCObject*, bool);
	auto reverseObjects();
	auto stringAtIndex(unsigned int);
}
class cocos2d::CCAtlasNode : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	auto CCAtlasNode();
	auto calculateMaxItems();
	auto create(char const*, unsigned int, unsigned int, unsigned int);
	auto draw();
	auto getBlendFunc();
	auto getColor();
	auto getQuadsToDraw();
	auto getTexture();
	auto getTextureAtlas();
	auto initWithTexture(cocos2d::CCTexture2D*, unsigned int, unsigned int, unsigned int);
	auto initWithTileFile(char const*, unsigned int, unsigned int, unsigned int);
	auto isOpacityModifyRGB();
	auto setBlendFunc(cocos2d::_ccBlendFunc);
	auto setColor(cocos2d::_ccColor3B const&);
	auto setIgnoreContentScaleFactor(bool);
	auto setOpacity(unsigned char);
	auto setOpacityModifyRGB(bool);
	auto setQuadsToDraw(unsigned int);
	auto setTexture(cocos2d::CCTexture2D*);
	auto setTextureAtlas(cocos2d::CCTextureAtlas*);
	auto updateAtlasValues();
	auto updateBlendFunc();
	auto updateOpacityModifyRGB();
}
class cocos2d::CCAutoreleasePool : cocos2d::CCObject {
	auto CCAutoreleasePool();
	auto addObject(cocos2d::CCObject*);
	auto clear();
	auto removeObject(cocos2d::CCObject*);
}
class cocos2d::CCBezierBy : cocos2d::CCActionInterval {
	auto CCBezierBy();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::_ccBezierConfig const&);
	auto initWithDuration(float, cocos2d::_ccBezierConfig const&);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCBezierTo : cocos2d::CCBezierBy {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::_ccBezierConfig const&);
	auto initWithDuration(float, cocos2d::_ccBezierConfig const&);
	auto startWithTarget(cocos2d::CCNode*);
}
class cocos2d::CCBlendProtocol {
}
class cocos2d::CCBlink : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, unsigned int);
	auto initWithDuration(float, unsigned int);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto stop();
	auto update(float);
}
class cocos2d::CCBMFontConfiguration : cocos2d::CCObject {
	auto CCBMFontConfiguration();
	auto create(char const*);
	auto description();
	auto getCharacterSet();
	auto initWithFNTfile(char const*);
	auto parseCharacterDefinition(gd::string, cocos2d::_BMFontDef*);
	auto parseCommonArguments(gd::string);
	auto parseConfigFile(char const*);
	auto parseImageFileName(gd::string, char const*);
	auto parseInfoArguments(gd::string);
	auto parseKerningEntry(gd::string);
	auto purgeFontDefDictionary();
	auto purgeKerningDictionary();
}
class cocos2d::CCBool : cocos2d::CCObject {
	auto acceptVisitor(cocos2d::CCDataVisitor&);
	auto create(bool);
}
class cocos2d::CCCallFunc : cocos2d::CCActionInstant {
	auto CCCallFunc();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(int);
	auto create(cocos2d::CCObject*, cocos2d::SEL_CallFunc);
	auto execute();
	auto initWithTarget(cocos2d::CCObject*);
	auto update(float);
}
class cocos2d::CCCallFuncN : cocos2d::CCCallFunc, cocos2d::TypeInfo {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(int);
	auto create(cocos2d::CCObject*, cocos2d::SEL_CallFuncN);
	auto execute();
	auto getClassTypeInfo();
	auto getClassTypeInfo();
	auto initWithTarget(cocos2d::CCObject*, cocos2d::SEL_CallFuncN);
}
class cocos2d::CCCallFuncND : cocos2d::CCCallFuncN {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCObject*, cocos2d::SEL_CallFuncND, void*);
	auto execute();
	auto getClassTypeInfo();
	auto getClassTypeInfo();
	auto initWithTarget(cocos2d::CCObject*, cocos2d::SEL_CallFuncND, void*);
}
class cocos2d::CCCallFuncO : cocos2d::CCCallFunc, cocos2d::TypeInfo {
	auto CCCallFuncO();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCObject*);
	auto execute();
	auto getClassTypeInfo();
	auto getClassTypeInfo();
	auto initWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCObject*);
}
class cocos2d::CCCamera : cocos2d::CCObject {
	auto CCCamera();
	auto description();
	auto getCenterXYZ(float*, float*, float*);
	auto getEyeXYZ(float*, float*, float*);
	auto getUpXYZ(float*, float*, float*);
	auto getZEye();
	auto init();
	auto locate();
	auto restore();
	auto setCenterXYZ(float, float, float);
	auto setEyeXYZ(float, float, float);
	auto setUpXYZ(float, float, float);
}
class cocos2d::CCCardinalSplineBy : cocos2d::CCCardinalSplineTo {
	auto CCCardinalSplineBy();
	auto create(float, cocos2d::CCPointArray*, float);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto updatePosition(cocos2d::CCPoint&);
}
class cocos2d::CCCardinalSplineTo : cocos2d::CCActionInterval {
	auto CCCardinalSplineTo();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCPointArray*, float);
	auto initWithDuration(float, cocos2d::CCPointArray*, float);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
	auto updatePosition(cocos2d::CCPoint&);
}
class cocos2d::CCCatmullRomBy : cocos2d::CCCardinalSplineBy {
	auto create(float, cocos2d::CCPointArray*);
	auto initWithDuration(float, cocos2d::CCPointArray*);
}
class cocos2d::CCCatmullRomTo : cocos2d::CCCardinalSplineTo {
	auto create(float, cocos2d::CCPointArray*);
	auto initWithDuration(float, cocos2d::CCPointArray*);
}
class cocos2d::CCClippingNode : cocos2d::CCNode {
	auto CCClippingNode();
	auto create(cocos2d::CCNode*);
	auto create();
	auto getAlphaThreshold();
	auto getStencil();
	auto init(cocos2d::CCNode*);
	auto init();
	auto isInverted();
	auto onEnter();
	auto onEnterTransitionDidFinish();
	auto onExit();
	auto onExitTransitionDidStart();
	auto setAlphaThreshold(float);
	auto setInverted(bool);
	auto setStencil(cocos2d::CCNode*);
	auto visit();
}
class cocos2d::CCComponent : cocos2d::CCObject {
	auto CCComponent();
	auto create();
	auto getName();
	auto getOwner();
	auto init();
	auto isEnabled();
	auto onEnter();
	auto onExit();
	auto serialize(void*);
	auto setEnabled(bool);
	auto setName(char const*);
	auto setOwner(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCComponentContainer {
	auto CCComponentContainer(cocos2d::CCNode*);
	auto add(cocos2d::CCComponent*);
	auto alloc();
	auto get(char const*);
	auto isEmpty();
	auto remove(char const*);
	auto remove(cocos2d::CCComponent*);
	auto removeAll();
	auto visit(float);
}
class cocos2d::CCConfiguration : cocos2d::CCObject {
	auto CCConfiguration();
	auto checkForGLExtension(gd::string const&);
	auto dumpInfo();
	auto gatherGPUInfo();
	auto getBool(char const*, bool);
	auto getCString(char const*, char const*);
	auto getMaxModelviewStackDepth();
	auto getMaxTextureSize();
	auto getMaxTextureUnits();
	auto getNumber(char const*, double);
	auto getObject(char const*);
	auto init();
	auto loadConfigFile(char const*);
	auto purgeConfiguration();
	auto setObject(char const*, cocos2d::CCObject*);
	auto sharedConfiguration();
	auto supportsBGRA8888();
	auto supportsDiscardFramebuffer();
	auto supportsNPOT();
	auto supportsPVRTC();
	auto supportsShareableVAO();
}
class cocos2d::CCCopying {
	auto copyWithZone(cocos2d::CCZone*);
}
class cocos2d::CCDataVisitor {
	auto visit(cocos2d::CCDictionary const*);
	auto visit(cocos2d::CCSet const*);
	auto visit(cocos2d::CCBool const*);
	auto visit(cocos2d::CCArray const*);
	auto visit(cocos2d::CCFloat const*);
	auto visit(cocos2d::CCDouble const*);
	auto visit(cocos2d::CCString const*);
	auto visit(cocos2d::CCInteger const*);
}
class cocos2d::CCDeccelAmplitude : cocos2d::CCActionInterval {
	auto create(cocos2d::CCAction*, float);
	auto initWithAction(cocos2d::CCAction*, float);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCDelayTime : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float);
	auto reverse();
	auto update(float);
}
class cocos2d::CCDictionary : cocos2d::CCObject {
	auto CCDictionary();
	auto acceptVisitor(cocos2d::CCDataVisitor&);
	auto allKeys();
	auto allKeysForObject(cocos2d::CCObject*);
	auto charForKey(gd::string const&);
	auto copyWithZone(cocos2d::CCZone*);
	auto count();
	auto create();
	auto createWithContentsOfFile(char const*);
	auto createWithContentsOfFileThreadSafe(char const*);
	auto createWithDictionary(cocos2d::CCDictionary*);
	auto getFirstKey();
	auto objectForKey(int);
	auto objectForKey(gd::string const&);
	auto randomObject();
	auto removeAllObjects();
	auto removeObjectForElememt(cocos2d::CCDictElement*);
	auto removeObjectForKey(int);
	auto removeObjectForKey(gd::string const&);
	auto removeObjectsForKeys(cocos2d::CCArray*);
	auto setObject(cocos2d::CCObject*, int);
	auto setObject(cocos2d::CCObject*, gd::string const&);
	auto setObjectUnSafe(cocos2d::CCObject*, int);
	auto setObjectUnSafe(cocos2d::CCObject*, gd::string const&);
	auto valueForKey(int);
	auto valueForKey(gd::string const&);
	auto writeToFile(char const*);
}
class cocos2d::CCDictMaker : cocos2d::CCSAXDelegator {
	auto CCDictMaker();
	auto endElement(void*, char const*);
	auto startElement(void*, char const*, char const**);
	auto textHandler(void*, char const*, int);
}
class cocos2d::CCDirector : cocos2d::CCObject, cocos2d::TypeInfo {
	auto CCDirector();
	auto calculateDeltaTime();
	auto calculateMPF();
	auto checkSceneReference();
	auto convertToGL(cocos2d::CCPoint const&);
	auto convertToUI(cocos2d::CCPoint const&);
	auto createStatsLabel();
	auto drawScene();
	auto end();
	auto getAccelerometer();
	auto getActionManager();
	auto getClassTypeInfo();
	auto getClassTypeInfo();
	auto getContentScaleFactor();
	auto getDelegate();
	auto getDeltaTime();
	auto getFPSImageData(unsigned char**, unsigned int*);
	auto getKeyboardDispatcher();
	auto getKeypadDispatcher();
	auto getMouseDispatcher();
	auto getNotificationNode();
	auto getSceneDelegate();
	auto getScheduler();
	auto getScreenBottom();
	auto getScreenLeft();
	auto getScreenRight();
	auto getScreenScaleFactor();
	auto getScreenScaleFactorH();
	auto getScreenScaleFactorMax();
	auto getScreenScaleFactorW();
	auto getScreenTop();
	auto getTouchDispatcher();
	auto getVisibleOrigin();
	auto getVisibleSize();
	auto getWinSize();
	auto getWinSizeInPixels();
	auto getZEye();
	auto init();
	auto levelForSceneInStack(cocos2d::CCScene*);
	auto pause();
	auto popScene();
	auto popSceneWithTransition(float, cocos2d::PopTransition);
	auto popToRootScene();
	auto popToSceneInStack(cocos2d::CCScene*);
	auto popToSceneStackLevel(int);
	auto purgeCachedData();
	auto purgeDirector();
	auto pushScene(cocos2d::CCScene*);
	auto removeStatsLabel();
	auto replaceScene(cocos2d::CCScene*);
	auto resetSmoothFixCounter();
	auto reshapeProjection(cocos2d::CCSize const&);
	auto resume();
	auto runWithScene(cocos2d::CCScene*);
	auto sceneCount();
	auto setAccelerometer(cocos2d::CCAccelerometer*);
	auto setActionManager(cocos2d::CCActionManager*);
	auto setAlphaBlending(bool);
	auto setContentScaleFactor(float);
	auto setDefaultValues();
	auto setDelegate(cocos2d::CCDirectorDelegate*);
	auto setDeltaTime(float);
	auto setDepthTest(bool);
	auto setGLDefaultValues();
	auto setKeyboardDispatcher(cocos2d::CCKeyboardDispatcher*);
	auto setKeypadDispatcher(cocos2d::CCKeypadDispatcher*);
	auto setMouseDispatcher(cocos2d::CCMouseDispatcher*);
	auto setNextDeltaTimeZero(bool);
	auto setNextScene();
	auto setNotificationNode(cocos2d::CCNode*);
	auto setOpenGLView(cocos2d::CCEGLView*);
	auto setProjection(cocos2d::ccDirectorProjection);
	auto setSceneDelegate(cocos2d::CCSceneDelegate*);
	auto setScheduler(cocos2d::CCScheduler*);
	auto setTouchDispatcher(cocos2d::CCTouchDispatcher*);
	auto setViewport();
	auto setupScreenScale(cocos2d::CCSize, cocos2d::CCSize, cocos2d::TextureQuality);
	auto sharedDirector();
	auto showStats();
	auto updateContentScale(cocos2d::TextureQuality);
	auto updateScreenScale(cocos2d::CCSize);
	auto willSwitchToScene(cocos2d::CCScene*);
}
class cocos2d::CCDisplayLinkDirector : cocos2d::CCDirector {
	auto mainLoop();
	auto setAnimationInterval(double);
	auto startAnimation();
	auto stopAnimation();
}
class cocos2d::CCDrawNode : cocos2d::CCNodeRGBA {
	auto CCDrawNode();
	auto clear();
	auto create();
	auto draw();
	auto drawDot(cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&);
	auto drawPolygon(cocos2d::CCPoint*, unsigned int, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&);
	auto drawSegment(cocos2d::CCPoint const&, cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&);
	auto ensureCapacity(unsigned int);
	auto getBlendFunc();
	auto init();
	auto listenBackToForeground(cocos2d::CCObject*);
	auto render();
	auto setBlendFunc(cocos2d::_ccBlendFunc const&);
}
class cocos2d::CCEaseBackIn : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseBackInOut : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseBackOut : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseBounce : cocos2d::CCActionEase {
	auto bounceTime(float);
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
}
class cocos2d::CCEaseBounceIn : cocos2d::CCEaseBounce {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseBounceInOut : cocos2d::CCEaseBounce {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseBounceOut : cocos2d::CCEaseBounce {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseElastic : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto create(cocos2d::CCActionInterval*, float);
	auto initWithAction(cocos2d::CCActionInterval*, float);
	auto reverse();
}
class cocos2d::CCEaseElasticIn : cocos2d::CCEaseElastic {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto create(cocos2d::CCActionInterval*, float);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseElasticInOut : cocos2d::CCEaseElastic {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto create(cocos2d::CCActionInterval*, float);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseElasticOut : cocos2d::CCEaseElastic {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto create(cocos2d::CCActionInterval*, float);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseExponentialIn : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseExponentialInOut : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseExponentialOut : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseIn : cocos2d::CCEaseRateAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*, float);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseInOut : cocos2d::CCEaseRateAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*, float);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseOut : cocos2d::CCEaseRateAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*, float);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseRateAction : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*, float);
	auto initWithAction(cocos2d::CCActionInterval*, float);
	auto reverse();
}
class cocos2d::CCEaseSineIn : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseSineInOut : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEaseSineOut : cocos2d::CCActionEase {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto reverse();
	auto update(float);
}
class cocos2d::CCEGLView : cocos2d::CCEGLViewProtocol {
	auto CCEGLView();
	auto end();
	auto isOpenGLReady();
	auto setIMEKeyboardState(bool);
	auto sharedOpenGLView();
	auto swapBuffers();
}
class cocos2d::CCEGLViewProtocol {
	auto CCEGLViewProtocol();
	auto getDesignResolutionSize();
	auto getFrameSize();
	auto getScaleX();
	auto getScaleY();
	auto getScissorRect();
	auto getSetOfTouchesEndOrCancel(cocos2d::CCSet&, int, int*, float*, float*);
	auto getViewName();
	auto getViewPortRect();
	auto getVisibleOrigin();
	auto getVisibleSize();
	auto handleTouchesBegin(int, int*, float*, float*);
	auto handleTouchesCancel(int, int*, float*, float*);
	auto handleTouchesEnd(int, int*, float*, float*);
	auto handleTouchesMove(int, int*, float*, float*);
	auto isScissorEnabled();
	auto pollInputEvents();
	auto setDesignResolutionSize(float, float, ResolutionPolicy);
	auto setFrameSize(float, float);
	auto setScissorInPoints(float, float, float, float);
	auto setTouchDelegate(cocos2d::EGLTouchDelegate*);
	auto setViewName(char const*);
	auto setViewPortInPoints(float, float, float, float);
	auto updateDesignResolutionSize();
}
class cocos2d::CCFadeIn : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float);
	auto reverse();
	auto update(float);
}
class cocos2d::CCFadeOut : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float);
	auto reverse();
	auto update(float);
}
class cocos2d::CCFadeOutBLTiles : cocos2d::CCFadeOutTRTiles {
	auto create(float, cocos2d::CCSize const&);
	auto testFunc(cocos2d::CCSize const&, float);
}
class cocos2d::CCFadeOutDownTiles : cocos2d::CCFadeOutUpTiles {
	auto create(float, cocos2d::CCSize const&);
	auto testFunc(cocos2d::CCSize const&, float);
}
class cocos2d::CCFadeOutTRTiles : cocos2d::CCTiledGrid3DAction {
	auto create(float, cocos2d::CCSize const&);
	auto testFunc(cocos2d::CCSize const&, float);
	auto transformTile(cocos2d::CCPoint const&, float);
	auto turnOffTile(cocos2d::CCPoint const&);
	auto turnOnTile(cocos2d::CCPoint const&);
	auto update(float);
}
class cocos2d::CCFadeOutUpTiles : cocos2d::CCFadeOutTRTiles {
	auto create(float, cocos2d::CCSize const&);
	auto testFunc(cocos2d::CCSize const&, float);
	auto transformTile(cocos2d::CCPoint const&, float);
}
class cocos2d::CCFadeTo : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, unsigned char);
	auto initWithDuration(float, unsigned char);
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCFileUtils : cocos2d::TypeInfo {
	auto CCFileUtils();
	auto addSearchPath(char const*);
	auto addSearchResolutionsOrder(char const*);
	auto addSuffix(gd::string, gd::string);
	auto createCCArrayWithContentsOfFile(gd::string const&);
	auto createCCDictionaryWithContentsOfFile(gd::string const&);
	auto fullPathForFilename(char const*, bool);
	auto fullPathFromRelativeFile(char const*, char const*);
	auto getClassTypeInfo();
	auto getClassTypeInfo();
	auto getFileData(char const*, char const*, unsigned long*);
	auto getFileDataFromZip(char const*, char const*, unsigned long*);
	auto getFullPathForDirectoryAndFilename(gd::string const&, gd::string const&);
	auto getNewFilename(char const*);
	auto getPathForFilename(gd::string const&, gd::string const&, gd::string const&);
	auto getSearchPaths();
	auto getSearchResolutionsOrder();
	auto getWritablePath2();
	auto init();
	auto isAbsolutePath(gd::string const&);
	auto isPopupNotify();
	auto loadFilenameLookupDictionaryFromFile(char const*);
	auto purgeCachedEntries();
	auto purgeFileUtils();
	auto removeAllPaths();
	auto removeFullPath(char const*);
	auto removeSearchPath(char const*);
	auto setFilenameLookupDictionary(cocos2d::CCDictionary*);
	auto setPopupNotify(bool);
	auto setSearchPaths(gd::vector<gd::string> const&);
	auto setSearchResolutionsOrder(gd::vector<gd::string> const&);
	auto sharedFileUtils();
	auto shouldUseHD();
	auto writeToFile(cocos2d::CCDictionary*, gd::string const&);
}
class cocos2d::CCFileUtilsAndroid : cocos2d::CCFileUtils {
	auto CCFileUtilsAndroid();
	auto doGetFileData(char const*, char const*, unsigned long*, bool);
	auto getFileData(char const*, char const*, unsigned long*);
	auto getFileDataForAsync(char const*, char const*, unsigned long*);
	auto getWritablePath();
	auto init();
	auto isAbsolutePath(gd::string const&);
	auto isFileExist(gd::string const&);
}
class cocos2d::CCFiniteTimeAction : cocos2d::CCAction {
	auto reverse();
}
class cocos2d::CCFlipX : cocos2d::CCActionInstant {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(bool);
	auto initWithFlipX(bool);
	auto reverse();
	auto update(float);
}
class cocos2d::CCFlipX3D : cocos2d::CCGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float);
	auto initWithDuration(float);
	auto initWithSize(cocos2d::CCSize const&, float);
	auto update(float);
}
class cocos2d::CCFlipY : cocos2d::CCActionInstant {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(bool);
	auto initWithFlipY(bool);
	auto reverse();
	auto update(float);
}
class cocos2d::CCFlipY3D : cocos2d::CCFlipX3D {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float);
	auto update(float);
}
class cocos2d::CCFollow : cocos2d::CCAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCNode*, cocos2d::CCRect const&);
	auto initWithTarget(cocos2d::CCNode*, cocos2d::CCRect const&);
	auto isDone();
	auto step(float);
	auto stop();
}
class cocos2d::CCGLProgram : cocos2d::CCObject {
	auto CCGLProgram();
	auto addAttribute(char const*, unsigned int);
	auto compileShader(unsigned int*, unsigned int, char const*);
	auto description();
	auto fragmentShaderLog();
	auto getUniformLocationForName(char const*);
	auto initWithVertexShaderByteArray(char const*, char const*);
	auto initWithVertexShaderFilename(char const*, char const*);
	auto link();
	auto programLog();
	auto reset();
	auto setUniformLocationWith1f(int, float);
	auto setUniformLocationWith1i(int, int);
	auto setUniformLocationWith2f(int, float, float);
	auto setUniformLocationWith2fv(int, float*, unsigned int);
	auto setUniformLocationWith2i(int, int, int);
	auto setUniformLocationWith2iv(int, int*, unsigned int);
	auto setUniformLocationWith3f(int, float, float, float);
	auto setUniformLocationWith3fv(int, float*, unsigned int);
	auto setUniformLocationWith3i(int, int, int, int);
	auto setUniformLocationWith3iv(int, int*, unsigned int);
	auto setUniformLocationWith4f(int, float, float, float, float);
	auto setUniformLocationWith4fv(int, float*, unsigned int);
	auto setUniformLocationWith4i(int, int, int, int, int);
	auto setUniformLocationWith4iv(int, int*, unsigned int);
	auto setUniformLocationWithMatrix3fv(int, float*, unsigned int);
	auto setUniformLocationWithMatrix4fv(int, float*, unsigned int);
	auto setUniformsForBuiltins();
	auto updateUniformLocation(int, void*, unsigned int);
	auto updateUniforms();
	auto use();
	auto vertexShaderLog();
}
class cocos2d::CCGrabber : cocos2d::CCObject {
	auto CCGrabber();
	auto afterRender(cocos2d::CCTexture2D*);
	auto beforeRender(cocos2d::CCTexture2D*);
	auto grab(cocos2d::CCTexture2D*);
}
class cocos2d::CCGrid3D : cocos2d::CCGridBase {
	auto CCGrid3D();
	auto blit();
	auto calculateVertexPoints();
	auto create(cocos2d::CCSize const&);
	auto create(cocos2d::CCSize const&, cocos2d::CCTexture2D*, bool);
	auto originalVertex(cocos2d::CCPoint const&);
	auto reuse();
	auto setVertex(cocos2d::CCPoint const&, cocos2d::_ccVertex3F const&);
	auto vertex(cocos2d::CCPoint const&);
}
class cocos2d::CCGrid3DAction : cocos2d::CCGridAction {
	auto CCGrid3DAction();
	auto getGrid();
	auto originalVertex(cocos2d::CCPoint const&);
	auto setVertex(cocos2d::CCPoint const&, cocos2d::_ccVertex3F const&);
	auto vertex(cocos2d::CCPoint const&);
}
class cocos2d::CCGridAction : cocos2d::CCActionInterval {
	auto CCGridAction();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&);
	auto getGrid();
	auto initWithDuration(float, cocos2d::CCSize const&);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
}
class cocos2d::CCGridBase : cocos2d::CCObject {
	auto CCGridBase();
	auto afterDraw(cocos2d::CCNode*);
	auto beforeDraw();
	auto blit();
	auto calculateVertexPoints();
	auto create(cocos2d::CCSize const&);
	auto create(cocos2d::CCSize const&, cocos2d::CCTexture2D*, bool);
	auto initWithSize(cocos2d::CCSize const&);
	auto initWithSize(cocos2d::CCSize const&, cocos2d::CCTexture2D*, bool);
	auto reuse();
	auto set2DProjection();
	auto setActive(bool);
	auto setTextureFlipped(bool);
}
class cocos2d::CCHide : cocos2d::CCActionInstant {
	auto copyWithZone(cocos2d::CCZone*);
	auto create();
	auto reverse();
	auto update(float);
}
class cocos2d::CCImage : cocos2d::CCObject {
	auto CCImage();
	auto _initWithJpgData(void*, int);
	auto _initWithPngData(void*, int);
	auto _initWithRawData(void*, int, int, int, int, bool);
	auto _initWithTiffData(void*, int);
	auto _initWithWebpData(void*, int);
	auto _saveImageToJPG(char const*);
	auto _saveImageToPNG(char const*, bool);
	auto initWithImageData(void*, int, cocos2d::CCImage::EImageFormat, int, int, int);
	auto initWithImageFile(char const*, cocos2d::CCImage::EImageFormat);
	auto initWithImageFileThreadSafe(char const*, cocos2d::CCImage::EImageFormat);
	auto initWithString(char const*, int, int, cocos2d::CCImage::ETextAlign, char const*, int);
	auto initWithStringShadowStroke(char const*, int, int, cocos2d::CCImage::ETextAlign, char const*, int, float, float, float, bool, float, float, float, float, bool, float, float, float, float);
	auto saveToFile(char const*, bool);
}
class cocos2d::CCIMEDelegate {
	auto CCIMEDelegate();
	auto attachWithIME();
	auto canAttachWithIME();
	auto canDetachWithIME();
	auto deleteBackward();
	auto detachWithIME();
	auto didAttachWithIME();
	auto didDetachWithIME();
	auto getContentText();
	auto insertText(char const*, int);
	auto keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	auto keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	auto keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	auto keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&);
}
class cocos2d::CCInteger : cocos2d::CCObject {
	auto acceptVisitor(cocos2d::CCDataVisitor&);
	auto create(int);
}
class cocos2d::CCJumpBy : cocos2d::CCActionInterval {
	auto CCJumpBy();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCPoint const&, float, unsigned int);
	auto initWithDuration(float, cocos2d::CCPoint const&, float, unsigned int);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCJumpTiles3D : cocos2d::CCTiledGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, unsigned int, float);
	auto initWithDuration(float, cocos2d::CCSize const&, unsigned int, float);
	auto update(float);
}
class cocos2d::CCJumpTo : cocos2d::CCJumpBy {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCPoint const&, float, int);
	auto startWithTarget(cocos2d::CCNode*);
}
class cocos2d::CCKeyboardDelegate {
	auto keyDown(cocos2d::enumKeyCodes);
	auto keyUp(cocos2d::enumKeyCodes);
}
class cocos2d::CCKeyboardDispatcher : cocos2d::CCObject {
	auto CCKeyboardDispatcher();
	auto addDelegate(cocos2d::CCKeyboardDelegate*);
	auto convertKeyCode(cocos2d::enumKeyCodes);
	auto dispatchKeyboardMSG(cocos2d::enumKeyCodes, bool);
	auto forceAddDelegate(cocos2d::CCKeyboardDelegate*);
	auto forceRemoveDelegate(cocos2d::CCKeyboardDelegate*);
	auto keyToString(cocos2d::enumKeyCodes);
	auto removeDelegate(cocos2d::CCKeyboardDelegate*);
	auto updateModifierKeys(bool, bool, bool, bool);
}
class cocos2d::CCKeyboardHandler : cocos2d::CCObject {
	auto getDelegate();
	auto handlerWithDelegate(cocos2d::CCKeyboardDelegate*);
	auto initWithDelegate(cocos2d::CCKeyboardDelegate*);
	auto setDelegate(cocos2d::CCKeyboardDelegate*);
}
class cocos2d::CCKeypadDelegate {
	auto keyBackClicked();
	auto keyMenuClicked();
}
class cocos2d::CCKeypadDispatcher : cocos2d::CCObject {
	auto CCKeypadDispatcher();
	auto addDelegate(cocos2d::CCKeypadDelegate*);
	auto dispatchKeypadMSG(cocos2d::ccKeypadMSGType);
	auto forceAddDelegate(cocos2d::CCKeypadDelegate*);
	auto forceRemoveDelegate(cocos2d::CCKeypadDelegate*);
	auto removeDelegate(cocos2d::CCKeypadDelegate*);
}
class cocos2d::CCKeypadHandler : cocos2d::CCObject {
	auto getDelegate();
	auto handlerWithDelegate(cocos2d::CCKeypadDelegate*);
	auto initWithDelegate(cocos2d::CCKeypadDelegate*);
	auto setDelegate(cocos2d::CCKeypadDelegate*);
}
class cocos2d::CCLabelAtlas : cocos2d::CCAtlasNode, cocos2d::CCLabelProtocol {
	auto create(char const*, char const*);
	auto create(char const*, char const*, unsigned int, unsigned int, unsigned int);
	auto getString();
	auto initWithString(char const*, cocos2d::CCTexture2D*, unsigned int, unsigned int, unsigned int);
	auto initWithString(char const*, char const*);
	auto initWithString(char const*, char const*, unsigned int, unsigned int, unsigned int);
	auto setString(char const*);
	auto updateAtlasValues();
}
class cocos2d::CCLabelBMFont : cocos2d::CCSpriteBatchNode, cocos2d::CCLabelProtocol, cocos2d::CCRGBAProtocol {
	auto CCLabelBMFont();
	auto create(char const*, char const*);
	auto create(char const*, char const*, float);
	auto create(char const*, char const*, float, cocos2d::CCTextAlignment);
	auto create(char const*, char const*, float, cocos2d::CCTextAlignment, cocos2d::CCPoint);
	auto create();
	auto createBatched(char const*, char const*, cocos2d::CCArray*);
	auto createFontChars();
	auto getColor();
	auto getConfiguration();
	auto getDisplayedColor();
	auto getDisplayedOpacity();
	auto getFntFile();
	auto getLetterPosXLeft(cocos2d::CCSprite*);
	auto getLetterPosXRight(cocos2d::CCSprite*);
	auto getOpacity();
	auto getString();
	auto init();
	auto initWithString(char const*, char const*, float, cocos2d::CCTextAlignment, cocos2d::CCPoint);
	auto isCascadeColorEnabled();
	auto isCascadeOpacityEnabled();
	auto isOpacityModifyRGB();
	auto kerningAmountForFirst(unsigned short, unsigned short);
	auto limitLabelWidth(float, float, float);
	auto purgeCachedData();
	auto setAlignment(cocos2d::CCTextAlignment);
	auto setAnchorPoint(cocos2d::CCPoint const&);
	auto setCString(char const*);
	auto setCascadeColorEnabled(bool);
	auto setCascadeOpacityEnabled(bool);
	auto setColor(cocos2d::_ccColor3B const&);
	auto setFntFile(char const*);
	auto setLineBreakWithoutSpace(bool);
	auto setOpacity(unsigned char);
	auto setOpacityModifyRGB(bool);
	auto setScale(float);
	auto setScaleX(float);
	auto setScaleY(float);
	auto setString(char const*);
	auto setString(char const*, bool);
	auto setString(unsigned short*, bool);
	auto setWidth(float);
	auto updateDisplayedColor(cocos2d::_ccColor3B const&);
	auto updateDisplayedOpacity(unsigned char);
	auto updateLabel();
}
class cocos2d::CCLabelProtocol {
}
class cocos2d::CCLabelTTF : cocos2d::CCSprite, cocos2d::CCLabelProtocol {
	auto CCLabelTTF();
	auto _prepareTextDefinition(bool);
	auto _updateWithTextDefinition(cocos2d::_ccFontDefinition&, bool);
	auto create(char const*, char const*, float);
	auto create(char const*, char const*, float, cocos2d::CCSize const&, cocos2d::CCTextAlignment);
	auto create(char const*, char const*, float, cocos2d::CCSize const&, cocos2d::CCTextAlignment, cocos2d::CCVerticalTextAlignment);
	auto create();
	auto createWithFontDefinition(char const*, cocos2d::_ccFontDefinition&);
	auto description();
	auto disableShadow(bool);
	auto disableStroke(bool);
	auto enableShadow(cocos2d::CCSize const&, float, float, bool);
	auto enableStroke(cocos2d::_ccColor3B const&, float, bool);
	auto getDimensions();
	auto getFontName();
	auto getFontSize();
	auto getHorizontalAlignment();
	auto getString();
	auto getTextDefinition();
	auto getVerticalAlignment();
	auto init();
	auto initWithString(char const*, char const*, float);
	auto initWithString(char const*, char const*, float, cocos2d::CCSize const&, cocos2d::CCTextAlignment);
	auto initWithString(char const*, char const*, float, cocos2d::CCSize const&, cocos2d::CCTextAlignment, cocos2d::CCVerticalTextAlignment);
	auto initWithStringAndTextDefinition(char const*, cocos2d::_ccFontDefinition&);
	auto setDimensions(cocos2d::CCSize const&);
	auto setFontFillColor(cocos2d::_ccColor3B const&, bool);
	auto setFontName(char const*);
	auto setFontSize(float);
	auto setHorizontalAlignment(cocos2d::CCTextAlignment);
	auto setString(char const*);
	auto setTextDefinition(cocos2d::_ccFontDefinition*);
	auto setVerticalAlignment(cocos2d::CCVerticalTextAlignment);
	auto updateTexture();
}
class cocos2d::CCLayer : cocos2d::CCNode, cocos2d::CCTouchDelegate, cocos2d::CCAccelerometerDelegate, cocos2d::CCKeypadDelegate, cocos2d::CCKeyboardDelegate, cocos2d::CCMouseDelegate {
	auto CCLayer();
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*);
	auto ccTouchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*);
	auto ccTouchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*);
	auto ccTouchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*);
	auto create();
	auto didAccelerate(cocos2d::CCAcceleration*);
	auto excuteScriptTouchHandler(int, cocos2d::CCSet*);
	auto excuteScriptTouchHandler(int, cocos2d::CCTouch*);
	auto getTouchMode();
	auto getTouchPriority();
	auto init();
	auto isAccelerometerEnabled();
	auto isKeyboardEnabled();
	auto isKeypadEnabled();
	auto isMouseEnabled();
	auto isTouchEnabled();
	auto keyBackClicked();
	auto keyDown(cocos2d::enumKeyCodes);
	auto keyMenuClicked();
	auto onEnter();
	auto onEnterTransitionDidFinish();
	auto onExit();
	auto registerScriptAccelerateHandler(int);
	auto registerScriptKeypadHandler(int);
	auto registerScriptTouchHandler(int, bool, int, bool);
	auto registerWithTouchDispatcher();
	auto setAccelerometerEnabled(bool);
	auto setAccelerometerInterval(double);
	auto setKeyboardEnabled(bool);
	auto setKeypadEnabled(bool);
	auto setMouseEnabled(bool);
	auto setTouchEnabled(bool);
	auto setTouchMode(cocos2d::ccTouchesMode);
	auto setTouchPriority(int);
	auto unregisterScriptAccelerateHandler();
	auto unregisterScriptKeypadHandler();
	auto unregisterScriptTouchHandler();
}
class cocos2d::CCLayerColor : cocos2d::CCLayerRGBA, cocos2d::CCBlendProtocol {
	auto CCLayerColor();
	auto changeHeight(float);
	auto changeWidth(float);
	auto changeWidthAndHeight(float, float);
	auto create(cocos2d::_ccColor4B const&);
	auto create(cocos2d::_ccColor4B const&, float, float);
	auto create();
	auto draw();
	auto getBlendFunc();
	auto init();
	auto initWithColor(cocos2d::_ccColor4B const&);
	auto initWithColor(cocos2d::_ccColor4B const&, float, float);
	auto setBlendFunc(cocos2d::_ccBlendFunc);
	auto setColor(cocos2d::_ccColor3B const&);
	auto setContentSize(cocos2d::CCSize const&);
	auto setOpacity(unsigned char);
	auto updateColor();
}
class cocos2d::CCLayerGradient : cocos2d::CCLayerColor {
	auto CCLayerGradient();
	auto create(cocos2d::_ccColor4B const&, cocos2d::_ccColor4B const&);
	auto create(cocos2d::_ccColor4B const&, cocos2d::_ccColor4B const&, cocos2d::CCPoint const&);
	auto create();
	auto getEndColor();
	auto getEndOpacity();
	auto getStartColor();
	auto getStartOpacity();
	auto getVector();
	auto init();
	auto initWithColor(cocos2d::_ccColor4B const&, cocos2d::_ccColor4B const&);
	auto initWithColor(cocos2d::_ccColor4B const&, cocos2d::_ccColor4B const&, cocos2d::CCPoint const&);
	auto isCompressedInterpolation();
	auto setCompressedInterpolation(bool);
	auto setEndColor(cocos2d::_ccColor3B const&);
	auto setEndOpacity(unsigned char);
	auto setStartColor(cocos2d::_ccColor3B const&);
	auto setStartOpacity(unsigned char);
	auto setVector(cocos2d::CCPoint const&);
	auto updateColor();
}
class cocos2d::CCLayerMultiplex : cocos2d::CCLayer {
	auto CCLayerMultiplex();
	auto addLayer(cocos2d::CCLayer*);
	// auto create(cocos2d::CCLayer*, ...);
	auto create();
	auto createWithArray(cocos2d::CCArray*);
	auto createWithLayer(cocos2d::CCLayer*);
	auto initWithArray(cocos2d::CCArray*);
	// auto initWithLayers(cocos2d::CCLayer*, gd::__va_list);
	auto switchTo(unsigned int);
	auto switchToAndReleaseMe(unsigned int);
}
class cocos2d::CCLayerRGBA : cocos2d::CCLayer, cocos2d::CCRGBAProtocol {
	auto CCLayerRGBA();
	auto getColor();
	auto getDisplayedColor();
	auto getDisplayedOpacity();
	auto getOpacity();
	auto init();
	auto isCascadeColorEnabled();
	auto isCascadeOpacityEnabled();
	auto isOpacityModifyRGB();
	auto setCascadeColorEnabled(bool);
	auto setCascadeOpacityEnabled(bool);
	auto setColor(cocos2d::_ccColor3B const&);
	auto setOpacity(unsigned char);
	auto setOpacityModifyRGB(bool);
	auto updateDisplayedColor(cocos2d::_ccColor3B const&);
	auto updateDisplayedOpacity(unsigned char);
}
class cocos2d::CCLens3D : cocos2d::CCGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, cocos2d::CCPoint const&, float);
	auto initWithDuration(float, cocos2d::CCSize const&, cocos2d::CCPoint const&, float);
	auto setPosition(cocos2d::CCPoint const&);
	auto update(float);
}
// class cocos2d::CCLightning : cocos2d::CCNode, cocos2d::CCRGBAProtocol {
// 	auto CCLightning();
// 	auto draw();
// 	auto getColor();
// 	auto getDisplayedColor();
// 	auto getDisplayedOpacity();
// 	auto getOpacity();
// 	auto initWithStrikePoint(cocos2d::CCPoint);
// 	auto initWithStrikePoint(cocos2d::CCPoint, cocos2d::CCPoint, float);
// 	auto isCascadeColorEnabled();
// 	auto isCascadeOpacityEnabled();
// 	auto isOpacityModifyRGB();
// 	auto lightningWithStrikePoint(cocos2d::CCPoint);
// 	auto lightningWithStrikePoint(cocos2d::CCPoint, cocos2d::CCPoint, float);
// 	auto setCascadeColorEnabled(bool);
// 	auto setCascadeOpacityEnabled(bool);
// 	auto setColor(cocos2d::_ccColor3B const&);
// 	auto setOpacity(unsigned char);
// 	auto setOpacityModifyRGB(bool);
// 	auto strike();
// 	auto strikeFinished();
// 	auto strikeRandom();
// 	auto strikeWithSeed(unsigned long);
// 	auto updateDisplayedColor(cocos2d::_ccColor3B const&);
// 	auto updateDisplayedOpacity(unsigned char);
// }
class cocos2d::CCLiquid : cocos2d::CCGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, unsigned int, float);
	auto initWithDuration(float, cocos2d::CCSize const&, unsigned int, float);
	auto update(float);
}
class cocos2d::CCMenu : cocos2d::CCLayerRGBA {
	auto addChild(cocos2d::CCNode*);
	auto addChild(cocos2d::CCNode*, int);
	auto addChild(cocos2d::CCNode*, int, int);
	auto alignItemsHorizontally();
	auto alignItemsHorizontallyWithPadding(float);
	// auto alignItemsInColumns(unsigned int, gd::__va_list);
	// auto alignItemsInColumns(unsigned int, ...);
	auto alignItemsInColumnsWithArray(cocos2d::CCArray*);
	// auto alignItemsInRows(unsigned int, gd::__va_list);
	// auto alignItemsInRows(unsigned int, ...);
	auto alignItemsInRowsWithArray(cocos2d::CCArray*);
	auto alignItemsVertically();
	auto alignItemsVerticallyWithPadding(float);
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	// auto create(cocos2d::CCMenuItem*, ...);
	auto create();
	auto createWithArray(cocos2d::CCArray*);
	auto createWithItem(cocos2d::CCMenuItem*);
	// auto createWithItems(cocos2d::CCMenuItem*, gd::__va_list);
	auto init();
	auto initWithArray(cocos2d::CCArray*);
	auto isEnabled();
	auto isOpacityModifyRGB();
	auto itemForTouch(cocos2d::CCTouch*);
	auto onExit();
	auto registerWithTouchDispatcher();
	auto removeChild(cocos2d::CCNode*, bool);
	auto setEnabled(bool);
	auto setHandlerPriority(int);
	auto setOpacityModifyRGB(bool);
}
class cocos2d::CCMenuItem : cocos2d::CCNodeRGBA {
	auto activate();
	auto create(cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto create();
	auto initWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto isEnabled();
	auto isSelected();
	auto rect();
	auto registerScriptTapHandler(int);
	auto selected();
	auto setEnabled(bool);
	auto setTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto unregisterScriptTapHandler();
	auto unselected();
}
class cocos2d::CCMenuItemAtlasFont : cocos2d::CCMenuItemLabel {
	auto create(char const*, char const*, int, int, char);
	auto create(char const*, char const*, int, int, char, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto initWithString(char const*, char const*, int, int, char, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
}
class cocos2d::CCMenuItemFont : cocos2d::CCMenuItemLabel {
	auto create(char const*);
	auto create(char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto fontName();
	auto fontNameObj();
	auto fontSize();
	auto fontSizeObj();
	auto initWithString(char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto recreateLabel();
	auto setFontName(char const*);
	auto setFontNameObj(char const*);
	auto setFontSize(unsigned int);
	auto setFontSizeObj(unsigned int);
}
class cocos2d::CCMenuItemImage : cocos2d::CCMenuItemSprite {
	auto CCMenuItemImage();
	auto create(char const*, char const*);
	auto create(char const*, char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto create(char const*, char const*, char const*);
	auto create(char const*, char const*, char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto create();
	auto init();
	auto initWithNormalImage(char const*, char const*, char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto setDisabledSpriteFrame(cocos2d::CCSpriteFrame*);
	auto setNormalSpriteFrame(cocos2d::CCSpriteFrame*);
	auto setSelectedSpriteFrame(cocos2d::CCSpriteFrame*);
}
class cocos2d::CCMenuItemLabel : cocos2d::CCMenuItem {
	auto CCMenuItemLabel();
	auto activate();
	auto create(cocos2d::CCNode*);
	auto create(cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto getDisabledColor();
	auto getLabel();
	auto initWithLabel(cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto selected();
	auto setDisabledColor(cocos2d::_ccColor3B const&);
	auto setEnabled(bool);
	auto setLabel(cocos2d::CCNode*);
	auto setString(char const*);
	auto unselected();
}
class cocos2d::CCMenuItemSprite : cocos2d::CCMenuItem {
	auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*);
	auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto getDisabledImage();
	auto getNormalImage();
	auto getSelectedImage();
	auto initWithNormalSprite(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto selected();
	auto setDisabledImage(cocos2d::CCNode*);
	auto setEnabled(bool);
	auto setNormalImage(cocos2d::CCNode*);
	auto setSelectedImage(cocos2d::CCNode*);
	auto unselected();
	auto updateImagesVisibility();
}
class cocos2d::CCMenuItemToggle : cocos2d::CCMenuItem {
	auto CCMenuItemToggle();
	auto activate();
	auto addSubItem(cocos2d::CCMenuItem*);
	auto create(cocos2d::CCMenuItem*);
	auto create();
	// auto createWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCMenuItem*, ...);
	auto createWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCArray*);
	auto getSelectedIndex();
	auto getSubItems();
	auto initWithItem(cocos2d::CCMenuItem*);
	// auto initWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, cocos2d::CCMenuItem*, gd::__va_list);
	auto selected();
	auto selectedItem();
	auto setEnabled(bool);
	auto setSelectedIndex(unsigned int);
	auto setSubItems(cocos2d::CCArray*);
	auto unselected();
}
class cocos2d::CCMotionStreak : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	auto CCMotionStreak();
	auto create(float, float, float, cocos2d::_ccColor3B const&, char const*);
	auto create(float, float, float, cocos2d::_ccColor3B const&, cocos2d::CCTexture2D*);
	auto draw();
	auto enableRepeatMode(float);
	auto getBlendFunc();
	auto getOpacity();
	auto getTexture();
	auto initWithFade(float, float, float, cocos2d::_ccColor3B const&, char const*);
	auto initWithFade(float, float, float, cocos2d::_ccColor3B const&, cocos2d::CCTexture2D*);
	auto isOpacityModifyRGB();
	auto reset();
	auto resumeStroke();
	auto setBlendFunc(cocos2d::_ccBlendFunc);
	auto setOpacity(unsigned char);
	auto setOpacityModifyRGB(bool);
	auto setPosition(cocos2d::CCPoint const&);
	auto setStroke(float);
	auto setTexture(cocos2d::CCTexture2D*);
	auto stopStroke();
	auto tintWithColor(cocos2d::_ccColor3B);
	auto update(float);
}
class cocos2d::CCMouseDelegate {
	auto rightKeyDown();
	auto rightKeyUp();
	auto scrollWheel(float, float);
}
class cocos2d::CCMouseDispatcher : cocos2d::CCObject {
	auto CCMouseDispatcher();
	auto addDelegate(cocos2d::CCMouseDelegate*);
	auto dispatchScrollMSG(float, float);
	auto forceAddDelegate(cocos2d::CCMouseDelegate*);
	auto forceRemoveDelegate(cocos2d::CCMouseDelegate*);
	auto removeDelegate(cocos2d::CCMouseDelegate*);
}
class cocos2d::CCMouseHandler : cocos2d::CCObject {
	auto getDelegate();
	auto handlerWithDelegate(cocos2d::CCMouseDelegate*);
	auto initWithDelegate(cocos2d::CCMouseDelegate*);
	auto setDelegate(cocos2d::CCMouseDelegate*);
}
class cocos2d::CCMoveBy : cocos2d::CCActionInterval {
	auto CCMoveBy();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCPoint const&);
	auto initWithDuration(float, cocos2d::CCPoint const&);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCMoveTo : cocos2d::CCMoveBy {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCPoint const&);
	auto initWithDuration(float, cocos2d::CCPoint const&);
	auto startWithTarget(cocos2d::CCNode*);
}
class cocos2d::CCNode : cocos2d::CCObject {
	auto CCNode();
	auto _setZOrder(int);
	auto addChild(cocos2d::CCNode*);
	auto addChild(cocos2d::CCNode*, int);
	auto addChild(cocos2d::CCNode*, int, int);
	auto addComponent(cocos2d::CCComponent*);
	auto boundingBox();
	auto childrenAlloc();
	auto cleanup();
	auto convertToNodeSpace(cocos2d::CCPoint const&);
	auto convertToNodeSpaceAR(cocos2d::CCPoint const&);
	auto convertToWindowSpace(cocos2d::CCPoint const&);
	auto convertToWorldSpace(cocos2d::CCPoint const&);
	auto convertToWorldSpaceAR(cocos2d::CCPoint const&);
	auto convertTouchToNodeSpace(cocos2d::CCTouch*);
	auto convertTouchToNodeSpaceAR(cocos2d::CCTouch*);
	auto create();
	auto description();
	auto detachChild(cocos2d::CCNode*, bool);
	auto draw();
	auto getActionByTag(int);
	auto getActionManager();
	auto getAnchorPoint();
	auto getAnchorPointInPoints();
	auto getCamera();
	auto getChildByTag(int);
	auto getChildren();
	auto getChildrenCount();
	auto getComponent(char const*);
	auto getContentSize();
	auto getGLServerState();
	auto getGrid();
	auto getOrderOfArrival();
	auto getParent();
	auto getPosition(float*, float*);
	auto getPosition();
	auto getPositionX();
	auto getPositionY();
	auto getRotation();
	auto getRotationX();
	auto getRotationY();
	auto getScale();
	auto getScaleX();
	auto getScaleY();
	auto getScaledContentSize();
	auto getScheduler();
	auto getShaderProgram();
	auto getSkewX();
	auto getSkewY();
	auto getUserData();
	auto getUserObject();
	auto getVertexZ();
	auto getZOrder();
	auto ignoreAnchorPointForPosition(bool);
	auto init();
	auto insertChild(cocos2d::CCNode*, int);
	auto isIgnoreAnchorPointForPosition();
	auto isRunning();
	auto isVisible();
	auto nodeToParentTransform();
	auto nodeToWorldTransform();
	auto numberOfRunningActions();
	auto onEnter();
	auto onEnterTransitionDidFinish();
	auto onExit();
	auto onExitTransitionDidStart();
	auto parentToNodeTransform();
	auto pauseSchedulerAndActions();
	auto registerScriptHandler(int);
	auto removeAllChildren();
	auto removeAllChildrenWithCleanup(bool);
	auto removeAllComponents();
	auto removeChild(cocos2d::CCNode*);
	auto removeChild(cocos2d::CCNode*, bool);
	auto removeChildByTag(int);
	auto removeChildByTag(int, bool);
	auto removeComponent(char const*);
	auto removeComponent(cocos2d::CCComponent*);
	auto removeFromParent();
	auto removeFromParentAndCleanup(bool);
	auto removeMeAndCleanup();
	auto reorderChild(cocos2d::CCNode*, int);
	auto resumeSchedulerAndActions();
	auto runAction(cocos2d::CCAction*);
	auto schedule(cocos2d::SEL_SCHEDULE);
	auto schedule(cocos2d::SEL_SCHEDULE, float);
	auto schedule(cocos2d::SEL_SCHEDULE, float, unsigned int, float);
	auto scheduleOnce(cocos2d::SEL_SCHEDULE, float);
	auto scheduleUpdate();
	auto scheduleUpdateWithPriority(int);
	auto scheduleUpdateWithPriorityLua(int, int);
	auto setActionManager(cocos2d::CCActionManager*);
	auto setAdditionalTransform(cocos2d::CCAffineTransform const&);
	auto setAnchorPoint(cocos2d::CCPoint const&);
	auto setContentSize(cocos2d::CCSize const&);
	auto setGLServerState(cocos2d::ccGLServerState);
	auto setGrid(cocos2d::CCGridBase*);
	auto setOrderOfArrival(unsigned int);
	auto setParent(cocos2d::CCNode*);
	auto setPosition(float, float);
	auto setPosition(cocos2d::CCPoint const&);
	auto setPositionX(float);
	auto setPositionY(float);
	auto setRotation(float);
	auto setRotationX(float);
	auto setRotationY(float);
	auto setScale(float);
	auto setScale(float, float);
	auto setScaleX(float);
	auto setScaleY(float);
	auto setScheduler(cocos2d::CCScheduler*);
	auto setShaderProgram(cocos2d::CCGLProgram*);
	auto setSkewX(float);
	auto setSkewY(float);
	auto setUserData(void*);
	auto setUserObject(cocos2d::CCObject*);
	auto setVertexZ(float);
	auto setVisible(bool);
	auto setZOrder(int);
	auto sortAllChildren();
	auto stopAction(cocos2d::CCAction*);
	auto stopActionByTag(int);
	auto stopAllActions();
	auto transform();
	auto transformAncestors();
	auto unregisterScriptHandler();
	auto unschedule(cocos2d::SEL_SCHEDULE);
	auto unscheduleAllSelectors();
	auto unscheduleUpdate();
	auto update(float);
	auto updateTransform();
	auto updateTweenAction(float, char const*);
	auto visit();
	auto worldToNodeTransform();
}
class cocos2d::CCNodeRGBA : cocos2d::CCNode, cocos2d::CCRGBAProtocol {
	auto CCNodeRGBA();
	auto create();
	auto getColor();
	auto getDisplayedColor();
	auto getDisplayedOpacity();
	auto getOpacity();
	auto init();
	auto isCascadeColorEnabled();
	auto isCascadeOpacityEnabled();
	auto isOpacityModifyRGB();
	auto setCascadeColorEnabled(bool);
	auto setCascadeOpacityEnabled(bool);
	auto setCascadeOpacityEnabled(bool);
	auto setColor(cocos2d::_ccColor3B const&);
	auto setColor(cocos2d::_ccColor3B const&);
	auto setOpacity(unsigned char);
	auto setOpacity(uchar);
	auto setOpacityModifyRGB(bool);
	auto updateDisplayedColor(cocos2d::_ccColor3B const&);
	auto updateDisplayedOpacity(unsigned char);
	auto updateDisplayedOpacity(uchar);
}
class cocos2d::CCNotificationCenter : cocos2d::CCObject {
	auto CCNotificationCenter();
	auto addObserver(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, char const*, cocos2d::CCObject*);
	auto getObserverHandlerByName(char const*);
	auto observerExisted(cocos2d::CCObject*, char const*);
	auto postNotification(char const*);
	auto postNotification(char const*, cocos2d::CCObject*);
	auto purgeNotificationCenter();
	auto registerScriptObserver(cocos2d::CCObject*, int, char const*);
	auto removeAllObservers(cocos2d::CCObject*);
	auto removeObserver(cocos2d::CCObject*, char const*);
	auto sharedNotificationCenter();
	auto unregisterScriptObserver(cocos2d::CCObject*, char const*);
}
class cocos2d::CCNotificationObserver : cocos2d::CCObject {
	auto CCNotificationObserver(cocos2d::CCObject*, cocos2d::SEL_MenuHandler, char const*, cocos2d::CCObject*);
	auto getHandler();
	auto getName();
	auto getObject();
	auto getSelector();
	auto getTarget();
	auto performSelector(cocos2d::CCObject*);
	auto setHandler(int);
}
class cocos2d::CCObject : cocos2d::CCCopying {
	auto CCObject();
	auto acceptVisitor(cocos2d::CCDataVisitor&);
	auto autorelease();
	auto canEncode();
	auto copy();
	auto createWithCoder(DS_Dictionary*);
	auto encodeWithCoder(DS_Dictionary*);
	auto getTag();
	auto isEqual(cocos2d::CCObject const*);
	auto isSingleReference();
	auto release();
	auto retain();
	auto retainCount();
	auto setTag(int);
	auto update(float);
}
class cocos2d::CCOrbitCamera : cocos2d::CCActionCamera {
	auto CCOrbitCamera();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, float, float, float, float, float, float);
	auto initWithDuration(float, float, float, float, float, float, float);
	auto sphericalRadius(float*, float*, float*);
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCPageTurn3D : cocos2d::CCGrid3DAction {
	auto create(float, cocos2d::CCSize const&);
	auto update(float);
}
class cocos2d::CCParallaxNode : cocos2d::CCNode {
	auto CCParallaxNode();
	auto absolutePosition();
	auto addChild(cocos2d::CCNode*, unsigned int, int);
	auto addChild(cocos2d::CCNode*, unsigned int, cocos2d::CCPoint const&, cocos2d::CCPoint const&);
	auto create();
	auto removeAllChildrenWithCleanup(bool);
	auto removeChild(cocos2d::CCNode*, bool);
	auto visit();
}
class cocos2d::CCParticleBatchNode : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	auto CCParticleBatchNode();
	auto addChild(cocos2d::CCNode*);
	auto addChild(cocos2d::CCNode*, int);
	auto addChild(cocos2d::CCNode*, int, int);
	auto addChildHelper(cocos2d::CCParticleSystem*, int, int);
	auto create(char const*, unsigned int);
	auto createWithTexture(cocos2d::CCTexture2D*, unsigned int);
	auto disableParticle(unsigned int);
	auto draw();
	auto getBlendFunc();
	auto getCurrentIndex(unsigned int*, unsigned int*, cocos2d::CCNode*, int);
	auto getTexture();
	auto increaseAtlasCapacityTo(unsigned int);
	auto initWithFile(char const*, unsigned int);
	auto initWithTexture(cocos2d::CCTexture2D*, unsigned int);
	auto insertChild(cocos2d::CCParticleSystem*, unsigned int);
	auto removeAllChildrenWithCleanup(bool);
	auto removeChild(cocos2d::CCNode*, bool);
	auto removeChildAtIndex(unsigned int, bool);
	auto reorderChild(cocos2d::CCNode*, int);
	auto searchNewPositionInChildrenForZ(int);
	auto setBlendFunc(cocos2d::_ccBlendFunc);
	auto setTexture(cocos2d::CCTexture2D*);
	auto updateAllAtlasIndexes();
	auto updateBlendFunc();
	auto visit();
}
class cocos2d::CCParticleExplosion : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleFire : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleFireworks : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleFlower : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleGalaxy : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleMeteor : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleRain : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleSmoke : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleSnow : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleSpiral : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleSun : cocos2d::CCParticleSystemQuad {
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto init();
	auto initWithTotalParticles(unsigned int);
}
class cocos2d::CCParticleSystem : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	auto CCParticleSystem();
	auto addParticle();
	auto create(char const*);
	auto createWithTotalParticles(unsigned int);
	auto getAngle();
	auto getAngleVar();
	auto getBatchNode();
	auto getBlendFunc();
	auto getDuration();
	auto getEmissionRate();
	auto getEmitterMode();
	auto getEndColor();
	auto getEndColorVar();
	auto getEndRadius();
	auto getEndRadiusVar();
	auto getEndSize();
	auto getEndSizeVar();
	auto getEndSpin();
	auto getEndSpinVar();
	auto getGravity();
	auto getLife();
	auto getLifeVar();
	auto getOpacityModifyRGB();
	auto getParticleCount();
	auto getPosVar();
	auto getPositionType();
	auto getRadialAccel();
	auto getRadialAccelVar();
	auto getRotatePerSecond();
	auto getRotatePerSecondVar();
	auto getRotationIsDir();
	auto getSourcePosition();
	auto getSpeed();
	auto getSpeedVar();
	auto getStartColor();
	auto getStartColorVar();
	auto getStartRadius();
	auto getStartRadiusVar();
	auto getStartSize();
	auto getStartSizeVar();
	auto getStartSpin();
	auto getStartSpinVar();
	auto getTangentialAccel();
	auto getTangentialAccelVar();
	auto getTexture();
	auto getTotalParticles();
	auto init();
	auto initParticle(cocos2d::sCCParticle*);
	auto initWithDictionary(cocos2d::CCDictionary*);
	auto initWithDictionary(cocos2d::CCDictionary*, char const*);
	auto initWithFile(char const*);
	auto initWithTotalParticles(unsigned int);
	auto isActive();
	auto isAutoRemoveOnFinish();
	auto isBlendAdditive();
	auto isFull();
	auto loadDefaults();
	auto loadScaledDefaults(float);
	auto postStep();
	auto resetSystem();
	auto resumeSystem();
	auto saveDefaults();
	auto setAngle(float);
	auto setAngleVar(float);
	auto setAutoRemoveOnFinish(bool);
	auto setBatchNode(cocos2d::CCParticleBatchNode*);
	auto setBlendAdditive(bool);
	auto setBlendFunc(cocos2d::_ccBlendFunc);
	auto setDuration(float);
	auto setEmissionRate(float);
	auto setEmitterMode(int);
	auto setEndColor(cocos2d::_ccColor4F const&);
	auto setEndColorVar(cocos2d::_ccColor4F const&);
	auto setEndRadius(float);
	auto setEndRadiusVar(float);
	auto setEndSize(float);
	auto setEndSizeVar(float);
	auto setEndSpin(float);
	auto setEndSpinVar(float);
	auto setGravity(cocos2d::CCPoint const&);
	auto setLife(float);
	auto setLifeVar(float);
	auto setOpacityModifyRGB(bool);
	auto setPosVar(cocos2d::CCPoint const&);
	auto setPositionType(cocos2d::tCCPositionType);
	auto setRadialAccel(float);
	auto setRadialAccelVar(float);
	auto setRotatePerSecond(float);
	auto setRotatePerSecondVar(float);
	auto setRotation(float);
	auto setRotationIsDir(bool);
	auto setScale(float);
	auto setScaleX(float);
	auto setScaleY(float);
	auto setSourcePosition(cocos2d::CCPoint const&);
	auto setSpeed(float);
	auto setSpeedVar(float);
	auto setStartColor(cocos2d::_ccColor4F const&);
	auto setStartColorVar(cocos2d::_ccColor4F const&);
	auto setStartRadius(float);
	auto setStartRadiusVar(float);
	auto setStartSize(float);
	auto setStartSizeVar(float);
	auto setStartSpin(float);
	auto setStartSpinVar(float);
	auto setTangentialAccel(float);
	auto setTangentialAccelVar(float);
	auto setTexture(cocos2d::CCTexture2D*);
	auto setTotalParticles(unsigned int);
	auto stopSystem();
	auto update(float);
	auto updateBlendFunc();
	auto updateQuadWithParticle(cocos2d::sCCParticle*, cocos2d::CCPoint const&);
	auto updateWithNoTime();
}
class cocos2d::CCParticleSystemQuad : cocos2d::CCParticleSystem {
	auto CCParticleSystemQuad();
	auto allocMemory();
	auto create(char const*);
	auto create();
	auto createWithTotalParticles(unsigned int);
	auto draw();
	auto initIndices();
	auto initTexCoordsWithRect(cocos2d::CCRect const&);
	auto initWithTotalParticles(unsigned int);
	auto listenBackToForeground(cocos2d::CCObject*);
	auto postStep();
	auto setBatchNode(cocos2d::CCParticleBatchNode*);
	auto setDisplayFrame(cocos2d::CCSpriteFrame*);
	auto setTexture(cocos2d::CCTexture2D*);
	auto setTextureWithRect(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	auto setTotalParticles(unsigned int);
	auto setupVBO();
	auto updateQuadWithParticle(cocos2d::sCCParticle*, cocos2d::CCPoint const&);
}
class cocos2d::CCPlace : cocos2d::CCActionInstant {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCPoint const&);
	auto initWithPosition(cocos2d::CCPoint const&);
	auto update(float);
}
class cocos2d::CCPointArray : cocos2d::CCObject {
	auto CCPointArray();
	auto addControlPoint(cocos2d::CCPoint);
	auto copyWithZone(cocos2d::CCZone*);
	auto count();
	auto create(unsigned int);
	auto getControlPointAtIndex(unsigned int);
	auto getControlPoints();
	auto initWithCapacity(unsigned int);
	auto insertControlPoint(cocos2d::CCPoint&, unsigned int);
	auto removeControlPointAtIndex(unsigned int);
	auto replaceControlPoint(cocos2d::CCPoint&, unsigned int);
	auto reverse();
	auto reverseInline();
	auto setControlPoints(gd::vector<cocos2d::CCPoint*>*);
}
class cocos2d::CCPointObject {
	auto pointWithCCPoint(cocos2d::CCPoint, cocos2d::CCPoint);
}
class cocos2d::CCPrettyPrinter : cocos2d::CCDataVisitor {
	auto CCPrettyPrinter(int);
	auto clear();
	auto getResult();
	auto setIndentLevel(int);
	auto visit(cocos2d::CCDictionary const*);
	auto visit(cocos2d::CCSet const*);
	auto visit(cocos2d::CCBool const*);
	auto visit(cocos2d::CCArray const*);
	auto visit(cocos2d::CCFloat const*);
	auto visit(cocos2d::CCDouble const*);
	auto visit(cocos2d::CCString const*);
	auto visit(cocos2d::CCInteger const*);
	auto visitObject(cocos2d::CCObject const*);
}
class cocos2d::CCProfiler : cocos2d::CCObject {
	auto createAndAddTimerWithName(char const*);
	auto displayTimers();
	auto init();
	auto releaseAllTimers();
	auto releaseTimer(char const*);
	auto sharedProfiler();
}
class cocos2d::CCProfilingTimer : cocos2d::CCObject {
	auto CCProfilingTimer();
	auto description();
	auto initWithName(char const*);
	auto reset();
}
class cocos2d::CCProgressFromTo : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, float, float);
	auto initWithDuration(float, float, float);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCProgressTimer : cocos2d::CCNodeRGBA {
	auto CCProgressTimer();
	auto boundaryTexCoord(char);
	auto create(cocos2d::CCSprite*);
	auto draw();
	auto getColor();
	auto getMidpoint();
	auto getOpacity();
	auto initWithSprite(cocos2d::CCSprite*);
	auto setAnchorPoint(cocos2d::CCPoint);
	auto setColor(cocos2d::_ccColor3B const&);
	auto setMidpoint(cocos2d::CCPoint);
	auto setOpacity(unsigned char);
	auto setPercentage(float);
	auto setReverseProgress(bool);
	auto setSprite(cocos2d::CCSprite*);
	auto setType(cocos2d::CCProgressTimerType);
	auto textureCoordFromAlphaPoint(cocos2d::CCPoint);
	auto updateBar();
	auto updateColor();
	auto updateProgress();
	auto updateRadial();
	auto vertexFromAlphaPoint(cocos2d::CCPoint);
}
class cocos2d::CCProgressTo : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, float);
	auto initWithDuration(float, float);
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCRemoveSelf : cocos2d::CCActionInstant {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(bool);
	auto init(bool);
	auto reverse();
	auto update(float);
}
class cocos2d::CCRenderTexture : cocos2d::CCNode {
	auto CCRenderTexture();
	auto begin();
	auto beginWithClear(float, float, float, float);
	auto beginWithClear(float, float, float, float, float);
	auto beginWithClear(float, float, float, float, float, int);
	auto beginWithClear(float, float, float, float, float, int, unsigned int);
	auto clear(float, float, float, float);
	auto clearDepth(float);
	auto clearStencil(int);
	auto create(int, int);
	auto create(int, int, cocos2d::CCTexture2DPixelFormat);
	auto create(int, int, cocos2d::CCTexture2DPixelFormat, unsigned int);
	auto draw();
	auto end();
	auto getClearColor();
	auto getClearDepth();
	auto getClearFlags();
	auto getClearStencil();
	auto getSprite();
	auto initWithWidthAndHeight(int, int, cocos2d::CCTexture2DPixelFormat);
	auto initWithWidthAndHeight(int, int, cocos2d::CCTexture2DPixelFormat, unsigned int);
	auto isAutoDraw();
	auto listenToBackground(cocos2d::CCObject*);
	auto listenToForeground(cocos2d::CCObject*);
	auto newCCImage(bool);
	auto saveToFile(char const*);
	auto saveToFile(char const*, cocos2d::eImageFormat);
	auto setAutoDraw(bool);
	auto setClearColor(cocos2d::_ccColor4F const&);
	auto setClearDepth(float);
	auto setClearFlags(unsigned int);
	auto setClearStencil(float);
	auto setSprite(cocos2d::CCSprite*);
	auto visit();
}
class cocos2d::CCRepeat : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCFiniteTimeAction*, unsigned int);
	auto initWithAction(cocos2d::CCFiniteTimeAction*, unsigned int);
	auto isDone();
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto stop();
	auto update(float);
}
class cocos2d::CCRepeatForever : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*);
	auto initWithAction(cocos2d::CCActionInterval*);
	auto isDone();
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto step(float);
}
class cocos2d::CCReuseGrid : cocos2d::CCActionInstant {
	auto create(int);
	auto initWithTimes(int);
	auto startWithTarget(cocos2d::CCNode*);
}
class cocos2d::CCReverseTime : cocos2d::CCActionInterval {
	auto CCReverseTime();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCFiniteTimeAction*);
	auto initWithAction(cocos2d::CCFiniteTimeAction*);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto stop();
	auto update(float);
}
class cocos2d::CCRGBAProtocol {
}
class cocos2d::CCRipple3D : cocos2d::CCGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, cocos2d::CCPoint const&, float, unsigned int, float);
	auto initWithDuration(float, cocos2d::CCSize const&, cocos2d::CCPoint const&, float, unsigned int, float);
	auto setPosition(cocos2d::CCPoint const&);
	auto update(float);
}
class cocos2d::CCRotateBy : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, float);
	auto create(float, float, float);
	auto initWithDuration(float, float);
	auto initWithDuration(float, float, float);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCRotateTo : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, float);
	auto create(float, float, float);
	auto initWithDuration(float, float);
	auto initWithDuration(float, float, float);
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCSAXDelegator {
}
class cocos2d::CCScaleBy : cocos2d::CCScaleTo {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, float);
	auto create(float, float, float);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
}
class cocos2d::CCScaleTo : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, float);
	auto create(float, float, float);
	auto initWithDuration(float, float);
	auto initWithDuration(float, float, float);
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCScene : cocos2d::CCNode {
	auto CCScene();
	auto create();
	auto getHighestChildZ();
	auto init();
}
class cocos2d::CCSceneDelegate {
	auto willSwitchToScene(cocos2d::CCScene*);
}
class cocos2d::CCSceneTransitionDelegate {
	auto sceneWillResume();
}
class cocos2d::CCScheduler : cocos2d::CCObject {
	auto CCScheduler();
	auto appendIn(cocos2d::_listEntry**, cocos2d::CCObject*, bool);
	auto isTargetPaused(cocos2d::CCObject*);
	auto pauseAllTargets();
	auto pauseAllTargetsWithMinPriority(int);
	auto pauseTarget(cocos2d::CCObject*);
	auto priorityIn(cocos2d::_listEntry**, cocos2d::CCObject*, int, bool);
	auto removeHashElement(cocos2d::_hashSelectorEntry*);
	auto removeUpdateFromHash(cocos2d::_listEntry*);
	auto resumeTarget(cocos2d::CCObject*);
	auto resumeTargets(cocos2d::CCSet*);
	auto scheduleScriptFunc(unsigned int, float, bool);
	auto scheduleSelector(cocos2d::SEL_SCHEDULE, cocos2d::CCObject*, float, bool);
	auto scheduleSelector(cocos2d::SEL_SCHEDULE, cocos2d::CCObject*, float, unsigned int, float, bool);
	auto scheduleUpdateForTarget(cocos2d::CCObject*, int, bool);
	auto unscheduleAll();
	auto unscheduleAllForTarget(cocos2d::CCObject*);
	auto unscheduleAllWithMinPriority(int);
	auto unscheduleScriptEntry(unsigned int);
	auto unscheduleSelector(cocos2d::SEL_SCHEDULE, cocos2d::CCObject*);
	auto unscheduleUpdateForTarget(cocos2d::CCObject const*);
	auto update(float);
}
class cocos2d::CCSchedulerScriptHandlerEntry : cocos2d::CCScriptHandlerEntry {
	auto create(int, float, bool);
	auto init(float, bool);
}
class cocos2d::CCScriptHandlerEntry : cocos2d::CCObject {
	auto CCScriptHandlerEntry(int);
	auto create(int);
}
class cocos2d::CCSequence : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	// auto create(cocos2d::CCFiniteTimeAction*, ...);
	auto create(cocos2d::CCArray*);
	auto createWithTwoActions(cocos2d::CCFiniteTimeAction*, cocos2d::CCFiniteTimeAction*);
	// auto createWithVariableList(cocos2d::CCFiniteTimeAction*, gd::__va_list);
	auto initWithTwoActions(cocos2d::CCFiniteTimeAction*, cocos2d::CCFiniteTimeAction*);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto stop();
	auto update(float);
}
class cocos2d::CCSet : cocos2d::CCObject {
	auto CCSet(cocos2d::CCSet const&);
	auto CCSet();
	auto acceptVisitor(cocos2d::CCDataVisitor&);
	auto addObject(cocos2d::CCObject*);
	auto anyObject();
	auto begin();
	auto containsObject(cocos2d::CCObject*);
	auto copy();
	auto count();
	auto create();
	auto end();
	auto mutableCopy();
	auto removeAllObjects();
	auto removeObject(cocos2d::CCObject*);
}
class cocos2d::CCShaderCache : cocos2d::CCObject {
	auto CCShaderCache();
	auto addProgram(cocos2d::CCGLProgram*, char const*);
	auto init();
	auto loadDefaultShader(cocos2d::CCGLProgram*, int);
	auto loadDefaultShaders();
	auto programForKey(char const*);
	auto purgeSharedShaderCache();
	auto reloadDefaultShaders();
	auto sharedShaderCache();
}
class cocos2d::CCShaky3D : cocos2d::CCGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, int, bool);
	auto initWithDuration(float, cocos2d::CCSize const&, int, bool);
	auto update(float);
}
class cocos2d::CCShakyTiles3D : cocos2d::CCTiledGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, int, bool);
	auto initWithDuration(float, cocos2d::CCSize const&, int, bool);
	auto update(float);
}
class cocos2d::CCShatteredTiles3D : cocos2d::CCTiledGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, int, bool);
	auto initWithDuration(float, cocos2d::CCSize const&, int, bool);
	auto update(float);
}
class cocos2d::CCShow : cocos2d::CCActionInstant {
	auto copyWithZone(cocos2d::CCZone*);
	auto create();
	auto reverse();
	auto update(float);
}
class cocos2d::CCShuffleTiles : cocos2d::CCTiledGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, unsigned int);
	auto getDelta(cocos2d::CCSize const&);
	auto initWithDuration(float, cocos2d::CCSize const&, unsigned int);
	auto placeTile(cocos2d::CCPoint const&, cocos2d::Tile*);
	auto shuffle(unsigned int*, unsigned int);
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCSkewBy : cocos2d::CCSkewTo {
	auto create(float, float, float);
	auto initWithDuration(float, float, float);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
}
class cocos2d::CCSkewTo : cocos2d::CCActionInterval {
	auto CCSkewTo();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, float, float);
	auto initWithDuration(float, float, float);
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCSpawn : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	// auto create(cocos2d::CCFiniteTimeAction*, ...);
	auto create(cocos2d::CCArray*);
	auto createWithTwoActions(cocos2d::CCFiniteTimeAction*, cocos2d::CCFiniteTimeAction*);
	// auto createWithVariableList(cocos2d::CCFiniteTimeAction*, gd::__va_list);
	auto initWithTwoActions(cocos2d::CCFiniteTimeAction*, cocos2d::CCFiniteTimeAction*);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto stop();
	auto update(float);
}
class cocos2d::CCSpeed : cocos2d::CCAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCActionInterval*, float);
	auto initWithAction(cocos2d::CCActionInterval*, float);
	auto isDone();
	auto reverse();
	auto setInnerAction(cocos2d::CCActionInterval*);
	auto startWithTarget(cocos2d::CCNode*);
	auto step(float);
	auto stop();
}
class cocos2d::CCSplitCols : cocos2d::CCTiledGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, unsigned int);
	auto initWithDuration(float, unsigned int);
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCSplitRows : cocos2d::CCTiledGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, unsigned int);
	auto initWithDuration(float, unsigned int);
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCSprite : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	auto CCSprite();
	auto addChild(cocos2d::CCNode*);
	auto addChild(cocos2d::CCNode*, int);
	auto addChild(cocos2d::CCNode*, int, int);
	auto create(char const*);
	auto create(char const*, cocos2d::CCRect const&);
	auto create();
	auto createWithSpriteFrame(cocos2d::CCSpriteFrame*);
	auto createWithSpriteFrameName(char const*);
	auto createWithTexture(cocos2d::CCTexture2D*);
	auto createWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	auto displayFrame();
	auto draw();
	auto getBatchNode();
	auto getBlendFunc();
	auto getTexture();
	auto ignoreAnchorPointForPosition(bool);
	auto init();
	auto initWithFile(char const*);
	auto initWithFile(char const*, cocos2d::CCRect const&);
	auto initWithSpriteFrame(cocos2d::CCSpriteFrame*);
	auto initWithSpriteFrameName(char const*);
	auto initWithTexture(cocos2d::CCTexture2D*);
	auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool);
	auto isDirty();
	auto isFlipX();
	auto isFlipY();
	auto isFrameDisplayed(cocos2d::CCSpriteFrame*);
	auto isOpacityModifyRGB();
	auto refreshTextureRect();
	auto removeAllChildrenWithCleanup(bool);
	auto removeChild(cocos2d::CCNode*, bool);
	auto reorderChild(cocos2d::CCNode*, int);
	auto setAnchorPoint(cocos2d::CCPoint const&);
	auto setBatchNode(cocos2d::CCSpriteBatchNode*);
	auto setBlendFunc(cocos2d::_ccBlendFunc);
	auto setChildColor(cocos2d::_ccColor3B const&);
	auto setChildOpacity(unsigned char);
	auto setColor(cocos2d::_ccColor3B const&);
	auto setDirty(bool);
	auto setDirtyRecursively(bool);
	auto setDisplayFrame(cocos2d::CCSpriteFrame*);
	auto setDisplayFrameWithAnimationName(char const*, int);
	auto setFlipX(bool);
	auto setFlipY(bool);
	auto setOpacity(unsigned char);
	auto setOpacityModifyRGB(bool);
	auto setPosition(cocos2d::CCPoint const&);
	auto setReorderChildDirtyRecursively();
	auto setRotation(float);
	auto setRotationX(float);
	auto setRotationY(float);
	auto setScale(float);
	auto setScaleX(float);
	auto setScaleY(float);
	auto setSkewX(float);
	auto setSkewY(float);
	auto setTexture(cocos2d::CCTexture2D*);
	auto setTextureCoords(cocos2d::CCRect const&);
	auto setTextureRect(cocos2d::CCRect const&);
	auto setTextureRect(cocos2d::CCRect const&, bool, cocos2d::CCSize const&);
	auto setVertexRect(cocos2d::CCRect const&);
	auto setVertexZ(float);
	auto setVisible(bool);
	auto sortAllChildren();
	auto updateBlendFunc();
	auto updateColor();
	auto updateDisplayedColor(cocos2d::_ccColor3B const&);
	auto updateDisplayedOpacity(unsigned char);
	auto updateTransform();
}
class cocos2d::CCSpriteBatchNode : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	auto CCSpriteBatchNode();
	auto addChild(cocos2d::CCNode*);
	auto addChild(cocos2d::CCNode*, int);
	auto addChild(cocos2d::CCNode*, int, int);
	auto addSpriteWithoutQuad(cocos2d::CCSprite*, unsigned int, int);
	auto appendChild(cocos2d::CCSprite*);
	auto atlasIndexForChild(cocos2d::CCSprite*, int);
	auto create(char const*, unsigned int);
	auto createWithTexture(cocos2d::CCTexture2D*);
	auto createWithTexture(cocos2d::CCTexture2D*, unsigned int);
	auto draw();
	auto getAtlasCapacity();
	auto getBlendFunc();
	auto getTexture();
	auto getUsedAtlasCapacity();
	auto highestAtlasIndexInChild(cocos2d::CCSprite*);
	auto increaseAtlasCapacity(unsigned int);
	auto increaseAtlasCapacity();
	auto init();
	auto initWithFile(char const*, unsigned int);
	auto initWithTexture(cocos2d::CCTexture2D*, unsigned int);
	auto insertChild(cocos2d::CCSprite*, unsigned int);
	auto insertQuadFromSprite(cocos2d::CCSprite*, unsigned int);
	auto lowestAtlasIndexInChild(cocos2d::CCSprite*);
	auto manualSortAllChildren();
	auto rebuildIndexInOrder(cocos2d::CCSprite*, unsigned int);
	auto removeAllChildrenWithCleanup(bool);
	auto removeChild(cocos2d::CCNode*, bool);
	auto removeChildAtIndex(unsigned int, bool);
	auto removeSpriteFromAtlas(cocos2d::CCSprite*);
	auto reorderBatch(bool);
	auto reorderChild(cocos2d::CCNode*, int);
	auto setBlendFunc(cocos2d::_ccBlendFunc);
	auto setTexture(cocos2d::CCTexture2D*);
	auto sortAllChildren();
	auto swap(int, int);
	auto updateAtlasIndex(cocos2d::CCSprite*, int*);
	auto updateBlendFunc();
	auto updateQuadFromSprite(cocos2d::CCSprite*, unsigned int);
	auto visit();
}
class cocos2d::CCSpriteExtra : cocos2d::CCSprite {
}
class cocos2d::CCSpriteFrame : cocos2d::CCObject {
	auto CCSpriteFrame();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(char const*, cocos2d::CCRect const&);
	auto create(char const*, cocos2d::CCRect const&, bool, cocos2d::CCPoint const&, cocos2d::CCSize const&);
	auto createWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	auto createWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool, cocos2d::CCPoint const&, cocos2d::CCSize const&);
	auto getOffset();
	auto getOffsetInPixels();
	auto getTexture();
	auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&);
	auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool, cocos2d::CCPoint const&, cocos2d::CCSize const&);
	auto initWithTextureFilename(char const*, cocos2d::CCRect const&);
	auto initWithTextureFilename(char const*, cocos2d::CCRect const&, bool, cocos2d::CCPoint const&, cocos2d::CCSize const&);
	auto setOffset(cocos2d::CCPoint const&);
	auto setOffsetInPixels(cocos2d::CCPoint const&);
	auto setRect(cocos2d::CCRect const&);
	auto setRectInPixels(cocos2d::CCRect const&);
	auto setTexture(cocos2d::CCTexture2D*);
}
class cocos2d::CCSpriteFrameCache : cocos2d::CCObject {
	auto addSpriteFrame(cocos2d::CCSpriteFrame*, char const*);
	auto addSpriteFramesWithDictionary(cocos2d::CCDictionary*, cocos2d::CCTexture2D*);
	auto addSpriteFramesWithFile(char const*);
	auto addSpriteFramesWithFile(char const*, cocos2d::CCTexture2D*);
	auto addSpriteFramesWithFile(char const*, char const*);
	auto init();
	auto purgeSharedSpriteFrameCache();
	auto removeSpriteFrameByName(char const*);
	auto removeSpriteFrames();
	auto removeSpriteFramesFromDictionary(cocos2d::CCDictionary*);
	auto removeSpriteFramesFromFile(char const*);
	auto removeSpriteFramesFromTexture(cocos2d::CCTexture2D*);
	auto removeUnusedSpriteFrames();
	auto sharedSpriteFrameCache();
	auto spriteFrameByName(char const*);
}
class cocos2d::CCStandardTouchHandler : cocos2d::CCTouchHandler {
	auto handlerWithDelegate(cocos2d::CCTouchDelegate*, int);
	auto initWithDelegate(cocos2d::CCTouchDelegate*, int);
}
class cocos2d::CCStopGrid : cocos2d::CCActionInstant {
	auto create();
	auto startWithTarget(cocos2d::CCNode*);
}
class cocos2d::CCString : cocos2d::CCObject {
	auto CCString(char const*);
	auto CCString(cocos2d::CCString const&);
	auto CCString(gd::string const&);
	auto CCString();
	auto acceptVisitor(cocos2d::CCDataVisitor&);
	auto boolValue();
	auto compare(char const*);
	auto copyWithZone(cocos2d::CCZone*);
	auto create(gd::string const&);
	auto createWithContentsOfFile(char const*);
	auto createWithData(unsigned char const*, unsigned long);
	// auto createWithFormat(char const*, ...);
	auto doubleValue();
	auto floatValue();
	auto getCString();
	// auto initWithFormat(char const*, ...);
	// auto initWithFormatAndValist(char const*, gd::__va_list);
	auto intValue();
	auto isEqual(cocos2d::CCObject const*);
	auto length();
	auto uintValue();
}
class cocos2d::CCTargetedAction : cocos2d::CCActionInterval {
	auto CCTargetedAction();
	auto copyWithZone(cocos2d::CCZone*);
	auto create(cocos2d::CCNode*, cocos2d::CCFiniteTimeAction*);
	auto initWithTarget(cocos2d::CCNode*, cocos2d::CCFiniteTimeAction*);
	auto startWithTarget(cocos2d::CCNode*);
	auto stop();
	auto update(float);
}
class cocos2d::CCTargetedTouchHandler : cocos2d::CCTouchHandler {
	auto getClaimedTouches();
	auto handlerWithDelegate(cocos2d::CCTouchDelegate*, int, bool);
	auto initWithDelegate(cocos2d::CCTouchDelegate*, int, bool);
	auto isSwallowsTouches();
	auto setSwallowsTouches(bool);
}
class cocos2d::CCTextFieldDelegate {
	auto onDraw(cocos2d::CCTextFieldTTF*);
	auto onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF*);
	auto onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF*, char const*, int);
	auto onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF*);
	auto onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int);
	auto textChanged();
}
class cocos2d::CCTextFieldTTF : cocos2d::CCLabelTTF, cocos2d::CCIMEDelegate {
	auto CCTextFieldTTF();
	auto attachWithIME();
	auto canAttachWithIME();
	auto canDetachWithIME();
	auto deleteBackward();
	auto detachWithIME();
	auto draw();
	auto getColorSpaceHolder();
	auto getContentText();
	auto getPlaceHolder();
	auto getString();
	auto initWithPlaceHolder(char const*, cocos2d::CCSize const&, cocos2d::CCTextAlignment, char const*, float);
	auto initWithPlaceHolder(char const*, char const*, float);
	auto insertText(char const*, int);
	auto isSecureTextEntry();
	auto setColorSpaceHolder(cocos2d::_ccColor3B const&);
	auto setPlaceHolder(char const*);
	auto setSecureTextEntry(bool);
	auto setString(char const*);
	auto textFieldWithPlaceHolder(char const*, cocos2d::CCSize const&, cocos2d::CCTextAlignment, char const*, float);
	auto textFieldWithPlaceHolder(char const*, char const*, float);
}
class cocos2d::CCTexture2D : cocos2d::CCObject {
	auto CCTexture2D();
	auto PVRImagesHavePremultipliedAlpha(bool);
	auto bitsPerPixelForFormat(cocos2d::CCTexture2DPixelFormat);
	auto bitsPerPixelForFormat();
	auto defaultAlphaPixelFormat();
	auto description();
	auto drawAtPoint(cocos2d::CCPoint const&);
	auto drawInRect(cocos2d::CCRect const&);
	auto generateMipmap();
	auto getContentSize();
	auto getContentSizeInPixels();
	auto getMaxS();
	auto getMaxT();
	auto getName();
	auto getPixelFormat();
	auto getPixelsHigh();
	auto getPixelsWide();
	auto getShaderProgram();
	auto hasMipmaps();
	auto hasPremultipliedAlpha();
	auto initPremultipliedATextureWithImage(cocos2d::CCImage*, unsigned int, unsigned int);
	auto initWithData(auto const*, cocos2d::CCTexture2DPixelFormat, unsigned int, unsigned int, cocos2d::CCSize const&);
	auto initWithETCFile(char const*);
	auto initWithImage(cocos2d::CCImage*);
	auto initWithPVRFile(char const*);
	auto initWithString(char const*, cocos2d::_ccFontDefinition*);
	auto initWithString(char const*, char const*, float);
	auto initWithString(char const*, char const*, float, cocos2d::CCSize const&, cocos2d::CCTextAlignment, cocos2d::CCVerticalTextAlignment);
	auto keepData(void*, unsigned int);
	auto releaseData(void*);
	auto releaseGLTexture();
	auto setAliasTexParameters();
	auto setAntiAliasTexParameters();
	auto setDefaultAlphaPixelFormat(cocos2d::CCTexture2DPixelFormat);
	auto setMaxS(float);
	auto setMaxT(float);
	auto setShaderProgram(cocos2d::CCGLProgram*);
	auto setTexParameters(cocos2d::_ccTexParams*);
	auto stringForFormat();
}
class cocos2d::CCTextureAtlas : cocos2d::CCObject {
	auto CCTextureAtlas();
	auto create(char const*, unsigned int);
	auto createWithTexture(cocos2d::CCTexture2D*, unsigned int);
	auto description();
	auto drawNumberOfQuads(unsigned int);
	auto drawNumberOfQuads(unsigned int, unsigned int);
	auto drawQuads();
	auto fillWithEmptyQuadsFromIndex(unsigned int, unsigned int);
	auto getCapacity();
	auto getQuads();
	auto getTexture();
	auto getTotalQuads();
	auto increaseTotalQuadsWith(unsigned int);
	auto initWithFile(char const*, unsigned int);
	auto initWithTexture(cocos2d::CCTexture2D*, unsigned int);
	auto insertQuad(cocos2d::_ccV3F_C4B_T2F_Quad*, unsigned int);
	auto insertQuadFromIndex(unsigned int, unsigned int);
	auto insertQuads(cocos2d::_ccV3F_C4B_T2F_Quad*, unsigned int, unsigned int);
	auto listenBackToForeground(cocos2d::CCObject*);
	auto mapBuffers();
	auto moveQuadsFromIndex(unsigned int, unsigned int);
	auto moveQuadsFromIndex(unsigned int, unsigned int, unsigned int);
	auto removeAllQuads();
	auto removeQuadAtIndex(unsigned int);
	auto removeQuadsAtIndex(unsigned int, unsigned int);
	auto resizeCapacity(unsigned int);
	auto setQuads(cocos2d::_ccV3F_C4B_T2F_Quad*);
	auto setTexture(cocos2d::CCTexture2D*);
	auto setupIndices();
	auto setupVBO();
	auto updateQuad(cocos2d::_ccV3F_C4B_T2F_Quad*, unsigned int);
}
class cocos2d::CCTextureCache : cocos2d::CCObject {
	auto CCTextureCache();
	auto addETCImage(char const*);
	auto addImage(char const*, bool);
	auto addImageAsync(char const*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler);
	auto addImageAsyncCallBack(float);
	auto addPVRImage(char const*);
	auto addUIImage(cocos2d::CCImage*, char const*);
	auto description();
	auto dumpCachedTextureInfo();
	auto purgeSharedTextureCache();
	auto reloadAllTextures();
	auto reloadTexture(char const*);
	auto removeAllTextures();
	auto removeTexture(cocos2d::CCTexture2D*);
	auto removeTextureForKey(char const*);
	auto removeUnusedTextures();
	auto sharedTextureCache();
	auto snapshotTextures();
	auto textureForKey(char const*);
}
class cocos2d::CCTextureETC : cocos2d::CCObject {
	auto CCTextureETC();
	auto getHeight();
	auto getName();
	auto getWidth();
	auto initWithFile(char const*);
	auto loadTexture(char const*);
}
class cocos2d::CCTextureProtocol : cocos2d::CCBlendProtocol {
}
class cocos2d::CCTexturePVR : cocos2d::CCObject {
	auto CCTexturePVR();
	auto create(char const*);
	auto createGLTexture();
	auto initWithContentsOfFile(char const*);
	auto unpackPVRv2Data(unsigned char*, unsigned int);
	auto unpackPVRv3Data(unsigned char*, unsigned int);
}
class cocos2d::CCTiledGrid3D : cocos2d::CCGridBase {
	auto CCTiledGrid3D();
	auto blit();
	auto calculateVertexPoints();
	auto create(cocos2d::CCSize const&);
	auto create(cocos2d::CCSize const&, cocos2d::CCTexture2D*, bool);
	auto originalTile(cocos2d::CCPoint const&);
	auto reuse();
	auto setTile(cocos2d::CCPoint const&, cocos2d::_ccQuad3 const&);
	auto tile(cocos2d::CCPoint const&);
}
class cocos2d::CCTiledGrid3DAction : cocos2d::CCGridAction {
	auto CCTiledGrid3DAction();
	auto create(float, cocos2d::CCSize const&);
	auto getGrid();
	auto originalTile(cocos2d::CCPoint const&);
	auto setTile(cocos2d::CCPoint const&, cocos2d::_ccQuad3 const&);
	auto tile(cocos2d::CCPoint const&);
}
class cocos2d::CCTileMapAtlas : cocos2d::CCAtlasNode {
	auto CCTileMapAtlas();
	auto calculateItemsToRender();
	auto create(char const*, char const*, int, int);
	auto getTGAInfo();
	auto initWithTileFile(char const*, char const*, int, int);
	auto loadTGAfile(char const*);
	auto releaseMap();
	auto setTGAInfo(cocos2d::sImageTGA*);
	auto setTile(cocos2d::_ccColor3B const&, cocos2d::CCPoint const&);
	auto tileAt(cocos2d::CCPoint const&);
	auto updateAtlasValueAt(cocos2d::CCPoint const&, cocos2d::_ccColor3B const&, unsigned int);
	auto updateAtlasValues();
}
class cocos2d::CCTimer : cocos2d::CCObject {
	auto CCTimer();
	auto getInterval();
	auto getSelector();
	auto initWithScriptHandler(int, float);
	auto initWithTarget(cocos2d::CCObject*, cocos2d::SEL_SCHEDULE);
	auto initWithTarget(cocos2d::CCObject*, cocos2d::SEL_SCHEDULE, float, unsigned int, float);
	auto setInterval(float);
	auto timerWithScriptHandler(int, float);
	auto timerWithTarget(cocos2d::CCObject*, cocos2d::SEL_SCHEDULE);
	auto timerWithTarget(cocos2d::CCObject*, cocos2d::SEL_SCHEDULE, float);
	auto update(float);
}
class cocos2d::CCTintBy : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, short, short, short);
	auto initWithDuration(float, short, short, short);
	auto reverse();
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCTintTo : cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, unsigned char, unsigned char, unsigned char);
	auto initWithDuration(float, unsigned char, unsigned char, unsigned char);
	auto startWithTarget(cocos2d::CCNode*);
	auto update(float);
}
class cocos2d::CCTMXLayer : cocos2d::CCSpriteBatchNode {
	auto CCTMXLayer();
	auto addChild(cocos2d::CCNode*, int, int);
	auto appendTileForGID(unsigned int, cocos2d::CCPoint const&);
	auto atlasIndexForExistantZ(unsigned int);
	auto atlasIndexForNewZ(int);
	auto calculateLayerOffset(cocos2d::CCPoint const&);
	auto create(cocos2d::CCTMXTilesetInfo*, cocos2d::CCTMXLayerInfo*, cocos2d::CCTMXMapInfo*);
	auto getProperties();
	auto getTileSet();
	auto initWithTilesetInfo(cocos2d::CCTMXTilesetInfo*, cocos2d::CCTMXLayerInfo*, cocos2d::CCTMXMapInfo*);
	auto insertTileForGID(unsigned int, cocos2d::CCPoint const&);
	auto parseInternalProperties();
	auto positionAt(cocos2d::CCPoint const&);
	auto positionForHexAt(cocos2d::CCPoint const&);
	auto positionForIsoAt(cocos2d::CCPoint const&);
	auto positionForOrthoAt(cocos2d::CCPoint const&);
	auto propertyNamed(char const*);
	auto releaseMap();
	auto removeChild(cocos2d::CCNode*, bool);
	auto removeTileAt(cocos2d::CCPoint const&);
	auto reusedTileWithRect(cocos2d::CCRect);
	auto setProperties(cocos2d::CCDictionary*);
	auto setTileGID(unsigned int, cocos2d::CCPoint const&);
	auto setTileGID(unsigned int, cocos2d::CCPoint const&, cocos2d::ccTMXTileFlags_);
	auto setTileSet(cocos2d::CCTMXTilesetInfo*);
	auto setupTileSprite(cocos2d::CCSprite*, cocos2d::CCPoint, unsigned int);
	auto setupTiles();
	auto tileAt(cocos2d::CCPoint const&);
	auto tileGIDAt(cocos2d::CCPoint const&);
	auto tileGIDAt(cocos2d::CCPoint const&, cocos2d::ccTMXTileFlags_*);
	auto updateTileForGID(unsigned int, cocos2d::CCPoint const&);
	auto vertexZForPos(cocos2d::CCPoint const&);
}
class cocos2d::CCTMXLayerInfo : cocos2d::CCObject {
	auto CCTMXLayerInfo();
	auto getProperties();
	auto setProperties(cocos2d::CCDictionary*);
}
class cocos2d::CCTMXMapInfo : cocos2d::CCObject, cocos2d::CCSAXDelegator {
	auto CCTMXMapInfo();
	auto endElement(void*, char const*);
	auto formatWithTMXFile(char const*);
	auto formatWithXML(char const*, char const*);
	auto getLayers();
	auto getObjectGroups();
	auto getProperties();
	auto getTileProperties();
	auto getTilesets();
	auto initWithTMXFile(char const*);
	auto initWithXML(char const*, char const*);
	auto internalInit(char const*, char const*);
	auto parseXMLFile(char const*);
	auto parseXMLString(char const*);
	auto setLayers(cocos2d::CCArray*);
	auto setObjectGroups(cocos2d::CCArray*);
	auto setProperties(cocos2d::CCDictionary*);
	auto setTileProperties(cocos2d::CCDictionary*);
	auto setTilesets(cocos2d::CCArray*);
	auto startElement(void*, char const*, char const**);
	auto textHandler(void*, char const*, int);
}
class cocos2d::CCTMXObjectGroup : cocos2d::CCObject {
	auto CCTMXObjectGroup();
	auto getObjects();
	auto getProperties();
	auto objectNamed(char const*);
	auto propertyNamed(char const*);
	auto setObjects(cocos2d::CCArray*);
	auto setProperties(cocos2d::CCDictionary*);
}
class cocos2d::CCTMXTiledMap : cocos2d::CCNode {
	auto CCTMXTiledMap();
	auto buildWithMapInfo(cocos2d::CCTMXMapInfo*);
	auto create(char const*);
	auto createWithXML(char const*, char const*);
	auto getObjectGroups();
	auto getProperties();
	auto initWithTMXFile(char const*);
	auto initWithXML(char const*, char const*);
	auto layerNamed(char const*);
	auto objectGroupNamed(char const*);
	auto parseLayer(cocos2d::CCTMXLayerInfo*, cocos2d::CCTMXMapInfo*);
	auto propertiesForGID(int);
	auto propertyNamed(char const*);
	auto setObjectGroups(cocos2d::CCArray*);
	auto setProperties(cocos2d::CCDictionary*);
	auto tilesetForLayer(cocos2d::CCTMXLayerInfo*, cocos2d::CCTMXMapInfo*);
}
class cocos2d::CCTMXTilesetInfo : cocos2d::CCObject {
	auto CCTMXTilesetInfo();
	auto rectForGID(unsigned int);
}
class cocos2d::CCToggleVisibility : cocos2d::CCActionInstant {
	auto copyWithZone(cocos2d::CCZone*);
	auto create();
	auto update(float);
}
class cocos2d::CCTouch : cocos2d::CCObject {
	auto getDelta();
	auto getLocation();
	auto getLocationInView();
	auto getPreviousLocation();
	auto getPreviousLocationInView();
	auto getStartLocation();
	auto getStartLocationInView();
}
class cocos2d::CCTouchDelegate {
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*);
	auto ccTouchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*);
	auto ccTouchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*);
	auto ccTouchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*);
}
class cocos2d::CCTouchDispatcher : cocos2d::CCObject, cocos2d::EGLTouchDelegate {
	auto addStandardDelegate(cocos2d::CCTouchDelegate*, int);
	auto addTargetedDelegate(cocos2d::CCTouchDelegate*, int, bool);
	auto decrementForcePrio(int);
	auto findHandler(cocos2d::CCTouchDelegate*);
	auto findHandler(cocos2d::CCArray*, cocos2d::CCTouchDelegate*);
	auto forceAddHandler(cocos2d::CCTouchHandler*, cocos2d::CCArray*);
	auto forceRemoveAllDelegates();
	auto forceRemoveDelegate(cocos2d::CCTouchDelegate*);
	auto incrementForcePrio(int);
	auto init();
	auto isDispatchEvents();
	auto rearrangeHandlers(cocos2d::CCArray*);
	auto removeAllDelegates();
	auto removeDelegate(cocos2d::CCTouchDelegate*);
	auto setDispatchEvents(bool);
	auto setPriority(int, cocos2d::CCTouchDelegate*);
	auto touches(cocos2d::CCSet*, cocos2d::CCEvent*, unsigned int);
	auto touchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*);
	auto touchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*);
	auto touchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*);
	auto touchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*);
}
class cocos2d::CCTouchHandler : cocos2d::CCObject {
	auto getDelegate();
	auto getEnabledSelectors();
	auto getPriority();
	auto handlerWithDelegate(cocos2d::CCTouchDelegate*, int);
	auto initWithDelegate(cocos2d::CCTouchDelegate*, int);
	auto setDelegate(cocos2d::CCTouchDelegate*);
	auto setEnalbedSelectors(int);
	auto setPriority(int);
}
class cocos2d::CCTouchScriptHandlerEntry : cocos2d::CCScriptHandlerEntry {
	auto create(int, bool, int, bool);
	auto init(bool, int, bool);
}
class cocos2d::CCTransitionCrossFade : cocos2d::CCTransitionScene {
	auto CCTransitionCrossFade();
	auto create(float, cocos2d::CCScene*);
	auto draw();
	auto onEnter();
	auto onExit();
}
class cocos2d::CCTransitionEaseScene {
}
class cocos2d::CCTransitionFade : cocos2d::CCTransitionScene {
	auto CCTransitionFade();
	auto create(float, cocos2d::CCScene*);
	auto create(float, cocos2d::CCScene*, cocos2d::_ccColor3B const&);
	auto initWithDuration(float, cocos2d::CCScene*);
	auto initWithDuration(float, cocos2d::CCScene*, cocos2d::_ccColor3B const&);
	auto onEnter();
	auto onExit();
}
class cocos2d::CCTransitionFadeBL : cocos2d::CCTransitionFadeTR {
	auto CCTransitionFadeBL();
	auto actionWithSize(cocos2d::CCSize const&);
	auto create(float, cocos2d::CCScene*);
}
class cocos2d::CCTransitionFadeDown : cocos2d::CCTransitionFadeTR {
	auto CCTransitionFadeDown();
	auto actionWithSize(cocos2d::CCSize const&);
	auto create(float, cocos2d::CCScene*);
}
class cocos2d::CCTransitionFadeTR : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	auto CCTransitionFadeTR();
	auto actionWithSize(cocos2d::CCSize const&);
	auto create(float, cocos2d::CCScene*);
	auto easeActionWithAction(cocos2d::CCActionInterval*);
	auto onEnter();
	auto sceneOrder();
}
class cocos2d::CCTransitionFadeUp : cocos2d::CCTransitionFadeTR {
	auto CCTransitionFadeUp();
	auto actionWithSize(cocos2d::CCSize const&);
	auto create(float, cocos2d::CCScene*);
}
class cocos2d::CCTransitionFlipAngular : cocos2d::CCTransitionSceneOriented {
	auto CCTransitionFlipAngular();
	auto create(float, cocos2d::CCScene*);
	auto create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	auto onEnter();
}
class cocos2d::CCTransitionFlipX : cocos2d::CCTransitionSceneOriented {
	auto CCTransitionFlipX();
	auto create(float, cocos2d::CCScene*);
	auto create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	auto onEnter();
}
class cocos2d::CCTransitionFlipY : cocos2d::CCTransitionSceneOriented {
	auto CCTransitionFlipY();
	auto create(float, cocos2d::CCScene*);
	auto create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	auto onEnter();
}
class cocos2d::CCTransitionJumpZoom : cocos2d::CCTransitionScene {
	auto CCTransitionJumpZoom();
	auto create(float, cocos2d::CCScene*);
	auto onEnter();
}
class cocos2d::CCTransitionMoveInB : cocos2d::CCTransitionMoveInL {
	auto CCTransitionMoveInB();
	auto create(float, cocos2d::CCScene*);
	auto initScenes();
}
class cocos2d::CCTransitionMoveInL : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	auto CCTransitionMoveInL();
	auto action();
	auto create(float, cocos2d::CCScene*);
	auto easeActionWithAction(cocos2d::CCActionInterval*);
	auto initScenes();
	auto onEnter();
}
class cocos2d::CCTransitionMoveInR : cocos2d::CCTransitionMoveInL {
	auto CCTransitionMoveInR();
	auto create(float, cocos2d::CCScene*);
	auto initScenes();
}
class cocos2d::CCTransitionMoveInT : cocos2d::CCTransitionMoveInL {
	auto CCTransitionMoveInT();
	auto create(float, cocos2d::CCScene*);
	auto initScenes();
}
class cocos2d::CCTransitionPageTurn : cocos2d::CCTransitionScene {
	auto CCTransitionPageTurn();
	auto actionWithSize(cocos2d::CCSize const&);
	auto create(float, cocos2d::CCScene*, bool);
	auto initWithDuration(float, cocos2d::CCScene*, bool);
	auto onEnter();
	auto sceneOrder();
}
class cocos2d::CCTransitionProgress : cocos2d::CCTransitionScene {
	auto CCTransitionProgress();
	auto create(float, cocos2d::CCScene*);
	auto onEnter();
	auto onExit();
	auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
	auto sceneOrder();
	auto setupTransition();
}
class cocos2d::CCTransitionProgressHorizontal : cocos2d::CCTransitionProgress {
	auto create(float, cocos2d::CCScene*);
	auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
}
class cocos2d::CCTransitionProgressInOut : cocos2d::CCTransitionProgress {
	auto create(float, cocos2d::CCScene*);
	auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
	auto sceneOrder();
	auto setupTransition();
}
class cocos2d::CCTransitionProgressOutIn : cocos2d::CCTransitionProgress {
	auto create(float, cocos2d::CCScene*);
	auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
}
class cocos2d::CCTransitionProgressRadialCCW : cocos2d::CCTransitionProgress {
	auto create(float, cocos2d::CCScene*);
	auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
}
class cocos2d::CCTransitionProgressRadialCW : cocos2d::CCTransitionProgress {
	auto create(float, cocos2d::CCScene*);
	auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
}
class cocos2d::CCTransitionProgressVertical : cocos2d::CCTransitionProgress {
	auto create(float, cocos2d::CCScene*);
	auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture*);
}
class cocos2d::CCTransitionRotoZoom : cocos2d::CCTransitionScene {
	auto CCTransitionRotoZoom();
	auto create(float, cocos2d::CCScene*);
	auto onEnter();
}
class cocos2d::CCTransitionScene : cocos2d::CCScene {
	auto CCTransitionScene();
	auto cleanup();
	auto create(float, cocos2d::CCScene*);
	auto draw();
	auto finish();
	auto hideOutShowIn();
	auto initWithDuration(float, cocos2d::CCScene*);
	auto onEnter();
	auto onExit();
	auto sceneOrder();
	auto setNewScene(float);
}
class cocos2d::CCTransitionSceneOriented : cocos2d::CCTransitionScene {
	auto CCTransitionSceneOriented();
	auto create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	auto initWithDuration(float, cocos2d::CCScene*, cocos2d::tOrientation);
}
class cocos2d::CCTransitionShrinkGrow : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	auto CCTransitionShrinkGrow();
	auto create(float, cocos2d::CCScene*);
	auto easeActionWithAction(cocos2d::CCActionInterval*);
	auto onEnter();
}
class cocos2d::CCTransitionSlideInB : cocos2d::CCTransitionSlideInL {
	auto CCTransitionSlideInB();
	auto action();
	auto create(float, cocos2d::CCScene*);
	auto initScenes();
	auto sceneOrder();
}
class cocos2d::CCTransitionSlideInL : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	auto CCTransitionSlideInL();
	auto action();
	auto create(float, cocos2d::CCScene*);
	auto easeActionWithAction(cocos2d::CCActionInterval*);
	auto initScenes();
	auto onEnter();
	auto sceneOrder();
}
class cocos2d::CCTransitionSlideInR : cocos2d::CCTransitionSlideInL {
	auto CCTransitionSlideInR();
	auto action();
	auto create(float, cocos2d::CCScene*);
	auto initScenes();
	auto sceneOrder();
}
class cocos2d::CCTransitionSlideInT : cocos2d::CCTransitionSlideInL {
	auto CCTransitionSlideInT();
	auto action();
	auto create(float, cocos2d::CCScene*);
	auto initScenes();
	auto sceneOrder();
}
class cocos2d::CCTransitionSplitCols : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	auto CCTransitionSplitCols();
	auto action();
	auto create(float, cocos2d::CCScene*);
	auto easeActionWithAction(cocos2d::CCActionInterval*);
	auto onEnter();
}
class cocos2d::CCTransitionSplitRows : cocos2d::CCTransitionSplitCols {
	auto CCTransitionSplitRows();
	auto action();
	auto create(float, cocos2d::CCScene*);
}
class cocos2d::CCTransitionTurnOffTiles : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	auto CCTransitionTurnOffTiles();
	auto create(float, cocos2d::CCScene*);
	auto easeActionWithAction(cocos2d::CCActionInterval*);
	auto onEnter();
	auto sceneOrder();
}
class cocos2d::CCTransitionZoomFlipAngular : cocos2d::CCTransitionSceneOriented {
	auto CCTransitionZoomFlipAngular();
	auto create(float, cocos2d::CCScene*);
	auto create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	auto onEnter();
}
class cocos2d::CCTransitionZoomFlipX : cocos2d::CCTransitionSceneOriented {
	auto CCTransitionZoomFlipX();
	auto create(float, cocos2d::CCScene*);
	auto create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	auto onEnter();
}
class cocos2d::CCTransitionZoomFlipY : cocos2d::CCTransitionSceneOriented {
	auto CCTransitionZoomFlipY();
	auto create(float, cocos2d::CCScene*);
	auto create(float, cocos2d::CCScene*, cocos2d::tOrientation);
	auto onEnter();
}
class cocos2d::CCTurnOffTiles : cocos2d::CCTiledGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&);
	auto create(float, cocos2d::CCSize const&, unsigned int);
	auto initWithDuration(float, cocos2d::CCSize const&, unsigned int);
	auto shuffle(unsigned int*, unsigned int);
	auto startWithTarget(cocos2d::CCNode*);
	auto turnOffTile(cocos2d::CCPoint const&);
	auto turnOnTile(cocos2d::CCPoint const&);
	auto update(float);
}
class cocos2d::CCTwirl : cocos2d::CCGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, cocos2d::CCPoint, unsigned int, float);
	auto initWithDuration(float, cocos2d::CCSize const&, cocos2d::CCPoint, unsigned int, float);
	auto setPosition(cocos2d::CCPoint const&);
	auto update(float);
}
class cocos2d::CCWaves : cocos2d::CCGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, unsigned int, float, bool, bool);
	auto initWithDuration(float, cocos2d::CCSize const&, unsigned int, float, bool, bool);
	auto update(float);
}
class cocos2d::CCWaves3D : cocos2d::CCGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, unsigned int, float);
	auto initWithDuration(float, cocos2d::CCSize const&, unsigned int, float);
	auto update(float);
}
class cocos2d::CCWavesTiles3D : cocos2d::CCTiledGrid3DAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create(float, cocos2d::CCSize const&, unsigned int, float);
	auto initWithDuration(float, cocos2d::CCSize const&, unsigned int, float);
	auto update(float);
}
class cocos2d::EGLTouchDelegate {
}
class cocos2d::extension::AssetsManager {
	auto AssetsManager(char const*, char const*, char const*);
	auto Helper::Helper();
	auto Helper::handleUpdateSucceed(cocos2d::extension::AssetsManager::_Message*);
	auto Helper::sendMessage(cocos2d::extension::AssetsManager::_Message*);
	auto Helper::update(float);
	auto Helper::~Helper();
	auto checkStoragePath();
	auto checkUpdate();
	auto createDirectory(char const*);
	auto deleteVersion();
	auto downLoad();
	auto getConnectionTimeout();
	auto getPackageUrl();
	auto getStoragePath();
	auto getVersion();
	auto getVersionFileUrl();
	auto sendErrorMessage(cocos2d::extension::AssetsManager::ErrorCode);
	auto setConnectionTimeout(unsigned int);
	auto setDelegate(cocos2d::extension::AssetsManagerDelegateProtocol*);
	auto setPackageUrl(char const*);
	auto setSearchPath();
	auto setStoragePath(char const*);
	auto setVersionFileUrl(char const*);
	auto uncompress();
	auto update();
}
class cocos2d::extension::AssetsManager::Helper : cocos2d::CCObject {
	auto Helper();
	auto handleUpdateSucceed(cocos2d::extension::AssetsManager::_Message*);
	auto sendMessage(cocos2d::extension::AssetsManager::_Message*);
	auto update(float);
}
class cocos2d::extension::CCArrayForObjectSorting : cocos2d::CCArray {
	auto indexOfSortedObject(cocos2d::extension::CCSortableObject*);
	auto insertSortedObject(cocos2d::extension::CCSortableObject*);
	auto objectWithObjectID(unsigned int);
	auto removeSortedObject(cocos2d::extension::CCSortableObject*);
	auto setObjectID_ofSortedObject(unsigned int, cocos2d::extension::CCSortableObject*);
}
class cocos2d::extension::CCColor3bObject : cocos2d::CCObject {
}
class cocos2d::extension::CCControl : cocos2d::CCLayerRGBA {
	auto CCControl();
	auto addHandleOfControlEvent(int, unsigned int);
	auto addTargetWithActionForControlEvent(cocos2d::CCObject*, cocos2d::extension::SEL_CCControlHandler, unsigned int);
	auto addTargetWithActionForControlEvents(cocos2d::CCObject*, cocos2d::extension::SEL_CCControlHandler, unsigned int);
	auto create();
	auto dispatchListforControlEvent(unsigned int);
	auto getHandleOfControlEvent(unsigned int);
	auto getTouchLocation(cocos2d::CCTouch*);
	auto hasVisibleParents();
	auto init();
	auto isEnabled();
	auto isHighlighted();
	auto isOpacityModifyRGB();
	auto isSelected();
	auto isTouchInside(cocos2d::CCTouch*);
	auto needsLayout();
	auto onEnter();
	auto onExit();
	auto registerWithTouchDispatcher();
	auto removeHandleOfControlEvent(unsigned int);
	auto removeTargetWithActionForControlEvent(cocos2d::CCObject*, cocos2d::extension::SEL_CCControlHandler, unsigned int);
	auto removeTargetWithActionForControlEvents(cocos2d::CCObject*, cocos2d::extension::SEL_CCControlHandler, unsigned int);
	auto sendActionsForControlEvents(unsigned int);
	auto setEnabled(bool);
	auto setHighlighted(bool);
	auto setOpacityModifyRGB(bool);
	auto setSelected(bool);
}
class cocos2d::extension::CCControlButton : cocos2d::extension::CCControl {
	auto CCControlButton();
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto create(cocos2d::extension::CCScale9Sprite*);
	auto create(cocos2d::CCNode*, cocos2d::extension::CCScale9Sprite*);
	auto create(gd::string, char const*, float);
	auto create();
	auto doesAdjustBackgroundImage();
	auto getBackgroundSpriteForState(unsigned int);
	auto getColor();
	auto getLabelAnchorPoint();
	auto getOpacity();
	auto getPreferredSize();
	auto getTitleBMFontForState(unsigned int);
	auto getTitleColorForState(unsigned int);
	auto getTitleForState(unsigned int);
	auto getTitleLabelForState(unsigned int);
	auto getTitleTTFForState(unsigned int);
	auto getTitleTTFSizeForState(unsigned int);
	auto getZoomOnTouchDown();
	auto init();
	auto initWithBackgroundSprite(cocos2d::extension::CCScale9Sprite*);
	auto initWithLabelAndBackgroundSprite(cocos2d::CCNode*, cocos2d::extension::CCScale9Sprite*);
	auto initWithTitleAndFontNameAndFontSize(gd::string, char const*, float);
	auto needsLayout();
	auto setAdjustBackgroundImage(bool);
	auto setBackgroundSprite(cocos2d::extension::CCScale9Sprite*);
	auto setBackgroundSpriteForState(cocos2d::extension::CCScale9Sprite*, unsigned int);
	auto setBackgroundSpriteFrameForState(cocos2d::CCSpriteFrame*, unsigned int);
	auto setColor(cocos2d::_ccColor3B const&);
	auto setEnabled(bool);
	auto setHighlighted(bool);
	auto setLabelAnchorPoint(cocos2d::CCPoint);
	auto setMargins(int, int);
	auto setOpacity(unsigned char);
	auto setPreferredSize(cocos2d::CCSize);
	auto setSelected(bool);
	auto setTitleBMFontForState(char const*, unsigned int);
	auto setTitleColorForState(cocos2d::_ccColor3B, unsigned int);
	auto setTitleForState(cocos2d::CCString*, unsigned int);
	auto setTitleLabel(cocos2d::CCNode*);
	auto setTitleLabelForState(cocos2d::CCNode*, unsigned int);
	auto setTitleTTFForState(char const*, unsigned int);
	auto setTitleTTFSizeForState(float, unsigned int);
	auto setZoomOnTouchDown(bool);
}
class cocos2d::extension::CCControlColourPicker : cocos2d::extension::CCControl {
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto colourPicker();
	auto colourSliderValueChanged(cocos2d::CCObject*, unsigned int);
	auto create();
	auto hueSliderValueChanged(cocos2d::CCObject*, unsigned int);
	auto init();
	auto setColorValue(cocos2d::_ccColor3B const&);
	auto updateControlPicker();
	auto updateHueAndControlPicker();
}
class cocos2d::extension::CCControlHuePicker : cocos2d::extension::CCControl {
	auto CCControlHuePicker();
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto checkSliderPosition(cocos2d::CCPoint, bool);
	auto create(cocos2d::CCNode*, cocos2d::CCPoint);
	auto initWithTargetAndPos(cocos2d::CCNode*, cocos2d::CCPoint);
	auto registerWithTouchDispatcher();
	auto setEnabled(bool);
	auto setHue(float);
	auto setHuePercentage(float);
	auto updateSliderPosition(cocos2d::CCPoint);
}
class cocos2d::extension::CCControlPotentiometer : cocos2d::extension::CCControl {
	auto CCControlPotentiometer();
	auto angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::CCPoint);
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto create(char const*, char const*, char const*);
	auto distanceBetweenPointAndPoint(cocos2d::CCPoint, cocos2d::CCPoint);
	auto getMaximumValue();
	auto getMinimumValue();
	auto getValue();
	auto initWithTrackSprite_ProgressTimer_ThumbSprite(cocos2d::CCSprite*, cocos2d::CCProgressTimer*, cocos2d::CCSprite*);
	auto isTouchInside(cocos2d::CCTouch*);
	auto potentiometerBegan(cocos2d::CCPoint);
	auto potentiometerEnded(cocos2d::CCPoint);
	auto potentiometerMoved(cocos2d::CCPoint);
	auto setEnabled(bool);
	auto setMaximumValue(float);
	auto setMinimumValue(float);
	auto setValue(float);
}
class cocos2d::extension::CCControlSaturationBrightnessPicker : cocos2d::extension::CCControl {
	auto CCControlSaturationBrightnessPicker();
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto checkSliderPosition(cocos2d::CCPoint, bool);
	auto create(cocos2d::CCNode*, cocos2d::CCPoint);
	auto initWithTargetAndPos(cocos2d::CCNode*, cocos2d::CCPoint);
	auto registerWithTouchDispatcher();
	auto setEnabled(bool);
	auto updateDraggerWithHSV(cocos2d::extension::HSV);
	auto updateSliderPosition(cocos2d::CCPoint);
	auto updateWithHSV(cocos2d::extension::HSV);
}
class cocos2d::extension::CCControlSlider : cocos2d::extension::CCControl {
	auto CCControlSlider();
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto create(char const*, char const*, char const*);
	auto create(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*);
	auto initWithSprites(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*);
	auto isTouchInside(cocos2d::CCTouch*);
	auto locationFromTouch(cocos2d::CCTouch*);
	auto needsLayout();
	auto setEnabled(bool);
	auto setMaximumValue(float);
	auto setMinimumValue(float);
	auto setValue(float);
	auto sliderBegan(cocos2d::CCPoint);
	auto sliderEnded(cocos2d::CCPoint);
	auto sliderMoved(cocos2d::CCPoint);
	auto valueForLocation(cocos2d::CCPoint);
}
class cocos2d::extension::CCControlStepper : cocos2d::extension::CCControl {
	auto CCControlStepper();
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto create(cocos2d::CCSprite*, cocos2d::CCSprite*);
	auto getValue();
	auto initWithMinusSpriteAndPlusSprite(cocos2d::CCSprite*, cocos2d::CCSprite*);
	auto isContinuous();
	auto setMaximumValue(double);
	auto setMinimumValue(double);
	auto setStepValue(double);
	auto setValue(double);
	auto setValueWithSendingEvent(double, bool);
	auto setWraps(bool);
	auto startAutorepeat();
	auto stopAutorepeat();
	auto update(float);
	auto updateLayoutUsingTouchLocation(cocos2d::CCPoint);
}
class cocos2d::extension::CCControlSwitch : cocos2d::extension::CCControl {
	auto CCControlSwitch();
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto create(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*);
	auto create(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCLabelTTF*, cocos2d::CCLabelTTF*);
	auto initWithMaskSprite(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*);
	auto initWithMaskSprite(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCLabelTTF*, cocos2d::CCLabelTTF*);
	auto locationFromTouch(cocos2d::CCTouch*);
	auto setEnabled(bool);
	auto setOn(bool);
	auto setOn(bool, bool);
}
class cocos2d::extension::CCControlSwitchSprite : cocos2d::CCSprite, cocos2d::CCActionTweenDelegate {
	auto CCControlSwitchSprite();
	auto draw();
	auto initWithMaskSprite(cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCSprite*, cocos2d::CCLabelTTF*, cocos2d::CCLabelTTF*);
	auto needsLayout();
	auto offSideWidth();
	auto onSideWidth();
	auto setSliderXPosition(float);
	auto updateTweenAction(float, char const*);
}
class cocos2d::extension::CCEditBox : cocos2d::extension::CCControlButton, cocos2d::CCIMEDelegate {
	auto CCEditBox();
	auto create(cocos2d::CCSize const&, cocos2d::extension::CCScale9Sprite*, cocos2d::extension::CCScale9Sprite*, cocos2d::extension::CCScale9Sprite*);
	auto getDelegate();
	auto getMaxLength();
	auto getPlaceHolder();
	auto getText();
	auto initWithSizeAndBackgroundSprite(cocos2d::CCSize const&, cocos2d::extension::CCScale9Sprite*);
	auto keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	auto keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	auto keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&);
	auto keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&);
	auto onEnter();
	auto onExit();
	auto registerScriptEditBoxHandler(int);
	auto setAnchorPoint(cocos2d::CCPoint const&);
	auto setContentSize(cocos2d::CCSize const&);
	auto setDelegate(cocos2d::extension::CCEditBoxDelegate*);
	auto setFont(char const*, int);
	auto setFontColor(cocos2d::_ccColor3B const&);
	auto setFontName(char const*);
	auto setFontSize(int);
	auto setInputFlag(cocos2d::extension::EditBoxInputFlag);
	auto setInputMode(cocos2d::extension::EditBoxInputMode);
	auto setMaxLength(int);
	auto setPlaceHolder(char const*);
	auto setPlaceholderFont(char const*, int);
	auto setPlaceholderFontColor(cocos2d::_ccColor3B const&);
	auto setPlaceholderFontName(char const*);
	auto setPlaceholderFontSize(int);
	auto setPosition(cocos2d::CCPoint const&);
	auto setReturnType(cocos2d::extension::KeyboardReturnType);
	auto setText(char const*);
	auto setVisible(bool);
	auto touchDownAction(cocos2d::CCObject*, unsigned int);
	auto unregisterScriptEditBoxHandler();
	auto visit();
}
class cocos2d::extension::CCEditBoxImpl {
}
class cocos2d::extension::CCEditBoxImplAndroid : cocos2d::extension::CCEditBoxImpl {
	auto CCEditBoxImplAndroid(cocos2d::extension::CCEditBox*);
	auto closeKeyboard();
	auto doAnimationWhenKeyboardMove(float, float);
	auto getMaxLength();
	auto getText();
	auto initWithSize(cocos2d::CCSize const&);
	auto isEditing();
	auto onEnter();
	auto openKeyboard();
	auto setAnchorPoint(cocos2d::CCPoint const&);
	auto setContentSize(cocos2d::CCSize const&);
	auto setFont(char const*, int);
	auto setFontColor(cocos2d::_ccColor3B const&);
	auto setInputFlag(cocos2d::extension::EditBoxInputFlag);
	auto setInputMode(cocos2d::extension::EditBoxInputMode);
	auto setMaxLength(int);
	auto setPlaceHolder(char const*);
	auto setPlaceholderFont(char const*, int);
	auto setPlaceholderFontColor(cocos2d::_ccColor3B const&);
	auto setPosition(cocos2d::CCPoint const&);
	auto setReturnType(cocos2d::extension::KeyboardReturnType);
	auto setText(char const*);
	auto setVisible(bool);
	auto visit();
}
class cocos2d::extension::CCHttpClient : cocos2d::CCObject {
	auto CCHttpClient();
	auto destroyInstance();
	auto dispatchResponseCallbacks(float);
	auto getInstance();
	auto lazyInitThreadSemphore();
	auto send(cocos2d::extension::CCHttpRequest*);
}
class cocos2d::extension::CCHttpRequest : cocos2d::CCObject {
	auto CCHttpRequest();
}
class cocos2d::extension::CCHttpResponse : cocos2d::CCObject {
	auto CCHttpResponse(cocos2d::extension::CCHttpRequest*);
}
class cocos2d::extension::CCInvocation : cocos2d::CCObject {
	auto CCInvocation(cocos2d::CCObject*, cocos2d::extension::SEL_CCControlHandler, unsigned int);
	auto create(cocos2d::CCObject*, cocos2d::extension::SEL_CCControlHandler, unsigned int);
	auto invoke(cocos2d::CCObject*);
}
class cocos2d::extension::CCScale9Sprite : cocos2d::CCNodeRGBA {
	auto CCScale9Sprite();
	auto create(cocos2d::CCRect, char const*);
	auto create(char const*);
	auto create(char const*, cocos2d::CCRect);
	auto create(char const*, cocos2d::CCRect, cocos2d::CCRect);
	auto create();
	auto createWithSpriteFrame(cocos2d::CCSpriteFrame*);
	auto createWithSpriteFrame(cocos2d::CCSpriteFrame*, cocos2d::CCRect);
	auto createWithSpriteFrameName(char const*);
	auto createWithSpriteFrameName(char const*, cocos2d::CCRect);
	auto getCapInsets();
	auto getColor();
	auto getInsetBottom();
	auto getInsetLeft();
	auto getInsetRight();
	auto getInsetTop();
	auto getOpacity();
	auto getPreferredSize();
	auto init();
	auto initWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, bool, cocos2d::CCRect);
	auto initWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, cocos2d::CCRect);
	auto initWithFile(cocos2d::CCRect, char const*);
	auto initWithFile(char const*);
	auto initWithFile(char const*, cocos2d::CCRect);
	auto initWithFile(char const*, cocos2d::CCRect, cocos2d::CCRect);
	auto initWithSpriteFrame(cocos2d::CCSpriteFrame*);
	auto initWithSpriteFrame(cocos2d::CCSpriteFrame*, cocos2d::CCRect);
	auto initWithSpriteFrameName(char const*);
	auto initWithSpriteFrameName(char const*, cocos2d::CCRect);
	auto isOpacityModifyRGB();
	auto resizableSpriteWithCapInsets(cocos2d::CCRect);
	auto setBlendAdditive(bool);
	auto setCapInsets(cocos2d::CCRect);
	auto setColor(cocos2d::_ccColor3B const&);
	auto setContentSize(cocos2d::CCSize const&);
	auto setInsetBottom(float);
	auto setInsetLeft(float);
	auto setInsetRight(float);
	auto setInsetTop(float);
	auto setOpacity(unsigned char);
	auto setOpacityModifyRGB(bool);
	auto setPreferredSize(cocos2d::CCSize);
	auto setSpriteFrame(cocos2d::CCSpriteFrame*);
	auto updateCapInset();
	auto updateDisplayedColor(cocos2d::_ccColor3B const&);
	auto updateDisplayedOpacity(unsigned char);
	auto updatePositions();
	auto updateWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, bool, cocos2d::CCRect);
	auto visit();
}
class cocos2d::extension::CCScrollView : cocos2d::CCLayer {
	auto CCScrollView();
	auto addChild(cocos2d::CCNode*);
	auto addChild(cocos2d::CCNode*, int);
	auto addChild(cocos2d::CCNode*, int, int);
	auto afterDraw();
	auto beforeDraw();
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto create(cocos2d::CCSize, cocos2d::CCNode*);
	auto create();
	auto deaccelerateScrolling(float);
	auto getContainer();
	auto getContentOffset();
	auto getContentSize();
	auto getScriptHandler(int);
	auto getViewRect();
	auto getZoomScale();
	auto init();
	auto initWithViewSize(cocos2d::CCSize, cocos2d::CCNode*);
	auto isNodeVisible(cocos2d::CCNode*);
	auto maxContainerOffset();
	auto minContainerOffset();
	auto pause(cocos2d::CCObject*);
	auto performedAnimatedScroll(float);
	auto registerScriptHandler(int, int);
	auto registerWithTouchDispatcher();
	auto relocateContainer(bool);
	auto resume(cocos2d::CCObject*);
	auto setContainer(cocos2d::CCNode*);
	auto setContentOffset(cocos2d::CCPoint, bool);
	auto setContentOffsetInDuration(cocos2d::CCPoint, float);
	auto setContentSize(cocos2d::CCSize const&);
	auto setDirection(cocos2d::extension::CCScrollViewDirection);
	auto setTouchEnabled(bool);
	auto setViewSize(cocos2d::CCSize);
	auto setZoomScale(float);
	auto setZoomScale(float, bool);
	auto setZoomScaleInDuration(float, float);
	auto stoppedAnimatedScroll(cocos2d::CCNode*);
	auto unregisterScriptHandler(int);
	auto updateInset();
	auto visit();
}
class cocos2d::extension::CCScrollViewDelegate {
}
class cocos2d::extension::CCSortableObject {
}
class cocos2d::extension::CCSortedObject : cocos2d::CCObject, cocos2d::extension::CCSortableObject {
	auto getObjectID();
	auto setObjectID(unsigned int);
}
class cocos2d::extension::CCTableView : cocos2d::extension::CCScrollView, cocos2d::extension::CCScrollViewDelegate {
	auto CCTableView();
	auto __indexFromOffset(cocos2d::CCPoint);
	auto __offsetFromIndex(unsigned int);
	auto _addCellIfNecessary(cocos2d::extension::CCTableViewCell*);
	auto _indexFromOffset(cocos2d::CCPoint);
	auto _moveCellOutOfSight(cocos2d::extension::CCTableViewCell*);
	auto _offsetFromIndex(unsigned int);
	auto _setIndexForCell(unsigned int, cocos2d::extension::CCTableViewCell*);
	auto _updateCellPositions();
	auto _updateContentSize();
	auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*);
	auto cellAtIndex(unsigned int);
	auto create(cocos2d::extension::CCTableViewDataSource*, cocos2d::CCSize);
	auto create(cocos2d::extension::CCTableViewDataSource*, cocos2d::CCSize, cocos2d::CCNode*);
	auto dequeueCell();
	auto getVerticalFillOrder();
	auto initWithViewSize(cocos2d::CCSize, cocos2d::CCNode*);
	auto insertCellAtIndex(unsigned int);
	auto reloadData();
	auto removeCellAtIndex(unsigned int);
	auto scrollViewDidScroll(cocos2d::extension::CCScrollView*);
	auto scrollViewDidZoom(cocos2d::extension::CCScrollView*);
	auto setVerticalFillOrder(cocos2d::extension::CCTableViewVerticalFillOrder);
	auto unregisterAllScriptHandler();
	auto updateCellAtIndex(unsigned int);
}
class cocos2d::extension::CCTableViewCell : cocos2d::CCNode, cocos2d::extension::CCSortableObject {
	auto getIdx();
	auto getObjectID();
	auto reset();
	auto setIdx(unsigned int);
	auto setObjectID(unsigned int);
}
class cocos2d::extension::ColorPickerDelegate {
	auto colorValueChanged(cocos2d::_ccColor3B);
}
class cocos2d::extension::WebSocket {
	auto WebSocket();
	auto close();
	auto getReadyState();
	auto init(cocos2d::extension::WebSocket::Delegate const&, gd::string const&, gd::vector<gd::string> const*);
	auto onSocketCallback(lws*, lws_callback_reasons, void*, void*, unsigned int);
	auto onSubThreadEnded();
	auto onSubThreadLoop();
	auto onSubThreadStarted();
	auto onUIThreadReceiveMessage(cocos2d::extension::WsMessage*);
	auto send(unsigned char const*, unsigned int);
	auto send(gd::string const&);
}
class cocos2d::extension::WsThreadHelper : cocos2d::CCObject {
	auto WsThreadHelper();
	auto createThread(cocos2d::extension::WebSocket const&);
	auto joinSubThread();
	auto quitSubThread();
	auto sendMessageToSubThread(cocos2d::extension::WsMessage*);
	auto sendMessageToUIThread(cocos2d::extension::WsMessage*);
	auto update(float);
	auto wsThreadEntryFunc(void*);
}
class cocos2d::ExtraAction : cocos2d::CCFiniteTimeAction {
	auto copyWithZone(cocos2d::CCZone*);
	auto create();
	auto reverse();
	auto step(float);
	auto update(float);
}
class cocos2d::TypeInfo {
}
class cocos2d::ZipFile {
	auto ZipFile(gd::string const&, gd::string const&);
	auto fileExists(gd::string const&);
	auto getFileData(gd::string const&, unsigned long*);
	auto getFileData(gd::string const&, unsigned long*, cocos2d::ZipFilePrivate*);
	auto setFilter(gd::string const&);
	auto setFilter(gd::string const&, cocos2d::ZipFilePrivate*);
}
