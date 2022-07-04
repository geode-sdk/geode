class cocos2d::CCActionTween {
	static cocos2d::CCActionTween* create(float, char const*, float, float) = mac 0x447590;
}

class cocos2d::CCAnimate {
	static cocos2d::CCAnimate* create(cocos2d::CCAnimation*) = mac 0x1f8fc0;
}

class cocos2d::CCAnimation {
	static auto createWithSpriteFrames(cocos2d::CCArray*, float) = mac 0x140df0;
}

class cocos2d::CCApplication {
	auto getCurrentLanguage() = mac 0x1a3f40;
	auto getTargetPlatform() = mac 0x1a3f20;
	auto openURL(char const*) = mac 0x1a4550;
	auto setAnimationInterval(double) = mac 0x1a3ee0;
}

class cocos2d::CCApplicationProtocol {
	auto gameDidSave() = mac 0x1a45f0;
}

class cocos2d::CCArray {
	auto addObject(cocos2d::CCObject*) = mac 0x419f90;
	auto addObjectNew(cocos2d::CCObject*) = mac 0x41a450;
	auto addObjectsFromArray(cocos2d::CCArray*) = mac 0x41a2d0;
	auto containsObject(cocos2d::CCObject*) const = mac 0x41a3e0;
	auto count() const = mac 0x41a2f0;
	static cocos2d::CCArray* create() = mac 0x419cb0;
	static auto createWithObject(cocos2d::CCObject*) = mac 0x419d50;
	auto fastRemoveObject(cocos2d::CCObject*) = mac 0x41a520;
	auto fastRemoveObjectAtIndex(unsigned int) = mac 0x41a500;
	auto fastRemoveObjectAtIndexNew(unsigned int) = mac 0x41a510;
	auto lastObject() = mac 0x41a360;
	auto objectAtIndex(unsigned int) = mac 0x41a340;
	auto removeAllObjects() = mac 0x41a4f0;
	auto removeLastObject(bool) = mac 0x41a470;
	auto removeObject(cocos2d::CCObject*, bool) = mac 0x41a490;
	auto removeObjectAtIndex(unsigned int, bool) = mac 0x41a4b0;
	auto stringAtIndex(unsigned int) = mac 0x41a320;
}

class cocos2d::CCBezierTo {
	static cocos2d::CCBezierTo* create(float, cocos2d::_ccBezierConfig const&) = mac 0x1f6c10;
}

class cocos2d::CCCallFunc {
	static cocos2d::CCCallFunc* create(int) = mac 0x454d90;
}

class cocos2d::CCClippingNode {
	static cocos2d::CCClippingNode* create() = mac 0x4192a0;
	static cocos2d::CCClippingNode* create(cocos2d::CCNode*) = mac 0x419330;
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

class cocos2d::CCDelayTime {
	static cocos2d::CCDelayTime* create(float) = mac 0x1f4380;
}

class cocos2d::CCDictionary {
	auto allKeys() = mac 0x190450;
	auto count() = mac 0x190430;
	static cocos2d::CCDictionary* create() = mac 0x192650;
	auto objectForKey(intptr_t) = mac 0x190bb0;
	auto objectForKey(gd::string const&) = mac 0x190870;
	auto removeAllObjects() = mac 0x190220;
	auto removeObjectForKey(intptr_t) = mac 0x1921d0;
	auto setObject(cocos2d::CCObject*, intptr_t) = mac 0x191790;
	auto setObject(cocos2d::CCObject*, gd::string const&) = mac 0x190dc0;
	auto valueForKey(intptr_t) = mac 0x190cf0;
	auto valueForKey(gd::string const&) = mac 0x1907a0;
}

class cocos2d::CCDirector {
	virtual ~CCDirector() = mac 0x2493a0;
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
	virtual auto getSceneDelegate() = mac 0x24b320;
	virtual auto setSceneDelegate(cocos2d::CCSceneDelegate*) = mac 0x24b330;


	auto getWinSize() = mac 0x24a0f0;
	auto pushScene(cocos2d::CCScene*) = mac 0x24a620;
	auto replaceScene(cocos2d::CCScene*) = mac 0x24a6d0;
	auto resetSmoothFixCounter() = mac 0x249bc0;
	static auto sharedDirector() = mac 0x248cb0;
	auto calculateDeltaTime() = mac 0x2497a0;
	auto calculateMPF() = mac 0x19eac0;
	auto convertToGL(cocos2d::CCPoint const&) = mac 0x24a210;
	auto drawScene() = mac 0x249690;
	auto setNextScene() = mac 0x2498d0;
	auto showStats() = mac 0x2499e0;
	auto updateContentScale(cocos2d::TextureQuality) = mac 0x249ff0;
	auto popSceneWithTransition(float, cocos2d::PopTransition) = mac 0x24a8b0;
}

class cocos2d::CCDrawNode {
	auto clear() = mac 0x379e80;
	static cocos2d::CCDrawNode* create() = mac 0x378d00;
	auto drawPolygon(cocos2d::CCPoint*, unsigned int, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&) = mac 0x3797f0;
	auto drawSegment(cocos2d::CCPoint const&, cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&) = mac 0x3792d0;
	auto getBlendFunc() const = mac 0x379ea0;
	auto init() = mac 0x378e00;
	auto setBlendFunc(cocos2d::_ccBlendFunc const&) = mac 0x379eb0;
	auto draw() = mac 0x379020;
	virtual ~CCDrawNode() = mac 0x378cc0;
}

class cocos2d::CCEaseElasticOut {
	static cocos2d::CCEaseElasticOut* create(cocos2d::CCActionInterval*, float) = mac 0x2a3080;
}

class cocos2d::CCEaseIn {
	static cocos2d::CCEaseIn* create(cocos2d::CCActionInterval*, float) = mac 0x2a1960;
}

class cocos2d::CCEaseInOut {
	static cocos2d::CCEaseInOut* create(cocos2d::CCActionInterval*, float) = mac 0x2a1d80;
}

class cocos2d::CCEaseOut {
	static cocos2d::CCEaseOut* create(cocos2d::CCActionInterval*, float) = mac 0x2a1b70;
}

class cocos2d::CCFadeOut {
	static cocos2d::CCFadeOut* create(float) = mac 0x1f7d80;
}

class cocos2d::CCFadeTo {
	static cocos2d::CCFadeTo* create(float, unsigned char) = mac 0x1f7ff0;
}

class cocos2d::CCHide {
	static cocos2d::CCHide* create() = mac 0x4543e0;
}

class cocos2d::CCIMEDelegate {
	virtual ~CCIMEDelegate() = mac 0x277670;
	virtual auto attachWithIME() = mac 0x2776a0;
	virtual auto detachWithIME() = mac 0x277880;
	// virtual auto canAttachWithIME() = mac 0x5ef30;
	// virtual auto didAttachWithIME() = mac 0x5ef40;
	// virtual auto canDetachWithIME() = mac 0x5ef50;
	// virtual auto didDetachWithIME() = mac 0x5ef60;
	// virtual auto insertText(char const*, int) = mac 0x5ef70;
	// virtual auto deleteBackward() = mac 0x5ef80;
	// virtual auto getContentText() = mac 0x5ef90;
	// virtual auto keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x126e80;
	// virtual auto keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5efa0;
	// virtual auto keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x126e90;
	// virtual auto keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5efb0;

	CCIMEDelegate() = mac 0x277310;
}

class cocos2d::CCImage {
	CCImage() = mac 0x24fa00;
	virtual ~CCImage() = mac 0x24fa80;
	auto initWithImageData(void*, int, cocos2d::CCImage::EImageFormat, int, int, int) = mac 0x24fcb0;
}

class cocos2d::CCKeyboardDelegate {
	void cocos2d::CCKeyboardDelegate::keyDown(cocos2d::enumKeyCodes) {}
	void cocos2d::CCKeyboardDelegate::keyUp(cocos2d::enumKeyCodes) {}

	cocos2d::CCKeyboardDelegate::CCKeyboardDelegate() {}
}

class cocos2d::CCKeyboardDispatcher {
	auto dispatchKeyboardMSG(cocos2d::enumKeyCodes, bool) = mac 0xe8190;
}

class cocos2d::CCKeyboardHandler {
    static cocos2d::CCKeyboardHandler* handlerWithDelegate(cocos2d::CCKeyboardDelegate*) = mac 0x242030;
    virtual auto initWithDelegate(cocos2d::CCKeyboardDelegate*) = mac 0x241ff0, , ios 0x13f8b8;
    ~CCKeyboardHandler() = mac 0x241e90, , ios 0x13f87c;
}

class cocos2d::CCKeypadHandler {
    static cocos2d::CCKeypadHandler* handlerWithDelegate(cocos2d::CCKeypadDelegate*) = mac 0x1ff2d0;
    virtual auto initWithDelegate(cocos2d::CCKeypadDelegate*) = mac 0x1ff290, , ios 0x69; // iOS stub
    virtual ~CCKeypadHandler() = mac 0x1ff130, , ios 0x2c530;
    cocos2d::CCKeypadDelegate* getDelegate() = mac 0x1ff0b0;
}


class cocos2d::CCLabelBMFont {
	static cocos2d::CCLabelBMFont* create(char const*, char const*) = mac 0x347660;
	auto limitLabelWidth(float, float, float) = mac 0x34a6e0;
	auto setScale(float) = mac 0x34a5d0;
	auto setString(char const*, bool) = mac 0x3489e0;
	auto setAnchorPoint(cocos2d::CCPoint const&) = mac 0x349440;
}

class cocos2d::CCLabelTTF {
	auto updateTexture() = mac 0x1fadc0;
}

class cocos2d::CCLayer {
    CCLayer() = mac 0x2725b0;
    virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2734d0, , ios 0xc810c;
    virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x273650, , ios 0xc82ac;
    virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2735d0, , ios 0xc820c;
    virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x273550, , ios 0xc816c;
    virtual auto ccTouchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x2736d0, , ios 0xc834c;
    virtual auto ccTouchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x273850, , ios 0xc852c;
    virtual auto ccTouchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x2737d0, , ios 0xc848c;
    virtual auto ccTouchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*) = mac 0x273750, , ios 0xc83ec;
    static cocos2d::CCLayer* create() = mac 0x272a00;
    virtual auto didAccelerate(cocos2d::CCAcceleration*) = mac 0x272ea0, , ios 0xc7c5c;
    virtual auto getTouchMode() = mac 0x272e10, , ios 0xc7b9c;
    virtual auto getTouchPriority() = mac 0x272e00, , ios 0xc7b94;
    virtual auto init() = mac 0x2729a0, , ios 0xc7894;
    virtual auto isAccelerometerEnabled() = mac 0x272e20, , ios 0xc7ba4;
    virtual auto isKeyboardEnabled() = mac 0x273010, , ios 0xc7d70;
    virtual auto isKeypadEnabled() = mac 0x272f70, , ios 0xc7cf4;
    virtual auto isMouseEnabled() = mac 0x273090, , ios 0xc7de0;
    virtual auto isTouchEnabled() = mac 0x272ce0, , ios 0xc7a54;
    virtual auto keyBackClicked() = mac 0x273160, , ios 0xc7e50;
    virtual auto keyDown(cocos2d::enumKeyCodes) = mac 0x273280, , ios 0xc7efc;
    virtual auto keyMenuClicked() = mac 0x273200, , ios 0xc7eac;
    virtual auto onEnter() = mac 0x273300, , ios 0xc7f38;
    virtual auto onEnterTransitionDidFinish() = mac 0x273490, , ios 0xc80cc;
    virtual auto onExit() = mac 0x2733c0, , ios 0xc8004;
    virtual auto registerScriptTouchHandler(int, bool, int, bool) = mac 0x272bd0, , ios 0xc79c8;
    virtual auto registerWithTouchDispatcher() = mac 0x272b40, , ios 0xc7958;
    virtual auto setAccelerometerEnabled(bool) = mac 0x272e30, , ios 0xc7bac;
    virtual auto setAccelerometerInterval(double) = mac 0x272e70, , ios 0xc7c10;
    virtual auto setKeyboardEnabled(bool) = mac 0x273020, , ios 0xc7d78;
    virtual auto setKeypadEnabled(bool) = mac 0x272f80, , ios 0xc7cfc;
    virtual auto setMouseEnabled(bool) = mac 0x2730a0, , ios 0xc7de8;
    virtual auto setTouchEnabled(bool) = mac 0x272cf0, , ios 0xc7a5c;
    virtual auto setTouchMode(cocos2d::ccTouchesMode) = mac 0x272d60, , ios 0xc7acc;
    virtual auto setTouchPriority(int) = mac 0x272db0, , ios 0xc7b30;
    virtual auto unregisterScriptTouchHandler() = mac 0x272c30, , ios 0xc7a28;
    ~CCLayer() = mac 0x2727b0, , ios 0xc7848;
}

class cocos2d::CCLayerColor {
    CCLayerColor() = mac 0x274320;
    static cocos2d::CCLayerColor* create(cocos2d::_ccColor4B const&, float, float) = mac 0x2745e0;
    auto draw() = mac 0x274b50, , ios 0xc8fe0;
    auto getBlendFunc() = mac 0x274480, , ios 0xc8bcc;
    auto init() = mac 0x274800, , ios 0xc8de8;
    auto initWithColor(cocos2d::_ccColor4B const&) = mac 0x2749a0, , ios 0xc8f14;
    auto initWithColor(cocos2d::_ccColor4B const&, float, float) = mac 0x274850, , ios 0xc8e34;
    auto setBlendFunc(cocos2d::_ccBlendFunc) = mac 0x2744a0, , ios 0xc8bdc;
    auto setColor(cocos2d::_ccColor3B const&) = mac 0x274c20, , ios 0xc90ac;
    auto setContentSize(cocos2d::CCSize const&) = mac 0x2749f0, , ios 0xc8f64;
    auto setOpacity(unsigned char) = mac 0x274db0, , ios 0xc9108;
    auto updateColor() = mac 0x274ae0, , ios 0xc8f80;
    ~CCLayerColor() = mac 0x2743d0, , ios 0x2743e0;
}

class cocos2d::CCLayerRGBA {
    CCLayerRGBA() = mac 0x2738d0;
    virtual auto init() = mac 0x273b40, , ios 0xc8de8; // ios version is not real. any errors can be traced back to this
    virtual auto getOpacity() = mac 0x273be0, , ios 0xc8698;
    virtual auto getDisplayedOpacity() = mac 0x273c00, , ios 0xc8724;
    virtual auto setOpacity(unsigned char) = mac 0x273c20, , ios 0xc8734;
    virtual auto updateDisplayedOpacity(unsigned char) = mac 0x273f20, , ios 0xc88d8;
    virtual auto isCascadeOpacityEnabled() = mac 0x2741f0, , ios 0xc8aac;
    virtual auto setCascadeOpacityEnabled(bool) = mac 0x274210, , ios 0xc8abc;
    virtual auto getColor() = mac 0x273d60, , ios 0xc87d8;
    virtual auto getDisplayedColor() = mac 0x273d80, , ios 0xc87e8;
    virtual auto setColor(cocos2d::_ccColor3B const&) = mac 0x273da0, , ios 0xc87f8;
    virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = mac 0x2740b0, , ios 0xc89a4;
    virtual auto isCascadeColorEnabled() = mac 0x274230, , ios 0xc8acc;
    virtual auto setCascadeColorEnabled(bool) = mac 0x274250, , ios 0xc8adc;
    // virtual auto setOpacityModifyRGB(bool) = mac 0x6180, , ios 0x68e8;
    // virtual auto isOpacityModifyRGB() = mac 0x6190, , ios 0x68ec;
    virtual ~CCLayerRGBA() = mac 0x273aa0, , ios 0xc77b0;
}

class cocos2d::CCMenu {
	auto alignItemsHorizontallyWithPadding(float) = mac 0x4393e0;
	auto alignItemsVerticallyWithPadding(float) = mac 0x439190;
	static cocos2d::CCMenu* create() = mac 0x438720;
	static cocos2d::CCMenu* createWithArray(cocos2d::CCArray*) = mac 0x4387e0;
	static cocos2d::CCMenu* createWithItem(cocos2d::CCMenuItem*) = mac 0x438b80;
}

class cocos2d::CCMenuItem {
	virtual ~CCMenuItem() = mac 0x1fb8e0;
	virtual auto activate() = mac 0x1fba70;
	virtual auto selected() = mac 0x1fb9e0;
	virtual auto unselected() = mac 0x1fb9f0;
	virtual auto registerScriptTapHandler(int) = mac 0x1fba00;
	virtual auto unregisterScriptTapHandler() = mac 0x1fba30;
	virtual auto isEnabled() = mac 0x1fbaf0;
	virtual auto setEnabled(bool) = mac 0x1fbae0;
	virtual auto isSelected() = mac 0x1fbb50;
}

class cocos2d::CCMenuItemImage {
	// virtual ~CCMenuItemImage() = mac 0x1febb0;
	virtual auto init() = mac 0x1fd750;
}

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

class cocos2d::CCMenuItemSprite {
	// virtual ~CCMenuItemSprite() = mac 0x1feab0;
	virtual auto selected() = mac 0x1fd3f0;
	virtual auto unselected() = mac 0x1fd470;
	virtual auto setEnabled(bool) = mac 0x1fd4e0;
	virtual auto getNormalImage() = mac 0x1fcee0;
	virtual auto setNormalImage(cocos2d::CCNode*) = mac 0x1fcef0;
	virtual auto getSelectedImage() = mac 0x1fcfc0;
	virtual auto setSelectedImage(cocos2d::CCNode*) = mac 0x1fcfd0;
	virtual auto getDisabledImage() = mac 0x1fd070;
	virtual auto setDisabledImage(cocos2d::CCNode*) = mac 0x1fd080;
	virtual auto updateImagesVisibility() = mac 0x1fd510;

	static auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*) = mac 0x1fd120;
	static auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1fd140;
	static auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1fd2d0;
}

class cocos2d::CCMotionStreak {
	auto reset() = mac 0x2ee190;
	auto resumeStroke() = mac 0x2edb30;
	auto stopStroke() = mac 0x2edb20;
}

class cocos2d::CCMouseDelegate {
	cocos2d::CCMouseDelegate::CCMouseDelegate() {}
	void cocos2d::CCMouseDelegate::rightKeyDown() {}
	void cocos2d::CCMouseDelegate::rightKeyUp() {}
	void cocos2d::CCMouseDelegate::scrollWheel(float, float) {}
}

class cocos2d::CCMouseHandler {
    static cocos2d::CCMouseHandler* handlerWithDelegate(cocos2d::CCMouseDelegate*) = mac 0x12ef80;
    virtual auto initWithDelegate(cocos2d::CCMouseDelegate*) = mac 0x12ef40, , ios 0x43798;
    ~CCMouseHandler() = mac 0x12ede0, , ios 0x4375c;
}

class cocos2d::CCMoveBy {
	static cocos2d::CCMoveBy* create(float, cocos2d::CCPoint const&) = mac 0x1f50e0;
}

class cocos2d::CCMoveTo {
	static cocos2d::CCMoveTo* create(float, cocos2d::CCPoint const&) = mac 0x1f54d0;
}

class cocos2d::CCNode {
    CCNode() = mac 0x122550;
    auto _setZOrder(int) = mac 0x122990, , ios 0x15dd7c;
    auto addChild(cocos2d::CCNode*) = mac 0x1233d0, , ios 0x15e5d4;
    auto addChild(cocos2d::CCNode*, int) = mac 0x1233b0, , ios 0x15e5c4;
    auto addChild(cocos2d::CCNode*, int, int) = mac 0x1232a0, , ios 0x15e4e4;
    auto addComponent(cocos2d::CCComponent*) = mac 0x124a40, , ios 0x15f6a4;
    auto cleanup() = mac 0x123100, , ios 0x15e3a4;
    auto convertToNodeSpace(cocos2d::CCPoint const&) = mac 0x124750;
    auto convertToWorldSpace(cocos2d::CCPoint const&) = mac 0x124790;
    static cocos2d::CCNode* create() = mac 0x1230a0;
    auto draw() = mac 0x123840, , ios 0x15e974;
    auto getActionByTag(int) = mac 0x123ee0;
    auto getActionManager() = mac 0x123e50, , ios 0x15ef54;
    auto getAnchorPoint() = mac 0x122d80, , ios 0x15e090;
    auto getAnchorPointInPoints() = mac 0x122d70, , ios 0x15e088;
    auto getCamera() = mac 0x122cb0, , ios 0x15dfe4;
    auto getChildByTag(int) = mac 0x123220, , ios 0x15e460;
    auto getChildren() = mac 0x122c80, , ios 0x15dfcc;
    auto getChildrenCount() const = mac 0x122c90, , ios 0x15dfd4;
    auto getContentSize() const = mac 0x122e00, , ios 0x15e118;
    auto getGLServerState() = mac 0x122f90, , ios 0x15e248;
    auto getGrid() = mac 0x122d00, , ios 0x15e034;
    auto getOrderOfArrival() = mac 0x122f50, , ios 0x15e228;
    auto getParent() = mac 0x122ed0, , ios 0x15e1e0;
    auto getPosition() = mac 0x122b60, , ios 0x15debc;
    auto getPosition(float*, float*) = mac 0x122b90, , ios 0x15dedc;
    auto getPositionX() = mac 0x122be0, , ios 0x15df30;
    auto getPositionY() = mac 0x122bf0, , ios 0x15df38;
    auto getRotation() = mac 0x122a00, , ios 0x15dde4;
    auto getRotationX() = mac 0x122a50, , ios 0x15de1c;
    auto getRotationY() = mac 0x122a80, , ios 0x15de38;
    auto getScale() = mac 0x122ab0, , ios 0x15de54;
    auto getScaleX() = mac 0x122b00, , ios 0x15de84;
    auto getScaleY() = mac 0x122b30, , ios 0x15dea0;
    auto getScaledContentSize() = mac 0x122e10, , ios 0x15e120;
    auto getScheduler() = mac 0x123f70, , ios 0x15f008;
    auto getShaderProgram() = mac 0x122f70, , ios 0x15e238;
    auto getSkewX() = mac 0x122920, , ios 0x15dd3c;
    auto getSkewY() = mac 0x122950, , ios 0x15dd58;
    auto getUserData() = mac 0x122f30, , ios 0x15e218;
    auto getUserObject() = mac 0x122f80, , ios 0x15e240;
    auto getVertexZ() = mac 0x1229e0, , ios 0x15ddd4;
    auto getZOrder() = mac 0x122980, , ios 0x15dd74;
    auto ignoreAnchorPointForPosition(bool) = mac 0x122f00, , ios 0x15e1f8;
    auto init() = mac 0x122910, , ios 0x15dd34;
    auto isIgnoreAnchorPointForPosition() = mac 0x122ef0, , ios 0x15e1f0;
    auto isRunning() = mac 0x122ec0, , ios 0x15e1d8;
    auto isVisible() = mac 0x122d50, , ios 0x15e078;
    auto nodeToParentTransform() = mac 0x124210, , ios 0x15f1a4;
    auto nodeToWorldTransform() = mac 0x124670, , ios 0x15f490;
    auto onEnter() = mac 0x123a90, , ios 0x15eb94;
    auto onEnterTransitionDidFinish() = mac 0x123b90, , ios 0x15ec94;
    auto onExit() = mac 0x123ca0, , ios 0x15edb0;
    auto onExitTransitionDidStart() = mac 0x123c00, , ios 0x15ed18;
    auto parentToNodeTransform() = mac 0x1245d0, , ios 0x15f410;
    auto pauseSchedulerAndActions() = mac 0x123d60;
    auto registerScriptHandler(int) = mac 0x123d90, , ios 0x15ee94;
    auto removeAllChildren() = mac 0x123600, , ios 0x15e788;
    auto removeAllChildrenWithCleanup(bool) = mac 0x123620, , ios 0x15e798;
    auto removeAllComponents() = mac 0x124aa0, , ios 0x15f6d4;
    auto removeChild(cocos2d::CCNode*) = mac 0x123460, , ios 0x15e630;
    auto removeChild(cocos2d::CCNode*, bool) = mac 0x123480, , ios 0x15e640;
    auto removeChildByTag(int) = mac 0x1235a0, , ios 0x15e724;
    auto removeChildByTag(int, bool) = mac 0x1235c0, , ios 0x15e734;
    auto removeComponent(char const*) = mac 0x124a60, , ios 0x15f6b4;
    auto removeComponent(cocos2d::CCComponent*) = mac 0x124a80, , ios 0x15f6c4;
    auto removeFromParent() = mac 0x1233f0, , ios 0x15e5e8;
    auto removeFromParentAndCleanup(bool) = mac 0x123410, , ios 0x15e5f8;
    auto removeMeAndCleanup() = mac 0x123440, , ios 0x15e620;
    auto reorderChild(cocos2d::CCNode*, int) = mac 0x123760, , ios 0x15e87c;
    auto resumeSchedulerAndActions() = mac 0x123b60;
    auto runAction(cocos2d::CCAction*) = mac 0x123e60;
    auto schedule(cocos2d::SEL_SCHEDULE) = mac 0x1240b0;
    auto schedule(cocos2d::SEL_SCHEDULE, float) = mac 0x124120;
    auto scheduleUpdate() = mac 0x123f80;
    auto setActionManager(cocos2d::CCActionManager*) = mac 0x123e00, , ios 0x15ef04;
    auto setAnchorPoint(cocos2d::CCPoint const&) = mac 0x122d90, , ios 0x15e098;
    auto setContentSize(cocos2d::CCSize const&) = mac 0x122e50, , ios 0x15e158;
    auto setGLServerState(cocos2d::ccGLServerState) = mac 0x122fa0, , ios 0x15e250;
    auto setGrid(cocos2d::CCGridBase*) = mac 0x122d10, , ios 0x15e03c;
    auto setOrderOfArrival(unsigned int) = mac 0x122f60, , ios 0x15e230;
    auto setParent(cocos2d::CCNode*) = mac 0x122ee0, , ios 0x15e1e8;
    auto setPosition(cocos2d::CCPoint const&) = mac 0x122b70, , ios 0x15dec4;
    auto setPosition(float, float) = mac 0x122ba0, , ios 0x15def0;
    auto setPositionX(float) = mac 0x122c00, , ios 0x15df40;
    auto setPositionY(float) = mac 0x122c40, , ios 0x15df84;
    auto setRotation(float) = mac 0x122a10, , ios 0x15ddec;
    auto setRotationX(float) = mac 0x122a60, , ios 0x15de24;
    auto setRotationY(float) = mac 0x122a90, , ios 0x15de40;
    auto setScale(float) = mac 0x122ac0, , ios 0x15de5c;
    auto setScale(float, float) = mac 0x122ae0, , ios 0x15de70;
    auto setScaleX(float) = mac 0x122b10, , ios 0x15de8c;
    auto setScaleY(float) = mac 0x122b40, , ios 0x15dea8;
    auto setScheduler(cocos2d::CCScheduler*) = mac 0x123f20, , ios 0x15efb8;
    auto setShaderProgram(cocos2d::CCGLProgram*) = mac 0x122ff0, , ios 0x15e294;
    auto setSkewX(float) = mac 0x122930, , ios 0x15dd44;
    auto setSkewY(float) = mac 0x122960, , ios 0x15dd60;
    auto setUserData(void*) = mac 0x122f40, , ios 0x15e220;
    auto setUserObject(cocos2d::CCObject*) = mac 0x122fb0, , ios 0x15e258;
    auto setVertexZ(float) = mac 0x1229f0, , ios 0x15dddc;
    auto setVisible(bool) = mac 0x122d60, , ios 0x15e080;
    auto setZOrder(int) = mac 0x1229a0, , ios 0x15dd84;
    auto sortAllChildren() = mac 0x1237b0, , ios 0x15e8d4;
    auto stopActionByTag(int) = mac 0x123ec0;
    auto stopAllActions() = mac 0x123190;
    auto unregisterScriptHandler() = mac 0x123dc0, , ios 0x15eec4;
    auto unschedule(cocos2d::SEL_SCHEDULE) = mac 0x124180;
    auto unscheduleAllSelectors() = mac 0x1231b0;
    auto unscheduleUpdate() = mac 0x124060;
    auto update(float) = mac 0x1241a0, , ios 0x15f124;
    auto updateTransform() = mac 0x1249d0, , ios 0x15f648;
    auto updateTweenAction(float, char const*) = mac 0x1249c0, , ios 0x15f644;
    auto visit() = mac 0x123850, , ios 0x15e978;
    auto worldToNodeTransform() = mac 0x124710, , ios 0x15f51c;
    ~CCNode() = mac 0x122750, , ios 0x6c98;
}

class cocos2d::CCNodeRGBA {
	CCNodeRGBA() = mac 0x124b30;
	virtual ~CCNodeRGBA() = mac 0x124bb0;
	virtual auto init() = mac 0x124bf0;
	virtual auto getOpacity() = mac 0x124cd0;
	virtual auto getDisplayedOpacity() = mac 0x124cf0;
	virtual auto setOpacity(GLubyte) = mac 0x124d10;
	virtual auto updateDisplayedOpacity(GLubyte) = mac 0x124e50;
	virtual auto isCascadeOpacityEnabled() = mac 0x124fe0;
	virtual auto setCascadeOpacityEnabled(bool) = mac 0x125000;
	virtual auto getColor() = mac 0x125020;
	virtual auto getDisplayedColor() = mac 0x125040;
	virtual auto setColor(cocos2d::_ccColor3B const&) = mac 0x125060;
	virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = mac 0x1251e0;
	virtual auto isCascadeColorEnabled() = mac 0x125320;
	virtual auto setCascadeColorEnabled(bool) = mac 0x125340;
}

class cocos2d::CCObject {
    CCObject() = mac 0x250ca0;
    auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x250f30, , ios 0x439f0;
    auto autorelease() = mac 0x250ed0;
    auto canEncode() = mac 0x250f90, , ios 0x43a24;
    auto encodeWithCoder(DS_Dictionary*) = mac 0x250f70, , ios 0x43a18;
    virtual auto getTag() const = mac 0x250f50, , ios 0x43a08;
    auto isEqual(cocos2d::CCObject const*) = mac 0x250f20, , ios 0x439e4;
    auto release() = mac 0x250ea0;
    auto retain() = mac 0x250ec0;
    virtual auto setTag(int) = mac 0x250f60, , ios 0x43a10;
    ~CCObject() = mac 0x250d20, , ios 0x6ac0;
}

class cocos2d::CCParticleSystem {
	auto resetSystem() = mac 0x46bd50;
	auto resumeSystem() = mac 0x46bd40;
	auto stopSystem() = mac 0x46bd10;
}

class cocos2d::CCParticleSystemQuad {
	static cocos2d::CCParticleSystemQuad* create(char const*) = mac 0x36b000;
}

class cocos2d::CCPoolManager {
	auto pop() = mac 0x214620;
	static cocos2d::CCPoolManager* sharedPoolManager() = mac 0x2142c0;
}

class cocos2d::CCRenderTexture {
	auto begin() = mac 0x35ce10;
	auto end() = mac 0x35d2c0;
	static cocos2d::CCRenderTexture* create(int, int, cocos2d::CCTexture2DPixelFormat) = mac 0x35c720;
	auto newCCImage(bool) = mac 0x35d7d0;
}

class cocos2d::CCRepeatForever {
	static cocos2d::CCRepeatForever* create(cocos2d::CCActionInterval*) = mac 0x1f3920;
}

class cocos2d::CCRotateBy {
	static cocos2d::CCRotateBy* create(float, float) = mac 0x1f4c50;
}

class cocos2d::CCScaleTo {
	static cocos2d::CCScaleTo* create(float, float) = mac 0x1f6ff0;
	static cocos2d::CCScaleTo* create(float, float, float) = mac 0x1f70f0;
}

class cocos2d::CCScene {
	static cocos2d::CCScene* create() = mac 0x13c140;
	auto getHighestChildZ() = mac 0x13c200;
}

class cocos2d::CCScheduler {
	auto scheduleSelector(cocos2d::SEL_SCHEDULE, cocos2d::CCObject*, float, unsigned int, float, bool) = mac 0x242b20;
	auto scheduleUpdateForTarget(cocos2d::CCObject*, int, bool) = mac 0x2438d0;
	auto unscheduleAllForTarget(cocos2d::CCObject*) = mac 0x243e40;
}

class cocos2d::CCSequence {
}

