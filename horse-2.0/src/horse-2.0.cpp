// C++ STL
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Libraries
#include <SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Header files
#include "Shader.hpp"
#include "Camera.hpp"

// Globals
int screenHeight = 480, screenWidth = 640;
SDL_Window* window = nullptr;
SDL_GLContext context = nullptr;

bool quit = false;

// VAO
GLuint vertexArrayObject = 0;
// VBO
GLuint vertexBufferObject = 0;      // position
// Program Objects (for shaders)
GLuint elementBufferObject = 0;

GLuint graphicsPipelineShaderProgram = 0;

float uOffset = -2.0f;
float uRotate = 0.0f;
float uScale = 1.0f;

Camera camera;

std::string LoadShaderAsString(const std::string& filename) {
    std::string result = "";

    std::string line = "";
    std::ifstream file(filename.c_str());

    if (file.is_open()) {
        while (std::getline(file, line)) {
            result += line + '\n';
        }
        file.close();
    }
    return result;
}

GLuint CompileShader(GLuint type, const std::string& source) {
    GLuint shaderObject;

    if (type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();                           // gets string source code
    glShaderSource(shaderObject, 1, &src, nullptr);             // copies shader source code string
    glCompileShader(shaderObject);                              // compiles

    // Error checking
    int success;
    char infoLog[512];
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);
        printf("Shader could not be compiled: %s\n", infoLog);
    }

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vs, const std::string& fs) {
    GLuint programObject = glCreateProgram();                   // shader-attachable program object, used to link shaders  
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vs);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fs);

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);                               // link shader objects

    // Validate program
    glValidateProgram(programObject);                           // checks whether the executable in proramObject can execute currently
    // glDetachShader / delte shader
    glDetachShader(programObject, vertexShader | fragmentShader);
    glDeleteShader(vertexShader | fragmentShader);

    return programObject;
}

void CreateGraphicsPipeline() {
    std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");

    graphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void GetOpenGLVersionInfo() {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void InitializeProgram() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL2 could not initialize video subsystem.");
        exit(1);
    }

    // Set GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    window = SDL_CreateWindow(
        "horse-2.0",            // title name
        SDL_WINDOWPOS_CENTERED, // x pos
        SDL_WINDOWPOS_CENTERED, // y pos
        screenWidth,            // width
        screenHeight,           // height
        SDL_WINDOW_OPENGL       // Unit32 flags
    );

    if (window == nullptr) {
        printf("Could not create window: %s\n", SDL_GetError());
        exit(1);
    }

    // Create context
    context = SDL_GL_CreateContext(window);

    if (context == nullptr) {
        printf("Could not create context: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialize the Glad Library

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Could not initialize glad.\n" << std::endl;
        exit(1);
    }

    GetOpenGLVersionInfo();
}

// SDL Input Handling
void Input() {
    static int mouseX = screenWidth / 2;
    static int mouseY = screenHeight / 2;

    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            std::cout << "goodbye!!" << std::endl;
            quit = true;
        }
        else if (e.type == SDL_MOUSEMOTION) {
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            camera.MouseLook(mouseX, mouseY);
        }
    }

   /* uOffset += .001f;
    uOffset -= .001f;
    uRotate += .01f;
    uRotate -= .01f;*/

    // Retrieve keyboard state
    const Uint8* state = SDL_GetKeyboardState(NULL);

    float speed = 0.001f;
    if (state[SDL_SCANCODE_W]) {
        camera.MoveForward(speed);
    }
    if (state[SDL_SCANCODE_S]) {
        camera.MoveBackward(speed);
    }
    if (state[SDL_SCANCODE_D]) {
        camera.MoveRight(speed);
    }
    if (state[SDL_SCANCODE_A]) {
        camera.MoveLeft(speed);
    }
    if (state[SDL_SCANCODE_C]) {
        camera.MoveDown(speed);
    }
    if (state[SDL_SCANCODE_SPACE]) {
        camera.MoveUp(speed);
    }
}

void VertexSpecification() {
    const std::vector<GLfloat> vertices{  // Create dynamic array
        // Diamond
        -0.25f, 0.0f, 0.0f,     // left
        139.0f, 0.25f, 0.5f,     // color

        0.0f, 0.25f, 0.0f,       // top
        139.0f, 0.5f, 0.25f,      // color

        0.25f, 0.0f, 0.0f,      // right
        139.0f, 0.55f, 0.3f,     // color

        0.0f, -0.25f, 0.0f,     // bottom
        139.0f, 0.15f, 0.15f,     // color
    };


    const std::vector<GLint> vertexIndicies{
        0, 1, 2,                // top triangle
        0, 3, 2,                // bot triangle
    };

    // VAO Specification
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Create VBO
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3, // xyz
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 6,
        (void*)0
    );

    // Enable color 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3, // rgb
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 6,
        (void*)(sizeof(GL_FLOAT) * 3)
    );

    // Create EBO
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndicies.size() * sizeof(GLint), vertexIndicies.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

// Open GL Drawing
void PreDraw() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, screenWidth, screenHeight);        // creates a viewport starting left corner (0,0) 
    glClearColor(0.2f, 0.3f, .3f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(graphicsPipelineShaderProgram);        // modifying shaders in program object will not affect curr executables

    // Important: ORDER MATTERS (T, R, S for rotation around object. R, T, S for simulated rotation around us
    // Rotation test
    uRotate -= .1f;
    //std::cout << "uRotate: " << uRotate << std::endl;

    // Translate
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, uOffset));
    // Rotate
    model = glm::rotate(model, glm::radians(uRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    // Scale
    model = glm::scale(model, glm::vec3(uScale, uScale, uScale));
    
    GLint u_ModelMatrixLocation = glGetUniformLocation(graphicsPipelineShaderProgram, "u_ModelMatrix");
    if (u_ModelMatrixLocation >= 0) {
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    } 
    else {
        std::cout << "could not find location of u_ModelMatrix. Mispelling?" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // View Matrix
    glm::mat4 view = camera.GetViewMatrix();
    GLint u_viewLocataion = glGetUniformLocation(graphicsPipelineShaderProgram, "u_ViewMatrix");
    if (u_viewLocataion >= 0) {
        glUniformMatrix4fv(u_viewLocataion, 1, GL_FALSE, &view[0][0]);
    }
    else {
        std::cout << "could not find location of u_View. Mispelling?" << std::endl;
        exit(EXIT_FAILURE);
    }

    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)screenWidth/(float)screenHeight, 0.1f, 10.0f);
    GLint u_ProjectionLocation = glGetUniformLocation(graphicsPipelineShaderProgram, "u_Projection");
    if (u_ProjectionLocation >= 0) {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
    }
    else {
        std::cout << "could not find location of u_Projection. Mispelling?" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Draw() {
    // Sin diamond test
    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void MainLoop() {

    // Set mouse in middle of window
    SDL_WarpMouseInWindow(window, screenWidth/2, screenHeight/2);

    // Set mouse to move relatively within application
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!quit) {
        Input();

        PreDraw();

        Draw();

        // Update the screen
        SDL_GL_SwapWindow(window);
    }
}

void CleanUp() {

    // Delete OpenGL objects
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteVertexArrays(1, &vertexArrayObject);

    // Delete pipeline
    glDeleteProgram(graphicsPipelineShaderProgram);

    SDL_DestroyWindow(window);
    SDL_Quit();
}

#undef main // bug fix: potential overlap of main declaration in SDL??
int main()
{
    InitializeProgram();

    VertexSpecification();

    CreateGraphicsPipeline();

    MainLoop();

    CleanUp();

    return 0;
}