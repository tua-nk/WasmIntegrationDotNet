#pragma once
#include <iostream>
#include <string>
#include <vector>

// screen position
struct sScreenPos {
    int iXpos;
    int iYpos;
    sScreenPos() {
        iXpos = 0;
        iYpos = 0;
    }
    sScreenPos(int x, int y) {
        iXpos = x;
        iYpos = y;
    }
};

// screen position
struct sWinSize {
    int iW;
    int iH;
    sWinSize() {
        iW = 0;
        iH = 0;
    }
    sWinSize (int w, int h) {
        iW = w;
        iH = h;
    }
};

enum eWpMouseButton {
    LeftButton,
    MiddleButton,
    RightButton
};

enum eWpCurserType {
    eWpCurser_Default,
    eWpCurser_Pointer,
    eWpCurser_Wait,
    eWpCurser_None
    // ...
};

class CWpUIBase
{
public:
    bool m_bAnimateOn;
    bool RenAnimateIsOn();

    CWpUIBase();

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
};