class cocos2d::CCSet {
	CCSet() = mac 0x45ad80;
	virtual ~CCSet() = mac 0x45b050;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x45b090;
}

class cocos2d::CCSprite {
	virtual ~CCSprite() = mac 0x133430;
	virtual auto init() = mac 0x132ef0;
	virtual auto setVertexZ(float) = mac 0x134a80;
	virtual auto setScaleX(float) = mac 0x134900;
	virtual auto setScaleY(float) = mac 0x134980;
	virtual auto setScale(float) = mac 0x134a00;
	virtual auto setPosition(cocos2d::CCPoint const&) = mac 0x134650;
	virtual auto setSkewX(float) = mac 0x134820;
	virtual auto setSkewY(float) = mac 0x134890;
	virtual auto setAnchorPoint(cocos2d::CCPoint const&) = mac 0x134af0;
	virtual auto setVisible(bool) = mac 0x134b70;
	virtual auto setRotation(float) = mac 0x1346d0;
	virtual auto setRotationX(float) = mac 0x134740;
	virtual auto setRotationY(float) = mac 0x1347b0;
	virtual auto ignoreAnchorPointForPosition(bool) = mac 0x134b60;
	virtual auto addChild(cocos2d::CCNode*) = mac 0x134190;
	virtual auto addChild(cocos2d::CCNode*, int) = mac 0x1341a0;
	virtual auto addChild(cocos2d::CCNode*, int, int) = mac 0x1341b0;
	virtual auto removeChild(cocos2d::CCNode*, bool) = mac 0x134300;
	virtual auto removeAllChildrenWithCleanup(bool) = mac 0x134340;
	virtual auto reorderChild(cocos2d::CCNode*, int) = mac 0x134270;
	virtual auto sortAllChildren() = mac 0x1343f0;
	virtual auto draw() = mac 0x134070;
	virtual auto updateTransform() = mac 0x133b70;
	virtual auto setOpacity(unsigned char) = mac 0x134da0;
	virtual auto updateDisplayedOpacity(unsigned char) = mac 0x1354c0;
	virtual auto setColor(cocos2d::_ccColor3B const&) = mac 0x134ff0;
	virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = mac 0x135370;
	virtual auto setOpacityModifyRGB(bool) = mac 0x135200;
	virtual auto isOpacityModifyRGB() = mac 0x135350;
	virtual auto initWithTexture(cocos2d::CCTexture2D*) = mac 0x133110;
	virtual auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&) = mac 0x1330f0;
	virtual auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool) = mac 0x132f10;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame*) = mac 0x133270;
	virtual auto initWithSpriteFrameName(char const*) = mac 0x1332c0;
	virtual auto initWithFile(char const*) = mac 0x133180;
	virtual auto initWithFile(char const*, cocos2d::CCRect const&) = mac 0x133210;
	virtual auto setTexture(cocos2d::CCTexture2D*) = mac 0x135a90;
	virtual auto getTexture() = mac 0x135c00;
	// virtual auto setBlendFunc(cocos2d::_ccBlendFunc) = mac 0x50590;
	// virtual cocos2d::_ccBlendFunc getBlendFunc() = mac 0x505a0;
	virtual auto setChildColor(cocos2d::_ccColor3B const&) = mac 0x135160;
	virtual auto setChildOpacity(unsigned char) = mac 0x134f50;
	virtual auto getBatchNode() = mac 0x135910;
	virtual auto setBatchNode(cocos2d::CCSpriteBatchNode*) = mac 0x135920;
	virtual auto refreshTextureRect() = mac 0x133520;
	virtual auto setTextureRect(cocos2d::CCRect const&) = mac 0x133560;
	virtual auto setTextureRect(cocos2d::CCRect const&, bool, cocos2d::CCSize const&) = mac 0x133580;
	virtual auto setVertexRect(cocos2d::CCRect const&) = mac 0x1338f0;
	virtual auto setDisplayFrame(cocos2d::CCSpriteFrame*) = mac 0x135610;
	virtual auto isFrameDisplayed(cocos2d::CCSpriteFrame*) = mac 0x1356d0;
	virtual auto displayFrame() = mac 0x135760;
	virtual auto setDisplayFrameWithAnimationName(char const*, int) = mac 0x135680;
	// virtual auto isDirty() = mac 0x505b0;
	// virtual auto setDirty(bool) = mac 0x505c0;
	virtual auto setTextureCoords(cocos2d::CCRect const&) = mac 0x133910;
	virtual auto updateBlendFunc() = mac 0x135a20;
	virtual auto setReorderChildDirtyRecursively() = mac 0x134550;
	virtual auto setDirtyRecursively(bool) = mac 0x1345b0;

	CCSprite() = mac 0x133300;

	static cocos2d::CCSprite* create() = mac 0x132df0;
	static cocos2d::CCSprite* create(char const*) = mac 0x132a80;
	static cocos2d::CCSprite* createWithSpriteFrame(cocos2d::CCSpriteFrame*) = mac 0x132cb0;
	static cocos2d::CCSprite* createWithSpriteFrameName(char const*) = mac 0x132dc0;
	static cocos2d::CCSprite* createWithTexture(cocos2d::CCTexture2D*) = mac 0x132790;
}

class cocos2d::CCSpriteBatchNode {
	static cocos2d::CCSpriteBatchNode* create(char const*, unsigned int) = mac 0xbb540;
	static auto createWithTexture(cocos2d::CCTexture2D*, unsigned int) = mac 0xbb310;
	auto getUsedAtlasCapacity() = mac 0xbc6b0;
	auto increaseAtlasCapacity(unsigned int) = mac 0xbc670;
}

class cocos2d::CCSpriteFrameCache {
	auto addSpriteFramesWithFile(char const*) = mac 0x199a10;
	static cocos2d::CCSpriteFrameCache* sharedSpriteFrameCache() = mac 0x198970;
	auto spriteFrameByName(char const*) = mac 0x19a7e0;
}

class cocos2d::CCStandardTouchHandler {
    static cocos2d::CCStandardTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate*, int) = mac 0x247f30;
    virtual auto initWithDelegate(cocos2d::CCTouchDelegate*, int) = mac 0x247ed0, , ios 0x69; // iOS stub
    ~CCStandardTouchHandler() = mac 0x2482a0, , ios 0x6d28;
}

class cocos2d::CCString {
	virtual ~CCString() = mac 0x44c590;
	virtual auto isEqual(cocos2d::CCObject const*) = mac 0x44c8f0;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = mac 0x44ccb0;
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) = mac 0x44c870;
	// CCString(gd::string const&) = mac 0x44c310;
	auto boolValue() const = mac 0x44c810;
	auto doubleValue() const = mac 0x44c7f0;
	auto floatValue() const = mac 0x44c7d0;
	auto getCString() const = mac 0x44c470;
	auto intValue() const = mac 0x44c780;
}

class cocos2d::CCTargetedTouchHandler {
    static cocos2d::CCTargetedTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate*, int, bool) = mac 0x248010;
    auto initWithDelegate(cocos2d::CCTouchDelegate*, int, bool) = mac 0x2480f0, , ios 0x69; // iOS stub
    ~CCTargetedTouchHandler() = mac 0x248180, , ios 0x6cec;
}


class cocos2d::CCTexture2D {
	~CCTexture2D() = mac 0x246350;
	CCTexture2D() = mac 0x246280;
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
	auto setMaxS(float) = mac 0x2464f0;
	auto setMaxT(float) = mac 0x246510;
	auto setShaderProgram(cocos2d::CCGLProgram*) = mac 0x246530;
	auto initWithImage(cocos2d::CCImage*) = mac 0x246940;
	auto setTexParameters(cocos2d::_ccTexParams*) = mac 0x247980;
}

class cocos2d::CCTextFieldDelegate {

}

class cocos2d::CCTextureCache {
	auto addImage(char const*, bool) = mac 0x358120;
	static cocos2d::CCTextureCache* sharedTextureCache() = mac 0x356e00;
}

class cocos2d::CCTintTo {
	static cocos2d::CCTintTo* create(float, unsigned char, unsigned char, unsigned char) = mac 0x1f82a0;
}

class cocos2d::CCTouch {
	auto getDelta() const = mac 0x38340;
	auto getLocationInView() const = mac 0x38250;
	auto getPreviousLocationInView() const = mac 0x38270;
}

class cocos2d::CCTouchDispatcher {
	auto decrementForcePrio(int) = mac 0x280f70;
	auto incrementForcePrio(int) = mac 0x280f60;
}

class cocos2d::CCTouchHandler {
    virtual auto initWithDelegate(cocos2d::CCTouchDelegate*, int) = mac 0x247d10, , ios 0x69f8;
    ~CCTouchHandler() = mac 0x247de0, , ios 0x6ac0;
}

class cocos2d::CCTransitionFade {
	static cocos2d::CCTransitionFade* create(float, cocos2d::CCScene*) = mac 0x8ea30;
}

class cocos2d::ZipUtils {
	static auto compressString(gd::string, bool, int) = mac 0xe9a50;
	static auto decompressString(gd::string, bool, int) = mac 0xea380;
}

class cocos2d::extension::CCControlColourPicker {
	auto setColorValue(cocos2d::_ccColor3B const&) = mac 0x1aac10;
}

class cocos2d::extension::CCControlUtils {
	static cocos2d::extension::HSV HSVfromRGB(cocos2d::extension::RGBA) = mac 0x1e6750;
	static cocos2d::extension::RGBA RGBfromHSV(cocos2d::extension::HSV) = mac 0x1e6850;
}

class cocos2d::extension::CCScale9Sprite {
    CCScale9Sprite() = mac 0x211330;
    static cocos2d::extension::CCScale9Sprite* create(char const*) = mac 0x2130d0;
    static cocos2d::extension::CCScale9Sprite* create(char const*, cocos2d::CCRect) = mac 0x212ef0;
    virtual ~CCScale9Sprite() = mac 0x211590;
    virtual auto init() = mac 0x2115d0;
    virtual auto setContentSize(cocos2d::CCSize const&) = mac 0x2127c0, , ios 0x13e400;
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

class cocos2d::extension::CCScrollView {
	CCScrollView() = mac 0x214800;
	virtual ~CCScrollView() = mac 0x214c30;
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


class AccountHelpLayer : GJDropDownLayer, GJAccountDelegate, FLAlertLayerProtocol {
	create();
	doUnlink();
	exitLayer();
	onAccountManagement(cocos2d::CCObject*);
	onReLogin(cocos2d::CCObject*);
	onUnlink(cocos2d::CCObject*);
	updatePage();
	verifyUnlink();
	virtual void FLAlert_Clicked(FLAlertLayer* ,bool);
	virtual bool accountStatusChanged(void);
	virtual void customSetup(void);
	virtual void layerHidden(void);
}

class AccountLayer : GJDropDownLayer, GJAccountDelegate, GJAccountBackupDelegate, GJAccountSyncDelegate, FLAlertLayerProtocol {
	create();
	createToggleButton(std::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint);
	doBackup();
	doSync();
	exitLayer();
	hideLoadingUI();
	onBackup(cocos2d::CCObject*);
	onHelp(cocos2d::CCObject*);
	onLogin(cocos2d::CCObject*);
	onMore(cocos2d::CCObject*);
	onRegister(cocos2d::CCObject*);
	onSync(cocos2d::CCObject*);
	showLoadingUI();
	toggleUI(bool);
	updatePage(bool);
	virtual void FLAlert_Clicked(FLAlertLayer* ,bool);
	virtual bool accountStatusChanged(void);
	virtual bool backupAccountFailed(BackupAccountError);
	virtual bool backupAccountFinished(void);
	virtual void customSetup(void);
	virtual void layerHidden(void);
	virtual bool syncAccountFailed(BackupAccountError);
	virtual bool syncAccountFinished(void);
}

class AchievementCell {
	void loadFromDict(cocos2d::CCDictionary*) = mac 0x10eaa0;
}

class AchievementManager {
	void getAllAchievements() = mac 0x434d60;
	void sharedState() = mac 0x424420;
}

class AchievementNotifier : cocos2d::CCNode {
	cocos2d::CCScene* m_currentScene;
	cocos2d::CCArray* m_queue;
	AchievementBar* m_currentAchievement;

	void sharedState() = mac 0x464e00;
	void willSwitchToScene(cocos2d::CCScene*) = mac 0x4650b0;
}

class AchievementsLayer {
	void customSetup() = mac 0x1bdea0;
	void loadPage(int) = mac 0x1be190;
}

class AnimatedGameObject {
	void playAnimation(int) = mac 0xc93d0;
	void updateChildSpriteColor(cocos2d::_ccColor3B) = mac 0xc8450;
}

class AppDelegate : cocos2d::CCApplication, cocos2d::CCSceneDelegate {
	void bgScale() = mac 0x3aaab0;
	virtual bool applicationDidFinishLaunching() = mac 0x3aa900;
	virtual void applicationDidEnterBackground() = mac 0x3aabe0;
	virtual void applicationWillEnterForeground() = mac 0x3aac80;
	virtual bool applicationWillBecomeActive() = mac 0x3aab30;
	virtual bool applicationWillResignActive() = mac 0x3aab50;
	virtual void trySaveGame() = mac 0x3aaf10;
	virtual void willSwitchToScene(cocos2d::CCScene*) = mac 0x3aaf40;
	static AppDelegate* get() = mac 0x3aab10;

	cocos2d::CCScene* m_runningScene;
}

class ArtistCell : TableViewCell {
	// ArtistCell(char const*, float, float) = mac 0x11c740;
	void draw() = mac 0x11c980;
	bool init() = mac 0x11c7c0;
	void loadFromObject(SongInfoObject*) = mac 0x1118b0;
	void onNewgrounds(cocos2d::CCObject*) = mac 0x11c7e0;
	void updateBGColor(int) = mac 0x110460;
	void* m_idk;
}

class AudioEffectsLayer {
	void audioStep(float) = mac 0x271f40;
	static AudioEffectsLayer* create(gd::string) = mac 0x271a00;
	void resetAudioVars() = mac 0x271ee0;
}

class BoomListView : cocos2d::CCLayer, TableViewDataSource, TableViewDelegate {
	static BoomListView* create(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x18ecb0;
	bool init(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x18ee00;
	void draw() = mac 0x18f790;
	
	virtual void setupList() = mac 0x18ef90;
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) = mac 0x18f030;
	virtual void cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f070;
	virtual void didSelectRowAtIndexPath(CCIndexPath&, TableView*) {}
	virtual int numberOfRowsInSection(unsigned int, TableView*) = mac 0x18f0b0;
	virtual void numberOfSectionsInTableView(TableView*) = mac 0x18f0e0;
	virtual void cellForRowAtIndexPath(CCIndexPath&, TableView*) = mac 0x18f100;
	virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&) = mac 0x18f770;
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) = mac 0x18f050;
	virtual TableViewCell* getListCell(char const*) = mac 0x18f200;
	virtual void loadCell(TableViewCell*, int) = mac 0x18f4a0;

	TableView* m_tableView;
	cocos2d::CCArray* m_content;
	BoomListType m_type; 
	float m_width;
	float m_height;
	float m_cellHeight;
	int m_page;
}

class BoomScrollLayer : cocos2d::CCLayer {
	BoomScrollLayer() = mac 0x1e42f0;

    cocos2d::CCArray* m_dots;  
    PAD = win 0x40;
    ExtendedLayer* m_layer;    
    PAD = win 0x48;
    int m_page;    
}

class ButtonSprite : cocos2d::CCSprite {
	static ButtonSprite* create(char const*) = mac 0x4fa10;
	static ButtonSprite* create(char const*, int, int, float, bool) = mac 0x4fa40;
	void updateBGImage(char const*) = mac 0x502d0;
	static ButtonSprite* create(char const*, float) = mac 0x4fa60;
	static ButtonSprite* create(cocos2d::CCSprite*, int, int, float, float, bool, char const*, bool) = mac 0x4fa90;
}

class CCAnimatedSprite : cocos2d::CCSprite {
	void runAnimation(gd::string) = mac 0x1a6430;
	void tweenToAnimation(gd::string, float) = mac 0x1a65b0;

	gd::string m_unknown1;
	gd::string m_unknown2;
	PAD = win 0x14;
	gd::string m_unknown3;
	PAD = win 0x4;
}

class CCBlockLayer {
	void disableUI() = mac 0x2a5a80;
	void draw() = mac 0x2a5c20;
	void enableUI() = mac 0x2a5a90;
	void enterAnimFinished() = mac 0x2a5bb0;
	void enterLayer() = mac 0x2a5aa0;
	void exitLayer() = mac 0x2a5b40;
	void hideLayer(bool) = mac 0x2a5ba0;
	bool init() = mac 0x2a59c0;
	void layerHidden() = mac 0x2a5be0;
	void layerVisible() = mac 0x2a5bc0;
	void registerWithTouchDispatcher() = mac 0x2a5ad0;
	void showLayer(bool) = mac 0x2a5b90;

	bool m_unknown;
	bool m_unknown2;
}

class CCCircleWave : cocos2d::CCNode {
	static CCCircleWave* create(float, float, float, bool) = mac 0xbd270;
	static CCCircleWave* create(float, float, float, bool, bool) = mac 0xbd290;
	bool init(float, float, float, bool, bool) = mac 0xbd380;
	void followObject(cocos2d::CCNode*, bool) = mac 0xbd670;
	void updatePosition(float) = mac 0xbd630;

	cocos2d::CCArray* m_children;
	PAD = win 0x4; 
	float m_currentRadius; 
	float m_currentOpacity;
	cocos2d::ccColor3B m_color = mac 0x134;
	cocos2d::CCPoint m_circleCenter;	
	int m_filled; 
	int m_lineWidth;			
	float m_opacityMultiplier;	
	bool m_blendFuncDirty;
	CCCircleWaveDelegate* m_delegate = mac 0x150;
}

class CCCircleWaveDelegate {
	void circleWaveWillBeRemoved(CCCircleWave* wave) {}
}

class CCLightFlash {
	static CCLightFlash* create() = mac 0x295870;
	void playEffect(cocos2d::CCPoint, cocos2d::_ccColor3B, float, float, float, float, float, float, float, float, float, float, float, float, float, float, int, bool, bool, float) = mac 0x295900;
}

class CCMenuItemSpriteExtra : cocos2d::CCMenuItemSprite {
	static CCMenuItemSpriteExtra* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1253c0, win 0x18ee0;
	void setSizeMult(float) = mac 0x1255e0, win 0x19080;
	CCMenuItemSpriteExtra() = mac 0x32670;
	bool init(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x125450, win 0x18fa0;

	float m_scaleMultiplier;	
	float m_baseScale;			
	bool m_animationEnabled;	
	bool m_colorEnabled;		
	PAD = win 0x34;
	float m_colorDip;			
	cocos2d::CCPoint m_destPosition;	
	cocos2d::CCPoint m_offset;	
	MenuAnimationType m_animationType;	
	cocos2d::CCPoint m_startPosition;	
	PAD = win 0xc;
}

class CCMenuItemToggler : cocos2d::CCMenuItem {
    static CCMenuItemToggler* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x38400, win 0x19600, ios 0xf5594;
    void setSizeMult(float) = mac 0x38a40;
    void toggle(bool) = mac 0x38950;
    inline CCMenuItemToggler() : CCMenuItem(), m_onButton(nullptr), m_offButton(nullptr), m_toggled(false), m_notClickable(false) {}

	CCMenuItemSpriteExtra* m_onButton;
	CCMenuItemSpriteExtra* m_offButton;
	bool m_toggled;
	bool m_notClickable;
}

class CCMoveCNode : cocos2d::CCNode {
	static CCMoveCNode* create() = mac 0x1842a0;
	bool init() = mac 0x18b3d0;
	~CCMoveCNode() = mac 0x18b2c0;
}

class CCNodeContainer : cocos2d::CCNode {
	static CCNodeContainer* create() = mac 0xb1090;
	bool init() = mac 0xba950;
	void visit() = mac 0xba960;
}

class CCScrollLayerExt : cocos2d::CCLayer {
	virtual ~CCScrollLayerExt() = mac 0x2359b0;
	virtual void visit() = mac 0x236550;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x235ef0;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x236300;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x236020;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2362a0;
	virtual void registerWithTouchDispatcher() = mac 0x235eb0;
	virtual void preVisitWithClippingRect(cocos2d::CCRect) = mac 0x2366a0;
	virtual void postVisit() = mac 0x236720;

	void moveToTop() = mac 0x235870;
	void moveToTopWithOffset(float) = mac 0x2357d0;
}

class CCScrollLayerExtDelegate {

}

class CCSpritePlus : cocos2d::CCSprite {
	bool initWithSpriteFrameName(char const*) = mac 0x248670;

	cocos2d::CCArray* m_followers;
	CCSpritePlus* m_followingSprite;
	bool m_hasFollower;
	bool m_propagateScaleChanges;
	bool m_propagateFlipChanges;
}

class CCTextInputNode : cocos2d::CCLayer, cocos2d::CCIMEDelegate, cocos2d::CCTextFieldDelegate {
	static CCTextInputNode* create(float, float, char const*, char const*, int, char const*) = mac 0x5cfb0;

	void setLabelNormalColor(ccColor3B color) {
	    m_normalColor = color;
	    this->refreshLabel();
	}
	void setLabelPlaceholderColor(ccColor3B color) {
	    m_placeholderColor = color;
	    this->refreshLabel();
	}
	void setLabelPlaceholderScale(float scale) {
	    m_placeholderScale = scale;
	    this->refreshLabel();
	}
	void setMaxLabelScale(float scale) {
	    m_maxLabelScale = scale;
	    this->refreshLabel();
	}
	void setMaxLabelWidth(int length) {
	    m_maxLabelLength = length;
	    this->refreshLabel();
	}
	void setAllowedChars(gd::string filter) {
	    m_filter = filter;
	}
	void forceOffset() {
	    m_forceOffset = true;
	}
	void setString(const char* text) {
	    m_textField->setString(text);
	}
	const char* getString() {
	    return m_textField->getString();
	}
	CCTextFieldTTF* getTextField() {
	    return m_textField;
	}
	CCLabelBMFont* getPlaceholderLabel() {
	    return m_placeholderLabel;
	}
	void setDelegate(TextInputDelegate* delegate) {
	    m_delegate = delegate;
	}
	void refreshLabel() = mac 0x5d730, win 0x21330;
	void updateLabel(gd::string) = mac 0x5d4a0;
	
	virtual void registerWithTouchDispatcher() = mac 0x5eec0;

	bool init(float, float, char const*, char const*, int, char const*) = mac 0x5d180;
	virtual ~CCTextInputNode() = mac 0x5ceb0;
	virtual void visit() = mac 0x5d380;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ec80;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ee80;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5ee60;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x5eea0;
	virtual void textChanged() = mac 0x5dd70;
	virtual void onClickTrackNode(bool) = mac 0x5dd40;
	virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5dad0;
	virtual void keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&) = mac 0x5dc20;
	virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF*, char const*, int) = mac 0x5de50;
	virtual bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF*) = mac 0x5e2c0;
	virtual bool onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF*) = mac 0x5e610;

	void* m_unknown0;
	gd::string m_caption; // mac 0x188
	int m_unknown1;
	bool m_selected; // mac 0x194
	bool m_unknown2;
	gd::string m_allowedChars; // mac 0x198
	float m_maxLabelWidth;
	float m_maxLabelScale;
	float m_placeholderScale;
	cocos2d::ccColor3B m_placeholderColor;
	cocos2d::ccColor3B m_textColor;
	cocos2d::CCLabelBMFont* m_cursor;
	cocos2d::CCTextFieldTTF* m_textField; // mac 0x1c0
	TextInputDelegate* m_delegate; // TextInputDelegate vtable
	int m_maxLabelLength;
	cocos2d::CCLabelBMFont* m_placeholderLabel;
	bool m_unknown3;
	bool m_usePasswordChar;
	bool m_forceOffset;
}

class CheckpointObject {
	static CheckpointObject* create() = mac 0x7e7d0;
	void getObject() = mac 0x7ef50;

	GameObject* m_gameObject; 
	PlayerCheckpoint* m_player1; 
	PlayerCheckpoint* m_player2; 
	bool m_isDual; 
	bool m_isFlipped; 
	cocos2d::CCPoint m_cameraPos; 
	int m_unk104; 
	GameObject* m_lastPortal; 
	PAD = win 0x4;
	double m_unk110;
	gd::string m_currentStateString;
	gd::string m_objectsStateString;
}

class CollisionBlockPopup {
	static CollisionBlockPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x130010;
	void onNextItemID(cocos2d::CCObject*) = mac 0x130e60;
}

class CollisionTriggerAction : cocos2d::CCNode {
	static CollisionTriggerAction* createFromString(gd::string) = mac 0x176ee0;
}

class ColorAction : cocos2d::CCNode {
	void getSaveString() = mac 0x17d080;
	void setupFromDict(cocos2d::CCDictionary*) = mac 0x17f310;
	void setupFromString(gd::string) = mac 0x17f270;
	
    PAD = 0xc;
    cocos2d::ccColor3B m_color = mac 0x12c;
    PAD = 0x5;
    float m_unk100;
    bool m_blending; 
    PAD = 0x3;
    int m_playerColor; 
    int m_unk10C;
    float m_opacity; 
    float m_unk114;
    float m_copyHue;
    float m_copySaturation;
    float m_copyBrightness;
    bool m_saturationChecked;
    bool m_brightnessChecked;
    int m_copyID; 
    PAD = 0x1;
    bool m_copyOpacity;
    PAD = 0x4;
    ColorActionSprite* m_colorSprite; 
    PAD = 0xc;
}

class ColorActionSprite : cocos2d::CCNode {
	float m_opacity;
	cocos2d::_ccColor3B m_f0124;
	cocos2d::_ccColor3B m_activeColor;
}

class ColorChannelSprite {
	void updateBlending(bool) = mac 0x16e1d0;
	void updateCopyLabel(int, bool) = mac 0x16ded0;
	void updateOpacity(float) = mac 0x16e080;
	void updateValues(ColorAction*) = mac 0x16e2e0;

    cocos2d::CCLabelBMFont* m_copyLabel;    
    cocos2d::CCLabelBMFont* m_opacityLabel; 
    cocos2d::CCLabelBMFont* m_blendingDot;  
}

class CommentCell : TableViewCell {
	PAD = win 0x4;
	cocos2d::CCSprite* m_iconSprite;
	cocos2d::CCLabelBMFont* m_likeLabel;
	GJComment* m_comment;
	PAD = win 0x4;

	void loadFromComment(GJComment*) = mac 0x111c70;
}

class CountTriggerAction : cocos2d::CCNode {
	static CountTriggerAction* createFromString(gd::string) = mac 0x1754f0;

	int m_previousCount;
	int m_targetCount;
	int m_targetID;
	bool m_activateGroup;
	bool m_multiActivate = mac 0x138;
}

class CreateMenuItem : CCMenuItemSpriteExtra {
    PAD = win 0x18;
    int m_objectID;
    int m_buildTabPage;
    int m_buildTab;

	static CreateMenuItem* create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = mac 0x1c580;
}

class CreatorLayer : cocos2d::CCLayer {
	void onMyLevels(cocos2d::CCObject*) = mac 0x142b70;
	void onSavedLevels(cocos2d::CCObject*) = mac 0x142860;
}

class CurrencyRewardLayer {
	virtual ~CurrencyRewardLayer() = mac 0x447950;
	virtual void update(float) = mac 0x44a5c0;
}

class CustomListView : cocos2d::CCLayerColor {
	static CustomListView* create(cocos2d::CCArray*, float, float, int, BoomListType) = mac 0x10d410;
	void getListCell(char const*) = mac 0x10d560;
	void loadCell(TableViewCell*, int) = mac 0x10e610;
	void setupList() = mac 0x116e70;
}

class CustomSongCell : TableViewCell {
	void loadFromObject(SongInfoObject*) = mac 0x110220;
}

class CustomSongLayer : FLAlertLayer, FLAlertLayerProtocol, TextInputDelegate, GJDropDownLayerDelegate {
	bool init(LevelSettingsObject*) = mac 0xf06f0;
	void onArtists(cocos2d::CCObject*) = mac 0xf1950;
	void onSongBrowser(cocos2d::CCObject*) = mac 0xf18a0;

	LevelSettingsObject* m_levelSettings;
	CCTextInputNode* m_songIDInput;
	CustomSongWidget* m_songWidget;
	LevelSettingsLayer* m_levelSettingsLayer;
}

class DialogLayer {
	virtual ~DialogLayer() = mac 0x204720;
	virtual void onEnter() = mac 0x205900;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205790;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205820;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2057e0;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x205840;
	virtual void registerWithTouchDispatcher() = mac 0x205890;
	virtual void keyBackClicked() = mac 0x2056a0;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x205ce0;
	virtual void fadeInTextFinished(TextArea*) = mac 0x205930;
}

class EditorOptionsLayer {
	void onButtonsPerRow(cocos2d::CCObject*) = mac 0x147b30;
}

class EditorPauseLayer : FLAlertLayer {
	virtual ~EditorPauseLayer() = mac 0x13c3b0;
	virtual void keyBackClicked() = mac 0x13f320;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x13f3a0;
	virtual void customSetup() = mac 0x13cc00;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x13f1b0;

	static EditorPauseLayer* create(LevelEditorLayer* editor) {
	    auto pRet = new EditorPauseLayer;
	
	    if (pRet && pRet->init(editor)) {
	        pRet->autorelease();
	        return pRet;
	    }
	
	    CC_SAFE_DELETE(pRet);
	    return nullptr;
	}
	void saveLevel() = mac 0x13ebd0;
	bool init(LevelEditorLayer*) = mac 0x13c7a0;
}

class EditorUI : cocos2d::CCLayer {
	void constrainGameLayerPosition() = mac 0x1c6d0;
	void create(LevelEditorLayer*) = mac 0x8a80;
	void deselectAll() = mac 0x1f300;
	void onDeselectAll(cocos2d::CCObject*) = mac 0x19cd0;
	void disableButton(CreateMenuItem*) = mac 0x1c0f0;
	void editButtonUsable() = mac 0x28f30;
	void editObject(cocos2d::CCObject*) = mac 0x195a0;
	void enableButton(CreateMenuItem*) = mac 0x1bff0;
	CCMenuItemSpriteExtra* getCreateBtn(int, int) = mac 0x1f6c0;
	void getGroupCenter(cocos2d::CCArray*, bool) = mac 0x23470;
	cocos2d::CCArray* getSelectedObjects() = mac 0x23f30;
	void init(LevelEditorLayer*) = mac 0x8ae0;
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2ed60;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2f3d0;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2fb00;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x30790;
	void moveObject(GameObject*, cocos2d::CCPoint) = mac 0x24b10;
	void onDuplicate(cocos2d::CCObject*) = mac 0x18ba0;
	void pasteObjects(gd::string) = mac 0x232d0;
	void playerTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x2ebf0;
	void playtestStopped() = mac 0x24790;
	void redoLastAction(cocos2d::CCObject*) = mac 0xb8e0;
	void replaceGroupID(GameObject*, int, int) = mac 0x27470;
	void scaleChanged(float) = mac 0x25490;
	void scaleObjects(cocos2d::CCArray*, float, cocos2d::CCPoint) = mac 0x252e0;
	void selectObjects(cocos2d::CCArray*, bool) = mac 0x23940;
	void setupCreateMenu() = mac 0xcb50;
	void undoLastAction(cocos2d::CCObject*) = mac 0xb830;
	void updateButtons() = mac 0x1a300;
	void updateObjectInfoLabel() = mac 0x1cb10;
	void updateSlider() = mac 0x18a90;
	void updateZoom(float) = mac 0x248c0;

	LevelEditorLayer* m_editorLayer = mac 0x408;
	cocos2d::CCArray* m_editBars = mac 0x358;
	cocos2d::CCNode* m_locationSlider = mac 0x228;
	GameObject* m_lastSelectedObject = mac 0x440;
	gd::string m_clipboard = mac 0x458;
}

class EffectGameObject : GameObject {
	static EffectGameObject* create(char const*) = mac 0xc9790;
	void getTargetColorIndex() = mac 0xca1f0;
	virtual void triggerObject(GJBaseGameLayer*) = mac 0xc9870;

	int m_targetGroup = mac 0x4F8;
	bool m_activateGroup = mac 0x578;
	bool m_touchHoldMode = mac 0x579;
	int m_animationID = mac 0x584;
	float m_spawnDelay = mac 0x588;
	bool m_multiTrigger = mac 0x594;
	int m_targetCount = mac 0x598;
	int m_compareType = mac 0x5A0;
	int m_itemBlockBID = mac 0x5A8;
	int m_itemBlockID = mac 0x5B0;
}

