#include "CWpUIBaseEm.h"

// Ref to call App object functions from Emscripten C functions.
CWpUIBaseEm* pWpUIEm = NULL;

const char* emscripten_result_to_string(EMSCRIPTEN_RESULT result) {
    if (result == EMSCRIPTEN_RESULT_SUCCESS) return "EMSCRIPTEN_RESULT_SUCCESS";
    if (result == EMSCRIPTEN_RESULT_DEFERRED) return "EMSCRIPTEN_RESULT_DEFERRED";
    if (result == EMSCRIPTEN_RESULT_NOT_SUPPORTED) return "EMSCRIPTEN_RESULT_NOT_SUPPORTED";
    if (result == EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED) return "EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED";
    if (result == EMSCRIPTEN_RESULT_INVALID_TARGET) return "EMSCRIPTEN_RESULT_INVALID_TARGET";
    if (result == EMSCRIPTEN_RESULT_UNKNOWN_TARGET) return "EMSCRIPTEN_RESULT_UNKNOWN_TARGET";
    if (result == EMSCRIPTEN_RESULT_INVALID_PARAM) return "EMSCRIPTEN_RESULT_INVALID_PARAM";
    if (result == EMSCRIPTEN_RESULT_FAILED) return "EMSCRIPTEN_RESULT_FAILED";
    if (result == EMSCRIPTEN_RESULT_NO_DATA) return "EMSCRIPTEN_RESULT_NO_DATA";
    return "Unknown EMSCRIPTEN_RESULT!";
}

// Console Output 
void WpUI_ToConsoleEm(std::string& ConOut)
{
    // Send to WASM console
    printf(ConOut.c_str());
}

void ToError(char* pztsError)
{
    // Send to WASM console
    printf(pztsError);
}

//========================================================================================================
// Emscripten Callback fucntions with translation code to Warp call

EM_BOOL em_key_callback(int eventType, const EmscriptenKeyboardEvent* e, void* userData)
{
    printf("%s, key: \"%s\", code: \"%s\", location: %lu,%s%s%s%s repeat: %d, locale: \"%s\", char: \"%s\", charCode: %lu, keyCode: %lu, which: %lu, timestamp: %lf\n",
        emscripten_event_type_to_string(eventType), e->key, e->code, e->location,
        e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "",
        e->repeat, e->locale, e->charValue, e->charCode, e->keyCode, e->which,
        e->timestamp);

    // Translation of emscripten params to "Warp params"...
    char cKey; bool bDown; bool bAlt; bool bShift; bool bCtrl; bool bFn;
    //cKey = e->key;
    //bAlt = e->altKey ? true: false;
    //bShift = e->shiftKey ? true : false;
    //bCtrl = e->ctrlKey ? true: false;

    // Call Warp base class equivalent func
    if (pWpUIEm != NULL) pWpUIEm->OnKeyboard(cKey, bDown, bAlt, bShift, bCtrl, bFn);
    else ToError("em_key_callback");

    return 0;
}

EM_BOOL em_mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData)
{
    printf("%s, screen: (%ld,%ld), client: (%ld,%ld),%s%s%s%s button: %hu, buttons: %hu, movement: (%ld,%ld), canvas: (%ld,%ld), timestamp: %lf\n",
        emscripten_event_type_to_string(eventType), e->screenX, e->screenY, e->clientX, e->clientY,
        e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "",
        e->button, e->buttons, e->movementX, e->movementY, e->canvasX, e->canvasY,
        e->timestamp);

    // Translation of emscripten params to "Warp params"...
    sScreenPos sNewPos;
    sNewPos.iXpos = e->movementX;
    sNewPos.iYpos = e->movementY;

    // Call Warp base class equivalent func
    if (pWpUIEm != NULL) pWpUIEm->OnMouse_Move(sNewPos);
    else ToError("em_mouse_callback");

    return 0;
}

