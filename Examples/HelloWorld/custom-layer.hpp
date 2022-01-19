#pragma once

#include <Cacao>

using namespace cocos2d;

// declare our own class based on CCLayer
class CustomLayer : public CCLayer {
protected:
    virtual bool init();
    virtual void keyBackClicked();
    void backButtonCallback(CCObject*);
public:
    static CustomLayer* create();
};

// continue reading on `custom-layer.cpp`
