

class cocos2d {
	static auto ccGLBlendFunc(GLenum p0, GLenum p1) = mac 0x1ae560;
	static auto ccDrawSolidRect(cocos2d::CCPoint p0, cocos2d::CCPoint p1, cocos2d::_ccColor4F p2) = mac 0xecf00;
}

class cocos2d::CCAccelAmplitude : cocos2d::CCActionInterval {
	virtual auto update(float p0) = mac 0xeea70;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xeea40;
	virtual auto reverse() = mac 0xeeac0;
	auto create(cocos2d::CCAction* p0, float p1);
	auto initWithAction(cocos2d::CCAction* p0, float p1);
}

class cocos2d::CCAccelDeccelAmplitude : cocos2d::CCActionInterval {
	virtual auto update(float p0) = mac 0xee6f0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xee6c0;
	virtual auto reverse() = mac 0xee750;
	auto create(cocos2d::CCAction* p0, float p1);
	auto initWithAction(cocos2d::CCAction* p0, float p1);
}

class cocos2d::CCAccelerometerDelegate {
	virtual auto didAccelerate(cocos2d::CCAcceleration* p0);
}

class cocos2d::CCAction : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x35b890, ios 0x19d964;
	virtual auto setTag(int p0);
	virtual auto isDone() = mac 0x35b870, ios 0x19d958;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x35b850, ios 0x19d948;
	virtual auto stop() = mac 0x35b860, ios 0x19d950;
	virtual auto step(float p0) = mac 0x35b880, ios 0x19d960;
	auto CCAction();
	auto create();
	auto description();
	~CCAction() = mac 0x35b6b0, ios 0x19d838;
}

class cocos2d::CCActionCamera : cocos2d::CCActionInterval {
	virtual auto startWithTarget(cocos2d::CCNode* p0);
	virtual auto reverse() = mac 0x1b2a10;
}

class cocos2d::CCActionEase : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a15e0, ios 0x1a658c;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x2a1580, ios 0x1a652c;
	virtual auto stop() = mac 0x2a15b0, ios 0x1a655c;
	virtual auto reverse() = mac 0x2a1600, ios 0x1a659c;
	virtual auto getInnerAction() = mac 0x2a1620, ios 0x1a65bc;
	auto CCActionEase();
	auto create(cocos2d::CCActionInterval* p0);
	auto initWithAction(cocos2d::CCActionInterval* p0);
	~CCActionEase() = mac 0x2a14d0, ios 0x1a64f0;
}

class cocos2d::CCActionInstant : cocos2d::CCFiniteTimeAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x4542f0, ios 0x112518;
	virtual auto isDone() = mac 0x4542c0, ios 0x112500;
	virtual auto step(float p0) = mac 0x4542d0, ios 0x112508;
	virtual auto reverse() = mac 0x454300, ios 0x11251c;
	auto CCActionInstant();
}

class cocos2d::CCActionInterval : cocos2d::CCFiniteTimeAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto isDone() = mac 0x1f2640, ios 0xf04a0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f2700, ios 0xf0530;
	virtual auto step(float p0) = mac 0x1f2660, ios 0xf04b0;
	virtual auto reverse() = mac 0x1f2720, ios 0xf055c;
	auto CCActionInterval();
	auto create(float p0);
	auto getAmplitudeRate();
	auto initWithDuration(float p0);
	auto setAmplitudeRate(float p0);
}

class cocos2d::CCActionManager : cocos2d::CCObject {
	auto addAction(cocos2d::CCAction* p0, cocos2d::CCNode* p1, bool p2) = mac 0x10bed0;
	virtual auto update(float p0) = mac 0x10c9a0, ios 0x213454;
	auto CCActionManager();
	auto actionAllocWithHashElement(cocos2d::_hashElement* p0);
	auto deleteHashElement(cocos2d::_hashElement* p0);
	auto getActionByTag(unsigned int p0, cocos2d::CCObject* p1);
	auto numberOfRunningActionsInTarget(cocos2d::CCObject* p0);
	auto pauseAllRunningActions();
	auto pauseTarget(cocos2d::CCObject* p0);
	auto removeAction(cocos2d::CCAction* p0);
	auto removeActionAtIndex(unsigned int p0, cocos2d::_hashElement* p1);
	auto removeActionByTag(unsigned int p0, cocos2d::CCObject* p1);
	auto removeAllActions();
	auto removeAllActionsFromTarget(cocos2d::CCObject* p0);
	auto resumeTarget(cocos2d::CCObject* p0);
	auto resumeTargets(cocos2d::CCSet* p0);
	~CCActionManager() = mac 0x10b980, ios 0x21264c;
}

class cocos2d::CCActionTween : cocos2d::CCActionInterval {
	static cocos2d::CCActionTween* create(float p0, char const* p1, float p2, float p3) = mac 0x447590;
	virtual auto update(float p0) = mac 0x447720, ios 0x28e6fc;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x4476f0, ios 0x28e6d0;
	virtual auto reverse() = mac 0x447760, ios 0x28e734;
	auto initWithDuration(float p0, char const* p1, float p2, float p3);
}

class cocos2d::CCActionTweenDelegate {
	~CCActionTweenDelegate();
}

class cocos2d::CCAnimate : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCAnimate* create(cocos2d::CCAnimation* p0) = mac 0x1f8fc0;
	virtual auto update(float p0) = mac 0x1f9610, ios 0xf3c04;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f9570, ios 0xf3b58;
	virtual auto stop() = mac 0x1f95d0, ios 0xf3bc0;
	virtual auto reverse() = mac 0x1f9840, ios 0xf3e04;
	auto CCAnimate();
	auto initWithAnimation(cocos2d::CCAnimation* p0);
}

class cocos2d::CCAnimation : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static auto createWithSpriteFrames(cocos2d::CCArray* p0, float p1) = mac 0x140df0;
	virtual auto getDuration() = mac 0x141460, ios 0x104c2c;
	auto CCAnimation();
	auto addSpriteFrame(cocos2d::CCSpriteFrame* p0);
	auto addSpriteFrameWithFileName(char const* p0);
	auto addSpriteFrameWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1);
	auto create(cocos2d::CCArray* p0, float p1, unsigned int p2);
	auto create();
	auto init();
	auto initWithAnimationFrames(cocos2d::CCArray* p0, float p1, unsigned int p2);
	auto initWithSpriteFrames(cocos2d::CCArray* p0, float p1);
	auto setFrames(cocos2d::CCArray* p0);
	~CCAnimation() = mac 0x141220, ios 0x104bf0;
}

class cocos2d::CCAnimationCache : cocos2d::CCObject {
	auto CCAnimationCache();
	auto addAnimation(cocos2d::CCAnimation* p0, char const* p1);
	auto addAnimationsWithDictionary(cocos2d::CCDictionary* p0, char const* p1);
	auto addAnimationsWithFile(char const* p0);
	auto animationByName(char const* p0);
	auto init();
	auto parseVersion1(cocos2d::CCDictionary* p0);
	auto parseVersion2(cocos2d::CCDictionary* p0);
	auto purgeSharedAnimationCache();
	auto removeAnimationByName(char const* p0);
	auto sharedAnimationCache();
	~CCAnimationCache() = mac 0x244bd0, ios 0x12dc48;
}

class cocos2d::CCAnimationFrame : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	auto CCAnimationFrame();
	auto initWithSpriteFrame(cocos2d::CCSpriteFrame* p0, float p1, cocos2d::CCDictionary* p2);
	~CCAnimationFrame() = mac 0x140ab0, ios 0x104740;
}

class cocos2d::CCApplication : cocos2d::CCApplicationProtocol {
	virtual auto getCurrentLanguage() = mac 0x1a3f40, ios 0x10e508;
	virtual auto getTargetPlatform() = mac 0x1a3f20, ios 0x10e57c;
	virtual auto openURL(char const* p0) = mac 0x1a4550, ios 0x10e7a4;
	virtual auto setAnimationInterval(double p0) = mac 0x1a3ee0, ios 0x10e494;
	static auto sharedApplication() = mac 0x1a3f30;
	~CCApplication() = mac 0x1a3d10, ios 0x10e384;
	auto CCApplication();
	auto run();
}

class cocos2d::CCApplicationProtocol {
	virtual auto applicationWillBecomeActive();
	virtual auto applicationWillResignActive();
	virtual auto trySaveGame();
	virtual auto gameDidSave();
	virtual auto openURL(char const* p0);
	~CCApplicationProtocol();
}

class cocos2d::CCArray : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	auto addObject(cocos2d::CCObject* p0) = mac 0x419f90, ios 0x16504c;
	auto addObjectNew(cocos2d::CCObject* p0) = mac 0x41a450;
	auto addObjectsFromArray(cocos2d::CCArray* p0) = mac 0x41a2d0;
	auto containsObject(cocos2d::CCObject* p0) const = mac 0x41a3e0;
	auto count() const = mac 0x41a2f0, ios 0x1650e8;
	static cocos2d::CCArray* create() = mac 0x419cb0, ios 0x164ec8;
	static auto createWithObject(cocos2d::CCObject* p0) = mac 0x419d50;
	auto fastRemoveObject(cocos2d::CCObject* p0) = mac 0x41a520;
	auto fastRemoveObjectAtIndex(unsigned int p0) = mac 0x41a500;
	auto fastRemoveObjectAtIndexNew(unsigned int p0) = mac 0x41a510;
	auto lastObject() = mac 0x41a360;
	auto objectAtIndex(unsigned int p0) = mac 0x41a340, ios 0x16510c;
	auto removeAllObjects() = mac 0x41a4f0, ios 0x1651f0;
	auto removeLastObject(bool p0) = mac 0x41a470;
	auto removeObject(cocos2d::CCObject* p0, bool p1) = mac 0x41a490;
	auto removeObjectAtIndex(unsigned int p0, bool p1) = mac 0x41a4b0;
	auto stringAtIndex(unsigned int p0) = mac 0x41a320;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0) = mac 0x41a880, ios 0x165398;
	auto CCArray(unsigned int p0);
	auto CCArray();
	auto capacity();
	auto createWithArray(cocos2d::CCArray* p0);
	auto createWithCapacity(unsigned int p0);
	auto createWithContentsOfFile(char const* p0);
	auto createWithContentsOfFileThreadSafe(char const* p0);
	auto exchangeObject(cocos2d::CCObject* p0, cocos2d::CCObject* p1);
	auto exchangeObjectAtIndex(unsigned int p0, unsigned int p1);
	auto indexOfObject(cocos2d::CCObject* p0);
	auto init();
	auto initWithArray(cocos2d::CCArray* p0);
	auto initWithCapacity(unsigned int p0);
	auto initWithObject(cocos2d::CCObject* p0);
	auto insertObject(cocos2d::CCObject* p0, unsigned int p1);
	auto isEqualToArray(cocos2d::CCArray* p0);
	auto randomObject();
	auto reduceMemoryFootprint();
	auto removeObjectsInArray(cocos2d::CCArray* p0);
	auto replaceObjectAtIndex(unsigned int p0, cocos2d::CCObject* p1, bool p2);
	auto reverseObjects();
	~CCArray() = mac 0x41a6c0, ios 0x165290;
}

class cocos2d::CCAtlasNode : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	virtual auto draw() = mac 0x1a4f70;
	virtual auto setOpacity(unsigned char p0) = mac 0x1a51c0;
	virtual auto getColor() = mac 0x1a5060;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x1a50b0;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x1a5240;
	virtual auto isOpacityModifyRGB() = mac 0x1a52f0;
	virtual auto getTextureAtlas() = mac 0x1a54a0;
	virtual auto setTextureAtlas(cocos2d::CCTextureAtlas* p0) = mac 0x1a5460;
	virtual auto getBlendFunc() = mac 0x1a5320;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x1a5340;
	virtual auto getQuadsToDraw() = mac 0x1a54b0;
	virtual auto setQuadsToDraw(unsigned int p0) = mac 0x1a54c0;
	virtual auto updateAtlasValues() = mac 0x1a4f60;
	virtual auto getTexture() = mac 0x1a5430;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x1a5360;
	auto CCAtlasNode();
	auto calculateMaxItems();
	auto create(char const* p0, unsigned int p1, unsigned int p2, unsigned int p3);
	auto initWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1, unsigned int p2, unsigned int p3);
	auto initWithTileFile(char const* p0, unsigned int p1, unsigned int p2, unsigned int p3);
	auto setIgnoreContentScaleFactor(bool p0);
	auto updateBlendFunc();
	auto updateOpacityModifyRGB();
	~CCAtlasNode() = mac 0x1a4a30;
}

class cocos2d::CCAutoreleasePool : cocos2d::CCObject {
	auto CCAutoreleasePool();
	auto addObject(cocos2d::CCObject* p0);
	auto clear();
	auto removeObject(cocos2d::CCObject* p0);
	~CCAutoreleasePool() = mac 0x214100, ios 0x114a1c;
}

class cocos2d::CCBMFontConfiguration : cocos2d::CCObject {
	static cocos2d::CCBMFontConfiguration* create(char const* p0) = mac 0x3450f0;
	auto CCBMFontConfiguration();
	auto description();
	auto getCharacterSet();
	auto initWithFNTfile(char const* p0);
	auto parseCharacterDefinition(gd::string p0, cocos2d::_BMFontDef* p1);
	auto parseCommonArguments(gd::string p0);
	auto parseConfigFile(char const* p0);
	auto parseImageFileName(gd::string p0, char const* p1);
	auto parseInfoArguments(gd::string p0);
	auto parseKerningEntry(gd::string p0);
	auto purgeFontDefDictionary();
	auto purgeKerningDictionary();
	~CCBMFontConfiguration() = mac 0x346130, ios 0x217f2c;
}

class cocos2d::CCBezierBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f6960, ios 0xf21ec;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f67a0, ios 0xf2088;
	virtual auto reverse() = mac 0x1f6b40, ios 0xf2368;
	auto CCBezierBy();
	auto create(float p0, cocos2d::_ccBezierConfig const& p1);
	auto initWithDuration(float p0, cocos2d::_ccBezierConfig const& p1);
}

class cocos2d::CCBezierTo : cocos2d::CCBezierBy {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCBezierTo* create(float p0, cocos2d::_ccBezierConfig const& p1) = mac 0x1f6c10;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f6f30, ios 0xf2690;
	auto initWithDuration(float p0, cocos2d::_ccBezierConfig const& p1);
}

class cocos2d::CCBlendProtocol {}

class cocos2d::CCBlink : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f79f0, ios 0xf2c3c;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f78b0, ios 0xf2b08;
	virtual auto stop() = mac 0x1f7880, ios 0xf2ad4;
	virtual auto reverse() = mac 0x1f7a70, ios 0xf2cc8;
	auto create(float p0, unsigned int p1);
	auto initWithDuration(float p0, unsigned int p1);
}

class cocos2d::CCBool : cocos2d::CCObject {
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0);
	auto create(bool p0);
	~CCBool();
}

class cocos2d::CCCallFunc : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static auto create(cocos2d::CCObject* p0, cocos2d::SEL_CallFunc p1) = mac 0x454d90;
	virtual auto update(float p0) = mac 0x455120, ios 0x112a2c;
	virtual auto initWithTarget(cocos2d::CCObject* p0) = mac 0x454eb0, ios 0x112838;
	virtual auto execute() = mac 0x455130, ios 0x112a38;
	auto CCCallFunc();
	auto create(int p0);
	~CCCallFunc() = mac 0x454f50, ios 0x1128ec;
}

class cocos2d::CCCallFuncN : cocos2d::CCCallFunc, cocos2d::TypeInfo {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto execute() = mac 0x455190;
	virtual auto getClassTypeInfo();
	virtual auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_CallFuncN p1) = mac 0x455330;
	auto create(int p0);
	auto create(cocos2d::CCObject* p0, cocos2d::SEL_CallFuncN p1);
}

class cocos2d::CCCallFuncND : cocos2d::CCCallFuncN {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto getClassTypeInfo();
	virtual auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_CallFuncND p1, void* p2) = mac 0x455560;
	virtual auto execute() = mac 0x4556e0;
	auto create(cocos2d::CCObject* p0, cocos2d::SEL_CallFuncND p1, void* p2);
}

class cocos2d::CCCallFuncO : cocos2d::CCCallFunc, cocos2d::TypeInfo {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static auto create(cocos2d::CCObject* p0, cocos2d::SEL_CallFuncO p1, cocos2d::CCObject* p2) = mac 0x455940;
	virtual auto execute() = mac 0x455910, ios 0x112b40;
	virtual auto getClassTypeInfo();
	virtual auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCObject* p2) = mac 0x455a20, ios 0x112c38;
	auto CCCallFuncO();
	auto create(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCObject* p2);
}

class cocos2d::CCCamera : cocos2d::CCObject {
	auto CCCamera();
	auto description();
	auto getCenterXYZ(float* p0, float* p1, float* p2);
	auto getEyeXYZ(float* p0, float* p1, float* p2);
	auto getUpXYZ(float* p0, float* p1, float* p2);
	auto getZEye();
	auto init();
	auto locate();
	auto restore();
	auto setCenterXYZ(float p0, float p1, float p2);
	auto setEyeXYZ(float p0, float p1, float p2);
	auto setUpXYZ(float p0, float p1, float p2);
	~CCCamera() = mac 0x2e0980, ios 0xfee84;
}

class cocos2d::CCCardinalSplineBy : cocos2d::CCCardinalSplineTo {
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1684b0;
	virtual auto reverse() = mac 0x168160;
	virtual auto updatePosition(cocos2d::CCPoint& p0) = mac 0x168110;
	auto CCCardinalSplineBy();
	auto create(float p0, cocos2d::CCPointArray* p1, float p2);
}

class cocos2d::CCCardinalSplineTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x167c40;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x167aa0;
	virtual auto reverse() = mac 0x167f60;
	virtual auto updatePosition(cocos2d::CCPoint& p0) = mac 0x167f30;
	auto CCCardinalSplineTo();
	auto create(float p0, cocos2d::CCPointArray* p1, float p2);
	auto initWithDuration(float p0, cocos2d::CCPointArray* p1, float p2);
	~CCCardinalSplineTo() = mac 0x167960;
}

class cocos2d::CCCatmullRomBy : cocos2d::CCCardinalSplineBy {
	auto create(float p0, cocos2d::CCPointArray* p1);
	auto initWithDuration(float p0, cocos2d::CCPointArray* p1);
	~CCCatmullRomBy();
}

class cocos2d::CCCatmullRomTo : cocos2d::CCCardinalSplineTo {
	auto create(float p0, cocos2d::CCPointArray* p1);
	auto initWithDuration(float p0, cocos2d::CCPointArray* p1);
	~CCCatmullRomTo();
}

class cocos2d::CCClippingNode : cocos2d::CCNode {
	static cocos2d::CCClippingNode* create() = mac 0x4192a0;
	static cocos2d::CCClippingNode* create(cocos2d::CCNode* p0) = mac 0x419330;
	auto getAlphaThreshold() const = mac 0x419a10;
	auto getStencil() const = mac 0x4199c0;
	auto isInverted() const = mac 0x419a30;
	auto onEnter() = mac 0x419470;
	auto onEnterTransitionDidFinish() = mac 0x4194a0;
	auto onExit() = mac 0x419500;
	auto onExitTransitionDidStart() = mac 0x4194d0;
	auto setAlphaThreshold(float p0) = mac 0x419a20;
	auto setInverted(bool p0) = mac 0x419a40;
	auto setStencil(cocos2d::CCNode* p0) = mac 0x4199d0;
	auto visit() = mac 0x419530;
	virtual auto init() = mac 0x4193e0;
	virtual auto init(cocos2d::CCNode* p0) = mac 0x419400;
	auto CCClippingNode();
	~CCClippingNode() = mac 0x4191e0;
}

class cocos2d::CCComponent : cocos2d::CCObject {
	virtual auto update(float p0) = mac 0x5a510;
	virtual auto init() = mac 0x5a4e0;
	virtual auto onEnter() = mac 0x5a4f0;
	virtual auto onExit() = mac 0x5a500;
	virtual auto serialize(void* p0) = mac 0x5a520;
	virtual auto isEnabled() const = mac 0x5a600;
	virtual auto setEnabled(bool p0) = mac 0x5a610;
	auto CCComponent();
	auto create();
	auto getName();
	auto getOwner();
	auto setName(char const* p0);
	auto setOwner(cocos2d::CCNode* p0);
	~CCComponent() = mac 0x5a420;
}

class cocos2d::CCComponentContainer {
	virtual auto get(char const* p0) const = mac 0x1663a0, ios 0xdc854;
	virtual auto add(cocos2d::CCComponent* p0) = mac 0x166470, ios 0xdc970;
	virtual auto remove(char const* p0) = mac 0x1665f0, ios 0xdcb74;
	virtual auto remove(cocos2d::CCComponent* p0) = mac 0x1668d0, ios 0xdce54;
	virtual auto removeAll() = mac 0x166ab0, ios 0xdd00c;
	virtual auto visit(float p0) = mac 0x166cb0, ios 0xdd1b8;
	auto CCComponentContainer(cocos2d::CCNode* p0);
	auto alloc();
	auto isEmpty();
	~CCComponentContainer();
}

class cocos2d::CCConfiguration : cocos2d::CCObject {
	auto CCConfiguration();
	auto checkForGLExtension(gd::string const& p0);
	auto dumpInfo();
	auto gatherGPUInfo();
	auto getBool(char const* p0, bool p1);
	auto getCString(char const* p0, char const* p1);
	auto getMaxModelviewStackDepth();
	auto getMaxTextureSize();
	auto getMaxTextureUnits();
	auto getNumber(char const* p0, double p1);
	auto getObject(char const* p0);
	auto init();
	auto loadConfigFile(char const* p0);
	auto purgeConfiguration();
	auto setObject(char const* p0, cocos2d::CCObject* p1);
	auto sharedConfiguration();
	auto supportsBGRA8888();
	auto supportsDiscardFramebuffer();
	auto supportsNPOT();
	auto supportsPVRTC();
	auto supportsShareableVAO();
	~CCConfiguration() = mac 0x2a6c60, ios 0x242258;
}

class cocos2d::CCCopying {
	auto copyWithZone(cocos2d::CCZone* p0);
}

class cocos2d::CCDataVisitor {
	virtual auto visit(cocos2d::CCBool const* p0);
	virtual auto visit(cocos2d::CCInteger const* p0);
	virtual auto visit(cocos2d::CCFloat const* p0);
	virtual auto visit(cocos2d::CCDouble const* p0);
	virtual auto visit(cocos2d::CCString const* p0);
	virtual auto visit(cocos2d::CCArray const* p0);
	virtual auto visit(cocos2d::CCDictionary const* p0);
	virtual auto visit(cocos2d::CCSet const* p0);
	~CCDataVisitor();
}

class cocos2d::CCDeccelAmplitude : cocos2d::CCActionInterval {
	virtual auto update(float p0) = mac 0xeedb0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xeed80;
	virtual auto reverse() = mac 0xeee10;
	auto create(cocos2d::CCAction* p0, float p1);
	auto initWithAction(cocos2d::CCAction* p0, float p1);
}

class cocos2d::CCDelayTime : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCDelayTime* create(float p0) = mac 0x1f4380;
	virtual auto update(float p0) = mac 0x1f8af0, ios 0xf36a4;
	virtual auto reverse() = mac 0x1f8b00, ios 0xf36a8;
}

class cocos2d::CCDictionary : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	auto allKeys() = mac 0x190450, ios 0x2de774;
	auto count() = mac 0x190430;
	static cocos2d::CCDictionary* create() = mac 0x192650;
	auto objectForKey(intptr_t p0) = mac 0x190bb0, ios 0x2decc0;
	auto objectForKey(gd::string const& p0) = mac 0x190870, ios 0x2de988;
	auto removeAllObjects() = mac 0x190220;
	auto removeObjectForKey(intptr_t p0) = mac 0x1921d0;
	auto setObject(cocos2d::CCObject* p0, intptr_t p1) = mac 0x191790, ios 0x2df734;
	auto setObject(cocos2d::CCObject* p0, gd::string const& p1) = mac 0x190dc0, ios 0x2dee7c;
	auto valueForKey(intptr_t p0) = mac 0x190cf0;
	auto valueForKey(gd::string const& p0) = mac 0x1907a0;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0) = mac 0x192790, ios 0x2e066c;
	auto CCDictionary();
	auto allKeysForObject(cocos2d::CCObject* p0);
	auto charForKey(gd::string const& p0);
	auto createWithContentsOfFile(char const* p0);
	auto createWithContentsOfFileThreadSafe(char const* p0);
	auto createWithDictionary(cocos2d::CCDictionary* p0);
	auto getFirstKey();
	auto objectForKey(int p0);
	auto randomObject();
	auto removeObjectForElememt(cocos2d::CCDictElement* p0);
	auto removeObjectForKey(int p0);
	auto removeObjectForKey(gd::string const& p0);
	auto removeObjectsForKeys(cocos2d::CCArray* p0);
	auto setObject(cocos2d::CCObject* p0, int p1);
	auto setObjectUnSafe(cocos2d::CCObject* p0, int p1);
	auto setObjectUnSafe(cocos2d::CCObject* p0, gd::string const& p1);
	auto valueForKey(int p0);
	auto writeToFile(char const* p0);
	~CCDictionary() = mac 0x190390, ios 0x2de71c;
}

