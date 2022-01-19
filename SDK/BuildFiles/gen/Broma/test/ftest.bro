class cocos2d::CCActionTween {
	static cocos2d::CCActionTween* create(float, char const*, float, float) = 0x447590;
}

class cocos2d::CCAnimate {
	static cocos2d::CCAnimate* create(cocos2d::CCAnimation*) = 0x1f8fc0;
}

class cocos2d::CCAnimation {
	static auto createWithSpriteFrames(cocos2d::CCArray*, float) = 0x140df0;
}

class cocos2d::CCApplication {
	auto getCurrentLanguage() = 0x1a3f40;
	auto getTargetPlatform() = 0x1a3f20;
	auto openURL(char const*) = 0x1a4550;
	auto setAnimationInterval(double) = 0x1a3ee0;
}

class cocos2d::CCApplicationProtocol {
	auto gameDidSave() = 0x1a45f0;
}

class cocos2d::CCArray {
	auto addObject(cocos2d::CCObject*) = 0x419f90;
	auto addObjectNew(cocos2d::CCObject*) = 0x41a450;
	auto addObjectsFromArray(cocos2d::CCArray*) = 0x41a2d0;
	auto containsObject(cocos2d::CCObject*) const = 0x41a3e0;
	auto count() const = 0x41a2f0;
	static cocos2d::CCArray* create() = 0x419cb0;
	static auto createWithObject(cocos2d::CCObject*) = 0x419d50;
	auto fastRemoveObject(cocos2d::CCObject*) = 0x41a520;
	auto fastRemoveObjectAtIndex(unsigned int) = 0x41a500;
	auto fastRemoveObjectAtIndexNew(unsigned int) = 0x41a510;
	auto lastObject() = 0x41a360;
	auto objectAtIndex(unsigned int) = 0x41a340;
	auto removeAllObjects() = 0x41a4f0;
	auto removeLastObject(bool) = 0x41a470;
	auto removeObject(cocos2d::CCObject*, bool) = 0x41a490;
	auto removeObjectAtIndex(unsigned int, bool) = 0x41a4b0;
	auto stringAtIndex(unsigned int) = 0x41a320;
}

class cocos2d::CCBezierTo {
	static cocos2d::CCBezierTo* create(float, cocos2d::_ccBezierConfig const&) = 0x1f6c10;
}

class cocos2d::CCCallFunc {
	static cocos2d::CCCallFunc* create(int) = 0x454d90;
}

class cocos2d::CCClippingNode {
	static cocos2d::CCClippingNode* create() = 0x4192a0;
	static cocos2d::CCClippingNode* create(cocos2d::CCNode*) = 0x419330;
	auto getAlphaThreshold() const = 0x419a10;
	auto getStencil() const = 0x4199c0;
	auto isInverted() const = 0x419a30;
	auto onEnter() = 0x419470;
	auto onEnterTransitionDidFinish() = 0x4194a0;
	auto onExit() = 0x419500;
	auto onExitTransitionDidStart() = 0x4194d0;
	auto setAlphaThreshold(float) = 0x419a20;
	auto setInverted(bool) = 0x419a40;
	auto setStencil(cocos2d::CCNode*) = 0x4199d0;
	auto visit() = 0x419530;
}

class cocos2d::CCCopying {
	inline cocos2d::CCObject* cocos2d::CCCopying::copyWithZone(cocos2d::CCZone*) {
		// 2 people complimented my hair today i feel happy
		//
		// add 3 to that c:
		return 0;
	}
}

class cocos2d::CCDelayTime {
	static cocos2d::CCDelayTime* create(float) = 0x1f4380;
}

class cocos2d::CCDictionary {
	auto allKeys() = 0x190450;
	auto count() = 0x190430;
	static cocos2d::CCDictionary* create() = 0x192650;
	auto objectForKey(intptr_t) = 0x190bb0;
	auto objectForKey(gd::string const&) = 0x190870;
	auto removeAllObjects() = 0x190220;
	auto removeObjectForKey(intptr_t) = 0x1921d0;
	auto setObject(cocos2d::CCObject*, intptr_t) = 0x191790;
	auto setObject(cocos2d::CCObject*, gd::string const&) = 0x190dc0;
	auto valueForKey(intptr_t) = 0x190cf0;
	auto valueForKey(gd::string const&) = 0x1907a0;
}

class cocos2d::CCDirector {
	virtual ~CCDirector() = 0x2493a0;
	virtual auto init() = 0x248df0;
	virtual auto getScheduler() = 0x24af00;
	virtual auto setScheduler(cocos2d::CCScheduler*) = 0x24aec0;
	virtual auto getActionManager() = 0x24af50;
	virtual auto setActionManager(cocos2d::CCActionManager*) = 0x24af10;
	virtual auto getTouchDispatcher() = 0x24afa0;
	virtual auto setTouchDispatcher(cocos2d::CCTouchDispatcher*) = 0x24af60;
	virtual auto getKeypadDispatcher() = 0x24b090;
	virtual auto setKeypadDispatcher(cocos2d::CCKeypadDispatcher*) = 0x24b050;
	virtual auto getKeyboardDispatcher() = 0x24aff0;
	virtual auto setKeyboardDispatcher(cocos2d::CCKeyboardDispatcher*) = 0x24afb0;
	virtual auto getMouseDispatcher() = 0x24b040;
	virtual auto setMouseDispatcher(cocos2d::CCMouseDispatcher*) = 0x24b000;
	virtual auto getAccelerometer() = 0x24b0e0;
	virtual auto setAccelerometer(cocos2d::CCAccelerometer*) = 0x24b0a0;
	virtual auto getDeltaTime() = 0x249bd0;
	virtual auto getSceneDelegate() = 0x24b320;
	virtual auto setSceneDelegate(cocos2d::CCSceneDelegate*) = 0x24b330;


