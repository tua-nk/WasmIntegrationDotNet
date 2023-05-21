#pragma once
#include <iostream>
#include <string>
#include <vector>

// screen position
struct sScreenPos {
    int iXpos;
    int iYpos;
};

// screen position
struct sWinSize {
    int iH;
    int iW;
};

enum eWpMouseButton {

};

enum eWpCurserType {
    eWpCurser_Arrow,
    eWpCurser_IBeam
};

class CWpUIBase
{
public:
    bool m_bAnimateOn;
    bool RenAnimateIsOn();

    CWpUIBase();

    //____________________________________
    // Init Plateform Specfic UI base code (emscripton in this case, but could also be GLFW or SDL etc)
    void Init();

    //__________________________
    // Render Call for animation
    void OnRenderAnimate();

    //========================================================================================================
    // WARP APPLICATION UI EVENTS CALLBACK API

    //_________________________
    // KEYBOARD EVENT CALLBACKS
    void OnKeyboard(char cKey, bool bDown, bool bAlt, bool bShift, bool bCtrl, bool bFn);

    //______________________
    // MOUSE EVENT CALLBACKS
    void OnMouse_Move(sScreenPos sNewPos);
    void OnMouse_Click(eWpMouseButton eButton, bool bDown, sScreenPos sMousePos);
    void OnMouse_DoubleClick(eWpMouseButton eButton, sScreenPos sPos);
    void OnMouse_Wheel(float fDx, float fDy);
    void OnMouse_EnterWin(sScreenPos sPos);
    void OnMouse_LeaveWin(sScreenPos sPos);

    //_______________________
    // WINDOW EVENT CALLBACKS
    void OnWin_Resize(int iNewH, int iNewW);
    void OnWin_Minimized();
    void OnWin_Maximized();
    void OnWin_Regular();
    void OnWin_FocusChange(bool bFocusGained);

    //________________________________
    // "DRAG AND DROP" EVENT CALLBACKS
    void OnDrag(bool bStart);
    void OnDropFiles(std::vector<std::string> FileNameList, sScreenPos MousePos);
    void OnDropText(std::string strText, sScreenPos MousePos);

    //========================================================================================================
    // WARP APPLICATION UI SET/GET FUNCTION API
    //   - Platform independant code - calls Plateform specific event functions (Emscription HTML5 / GLFW / SDL)

    //___________
    // WINDOW API
    void Win_SetTitle(std::string strTitle);
    std::string Win_GetTitle();

    void Win_SetFocus();
    void Win_SetSize(int iWidth, int iHeight);
    sWinSize Win_GetSize();

    //__________
    // MOUSE API
    sScreenPos Mouse_PosGet();
    void Mouse_PosSet(sScreenPos NewPos);
    void Mouse_CurserSet(eWpCurserType eWpNewCurser);
    void Mouse_CurserShow();
    void Mouse_CurserHide();

    //______________
    // CLIPBOARD API
    void ClipboardSet(std::string strClipText);
    std::string ClipboardGet();
};