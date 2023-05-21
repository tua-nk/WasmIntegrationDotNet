#pragma once
#include "CWpUIBaseEm.h"

class CWpUIAppRotTri : public  CWpUIBaseEm
{
public:
    // Application specific implimentations of UI events and functions
    void OnKeyboard(char cKey, bool bDown, bool bAlt, bool bShift, bool bCtrl, bool bFn);
    void OnMouse_Move(sScreenPos sNewPos);
    void OnRenderAnimate();

    // ...ETC.
};