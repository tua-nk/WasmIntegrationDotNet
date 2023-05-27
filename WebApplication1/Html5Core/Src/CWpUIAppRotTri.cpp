#include "CWpUIAppRotTri.h"

const char* vertexShaderSource = "#version 300 es\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 uViewMatrix;\n"
"void main()\n"
"{\n"
"   gl_Position = uViewMatrix * vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 300 es\n"
"precision mediump float;\n"
"uniform vec4 uFragColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = uFragColor;\n"
"}\n\0";

bool bRotateOn = true; // GND: added for user toggle on/off
float angle = 0.5f;
bool color;

GLuint vertexShader, fragmentShader, shaderProgram, VBO, VAO;

void compileShaders() {
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glm::mat4 view = glm::mat4(1.0f);
    angle += 0.1f;
    view = glm::rotate(view, angle, glm::vec3(0.0f, 0.0f, 1.0f));

    GLint viewLoc = glGetUniformLocation(shaderProgram, "uViewMatrix");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    GLint fragColorLoc = glGetUniformLocation(shaderProgram, "uFragColor");
    if (color) {
        glUniform4f(fragColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
    }
    else {
        glUniform4f(fragColorLoc, 1.0f, 0.0f, 1.0f, 1.0f);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void timer_callback(void* userData) {
    color = !color;
    render();
}

CWpUIAppRotTri::CWpUIAppRotTri() {
}

//____________________________________
// Init Plateform Specfic UI base code (emscripton in this case, but could also be GLFW or SDL etc)
void CWpUIAppRotTri::Init()
{
    CWpUIBaseEm::Init();

    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attrs);
    emscripten_webgl_make_context_current(ctx);

    GLfloat vertices[] = {
      0.0f,  0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    compileShaders();

    emscripten_set_interval(timer_callback, 1000, NULL);
    emscripten_set_main_loop(render, 0, 1);
}

//__________________________
// Render Call for animation
void CWpUIAppRotTri::OnRenderAnimate()
{
    // Override to call render code
}

//========================================================================================================
// WARP APPLICATION UI EVENTS CALLBACK API

//_________________________
// KEYBOARD EVENT CALLBACKS
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
        case 'y':
        {
            Win_GetTitle();
            break;
        }
        case 'u':
        {
            Win_SetSize(800, 600);
            break;
        }
        case 'i':
        {
            Win_GetSize();
            break;
        }

        case 'q':
        {
            Mouse_PosGet();
            break;
        }
        case 'w':
        {
            Mouse_PosSet(sScreenPos(400, 300));
            break;
        }
        case 'e':
        {
            Mouse_CurserShow();
            break;
        }
        case 'r':
        {
            Mouse_CurserHide();
            break;
        }
        case 'o':
        {
            Mouse_CurserSet(eWpCurserType::eWpCurser_Arrow);
            break;
        }
        //...ETC.

        default:
            break;
        }
    }
}

//______________________
// MOUSE EVENT CALLBACKS

void CWpUIAppRotTri::OnMouse_Move(sScreenPos sNewPos) {
    // Override
    //ToConsole("CWpUIAppRotTri::OnMouse_Move", sNewPos);
}
void CWpUIAppRotTri::OnMouse_Click(eWpMouseButton eButton, bool bDown, sScreenPos sMousePos) {
    // Override
    // if bDown = false then button is going UP.
    ToConsole("CWpUIAppRotTri::OnMouse_Click", sMousePos);
}

void CWpUIAppRotTri::OnMouse_DoubleClick(eWpMouseButton, sScreenPos sPos) {
    // Override
    ToConsole("CWpUIAppRotTri::OnMouse_DoubleClick", sPos);
}
void CWpUIAppRotTri::OnMouse_Wheel(float fDx, float fDy) {
    // Override
    ToConsole("CWpUIAppRotTri::OnMouse_Wheel");
}
void CWpUIAppRotTri::OnMouse_EnterWin(sScreenPos sPos) {
    // Override
    ToConsole("CWpUIAppRotTri::OnMouse_EnterWin", sPos);
}
void CWpUIAppRotTri::OnMouse_LeaveWin(sScreenPos sPos) {
    // Override
    ToConsole("CWpUIAppRotTri::OnMouse_LeaveWin", sPos);
}
void CWpUIAppRotTri::OnMouse_Over(sScreenPos sPos)
{
    // Override
    ToConsole("CWpUIAppRotTri::OnMouse_Over", sPos);
}
void CWpUIAppRotTri::OnMouse_Out(sScreenPos sPos)
{
    // Override
    ToConsole("CWpUIAppRotTri::OnMouse_Out", sPos);
}