class cocos2d::CCDirector : cocos2d::CCObject, cocos2d::TypeInfo {
	~CCDirector() = mac 0x2493a0, ios 0xec4e4;
	virtual auto init() = mac 0x248df0, ios 0xec064;
	virtual auto getScheduler() = mac 0x24af00, ios 0xed6bc;
	virtual auto setScheduler(cocos2d::CCScheduler* p0) = mac 0x24aec0, ios 0xed674;
	virtual auto getActionManager() = mac 0x24af50, ios 0xed70c;
	virtual auto setActionManager(cocos2d::CCActionManager* p0) = mac 0x24af10, ios 0xed6c4;
	virtual auto getTouchDispatcher() = mac 0x24afa0, ios 0xed75c;
	virtual auto setTouchDispatcher(cocos2d::CCTouchDispatcher* p0) = mac 0x24af60, ios 0xed714;
	virtual auto getKeypadDispatcher() = mac 0x24b090, ios 0xed828;
	virtual auto setKeypadDispatcher(cocos2d::CCKeypadDispatcher* p0) = mac 0x24b050, ios 0xed7ec;
	virtual auto getKeyboardDispatcher() = mac 0x24aff0, ios 0xed7a0;
	virtual auto setKeyboardDispatcher(cocos2d::CCKeyboardDispatcher* p0) = mac 0x24afb0, ios 0xed764;
	virtual auto getMouseDispatcher() = mac 0x24b040, ios 0xed7e4;
	virtual auto setMouseDispatcher(cocos2d::CCMouseDispatcher* p0) = mac 0x24b000, ios 0xed7a8;
	virtual auto getAccelerometer() = mac 0x24b0e0, ios 0xed894;
	virtual auto setAccelerometer(cocos2d::CCAccelerometer* p0) = mac 0x24b0a0, ios 0xed830;
	virtual auto getDeltaTime() = mac 0x249bd0, ios 0xecbc8;
	virtual auto getSceneDelegate() = mac 0x24b320, ios 0xedaa8;
	virtual auto setSceneDelegate(cocos2d::CCSceneDelegate* p0) = mac 0x24b330, ios 0xedab0;
	auto getWinSize() = mac 0x24a0f0, ios 0xece34;
	auto pushScene(cocos2d::CCScene* p0) = mac 0x24a620;
	auto replaceScene(cocos2d::CCScene* p0) = mac 0x24a6d0, ios 0xed08c;
	auto resetSmoothFixCounter() = mac 0x249bc0;
	static auto sharedDirector() = mac 0x248cb0, ios 0xebf84;
	auto calculateDeltaTime() = mac 0x2497a0;
	auto calculateMPF() = mac 0x19eac0;
	auto convertToGL(cocos2d::CCPoint const& p0) = mac 0x24a210;
	auto drawScene() = mac 0x249690;
	auto setNextScene() = mac 0x2498d0;
	auto showStats() = mac 0x2499e0;
	auto updateContentScale(cocos2d::TextureQuality p0) = mac 0x249ff0;
	auto popSceneWithTransition(float p0, cocos2d::PopTransition p1) = mac 0x24a8b0;
	virtual auto getClassTypeInfo();
	auto CCDirector();
	auto checkSceneReference();
	auto convertToUI(cocos2d::CCPoint const& p0);
	auto createStatsLabel();
	auto end();
	auto getContentScaleFactor();
	auto getDelegate();
	auto getFPSImageData(unsigned char** p0, unsigned int* p1);
	auto getNotificationNode();
	auto getScreenBottom();
	auto getScreenLeft();
	auto getScreenRight();
	auto getScreenScaleFactor();
	auto getScreenScaleFactorH();
	auto getScreenScaleFactorMax();
	auto getScreenScaleFactorW();
	auto getScreenTop();
	auto getVisibleOrigin();
	auto getVisibleSize();
	auto getWinSizeInPixels();
	auto getZEye();
	auto levelForSceneInStack(cocos2d::CCScene* p0);
	auto pause();
	auto popScene();
	auto popToRootScene();
	auto popToSceneInStack(cocos2d::CCScene* p0);
	auto popToSceneStackLevel(int p0);
	auto purgeCachedData();
	auto purgeDirector();
	auto removeStatsLabel();
	auto reshapeProjection(cocos2d::CCSize const& p0);
	auto resume();
	auto runWithScene(cocos2d::CCScene* p0);
	auto sceneCount();
	auto setAlphaBlending(bool p0);
	auto setContentScaleFactor(float p0);
	auto setDefaultValues();
	auto setDelegate(cocos2d::CCDirectorDelegate* p0);
	auto setDeltaTime(float p0);
	auto setDepthTest(bool p0);
	auto setGLDefaultValues();
	auto setNextDeltaTimeZero(bool p0);
	auto setNotificationNode(cocos2d::CCNode* p0);
	auto setOpenGLView(cocos2d::CCEGLView* p0);
	auto setProjection(cocos2d::ccDirectorProjection p0);
	auto setViewport();
	auto setupScreenScale(cocos2d::CCSize p0, cocos2d::CCSize p1, cocos2d::TextureQuality p2);
	auto updateScreenScale(cocos2d::CCSize p0);
	auto willSwitchToScene(cocos2d::CCScene* p0);
}

class cocos2d::CCDisplayLinkDirector : cocos2d::CCDirector {
	virtual auto setAnimationInterval(double p0) = mac 0x24b180, ios 0xed918;
	virtual auto stopAnimation() = mac 0x24b170, ios 0xed90c;
	virtual auto startAnimation() = mac 0x24b0f0, ios 0xed89c;
	virtual auto mainLoop() = mac 0x24b130, ios 0xed8d8;
}

class cocos2d::CCDrawNode : cocos2d::CCNodeRGBA {
	auto clear() = mac 0x379e80;
	static cocos2d::CCDrawNode* create() = mac 0x378d00;
	auto drawPolygon(cocos2d::CCPoint* p0, unsigned int p1, cocos2d::_ccColor4F const& p2, float p3, cocos2d::_ccColor4F const& p4) = mac 0x3797f0;
	auto drawSegment(cocos2d::CCPoint const& p0, cocos2d::CCPoint const& p1, float p2, cocos2d::_ccColor4F const& p3) = mac 0x3792d0;
	auto getBlendFunc() const = mac 0x379ea0;
	auto init() = mac 0x378e00, ios 0x26d00;
	auto setBlendFunc(cocos2d::ccBlendFunc const& p0) = mac 0x379eb0;
	auto draw() = mac 0x379020, ios 0x26eb4;
	~CCDrawNode() = mac 0x378cc0, ios 0x26bc8;
	auto CCDrawNode();
	auto drawDot(cocos2d::CCPoint const& p0, float p1, cocos2d::_ccColor4F const& p2);
	auto ensureCapacity(unsigned int p0);
	auto listenBackToForeground(cocos2d::CCObject* p0);
	auto render();
}

class cocos2d::CCEGLView : cocos2d::CCEGLViewProtocol {
	virtual auto swapBuffers() = mac 0x295510, ios 0xbad54;
	void updateWindow(int width, int height);
	void toggleFullScreen(bool fullscreen);
	void pollEvents();
	void onGLFWCharCallback(GLFWwindow* window, unsigned int entered);
	void onGLFWCursorEnterFunCallback(GLFWwindow* window, int entered);
	void onGLFWDeviceChangeFunCallback(GLFWwindow* window);
	void onGLFWError(int code, const char* description);
	void onGLFWframebuffersize(GLFWwindow* window, int width, int height);
	void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y);
	void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods);
	void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void onGLFWWindowIconifyFunCallback(GLFWwindow* window, int iconified);
	void onGLFWWindowPosCallback(GLFWwindow* window, int x, int y);
	void onGLFWWindowSizeFunCallback(GLFWwindow* window, int width, int height);
	virtual auto end() = mac 0x2954a0, ios 0xbad14;
	virtual auto isOpenGLReady() = mac 0x295470, ios 0xbac80;
	virtual auto setIMEKeyboardState(bool p0) = mac 0x295550, ios 0xbad80;
	virtual auto setViewPortInPoints(float p0, float p1, float p2, float p3) = mac 0x295490, ios 0x20efb4;
	virtual auto setScissorInPoints(float p0, float p1, float p2, float p3) = mac 0x295730, ios 0x20efe8;
	auto CCEGLView();
	auto sharedOpenGLView();
	~CCEGLView();
}

class cocos2d::CCEGLViewProtocol {
	auto getViewPortRect() const = mac 0x29e2f0;
	virtual auto getFrameSize() const = mac 0x29d950, ios 0x20eee4;
	virtual auto setFrameSize(float p0, float p1) = mac 0x29d960, ios 0x20eeec;
	virtual auto getVisibleSize() const = mac 0x29d9a0, ios 0x20ef30;
	virtual auto getVisibleOrigin() const = mac 0x29d9f0, ios 0x20ef60;
	virtual auto setDesignResolutionSize(float p0, float p1, ResolutionPolicy p2) = mac 0x29d7c0, ios 0x20edb0;
	virtual auto getDesignResolutionSize() const = mac 0x29d940, ios 0x20eedc;
	virtual auto setTouchDelegate(cocos2d::EGLTouchDelegate* p0) = mac 0x29da60, ios 0x20efac;
	virtual auto setViewPortInPoints(float p0, float p1, float p2, float p3) = mac 0x29da70, ios 0x20efb4;
	virtual auto setScissorInPoints(float p0, float p1, float p2, float p3) = mac 0x29dab0, ios 0x20efe8;
	virtual auto isScissorEnabled() = mac 0x29daf0, ios 0x20f01c;
	virtual auto getScissorRect() = mac 0x29db10, ios 0x20f03c;
	virtual auto setViewName(char const* p0) = mac 0x29dba0, ios 0x20f0d4;
	virtual auto handleTouchesBegin(int p0, int* p1, float* p2, float* p3) = mac 0x29dbd0, ios 0x20f0f0;
	virtual auto handleTouchesMove(int p0, int* p1, float* p2, float* p3) = mac 0x29deb0, ios 0x20f350;
	virtual auto handleTouchesEnd(int p0, int* p1, float* p2, float* p3) = mac 0x29e1d0, ios 0x20f5cc;
	virtual auto handleTouchesCancel(int p0, int* p1, float* p2, float* p3) = mac 0x29e260, ios 0x20f66c;
	virtual auto pollInputEvents() = mac 0x29e320, ios 0x20f724;
	auto CCEGLViewProtocol();
	auto getScaleX();
	auto getScaleY();
	auto getSetOfTouchesEndOrCancel(cocos2d::CCSet& p0, int p1, int* p2, float* p3, float* p4);
	auto getViewName();
	auto updateDesignResolutionSize();
	~CCEGLViewProtocol();
}

class cocos2d::CCEaseBackIn : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a42a0, ios 0x1a85a4;
	virtual auto reverse() = mac 0x2a42d0, ios 0x1a85d4;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBackInOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a4670, ios 0x1a8914;
	virtual auto reverse() = mac 0x2a4700, ios 0x1a89a0;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBackOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a4480, ios 0x1a8754;
	virtual auto reverse() = mac 0x2a44c0, ios 0x1a8794;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBounce : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto reverse() = mac 0x2a3890;
	auto bounceTime(float p0);
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBounceIn : cocos2d::CCEaseBounce {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a3a40, ios 0x1a804c;
	virtual auto reverse() = mac 0x2a3b20, ios 0x1a808c;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBounceInOut : cocos2d::CCEaseBounce {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a3f40, ios 0x1a83b8;
	virtual auto reverse() = mac 0x2a40f0, ios 0x1a8424;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseBounceOut : cocos2d::CCEaseBounce {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a3cd0, ios 0x1a820c;
	virtual auto reverse() = mac 0x2a3d90, ios 0x1a8238;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseElastic : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto reverse() = mac 0x2a2dd0;
	auto create(cocos2d::CCActionInterval* p0);
	auto create(cocos2d::CCActionInterval* p0, float p1);
	auto initWithAction(cocos2d::CCActionInterval* p0, float p1);
}

class cocos2d::CCEaseElasticIn : cocos2d::CCEaseElastic {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2fb0, ios 0x1a7878;
	virtual auto reverse() = mac 0x2a3050, ios 0x1a7904;
	auto create(cocos2d::CCActionInterval* p0);
	auto create(cocos2d::CCActionInterval* p0, float p1);
}

class cocos2d::CCEaseElasticInOut : cocos2d::CCEaseElastic {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a34f0, ios 0x1a7d00;
	virtual auto reverse() = mac 0x2a3620, ios 0x1a7e00;
	auto create(cocos2d::CCActionInterval* p0);
	auto create(cocos2d::CCActionInterval* p0, float p1);
}

class cocos2d::CCEaseElasticOut : cocos2d::CCEaseElastic {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCEaseElasticOut* create(cocos2d::CCActionInterval* p0, float p1) = mac 0x2a3080;
	virtual auto update(float p0) = mac 0x2a3250, ios 0x1a7abc;
	virtual auto reverse() = mac 0x2a32f0, ios 0x1a7b54;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseExponentialIn : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2160, ios 0x1a6d3c;
	virtual auto reverse() = mac 0x2a21b0, ios 0x1a6da4;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseExponentialInOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2560, ios 0x1a7100;
	virtual auto reverse() = mac 0x2a25d0, ios 0x1a716c;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseExponentialOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2360, ios 0x1a6f24;
	virtual auto reverse() = mac 0x2a23b0, ios 0x1a6f80;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseIn : cocos2d::CCEaseRateAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCEaseIn* create(cocos2d::CCActionInterval* p0, float p1) = mac 0x2a1960;
	virtual auto update(float p0) = mac 0x2a1b10, ios 0x1a6740;
	virtual auto reverse() = mac 0x2a1b40, ios 0x1a6780;
}

class cocos2d::CCEaseInOut : cocos2d::CCEaseRateAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCEaseInOut* create(cocos2d::CCActionInterval* p0, float p1) = mac 0x2a1d80;
	virtual auto update(float p0) = mac 0x2a1f30, ios 0x1a6b30;
	virtual auto reverse() = mac 0x2a1fa0, ios 0x1a6bac;
}

class cocos2d::CCEaseOut : cocos2d::CCEaseRateAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCEaseOut* create(cocos2d::CCActionInterval* p0, float p1) = mac 0x2a1b70;
	virtual auto update(float p0) = mac 0x2a1d20, ios 0x1a6934;
	virtual auto reverse() = mac 0x2a1d50, ios 0x1a697c;
}

class cocos2d::CCEaseRateAction : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto reverse() = mac 0x2a1930;
	auto create(cocos2d::CCActionInterval* p0, float p1);
	auto initWithAction(cocos2d::CCActionInterval* p0, float p1);
	~CCEaseRateAction() = mac 0x2a1880;
}

class cocos2d::CCEaseSineIn : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2780, ios 0x1a72ec;
	virtual auto reverse() = mac 0x2a27c0, ios 0x1a733c;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseSineInOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2b50, ios 0x1a7684;
	virtual auto reverse() = mac 0x2a2b90, ios 0x1a76dc;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCEaseSineOut : cocos2d::CCActionEase {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x2a2970, ios 0x1a74bc;
	virtual auto reverse() = mac 0x2a29a0, ios 0x1a7504;
	auto create(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCFadeIn : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f7c80, ios 0xf2e34;
	virtual auto reverse() = mac 0x1f7ce0, ios 0xf2e98;
	auto create(float p0);
}

class cocos2d::CCFadeOut : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCFadeOut* create(float p0) = mac 0x1f7d80;
	virtual auto update(float p0) = mac 0x1f7ee0, ios 0xf3000;
	virtual auto reverse() = mac 0x1f7f50, ios 0xf306c;
}

class cocos2d::CCFadeOutBLTiles : cocos2d::CCFadeOutTRTiles {
	virtual auto testFunc(cocos2d::CCSize const& p0, float p1) = mac 0x45cf10;
	auto create(float p0, cocos2d::CCSize const& p1);
}

class cocos2d::CCFadeOutDownTiles : cocos2d::CCFadeOutUpTiles {
	virtual auto testFunc(cocos2d::CCSize const& p0, float p1) = mac 0x45d280;
	auto create(float p0, cocos2d::CCSize const& p1);
}

class cocos2d::CCFadeOutTRTiles : cocos2d::CCTiledGrid3DAction {
	virtual auto update(float p0) = mac 0x45ccc0;
	virtual auto testFunc(cocos2d::CCSize const& p0, float p1) = mac 0x45cad0;
	virtual auto transformTile(cocos2d::CCPoint const& p0, float p1) = mac 0x45cbc0;
	auto create(float p0, cocos2d::CCSize const& p1);
	auto turnOffTile(cocos2d::CCPoint const& p0);
	auto turnOnTile(cocos2d::CCPoint const& p0);
}

class cocos2d::CCFadeOutUpTiles : cocos2d::CCFadeOutTRTiles {
	virtual auto testFunc(cocos2d::CCSize const& p0, float p1) = mac 0x45d070;
	virtual auto transformTile(cocos2d::CCPoint const& p0, float p1) = mac 0x45d0f0;
	auto create(float p0, cocos2d::CCSize const& p1);
}

class cocos2d::CCFadeTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCFadeTo* create(float p0, unsigned char p1) = mac 0x1f7ff0;
	virtual auto update(float p0) = mac 0x1f8220, ios 0xf3274;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f81c0, ios 0xf3210;
	auto initWithDuration(float p0, unsigned char p1);
}

class cocos2d::CCFileUtils : cocos2d::TypeInfo {
	static cocos2d::CCFileUtils* sharedFileUtils() = mac 0x377030, ios 0x159450;
	virtual auto getClassTypeInfo();
	virtual auto purgeCachedEntries() = mac 0x23f100, ios 0xcdd6c;
	virtual auto getFileData(char const* p0, char const* p1, unsigned long* p2) = mac 0x23f140, ios 0xcdd9c;
	virtual auto getFileDataFromZip(char const* p0, char const* p1, unsigned long* p2) = mac 0x23f2e0, ios 0xcdf0c;
	virtual auto fullPathForFilename(char const* p0, bool p1) = mac 0x23f940, ios 0xce604;
	virtual auto removeFullPath(char const* p0) = mac 0x240710, ios 0xcf170;
	virtual auto loadFilenameLookupDictionaryFromFile(char const* p0) = mac 0x2416f0, ios 0xd0084;
	virtual auto setFilenameLookupDictionary(cocos2d::CCDictionary* p0) = mac 0x241680, ios 0xd001c;
	virtual auto fullPathFromRelativeFile(char const* p0, char const* p1) = mac 0x2408c0, ios 0xcf270;
	virtual auto setSearchResolutionsOrder(gd::vector<gd::string>const& p0) = mac 0x240ac0, ios 0xcf43c;
	virtual auto addSearchResolutionsOrder(char const* p0) = mac 0x240d30, ios 0xcf658;
	virtual auto getSearchResolutionsOrder() = mac 0x240df0, ios 0xcf75c;
	virtual auto setSearchPaths(gd::vector<gd::string>const& p0) = mac 0x240e10, ios 0xcf76c;
	virtual auto addSearchPath(char const* p0) = mac 0x241180, ios 0xcfb6c;
	virtual auto removeSearchPath(char const* p0) = mac 0x241390, ios 0xcfd50;
	virtual auto getSearchPaths() = mac 0x240e00, ios 0xcf764;
	virtual auto getWritablePath2() = mac 0x241a90, ios 0xd03b0;
	virtual auto isAbsolutePath(gd::string const& p0) = mac 0x2419c0, ios 0xd02ac;
	virtual auto setPopupNotify(bool p0) = mac 0x2419d0, ios 0xd02cc;
	virtual auto isPopupNotify() = mac 0x2419e0, ios 0xd02d8;
	virtual auto init() = mac 0x23f010, ios 0xcdcb4;
	virtual auto getNewFilename(char const* p0) = mac 0x23f3f0, ios 0xce02c;
	virtual auto shouldUseHD() = mac 0x23f4e0, ios 0xce1a0;
	virtual auto addSuffix(gd::string p0, gd::string p1) = mac 0x23f510, ios 0xce1c4;
	virtual auto getPathForFilename(gd::string const& p0, gd::string const& p1, gd::string const& p2) = mac 0x23f650, ios 0xce3d8;
	virtual auto getFullPathForDirectoryAndFilename(gd::string const& p0, gd::string const& p1) = mac 0x241900, ios 0xd023c;
	virtual auto createCCDictionaryWithContentsOfFile(gd::string const& p0) = mac 0x23ec30, ios 0xcdb08;
	virtual auto writeToFile(cocos2d::CCDictionary* p0, gd::string const& p1) = mac 0x23ec40, ios 0xcdb10;
	virtual auto createCCArrayWithContentsOfFile(gd::string const& p0) = mac 0x23ec50, ios 0xcdb18;
	auto CCFileUtils();
	auto purgeFileUtils();
	auto removeAllPaths();
	~CCFileUtils() = mac 0x23efd0, ios 0xcdc78;
}

class cocos2d::CCFiniteTimeAction : cocos2d::CCAction {
	virtual auto reverse();
	~CCFiniteTimeAction();
}

class cocos2d::CCFlipX : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x454910;
	virtual auto reverse() = mac 0x454930;
	auto create(bool p0);
	auto initWithFlipX(bool p0);
}

class cocos2d::CCFlipX3D : cocos2d::CCGrid3DAction {
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0);
	auto initWithDuration(float p0);
	auto initWithSize(cocos2d::CCSize const& p0, float p1);
	auto update(float p0);
}

class cocos2d::CCFlipY : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x454ab0;
	virtual auto reverse() = mac 0x454ad0;
	auto create(bool p0);
	auto initWithFlipY(bool p0);
}

class cocos2d::CCFlipY3D : cocos2d::CCFlipX3D {
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0);
	auto update(float p0);
}

class cocos2d::CCFollow : cocos2d::CCAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto isDone() = mac 0x35c2e0;
	virtual auto stop() = mac 0x35c300;
	virtual auto step(float p0) = mac 0x35c1f0;
	auto create(cocos2d::CCNode* p0, cocos2d::CCRect const& p1);
	auto initWithTarget(cocos2d::CCNode* p0, cocos2d::CCRect const& p1);
}

class cocos2d::CCGLProgram : cocos2d::CCObject {
	auto CCGLProgram();
	auto addAttribute(char const* p0, unsigned int p1);
	auto compileShader(unsigned int* p0, unsigned int p1, char const* p2);
	auto description();
	auto fragmentShaderLog();
	auto getUniformLocationForName(char const* p0);
	auto initWithVertexShaderByteArray(char const* p0, char const* p1);
	auto initWithVertexShaderFilename(char const* p0, char const* p1);
	auto link();
	auto programLog();
	auto reset();
	auto setUniformLocationWith1f(int p0, float p1);
	auto setUniformLocationWith1i(int p0, int p1);
	auto setUniformLocationWith2f(int p0, float p1, float p2);
	auto setUniformLocationWith2fv(int p0, float* p1, unsigned int p2);
	auto setUniformLocationWith2i(int p0, int p1, int p2);
	auto setUniformLocationWith2iv(int p0, int* p1, unsigned int p2);
	auto setUniformLocationWith3f(int p0, float p1, float p2, float p3);
	auto setUniformLocationWith3fv(int p0, float* p1, unsigned int p2);
	auto setUniformLocationWith3i(int p0, int p1, int p2, int p3);
	auto setUniformLocationWith3iv(int p0, int* p1, unsigned int p2);
	auto setUniformLocationWith4f(int p0, float p1, float p2, float p3, float p4);
	auto setUniformLocationWith4fv(int p0, float* p1, unsigned int p2);
	auto setUniformLocationWith4i(int p0, int p1, int p2, int p3, int p4);
	auto setUniformLocationWith4iv(int p0, int* p1, unsigned int p2);
	auto setUniformLocationWithMatrix3fv(int p0, float* p1, unsigned int p2);
	auto setUniformLocationWithMatrix4fv(int p0, float* p1, unsigned int p2);
	auto setUniformsForBuiltins();
	auto updateUniformLocation(int p0, void* p1, unsigned int p2);
	auto updateUniforms();
	auto use();
	auto vertexShaderLog();
	~CCGLProgram() = mac 0x231950, ios 0xbeadc;
}

class cocos2d::CCGrabber : cocos2d::CCObject {
	auto CCGrabber();
	auto afterRender(cocos2d::CCTexture2D* p0);
	auto beforeRender(cocos2d::CCTexture2D* p0);
	auto grab(cocos2d::CCTexture2D* p0);
	~CCGrabber() = mac 0x360fe0;
}

class cocos2d::CCGrid3D : cocos2d::CCGridBase {
	virtual auto blit() = mac 0x2884b0;
	virtual auto reuse() = mac 0x288da0;
	virtual auto calculateVertexPoints() = mac 0x288550;
	auto CCGrid3D();
	auto create(cocos2d::CCSize const& p0);
	auto create(cocos2d::CCSize const& p0, cocos2d::CCTexture2D* p1, bool p2);
	auto originalVertex(cocos2d::CCPoint const& p0);
	auto setVertex(cocos2d::CCPoint const& p0, cocos2d::_ccVertex3F const& p1);
	auto vertex(cocos2d::CCPoint const& p0);
	~CCGrid3D() = mac 0x288470;
}

class cocos2d::CCGrid3DAction : cocos2d::CCGridAction {
	virtual auto getGrid() = mac 0xee2b0;
	auto CCGrid3DAction();
	auto originalVertex(cocos2d::CCPoint const& p0);
	auto setVertex(cocos2d::CCPoint const& p0, cocos2d::_ccVertex3F const& p1);
	auto vertex(cocos2d::CCPoint const& p0);
}

class cocos2d::CCGridAction : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xee0d0;
	virtual auto reverse() = mac 0xee1a0;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1) = mac 0xee0a0;
	virtual auto getGrid() = mac 0xee190;
	auto CCGridAction();
	auto create(float p0, cocos2d::CCSize const& p1);
}

class cocos2d::CCGridBase : cocos2d::CCObject {
	virtual auto blit() = mac 0x2880e0;
	virtual auto reuse() = mac 0x2880f0;
	virtual auto calculateVertexPoints() = mac 0x288100;
	auto CCGridBase();
	auto afterDraw(cocos2d::CCNode* p0);
	auto beforeDraw();
	auto create(cocos2d::CCSize const& p0);
	auto create(cocos2d::CCSize const& p0, cocos2d::CCTexture2D* p1, bool p2);
	auto initWithSize(cocos2d::CCSize const& p0);
	auto initWithSize(cocos2d::CCSize const& p0, cocos2d::CCTexture2D* p1, bool p2);
	auto set2DProjection();
	auto setActive(bool p0);
	auto setTextureFlipped(bool p0);
	~CCGridBase() = mac 0x287e30;
}

class cocos2d::CCHide : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCHide* create() = mac 0x4543e0;
	virtual auto update(float p0);
	virtual auto reverse();
}

class cocos2d::CCIMEDelegate {
	inline cocos2d::CCIMEDelegate::~CCIMEDelegate() {
	    CCIMEDispatcher::sharedDispatcher()->removeDelegate(this);
	}
	inline cocos2d::CCIMEDelegate::CCIMEDelegate() {
		CCIMEDispatcher::sharedDispatcher()->addDelegate(this);
	}

	virtual auto attachWithIME() = mac 0x2776a0, ios 0x12d3d4;
	virtual auto detachWithIME() = mac 0x277880, ios 0x12d4e8;
	virtual auto canAttachWithIME();
	virtual auto didAttachWithIME();
	virtual auto canDetachWithIME();
	virtual auto didDetachWithIME();
	virtual auto insertText(char const* p0, int p1);
	virtual auto deleteBackward();
	virtual auto getContentText();
	virtual auto keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& p0);
	virtual auto keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo& p0);
	virtual auto keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& p0);
	virtual auto keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo& p0);
	auto CCIMEDelegate();
	~CCIMEDelegate();
}

