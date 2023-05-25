#include "CWpUIAppRotTri.h"

// Global UI Class
CWpUIAppRotTri* gRotTriApp = NULL;

int main() {
    gRotTriApp = new CWpUIAppRotTri();
    gRotTriApp->Init();
    return 0;
}

//======================================================================================================
// OPEN FILE DIALOG (on local Machine)

//======================================================================================================
// FILE READ (local)

//======================================================================================================
// ZOOM TO FILL VIEW & PERSPECTIVE/ORTHOGRAPHIC PROJECTION 

//======================================================================================================
// MULTISAMPLING IN WASM

//======================================================================================================
// NATIVE CONTEXT MENU SUPPRESSION (recieve right click but suppress native menu so we show our own, in WASM)