	auto getWinSize() = 0x24a0f0;
	auto pushScene(cocos2d::CCScene*) = 0x24a620;
	auto replaceScene(cocos2d::CCScene*) = 0x24a6d0;
	auto resetSmoothFixCounter() = 0x249bc0;
	static auto sharedDirector() = 0x248cb0;
	auto calculateDeltaTime() = 0x2497a0;
	auto calculateMPF() = 0x19eac0;
	auto convertToGL(cocos2d::CCPoint const&) = 0x24a210;
	auto drawScene() = 0x249690;
	auto setNextScene() = 0x2498d0;
	auto showStats() = 0x2499e0;
	auto updateContentScale(cocos2d::TextureQuality) = 0x249ff0;
	auto popSceneWithTransition(float, cocos2d::PopTransition) = 0x24a8b0;
}

class cocos2d::CCDrawNode {
	auto clear() = 0x379e80;
	static cocos2d::CCDrawNode* create() = 0x378d00;
	auto drawPolygon(cocos2d::CCPoint*, unsigned int, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&) = 0x3797f0;
	auto drawSegment(cocos2d::CCPoint const&, cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&) = 0x3792d0;
	auto getBlendFunc() const = 0x379ea0;
	auto init() = 0x378e00;
	auto setBlendFunc(cocos2d::_ccBlendFunc const&) = 0x379eb0;
	auto draw() = 0x379020;
	virtual ~CCDrawNode() = 0x378cc0;
}

class cocos2d::CCEaseElasticOut {
	static cocos2d::CCEaseElasticOut* create(cocos2d::CCActionInterval*, float) = 0x2a3080;
}

class cocos2d::CCEaseIn {
	static cocos2d::CCEaseIn* create(cocos2d::CCActionInterval*, float) = 0x2a1960;
}

class cocos2d::CCEaseInOut {
	static cocos2d::CCEaseInOut* create(cocos2d::CCActionInterval*, float) = 0x2a1d80;
}

class cocos2d::CCEaseOut {
	static cocos2d::CCEaseOut* create(cocos2d::CCActionInterval*, float) = 0x2a1b70;
}

class cocos2d::CCFadeOut {
	static cocos2d::CCFadeOut* create(float) = 0x1f7d80;
}

class cocos2d::CCFadeTo {
	static cocos2d::CCFadeTo* create(float, unsigned char) = 0x1f7ff0;
}

class cocos2d::CCHide {
	static cocos2d::CCHide* create() = 0x4543e0;
}

class cocos2d::CCIMEDelegate {
	virtual ~CCIMEDelegate() = 0x277670;
	virtual auto attachWithIME() = 0x2776a0;
	virtual auto detachWithIME() = 0x277880;
	// virtual auto canAttachWithIME() = 0x5ef30;
	// virtual auto didAttachWithIME() = 0x5ef40;
	// virtual auto canDetachWithIME() = 0x5ef50;
	// virtual auto didDetachWithIME() = 0x5ef60;
	// virtual auto insertText(char const*, int) = 0x5ef70;
	// virtual auto deleteBackward() = 0x5ef80;
	// virtual auto getContentText() = 0x5ef90;
	// virtual auto keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo&) = 0x126e80;
	// virtual auto keyboardDidShow(cocos2d::CCIMEKeyboardNotificationInfo&) = 0x5efa0;
	// virtual auto keyboardWillHide(cocos2d::CCIMEKeyboardNotificationInfo&) = 0x126e90;
	// virtual auto keyboardDidHide(cocos2d::CCIMEKeyboardNotificationInfo&) = 0x5efb0;

	CCIMEDelegate() = 0x277310;
}

class cocos2d::CCImage {
	CCImage() = 0x24fa00;
	virtual ~CCImage() = 0x24fa80;
	auto initWithImageData(void*, int, cocos2d::CCImage::EImageFormat, int, int, int) = 0x24fcb0;
}

class cocos2d::CCKeyboardDelegate {
	inline void cocos2d::CCKeyboardDelegate::keyDown(cocos2d::enumKeyCodes) {};
	inline void cocos2d::CCKeyboardDelegate::keyUp(cocos2d::enumKeyCodes) {};

	inline cocos2d::CCKeyboardDelegate::CCKeyboardDelegate() {};
}

class cocos2d::CCKeyboardDispatcher {
	auto dispatchKeyboardMSG(cocos2d::enumKeyCodes, bool) = 0xe8190;
}

class cocos2d::CCKeyboardHandler {
    static cocos2d::CCKeyboardHandler* handlerWithDelegate(cocos2d::CCKeyboardDelegate*) = 0x242030;
    virtual auto initWithDelegate(cocos2d::CCKeyboardDelegate*) = 0x241ff0, , 0x13f8b8;
    ~CCKeyboardHandler() = 0x241e90, , 0x13f87c;
}

class cocos2d::CCKeypadHandler {
    static cocos2d::CCKeypadHandler* handlerWithDelegate(cocos2d::CCKeypadDelegate*) = 0x1ff2d0;
    virtual auto initWithDelegate(cocos2d::CCKeypadDelegate*) = 0x1ff290, , 0x69; // iOS stub
    virtual ~CCKeypadHandler() = 0x1ff130, , 0x2c530;
    cocos2d::CCKeypadDelegate* getDelegate() = 0x1ff0b0;
}


class cocos2d::CCLabelBMFont {
	static cocos2d::CCLabelBMFont* create(char const*, char const*) = 0x347660;
	auto limitLabelWidth(float, float, float) = 0x34a6e0;
	auto setScale(float) = 0x34a5d0;
	auto setString(char const*, bool) = 0x3489e0;
	auto setAnchorPoint(cocos2d::CCPoint const&) = 0x349440;
}

class cocos2d::CCLabelTTF {
	auto updateTexture() = 0x1fadc0;
}