class cocos2d::CCIMEDispatcher {
	static auto sharedDispatcher() = mac 0x2773f0, ios 0x12d170;
	auto addDelegate(cocos2d::CCIMEDelegate* p0) = mac 0x277480, ios 0x12d204;
	auto removeDelegate(cocos2d::CCIMEDelegate* p0) = mac 0x2775f0, ios 0x12d2c4;
}

class cocos2d::CCImage : cocos2d::CCObject {
	CCImage() = mac 0x24fa00;
	~CCImage() = mac 0x24fa80, ios 0x1e598;
	auto initWithImageData(void* p0, int p1, cocos2d::CCImage::EImageFormat p2, int p3, int p4, int p5) = mac 0x24fcb0;
	auto _initWithJpgData(void* p0, int p1);
	auto _initWithPngData(void* p0, int p1);
	auto _initWithRawData(void* p0, int p1, int p2, int p3, int p4, bool p5);
	auto _initWithTiffData(void* p0, int p1);
	auto _initWithWebpData(void* p0, int p1);
	auto _saveImageToJPG(char const* p0);
	auto _saveImageToPNG(char const* p0, bool p1);
	auto initWithImageFile(char const* p0, cocos2d::CCImage::EImageFormat p1);
	auto initWithImageFileThreadSafe(char const* p0, cocos2d::CCImage::EImageFormat p1);
	auto initWithString(char const* p0, int p1, int p2, cocos2d::CCImage::ETextAlign p3, char const* p4, int p5);
	auto initWithStringShadowStroke(char const* p0, int p1, int p2, cocos2d::CCImage::ETextAlign p3, char const* p4, int p5, float p6, float p7, float p8, bool p9, float p10, float p11, float p12, float p13, bool p14, float p15, float p16, float p17, float p18);
	auto saveToFile(char const* p0, bool p1);
}

class cocos2d::CCInteger : cocos2d::CCObject {
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0);
	auto create(int p0);
	~CCInteger();
}

class cocos2d::CCJumpBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f6160;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f6110;
	virtual auto reverse() = mac 0x1f6290;
	auto CCJumpBy();
	auto create(float p0, cocos2d::CCPoint const& p1, float p2, unsigned int p3);
	auto initWithDuration(float p0, cocos2d::CCPoint const& p1, float p2, unsigned int p3);
}

class cocos2d::CCJumpTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual auto update(float p0) = mac 0x45df60;
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
}

class cocos2d::CCJumpTo : cocos2d::CCJumpBy {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f6570;
	auto create(float p0, cocos2d::CCPoint const& p1, float p2, int p3);
}

class cocos2d::CCKeyboardDelegate {
	virtual auto keyDown(cocos2d::enumKeyCodes p0);
	virtual auto keyUp(cocos2d::enumKeyCodes p0);
}

class cocos2d::CCKeyboardDispatcher : cocos2d::CCObject {
	auto dispatchKeyboardMSG(cocos2d::enumKeyCodes p0, bool p1) = mac 0xe8190;
	auto keyToString(cocos2d::enumKeyCodes p0) = mac 0xe8450;
	auto CCKeyboardDispatcher();
	auto addDelegate(cocos2d::CCKeyboardDelegate* p0);
	auto convertKeyCode(cocos2d::enumKeyCodes p0);
	auto forceAddDelegate(cocos2d::CCKeyboardDelegate* p0);
	auto forceRemoveDelegate(cocos2d::CCKeyboardDelegate* p0);
	auto removeDelegate(cocos2d::CCKeyboardDelegate* p0);
	auto updateModifierKeys(bool p0, bool p1, bool p2, bool p3);
	~CCKeyboardDispatcher() = mac 0xe7f90, ios 0x15d8ac;
}

class cocos2d::CCKeyboardHandler : cocos2d::CCObject {
	static cocos2d::CCKeyboardHandler* handlerWithDelegate(cocos2d::CCKeyboardDelegate* p0) = mac 0x242030;
	virtual auto initWithDelegate(cocos2d::CCKeyboardDelegate* p0) = mac 0x241ff0, ios 0x13f8b8;
	~CCKeyboardHandler() = mac 0x241e90, ios 0x13f87c;
	auto getDelegate();
	auto setDelegate(cocos2d::CCKeyboardDelegate* p0);
}

class cocos2d::CCKeypadDelegate {
	virtual auto keyBackClicked();
	virtual auto keyMenuClicked();
}

class cocos2d::CCKeypadDispatcher : cocos2d::CCObject {
	auto CCKeypadDispatcher();
	auto addDelegate(cocos2d::CCKeypadDelegate* p0);
	auto dispatchKeypadMSG(cocos2d::ccKeypadMSGType p0);
	auto forceAddDelegate(cocos2d::CCKeypadDelegate* p0);
	auto forceRemoveDelegate(cocos2d::CCKeypadDelegate* p0);
	auto removeDelegate(cocos2d::CCKeypadDelegate* p0);
	~CCKeypadDispatcher() = mac 0x360110, ios 0x1120e0;
}

class cocos2d::CCKeypadHandler : cocos2d::CCObject {
	static cocos2d::CCKeypadHandler* handlerWithDelegate(cocos2d::CCKeypadDelegate* p0) = mac 0x1ff2d0;
	virtual auto initWithDelegate(cocos2d::CCKeypadDelegate* p0) = mac 0x1ff290, ios 0x69;
	~CCKeypadHandler() = mac 0x1ff130, ios 0x2c530;
	cocos2d::CCKeypadDelegate* getDelegate() = mac 0x1ff0b0;
	auto setDelegate(cocos2d::CCKeypadDelegate* p0);
}

class cocos2d::CCLabelAtlas : cocos2d::CCAtlasNode, cocos2d::CCLabelProtocol {
	virtual auto updateAtlasValues() = mac 0x43d590;
	virtual auto setString(char const* p0) = mac 0x43d840;
	virtual auto getString() = mac 0x43d930;
	auto create(char const* p0, char const* p1);
	auto create(char const* p0, char const* p1, unsigned int p2, unsigned int p3, unsigned int p4);
	auto initWithString(char const* p0, cocos2d::CCTexture2D* p1, unsigned int p2, unsigned int p3, unsigned int p4);
	auto initWithString(char const* p0, char const* p1);
	auto initWithString(char const* p0, char const* p1, unsigned int p2, unsigned int p3, unsigned int p4);
}

class cocos2d::CCLabelBMFont : cocos2d::CCSpriteBatchNode, cocos2d::CCLabelProtocol, cocos2d::CCRGBAProtocol {
	static cocos2d::CCLabelBMFont* create(char const* p0, char const* p1) = mac 0x347660;
	auto limitLabelWidth(float p0, float p1, float p2) = mac 0x34a6e0, ios 0x21b740;
	auto setScale(float p0) = mac 0x34a5d0, ios 0x21b6bc;
	auto setString(char const* p0, bool p1) = mac 0x3489e0, ios 0x21a42c;
	auto setAnchorPoint(cocos2d::CCPoint const& p0) = mac 0x349440, ios 0x21aa7c;
	virtual auto init() = mac 0x347b10, ios 0x2198e0;
	virtual auto setScaleX(float p0) = mac 0x34a5b0, ios 0x21b6e8;
	virtual auto setScaleY(float p0) = mac 0x34a5d0, ios 0x21b714;
	virtual auto setString(char const* p0) = mac 0x348990, ios 0x21a408;
	virtual auto getString() = mac 0x348bf0, ios 0x21a60c;
	virtual auto setCString(char const* p0) = mac 0x348c10, ios 0x21a63c;
	virtual auto updateLabel() = mac 0x349480, ios 0x21aad0;
	virtual auto setAlignment(cocos2d::CCTextAlignment p0) = mac 0x34a530, ios 0x21b68c;
	virtual auto setWidth(float p0) = mac 0x34a550, ios 0x21b69c;
	virtual auto setLineBreakWithoutSpace(bool p0) = mac 0x34a570, ios 0x21b6ac;
	virtual auto isOpacityModifyRGB() = mac 0x3490e0, ios 0x21a8c4;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x348f70, ios 0x21a80c;
	virtual auto getOpacity() = mac 0x348df0, ios 0x21a748;
	virtual auto getDisplayedOpacity() = mac 0x348e10, ios 0x21a758;
	virtual auto setOpacity(unsigned char p0) = mac 0x348e30, ios 0x21a768;
	virtual auto updateDisplayedOpacity(unsigned char p0) = mac 0x349100, ios 0x21a8d4;
	virtual auto isCascadeOpacityEnabled() = mac 0x349400, ios 0x21aa5c;
	virtual auto setCascadeOpacityEnabled(bool p0) = mac 0x349420, ios 0x21aa6c;
	virtual auto getColor() = mac 0x348c30, ios 0x21a648;
	virtual auto getDisplayedColor() = mac 0x348c50, ios 0x21a658;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x348c70, ios 0x21a668;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const& p0) = mac 0x349210, ios 0x21a968;
	virtual auto isCascadeColorEnabled() = mac 0x3493c0, ios 0x21aa3c;
	virtual auto setCascadeColorEnabled(bool p0) = mac 0x3493e0, ios 0x21aa4c;
	virtual auto setString(unsigned short* p0, bool p1) = mac 0x348a60, ios 0x21a4b4;
	auto CCLabelBMFont();
	auto create(char const* p0, char const* p1, float p2);
	auto create(char const* p0, char const* p1, float p2, cocos2d::CCTextAlignment p3);
	auto create(char const* p0, char const* p1, float p2, cocos2d::CCTextAlignment p3, cocos2d::CCPoint p4);
	auto create();
	auto createBatched(char const* p0, char const* p1, cocos2d::CCArray* p2);
	auto createFontChars();
	auto getConfiguration();
	auto getFntFile();
	auto getLetterPosXLeft(cocos2d::CCSprite* p0);
	auto getLetterPosXRight(cocos2d::CCSprite* p0);
	auto initWithString(char const* p0, char const* p1, float p2, cocos2d::CCTextAlignment p3, cocos2d::CCPoint p4);
	auto kerningAmountForFirst(unsigned short p0, unsigned short p1);
	auto purgeCachedData();
	auto setFntFile(char const* p0);
	~CCLabelBMFont() = mac 0x347e80, ios 0x219afc;
}

class cocos2d::CCLabelProtocol {}

class cocos2d::CCLabelTTF : cocos2d::CCSprite, cocos2d::CCLabelProtocol {
	static cocos2d::CCLabelTTF* create() = mac 0x1fa7e0;
	static cocos2d::CCLabelTTF* create(char const* p0, char const* p1, float p2) = mac 0x1fa840;
	auto updateTexture() = mac 0x1fadc0;
	virtual auto setString(char const* p0) = mac 0x1fad70, ios 0x3a558;
	virtual auto init() = mac 0x1fac60, ios 0x3a51c;
	virtual auto getString() = mac 0x1faf70, ios 0x3a6b4;
	auto CCLabelTTF();
	auto _prepareTextDefinition(bool p0);
	auto _updateWithTextDefinition(cocos2d::_ccFontDefinition& p0, bool p1);
	auto create(char const* p0, char const* p1, float p2, cocos2d::CCSize const& p3, cocos2d::CCTextAlignment p4);
	auto create(char const* p0, char const* p1, float p2, cocos2d::CCSize const& p3, cocos2d::CCTextAlignment p4, cocos2d::CCVerticalTextAlignment p5);
	auto createWithFontDefinition(char const* p0, cocos2d::_ccFontDefinition& p1);
	auto description();
	auto disableShadow(bool p0);
	auto disableStroke(bool p0);
	auto enableShadow(cocos2d::CCSize const& p0, float p1, float p2, bool p3);
	auto enableStroke(cocos2d::ccColor3B const& p0, float p1, bool p2);
	auto getDimensions();
	auto getFontName();
	auto getFontSize();
	auto getHorizontalAlignment();
	auto getTextDefinition();
	auto getVerticalAlignment();
	auto initWithString(char const* p0, char const* p1, float p2);
	auto initWithString(char const* p0, char const* p1, float p2, cocos2d::CCSize const& p3, cocos2d::CCTextAlignment p4);
	auto initWithString(char const* p0, char const* p1, float p2, cocos2d::CCSize const& p3, cocos2d::CCTextAlignment p4, cocos2d::CCVerticalTextAlignment p5);
	auto initWithStringAndTextDefinition(char const* p0, cocos2d::_ccFontDefinition& p1);
	auto setDimensions(cocos2d::CCSize const& p0);
	auto setFontFillColor(cocos2d::ccColor3B const& p0, bool p1);
	auto setFontName(char const* p0);
	auto setFontSize(float p0);
	auto setHorizontalAlignment(cocos2d::CCTextAlignment p0);
	auto setTextDefinition(cocos2d::_ccFontDefinition* p0);
	auto setVerticalAlignment(cocos2d::CCVerticalTextAlignment p0);
	~CCLabelTTF() = mac 0x1fa620, ios 0x3a388;
}

class cocos2d::CCLayer : cocos2d::CCNode, cocos2d::CCTouchDelegate, cocos2d::CCAccelerometerDelegate, cocos2d::CCKeypadDelegate, cocos2d::CCKeyboardDelegate, cocos2d::CCMouseDelegate {
	CCLayer() = mac 0x2725b0, ios 0xc7708;
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2734d0, ios 0xc810c;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x273650, ios 0xc82ac;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x2735d0, ios 0xc820c;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x273550, ios 0xc816c;
	virtual auto ccTouchesBegan(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x2736d0, ios 0xc834c;
	virtual auto ccTouchesCancelled(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x273850, ios 0xc852c;
	virtual auto ccTouchesEnded(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x2737d0, ios 0xc848c;
	virtual auto ccTouchesMoved(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x273750, ios 0xc83ec;
	static cocos2d::CCLayer* create() = mac 0x272a00;
	virtual auto didAccelerate(cocos2d::CCAcceleration* p0) = mac 0x272ea0, ios 0xc7c5c;
	virtual auto getTouchMode() = mac 0x272e10, ios 0xc7b9c;
	virtual auto getTouchPriority() = mac 0x272e00, ios 0xc7b94;
	virtual auto init() = mac 0x2729a0, ios 0xc7894;
	virtual auto isAccelerometerEnabled() = mac 0x272e20, ios 0xc7ba4;
	virtual auto isKeyboardEnabled() = mac 0x273010, ios 0xc7d70;
	virtual auto isKeypadEnabled() = mac 0x272f70, ios 0xc7cf4;
	virtual auto isMouseEnabled() = mac 0x273090, ios 0xc7de0;
	virtual auto isTouchEnabled() = mac 0x272ce0, ios 0xc7a54;
	virtual auto keyBackClicked() = mac 0x273160, ios 0xc7e50;
	virtual auto keyDown(cocos2d::enumKeyCodes p0) = mac 0x273280, ios 0xc7efc;
	virtual auto keyMenuClicked() = mac 0x273200, ios 0xc7eac;
	virtual auto onEnter() = mac 0x273300, ios 0xc7f38;
	virtual auto onEnterTransitionDidFinish() = mac 0x273490, ios 0xc80cc;
	virtual auto onExit() = mac 0x2733c0, ios 0xc8004;
	virtual auto registerScriptTouchHandler(int p0, bool p1, int p2, bool p3) = mac 0x272bd0, ios 0xc79c8;
	virtual auto registerWithTouchDispatcher() = mac 0x272b40, ios 0xc7958;
	virtual auto setAccelerometerEnabled(bool p0) = mac 0x272e30, ios 0xc7bac;
	virtual auto setAccelerometerInterval(double p0) = mac 0x272e70, ios 0xc7c10;
	virtual auto setKeyboardEnabled(bool p0) = mac 0x273020, ios 0xc7d78;
	virtual auto setKeypadEnabled(bool p0) = mac 0x272f80, ios 0xc7cfc;
	virtual auto setMouseEnabled(bool p0) = mac 0x2730a0, ios 0xc7de8;
	virtual auto setTouchEnabled(bool p0) = mac 0x272cf0, ios 0xc7a5c;
	virtual auto setTouchMode(cocos2d::ccTouchesMode p0) = mac 0x272d60, ios 0xc7acc;
	virtual auto setTouchPriority(int p0) = mac 0x272db0, ios 0xc7b30;
	virtual auto unregisterScriptTouchHandler() = mac 0x272c30, ios 0xc7a28;
	~CCLayer() = mac 0x2727b0, ios 0xc7848;
	auto excuteScriptTouchHandler(int p0, cocos2d::CCSet* p1);
	auto excuteScriptTouchHandler(int p0, cocos2d::CCTouch* p1);
	auto registerScriptAccelerateHandler(int p0);
	auto registerScriptKeypadHandler(int p0);
	auto unregisterScriptAccelerateHandler();
	auto unregisterScriptKeypadHandler();
}

class cocos2d::CCLayerColor : cocos2d::CCLayerRGBA, cocos2d::CCBlendProtocol {
	CCLayerColor() = mac 0x274320, ios 0xc8aec;
	static cocos2d::CCLayerColor* create(cocos2d::ccColor4B const& p0, float p1, float p2) = mac 0x2745e0;
	auto draw() = mac 0x274b50, ios 0xc8fe0;
	auto getBlendFunc() = mac 0x274480, ios 0xc8bcc;
	auto init() = mac 0x274800, ios 0xc8de8;
	auto initWithColor(cocos2d::ccColor4B const& p0) = mac 0x2749a0, ios 0xc8f14;
	auto initWithColor(cocos2d::ccColor4B const& p0, float p1, float p2) = mac 0x274850, ios 0xc8e34;
	auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x2744a0, ios 0xc8bdc;
	auto setColor(cocos2d::ccColor3B const& p0) = mac 0x274c20, ios 0xc90ac;
	auto setContentSize(cocos2d::CCSize const& p0) = mac 0x2749f0, ios 0xc8f64;
	auto setOpacity(unsigned char p0) = mac 0x274db0, ios 0xc9108;
	auto updateColor() = mac 0x274ae0, ios 0xc8f80;
	~CCLayerColor() = mac 0x2743d0, ios 0x2743e0;
	auto changeHeight(float p0);
	auto changeWidth(float p0);
	auto changeWidthAndHeight(float p0, float p1);
	auto create(cocos2d::ccColor4B const& p0);
	auto create();
}

class cocos2d::CCLayerGradient : cocos2d::CCLayerColor {
	virtual auto init() = mac 0x275280;
	virtual auto updateColor() = mac 0x2753c0;
	virtual auto initWithColor(cocos2d::ccColor4B const& p0, cocos2d::ccColor4B const& p1) = mac 0x2752b0;
	virtual auto initWithColor(cocos2d::ccColor4B const& p0, cocos2d::ccColor4B const& p1, cocos2d::CCPoint const& p2) = mac 0x275310;
	virtual auto getStartColor() = mac 0x275610;
	virtual auto setStartColor(cocos2d::ccColor3B const& p0) = mac 0x275620;
	virtual auto getEndColor() = mac 0x275670;
	virtual auto setEndColor(cocos2d::ccColor3B const& p0) = mac 0x275640;
	virtual auto getStartOpacity() = mac 0x2756a0;
	virtual auto setStartOpacity(unsigned char p0) = mac 0x275680;
	virtual auto getEndOpacity() = mac 0x2756d0;
	virtual auto setEndOpacity(unsigned char p0) = mac 0x2756b0;
	virtual auto getVector() = mac 0x275710;
	virtual auto setVector(cocos2d::CCPoint const& p0) = mac 0x2756e0;
	virtual auto setCompressedInterpolation(bool p0) = mac 0x275730;
	virtual auto isCompressedInterpolation() = mac 0x275720;
	auto CCLayerGradient();
	auto create(cocos2d::ccColor4B const& p0, cocos2d::ccColor4B const& p1);
	auto create(cocos2d::ccColor4B const& p0, cocos2d::ccColor4B const& p1, cocos2d::CCPoint const& p2);
	auto create();
}

class cocos2d::CCLayerMultiplex : cocos2d::CCLayer {
	auto CCLayerMultiplex();
	auto addLayer(cocos2d::CCLayer* p0);
	auto create();
	auto createWithArray(cocos2d::CCArray* p0);
	auto createWithLayer(cocos2d::CCLayer* p0);
	auto initWithArray(cocos2d::CCArray* p0);
	auto switchTo(unsigned int p0);
	auto switchToAndReleaseMe(unsigned int p0);
	~CCLayerMultiplex() = mac 0x275980;
}

class cocos2d::CCLayerRGBA : cocos2d::CCLayer, cocos2d::CCRGBAProtocol {
	CCLayerRGBA() = mac 0x2738d0, ios 0xc85cc;
	virtual auto init() = mac 0x273b40, ios 0xc8de8;
	virtual auto getOpacity() = mac 0x273be0, ios 0xc8698;
	virtual auto getDisplayedOpacity() = mac 0x273c00, ios 0xc8724;
	virtual auto setOpacity(unsigned char p0) = mac 0x273c20, ios 0xc8734;
	virtual auto updateDisplayedOpacity(unsigned char p0) = mac 0x273f20, ios 0xc88d8;
	virtual auto isCascadeOpacityEnabled() = mac 0x2741f0, ios 0xc8aac;
	virtual auto setCascadeOpacityEnabled(bool p0) = mac 0x274210, ios 0xc8abc;
	virtual auto getColor() = mac 0x273d60, ios 0xc87d8;
	virtual auto getDisplayedColor() = mac 0x273d80, ios 0xc87e8;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x273da0, ios 0xc87f8;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const& p0) = mac 0x2740b0, ios 0xc89a4;
	virtual auto isCascadeColorEnabled() = mac 0x274230, ios 0xc8acc;
	virtual auto setCascadeColorEnabled(bool p0) = mac 0x274250, ios 0xc8adc;
	~CCLayerRGBA() = mac 0x273aa0, ios 0xc77b0;
	virtual auto setOpacityModifyRGB(bool p0);
	virtual auto isOpacityModifyRGB();
}

class cocos2d::CCLens3D : cocos2d::CCGrid3DAction {
	virtual auto update(float p0) = mac 0x128140;
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint const& p2, float p3);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint const& p2, float p3);
	auto setPosition(cocos2d::CCPoint const& p0);
}

class cocos2d::CCLiquid : cocos2d::CCGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x128ed0;
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
}

class cocos2d::CCMenu : cocos2d::CCLayerRGBA {
	auto alignItemsHorizontallyWithPadding(float p0) = mac 0x4393e0, ios 0x132508;
	auto alignItemsVerticallyWithPadding(float p0) = mac 0x439190;
	static cocos2d::CCMenu* create() = mac 0x438720, ios 0x131ba4;
	static cocos2d::CCMenu* createWithArray(cocos2d::CCArray* p0) = mac 0x4387e0;
	static cocos2d::CCMenu* createWithItem(cocos2d::CCMenuItem* p0) = mac 0x438b80;
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0x438ba0, ios 0x131ec8;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0x438bb0, ios 0x131ecc;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x438bc0, ios 0x131ed0;
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x438d20, ios 0x131fe8;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x4390b0, ios 0x1322ac;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x438fd0, ios 0x1321bc;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x439050, ios 0x132244;
	virtual auto init() = mac 0x438b90, ios 0x131ec0;
	virtual auto registerWithTouchDispatcher() = mac 0x438cd0, ios 0x131f8c;
	virtual auto onExit() = mac 0x438bd0, ios 0x131ed4;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x438c20, ios 0x15e630;
	auto initWithArray(cocos2d::CCArray* p0) = mac 0x4389f0, ios 0x131d04;
	virtual auto setOpacityModifyRGB(bool p0);
	virtual auto isOpacityModifyRGB();
	virtual auto isEnabled();
	virtual auto setEnabled(bool p0);
	auto alignItemsHorizontally();
	auto alignItemsInColumnsWithArray(cocos2d::CCArray* p0);
	auto alignItemsInRowsWithArray(cocos2d::CCArray* p0);
	auto alignItemsVertically();
	auto itemForTouch(cocos2d::CCTouch* p0);
	auto setHandlerPriority(int p0);
}

class cocos2d::CCMenuItem : cocos2d::CCNodeRGBA {
	~CCMenuItem() = mac 0x1fb8e0, ios 0x2cdf4;
	virtual auto activate() = mac 0x1fba70, ios 0x2ceb0;
	virtual auto selected() = mac 0x1fb9e0, ios 0x2ce2e;
	virtual auto unselected() = mac 0x1fb9f0, ios 0x2ce38;
	virtual auto registerScriptTapHandler(int p0) = mac 0x1fba00, ios 0x2ce40;
	virtual auto unregisterScriptTapHandler() = mac 0x1fba30, ios 0x2ce70;
	virtual auto isEnabled() = mac 0x1fbaf0, ios 0x2cf34;
	virtual auto setEnabled(bool p0) = mac 0x1fbae0, ios 0x2cf2c;
	virtual auto isSelected() = mac 0x1fbb50, ios 0x2cf60;
	auto setTarget(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1) = mac 0x1fbb60;
	auto rect() = mac 0x1fbb00, ios 0x2cf3c;
	auto create(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1);
	auto create();
	auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1);
}

class cocos2d::CCMenuItemAtlasFont : cocos2d::CCMenuItemLabel {
	auto create(char const* p0, char const* p1, int p2, int p3, char p4);
	auto create(char const* p0, char const* p1, int p2, int p3, char p4, cocos2d::CCObject* p5, cocos2d::SEL_MenuHandler p6);
	auto initWithString(char const* p0, char const* p1, int p2, int p3, char p4, cocos2d::CCObject* p5, cocos2d::SEL_MenuHandler p6);
	~CCMenuItemAtlasFont();
}

class cocos2d::CCMenuItemFont : cocos2d::CCMenuItemLabel {
	auto create(char const* p0);
	auto create(char const* p0, cocos2d::CCObject* p1, cocos2d::SEL_MenuHandler p2);
	auto fontName();
	auto fontNameObj();
	auto fontSize();
	auto fontSizeObj();
	auto initWithString(char const* p0, cocos2d::CCObject* p1, cocos2d::SEL_MenuHandler p2);
	auto recreateLabel();
	auto setFontName(char const* p0);
	auto setFontNameObj(char const* p0);
	auto setFontSize(unsigned int p0);
	auto setFontSizeObj(unsigned int p0);
	~CCMenuItemFont();
}

class cocos2d::CCMenuItemImage : cocos2d::CCMenuItemSprite {
	virtual auto init() = mac 0x1fd750;
	auto CCMenuItemImage();
	auto create(char const* p0, char const* p1);
	auto create(char const* p0, char const* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3);
	auto create(char const* p0, char const* p1, char const* p2);
	auto create(char const* p0, char const* p1, char const* p2, cocos2d::CCObject* p3, cocos2d::SEL_MenuHandler p4);
	auto create();
	auto initWithNormalImage(char const* p0, char const* p1, char const* p2, cocos2d::CCObject* p3, cocos2d::SEL_MenuHandler p4);
	auto setDisabledSpriteFrame(cocos2d::CCSpriteFrame* p0);
	auto setNormalSpriteFrame(cocos2d::CCSpriteFrame* p0);
	auto setSelectedSpriteFrame(cocos2d::CCSpriteFrame* p0);
}

