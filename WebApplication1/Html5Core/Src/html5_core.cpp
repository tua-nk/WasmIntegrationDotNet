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
bool color = false;

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
        //        glUniform4f(fragColorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void timer_callback(void* userData) {
    color = !color;
    render();
}

// Global UI Class
CWpUIAppRotTri gRotTriApp;

int main() {
    gRotTriApp.Init();

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

    emscripten_set_main_loop(render, 0, 1);
    emscripten_set_interval(timer_callback, 100, NULL);

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