class cocos2d::CCLayer {
    CCLayer() = 0x2725b0;
    virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = 0x2734d0, , 0xc810c;
    virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = 0x273650, , 0xc82ac;
    virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = 0x2735d0, , 0xc820c;
    virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = 0x273550, , 0xc816c;
    virtual auto ccTouchesBegan(cocos2d::CCSet*, cocos2d::CCEvent*) = 0x2736d0, , 0xc834c;
    virtual auto ccTouchesCancelled(cocos2d::CCSet*, cocos2d::CCEvent*) = 0x273850, , 0xc852c;
    virtual auto ccTouchesEnded(cocos2d::CCSet*, cocos2d::CCEvent*) = 0x2737d0, , 0xc848c;
    virtual auto ccTouchesMoved(cocos2d::CCSet*, cocos2d::CCEvent*) = 0x273750, , 0xc83ec;
    static cocos2d::CCLayer* create() = 0x272a00;
    virtual auto didAccelerate(cocos2d::CCAcceleration*) = 0x272ea0, , 0xc7c5c;
    virtual auto getTouchMode() = 0x272e10, , 0xc7b9c;
    virtual auto getTouchPriority() = 0x272e00, , 0xc7b94;
    virtual auto init() = 0x2729a0, , 0xc7894;
    virtual auto isAccelerometerEnabled() = 0x272e20, , 0xc7ba4;
    virtual auto isKeyboardEnabled() = 0x273010, , 0xc7d70;
    virtual auto isKeypadEnabled() = 0x272f70, , 0xc7cf4;
    virtual auto isMouseEnabled() = 0x273090, , 0xc7de0;
    virtual auto isTouchEnabled() = 0x272ce0, , 0xc7a54;
    virtual auto keyBackClicked() = 0x273160, , 0xc7e50;
    virtual auto keyDown(cocos2d::enumKeyCodes) = 0x273280, , 0xc7efc;
    virtual auto keyMenuClicked() = 0x273200, , 0xc7eac;
    virtual auto onEnter() = 0x273300, , 0xc7f38;
    virtual auto onEnterTransitionDidFinish() = 0x273490, , 0xc80cc;
    virtual auto onExit() = 0x2733c0, , 0xc8004;
    virtual auto registerScriptTouchHandler(int, bool, int, bool) = 0x272bd0, , 0xc79c8;
    virtual auto registerWithTouchDispatcher() = 0x272b40, , 0xc7958;
    virtual auto setAccelerometerEnabled(bool) = 0x272e30, , 0xc7bac;
    virtual auto setAccelerometerInterval(double) = 0x272e70, , 0xc7c10;
    virtual auto setKeyboardEnabled(bool) = 0x273020, , 0xc7d78;
    virtual auto setKeypadEnabled(bool) = 0x272f80, , 0xc7cfc;
    virtual auto setMouseEnabled(bool) = 0x2730a0, , 0xc7de8;
    virtual auto setTouchEnabled(bool) = 0x272cf0, , 0xc7a5c;
    virtual auto setTouchMode(cocos2d::ccTouchesMode) = 0x272d60, , 0xc7acc;
    virtual auto setTouchPriority(int) = 0x272db0, , 0xc7b30;
    virtual auto unregisterScriptTouchHandler() = 0x272c30, , 0xc7a28;
    ~CCLayer() = 0x2727b0, , 0xc7848;
}

class cocos2d::CCLayerColor {
    CCLayerColor() = 0x274320;
    static cocos2d::CCLayerColor* create(cocos2d::_ccColor4B const&, float, float) = 0x2745e0;
    auto draw() = 0x274b50, , 0xc8fe0;
    auto getBlendFunc() = 0x274480, , 0xc8bcc;
    auto init() = 0x274800, , 0xc8de8;
    auto initWithColor(cocos2d::_ccColor4B const&) = 0x2749a0, , 0xc8f14;
    auto initWithColor(cocos2d::_ccColor4B const&, float, float) = 0x274850, , 0xc8e34;
    auto setBlendFunc(cocos2d::_ccBlendFunc) = 0x2744a0, , 0xc8bdc;
    auto setColor(cocos2d::_ccColor3B const&) = 0x274c20, , 0xc90ac;
    auto setContentSize(cocos2d::CCSize const&) = 0x2749f0, , 0xc8f64;
    auto setOpacity(unsigned char) = 0x274db0, , 0xc9108;
    auto updateColor() = 0x274ae0, , 0xc8f80;
    ~CCLayerColor() = 0x2743d0, , 0x2743e0;
}

class cocos2d::CCLayerRGBA {
    CCLayerRGBA() = 0x2738d0;
    virtual auto init() = 0x273b40, , 0xc8de8; // ios version is not real. any errors can be traced back to this
    virtual auto getOpacity() = 0x273be0, , 0xc8698;
    virtual auto getDisplayedOpacity() = 0x273c00, , 0xc8724;
    virtual auto setOpacity(unsigned char) = 0x273c20, , 0xc8734;
    virtual auto updateDisplayedOpacity(unsigned char) = 0x273f20, , 0xc88d8;
    virtual auto isCascadeOpacityEnabled() = 0x2741f0, , 0xc8aac;
    virtual auto setCascadeOpacityEnabled(bool) = 0x274210, , 0xc8abc;
    virtual auto getColor() = 0x273d60, , 0xc87d8;
    virtual auto getDisplayedColor() = 0x273d80, , 0xc87e8;
    virtual auto setColor(cocos2d::_ccColor3B const&) = 0x273da0, , 0xc87f8;
    virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = 0x2740b0, , 0xc89a4;
    virtual auto isCascadeColorEnabled() = 0x274230, , 0xc8acc;
    virtual auto setCascadeColorEnabled(bool) = 0x274250, , 0xc8adc;
    // virtual auto setOpacityModifyRGB(bool) = 0x6180, , 0x68e8;
    // virtual auto isOpacityModifyRGB() = 0x6190, , 0x68ec;
    virtual ~CCLayerRGBA() = 0x273aa0, , 0xc77b0;
}

class cocos2d::CCMenu {
	auto alignItemsHorizontallyWithPadding(float) = 0x4393e0;
	auto alignItemsVerticallyWithPadding(float) = 0x439190;
	static cocos2d::CCMenu* create() = 0x438720;
	static cocos2d::CCMenu* createWithArray(cocos2d::CCArray*) = 0x4387e0;
	static cocos2d::CCMenu* createWithItem(cocos2d::CCMenuItem*) = 0x438b80;
}

