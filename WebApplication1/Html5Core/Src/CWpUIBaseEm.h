#pragma once
#include "CWpUIBase.h"
#include <emscripten.h>
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
void WpUI_ToConsoleEm(std::string& ConOut);
void ToError(char* pztsError);

//========================================================================================================
// New Class override to impliment Emscripten UI framework
class CWpUIBaseEm : public CWpUIBase
{
public:
    CWpUIBaseEm();

    void Init();

    //___________
    // WINDOW API
    void Win_SetTitle(std::string strTitle);
    std::string Win_GetTitle();

    void Mouse_CurserSet(eWpCurserType eWpNewCurser);

    // ...ETC.. (impliment all base class set/get functions)

};

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