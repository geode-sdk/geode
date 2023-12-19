// clang-format off

[[link(win, android)]]
class cocos2d::CCActionTween {
	static cocos2d::CCActionTween* create(float, char const*, float, float) = mac 0x447590;
}

[[link(win, android)]]
class cocos2d::CCActionManager {
	CCActionManager() {
		m_pTargets = nullptr;
		m_pCurrentTarget = nullptr;
		m_bCurrentTargetSalvaged = false;
	}
	~CCActionManager() {}

	virtual auto update(float) = mac 0x10c9a0;
	auto addAction(cocos2d::CCAction*, cocos2d::CCNode*, bool) = mac 0x10bed0;
	auto pauseTarget(cocos2d::CCObject*) = mac 0x10bc50;
	auto resumeTargets(cocos2d::CCSet*) = mac 0x10be80;
	auto resumeTarget(cocos2d::CCObject*) = mac 0x10bd20;
}

[[link(win, android)]]
class cocos2d::CCAnimate {
	static cocos2d::CCAnimate* create(cocos2d::CCAnimation*) = mac 0x1f8fc0;
}

[[link(win, android)]]
class cocos2d::CCAnimation {
	static auto createWithSpriteFrames(cocos2d::CCArray*, float) = mac 0x140df0;
}

[[link(win, android)]]
class cocos2d::CCApplication {
	virtual auto run();
	virtual auto getCurrentLanguage() = mac 0x1a3f40, ios 0x10e508;
	virtual auto getTargetPlatform() = mac 0x1a3f20, ios 0x10e57c;
	virtual auto openURL(char const*) = mac 0x1a4550, ios 0x10e7a4;
	virtual auto setAnimationInterval(double) = mac 0x1a3ee0, ios 0x10e494;
	static auto sharedApplication() = mac 0x1a3f30;
	
	[[link(win)]]
	bool getControllerConnected() const = mac 0x27d1b0;
	// ~CCApplication() = mac 0x1a3d10, ios 0x10e384;
	CCApplication() {}
	~CCApplication() {}
}

[[link(win, android)]]
class cocos2d::CCArray {
	// auto addObject(cocos2d::CCObject*) = mac 0x419f90, ios 0x16504c;
	auto addObjectNew(cocos2d::CCObject*) = mac 0x41a450;
	// auto addObjectsFromArray(cocos2d::CCArray*) = mac 0x41a2d0;
	// auto containsObject(cocos2d::CCObject*) const = mac 0x41a3e0;
	// auto count() const = mac 0x41a2f0, ios 0x1650e8;
	// static cocos2d::CCArray* create() = mac 0x419cb0, ios 0x164ec8;
	// static auto createWithObject(cocos2d::CCObject*) = mac 0x419d50;
	// auto fastRemoveObject(cocos2d::CCObject*) = mac 0x41a520;
	// auto fastRemoveObjectAtIndex(unsigned int) = mac 0x41a500;
	auto fastRemoveObjectAtIndexNew(unsigned int) = mac 0x41a510;
	// auto insertObject(cocos2d::CCObject*, unsigned int) = mac 0x41a460;
	// auto lastObject() = mac 0x41a360;
	// auto objectAtIndex(unsigned int) = mac 0x41a340, ios 0x16510c;
	// auto removeAllObjects() = mac 0x41a4f0, ios 0x1651f0;
	// auto removeLastObject(bool) = mac 0x41a470;
	// auto removeObject(cocos2d::CCObject*, bool) = mac 0x41a490;
	// auto removeObjectAtIndex(unsigned int, bool) = mac 0x41a4b0;
	// auto stringAtIndex(unsigned int) = mac 0x41a320;
}

[[link(win, android)]]
class cocos2d::CCBezierTo {
	static cocos2d::CCBezierTo* create(float, cocos2d::_ccBezierConfig const&) = mac 0x1f6c10;
}

[[link(win, android)]]
class cocos2d::CCBMFontConfiguration {
	static cocos2d::CCBMFontConfiguration* create(char const*) = mac 0x3450f0;
}

[[link(win, android)]]
class cocos2d::CCCallFunc {
	static auto create(cocos2d::CCObject*, cocos2d::SEL_CallFunc) = mac 0x454d90;
}

[[link(win, android)]]
class cocos2d::CCCallFuncO {
	static auto create(cocos2d::CCObject*, cocos2d::SEL_CallFuncO, cocos2d::CCObject*) = mac 0x455940;
}

[[link(win, android)]]
class cocos2d::CCCallFuncND {
	static auto create(cocos2d::CCObject*, cocos2d::SEL_CallFuncND, void*) = mac 0x455470;
}

[[link(win, android)]]
class cocos2d::CCClippingNode {
	CCClippingNode() {
		m_pStencil = nullptr;
		m_fAlphaThreshold = 1.f;
		m_bInverted = false;
	}
	~CCClippingNode() = mac 0x4191e0, win 0xadae0;
	static cocos2d::CCClippingNode* create() = mac 0x4192a0;
	static cocos2d::CCClippingNode* create(cocos2d::CCNode*) = mac 0x419330;
	// Inlined in create() call
	auto init() = mac 0x4193e0;
	// Inlined in create() call
	auto init(cocos2d::CCNode* stencil) = mac 0x419400;
	auto getAlphaThreshold() const = mac 0x419a10;
	auto getStencil() const = mac 0x4199c0;
	auto isInverted() const = mac 0x419a30;
	auto onEnter() = mac 0x419470;
	auto onEnterTransitionDidFinish() = mac 0x4194a0;
	auto onExit() = mac 0x419500;
	auto onExitTransitionDidStart() = mac 0x4194d0;
	auto setAlphaThreshold(float) = mac 0x419a20;
	auto setInverted(bool) = mac 0x419a40;
	auto setStencil(cocos2d::CCNode*) = mac 0x4199d0;
	auto visit() = mac 0x419530;
}

// not sure how the entire class is supposed to be added to only windows cuz idk if it even exists on mac
// class cocos2d::CCControllerHandler {
//	CCControllerHandler(int userIndex) = win 0xc7f50;
//	cocos2d::CCControllerState getState() = win 0xc7f80;
//	void updateConnected() = win 0xc7fb0;
//}

[[link(win, android)]]
class cocos2d::CCConfiguration {
	void gatherGPUInfo() = mac 0x2a6e10;
}

[[link(win)]]
class cocos2d::CCDelayTime {
	static cocos2d::CCDelayTime* create(float) = mac 0x1f4380;
}

[[link(win, android)]]
class cocos2d::CCDictionary {
	auto allKeys() = mac 0x190450, ios 0x2de774;
	auto count() = mac 0x190430;
	static cocos2d::CCDictionary* create() = mac 0x192650;
	auto objectForKey(intptr_t) = mac 0x190bb0, ios 0x2decc0;
	auto objectForKey(gd::string const&) = mac 0x190870, ios 0x2de988;
	auto removeAllObjects() = mac 0x190220;
	auto removeObjectForKey(intptr_t) = mac 0x1921d0;
	auto removeObjectForKey(gd::string const&) = mac 0x191d40;
	auto setObject(cocos2d::CCObject*, intptr_t) = mac 0x191790, ios 0x2df734;
	auto setObject(cocos2d::CCObject*, gd::string const&) = mac 0x190dc0, ios 0x2dee7c;
	auto valueForKey(intptr_t) = mac 0x190cf0;
	auto valueForKey(gd::string const&) = mac 0x1907a0;
}

[[link(win, android)]]
class cocos2d::CCDirector {
	CCDirector() {}
	~CCDirector() {}
	// virtual ~CCDirector() = mac 0x2493a0;
	virtual auto init() = mac 0x248df0;
	virtual auto getScheduler() = mac 0x24af00;
	virtual auto setScheduler(cocos2d::CCScheduler*) = mac 0x24aec0;
	virtual auto getActionManager() = mac 0x24af50;
	virtual auto setActionManager(cocos2d::CCActionManager*) = mac 0x24af10;
	virtual auto getTouchDispatcher() = mac 0x24afa0;
	virtual auto setTouchDispatcher(cocos2d::CCTouchDispatcher*) = mac 0x24af60;
	virtual auto getKeypadDispatcher() = mac 0x24b090;
	virtual auto setKeypadDispatcher(cocos2d::CCKeypadDispatcher*) = mac 0x24b050;
	virtual auto getKeyboardDispatcher() = mac 0x24aff0;
	virtual auto setKeyboardDispatcher(cocos2d::CCKeyboardDispatcher*) = mac 0x24afb0;
	virtual auto getMouseDispatcher() = mac 0x24b040;
	virtual auto setMouseDispatcher(cocos2d::CCMouseDispatcher*) = mac 0x24b000;
	virtual auto getAccelerometer() = mac 0x24b0e0;
	virtual auto setAccelerometer(cocos2d::CCAccelerometer*) = mac 0x24b0a0;
	virtual auto getDeltaTime() = mac 0x249bd0;

	auto getWinSize() = mac 0x24a0f0, ios 0xece34;
	auto pushScene(cocos2d::CCScene*) = mac 0x24a620;
	auto replaceScene(cocos2d::CCScene*) = mac 0x24a6d0, ios 0xed08c;
	auto resetSmoothFixCounter() = mac 0x249bc0;
	static auto sharedDirector() = mac 0x248cb0, ios 0xebf84;
	auto calculateDeltaTime() = mac 0x2497a0;
	auto calculateMPF() = mac 0x19eac0;
	auto convertToGL(cocos2d::CCPoint const&) = mac 0x24a210;
	auto convertToUI(cocos2d::CCPoint const&) = mac 0x24a340;
	auto drawScene() = mac 0x249690;
	auto willSwitchToScene(cocos2d::CCScene* scene) = mac 0x24a520;
	auto setOpenGLView(cocos2d::CCEGLView *pobOpenGLView) = mac 0x249be0;
	auto updateScreenScale(cocos2d::CCSize) = mac 0x249f10;
	auto setContentScaleFactor(float);