class cocos2d::CCMenuItem {
	virtual ~CCMenuItem() = 0x1fb8e0;
	virtual auto activate() = 0x1fba70;
	virtual auto selected() = 0x1fb9e0;
	virtual auto unselected() = 0x1fb9f0;
	virtual auto registerScriptTapHandler(int) = 0x1fba00;
	virtual auto unregisterScriptTapHandler() = 0x1fba30;
	virtual auto isEnabled() = 0x1fbaf0;
	virtual auto setEnabled(bool) = 0x1fbae0;
	virtual auto isSelected() = 0x1fbb50;
}

class cocos2d::CCMenuItemImage {
	// virtual ~CCMenuItemImage() = 0x1febb0;
	virtual auto init() = 0x1fd750;
}

class cocos2d::CCMenuItemLabel {
	virtual ~CCMenuItemLabel() = 0x1fc0d0;
	virtual auto activate() = 0x1fc240;
	virtual auto selected() = 0x1fc2e0;
	virtual auto unselected() = 0x1fc380;
	virtual auto setEnabled(bool) = 0x1fc3f0;
	virtual auto getDisabledColor() = 0x1fbb80;
	virtual auto setDisabledColor(cocos2d::_ccColor3B const&) = 0x1fbb90;
	virtual auto getLabel() = 0x1fbbb0;
	virtual auto setLabel(cocos2d::CCNode*) = 0x1fbbc0;
}

class cocos2d::CCMenuItemSprite {
	// virtual ~CCMenuItemSprite() = 0x1feab0;
	virtual auto selected() = 0x1fd3f0;
	virtual auto unselected() = 0x1fd470;
	virtual auto setEnabled(bool) = 0x1fd4e0;
	virtual auto getNormalImage() = 0x1fcee0;
	virtual auto setNormalImage(cocos2d::CCNode*) = 0x1fcef0;
	virtual auto getSelectedImage() = 0x1fcfc0;
	virtual auto setSelectedImage(cocos2d::CCNode*) = 0x1fcfd0;
	virtual auto getDisabledImage() = 0x1fd070;
	virtual auto setDisabledImage(cocos2d::CCNode*) = 0x1fd080;
	virtual auto updateImagesVisibility() = 0x1fd510;

	static auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*) = 0x1fd120;
	static auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = 0x1fd140;
	static auto create(cocos2d::CCNode*, cocos2d::CCNode*, cocos2d::CCObject*, cocos2d::SEL_MenuHandler) = 0x1fd2d0;
}

class cocos2d::CCMotionStreak {
	auto reset() = 0x2ee190;
	auto resumeStroke() = 0x2edb30;
	auto stopStroke() = 0x2edb20;
}

class cocos2d::CCMouseDelegate {
	inline cocos2d::CCMouseDelegate::CCMouseDelegate() {};
	inline void cocos2d::CCMouseDelegate::rightKeyDown() {};
	inline void cocos2d::CCMouseDelegate::rightKeyUp() {};
	inline void cocos2d::CCMouseDelegate::scrollWheel(float, float) {};
}

class cocos2d::CCMouseHandler {
    static cocos2d::CCMouseHandler* handlerWithDelegate(cocos2d::CCMouseDelegate*) = 0x12ef80;
    virtual auto initWithDelegate(cocos2d::CCMouseDelegate*) = 0x12ef40, , 0x43798;
    ~CCMouseHandler() = 0x12ede0, , 0x4375c;
}

class cocos2d::CCMoveBy {
	static cocos2d::CCMoveBy* create(float, cocos2d::CCPoint const&) = 0x1f50e0;
}

class cocos2d::CCMoveTo {
	static cocos2d::CCMoveTo* create(float, cocos2d::CCPoint const&) = 0x1f54d0;
}