class EndLevelLayer {
	static EndLevelLayer* create() = mac 0x2787d0;
}

class EndPortalObject : GameObject {
	static EndPortalObject* create() = mac 0x1da8f0;
	void updateColors(cocos2d::_ccColor3B) = mac 0x1dacb0;
}

class FLAlertLayer : cocos2d::CCLayerColor {
    virtual ~FLAlertLayer() {
	    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->decrementForcePrio(2);
	}
    virtual void onEnter() = mac 0x25f350;
    virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25ee40;
    virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25f0a0;
    virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25ef60;
    virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = mac 0x25f020;
    virtual void registerWithTouchDispatcher() = mac 0x25f2e0;
    virtual void keyBackClicked() = mac 0x25ed90;
    virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x25ece0;
    virtual void show() = mac 0x25f120, win 0x23560, ios 0x1feff4;

    bool init(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float) =                   mac 0x25e1b0;

    static FLAlertLayer* create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*) =                             , win 0x22680;
    [[mangle('_ZN12FLAlertLayer6createEP20FLAlertLayerProtocolPKcSsS3_S3_f')]]
    static FLAlertLayer* create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float) =              mac 0x25e0e0, win 0x22730, ios 0x1fe374;
    static FLAlertLayer* create(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float) = mac 0x25dec0;
    
    static FLAlertLayer* create(char const* title, const gd::string &desc, char const* btn) {
        return FLAlertLayer::create(nullptr, title, desc, btn, nullptr, 300.0);
    }

    FLAlertLayer() {
    	m_buttonMenu = nullptr;
		m_controlConnected = -1;
		m_mainLayer = nullptr;
		m_ZOrder = 0;
		m_noElasticity = false;
		m_reverseKeyBack = false;
		m_scene = nullptr;
		m_alertProtocol = nullptr;
		m_scrollingLayer = nullptr;
		m_button2 = nullptr;
		m_button1 = nullptr;
		m_joystickConnected = -1;
		m_containsBorder = 0;
    }

    void onBtn1(cocos2d::CCObject*) = mac 0x25ec20;
    void onBtn2(cocos2d::CCObject*) = mac 0x25ec80;

    cocos2d::CCMenu* m_buttonMenu;                      // mac 0x1f8
    int m_controlConnected;                             // mac 0x200
    FLAlertLayerProtocol* m_alertProtocol;              // mac 0x208
    cocos2d::CCNode* m_scene;                           // mac 0x210
    bool m_reverseKeyBack;                              // mac 0x211
    cocos2d::ccColor3B m_color;                         // mac 0x212
    cocos2d::CCLayer* m_mainLayer;                      // mac 0x220
    int m_ZOrder;                                       // mac 0x228
    bool m_noElasticity;                                // mac 0x22c
    cocos2d::ccColor3B m_color2;                        // mac 0x230
    ButtonSprite* m_button1;                            // mac 0x238
    ButtonSprite* m_button2;                            // mac 0x240
    cocos2d::CCLayerColor* m_scrollingLayer;            // mac 0x248
    int m_joystickConnected;                            // mac 0x250
    bool m_containsBorder;                              // mac 0x251
    bool m_noAction;                                    // mac 0x252
}

class FLAlertLayerProtocol {
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) {}
}

class GameToolbox {
	[[mangle('_ZN11GameToolbox18createToggleButtonESsMN7cocos2d8cocos2d::CCObjectEFvPS1_EbPNS0_6CCMenuENS0_7CCPointEPNS0_6CCNodeES9_PNS0_7cocos2d::CCArrayE')]]
	static CCMenuItemToggler createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCArray*) = mac 0x0;
	[[mangle('_ZN11GameToolbox18createToggleButtonESsMN7cocos2d8cocos2d::CCObjectEFvPS1_EbPNS0_6CCMenuENS0_7CCPointEPNS0_6CCNodeES9_fffS7_PKcbiPNS0_7cocos2d::CCArrayE')]]
	static CCMenuItemToggler createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCNode*, cocos2d::CCNode*, float, float, float, cocos2d::CCPoint, char const*, bool, int, cocos2d::CCArray*) = mac 0x0;
}

class GJAccountBackupDelegate {
	virtual bool backupAccountFailed(BackupAccountError) {
		return false;
	}
	virtual bool backupAccountFinished(void) {
		return false;
	}
}
class GJAccountDelegate {
	virtual bool accountStatusChanged(void) {
		return false;
	}
}
class GJAccountLoginDelegate {
	virtual bool loginAccountFailed(AccountError) {
		return false;
	}
	virtual bool loginAccountFinished(int,int) {
		return false;
	}
}

class GJAccountSyncDelegate {
	virtual bool syncAccountFailed(BackupAccountError) {
		return false;
	}
	virtual bool syncAccountFinished(void) {
		return false;
	}
}

class GJRotationControl : cocos2d::CCLayer {
	virtual bool init() = mac 0x31510, win 0x93f10;
	void updateSliderPosition(cocos2d::CCPoint) = mac 0x316a0, win 0x94020;

	float m_unknown0;
	float m_unknown1;
	cocos2d::CCPoint m_sliderPosition;
	cocos2d::CCSprite* m_sliderThumb;
	float m_objAngle;
	float m_angle;
	int m_touchID;
}

class GJBaseGameLayer : cocos2d::CCLayer {
	virtual void objectsCollided(int, int) = mac 0xb6d90;
	virtual void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int) = mac 0xb73a0;
	virtual void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*) = mac 0xb7420;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0xba990;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0xba9a0;
	virtual void toggleGroupTriggered(int, bool) = mac 0xb75a0;
	virtual void spawnGroup(int) = mac 0xb7050;
	virtual void addToSection(GameObject*) = mac 0xb7b70;
	virtual void addToGroup(GameObject*, int, bool) = mac 0xb77f0;
	virtual void removeFromGroup(GameObject*, int) = mac 0xb7a60;
	virtual bool init() = mac 0xafc90;

	void addObjectCounter(LabelGameObject*, int) = mac 0xb9eb0;
	void addToGroups(GameObject*, bool) = mac 0xb7780;
	void atlasValue(int) = mac 0xb21e0;
	void bumpPlayer(PlayerObject*, GameObject*) = mac 0xb6860;
	void calculateOpacityValues(EffectGameObject*, EffectGameObject*, float, GJEffectManager*) = mac 0xb5be0;
	void checkSpawnObjects() = mac 0xb6f90;
	void collectItem(int, int) = mac 0xb9e20;
	void collectedObject(EffectGameObject*) = mac 0xb9b60;
	void createTextLayers() = mac 0xb5260;
	cocos2d::CCArray* damagingObjectsInRect(cocos2d::CCRect) = mac 0xb6140;
	void enableHighCapacityMode() = mac 0xb11e0;
	void getCapacityString() = mac 0xb2210;
	void getGroundHeightForMode(int) = mac 0xb6630;
	void getGroup(int) = mac 0xb6f20;
	void getMoveDeltaForObjects(int, int) = mac 0xb6db0;
	void getOptimizedGroup(int) = mac 0xb7940;
	void getStaticGroup(int) = mac 0xb79a0;
	void isGroupDisabledForObject(GameObject*) = mac 0xb5cc0;
	void isGroupDisabledForObjectFull(GameObject*, cocos2d::CCArray*) = mac 0xb5de0;
	void loadUpToPosition(float) = mac 0xba680;
	void objectIntersectsCircle(GameObject*, GameObject*) = mac 0xb66e0;
	void objectTriggered(EffectGameObject*) = mac 0xb71b0;
	void optimizeMoveGroups() = mac 0xb96c0;
	void parentForZLayer(int, bool, int) = mac 0xb55d0;
	void playerTouchedRing(PlayerObject*, GameObject*) = mac 0xb69e0;
	void processColorObject(EffectGameObject*, int, cocos2d::CCDictionary*, float, GJEffectManager*) = mac 0xb5a90;
	void processFollowActions() = mac 0xb8fd0;
	void processMoveActions() = mac 0xb86c0;
	void processMoveActionsStep(float) = mac 0xb7ea0;
	void processOpacityObject(EffectGameObject*, cocos2d::CCDictionary*, float, GJEffectManager*) = mac 0xb5ae0;
	void processPlayerFollowActions(float) = mac 0xb8b50;
	void processRotationActions() = mac 0xb7fd0;
	void pushButton(int, bool) = mac 0xb9920;
	void rectIntersectsCircle(cocos2d::CCRect, cocos2d::CCPoint, float) = mac 0xb6470;
	void refreshCounterLabels() = mac 0xb9fc0;
	void releaseButton(int, bool) = mac 0xb9a00;
	void removeFromGroups(GameObject*) = mac 0xb7a00;
	void removeObjectFromSection(GameObject*) = mac 0xb7e00;
	void reorderObjectSection(GameObject*) = mac 0xb7cb0;
	void resetGroupCounters(bool) = mac 0xba300;
	void resetMoveOptimizedValue() = mac 0xb9670;
	int sectionForPos(float) = mac 0xb6120;
	void setupLayers() = mac 0xaffe0;
	void shouldExitHackedLevel() = mac 0xb1100;
	void spawnGroupTriggered(int, float, int) = mac 0xb7020;
	cocos2d::CCArray* staticObjectsInRect(cocos2d::CCRect) = mac 0xb5f90;
	void testInstantCountTrigger(int, int, int, bool, int) = mac 0xb9ae0;
	void toggleGroup(int, bool) = mac 0xb75f0;
	void togglePlayerVisibility(bool) = mac 0xba910;
	void triggerMoveCommand(EffectGameObject*) = mac 0xb7290;
	void updateCollisionBlocks() = mac 0xb6a30;
	void updateCounters(int, int) = mac 0xb9bc0;
	void updateDisabledObjectsLastPos(cocos2d::CCArray*) = mac 0xb95b0;
	void updateLayerCapacity(gd::string) = mac 0xb1680;
	void updateLegacyLayerCapacity(int, int, int, int) = mac 0xb1590;
	void updateOBB2(cocos2d::CCRect) = mac 0xb63f0;
	void updateQueuedLabels() = mac 0xb9f30;
	~GJBaseGameLayer() = mac 0xaf990;

	GJEffectManager* m_effectManager = mac 0x180;
	cocos2d::CCLayer* m_objectLayer = mac 0x188;
	cocos2d::CCArray* m_objects = mac 0x3a0;
	cocos2d::CCArray* m_sections = mac 0x3a8;
	PlayerObject* m_player1 = mac 0x380;
	PlayerObject* m_player2 = mac 0x388;
	LevelSettingsObject* m_levelSettings = mac 0x390;
	cocos2d::CCDictionary* m_unknownDict = mac 0x398;
}

class GJComment : cocos2d::CCNode {
	virtual ~GJComment() = mac 0x2dfd70;
	virtual bool init() = mac 0x2dfec0;

	gd::string m_commentString;
	PAD = win 0x18;
	int m_commentID;
	PAD = win 0x4;
	int m_likeCount;
	PAD = win 0x8;
	int m_accountID;
	gd::string m_uploadDate;
}

class GJCommentListLayer : cocos2d::CCLayerColor {
	virtual ~GJCommentListLayer() = mac 0x1482a0;
	static GJCommentListLayer* create(BoomListView*, char const*, cocos2d::_ccColor4B, float, float, bool) = mac 0x147d00;

    BoomListView* m_list; 
}

class GJDropDownLayer : cocos2d::CCLayerColor {
	virtual void customSetup() {}
	virtual void enterLayer() = mac 0x3525c0;
	virtual void exitLayer(cocos2d::CCObject*) = mac 0x352670;
	virtual void showLayer(bool) = mac 0x3526c0;
	virtual void hideLayer(bool) = mac 0x3527b0;
	virtual void layerVisible() = mac 0x3528b0;
	virtual void layerHidden() = mac 0x3528d0;
	virtual void enterAnimFinished() {}
	virtual void disableUI() = mac 0x352580;
	virtual void enableUI() = mac 0x3525a0;
	virtual void draw() = mac 0x352910;
	bool init(char const*, float) = mac 0x352100;
	virtual void registerWithTouchDispatcher() = mac 0x3525f0;
	virtual void keyBackClicked() = mac 0x352630;

	static GJDropDownLayer* create(const char* title, float height) {
	    GJDropDownLayer* pRet = new GJDropDownLayer();
	    if (pRet && pRet->init(title, height)) {
	        pRet->autorelease();
	        return pRet;
	    }
	    else {
	        CC_SAFE_DELETE(pRet);
	        return nullptr;
	    }
	}

	cocos2d::CCPoint m_endPosition;
	cocos2d::CCPoint m_startPosition;
	cocos2d::CCMenu* m_buttonMenu;
	GJListLayer* m_listLayer;
	bool m_controllerEnabled;
	cocos2d::CCLayer* m_mainLayer;
	bool m_hidden;
	void* m_unknown;
}

class GJEffectManager : cocos2d::CCNode {
	virtual bool init() = mac 0x180230;

	void activeColorForIndex(int) = mac 0x180cb0;
	void activeOpacityForIndex(int) = mac 0x180e10;
	void addAllInheritedColorActions(cocos2d::CCArray*) = mac 0x1817a0;
	void addGroupPulseEffect(PulseEffectAction*) = mac 0x184c10;
	void calculateBaseActiveColors() = mac 0x180f70;
	void calculateInheritedColor(int, ColorAction*) = mac 0x1818f0;
	void calculateLightBGColor(cocos2d::_ccColor3B) = mac 0x185b90;
	void colorActionChanged(ColorAction*) = mac 0x181dc0;
	void colorExists(int) = mac 0x181da0;
	void colorForEffect(cocos2d::_ccColor3B, cocos2d::_ccHSVValue) = mac 0x182650;
	cocos2d::_ccColor3B colorForGroupID(int, cocos2d::_ccColor3B const&, bool) = mac 0x184f90;
	void colorForIndex(int) = mac 0x180ad0;
	void colorForPulseEffect(cocos2d::_ccColor3B const&, PulseEffectAction*) = mac 0x181bf0;
	void countChangedForItem(int) = mac 0x185a40;
	void countForItem(int) = mac 0x185a10;
	static GJEffectManager* create() = mac 0x1800f0;
	void createFollowCommand(float, float, float, int, int, bool, int) = mac 0x182ed0;
	void createMoveCommand(cocos2d::CCPoint, int, float, int, float, bool, bool, int) = mac 0x182cc0;
	void createPlayerFollowCommand(float, float, int, float, float, int, int) = mac 0x182fe0;
	void createRotateCommand(int, float, int, int, int, float, bool, int) = mac 0x182df0;
	void getAllColorActions() = mac 0x180980;
	void getAllColorSprites() = mac 0x1809e0;
	const cocos2d::_ccColor3B& getColorAction(int) = mac 0x180b00;
	const cocos2d::_ccColor3B& getColorSprite(int) = mac 0x180d00;
	void getCurrentStateString() = mac 0x1867e0;
	void getLoadedMoveOffset() = mac 0x184390;
	void getMixedColor(cocos2d::_ccColor3B, cocos2d::_ccColor3B, float) = mac 0x185d30;
	uint8_t getOpacityActionForGroup(int) = mac 0x1845b0;
	void getSaveString() = mac 0x185e90;
	void handleObjectCollision(bool, int, int) = mac 0x1828f0;
	void hasActiveDualTouch() = mac 0x185540;
	void hasBeenTriggered(int) = mac 0x1853b0;
	void hasPulseEffectForGroupID(int) = mac 0x184f60;
	bool isGroupEnabled(int) = mac 0x1853d0;
	void keyForGroupIDColor(int, cocos2d::_ccColor3B const&, bool) = mac 0x184c90;
	void loadState(gd::string) = mac 0x188db0;
	void objectsCollided(int, int) = mac 0x182a00;
	void opacityForIndex(int) = mac 0x180c80;
	float opacityModForGroup(int) = mac 0x184740;
	void playerButton(bool, bool) = mac 0x1855a0;
	void playerDied() = mac 0x185860;
	void postCollisionCheck() = mac 0x182720;
	void preCollisionCheck() = mac 0x182680;
	void prepareMoveActions(float, bool) = mac 0x183660;
	void processColors() = mac 0x180e70;
	void processCopyColorPulseActions() = mac 0x181530;
	void processInheritedColors() = mac 0x181190;
	void processPulseActions() = mac 0x181040;
	void registerCollisionTrigger(int, int, int, bool, bool, int) = mac 0x182b70;
	void removeAllPulseActions() = mac 0x1825e0;
	void removeColorAction(int) = mac 0x181d60;
	void reset() = mac 0x180690;
	void resetColorCache() = mac 0x185280;
	void resetEffects() = mac 0x1807d0;
	void resetMoveActions() = mac 0x180940;
	void resetToggledGroups() = mac 0x1853f0;
	void resetTriggeredIDs() = mac 0x182630;
	void runCountTrigger(int, int, bool, int, bool, int) = mac 0x1858d0;
	void runDeathTrigger(int, bool, int) = mac 0x1857a0;
	OpacityEffectAction* runOpacityActionOnGroup(int, float, float, int) = mac 0x1845d0;
	void runPulseEffect(int, bool, float, float, float, PulseEffectType, cocos2d::_ccColor3B, cocos2d::_ccHSVValue, int, bool, bool, bool, int) = mac 0x184890;
	void runTouchTriggerCommand(int, bool, TouchTriggerType, bool, int) = mac 0x185460;
	void setColorAction(ColorAction*, int) = mac 0x181d00;
	void setFollowing(int, int, bool) = mac 0x185e00;
	void setupFromString(gd::string) = mac 0x186290;
	void shouldBlend(int) = mac 0x180e40;
	void spawnGroup(int, float, int) = mac 0x1852a0;
	void stopActionsForTrigger(EffectGameObject*) = mac 0x183150;
	void stopMoveActionsForGroup(int) = mac 0x1830e0;
	void storeTriggeredID(int) = mac 0x185380;
	void toggleGroup(int, bool) = mac 0x182c80;
	void traverseInheritanceChain(InheritanceNode*) = mac 0x181850;
	void updateActiveOpacityEffects() = mac 0x1847e0;
	void updateColorAction(ColorAction*) = mac 0x184560;
	void updateColorEffects(float) = mac 0x181f40;
	void updateColors(cocos2d::_ccColor3B, cocos2d::_ccColor3B) = mac 0x180a40;
	void updateEffects(float) = mac 0x181df0;
	void updateOpacityAction(OpacityEffectAction*) = mac 0x184780;
	void updateOpacityEffects(float) = mac 0x1823e0;
	void updatePulseEffects(float) = mac 0x182130;
	void updateSpawnTriggers(float) = mac 0x182510;
	void wasFollowing(int, int) = mac 0x185e60;
	void wouldCreateLoop(InheritanceNode*, int) = mac 0x181820;
	~GJEffectManager() = mac 0x17fe00;

	GJBaseGameLayer* m_gameLayer;
	cocos2d::CCDictionary* m_colorActions;
	cocos2d::CCDictionary* m_colorSprites;
	cocos2d::CCDictionary* m_pulseActionsForGroup;
	cocos2d::CCDictionary* m_colorCache;
	cocos2d::CCDictionary* m_opacityActionsForGroup;
	cocos2d::CCDictionary* m_f0150;
	cocos2d::CCArray* m_opacityActions;
	cocos2d::CCArray* m_touchActions;
	cocos2d::CCDictionary* m_countActions;
	cocos2d::CCArray* m_onDeathActions;
	cocos2d::CCArray* m_collisionActions;
	cocos2d::CCDictionary* m_f0180;
	cocos2d::CCDictionary* m_f0188;
	gd::vector<InheritanceNode*> m_inheritanceNodesForGroup;
	cocos2d::CCDictionary* m_f01a8;
	cocos2d::CCDictionary* m_collisionActionsForGroup1;
	cocos2d::CCDictionary* m_collisionActionsForGroup2;
	gd::vector<ColorAction*> m_colorActionsForGroup;
	gd::vector<ColorActionSprite*> m_colorSpritesForGroup;
	bool m_pulseExistsForGroup[1100];
	bool m_f063c;
	bool m_opactiyExistsForGroup[1100];
	int m_itemValues[1100];
	int m_unusued;
	int* m_unused2;
	cocos2d::CCArray* m_f1bc8;
	cocos2d::CCArray* m_inheritanceChain;
	cocos2d::CCDictionary* m_f1bd8;
	gd::vector<bool> m_groupToggled;
	cocos2d::CCDictionary* m_triggeredIDs;
	cocos2d::CCDictionary* m_followActions;
	cocos2d::CCArray* m_spawnActions;
	cocos2d::CCArray* m_moveActions;
	cocos2d::CCArray* m_f1c28;
	cocos2d::CCNode* m_f1c30;
	cocos2d::CCDictionary* m_f1c38;
	cocos2d::CCDictionary* m_f1c40;
	cocos2d::CCDictionary* m_f1c48;
	cocos2d::CCDictionary* m_f1c50;
	float m_time;
	float m_velocity;
	float m_acceleration;
	bool m_moveOptimizationEnabled;
}

class GJGameLevel {
	virtual ~GJGameLevel() = mac 0x2db2d0;
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x2dd1c0;
	virtual bool canEncode() = mac 0x2ddae0;
	virtual bool init() = mac 0x2db310;

	static GJGameLevel* create() = mac 0x2b83e0;
	void getAudioFileName() = mac 0x2dbe70;
	void getCoinKey(int) = mac 0x2ce360;
	void getLengthKey(int) = mac 0x2dbba0;
	void getNormalPercent() = mac 0x2b8b20;
	void levelWasAltered() = mac 0x2db530;
	void savePercentage(int, bool, int, int, bool) = mac 0x2db700;

	int m_levelId = mac 0x130;
	gd::string m_name = mac 0x138;
	gd::string m_levelString = mac 0x148;
	gd::string m_author = mac 0x150;
	int m_audioTrack = mac 0x18c;
	int m_songID = mac 0x190;
	int m_difficultyDenominator = mac 0x1ac;
	int m_difficultyNumerator = mac 0x1b0;  
	bool m_lowDetail = mac 0x1c5;
	int m_bestNormal = mac 0x214;
	int m_bestPractice = mac 0x238;
	int m_score = mac 0x248;
	int m_epic = mac 0x24c;
	int m_demon = mac 0x26c;
	int m_stars = mac 0x27c;
	OBB2D* m_hitbox = mac 0x2b0;
	bool m_official = mac 0x324;
}

class GJListLayer : cocos2d::CCLayerColor {
	virtual ~GJListLayer() = mac 0x344350;

	BoomListView* m_listView;
}

class GJRobotSprite {
	virtual ~GJRobotSprite() = mac 0x34aaf0;
	virtual bool init() = mac 0x34ad50;
	virtual void setOpacity(unsigned char) = mac 0x34bcc0;
	virtual void hideSecondary() = mac 0x34c3b0;
	
	static GJRobotSprite* create() = mac 0x34ac00;
	void updateColor02(cocos2d::_ccColor3B) = mac 0x34bbd0;
	void updateFrame(int) = mac 0x34bdd0;

	PAD = win 0x8;
	cocos2d::ccColor3B m_secondaryColor; 
}

class GJSpecialColorSelectDelegate {

}

class GJSpiderSprite {
	virtual ~GJSpiderSprite() = mac 0x34c4a0;
	virtual bool init() = mac 0x34c700;

	static GJSpiderSprite* create() = mac 0x34c5b0;
}

class GManager : cocos2d::CCNode {
	virtual void setup() {}

	void save() = mac 0x26f300;
	void saveData(DS_Dictionary*, gd::string) = mac 0x26f4b0;
	void saveGMTo(gd::string) = mac 0x26f3b0;

	gd::string m_fileName;
	bool m_setup;
	bool m_saved;
}

class GameLevelManager {
	GJGameLevel* createNewLevel() = mac 0x2b8180;
	static GameLevelManager* sharedState() = mac 0x2a8340;
	void getPageInfo(char const*) = mac 0x2c0050;
	cocos2d::CCArray* getStoredOnlineLevels(char const*) = mac 0x2bfe80;
	void getTopArtists(int, int) = mac 0x2ce3d0;
	void getTopArtistsKey(int) = mac 0x2ce7a0;
	void makeTimeStamp(char const*) = mac 0x2bfd90;

	cocos2d::CCDictionary* m_timerDict = mac 0x1e8;
}

class GameManager : cocos2d::CCNode {
    void accountStatusChanged() = mac 0x1cdad0;
    const cocos2d::_ccColor3B& colorForIdx(int) = mac 0x1cbc80, , ios 0x237488;
    void didExitPlayscene() = mac 0x1d0230;
    void doQuickSave() = mac 0x1d0200;
    void fadeInMusic(char const*) = mac 0x1c2ff0;
    void getBGTexture(int) = mac 0x1cca00;
    void getFontFile(int) = mac 0x1cc5f0;
    bool getGameVariable(char const*) = mac 0x1cccd0;
    int getIntGameVariable(char const*) = mac 0x1cd1d0;
    void getUGV(char const*) = mac 0x1ccfa0;
    void loadDeathEffect(int) = mac 0x1cc690;
    void loadFont(int) = mac 0x1cc550;
    void reloadAll(bool, bool, bool) = mac 0x1d08a0;
    void reloadAllStep5() = mac 0x1d0b00;
    void reportPercentageForLevel(int, int, bool) = mac 0x1c5b00;
    void setGameVariable(char const*, bool) = mac 0x1cca80;
    void setIntGameVariable(char const*, int) = mac 0x1cd0e0;
    void setUGV(char const*, bool) = mac 0x1cce50;
    static GameManager* sharedState() = mac 0x1c2b30, , ios 0x2321b8;
    ~GameManager() = mac 0x1d0e00;
    void getGTexture(int) = mac 0x1cca40;
    bool init() = mac 0x1c2ec0;
    void reportAchievementWithID(char const*, int, bool) = mac 0x1c6460;
    void resolutionForKey(int) = mac 0x1d0b40;
    void update(float) = mac 0x1d0270;

    PlayLayer* m_playLayer = mac 0x180;
    LevelEditorLayer* m_editorLayer = mac 0x188;
    int m_scene = mac 0x1f4;
    bool m_ldm = mac 0x2a1;
}

class GameObject : CCSpritePlus {
	virtual ~GameObject() = mac 0x2f4ca0;
	virtual void update(float) = mac 0x2fbb90;
	virtual void setScaleX(float) = mac 0x335b00;
	virtual void setScaleY(float) = mac 0x335b90;
	virtual void setScale(float) = mac 0x335c20;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x335850;
	virtual void setVisible(bool) = mac 0x336280;
	virtual void setRotation(float) = mac 0x335970;
	virtual void setOpacity(unsigned char) = mac 0x335f10;
	virtual bool initWithTexture(cocos2d::CCTexture2D*) = mac 0x2f56a0;
	virtual void setChildColor(cocos2d::_ccColor3B const&) = mac 0x341f20;
	virtual void setFlipX(bool) = mac 0x335a60;
	virtual void setFlipY(bool) = mac 0x335ab0;
	virtual void customSetup() = mac 0x2fbba0;
	virtual void setupCustomSprites() = mac 0x307f60;
	virtual void addMainSpriteToParent(bool) = mac 0x33a5b0;
	virtual void resetObject() = mac 0x2fa620;
	virtual void triggerObject(GJBaseGameLayer*) = mac 0x2fa8f0;
	virtual void activateObject() = mac 0x2faf60;
	virtual void deactivateObject(bool) = mac 0x2fb8f0;
	virtual cocos2d::CCRect* getObjectRect() = mac 0x3352b0;
	virtual cocos2d::CCRect* getObjectRect(float, float) = mac 0x3352d0;
	virtual cocos2d::CCRect* getObjectRect2(float, float) = mac 0x3354e0;
	virtual cocos2d::CCRect* getObjectTextureRect() = mac 0x3355b0;
	virtual void getRealPosition() = mac 0x335750;
	virtual void setStartPos(cocos2d::CCPoint) = mac 0x2fa520;
	virtual void updateStartValues() = mac 0x2fa800;
	virtual void customObjectSetup() = mac 0xdc1a0;
	virtual gd::string getSaveString() = mac 0x33d3d0;
	virtual bool isFlipX() = mac 0x335a40;
	virtual bool isFlipY() = mac 0x335a50;
	virtual void setRScaleX(float) = mac 0x335cb0;
	virtual void setRScaleY(float) = mac 0x335d60;
	virtual void setRScale(float) = mac 0x335e10;
	virtual void getRScaleX() = mac 0x335e50;
	virtual void getRScaleY() = mac 0x335e80;
	virtual void calculateSpawnXPos() = mac 0x336970;
	virtual void triggerActivated(float) = mac 0x336990;
	virtual void powerOnObject() = mac 0x3369a0;
	virtual void powerOffObject() = mac 0x3369c0;
	virtual void setObjectColor(cocos2d::_ccColor3B const&) = mac 0x341c90;
	virtual void setGlowColor(cocos2d::_ccColor3B const&) = mac 0x341ed0;
	virtual void getOrientedBox() = mac 0x342ad0;
	virtual void addToGroup(int) = mac 0x33ad00;
	virtual void removeFromGroup(int) = mac 0x33ada0;
	virtual void spawnXPosition() = mac 0xdc1b0;
	virtual void getObjectRectDirty() const = mac 0xdc1d0;
	virtual void setObjectRectDirty(bool) = mac 0xdc1e0;
	virtual void getOrientedRectDirty() const = mac 0xdc1f0;
	virtual void setOrientedRectDirty(bool) = mac 0xdc200;
	virtual GameObjectType getType() const = mac 0xdc210;
	virtual void setType(GameObjectType) = mac 0xdc220;
	virtual void getStartPos() const = mac 0xdc230;

	void activatedByPlayer(GameObject*) = mac 0x342a20;
	void addColorSprite() = mac 0x2f7fe0;
	void addColorSpriteToParent(bool) = mac 0x2fb470;
	void addToTempOffset(float, float) = mac 0x335700;
	void calculateOrientedBox() = mac 0x342b20;
	void canChangeCustomColor() = mac 0x342db0;
	void colorForMode(int, bool) = mac 0x343460;
	void commonSetup() = mac 0x2f5570;
	void copyGroups(GameObject*) = mac 0x33ae30;
	static GameObject* createWithFrame(char const*) = mac 0x2f5490;
	static GameObject* createWithKey(int) = mac 0x2f4ce0;
	void destroyObject() = mac 0x336a00;
	void determineSlopeDirection() = mac 0x33a9e0;
	void getActiveColorForMode(int, bool) = mac 0x343860;
	void getBallFrame(int) = mac 0x341bf0;
	void getBoxOffset() = mac 0x3353d0;
	const cocos2d::_ccColor3B& getColorIndex() = mac 0x343b90;
	void getDidUpdateLastPosition() = mac 0x343a20;
	void getLastPosition() = mac 0x3439d0;
	void getMainColorMode() = mac 0x334c30;
	void getObjectZOrder() = mac 0x337d70;
	float getObjectRadius() = mac 0x343c10;
	void getSecondaryColorMode() = mac 0x341c20;
	void getSectionIdx() = mac 0x343a00;
	void groupWasDisabled() = mac 0x33b110;
	void groupWasEnabled() = mac 0x33b0f0;
	void hasBeenActivated() = mac 0x342a80;
	void hasBeenActivatedByPlayer(GameObject*) = mac 0x342a50;
	void hasSecondaryColor() = mac 0x342f80;
	void ignoreEnter() = mac 0x3352a0;
	void ignoreFade() = mac 0x335290;
	void isBasicTrigger() = mac 0x343d10;
	void isColorTrigger() = mac 0x343b40;
	void isSpawnableTrigger() = mac 0x343a60;
	void isSpecialObject() = mac 0x343c40;
	static GameObject* objectFromString(gd::string, bool) = mac 0x33b720;
	void playShineEffect() = mac 0x2fa9d0;
	void quickUpdatePosition() = mac 0x335790;
	void removeGlow() = mac 0x2f7f70;
	void resetGroupDisabled() = mac 0x2fa7e0;
	void saveActiveColors() = mac 0x33d250;
	void selectObject(cocos2d::_ccColor3B) = mac 0x341f90;
	void setDefaultMainColorMode(int) = mac 0x304fc0;
	void setDidUpdateLastPosition(bool const&) = mac 0x343a30;
	void setGlowOpacity(unsigned char) = mac 0x336200;
	void setLastPosition(cocos2d::CCPoint const&) = mac 0x3439e0;
	void setMainColorMode(int) = mac 0x342e70;
	void setSectionIdx(int const&) = mac 0x343a10;
	void setupCoinArt() = mac 0x337dd0;
	void slopeFloorTop() = mac 0x342800;
	void slopeWallLeft() = mac 0x3427e0;
	void updateCustomScale(float) = mac 0x335eb0;
	void updateMainColor() = mac 0x343340;
	void updateOrientedBox() = mac 0x342b50;
	void updateSecondaryColor() = mac 0x343740;
	void updateStartPos() = mac 0x2fa590;
	void updateState() = mac 0x3369e0;
	void updateSyncedAnimation(float) = mac 0x337f00;