	auto setNextScene() = mac 0x2498d0;
	auto showStats() = mac 0x2499e0;
	auto updateContentScale(cocos2d::TextureQuality) = mac 0x249ff0;
	auto popSceneWithTransition(float, cocos2d::PopTransition) = mac 0x24a8b0;
}

[[link(win, android)]]
class cocos2d::CCDrawNode {
	CCDrawNode() = mac 0x378b40, win 0x6b9f0;
	auto clear() = mac 0x379e80;
	static cocos2d::CCDrawNode* create() = mac 0x378d00;
	auto drawPolygon(cocos2d::CCPoint*, unsigned int, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&) = mac 0x3797f0;
	auto drawSegment(cocos2d::CCPoint const&, cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&) = mac 0x3792d0;
	auto drawDot(cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&) = mac 0x379100;

	auto getBlendFunc() const = mac 0x379ea0;
	virtual auto init() = mac 0x378e00;
	auto setBlendFunc(cocos2d::_ccBlendFunc const&) = mac 0x379eb0;
	virtual auto draw() = mac 0x379020;
	virtual ~CCDrawNode() = mac 0x378cc0, win 0x6ba60;
}

[[link(win, android)]]
class cocos2d::CCAction {
	CCAction() = mac 0x35b610, win 0x7a6d0;
	virtual ~CCAction() = mac 0x35b6b0, win 0x7a7f0;
    auto stop() = mac 0x35b860;
	auto copyWithZone(cocos2d::CCZone* zone) = mac 0x35b7a0;
	auto startWithTarget(cocos2d::CCNode* target) = mac 0x35b850;
	auto step(float dt) = mac 0x35b880;
	auto isDone() = mac 0x35b870;
	auto update(float time) = mac 0x35b890;
}

[[link(win, android)]]
class cocos2d::CCFiniteTimeAction {
	// same as CCActionInterval::reverse i think
	auto reverse() = mac 0x1f2720;
}

[[link(win, android)]]
class cocos2d::CCActionInterval {
	auto copyWithZone(cocos2d::CCZone* zone) = mac 0x1f2550;
	auto isDone() = mac 0x1f2640;
    auto startWithTarget(cocos2d::CCNode* pTarget) = mac 0x1f2700;
	auto step(float dt) = mac 0x1f2660;
	auto reverse() = mac 0x1f2720;
	bool initWithDuration(float d) = mac 0x1f2510;
}

[[link(win, android)]]
class cocos2d::CCEaseBackIn {
	static cocos2d::CCEaseBackIn* create(cocos2d::CCActionInterval*) = mac 0x2a41b0;
}

[[link(win, android)]]
class cocos2d::CCEaseElasticIn {
	static cocos2d::CCEaseElasticIn* create(cocos2d::CCActionInterval*, float) = mac 0x2a2e00;
}

[[link(win, android)]]
class cocos2d::CCEaseElasticOut {
	static cocos2d::CCEaseElasticOut* create(cocos2d::CCActionInterval*, float) = mac 0x2a3080;
}

[[link(win, android)]]
class cocos2d::CCEaseIn {
	static cocos2d::CCEaseIn* create(cocos2d::CCActionInterval*, float) = mac 0x2a1960;
}

[[link(win, android)]]
class cocos2d::CCEaseInOut {
	static cocos2d::CCEaseInOut* create(cocos2d::CCActionInterval*, float) = mac 0x2a1d80;
}

[[link(win, android)]]
class cocos2d::CCEaseOut {
	static cocos2d::CCEaseOut* create(cocos2d::CCActionInterval*, float) = mac 0x2a1b70;
}

[[link(win)]]
class cocos2d::CCEGLView {
    CCEGLView();
	static cocos2d::CCEGLView* sharedOpenGLView() = mac 0x295320;
	virtual void swapBuffers() = mac 0x295510;
	void updateWindow(int width, int height);
	void setupWindow(cocos2d::CCRect);
	bool initGlew();
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
}

[[link(win, android)]]
class cocos2d::CCEGLViewProtocol {
    CCEGLViewProtocol() = win 0xbac00;
    virtual ~CCEGLViewProtocol() = win 0xbacc0;
    auto getViewPortRect() const = mac 0x29e2f0;
    auto getScaleX() const = mac 0x29e300;
    auto getScaleY() const = mac 0x29e310;
    virtual auto setDesignResolutionSize(float, float, ResolutionPolicy);
	virtual void setFrameSize(float, float) = mac 0x29d960;
}

[[link(win, android)]]
class cocos2d::CCFadeOut {
	static cocos2d::CCFadeOut* create(float) = mac 0x1f7d80;
}

[[link(win, android)]]
class cocos2d::CCFadeTo {
	static cocos2d::CCFadeTo* create(float, unsigned char) = mac 0x1f7ff0;
}

[[link(win, android)]]
class cocos2d::CCFileUtils : cocos2d::TypeInfo {
	static cocos2d::CCFileUtils* sharedFileUtils() = mac 0x377030, ios 0x159450;
	static void purgeFileUtils();
    virtual void addSearchPath(const char* path);
	virtual void removeSearchPath(const char *path);
    virtual gd::string fullPathForFilename(const char* filename, bool unk) = mac 0x23f940;
    void removeAllPaths() = mac 0x241600;
}

[[link(win, android)]]
class cocos2d::CCGLProgram {
	auto setUniformsForBuiltins() = mac 0x232c70;
	auto use() = mac 0x231d70;
	bool compileShader(unsigned int* shader, unsigned int type, const char* source) = mac 0x231a30;
}

[[link(win, android)]]
class cocos2d::CCHide {
	static cocos2d::CCHide* create() = mac 0x4543e0;
}

[[link(win, android)]]
class cocos2d::CCIMEDelegate {
	~CCIMEDelegate() {
	    CCIMEDispatcher::sharedDispatcher()->removeDelegate(this);
	}
	virtual auto attachWithIME() = mac 0x2776a0, ios 0x12d3d4;
	virtual auto detachWithIME() = mac 0x277880, ios 0x12d4e8;
	CCIMEDelegate() {
		CCIMEDispatcher::sharedDispatcher()->addDelegate(this);
	}
}

[[link(win, android)]]
class cocos2d::CCIMEDispatcher {
	static auto sharedDispatcher() = mac 0x2773f0, ios 0x12d170;
	auto addDelegate(cocos2d::CCIMEDelegate*) = mac 0x277480, ios 0x12d204;
	auto removeDelegate(cocos2d::CCIMEDelegate*) = mac 0x2775f0, ios 0x12d2c4;
	void dispatchInsertText(const char* text, int len) = mac 0x277ac0;
	void dispatchDeleteBackward() = mac 0x277af0;
}

[[link(win, android)]]
class cocos2d::CCImage {
	CCImage() = mac 0x24fa00, win 0xc5fd0;
	virtual ~CCImage() = mac 0x24fa80, win 0xc6100;
	auto initWithImageFile(const char*, cocos2d::CCImage::EImageFormat imageType);
	auto initWithImageFileThreadSafe(const char*, cocos2d::CCImage::EImageFormat imageType);
	auto initWithImageData(void*, int, cocos2d::CCImage::EImageFormat, int, int, int) = mac 0x24fcb0;
}

[[link(win, android)]]
class cocos2d::CCKeyboardDispatcher {
	bool dispatchKeyboardMSG(cocos2d::enumKeyCodes, bool) = mac 0xe8190;
	const char* keyToString(cocos2d::enumKeyCodes) = mac 0xe8450;
	void updateModifierKeys(bool shft, bool ctrl, bool alt, bool cmd) = mac 0xe8430;
}

[[link(win, android)]]
class cocos2d::CCKeyboardHandler {
    static cocos2d::CCKeyboardHandler* handlerWithDelegate(cocos2d::CCKeyboardDelegate*) = mac 0x242030;
    virtual auto initWithDelegate(cocos2d::CCKeyboardDelegate*) = mac 0x241ff0, ios 0x13f8b8;
    ~CCKeyboardHandler() = mac 0x241e90, ios 0x13f87c, win 0x99a10;
}

[[link(win, android)]]
class cocos2d::CCKeypadHandler {
    static cocos2d::CCKeypadHandler* handlerWithDelegate(cocos2d::CCKeypadDelegate*) = mac 0x1ff2d0;
    virtual auto initWithDelegate(cocos2d::CCKeypadDelegate*) = mac 0x1ff290, ios 0x69; // iOS stub
    virtual ~CCKeypadHandler() = mac 0x1ff130, ios 0x2c530, win 0xf5f50;
    cocos2d::CCKeypadDelegate* getDelegate() = mac 0x1ff0b0;
}


[[link(win, android)]]
class cocos2d::CCLabelBMFont {
	CCLabelBMFont() = mac 0x347b60;
	static cocos2d::CCLabelBMFont* create(char const*, char const*) = mac 0x347660;
	auto limitLabelWidth(float, float, float) = mac 0x34a6e0, ios 0x21b740;
	auto setFntFile(char const*) = mac 0x34a5f0;
	auto createFontChars() = mac 0x347fc0;
	static auto create() = mac 0x3473f0;