class cocos2d::CCMenuItemLabel : cocos2d::CCMenuItem {
	~CCMenuItemLabel() = mac 0x1fc0d0;
	virtual auto activate() = mac 0x1fc240;
	virtual auto selected() = mac 0x1fc2e0;
	virtual auto unselected() = mac 0x1fc380;
	virtual auto setEnabled(bool p0) = mac 0x1fc3f0;
	virtual auto getDisabledColor() = mac 0x1fbb80;
	virtual auto setDisabledColor(cocos2d::ccColor3B const& p0) = mac 0x1fbb90;
	virtual auto getLabel() = mac 0x1fbbb0;
	virtual auto setLabel(cocos2d::CCNode* p0) = mac 0x1fbbc0;
	auto CCMenuItemLabel();
	auto create(cocos2d::CCNode* p0);
	auto create(cocos2d::CCNode* p0, cocos2d::CCObject* p1, cocos2d::SEL_MenuHandler p2);
	auto initWithLabel(cocos2d::CCNode* p0, cocos2d::CCObject* p1, cocos2d::SEL_MenuHandler p2);
	auto setString(char const* p0);
}

class cocos2d::CCMenuItemSprite : cocos2d::CCMenuItem {
	virtual auto selected() = mac 0x1fd3f0, ios 0x02d2cc;
	virtual auto unselected() = mac 0x1fd470, ios 0x2d34c;
	virtual auto setEnabled(bool p0) = mac 0x1fd4e0, ios 0x2d3bc;
	virtual auto getNormalImage() = mac 0x1fcee0, ios 0x2cf68;
	virtual auto setNormalImage(cocos2d::CCNode* p0) = mac 0x1fcef0, ios 0x2cf70;
	virtual auto getSelectedImage() = mac 0x1fcfc0, ios 0x2d044;
	virtual auto setSelectedImage(cocos2d::CCNode* p0) = mac 0x1fcfd0, ios 0x2d04;
	virtual auto getDisabledImage() = mac 0x1fd070, ios 0x2d0fc;
	virtual auto setDisabledImage(cocos2d::CCNode* p0) = mac 0x1fd080, ios 0x2d104;
	virtual auto updateImagesVisibility() = mac 0x1fd510, ios 0x2d3dc;
	bool initWithNormalSprite(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCNode* p2, cocos2d::CCObject* p3, cocos2d::SEL_MenuHandler p4) = mac 0x1fd2f0;
	static auto create(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCNode* p2) = mac 0x1fd120;
	static auto create(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCNode* p2, cocos2d::CCObject* p3, cocos2d::SEL_MenuHandler p4) = mac 0x1fd140;
	static auto create(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3) = mac 0x1fd2d0;
}

class cocos2d::CCMenuItemToggle : cocos2d::CCMenuItem {
	virtual auto activate() = mac 0x1fe950;
	virtual auto selected() = mac 0x1fe8f0;
	virtual auto unselected() = mac 0x1fe920;
	virtual auto setEnabled(bool p0) = mac 0x1fea00;
	virtual auto getSelectedIndex() = mac 0x1fe8e0;
	virtual auto setSelectedIndex(unsigned int p0) = mac 0x1fe7e0;
	virtual auto getSubItems() = mac 0x1fde50;
	virtual auto setSubItems(cocos2d::CCArray* p0) = mac 0x1fde10;
	auto CCMenuItemToggle();
	auto addSubItem(cocos2d::CCMenuItem* p0);
	auto create(cocos2d::CCMenuItem* p0);
	auto create();
	auto createWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1, cocos2d::CCArray* p2);
	auto initWithItem(cocos2d::CCMenuItem* p0);
	auto selectedItem();
	~CCMenuItemToggle() = mac 0x1fe7a0;
}

class cocos2d::CCMotionStreak : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	auto reset() = mac 0x2ee190;
	auto resumeStroke() = mac 0x2edb30;
	auto stopStroke() = mac 0x2edb20;
	bool initWithFade(float fade, float minSeg, float stroke, cocos2d::ccColor3B const& color, cocos2d::CCTexture2D* texture) = mac 0x2ed6f0;
	virtual auto update(float p0) = mac 0x2edb40, ios 0xee07c;
	virtual auto setPosition(cocos2d::CCPoint const& p0) = mac 0x2ed8b0, ios 0xedf5c;
	virtual auto draw() = mac 0x2ee1b0, ios 0xee574;
	virtual auto getOpacity() = mac 0x2eda60, ios 0xedff4;
	virtual auto setOpacity(unsigned char p0) = mac 0x2eda40, ios 0xedfec;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x2eda80, ios 0xee004;
	virtual auto isOpacityModifyRGB() = mac 0x2edaa0, ios 0xee00c;
	virtual auto getTexture() = mac 0x2ed950, ios 0xedf6c;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x2ed970, ios 0xedf7c;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x2eda00, ios 0xedfcc;
	virtual auto getBlendFunc() = mac 0x2eda20, ios 0xedfdc;
	auto CCMotionStreak();
	auto create(float p0, float p1, float p2, cocos2d::ccColor3B const& p3, char const* p4);
	auto create(float p0, float p1, float p2, cocos2d::ccColor3B const& p3, cocos2d::CCTexture2D* p4);
	auto enableRepeatMode(float p0);
	auto initWithFade(float p0, float p1, float p2, cocos2d::ccColor3B const& p3, char const* p4);
	auto setStroke(float p0);
	auto tintWithColor(cocos2d::ccColor3B p0);
	~CCMotionStreak() = mac 0x2ed330, ios 0xedce8;
}

class cocos2d::CCMouseDelegate {
	virtual auto rightKeyDown();
	virtual auto rightKeyUp();
	virtual auto scrollWheel(float p0, float p1);
}

class cocos2d::CCMouseDispatcher : cocos2d::CCObject {
	bool dispatchScrollMSG(float x, float y);
	auto removeDelegate(cocos2d::CCMouseDelegate* delegate);
	auto CCMouseDispatcher();
	auto addDelegate(cocos2d::CCMouseDelegate* p0);
	auto forceAddDelegate(cocos2d::CCMouseDelegate* p0);
	auto forceRemoveDelegate(cocos2d::CCMouseDelegate* p0);
	~CCMouseDispatcher() = mac 0x2e8d40, ios 0x21cc20;
}

class cocos2d::CCMouseHandler : cocos2d::CCObject {
	static cocos2d::CCMouseHandler* handlerWithDelegate(cocos2d::CCMouseDelegate* p0) = mac 0x12ef80;
	virtual auto initWithDelegate(cocos2d::CCMouseDelegate* p0) = mac 0x12ef40, ios 0x43798;
	~CCMouseHandler() = mac 0x12ede0, ios 0x4375c;
	auto getDelegate();
	auto setDelegate(cocos2d::CCMouseDelegate* p0);
}

class cocos2d::CCMoveBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCMoveBy* create(float p0, cocos2d::CCPoint const& p1) = mac 0x1f50e0;
	virtual auto update(float p0) = mac 0x1f5400, ios 0xf1c24;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f5360, ios 0xf1b8c;
	virtual auto reverse() = mac 0x1f53b0, ios 0xf1be0;
	auto CCMoveBy();
	auto initWithDuration(float p0, cocos2d::CCPoint const& p1);
}

class cocos2d::CCMoveTo : cocos2d::CCMoveBy {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCMoveTo* create(float p0, cocos2d::CCPoint const& p1) = mac 0x1f54d0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f5740, ios 0xf1ef8;
	auto initWithDuration(float p0, cocos2d::CCPoint const& p1);
}

class cocos2d::CCNode : cocos2d::CCObject {
	CCNode() = mac 0x122550;
	virtual auto _setZOrder(int p0) = mac 0x122990, ios 0x15dd7c;
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0x1233d0, ios 0x15e5d4;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0x1233b0, ios 0x15e5c4;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x1232a0, ios 0x15e4e4;
	virtual auto addComponent(cocos2d::CCComponent* p0) = mac 0x124a40, ios 0x15f6a4;
	virtual auto cleanup() = mac 0x123100, ios 0x15e3a4;
	virtual auto convertToNodeSpace(cocos2d::CCPoint const& p0) = mac 0x124750, ios 0x15f55c;
	virtual auto convertToWorldSpace(cocos2d::CCPoint const& p0) = mac 0x124790;
	static cocos2d::CCNode* create() = mac 0x1230a0;
	virtual auto draw() = mac 0x123840, ios 0x15e974;
	virtual auto getActionByTag(int p0) = mac 0x123ee0;
	virtual auto getActionManager() = mac 0x123e50, ios 0x15ef54;
	virtual auto getAnchorPoint() = mac 0x122d80, ios 0x15e090;
	virtual auto getAnchorPointInPoints() = mac 0x122d70, ios 0x15e088;
	virtual auto getCamera() = mac 0x122cb0, ios 0x15dfe4;
	virtual auto getChildByTag(int p0) = mac 0x123220, ios 0x15e460;
	virtual auto getChildren() = mac 0x122c80, ios 0x15dfcc;
	virtual auto getChildrenCount() const = mac 0x122c90, ios 0x15dfd4;
	virtual auto getContentSize() const = mac 0x122e00, ios 0x15e118;
	virtual auto getGLServerState() = mac 0x122f90, ios 0x15e248;
	virtual auto getGrid() = mac 0x122d00, ios 0x15e034;
	virtual auto getOrderOfArrival() = mac 0x122f50, ios 0x15e228;
	virtual auto getParent() = mac 0x122ed0, ios 0x15e1e0;
	virtual auto getPosition() = mac 0x122b60, ios 0x15debc;
	virtual auto getPosition(float* p0, float* p1) = mac 0x122b90, ios 0x15dedc;
	virtual auto getPositionX() = mac 0x122be0, ios 0x15df30;
	virtual auto getPositionY() = mac 0x122bf0, ios 0x15df38;
	virtual auto getRotation() = mac 0x122a00, ios 0x15dde4;
	virtual auto getRotationX() = mac 0x122a50, ios 0x15de1c;
	virtual auto getRotationY() = mac 0x122a80, ios 0x15de38;
	virtual auto getScale() = mac 0x122ab0, ios 0x15de54;
	virtual auto getScaleX() = mac 0x122b00, ios 0x15de84;
	virtual auto getScaleY() = mac 0x122b30, ios 0x15dea0;
	virtual auto getScaledContentSize() = mac 0x122e10, ios 0x15e120;
	virtual auto getScheduler() = mac 0x123f70, ios 0x15f008;
	virtual auto getShaderProgram() = mac 0x122f70, ios 0x15e238;
	virtual auto getSkewX() = mac 0x122920, ios 0x15dd3c;
	virtual auto getSkewY() = mac 0x122950, ios 0x15dd58;
	virtual auto getUserData() = mac 0x122f30, ios 0x15e218;
	virtual auto getUserObject() = mac 0x122f80, ios 0x15e240;
	virtual auto getVertexZ() = mac 0x1229e0, ios 0x15ddd4;
	virtual auto getZOrder() = mac 0x122980, ios 0x15dd74;
	virtual auto ignoreAnchorPointForPosition(bool p0) = mac 0x122f00, ios 0x15e1f8;
	virtual auto init() = mac 0x122910, ios 0x15dd34;
	virtual auto isIgnoreAnchorPointForPosition() = mac 0x122ef0, ios 0x15e1f0;
	virtual auto isRunning() = mac 0x122ec0, ios 0x15e1d8;
	virtual auto isVisible() = mac 0x122d50, ios 0x15e078;
	virtual auto nodeToParentTransform() = mac 0x124210, ios 0x15f1a4;
	virtual auto nodeToWorldTransform() = mac 0x124670, ios 0x15f490;
	virtual auto onEnter() = mac 0x123a90, ios 0x15eb94;
	virtual auto onEnterTransitionDidFinish() = mac 0x123b90, ios 0x15ec94;
	virtual auto onExit() = mac 0x123ca0, ios 0x15edb0;
	virtual auto onExitTransitionDidStart() = mac 0x123c00, ios 0x15ed18;
	virtual auto parentToNodeTransform() = mac 0x1245d0, ios 0x15f410;
	virtual auto pauseSchedulerAndActions() = mac 0x123d60;
	virtual auto registerScriptHandler(int p0) = mac 0x123d90, ios 0x15ee94;
	virtual auto removeAllChildren() = mac 0x123600, ios 0x15e788;
	virtual auto removeAllChildrenWithCleanup(bool p0) = mac 0x123620, ios 0x15e798;
	virtual auto removeAllComponents() = mac 0x124aa0, ios 0x15f6d4;
	virtual auto removeChild(cocos2d::CCNode* p0) = mac 0x123460, ios 0x15e630;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x123480, ios 0x15e640;
	virtual auto removeChildByTag(int p0) = mac 0x1235a0, ios 0x15e724;
	virtual auto removeChildByTag(int p0, bool p1) = mac 0x1235c0, ios 0x15e734;
	virtual auto removeComponent(char const* p0) = mac 0x124a60, ios 0x15f6b4;
	virtual auto removeComponent(cocos2d::CCComponent* p0) = mac 0x124a80, ios 0x15f6c4;
	virtual auto removeFromParent() = mac 0x1233f0, ios 0x15e5e8;
	virtual auto removeFromParentAndCleanup(bool p0) = mac 0x123410, ios 0x15e5f8;
	virtual auto removeMeAndCleanup() = mac 0x123440, ios 0x15e620;
	virtual auto reorderChild(cocos2d::CCNode* p0, int p1) = mac 0x123760, ios 0x15e87c;
	virtual auto resumeSchedulerAndActions() = mac 0x123b60;
	virtual auto runAction(cocos2d::CCAction* p0) = mac 0x123e60;
	virtual auto schedule(cocos2d::SEL_SCHEDULE p0) = mac 0x1240b0;
	virtual auto schedule(cocos2d::SEL_SCHEDULE p0, float p1) = mac 0x124120;
	virtual auto scheduleUpdate() = mac 0x123f80;
	virtual auto setActionManager(cocos2d::CCActionManager* p0) = mac 0x123e00, ios 0x15ef04;
	virtual auto setAnchorPoint(cocos2d::CCPoint const& p0) = mac 0x122d90, ios 0x15e098;
	virtual auto setContentSize(cocos2d::CCSize const& p0) = mac 0x122e50, ios 0x15e158;
	virtual auto setGLServerState(cocos2d::ccGLServerState p0) = mac 0x122fa0, ios 0x15e250;
	virtual auto setGrid(cocos2d::CCGridBase* p0) = mac 0x122d10, ios 0x15e03c;
	virtual auto setOrderOfArrival(unsigned int p0) = mac 0x122f60, ios 0x15e230;
	virtual auto setParent(cocos2d::CCNode* p0) = mac 0x122ee0, ios 0x15e1e8;
	virtual auto setPosition(cocos2d::CCPoint const& p0) = mac 0x122b70, ios 0x15dec4;
	virtual auto setPosition(float p0, float p1) = mac 0x122ba0, ios 0x15def0;
	virtual auto setPositionX(float p0) = mac 0x122c00, ios 0x15df40;
	virtual auto setPositionY(float p0) = mac 0x122c40, ios 0x15df84;
	virtual auto setRotation(float p0) = mac 0x122a10, ios 0x15ddec;
	virtual auto setRotationX(float p0) = mac 0x122a60, ios 0x15de24;
	virtual auto setRotationY(float p0) = mac 0x122a90, ios 0x15de40;
	virtual auto setScale(float p0) = mac 0x122ac0, ios 0x15de5c;
	virtual auto setScale(float p0, float p1) = mac 0x122ae0, ios 0x15de70;
	virtual auto setScaleX(float p0) = mac 0x122b10, ios 0x15de8c;
	virtual auto setScaleY(float p0) = mac 0x122b40, ios 0x15dea8;
	virtual auto setScheduler(cocos2d::CCScheduler* p0) = mac 0x123f20, ios 0x15efb8;
	virtual auto setShaderProgram(cocos2d::CCGLProgram* p0) = mac 0x122ff0, ios 0x15e294;
	virtual auto setSkewX(float p0) = mac 0x122930, ios 0x15dd44;
	virtual auto setSkewY(float p0) = mac 0x122960, ios 0x15dd60;
	virtual auto setUserData(void* p0) = mac 0x122f40, ios 0x15e220;
	virtual auto setUserObject(cocos2d::CCObject* p0) = mac 0x122fb0, ios 0x15e258;
	virtual auto setVertexZ(float p0) = mac 0x1229f0, ios 0x15dddc;
	virtual auto setVisible(bool p0) = mac 0x122d60, ios 0x15e080;
	virtual auto setZOrder(int p0) = mac 0x1229a0, ios 0x15dd84;
	virtual auto sortAllChildren() = mac 0x1237b0, ios 0x15e8d4;
	virtual auto stopActionByTag(int p0) = mac 0x123ec0;
	virtual auto stopAllActions() = mac 0x123190;
	virtual auto unregisterScriptHandler() = mac 0x123dc0, ios 0x15eec4;
	virtual auto unschedule(cocos2d::SEL_SCHEDULE p0) = mac 0x124180;
	virtual auto unscheduleAllSelectors() = mac 0x1231b0;
	virtual auto unscheduleUpdate() = mac 0x124060;
	virtual auto update(float p0) = mac 0x1241a0, ios 0x15f124;
	virtual auto updateTransform() = mac 0x1249d0, ios 0x15f648;
	virtual auto updateTweenAction(float p0, char const* p1) = mac 0x1249c0, ios 0x15f644;
	virtual auto visit() = mac 0x123850, ios 0x15e978;
	virtual auto worldToNodeTransform() = mac 0x124710, ios 0x15f51c;
	~CCNode() = mac 0x122750, ios 0x6c98;
	auto boundingBox();
	auto childrenAlloc();
	auto convertToNodeSpaceAR(cocos2d::CCPoint const& p0);
	auto convertToWindowSpace(cocos2d::CCPoint const& p0);
	auto convertToWorldSpaceAR(cocos2d::CCPoint const& p0);
	auto convertTouchToNodeSpace(cocos2d::CCTouch* p0);
	auto convertTouchToNodeSpaceAR(cocos2d::CCTouch* p0);
	auto description();
	auto detachChild(cocos2d::CCNode* p0, bool p1);
	auto getComponent(char const* p0);
	auto insertChild(cocos2d::CCNode* p0, int p1);
	auto numberOfRunningActions();
	auto schedule(cocos2d::SEL_SCHEDULE p0, float p1, unsigned int p2, float p3);
	auto scheduleOnce(cocos2d::SEL_SCHEDULE p0, float p1);
	auto scheduleUpdateWithPriority(int p0);
	auto scheduleUpdateWithPriorityLua(int p0, int p1);
	auto setAdditionalTransform(cocos2d::CCAffineTransform const& p0);
	auto stopAction(cocos2d::CCAction* p0);
	auto transform();
	auto transformAncestors();
}

class cocos2d::CCNodeRGBA : cocos2d::CCNode, cocos2d::CCRGBAProtocol {
	CCNodeRGBA() = mac 0x124b30;
	~CCNodeRGBA() = mac 0x124bb0, ios 0x15f748;
	virtual auto init() = mac 0x124bf0, ios 0x15f780;
	virtual auto getOpacity() = mac 0x124cd0, ios 0x15f7b8;
	virtual auto getDisplayedOpacity() = mac 0x124cf0, ios 0x15f7c8;
	virtual auto setOpacity(unsigned char p0) = mac 0x124d10, ios 0x15f7d8;
	virtual auto updateDisplayedOpacity(unsigned char p0) = mac 0x124e50, ios 0x15f87c;
	virtual auto isCascadeOpacityEnabled() = mac 0x124fe0, ios 0x15f948;
	virtual auto setCascadeOpacityEnabled(bool p0) = mac 0x125000, ios 0x15f958;
	virtual auto getColor() = mac 0x125020, ios 0x15f968;
	virtual auto getDisplayedColor() = mac 0x125040, ios 0x15f978;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x125060, ios 0x15f9888;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const& p0) = mac 0x1251e0, ios 0x15fa68;
	virtual auto isCascadeColorEnabled() = mac 0x125320, ios 0x15fb70;
	virtual auto setCascadeColorEnabled(bool p0) = mac 0x125340, ios 0x15fb80;
	virtual auto setOpacityModifyRGB(bool p0);
	virtual auto isOpacityModifyRGB();
	auto create();
	auto setOpacity(uchar p0);
	auto updateDisplayedOpacity(uchar p0);
}

class cocos2d::CCNotificationCenter : cocos2d::CCObject {
	auto CCNotificationCenter();
	auto addObserver(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1, char const* p2, cocos2d::CCObject* p3);
	auto getObserverHandlerByName(char const* p0);
	auto observerExisted(cocos2d::CCObject* p0, char const* p1);
	auto postNotification(char const* p0);
	auto postNotification(char const* p0, cocos2d::CCObject* p1);
	auto purgeNotificationCenter();
	auto registerScriptObserver(cocos2d::CCObject* p0, int p1, char const* p2);
	auto removeAllObservers(cocos2d::CCObject* p0);
	auto removeObserver(cocos2d::CCObject* p0, char const* p1);
	auto sharedNotificationCenter();
	auto unregisterScriptObserver(cocos2d::CCObject* p0, char const* p1);
	~CCNotificationCenter() = mac 0x131c00;
}

class cocos2d::CCNotificationObserver : cocos2d::CCObject {
	virtual auto getTarget() = mac 0x1326e0;
	virtual auto getSelector() = mac 0x1326f0;
	virtual auto getName() = mac 0x132700;
	virtual auto getObject() = mac 0x132710;
	virtual auto getHandler() = mac 0x132720;
	virtual auto setHandler(int p0) = mac 0x132730;
	auto CCNotificationObserver(cocos2d::CCObject* p0, cocos2d::SEL_MenuHandler p1, char const* p2, cocos2d::CCObject* p3);
	auto performSelector(cocos2d::CCObject* p0);
	~CCNotificationObserver() = mac 0x132640;
}

class cocos2d::CCObject : cocos2d::CCCopying {
	CCObject() = mac 0x250ca0, ios 0x43864;
	auto acceptVisitor(cocos2d::CCDataVisitor& p0) = mac 0x250f30, ios 0x439f0;
	auto autorelease() = mac 0x250ed0, ios 0x439b8;
	auto canEncode() = mac 0x250f90, ios 0x43a24;
	auto encodeWithCoder(DS_Dictionary* p0) = mac 0x250f70, ios 0x43a18;
	virtual auto getTag() const = mac 0x250f50, ios 0x43a08;
	auto isEqual(cocos2d::CCObject const* p0) = mac 0x250f20, ios 0x439e4;
	auto release() = mac 0x250ea0, ios 0x43984;
	auto retain() = mac 0x250ec0, ios 0x439a8;
	virtual auto setTag(int p0) = mac 0x250f60, ios 0x43a10;
	~CCObject() = mac 0x250d20, ios 0x6ac0;
	virtual auto update(float p0);
	auto copy();
	auto createWithCoder(DS_Dictionary* p0);
	auto isSingleReference();
	auto retainCount();
}

class cocos2d::CCOrbitCamera : cocos2d::CCActionCamera {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1b30c0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1b2de0;
	auto CCOrbitCamera();
	auto create(float p0, float p1, float p2, float p3, float p4, float p5, float p6);
	auto initWithDuration(float p0, float p1, float p2, float p3, float p4, float p5, float p6);
	auto sphericalRadius(float* p0, float* p1, float* p2);
}

class cocos2d::CCPageTurn3D : cocos2d::CCGrid3DAction {
	virtual auto update(float p0) = mac 0x194130;
	auto create(float p0, cocos2d::CCSize const& p1);
}

class cocos2d::CCParallaxNode : cocos2d::CCNode {
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x2712c0;
	virtual auto removeAllChildrenWithCleanup(bool p0) = mac 0x271350;
	virtual auto visit() = mac 0x271410;
	virtual auto addChild(cocos2d::CCNode* p0, unsigned int p1, cocos2d::CCPoint const& p2, cocos2d::CCPoint const& p3) = mac 0x271100;
	virtual auto addChild(cocos2d::CCNode* p0, unsigned int p1, int p2) = mac 0x2710f0;
	auto CCParallaxNode();
	auto absolutePosition();
	auto create();
	~CCParallaxNode() = mac 0x270fe0;
}

class cocos2d::CCParticleBatchNode : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0x206360;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0x206370;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x206380;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x206bd0;
	virtual auto removeAllChildrenWithCleanup(bool p0) = mac 0x206d10;
	virtual auto reorderChild(cocos2d::CCNode* p0, int p1) = mac 0x2067c0;
	virtual auto draw() = mac 0x206da0;
	virtual auto visit() = mac 0x2062e0;
	virtual auto getTexture() = mac 0x206f90;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x206ee0;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x206fc0;
	virtual auto getBlendFunc() = mac 0x206fe0;
	auto CCParticleBatchNode();
	auto addChildHelper(cocos2d::CCParticleSystem* p0, int p1, int p2);
	auto create(char const* p0, unsigned int p1);
	auto createWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1);
	auto disableParticle(unsigned int p0);
	auto getCurrentIndex(unsigned int* p0, unsigned int* p1, cocos2d::CCNode* p2, int p3);
	auto increaseAtlasCapacityTo(unsigned int p0);
	auto initWithFile(char const* p0, unsigned int p1);
	auto initWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1);
	auto insertChild(cocos2d::CCParticleSystem* p0, unsigned int p1);
	auto removeChildAtIndex(unsigned int p0, bool p1);
	auto searchNewPositionInChildrenForZ(int p0);
	auto updateAllAtlasIndexes();
	auto updateBlendFunc();
	~CCParticleBatchNode() = mac 0x205ff0;
}

class cocos2d::CCParticleExplosion : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24cad0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleFire : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24b570;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleFireworks : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24b910;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleFlower : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24c1a0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleGalaxy : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24be90;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleMeteor : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24c4c0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleRain : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24d3f0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleSmoke : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24cdd0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleSnow : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24d0d0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleSpiral : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24c7e0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleSun : cocos2d::CCParticleSystemQuad {
	virtual auto init();
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x24bbb0;
	auto create();
	auto createWithTotalParticles(unsigned int p0);
}

