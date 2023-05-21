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
void WpUI_ToConsoleEm(std::string ConOut)
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
    cKey = e->key[0];
    bDown = eventType == EMSCRIPTEN_EVENT_KEYDOWN? true: false;
    bAlt = e->altKey ? true: false;
    bShift = e->shiftKey ? true : false;
    bCtrl = e->ctrlKey ? true: false;

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
    sScreenPos sNewPos(e->screenX, e->screenY);
    bool bDown = eventType == EMSCRIPTEN_EVENT_MOUSEDOWN ? true : false;

    // Call Warp base class equivalent func
    if (pWpUIEm != NULL) pWpUIEm->OnMouse_Move(sNewPos);
    else ToError("em_mouse_callback");

    if (pWpUIEm != NULL)
    {
        switch (eventType)
        {
        case EMSCRIPTEN_EVENT_MOUSEMOVE:
        {
            pWpUIEm->OnMouse_Move(sNewPos);
            break;
        }
        case EMSCRIPTEN_EVENT_CLICK:
        {
            pWpUIEm->OnMouse_Click((eWpMouseButton)e->button, bDown, sNewPos);
            break;
        }
        case EMSCRIPTEN_EVENT_DBLCLICK:
        {
            pWpUIEm->OnMouse_DoubleClick((eWpMouseButton)e->button, sNewPos);
            break;
        }
        case EMSCRIPTEN_EVENT_WHEEL:
        {
            //virtual void OnMouse_Wheel(float fDx, float fDy);
            break;
        }
        case EMSCRIPTEN_EVENT_MOUSEENTER:
        {
            pWpUIEm->OnMouse_EnterWin(sNewPos);
            break;
        }
        case EMSCRIPTEN_EVENT_MOUSELEAVE:
        {
            pWpUIEm->OnMouse_LeaveWin(sNewPos);
            break;
        }
        default:
            break;
        }
    }

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
    ret = emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_key_callback);
    TEST_RESULT(emscripten_set_keydown_callback);
    ret = emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_key_callback);
    TEST_RESULT(emscripten_set_keyup_callback);

    ret = emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_mouse_callback);
    TEST_RESULT(emscripten_set_click_callback);
    ret = emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_mouse_callback);
    TEST_RESULT(emscripten_set_mousedown_callback);
    ret = emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_mouse_callback);
    TEST_RESULT(emscripten_set_mouseup_callback);
    ret = emscripten_set_dblclick_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_mouse_callback);
    TEST_RESULT(emscripten_set_dblclick_callback);
    ret = emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_mouse_callback);
    TEST_RESULT(emscripten_set_mousemove_callback);
    ret = emscripten_set_mouseenter_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_mouse_callback);
    TEST_RESULT(emscripten_set_mouseenter_callback);
    ret = emscripten_set_mouseleave_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_mouse_callback);
    TEST_RESULT(emscripten_set_mouseleave_callback);
    ret = emscripten_set_mouseover_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_mouse_callback);
    TEST_RESULT(emscripten_set_mouseover_callback);
    ret = emscripten_set_mouseout_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_mouse_callback);
    TEST_RESULT(emscripten_set_mouseout_callback);

    ret = emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_wheel_callback);
    TEST_RESULT(emscripten_set_wheel_callback);

    ret = emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_uievent_callback);
    TEST_RESULT(emscripten_set_resize_callback);
    ret = emscripten_set_scroll_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, 1, em_uievent_callback);
    TEST_RESULT(emscripten_set_scroll_callback);

    ret = emscripten_set_blur_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_focusevent_callback);
    TEST_RESULT(emscripten_set_blur_callback);
    ret = emscripten_set_focus_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_focusevent_callback);
    TEST_RESULT(emscripten_set_focus_callback);
    ret = emscripten_set_focusin_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_focusevent_callback);
    TEST_RESULT(emscripten_set_focusin_callback);
    ret = emscripten_set_focusout_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_focusevent_callback);
    TEST_RESULT(emscripten_set_focusout_callback);

    ret = emscripten_set_deviceorientation_callback(0, 1, em_deviceorientation_callback);
    TEST_RESULT(emscripten_set_deviceorientation_callback);
    ret = emscripten_set_devicemotion_callback(0, 1, em_devicemotion_callback);
    TEST_RESULT(emscripten_set_devicemotion_callback);

    ret = emscripten_set_orientationchange_callback(0, 1, em_orientationchange_callback);
    TEST_RESULT(emscripten_set_orientationchange_callback);

    // Test the polling of orientation.
    EmscriptenOrientationChangeEvent oce;
    ret = emscripten_get_orientation_status(&oce);
    TEST_RESULT(emscripten_get_orientation_status);
    if (ret == EMSCRIPTEN_RESULT_SUCCESS) {
        printf("The current orientation is:\n");
        em_orientationchange_callback(EMSCRIPTEN_EVENT_ORIENTATIONCHANGE, &oce, 0);
    }

    int newOrientation = (oce.orientationIndex == EMSCRIPTEN_ORIENTATION_PORTRAIT_PRIMARY
        || oce.orientationIndex == EMSCRIPTEN_ORIENTATION_PORTRAIT_SECONDARY) ? EMSCRIPTEN_ORIENTATION_LANDSCAPE_PRIMARY : EMSCRIPTEN_ORIENTATION_PORTRAIT_PRIMARY;
    // Test locking of orientation.
    ret = emscripten_lock_orientation(newOrientation);
    TEST_RESULT(emscripten_lock_orientation);
    if (ret == EMSCRIPTEN_RESULT_SUCCESS) {
        printf("Locked orientation to state %d.\n", newOrientation);
    }

    ret = emscripten_get_orientation_status(&oce);
    TEST_RESULT(emscripten_get_orientation_status);
    if (ret == EMSCRIPTEN_RESULT_SUCCESS) {
        printf("The current orientation is after locking:\n");
        em_orientationchange_callback(18, &oce, 0);
    }

    ret = emscripten_unlock_orientation();
    TEST_RESULT(emscripten_unlock_orientation);
    if (ret == EMSCRIPTEN_RESULT_SUCCESS) {
        printf("Unlocked orientation.\n");
    }

    EmscriptenFullscreenChangeEvent fsce;
    ret = emscripten_get_fullscreen_status(&fsce);
    TEST_RESULT(emscripten_get_fullscreen_status);
    if (ret == EMSCRIPTEN_RESULT_SUCCESS) {
        printf("The current fullscreen status is:\n");
        em_fullscreenchange_callback(EMSCRIPTEN_EVENT_FULLSCREENCHANGE, &fsce, 0);
    }

    ret = emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, 1, em_fullscreenchange_callback);
    TEST_RESULT(emscripten_set_fullscreenchange_callback);

    // These won't do anything, since fullscreen must be requested in an event handler,
    // but call these anyways to confirm that they don't crash in an exception in the test suite.
    ret = emscripten_request_fullscreen("#canvas", 1);
    TEST_RESULT(emscripten_request_fullscreen);
    ret = emscripten_exit_fullscreen();
    TEST_RESULT(emscripten_exit_fullscreen);

    EmscriptenPointerlockChangeEvent plce;
    ret = emscripten_get_pointerlock_status(&plce);
    TEST_RESULT(emscripten_get_pointerlock_status);
    if (ret == EMSCRIPTEN_RESULT_SUCCESS) {
        printf("The current pointerlock status is:\n");
        em_pointerlockchange_callback(EMSCRIPTEN_EVENT_POINTERLOCKCHANGE, &plce, 0);
    }

    ret = emscripten_set_pointerlockchange_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, 0, 1, em_pointerlockchange_callback);
    TEST_RESULT(emscripten_set_pointerlockchange_callback);

    // These won't do anything, since pointer lock must be requested in an event handler,
    // but call these anyways to confirm that they don't crash in an exception in the test suite.
    ret = emscripten_request_pointerlock("#canvas", 1);
    TEST_RESULT(emscripten_request_pointerlock);
    ret = emscripten_exit_pointerlock();
    TEST_RESULT(emscripten_exit_pointerlock);

    int vibratePattern[] = {
      150, 500,
      300, 500,
      450
    };
    ret = emscripten_vibrate_pattern(vibratePattern, sizeof(vibratePattern) / sizeof(vibratePattern[0]));
    TEST_RESULT(emscripten_vibrate_pattern);

    EmscriptenVisibilityChangeEvent vce;
    ret = emscripten_get_visibility_status(&vce);
    TEST_RESULT(emscripten_get_visibility_status);
    if (ret == EMSCRIPTEN_RESULT_SUCCESS) {
        printf("Current visibility status:\n");
        em_visibilitychange_callback(EMSCRIPTEN_EVENT_VISIBILITYCHANGE, &vce, 0);
    }

    ret = emscripten_set_visibilitychange_callback(0, 1, em_visibilitychange_callback);
    TEST_RESULT(emscripten_set_visibilitychange_callback);

    ret = emscripten_set_touchstart_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_touch_callback);
    TEST_RESULT(emscripten_set_touchstart_callback);
    ret = emscripten_set_touchend_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_touch_callback);
    TEST_RESULT(emscripten_set_touchend_callback);
    ret = emscripten_set_touchmove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_touch_callback);
    TEST_RESULT(emscripten_set_touchmove_callback);
    ret = emscripten_set_touchcancel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, em_touch_callback);
    TEST_RESULT(emscripten_set_touchcancel_callback);

    ret = emscripten_set_beforeunload_callback(0, beforeunload_callback);
    TEST_RESULT(emscripten_set_beforeunload_callback);

    ret = emscripten_set_batterychargingchange_callback(0, em_battery_callback);
    TEST_RESULT(emscripten_set_batterychargingchange_callback);
    ret = emscripten_set_batterylevelchange_callback(0, em_battery_callback);
    TEST_RESULT(emscripten_set_batterylevelchange_callback);

    EmscriptenBatteryEvent bs;
    ret = emscripten_get_battery_status(&bs);
    TEST_RESULT(emscripten_get_battery_status);
    if (ret == EMSCRIPTEN_RESULT_SUCCESS) {
        printf("Current battery status:\n");
        em_battery_callback(EMSCRIPTEN_EVENT_BATTERYLEVELCHANGE, &bs, 0);
    }

    ret = emscripten_set_webglcontextlost_callback("#canvas", 0, 1, em_webglcontext_callback);
    TEST_RESULT(emscripten_set_webglcontextlost_callback);
    ret = emscripten_set_webglcontextrestored_callback("#canvas", 0, 1, em_webglcontext_callback);
    TEST_RESULT(emscripten_set_webglcontextrestored_callback);

    const char* source_window_title = "test window title";
    emscripten_set_window_title(source_window_title);
    char* current_window_title = emscripten_get_window_title();
    ret = (strcmp(source_window_title, current_window_title) == 0 \
        ? EMSCRIPTEN_RESULT_SUCCESS : EMSCRIPTEN_RESULT_FAILED);
    TEST_RESULT(emscripten_get_window_title);

    int width, height;
    emscripten_get_screen_size(&width, &height);
    ret = (width && height) ? EMSCRIPTEN_RESULT_SUCCESS : EMSCRIPTEN_RESULT_FAILED;
    TEST_RESULT(emscripten_get_screen_size);

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