	virtual auto init() = mac 0x347b10, ios 0x2198e0;
	bool initWithString(const char* str, const char* fnt, float width, cocos2d::CCTextAlignment align, cocos2d::CCPoint offset) = mac 0x347710;
	virtual auto setScaleX(float) = mac 0x34a5b0, ios 0x21b6e8;
	virtual auto setScaleY(float) = mac 0x34a5d0, ios 0x21b714;
	virtual auto setScale(float) = mac 0x34a590, ios 0x21b6bc;
	virtual auto setAnchorPoint(cocos2d::CCPoint const&) = mac 0x349440, ios 0x21aa7c;
	virtual auto setString(char const*) = mac 0x348990, ios 0x21a408;
	virtual auto setString(char const*, bool) = mac 0x3489e0, ios 0x21a42c;
	virtual auto getString() = mac 0x348bf0, ios 0x21a60c;
	virtual auto setCString(char const*) = mac 0x348c10, ios 0x21a63c;
	virtual auto updateLabel() = mac 0x349480, ios 0x21aad0;
	virtual auto setAlignment(cocos2d::CCTextAlignment) = mac 0x34a530, ios 0x21b68c;
	virtual auto setWidth(float) = mac 0x34a550, ios 0x21b69c;
	virtual auto setLineBreakWithoutSpace(bool) = mac 0x34a570, ios 0x21b6ac;
	virtual auto isOpacityModifyRGB() = mac 0x3490e0, ios 0x21a8c4;
	virtual auto setOpacityModifyRGB(bool) = mac 0x348f70, ios 0x21a80c;
	virtual auto getOpacity() = mac 0x348df0, ios 0x21a748;
	virtual auto getDisplayedOpacity() = mac 0x348e10, ios 0x21a758;
	virtual auto setOpacity(unsigned char) = mac 0x348e30, ios 0x21a768;
	virtual auto updateDisplayedOpacity(unsigned char) = mac 0x349100, ios 0x21a8d4;
	virtual auto isCascadeOpacityEnabled() = mac 0x349400, ios 0x21aa5c;
	virtual auto setCascadeOpacityEnabled(bool) = mac 0x349420, ios 0x21aa6c;
	virtual auto getColor() = mac 0x348c30, ios 0x21a648;
	virtual auto getDisplayedColor() = mac 0x348c50, ios 0x21a658;
	virtual auto setColor(cocos2d::ccColor3B const&) = mac 0x348c70, ios 0x21a668;
	virtual auto updateDisplayedColor(cocos2d::ccColor3B const&) = mac 0x349210, ios 0x21a968;
	virtual auto isCascadeColorEnabled() = mac 0x3493c0, ios 0x21aa3c;
	virtual auto setCascadeColorEnabled(bool) = mac 0x3493e0, ios 0x21aa4c;
	virtual auto setString(unsigned short*, bool) = mac 0x348a60, ios 0x21a4b4;
	virtual ~CCLabelBMFont() = mac 0x347e80, win 0x9be70;
}

[[link(win, android)]]
class cocos2d::CCLabelTTF {
	static cocos2d::CCLabelTTF* create() = mac 0x1fa7e0;
	static cocos2d::CCLabelTTF* create(char const*, char const*, float) = mac 0x1fa840;
	auto updateTexture() = mac 0x1fadc0;
	virtual auto setString(char const*) = mac 0x1fad70;
}

[[link(win, android)]]
class cocos2d::CCLayer {
    CCLayer() = mac 0x2725b0, ios 0xc7708, win 0xa15e0;
    virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2734d0, ios 0xc810c;
    virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x273650, ios 0xc82ac;
    virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2735d0, ios 0xc820c;
    virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x273550, ios 0xc816c;
    virtual auto ccTouchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x2736d0, ios 0xc834c;
    virtual auto ccTouchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x273850, ios 0xc852c;
    virtual auto ccTouchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x2737d0, ios 0xc848c;
    virtual auto ccTouchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x273750, ios 0xc83ec;
    static cocos2d::CCLayer* create() = mac 0x272a00;
    virtual auto didAccelerate(cocos2d::CCAcceleration*) = mac 0x272ea0, ios 0xc7c5c;
    virtual auto getTouchMode() = mac 0x272e10, ios 0xc7b9c;
    virtual auto getTouchPriority() = mac 0x272e00, ios 0xc7b94;
    virtual auto init() = mac 0x2729a0, ios 0xc7894;
    virtual auto isAccelerometerEnabled() = mac 0x272e20, ios 0xc7ba4;
    virtual auto isKeyboardEnabled() = mac 0x273010, ios 0xc7d70;
    virtual auto isKeypadEnabled() = mac 0x272f70, ios 0xc7cf4;
    virtual auto isMouseEnabled() = mac 0x273090, ios 0xc7de0;
    virtual auto isTouchEnabled() = mac 0x272ce0, ios 0xc7a54;
    virtual auto keyBackClicked() = mac 0x273160, ios 0xc7e50;
    virtual auto keyDown(cocos2d::enumKeyCodes) = mac 0x273280, ios 0xc7efc;
    virtual auto keyMenuClicked() = mac 0x273200, ios 0xc7eac;
    virtual auto onEnter() = mac 0x273300, ios 0xc7f38;
    virtual auto onEnterTransitionDidFinish() = mac 0x273490, ios 0xc80cc;
    virtual auto onExit() = mac 0x2733c0, ios 0xc8004;
    virtual auto registerScriptTouchHandler(int, bool, int, bool) = mac 0x272bd0, ios 0xc79c8;
    virtual auto registerWithTouchDispatcher() = mac 0x272b40, ios 0xc7958;
    virtual auto setAccelerometerEnabled(bool) = mac 0x272e30, ios 0xc7bac;
    virtual auto setAccelerometerInterval(double) = mac 0x272e70, ios 0xc7c10;
    virtual auto setKeyboardEnabled(bool) = mac 0x273020, ios 0xc7d78;
    virtual auto setKeypadEnabled(bool) = mac 0x272f80, ios 0xc7cfc;
    virtual auto setMouseEnabled(bool) = mac 0x2730a0, ios 0xc7de8;
    virtual auto setTouchEnabled(bool) = mac 0x272cf0, ios 0xc7a5c;
    virtual auto setTouchMode(cocos2d::ccTouchesMode) = mac 0x272d60, ios 0xc7acc;
    virtual auto setTouchPriority(int) = mac 0x272db0, ios 0xc7b30;
    virtual auto unregisterScriptTouchHandler() = mac 0x272c30, ios 0xc7a28;
    virtual ~CCLayer() = mac 0x2727b0, ios 0xc7848, win 0xa1940;
}

[[link(win, android)]]
class cocos2d::CCLayerColor {
    CCLayerColor() = mac 0x274320, ios 0xc8aec, win 0xa1710;
    static cocos2d::CCLayerColor* create(cocos2d::_ccColor4B const&, float, float) = mac 0x2745e0;
    static cocos2d::CCLayerColor* create(cocos2d::_ccColor4B const&) = mac 0x2744c0;
    virtual auto draw() = mac 0x274b50, ios 0xc8fe0;
    virtual auto getBlendFunc() = mac 0x274480, ios 0xc8bcc;
    virtual auto init() = mac 0x274800, ios 0xc8de8;
    virtual auto initWithColor(cocos2d::_ccColor4B const&) = mac 0x2749a0, ios 0xc8f14;
    virtual auto initWithColor(cocos2d::_ccColor4B const&, float, float) = mac 0x274850, ios 0xc8e34;
    virtual auto setBlendFunc(cocos2d::_ccBlendFunc) = mac 0x2744a0, ios 0xc8bdc;
    virtual auto setColor(cocos2d::_ccColor3B const&) = mac 0x274c20, ios 0xc90ac;
    virtual auto setContentSize(cocos2d::CCSize const&) = mac 0x2749f0, ios 0xc8f64;
    virtual auto setOpacity(unsigned char) = mac 0x274db0, ios 0xc9108;
    virtual auto updateColor() = mac 0x274ae0, ios 0xc8f80;
    virtual ~CCLayerColor() = mac 0x2743d0, ios 0x2743e0, win 0xa1a20;
}

[[link(win, android)]]
class cocos2d::CCLayerRGBA {
    CCLayerRGBA() = mac 0x2738d0, ios 0xc85cc, win 0xa1890;
    virtual auto init() = mac 0x273b40, ios 0xc8de8;
    virtual auto getOpacity() = mac 0x273be0, ios 0xc8698;
    virtual auto getDisplayedOpacity() = mac 0x273c00, ios 0xc8724;
    virtual auto setOpacity(unsigned char) = mac 0x273c20, ios 0xc8734;
    virtual auto updateDisplayedOpacity(unsigned char) = mac 0x273f20, ios 0xc88d8;
    virtual auto isCascadeOpacityEnabled() = mac 0x2741f0, ios 0xc8aac;
    virtual auto setCascadeOpacityEnabled(bool) = mac 0x274210, ios 0xc8abc;
    virtual auto getColor() = mac 0x273d60, ios 0xc87d8;
    virtual auto getDisplayedColor() = mac 0x273d80, ios 0xc87e8;
    virtual auto setColor(cocos2d::_ccColor3B const&) = mac 0x273da0, ios 0xc87f8;
    virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = mac 0x2740b0, ios 0xc89a4;
    virtual auto isCascadeColorEnabled() = mac 0x274230, ios 0xc8acc;
    virtual auto setCascadeColorEnabled(bool) = mac 0x274250, ios 0xc8adc;
    // virtual auto setOpacityModifyRGB(bool) = mac 0x6180, ios 0x68e8;
    // virtual auto isOpacityModifyRGB() = mac 0x6190, ios 0x68ec;
    virtual ~CCLayerRGBA() = mac 0x273aa0, ios 0xc77b0, win 0xa1b20;
}

