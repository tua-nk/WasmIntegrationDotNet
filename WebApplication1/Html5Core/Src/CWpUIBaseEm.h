#pragma once
#include "CWpUIBase.h"
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>

#include <GLES3/gl3.h>
#define GLM_FORCE_PURE
#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"

static inline const char* emscripten_event_type_to_string(int eventType) {
    const char* events[] = { "(invalid)", "(none)", "keypress", "keydown", "keyup", "click", "mousedown", "mouseup", "dblclick", "mousemove", "wheel", "resize",
      "scroll", "blur", "focus", "focusin", "focusout", "deviceorientation", "devicemotion", "orientationchange", "fullscreenchange", "pointerlockchange",
      "visibilitychange", "touchstart", "touchend", "touchmove", "touchcancel", "gamepadconnected", "gamepaddisconnected", "beforeunload",
      "batterychargingchange", "batterylevelchange", "webglcontextlost", "webglcontextrestored", "mouseenter", "mouseleave", "mouseover", "mouseout", "(invalid)" };
    ++eventType;
    if (eventType < 0) eventType = 0;
    if (eventType >= sizeof(events) / sizeof(events[0])) eventType = sizeof(events) / sizeof(events[0]) - 1;
    return events[eventType];
}

const char* emscripten_result_to_string(EMSCRIPTEN_RESULT result);

#define TEST_RESULT(x) if (ret != EMSCRIPTEN_RESULT_SUCCESS) printf("%s returned %s.\n", #x, emscripten_result_to_string(ret));

// Console Output 
void ToConsole(std::string ConOut);
void ToConsole(std::string ConOut, sScreenPos sPos);
void ToConsole(std::string ConOut, int x, int y);
void ToError(char* pztsError);

//========================================================================================================
// Emscripten Callback fucntions with translation code to Warp call

EM_BOOL em_key_callback(int eventType, const EmscriptenKeyboardEvent* e, void* userData);
EM_BOOL em_mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData);
EM_BOOL em_wheel_callback(int eventType, const EmscriptenWheelEvent* e, void* userData);
EM_BOOL em_uievent_callback(int eventType, const EmscriptenUiEvent* e, void* userData);
EM_BOOL em_focusevent_callback(int eventType, const EmscriptenFocusEvent* e, void* userData);
EM_BOOL em_deviceorientation_callback(int eventType, const EmscriptenDeviceOrientationEvent* e, void* userData);
EM_BOOL em_devicemotion_callback(int eventType, const EmscriptenDeviceMotionEvent* e, void* userData);
EM_BOOL em_orientationchange_callback(int eventType, const EmscriptenOrientationChangeEvent* e, void* userData);
EM_BOOL em_fullscreenchange_callback(int eventType, const EmscriptenFullscreenChangeEvent* e, void* userData);
EM_BOOL em_pointerlockchange_callback(int eventType, const EmscriptenPointerlockChangeEvent* e, void* userData);
EM_BOOL em_visibilitychange_callback(int eventType, const EmscriptenVisibilityChangeEvent* e, void* userData);
EM_BOOL em_touch_callback(int eventType, const EmscriptenTouchEvent* e, void* userData);
const char* beforeunload_callback(int eventType, const void* reserved, void* userData);
void formatTime(char* str, int seconds);
EM_BOOL em_battery_callback(int eventType, const EmscriptenBatteryEvent* e, void* userData);
EM_BOOL em_webglcontext_callback(int eventType, const void* reserved, void* userData);
//...ETC..

//========================================================================================================
// Emscripten Init Callbacks
void em_init();

//========================================================================================================
// Emscripten Render Call in main loop (for animation) 
void em_render();

//========================================================================================================
// New Class override to impliment Emscripten UI framework
class CWpUIBaseEm : public CWpUIBase
{
public:
    CWpUIBaseEm();

    //____________________________________
    // Init Plateform Specfic UI base code (emscripton in this case, but could also be GLFW or SDL etc)
    virtual void Init();

    //__________________________
    // Render Call for animation
    virtual void OnRenderAnimate();

    //========================================================================================================
    // WARP APPLICATION UI EVENTS CALLBACK API

    //_________________________
    // KEYBOARD EVENT CALLBACKS
    virtual void OnKeyboard(char cKey, bool bDown, bool bAlt, bool bShift, bool bCtrl, bool bFn);

    //______________________
    // MOUSE EVENT CALLBACKS
    virtual void OnMouse_Move(sScreenPos sNewPos);
    virtual void OnMouse_Click(eWpMouseButton eButton, bool bDown, sScreenPos sMousePos);
    virtual void OnMouse_DoubleClick(eWpMouseButton eButton, sScreenPos sPos);
    virtual void OnMouse_Wheel(float fDx, float fDy);
    virtual void OnMouse_EnterWin(sScreenPos sPos);
    virtual void OnMouse_LeaveWin(sScreenPos sPos);
    virtual void OnMouse_Over(sScreenPos sPos);
    virtual void OnMouse_Out(sScreenPos sPos);

    //_______________________
    // WINDOW EVENT CALLBACKS
    virtual void OnWin_Resize(int iNewW, int iNewH);
    virtual void OnWin_Minimized();
    virtual void OnWin_Maximized();
    virtual void OnWin_Regular();
    virtual void OnWin_FocusChange(bool bFocusGained);

    //________________________________
    // "DRAG AND DROP" EVENT CALLBACKS
    virtual void OnDrag(bool bStart);
    virtual void OnDropFiles(std::vector<std::string> FileNameList, sScreenPos MousePos);
    virtual void OnDropText(std::string strText, sScreenPos MousePos);

    //========================================================================================================
    // WARP APPLICATION UI SET/GET FUNCTION API
    //   - Platform independant code - calls Plateform specific event functions (Emscription HTML5 / GLFW / SDL)

    //___________
    // WINDOW API
    virtual void Win_SetTitle(std::string strTitle);
    virtual std::string Win_GetTitle();

    virtual void Win_SetFocus();
    virtual void Win_SetSize(int iWidth, int iHeight);
    virtual sWinSize Win_GetSize();

    //__________
    // MOUSE API
    sScreenPos Mouse_PosGet();
    virtual void Mouse_PosSet(sScreenPos NewPos);
    virtual void Mouse_CurserSet(eWpCurserType eWpNewCurser);
    virtual void Mouse_CurserShow();
    virtual void Mouse_CurserHide();

    //______________
    // CLIPBOARD API
    virtual void ClipboardSet(std::string strClipText);
    virtual std::string ClipboardGet();

    //...ETC..
};