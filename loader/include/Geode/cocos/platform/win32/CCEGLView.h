/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CC_EGLVIEW_WIN32_H__
#define __CC_EGLVIEW_WIN32_H__

#include "CCStdC.h"
#include "../CCCommon.h"
#include "../../cocoa/CCGeometry.h"
#include "../CCEGLViewProtocol.h"

#include "../../robtop/glfw/glfw3.h"

NS_CC_BEGIN

typedef LRESULT (*CUSTOM_WND_PROC)(UINT message, WPARAM wParam, LPARAM lParam, BOOL* pProcessed);

class CCEGL;

// @note RobTop Addition: added CCObject inheritance
class CC_DLL CCEGLView : public CCEGLViewProtocol, public CCObject
{
    GEODE_FRIEND_MODIFY
protected:
    virtual ~CCEGLView();
public:
    CCEGLView();

    CCEGLView(geode::ZeroConstructorType, size_t fill) :
        CCEGLViewProtocol(geode::ZeroConstructor, fill),
        CCObject(geode::ZeroConstructor, fill - sizeof(CCEGLViewProtocol)) {}

    CCEGLView(geode::ZeroConstructorType) :
        CCEGLViewProtocol(geode::ZeroConstructor, sizeof(CCEGLView)),
        CCObject(geode::ZeroConstructor, sizeof(CCEGLView) - sizeof(CCEGLViewProtocol)) {}

    CCEGLView(geode::CutoffConstructorType, size_t fill) : CCEGLView() {}

    /* override functions */
    virtual bool isOpenGLReady();
    virtual void end();
    virtual void swapBuffers();
    virtual void setFrameSize(float width, float height);
	// Robtop Removal
    // virtual void setEditorFrameSize(float width, float height,HWND hWnd);
    virtual void setIMEKeyboardState(bool bOpen);
    void updateWindow(int width, int height);
    void pollEvents(void);

    void setMenuResource(LPCWSTR menu);
    void setWndProc(CUSTOM_WND_PROC proc);

protected:
    // Robtop Removal
    // virtual bool Create();
    void setupWindow(cocos2d::CCRect rect);
    // @note RobTop Addition
    bool initGlew();

public:
    // Robtop Removal
    // bool initGL();
    // Robtop Removal
    // void destroyGL();

    // Robtop Removal
    // virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void setHWnd(HWND hWnd);
    // win32 platform function
    // Robtop Removal
    // HWND getHWnd();
    // Robtop Removal
    // virtual void resize(int width, int height);
    // @note RobTop Addition
     void resizeWindow(int width, int height);
	
    /* 
     * Set zoom factor for frame. This method is for debugging big resolution (e.g.new ipad) app on desktop.
     */
    void setFrameZoomFactor(float fZoomFactor);
	float getFrameZoomFactor();
    // @note RobTop Addition: made non virtual
    void centerWindow();
    // @note RobTop Addition
    bool windowShouldClose();
    // @note RobTop Addition
    void showCursor(bool state);
	    
    typedef void (*LPFN_ACCELEROMETER_KEYHOOK)( UINT message,WPARAM wParam, LPARAM lParam );
    void setAccelerometerKeyHook( LPFN_ACCELEROMETER_KEYHOOK lpfnAccelerometerKeyHook );

    virtual void setViewPortInPoints(float x , float y , float w , float h);
    virtual void setScissorInPoints(float x , float y , float w , float h);
    
    // static function
    /**
    @brief    get the shared main open gl window
    */
    static CCEGLView* sharedOpenGLView();

    /**
     * @note Geode addition
     */
    static GEODE_DLL CCEGLView* get();

    /**
     * @note RobTop addition
     */
    static CCEGLView* create(const gd::string&);

    static cocos2d::CCEGLView* createWithFullScreen(gd::string const&, bool);
	static cocos2d::CCEGLView* createWithFullScreen(gd::string const&, bool, GLFWvidmode const&, GLFWmonitor*);
	static cocos2d::CCEGLView* createWithRect(gd::string const&, cocos2d::CCRect, float);