[[link(win, android)]]
class cocos2d::CCMenu {
	auto alignItemsHorizontallyWithPadding(float) = mac 0x4393e0, ios 0x132508;
	auto alignItemsVerticallyWithPadding(float) = mac 0x439190;
	static cocos2d::CCMenu* create() = mac 0x438720, ios 0x131ba4;
	static cocos2d::CCMenu* createWithArray(cocos2d::CCArray*) = mac 0x4387e0;
	static cocos2d::CCMenu* createWithItem(cocos2d::CCMenuItem*) = mac 0x438b80;

	virtual auto addChild(cocos2d::CCNode*) = mac 0x438ba0, ios 0x131ec8;
    virtual auto addChild(cocos2d::CCNode*, int) = mac 0x438bb0, ios 0x131ecc;
    virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x438bc0, ios 0x131ed0;

	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x438d20, ios 0x131fe8;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x4390b0, ios 0x1322ac;
    virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x438fd0, ios 0x1321bc;
    virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x439050, ios 0x132244;
    virtual auto init() = mac 0x438b90, ios 0x131ec0;
    virtual auto registerWithTouchDispatcher() = mac 0x438cd0, ios 0x131f8c;
    virtual auto onExit() = mac 0x438bd0, ios 0x131ed4;
    virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0x438c20, ios 0x15e630;
    bool initWithArray(cocos2d::CCArray*) = mac 0x4389f0, ios 0x131d04;
    cocos2d::CCMenuItem* itemForTouch(cocos2d::CCTouch*) = mac 0x438dd0;
}

[[link(win, android)]]
class cocos2d::CCMenuItem {
	bool initWithTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1fb7f0;
	virtual ~CCMenuItem() = mac 0x1fb8e0, ios 0x2cdf4, win 0xab9c0;
	virtual auto activate() = mac 0x1fba70, ios 0x2ceb0;
	virtual auto selected() = mac 0x1fb9e0, ios 0x2ce2e;
	virtual auto unselected() = mac 0x1fb9f0, ios 0x2ce38;
	virtual auto registerScriptTapHandler(int) = mac 0x1fba00, ios 0x2ce40;
	virtual auto unregisterScriptTapHandler() = mac 0x1fba30, ios 0x2ce70;
	virtual auto isEnabled() = mac 0x1fbaf0, ios 0x2cf34;
	virtual auto setEnabled(bool) = mac 0x1fbae0, ios 0x2cf2c;
	virtual auto isSelected() = mac 0x1fbb50, ios 0x2cf60;
    auto setTarget(cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1fbb60;
	auto rect() = mac 0x1fbb00, ios 0x2cf3c;
}

[[link(win, android)]]
class cocos2d::CCMenuItemImage {
	// virtual ~CCMenuItemImage() = mac 0x1febb0;
	virtual auto init() = mac 0x1fd750;
}

[[link(win, android)]]
class cocos2d::CCMenuItemLabel {
	virtual ~CCMenuItemLabel() = mac 0x1fc0d0;
	virtual auto activate() = mac 0x1fc240;
	virtual auto selected() = mac 0x1fc2e0;
	virtual auto unselected() = mac 0x1fc380;
	virtual auto setEnabled(bool) = mac 0x1fc3f0;
	virtual auto getDisabledColor() = mac 0x1fbb80;
	virtual auto setDisabledColor(cocos2d::_ccColor3B const&) = mac 0x1fbb90;
	virtual auto getLabel() = mac 0x1fbbb0;
	virtual auto setLabel(cocos2d::CCNode*) = mac 0x1fbbc0;
}

[[link(win, android)]]
class cocos2d::CCMenuItemSprite {
	// virtual ~CCMenuItemSprite() = mac 0x1feab0;
	virtual auto selected() = mac 0x1fd3f0, ios 0x2d2cc;
	virtual auto unselected() = mac 0x1fd470, ios 0x2d34c;
	virtual auto setEnabled(bool) = mac 0x1fd4e0, ios 0x2d3bc;
	virtual auto getNormalImage() = mac 0x1fcee0, ios 0x2cf68;
	virtual auto setNormalImage(cocos2d::CCNode*) = mac 0x1fcef0, ios 0x2cf70;
	virtual auto getSelectedImage() = mac 0x1fcfc0, ios 0x2d044;
	virtual auto setSelectedImage(cocos2d::CCNode*) = mac 0x1fcfd0, ios 0x2d04;
	virtual auto getDisabledImage() = mac 0x1fd070, ios 0x2d0fc;
	virtual auto setDisabledImage(cocos2d::CCNode*) = mac 0x1fd080, ios 0x2d104;
	virtual auto updateImagesVisibility() = mac 0x1fd510, ios 0x2d3dc;