	// float m_float250;
 //    float m_float254;
 //    float m_float258;
 //    float m_float25c;
 //    bool m_bool260;
 //    float m_animationSpeed;
 //    bool m_isAnimatedCircle;
 //    bool m_randomizeAnimationStart;
 //    float m_animationSpeed2; //what
 //    bool m_black;
 //    bool m_showSelection;
 //    bool m_blackOpacity;
 //    bool m_bool278;
 //    bool m_inEditorMode;
 //    bool m_toggledOff;
 //    bool m_colorOnTop;
 //    int m_int280;
 //    float m_float284;
 //    float m_relativeMovePos;
 //    float m_relativeRotation;
 //    bool m_isTrigger;
 //    bool m_flippedX;
 //    bool m_flippedY;
 //    cocos2d::CCSize m_origHitboxOffset;
 //    char m_unused0;
 //    cocos2d::CCSize m_adjustedHitboxOffset;
 //    OBB2D* m_orientedBox;
 //    bool m_useOrientedBox;
 //    cocos2d::CCSprite* m_glowSprite;
 //    bool m_inPlaylayer;
 //    cocos2d::CCAction* m_myAction; // rob named it not me
 //    bool m_dontRunAction;
 //    bool m_runActionOnObject;
 //    bool m_poweredOn;
 //    cocos2d::CCSize m_objectSize;
 //    bool m_modifierBlock;
 //    bool m_active;
 //    bool m_animationFinished;
 //    cocos2d::CCParticleSystemQuad* m_particleSystem;
 //    gd::string m_effectPlist;
 //    bool m_particleAdded;
 //    bool m_hasParticles;
 //    bool m_customRing;
 //    bool m_portalPosition;
 //    bool m_pulseWithMusic;
 //    cocos2d::CCRect m_textureRect;
 //    bool m_textureRectDirty;
 //    float m_textureRectCenterX;
 //    cocos2d::CCRect m_objectRect;
 //    bool m_objectRectDirty;
 //    bool m_orientdBoxDirty;
 //    bool m_activated;
 //    bool m_activatedByP2;
 //    bool m_hasAnimatedPart;
 //    int m_linkedGroup;
 //    bool m_isSpinning;
 //    float m_rotationSpeed;
 //    bool m_disableRotation;
 //    cocos2d::CCSprite* m_animatedPart;
 //    bool m_unknown358;
 //    float m_hitboxScale;
 //    bool m_onSide;
 //    cocos2d::CCSize m_unknown364;
 //    int m_uuid;
 //    GameObjectType m_objectType;
 //    int m_section;
 //    bool m_touchTriggered;
 //    bool m_spawnTriggered;
 //    cocos2d::CCPoint m_startPosition;
 //    gd::string m_textureName; //mac 0x388


	// // mac 0x250
	// bool m_unk3;
	// bool m_isBlueMaybe;
	// float m_unk2;
	// float m_unk;
	// float m_unk5;
	// // mac 0x260
	// float m_unk4;
	// bool m_unk6;
	// float m_animSpeed2;
	// bool m_isEffectObject;
	// bool m_randomisedAnimStart;
	// float m_animSpeed;
	// // mac 0x270
	// bool m_blackChild;
	// bool m_unkOutlineMaybe;
	// float m_blackChildOpacity;
	// bool m_unk8;
	// bool m_inEditLayer;
	// bool m_groupDisabled;
	// bool m_colourOnTop;
	// // mac 0x27c
	// int m_mainColorID;
	// int m_secondaryColorID;

	// // mac 0x284
	// bool m_col1;
	// bool m_col2;

	// float m_tempXOffset;
	// float m_tempYOffset;
	// // mac 0x290
	// float m_unkRotationField;
	// bool m_changeXOffset;
	// bool m_isFlippedX;
	// bool m_isFlippedY;
	// // mac 0x298
	// cocos2d::CCPoint m_boxOffset; 
	// bool m_isOriented;
	// cocos2d::CCPoint m_boxOffset2;
	// // mac 0x2b0
	// OBB2D* m_hitbox;
	// bool m_oriented;
	// // mac 0x2c0
	// cocos2d::CCSprite* m_glowSprite;
	// bool m_notEditor;
	// // mac 0x2d0
	// cocos2d::CCAction* m_myAction;
	// bool m_unk7;
	// bool m_runActionWithTag;
	// bool m_objectPoweredOn;
	// // mac 0x2e0
	// cocos2d::CCSize m_objectSize;
	// // mac 0x2f0
	// bool m_trigger;	// mac 0x270
	// bool m_active; 	// mac 0x271
	// bool m_animationFinished;	// mac 0x272
	// cocos2d::CCParticleSystemQuad* m_particleSystem;	// mac 0x274
	// std::string m_effectPlistName; // mac 0x278
	// bool m_particleAdded;	// mac 0x290
	// bool m_hasParticles;	// mac 0x291
	// bool m_unkCustomRing;	// mac 0x292
	// cocos2d::CCPoint m_portalPosition;	// mac 0x294
	// bool m_unkParticleSystem;	// mac 0x29c
	// cocos2d::CCRect m_objectTextureRect;	// mac 0x2a0
	// bool m_textureRectDirty;	// mac 0x2b0
	// float m_rectXCenterMaybe;	// mac 0x2b4
	// cocos2d::CCRect m_objectRect2; //mac 0x2B8
	// bool m_isObjectRectDirty; //mac 0x2C8
	// bool m_isOrientedRectDirty; //mac 0x2C9
	// bool m_hasBeenActivated; //mac 0x2CA
	// bool m_hasBeenActivatedP2; //mac 0x2CB

	int m_type = mac 0x370;
	int m_id = mac 0x3c4;
	cocos2d::CCPoint m_startPos = mac 0x37c;
	bool m_touchTriggered = mac 0x378;
	bool m_spawnTriggered = mac 0x379;
	int m_uuid = mac 0x36c;
	int m_colorID = mac 0x3bc;
	int m_zOrder = mac 0x42c;
	int m_unknownType = mac 0x3d4;
	int m_coinID = mac 0x3e8;
	float m_scale = mac 0x3c0;
	float m_multiScaleMultiplier = mac 0x44c;
}

class GameObjectCopy {
	virtual ~GameObjectCopy() = mac 0xa3290;

	static GameObjectCopy* create(GameObject*) = mac 0x975a0;
	void resetObject() = mac 0x976a0;
}

class GameSoundManager {
	void disableMetering() = mac 0x362d80;
	void enableMetering() = mac 0x362d00;
	void getMeteringValue() = mac 0x362db0;
	void playBackgroundMusic(gd::string, bool, bool) = mac 0x362070;
	void playEffect(gd::string, float, float, float) = mac 0x3623d0;
	void sharedManager() = mac 0x3610f0;
	void stopBackgroundMusic() = mac 0x362130;
	~GameSoundManager() = mac 0x362c00;
}

class GameStatsManager {
	void awardCurrencyForLevel(GJGameLevel*) = mac 0x43600;
	void awardDiamondsForLevel(GJGameLevel*) = mac 0x43c60;
	void awardSecretKey() = mac 0x4b1e0;
	void getSecretCoinKey(char const*) = mac 0x429f0;
	void getStat(char const*) = mac 0x3d310;
	void hasPendingUserCoin(char const*) = mac 0x42730;
	void hasSecretCoin(char const*) = mac 0x40730;
	void hasUserCoin(char const*) = mac 0x427e0;
	void incrementStat(char const*) = mac 0x3d6d0;
	void incrementStat(char const*, int) = mac 0x3d6e0;
	void sharedState() = mac 0x38f20;
	void storePendingUserCoin(char const*) = mac 0x42940;
	void storeSecretCoin(char const*) = mac 0x42a10;
	void storeUserCoin(char const*) = mac 0x42890;
}

class GJGarageLayer {
	void onPlayerColor1(cocos2d::CCObject*) = mac 0x1ba640, , ios 0x22531c;
	void onPlayerColor2(cocos2d::CCObject*) = mac 0x1ba8c0, , ios 0x225408;

	CCTextInputNode* m_nameInput;  
	SimplePlayer* m_playerPreview; 
	PAD = win 0x8;
	cocos2d::CCSprite* m_unkSprite0x140; 
	cocos2d::CCSprite* m_unkSprite0x144; 
	CCMenuItemSpriteExtra* m_unkButton0x148; 
	CCMenuItemSpriteExtra* m_unkButton0x14c; 
	PAD = win 0x8;
	cocos2d::CCArray* m_pagesArray;    
	PAD = win 0x8;
	CCMenuItemToggler* m_tabToggleCube;    
	CCMenuItemToggler* m_tabToggleShip;    
	CCMenuItemToggler* m_tabToggleBall;    
	CCMenuItemToggler* m_tabToggleUfo;     
	CCMenuItemToggler* m_tabToggleWave;    
	CCMenuItemToggler* m_tabToggleRobot;   
	CCMenuItemToggler* m_tabToggleSpider;  
	CCMenuItemToggler* m_tabToggleSpecial; 
	CCMenuItemToggler* m_tabToggleDeathEffect; 
	PAD = win 0x4;
	bool m_updateSelector;
}

class HardStreak : cocos2d::CCDrawNode {
	virtual ~HardStreak() = mac 0x5bf00;
	virtual bool init() = mac 0x5c090;
	
	void addPoint(cocos2d::CCPoint) = mac 0x5c950;
	void reset() = mac 0x5c930;
	void resumeStroke() = mac 0x5c210;
	void stopStroke() = mac 0x5c8f0;

	cocos2d::CCArray* m_pointsArr; // mac 0x0160
	cocos2d::CCPoint m_currentPoint; // mac 0x0168
	float m_waveSize; // mac 0x0170
	float m_pulseSize; // mac 0x0174
	bool m_isSolid; // mac 0x0178
}

class InheritanceNode : cocos2d::CCObject {

}

class LabelGameObject : GameObject {
	bool init() = mac 0x2f5520;
	void setObjectColor(cocos2d::_ccColor3B const&) = mac 0xdbca0;
}

class LevelEditorLayer : GJBaseGameLayer {
	virtual ~LevelEditorLayer() = mac 0x90e80;
	virtual void update(float) = mac 0xa1b70;
	virtual void draw() = mac 0xa2a70;
	virtual void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*) = mac 0x9c200;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0xa04e0;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0x9c590;
	virtual void addToGroup(GameObject*, int, bool) = mac 0x9dab0;
	virtual void removeFromGroup(GameObject*, int) = mac 0x9db60;
	virtual void timeForXPos(float) = mac 0x9c7d0;
	virtual void xPosForTime(float) = mac 0x9c800;
	virtual void levelSettingsUpdated() = mac 0x93f30;

	void activateTriggerEffect(EffectGameObject*, float, float, float) = mac 0x9b520;
	GameObject* addObjectFromString(gd::string) = mac 0x94640;
	void addSpecial(GameObject*) = mac 0x94f30;
	void addToRedoList(UndoObject*) = mac 0x96f80;
	void addToUndoList(UndoObject*, bool) = mac 0x94e20;
	void animateInDualGround(GameObject*, float, bool) = mac 0xa2780;
	void checkCollisions(PlayerObject*, float) = mac 0x9e620;
	static LevelEditorLayer* create(GJGameLevel*) = mac 0x90fb0;
	void createBackground() = mac 0x929f0;
	void createGroundLayer() = mac 0x92840;
	GameObject* createObject(int, cocos2d::CCPoint, bool) = mac 0x957c0;
	void createObjectsFromSetup(gd::string) = mac 0x92230;
	void createObjectsFromString(gd::string, bool) = mac 0x94730;
	void getLastObjectX() = mac 0x9c860;
	void getLevelString() = mac 0x97790;
	void getNextColorChannel() = mac 0x9a610;
	void getNextFreeBlockID(cocos2d::CCArray*) = mac 0x9a4e0;
	int getNextFreeGroupID(cocos2d::CCArray*) = mac 0x9a1b0;
	void getNextFreeItemID(cocos2d::CCArray*) = mac 0x9a390;
	void getObjectRect(GameObject*, bool) = mac 0x96240;
	void getRelativeOffset(GameObject*) = mac 0x96840;
	void handleAction(bool, cocos2d::CCArray*) = mac 0x97020;
	bool init(GJGameLevel*) = mac 0x91010;
	void objectAtPosition(cocos2d::CCPoint) = mac 0x960c0;
	void objectMoved(GameObject*) = mac 0x999f0;
	void objectsInRect(cocos2d::CCRect, bool) = mac 0x95e60;
	void onPlaytest() = mac 0xa06b0;
	void onStopPlaytest() = mac 0xa1780;
	void playMusic() = mac 0xa13c0;
	void recreateGroups() = mac 0x9dbf0;
	void redoLastAction() = mac 0x97750;
	void removeAllObjects() = mac 0x93d80;
	void removeAllObjectsOfType(int) = mac 0x96d40;
	void removeObject(GameObject*, bool) = mac 0x96890;
	void removeSpecial(GameObject*) = mac 0x969c0;
	void resetMovingObjects() = mac 0x9ddc0;
	void resetObjectVector() = mac 0x9c4b0;
	void resetToggledGroups() = mac 0x9aa70;
	void resetToggledGroupsAndObjects() = mac 0x9c3c0;
	void resetUnusedColorChannels() = mac 0x9a870;
	void rotationForSlopeNearObject(GameObject*) = mac 0x95cd0;
	void runColorEffect(EffectGameObject*, int, float, float, bool) = mac 0x9bd30;
	void scene(GJGameLevel*) = mac 0x90f20;
	void setupLevelStart(LevelSettingsObject*) = mac 0xa0ca0;
	void sortStickyGroups() = mac 0x92b10;
	void stopTriggersInGroup(int, float) = mac 0x9c030;
	void toggleDualMode(GameObject*, bool, PlayerObject*, bool) = mac 0xa0200;
	void toggleGroupPreview(int, bool) = mac 0x9bea0;
	void transferDefaultColors(GJEffectManager*, GJEffectManager*) = mac 0x9ab50;
	void undoLastAction() = mac 0x97770;
	void updateBGAndGColors() = mac 0x9b9b0;
	void updateBlendValues() = mac 0x9bc60;
	void updateDualGround(PlayerObject*, int, bool) = mac 0xa1a60;
	void updateEditorMode() = mac 0x93b50;
	void updateGameObjectsNew() = mac 0x9adc0;
	void updateGround(float) = mac 0x93a60;
	void updateGroundWidth() = mac 0x92af0;
	void updateOptions() = mac 0x91ed0;
	void updateToggledGroups() = mac 0x9bb10;
	void updateVisibility(float) = mac 0x92c70;

	cocos2d::CCArray* m_objects = mac 0x3a0;
	EditorUI* m_editorUI = mac 0x5d8;
}

class LevelSettingsObject : cocos2d::CCNode {
	virtual ~LevelSettingsObject() = mac 0xa5650;
	virtual bool init() = mac 0xa5690;

	static LevelSettingsObject* create() = mac 0x92760;
	void objectFromDict(cocos2d::CCDictionary*) = mac 0xa5810;
	static LevelSettingsObject* objectFromString(gd::string) = mac 0x945a0;
	void setupColorsFromLegacyMode(cocos2d::CCDictionary*) = mac 0xa6a30;

	GJEffectManager* m_effectManager = mac 0x120;
	PAD = win 0x9;
	bool m_isTwoPlayer = mac 0x132;
	PAD = win 0x10;
	int m_fontType = mac 0x144;
	PAD = win 0x8;
	GJGameLevel* m_level = mac 0x150;
	gd::string m_unknownStr;
}

class LocalLevelManager : cocos2d::CCNode {
	static LocalLevelManager* sharedState() = mac 0x35dd60;

    PAD = win 0x1C;
    cocos2d::CCDictionary* m_loadData; 
    cocos2d::CCDictionary* m_levelData; 
    cocos2d::CCArray* m_localLevels;
}

class MenuGameLayer {
	void resetPlayer() = mac 0x28fdc0;
	void update(float) = mac 0x28fa70;
}

class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol {
	virtual ~MenuLayer() = mac 0x1d1230;
	virtual bool init() = mac 0x1d14b0, win 0x1907b0;
	virtual void keyBackClicked() = mac 0x1d3160, win 0x191f90;
	virtual void keyDown(cocos2d::enumKeyCodes) = mac 0x1d33d0, win 0x1922c0;
	virtual void googlePlaySignedIn() = mac 0x1d2f30;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = mac 0x1d3190;
	void onMoreGames(cocos2d::CCObject*) = win 0x1919c0, ios 0x19fbb4;
}

class MenuLayer : cocos2d::CCLayer, FLAlertLayerProtocol {
	onGarage(cocos2d::CCObject*);
    void onMoreGames(cocos2d::CCObject*) = mac 0x1d2ad0;
    void onQuit(cocos2d::CCObject*) = mac 0x1d2b40;
    static cocos2d::CCScene* scene(bool) = mac 0x1d12d0, win 0x190720;
}

class MoreVideoOptionsLayer : FLAlertLayer {
	static MoreVideoOptionsLayer* create() = mac 0x443c10;
	bool init() = mac 0x444150;
}

class VideoOptionsLayer : FLAlertLayer {
	PAD = win 0x18;
	cocos2d::CCArray* m_resolutions;   
	PAD = win 0x4;
	int m_currentResolution;   
}

class MusicDownloadManager {
	void incrementPriorityForSong(int) = mac 0x2ef750;
	static MusicDownloadManager* sharedState() = mac 0x2ee4c0;

	PAD = win 0x4;
	cocos2d::CCDictionary* m_unknownDict;
	cocos2d::CCArray* m_handlers; 
	cocos2d::CCDictionary* m_songsDict;
}

class OBB2D : cocos2d::CCNode  {
	void calculateWithCenter(cocos2d::CCPoint, float, float, float) = mac 0x35a9c0;
	static OBB2D* create(cocos2d::CCPoint, float, float, float) = mac 0x35a890;
	cocos2d::CCRect getBoundingRect() = mac 0x35b2b0;
	void overlaps(OBB2D*) = mac 0x35b0a0;
	void overlaps1Way(OBB2D*) = mac 0x35b0d0;

	cocos2d::CCPoint m_p1_1;
	cocos2d::CCPoint m_p1_2;
	cocos2d::CCPoint m_p1_3;
	cocos2d::CCPoint m_p1_4;
	cocos2d::CCPoint m_p2_1;
	cocos2d::CCPoint m_p2_2;
	cocos2d::CCPoint m_p2_3;
	cocos2d::CCPoint m_p2_4;
	cocos2d::CCPoint m_p3_1;
	cocos2d::CCPoint m_p3_2;
	cocos2d::CCPoint m_p3_3;
	cocos2d::CCPoint m_p3_4;
	double m_rot1;
	double m_rot2;
	cocos2d::CCPoint m_center;
}

class ObjectToolbox {
	bool init() = mac 0x3b2d80;
	cocos2d::CCArray* allKeys() {
	    return m_frameToKeyDict->allKeys();
	}
	const char* frameToKey(const char* frame) {
	    return reinterpret_cast<cocos2d::CCString*>(m_frameToKeyDict->objectForKey(frame))->getCString();
	}
	const char* intKeyToFrame(int key) {
	    return reinterpret_cast<cocos2d::CCString*>(m_feyToFrameDict->objectForKey(key))->getCString();
	}
	const char* keyToFrame(const char* key) {
	    return intKeyToFrame(atoi(key));
	}
	static ObjectToolbox* sharedState() = mac 0x3b2bc0;

	cocos2d::CCDictionary* m_strKeyObjects = mac 0x120;
	cocos2d::CCDictionary* m_intKeyObjects = mac 0x128;
}

class OpacityEffectAction : cocos2d::CCNode {
	static OpacityEffectAction* create(float, float, float, int) = mac 0x1789f0;
	static OpacityEffectAction* createFromString(gd::string) = mac 0x178c10;
	bool init(float, float, float, int) = mac 0x178b00;
	void step(float) = mac 0x178b90;

	float m_time;
	float m_beginOpacity;
	float m_endOpacity;
	bool m_finished;
	float m_elapsed;
	int m_group;
	float m_opacity;
	int m_uuid;
	float m_delta;
}

class PauseLayer : CCBlockLayer {
	static PauseLayer* create(bool) = mac 0x20b1e0;
	void onEdit(cocos2d::CCObject*) = mac 0x20c630;

	bool m_unknown;
	bool m_unknown2;
}

class PlatformToolbox {
	static void hideCursor() = mac 0x27c340;
	static void showCursor() = mac 0x27c360;
	static bool isControllerConnected() = mac 0x27d1b0;
}

class PlayLayer : GJBaseGameLayer, CCCircleWaveDelegate {
	void addCircle(CCCircleWave*) = mac 0x7e0f0;
	void addObject(GameObject*) = mac 0x70e50;
	void addToGroupOld(GameObject*) = mac 0x77680;
	void addToSpeedObjects(GameObject*) = mac 0x7cfc0;
	void animateInDualGround(GameObject*, float, bool) = mac 0x7d710;
	void animateInGround(bool) = mac 0x7d9d0;
	void animateOutGround(bool) = mac 0x6f350;
	void animateOutGroundFinished() = mac 0x7de80;
	void applyEnterEffect(GameObject*) = mac 0x7c310;
	virtual void calculateColorValues(EffectGameObject*, EffectGameObject*, int, float, ColorActionSprite*, GJEffectManager*) = mac 0x7aa10;
	void cameraMoveX(float, float, float) = mac 0x7cbe0;
	void cameraMoveY(float, float, float) = mac 0x7cc60;
	void checkCollisions(PlayerObject*, float) = mac 0x78c90;
	void circleWaveWillBeRemoved(CCCircleWave*) = mac 0x7e110;
	void claimParticle(gd::string) = mac 0x76ba0;
	void clearPickedUpItems() = mac 0x7cfa0;
	void colorObject(int, cocos2d::_ccColor3B) = mac 0x77810;
	void commitJumps() = mac 0x737e0;
	static PlayLayer* create(GJGameLevel*) = mac 0x6b590;
	void createCheckpoint() = mac 0x7e470;
	void createObjectsFromSetup(gd::string) = mac 0x6d130;
	void createParticle(int, char const*, int, cocos2d::tCCPositionType) = mac 0x76800;
	void currencyWillExit(CurrencyRewardLayer*) = mac 0x7e070;
	void delayedResetLevel() = mac 0x7e050;
	void destroyPlayer(PlayerObject*, GameObject*) = mac 0x7ab80;
	void dialogClosed(DialogLayer*) = mac 0x7e0b0;
	virtual void draw() = mac 0x7d160;
	void enterDualMode(GameObject*, bool) = mac 0x7d6a0;
	void exitAirMode() = mac 0x7dd40;
	void exitBirdMode(PlayerObject*) = mac 0x7dd80;
	void exitDartMode(PlayerObject*) = mac 0x7ddd0;
	void exitFlyMode(PlayerObject*) = mac 0x7dcf0;
	void exitRobotMode(PlayerObject*) = mac 0x7de20;
	void exitRollMode(PlayerObject*) = mac 0x7de60;
	void exitSpiderMode(PlayerObject*) = mac 0x7de40;
	void flipFinished() = mac 0x7e150;
	virtual void flipGravity(PlayerObject*, bool, bool) = mac 0x7cd10;
	void flipObjects() = mac 0x76130;
	void fullReset() = mac 0x7f8e0;
	void getLastCheckpoint() = mac 0x7f840;
	void getMaxPortalY() = mac 0x7b4e0;
	void getMinPortalY() = mac 0x7b550;
	void getObjectsState() = mac 0x7e9d0;
	void getOtherPlayer(PlayerObject*) = mac 0x7dcc0;
	void getParticleKey(int, char const*, int, cocos2d::tCCPositionType) = mac 0x764d0;
	void getParticleKey2(gd::string) = mac 0x767b0;
	void getRelativeMod(cocos2d::CCPoint, float, float, float) = mac 0x7c2a0;
	void getTempMilliTime() = mac 0x778e0;
	void gravityEffectFinished() = mac 0x7cec0;
	void hasItem(int) = mac 0x7cee0;
	void hasUniqueCoin(GameObject*) = mac 0x77510;
	void incrementJumps() = mac 0x7ff40;
	bool init(GJGameLevel*) = mac 0x6b5f0;
	void isFlipping() = mac 0x76100;
	void levelComplete() = mac 0x72b80;
	void lightningFlash(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::_ccColor3B, float, float, int, bool, float) = mac 0x75cc0;
	void lightningFlash(cocos2d::CCPoint, cocos2d::_ccColor3B) = mac 0x75bf0;
	void loadDefaultColors() = mac 0x6ef30;
	void loadFromCheckpoint(CheckpointObject*) = mac 0x7f000;
	void loadLastCheckpoint() = mac 0x7efc0;
	void loadSavedObjectsState(gd::string) = mac 0x7f3d0;
	void markCheckpoint() = mac 0x7ef60;
	void moveCameraToPos(cocos2d::CCPoint) = mac 0x7c980;
	virtual void onEnterTransitionDidFinish() = mac 0x806e0;
	virtual void onExit() = mac 0x80710;
	void onQuit() = mac 0x72710;
	void optimizeColorGroups() = mac 0x6dad0;
	void optimizeOpacityGroups() = mac 0x6dc20;
	void optimizeSaveRequiredGroups() = mac 0x6dd70;
	void pauseGame(bool) = mac 0x802d0;
	void pickupItem(GameObject*) = mac 0x7c1d0;
	void playAnimationCommand(int, int) = mac 0x75930;
	void playEndAnimationToPos(cocos2d::CCPoint) = mac 0x759a0;
	void playExitDualEffect(PlayerObject*) = mac 0x7d1d0;
	void playFlashEffect(float, int, float) = mac 0x75e50;
	void playGravityEffect(bool) = mac 0x7b5a0;
	void playSpeedParticle(float) = mac 0x77030;
	void playerWillSwitchMode(PlayerObject*, GameObject*) = mac 0x7b820;
	void prepareSpawnObjects() = mac 0x7fc00;
	void processItems() = mac 0x735c0;
	void processLoadedMoveActions() = mac 0x7a7c0;
	void recordAction(bool, PlayerObject*) = mac 0x7e190;
	void registerActiveObject(GameObject*) = mac 0x77620;
	void registerStateObject(GameObject*) = mac 0x777b0;
	void removeAllObjects() = mac 0x727b0;
	void removeFromGroupOld(GameObject*) = mac 0x77750;
	void removeLastCheckpoint() = mac 0x7f870;
	void removePlayer2() = mac 0x7d630;
	void resetLevel() = mac 0x71c50;
	void resume() = mac 0x80480;
	void resumeAndRestart() = mac 0x80400;
	void saveRecordAction(bool, PlayerObject*) = mac 0x78750;
	void scene(GJGameLevel*) = mac 0x6b500;
	void setupLevelStart(LevelSettingsObject*) = mac 0x6f560;
	void setupReplay(gd::string) = mac 0x7e1e0;
	void shakeCamera(float, float, float) = mac 0x744a0;
	void shouldBlend(int) = mac 0x771b0;
	void showCompleteEffect() = mac 0x738e0;
	void showCompleteText() = mac 0x73be0;
	void showEndLayer() = mac 0x74450;
	void showHint() = mac 0x7deb0;
	void showNewBest(bool, int, int, bool, bool, bool) = mac 0x74580;
	void showRetryLayer() = mac 0x75ba0;
	void showTwoPlayerGuide() = mac 0x6df00;
	void sortGroups() = mac 0x6d9e0;
	void spawnCircle() = mac 0x73820;
	void spawnFirework() = mac 0x74200;
	void spawnParticle(char const*, int, cocos2d::tCCPositionType, cocos2d::CCPoint) = mac 0x76330;
	void spawnPlayer2() = mac 0x7d170;
	void startGame() = mac 0x726b0;
	void startMusic() = mac 0x72910;
	void startRecording() = mac 0x7fec0;
	void startRecordingDelayed() = mac 0x7fed0;
	void stopCameraShake() = mac 0x75900;
	void stopRecording() = mac 0x6d090;
	void storeCheckpoint(CheckpointObject*) = mac 0x7ef10;
	void switchToFlyMode(PlayerObject*, GameObject*, bool, int) = mac 0x7baf0;
	void switchToRobotMode(PlayerObject*, GameObject*, bool) = mac 0x7bc80;
	void switchToRollMode(PlayerObject*, GameObject*, bool) = mac 0x7bbe0;
	void switchToSpiderMode(PlayerObject*, GameObject*, bool) = mac 0x7bd20;
	void timeForXPos(float) = mac 0x7d120;
	void timeForXPos2(float, bool) = mac 0x293eb0;
	void toggleBGEffectVisibility(bool) = mac 0x7fe80;
	void toggleDualMode(GameObject*, bool, PlayerObject*, bool) = mac 0x7bf90;
	void toggleFlipped(bool, bool) = mac 0x7bdc0;
	void toggleGhostEffect(int) = mac 0x7fe40;
	void toggleGlitter(bool) = mac 0x70e00;
	void togglePracticeMode(bool) = mac 0x7f9e0;
	void toggleProgressbar() = mac 0x6eeb0;
	void tryStartRecord() = mac 0x7fe00;
	void unclaimParticle(char const*, cocos2d::CCParticleSystemQuad*) = mac 0x76e00;
	void unregisterActiveObject(GameObject*) = mac 0x77660;
	void unregisterStateObject(GameObject*) = mac 0x777f0;
	virtual void update(float) = mac 0x77900;
	void updateAttempts() = mac 0x7fcd0;
	void updateCamera(float) = mac 0x6e2b0;
	virtual void updateColor(cocos2d::_ccColor3B, float, int, bool, float, cocos2d::_ccHSVValue, int, bool, int, EffectGameObject*) = mac 0x7c7f0;
	void updateDualGround(PlayerObject*, int, bool) = mac 0x7caa0;
	void updateEffectPositions() = mac 0x7a6d0;
	void updateLevelColors() = mac 0x6f1e0;
	void updateMoveObjectsLastPosition() = mac 0x7a720;
	void updateProgressbar() = mac 0x6ed70;
	void updateReplay(float) = mac 0x78b60;
	void updateTimeMod(float, bool) = mac 0x786f0;
	virtual void updateTweenAction(float, char const*) = mac 0x7ffb0;
	void updateVisibility() = mac 0x6fb90;
	void vfDChk() = mac 0x7fcb0;
	virtual void visit() = mac 0x75ef0;
	void visitWithColorFlash() = mac 0x761f0;
	void willSwitchToMode(int, PlayerObject*) = mac 0x7b9e0;
	void xPosForTime(float) = mac 0x7d140;
	~PlayLayer() = mac 0x6b090;

	bool m_gameStarted = mac 0x4dc;
	EndPortalObject* m_endPortal = mac 0x530;
	float m_length = mac 0x5f8;
	float m_trueLength = mac 0x5fc;
	UILayer* m_uiLayer = mac 0x720;
	GJGameLevel* m_level = mac 0x728;
	int m_attempt = mac 0x754;
	bool m_testMode = mac 0x738;
	bool m_practiceMode = mac 0x739;
	float m_time = mac 0x760;
	bool m_cube = mac 0x76f;
	bool m_ship = mac 0x770;
	bool m_ufo = mac 0x771;
	bool m_ball = mac 0x772;
	bool m_wave = mac 0x773;
	bool m_robot = mac 0x774;
	bool m_spider = mac 0x775;
}

class PlayerCheckpoint {
	virtual ~PlayerCheckpoint() = mac 0x80760;
	virtual bool init() = mac 0x807a0;

	static PlayerCheckpoint* create() = mac 0x7e8c0;

