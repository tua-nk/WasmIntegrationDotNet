#pragma once
#include "CWpUIBaseEm.h"

class CWpUIAppRotTri : public  CWpUIBaseEm
{
public:
    CWpUIAppRotTri();

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

    //_______________________
    // WINDOW EVENT CALLBACKS
    virtual void OnWin_Resize(int iNewH, int iNewW);
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

    // ...ETC.
};