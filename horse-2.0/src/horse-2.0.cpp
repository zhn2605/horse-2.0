#include <SDL.h>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Shader.hpp"

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

float y_uOffset = 0.0f, x_uOffset = 0.0f, z_uOffset = 0.0f;

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
        printf("Could not initialize glad.\n");
        exit(1);
    }

    GetOpenGLVersionInfo();
}

// SDL Input Handling
void Input() {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            printf("goodbye!!");
            quit = true;
        }
    }

    // Retrieve keyboard state
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) {
        y_uOffset += .001f;

    }
    if (state[SDL_SCANCODE_DOWN]) {
        y_uOffset -= .001f;
    }
}

void VertexSpecification() {
    const std::vector<GLfloat> vertices{  // Create dynamic array
        // Diamond
        -0.25f, 0.0f, 0.0f,     // left
        0.25f, 0.5f, 0.25f,     // color

        0.0f, 0.25f, 0.0f,       // top
        0.25f, .25f, 0.5f,      // color

        0.25f, 0.0f, 0.0f,      // right
        0.5f, 0.25f, 0.25f,     // color

        0.0f, -0.25f, 0.0f,     // bottom
        0.25f, 0.5f, 0.25f,     // color
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
    GLint location = glGetUniformLocation(graphicsPipelineShaderProgram, "u_Offset");
    if (location >= 0) {
        glUniform1f(location, y_uOffset);
    } 
    else {
        std::cout << "could not find location of u_Offset. Mispelling?" << std::endl;
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