//_______________________
// WINDOW EVENT CALLBACKS

void CWpUIAppRotTri::OnWin_Resize(int iNewH, int iNewW) {
    // Send new size to console, use WpWinGetsize()
    ToConsole("CWpUIAppRotTri::OnWin_Resize", iNewW, iNewH);
}
void CWpUIAppRotTri::OnWin_Minimized() {
    // Override
    ToConsole("CWpUIAppRotTri::OnWin_Minimized");
}
void CWpUIAppRotTri::OnWin_Maximized() {
    // Override
    ToConsole("CWpUIAppRotTri::OnWin_Maximized");
}
void CWpUIAppRotTri::OnWin_Regular() { // size back to regular from min or max
    // Override
    ToConsole("CWpUIAppRotTri::OnWin_Regular");
}
void CWpUIAppRotTri::OnWin_FocusChange(bool bFocusGained) {
    // if false then focus was lost from this window.
    // Override
    ToConsole("CWpUIAppRotTri::OnWin_FocusChange");
}

//________________________________
// "DRAG AND DROP" EVENT CALLBACKS
void CWpUIAppRotTri::OnDrag(bool bStart) {
    // if bStart is true then draging has started, if false then dragging has stopped
    // Override
}
void CWpUIAppRotTri::OnDropFiles(std::vector<std::string> FileNameList, sScreenPos MousePos) {
    // Override
}
void CWpUIAppRotTri::OnDropText(std::string strText, sScreenPos MousePos) {
    // Override
}

//========================================================================================================
// WARP APPLICATION UI SET/GET FUNCTION API
//   - Platform independant code - calls Plateform specific event functions (Emscription HTML5 / GLFW / SDL)

//___________
// WINDOW API
void CWpUIAppRotTri::Win_SetTitle(std::string strTitle) {
    ToConsole("CWpUIAppRotTri::Win_SetTitle");
    // Emscripten specific code to Set the window tittle
    CWpUIBaseEm::Win_SetTitle(strTitle);
}
std::string CWpUIAppRotTri::Win_GetTitle() {
    ToConsole("CWpUIAppRotTri::Win_GetTitle");
    // Emscripten specific code to Set the window tittle
    std::string s = CWpUIBaseEm::Win_GetTitle();
    ToConsole(s);
    return s;
}

void CWpUIAppRotTri::Win_SetFocus() {
    ToConsole("CWpUIAppRotTri::Win_SetFocus");
    // platform specific code to Set(force?) the focus to this window
}
void CWpUIAppRotTri::Win_SetSize(int iWidth, int iHeight) {
    ToConsole("CWpUIAppRotTri::Win_SetSize", iWidth, iHeight);
    // platform specific code to Set the window size explicitly
    CWpUIBaseEm::Win_SetSize(iWidth, iHeight);
}
sWinSize CWpUIAppRotTri::Win_GetSize() {
    ToConsole("CWpUIAppRotTri::Win_GetSize");
    sWinSize s = CWpUIBaseEm::Win_GetSize();
    ToConsole("CWpUIAppRotTri::Win_GetSize", s.iW, s.iH);
    // platform specific code to Get the window size. send to console
    return s;
}

//__________
// MOUSE API
sScreenPos CWpUIAppRotTri::Mouse_PosGet() {
    // platform specific code to Get the current mouse position
    return sScreenPos();
}
void CWpUIAppRotTri::Mouse_PosSet(sScreenPos NewPos) {
    // platform specific code to Set the current mouse position
}
void CWpUIAppRotTri::Mouse_CurserSet(eWpCurserType eWpNewCurser) {
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
void CWpUIAppRotTri::Mouse_CurserShow() {
    // platform specific code to hide the curser
}
void CWpUIAppRotTri::Mouse_CurserHide() {
    // platform specific code to show the curser
}

//______________
// CLIPBOARD API
void CWpUIAppRotTri::ClipboardSet(std::string strClipText) {
    // platform specific code to save strClipText to the clip board.
}
std::string CWpUIAppRotTri::ClipboardGet() {
    // platform specific code to return clipboard's text in strClipText.
    return "";
}