EM_BOOL em_wheel_callback(int eventType, const EmscriptenWheelEvent* e, void* userData)
{
    printf("%s, screen: (%ld,%ld), client: (%ld,%ld),%s%s%s%s button: %hu, buttons: %hu, canvas: (%ld,%ld), delta:(%g,%g,%g), deltaMode:%lu, timestamp: %lf\n",
        emscripten_event_type_to_string(eventType), e->mouse.screenX, e->mouse.screenY, e->mouse.clientX, e->mouse.clientY,
        e->mouse.ctrlKey ? " CTRL" : "", e->mouse.shiftKey ? " SHIFT" : "", e->mouse.altKey ? " ALT" : "", e->mouse.metaKey ? " META" : "",
        e->mouse.button, e->mouse.buttons, e->mouse.canvasX, e->mouse.canvasY,
        (float)e->deltaX, (float)e->deltaY, (float)e->deltaZ, e->deltaMode,
        e->mouse.timestamp);

    return 0;
}

EM_BOOL em_uievent_callback(int eventType, const EmscriptenUiEvent* e, void* userData)
{
    printf("%s, detail: %ld, document.body.client size: (%d,%d), window.inner size: (%d,%d), scrollPos: (%d, %d)\n",
        emscripten_event_type_to_string(eventType), e->detail, e->documentBodyClientWidth, e->documentBodyClientHeight,
        e->windowInnerWidth, e->windowInnerHeight, e->scrollTop, e->scrollLeft);

    return 0;
}

EM_BOOL em_focusevent_callback(int eventType, const EmscriptenFocusEvent* e, void* userData)
{
    printf("%s, nodeName: \"%s\", id: \"%s\"\n", emscripten_event_type_to_string(eventType), e->nodeName, e->id[0] == '\0' ? "(empty string)" : e->id);

    return 0;
}

EM_BOOL em_deviceorientation_callback(int eventType, const EmscriptenDeviceOrientationEvent* e, void* userData)
{
    printf("%s, (%g, %g, %g)\n", emscripten_event_type_to_string(eventType), e->alpha, e->beta, e->gamma);

    return 0;
}

EM_BOOL em_devicemotion_callback(int eventType, const EmscriptenDeviceMotionEvent* e, void* userData)
{
    printf("%s, accel: (%g, %g, %g), accelInclGravity: (%g, %g, %g), rotationRate: (%g, %g, %g), supportedFields: %s %s %s\n",
        emscripten_event_type_to_string(eventType),
        e->accelerationX, e->accelerationY, e->accelerationZ,
        e->accelerationIncludingGravityX, e->accelerationIncludingGravityY, e->accelerationIncludingGravityZ,
        e->rotationRateAlpha, e->rotationRateBeta, e->rotationRateGamma,
        (e->supportedFields & EMSCRIPTEN_DEVICE_MOTION_EVENT_SUPPORTS_ACCELERATION) ? "EMSCRIPTEN_DEVICE_MOTION_EVENT_SUPPORTS_ACCELERATION" : "",
        (e->supportedFields & EMSCRIPTEN_DEVICE_MOTION_EVENT_SUPPORTS_ACCELERATION_INCLUDING_GRAVITY) ? "EMSCRIPTEN_DEVICE_MOTION_EVENT_SUPPORTS_ACCELERATION_INCLUDING_GRAVITY" : "",
        (e->supportedFields & EMSCRIPTEN_DEVICE_MOTION_EVENT_SUPPORTS_ROTATION_RATE) ? "EMSCRIPTEN_DEVICE_MOTION_EVENT_SUPPORTS_ROTATION_RATE" : "");

    return 0;
}

EM_BOOL em_orientationchange_callback(int eventType, const EmscriptenOrientationChangeEvent* e, void* userData)
{
    printf("%s, orientationAngle: %d, orientationIndex: %d\n", emscripten_event_type_to_string(eventType), e->orientationAngle, e->orientationIndex);

    return 0;
}

EM_BOOL em_fullscreenchange_callback(int eventType, const EmscriptenFullscreenChangeEvent* e, void* userData)
{
    printf("%s, isFullscreen: %d, fullscreenEnabled: %d, fs element nodeName: \"%s\", fs element id: \"%s\". New size: %dx%d pixels. Screen size: %dx%d pixels.\n",
        emscripten_event_type_to_string(eventType), e->isFullscreen, e->fullscreenEnabled, e->nodeName, e->id, e->elementWidth, e->elementHeight, e->screenWidth, e->screenHeight);

    return 0;
}

EM_BOOL em_pointerlockchange_callback(int eventType, const EmscriptenPointerlockChangeEvent* e, void* userData)
{
    printf("%s, isActive: %d, pointerlock element nodeName: \"%s\", id: \"%s\"\n",
        emscripten_event_type_to_string(eventType), e->isActive, e->nodeName, e->id);

    return 0;
}