class cocos2d::CCNode {
    CCNode() = 0x122550;
    auto _setZOrder(int) = 0x122990, , 0x15dd7c;
    auto addChild(cocos2d::CCNode*) = 0x1233d0, , 0x15e5d4;
    auto addChild(cocos2d::CCNode*, int) = 0x1233b0, , 0x15e5c4;
    auto addChild(cocos2d::CCNode*, int, int) = 0x1232a0, , 0x15e4e4;
    auto addComponent(cocos2d::CCComponent*) = 0x124a40, , 0x15f6a4;
    auto cleanup() = 0x123100, , 0x15e3a4;
    auto convertToNodeSpace(cocos2d::CCPoint const&) = 0x124750;
    auto convertToWorldSpace(cocos2d::CCPoint const&) = 0x124790;
    static cocos2d::CCNode* create() = 0x1230a0;
    auto draw() = 0x123840, , 0x15e974;
    auto getActionByTag(int) = 0x123ee0;
    auto getActionManager() = 0x123e50, , 0x15ef54;
    auto getAnchorPoint() = 0x122d80, , 0x15e090;
    auto getAnchorPointInPoints() = 0x122d70, , 0x15e088;
    auto getCamera() = 0x122cb0, , 0x15dfe4;
    auto getChildByTag(int) = 0x123220, , 0x15e460;
    auto getChildren() = 0x122c80, , 0x15dfcc;
    auto getChildrenCount() const = 0x122c90, , 0x15dfd4;
    auto getContentSize() const = 0x122e00, , 0x15e118;
    auto getGLServerState() = 0x122f90, , 0x15e248;
    auto getGrid() = 0x122d00, , 0x15e034;
    auto getOrderOfArrival() = 0x122f50, , 0x15e228;
    auto getParent() = 0x122ed0, , 0x15e1e0;
    auto getPosition() = 0x122b60, , 0x15debc;
    auto getPosition(float*, float*) = 0x122b90, , 0x15dedc;
    auto getPositionX() = 0x122be0, , 0x15df30;
    auto getPositionY() = 0x122bf0, , 0x15df38;
    auto getRotation() = 0x122a00, , 0x15dde4;
    auto getRotationX() = 0x122a50, , 0x15de1c;
    auto getRotationY() = 0x122a80, , 0x15de38;
    auto getScale() = 0x122ab0, , 0x15de54;
    auto getScaleX() = 0x122b00, , 0x15de84;
    auto getScaleY() = 0x122b30, , 0x15dea0;
    auto getScaledContentSize() = 0x122e10, , 0x15e120;
    auto getScheduler() = 0x123f70, , 0x15f008;
    auto getShaderProgram() = 0x122f70, , 0x15e238;
    auto getSkewX() = 0x122920, , 0x15dd3c;
    auto getSkewY() = 0x122950, , 0x15dd58;
    auto getUserData() = 0x122f30, , 0x15e218;
    auto getUserObject() = 0x122f80, , 0x15e240;
    auto getVertexZ() = 0x1229e0, , 0x15ddd4;
    auto getZOrder() = 0x122980, , 0x15dd74;
    auto ignoreAnchorPointForPosition(bool) = 0x122f00, , 0x15e1f8;
    auto init() = 0x122910, , 0x15dd34;
    auto isIgnoreAnchorPointForPosition() = 0x122ef0, , 0x15e1f0;
    auto isRunning() = 0x122ec0, , 0x15e1d8;
    auto isVisible() = 0x122d50, , 0x15e078;
    auto nodeToParentTransform() = 0x124210, , 0x15f1a4;
    auto nodeToWorldTransform() = 0x124670, , 0x15f490;
    auto onEnter() = 0x123a90, , 0x15eb94;
    auto onEnterTransitionDidFinish() = 0x123b90, , 0x15ec94;
    auto onExit() = 0x123ca0, , 0x15edb0;
    auto onExitTransitionDidStart() = 0x123c00, , 0x15ed18;
    auto parentToNodeTransform() = 0x1245d0, , 0x15f410;
    auto pauseSchedulerAndActions() = 0x123d60;
    auto registerScriptHandler(int) = 0x123d90, , 0x15ee94;
    auto removeAllChildren() = 0x123600, , 0x15e788;
    auto removeAllChildrenWithCleanup(bool) = 0x123620, , 0x15e798;
    auto removeAllComponents() = 0x124aa0, , 0x15f6d4;
    auto removeChild(cocos2d::CCNode*) = 0x123460, , 0x15e630;
    auto removeChild(cocos2d::CCNode*, bool) = 0x123480, , 0x15e640;
    auto removeChildByTag(int) = 0x1235a0, , 0x15e724;
    auto removeChildByTag(int, bool) = 0x1235c0, , 0x15e734;
    auto removeComponent(char const*) = 0x124a60, , 0x15f6b4;
    auto removeComponent(cocos2d::CCComponent*) = 0x124a80, , 0x15f6c4;
    auto removeFromParent() = 0x1233f0, , 0x15e5e8;
    auto removeFromParentAndCleanup(bool) = 0x123410, , 0x15e5f8;
    auto removeMeAndCleanup() = 0x123440, , 0x15e620;
    auto reorderChild(cocos2d::CCNode*, int) = 0x123760, , 0x15e87c;
    auto resumeSchedulerAndActions() = 0x123b60;
    auto runAction(cocos2d::CCAction*) = 0x123e60;
    auto schedule(cocos2d::SEL_SCHEDULE) = 0x1240b0;
    auto schedule(cocos2d::SEL_SCHEDULE, float) = 0x124120;
    auto scheduleUpdate() = 0x123f80;
    auto setActionManager(cocos2d::CCActionManager*) = 0x123e00, , 0x15ef04;
    auto setAnchorPoint(cocos2d::CCPoint const&) = 0x122d90, , 0x15e098;
    auto setContentSize(cocos2d::CCSize const&) = 0x122e50, , 0x15e158;
    auto setGLServerState(cocos2d::ccGLServerState) = 0x122fa0, , 0x15e250;
    auto setGrid(cocos2d::CCGridBase*) = 0x122d10, , 0x15e03c;
    auto setOrderOfArrival(unsigned int) = 0x122f60, , 0x15e230;
    auto setParent(cocos2d::CCNode*) = 0x122ee0, , 0x15e1e8;
    auto setPosition(cocos2d::CCPoint const&) = 0x122b70, , 0x15dec4;
    auto setPosition(float, float) = 0x122ba0, , 0x15def0;
    auto setPositionX(float) = 0x122c00, , 0x15df40;
    auto setPositionY(float) = 0x122c40, , 0x15df84;
    auto setRotation(float) = 0x122a10, , 0x15ddec;
    auto setRotationX(float) = 0x122a60, , 0x15de24;
    auto setRotationY(float) = 0x122a90, , 0x15de40;
    auto setScale(float) = 0x122ac0, , 0x15de5c;
    auto setScale(float, float) = 0x122ae0, , 0x15de70;
    auto setScaleX(float) = 0x122b10, , 0x15de8c;
    auto setScaleY(float) = 0x122b40, , 0x15dea8;
    auto setScheduler(cocos2d::CCScheduler*) = 0x123f20, , 0x15efb8;
    auto setShaderProgram(cocos2d::CCGLProgram*) = 0x122ff0, , 0x15e294;
    auto setSkewX(float) = 0x122930, , 0x15dd44;
    auto setSkewY(float) = 0x122960, , 0x15dd60;
    auto setUserData(void*) = 0x122f40, , 0x15e220;
    auto setUserObject(cocos2d::CCObject*) = 0x122fb0, , 0x15e258;
    auto setVertexZ(float) = 0x1229f0, , 0x15dddc;
    auto setVisible(bool) = 0x122d60, , 0x15e080;
    auto setZOrder(int) = 0x1229a0, , 0x15dd84;
    auto sortAllChildren() = 0x1237b0, , 0x15e8d4;
    auto stopActionByTag(int) = 0x123ec0;
    auto stopAllActions() = 0x123190;
    auto unregisterScriptHandler() = 0x123dc0, , 0x15eec4;
    auto unschedule(cocos2d::SEL_SCHEDULE) = 0x124180;
    auto unscheduleAllSelectors() = 0x1231b0;
    auto unscheduleUpdate() = 0x124060;
    auto update(float) = 0x1241a0, , 0x15f124;
    auto updateTransform() = 0x1249d0, , 0x15f648;
    auto updateTweenAction(float, char const*) = 0x1249c0, , 0x15f644;
    auto visit() = 0x123850, , 0x15e978;
    auto worldToNodeTransform() = 0x124710, , 0x15f51c;
    ~CCNode() = 0x122750, , 0x6c98;
}