class cocos2d::CCParticleSystem : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	auto resetSystem() = mac 0x46bd50;
	auto resumeSystem() = mac 0x46bd40;
	auto stopSystem() = mac 0x46bd10;
	virtual auto update(float p0) = mac 0x46be50, ios 0xf8fc4;
	virtual auto init() = mac 0x4692b0, ios 0xf7040;
	virtual auto setScaleX(float p0) = mac 0x46cdf0, ios 0xf99b8;
	virtual auto setScaleY(float p0) = mac 0x46ce10, ios 0xf99c4;
	virtual auto setScale(float p0) = mac 0x46cdb0, ios 0xf99a0;
	virtual auto setRotation(float p0) = mac 0x46cdd0, ios 0xf99ac;
	virtual auto getBatchNode() = mac 0x46ccd0, ios 0xf995c;
	virtual auto setBatchNode(cocos2d::CCParticleBatchNode* p0) = mac 0x46cce0, ios 0xf9964;
	virtual auto getParticleCount() = mac 0x46c8a0, ios 0xf9734;
	virtual auto getDuration() = mac 0x46c8b0, ios 0xf973c;
	virtual auto setDuration(float p0) = mac 0x46c8c0, ios 0xf9744;
	virtual auto getSourcePosition() = mac 0x46c8d0, ios 0xf974c;
	virtual auto setSourcePosition(cocos2d::CCPoint const& p0) = mac 0x46c8e0, ios 0xf9754;
	virtual auto getPosVar() = mac 0x46c900, ios 0xf975c;
	virtual auto setPosVar(cocos2d::CCPoint const& p0) = mac 0x46c910, ios 0xf9764;
	virtual auto getLife() = mac 0x46c930, ios 0xf976c;
	virtual auto setLife(float p0) = mac 0x46c940, ios 0xf9774;
	virtual auto getLifeVar() = mac 0x46c950, ios 0xf977c;
	virtual auto setLifeVar(float p0) = mac 0x46c960, ios 0xf9784;
	virtual auto getAngle() = mac 0x46c970, ios 0xf978c;
	virtual auto setAngle(float p0) = mac 0x46c980, ios 0xf9794;
	virtual auto getAngleVar() = mac 0x46c990, ios 0xf979c;
	virtual auto setAngleVar(float p0) = mac 0x46c9a0, ios 0xf97a4;
	virtual auto getGravity() = mac 0x46c780, ios 0xf96a4;
	virtual auto setGravity(cocos2d::CCPoint const& p0) = mac 0x46c760, ios 0xf969c;
	virtual auto getSpeed() = mac 0x46c7a0, ios 0xf96b4;
	virtual auto setSpeed(float p0) = mac 0x46c790, ios 0xf96ac;
	virtual auto getSpeedVar() = mac 0x46c7c0, ios 0xf96c4;
	virtual auto setSpeedVar(float p0) = mac 0x46c7b0, ios 0xf96bc;
	virtual auto getTangentialAccel() = mac 0x46c6d0, ios 0xf9654;
	virtual auto setTangentialAccel(float p0) = mac 0x46c6c0, ios 0xf964c;
	virtual auto getTangentialAccelVar() = mac 0x46c6f0, ios 0xf9664;
	virtual auto setTangentialAccelVar(float p0) = mac 0x46c6e0, ios 0xf965c;
	virtual auto getRadialAccel() = mac 0x46c710, ios 0xf9674;
	virtual auto setRadialAccel(float p0) = mac 0x46c700, ios 0xf966c;
	virtual auto getRadialAccelVar() = mac 0x46c730, ios 0xf9684;
	virtual auto setRadialAccelVar(float p0) = mac 0x46c720, ios 0xf967c;
	virtual auto getRotationIsDir() = mac 0x46c750, ios 0xf9694;
	virtual auto setRotationIsDir(bool p0) = mac 0x46c740, ios 0xf968c;
	virtual auto getStartRadius() = mac 0x46c7e0, ios 0xf96d4;
	virtual auto setStartRadius(float p0) = mac 0x46c7d0, ios 0xf96cc;
	virtual auto getStartRadiusVar() = mac 0x46c800, ios 0xf96e4;
	virtual auto setStartRadiusVar(float p0) = mac 0x46c7f0, ios 0xf96dc;
	virtual auto getEndRadius() = mac 0x46c820, ios 0xf96f4;
	virtual auto setEndRadius(float p0) = mac 0x46c810, ios 0xf96ec;
	virtual auto getEndRadiusVar() = mac 0x46c840, ios 0xf9704;
	virtual auto setEndRadiusVar(float p0) = mac 0x46c830, ios 0xf96fc;
	virtual auto getRotatePerSecond() = mac 0x46c860, ios 0xf9714;
	virtual auto setRotatePerSecond(float p0) = mac 0x46c850, ios 0xf970c;
	virtual auto getRotatePerSecondVar() = mac 0x46c880, ios 0xf9724;
	virtual auto setRotatePerSecondVar(float p0) = mac 0x46c870, ios 0xf971c;
	virtual auto isActive() = mac 0x46c890, ios 0xf972c;
	virtual auto isBlendAdditive() = mac 0x46c6a0, ios 0xf9628;
	virtual auto setBlendAdditive(bool p0) = mac 0x46c640, ios 0xf95d4;
	virtual auto getStartSize() = mac 0x46c9b0, ios 0xf97ac;
	virtual auto setStartSize(float p0) = mac 0x46c9c0, ios 0xf97b4;
	virtual auto getStartSizeVar() = mac 0x46c9d0, ios 0xf97bc;
	virtual auto setStartSizeVar(float p0) = mac 0x46c9e0, ios 0xf97c4;
	virtual auto getEndSize() = mac 0x46c9f0, ios 0xf97cc;
	virtual auto setEndSize(float p0) = mac 0x46ca00, ios 0xf97d4;
	virtual auto getEndSizeVar() = mac 0x46ca10, ios 0xf97dc;
	virtual auto setEndSizeVar(float p0) = mac 0x46ca20, ios 0xf97e4;
	virtual auto getStartColor() = mac 0x46ca30, ios 0xf97ec;
	virtual auto setStartColor(cocos2d::_ccColor4F const& p0) = mac 0x46ca40, ios 0xf97f4;
	virtual auto getStartColorVar() = mac 0x46ca60, ios 0xf9800;
	virtual auto setStartColorVar(cocos2d::_ccColor4F const& p0) = mac 0x46ca70, ios 0xf9808;
	virtual auto getEndColor() = mac 0x46ca90, ios 0xf9814;
	virtual auto setEndColor(cocos2d::_ccColor4F const& p0) = mac 0x46caa0, ios 0xf981c;
	virtual auto getEndColorVar() = mac 0x46cac0, ios 0xf9828;
	virtual auto setEndColorVar(cocos2d::_ccColor4F const& p0) = mac 0x46cad0, ios 0xf9830;
	virtual auto getStartSpin() = mac 0x46caf0, ios 0xf983c;
	virtual auto setStartSpin(float p0) = mac 0x46cb00, ios 0xf9844;
	virtual auto getStartSpinVar() = mac 0x46cb10, ios 0xf984c;
	virtual auto setStartSpinVar(float p0) = mac 0x46cb20, ios 0xf9854;
	virtual auto getEndSpin() = mac 0x46cb30, ios 0xf985c;
	virtual auto setEndSpin(float p0) = mac 0x46cb40, ios 0xf9864;
	virtual auto getEndSpinVar() = mac 0x46cb50, ios 0xf986c;
	virtual auto setEndSpinVar(float p0) = mac 0x46cb60, ios 0xf9874;
	virtual auto getEmissionRate() = mac 0x46cb70, ios 0xf987c;
	virtual auto setEmissionRate(float p0) = mac 0x46cb80, ios 0xf9884;
	virtual auto getTotalParticles() = mac 0x46cb90, ios 0xf988c;
	virtual auto setTotalParticles(unsigned int p0) = mac 0x46cba0, ios 0xf9894;
	virtual auto getTexture() = mac 0x46c620, ios 0xf95c4;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x46c4f0, ios 0xf94f4;
	virtual auto getBlendFunc() = mac 0x46cbb0, ios 0xf989c;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0x46cbd0, ios 0xf98ac;
	virtual auto getOpacityModifyRGB() = mac 0x46cc50, ios 0xf991c;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x46cc60, ios 0xf9924;
	virtual auto getPositionType() = mac 0x46cc70, ios 0xf992c;
	virtual auto setPositionType(cocos2d::tCCPositionType p0) = mac 0x46cc80, ios 0xf9934;
	virtual auto isAutoRemoveOnFinish() = mac 0x46cc90, ios 0xf993c;
	virtual auto setAutoRemoveOnFinish(bool p0) = mac 0x46cca0, ios 0xf9944;
	virtual auto getEmitterMode() = mac 0x46ccb0, ios 0xf994c;
	virtual auto setEmitterMode(int p0) = mac 0x46ccc0, ios 0xf9954;
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x46b130, ios 0xf87f4;
	virtual auto updateQuadWithParticle(cocos2d::sCCParticle* p0, cocos2d::CCPoint const& p1) = mac 0x46c4d0, ios 0xf94ec;
	virtual auto postStep() = mac 0x46c4e0, ios 0xf94f0;
	virtual auto updateWithNoTime() = mac 0x46c4b0, ios 0xf94dc;
	virtual auto updateBlendFunc() = mac 0x46c5b0, ios 0xf955c;
	auto CCParticleSystem();
	auto addParticle();
	auto create(char const* p0);
	auto createWithTotalParticles(unsigned int p0);
	auto initParticle(cocos2d::sCCParticle* p0);
	auto initWithDictionary(cocos2d::CCDictionary* p0);
	auto initWithDictionary(cocos2d::CCDictionary* p0, char const* p1);
	auto initWithFile(char const* p0);
	auto isFull();
	auto loadDefaults();
	auto loadScaledDefaults(float p0);
	auto saveDefaults();
	~CCParticleSystem() = mac 0x46b390, ios 0xf8948;
}

class cocos2d::CCParticleSystemQuad : cocos2d::CCParticleSystem {
	static cocos2d::CCParticleSystemQuad* create(char const* p0) = mac 0x36b000;
	virtual auto draw() = mac 0x36b850, ios 0x16e080;
	virtual auto setBatchNode(cocos2d::CCParticleBatchNode* p0) = mac 0x36bb70, ios 0x16e294;
	virtual auto setTotalParticles(unsigned int p0) = mac 0x36b910, ios 0x16e144;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x36b3c0, ios 0x16ddd4;
	virtual auto initWithTotalParticles(unsigned int p0) = mac 0x36aa80, ios 0x16d7b4;
	virtual auto updateQuadWithParticle(cocos2d::sCCParticle* p0, cocos2d::CCPoint const& p1) = mac 0x36b570, ios 0x16de3c;
	virtual auto postStep() = mac 0x36b800, ios 0x16e034;
	auto CCParticleSystemQuad();
	auto allocMemory();
	auto create();
	auto createWithTotalParticles(unsigned int p0);
	auto initIndices();
	auto initTexCoordsWithRect(cocos2d::CCRect const& p0);
	auto listenBackToForeground(cocos2d::CCObject* p0);
	auto setDisplayFrame(cocos2d::CCSpriteFrame* p0);
	auto setTextureWithRect(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1);
	auto setupVBO();
	~CCParticleSystemQuad() = mac 0x36afc0, ios 0x16db40;
}

class cocos2d::CCPlace : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x454d70;
	auto create(cocos2d::CCPoint const& p0);
	auto initWithPosition(cocos2d::CCPoint const& p0);
}

class cocos2d::CCPointArray : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	auto CCPointArray();
	auto addControlPoint(cocos2d::CCPoint p0);
	auto count();
	auto create(unsigned int p0);
	auto getControlPointAtIndex(unsigned int p0);
	auto getControlPoints();
	auto initWithCapacity(unsigned int p0);
	auto insertControlPoint(cocos2d::CCPoint& p0, unsigned int p1);
	auto removeControlPointAtIndex(unsigned int p0);
	auto replaceControlPoint(cocos2d::CCPoint& p0, unsigned int p1);
	auto reverse();
	auto reverseInline();
	auto setControlPoints(gd::vector<cocos2d::CCPoint*>* p0);
	~CCPointArray() = mac 0x167090;
}

class cocos2d::CCPoolManager {
	auto pop() = mac 0x214620;
	static cocos2d::CCPoolManager* sharedPoolManager() = mac 0x2142c0;
}

class cocos2d::CCPrettyPrinter : cocos2d::CCDataVisitor {
	virtual auto visitObject(cocos2d::CCObject const* p0) = mac 0x2763e0, ios 0x16b074;
	virtual auto visit(cocos2d::CCBool const* p0) = mac 0x276470, ios 0x16b0f4;
	virtual auto visit(cocos2d::CCInteger const* p0) = mac 0x276510, ios 0x16b18c;
	virtual auto visit(cocos2d::CCFloat const* p0) = mac 0x2765a0, ios 0x16b210;
	virtual auto visit(cocos2d::CCDouble const* p0) = mac 0x276630, ios 0x16b298;
	virtual auto visit(cocos2d::CCString const* p0) = mac 0x2766c0, ios 0x16b31c;
	virtual auto visit(cocos2d::CCArray const* p0) = mac 0x276700, ios 0x16b348;
	virtual auto visit(cocos2d::CCDictionary const* p0) = mac 0x276b50, ios 0x16b644;
	virtual auto visit(cocos2d::CCSet const* p0) = mac 0x276f20, ios 0x16b8d0;
	virtual auto clear() = mac 0x2763a0, ios 0x16b044;
	virtual auto getResult() = mac 0x2763c0, ios 0x16b068;
	auto CCPrettyPrinter(int p0);
	auto setIndentLevel(int p0);
	~CCPrettyPrinter();
}

class cocos2d::CCProfiler : cocos2d::CCObject {
	auto createAndAddTimerWithName(char const* p0);
	auto displayTimers();
	auto init();
	auto releaseAllTimers();
	auto releaseTimer(char const* p0);
	auto sharedProfiler();
	~CCProfiler() = mac 0x28a2e0;
}

class cocos2d::CCProfilingTimer : cocos2d::CCObject {
	auto CCProfilingTimer();
	auto description();
	auto initWithName(char const* p0);
	auto reset();
	~CCProfilingTimer() = mac 0x28a470;
}

class cocos2d::CCProgressFromTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x23d650;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x23d640;
	virtual auto reverse() = mac 0x23d590;
	auto create(float p0, float p1, float p2);
	auto initWithDuration(float p0, float p1, float p2);
}

class cocos2d::CCProgressTimer : cocos2d::CCNodeRGBA {
	virtual auto draw() = mac 0x36e8f0;
	virtual auto setOpacity(unsigned char p0) = mac 0x36c720;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x36c590;
	virtual auto getColor() const = mac 0x36c700;
	virtual auto getOpacity() const = mac 0x36c820;
	virtual auto getMidpoint() = mac 0x36e7c0;
	virtual auto setMidpoint(cocos2d::CCPoint p0) = mac 0x36e7e0;
	auto CCProgressTimer();
	auto boundaryTexCoord(char p0);
	auto create(cocos2d::CCSprite* p0);
	auto initWithSprite(cocos2d::CCSprite* p0);
	auto setAnchorPoint(cocos2d::CCPoint p0);
	auto setPercentage(float p0);
	auto setReverseProgress(bool p0);
	auto setSprite(cocos2d::CCSprite* p0);
	auto setType(cocos2d::CCProgressTimerType p0);
	auto textureCoordFromAlphaPoint(cocos2d::CCPoint p0);
	auto updateBar();
	auto updateColor();
	auto updateProgress();
	auto updateRadial();
	auto vertexFromAlphaPoint(cocos2d::CCPoint p0);
	~CCProgressTimer() = mac 0x36c3b0;
}

class cocos2d::CCProgressTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x23d380;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x23d340;
	auto create(float p0, float p1);
	auto initWithDuration(float p0, float p1);
}

class cocos2d::CCRGBAProtocol {}

class cocos2d::CCRemoveSelf : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x454770;
	virtual auto reverse() = mac 0x454790;
	auto create(bool p0);
	auto init(bool p0);
}

class cocos2d::CCRenderTexture : cocos2d::CCNode {
	auto begin() = mac 0x35ce10;
	auto end() = mac 0x35d2c0;
	static cocos2d::CCRenderTexture* create(int p0, int p1, cocos2d::CCTexture2DPixelFormat p2) = mac 0x35c720;
	auto newCCImage(bool p0) = mac 0x35d7d0;
	virtual auto draw() = mac 0x35d570;
	virtual auto visit() = mac 0x35d4e0;
	virtual auto getSprite() = mac 0x35c620;
	virtual auto setSprite(cocos2d::CCSprite* p0) = mac 0x35c630;
	auto CCRenderTexture();
	auto beginWithClear(float p0, float p1, float p2, float p3);
	auto beginWithClear(float p0, float p1, float p2, float p3, float p4);
	auto beginWithClear(float p0, float p1, float p2, float p3, float p4, int p5);
	auto beginWithClear(float p0, float p1, float p2, float p3, float p4, int p5, unsigned int p6);
	auto clear(float p0, float p1, float p2, float p3);
	auto clearDepth(float p0);
	auto clearStencil(int p0);
	auto create(int p0, int p1);
	auto create(int p0, int p1, cocos2d::CCTexture2DPixelFormat p2, unsigned int p3);
	auto getClearColor();
	auto getClearDepth();
	auto getClearFlags();
	auto getClearStencil();
	auto initWithWidthAndHeight(int p0, int p1, cocos2d::CCTexture2DPixelFormat p2);
	auto initWithWidthAndHeight(int p0, int p1, cocos2d::CCTexture2DPixelFormat p2, unsigned int p3);
	auto isAutoDraw();
	auto listenToBackground(cocos2d::CCObject* p0);
	auto listenToForeground(cocos2d::CCObject* p0);
	auto saveToFile(char const* p0);
	auto saveToFile(char const* p0, cocos2d::eImageFormat p1);
	auto setAutoDraw(bool p0);
	auto setClearColor(cocos2d::_ccColor4F const& p0);
	auto setClearDepth(float p0);
	auto setClearFlags(unsigned int p0);
	auto setClearStencil(float p0);
	~CCRenderTexture() = mac 0x35c5c0;
}

class cocos2d::CCRepeat : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f36b0;
	virtual auto isDone() = mac 0x1f37e0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f3630;
	virtual auto stop() = mac 0x1f3680;
	virtual auto reverse() = mac 0x1f37f0;
	auto create(cocos2d::CCFiniteTimeAction* p0, unsigned int p1);
	auto initWithAction(cocos2d::CCFiniteTimeAction* p0, unsigned int p1);
}

class cocos2d::CCRepeatForever : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCRepeatForever* create(cocos2d::CCActionInterval* p0) = mac 0x1f3920;
	virtual auto isDone() = mac 0x1f3b40, ios 0xf0d70;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f3a90, ios 0xf0c98;
	virtual auto step(float p0) = mac 0x1f3ad0, ios 0xf0cd8;
	virtual auto reverse() = mac 0x1f3b50, ios 0xf0d78;
	auto initWithAction(cocos2d::CCActionInterval* p0);
}

class cocos2d::CCReuseGrid : cocos2d::CCActionInstant {
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xef010;
	auto create(int p0);
	auto initWithTimes(int p0);
}

class cocos2d::CCReverseTime : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f8f70;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f8f00;
	virtual auto stop() = mac 0x1f8f40;
	virtual auto reverse() = mac 0x1f8fa0;
	auto CCReverseTime();
	auto create(cocos2d::CCFiniteTimeAction* p0);
	auto initWithAction(cocos2d::CCFiniteTimeAction* p0);
}

class cocos2d::CCRipple3D : cocos2d::CCGrid3DAction {
	virtual auto update(float p0) = mac 0x1286c0;
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint const& p2, float p3, unsigned int p4, float p5);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint const& p2, float p3, unsigned int p4, float p5);
	auto setPosition(cocos2d::CCPoint const& p0);
}

class cocos2d::CCRotateBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCRotateBy* create(float p0, float p1) = mac 0x1f4c50;
	virtual auto update(float p0) = mac 0x1f4f90, ios 0xf18e0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f4f40, ios 0xf1888;
	virtual auto reverse() = mac 0x1f5000, ios 0xf1964;
	auto create(float p0, float p1, float p2);
	auto initWithDuration(float p0, float p1);
	auto initWithDuration(float p0, float p1, float p2);
}

class cocos2d::CCRotateTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f4be0, ios 0xf15c0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f4aa0, ios 0xf14b8;
	virtual auto initWithDuration(float p0, float p1, float p2) = mac 0x1f4980, ios 0xf13a8;
	auto create(float p0, float p1);
	auto create(float p0, float p1, float p2);
	auto initWithDuration(float p0, float p1);
}

class cocos2d::CCSAXDelegator {}

class cocos2d::CCScaleBy : cocos2d::CCScaleTo {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f7640;
	virtual auto reverse() = mac 0x1f76b0;
	auto create(float p0, float p1);
	auto create(float p0, float p1, float p2);
}

class cocos2d::CCScaleTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCScaleTo* create(float p0, float p1) = mac 0x1f6ff0;
	static cocos2d::CCScaleTo* create(float p0, float p1, float p2) = mac 0x1f70f0;
	virtual auto update(float p0) = mac 0x1f7360, ios 0xf29c0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f72f0, ios 0xf2954;
	auto initWithDuration(float p0, float p1);
	auto initWithDuration(float p0, float p1, float p2);
}

class cocos2d::CCScene : cocos2d::CCNode {
	static cocos2d::CCScene* create() = mac 0x13c140, ios 0x163070;
	auto getHighestChildZ() = mac 0x13c200, ios 0x1630e4;
	virtual auto init() = mac 0x13c0f0, ios 0x163024;
	auto CCScene();
	~CCScene() = mac 0x13c0b0, ios 0x162fe8;
}

class cocos2d::CCSceneDelegate {
	virtual auto willSwitchToScene(cocos2d::CCScene* p0);
}

class cocos2d::CCSceneTransitionDelegate {
	virtual auto sceneWillResume();
}

class cocos2d::CCScheduler : cocos2d::CCObject {
	auto scheduleSelector(cocos2d::SEL_SCHEDULE p0, cocos2d::CCObject* p1, float p2, unsigned int p3, float p4, bool p5) = mac 0x242b20;
	auto scheduleUpdateForTarget(cocos2d::CCObject* p0, int p1, bool p2) = mac 0x2438d0;
	auto unscheduleAllForTarget(cocos2d::CCObject* p0) = mac 0x243e40;
	virtual auto update(float delta) = mac 0x2446d0, ios 0x10df78;
	auto CCScheduler();
	auto appendIn(cocos2d::_listEntry** p0, cocos2d::CCObject* p1, bool p2);
	auto isTargetPaused(cocos2d::CCObject* p0);
	auto pauseAllTargets();
	auto pauseAllTargetsWithMinPriority(int p0);
	auto pauseTarget(cocos2d::CCObject* p0);
	auto priorityIn(cocos2d::_listEntry** p0, cocos2d::CCObject* p1, int p2, bool p3);
	auto removeHashElement(cocos2d::_hashSelectorEntry* p0);
	auto removeUpdateFromHash(cocos2d::_listEntry* p0);
	auto resumeTarget(cocos2d::CCObject* p0);
	auto resumeTargets(cocos2d::CCSet* p0);
	auto scheduleScriptFunc(unsigned int p0, float p1, bool p2);
	auto scheduleSelector(cocos2d::SEL_SCHEDULE p0, cocos2d::CCObject* p1, float p2, bool p3);
	auto unscheduleAll();
	auto unscheduleAllWithMinPriority(int p0);
	auto unscheduleScriptEntry(unsigned int p0);
	auto unscheduleSelector(cocos2d::SEL_SCHEDULE p0, cocos2d::CCObject* p1);
	auto unscheduleUpdateForTarget(cocos2d::CCObject const* p0);
	~CCScheduler() = mac 0x242910, ios 0x10c4f4;
}

class cocos2d::CCSchedulerScriptHandlerEntry : cocos2d::CCScriptHandlerEntry {
	auto create(int p0, float p1, bool p2);
	auto init(float p0, bool p1);
	~CCSchedulerScriptHandlerEntry() = mac 0x1985f0;
}

class cocos2d::CCScriptHandlerEntry : cocos2d::CCObject {
	auto CCScriptHandlerEntry(int p0);
	auto create(int p0);
	~CCScriptHandlerEntry() = mac 0x198280, ios 0x1a8edc;
}

class cocos2d::CCSequence : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static auto createWithVariableList(cocos2d::CCFiniteTimeAction* p0, va_list p1) = mac 0x1f2910;
	virtual auto update(float p0);
	virtual auto startWithTarget(cocos2d::CCNode* p0);
	virtual auto stop();
	virtual auto reverse();
	auto create(cocos2d::CCArray* p0);
	auto createWithTwoActions(cocos2d::CCFiniteTimeAction* p0, cocos2d::CCFiniteTimeAction* p1);
	auto initWithTwoActions(cocos2d::CCFiniteTimeAction* p0, cocos2d::CCFiniteTimeAction* p1);
}

class cocos2d::CCSet : cocos2d::CCObject {
	CCSet() = mac 0x45ad80, ios 0x10e870;
	~CCSet() = mac 0x45b050, ios 0x10ebcc;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0) = mac 0x45b090, ios 0x10ec04;
	auto anyObject() = mac 0x45b410;
	auto CCSet(cocos2d::CCSet const& p0);
	auto addObject(cocos2d::CCObject* p0);
	auto begin();
	auto containsObject(cocos2d::CCObject* p0);
	auto copy();
	auto count();
	auto create();
	auto end();
	auto mutableCopy();
	auto removeAllObjects();
	auto removeObject(cocos2d::CCObject* p0);
}

class cocos2d::CCShaderCache : cocos2d::CCObject {
	auto CCShaderCache();
	auto addProgram(cocos2d::CCGLProgram* p0, char const* p1);
	auto init();
	auto loadDefaultShader(cocos2d::CCGLProgram* p0, int p1);
	auto loadDefaultShaders();
	auto programForKey(char const* p0);
	auto purgeSharedShaderCache();
	auto reloadDefaultShaders();
	auto sharedShaderCache();
	~CCShaderCache() = mac 0xe6ee0, ios 0xba20c;
}

class cocos2d::CCShaky3D : cocos2d::CCGrid3DAction {
	virtual auto update(float p0) = mac 0x128ae0;
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0, cocos2d::CCSize const& p1, int p2, bool p3);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, int p2, bool p3);
}

class cocos2d::CCShakyTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45b900;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1, int p2, bool p3) = mac 0x45b7b0;
	auto create(float p0, cocos2d::CCSize const& p1, int p2, bool p3);
}

