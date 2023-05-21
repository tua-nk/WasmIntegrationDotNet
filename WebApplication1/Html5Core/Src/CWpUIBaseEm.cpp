#include "CWpUIBaseEm.h"

//CWpUIBaseEm::CWpUIBaseEm()
//{
//    if (pWpUIEm == NULL) pWpUIEm = this; // setup ref to this object for callbacks
//}
//
//void CWpUIBaseEm::Init()
//{
//    // Setup Base class UI Event call back functions
//    em_init(); // setup callbacks etc.
//}

// ____________________________________
// Impliment Base class Setters/Getters

void CWpUIBaseEm::Win_SetTitle(std::string strTitle) {
    // Emscripten specific code to Set the window tittle
    emscripten_set_window_title(source_window_title);
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