	cocos2d::CCPoint m_position;
	float m_yAccel; 
	bool m_isUpsideDown; 
	bool m_isShip; 
	bool m_isBall; 
	bool m_isUFO; 
	bool m_isWave; 
	bool m_isRobot; 
	bool m_isSpider; 
	bool m_isOnGround; 
	int m_hasGhostTrail; 
	bool m_small; 
	float m_speed; 
	bool m_hidden; 
}

class PlayerObject : GameObject, AnimatedSpriteDelegate {
	virtual ~PlayerObject() = mac 0x217220;
	virtual void update(float) = mac 0x218bf0;
	virtual void setScaleX(float) = mac 0x22e7f0;
	virtual void setScaleY(float) = mac 0x22e830;
	virtual void setScale(float) = mac 0x22e870;
	virtual void setPosition(cocos2d::CCPoint const&) = mac 0x22c8b0;
	virtual void setVisible(bool) = mac 0x22e8b0;
	virtual void setRotation(float) = mac 0x22e6e0;
	virtual void setOpacity(unsigned char) = mac 0x22d400;
	virtual void setColor(cocos2d::_ccColor3B const&) = mac 0x22cdf0;
	virtual void setFlipX(bool) = mac 0x22e720;
	virtual void setFlipY(bool) = mac 0x22e7b0;
	virtual void resetObject() = mac 0x223170;
	virtual void getRealPosition() = mac 0x22d5f0;
	virtual void getOrientedBox() = mac 0x22dee0;
	virtual void animationFinished(char const*) = mac 0x22e9d0;

	void activateStreak() = mac 0x21aef0;
	void addAllParticles() = mac 0x2189b0;
	void addToTouchedRings(GameObject*) = mac 0x22b800;
	void boostPlayer(float) = mac 0x21d6b0;
	void bumpPlayer(float, int) = mac 0x22d890;
	void buttonDown(PlayerButton) = mac 0x22b7e0;
	void checkSnapJumpToObject(GameObject*) = mac 0x2217f0;
	void collidedWithObject(float, GameObject*) = mac 0x21d880;
	void collidedWithObject(float, GameObject*, cocos2d::CCRect) = mac 0x21f0b0;
	void collidedWithSlope(float, GameObject*, bool) = mac 0x21d8d0;
	void convertToClosestRotation(float) = mac 0x21c860;
	void copyAttributes(PlayerObject*) = mac 0x22dc70;
	static PlayerObject* create(int, int, cocos2d::CCLayer*) = mac 0x217260;
	void deactivateParticle() = mac 0x21a540;
	void deactivateStreak(bool) = mac 0x218b30;
	void fadeOutStreak2(float) = mac 0x225890;
	void flashPlayer(float, float, cocos2d::_ccColor3B, cocos2d::_ccColor3B) = mac 0x221c80;
	void flipGravity(bool, bool) = mac 0x21c090;
	void flipMod() = mac 0x21a4c0;
	void getActiveMode() = mac 0x22b950;
	void getModifiedSlopeYVel() = mac 0x21bff0;
	void getOldPosition(float) = mac 0x21a830;
	void getSecondColor() = mac 0x22cee0;
	void gravityDown() = mac 0x22e930;
	void gravityUp() = mac 0x22e900;
	void hardFlipGravity() = mac 0x22b860;
	void hitGround(bool) = mac 0x220a30;
	void incrementJumps() = mac 0x21c050;
	bool init(int, int, cocos2d::CCLayer*) = mac 0x2172e0;
	void isBoostValid(float) = mac 0x21d650;
	void isFlying() = mac 0x21a4e0;
	void isSafeFlip(float) = mac 0x2209f0;
	void isSafeMode(float) = mac 0x2209b0;
	void isSafeSpiderFlip(float) = mac 0x221be0;
	void levelFlipFinished() = mac 0x21b060;
	void levelFlipping() = mac 0x21a510;
	void levelWillFlip() = mac 0x21b020;
	void loadFromCheckpoint(PlayerCheckpoint*) = mac 0x22e420;
	void lockPlayer() = mac 0x22d680;
	void logValues() = mac 0x221220;
	void modeDidChange() = mac 0x22bfd0;
	void placeStreakPoint() = mac 0x21af90;
	void playBurstEffect() = mac 0x21c780;
	void playDeathEffect() = mac 0x225930;
	void playDynamicSpiderRun() = mac 0x222ec0;
	void playerDestroyed(bool) = mac 0x2256d0;
	void playerIsFalling() = mac 0x21c730;
	void playerTeleported() = mac 0x22b840;
	void playingEndEffect() = mac 0x22d7e0;
	void postCollision(float) = mac 0x21cd10;
	void preCollision() = mac 0x21ccc0;
	void preSlopeCollision(float, GameObject*) = mac 0x21ec80;
	void propellPlayer(float) = mac 0x22d8e0;
	void pushButton(PlayerButton) = mac 0x22aa00;
	void pushDown() = mac 0x22dbd0;
	void pushPlayer(float) = mac 0x22dbb0;
	void releaseButton(PlayerButton) = mac 0x22b6f0;
	void removeAllParticles() = mac 0x218ac0;
	void removePendingCheckpoint() = mac 0x2237b0;
	void resetAllParticles() = mac 0x21adb0;
	void resetCollisionLog() = mac 0x21cc20;
	void resetPlayerIcon() = mac 0x22be00;
	void resetStateVariables() = mac 0x223760;
	void resetStreak() = mac 0x21ae10;
	void ringJump(GameObject*) = mac 0x22abf0;
	void runBallRotation(float) = mac 0x21ca10;
	void runBallRotation2() = mac 0x21cb10;
	void runNormalRotation() = mac 0x21c960;
	void runRotateAction(bool) = mac 0x21c570;
	void saveToCheckpoint(PlayerCheckpoint*) = mac 0x22e2f0;
	void setSecondColor(cocos2d::_ccColor3B const&) = mac 0x219610;
	void setupStreak() = mac 0x218720;
	void spawnCircle() = mac 0x225480;
	void spawnCircle2() = mac 0x2252a0;
	void spawnDualCircle() = mac 0x2255c0;
	void spawnFromPlayer(PlayerObject*) = mac 0x22dde0;
	void spawnPortalCircle(cocos2d::_ccColor3B, float) = mac 0x225350;
	void spawnScaleCircle() = mac 0x2251b0;
	void specialGroundHit() = mac 0x22dbf0;
	void speedDown() = mac 0x22e970;
	void speedUp() = mac 0x22e950;
	void spiderTestJump(bool) = mac 0x21b160;
	void startDashing(GameObject*) = mac 0x221d70;
	void stopBurstEffect() = mac 0x22c680;
	void stopDashing() = mac 0x222990;
	void stopRotation(bool) = mac 0x21c830;
	void storeCollision(bool, int) = mac 0x21cc60;
	void switchedToMode(GameObjectType) = mac 0x22b9a0;
	void testForMoving(float, GameObject*) = mac 0x21eb70;
	void toggleBirdMode(bool) = mac 0x224070;
	void toggleDartMode(bool) = mac 0x2243f0;
	void toggleFlyMode(bool) = mac 0x223820;
	void toggleGhostEffect(GhostType) = mac 0x225000;
	void togglePlayerScale(bool) = mac 0x224bd0;
	void toggleRobotMode(bool) = mac 0x223c70;
	void toggleRollMode(bool) = mac 0x223b20;
	void toggleSpiderMode(bool) = mac 0x224830;
	void toggleVisibility(bool) = mac 0x21abf0;
	void touchedObject(GameObject*) = mac 0x22e660;
	void tryPlaceCheckpoint() = mac 0x21a950;
	void updateCheckpointMode(bool) = mac 0x218980;
	void updateCheckpointTest() = mac 0x21a890;
	void updateCollide(bool, int) = mac 0x220f10;
	void updateCollideBottom(float, int) = mac 0x221790;
	void updateCollideTop(float, int) = mac 0x221c20;
	void updateDashAnimation() = mac 0x21a570;
	void updateDashArt() = mac 0x222520;
	void updateGlowColor() = mac 0x22cf10;
	void updateJump(float) = mac 0x219680;
	void updateJumpVariables() = mac 0x21a740;
	void updatePlayerBirdFrame(int) = mac 0x22bfe0;
	void updatePlayerDartFrame(int) = mac 0x22c260;
	void updatePlayerFrame(int) = mac 0x22c470;
	void updatePlayerGlow() = mac 0x22bc50;
	void updatePlayerRobotFrame(int) = mac 0x22d620;
	void updatePlayerRollFrame(int) = mac 0x22c6a0;
	void updatePlayerScale() = mac 0x22b8b0;
	void updatePlayerShipFrame(int) = mac 0x22ba40;
	void updatePlayerSpiderFrame(int) = mac 0x22d650;
	void updatePlayerSpriteExtra(gd::string) = mac 0x218440;
	void updateRobotAnimationSpeed() = mac 0x22df40;
	void updateRotation(float) = mac 0x2214b0;
	void updateRotation(float, float) = mac 0x221230;
	void updateShipRotation(float) = mac 0x221310;
	void updateShipSpriteExtra(gd::string) = mac 0x218510;
	void updateSlopeRotation(float) = mac 0x221030;
	void updateSlopeYVelocity(float) = mac 0x22e920;
	void updateSpecial(float) = mac 0x21a790;
	void updateStateVariables() = mac 0x21a770;
	void updateTimeMod(float) = mac 0x2185e0;
	void usingWallLimitedMode() = mac 0x22df00;
	void yStartDown() = mac 0x22e9b0;
	void yStartUp() = mac 0x22e990;

	HardStreak* m_waveStreak = mac 0x600;
	double m_speed = mac 0x608;
	double m_gravity = mac 0x618;
	bool m_inPlayLayer = mac 0x62c;
	GJRobotSprite* m_robotSprite = mac 0x6a8;
	GJSpiderSprite* m_spiderSprite = mac 0x6b0;
	bool m_isHolding = mac 0x745;
	bool m_hasJustHeld = mac 0x746;
	double m_yAccel = mac 0x760;
	bool m_isShip = mac 0x770;
	bool m_isBird = mac 0x771;
	bool m_isBall = mac 0x772;
	bool m_isDart = mac 0x773;
	bool m_isRobot = mac 0x774;
	bool m_isSpider = mac 0x775;
	bool m_upsideDown = mac 0x776;
	bool m_dead = mac 0x777;
	bool m_onGround = mac 0x778;
	float m_vehicleSize = mac 0x77c;
	cocos2d::CCPoint m_lastPortalLocation = mac 0x78c;
	bool m_isSliding = mac 0x7a0;
	bool m_isRising = mac 0x7a1;
	cocos2d::CCPoint m_lastHitGround = mac 0x7a4;
	GameObject* m_lastPortal = mac 0x7b8;
	cocos2d::_ccColor3B m_col1 = mac 0x7c2;
	cocos2d::_ccColor3B m_col2 = mac 0x7c5;
	float m_xPos = mac 0x7c8;
	float m_yPos = mac 0x7cc;
}

class PulseEffectAction : cocos2d::CCNode {
	static PulseEffectAction* createFromString(gd::string) = mac 0x179e90;
	void getSaveString() = mac 0x17a850;
	int m_group = mac 0x130;
}

class RetryLevelLayer {
	static RetryLevelLayer* create() = mac 0x28dd60;
}

class ScrollingLayer : cocos2d::CCLayerColor {
	static ScrollingLayer* create(cocos2d::CCSize, cocos2d::CCPoint, float) = mac 0x41a900;
}

class SetGroupIDLayer {
	void onNextGroupID1(cocos2d::CCObject*) = mac 0x1967a0;
	void textChanged(CCTextInputNode*) = mac 0x197af0;
	void updateGroupIDLabel() = mac 0x197260;
	~SetGroupIDLayer() = mac 0x194410;
}

class SetIDLayer {
	static SetIDLayer* create(GameObject*) = mac 0x168f20;
}

class SetIDPopup {
}

class SetItemIDLayer {
	static SetItemIDLayer* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x5a830;
}

class SetTargetIDLayer {
	static SetTargetIDLayer* create(EffectGameObject*, cocos2d::CCArray*, gd::string) = mac 0x159d20;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x15aed0;
	void textChanged(CCTextInputNode*) = mac 0x15b6c0;
	void updateTargetID() = mac 0x15b4a0;
}

class SetupAnimationPopup : FLAlertLayer {
	static SetupAnimationPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x208b70;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x209fc0;
	void textChanged(CCTextInputNode*) = mac 0x20ab30;
	void updateTargetID() = mac 0x20a910;
}

class SetupCollisionTriggerPopup : FLAlertLayer {
	static SetupCollisionTriggerPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x1d6120;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x1d77b0;
	void textChanged(CCTextInputNode*) = mac 0x1d84d0;
	void updateTargetID() = mac 0x1d82b0;
}

class SetupCountTriggerPopup : FLAlertLayer {
	static SetupCountTriggerPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x15c6c0;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x15dd40;
	void textChanged(CCTextInputNode*) = mac 0x15e9a0;
	void updateTargetID() = mac 0x15e8a0;
}

class SetupInstantCountPopup : FLAlertLayer {
	static SetupInstantCountPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x352c10;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x354520;
	void textChanged(CCTextInputNode*) = mac 0x355270;
	void updateTargetID() = mac 0x355170;
}

class SetupInteractObjectPopup : FLAlertLayer {
	static SetupInteractObjectPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x29a400;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x29bbc0;
	void textChanged(CCTextInputNode*) = mac 0x29c2b0;
	void updateTargetID() = mac 0x29c120;
}

class SetupObjectTogglePopup : FLAlertLayer {
	static SetupObjectTogglePopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x1c0860;
	bool init(EffectGameObject*, cocos2d::CCArray*) = mac 0x1c0a40;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x1c1c40;
	void textChanged(CCTextInputNode*) = mac 0x1c2660;
	void updateTargetID() = mac 0x1c2440;
}

class SetupOpacityPopup : FLAlertLayer {
	static SetupOpacityPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x32b70;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x340a0;
	void textChanged(CCTextInputNode*) = mac 0x34a60;
	void updateTargetID() = mac 0x34760;
}

class SetupPickupTriggerPopup : FLAlertLayer {
	static SetupPickupTriggerPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x35e70;
	void onItemIDArrow(cocos2d::CCObject*) = mac 0x37100;
	void onNextItemID(cocos2d::CCObject*) = mac 0x37260;
	void textChanged(CCTextInputNode*) = mac 0x37ca0;
	void updateItemID() = mac 0x37ab0;

	PAD = win 0xc;
	CCTextInputNode* m_countInput; 
}


class DialogObject : cocos2d::CCObject {
	gd::string m_text;
	gd::string m_title;
	int m_dialogType;
	cocos2d::ccColor3B m_colour;
	float m_textWidth;
	bool m_canSkip;
}

class GJRewardObject : cocos2d::CCObject {
	SpecialRewardItem m_specialRewardItem;
	UnlockType m_unlockType;
	int m_itemID;
	int m_total;
}

class TeleportPortalObject : GameObject {
    PAD = win 0x4;
    TeleportPortalObject* m_orangePortal; 
    bool m_unk470;
    float m_unk474;
    bool m_unk478;
}

class SetupShakePopup : FLAlertLayer {
	static SetupShakePopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x3adc00;
}

class SetupSpawnPopup : FLAlertLayer {
	static SetupSpawnPopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x139790;
	void createToggleButton(gd::string, cocos2d::SEL_MenuHandler, bool, cocos2d::CCMenu*, cocos2d::CCPoint, cocos2d::CCArray*) = mac 0x13b0e0;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x13ad80;
	void textChanged(CCTextInputNode*) = mac 0x13b990;
	void updateTargetID() = mac 0x13b770;

	EffectGameObject* object = mac 0x258;
	CCTextInputNode* numInput = mac 0x290;
}

class SetupTouchTogglePopup : FLAlertLayer {
	static SetupTouchTogglePopup* create(EffectGameObject*, cocos2d::CCArray*) = mac 0x1576a0;
	void onTargetIDArrow(cocos2d::CCObject*) = mac 0x158b60;
	void textChanged(CCTextInputNode*) = mac 0x1596a0;
	void updateTargetID() = mac 0x159480;
}

class SimplePlayer {
	static SimplePlayer* create(int) = mac 0x1b6140;
	void setSecondColor(cocos2d::_ccColor3B const&) = mac 0x1bace0;
	void updateColors() = mac 0x1ba1f0;
	void updatePlayerFrame(int, IconType) = mac 0x1b62f0;
}

class SliderTouchLogic {
	SliderThumb* getThumb() const { return m_thumb; }

    PAD = win 0x4;
    float m_length;
    SliderThumb* m_thumb;
    Slider* m_slider;
    bool m_unknown;
    PAD = win 0x8;
    bool m_vertical;
}

class Slider {
	void setValue(float val) {
	    this->m_touchLogic->getThumb()->setValue(val);
	}
	float getValue() = mac 0x18e0c0, win 0x2e970;
	void setBarVisibility(bool v) {
	    this->m_sliderBar->setVisible(v);
	}
	void updateBar() = win 0x2ea10;
	static Slider* create(cocos2d::CCNode target, cocos2d::SEL_MenuHandler click, const char* grooveSpr, const char* thumbSpr, const char* thumbSprSel, float scale) = mac 0x18dd80, win 0x2e6e0;
	static Slider* create(cocos2d::CCNode target, cocos2d::SEL_MenuHandler click, float scale) {
	    return create(target, click, "slidergroove.png", "sliderthumb.png", "sliderthumbsel.png", scale);
	}

    SliderTouchLogic* m_touchLogic;
    cocos2d::CCSprite* m_sliderBar;
    cocos2d::CCSprite* m_groove;
    float m_unknown;
    float m_height;
}

class SongInfoObject : cocos2d::CCNode {
	virtual ~SongInfoObject() = mac 0x2f2040;
	virtual void encodeWithCoder(DS_Dictionary*) = mac 0x2f2c70;
	virtual bool canEncode() = mac 0x2f2da0;

    int m_songID;
    gd::string m_songName;
    gd::string m_artistName;
    gd::string m_youtubeVideo; 
    gd::string m_youtubeChannel;
    gd::string m_songURL;
    int m_artistID;
    float m_fileSize;
    bool m_isUnknownSong;
    int m_priority;
    LevelSettingsObject* m_levelSettings;
}

class SpawnTriggerAction : cocos2d::CCNode {
	static SpawnTriggerAction* createFromString(gd::string) = mac 0x17bf50;
	bool m_timerEnded;
	float m_delay;
	float m_timer;
	int m_group;
	int m_uuid;
}

class TableView : CCScrollLayerExt, CCScrollLayerExtDelegate {
	static TableView* create(TableViewDelegate*, TableViewDataSource*, cocos2d::CCRect) = mac 0x37eb30;
	void reloadData() = mac 0x37f970;

	float m_unknown = mac 0x1c8;
}

class TableViewCell : cocos2d::CCLayer {
	TableViewCell(char const*, float, float) = mac 0x383de0;
	// default constructor
	TableViewCell() {}
	
	bool m_unknown;
	TableView* m_tableView;
	CCIndexPath m_indexPath;
	gd::string m_unknownString;
	PAD = win 0x4;
	float m_parentHeight;
	float m_height;
	cocos2d::CCLayerColor* m_BGLayer;
	cocos2d::CCLayer* m_mainLayer;
	PAD = win 0x4;
}

class TableViewDataSource {
	virtual void willTweenToIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	virtual void didEndTweenToIndexPath(CCIndexPath&, TableView*) {}
	virtual void TableViewWillDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	virtual void TableViewDidDisplayCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	virtual void TableViewWillReloadCellForRowAtIndexPath(CCIndexPath&, TableViewCell*, TableView*) {}
	virtual void cellHeightForRowAtIndexPath(CCIndexPath&, TableView*) {}
	virtual void didSelectRowAtIndexPath(CCIndexPath&, TableView*) {}
}

class TableViewDelegate {
	virtual int numberOfRowsInSection(unsigned int, TableView*) {return 0;}
	virtual void numberOfSectionsInTableView(TableView*) {}
	virtual void TableViewCommitCellEditingStyleForRowAtIndexPath(TableView*, TableViewCellEditingStyle, CCIndexPath&) {}
	virtual void cellForRowAtIndexPath(CCIndexPath&, TableView*) {}
}

class TextArea : cocos2d::CCSprite {
	virtual ~TextArea() = mac 0x19faa0;
	virtual void draw() = mac 0x19f890;
	virtual void setOpacity(unsigned char) = mac 0x19f760;

	static TextArea* create(gd::string, char const*, float, float, cocos2d::CCPoint, float, bool) = mac 0x19eb40;
}

class TextInputDelegate {
	virtual void textChanged(CCTextInputNode*) {}
	virtual void textInputOpened(CCTextInputNode*) {}
	virtual void textInputClosed(CCTextInputNode*) {}
	virtual void textInputShouldOffset(CCTextInputNode*, float) {}
	virtual void textInputReturn(CCTextInputNode*) {}
	virtual bool allowTextInput(CCTextInputNode*) {return true;}
}

class ToggleTriggerAction : cocos2d::CCNode {
	static ToggleTriggerAction* createFromString(gd::string) = mac 0x1765e0;
}

class TopArtistsLayer : FLAlertLayer {
	static TopArtistsLayer* create() = mac 0x192a90;
	void setupLeaderboard(cocos2d::CCArray*) = mac 0x193420;
	virtual bool init() = mac 0x192c30;
	void loadPage(int) = mac 0x193b60;
	void setupPageInfo(gd::string, char const*) = mac 0x193730;

	cocos2d::CCNode* m_unknown = mac 0x220;
	GJCommentListLayer* m_commentLayer = mac 0x260;
}

class TouchToggleAction : cocos2d::CCNode {
	static TouchToggleAction* createFromString(gd::string) = mac 0x177e10;
}

class UILayer {
	static UILayer* create() = mac 0x27fd10;
	void disableMenu() = mac 0x280960;
	void enableMenu() = mac 0x280940;
	void pCommand(cocos2d::CCNode*) = mac 0x280830;
	void toggleCheckpointsMenu(bool) = mac 0x280430;

	PAD = win 0x8;
	cocos2d::CCMenu* m_checkPointMenu;		
}

class FMODAudioEngine : cocos2d::CCNode {
	static FMODAudioEngine* sharedEngine() = mac 0x20ef80;
	cocos2d::CCDictionary* m_dictionary;
	std::string m_filePath;
	float m_backgroundMusicVolume;
	float m_effectsVolume;
	float m_pulse1;
	float m_pulse2;
	float m_pulse3;
	int m_pulseCounter;
	bool m_metering;
	bool m_fading;
	bool m_fadeIn;
	float m_fadeInDuration;
	FMOD::System* m_system;
	FMOD::Sound* m_sound;
	FMOD::Channel* m_currentSoundChannel;
	FMOD::Channel* m_globalChannel;
	FMOD::DSP* m_DSP;
	FMOD_RESULT m_lastResult;
	int m_version;
	void* m_extraDriverData;
	int m_musicOffset;

}

class UndoObject {
	virtual ~UndoObject() = mac 0xa2fd0;

	static UndoObject* create(GameObject*, UndoCommand) = mac 0x94ea0;
	static UndoObject* createWithArray(cocos2d::CCArray*, UndoCommand) = mac 0x96ee0;
}


//
//Fod's stuff
//

class AchievementBar {
	PAD = win 0x24;
	static AchievementBar* create(const char* title, const char* desc, const char* icon, bool quest) = win 0x3b120;
}

class AchievementNotifier {
	static AchievementNotifier* sharedState() = win 0xfc90;
	void showNextAchievement() = win 0xfd60;
	void notifyAchievement(const char* title, const char* desc, const char* icon, bool quest) {
	    m_queue->addObject(AchievementBar::create(title, desc, icon, quest));
	    if (!m_currentAchievement) {
	        this->showNextAchievement();
	    }
	}
}

class CCCircleWaveDelegate {
	void circleWaveWillBeRemoved(CCCircleWave* wave) {}
}

class CCCircleWave {
	void setPosition(CCPoint const& pos) = win 0x16ed0;
	void removeMeAndCleanup() = win 0x17280;
	void draw() = win 0x17100;
	void updateTweenAction(float dt, const char* key) = win 0x16f90;
	void followObject(cocos2d::CCNode obj, bool repeat) = win 0x16f20;
	void updatePosition(float dt) = win 0x16f00;
	static CCCircleWave* create(float from, float to, float duration, bool fadeIn, bool ease) = win 0x16c00;
}

class FMODAudioEngine {
	static FMODAudioEngine* sharedEngine() = win 0x239f0;
	void preloadEffect(gd::string filename) = win 0x24240;
	bool isBackgroundMusicPlaying() = win 0x24050;
	bool isBackgroundMusicPlaying(gd::string path) = win 0x24080;
	void playBackgroundMusic(gd::string path, bool fade, bool paused) = win 0x23d80;
}

class FMODSound : cocos2d::CCNode {
	FMOD::Sound* m_sound;

	static FMODSound* create(FMOD::Sound* sound) = win 0x246b0;
}

class GJEffectManager {
	ColorAction* getColorAction(int num) = win 0x11cde0;
}

class TextAlertPopup {
	static TextAlertPopup* create(gd::string const& text, float time, float scale) = win 0x1450b0;
}

class TableViewCell {
	TableViewCell(const char* name, float width, float height) = win 0x32e70;
}

class StatsCell {
	void updateBGColor(unsigned int index) = win 0x59cf0;
	void draw() = win 0x59d40;
}

class GJComment {
	static GJComment* create(CCDictionary* dict) = win 0xc3740;
}

class CCTextInputNode {
	void visit() = win 0x21000;
	void registerWithTouchDispatcher() = win 0x220e0;
	bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) = win 0x1f20;
	void onClickTrackNode(bool unk) = win 0x216b0;
	bool init(const char* sCaption, cocos2d::CCObject* pTarget, const char* sFontFile, float fWidth, float fHeight) = win 0x20e50;
	static CCTextInputNode* create(const char* caption, cocos2d::CCObject* target, const char* fntFile, float width, float height) = win 0x20d90;
}

class GJRotationControl {
	void setAngle(float angle) {
	    this->m_sliderPosition = CCPoint { sinf(angle) * 60.0f, cosf(angle) * 60.0f };
	    this->m_angle = angle;
	
	    this->m_sliderThumb->setPosition(this->m_sliderPosition);
	}
	void updateSliderPosition(CCPoint const& pos) = win 0x94020;
	float m_unknown0;
    float m_unknown1;
    cocos2d::CCPoint m_sliderPosition;
    cocos2d::CCSprite* m_sliderThumb;
    float m_objAngle;
    float m_angle;
    int m_touchID;
}

class GJScaleControl : cocos2d::CCLayer {
	void updateLabel(float value) = win 0x94990;
	void loadValues(GameObject* obj, cocos2d::CCArray* objs) = win 0x94590;

	Slider* m_slider;
	unsigned int m_touchID;
	float m_value;
	PAD = win 0x4;
	cocos2d::CCLabelBMFont* m_label;
	GJScaleControlDelegate* m_delegate;
}

class CustomizeObjectLayer : FLAlertLayer, TextInputDelegate, HSVWidgetPopupDelegate, ColorSelectDelegate, ColorSetupDelegate {
	void onNextColorChannel(cocos2d::CCObject* pSender) = win 0x56c80;
	void onSelectColor(cocos2d::CCObject* pSender) = win 0x577b0;
	int getActiveMode(bool unknown) = win 0x57210;

    EffectGameObject* m_targetObject;  
    cocos2d::CCArray* m_targetObjects; 
    cocos2d::CCArray* m_colorButtons; 
    cocos2d::CCArray* m_colorNodes; 
    cocos2d::CCArray* m_textInputNodes; 
    PAD = win 0x4;
    cocos2d::CCArray* m_detailColorButtons; 
    int m_selectedMode;    
    int m_customColorChannel;  
    bool m_unk0x200;   
    bool m_unk0x201;   
    bool m_glowDisabled; 
    CCMenuItemSpriteExtra* m_baseButton; 
    CCMenuItemSpriteExtra* m_detailButton; 
    CCMenuItemSpriteExtra* m_textButton; 
    CCMenuItemSpriteExtra* m_baseColorHSV; 
    CCMenuItemSpriteExtra* m_detailColorHSV; 
    cocos2d::CCLabelBMFont* m_titleLabel;   
    cocos2d::CCLabelBMFont* m_selectedColorLabel;   
    CCTextInputNode* m_customColorInput;    
    CCTextInputNode* m_textInput;    
    ButtonSprite* m_customColorButtonSprite; 
    CCMenuItemSpriteExtra* m_customColorButton; 
    CCMenuItemSpriteExtra* m_arrowDown; 
    CCMenuItemSpriteExtra* m_arrowUp; 
    cocos2d::extension::CCScale9Sprite* m_customColorInputBG;
    ColorChannelSprite* m_colorSprite; 
    CCMenuItemSpriteExtra* m_colorSpriteButton; 
    bool m_showTextInput;
    bool m_customColorSelected;
}

class HSVWidgetPopup : FLAlertLayer {
	ConfigureHSVWidget* m_configureWidget;
	HSVWidgetPopupDelegate* m_delegate;
}

class GJDropDownLayer : cocos2d::CCLayerColor {
	void registerWithTouchDispatcher() = win 0x16990;
	void draw() = win 0x16a80;
	
	bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {
	    return true;
	}
	void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}
	void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}
	void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}
	void keyBackClicked() = win 0x113960;
	
	void enterLayer() = win 0x16970;
	void exitLayer(cocos2d::CCObject* btn) = win 0x113980;
	void showLayer(bool noTransition) = win 0x1139c0;
	void hideLayer(bool noTransition) = win 0x113a90;
	void layerVisible() = win 0x16a40;
	void layerHidden() = win 0x113b60;
	

	void disableUI() = win 0x113920;
	void enableUI() = win 0x113940;
	GJDropDownLayer() = win 0x038470;
	bool init(const char* title, float height) = win 0x113530;

}

class CCNodeContainer {
	bool init() = win 0x33b40;
	void visit() = win 0x112420;
	static CCNodeContainer* create() = win 0x112370;
}

class SelectArtLayer {
	static SelectArtLayer* create(SelectArtType type) = win 0x174b00;
}

class LevelSettingsLayer {
	static LevelSettingsLayer* create(LevelSettingsObject* levelSettings, LevelEditorLayer* editor) = win 0x170d90;
}

class SliderThumb : cocos2d::CCMenuItemImage {
    float m_length;
    bool m_vertical;

	void setValue(float val) = win 0x2e1b0;
	// float getValue() {
	//     float pos = this->m_vertical ?
	//         this->getPositionY() : 
	//         this->getPositionX();
	    
	//     float scale = this->getScale();
	    
	//     return (scale * this->m_length * .5f + pos) / (scale * this->m_length);
	// }
}

class InfoAlertButton {
	bool init(gd::string const& title, gd::string const& text, float scale) = win 0x14ef50;
	void activate() = win 0x14f050;
	InfoAlertButton() = win 0x14ef50;
	static InfoAlertButton* create(gd::string const& title, gd::string const& text, float scale) = win 0x14ed20;
}

class CCMenuItemSpriteExtra {
	bool init(cocos2d::CCNode spr) = win 0x18fa0;
	CCMenuItemSpriteExtra* destructor(bool rel) = win 0x18eb0;
	CCMenuItemSpriteExtra() = win 0x18db0;
	void activate() = win 0x191c0;
	void selected() = win 0x19270;
	void unselected() = win 0x19430;
	static CCMenuItemSpriteExtra* create(cocos2d::CCNode sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback) = win 0x18ee0;
	void setSizeMult(float mult) = win 0x19080;
	void useAnimationType(MenuAnimationType type) {
	    this->m_startPosition = this->getNormalImage()->getPosition();
	    this->m_animationType = type;
	}
	void setDestination(CCPoint const& pos) {
	    this->m_destPosition = pos;
	}
	void setOffset(CCPoint const& pos) {
	    this->m_offset = pos;
	}
	void setScale(float scale) {
	    this->CCMenuItemSprite::setScale(scale);
	    this->m_baseScale = scale;
	}
	float m_scaleMultiplier;	// 0x118
	float m_baseScale;			// 0x11c
	bool m_animationEnabled;	// 0x120
	bool m_colorEnabled;		// 0x121
	PAD = win 0x54;
	float m_fColorDip;			// 0x158
	cocos2d::CCPoint m_obDestPosition;	// 0x15c
	cocos2d::CCPoint m_obOffset;	// 0x164
	MenuAnimationType m_nAnimationType;	// 0x16c
	cocos2d::CCPoint m_obStartPosition;	// 0x170
}