class cocos2d::CCShatteredTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45bdc0;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1, int p2, bool p3) = mac 0x45bc70;
	auto create(float p0, cocos2d::CCSize const& p1, int p2, bool p3);
}

class cocos2d::CCShow : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0);
	virtual auto reverse();
	auto create();
}

class cocos2d::CCShuffleTiles : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45c830;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x45c590;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2) = mac 0x45c140;
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2);
	auto getDelta(cocos2d::CCSize const& p0);
	auto placeTile(cocos2d::CCPoint const& p0, cocos2d::Tile* p1);
	auto shuffle(unsigned int* p0, unsigned int p1);
}

class cocos2d::CCSkewBy : cocos2d::CCSkewTo {
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f5d00;
	virtual auto reverse() = mac 0x1f5d40;
	virtual auto initWithDuration(float p0, float p1, float p2) = mac 0x1f5cb0;
	auto create(float p0, float p1, float p2);
}

class cocos2d::CCSkewTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f5b20;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f59f0;
	virtual auto initWithDuration(float p0, float p1, float p2) = mac 0x1f58d0;
	auto CCSkewTo();
	auto create(float p0, float p1, float p2);
}

class cocos2d::CCSpawn : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f46c0, ios 0xf1258;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f4650, ios 0xf11c4;
	virtual auto stop() = mac 0x1f4690, ios 0xf1218;
	virtual auto reverse() = mac 0x1f4710, ios 0xf12bc;
	auto create(cocos2d::CCArray* p0);
	auto createWithTwoActions(cocos2d::CCFiniteTimeAction* p0, cocos2d::CCFiniteTimeAction* p1);
	auto initWithTwoActions(cocos2d::CCFiniteTimeAction* p0, cocos2d::CCFiniteTimeAction* p1);
}

class cocos2d::CCSpeed : cocos2d::CCAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto isDone() = mac 0x35bc50;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x35bbe0;
	virtual auto stop() = mac 0x35bc00;
	virtual auto step(float p0) = mac 0x35bc30;
	virtual auto reverse() = mac 0x35bc60;
	auto create(cocos2d::CCActionInterval* p0, float p1);
	auto initWithAction(cocos2d::CCActionInterval* p0, float p1);
	auto setInnerAction(cocos2d::CCActionInterval* p0);
	~CCSpeed() = mac 0x35b910;
}

class cocos2d::CCSplitCols : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45e790;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x45e750;
	virtual auto initWithDuration(float p0, unsigned int p1) = mac 0x45e5d0;
	auto create(float p0, unsigned int p1);
}

class cocos2d::CCSplitRows : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45e3e0;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x45e3a0;
	virtual auto initWithDuration(float p0, unsigned int p1) = mac 0x45e220;
	auto create(float p0, unsigned int p1);
}

class cocos2d::CCSprite : cocos2d::CCNodeRGBA, cocos2d::CCTextureProtocol {
	~CCSprite() = mac 0x133430, ios 0x15b92c;
	virtual auto init() = mac 0x132ef0, ios 0x15b488;
	virtual auto setVertexZ(float p0) = mac 0x134a80, ios 0x15ccc8;
	virtual auto setScaleX(float p0) = mac 0x134900, ios 0x15cb3c;
	virtual auto setScaleY(float p0) = mac 0x134980, ios 0x15cbbc;
	virtual auto setScale(float p0) = mac 0x134a00, ios 0x15cc3c;
	virtual auto setPosition(cocos2d::CCPoint const& p0) = mac 0x134650, ios 0x15c878;
	virtual auto setSkewX(float p0) = mac 0x134820, ios 0x15ca5c;
	virtual auto setSkewY(float p0) = mac 0x134890, ios 0x15cacc;
	virtual auto setAnchorPoint(cocos2d::CCPoint const& p0) = mac 0x134af0, ios 0x15cd38;
	virtual auto setVisible(bool p0) = mac 0x134b70, ios 0x15cdac;
	virtual auto setRotation(float p0) = mac 0x1346d0, ios 0x15c90c;
	virtual auto setRotationX(float p0) = mac 0x134740, ios 0x15c97c;
	virtual auto setRotationY(float p0) = mac 0x1347b0, ios 0x15c9ec;
	virtual auto ignoreAnchorPointForPosition(bool p0) = mac 0x134b60, ios 0x15cda8;
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0x134190, ios 0x15c34c;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0x1341a0, ios 0x15c350;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x1341b0, ios 0x15c354;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x134300, ios 0x15c4cc;
	virtual auto removeAllChildrenWithCleanup(bool p0) = mac 0x134340, ios 0x15c514;
	virtual auto reorderChild(cocos2d::CCNode* p0, int p1) = mac 0x134270, ios 0x15c438;
	virtual auto sortAllChildren() = mac 0x1343f0, ios 0x15c5cc;
	virtual auto draw() = mac 0x134070, ios 0x15c240;
	virtual auto updateTransform() = mac 0x133b70, ios 0x15be0c;
	virtual auto setOpacity(unsigned char p0) = mac 0x134da0, ios 0x15cf48;
	virtual auto updateDisplayedOpacity(unsigned char p0) = mac 0x1354c0, ios 0x15d228;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x134ff0, ios 0x15d088;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const& p0) = mac 0x135370, ios 0x15d1dc;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x135200, ios 0x15d198;
	virtual auto isOpacityModifyRGB() = mac 0x135350, ios 0x15d1cc;
	virtual auto initWithTexture(cocos2d::CCTexture2D* p0) = mac 0x133110, ios 0x15b608;
	virtual auto initWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1) = mac 0x1330f0, ios 0x15b5f8;
	virtual auto initWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1, bool p2) = mac 0x132f10, ios 0x15b4a0;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame* p0) = mac 0x133270, ios 0x15b78c;
	virtual auto initWithSpriteFrameName(char const* p0) = mac 0x1332c0, ios 0x15b7f0;
	virtual auto initWithFile(char const* p0) = mac 0x133180, ios 0x15b684;
	virtual auto initWithFile(char const* p0, cocos2d::CCRect const& p1) = mac 0x133210, ios 0x15b720;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x135a90, ios 0x15d610;
	virtual auto getTexture() = mac 0x135c00, ios 0x15d748;
	virtual auto setChildColor(cocos2d::ccColor3B const& p0) = mac 0x135160, ios 0x15d0f4;
	virtual auto setChildOpacity(unsigned char p0) = mac 0x134f50, ios 0x15cfe4;
	virtual auto getBatchNode() = mac 0x135910, ios 0x15d4e0;
	virtual auto setBatchNode(cocos2d::CCSpriteBatchNode* p0) = mac 0x135920, ios 0x15d4e8;
	virtual auto refreshTextureRect() = mac 0x133520, ios 0x15b964;
	virtual auto setTextureRect(cocos2d::CCRect const& p0) = mac 0x133560, ios 0x15b9a8;
	virtual auto setTextureRect(cocos2d::CCRect const& p0, bool p1, cocos2d::CCSize const& p2) = mac 0x133580, ios 0x15b9bc;
	virtual auto setVertexRect(cocos2d::CCRect const& p0) = mac 0x1338f0, ios 0x15bc5c;
	virtual auto setDisplayFrame(cocos2d::CCSpriteFrame* p0) = mac 0x135610, ios 0x15d274;
	virtual auto isFrameDisplayed(cocos2d::CCSpriteFrame* p0) = mac 0x1356d0, ios 0x15d344;
	virtual auto displayFrame() = mac 0x135760, ios 0x15d3d8;
	virtual auto setDisplayFrameWithAnimationName(char const* p0, int p1) = mac 0x135680, ios 0x15d2f0;
	virtual auto setTextureCoords(cocos2d::CCRect const& p0) = mac 0x133910, ios 0x15bc64;
	virtual auto updateBlendFunc() = mac 0x135a20, ios 0x15d5a0;
	virtual auto setReorderChildDirtyRecursively() = mac 0x134550, ios 0x15c764;
	virtual auto setDirtyRecursively(bool p0) = mac 0x1345b0, ios 0x15c7cc;
	CCSprite() = mac 0x133300, ios 0x15b82c;
	static cocos2d::CCSprite* create() = mac 0x132df0;
	static cocos2d::CCSprite* create(char const* p0) = mac 0x132a80;
	static cocos2d::CCSprite* createWithSpriteFrame(cocos2d::CCSpriteFrame* p0) = mac 0x132cb0;
	static cocos2d::CCSprite* createWithSpriteFrameName(char const* p0) = mac 0x132dc0, ios 0x15b3ec;
	static cocos2d::CCSprite* createWithTexture(cocos2d::CCTexture2D* p0) = mac 0x132790;
	static cocos2d::CCSprite* createWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1) = mac 0x132960;
	auto setFlipX(bool p0) = mac 0x134be0;
	auto setFlipY(bool p0) = mac 0x134c30;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0);
	virtual auto getBlendFunc();
	virtual auto isDirty();
	virtual auto setDirty(bool p0);
	auto create(char const* p0, cocos2d::CCRect const& p1);
	auto isFlipX();
	auto isFlipY();
	auto updateColor();
}

class cocos2d::CCSpriteBatchNode : cocos2d::CCNode, cocos2d::CCTextureProtocol {
	static cocos2d::CCSpriteBatchNode* create(char const* p0, unsigned int p1) = mac 0xbb540;
	static auto createWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1) = mac 0xbb310;
	auto getUsedAtlasCapacity() = mac 0xbc6b0;
	auto increaseAtlasCapacity(unsigned int p0) = mac 0xbc670;
	virtual auto init() = mac 0xbb680, ios 0x130c20;
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0xbbb40, ios 0x131034;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0xbbb50, ios 0x131038;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0xbb930, ios 0x130e0c;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0xbbbb0, ios 0x1310a4;
	virtual auto removeAllChildrenWithCleanup(bool p0) = mac 0xbbe30, ios 0x1312d8;
	virtual auto reorderChild(cocos2d::CCNode* p0, int p1) = mac 0xbbb60, ios 0x13103c;
	virtual auto sortAllChildren() = mac 0xbbee0, ios 0x13137c;
	virtual auto draw() = mac 0xbc540, ios 0x13191c;
	virtual auto visit() = mac 0xbb880, ios 0x130d54;
	virtual auto getTexture() = mac 0xbcd70, ios 0x131a70;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0xbcda0, ios 0x131a90;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc p0) = mac 0xbcd30, ios 0x131a50;
	virtual auto getBlendFunc() = mac 0xbcd50, ios 0x131a60;
	auto CCSpriteBatchNode();
	auto addSpriteWithoutQuad(cocos2d::CCSprite* p0, unsigned int p1, int p2);
	auto appendChild(cocos2d::CCSprite* p0);
	auto atlasIndexForChild(cocos2d::CCSprite* p0, int p1);
	auto createWithTexture(cocos2d::CCTexture2D* p0);
	auto getAtlasCapacity();
	auto highestAtlasIndexInChild(cocos2d::CCSprite* p0);
	auto increaseAtlasCapacity();
	auto initWithFile(char const* p0, unsigned int p1);
	auto initWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1);
	auto insertChild(cocos2d::CCSprite* p0, unsigned int p1);
	auto insertQuadFromSprite(cocos2d::CCSprite* p0, unsigned int p1);
	auto lowestAtlasIndexInChild(cocos2d::CCSprite* p0);
	auto manualSortAllChildren();
	auto rebuildIndexInOrder(cocos2d::CCSprite* p0, unsigned int p1);
	auto removeChildAtIndex(unsigned int p0, bool p1);
	auto removeSpriteFromAtlas(cocos2d::CCSprite* p0);
	auto reorderBatch(bool p0);
	auto swap(int p0, int p1);
	auto updateAtlasIndex(cocos2d::CCSprite* p0, int* p1);
	auto updateBlendFunc();
	auto updateQuadFromSprite(cocos2d::CCSprite* p0, unsigned int p1);
	~CCSpriteBatchNode() = mac 0xbb790, ios 0x130d18;
}

class cocos2d::CCSpriteFrame : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static auto createWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1, bool p2, cocos2d::CCPoint const& p3, cocos2d::CCSize const& p4) = mac 0x1ac7f0;
	static auto createWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1) = mac 0x1ac5c0;
	auto CCSpriteFrame();
	auto create(char const* p0, cocos2d::CCRect const& p1);
	auto create(char const* p0, cocos2d::CCRect const& p1, bool p2, cocos2d::CCPoint const& p3, cocos2d::CCSize const& p4);
	auto getOffset();
	auto getOffsetInPixels();
	auto getTexture();
	auto initWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1);
	auto initWithTexture(cocos2d::CCTexture2D* p0, cocos2d::CCRect const& p1, bool p2, cocos2d::CCPoint const& p3, cocos2d::CCSize const& p4);
	auto initWithTextureFilename(char const* p0, cocos2d::CCRect const& p1);
	auto initWithTextureFilename(char const* p0, cocos2d::CCRect const& p1, bool p2, cocos2d::CCPoint const& p3, cocos2d::CCSize const& p4);
	auto setOffset(cocos2d::CCPoint const& p0);
	auto setOffsetInPixels(cocos2d::CCPoint const& p0);
	auto setRect(cocos2d::CCRect const& p0);
	auto setRectInPixels(cocos2d::CCRect const& p0);
	auto setTexture(cocos2d::CCTexture2D* p0);
	~CCSpriteFrame() = mac 0x1ace20, ios 0x16ace8;
}

class cocos2d::CCSpriteFrameCache : cocos2d::CCObject {
	auto addSpriteFramesWithFile(char const* p0) = mac 0x199a10, ios 0x29e818;
	static cocos2d::CCSpriteFrameCache* sharedSpriteFrameCache() = mac 0x198970, ios 0x29dc4c;
	auto spriteFrameByName(char const* p0) = mac 0x19a7e0;
	auto addSpriteFrame(cocos2d::CCSpriteFrame* p0, char const* p1);
	auto addSpriteFramesWithDictionary(cocos2d::CCDictionary* p0, cocos2d::CCTexture2D* p1);
	auto addSpriteFramesWithFile(char const* p0, cocos2d::CCTexture2D* p1);
	auto addSpriteFramesWithFile(char const* p0, char const* p1);
	auto init();
	auto purgeSharedSpriteFrameCache();
	auto removeSpriteFrameByName(char const* p0);
	auto removeSpriteFrames();
	auto removeSpriteFramesFromDictionary(cocos2d::CCDictionary* p0);
	auto removeSpriteFramesFromFile(char const* p0);
	auto removeSpriteFramesFromTexture(cocos2d::CCTexture2D* p0);
	auto removeUnusedSpriteFrames();
	~CCSpriteFrameCache() = mac 0x198b50, ios 0x29dde0;
}

class cocos2d::CCStandardTouchHandler : cocos2d::CCTouchHandler {
	static cocos2d::CCStandardTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate* p0, int p1) = mac 0x247f30;
	virtual auto initWithDelegate(cocos2d::CCTouchDelegate* p0, int p1) = mac 0x247ed0, ios 0x69;
	~CCStandardTouchHandler() = mac 0x2482a0, ios 0x6d28;
}

class cocos2d::CCStopGrid : cocos2d::CCActionInstant {
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0xeeee0;
	auto create();
}

class cocos2d::CCString : cocos2d::CCObject {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto isEqual(cocos2d::CCObject const* p0) = mac 0x44c8f0, ios 0x1a1e6c;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor& p0) = mac 0x44ccb0, ios 0x1a2168;
	auto boolValue() const = mac 0x44c810, ios 0x1a1d88;
	auto doubleValue() const = mac 0x44c7f0;
	auto floatValue() const = mac 0x44c7d0;
	auto getCString() const = mac 0x44c470, ios 0x1a1b7c;
	auto intValue() const = mac 0x44c780, ios 0x1a1ca8;
	auto CCString(char const* p0);
	auto CCString(cocos2d::CCString const& p0);
	auto CCString(gd::string const& p0);
	auto CCString();
	auto compare(char const* p0);
	auto create(gd::string const& p0);
	auto createWithContentsOfFile(char const* p0);
	auto createWithData(unsigned char const* p0, unsigned long p1);
	auto length();
	auto uintValue();
}

class cocos2d::CCTMXLayer : cocos2d::CCSpriteBatchNode {
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x1222b0;
	virtual auto removeChild(cocos2d::CCNode* p0, bool p1) = mac 0x1222c0;
	virtual auto getTileSet() = mac 0x120d20;
	virtual auto setTileSet(cocos2d::CCTMXTilesetInfo* p0) = mac 0x120d30;
	virtual auto getProperties() = mac 0x1224a0;
	virtual auto setProperties(cocos2d::CCDictionary* p0) = mac 0x1224b0;
	auto CCTMXLayer();
	auto appendTileForGID(unsigned int p0, cocos2d::CCPoint const& p1);
	auto atlasIndexForExistantZ(unsigned int p0);
	auto atlasIndexForNewZ(int p0);
	auto calculateLayerOffset(cocos2d::CCPoint const& p0);
	auto create(cocos2d::CCTMXTilesetInfo* p0, cocos2d::CCTMXLayerInfo* p1, cocos2d::CCTMXMapInfo* p2);
	auto initWithTilesetInfo(cocos2d::CCTMXTilesetInfo* p0, cocos2d::CCTMXLayerInfo* p1, cocos2d::CCTMXMapInfo* p2);
	auto insertTileForGID(unsigned int p0, cocos2d::CCPoint const& p1);
	auto parseInternalProperties();
	auto positionAt(cocos2d::CCPoint const& p0);
	auto positionForHexAt(cocos2d::CCPoint const& p0);
	auto positionForIsoAt(cocos2d::CCPoint const& p0);
	auto positionForOrthoAt(cocos2d::CCPoint const& p0);
	auto propertyNamed(char const* p0);
	auto releaseMap();
	auto removeTileAt(cocos2d::CCPoint const& p0);
	auto reusedTileWithRect(cocos2d::CCRect p0);
	auto setTileGID(unsigned int p0, cocos2d::CCPoint const& p1);
	auto setTileGID(unsigned int p0, cocos2d::CCPoint const& p1, cocos2d::ccTMXTileFlags_ p2);
	auto setupTileSprite(cocos2d::CCSprite* p0, cocos2d::CCPoint p1, unsigned int p2);
	auto setupTiles();
	auto tileAt(cocos2d::CCPoint const& p0);
	auto tileGIDAt(cocos2d::CCPoint const& p0);
	auto tileGIDAt(cocos2d::CCPoint const& p0, cocos2d::ccTMXTileFlags_* p1);
	auto updateTileForGID(unsigned int p0, cocos2d::CCPoint const& p1);
	auto vertexZForPos(cocos2d::CCPoint const& p0);
	~CCTMXLayer() = mac 0x120ce0;
}

class cocos2d::CCTMXLayerInfo : cocos2d::CCObject {
	virtual auto getProperties() = mac 0x282900;
	virtual auto setProperties(cocos2d::CCDictionary* p0) = mac 0x282910;
	auto CCTMXLayerInfo();
	~CCTMXLayerInfo() = mac 0x2828c0;
}

class cocos2d::CCTMXMapInfo : cocos2d::CCObject, cocos2d::CCSAXDelegator {
	virtual auto getLayers() = mac 0x283640;
	virtual auto setLayers(cocos2d::CCArray* p0) = mac 0x283650;
	virtual auto getTilesets() = mac 0x283690;
	virtual auto setTilesets(cocos2d::CCArray* p0) = mac 0x2836a0;
	virtual auto getObjectGroups() = mac 0x2836e0;
	virtual auto setObjectGroups(cocos2d::CCArray* p0) = mac 0x2836f0;
	virtual auto getProperties() = mac 0x283730;
	virtual auto setProperties(cocos2d::CCDictionary* p0) = mac 0x283740;
	virtual auto startElement(void* p0, char const* p1, char const** p2) = mac 0x2837d0;
	virtual auto endElement(void* p0, char const* p1) = mac 0x286210;
	virtual auto textHandler(void* p0, char const* p1, int p2) = mac 0x286460;
	auto CCTMXMapInfo();
	auto formatWithTMXFile(char const* p0);
	auto formatWithXML(char const* p0, char const* p1);
	auto getTileProperties();
	auto initWithTMXFile(char const* p0);
	auto initWithXML(char const* p0, char const* p1);
	auto internalInit(char const* p0, char const* p1);
	auto parseXMLFile(char const* p0);
	auto parseXMLString(char const* p0);
	auto setTileProperties(cocos2d::CCDictionary* p0);
	~CCTMXMapInfo() = mac 0x283600;
}

class cocos2d::CCTMXObjectGroup : cocos2d::CCObject {
	virtual auto getProperties() = mac 0x25f800;
	virtual auto setProperties(cocos2d::CCDictionary* p0) = mac 0x25f810;
	virtual auto getObjects() = mac 0x25f850;
	virtual auto setObjects(cocos2d::CCArray* p0) = mac 0x25f860;
	auto CCTMXObjectGroup();
	auto objectNamed(char const* p0);
	auto propertyNamed(char const* p0);
	~CCTMXObjectGroup() = mac 0x25f5c0;
}

class cocos2d::CCTMXTiledMap : cocos2d::CCNode {
	virtual auto getObjectGroups() = mac 0x2a63e0;
	virtual auto setObjectGroups(cocos2d::CCArray* p0) = mac 0x2a63f0;
	virtual auto getProperties() = mac 0x2a6430;
	virtual auto setProperties(cocos2d::CCDictionary* p0) = mac 0x2a6440;
	auto CCTMXTiledMap();
	auto buildWithMapInfo(cocos2d::CCTMXMapInfo* p0);
	auto create(char const* p0);
	auto createWithXML(char const* p0, char const* p1);
	auto initWithTMXFile(char const* p0);
	auto initWithXML(char const* p0, char const* p1);
	auto layerNamed(char const* p0);
	auto objectGroupNamed(char const* p0);
	auto parseLayer(cocos2d::CCTMXLayerInfo* p0, cocos2d::CCTMXMapInfo* p1);
	auto propertiesForGID(int p0);
	auto propertyNamed(char const* p0);
	auto tilesetForLayer(cocos2d::CCTMXLayerInfo* p0, cocos2d::CCTMXMapInfo* p1);
	~CCTMXTiledMap() = mac 0x2a63a0;
}

class cocos2d::CCTMXTilesetInfo : cocos2d::CCObject {
	auto CCTMXTilesetInfo();
	auto rectForGID(unsigned int p0);
	~CCTMXTilesetInfo() = mac 0x282ad0;
}

class cocos2d::CCTargetedAction : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f9d40;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f9d00;
	virtual auto stop() = mac 0x1f9d30;
	auto CCTargetedAction();
	auto create(cocos2d::CCNode* p0, cocos2d::CCFiniteTimeAction* p1);
	auto initWithTarget(cocos2d::CCNode* p0, cocos2d::CCFiniteTimeAction* p1);
}

class cocos2d::CCTargetedTouchHandler : cocos2d::CCTouchHandler {
	static cocos2d::CCTargetedTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate* p0, int p1, bool p2) = mac 0x248010;
	auto initWithDelegate(cocos2d::CCTouchDelegate* p0, int p1, bool p2) = mac 0x2480f0, ios 0x69;
	~CCTargetedTouchHandler() = mac 0x248180, ios 0x6cec;
	auto getClaimedTouches();
	auto isSwallowsTouches();
	auto setSwallowsTouches(bool p0);
}

class cocos2d::CCTextFieldDelegate {
	virtual auto onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF* p0);
	virtual auto onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF* p0);
	virtual auto onTextFieldInsertText(cocos2d::CCTextFieldTTF* p0, char const* p1, int p2);
	virtual auto onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF* p0, char const* p1, int p2);
	virtual auto onDraw(cocos2d::CCTextFieldTTF* p0);
	virtual auto textChanged();
}

class cocos2d::CCTextFieldTTF : cocos2d::CCLabelTTF, cocos2d::CCIMEDelegate {
	virtual auto draw() = mac 0x126a00, ios 0x12931c;
	virtual auto setString(char const* p0) = mac 0x126ac0, ios 0x1293fc;
	virtual auto getString() = mac 0x126d00, ios 0x129654;
	virtual auto attachWithIME() = mac 0x126370, ios 0x128cd8;
	virtual auto detachWithIME() = mac 0x1263f0, ios 0x128d28;
	virtual auto getColorSpaceHolder() = mac 0x126a90, ios 0x1293e0;
	virtual auto setColorSpaceHolder(cocos2d::ccColor3B const& p0) = mac 0x126aa0, ios 0x1293e8;
	virtual auto setPlaceHolder(char const* p0) = mac 0x126d20, ios 0x12967c;
	virtual auto getPlaceHolder() = mac 0x126e10, ios 0x1297bc;
	virtual auto setSecureTextEntry(bool p0) = mac 0x126e20, ios 0x1297d0;
	virtual auto isSecureTextEntry() = mac 0x126e70, ios 0x129838;
	virtual auto canAttachWithIME() = mac 0x126470, ios 0x128d78;
	virtual auto canDetachWithIME() = mac 0x1264d0, ios 0x128de8;
	virtual auto insertText(char const* p0, int p1) = mac 0x126530, ios 0x128e58;
	virtual auto deleteBackward() = mac 0x126760, ios 0x1290ac;
	virtual auto getContentText() = mac 0x1269e0, ios 0x1292f4;
	auto CCTextFieldTTF();
	auto initWithPlaceHolder(char const* p0, cocos2d::CCSize const& p1, cocos2d::CCTextAlignment p2, char const* p3, float p4);
	auto initWithPlaceHolder(char const* p0, char const* p1, float p2);
	auto textFieldWithPlaceHolder(char const* p0, cocos2d::CCSize const& p1, cocos2d::CCTextAlignment p2, char const* p3, float p4);
	auto textFieldWithPlaceHolder(char const* p0, char const* p1, float p2);
	~CCTextFieldTTF() = mac 0x125ff0, ios 0x128bdc;
}