class cocos2d::CCNodeRGBA {
	CCNodeRGBA() = 0x124b30;
	virtual ~CCNodeRGBA() = 0x124bb0;
	virtual auto init() = 0x124bf0;
	virtual auto getOpacity() = 0x124cd0;
	virtual auto getDisplayedOpacity() = 0x124cf0;
	virtual auto setOpacity(GLubyte) = 0x124d10;
	virtual auto updateDisplayedOpacity(GLubyte) = 0x124e50;
	virtual auto isCascadeOpacityEnabled() = 0x124fe0;
	virtual auto setCascadeOpacityEnabled(bool) = 0x125000;
	virtual auto getColor() = 0x125020;
	virtual auto getDisplayedColor() = 0x125040;
	virtual auto setColor(cocos2d::_ccColor3B const&) = 0x125060;
	virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = 0x1251e0;
	virtual auto isCascadeColorEnabled() = 0x125320;
	virtual auto setCascadeColorEnabled(bool) = 0x125340;
}

class cocos2d::CCObject {
    CCObject() = 0x250ca0;
    auto acceptVisitor(cocos2d::CCDataVisitor&) = 0x250f30, , 0x439f0;
    auto autorelease() = 0x250ed0;
    auto canEncode() = 0x250f90, , 0x43a24;
    auto encodeWithCoder(DS_Dictionary*) = 0x250f70, , 0x43a18;
    auto getTag() const = 0x250f50, , 0x43a08;
    auto isEqual(cocos2d::CCObject const*) = 0x250f20, , 0x439e4;
    auto release() = 0x250ea0;
    auto retain() = 0x250ec0;
    auto setTag(int) = 0x250f60, , 0x43a10;
    ~CCObject() = 0x250d90, , 0x6ac0;
}

class cocos2d::CCParticleSystem {
	auto resetSystem() = 0x46bd50;
	auto resumeSystem() = 0x46bd40;
	auto stopSystem() = 0x46bd10;
}

class cocos2d::CCParticleSystemQuad {
	static cocos2d::CCParticleSystemQuad* create(char const*) = 0x36b000;
}

class cocos2d::CCPoolManager {
	auto pop() = 0x214620;
	static cocos2d::CCPoolManager* sharedPoolManager() = 0x2142c0;
}

class cocos2d::CCRenderTexture {
	auto begin() = 0x35ce10;
	auto end() = 0x35d2c0;
	static cocos2d::CCRenderTexture* create(int, int, cocos2d::CCTexture2DPixelFormat) = 0x35c720;
	auto newCCImage(bool) = 0x35d7d0;
}

class cocos2d::CCRepeatForever {
	static cocos2d::CCRepeatForever* create(cocos2d::CCActionInterval*) = 0x1f3920;
}

class cocos2d::CCRotateBy {
	static cocos2d::CCRotateBy* create(float, float) = 0x1f4c50;
}

class cocos2d::CCScaleTo {
	static cocos2d::CCScaleTo* create(float, float) = 0x1f6ff0;
	static cocos2d::CCScaleTo* create(float, float, float) = 0x1f70f0;
}

class cocos2d::CCScene {
	static cocos2d::CCScene* create() = 0x13c140;
	auto getHighestChildZ() = 0x13c200;
}

class cocos2d::CCScheduler {
	auto scheduleSelector(cocos2d::SEL_SCHEDULE, cocos2d::CCObject*, float, unsigned int, float, bool) = 0x242b20;
	auto scheduleUpdateForTarget(cocos2d::CCObject*, int, bool) = 0x2438d0;
	auto unscheduleAllForTarget(cocos2d::CCObject*) = 0x243e40;
}

class cocos2d::CCSequence {
}

class cocos2d::CCSet {
	CCSet() = 0x45ad80;
	virtual ~CCSet() = 0x45b050;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = 0x45b090;
}