	bool initWithNormalSprite(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1fd2f0;
	static auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*) = mac 0x1fd120;
	static auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1fd140;
	static auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1fd2d0;
}

[[link(win, android)]]
class cocos2d::CCMotionStreak {
	CCMotionStreak() = win 0xae310;
	virtual ~CCMotionStreak() = win 0xae450;
	auto reset() = mac 0x2ee190;
	auto resumeStroke() = mac 0x2edb30;
	auto stopStroke() = mac 0x2edb20;
	bool initWithFade(float fade, float minSeg, float stroke, cocos2d::ccColor3B const& color, cocos2d::CCTexture2D* texture) = mac 0x2ed6f0;
	virtual auto draw();
}

[[link(win, android)]]
class cocos2d::CCMouseDispatcher {
    bool dispatchScrollMSG(float x, float y) = mac 0x2e8f40;
    void removeDelegate(cocos2d::CCMouseDelegate* delegate);
}

[[link(win, android)]]
class cocos2d::CCMouseHandler {
    static cocos2d::CCMouseHandler* handlerWithDelegate(cocos2d::CCMouseDelegate*) = mac 0x12ef80;
    virtual auto initWithDelegate(cocos2d::CCMouseDelegate*) = mac 0x12ef40, ios 0x43798;
    ~CCMouseHandler() = mac 0x12ede0, ios 0x4375c, win 0xb1fd0;
}

[[link(win, android)]]
class cocos2d::CCMoveBy {
	static cocos2d::CCMoveBy* create(float, cocos2d::CCPoint const&) = mac 0x1f50e0;
}

[[link(win, android)]]
class cocos2d::CCMoveTo {
	static cocos2d::CCMoveTo* create(float, cocos2d::CCPoint const&) = mac 0x1f54d0;
}

[[link(win, android)]]
class cocos2d::CCNode {
    CCNode() = mac 0x122550, win 0x5e7d0;
    auto boundingBox() = mac 0x123030;
    virtual auto _setZOrder(int) = mac 0x122990, ios 0x15dd7c;
    virtual auto addChild(cocos2d::CCNode*) = mac 0x1233d0, ios 0x15e5d4;
    virtual auto addChild(cocos2d::CCNode*, int) = mac 0x1233b0, ios 0x15e5c4;
    virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x1232a0, ios 0x15e4e4;
    virtual auto addComponent(cocos2d::CCComponent*) = mac 0x124a40, ios 0x15f6a4;
    virtual auto cleanup() = mac 0x123100, ios 0x15e3a4;
    auto convertToNodeSpace(cocos2d::CCPoint const&) = mac 0x124750, ios 0x15f55c;
    auto convertToWorldSpace(cocos2d::CCPoint const&) = mac 0x124790;
    cocos2d::CCPoint convertToNodeSpaceAR(cocos2d::CCPoint const& worldPoint) {
        return convertToNodeSpace(worldPoint) - getAnchorPointInPoints();
    }
    cocos2d::CCPoint convertToWorldSpaceAR(cocos2d::CCPoint const& nodePoint) {
        return convertToWorldSpace(nodePoint + getAnchorPointInPoints());
    }
    static cocos2d::CCNode* create() = mac 0x1230a0;
    virtual auto draw() = mac 0x123840, ios 0x15e974;
    auto getActionByTag(int) = mac 0x123ee0;
    virtual auto getActionManager() = mac 0x123e50, ios 0x15ef54;
    virtual auto getAnchorPoint() = mac 0x122d80, ios 0x15e090;
    virtual auto getAnchorPointInPoints() = mac 0x122d70, ios 0x15e088;
    virtual auto getCamera() = mac 0x122cb0, ios 0x15dfe4;
    virtual auto getChildByTag(int) = mac 0x123220, ios 0x15e460;
    virtual auto getChildren() = mac 0x122c80, ios 0x15dfcc;
    virtual auto getChildrenCount() const = mac 0x122c90, ios 0x15dfd4;
    virtual auto getContentSize() const = mac 0x122e00, ios 0x15e118;
    virtual auto getGLServerState() = mac 0x122f90, ios 0x15e248;
    virtual auto getGrid() = mac 0x122d00, ios 0x15e034;
    virtual auto getOrderOfArrival() = mac 0x122f50, ios 0x15e228;
    virtual auto getParent() = mac 0x122ed0, ios 0x15e1e0;
    virtual auto getPosition() = mac 0x122b60, ios 0x15debc;
    virtual auto getPosition(float*, float*) = mac 0x122b90, ios 0x15dedc;
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
    virtual auto ignoreAnchorPointForPosition(bool) = mac 0x122f00, ios 0x15e1f8;
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
    auto pauseSchedulerAndActions() = mac 0x123d60;
    virtual auto registerScriptHandler(int) = mac 0x123d90, ios 0x15ee94;
    virtual auto removeAllChildren() = mac 0x123600, ios 0x15e788;
    virtual auto removeAllChildrenWithCleanup(bool) = mac 0x123620, ios 0x15e798;
    virtual auto removeAllComponents() = mac 0x124aa0, ios 0x15f6d4;
    virtual auto removeChild(cocos2d::CCNode*) = mac 0x123460, ios 0x15e630;
    virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0x123480, ios 0x15e640;
    virtual auto removeChildByTag(int) = mac 0x1235a0, ios 0x15e724;
    virtual auto removeChildByTag(int, bool) = mac 0x1235c0, ios 0x15e734;
    virtual auto removeComponent(char const*) = mac 0x124a60, ios 0x15f6b4;
    virtual auto removeComponent(cocos2d::CCComponent*) = mac 0x124a80, ios 0x15f6c4;
    virtual auto removeFromParent() = mac 0x1233f0, ios 0x15e5e8;
    virtual auto removeFromParentAndCleanup(bool) = mac 0x123410, ios 0x15e5f8;
    virtual auto removeMeAndCleanup() = mac 0x123440, ios 0x15e620;
    virtual auto reorderChild(cocos2d::CCNode*, int) = mac 0x123760, ios 0x15e87c;
    auto resumeSchedulerAndActions() = mac 0x123b60;
    auto runAction(cocos2d::CCAction*) = mac 0x123e60;
    auto schedule(cocos2d::SEL_SCHEDULE) = mac 0x1240b0;
    auto schedule(cocos2d::SEL_SCHEDULE, float) = mac 0x124120;
    auto scheduleUpdate() = mac 0x123f80;
    virtual auto setActionManager(cocos2d::CCActionManager*) = mac 0x123e00, ios 0x15ef04;
    virtual auto setAnchorPoint(cocos2d::CCPoint const&) = mac 0x122d90, ios 0x15e098;
    virtual auto setContentSize(cocos2d::CCSize const&) = mac 0x122e50, ios 0x15e158;
    virtual auto setGLServerState(cocos2d::ccGLServerState) = mac 0x122fa0, ios 0x15e250;
    virtual auto setGrid(cocos2d::CCGridBase*) = mac 0x122d10, ios 0x15e03c;
    virtual auto setOrderOfArrival(unsigned int) = mac 0x122f60, ios 0x15e230;
    virtual auto setParent(cocos2d::CCNode*) = mac 0x122ee0, ios 0x15e1e8;
    virtual auto setPosition(cocos2d::CCPoint const&) = mac 0x122b70, ios 0x15dec4;
    virtual auto setPosition(float, float) = mac 0x122ba0, ios 0x15def0;
    virtual auto setPositionX(float) = mac 0x122c00, ios 0x15df40;
    virtual auto setPositionY(float) = mac 0x122c40, ios 0x15df84;
    virtual auto setRotation(float) = mac 0x122a10, ios 0x15ddec;
    virtual auto setRotationX(float) = mac 0x122a60, ios 0x15de24;
    virtual auto setRotationY(float) = mac 0x122a90, ios 0x15de40;
    virtual auto setScale(float) = mac 0x122ac0, ios 0x15de5c;
    virtual auto setScale(float, float) = mac 0x122ae0, ios 0x15de70;
    virtual auto setScaleX(float) = mac 0x122b10, ios 0x15de8c;
    virtual auto setScaleY(float) = mac 0x122b40, ios 0x15dea8;
    virtual auto setScheduler(cocos2d::CCScheduler*) = mac 0x123f20, ios 0x15efb8;
    virtual auto setShaderProgram(cocos2d::CCGLProgram*) = mac 0x122ff0, ios 0x15e294;
    virtual auto setSkewX(float) = mac 0x122930, ios 0x15dd44;
    virtual auto setSkewY(float) = mac 0x122960, ios 0x15dd60;
    virtual auto setUserData(void*) = mac 0x122f40, ios 0x15e220;
    virtual auto setUserObject(cocos2d::CCObject*) = mac 0x122fb0, ios 0x15e258;
    virtual auto setVertexZ(float) = mac 0x1229f0, ios 0x15dddc;
    virtual auto setVisible(bool) = mac 0x122d60, ios 0x15e080;
    virtual auto setZOrder(int) = mac 0x1229a0, ios 0x15dd84;
    virtual auto sortAllChildren() = mac 0x1237b0, ios 0x15e8d4;
    auto stopActionByTag(int) = mac 0x123ec0;
    auto stopAllActions() = mac 0x123190;
    virtual auto unregisterScriptHandler() = mac 0x123dc0, ios 0x15eec4;
    auto unschedule(cocos2d::SEL_SCHEDULE) = mac 0x124180;
    auto unscheduleAllSelectors() = mac 0x1231b0;
    auto unscheduleUpdate() = mac 0x124060;
    virtual auto update(float) = mac 0x1241a0, ios 0x15f124;
    virtual auto updateTransform() = mac 0x1249d0, ios 0x15f648;
    virtual auto updateTweenAction(float, char const*) = mac 0x1249c0, ios 0x15f644;
    virtual auto visit() = mac 0x123850, ios 0x15e978;
    virtual auto worldToNodeTransform() = mac 0x124710, ios 0x15f51c;
    virtual ~CCNode() = mac 0x122750, ios 0x6c98, win 0x5ea40;
}

[[link(win, android)]]
class cocos2d::CCNodeRGBA {
	CCNodeRGBA() = mac 0x124b30, win 0x5e9d0;
	virtual ~CCNodeRGBA() = mac 0x124bb0, ios 0x15f748, win 0x5ebb0;
	virtual auto init() = mac 0x124bf0, ios 0x15f780;
	virtual auto getOpacity() = mac 0x124cd0, ios 0x15f7b8;
	virtual auto getDisplayedOpacity() = mac 0x124cf0, ios 0x15f7c8;
	virtual auto setOpacity(GLubyte) = mac 0x124d10, ios 0x15f7d8;
	virtual auto updateDisplayedOpacity(GLubyte) = mac 0x124e50, ios 0x15f87c;
	virtual auto isCascadeOpacityEnabled() = mac 0x124fe0, ios 0x15f948;
	virtual auto setCascadeOpacityEnabled(bool) = mac 0x125000, ios 0x15f958;
	virtual auto getColor() = mac 0x125020, ios 0x15f968;
	virtual auto getDisplayedColor() = mac 0x125040, ios 0x15f978;
	virtual auto setColor(cocos2d::_ccColor3B const&) = mac 0x125060, ios 0x15f9888;
	virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = mac 0x1251e0, ios 0x15fa68;
	virtual auto isCascadeColorEnabled() = mac 0x125320, ios 0x15fb70;
	virtual auto setCascadeColorEnabled(bool) = mac 0x125340, ios 0x15fb80;
}

[[link(win, android)]]
class cocos2d::CCObject {
    CCObject() = mac 0x250ca0, ios 0x43864, win 0x69230;
    auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x250f30, ios 0x439f0;
    auto autorelease() = mac 0x250ed0, ios 0x439b8;
    auto canEncode() = mac 0x250f90, ios 0x43a24;
    auto encodeWithCoder(DS_Dictionary*) = mac 0x250f70, ios 0x43a18;
    virtual auto getTag() const = mac 0x250f50, ios 0x43a08;
    auto isEqual(cocos2d::CCObject const*) = mac 0x250f20, ios 0x439e4;
    auto release() = mac 0x250ea0, ios 0x43984;
    auto retain() = mac 0x250ec0, ios 0x439a8;