class CCMenuItemToggler {
	bool init(cocos2d::CCNode off, cocos2d::CCNode on, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler handler) = win 0x196e0;
	void activate() = win 0x198d0;
	void selected() = win 0x198a0;
	void unselected() = win 0x19900;
	void setEnabled(bool enabled) = win 0x19930;
	static CCMenuItemToggler* createWithSize(const char* spr1, const char* spr2, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale) {
	    auto sprOff = cocos2d::CCSprite::createWithSpriteFrameName(spr1);
	    auto sprOn = cocos2d::CCSprite::createWithSpriteFrameName(spr2);
	
	    sprOff->setScale(scale);
	    sprOn->setScale(scale);
	
	    return create(sprOff, sprOn, target, callback);
	}
	static CCMenuItemToggler* createWithStandardSprites(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale) {
	    auto sprOff = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
	    auto sprOn = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
	
	    sprOff->setScale(scale);
	    sprOn->setScale(scale);
	
	    return create(sprOff, sprOn, target, callback);
	}
	void setSizeMult(float mult) = win 0x19850;
	bool isOn() {
	    return m_toggled;
	}
	bool isToggled() {
	    return m_toggled;
	}
	void setClickable(bool on) {
	    m_notClickable = !on;
	}
	void toggle(bool on) = win 0x199b0;
	void toggleWithCallback(bool on) {
	    this->activate();
	    this->toggle(on);
	}
}

class DrawGridLayer : cocos2d::CCLayer {
	void draw() = win 0x16ce90;

	inline using CCPointArray400 = cocos2d::CCPoint(*)[400];
	inline using CCPointArray200 = cocos2d::CCPoint(*)[400];

	CCPointArray400 m_commonLines; 	
	CCPointArray200 m_yellowGuidelines;  				
	CCPointArray200 m_greenGuidelines; 
	float m_songOffset1;       
	float m_songOffset2;       
	float m_lastMusicXPosition;
	bool m_effectSortDirty;    
	LevelEditorLayer* m_editor;
	gd::string m_guidelineString; 
	cocos2d::CCNode* m_grid;       
	cocos2d::CCArray* m_guidelines;
	cocos2d::CCArray* m_effects;   
	cocos2d::CCArray* m_guides;    
	cocos2d::CCArray* m_speedObjects1;     
	cocos2d::CCArray* m_speedObjects2;     
	cocos2d::CCArray* m_playerNodePoints;  
	cocos2d::CCArray* m_player2NodePoints; 
	double m_unkDouble;
	float m_guidelineSpacing;      
	float m_slowGuidelineSpacing;  
	float m_normalGuidelineSpacing;
	float m_fastGuidelineSpacing;  
	float m_fasterGuidelineSpacing;
	float m_fastestGuidelineSpacing;   
	bool m_updatingTimeMarkers;    
	bool m_timeNeedsUpdate;        
	float m_activeGridNodeSize;    
	float m_gridSize;
}

class EditButtonBar : cocos2d::CCNode {
	void loadFromItems(cocos2d::CCArray* buttons, int rowCount, int columnCount, bool idk) = mac 0x351010, win 0x6e5e0;
	void removeAllItems() {
	    this->m_buttonArray->removeAllObjects();
	    this->reloadItemsInNormalSize();
	}
	void reloadItems(int rowCount, int columnCount) {
	    if (this->m_buttonArray)
	        this->loadFromItems(this->m_buttonArray, rowCount, columnCount, this->m_unknown);
	}
	void reloadItemsInNormalSize() {
		// TODO: fix this
	    // this->reloadItems(
	    //     GameManager::sharedState()->getIntGameVariable("0049"),
	    //     GameManager::sharedState()->getIntGameVariable("0050")
	    // );
	}
	void addButton(CCMenuItemSpriteExtra* btn, bool reload) {
	    if (this->m_buttonArray)
	        this->m_buttonArray->addObject(reinterpret_cast<cocos2d::CCObject*>(btn));
	    if (reload)
	        this->reloadItemsInNormalSize();
	}

	cocos2d::CCPoint m_position;
	int m_unknown;
	bool m_unknownBool;
	cocos2d::CCArray* m_buttonArray;
	BoomScrollLayer* m_scrollLayer;
	cocos2d::CCArray* m_pagesArray;
}

class EditorPauseLayer : CCBlockLayer, FLAlertLayerProtocol {
	void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) = win 0x75780;
	void saveLevel() = win 0x75010;
	void onExitEditor(cocos2d::CCObject* pSender) = win 0x75660;
	void playStep2() = win 0x75440;
	void keyBackClicked() = win 0x758d0;
	void onResume(cocos2d::CCObject* pSender) = win 0x74fe0;
	void onSaveAndPlay(cocos2d::CCObject* pSender) = win 0x753d0;
	void onSaveAndExit(cocos2d::CCObject* pSender) = win 0x75620;
	void onSave(cocos2d::CCObject* pSender) = win 0x755a0;
	void onExitNoSave(cocos2d::CCObject* pSender) = win 0x75700;
	void uncheckAllPortals(cocos2d::CCObject* pSender) = win 0x74760;
	EditorPauseLayer() = win 0x72f10;
	bool init(LevelEditorLayer* editor) = win 0x730e0;

	bool m_saved;
	PAD = win 0x4;
	CCMenuItemSpriteExtra* m_button0;
	CCMenuItemSpriteExtra* m_button1;
	LevelEditorLayer* m_editorLayer;
	
}

class BoomScrollLayer {
	void instantMoveToPage(int page) = win 0x12330;
	void moveToPage(int page) = win 0x12400;
}

class EditorUI : cocos2d::CCLayer, FLAlertLayerProtocol, ColorSelectDelegate, GJRotationControlDelegate, GJScaleControlDelegate, MusicDownloadDelegate {
	static EditorUI* get() {
	    auto lel = LevelEditorLayer::get();
	    if (!lel) return nullptr;
	    return lel->m_editorUI;
	}
	cocos2d::CCArray* pasteObjects(gd::string const& _str) = win 0x88240;
	void deselectAll() = win 0x86af0;
	void selectObject(GameObject* obj, bool idk) = win 0x86250;
	void selectObjects(cocos2d::CCArray* objs, bool idk) = win 0x864a0;
	void selectAll() = win 0x86c40;
	void selectAllWithDirection(bool left) = win 0x86d80;
	cocos2d::CCArray* getSelectedObjects() = win 0x86900;
	cocos2d::CCPoint getTouchPoint(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) = win 0x90620;
	void onSelectBuildTab(cocos2d::CCObject* pSender) = win 0x887f0;
	void onCreateButton(cocos2d::CCObject* pSender) = win 0x854f0;
	void enableButton(CreateMenuItem* btn) = win 0x78990;
	void disableButton(CreateMenuItem* btn) = win 0x78af0;
	CreateMenuItem* getCreateBtn(int id, int bg) = win 0x85120;
	CCMenuItemSpriteExtra* getSpriteButton(const char* sprite, cocos2d::SEL_MenuHandler callback, cocos2d::CCMenu* menu, float scale) = win 0x78bf0;
	cocos2d::CCPoint getGroupCenter(cocos2d::CCArray* objs, bool idk) = win 0x8fc30;
	cocos2d::CCPoint offsetForKey(int objID) = win 0x92310;
	void updateButtons() = win 0x78280;
	void updateDeleteMenu() = win 0x7c5d0;
	void updateCreateMenu(bool updateTab) = win 0x85530;
	void toggleMode(cocos2d::CCObject* pSender) = win 0x7ad20;
	void moveObject(GameObject* obj, cocos2d::CCPoint position) = win 0x8ddb0;
	void zoomIn(cocos2d::CCObject* pSender) = win 0x877c0;
	void zoomOut(cocos2d::CCObject* pSender) = win 0x87830;
	void updateZoom(float amt) = win 0x878a0;
	void updateSlider() = win 0x78f10;
	void rotateObjects(cocos2d::CCArray* objects, float angle, cocos2d::CCPoint center) = win 0x8ee80;
	void scaleObjects(cocos2d::CCArray* objects, float scale, cocos2d::CCPoint center) = win 0x8f150;
	void updateGridNodeSize() = win 0x78f60;
	void updateObjectInfoLabel() = win 0x793b0;
	void updateSpecialUIElements() = win 0x87030;
	void constrainGameLayerPosition(float x, float y) = win 0x8f920;
	void moveGameLayer(cocos2d::CCPoint const& pos) = win 0x79290;
	void showUI(bool show) = win 0x87180;
	void editObject(cocos2d::CCObject* pSender) = win 0x8ca50;
	void editObject2(cocos2d::CCObject* pSender) = win 0x8d1b0;
	void editGroup(cocos2d::CCObject* pSender) = win 0x8d720;
	void moveObjectCall(cocos2d::CCObject* pSender) = win 0x8db30;
	void moveObjectCall(EditCommand command) = win 0x8db50;
	void transformObjectCall(cocos2d::CCObject* pSender) = win 0x8def0;
	void transformObjectCall(EditCommand command) = win 0x8df10;
	void onDeleteSelected(cocos2d::CCObject* pSender) = win 0x7bf50;
	void undoLastAction(cocos2d::CCObject* pSender) = win 0x87070;
	void redoLastAction(cocos2d::CCObject* pSender) = win 0x870f0;
	void onCopy(cocos2d::CCObject* pSender) = win 0x87fb0;
	void onPaste(cocos2d::CCObject* pSender) = win 0x880c0;
	void onDuplicate(cocos2d::CCObject* pSender) = win 0x87d20;
	void toggleEnableRotate(cocos2d::CCObject* pSender) = win 0x860d0;
	void toggleFreeMove(cocos2d::CCObject* pSender) = win 0x85eb0;
	void toggleSwipe(cocos2d::CCObject* pSender) = win 0x85dd0;
	void toggleSnap(cocos2d::CCObject* pSender) = win 0x85fa0;
	void onPlayback(cocos2d::CCObject* pSender) = win 0x87340;
	void onPlaytest(cocos2d::CCObject* pSender) = win 0x87600;
	void onStopPlaytest(cocos2d::CCObject* pSender) = win 0x876e0;
	void onGroupUp(cocos2d::CCObject* pSender) = win 0x8d780;
	void onGroupDown(cocos2d::CCObject* pSender) = win 0x8d7e0;
	void selectBuildTab(int tab) = win 0x88810;
	void onPause(cocos2d::CCObject* pSender) = win 0x78020;
	void onSettings(cocos2d::CCObject* pSender) = win 0x77fe0;
	void activateRotationControl(cocos2d::CCObject* pSender) = win 0x8fe70;
	void activateScaleControl(cocos2d::CCObject* pSender) = win 0x889b0;
	void dynamicGroupUpdate(bool idk) = win 0x8ad10;
	void createRockOutline() = win 0x89c10;
	void createRockEdges() = win 0x88ec0;
	void createRockBase() = win 0x8a2c0;
	void onCopyState(cocos2d::CCObject* pSender) = win 0x88490;
	void onPasteColor(cocos2d::CCObject* pSender) = win 0x88580;
	void onPasteState(cocos2d::CCObject* pSender) = win 0x884c0;
	void onGroupSticky(cocos2d::CCObject* pSender) = win 0x87a80;
	void onUngroupSticky(cocos2d::CCObject* pSender) = win 0x87ac0;
	void onGoToLayer(cocos2d::CCObject* pSender) = win 0x886b0;
	void onGoToBaseLayer(cocos2d::CCObject* pSender) = win 0x88790;
	void editColor(cocos2d::CCObject* pSender) = win 0x8d3c0;
	void alignObjects(cocos2d::CCArray* objs, bool alignY) = win 0x8f320;
	void keyDown(cocos2d::enumKeyCodes key) = win 0x91a30;
	void keyUp(cocos2d::enumKeyCodes key) = win 0x92180;

	EditButtonBar* m_buttonBar; 
	PAD = win 0x4;
	cocos2d::CCArray* m_hideableUIElementArray; 
	PAD = win 0x4;
	float m_gridSize; 
	PAD = win 0x14;
	bool m_moveModifier; 
	int m_rotationTouchID; 
	int m_scaleTouchID; 
	int m_touchID; 
	cocos2d::CCLabelBMFont* m_objectInfoLabel; 
	GJRotationControl* m_rotationControl; 
	cocos2d::CCPoint m_scalePos;  
	bool m_touchDown; 
	GJScaleControl* m_scaleControl; 
	cocos2d::CCDictionary* m_editButtonDict; 
	EditButtonBar* m_createButtonBar; 
	EditButtonBar* m_editButtonBar; 
	Slider* m_positionSlider; 
	float m_unknown0;      
	float m_minYLimit;     
	float m_unknown2;      
	bool m_swipeEnabled; 
	bool m_freeMoveEnabled;  
	PAD = win 0xc;
	cocos2d::CCArray* m_unknownArray2; 
	PAD = win 0x8;
	cocos2d::CCArray* m_selectedObjects; 
	cocos2d::CCMenu* m_deleteMenu; 
	cocos2d::CCArray* m_unknownArray4; 
	CCMenuItemSpriteExtra* m_deleteModeBtn; 
	CCMenuItemSpriteExtra* m_buildModeBtn; 
	CCMenuItemSpriteExtra* m_editModeBtn; 
	CCMenuItemSpriteExtra* m_swipeBtn; 
	CCMenuItemSpriteExtra* m_freeMoveBtn; 
	CCMenuItemSpriteExtra* m_deselectBtn; 
	CCMenuItemSpriteExtra* m_snapBtn; 
	CCMenuItemSpriteExtra* m_rotateBtn; 
	CCMenuItemSpriteExtra* m_playbackBtn; 
	CCMenuItemSpriteExtra* m_PlaytestBtn; 
	CCMenuItemSpriteExtra* m_playtestStopBtn; 
	CCMenuItemSpriteExtra* m_trashBtn; 
	CCMenuItemSpriteExtra* m_linkBtn; 
	CCMenuItemSpriteExtra* m_unlinkBtn; 
	CCMenuItemSpriteExtra* m_undoBtn; 
	CCMenuItemSpriteExtra* m_redoBtn; 
	CCMenuItemSpriteExtra* m_editObjectBtn; 
	CCMenuItemSpriteExtra* m_editGroupBtn; 
	CCMenuItemSpriteExtra* m_editHSVBtn; 
	CCMenuItemSpriteExtra* m_editSpecialBtn; 
	CCMenuItemSpriteExtra* m_copyPasteBtn; 
	CCMenuItemSpriteExtra* m_copyBtn; 
	CCMenuItemSpriteExtra* m_pasteBtn; 
	CCMenuItemSpriteExtra* m_copyValuesBtn; 
	CCMenuItemSpriteExtra* m_pasteStateBtn; 
	CCMenuItemSpriteExtra* m_pasteColorBtn; 
	CCMenuItemSpriteExtra* m_goToLayerBtn; 
	CCMenuItemToggler* m_guideToggle; 
	cocos2d::CCArray* m_createButtonBars; 
	cocos2d::CCMenu* m_tabsMenu; 
	cocos2d::CCArray* m_tabsArray; 
	cocos2d::CCSprite* m_idkSprite0; 
	cocos2d::CCSprite* m_idkSprite1; 
	CCMenuItemSpriteExtra* m_button27; 
	CCMenuItemSpriteExtra* m_button28; 
	CCMenuItemSpriteExtra* m_deleteFilterNone; 
	CCMenuItemSpriteExtra* m_deleteFilterStatic; 
	CCMenuItemSpriteExtra* m_deleteFilterDetails; 
	CCMenuItemSpriteExtra* m_deleteFilterCustom; 
	cocos2d::CCLabelBMFont* m_currentLayerLabel; 
	CCMenuItemSpriteExtra* m_layerNextBtn; 
	CCMenuItemSpriteExtra* m_layerPrevBtn; 
	CCMenuItemSpriteExtra* m_goToBaseBtn; 
	PAD = win 0x8;
	int m_selectedCreateObjectID; 
	cocos2d::CCArray* m_createButtonArray;  
	cocos2d::CCArray* m_customObjectButtonArray; 
	cocos2d::CCArray* m_unknownArray9; 
	int m_selectedMode;
	LevelEditorLayer* m_editorLayer;  
	cocos2d::CCPoint m_swipeStart;    
	cocos2d::CCPoint m_swipeEnd;      
	PAD = win 0x20;
	GameObject* m_selectedObject;  
	PAD = win 0x8;
	gd::string m_clipboard;   
	PAD = win 0x8;
	int m_selectedTab; 
	int m_timesSelected; 
	PAD = win 0x20;
	bool m_spaceKeyPressed;
}

class LevelEditorLayer : GJBaseGameLayer, LevelSettingsDelegate {
	static LevelEditorLayer* get() {
	    return GameManager::sharedState()->getEditorLayer();
	}
	EditorUI* getEditorUI() {
	    return this->m_editorUI;
	}
	static LevelEditorLayer* create(GJGameLevel* level) = win 0x15ed60;
	static cocos2d::CCScene* scene(GJGameLevel* level) {
	    auto scene = cocos2d::CCScene::create();
	
	    scene->addChild(LevelEditorLayer::create(level));
	    scene->setObjType(cocos2d::kCCObjectTypeLevelEditorLayer);
	
	    cocos2d::CCDirector::sharedDirector()->replaceScene(
	        cocos2d::CCTransitionFade::create(0.5f, scene)
	    );
	    return scene;
	}
	GameObject* createObject(int id, cocos2d::CCPoint const& position, bool undo) = win 0x160d70;
	void removeObject(GameObject * obj, bool idk) = win 0x161cb0;
	int getNextFreeGroupID(cocos2d::CCArray* objs) = win 0x164ae0;
	GameObject* addObjectFromString(gd::string const& str) = win 0x160c80;
	GameObject* objectAtPosition(cocos2d::CCPoint const& pos) = win 0x161300;
	cocos2d::CCRect getObjectRect(GameObject* obj, bool unk) = win 0x1616b0;
	void pasteAtributeState(GameObject* obj, cocos2d::CCArray* objs) = win 0x16b740;
	int getCurrentLayer() {
	    return m_currentLayer;
	}
	void setCurrentLayer(int n) {
	    m_currentLayer = n;
	}
	float getLastObjectX() = win 0x167290;
	gd::string getLevelString() = win 0x162480;
	void removeAllObjects() = win 0x161d60;
	void updateOptions() = win 0x15fcc0;
	void updateEditorMode() = win 0x1652b0;
	void addSpecial(GameObject* obj) = win 0x162650;

	PAD = win 0x4;
	bool m_unk2d0;   
	bool m_unk2d1;   
	bool m_unk2d2;   
	bool m_unk2d3;   
	bool m_gridEnabled;   
	bool m_unk2d5;   
	bool m_unk2d6;   
	bool m_unk2d7;   
	bool m_hasLDM;   
	bool m_moreUndo;   
	bool m_unk2da;   
	bool m_unk2db;   
	bool m_highDetail;   
	PAD = win 0x30;
	GameObject* m_copyStateObject; 
	PAD = win 0xc;
	int m_coinCountRand1;	
	int m_coinCountRand2;	
	int m_coinCount;		
	PAD = win 0x28;
	int m_currentLayer;    
	PAD = win 0x28;
	EditorUI* m_editorUI;  
	PAD = win 0x4;
	cocos2d::CCArray* m_undoObjects;   
	cocos2d::CCArray* m_someArray; 
	PAD = win 0x8;
	int m_objectsRand1;	
	int m_objectsRand2;	
	int m_objects;			
	DrawGridLayer* m_drawGridLayer;    
	GJGameLevel* m_level;			
	PlaybackMode m_playbackMode;   
	PAD = win 0x14;
	GJGroundLayer* m_groundLayer;
}

class GJSpecialColorSelect {
	const char* textForColorIdx(int id) = win 0x14e1d0;
}

class SetGroupIDLayer : FLAlertLayer, TextInputDelegate {
	void updateEditorLayerID() = win 0x22e0b0;
	void updateEditorLayerID2() = win 0x22e110;
	void updateZOrder() = win 0x22e3d0;
	void updateGroupIDLabel() = win 0x22e450;

	GameObject* m_obj;         
	cocos2d::CCArray* m_objs;      
	cocos2d::CCArray* m_array0;    
	cocos2d::CCArray* m_array1;    
	cocos2d::CCLabelBMFont* m_editorLayerText;     
	cocos2d::CCLabelBMFont* m_editorLayer2Text;    
	cocos2d::CCLabelBMFont* m_zOrderText;          
	CCTextInputNode* m_groupIDInput;           
	int m_groupIDValue;        
	int m_editorLayerValue;    
	int m_editorLayer2Value;   
	int m_zOrderValue;         
	ZLayer m_zLayerValue;      
	bool m_unk204;             
	bool m_unk205;             
	bool m_highDetail;         
	bool m_dontFade;           
	bool m_dontEnter;          
	void* m_somePointerProlly; 
	bool m_groupParent;        
	bool m_hasEditedGroups;    
	int m_unknown;             
}

class SetupPulsePopup : FLAlertLayer, ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
    cocos2d::extension::CCControlColourPicker* m_colorPicker;
    PAD = win 0x30;
    cocos2d::CCSprite* m_currentColorSpr;
    cocos2d::CCSprite* m_prevColorSpr;
    PAD = win 0x64;
    int m_pulseMode;

    virtual void colorValueChanged(cocos2d::ccColor3B color) {}
}

class UndoObject : cocos2d::CCObject {
	static UndoObject* create(GameObject* pObject, UndoCommand nCommand) = win 0x16bc20;
	static UndoObject* createWithArray(cocos2d::CCArray* pObjects, UndoCommand nCommand) = win 0x16bee0;
	static UndoObject* createWithTransformObjects(cocos2d::CCArray* pObjects, UndoCommand nCommand) = win 0x16bcd0;

	GameObject* m_gameObject;      
	UndoCommand m_command;         
	cocos2d::CCArray* m_objects;   
	bool m_redo;                   
}

class GaragePage : cocos2d::CCLayer, ListButtonBarDelegate {
	void listButtonBarSwitchedPage(ListButtonBar* bar, int idk) = win 0x12bb40;
	GaragePage() = win 0x1252f0;
	bool init(IconType type, GJGarageLayer* pGarage, cocos2d::SEL_MenuHandler pSelectCallback) = win 0x12af70;
	static GaragePage* create(IconType type, GJGarageLayer* pGarage, cocos2d::SEL_MenuHandler pSelectCallback) {
	    auto pRet = new GaragePage();
	
	    if (pRet && pRet->init(type, pGarage, pSelectCallback)) {
	        pRet->autorelease();
	        return pRet;
	    }
	
	    CC_SAFE_DELETE(pRet);
	    return nullptr;
	}

    GJGarageLayer* m_garage;
    cocos2d::SEL_MenuHandler m_handler;
    cocos2d::CCSprite* m_selectSprite;
    CCMenuItemSpriteExtra* m_unkNode0x12c;
    IconType m_type;
    int m_unknown;
    PAD = win 0x4;
}

class ListButtonBar : cocos2d::CCNode {
    BoomScrollLayer* m_scrollLayer;    
}

class GauntletSelectLayer {
	static GauntletSelectLayer* create() = win 0x105120;
}

class GJBaseGameLayer : cocos2d::CCLayer, TriggerEffectDelegate {
	cocos2d::CCLayer* getObjectLayer() {
	    return m_objectLayer; 
	}
	cocos2d::CCArray* getAllObjects() {
	    return this->m_objects;
	}
	void pushButton(int uk, bool btn2) = win 0x111500;
	void releaseButton(int uk, bool btn2) = win 0x111660;
	void toggleGroupTriggered(int group, bool on) = win 0x10f890;
	void spawnGroup(int group) = win 0x10f400;

	OBB2D* m_boundingBox; 
	GJEffectManager* m_effectManager; 
	cocos2d::CCLayer* m_objectLayer; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddTop4; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddTop4; 
	cocos2d::CCSpriteBatchNode* m_batchNodeTop3; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddTop3; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddGlowTop3; 
	CCNodeContainer* m_batchNodeTop3Container; 
	cocos2d::CCSpriteBatchNode* m_batchNodeTextTop3; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddTextTop3; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeTop3; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddTop3; 
	cocos2d::CCSpriteBatchNode* m_batchNodeTop2; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddTop2; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddGlowTop2; 
	CCNodeContainer* m_batchNodeTop2Container; 
	cocos2d::CCSpriteBatchNode* m_batchNodeTextTop2; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddTextTop2; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeTop2; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddTop2; 
	cocos2d::CCSpriteBatchNode* m_batchNode; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAdd; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddGlow; 
	CCNodeContainer* m_batchNodeTop1Container; 
	cocos2d::CCSpriteBatchNode* m_batchNodeTextTop1; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddTextTop1; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeTop1; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddTop1; 
	cocos2d::CCSpriteBatchNode* m_batchNodePlayer; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddPlayer; 
	cocos2d::CCSpriteBatchNode* m_batchNodePlayerGlow; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddMid; 
	cocos2d::CCSpriteBatchNode* m_batchNodeBottom; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddBottomGlow; 
	CCNodeContainer* m_batchNodeBot1Container; 
	cocos2d::CCSpriteBatchNode* m_batchNodeText; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddText; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNode; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeAdd; 
	cocos2d::CCSpriteBatchNode* m_batchNodeBottom2; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom2; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom2Glow; 
	CCNodeContainer* m_batchNodeBot2Container; 
	cocos2d::CCSpriteBatchNode* m_batchNodeTextBot2; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddTextBot2; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeBot2; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddBot2; 
	cocos2d::CCSpriteBatchNode* m_batchNodeBottom3; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom3; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom3Glow; 
	CCNodeContainer* m_batchNodeBot3Container; 
	cocos2d::CCSpriteBatchNode* m_batchNodeTextBot3; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddTextBot3; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeBot3; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddBot3; 
	cocos2d::CCSpriteBatchNode* m_batchNodeBottom4; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom4; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddBottom4Glow; 
	CCNodeContainer* m_batchNodeBot4Container; 
	cocos2d::CCSpriteBatchNode* m_batchNodeTextBot4; 
	cocos2d::CCSpriteBatchNode* m_batchNodeAddTextBot4; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeBot4; 
	cocos2d::CCSpriteBatchNode* m_effectBatchNodeAddBot4; 
	PlayerObject* m_player1; 
	PlayerObject* m_player2; 
	LevelSettingsObject* m_levelSettings; 
	cocos2d::CCDictionary* m_disabledGroupsDict; 
	cocos2d::CCArray* m_objects; 
	cocos2d::CCArray* m_sectionObjects; 
	cocos2d::CCArray* m_sections; 
	cocos2d::CCArray* m_collisionBlocks; 
	cocos2d::CCArray* m_spawnObjects; 
	cocos2d::CCArray* m_unk248;
	cocos2d::CCNode* m_groupNodes; 
	gd::vector<GameObject*> m_objectsVec; 
	gd::vector<GameObject*> m_disabledObjects; 
	cocos2d::CCDictionary* m_groupDict; 
	cocos2d::CCDictionary* m_staticGroupDict; 
	cocos2d::CCDictionary* m_optimizedGroupDict; 
	gd::vector<cocos2d::CCArray*> m_groups; 
	gd::vector<cocos2d::CCArray*> m_staticGroups; 
	gd::vector<cocos2d::CCArray*> m_optimizedGroups; 
	cocos2d::CCArray* m_batchNodes; 
	cocos2d::CCArray* m_processedGroups; 
	cocos2d::CCDictionary* m_counterDict; 
	cocos2d::CCDictionary* m_spawnedGroups; 
	bool m_didUpdateNormalCapacity; 
	bool m_isDualMode; 
	int m_unk2AC;
	bool m_activeDualTouch; 
	int m_attemptClickCount; 
	int m_currentSection; 
	int m_oldSection; 
	bool m_disabledObjects; 
	bool m_blending; 
	PAD = win 0x8;
}

class GJListLayer {
	static GJListLayer* create(cocos2d::CCObject* target, const char* title, cocos2d::ccColor4B color, float width, float height) = win 0x12e000;
}

class GJGameLevel {
	void dataLoaded(DS_Dictionary* dict) = win 0xbded0;
	static GJGameLevel* create() = win 0xbd2b0;
	static GJGameLevel* createWithCoder(DS_Dictionary* dict) {
	    auto ret = GJGameLevel::create();
	    ret->dataLoaded(dict);
	    return ret;
	}
	gd::string getAudioFileName() = win 0xbdc70;
	void setLevelData(const char* data) {
	    auto len = strlen(data);
	    auto addedLvlStr = reinterpret_cast<uintptr_t>(this) + 0x12c;
	
	    *reinterpret_cast<size_t*>(addedLvlStr + 16) = len;   
	    *reinterpret_cast<size_t*>(addedLvlStr + 20) = max(len, 15); 
	
	    if (len <= 15)
	        memcpy(reinterpret_cast<char*>(addedLvlStr), data, len + 1);
	    else {
	        void* newb = malloc(len + 1);
	        memcpy(newb, data, len + 1);
	        *reinterpret_cast<void**>(addedLvlStr) = newb;
	    }
	}
	void setLevelData(gd::string const& data) {
	    this->setLevelData(data.c_str());
	}
	GJDifficulty getAverageDifficulty() = win 0xbd9b0;
	void levelWasAltered() = win 0xbd550;
	
	cocos2d::CCDictionary* m_lastBuildSave;   
	int m_levelID_rand; 
	int m_levelID_seed; 
	int m_levelID;      
	gd::string m_levelName;       
	gd::string m_levelDesc;       
	gd::string m_levelString;     
	gd::string m_creatorName;     
	gd::string m_recordString;    
	gd::string m_uploadDate; 
	gd::string m_updateDate; 
	int m_userID_rand;     
	int m_userID_seed;     
	int m_userID;          
	int m_accountID_rand;  
	int m_accountID_seed;  
	int m_accountID;       
	GJDifficulty m_difficulty;      
	int m_audioTrack;      
	int m_songID;          
	int m_levelRev;        
	bool m_unlisted;       
	int m_objectCount_rand;
	int m_objectCount_seed;
	int m_objectCount;     
	int m_levelIndex;      
	int m_ratings;         
	int m_ratingsSum;      
	int m_downloads;       
	bool m_isEditable;     
	bool m_gauntletLevel;  
	bool m_gauntletLevel2; 
	int m_workingTime;     
	int m_workingTime2;    
	bool m_lowDetailMode;  
	bool m_lowDetailModeToggled;   
	int m_isVerified_rand; 
	int m_isVerified_seed; 
	bool m_isVerified;     
	bool m_isUploaded;     
	bool m_hasBeenModified;
	int m_levelVersion;    
	int m_gameVersion;     
	int m_attempts_rand;   
	int m_attempts_seed;   
	int m_attempts;        
	int m_jumps_rand;      
	int m_jumps_seed;      
	int m_jumps;           
	int m_clicks_rand;     
	int m_clicks_seed;     
	int m_clicks;          
	int m_attemptTime_rand;
	int m_attemptTime_seed;
	int m_attemptTime;     
	int m_chk;             
	bool m_isChkValid;     
	bool m_isCompletionLegitimate; 
	int m_normalPercent;   
	int m_normalPercent_seed;  
	int m_normalPercent_rand;  
	int m_orbCompletion_rand;  
	int m_orbCompletion_seed;  
	int m_orbCompletion;       
	int m_newNormalPercent2_rand;  
	int m_newNormalPercent2_seed;  
	int m_newNormalPercent2;       
	int m_practicePercent; 
	int m_likes;           
	int m_dislikes;        
	int m_levelLength;     
	int m_featured;        
	bool m_isEpic;         
	bool m_levelFavorited; 
	int m_levelFolder;     
	int m_dailyID_rand;    
	int m_dailyID_seed;    
	int m_dailyID;         
	int m_demon_rand;      
	int m_demon_seed;      
	int m_demon;           
	int m_demonDifficulty; 
	int m_stars_rand;      
	int m_stars_seed;      
	int m_stars;           
	bool m_autoLevel;      
	int m_coins;           
	int m_coinsVerified_rand;  
	int m_coinsVerified_seed;  
	int m_coinsVerified;       
	int m_password_rand;       
	int m_password_seed;       
	int m_originalLevel_rand;  
	int m_originalLevel_seed;  
	int m_originalLevel;       
	bool m_twoPlayerMode;      
	int m_failedPasswordAttempts;  
	int m_firstCoinVerified_rand;  
	int m_firstCoinVerified_seed;  
	int m_firstCoinVerified;       
	int m_secondCoinVerified_rand; 
	int m_secondCoinVerified_seed; 
	int m_secondCoinVerified;      
	int m_thirdCoinVerified_rand;  
	int m_thirdCoinVerified_seed;  
	int m_thirdCoinVerified;   
	int m_starsRequested;      
	bool m_showedSongWarning;  
	int m_starRatings;         
	int m_starRatingsSum;      
	int m_maxStarRatings;      
	int m_minStarRatings;      
	int m_demonVotes;          
	int m_rateStars;           
	int m_rateFeature;         
	gd::string m_rateUser;    
	bool m_dontSave;           
	bool m_levelNotDownloaded; 
	int m_requiredCoins;       
	bool m_isUnlocked;         
	cocos2d::CCPoint m_lastCameraPos; 
	float m_fastEditorZoom;    
	int m_lastBuildTab;        
	int m_lastBuildPage;       
	int m_lastBuildGroupID;    
	GJLevelType m_levelType;   
	int m_M_ID;                
	gd::string m_tempName;    
	gd::string m_capacityString;  
	bool m_highObjectsEnabled;     
	gd::string m_personalBests;   
}