class cocos2d::CCSprite {
	virtual ~CCSprite() = 0x133430;
	virtual auto init() = 0x132ef0;
	virtual auto setVertexZ(float) = 0x134a80;
	virtual auto setScaleX(float) = 0x134900;
	virtual auto setScaleY(float) = 0x134980;
	virtual auto setScale(float) = 0x134a00;
	virtual auto setPosition(cocos2d::CCPoint const&) = 0x134650;
	virtual auto setSkewX(float) = 0x134820;
	virtual auto setSkewY(float) = 0x134890;
	virtual auto setAnchorPoint(cocos2d::CCPoint const&) = 0x134af0;
	virtual auto setVisible(bool) = 0x134b70;
	virtual auto setRotation(float) = 0x1346d0;
	virtual auto setRotationX(float) = 0x134740;
	virtual auto setRotationY(float) = 0x1347b0;
	virtual auto ignoreAnchorPointForPosition(bool) = 0x134b60;
	virtual auto addChild(cocos2d::CCNode*) = 0x134190;
	virtual auto addChild(cocos2d::CCNode*, int) = 0x1341a0;
	virtual auto addChild(cocos2d::CCNode*, int, int) = 0x1341b0;
	virtual auto removeChild(cocos2d::CCNode*, bool) = 0x134300;
	virtual auto removeAllChildrenWithCleanup(bool) = 0x134340;
	virtual auto reorderChild(cocos2d::CCNode*, int) = 0x134270;
	virtual auto sortAllChildren() = 0x1343f0;
	virtual auto draw() = 0x134070;
	virtual auto updateTransform() = 0x133b70;
	virtual auto setOpacity(unsigned char) = 0x134da0;
	virtual auto updateDisplayedOpacity(unsigned char) = 0x1354c0;
	virtual auto setColor(cocos2d::_ccColor3B const&) = 0x134ff0;
	virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = 0x135370;
	virtual auto setOpacityModifyRGB(bool) = 0x135200;
	virtual auto isOpacityModifyRGB() = 0x135350;
	virtual auto initWithTexture(cocos2d::CCTexture2D*) = 0x133110;
	virtual auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&) = 0x1330f0;
	virtual auto initWithTexture(cocos2d::CCTexture2D*, cocos2d::CCRect const&, bool) = 0x132f10;
	virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame*) = 0x133270;
	virtual auto initWithSpriteFrameName(char const*) = 0x1332c0;
	virtual auto initWithFile(char const*) = 0x133180;
	virtual auto initWithFile(char const*, cocos2d::CCRect const&) = 0x133210;
	virtual auto setTexture(cocos2d::CCTexture2D*) = 0x135a90;
	virtual auto getTexture() = 0x135c00;
	// virtual auto setBlendFunc(cocos2d::_ccBlendFunc) = 0x50590;
	// virtual cocos2d::_ccBlendFunc getBlendFunc() = 0x505a0;
	virtual auto setChildColor(cocos2d::_ccColor3B const&) = 0x135160;
	virtual auto setChildOpacity(unsigned char) = 0x134f50;
	virtual auto getBatchNode() = 0x135910;
	virtual auto setBatchNode(cocos2d::CCSpriteBatchNode*) = 0x135920;
	virtual auto refreshTextureRect() = 0x133520;
	virtual auto setTextureRect(cocos2d::CCRect const&) = 0x133560;
	virtual auto setTextureRect(cocos2d::CCRect const&, bool, cocos2d::CCSize const&) = 0x133580;
	virtual auto setVertexRect(cocos2d::CCRect const&) = 0x1338f0;
	virtual auto setDisplayFrame(cocos2d::CCSpriteFrame*) = 0x135610;
	virtual auto isFrameDisplayed(cocos2d::CCSpriteFrame*) = 0x1356d0;
	virtual auto displayFrame() = 0x135760;
	virtual auto setDisplayFrameWithAnimationName(char const*, int) = 0x135680;
	// virtual auto isDirty() = 0x505b0;
	// virtual auto setDirty(bool) = 0x505c0;
	virtual auto setTextureCoords(cocos2d::CCRect const&) = 0x133910;
	virtual auto updateBlendFunc() = 0x135a20;
	virtual auto setReorderChildDirtyRecursively() = 0x134550;
	virtual auto setDirtyRecursively(bool) = 0x1345b0;

	CCSprite() = 0x133300;

	static cocos2d::CCSprite* create() = 0x132df0;
	static cocos2d::CCSprite* create(char const*) = 0x132a80;
	static cocos2d::CCSprite* createWithSpriteFrame(cocos2d::CCSpriteFrame*) = 0x132cb0;
	static cocos2d::CCSprite* createWithSpriteFrameName(char const*) = 0x132dc0;
	static cocos2d::CCSprite* createWithTexture(cocos2d::CCTexture2D*) = 0x132790;
}

class cocos2d::CCSpriteBatchNode {
	static cocos2d::CCSpriteBatchNode* create(char const*, unsigned int) = 0xbb540;
	static auto createWithTexture(cocos2d::CCTexture2D*, unsigned int) = 0xbb310;
	auto getUsedAtlasCapacity() = 0xbc6b0;
	auto increaseAtlasCapacity(unsigned int) = 0xbc670;
}

class cocos2d::CCSpriteFrameCache {
	auto addSpriteFramesWithFile(char const*) = 0x199a10;
	static cocos2d::CCSpriteFrameCache* sharedSpriteFrameCache() = 0x198970;
	auto spriteFrameByName(char const*) = 0x19a7e0;
}

class cocos2d::CCStandardTouchHandler {
    static cocos2d::CCStandardTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate*, int) = 0x247f30;
    virtual auto initWithDelegate(cocos2d::CCTouchDelegate*, int) = 0x247ed0, , 0x69; // iOS stub
    ~CCStandardTouchHandler() = 0x2482a0, , 0x6d28;
}

class cocos2d::CCString {
	virtual ~CCString() = 0x44c590;
	virtual auto isEqual(cocos2d::CCObject const*) = 0x44c8f0;
	virtual auto acceptVisitor(cocos2d::CCDataVisitor&) = 0x44ccb0;
	virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone*) = 0x44c870;
	CCString(gd::string const&) = 0x44c310;
	auto boolValue() const = 0x44c810;
	auto doubleValue() const = 0x44c7f0;
	auto floatValue() const = 0x44c7d0;
	auto getCString() const = 0x44c470;
	auto intValue() const = 0x44c780;
}

class cocos2d::CCTargetedTouchHandler {
    static cocos2d::CCTargetedTouchHandler* handlerWithDelegate(cocos2d::CCTouchDelegate*, int, bool) = 0x248010;
    virtual auto initWithDelegate(cocos2d::CCTouchDelegate*, int, bool) = 0x2480f0, , 0x69; // iOS stub
    ~CCTargetedTouchHandler() = 0x248180, , 0x6cec;
}


class cocos2d::CCTexture2D {
	~CCTexture2D() = 0x246350;
	CCTexture2D() = 0x246280;
	auto getContentSize() = 0x246460;
	auto getMaxS() = 0x2464e0;
	auto getMaxT() = 0x246500;
	auto getName() = 0x246450;
	auto getPixelFormat() = 0x246420;
	auto getPixelsHigh() = 0x246440;
	auto getPixelsWide() = 0x246430;
	auto getShaderProgram() = 0x246520;
	auto initPremultipliedATextureWithImage(cocos2d::CCImage*, unsigned int, unsigned int) = 0x2469a0;
	auto initWithData(void const*, cocos2d::CCTexture2DPixelFormat, unsigned int, unsigned int, cocos2d::CCSize const&) = 0x2465d0;
	auto setAliasTexParameters() = 0x247a20;
	auto setAntiAliasTexParameters() = 0x247a80;
	auto setMaxS(float) = 0x2464f0;
	auto setMaxT(float) = 0x246510;
	auto setShaderProgram(cocos2d::CCGLProgram*) = 0x246530;
	auto initWithImage(cocos2d::CCImage*) = 0x246940;
	auto setTexParameters(cocos2d::_ccTexParams*) = 0x247980;
}