    virtual auto setTag(int) = mac 0x250f60, ios 0x43a10;
    ~CCObject() = mac 0x250d20, ios 0x6ac0, win 0x69270;
}

[[link(win, android)]]
class cocos2d::CCParticleSystem {
	CCParticleSystem() = win 0xb6650;
	virtual ~CCParticleSystem() = win 0xb68e0;
	virtual auto update(float);
	auto initParticle(cocos2d::tCCParticle*);
	auto resetSystem() = mac 0x46bd50;
	auto resumeSystem() = mac 0x46bd40;
	auto stopSystem() = mac 0x46bd10;
}

[[link(win, android)]]
class cocos2d::CCParticleSystemQuad {
	CCParticleSystemQuad() = win 0xb9bd0;
	virtual ~CCParticleSystemQuad() = win 0xb9c10;
	static cocos2d::CCParticleSystemQuad* create(char const*) = mac 0x36b000;
	virtual auto draw();
	auto setupVBO();
}

[[link(win, android)]]
class cocos2d::CCPoolManager {
	auto pop() = mac 0x214620;
	static cocos2d::CCPoolManager* sharedPoolManager() = mac 0x2142c0;
}

[[link(win, android)]]
class cocos2d::CCRemoveSelf {
	static cocos2d::CCRemoveSelf* create(bool) = mac 0x454700;
}

[[link(win, android)]]
class cocos2d::CCRenderTexture {
	auto begin() = mac 0x35ce10;
	auto end() = mac 0x35d2c0;
	static cocos2d::CCRenderTexture* create(int, int, cocos2d::CCTexture2DPixelFormat) = mac 0x35c720;
	auto newCCImage(bool) = mac 0x35d7d0;
	auto saveToFile(char const*) = mac 0x35dab0;
	auto beginWithClear(float r, float g, float b, float a) = mac 0x35d010;
}

[[link(win, android)]]
class cocos2d::CCRepeat {
	static cocos2d::CCRepeat* create(cocos2d::CCFiniteTimeAction*, unsigned int) = mac 0x1f3230;
}

[[link(win, android)]]
class cocos2d::CCRepeatForever {
	static cocos2d::CCRepeatForever* create(cocos2d::CCActionInterval*) = mac 0x1f3920;
}

[[link(win, android)]]
class cocos2d::CCRotateBy {
	static cocos2d::CCRotateBy* create(float, float) = mac 0x1f4c50;
}

[[link(win, android)]]
class cocos2d::CCRotateTo {
	static cocos2d::CCRotateTo* create(float, float) = mac 0x1f47b0;
}

[[link(win, android)]]
class cocos2d::CCScaleTo {
	static cocos2d::CCScaleTo* create(float, float) = mac 0x1f6ff0;
	static cocos2d::CCScaleTo* create(float, float, float) = mac 0x1f70f0;
}

[[link(win, android)]]
class cocos2d::CCScaleBy {
	static cocos2d::CCScaleTo* create(float, float) = mac 0x1f73c0;
	static cocos2d::CCScaleTo* create(float, float, float) = mac 0x1f7480;
}

[[link(win, android)]]
class cocos2d::CCScene {
	static cocos2d::CCScene* create() = mac 0x13c140, ios 0x163070;
	auto getHighestChildZ() = mac 0x13c200, ios 0x1630e4;
}

[[link(win, android)]]
class cocos2d::CCScheduler {
	auto scheduleSelector(cocos2d::SEL_SCHEDULE, cocos2d::CCObject*, float, unsigned int, float, bool) = mac 0x242b20;
	void scheduleSelector(cocos2d::SEL_SCHEDULE selector, cocos2d::CCObject* target, float interval, bool paused) {
		this->scheduleSelector(selector, target, interval, kCCRepeatForever, 0, paused);
	}
    void unscheduleSelector(cocos2d::SEL_SCHEDULE pfnSelector, cocos2d::CCObject *pTarget) = mac 0x2425f0;
	auto scheduleUpdateForTarget(cocos2d::CCObject*, int, bool) = mac 0x2438d0;
	auto unscheduleAllForTarget(cocos2d::CCObject*) = mac 0x243e40;
	auto unscheduleUpdateForTarget(cocos2d::CCObject const*) = mac 0x243c60;
	auto resumeTargets(cocos2d::CCSet*) = mac 0x244680;
	auto pauseAllTargets() = mac 0x244550;
	virtual void update(float delta) = mac 0x2446d0;
}

[[link(win, android)]]
class cocos2d::CCSequence {
	static auto createWithVariableList(cocos2d::CCFiniteTimeAction*, va_list) = mac 0x1f2910;
}

[[link(win, android)]]
class cocos2d::CCSet {
	CCSet() = mac 0x45ad80, ios 0x10e870, win 0x699e0;
	static auto create() = mac 0x45b0b0;
	auto addObject(cocos2d::CCObject*) = mac 0x45b1f0;
	virtual ~CCSet() = mac 0x45b050, ios 0x10ebcc, win 0x69a80;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x45b090, ios 0x10ec04;
	auto anyObject() = mac 0x45b410;

	void removeObject(cocos2d::CCObject* obj) {
		m_pSet->erase(obj);
		CC_SAFE_RELEASE(obj);
	}
}

[[link(win, android)]]
class cocos2d::CCShaderCache {
	static auto sharedShaderCache() = mac 0xe6d10;
	auto programForKey(const char*) = mac 0xe7d40;
	void reloadDefaultShaders();
}

[[link(win, android)]]
class cocos2d::CCSprite {
	virtual ~CCSprite() = mac 0x133430, ios 0x15b92c, win 0xd2f90;
    virtual auto init() = mac 0x132ef0, ios 0x15b488;
    virtual auto setVertexZ(float) = mac 0x134a80, ios 0x15ccc8;
    virtual auto setScaleX(float) = mac 0x134900, ios 0x15cb3c;
    virtual auto setScaleY(float) = mac 0x134980, ios 0x15cbbc;
    virtual auto setScale(float) = mac 0x134a00, ios 0x15cc3c;
    virtual auto setPosition(cocos2d::CCPoint const&) = mac 0x134650, ios 0x15c878;
    virtual auto setSkewX(float) = mac 0x134820, ios 0x15ca5c;
    virtual auto setSkewY(float) = mac 0x134890, ios 0x15cacc;
    virtual auto setAnchorPoint(cocos2d::CCPoint const&) = mac 0x134af0, ios 0x15cd38;
    virtual auto setVisible(bool) = mac 0x134b70, ios 0x15cdac;
    virtual auto setRotation(float) = mac 0x1346d0, ios 0x15c90c;
    virtual auto setRotationX(float) = mac 0x134740, ios 0x15c97c;
    virtual auto setRotationY(float) = mac 0x1347b0, ios 0x15c9ec;
    virtual auto ignoreAnchorPointForPosition(bool) = mac 0x134b60, ios 0x15cda8;
    virtual auto addChild(cocos2d::CCNode*) = mac 0x134190, ios 0x15c34c;
    virtual auto addChild(cocos2d::CCNode*, int) = mac 0x1341a0, ios 0x15c350;
    virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x1341b0, ios 0x15c354;
    virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0x134300, ios 0x15c4cc;
    virtual auto removeAllChildrenWithCleanup(bool) = mac 0x134340, ios 0x15c514;
    virtual auto reorderChild(cocos2d::CCNode*, int) = mac 0x134270, ios 0x15c438;
    virtual auto sortAllChildren() = mac 0x1343f0, ios 0x15c5cc;
    virtual auto draw() = mac 0x134070, ios 0x15c240;
    virtual auto updateTransform() = mac 0x133b70, ios 0x15be0c;
    virtual auto setOpacity(unsigned char) = mac 0x134da0, ios 0x15cf48;
    virtual auto updateDisplayedOpacity(unsigned char) = mac 0x1354c0, ios 0x15d228;
    virtual auto setColor(cocos2d::_ccColor3B const&) = mac 0x134ff0, ios 0x15d088;
    virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = mac 0x135370, ios 0x15d1dc;
    virtual auto setOpacityModifyRGB(bool) = mac 0x135200, ios 0x15d198;
    virtual auto isOpacityModifyRGB() = mac 0x135350, ios 0x15d1cc;
    virtual auto initWithTexture(cocos2d::CCTexture2D*) = mac 0x133110, ios 0x15b608;
    virtual auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&) = mac 0x1330f0, ios 0x15b5f8;
    virtual auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool) = mac 0x132f10, ios 0x15b4a0;
    virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame*) = mac 0x133270, ios 0x15b78c;
    virtual auto initWithSpriteFrameName(char const*) = mac 0x1332c0, ios 0x15b7f0;
    virtual auto initWithFile(char const*) = mac 0x133180, ios 0x15b684;
    virtual auto initWithFile(char const*, cocos2d::CCRect const&) = mac 0x133210, ios 0x15b720;
    virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0x135a90, ios 0x15d610;
    virtual auto getTexture() = mac 0x135c00, ios 0x15d748;
    // virtual auto setBlendFunc(cocos2d::_ccBlendFunc) = mac 0x50590, ios 0x14c7c;
    // virtual cocos2d::_ccBlendFunc getBlendFunc() = mac 0x505a0, ios 0x14c88;
    virtual auto setChildColor(cocos2d::_ccColor3B const&) = mac 0x135160, ios 0x15d0f4;
    virtual auto setChildOpacity(unsigned char) = mac 0x134f50, ios 0x15cfe4;
    virtual auto getBatchNode() = mac 0x135910, ios 0x15d4e0;
    virtual auto setBatchNode(cocos2d::CCSpriteBatchNode*) = mac 0x135920, ios 0x15d4e8;
    virtual auto refreshTextureRect() = mac 0x133520, ios 0x15b964;
    virtual auto setTextureRect(cocos2d::CCRect const&) = mac 0x133560, ios 0x15b9a8;
    virtual auto setTextureRect(cocos2d::CCRect const&, bool, cocos2d::CCSize const&) = mac 0x133580, ios 0x15b9bc;
    virtual auto setVertexRect(cocos2d::CCRect const&) = mac 0x1338f0, ios 0x15bc5c;
    virtual auto setDisplayFrame(cocos2d::CCSpriteFrame*) = mac 0x135610, ios 0x15d274;
    virtual auto isFrameDisplayed(cocos2d::CCSpriteFrame*) = mac 0x1356d0, ios 0x15d344;
    virtual auto displayFrame() = mac 0x135760, ios 0x15d3d8;
    virtual auto setDisplayFrameWithAnimationName(char const*, int) = mac 0x135680, ios 0x15d2f0;
    // virtual auto isDirty() = mac 0x505b0, ios 0x14c94;
    // virtual auto setDirty(bool) = mac 0x505c0, ios 0x14c9c;
    virtual auto setTextureCoords(cocos2d::CCRect const&) = mac 0x133910, ios 0x15bc64;
    virtual auto updateBlendFunc() = mac 0x135a20, ios 0x15d5a0;
    virtual auto setReorderChildDirtyRecursively() = mac 0x134550, ios 0x15c764;
    virtual auto setDirtyRecursively(bool) = mac 0x1345b0, ios 0x15c7cc;

	CCSprite() = mac 0x133300, ios 0x15b82c, win 0xd2eb0;

	static cocos2d::CCSprite* create() = mac 0x132df0;
	static cocos2d::CCSprite* create(char const*) = mac 0x132a80;
	static cocos2d::CCSprite* createWithSpriteFrame(cocos2d::CCSpriteFrame*) = mac 0x132cb0;
	static cocos2d::CCSprite* createWithSpriteFrameName(char const*) = mac 0x132dc0, ios 0x15b3ec;
	static cocos2d::CCSprite* createWithTexture(cocos2d::CCTexture2D*) = mac 0x132790;
	static cocos2d::CCSprite* createWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&) = mac 0x132960;

	void setFlipX(bool) = mac 0x134be0;
	void setFlipY(bool) = mac 0x134c30;
}

