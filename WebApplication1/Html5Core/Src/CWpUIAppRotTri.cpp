#include "CWpUIAppRotTri.h"

// Application specific implimentations of UI events and functions
void CWpUIAppRotTri::OnKeyboard(char cKey, bool bDown, bool bAlt, bool bShift, bool bCtrl, bool bFn)
{
    // if bDown = false then key is going UP.
    // Override to handle keyboard events
    if (bDown) {
        switch (cKey)
        {
            case 'c':
            {
                color = !color; // invert rotating triangle colour
                break;
            }
            case 't':
            {
                Win_SetTitle(std::string("New Warp Title"));
                break;
            }
            //...ETC.

            default:
                break;
        }
    }
}

void CWpUIAppRotTri::OnMouse_Move(sScreenPos sNewPos)
{
    // move centre of rotating Triangle
}

void CWpUIAppRotTri::OnRenderAnimate()
{
    // Call OpenGL draw code
}
