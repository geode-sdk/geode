#ifndef __CC_APPLICATION_WIN32_H__
#define __CC_APPLICATION_WIN32_H__

#include "../../include/ccMacros.h"
#include "CCStdC.h"
#include "../CCCommon.h"
#include "../CCApplicationProtocol.h"
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
    PAD(8);
    std::string         m_resourceRootPath;
    std::string         m_startupScriptFilename;
    void* m_pUnknown;
    bool m_bUpdateController;
    CC_SYNTHESIZE_NV(bool, m_bShutdownCalled, ShutdownCalled);
    INPUT m_iInput;
    CCPoint m_obUnknown1;
    CCPoint m_obUnknown2;
    bool m_bMouseControl;
    float m_fOldAnimationInterval;
    float m_fAnimationInterval;
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