//========================================================================================================
// New Class override to impliment Emscripten UI framework

CWpUIBaseEm::CWpUIBaseEm()
{
    if (pWpUIEm == NULL) {
            pWpUIEm = this; // setup ref to this object for callbacks
    }
}

//____________________________________
// Init Plateform Specfic UI base code (emscripton in this case, but could also be GLFW or SDL etc)
void CWpUIBaseEm::Init() {
    // Setup Base class UI Event call back functions
    em_init(); // setup callbacks etc.
}

//__________________________
// Render Call for animation
void CWpUIBaseEm::OnRenderAnimate()
{
    // Override to call render code
}

//========================================================================================================
// WARP APPLICATION UI EVENTS CALLBACK API

//_________________________
// KEYBOARD EVENT CALLBACKS
void CWpUIBaseEm::OnKeyboard(char cKey, bool bDown, bool bAlt, bool bShift, bool bCtrl, bool bFn) {
    // if bDown = false then key is going UP.
    // Override to handle keyboard events
    WpUI_ToConsoleEm("CWpUIBaseEm::OnKeyboard");
}

//______________________
// MOUSE EVENT CALLBACKS

void CWpUIBaseEm::OnMouse_Move(sScreenPos sNewPos) {
    // Override
    WpUI_ToConsoleEm("CWpUIBaseEm::OnMouse_Move");
}
void CWpUIBaseEm::OnMouse_Click(eWpMouseButton eButton, bool bDown, sScreenPos sMousePos) {
    // Override
    // if bDown = false then button is going UP.
    WpUI_ToConsoleEm("CWpUIBaseEm::OnMouse_Click");
}