class cocos2d::CCTexture2D : cocos2d::CCObject {
	~CCTexture2D() = mac 0x246350, ios 0xbdccc;
	CCTexture2D() = mac 0x246280;
	auto getContentSize() = mac 0x246460, ios 0xbdd28;
	auto getMaxS() = mac 0x2464e0, ios 0xbdd80;
	auto getMaxT() = mac 0x246500, ios 0xbdd90;
	auto getName() = mac 0x246450, ios 0xbdd20;
	auto getPixelFormat() = mac 0x246420, ios 0xbdd08;
	auto getPixelsHigh() = mac 0x246440, ios 0xbdd18;
	auto getPixelsWide() = mac 0x246430, ios 0xbdd10;
	auto getShaderProgram() = mac 0x246520, ios 0xbdda0;
	auto initPremultipliedATextureWithImage(cocos2d::CCImage* p0, unsigned int p1, unsigned int p2) = mac 0x2469a0;
	auto initWithData(void const* p0, cocos2d::CCTexture2DPixelFormat p1, unsigned int p2, unsigned int p3, cocos2d::CCSize const& p4) = mac 0x2465d0;
	auto setAliasTexParameters() = mac 0x247a20;
	auto setAntiAliasTexParameters() = mac 0x247a80;
	auto setMaxS(float p0) = mac 0x2464f0, ios 0xbdd88;
	auto setMaxT(float p0) = mac 0x246510, ios 0xbdd98;
	auto setShaderProgram(cocos2d::CCGLProgram* p0) = mac 0x246530, ios 0xbdda8;
	auto initWithImage(cocos2d::CCImage* p0) = mac 0x246940;
	auto setTexParameters(cocos2d::_ccTexParams* p0) = mac 0x247980;
	auto PVRImagesHavePremultipliedAlpha(bool p0);
	auto bitsPerPixelForFormat(cocos2d::CCTexture2DPixelFormat p0);
	auto bitsPerPixelForFormat();
	auto defaultAlphaPixelFormat();
	auto description();
	auto drawAtPoint(cocos2d::CCPoint const& p0);
	auto drawInRect(cocos2d::CCRect const& p0);
	auto generateMipmap();
	auto getContentSizeInPixels();
	auto hasMipmaps();
	auto hasPremultipliedAlpha();
	auto initWithData(auto const* p0, cocos2d::CCTexture2DPixelFormat p1, unsigned int p2, unsigned int p3, cocos2d::CCSize const& p4);
	auto initWithETCFile(char const* p0);
	auto initWithPVRFile(char const* p0);
	auto initWithString(char const* p0, cocos2d::_ccFontDefinition* p1);
	auto initWithString(char const* p0, char const* p1, float p2);
	auto initWithString(char const* p0, char const* p1, float p2, cocos2d::CCSize const& p3, cocos2d::CCTextAlignment p4, cocos2d::CCVerticalTextAlignment p5);
	auto keepData(void* p0, unsigned int p1);
	auto releaseData(void* p0);
	auto releaseGLTexture();
	auto setDefaultAlphaPixelFormat(cocos2d::CCTexture2DPixelFormat p0);
	auto stringForFormat();
}

class cocos2d::CCTextureAtlas : cocos2d::CCObject {
	virtual auto getTotalQuads() = mac 0x26fe70, ios 0x1a3e94;
	virtual auto getCapacity() = mac 0x26fe80, ios 0x1a3e9c;
	virtual auto getTexture() = mac 0x26fe90, ios 0x1a3ea4;
	virtual auto setTexture(cocos2d::CCTexture2D* p0) = mac 0x26fea0, ios 0x1a3eac;
	virtual auto getQuads() = mac 0x26fee0, ios 0x1a3ee8;
	virtual auto setQuads(cocos2d::_ccV3F_C4B_T2F_Quad* p0) = mac 0x26fef0, ios 0x1a3ef8;
	auto CCTextureAtlas();
	auto create(char const* p0, unsigned int p1);
	auto createWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1);
	auto description();
	auto drawNumberOfQuads(unsigned int p0);
	auto drawNumberOfQuads(unsigned int p0, unsigned int p1);
	auto drawQuads();
	auto fillWithEmptyQuadsFromIndex(unsigned int p0, unsigned int p1);
	auto increaseTotalQuadsWith(unsigned int p0);
	auto initWithFile(char const* p0, unsigned int p1);
	auto initWithTexture(cocos2d::CCTexture2D* p0, unsigned int p1);
	auto insertQuad(cocos2d::_ccV3F_C4B_T2F_Quad* p0, unsigned int p1);
	auto insertQuadFromIndex(unsigned int p0, unsigned int p1);
	auto insertQuads(cocos2d::_ccV3F_C4B_T2F_Quad* p0, unsigned int p1, unsigned int p2);
	auto listenBackToForeground(cocos2d::CCObject* p0);
	auto mapBuffers();
	auto moveQuadsFromIndex(unsigned int p0, unsigned int p1);
	auto moveQuadsFromIndex(unsigned int p0, unsigned int p1, unsigned int p2);
	auto removeAllQuads();
	auto removeQuadAtIndex(unsigned int p0);
	auto removeQuadsAtIndex(unsigned int p0, unsigned int p1);
	auto resizeCapacity(unsigned int p0);
	auto setupIndices();
	auto setupVBO();
	auto updateQuad(cocos2d::_ccV3F_C4B_T2F_Quad* p0, unsigned int p1);
	~CCTextureAtlas() = mac 0x26fe30, ios 0x1a3e58;
}

class cocos2d::CCTextureCache : cocos2d::CCObject {
	auto addImage(char const* p0, bool p1) = mac 0x358120, ios 0xa8388;
	static cocos2d::CCTextureCache* sharedTextureCache() = mac 0x356e00, ios 0xa81ec;
	auto CCTextureCache();
	auto addETCImage(char const* p0);
	auto addImageAsync(char const* p0, cocos2d::CCObject* p1, cocos2d::SEL_MenuHandler p2);
	auto addImageAsyncCallBack(float p0);
	auto addPVRImage(char const* p0);
	auto addUIImage(cocos2d::CCImage* p0, char const* p1);
	auto description();
	auto dumpCachedTextureInfo();
	auto purgeSharedTextureCache();
	auto reloadAllTextures();
	auto reloadTexture(char const* p0);
	auto removeAllTextures();
	auto removeTexture(cocos2d::CCTexture2D* p0);
	auto removeTextureForKey(char const* p0);
	auto removeUnusedTextures();
	auto snapshotTextures();
	auto textureForKey(char const* p0);
	~CCTextureCache() = mac 0x356fe0, ios 0xa8320;
}

class cocos2d::CCTexturePVR : cocos2d::CCObject {
	~CCTexturePVR() = mac 0x12f100, ios 0x27568c;
}

class cocos2d::CCTextureProtocol : cocos2d::CCBlendProtocol {}

class cocos2d::CCTileMapAtlas : cocos2d::CCAtlasNode {
	virtual auto updateAtlasValues() = mac 0x105f00;
	virtual auto getTGAInfo() = mac 0x106120;
	virtual auto setTGAInfo(cocos2d::sImageTGA* p0) = mac 0x106110;
	auto CCTileMapAtlas();
	auto calculateItemsToRender();
	auto create(char const* p0, char const* p1, int p2, int p3);
	auto initWithTileFile(char const* p0, char const* p1, int p2, int p3);
	auto loadTGAfile(char const* p0);
	auto releaseMap();
	auto setTile(cocos2d::ccColor3B const& p0, cocos2d::CCPoint const& p1);
	auto tileAt(cocos2d::CCPoint const& p0);
	auto updateAtlasValueAt(cocos2d::CCPoint const& p0, cocos2d::ccColor3B const& p1, unsigned int p2);
	~CCTileMapAtlas() = mac 0x105970;
}

class cocos2d::CCTiledGrid3D : cocos2d::CCGridBase {
	virtual auto blit() = mac 0x2891c0;
	virtual auto reuse() = mac 0x289780;
	virtual auto calculateVertexPoints() = mac 0x289260;
	auto CCTiledGrid3D();
	auto create(cocos2d::CCSize const& p0);
	auto create(cocos2d::CCSize const& p0, cocos2d::CCTexture2D* p1, bool p2);
	auto originalTile(cocos2d::CCPoint const& p0);
	auto setTile(cocos2d::CCPoint const& p0, cocos2d::_ccQuad3 const& p1);
	auto tile(cocos2d::CCPoint const& p0);
	~CCTiledGrid3D() = mac 0x288fe0;
}

class cocos2d::CCTiledGrid3DAction : cocos2d::CCGridAction {
	virtual auto getGrid() = mac 0xee350;
	auto CCTiledGrid3DAction();
	auto create(float p0, cocos2d::CCSize const& p1);
	auto originalTile(cocos2d::CCPoint const& p0);
	auto setTile(cocos2d::CCPoint const& p0, cocos2d::_ccQuad3 const& p1);
	auto tile(cocos2d::CCPoint const& p0);
}

class cocos2d::CCTime {
	static auto gettimeofdayCocos2d(cocos2d::cc_timeval* p0, void* p1) = mac 0x19eac0;
}

class cocos2d::CCTimer : cocos2d::CCObject {
	virtual auto update(float p0) = mac 0x242440, ios 0x10c048;
	auto CCTimer();
	auto getInterval();
	auto getSelector();
	auto initWithScriptHandler(int p0, float p1);
	auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_SCHEDULE p1);
	auto initWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_SCHEDULE p1, float p2, unsigned int p3, float p4);
	auto setInterval(float p0);
	auto timerWithScriptHandler(int p0, float p1);
	auto timerWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_SCHEDULE p1);
	auto timerWithTarget(cocos2d::CCObject* p0, cocos2d::SEL_SCHEDULE p1, float p2);
}

class cocos2d::CCTintBy : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x1f8870;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f8800;
	virtual auto reverse() = mac 0x1f8950;
	auto create(float p0, short p1, short p2, short p3);
	auto initWithDuration(float p0, short p1, short p2, short p3);
}

class cocos2d::CCTintTo : cocos2d::CCActionInterval {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	static cocos2d::CCTintTo* create(float p0, unsigned char p1, unsigned char p2, unsigned char p3) = mac 0x1f82a0;
	virtual auto update(float p0) = mac 0x1f8510, ios 0xf3518;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x1f84b0, ios 0xf34ac;
	auto initWithDuration(float p0, unsigned char p1, unsigned char p2, unsigned char p3);
}

class cocos2d::CCToggleVisibility : cocos2d::CCActionInstant {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0);
	auto create();
}

class cocos2d::CCTouch : cocos2d::CCObject {
	auto getDelta() const = mac 0x38340;
	auto getLocationInView() const = mac 0x38250;
	auto getPreviousLocationInView() const = mac 0x38270;
	auto getLocation() const = mac 0x382b0, ios 0x21ce78;
	auto getPreviousLocation();
	auto getStartLocation();
	auto getStartLocationInView();
	~CCTouch();
}

class cocos2d::CCTouchDelegate {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchesBegan(cocos2d::CCSet* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchesMoved(cocos2d::CCSet* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchesEnded(cocos2d::CCSet* p0, cocos2d::CCEvent* p1);
	virtual auto ccTouchesCancelled(cocos2d::CCSet* p0, cocos2d::CCEvent* p1);
	~CCTouchDelegate();
}

class cocos2d::CCTouchDispatcher : cocos2d::CCObject, cocos2d::EGLTouchDelegate {
	auto addTargetedDelegate(cocos2d::CCTouchDelegate* p0, int p1, bool p2) = mac 0x281180;
	auto decrementForcePrio(int p0) = mac 0x280f70, ios 0xcc280;
	auto incrementForcePrio(int p0) = mac 0x280f60, ios 0xcc268;
	auto touches(cocos2d::CCSet* p0, cocos2d::CCEvent* p1, unsigned int p2) = mac 0x281a60;
	auto removeDelegate(cocos2d::CCTouchDelegate* delegate);
	virtual auto touchesBegan(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x282050, ios 0xccbf4;
	virtual auto touchesMoved(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x282090, ios 0xccc20;
	virtual auto touchesEnded(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x2820d0, ios 0xccc4c;
	virtual auto touchesCancelled(cocos2d::CCSet* p0, cocos2d::CCEvent* p1) = mac 0x282110, ios 0xccc78;
	auto addStandardDelegate(cocos2d::CCTouchDelegate* p0, int p1);
	auto findHandler(cocos2d::CCTouchDelegate* p0);
	auto findHandler(cocos2d::CCArray* p0, cocos2d::CCTouchDelegate* p1);
	auto forceAddHandler(cocos2d::CCTouchHandler* p0, cocos2d::CCArray* p1);
	auto forceRemoveAllDelegates();
	auto forceRemoveDelegate(cocos2d::CCTouchDelegate* p0);
	auto init();
	auto isDispatchEvents();
	auto rearrangeHandlers(cocos2d::CCArray* p0);
	auto removeAllDelegates();
	auto setDispatchEvents(bool p0);
	auto setPriority(int p0, cocos2d::CCTouchDelegate* p1);
	~CCTouchDispatcher() = mac 0x280ed0, ios 0xcc21c;
}

class cocos2d::CCTouchHandler : cocos2d::CCObject {
	virtual auto initWithDelegate(cocos2d::CCTouchDelegate* p0, int p1) = mac 0x247d10, ios 0x69f8;
	auto getPriority() = mac 0x247c20;
	~CCTouchHandler() = mac 0x247de0, ios 0x6ac0;
	auto getDelegate();
	auto getEnabledSelectors();
	auto handlerWithDelegate(cocos2d::CCTouchDelegate* p0, int p1);
	auto setDelegate(cocos2d::CCTouchDelegate* p0);
	auto setEnalbedSelectors(int p0);
	auto setPriority(int p0);
}

class cocos2d::CCTouchScriptHandlerEntry : cocos2d::CCScriptHandlerEntry {
	auto create(int p0, bool p1, int p2, bool p3);
	auto init(bool p0, int p1, bool p2);
	~CCTouchScriptHandlerEntry() = mac 0x1987d0, ios 0x1a9040;
}

class cocos2d::CCTransitionCrossFade : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x8ef30;
	virtual auto onExit() = mac 0x8f290;
	virtual auto draw() = mac 0x8ef20;
	auto CCTransitionCrossFade();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionCrossFade() = mac 0x8ed70;
}

class cocos2d::CCTransitionEaseScene {}

class cocos2d::CCTransitionFade : cocos2d::CCTransitionScene {
	static cocos2d::CCTransitionFade* create(float p0, cocos2d::CCScene* p1) = mac 0x8ea30, ios 0x12c244;
	virtual auto onEnter() = mac 0x8eb30, ios 0x12c2d0;
	virtual auto onExit() = mac 0x8ec60, ios 0x12c3d4;
	virtual auto initWithDuration(float p0, cocos2d::CCScene* p1) = mac 0x8eb10, ios 0x12c2a8;
	virtual auto initWithDuration(float p0, cocos2d::CCScene* p1, cocos2d::ccColor3B const& p2) = mac 0x8ea50, ios 0x12c260;
	auto CCTransitionFade();
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::ccColor3B const& p2);
	~CCTransitionFade() = mac 0x8e860, ios 0x12c164;
}

class cocos2d::CCTransitionFadeBL : cocos2d::CCTransitionFadeTR {
	virtual auto actionWithSize(cocos2d::CCSize const& p0) = mac 0x90380;
	auto CCTransitionFadeBL();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionFadeBL() = mac 0x901c0;
}

class cocos2d::CCTransitionFadeDown : cocos2d::CCTransitionFadeTR {
	virtual auto actionWithSize(cocos2d::CCSize const& p0) = mac 0x908c0;
	auto CCTransitionFadeDown();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionFadeDown() = mac 0x90700;
}

class cocos2d::CCTransitionFadeTR : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8ffc0;
	virtual auto sceneOrder() = mac 0x8ffb0;
	virtual auto actionWithSize(cocos2d::CCSize const& p0) = mac 0x900c0;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x900e0;
	auto CCTransitionFadeTR();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionFadeTR() = mac 0x8fdf0;
}

class cocos2d::CCTransitionFadeUp : cocos2d::CCTransitionFadeTR {
	virtual auto actionWithSize(cocos2d::CCSize const& p0) = mac 0x90620;
	auto CCTransitionFadeUp();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionFadeUp() = mac 0x90460;
}

class cocos2d::CCTransitionFlipAngular : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8d7a0;
	auto CCTransitionFlipAngular();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionFlipAngular() = mac 0x8d6d0;
}

class cocos2d::CCTransitionFlipX : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8cf90;
	auto CCTransitionFlipX();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionFlipX() = mac 0x8cec0;
}

class cocos2d::CCTransitionFlipY : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8d390;
	auto CCTransitionFlipY();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionFlipY() = mac 0x8d2c0;
}

class cocos2d::CCTransitionJumpZoom : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x8abf0;
	auto CCTransitionJumpZoom();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionJumpZoom() = mac 0x8aa40;
}

class cocos2d::CCTransitionMoveInB : cocos2d::CCTransitionMoveInL {
	virtual auto initScenes() = mac 0x8ba90;
	auto CCTransitionMoveInB();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionMoveInB() = mac 0x8b8d0;
}

class cocos2d::CCTransitionMoveInL : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8b0b0, ios 0x12bf40;
	virtual auto initScenes() = mac 0x8b1e0;
	virtual auto action() = mac 0x8b180, ios 0x12bfe4;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x8b1c0, ios 0x12c024;
	auto CCTransitionMoveInL();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionMoveInL() = mac 0x8aef0;
}

class cocos2d::CCTransitionMoveInR : cocos2d::CCTransitionMoveInL {
	virtual auto initScenes() = mac 0x8b4d0;
	auto CCTransitionMoveInR();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionMoveInR() = mac 0x8b310;
}

class cocos2d::CCTransitionMoveInT : cocos2d::CCTransitionMoveInL {
	virtual auto initScenes() = mac 0x8b7b0, ios 0x12c10c;
	auto CCTransitionMoveInT();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionMoveInT() = mac 0x8b5f0, ios 0x12c034;
}

class cocos2d::CCTransitionPageTurn : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x20d1d0;
	virtual auto sceneOrder() = mac 0x20d1b0;
	virtual auto initWithDuration(float p0, cocos2d::CCScene* p1, bool p2) = mac 0x20d190;
	auto CCTransitionPageTurn();
	auto actionWithSize(cocos2d::CCSize const& p0);
	auto create(float p0, cocos2d::CCScene* p1, bool p2);
	~CCTransitionPageTurn() = mac 0x20d0d0;
}

class cocos2d::CCTransitionProgress : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x137970;
	virtual auto onExit() = mac 0x137b30;
	virtual auto sceneOrder() = mac 0x137b60;
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x137ba0;
	virtual auto setupTransition() = mac 0x137b70;
	auto CCTransitionProgress();
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressHorizontal : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x137f50;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressInOut : cocos2d::CCTransitionProgress {
	virtual auto sceneOrder() = mac 0x1382d0;
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x138300;
	virtual auto setupTransition() = mac 0x1382e0;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressOutIn : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x1384c0;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressRadialCCW : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x137bb0;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressRadialCW : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x137dd0;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionProgressVertical : cocos2d::CCTransitionProgress {
	virtual auto progressTimerNodeWithRenderTexture(cocos2d::CCRenderTexture* p0) = mac 0x138110;
	auto create(float p0, cocos2d::CCScene* p1);
}

class cocos2d::CCTransitionRotoZoom : cocos2d::CCTransitionScene {
	virtual auto onEnter() = mac 0x8a7d0;
	auto CCTransitionRotoZoom();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionRotoZoom() = mac 0x8a700;
}

class cocos2d::CCTransitionScene : cocos2d::CCScene {
	virtual auto onEnter() = mac 0x8a1a0, ios 0x12be58;
	virtual auto onExit() = mac 0x8a1f0, ios 0x12beac;
	virtual auto cleanup() = mac 0x8a250, ios 0x12bf00;
	virtual auto draw() = mac 0x89f40, ios 0x12bc10;
	virtual auto initWithDuration(float p0, cocos2d::CCScene* p1) = mac 0x89ea0, ios 0x12bb80;
	virtual auto sceneOrder() = mac 0x89f30, ios 0x12bc04;
	auto CCTransitionScene();
	auto create(float p0, cocos2d::CCScene* p1);
	auto finish();
	auto hideOutShowIn();
	auto setNewScene(float p0);
	~CCTransitionScene() = mac 0x89cf0, ios 0x12bb44;
}

class cocos2d::CCTransitionSceneOriented : cocos2d::CCTransitionScene {
	virtual auto initWithDuration(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2) = mac 0x8a4e0;
	auto CCTransitionSceneOriented();
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionSceneOriented() = mac 0x8a330;
}

class cocos2d::CCTransitionShrinkGrow : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8cc50;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x8cde0;
	auto CCTransitionShrinkGrow();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionShrinkGrow() = mac 0x8ca90;
}

class cocos2d::CCTransitionSlideInB : cocos2d::CCTransitionSlideInL {
	virtual auto sceneOrder() = mac 0x8c8f0;
	virtual auto initScenes() = mac 0x8c900;
	virtual auto action() = mac 0x8c970;
	auto CCTransitionSlideInB();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSlideInB() = mac 0x8c730;
}

class cocos2d::CCTransitionSlideInL : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8bc90;
	virtual auto sceneOrder() = mac 0x8bd80;
	virtual auto initScenes() = mac 0x8bd90;
	virtual auto action() = mac 0x8be00;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x8be60;
	auto CCTransitionSlideInL();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSlideInL() = mac 0x8bbc0;
}

class cocos2d::CCTransitionSlideInR : cocos2d::CCTransitionSlideInL {
	virtual auto sceneOrder() = mac 0x8c210;
	virtual auto initScenes() = mac 0x8c220;
	virtual auto action() = mac 0x8c290;
	auto CCTransitionSlideInR();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSlideInR() = mac 0x8c050;
}

class cocos2d::CCTransitionSlideInT : cocos2d::CCTransitionSlideInL {
	virtual auto sceneOrder() = mac 0x8c580;
	virtual auto initScenes() = mac 0x8c590;
	virtual auto action() = mac 0x8c600;
	auto CCTransitionSlideInT();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSlideInT() = mac 0x8c3c0;
}

class cocos2d::CCTransitionSplitCols : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8f940;
	virtual auto action() = mac 0x8fa30;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x8fa50;
	auto CCTransitionSplitCols();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSplitCols() = mac 0x8f780;
}

class cocos2d::CCTransitionSplitRows : cocos2d::CCTransitionSplitCols {
	virtual auto action() = mac 0x8fc20;
	auto CCTransitionSplitRows();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionSplitRows() = mac 0x8fb50;
}

class cocos2d::CCTransitionTurnOffTiles : cocos2d::CCTransitionScene, cocos2d::CCTransitionEaseScene {
	virtual auto onEnter() = mac 0x8f590;
	virtual auto sceneOrder() = mac 0x8f580;
	virtual auto easeActionWithAction(cocos2d::CCActionInterval* p0) = mac 0x8f6a0;
	auto CCTransitionTurnOffTiles();
	auto create(float p0, cocos2d::CCScene* p1);
	~CCTransitionTurnOffTiles() = mac 0x8f3c0;
}

class cocos2d::CCTransitionZoomFlipAngular : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8e4a0;
	auto CCTransitionZoomFlipAngular();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionZoomFlipAngular() = mac 0x8e3d0;
}

class cocos2d::CCTransitionZoomFlipX : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8dbb0;
	auto CCTransitionZoomFlipX();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionZoomFlipX() = mac 0x8dae0;
}

class cocos2d::CCTransitionZoomFlipY : cocos2d::CCTransitionSceneOriented {
	virtual auto onEnter() = mac 0x8e020;
	auto CCTransitionZoomFlipY();
	auto create(float p0, cocos2d::CCScene* p1);
	auto create(float p0, cocos2d::CCScene* p1, cocos2d::tOrientation p2);
	~CCTransitionZoomFlipY() = mac 0x8df50;
}

class cocos2d::CCTurnOffTiles : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45d870;
	virtual auto startWithTarget(cocos2d::CCNode* p0) = mac 0x45d7c0;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2) = mac 0x45d4d0;
	auto create(float p0, cocos2d::CCSize const& p1);
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2);
	auto shuffle(unsigned int* p0, unsigned int p1);
	auto turnOffTile(cocos2d::CCPoint const& p0);
	auto turnOnTile(cocos2d::CCPoint const& p0);
}

class cocos2d::CCTwirl : cocos2d::CCGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x129850;
	auto create(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint p2, unsigned int p3, float p4);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, cocos2d::CCPoint p2, unsigned int p3, float p4);
	auto setPosition(cocos2d::CCPoint const& p0);
}

class cocos2d::CCWaves : cocos2d::CCGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x129340;
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3, bool p4, bool p5);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3, bool p4, bool p5);
}

class cocos2d::CCWaves3D : cocos2d::CCGrid3DAction {
	virtual auto update(float p0) = mac 0x127170;
	auto copyWithZone(cocos2d::CCZone* p0);
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
	auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
}

class cocos2d::CCWavesTiles3D : cocos2d::CCTiledGrid3DAction {
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone* p0) { return nullptr; }

	virtual auto update(float p0) = mac 0x45dba0;
	virtual auto initWithDuration(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3) = mac 0x45da40;
	auto create(float p0, cocos2d::CCSize const& p1, unsigned int p2, float p3);
}

class cocos2d::EGLTouchDelegate {
	~EGLTouchDelegate();
}

class cocos2d::TypeInfo {}

class cocos2d::ZipFile {
	auto ZipFile(gd::string const& p0, gd::string const& p1);
	auto fileExists(gd::string const& p0);
	auto getFileData(gd::string const& p0, unsigned long* p1);
	auto getFileData(gd::string const& p0, unsigned long* p1, cocos2d::ZipFilePrivate* p2);
	auto setFilter(gd::string const& p0);
	auto setFilter(gd::string const& p0, cocos2d::ZipFilePrivate* p1);
	~ZipFile();
}

class cocos2d::extension::CCArrayForObjectSorting : cocos2d::CCArray {
	~CCArrayForObjectSorting();
}