class GJMapPack : cocos2d::CCNode {
	cocos2d::CCArray* m_levels;
	int m_packID;
	GJDifficulty m_difficulty;
	int m_stars;
	int m_coins;
	gd::string m_packName;
	gd::string m_levelStrings;
	cocos2d::ccColor3B m_textColour;
	cocos2d::ccColor3B m_barColour;
	int m_MId;
	bool m_isGauntlet;
}

class GJSearchObject : cocos2d::CCNode {
	static GJSearchObject* create(SearchType nID) = win 0xc2b90;
	SearchType getType() {
	    return this->m_searchType;
	}

    SearchType m_searchType;
    gd::string m_searchQuery;
    gd::string m_difficulty;
    gd::string m_length;
    int m_page;
    bool m_starFilter;
    bool m_noStarFilter;
    int m_unknownMember;
    bool m_uncompletedFilter;
    bool m_completedFilter;
    bool m_featuredFilter;
    bool m_originalFilter;
    bool m_twoPlayerFilter;
    bool m_coinsFilter;
    bool m_epicFilter;
    GJDifficulty m_demonFilter;
    int m_unk;
    int m_songID;
    bool m_customSongFilter;
    bool m_songFilter;
}

class PauseLayer {
	void createToggleButton(cocos2d::SEL_MenuHandler callback, bool on, cocos2d::CCMenu* menu, gd::string caption, cocos2d::CCPoint pos) = win 0x1e5570;
}

class PlayerObject {
	void setVisible(bool visible) = win 0x1fa860;
	void pushButton(int button) = win 0x1f4e40;
	void releaseButton(int button) = win 0x1f4f70;
	bool playerIsFalling() = win 0x1f5d60;
	void runRotateAction() = win 0x1e9bf0;
	void playBurstEffect() = win 0x1f6790;
	void spiderTestJump(bool param1) = win 0x1ed360;
	void incrementJumps() = win 0x1e9a20;
	void flipGravity(bool param1, bool param2) = win 0x1f59d0;
	void runNormalRotation() = win 0x1e9c50;
	void runBallRotation() = win 0x1e9d10;
	void activateStreak() = win 0x1f9080;

	PAD = win 0x18;
	bool m_unk480;
	cocos2d::CCNode* m_unk484;
	cocos2d::CCDictionary* m_collisionLog; 
	cocos2d::CCDictionary* m_collisionLog1; 
	PAD = win 0x20;
	bool m_unk4B0;
	cocos2d::CCSprite* m_unk4B4;
	PAD = 0x1c;
	bool m_unk4D4;
	cocos2d::CCArray* m_particleSystems; 
	bool m_unk4DC;
	bool m_isHidden; 
	int m_hasGhostTrail; 
	GhostTrailEffect* m_ghostTrail; 
	cocos2d::CCSprite* m_iconSprite;
	cocos2d::CCSprite* m_iconSpriteSecondary;
	cocos2d::CCSprite* m_iconSpriteWhitener;
	cocos2d::CCSprite* m_iconGlow;
	cocos2d::CCSprite* m_vehicleSprite;
	cocos2d::CCSprite* m_vehicleSpriteSecondary;
	cocos2d::CCSprite* m_unk500;
	cocos2d::CCSprite* m_vehicleSpriteWhitener;
	cocos2d::CCSprite* m_vehicleGlow;
	cocos2d::CCSprite* m_unk50C;
	cocos2d::CCMotionStreak* m_regularTrail; 
	HardStreak* m_waveTrail; 
	double m_xAccel; 
	double m_jumpAccel; 
	double m_gravity; 
	PAD = win 0x10;
	bool m_unk538;
	bool m_unk539;
	bool m_unk53A;
	bool m_unk53B;
	bool m_isInPlayLayer; 
	bool m_unk53D;
	bool m_unk53E; 
	bool m_unk53F;
	PAD = win 0x10;
	double m_lastJumpTime; 
	double m_unk558;
	PAD = win 0x24;
	float m_unk584;
	PAD = win 0x14;
	GameObject* m_unk59C;
	PAD = win 0x8;
	GJRobotSprite* m_robot; 
	GJSpiderSprite* m_spider; 
	bool m_unk5B0;
	cocos2d::CCParticleSystemQuad* m_unk5B4;
	cocos2d::CCParticleSystemQuad* m_unk5B8;
	cocos2d::CCParticleSystemQuad* m_unk5BC;
	cocos2d::CCParticleSystemQuad* m_unk5C0;
	cocos2d::CCParticleSystemQuad* m_unk5C4;
	cocos2d::CCParticleSystemQuad* m_unk5C8;
	cocos2d::CCParticleSystemQuad* m_unk5CC;
	PAD = win 0x4;
	cocos2d::CCParticleSystemQuad* m_unk5D4;
	cocos2d::CCParticleSystemQuad* m_unk5D8;
	PAD = win 0x8;
	int m_streakID;	
	float m_wellIdk;	
	PAD = win 0x10;
	bool m_unk5FC;
	bool m_unk5FD;
	bool m_unk5FE;
	PAD = win 0x10;
	bool m_unk610;
	bool m_isHolding; 
	bool m_hasJustHeld; 
	bool m_isHolding2; 
	bool m_hasJustHeld2; 
	int m_unk618;
	float m_unk61C;
	int m_unk620; 
	bool m_canRobotJump; 
	double m_yAccel; 
	bool m_unk630;
	bool m_unk631;
	float m_unk634;
	bool m_isShip; 
	bool m_isBird; 
	bool m_isBall; 
	bool m_isDart; 
	bool m_isRobot; 
	bool m_isSpider; 
	bool m_isUpsideDown; 
	bool m_isOnGround; 
	bool m_isDashing; 
	float m_vehicleSize; 
	float m_playerSpeed; 
	cocos2d::CCPoint m_unk64C; 
	cocos2d::CCPoint m_lastPortalPos; 
	cocos2d::CCLayer* m_unk65C;
	bool m_isSliding; 
	bool m_isRising; 
	bool m_unk662;
	cocos2d::CCPoint m_lastGroundedPos; 
	cocos2d::CCArray* m_touchingRings; 
	GameObject* m_lastActivatedPortal; 
	bool m_unk674;
	PAD = win 0x4;
	cocos2d::CCPoint m_position; 
	bool m_unk684;
	bool m_unk685;
	double m_unk688;
	PAD = win 0x8;
	float m_groundHeight; 
	float m_unk69C; 
	PAD = win 0x4;
	float m_unk6A4[200]; 
	PAD = win 0x1c;
}

class CCAnimatedSprite {
	static CCAnimatedSprite* create(const char* file) = win 0x14540;
	void runAnimation(gd::string name) = win 0x14f60;
}

class UILayer {
	void onCheck(cocos2d::CCObject* pSender) = win 0x25fb60;
	void onDeleteCheck(cocos2d::CCObject* pSender) = win 0x25fc90;
	void onPause(cocos2d::CCObject* pSender) = win 0x25fad0;
	void keyDown(cocos2d::enumKeyCodes key) = win 0x25f890;
	void keyUp(cocos2d::enumKeyCodes key) = win 0x25fa10;
}

class GJGroundLayer {
	void updateGroundWidth() = win 0x12dda0;
}

class PlayLayer {
	static PlayLayer* get() {
	    return GameManager::sharedState()->getPlayLayer();
	}
	static PlayLayer* create(GJGameLevel* lvl) = win 0x1fb6d0;
	static cocos2d::CCScene* scene(GJGameLevel* lvl) = win 0x1fb690;
	void togglePracticeMode(bool on) = win 0x20d0d0;
	
	PAD = win 0x4;
	bool unk2DC;
	bool m_hasCheated; 
	int unk2E0; 
	int unk2E4; 
	int unk2E8;
	bool unk2EC; 
	bool unk2ED;
	float unk2F0;
	PAD = win 0x4;
	cocos2d::CCDrawNode* unk2F8;
	float unk2FC; 
	float unk300;
	float unk304;
	float unk308; 
	float unk30C; 
	bool unk310;
	bool unk311;
	bool unk312;
	PAD = win 0x19;
	StartPosObject* m_startPos; 
	CheckpointObject* unk330;
	EndPortalObject* m_endPortal; 
	cocos2d::CCArray* m_checkpoints; 
	cocos2d::CCArray* unk33C;
	cocos2d::CCArray* unk340;
	cocos2d::CCArray* unk344;
	cocos2d::CCSprite* unk348;
	float m_backgroundRepeat; 
	bool unk350;
	cocos2d::CCArray* unk354;
	cocos2d::CCArray* unk358;
	cocos2d::CCArray* unk35C;
	cocos2d::CCArray* unk360;
	bool m_isMute; 
	bool unk365;
	bool unk366;
	bool unk367;
	bool unk368;
	bool unk369;
	bool unk36A;
	bool unk36B;
	cocos2d::CCArray* m_screenRingObjects; 
	cocos2d::CCParticleSystemQuad* unk370;
	cocos2d::CCDictionary* m_pickedUpItems; 
	cocos2d::CCArray* m_circleWaves; 
	cocos2d::CCArray* unk37C;
	AudioEffectsLayer* m_audioEffectsLayer; 
	PAD = win 0x8;
	GJGroundLayer* m_bottomGround; 
	GJGroundLayer* m_topGround; 
	PAD = win 0x8;
	bool m_isDead; 
	bool unk39D; 
	bool unk39E; 
	bool unk39F; 
	bool unk3A0;
	int unk3A4;
	float unk3A8;
	bool unk3AC;
	bool unk3AD;
	bool unk3AE;
	bool unk3AF;
	PAD = win 0x4;
	float m_levelLength; 
	float unk3B8;
	cocos2d::CCLabelBMFont* m_attemptLabel; 
	cocos2d::CCLabelBMFont* m_percentLabel; 
	bool m_isCameraShaking; 
	float m_currentShakeStrength; 
	float m_currentShakeInterval; 
	double m_lastShakeTime; 
	cocos2d::CCPoint m_cameraShakeOffset; 
	bool unk3E0;
	float m_backgroundScrollOffset; 
	float m_cameraFlip; 
	bool unk3EC;
	int unk3F0; 
	cocos2d::CCDictionary* m_particleSystemsPlist; 
	cocos2d::CCDictionary* unk3F8;
	cocos2d::CCArray* m_particleSystems; 
	cocos2d::CCNode* unk400;
	cocos2d::CCSprite* m_sliderGrooveSprite; 
	cocos2d::CCSprite* m_sliderBarSprite; 
	float m_sliderWidth; 
	float unk410; 
	PAD = win 0x4;
	int unk418; 
	PAD = win 0x8;
	cocos2d::CCArray* m_gravitySprites; 
	bool unk428;
	bool m_shouldRecordActions; 
	bool unk42A;
	bool m_isPaused; 
	bool unk42C;
	bool m_isPlayer2Frozen; 
	gd::string m_previousRecords; 
	PAD = win 0x8;
	double m_time; 
	PAD = win 0x8;
	bool unk460;
	bool unk461;
	cocos2d::CCDictionary* unk464;
	gd::map<short, bool> unk468;
	bool m_isPlaybackMode; 
	GameObject* unk474;
	PAD = win 0x4;
	bool m_isFlipped; 
	float m_mirrorTransition; 
	UILayer* m_UILayer; 
	GJGameLevel* m_level; 
	cocos2d::CCPoint m_cameraPos; 
	bool m_isTestMode; 
	bool m_isPracticeMode; 
	bool unk496;
	bool unk497;
	cocos2d::CCArray* unk498; 
	bool unk49C;
	cocos2d::CCPoint unk4A0;
	int m_currentAttempt; 
	int m_jumpCount; 
	bool unk4B0;
	float m_totalTime; 
	int m_attemptJumpCount; 
	bool unk4BC;
	bool m_hasLevelCompleteMenu; 
	bool m_hasCompletedLevel; 
	bool unk4BF;
	int m_lastDeathPercent; 
	bool unk4C4;
	PAD = win 0xb;
	bool unk4D0;
	bool unk4D1;
	cocos2d::CCArray* unk4D4;
	cocos2d::CCDictionary* unk4D8;
	PAD = win 0x4;
	double unk4E0;
	double unk4E8; 
	PAD = win 0xc;
	float unk4FC;
	int unk500;
	PAD = win 0x4;
	double unk508; 
	PAD = win 0x8;
	double unk518;
	double unk520;
	PAD = win 0x4;
	bool m_hasGlitter; 
	bool m_isBgEffectOff; 
	bool unk52F; 
	GameObject* unk530;
	bool unk534;
	bool unk535; 
	bool m_disableGravityEffect;
}

class AchievementManager {
	static AchievementManager* sharedState() = win 0x7b10;

	PAD = win 0x10;
	cocos2d::CCDictionary* m_achievements;
	PAD = win 0x4;
}

class GameLevelManager {
	static GameLevelManager* sharedState() = win 0x9f860;
	GJGameLevel* createNewLevel() = win 0xa0db0;
	GJGameLevel* getMainLevel(int id, bool unk) = win 0xa0940;

	cocos2d::CCDictionary* m_mainLevels; 
	cocos2d::CCDictionary* m_searchFilters; 
	cocos2d::CCDictionary* m_onlineLevels;
	PAD = win 0x4;
	cocos2d::CCDictionary* m_followedCreators; 
	cocos2d::CCDictionary* m_downloadedLevels;
	
	
	cocos2d::CCDictionary* m_likedLevels; 
	cocos2d::CCDictionary* m_ratedLevels;
	cocos2d::CCDictionary* m_reportedLevels;
	
	cocos2d::CCDictionary* m_onlineFolders;
	cocos2d::CCDictionary* m_localLevelsFolders;
	cocos2d::CCDictionary* m_dailyLevels;
	int m_dailyTimeLeft; 
	int m_dailyID;
	int m_dailyIDUnk; 
	PAD = win 0x4;
	int m_weeklyTimeLeft;
	int m_weeklyID;
	int m_weeklyIDUnk; 
	cocos2d::CCDictionary* m_gauntletLevels;
	PAD = win 0x8;
	cocos2d::CCDictionary* m_unkDict14; 
	cocos2d::CCDictionary* m_knownUsers; 
	cocos2d::CCDictionary* m_accountIDtoUserIDDict; 
	cocos2d::CCDictionary* m_userIDtoAccountIDDict; 
	cocos2d::CCDictionary* m_unkDict18; 
	cocos2d::CCDictionary* m_unkDict19; 
	cocos2d::CCDictionary* m_unkDict20;
	cocos2d::CCDictionary* m_unkDict21;
	cocos2d::CCDictionary* m_unkDict22;
	cocos2d::CCDictionary* m_unkDict23;
	cocos2d::CCDictionary* m_unkDict24;
	cocos2d::CCDictionary* m_unkDict25;
	cocos2d::CCDictionary* m_unkDict26;
	cocos2d::CCDictionary* m_unkDict27;
	cocos2d::CCDictionary* m_unkDict28;
	gd::string m_unkStr1;
	gd::string m_unkStr2; 
	PAD = win 0x5c;
	gd::string m_unkStr3;
	cocos2d::CCString* m_unkStr4;
}

class GameManager {
	int getPlayerFrame() {
	    return m_playerFrame;
	}
	int getPlayerShip() {
	    return m_playerShip;
	}
	int getPlayerBall() {
	    return m_playerBall;
	}
	int getPlayerBird() {
	    return m_playerBird;
	}
	int getPlayerDart() {
	    return m_playerDart;
	}
	int getPlayerRobot() { 
	    return m_playerRobot; 
	}
	int getPlayerSpider() { 
	    return m_playerSpider; 
	}
	int getPlayerStreak() { 
	    return m_playerStreak; 
	}
	int getPlayerDeathEffect() {
	    return m_playerDeathEffect; 
	}
	int getPlayerColor() {
	    return m_playerColor;
	}
	int getPlayerColor2() {
	    return m_playerColor2;
	}
	bool getPlayerGlow() {
	    return m_playerGlow; 
	}
	IconType getPlayerIconType() {
	    return m_playerIconType; 
	}
	void setPlayerFrame(int id) {
	    m_playerFrame = id;
	    m_playerFrameRand1 = id + m_playerFrameRand2;
	}
	void setPlayerShip(int id) {
	    m_playerShip = id;
	    m_playerShipRand1 = id + m_playerShipRand2;
	}
	void setPlayerBall(int id) {
	    m_playerBall = id;
	    m_playerBallRand1 = id + m_playerBallRand2;
	}
	void setPlayerBird(int id) {
	    m_playerBird = id;
	    m_playerBirdRand1 = id + m_playerBirdRand2;
	}
	void setPlayerDart(int id) {
	    m_playerDart = id;
	    m_playerDartRand1 = id + m_playerDartRand2;
	}
	void setPlayerRobot(int id) {
	    m_playerRobot = id;
	    m_playerRobotRand1 = id + m_playerRobotRand2;
	}
	void setPlayerSpider(int id) {
	    m_playerSpider = id;
	    m_playerSpiderRand1 = id + m_playerSpiderRand2;
	}
	void setPlayerStreak(int id) {
	    m_playerStreak = id;
	    m_playerStreakRand1 = id + m_playerStreakRand2;
	}
	void setPlayerDeathEffect(int id) {
	    m_playerDeathEffect = id;
	    m_playerDeathEffectRand1 = id + m_playerDeathEffectRand2;
	}
	void setPlayerColor(int id) {
	    m_playerColor = id;
	    m_playerColorRand1 = id + m_playerColorRand2;
	}
	void setPlayerColor2(int id) {
	    m_playerColor2 = id;
	    m_playerColor2Rand1 = id + m_playerColor2Rand2;
	}
	void setPlayerGlow(bool v) {
	    m_playerGlow = v;
	}
	void setPlayerIconType(IconType v) {
	    m_playerIconType = v;
	}
	bool isColorUnlocked(int _id, bool _type) = win 0xc53f0;
	bool isIconUnlocked(int _id, IconType _type) = win 0xc4fc0;
	cocos2d::ccColor3B colorForIdx(int _id) = win 0xc8d10;
	static GameManager* sharedState() = win 0xc4a50;
	void reloadAll(bool bSwitch, bool bFullscreen, bool bReloadedInSession) = win 0xce950;
	void setQuality(cocos2d::TextureQuality quality) {
	    m_quality = quality;
	}
	bool getGameVariable(const char* key) = win 0xc9d30;
	void setGameVariable(const char* key, bool value) = win 0xc9b50;
	int getIntGameVariable(const char* key) = win 0xca330;
	void setIntGameVariable(const char* key, int value) = win 0xca230;
	void toggleGameVariable(const char* key) = win 0xc9e90;
	bool getUGV(const char* key) = win 0xca0d0;
	void fadeInMusic(const char* filename) = win 0xc4bd0;
	PlayLayer* getPlayLayer() { 
	    return m_playLayer;
	}
	LevelEditorLayer* getEditorLayer() { 
	    return m_levelEditorLayer;
	}
	bool getGameVariableDefault(const char* key, bool defaultValue) {
	    auto object = reinterpret_cast<cocos2d::CCString*>(m_valueKeeper->objectForKey(gd::string("gv_") + key));
	    if (object == nullptr)
	        return defaultValue;
	    return object->boolValue();
	}
	int getIntGameVariableDefault(const char* key, int defaultValue) {
	    auto object = reinterpret_cast<cocos2d::CCString*>(m_valueKeeper->objectForKey(gd::string("gv_") + key));
	    if (object == nullptr)
	        return defaultValue;
	    return object->intValue();
	}
	static void returnToLastScene(GJGameLevel* level) = win 0xce6a0;

	bool m_switchModes;
	bool m_toFullscreen;
	bool m_reloading;
	bool m_unknown;
	PAD = win 0x4;
	cocos2d::CCDictionary* m_valueKeeper;
	cocos2d::CCDictionary* m_unlockValueKeeper;
	cocos2d::CCDictionary* m_customObjectDict;
	PAD = win 0x4;
	double m_unknown;
	PAD = win 0x10;
	double m_unknown2;
	PAD = win 0x8;
	bool m_loaded; 
	gd::string m_unknown;     
	PlayLayer* m_playLayer;    
	LevelEditorLayer* m_levelEditorLayer;  
	PAD = win 0x14;
	gd::string m_playerUDID;  
	gd::string m_playerName;  
	bool m_commentsEnabled;    
	int m_playerUserIDRand1;   
	int m_playerUserIDRand2;   
	int m_playerUserID;        
	float m_backgroundMusicVolume; 
	float m_effectsVolume;         
	int m_timeOffset;          
	PAD = win 0x1c;
	int m_playerFrameRand1;    
	int m_playerFrameRand2;    
	int m_playerFrame;         
	int m_playerShipRand1;     
	int m_playerShipRand2;     
	int m_playerShip;          
	int m_playerBallRand1;     
	int m_playerBallRand2;     
	int m_playerBall;          
	int m_playerBirdRand1;     
	int m_playerBirdRand2;     
	int m_playerBird;          
	int m_playerDartRand1;     
	int m_playerDartRand2;     
	int m_playerDart;          
	int m_playerRobotRand1;    
	int m_playerRobotRand2;    
	int m_playerRobot;         
	int m_playerSpiderRand1;   
	int m_playerSpiderRand2;   
	int m_playerSpider;        
	int m_playerColorRand1;    
	int m_playerColorRand2;    
	int m_playerColor;         
	int m_playerColor2Rand1;   
	int m_playerColor2Rand2;   
	int m_playerColor2;        
	int m_playerStreakRand1;   
	int m_playerStreakRand2;   
	int m_playerStreak;        
	int m_playerDeathEffectRand1;  
	int m_playerDeathEffectRand2;  
	int m_playerDeathEffect;       
	PAD = win 0x8;
	int m_secretNumberRand1;   
	int m_secretNumberRand2;   
	bool m_playerGlow;         
	IconType m_playerIconType; 
	bool m_unknown2;           
	bool m_showSongMarkers;    
	bool m_showBPMMarkers;     
	bool m_recordGameplay;     
	bool m_showProgressBar;    
	bool m_performanceMode;    
	bool m_clickedGarage;      
	bool m_clickedEditor;      
	bool m_clickedName;        
	bool m_clickedPractice;    
	bool m_showedEditorGuide;  
	bool m_showedRateDiffDialog;   
	bool m_showedRateStarDialog;   
	bool m_showedLowDetailDialog;  
	PAD = win 0x30;
	int m_bootups;
	bool m_hasRatedGame;
	bool m_unk0;
	bool m_unk1;
	bool m_unk2;
	bool m_gameCenterEnabled;
	bool m_smoothFix;
	PAD = win 0x10;
	int m_resolution;
	cocos2d::TextureQuality m_quality; 
}

class GameSoundManager {
	static GameSoundManager* sharedState() = win 0x24800;

	cocos2d::CCDictionary* m_dictionary1;
	cocos2d::CCDictionary* m_dictionary2;
	PAD = win 0xc;
	bool m_preloaded;
	PAD = win 0x4;
	gd::string m_filePath;
}

class GameStatsManager {
	static GameStatsManager* sharedState() = win 0xf1e50;

	PAD = win 0x28;
	cocos2d::CCDictionary* m_dailyChests;
	cocos2d::CCDictionary* m_worldAdvertChests;
	cocos2d::CCDictionary* m_activeChallenges;
	cocos2d::CCDictionary* m_upcomingChallenges;
	PAD = win 0xc;
	cocos2d::CCDictionary* m_playerStats;
	PAD = win 0x10;
	cocos2d::CCDictionary* m_completedLevels;
	cocos2d::CCDictionary* m_verifiedUserCoins;
	cocos2d::CCDictionary* m_pendingUserCoins;
	cocos2d::CCDictionary* m_purchasedItems;
	cocos2d::CCDictionary* m_onlineCurrencyScores;
	cocos2d::CCDictionary* m_mainCurrencyScores;
	cocos2d::CCDictionary* m_gauntletCurrencyScores;
	cocos2d::CCDictionary* m_timelyCurrencyScores;
	cocos2d::CCDictionary* m_onlineStars;
	cocos2d::CCDictionary* m_timelyStars;
	cocos2d::CCDictionary* m_gauntletDiamondScores;
	cocos2d::CCDictionary* m_timelyDiamondScores;
	cocos2d::CCDictionary* m_unusedCurrencyAwardDict;
	cocos2d::CCDictionary* m_challengeDiamonds;
	cocos2d::CCDictionary* m_completedMappacks;
	cocos2d::CCDictionary* m_weeklyChest;
	cocos2d::CCDictionary* m_treasureRoomChests;
	int m_bonusKeySeed;
	int m_bonusKeyRand;
	int m_bonusKey;
	cocos2d::CCDictionary* m_miscChests;
}

class GJAccountManager {
	static GJAccountManager* sharedState() = win 0x107d50;

	PAD = win 0x4;
	gd::string m_password;
	gd::string m_username;
}

class GJUserScore {
	IconType getIconType() const { 
	    return this->m_iconType; 
	}
	int getPlayerCube() const { 
	    return this->m_playerCube; 
	}
	int getPlayerShip() const { 
	    return this->m_playerShip;
	}
	int getPlayerBall() const {
	    return this->m_playerBall;
	}
	int getPlayerUfo() const {
	    return this->m_playerUfo;
	}
	int getPlayerWave() const { 
	    return this->m_playerWave; 
	}
	int getPlayerRobot() const { 
	    return this->m_playerRobot; 
	}
	int getPlayerSpider() const { 
	    return this->m_playerSpider; 
	}
	int getPlayerStreak() const { 
	    return this->m_playerStreak; 
	}
	bool getGlowEnabled() const { 
	    return this->m_glowEnabled; 
	}
	int getPlayerExplosion() const { 
	    return this->m_playerExplosion; 
	}
	int getPlayerColor1() const { 
	    return this->m_color1;
	}
	int getPlayerColor2() const { 
	    return this->m_color2; 
	}
	gd::string getPlayerName() const { 
	    return this->m_userName; 
	}
	
	gd::string m_userName;
    gd::string m_userUDID;
    int m_scoreType; 
    int m_userID;
    int m_accountID;

    int m_stars;
    int m_diamonds;
    int m_demons;
    int m_playerRank;
    int m_creatorPoints; 
    int m_secretCoins;
    int m_userCoins;

    int m_iconID; 
    int m_color1;
    int m_color2;
    int m_special;
    IconType m_iconType;

    int m_messageState;
    int m_friendStatus;
    int m_commentHistoryStatus;

    gd::string m_youtubeURL;
    gd::string m_twitterURL;
    gd::string m_twitchURL;

    int m_playerCube;
    int m_playerShip;
    int m_playerBall;
    int m_playerUfo;
    int m_playerWave;
    int m_playerRobot;
    int m_playerSpider;
    int m_playerStreak;
    bool m_glowEnabled;

    int m_playerExplosion;

    int m_modBadge;
    int m_globalRank;

    int m_friendReqStatus;
    int m_newMsgCount;
    int m_friendReqCount;
    bool m_isBlocked;

    gd::string m_lastScoreAge;
}

class GManager {
	void save() = win 0x29250;
	void setup() = win 0x28f60;
	void encodeDataTo(DS_Dictionary* data) {}
	void dataLoaded(DS_Dictionary* data) {}
	void firstLoad() {}

	gd::string m_fileName;
	bool m_setup;
	bool m_saved;
	bool m_quickSave;
}

class MusicDownloadManager {
	static MusicDownloadManager* sharedState() = win 0x1945b0;
	gd::string pathForSong(int id) = win 0x1960e0;
	cocos2d::CCArray* getDownloadedSongs() = win 0x195640;
	void songStateChanged() = win 0x194d90;
}

class ObjectToolbox {
	static ObjectToolbox* sharedState() = win 0x198a80;
	
	float gridNodeSizeForKey(int key) = win 0x1dc920;
	const char* perspectiveBlockFrame(int key) = win 0x1dcd00;
	void addObject(int id, const char* frame) {
	    m_frameToKeyDict->setObject(cocos2d::CCString::createWithFormat("%i", id), frame);
	    m_feyToFrameDict->setObject(cocos2d::CCString::create(frame), id);
	}
}

class CustomSongWidget : cocos2d::CCNode, MusicDownloadDelegate, FLAlertLayerProtocol {
	void updateSongObject(SongInfoObject* song) = win 0x69280;
	void FLAlert_Clicked(FLAlertLayer*, bool) {}
	void loadSongInfoFinished(SongInfoObject*) {}

    SongInfoObject* m_songInfo;
    PAD = win 0x1C;
    CCMenuItemSpriteExtra *m_downloadBtn;
    PAD = win 0x30;
}

class LoadingCircle {
	static LoadingCircle* create() = win 0x2a0d0;
	void show() = win 0x2a290;
	void setParentLayer(cocos2d::CCLayer* layer) {
	    m_parentLayer = layer;
	}
	void setFade(bool fade) {
	    m_fade = fade;
	}
	void fadeAndRemove() = win 0x2a370;

	cocos2d::CCSprite* m_sprite;
	cocos2d::CCLayer* m_parentLayer;
	bool m_fade;
}

class GameObject {
	void setScaleX(float scale) = win 0xe5050;
	void setScaleY(float scale) = win 0xe50e0;
	void setScale(float scale) = win 0xe5170;
	void setPosition(const cocos2d::CCPoint& pos) = win 0xe4de0;
	void setVisible(bool visible) = win 0xe57c0;
	void setRotation(float rotation) = win 0xe4ed0;
	bool initWithTexture(cocos2d::CCTexture2D* texture) = win 0xcfa80;
	void setChildColor(const cocos2d::ccColor3B& color) = win 0xee900;
	void setOpacity(GLubyte opacity) = win 0xe53c0;
	static GameObject* createWithKey(int key) = win 0xcf4f0;
	static GameObject* createWithFrame(const char* frame) = win 0xcf8f0;
	GameObject* objectFromString(gd::string str, bool unknown) = win 0xebe50;
	void deselectObject() = win 0xeee50;
	gd::string getSaveString() = win 0xed0c0;
	void addToGroup(int id) = win 0xeb8d0;
	void removeFromGroup(int id) = win 0xeb930;
	cocos2d::CCPoint getBoxOffset() = win 0xef350;
	cocos2d::CCPoint getStartPosition() { 
	    return m_startPosition;
	}
	void setStartPosition(cocos2d::CCPoint const& p) {
	    m_startPosition = p;
	}
	unsigned int getUniqueID() {
	    return m_uniqueID;
	}
	short getGroupID(int ix) {
	    return m_groups[ix];
	}
	short getGroupIDCount() {
	    return m_groupCount; 
	}
	gd::vector<short> getGroupIDs() {
	    std::vector<short> res;
	
	    if (m_groups && m_groups[0])
	        for (auto i = 0; i < m_groupCount; i++)
	            res.push_back(m_groups[i]);
	
	    return res;
	}
	int getGameZOrder() {
	    return m_gameZOrder;
	}
	void setGameZOrder(int z) {
	    m_gameZOrder = z;
	}
	void setGameObjType(GameObjectType t) {
	    m_objectType = t;
	}
	GJSpriteColor* getBaseColor() {
	    return m_baseColor;
	}
	GJSpriteColor* getDetailColor() {
	    return m_detailColor;
	}
	void updateCustomScale(float scale) = win 0xe5340;
	void setRScale(float scale) = win 0xe5280;
	void setObjectColor(const cocos2d::ccColor3B& color) = win 0xee620;
	void selectObject(const cocos2d::ccColor3B& color) = win 0xee960;
	cocos2d::CCRepeatForever* createRotateAction(float f, int n) = win 0xe49b0;
	void setMyAction(cocos2d::CCAction* pAction) = win 0xd1b90;
	