[[link(win, android)]]
class cocos2d::CCSpriteBatchNode {
	static cocos2d::CCSpriteBatchNode* create(char const*, unsigned int) = mac 0xbb540;
	static auto createWithTexture(cocos2d::CCTexture2D*, unsigned int) = mac 0xbb310;
	auto getUsedAtlasCapacity() = mac 0xbc6b0;
	auto increaseAtlasCapacity(unsigned int) = mac 0xbc670;

	virtual ~CCSpriteBatchNode() = mac 0xbb790, ios 0x130d18, win 0xd5580;

	virtual auto init() = mac 0xbb680, ios 0x130c20;
	virtual auto addChild(cocos2d::CCNode*) = mac 0xbbb40, ios 0x131034;
	virtual auto addChild(cocos2d::CCNode*, int) = mac 0xbbb50, ios 0x131038;
	virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0xbb930, ios 0x130e0c;
	virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0xbbbb0, ios 0x1310a4;
	virtual auto removeAllChildrenWithCleanup(bool) = mac 0xbbe30, ios 0x1312d8;
	virtual auto reorderChild(cocos2d::CCNode*, int) = mac 0xbbb60, ios 0x13103c;
	virtual auto sortAllChildren() = mac 0xbbee0, ios 0x13137c;
	virtual auto draw() = mac 0xbc540, ios 0x13191c;
	virtual auto visit() = mac 0xbb880, ios 0x130d54;
	virtual auto getTexture() = mac 0xbcd70, ios 0x131a70;
	virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0xbcda0, ios 0x131a90;
	virtual auto setBlendFunc(cocos2d::ccBlendFunc) = mac 0xbcd30, ios 0x131a50;
	virtual auto getBlendFunc() = mac 0xbcd50, ios 0x131a60;
}

[[link(win, android)]]
class cocos2d::CCSpriteFrame {
	static auto createWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool, cocos2d::CCPoint const&, cocos2d::CCSize const&) = mac 0x1ac7f0;
	static auto createWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&) = mac 0x1ac5c0;
	auto getTexture() = mac 0x1ad250;
}

[[link(win, android)]]
class cocos2d::CCSpriteFrameCache {
	auto addSpriteFramesWithFile(char const*) = mac 0x199a10, ios 0x29e818;
	static cocos2d::CCSpriteFrameCache* sharedSpriteFrameCache() = mac 0x198970, ios 0x29dc4c;
	auto spriteFrameByName(char const*) = mac 0x19a7e0;
}

[[link(win, android)]]
class cocos2d::CCStandardTouchHandler {
    static cocos2d::CCStandardTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate*, int) = mac 0x247f30;
    virtual auto initWithDelegate(cocos2d::CCTouchDelegate*, int) = mac 0x247ed0, ios 0x69; // iOS stub
    ~CCStandardTouchHandler() = mac 0x2482a0, ios 0x6d28, win 0xf5a40;
}

[[link(win, android)]]
class cocos2d::CCString {
	// virtual ~CCString() = mac 0x44c590;
	virtual auto isEqual(cocos2d::CCObject const*) = mac 0x44c8f0, ios 0x1a1e6c;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x44ccb0, ios 0x1a2168;
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) = mac 0x44c870, ios 0x1a1e14;
	static cocos2d::CCString* createWithData(unsigned char const*, unsigned long) = mac 0x44c9d0;
	// cocos2d::CCString::create(gd::string const&) = mac 0x44c960;
	// CCString(gd::string const&) = mac 0x44c310;
	auto boolValue() const = mac 0x44c810, ios 0x1a1d88;
	auto doubleValue() const = mac 0x44c7f0;
	auto floatValue() const = mac 0x44c7d0;
	auto getCString() const = mac 0x44c470, ios 0x1a1b7c;
	auto intValue() const = mac 0x44c780, ios 0x1a1ca8;
}

[[link(win, android)]]
class cocos2d::CCTargetedTouchHandler {
    static cocos2d::CCTargetedTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate*, int, bool) = mac 0x248010;
    auto initWithDelegate(cocos2d::CCTouchDelegate*, int, bool) = mac 0x2480f0, ios 0x69; // iOS stub
    ~CCTargetedTouchHandler() = mac 0x248180, ios 0x6cec, win 0xf59a0;
}


[[link(win, android)]]
class cocos2d::CCTexture2D {
	CCTexture2D() = mac 0x246280, win 0xe9300;
	~CCTexture2D() = mac 0x246350, win 0xe93f0;
	auto getContentSize() = mac 0x246460;
	auto getMaxS() = mac 0x2464e0;
	auto getMaxT() = mac 0x246500;
	auto getName() = mac 0x246450;
	auto getPixelFormat() = mac 0x246420;
	auto getPixelsHigh() = mac 0x246440;
	auto getPixelsWide() = mac 0x246430;
	auto getShaderProgram() = mac 0x246520;
	auto initPremultipliedATextureWithImage(cocos2d::CCImage*, unsigned int, unsigned int) = mac 0x2469a0;
	auto initWithData(void const*, cocos2d::CCTexture2DPixelFormat, unsigned int, unsigned int, cocos2d::CCSize const&) = mac 0x2465d0;
	auto setAliasTexParameters() = mac 0x247a20;
	auto setAntiAliasTexParameters() = mac 0x247a80;
	static void setDefaultAlphaPixelFormat(cocos2d::CCTexture2DPixelFormat);
	static cocos2d::CCTexture2DPixelFormat defaultAlphaPixelFormat();
	auto setMaxS(float) = mac 0x2464f0;
	auto setMaxT(float) = mac 0x246510;
	auto setShaderProgram(cocos2d::CCGLProgram*) = mac 0x246530;
	auto initWithImage(cocos2d::CCImage*) = mac 0x246940;
	auto setTexParameters(cocos2d::_ccTexParams*) = mac 0x247980;
}

[[link(win, android)]]
class cocos2d::CCTextureAtlas {
	CCTextureAtlas() = win 0xea680;
	virtual ~CCTextureAtlas() = win 0xea6c0;
	auto drawNumberOfQuads(unsigned int, unsigned int);
	auto mapBuffers();
}

[[link(win, android)]]
class cocos2d::CCTextFieldTTF {
	static auto textFieldWithPlaceHolder(char const*, char const*, float) = mac 0x126220;
}

[[link(win, android)]]
class cocos2d::CCTextureCache {
	auto addImage(char const*, bool) = mac 0x358120, ios 0xa8388;
	auto textureForKey(char const*) = mac 0x359050;

	static cocos2d::CCTextureCache* sharedTextureCache() = mac 0x356e00, ios 0xa81ec;
}

[[link(win, android)]]
class cocos2d::CCTime {
	static auto gettimeofdayCocos2d(cocos2d::cc_timeval*, void*) = mac 0x19eac0;
}

[[link(win, android)]]
class cocos2d::CCTintTo {
	static cocos2d::CCTintTo* create(float, unsigned char, unsigned char, unsigned char) = mac 0x1f82a0;
}

[[link(win, android)]]
class cocos2d::CCTouch {
	auto getDelta() const = mac 0x38340;
	auto getLocationInView() const = mac 0x38250;
	auto getPreviousLocationInView() const = mac 0x38270;
	auto getLocation() const = mac 0x382b0, ios 0x21ce78;
	auto getPreviousLocation() const = mac 0x382e0;
	auto getStartLocation() const = mac 0x38310;
}

[[link(win, android)]]
class cocos2d::CCTouchDispatcher {
	auto addTargetedDelegate(cocos2d::CCTouchDelegate*, int, bool) = mac 0x281180;
	auto addStandardDelegate(cocos2d::CCTouchDelegate*, int) = mac 0x281060;
	auto removeDelegate(cocos2d::CCTouchDelegate* delegate) = mac 0x2813b0;
	auto decrementForcePrio(int) = mac 0x280f70, ios 0xcc280;
	auto incrementForcePrio(int) = mac 0x280f60, ios 0xcc268;
	void touches(cocos2d::CCSet*, cocos2d::CCEvent*, unsigned int) = mac 0x281a60;
}

[[link(win, android)]]
class cocos2d::CCTouchHandler {
    virtual auto initWithDelegate(cocos2d::CCTouchDelegate*, int) = mac 0x247d10, ios 0x69f8;
    auto getPriority() = mac 0x247c20;
    cocos2d::CCTouchDelegate* getDelegate() {
    	return m_pDelegate;
    }
    ~CCTouchHandler() = mac 0x247de0, ios 0x6ac0;
}

[[link(win, android)]]
class cocos2d::CCTransitionFade {
	static cocos2d::CCTransitionFade* create(float, cocos2d::CCScene*) = mac 0x8ea30, ios 0x12c244;
	virtual bool initWithDuration(float t, cocos2d::CCScene* scene, cocos2d::ccColor3B const& color) = mac 0x8e930;
}

