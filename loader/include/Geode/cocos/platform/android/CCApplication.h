#ifndef __CC_APPLICATION_ANDROID_H__
#define __CC_APPLICATION_ANDROID_H__

#include "../CCCommon.h"
#include "../CCApplicationProtocol.h"

NS_CC_BEGIN

class CCRect;

class CC_DLL CCApplication : public CCApplicationProtocol
{
    GEODE_FRIEND_MODIFY
public:
    GEODE_CUSTOM_CONSTRUCTOR_BEGIN(CCApplication)
    CCApplication();
    virtual ~CCApplication();

    /**
    @brief    Callback by CCDirector to limit FPS.
    @interval       The time, expressed in seconds, between current frame and next. 
    */
    virtual void setAnimationInterval(double interval) override;

    /**
    @brief    Run the message loop.
    */
    int run();

    /**
    @brief    Get current application instance.
    @return Current application instance pointer.
    */
    static CCApplication* sharedApplication();

    static GEODE_DLL CCApplication* get();

    /**
    @brief Get current language config
    @return Current language config
    */
    virtual ccLanguageType getCurrentLanguage() override;
    
    /**
     @brief Get target platform
     */
    virtual TargetPlatform getTargetPlatform() override;

    virtual void openURL(char const* url) override;

protected:
    static CCApplication * sm_pSharedApplication;
};

NS_CC_END

#endif    // __CC_APPLICATION_ANDROID_H__
