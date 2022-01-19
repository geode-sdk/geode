#include "custom-layer.hpp"

// This is the create method, a static method that creates
// a new instance, and initializes it.
CustomLayer* CustomLayer::create() {
    auto ret = new CustomLayer();
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        // if `new` or `init` went wrong, delete the object and return a nullptr
        delete ret;
        ret = nullptr;
    }
    return ret;
}

// This is the init function, where most of your initialization
// code should be. Here we add nodes to the layer, set their position, etc
bool CustomLayer::init() {
    // This is a bitmap font label, the first argument is the text we want
    // and the second is the .fnt file we want to use. This acts the same as sprites,
    // where we dont need to provide the -hd or -uhd suffix
    auto label = CCLabelBMFont::create("Hello world!", "bigFont.fnt");

    label->setPosition({200, 100});
    label->setRotation(45);

    // equivalent to `this->addChild(label)`
    addChild(label);

    auto backgroundSprite = CCSprite::create("GJ_gradientBG.png");
    
    // these two variables will be used for scaling the sprite
    // to the correct size
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto size = backgroundSprite->getContentSize(); // this is the size of the sprite, in cocos units
    
    // here we do some basic math to scale the sprite
    // so that it'll match the size of the window
    backgroundSprite->setScaleX(winSize.width / size.width);
    backgroundSprite->setScaleY(winSize.height / size.height);
    
    /*
      in cocos, anchor points are between 0 and 1, with (0, 0) being the
      bottom left corner. By default the anchor is at (0.5, 0.5), meaning
      its at the center of the node. Since we have not changed the position
      of the sprite, it's still at its default position (0, 0), so setting the
      anchor to the bottom right corner will make the sprite be at the center
      of the screen, which is where we want it.
      You could also do this by setting the position to half of the window size,
      however i find this solution more neat.
      `backgroundSprite->setPosition({winSize.width / 2, winSize.heigth / 2});`
      extra: cocos overrides operator/ for CCSize, so you could also just do this
      `backgroundSprite->setPosition(winSize / 2);`
    */
    backgroundSprite->setAnchorPoint({0, 0});
    
    // set the color tint of the background. rob usually uses a dark blue color
    // but here we just use gray instead
    backgroundSprite->setColor({100, 100, 100});
    
    // set the Z order to -1, to make sure the sprite ends up as the background.
    // if you want to really make sure its at the bottom you could set this to -999
    // but thats a bit excessive for this example
    backgroundSprite->setZOrder(-1);
    addChild(backgroundSprite);

    auto button = CCMenuItemSpriteExtra::create(
        // directly create the sprite here, as its our only use for it
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        nullptr,
        this,
        menu_selector(CustomLayer::backButtonCallback)
    );

    auto menu = CCMenu::create();
    menu->addChild(button);
    // set the position to the top left of the screen
    menu->setPosition({25, winSize.height - 25});

    addChild(menu);

    // This enables the "keypad" for the layer, the keypad being
    // the thing with the back button, which in our case is the ESC key
    // later you will see keyBackClicked, which is the function that is called
    setKeypadEnabled(true);

    return true;
}

// this function is called whenever you press ESC on a function (or on mobile, the back button)
void CustomLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

// this is the callback for the back button we added
// and since this does the exact same thing as pressing ESC,
// we can just "cheat" and call the keyBackClicked() function
void CustomLayer::backButtonCallback(CCObject* object) {
    keyBackClicked();
}

