#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

// Globals
int screenHeight = 480, screenWidth = 640;
SDL_Window* window = nullptr;
SDL_GLContext context = nullptr;

//float* verticies;

bool quit = false;

// VAO
GLuint vertexArrayObject = 0;
// VBO
GLuint vertexBufferObject = 0;
// Program Objects(for our shaders)
GLuint graphicsPipelineShaderProgram = 0;

// Vertex shader executes once per vertex
// in charge of final position of vertex
const std::string vertexShaderSource =
    "#version 410 core\n"
    "in vec4 position;\n"
    "void main() {\n"
    "   gl_Position = vec4(position.x, position.y, position.z);\n"
    "}\n";

// Fragment shader executes once per fragment (pixel)
// in part determines final color sent to screen
const std::string fragmentShaderSource =
    "#version 410 core\n"
    "out vec4 color;\n"
    "void main() {\n"
    "   color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
    "}\n";

GLuint CompileShader(GLuint type, const std::string& source) {
    GLuint shaderObject;

    if (type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vs, const std::string& fs) {
    GLuint programObject = glCreateProgram();
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vs);
    GLuint fragmentShader = CompileShader(GL_VERTEX_SHADER, fs);

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    // Validate program
    glValidateProgram(programObject);
    // glDetachShader / deelte shader
    return programObject;
}

void CreateGraphicsPipeline() {
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
}

// Open GL Drawing
void PreDraw() {
    
}

void Draw() {

}

void VertexSpecification() {
    const std::vector<GLfloat> vertexPosition{  // Create dynamic array
        // Diamond
        -0.25f, 0.0f, 0.0f,
        0.0f, 0.25f, 0.0f,
        0.25f, 0.0f, 0.0f,
        0.0f, -0.25f, 0.0f
    };

    //verticies = vertexPosition;

    // VAO Specification
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Create VBO
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);

    const char* vertexShaderSource =
        "#version";
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
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#undef main
int main()
{
    InitializeProgram();
    VertexSpecification();
    CreateGraphicsPipeline();
    MainLoop();
    CleanUp();
    return 0;
}