    /**
     * @note Geode addition
     */
    inline CCPoint getMousePosition() { return { m_fMouseX, m_fMouseY }; }

    /**
     * @note RobTop addition
     */
    void toggleFullScreen(bool fullscreen, bool borderless);

    /**
     * @note RobTop addition
     */
    GLFWwindow* getWindow(void) const;

    /**
     * @note RobTop addition
     */
    CCSize getDisplaySize();

	void capture();
	void checkErrorGL(char const*);

	void enableRetina(bool);

	bool getCursorLocked() const;
	bool getGameplayActive() const;
	bool getIsBorderless() const;
	bool getIsFullscreen() const;
	int getRetinaFactor() const;
	bool getShouldHideCursor() const;
	void iconify();

    bool initWithFullScreen(gd::string const&, bool);
	bool initWithFullscreen(gd::string const&, bool, GLFWvidmode const&, GLFWmonitor*);
	bool initWithRect(gd::string const&, cocos2d::CCRect, float);

	bool isRetinaEnabled() const;

	void onGLFWWindowCloseFunCallback(GLFWwindow*);
	void releaseCapture();
	void showMessage(gd::string);

	void toggleGameplayActive(bool);
	void toggleLockCursor(bool);
	void updateDesignSize(int, int);
	void updateFrameSize();


protected:
	static CCEGLView* s_pEglView;
    bool m_bCaptured;
    // Robtop Removal
    // HWND m_hWnd;
    // Robtop Removal
    // HDC  m_hDC;
    // Robtop Removal
    // HGLRC m_hRC;
    // Robtop Removal
    // LPFN_ACCELEROMETER_KEYHOOK m_lpfnAccelerometerKeyHook;
    bool m_bSupportTouch;
    // @note RobTop Addition
    bool m_bInRetinaMonitor;
    // @note RobTop Addition
    bool m_bRetinaEnabled;
    // @note RobTop Addition
    int m_nRetinaFactor;
    // @note RobTop Addition
    bool m_bCursorHidden;
    // Robtop Removal
    // LPCWSTR m_menu;
    // Robtop Removal
    // CUSTOM_WND_PROC m_wndproc;
    float m_fFrameZoomFactor;
    // @note RobTop Addition
    GLFWwindow* m_pMainWindow;
    // @note RobTop Addition
    GLFWmonitor* m_pPrimaryMonitor;
public:
    // @note RobTop Addition
    CC_SYNTHESIZE_NV(CCSize, m_obWindowedSize, WindowedSize);

    // @note RobTop Addition
    float m_fMouseX;
    // @note RobTop Addition
    float m_fMouseY;
    // @note RobTop Addition
    bool m_bIsFullscreen;
    // @note RobTop Addition
    bool m_bIsBorderless;
    // @note RobTop Addition
    bool m_bShouldHideCursor;
    // @note RobTop Addition
    bool m_bCursorLocked;
    // @note RobTop Addition
    bool m_bShouldCallGLFinish;

protected:
    // @note RobTop Addition
    void onGLFWCharCallback(GLFWwindow* window, unsigned int entered);
    // @note RobTop Addition
    void onGLFWCursorEnterFunCallback(GLFWwindow* window, int entered);
    // @note RobTop Addition
    void onGLFWDeviceChangeFunCallback(GLFWwindow* window);
    // @note RobTop Addition
    void onGLFWError(int code, const char* description);
    // @note RobTop Addition
    void onGLFWframebuffersize(GLFWwindow* window, int width, int height);
    // @note RobTop Addition
    void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y);
    // @note RobTop Addition
    void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int mods);
    // @note RobTop Addition
    void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    // @note RobTop Addition
    void onGLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    // @note RobTop Addition
    void onGLFWWindowIconifyFunCallback(GLFWwindow* window, int iconified);
    // @note RobTop Addition
    void onGLFWWindowPosCallback(GLFWwindow* window, int x, int y);
    // @note RobTop Addition
    void onGLFWWindowSizeFunCallback(GLFWwindow* window, int width, int height);
};

NS_CC_END

#endif    // end of __CC_EGLVIEW_WIN32_H__