	bool m_unk3;				
	bool m_isBlueMaybe;		
	float m_unk2;				
	float m_unk;				
	float m_unk3f;				
	float m_unk4;				
	bool m_unkidk;				
	float m_animSpeed2;		
	bool m_isEffectObject;		
	bool m_randomisedAnimStart;
	float m_animSpeed;			
	bool m_blackChild;			
	bool m_unkOutlineMaybe;	
	float m_blackChildOpacity;	
	bool field_21C;	
	bool m_editor;	
	bool m_groupDisabled;	
	bool m_colourOnTop;	
	GJSpriteColor* m_mainColourMode;		
	GJSpriteColor* m_secondaryColourMode;	
	bool m_col1;	
	bool m_col2;	
	cocos2d::CCPoint m_startPosOffset; 
	float m_unkRotationField;	
	bool m_tintTrigger;	
	bool m_isFlippedX; 	
	bool m_isFlippedY; 	
	cocos2d::CCPoint m_boxOffset; 
	bool m_isOriented; 
	cocos2d::CCPoint m_boxOffset2; 
	OBB2D* m_objectOBB2D;	
	bool m_oriented;	
	cocos2d::CCSprite* m_glowSprite;	
	bool m_notEditor;	
	cocos2d::CCAction* m_myAction; 
	bool m_unk1;	
	bool m_runActionWithTag;	
	bool m_objectPoweredOn;	
	cocos2d::CCSize m_objectSize; 
	bool m_trigger;	
	bool m_active; 	
	bool m_animationFinished;	
	cocos2d::CCParticleSystemQuad* m_particleSystem;	
	gd::string m_effectPlistName; 
	bool m_particleAdded;	
	bool m_hasParticles;	
	bool m_unkCustomRing;	
	cocos2d::CCPoint m_portalPosition;	
	bool m_unkParticleSystem;	
	cocos2d::CCRect m_objectTextureRect;	
	bool m_textureRectDirty;	
	float m_rectXCenterMaybe;	
	cocos2d::CCRect m_objectRect2; 
	bool m_isObjectRectDirty; 
	bool m_isOrientedRectDirty; 
	bool m_hasBeenActivated; 
	bool m_hasBeenActivatedP2; 
	PAD = win 0x10;
	bool m_sawIsDisabled; 
	PAD = win 0x4;
	cocos2d::CCSprite* m_detailSprite; 
	PAD = win 0x8;
	bool m_isRotatedSide; 
	float m_unk2F4;
	float m_unk2F8;
	int m_uniqueID; 
	GameObjectType m_objectType; 
	int m_section; 
	bool m_touchTriggered; 
	bool m_spawnTriggered; 
	cocos2d::CCPoint m_startPosition; 
	gd::string m_textureName; 
	bool m_unk32C;
	bool m_unk32D;
	PAD = win 0xc;
	float m_unk33C;
	float m_unk340;
	PAD = win 0x10;
	bool m_isGlowDisabled; 
	int m_targetColorID;	
	float m_scale; 
	int m_objectID; 
	PAD = win 0x4;
	bool m_unk368;
	bool m_unk369;
	bool m_unk36A;
	bool m_isDontEnter; 
	bool m_isDontFade; 
	int m_defaultZOrder; 
	PAD = win 0x17;
	bool m_unk38C;
	bool m_unk38D;
	bool m_unk38E;
	float m_unk390;
	PAD = win 0x14;
	GJSpriteColor* m_baseColor; 
	GJSpriteColor* m_detailColor; 
	PAD = win 0x4;
	ZLayer m_defaultZLayer; 
	ZLayer m_zLayer; 
	int m_gameZOrder; 
	gd::string m_unk3C0;
	bool m_showGamemodeBorders;
	bool m_unk3D9;
	bool m_isSelected; 
	int m_globalClickCounter; 
	PAD = win 0x8;
	bool m_unknownLayerRelated;	
	float m_multiScaleMultiplier;	
	bool m_isGroupParent; 
	short* m_groups; 
	short m_groupCount; 
	PAD = win 0x12;
	int m_editorLayer; 
	int m_editorLayer2; 
	int m_unk414;
	PAD = win 0xc;
	cocos2d::CCPoint m_firstPosition; 
	PAD = win 0x1c;
	bool m_highDetail; 
	ColorActionSprite* m_colorActionSprite1; 
	ColorActionSprite* m_colorActionSprite2; 
	GJEffectManager* m_effectManager; 
	PAD = win 0x10;
}

class GJSpriteColor : cocos2d::CCNode {
    int m_colorID; 
    int m_defaultColorID; 
    float m_unk_0F4;
    cocos2d::ccHSVValue m_hsv;
    bool unk_108;
    float unk_10C;
    bool unk_110;
}

class EffectGameObject : GameObject {
	static EffectGameObject* create(const char* sprName) = win 0x253c30;
	gd::string getSaveString() = win 0x257560;
	void updateLabel() {
		auto label = reinterpret_cast<cocos2d::CCLabelBMFont*>(this->getChildByTag(999));
		if (label) {
			switch (this->m_objectID) {
				// instant count
				case 0x713:
					label->setString(
						cocos2d::CCString::createWithFormat("%i", this->m_targetItemID)->getCString()
					);
					break;
				//   color,    pulse
				case 899: case 1006: {
					int target = this->m_objectID == 1006 ? m_targetGroupID : m_targetColorID;
					if (target > 999) {
						label->setString(GJSpecialColorSelect::textForColorIdx(target));
					} else {
						label->setString(
							cocos2d::CCString::createWithFormat("%i", target)->getCString()
						);
					}
					label->limitLabelWidth(30.0f, 0.5f, 0.0f);
					} break;
				default:
					label->setString(
						cocos2d::CCString::createWithFormat("%i", this->m_targetGroupID)->getCString()
					);
			}
		}
	}

    cocos2d::ccColor3B m_colColor; 
    float m_duration; 
    float m_opacity; 
    int m_targetGroupID; 
    int m_centerGroupID; 
    float m_shakeStrength; 
    float m_shakeInterval; 
    bool m_tintGround;
    bool m_playerColor1; 
    bool m_playerColor2; 
    bool m_blending; 
    cocos2d::CCPoint m_move; 
    EasingType m_easingType; 
    float m_easingRate; 
    bool m_lockToPlayerX; 
    bool m_lockToPlayerY; 
    bool m_useTarget; 
    MoveTargetType m_moveTargetType; 
    int m_rotateDegrees; 
    int m_times360; 
    bool m_lockObjectRotation; 
    cocos2d::CCPoint m_followMod; 
    bool UndocuementedLevelProperty74;  
    float m_followYSpeed; 
    float m_followYDelay; 
    int m_followYOffset; 
    float m_followYMaxSpeed; 
    float m_fadeInTime; 
    float m_holdTime; 
    float m_fadeOutTime; 
    int m_pulseHSVMode; 
    int m_pulseGroupMode; 
    cocos2d::ccHSVValue m_hsvValue; 
    int m_copyColorID; 
    bool m_copyOpacity; 
    bool m_pulseMainOnly; 
    bool m_pulseDetailOnly; 
    bool m_pulseExclusive; 
    bool m_activateGroup; 
    bool m_touchHoldMode; 
    TouchToggleMode m_touchToggleMode; 
    bool m_touchDualMode; 
    int m_animationID; 
    float m_spawnDelay; 
    cocos2d::CCPoint m_spawnPosition;
    bool m_multiTrigger; 
    bool m_editorDisabled; 
    int m_targetCount; 
    bool m_subtractCount;
    ComparisonType m_comparisonType; 
    bool m_multiActivate; 
    bool m_triggerOnExit; 
    int m_blockBID; 
    bool m_dynamicBlock; 
    int m_targetItemID; 
    int m_pickupMode;
    PAD = win 0x24;
}

class PointNode {
	static PointNode* create(cocos2d::CCPoint point) = win 0x14ec80;

    cocos2d::CCPoint m_point;
}

class ChallengesPage {
	static ChallengesPage* create() = win 0x3e050;
}

class ConfigureHSVWidget : cocos2d::CCNode {
	cocos2d::CCLabelBMFont* m_hueLabel;
	cocos2d::CCLabelBMFont* m_saturationLabel;
	cocos2d::CCLabelBMFont* m_brightnessLabel;

	Slider* m_hueSlider;
	Slider* m_saturationSlider;
	Slider* m_brightnessSlider;

	cocos2d::ccHSVValue m_value;
}

class ColorSelectPopup : FLAlertLayer, ColorPickerDelegate, TextInputDelegate, GJSpecialColorSelectDelegate {
    cocos2d::extension::CCControlColourPicker* m_colorPicker; 
    cocos2d::CCLabelBMFont* m_unk1DC;
    cocos2d::CCLabelBMFont* m_label; 
    Slider* m_unk1E4;
    Slider* m_slider;
    EffectGameObject* m_effectGameObject; 
    cocos2d::CCArray* m_unk1F0;
    CCMenuItemToggler* m_toggler1; 
    CCMenuItemToggler* m_toggler2; 
    unsigned int m_rgbLastColor; 
    cocos2d::CCSprite* m_unk200;
    cocos2d::CCSprite* m_unk204;
    unsigned int m_bgrColor; 
    GJColorSetupLayer* m_colorSetupLayer; 
    float m_fadeTime; 
    int m_olayerColor; 
    bool m_isBlending; 
    float m_opacity; 
    ColorAction* m_colorAction; 
    CCTextInputNode* m_textInput1;
    bool m_unk228;
    bool m_isTouchTrigger; 
    bool m_unk22A;
    bool m_isMultipleColorTrigger; 
    bool m_unk22C;
    bool m_isColorTrigger; 
    int m_colorID; 
    bool m_unk234;
    int m_copyChannelID; 
    bool m_copyOpacity;
    ConfigureHSVWidget* m_configurehsvwidget; 
    PAD = win 0x10;
    cocos2d::CCArray* m_unk254;
    cocos2d::CCArray* m_unk258;
    CCTextInputNode* m_textInput2; 
    PAD = win 0x4;
    CCMenuItemToggler* m_toggler3;
    CCMenuItemToggler* m_toggler4;
    PAD = win 0x8;
    cocos2d::CCArray* m_unk274;
    bool m_spawnTrigger; 
    bool m_multiTrigger; 
    bool m_copyColor; 

	virtual void colorValueChanged(cocos2d::ccColor3B color) = win 0x46ee0;
	// cocos2d::ccColor3B getPickerColor() {
	//     return *reinterpret_cast<cocos2d::ccColor3B*>(
	//         reinterpret_cast<uintptr_t>(m_colorPicker) + 0x144
	//     );
	// }
	// void setPickerColor(cocos2d::ccColor3B color) {
	//     const static auto address = GetProcAddress(
	//         GetModuleHandleA("libExtensions.dll"),
	//         "?setColorValue@CCControlColourPicker@extension@cocos2d@@UAEXABU_ccColor3B@3@@Z"
	//     );
	//     reinterpret_cast<void (__thiscall*)(
	//         cocos2d::extension::CCControlColourPicker*, const cocos2d::ccColor3B&
	//     )>(address)(m_colorPicker, color);
	// }
}

class CreateGuidelinesLayer : FLAlertLayer, FLAlertLayerProtocol {
    PAD = win 0x24;
    gd::string m_guidelineString;

	void onStop(cocos2d::CCObject* pSender) = win 0x4d2c0;
}

class DailyLevelPage {
	static DailyLevelPage* create(bool weekly) = win 0x6a860;
}

class FLAlertLayer {
	void onEnter() = win 0x23750;
	void registerWithTouchDispatcher() = win 0x236f0;
	bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) = win 0x233c0;
	void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) = win 0x23510;
	void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) = win 0x23450;
	void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) = win 0x234c0;
	void keyDown(cocos2d::enumKeyCodes key) = win 0x23250;
	void keyBackClicked() = win 0x232c0;
	void show() = win 0x23560;
	FLAlertLayer() = win 0x224b0;
	void onBtn1(cocos2d::CCObject* btn) = win 0x23340;
	void onBtn2(cocos2d::CCObject* btn) = win 0x23380;
	static FLAlertLayer* create(FLAlertLayerProtocol* target, const char* title, const char* btn1, const char* btn2, gd::string caption) = win 0x22680;
	static FLAlertLayer* create(FLAlertLayerProtocol* target, const char* title, const char* btn1, const char* btn2, float width, gd::string caption) = win 0x22730;
	cocos2d::CCLayer* getLayer() {
	    return m_mainLayer;
	}
	cocos2d::CCLayer* getButtonMenu() {
	    return m_buttonMenu;
	}
}

class GJGarageLayer {
	static GJGarageLayer* create() = win 0x125220;
	void onSelectTab(cocos2d::CCObject* pSender) = win 0x127c30;
	void onPlayerIcon(cocos2d::CCObject* pSender) = win 0x127f30;
	void onShipIcon(cocos2d::CCObject* pSender) = win 0x1281e0;
	void onBallIcon(cocos2d::CCObject* pSender) = win 0x1282a0;
	void onBirdIcon(cocos2d::CCObject* pSender) = win 0x128360;
	void onDartIcon(cocos2d::CCObject* pSender) = win 0x128420;
	void onRobotIcon(cocos2d::CCObject* pSender) = win 0x1286d0;
	void onSpiderIcon(cocos2d::CCObject* pSender) = win 0x128890;
	void onShards(cocos2d::CCObject* pSender) = win 0x12ad70;
	void onBack(cocos2d::CCObject* pSender) = win 0x12adf0;
	void onShop(cocos2d::CCObject* pSender) = win 0x12ad90;
}

class ProfilePage : FLAlertLayer, FLAlertLayerProtocol, LevelCommentDelegate, CommentUploadDelegate, UserInfoDelegate, UploadActionDelegate, UploadPopupDelegate, LeaderboardManagerDelegate {
	static ProfilePage* create(int accountID, bool idk) = win 0x20ee50;

	PAD = win 0x4;
	int m_accountID;
}

class SongInfoLayer {
	static SongInfoLayer* create(int songID) = win 0x250520;
	static SongInfoLayer* create(gd::string songName, gd::string artistName, gd::string downloadLink, gd::string artistNG, gd::string artistYT, gd::string artistFB) = win 0x250830;

	gd::string m_downloadLink;
	gd::string m_artistNewgrounds;
	gd::string m_artistYoutube;
	gd::string m_artistFacebook;
}

class KeybindingsLayer {
	void onClose(cocos2d::CCObject* pSender) = win 0x49c60;
	void onPrevPage(cocos2d::CCObject* pSender) = win 0x153cd0;
	void onNextPage(cocos2d::CCObject* pSender) = win 0x153cc0;
	void goToPage(int page) = win 0x153ce0;

	int m_currentPage;
	int m_itemCount;
	int m_pageCount;
	cocos2d::CCDictionary* m_pages;
	cocos2d::CCDictionary* m_unused;
	cocos2d::CCNode* m_leftArrow;
	cocos2d::CCNode* m_rightArrow;
}

class MoreOptionsLayer {
	static MoreOptionsLayer* create() = win 0x1de850;
	void addToggle(const char* name, const char* key, const char* info) = win 0x1df6b0;
}

class GJColorSetupLayer {

}

class OptionsLayer : GJDropDownLayer, FLAlertLayerProtocol {
	static OptionsLayer* create() = mac 0x43db60, win 0x1dd310;
	static OptionsLayer* addToCurrentScene(bool noTransition) {
	    auto layer = OptionsLayer::create();
	    cocos2d::CCDirector::sharedDirector()
	        ->getRunningScene()
	        ->addChild(layer);
	    layer->showLayer(noTransition);
	    return layer;
	}
}

class CreatorLayer {
	static CreatorLayer* create() = win 0x4dda0;
}

class LeaderboardsLayer {
	static LeaderboardsLayer* create(LeaderboardState state) = win 0x158710;
}

class LevelBrowserLayer {
	bool init(GJSearchObject* pSearch) = win 0x15a040;
	void setupLevelBrowser(cocos2d::CCArray* levels) = win 0x15bb40;
	static LevelBrowserLayer* create(GJSearchObject* pSearch) = win 0x159fa0;
	static void scene(GJSearchObject* pSearch) {
	    auto pScene = cocos2d::CCScene::create();
	
	    pScene->addChild(LevelBrowserLayer::create(pSearch));
	
	    cocos2d::CCDirector::sharedDirector()->replaceScene(
	        cocos2d::CCTransitionFade::create(.5f, pScene)
	    );
	}

    PAD = win 0x18;
    GJListLayer* m_list;   
}

class InfoLayer : FLAlertLayer, LevelCommentDelegate, CommentUploadDelegate, FLAlertLayerProtocol  {
	bool init(GJGameLevel* level, GJUserScore* score) = win 0x14f5a0;
	void setupCommentsBrowser(cocos2d::CCArray* comments) = win 0x152270;

    PAD = win 0x2c;
    GJCommentListLayer* m_list; 
}

class LevelInfoLayer : cocos2d::CCLayer, LevelDownloadDelegate, LevelUpdateDelegate, RateLevelDelegate, LikeItemDelegate, FLAlertLayerProtocol, LevelDeleteDelegate, NumberInputDelegate, SetIDPopupDelegate {
	static LevelInfoLayer* create(GJGameLevel* level) = win 0x175d50;
	void onGarage(cocos2d::CCObject* pSender) = win 0x177c10;

    PAD = win 0x4;
    cocos2d::CCMenu* m_playBtnMenu;
    GJGameLevel* m_level;
    cocos2d::CCArray* m_unknown;
    CCMenuItemSpriteExtra* m_likeBtn;
    CCMenuItemSpriteExtra* m_starRateBtn;
    CCMenuItemSpriteExtra* m_demonRateBtn;
    PAD = win 0x4;
    CCMenuItemToggler* m_toggler;
    cocos2d::CCLabelBMFont* m_label0;
    cocos2d::CCLabelBMFont* m_label1;
    cocos2d::CCLabelBMFont* m_label2;
    cocos2d::CCLabelBMFont* m_label3;
    cocos2d::CCLabelBMFont* m_label4;
    cocos2d::CCLabelBMFont* m_label5;
    CCMenuItemSpriteExtra* m_cloneBtn;
    PAD = win 0x4;
}

class EditLevelLayer {
	static EditLevelLayer* create(GJGameLevel* level) = win 0x6f530;
	static void scene(GJGameLevel* level) {
	    auto pScene = cocos2d::CCScene::create();
	
	    pScene->addChild(EditLevelLayer::create(level));
	
	    cocos2d::CCDirector::sharedDirector()->replaceScene(
	        cocos2d::CCTransitionFade::create(.5f, pScene)
	    );
	}

    cocos2d::CCMenu* m_buttonMenu;
    GJGameLevel* m_level;
    TextArea* m_descriptionInput;
    cocos2d::CCArray* m_someArray;
    cocos2d::CCLabelBMFont* m_someFont;
}

class TextArea {
	static TextArea* create(const char* font, bool disableColor, gd::string caption, float scale, float width, float height, cocos2d::CCPoint const& anchorPos) = win 0x33270;
	void colorAllCharactersTo(cocos2d::ccColor3B color) = win 0x33830;
	void setString(gd::string const& str) = win 0x33480;
}

class LevelSearchLayer {
	static LevelSearchLayer* create() = win 0x17d9c0;
}

class LevelSelectLayer : cocos2d::CCLayer {
	static LevelSelectLayer* create(int lvl) = win 0x185500;
    PAD = win 0x10;
    BoomScrollLayer* m_scrollLayer;    
}

class LoadingLayer : cocos2d::CCLayer {
	static LoadingLayer* create(bool fromReload) = win 0x18bfe0;
	void setFromRefresh(bool value) {
	    m_fromRefresh = value;
	}

	PAD = win 0x4;
	int m_loadIndex;
	cocos2d::CCLabelBMFont* m_caption;
	PAD = win 0x4;
}

class MenuLayer {
	bool init() = win 0x1907b0;
	MenuLayer* node() = win 0x190550;
	static cocos2d::CCScene* scene(bool options) = win 0x190720;
}

class TableView {
	TableView() {}
	inline TableView(cocos2d::CCRect rect) : CCScrollLayerExt(rect) {}
	void reloadData() = win 0x317e0;
	static TableView* create(TableViewDelegate* delegate, TableViewDataSource* dataSource, cocos2d::CCRect rect) = win 0x30ed0;

	bool m_touchDown2;
	PAD = win 0x4;
	cocos2d::CCPoint m_touchStartPosition2;	
	cocos2d::CCPoint m_unknown2;	
	cocos2d::CCPoint m_touchPosition2;	
	PAD = win 0x4;
	bool m_touchMoved;
	cocos2d::CCArray* m_cellArray;	
	cocos2d::CCArray* m_array2;	
	cocos2d::CCArray* m_array3;	
	TableViewDelegate* m_delegate;	
	TableViewDataSource* m_dataSource;	
	PAD = win 0x10;
	float m_touchLastY;	
	PAD = win 0x4;
}

class BoomListView {
	float cellHeightForRowAtIndexPath(CCIndexPath& path, TableView* view) = win 0x10e50;
	unsigned int numberOfRowsInSection(unsigned int section, TableView* view) = win 0x10e60;
	TableViewCell* cellForRowAtIndexPath(CCIndexPath& path, TableView* view) = win 0x10e70;
	~BoomListView() {
	    CC_SAFE_RELEASE(m_entries);
	}
	bool init(cocos2d::CCArray* entries, BoomListType btype, float width, float height) = win 0x10c20;
	void setupList() = win 0x10dc0;
	TableViewCell* getListCell(const char* key) = win 0x10ed0;
	void loadCell(TableViewCell* cell, unsigned int index) = win 0x10ff0;
	void didSelectRowAtIndexPath(CCIndexPath&, TableView*) {}

	TableView* m_tableView;		
	cocos2d::CCArray* m_entries;	
	BoomListType m_type; 			
	float m_height;				
	float m_width;					
	float m_itemSeparation;		
	PAD = win 0x4;
}

class CCContentLayer {
	static CCContentLayer* create(cocos2d::ccColor4B const& color, float width, float height) = win 0x172a0;
}

class CCIndexPath : cocos2d::CCObject {
	static CCIndexPath* create(unsigned int idk1, int idk2) = win 0x30e40;

	int m_unknown1;
	int m_unknown2;
}

class CCScrollLayerExt : cocos2d::CCLayer {
	CCScrollLayerExt::CCScrollLayerExt(cocos2d::CCRect rect) = win 0x1b020;
	void preVisitWithClippingRect(cocos2d::CCRect rect) = win 0x1c000;
	void postVisit() = win 0x1c090;
	void visit() = win 0x1bed0;
	void registerWithTouchDispatcher() = win 0x1b980;
	CCScrollLayerExt() {}
	CCScrollLayerExt(cocos2d::CCRect) {
		//TODO: do this
	}
	static CCScrollLayerExt* create(cocos2d::CCRect rect, bool vertical) {
	    auto pRet = new CCScrollLayerExt(rect);
	
	    if (pRet) {
	        pRet->autorelease();
	        pRet->m_disableVertical = !vertical;
	        pRet->m_disableHorizontal = vertical;
	        pRet->m_cutContent = true;
	        return pRet;
	    }
	
	    CC_SAFE_DELETE(pRet);
	    return nullptr;
	}
	void scrollLayer(float scroll) = win 0x1be20;
	void moveToTop() = win 0x1b4a0;
	void moveToTopWithOffset(float fOffset) = win 0x1b420;
	float getMinY() {
		//TODO: fix this
	    // return this->getContentSize().height -
	    //     this->m_contentLayer->getContentSize().height -
	    //     this->m_scrollLimitTop;
	}
	float getMaxY() {
	    return this->m_scrollLimitBottom;
	}
	bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) = win 0x1b9b0;
	void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) = win 0x1bcc0;
	void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) = win 0x1baa0;
	void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) = win 0x1bce0;
	// void addItem(cocos2d::CCNode cell, bool addToTop) {
	//     float height = 0.0f;
	//     cocos2d::CCObject* obj;
	//     CCARRAY_FOREACH(this->m_pContentLayer->getChildren(), obj) {
	//         auto node = reinterpret_cast<cocos2d::CCNode>(obj);
	
	//         height += node->getScaledContentSize().height;
	
	//         if (!addToTop)
	//         node->setPositionY(node->getPositionY() + cell->getScaledContentSize().height);
	//     }
	
	//     cell->setPosition(
	//         this->getScaledContentSize().width / 2,
	//         addToTop ? 
	//         height + cell->getScaledContentSize().height / 2 :
	//         cell->getScaledContentSize().height / 2
	//     );
	
	//     height += cell->getScaledContentSize().height;
	
	//     this->m_contentLayer->addChild(cell);
	
	//     this->m_contentLayer->setContentSize({
	//         cell->getScaledContentSize().width, height
	//     });
	// }
	
	cocos2d::CCTouch* m_touch;	
	cocos2d::CCPoint m_touchPosition;		 
	cocos2d::CCPoint m_touchStartPosition; 
	PAD = win 0x8;
	bool m_touchDown;	
	bool m_notAtEndOfScroll;	
	cocos2d::CCLayerColor* m_verticalScrollbar;	
	cocos2d::CCLayerColor* m_horizontalScrollbar;	
	CCScrollLayerExtDelegate* m_delegate;	
	CCContentLayer* m_contentLayer;	
	bool m_cutContent; 
	bool m_vScrollbarVisible;	
	bool m_hScrollbarVisible;	
	bool m_disableHorizontal; 
	bool m_disableVertical; 
	bool m_disableMovement; 
	float m_scrollLimitTop; 
	float m_scrollLimitBottom; 
	float m_peekLimitTop; 
	float m_peekLimitBottom; 
}

class CustomListView {
	CustomListView::CustomListView() = win 0x57e60;
	static CustomListView* create(cocos2d::CCArray* entries, BoomListType type, float width, float height) = win 0x57f90;
	void setupList() = win 0x58870;
	TableViewCell* getListCell(const char* key) = win 0x58050;
	void loadCell(TableViewCell* cell, unsigned int index) = win 0x585c0;
}

class AnimatedShopKeeper : CCAnimatedSprite {
	static AnimatedShopKeeper* create(ShopType type) = win 0x14c4d0;
	void startAnimating() = win 0x14c690;
	void animationFinished(const char*) {}

	float m_unknown1;
	float m_unknown2;
	bool m_unknown;
}

class ButtonSprite : cocos2d::CCSprite {
	static ButtonSprite* create(const char* caption, int width, bool absolute, const char* font, const char* texture, float height, float scale) = win 0x137d0;
	static ButtonSprite* create(cocos2d::CCNode subSpr, int width, bool absolute, float scale, int idk0, const char* texBG, bool idk1, float height) = win 0x134b0;
	void updateBGImage(const char* spr) = win 0x13af0;
	void setString(const char* txt) = win 0x14030;
	void updateSpriteBGSize(cocos2d::CCPoint const& offset) = win 0x13c00;

	PAD = win 0x18;
	cocos2d::CCLabelBMFont* m_label; 
	cocos2d::CCSprite* m_subSprite;	
	cocos2d::CCSprite* m_subBGSprite;  
	cocos2d::extension::CCScale9Sprite* m_BGSprite;	
	PAD = win 0x8;
	cocos2d::CCPoint m_spritePosition;	
}

class CCSpritePlus {
	void setScaleX(float scale) = win 0x1c440;
	void setScaleY(float scale) = win 0x1c4c0;
	void setScale(float scale) = win 0x1c540;
	void setPosition(const cocos2d::CCPoint& pos) = win 0x1c220;
	void setRotation(float rotation) = win 0x1c280;
	bool initWithTexture(cocos2d::CCTexture2D* texture) = win 0x1c200;
	bool initWithSpriteFrameName(const char* name) = win 0x1c1e0;
	void setFlipX(bool flip) = win 0x1c300;
	void setFlipY(bool flip) = win 0x1c3a0;
	static CCSpritePlus* createWithSpriteFrame(cocos2d::CCSpriteFrame* frame) = win 0x1c130;
}

class ColorChannelSprite {
	static ColorChannelSprite* create() = win 0x114340;
	void updateValues(ColorAction* action) = win 0x114800;
	void updateOpacity(float opacity) = win 0x1145c0;
	void updateCopyLabel(int channel, bool unk) = win 0x114440;
	void updateBlending(bool enabled) = win 0x114710;
}

class GJItemIcon {
	static GJItemIcon* create(UnlockType _type, int _id, cocos2d::ccColor3B _col1, cocos2d::ccColor3B _col2, bool _un0, bool _un1, bool _un2, cocos2d::ccColor3B _col3) = win 0x12cbf0;
	GJItemIcon* createBrowserIcon(UnlockType _type, int _id) {
	    return GJItemIcon::create(_type, _id,
	        { 0xaf, 0xaf, 0xaf }, { 0xff, 0xff, 0xff },
	        false, true, true,
	        { 0xff, 0xff, 0xff }
	    );
	}
}

class SimplePlayer : cocos2d::CCSprite {
	bool init(int iconID) = win 0x12be20;
	static SimplePlayer* create(int iconID) = win 0x12bd80;
	void updatePlayerFrame(int iconID, IconType iconType) = win 0x12c650;
	void updateColors() = win 0x12c440;
	void setFrames(const char* firstLayer, const char* secondLayer, const char* birdDome, const char* outlineSprite, const char* detailSprite) = win 0x12c9e0;
	void setColor(const cocos2d::ccColor3B& color) = win 0x12c410;
	void setSecondColor(const cocos2d::ccColor3B& color) {
	    
	    m_secondLayer->setColor(color);
	    updateColors();
	}
	void setOpacity(unsigned char opacity) = win 0x12cb90;
	bool hasGlowOutline() {
	    return m_hasGlowOutline;
	}
	void setGlowOutline(bool value) {
	    m_hasGlowOutline = value;
	    updateColors();
	}

	cocos2d::CCSprite* m_firstLayer; 
	cocos2d::CCSprite* m_secondLayer;
	cocos2d::CCSprite* m_birdDome; 
	cocos2d::CCSprite* m_outlineSprite;
	cocos2d::CCSprite* m_detailSprite;
	GJRobotSprite* m_robotSprite;
	GJSpiderSprite* m_spiderSprite;
	PAD = win 0x4; 
	bool m_hasGlowOutline;
}

class GameToolbox {
	CCMenuItemToggler* createToggleButton(cocos2d::SEL_MenuHandler onToggle, bool bToggled, cocos2d::CCMenu* pToggleMenu, cocos2d::CCNode* pTarget, cocos2d::CCNode* pLabelParent, float fCheckboxScale, float fLabelSize, float fMaxWidth, const char* sIdk, bool bIdk, int nTag, cocos2d::CCArray* pToggleArray, gd::string sText, cocos2d::CCPoint obPosition, cocos2d::CCPoint obLabelOffset) = win 0x25fe0;
	void transformColor(cocos2d::ccColor3B const& src, cocos2d::ccColor3B & dest, cocos2d::ccHSVValue hsv) = win 0x26a60;
	void alignItemsHorisontally(cocos2d::CCArray* array, bool idk, cocos2d::CCPoint start, float pad) = win 0x25b20;
}

class LevelCommentDelegate {}
class CommentUploadDelegate {}
class ColorPickerDelegate {}
class LevelDownloadDelegate {}
class LevelUpdateDelegate {}
class RateLevelDelegate {}
class LikeItemDelegate {}
class LevelDeleteDelegate {}
class NumberInputDelegate {}
class SetIDPopupDelegate {}
class HSVWidgetPopupDelegate {}
class ColorSelectDelegate {}
class ColorSetupDelegate {}
class AnimatedSpriteDelegate {}
class TriggerEffectDelegate {}
class UserInfoDelegate {}
class UploadActionDelegate {}
class UploadPopupDelegate {}
class LeaderboardManagerDelegate {}
class GJDropDownLayerDelegate {}
class MusicDownloadDelegate {}
class GJRotationControlDelegate {}
class GJScaleControlDelegate {}
class MusicDownloadDelegate {}
class ListButtonBarDelegate {}
class LevelSettingsDelegate {}
