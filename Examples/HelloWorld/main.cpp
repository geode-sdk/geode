#include <Cacao>

#include "custom-layer.hpp"

class $(MenuLayer, CustomLayerAdd) {
	// this is the callback function for the button we added in MenuLayer
	// inside this function, the `this` pointer will be what we set the 
	// target to, so it'll be the instance of MenuLayer
	// The only argument to this function `CCObject*` is the button which
	// the callback came from. Here its a CCObject as pretty much every
	// cocos class inherits CCObject, and can be casted into a pointer of it
	void onCustomLayer(CCObject* object) {
	    /*
	      Here we create an instance of our layer, and since its its own
	      thing we have to make a scene for it, which can easily be done
	      by just using `CCScene::create()`
	    */
	    auto layer = CustomLayer::create();
	    auto scene = CCScene::create();
	    scene->addChild(layer);
	    // Here we create a transition for the layer, which is that black fade
	    // present in gd
	    auto transition = CCTransitionFade::create(0.5f, scene);
	    // use the director to push the transition, which is
	    // technically a scene
	    CCDirector::sharedDirector()->pushScene(transition);
	}

	void onMoreGames(CCObject* ob) {
		// FLAlertLayer::create("Cacao", "Hello from custom mod!", "OK")->show();
		onCustomLayer(nullptr);
	} 

	bool init() {
        /*
          In cocos2d-x, init functions return a boolean, which is false if
          the initialization went wrong, so here we check for it. most of the time
          gd just crashes if an init function goes wrong so this is kinda useless,
          except for the part where we return true at the end
        */

		if (!$MenuLayer::init()) return false;


        /*
          Note that we're using the one without the -uhd and -hd suffix; which is
          because gd deals with selecting the right resolution texture for us.
          Note that this texture is also its own file in the gd Resources/ folder,
          meaning you access it via CCSprite::create
        */
        auto sprite = CCSprite::create("dialogIcon_017.png");

        /*
          Also note that this one is different, as the texture is contained within a
          spritesheet, so for that we use CCSprite::createWithSpriteFrameName
          For finding the name of a texture that is inside a spritesheet, you can either
          look at the .plist file directly, or use Absolute's texture splitter.
        */
        auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_stopEditorBtn_001.png");

        /*
          As with every CCNode, you can set it's position, scale, rotation, etc.
          cocos2d-x's position system is based off the bottom left corner of your screen,
          and the values aren't in pixels (sometimes they aren't even consistent), so
          positioning may require a lot of trial and error.
        */
        sprite->setPosition({100, 100});
        sprite->setScale(0.5f);
        // make sure to actually add it to the layer, otherwise nothing will show up
        addChild(sprite);


        /*
          Here we create a button using one of rob's classes, CCMenuItemSpriteExtra
          (despite the name it's not in cocos2d), and to do this we use gd.h, which
          contains a bunch of useful functions.
          CCMenuItemSpriteExtra is the button you can click on and it does a bounce animation
        */
        auto button = CCMenuItemSpriteExtra::create(
            buttonSprite, // the sprite the button will use
            nullptr,
            /*
              Here is the "target" for the callback function, most of the time it
              will be the current `this` (in our case self) and since callback
              functions have to be class methods, the target will be
              the `this` pointer in the method. It can be anything (except 0) but
              it's an useful way of using something in the callback.
            */
            this,
            /*
              Here is the callback that is called when you press the button. It's
              wrapped in a macro, which casts it to the correct type used by cocos.
              The callback is in our hook class because as previously mentioned, it has to
              be a class method.
            */
            menu_selector(CustomLayerAdd::onCustomLayer)
        );

        // continue reading on `custom-layer.hpp`, where our CustomLayer is defined

        // All CCMenuItems need to be in a CCMenu to work,
        // otherwise you can't click on the button
        auto menu = CCMenu::create();
        menu->addChild(button);
        menu->setPosition({150, 100});

        addChild(menu);

        return true;
	} 
};