class cocos2d::CCTextureCache {
	auto addImage(char const*, bool) = 0x358120;
	static cocos2d::CCTextureCache* sharedTextureCache() = 0x356e00;
}

class cocos2d::CCTintTo {
	static cocos2d::CCTintTo* create(float, unsigned char, unsigned char, unsigned char) = 0x1f82a0;
}

class cocos2d::CCTouch {
	auto getDelta() const = 0x38340;
	auto getLocationInView() const = 0x38250;
}

class cocos2d::CCTouchDispatcher {
	auto decrementForcePrio(int) = 0x280f70;
	auto incrementForcePrio(int) = 0x280f60;
}

class cocos2d::CCTouchHandler {
    virtual auto initWithDelegate(cocos2d::CCTouchDelegate*, int) = 0x247d10, , 0x69f8;
    ~CCTouchHandler() = 0x247de0, , 0x6ac0;
}

class cocos2d::CCTransitionFade {
	static cocos2d::CCTransitionFade* create(float, cocos2d::CCScene*) = 0x8ea30;
}

class cocos2d::ZipUtils {
	static auto compressString(gd::string, bool, int) = 0xe9a50;
	static auto decompressString(gd::string, bool, int) = 0xea380;
}

class cocos2d::extension::CCControlColourPicker {
	auto setColorValue(cocos2d::_ccColor3B const&) = 0x1aac10;
}

class cocos2d::extension::CCControlUtils {
	static cocos2d::extension::HSV HSVfromRGB(cocos2d::extension::RGBA) = 0x1e6750;
	static cocos2d::extension::RGBA RGBfromHSV(cocos2d::extension::HSV) = 0x1e6850;
}

class cocos2d::extension::CCScale9Sprite {
    CCScale9Sprite() = 0x211330;
    static cocos2d::extension::CCScale9Sprite* create(char const*) = 0x2130d0;
    static cocos2d::extension::CCScale9Sprite* create(char const*, cocos2d::CCRect) = 0x212ef0;
    virtual ~CCScale9Sprite() = 0x211590;
    virtual auto init() = 0x2115d0;
    virtual auto setContentSize(cocos2d::CCSize const&) = 0x2127c0, , 0x13e400;
    virtual auto visit() = 0x213bf0;
    virtual auto getOpacity() = 0x213f30;
    virtual auto setOpacity(unsigned char) = 0x213dd0;
    virtual auto updateDisplayedOpacity(unsigned char) = 0x2139b0;
    virtual auto getColor() = 0x213db0;
    virtual auto setColor(cocos2d::_ccColor3B const&) = 0x213c20;
    virtual auto updateDisplayedColor(cocos2d::_ccColor3B const&) = 0x213a20;
    virtual auto setOpacityModifyRGB(bool) = 0x213830;
    virtual auto isOpacityModifyRGB() = 0x213990;
    virtual auto getPreferredSize() = 0x213620;
    virtual auto setPreferredSize(cocos2d::CCSize) = 0x2135f0;
    virtual auto getCapInsets() = 0x2136e0;
    virtual auto setCapInsets(cocos2d::CCRect) = 0x213640;
    virtual auto getInsetLeft() = 0x213b30;
    virtual auto setInsetLeft(float) = 0x213b70;
    virtual auto getInsetTop() = 0x213b40;
    virtual auto setInsetTop(float) = 0x213b90;
    virtual auto getInsetRight() = 0x213b50;
    virtual auto setInsetRight(float) = 0x213bb0;
    virtual auto getInsetBottom() = 0x213b60;
    virtual auto setInsetBottom(float) = 0x213bd0;
    virtual auto initWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, bool, cocos2d::CCRect) = 0x211690;
    virtual auto initWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, cocos2d::CCRect) = 0x211630;
    virtual auto initWithFile(char const*, cocos2d::CCRect, cocos2d::CCRect) = 0x212d50;
    virtual auto initWithFile(char const*, cocos2d::CCRect) = 0x212e80;
    virtual auto initWithFile(cocos2d::CCRect, char const*) = 0x212f80;
    virtual auto initWithFile(char const*) = 0x213080;
    virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame*, cocos2d::CCRect) = 0x213140;
    virtual auto initWithSpriteFrame(cocos2d::CCSpriteFrame*) = 0x213250;
    virtual auto initWithSpriteFrameName(char const*, cocos2d::CCRect) = 0x213310;
    virtual auto initWithSpriteFrameName(char const*) = 0x213410;
    virtual auto updateWithBatchNode(cocos2d::CCSpriteBatchNode*, cocos2d::CCRect, bool, cocos2d::CCRect) = 0x211730;
    virtual auto setSpriteFrame(cocos2d::CCSpriteFrame*) = 0x213a90;
}

class cocos2d::extension::CCScrollView {
	CCScrollView() = 0x214800;
	virtual ~CCScrollView() = 0x214c30;
	virtual auto init() = 0x214fb0;
	virtual auto setContentSize(cocos2d::CCSize const&) = 0x215eb0;
	virtual auto getContentSize() const = 0x215e90;
	virtual auto addChild(cocos2d::CCNode*) = 0x216160;
	virtual auto addChild(cocos2d::CCNode*, int) = 0x216120;
	virtual auto addChild(cocos2d::CCNode*, int, int) = 0x216080;
	virtual auto visit() = 0x2164a0;
	virtual auto ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = 0x216620;
	virtual auto ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = 0x216890;
	virtual auto ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = 0x216c70;
	virtual auto ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = 0x216d30;
	virtual auto registerWithTouchDispatcher() = 0x214ff0;
	virtual auto setTouchEnabled(bool) = 0x215250;
}
