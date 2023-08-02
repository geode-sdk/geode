#ifndef __CC_APPLICATION_WIN32_H__
#define __CC_APPLICATION_WIN32_H__

#include "../../include/ccMacros.h"
#include "CCStdC.h"
#include "../CCCommon.h"
#include "../CCApplicationProtocol.h"
#include "CCControllerHandler.h"
#include <string>

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
    @brief    Run the message loop.
    */
    RT_REMOVE(  virtual int run();  )
    RT_ADD(     virtual void gameDidSave(); )

    /**
    @brief    Get current applicaiton instance.
    @return Current application instance pointer.
    */
    static CCApplication* sharedApplication();

    static GEODE_DLL CCApplication* get();

    /* override functions */
    virtual void setAnimationInterval(double interval);
    virtual ccLanguageType getCurrentLanguage();
    
    /**
     @brief Get target platform
     */
    virtual TargetPlatform getTargetPlatform();

    virtual void openURL(const char* url);
    virtual int run();
    virtual void setupGLView();
    virtual void platformShutdown();
    void toggleVerticalSync(bool);
    RT_ADD(
        void setupVerticalSync();
        void updateVerticalSync();
        void updateControllerKeys();
    )

    /**
     *  Sets the Resource root path.
     *  @deprecated Please use CCFileUtils::sharedFileUtils()->setSearchPaths() instead.
     */
    CC_DEPRECATED_ATTRIBUTE void setResourceRootPath(const gd::string& rootResDir);

    /** 
     *  Gets the Resource root path.
     *  @deprecated Please use CCFileUtils::sharedFileUtils()->getSearchPaths() instead. 
     */
    CC_DEPRECATED_ATTRIBUTE const gd::string& getResourceRootPath(void);

    void setStartupScriptFilename(const gd::string& startupScriptFile);
    
    const gd::string& getStartupScriptFilename(void)
    {
        return m_startupScriptFilename;
    }

public:
    HINSTANCE           m_hInstance;
    HACCEL              m_hAccelTable;
    LARGE_INTEGER       m_nAnimationInterval;
    LARGE_INTEGER       m_nVsyncInterval;
    gd::string          m_resourceRootPath;
    gd::string          m_startupScriptFilename;
    CCControllerHandler* m_pControllerHandler;
    bool m_bUpdateController;
    CC_SYNTHESIZE_NV(bool, m_bShutdownCalled, ShutdownCalled);
    INPUT m_iInput;
    CCPoint m_obLeftThumb;
    CCPoint m_obRightThumb;
    bool m_bMouseControl;
    float m_fAnimationInterval;
    float m_fVsyncInterval;
    CC_SYNTHESIZE_READONLY_NV(bool, m_bVerticalSyncEnabled, VerticalSyncEnabled);
    CC_SYNTHESIZE_READONLY_NV(bool, m_bControllerConnected, ControllerConnected);
    CC_SYNTHESIZE_NV(bool, m_bSleepMode, SleepMode);
    CC_SYNTHESIZE_NV(bool, m_bForceTimer, ForceTimer);
    CC_SYNTHESIZE_NV(bool, m_bSmoothFix, SmoothFix);
    CC_SYNTHESIZE_NV(bool, m_bFullscreen, Fullscreen);

    static CCApplication * sm_pSharedApplication;
};

NS_CC_END

#endif    // __CC_APPLICATION_WIN32_H__