void CWpUIBaseEm::OnMouse_DoubleClick(eWpMouseButton, sScreenPos sPos) {
    // Override
    WpUI_ToConsoleEm("CWpUIBaseEm::OnMouse_DoubleClick");
}
void CWpUIBaseEm::OnMouse_Wheel(float fDx, float fDy) {
    // Override
    WpUI_ToConsoleEm("CWpUIBaseEm::OnMouse_Wheel");
}
void CWpUIBaseEm::OnMouse_EnterWin(sScreenPos sPos) {
    // Override
    WpUI_ToConsoleEm("CWpUIBaseEm::OnMouse_EnterWin");
}
void CWpUIBaseEm::OnMouse_LeaveWin(sScreenPos sPos) {
    // Override
    WpUI_ToConsoleEm("CWpUIBaseEm::OnMouse_LeaveWin");
}

//_______________________
// WINDOW EVENT CALLBACKS

void CWpUIBaseEm::OnWin_Resize(int iNewH, int iNewW) {
    // Send new size to console, use WpWinGetsize()
    WpUI_ToConsoleEm("CWpUIBaseEm::OnWin_Resize");
}
void CWpUIBaseEm::OnWin_Minimized() {
    // Override
    WpUI_ToConsoleEm("CWpUIBaseEm::OnWin_Minimized");
}
void CWpUIBaseEm::OnWin_Maximized() {
    // Override
    WpUI_ToConsoleEm("CWpUIBaseEm::OnWin_Maximized");
}
void CWpUIBaseEm::OnWin_Regular() { // size back to regular from min or max
    // Override
    WpUI_ToConsoleEm("CWpUIBaseEm::OnWin_Regular");
}

