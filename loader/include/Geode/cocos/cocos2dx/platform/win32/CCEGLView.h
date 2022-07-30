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
#include "platform/CCCommon.h"
#include "cocoa/CCGeometry.h"
#include "platform/CCEGLViewProtocol.h"

#include "robtop/glfw/glfw3.h"

NS_CC_BEGIN

typedef LRESULT (*CUSTOM_WND_PROC)(UINT message, WPARAM wParam, LPARAM lParam, BOOL* pProcessed);

class CCEGL;

class CC_DLL CCEGLView : public CCEGLViewProtocol RT_ADD(, public CCObject)
{
    GEODE_FRIEND_MODIFY
protected:
    RT_ADD( virtual ~CCEGLView(); )
public:
    CCEGLView();
    RT_REMOVE(  virtual ~CCEGLView();   )

    /* override functions */
    virtual bool isOpenGLReady();
    virtual void end();
    virtual void swapBuffers();
    virtual void setFrameSize(float width, float height);
	RT_REMOVE(  virtual void setEditorFrameSize(float width, float height,HWND hWnd);   )
    virtual void setIMEKeyboardState(bool bOpen);
    void updateWindow(int width, int height);
    void pollEvents(void);

    void setMenuResource(LPCWSTR menu);
    void setWndProc(CUSTOM_WND_PROC proc);

protected:
    RT_REMOVE(  virtual bool Create();  )
public:
    bool initGL();
    void destroyGL();

    RT_REMOVE(  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam); )

	void setHWnd(HWND hWnd);
    // win32 platform function
    RT_REMOVE( HWND getHWnd(); )
    RT_REMOVE(  virtual void resize(int width, int height); )
    RT_ADD(     void resizeWindow(int width, int height);   )
	
    /* 
     * Set zoom factor for frame. This method is for debugging big resolution (e.g.new ipad) app on desktop.
     */
    void setFrameZoomFactor(float fZoomFactor);
	float getFrameZoomFactor();
    RT_REMOVE(  virtual void centerWindow();    )
    RT_ADD(     void centerWindow();            )

    RT_ADD(     void showCursor(bool state);    )
	    
    typedef void (*LPFN_ACCELEROMETER_KEYHOOK)( UINT message,WPARAM wParam, LPARAM lParam );
    void setAccelerometerKeyHook( LPFN_ACCELEROMETER_KEYHOOK lpfnAccelerometerKeyHook );

    virtual void setViewPortInPoints(float x , float y , float w , float h);
    virtual void setScissorInPoints(float x , float y , float w , float h);
    
    // static function
    /**
    @brief    get the shared main open gl window
    */
    static CCEGLView* sharedOpenGLView();
    RT_ADD( static CCEGLView* create(const gd::string&);   )

    RT_ADD(
        //actually this is my function but i dont wanna make a new macro for it
        inline CCPoint getMousePosition() { return { m_fMouseX, m_fMouseY }; }

        void toggleFullScreen(bool fullscreen);

        GLFWwindow* getWindow(void) const;
    )

protected:
	static CCEGLView* s_pEglView;
    bool m_bCaptured;
    RT_REMOVE(
    HWND m_hWnd;
    HDC  m_hDC;
    HGLRC m_hRC;
    LPFN_ACCELEROMETER_KEYHOOK m_lpfnAccelerometerKeyHook;
    )
    bool m_bSupportTouch;
    RT_ADD(
        bool m_bInRetinaMonitor;
        bool m_bRetinaEnabled;
        int m_nRetinaFactor;
        bool m_bCursorHidden;
    )
    RT_REMOVE(
    LPCWSTR m_menu;
    CUSTOM_WND_PROC m_wndproc;
    )
    float m_fFrameZoomFactor;
    RT_ADD(
        GLFWwindow* m_pMainWindow;
        GLFWmonitor* m_pPrimaryMonitor;
        CCSize m_obWindowedSize;
        float m_fMouseX;
        float m_fMouseY;
        bool m_bIsFullscreen;
        bool m_bShouldHideCursor;
        bool m_bShouldCallGLFinish;
    )

    RT_ADD(
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
    )
};

NS_CC_END

#endif    // end of __CC_EGLVIEW_WIN32_H__
