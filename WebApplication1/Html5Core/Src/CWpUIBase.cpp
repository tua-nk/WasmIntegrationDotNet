#include "CWpUIBase.h"

bool CWpUIBase::RenAnimateIsOn()
{
    return m_bAnimateOn;
}

//CWpUIBase::CWpUIBase()
//{
//    m_bAnimateOn = false;
//}

//____________________________________
// Init Plateform Specfic UI base code (emscripton in this case, but could also be GLFW or SDL etc)
void CWpUIBase::Init()
{
    // Init Graphics subsystem
}

//__________________________
// Render Call for animation
void CWpUIBase::OnRenderAnimate()
{
    // Override to call render code
}

//========================================================================================================
// WARP APPLICATION UI EVENTS CALLBACK API

//_________________________
// KEYBOARD EVENT CALLBACKS
void CWpUIBase::OnKeyboard(char cKey, bool bDown, bool bAlt, bool bShift, bool bCtrl, bool bFn) {
    // if bDown = false then key is going UP.
    // Override to handle keyboard events
}

//______________________
// MOUSE EVENT CALLBACKS

void CWpUIBase::OnMouse_Move(sScreenPos sNewPos) {
    // Override
}
void CWpUIBase::OnMouse_Click(eWpMouseButton eButton, bool bDown, sScreenPos sMousePos) {
    // Override
    // if bDown = false then button is going UP.
}

void CWpUIBase::OnMouse_DoubleClick(eWpMouseButton, sScreenPos sPos) {
    // Override
}
void CWpUIBase::OnMouse_Wheel(float fDx, float fDy) {
    // Override
}
void CWpUIBase::OnMouse_EnterWin(sScreenPos sPos) {
    // Override
}
void CWpUIBase::OnMouse_LeaveWin(sScreenPos sPos) {
    // Override
}

//_______________________
// WINDOW EVENT CALLBACKS

void CWpUIBase::OnWin_Resize(int iNewH, int iNewW) {
    // Send new size to console, use WpWinGetsize()
}
void CWpUIBase::OnWin_Minimized() {
    // Override
}
void CWpUIBase::OnWin_Maximized() {
    // Override
}
void CWpUIBase::OnWin_Regular() { // size back to regular from min or max
    // Override
}

void CWpUIBase::OnWin_FocusChange(bool bFocusGained) {
    // if false then focus was lost from this window.
    // Override
}

//________________________________
// "DRAG AND DROP" EVENT CALLBACKS
void CWpUIBase::OnDrag(bool bStart) {
    // if bStart is true then draging has started, if false then dragging has stopped
    // Override
}
void CWpUIBase::OnDropFiles(std::vector<std::string> FileNameList, sScreenPos MousePos) {
    // Override
}
void OnDropText(std::string strText, sScreenPos MousePos) {
    // Override
}

//========================================================================================================
// WARP APPLICATION UI SET/GET FUNCTION API
//   - Platform independant code - calls Plateform specific event functions (Emscription HTML5 / GLFW / SDL)

//___________
// WINDOW API
void CWpUIBase::Win_SetTitle(std::string strTitle) {
    // platform specific code to Set the window tittle
}
std::string CWpUIBase::Win_GetTitle() {
    // platform specific code to Set the window tittle
    return "";
}

void CWpUIBase::Win_SetFocus() {
    // platform specific code to Set(force?) the focus to this window
}
void CWpUIBase::Win_SetSize(int iHeight, int iWidth) {
    // platform specific code to Set the window size explicitly
}
sWinSize CWpUIBase::Win_GetSize() {
    // platform specific code to Get the window size. send to console
    return sWinSize();
}

//__________
// MOUSE API
sScreenPos CWpUIBase::Mouse_PosGet() {
    // platform specific code to Get the current mouse position
    return sScreenPos();
}
void CWpUIBase::Mouse_PosSet(sScreenPos NewPos) {
    // platform specific code to Set the current mouse position
}
void CWpUIBase::Mouse_CurserSet(eWpCurserType eWpNewCurser) {
    // platform specific code to set to new curser type based on eWpNewCurser
}
void CWpUIBase::Mouse_CurserShow() {
    // platform specific code to hide the curser
}
void CWpUIBase::Mouse_CurserHide() {
    // platform specific code to show the curser
}

//______________
// CLIPBOARD API
void CWpUIBase::ClipboardSet(std::string strClipText) {
    // platform specific code to save strClipText to the clip board.
}
std::string CWpUIBase::ClipboardGet() {
    // platform specific code to return clipboard's text in strClipText.
    return "";
}