class cocos2d::extension::CCControl : cocos2d::CCLayerRGBA {
	virtual bool init() = mac 0x1a71c0, ios 0x10644c;
	~CCControl() = mac 0x1a7380, ios 0x1065a0;
	auto sendActionsForControlEvents(cocos2d::extension::CCControlEvent p0) = mac 0x1a7490;
	auto registerWithTouchDispatcher() = mac 0x1a7420, ios 0x1065ec;
	auto setOpacityModifyRGB(bool p0) = mac 0x1a7c10, ios 0x106aac;
	auto onExit() = mac 0x1a7480, ios 0x10664c;
	auto isTouchInside(cocos2d::CCTouch* p0) = mac 0x1a7de0, ios 0x106bb8;
	auto setEnabled(bool p0) = mac 0x1a7e60, ios 0x106c2c;
	auto onEnter() = mac 0x1a7470, ios 0x106648;
	auto isEnabled() = mac 0x1a7e90, ios 0x106c4c;
	auto setSelected(bool p0) = mac 0x1a7ea0, ios 0x106c54;
	auto isOpacityModifyRGB() = mac 0x1a7d70, ios 0x106b5c;
	auto setHighlighted(bool p0) = mac 0x1a7ed0, ios 0x106c6c;
	auto needsLayout() = mac 0x1a7e50, ios 0x106c28;
	auto getTouchLocation(cocos2d::CCTouch* p0) = mac 0x1a7d90, ios 0x106b6c;
	auto isHighlighted() = mac 0x1a7ef0, ios 0x106c7c;
	auto addTargetWithActionForControlEvents(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, cocos2d::extension::CCControlEvent p2) = mac 0x1a7820;
	auto removeTargetWithActionForControlEvents(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, cocos2d::extension::CCControlEvent p2) = mac 0x1a7950;
	auto isSelected() = mac 0x1a7ec0, ios 0x106c64;
	virtual auto sendActionsForControlEvents(unsigned int p0);
	virtual auto addTargetWithActionForControlEvents(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
	virtual auto removeTargetWithActionForControlEvents(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
	auto CCControl();
	auto addHandleOfControlEvent(int p0, unsigned int p1);
	auto addTargetWithActionForControlEvent(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
	auto create();
	auto dispatchListforControlEvent(unsigned int p0);
	auto getHandleOfControlEvent(unsigned int p0);
	auto hasVisibleParents();
	auto removeHandleOfControlEvent(unsigned int p0);
	auto removeTargetWithActionForControlEvent(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
}

class cocos2d::extension::CCControlButton : cocos2d::extension::CCControl {
	virtual auto init() = mac 0x1a8420;
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1a9c20;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1a9db0;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1a9ef0;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1aa1a0;
	virtual auto getOpacity() = mac 0x1aa090;
	virtual auto setOpacity(unsigned char p0) = mac 0x1a9fc0;
	virtual auto getColor() = mac 0x1aa180;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x1aa0b0;
	virtual auto setEnabled(bool p0) = mac 0x1a8b00;
	virtual auto setSelected(bool p0) = mac 0x1a8b30;
	virtual auto setHighlighted(bool p0) = mac 0x1a8b60;
	virtual auto needsLayout() = mac 0x1a9700;
	virtual auto getPreferredSize() = mac 0x1a8d20;
	virtual auto setPreferredSize(cocos2d::CCSize p0) = mac 0x1a8c60;
	virtual auto getZoomOnTouchDown() = mac 0x1a8c50;
	virtual auto setZoomOnTouchDown(bool p0) = mac 0x1a8c40;
	virtual auto getLabelAnchorPoint() = mac 0x1a8d50;
	virtual auto setLabelAnchorPoint(cocos2d::CCPoint p0) = mac 0x1a8d70;
	virtual auto setMargins(int p0, int p1) = mac 0x1a8ae0;
	virtual auto initWithLabelAndBackgroundSprite(cocos2d::CCNode* p0, cocos2d::extension::CCScale9Sprite* p1) = mac 0x1a8480;
	virtual auto initWithTitleAndFontNameAndFontSize(gd::string p0, char const* p1, float p2) = mac 0x1a88f0;
	virtual auto initWithBackgroundSprite(cocos2d::extension::CCScale9Sprite* p0) = mac 0x1a8a30;
	virtual auto getTitleForState(unsigned int p0) = mac 0x1a8db0;
	virtual auto setTitleForState(cocos2d::CCString* p0, unsigned int p1) = mac 0x1a8e90;
	virtual auto getTitleColorForState(unsigned int p0) = mac 0x1a8f00;
	virtual auto setTitleColorForState(cocos2d::ccColor3B p0, unsigned int p1) = mac 0x1a8f60;
	virtual auto getTitleLabelForState(unsigned int p0) = mac 0x1a9020;
	virtual auto setTitleLabelForState(cocos2d::CCNode* p0, unsigned int p1) = mac 0x1a9060;
	virtual auto setTitleTTFForState(char const* p0, unsigned int p1) = mac 0x1a9140;
	virtual auto getTitleTTFForState(unsigned int p0) = mac 0x1a9240;
	virtual auto setTitleTTFSizeForState(float p0, unsigned int p1) = mac 0x1a92b0;
	virtual auto getTitleTTFSizeForState(unsigned int p0) = mac 0x1a9330;
	virtual auto setTitleBMFontForState(char const* p0, unsigned int p1) = mac 0x1a93a0;
	virtual auto getTitleBMFontForState(unsigned int p0) = mac 0x1a9490;
	virtual auto getBackgroundSpriteForState(unsigned int p0) = mac 0x1a9500;
	virtual auto setBackgroundSpriteForState(cocos2d::extension::CCScale9Sprite* p0, unsigned int p1) = mac 0x1a9540;
	virtual auto setBackgroundSpriteFrameForState(cocos2d::CCSpriteFrame* p0, unsigned int p1) = mac 0x1a96d0;
	auto CCControlButton();
	auto create(cocos2d::extension::CCScale9Sprite* p0);
	auto create(cocos2d::CCNode* p0, cocos2d::extension::CCScale9Sprite* p1);
	auto create(gd::string p0, char const* p1, float p2);
	auto create();
	auto doesAdjustBackgroundImage();
	auto setAdjustBackgroundImage(bool p0);
	auto setBackgroundSprite(cocos2d::extension::CCScale9Sprite* p0);
	auto setTitleLabel(cocos2d::CCNode* p0);
	~CCControlButton() = mac 0x1a8380;
}

class cocos2d::extension::CCControlColourPicker : cocos2d::extension::CCControl {
	~CCControlColourPicker() = mac 0x1aae30;
	auto setColorValue(cocos2d::ccColor3B const& p0) = mac 0x1aac10;
	auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1aae10;
	auto init() = mac 0x1aa400;
	static auto colourPicker() = mac 0x1aaa30;
	auto colorSliderValueChanged(cocos2d::CCObject* p0, unsigned int p1);
	auto create();
	auto hueSliderValueChanged(cocos2d::CCObject* p0, unsigned int p1);
	auto updateControlPicker();
	auto updateHueAndControlPicker();
}

class cocos2d::extension::CCControlHuePicker : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1ac050, ios 0x1147a8;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1ac150, ios 0x114830;
	virtual auto registerWithTouchDispatcher() = mac 0x1abcd0, ios 0x1144c4;
	virtual auto setEnabled(bool p0) = mac 0x1abe70, ios 0x114624;
	virtual auto setHue(float p0) = mac 0x1abd40, ios 0x114534;
	virtual auto setHuePercentage(float p0) = mac 0x1abd70, ios 0x114550;
	virtual auto initWithTargetAndPos(cocos2d::CCNode* p0, cocos2d::CCPoint p1) = mac 0x1abb30, ios 0x1142ec;
	auto CCControlHuePicker();
	auto checkSliderPosition(cocos2d::CCPoint p0, bool p1);
	auto create(cocos2d::CCNode* p0, cocos2d::CCPoint p1);
	auto updateSliderPosition(cocos2d::CCPoint p0);
	~CCControlHuePicker() = mac 0x1ab960, ios 0x11421c;
}

class cocos2d::extension::CCControlPotentiometer : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1ade70;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1ae030;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1ae2a0;
	virtual auto setEnabled(bool p0) = mac 0x1adb80;
	virtual auto isTouchInside(cocos2d::CCTouch* p0) = mac 0x1add50;
	auto CCControlPotentiometer();
	auto angleInDegreesBetweenLineFromPoint_toPoint_toLineFromPoint_toPoint(cocos2d::CCPoint p0, cocos2d::CCPoint p1, cocos2d::CCPoint p2, cocos2d::CCPoint p3);
	auto create(char const* p0, char const* p1, char const* p2);
	auto distanceBetweenPointAndPoint(cocos2d::CCPoint p0, cocos2d::CCPoint p1);
	auto getMaximumValue();
	auto getMinimumValue();
	auto getValue();
	auto initWithTrackSprite_ProgressTimer_ThumbSprite(cocos2d::CCSprite* p0, cocos2d::CCProgressTimer* p1, cocos2d::CCSprite* p2);
	auto potentiometerBegan(cocos2d::CCPoint p0);
	auto potentiometerEnded(cocos2d::CCPoint p0);
	auto potentiometerMoved(cocos2d::CCPoint p0);
	auto setMaximumValue(float p0);
	auto setMinimumValue(float p0);
	auto setValue(float p0);
	~CCControlPotentiometer() = mac 0x1ad590;
}

class cocos2d::extension::CCControlSaturationBrightnessPicker : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1b4030, ios 0x11fb14;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1b4130, ios 0x11fb9c;
	virtual auto registerWithTouchDispatcher() = mac 0x1b3fc0, ios 0x11faa4;
	virtual auto setEnabled(bool p0) = mac 0x1b3b00, ios 0x11f6bc;
	virtual auto initWithTargetAndPos(cocos2d::CCNode* p0, cocos2d::CCPoint p1) = mac 0x1b3820, ios 0x11f4b8;
	virtual auto updateWithHSV(cocos2d::extension::HSV p0) = mac 0x1b3b50, ios 0x11f70c;
	virtual auto updateDraggerWithHSV(cocos2d::extension::HSV p0) = mac 0x1b3bf0, ios 0x11f77c;
	auto CCControlSaturationBrightnessPicker();
	auto checkSliderPosition(cocos2d::CCPoint p0, bool p1);
	auto create(cocos2d::CCNode* p0, cocos2d::CCPoint p1);
	auto updateSliderPosition(cocos2d::CCPoint p0);
	~CCControlSaturationBrightnessPicker() = mac 0x1b3550, ios 0x11f46c;
}

class cocos2d::extension::CCControlSlider : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1de890;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1deaf0;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1decc0;
	virtual auto setEnabled(bool p0) = mac 0x1de610;
	virtual auto needsLayout() = mac 0x1deea0;
	virtual auto isTouchInside(cocos2d::CCTouch* p0) = mac 0x1de750;
	virtual auto setValue(float p0) = mac 0x1de660;
	virtual auto setMinimumValue(float p0) = mac 0x1de6b0;
	virtual auto setMaximumValue(float p0) = mac 0x1de700;
	virtual auto initWithSprites(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2) = mac 0x1de320;
	auto CCControlSlider();
	auto create(char const* p0, char const* p1, char const* p2);
	auto create(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2);
	auto locationFromTouch(cocos2d::CCTouch* p0);
	auto sliderBegan(cocos2d::CCPoint p0);
	auto sliderEnded(cocos2d::CCPoint p0);
	auto sliderMoved(cocos2d::CCPoint p0);
	auto valueForLocation(cocos2d::CCPoint p0);
	~CCControlSlider() = mac 0x1de040;
}

class cocos2d::extension::CCControlStepper : cocos2d::extension::CCControl {
	virtual auto update(float p0) = mac 0x1e12a0;
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e1470;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e1620;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e1750;
	virtual auto setWraps(bool p0) = mac 0x1e0fe0;
	virtual auto setMinimumValue(double p0) = mac 0x1e1060;
	virtual auto setMaximumValue(double p0) = mac 0x1e1090;
	virtual auto setValue(double p0) = mac 0x1e10c0;
	virtual auto getValue() = mac 0x1e10e0;
	virtual auto setStepValue(double p0) = mac 0x1e10f0;
	virtual auto setValueWithSendingEvent(double p0, bool p1) = mac 0x1e1110;
	virtual auto isContinuous() = mac 0x1e1100;
	auto CCControlStepper();
	auto create(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1);
	auto initWithMinusSpriteAndPlusSprite(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1);
	auto startAutorepeat();
	auto stopAutorepeat();
	auto updateLayoutUsingTouchLocation(cocos2d::CCPoint p0);
	~CCControlStepper() = mac 0x1e0960;
}

class cocos2d::extension::CCControlSwitch : cocos2d::extension::CCControl {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e5bc0;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e5cb0;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e5d80;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x1e5ef0;
	virtual auto setEnabled(bool p0) = mac 0x1e5b30;
	auto CCControlSwitch();
	auto create(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2, cocos2d::CCSprite* p3);
	auto create(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2, cocos2d::CCSprite* p3, cocos2d::CCLabelTTF* p4, cocos2d::CCLabelTTF* p5);
	auto initWithMaskSprite(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2, cocos2d::CCSprite* p3);
	auto initWithMaskSprite(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2, cocos2d::CCSprite* p3, cocos2d::CCLabelTTF* p4, cocos2d::CCLabelTTF* p5);
	auto locationFromTouch(cocos2d::CCTouch* p0);
	auto setOn(bool p0);
	auto setOn(bool p0, bool p1);
	~CCControlSwitch() = mac 0x1e5330;
}

class cocos2d::extension::CCControlSwitchSprite : cocos2d::CCSprite, cocos2d::CCActionTweenDelegate {
	virtual auto draw() = mac 0x1e4f90;
	virtual auto updateTweenAction(float p0, char const* p1) = mac 0x1e4ed0;
	auto CCControlSwitchSprite();
	auto initWithMaskSprite(cocos2d::CCSprite* p0, cocos2d::CCSprite* p1, cocos2d::CCSprite* p2, cocos2d::CCSprite* p3, cocos2d::CCLabelTTF* p4, cocos2d::CCLabelTTF* p5);
	auto needsLayout();
	auto offSideWidth();
	auto onSideWidth();
	auto setSliderXPosition(float p0);
	~CCControlSwitchSprite() = mac 0x1e4760;
}

class cocos2d::extension::CCControlUtils {
	static cocos2d::extension::HSV HSVfromRGB(cocos2d::extension::RGBA p0) = mac 0x1e6750;
	static cocos2d::extension::RGBA RGBfromHSV(cocos2d::extension::HSV p0) = mac 0x1e6850;
}

class cocos2d::extension::CCEditBox : cocos2d::extension::CCControlButton, cocos2d::CCIMEDelegate {
	virtual auto setPosition(cocos2d::CCPoint const& p0) = mac 0x26d850;
	virtual auto setAnchorPoint(cocos2d::CCPoint const& p0) = mac 0x26d910;
	virtual auto setContentSize(cocos2d::CCSize const& p0) = mac 0x26d8d0;
	virtual auto setVisible(bool p0) = mac 0x26d890;
	virtual auto onEnter() = mac 0x26d980;
	virtual auto onExit() = mac 0x26d9b0;
	virtual auto visit() = mac 0x26d950;
	virtual auto keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& p0) = mac 0x26d9e0;
	virtual auto keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo& p0) = mac 0x26daf0;
	virtual auto keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo& p0) = mac 0x26db10;
	virtual auto keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo& p0) = mac 0x26db80;
	auto CCEditBox();
	auto create(cocos2d::CCSize const& p0, cocos2d::extension::CCScale9Sprite* p1, cocos2d::extension::CCScale9Sprite* p2, cocos2d::extension::CCScale9Sprite* p3);
	auto getDelegate();
	auto getMaxLength();
	auto getPlaceHolder();
	auto getText();
	auto initWithSizeAndBackgroundSprite(cocos2d::CCSize const& p0, cocos2d::extension::CCScale9Sprite* p1);
	auto registerScriptEditBoxHandler(int p0);
	auto setDelegate(cocos2d::extension::CCEditBoxDelegate* p0);
	auto setFont(char const* p0, int p1);
	auto setFontColor(cocos2d::ccColor3B const& p0);
	auto setFontName(char const* p0);
	auto setFontSize(int p0);
	auto setInputFlag(cocos2d::extension::EditBoxInputFlag p0);
	auto setInputMode(cocos2d::extension::EditBoxInputMode p0);
	auto setMaxLength(int p0);
	auto setPlaceHolder(char const* p0);
	auto setPlaceholderFont(char const* p0, int p1);
	auto setPlaceholderFontColor(cocos2d::ccColor3B const& p0);
	auto setPlaceholderFontName(char const* p0);
	auto setPlaceholderFontSize(int p0);
	auto setReturnType(cocos2d::extension::KeyboardReturnType p0);
	auto setText(char const* p0);
	auto touchDownAction(cocos2d::CCObject* p0, unsigned int p1);
	auto unregisterScriptEditBoxHandler();
	~CCEditBox() = mac 0x26cfd0;
}

class cocos2d::extension::CCEditBoxImpl {
	~CCEditBoxImpl();
}

class cocos2d::extension::CCHttpClient : cocos2d::CCObject {
	auto CCHttpClient();
	auto destroyInstance();
	auto dispatchResponseCallbacks(float p0);
	auto getInstance();
	auto lazyInitThreadSemphore();
	auto send(cocos2d::extension::CCHttpRequest* p0);
	~CCHttpClient() = mac 0x23da60, ios 0x1638ec;
}

class cocos2d::extension::CCHttpRequest : cocos2d::CCObject {
	auto CCHttpRequest();
	~CCHttpRequest();
}

class cocos2d::extension::CCHttpResponse : cocos2d::CCObject {
	auto CCHttpResponse(cocos2d::extension::CCHttpRequest* p0);
	~CCHttpResponse();
}

class cocos2d::extension::CCInvocation : cocos2d::CCObject {
	auto CCInvocation(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
	auto create(cocos2d::CCObject* p0, cocos2d::extension::SEL_CCControlHandler p1, unsigned int p2);
	auto invoke(cocos2d::CCObject* p0);
	~CCInvocation();
}

class cocos2d::extension::CCScale9Sprite : cocos2d::CCNodeRGBA {
	CCScale9Sprite() = mac 0x211330;
	static cocos2d::extension::CCScale9Sprite* create(char const* p0) = mac 0x2130d0;
	static cocos2d::extension::CCScale9Sprite* create(char const* p0, cocos2d::CCRect p1) = mac 0x212ef0;
	~CCScale9Sprite() = mac 0x211590, ios 0x13d4b8;
	virtual auto init() = mac 0x2115d0, ios 0x13d4f4;
	virtual auto setContentSize(cocos2d::CCSize const& p0) = mac 0x2127c0, ios 0x13e400;
	virtual auto visit() = mac 0x213bf0, ios 0x13f144;
	virtual auto getOpacity() = mac 0x213f30, ios 0x13f30c;
	virtual auto setOpacity(unsigned char p0) = mac 0x213dd0, ios 0x13f250;
	virtual auto updateDisplayedOpacity(unsigned char p0) = mac 0x2139b0, ios 0x13ef98;
	virtual auto getColor() = mac 0x213db0, ios 0x13f240;
	virtual auto setColor(cocos2d::ccColor3B const& p0) = mac 0x213c20, ios 0x13f178;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const& p0) = mac 0x213a20, ios 0x13f004;
	virtual auto setOpacityModifyRGB(bool p0) = mac 0x213830, ios 0x13eed0;
	virtual auto isOpacityModifyRGB() = mac 0x213990, ios 0x13ef88;
	virtual auto getPreferredSize() = mac 0x213620, ios 0x13ed18;
	virtual auto setPreferredSize(cocos2d::CCSize p0) = mac 0x2135f0, ios 0x13ece4;
	virtual auto getCapInsets() = mac 0x2136e0, ios 0x13edc4;
	virtual auto setCapInsets(cocos2d::CCRect p0) = mac 0x213640, ios 0x13ed24;
	virtual auto getInsetLeft() = mac 0x213b30, ios 0x13f104;
	virtual auto setInsetLeft(float p0) = mac 0x213b70, ios 0x13f124;
	virtual auto getInsetTop() = mac 0x213b40, ios 0x13f10c;
	virtual auto setInsetTop(float p0) = mac 0x213b90, ios 0x13f12c;
	virtual auto getInsetRight() = mac 0x213b50, ios 0x13f114;
	virtual auto setInsetRight(float p0) = mac 0x213bb0, ios 0x13f134;
	virtual auto getInsetBottom() = mac 0x213b60, ios 0x13f11c;
	virtual auto setInsetBottom(float p0) = mac 0x213bd0, ios 0x13f13c;
	virtual auto initWithBatchNode(cocos2d::CCSpriteBatchNode* p0, cocos2d::CCRect p1, bool p2, cocos2d::CCRect p3) = mac 0x211690, ios 0x13d5c8;
	virtual auto initWithBatchNode(cocos2d::CCSpriteBatchNode* p0, cocos2d::CCRect p1, cocos2d::CCRect p2) = mac 0x211630, ios 0x13d55c;
	virtual auto initWithFile(char const* p0, cocos2d::CCRect p1, cocos2d::CCRect p2) = mac 0x212d50, ios 0x13e8e0;
	virtual auto initWithFile(char const* p0, cocos2d::CCRect p1) = mac 0x212e80, ios 0x13e964;
	virtual auto initWithFile(cocos2d::CCRect p0, char const* p1) = mac 0x212f80, ios 0x13ea6c;
	virtual auto initWithFile(char const* p0) = mac 0x213080, ios 0x13ead8;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame* p0, cocos2d::CCRect p1) = mac 0x213140, ios 0x13eb30;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame* p0) = mac 0x213250, ios 0x13ebbc;
	virtual auto initWithSpriteFrameName(char const* p0, cocos2d::CCRect p1) = mac 0x213310, ios 0x13ec14;
	virtual auto initWithSpriteFrameName(char const* p0) = mac 0x213410, ios 0x13ec8c;
	virtual auto updateWithBatchNode(cocos2d::CCSpriteBatchNode* p0, cocos2d::CCRect p1, bool p2, cocos2d::CCRect p3) = mac 0x211730, ios 0x13d674;
	virtual auto setSpriteFrame(cocos2d::CCSpriteFrame* p0) = mac 0x213a90, ios 0x13f070;
	auto create(cocos2d::CCRect p0, char const* p1);
	auto create(char const* p0, cocos2d::CCRect p1, cocos2d::CCRect p2);
	auto create();
	auto createWithSpriteFrame(cocos2d::CCSpriteFrame* p0);
	auto createWithSpriteFrame(cocos2d::CCSpriteFrame* p0, cocos2d::CCRect p1);
	auto createWithSpriteFrameName(char const* p0);
	auto createWithSpriteFrameName(char const* p0, cocos2d::CCRect p1);
	auto resizableSpriteWithCapInsets(cocos2d::CCRect p0);
	auto setBlendAdditive(bool p0);
	auto updateCapInset();
	auto updatePositions();
}

class cocos2d::extension::CCScrollView : cocos2d::CCLayer {
	CCScrollView() = mac 0x214800;
	~CCScrollView() = mac 0x214c30;
	virtual auto init() = mac 0x214fb0;
	virtual auto setContentSize(cocos2d::CCSize const& p0) = mac 0x215eb0;
	virtual auto getContentSize() const = mac 0x215e90;
	virtual auto addChild(cocos2d::CCNode* p0) = mac 0x216160;
	virtual auto addChild(cocos2d::CCNode* p0, int p1) = mac 0x216120;
	virtual auto addChild(cocos2d::CCNode* p0, int p1, int p2) = mac 0x216080;
	virtual auto visit() = mac 0x2164a0;
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x216620;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x216890;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x216c70;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x216d30;
	virtual auto registerWithTouchDispatcher() = mac 0x214ff0;
	virtual auto setTouchEnabled(bool p0) = mac 0x215250;
	virtual auto setDirection(cocos2d::extension::CCScrollViewDirection p0);
	auto afterDraw();
	auto beforeDraw();
	auto create(cocos2d::CCSize p0, cocos2d::CCNode* p1);
	auto create();
	auto deaccelerateScrolling(float p0);
	auto getContainer();
	auto getContentOffset();
	auto getScriptHandler(int p0);
	auto getViewRect();
	auto getZoomScale();
	auto initWithViewSize(cocos2d::CCSize p0, cocos2d::CCNode* p1);
	auto isNodeVisible(cocos2d::CCNode* p0);
	auto maxContainerOffset();
	auto minContainerOffset();
	auto pause(cocos2d::CCObject* p0);
	auto performedAnimatedScroll(float p0);
	auto registerScriptHandler(int p0, int p1);
	auto relocateContainer(bool p0);
	auto resume(cocos2d::CCObject* p0);
	auto setContainer(cocos2d::CCNode* p0);
	auto setContentOffset(cocos2d::CCPoint p0, bool p1);
	auto setContentOffsetInDuration(cocos2d::CCPoint p0, float p1);
	auto setViewSize(cocos2d::CCSize p0);
	auto setZoomScale(float p0);
	auto setZoomScale(float p0, bool p1);
	auto setZoomScaleInDuration(float p0, float p1);
	auto stoppedAnimatedScroll(cocos2d::CCNode* p0);
	auto unregisterScriptHandler(int p0);
	auto updateInset();
}

class cocos2d::extension::CCScrollViewDelegate {
	~CCScrollViewDelegate();
}

class cocos2d::extension::CCSortableObject {
	~CCSortableObject();
}

class cocos2d::extension::CCTableView : cocos2d::extension::CCScrollView, cocos2d::extension::CCScrollViewDelegate {
	virtual auto ccTouchBegan(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x234bf0;
	virtual auto ccTouchMoved(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x234da0;
	virtual auto ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x234ae0;
	virtual auto ccTouchCancelled(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) = mac 0x234e50;
	virtual auto scrollViewDidScroll(cocos2d::extension::CCScrollView* p0) = mac 0x234830;
	virtual auto scrollViewDidZoom(cocos2d::extension::CCScrollView* p0);
	auto CCTableView();
	auto __indexFromOffset(cocos2d::CCPoint p0);
	auto __offsetFromIndex(unsigned int p0);
	auto _addCellIfNecessary(cocos2d::extension::CCTableViewCell* p0);
	auto _indexFromOffset(cocos2d::CCPoint p0);
	auto _moveCellOutOfSight(cocos2d::extension::CCTableViewCell* p0);
	auto _offsetFromIndex(unsigned int p0);
	auto _setIndexForCell(unsigned int p0, cocos2d::extension::CCTableViewCell* p1);
	auto _updateCellPositions();
	auto _updateContentSize();
	auto cellAtIndex(unsigned int p0);
	auto create(cocos2d::extension::CCTableViewDataSource* p0, cocos2d::CCSize p1);
	auto create(cocos2d::extension::CCTableViewDataSource* p0, cocos2d::CCSize p1, cocos2d::CCNode* p2);
	auto dequeueCell();
	auto getVerticalFillOrder();
	auto initWithViewSize(cocos2d::CCSize p0, cocos2d::CCNode* p1);
	auto insertCellAtIndex(unsigned int p0);
	auto reloadData();
	auto removeCellAtIndex(unsigned int p0);
	auto setVerticalFillOrder(cocos2d::extension::CCTableViewVerticalFillOrder p0);
	auto unregisterAllScriptHandler();
	auto updateCellAtIndex(unsigned int p0);
	~CCTableView() = mac 0x233a60;
}

class cocos2d::extension::CCTableViewCell : cocos2d::CCNode, cocos2d::extension::CCSortableObject {
	virtual auto setObjectID(unsigned int p0);
	virtual auto getObjectID();
	auto getIdx();
	auto reset();
	auto setIdx(unsigned int p0);
	~CCTableViewCell();
}

class cocos2d::extension::ColorPickerDelegate {
	virtual auto colorValueChanged(cocos2d::ccColor3B p0);
}