void CWpUIBaseEm::OnWin_FocusChange(bool bFocusGained) {
    // if false then focus was lost from this window.
    // Override
    WpUI_ToConsoleEm("CWpUIBaseEm::OnWin_FocusChange");
}

//________________________________
// "DRAG AND DROP" EVENT CALLBACKS
void CWpUIBaseEm::OnDrag(bool bStart) {
    // if bStart is true then draging has started, if false then dragging has stopped
    // Override
}
void CWpUIBaseEm::OnDropFiles(std::vector<std::string> FileNameList, sScreenPos MousePos) {
    // Override
}
void CWpUIBaseEm::OnDropText(std::string strText, sScreenPos MousePos) {
    // Override
}

//========================================================================================================
// WARP APPLICATION UI SET/GET FUNCTION API
//   - Platform independant code - calls Plateform specific event functions (Emscription HTML5 / GLFW / SDL)

//___________
// WINDOW API
void CWpUIBaseEm::Win_SetTitle(std::string strTitle) {
    // Emscripten specific code to Set the window tittle
    emscripten_set_window_title(strTitle.c_str());
}
std::string CWpUIBaseEm::Win_GetTitle() {
    // Emscripten specific code to Set the window tittle
    char* current_window_title = emscripten_get_window_title();
    return std::string(current_window_title);
}

void CWpUIBaseEm::Win_SetFocus() {
    // platform specific code to Set(force?) the focus to this window
}
void CWpUIBaseEm::Win_SetSize(int iHeight, int iWidth) {
    // platform specific code to Set the window size explicitly
}
sWinSize CWpUIBaseEm::Win_GetSize() {
    // platform specific code to Get the window size. send to console
    return sWinSize();
}

//__________
// MOUSE API
sScreenPos CWpUIBaseEm::Mouse_PosGet() {
    // platform specific code to Get the current mouse position
    return sScreenPos();
}
void CWpUIBaseEm::Mouse_PosSet(sScreenPos NewPos) {
    // platform specific code to Set the current mouse position
}
void CWpUIBaseEm::Mouse_CurserSet(eWpCurserType eWpNewCurser) {
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
void CWpUIBaseEm::Mouse_CurserShow() {
    // platform specific code to hide the curser
}
void CWpUIBaseEm::Mouse_CurserHide() {
    // platform specific code to show the curser
}

//______________
// CLIPBOARD API
void CWpUIBaseEm::ClipboardSet(std::string strClipText) {
    // platform specific code to save strClipText to the clip board.
}
std::string CWpUIBaseEm::ClipboardGet() {
    // platform specific code to return clipboard's text in strClipText.
    return "";
}