[[link(win, android)]]
class cocos2d::ZipUtils {
	static auto compressString(gd::string, bool, int) = mac 0xe9a50;
	static auto decompressString(gd::string, bool, int) = mac 0xea380;
	static auto decompressString2(unsigned char* data, bool decrypt, int size, int decryptionKey);
	static int ccDeflateMemory(unsigned char*, unsigned int, unsigned char**) = mac 0xe9cf0;
}

[[link(win, android)]]
class cocos2d::extension::CCControl {
	CCControl() {}
    virtual bool init() = mac 0x1a71c0;
    virtual ~CCControl() = mac 0x1a7380;
    auto sendActionsForControlEvents(cocos2d::extension::CCControlEvent) = mac 0x1a7490;
    auto registerWithTouchDispatcher() = mac 0x1a7420;
    auto setOpacityModifyRGB(bool) = mac 0x1a7c10;
    auto onExit() = mac 0x1a7480;
    auto isTouchInside(cocos2d::CCTouch*) = mac 0x1a7de0;
    auto setEnabled(bool) = mac 0x1a7e60;
    auto onEnter() = mac 0x1a7470;
    auto isEnabled() = mac 0x1a7e90;
    auto setSelected(bool) = mac 0x1a7ea0;
    auto isOpacityModifyRGB() = mac 0x1a7d70;
    auto setHighlighted(bool) = mac 0x1a7ed0;
    auto needsLayout() = mac 0x1a7e50;
    auto getTouchLocation(cocos2d::CCTouch*) = mac 0x1a7d90;
    auto isHighlighted() = mac 0x1a7ef0;
    auto addTargetWithActionForControlEvents(cocos2d::CCObject*, cocos2d::extension::SEL_CCControlHandler, cocos2d::extension::CCControlEvent) = mac 0x1a7820;
    auto removeTargetWithActionForControlEvents(cocos2d::CCObject*, cocos2d::extension::SEL_CCControlHandler, cocos2d::extension::CCControlEvent) = mac 0x1a7950;
    auto isSelected() = mac 0x1a7ec0;
}


class cocos2d::extension::CCControlColourPicker {
    [[link(win)]]
    CCControlColourPicker() {}
    [[link(win)]]
    ~CCControlColourPicker() = mac 0x1aae30;
    [[link(win, android)]]
    auto setColorValue(cocos2d::_ccColor3B const&) = mac 0x1aac10;
    [[link(win)]]
    auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x1aae10;
    [[link(win)]]
    auto init() = mac 0x1aa400;
    [[link(win)]]
    static auto colourPicker() = mac 0x1aaa30;
    [[link(win)]]
    cocos2d::ccColor3B const& getColorValue() const {
        return m_rgb;
    }

}

[[link(win, android)]]
class cocos2d::extension::CCControlUtils {
	static cocos2d::extension::HSV HSVfromRGB(cocos2d::extension::RGBA) = mac 0x1e6750;
	static cocos2d::extension::RGBA RGBfromHSV(cocos2d::extension::HSV) = mac 0x1e6850;
}

[[link(win, android)]]
class cocos2d::extension::CCScale9Sprite {
    CCScale9Sprite() = mac 0x211330;
    static cocos2d::extension::CCScale9Sprite* create(char const*) = mac 0x2130d0;
    static cocos2d::extension::CCScale9Sprite* create(char const*, cocos2d::CCRect) = mac 0x212ef0;
    static cocos2d::extension::CCScale9Sprite* create(char const*, cocos2d::CCRect, cocos2d::CCRect) = mac 0x212dd0;
    static cocos2d::extension::CCScale9Sprite* createWithSpriteFrameName(char const*, cocos2d::CCRect) = mac 0x213380;
    static cocos2d::extension::CCScale9Sprite* createWithSpriteFrameName(char const*) = mac 0x213460;
    virtual ~CCScale9Sprite() = mac 0x211590;
    virtual auto init() = mac 0x2115d0;
    virtual auto setContentSize(cocos2d::CCSize const&) = mac 0x2127c0, ios 0x13e400;
    virtual auto visit() = mac 0x213bf0;
    virtual auto getOpacity() = mac 0x213f30;
    virtual auto setOpacity(unsigned char) = mac 0x213dd0;
    virtual auto updateDisplayedOpacity(unsigned char) = mac 0x2139b0;
    virtual auto getColor() = mac 0x213db0;
    virtual auto setColor(cocos2d::_ccColor3B const&) = mac 0x213c20;
    virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = mac 0x213a20;
    virtual auto setOpacityModifyRGB(bool) = mac 0x213830;
    virtual auto isOpacityModifyRGB() = mac 0x213990;
    virtual auto getPreferredSize() = mac 0x213620;
    virtual auto setPreferredSize(cocos2d::CCSize) = mac 0x2135f0;
    virtual auto getCapInsets() = mac 0x2136e0;
    virtual auto setCapInsets(cocos2d::CCRect) = mac 0x213640;
    virtual auto getInsetLeft() = mac 0x213b30;
    virtual auto setInsetLeft(float) = mac 0x213b70;
    virtual auto getInsetTop() = mac 0x213b40;
    virtual auto setInsetTop(float) = mac 0x213b90;
    virtual auto getInsetRight() = mac 0x213b50;
    virtual auto setInsetRight(float) = mac 0x213bb0;
    virtual auto getInsetBottom() = mac 0x213b60;
    virtual auto setInsetBottom(float) = mac 0x213bd0;
    virtual auto initWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, bool, cocos2d::CCRect) = mac 0x211690;
    virtual auto initWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, cocos2d::CCRect) = mac 0x211630;
    virtual auto initWithFile(char const*, cocos2d::CCRect, cocos2d::CCRect) = mac 0x212d50;
    virtual auto initWithFile(char const*, cocos2d::CCRect) = mac 0x212e80;
    virtual auto initWithFile(cocos2d::CCRect, char const*) = mac 0x212f80;
    virtual auto initWithFile(char const*) = mac 0x213080;
    virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame*, cocos2d::CCRect) = mac 0x213140;
    virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame*) = mac 0x213250;
    virtual auto initWithSpriteFrameName(char const*, cocos2d::CCRect) = mac 0x213310;
    virtual auto initWithSpriteFrameName(char const*) = mac 0x213410;
    virtual auto updateWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, bool, cocos2d::CCRect) = mac 0x211730;
    virtual auto setSpriteFrame(cocos2d::CCSpriteFrame*) = mac 0x213a90;
}

[[link(win, android)]]
class cocos2d::extension::CCScrollView {
	CCScrollView() = mac 0x214800;
	virtual ~CCScrollView() = mac 0x214c30;
	static auto create(cocos2d::CCSize, cocos2d::CCNode*) = mac 0x214cd0;
	virtual auto init() = mac 0x214fb0;
	virtual auto setContentSize(cocos2d::CCSize const&) = mac 0x215eb0;
	virtual auto getContentSize() const = mac 0x215e90;
	virtual auto addChild(cocos2d::CCNode*) = mac 0x216160;
	virtual auto addChild(cocos2d::CCNode*, int) = mac 0x216120;
	virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x216080;
	virtual auto visit() = mac 0x2164a0;
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x216620;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x216890;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x216c70;
	virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x216d30;
	virtual auto registerWithTouchDispatcher() = mac 0x214ff0;
	virtual auto setTouchEnabled(bool) = mac 0x215250;
}

[[link(win, android)]]
class cocos2d {
	static auto FNTConfigLoadFile(char const*) = mac 0x344f10;
	static auto ccGLUseProgram(GLuint) = mac 0x1ae540;
	static auto ccGLBlendFunc(GLenum, GLenum) = mac 0x1ae560;
	static auto ccDrawSolidRect(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::_ccColor4F) = mac 0xecf00;
	static auto ccGLEnableVertexAttribs(unsigned int) = mac 0x1ae740;
	static auto ccGLBindTexture2D(GLuint) = mac 0x1ae610;
	static auto ccGLBindTexture2DN(GLuint, GLuint) = mac 0x1ae650;
	static float ccpDistance(cocos2d::CCPoint const&, cocos2d::CCPoint const&) = mac 0x1aaf90;
	static auto ccDrawLine(cocos2d::CCPoint const&, cocos2d::CCPoint const&) = mac 0xeccc0;
	static void ccDrawPoly(cocos2d::CCPoint const*, unsigned int, bool) = mac 0xed0a0;
	static void ccDrawColor4B(GLubyte, GLubyte, GLubyte, GLubyte) = mac 0xeddd0;
	static void CCMessageBox(const char* msg, const char* title) = mac 0xbabc0;
}

[[link(win)]]
class DS_Dictionary {
	DS_Dictionary() = mac 0xbe9a0;
	~DS_Dictionary() = mac 0x393c30;
	bool saveRootSubDictToString() = mac 0xc09c0;
	bool loadRootSubDictFromString(gd::string) = mac 0xbfd80;
	bool stepIntoSubDictWithKey(char const*) = mac 0xc0cd0;
	int getIntegerForKey(char const*) = mac 0xc1610;
	void setIntegerForKey(char const*, int) = mac 0xc26b0;
	void setDictForKey(char const*, cocos2d::CCDictionary*) = mac 0xC4EA0;
	auto getObjectForKey(char const*) = mac 0xC4BB0;
}

[[link(win, android)]]
class pugi::xml_document {
	xml_document() = mac 0x393a80;
	~xml_document() = mac 0x393b50;
}

// clang-format on