EM_BOOL em_visibilitychange_callback(int eventType, const EmscriptenVisibilityChangeEvent* e, void* userData)
{
    printf("%s, hidden: %d, visibilityState: %d\n", emscripten_event_type_to_string(eventType), e->hidden, e->visibilityState);

    return 0;
}

EM_BOOL em_touch_callback(int eventType, const EmscriptenTouchEvent* e, void* userData)
{
    printf("%s, numTouches: %d timestamp: %lf %s%s%s%s\n",
        emscripten_event_type_to_string(eventType), e->numTouches, e->timestamp,
        e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "");
    for (int i = 0; i < e->numTouches; ++i)
    {
        const EmscriptenTouchPoint* t = &e->touches[i];
        printf("  %ld: screen: (%ld,%ld), client: (%ld,%ld), page: (%ld,%ld), isChanged: %d, onTarget: %d, canvas: (%ld, %ld)\n",
            t->identifier, t->screenX, t->screenY, t->clientX, t->clientY, t->pageX, t->pageY, t->isChanged, t->onTarget, t->canvasX, t->canvasY);
    }

    return 0;
}

const char* beforeunload_callback(int eventType, const void* reserved, void* userData)
{
#ifdef REPORT_RESULT
    return ""; // For test harness, don't show a confirmation dialog to not block and keep the test runner automated.
#else
    return "Do you really want to leave the page?";
#endif
}

void formatTime(char* str, int seconds)
{
    int h = seconds / (60 * 60);
    seconds -= h * 60 * 60;
    int m = seconds / 60;
    seconds -= m * 60;
    if (h > 0)
    {
        sprintf(str, "%dh:%02dm:%02ds", h, m, seconds);
    }
    else
    {
        sprintf(str, "%02dm:%02ds", m, seconds);
    }
}

EM_BOOL em_battery_callback(int eventType, const EmscriptenBatteryEvent* e, void* userData)
{
    char t1[64];
    formatTime(t1, (int)e->chargingTime);
    char t2[64];
    formatTime(t2, (int)e->dischargingTime);
    printf("%s: chargingTime: %s, dischargingTime: %s, level: %g%%, charging: %d\n",
        emscripten_event_type_to_string(eventType), t1, t2, e->level * 100, e->charging);

    return 0;
}

EM_BOOL em_webglcontext_callback(int eventType, const void* reserved, void* userData)
{
    printf("%s.\n", emscripten_event_type_to_string(eventType));

    return 0;
}

//...ETC..

//========================================================================================================
// Emscripten Init Callbacks

void em_init()
{
    // Set Enscripton call back funcs
    EMSCRIPTEN_RESULT ret = emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_key_callback);
    TEST_RESULT(emscripten_set_keypress_callback);

    ret = emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_mouse_callback);
    TEST_RESULT(emscripten_set_click_callback);

    //....ETC..
}

//========================================================================================================
// Emscripten Render Call in main loop (for animation) 
void em_render()
{
    if (pWpUIEm != NULL) {
        if (pWpUIEm->RenAnimateIsOn()) pWpUIEm->OnRenderAnimate();
    }
    else ToError("em_render");
}

CWpUIBaseEm::CWpUIBaseEm()
{
    if (pWpUIEm == NULL) pWpUIEm = this; // setup ref to this object for callbacks
}

void CWpUIBaseEm::Init()
{
    // Setup Base class UI Event call back functions
    em_init(); // setup callbacks etc.
}

// ____________________________________
// Impliment Base class Setters/Getters

void CWpUIBaseEm::Win_SetTitle(std::string strTitle) {
    // Emscripten specific code to Set the window tittle
    emscripten_set_window_title(strTitle.c_str());
}

std::string CWpUIBaseEm::Win_GetTitle() {
    // Emscripten specific code to Set the window tittle
    char* current_window_title = emscripten_get_window_title();
}

void CWpUIBaseEm::Mouse_CurserSet(eWpCurserType eWpNewCurser)
{
    switch (eWpNewCurser) {
        case eWpCurserType::eWpCurser_Arrow:
        {
            // Emscripten specific code to set to new curser type to an "Arrow"
            break;
        }
        case eWpCurserType::eWpCurser_IBeam:
        {
            //.etc
            break;
        }
    }
}