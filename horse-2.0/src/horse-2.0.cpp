// C++ STL
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

// Third Party Libraries
#include <SDL.h>
#include <glad/glad.h>

// Audio
//#include <SFML/Audio.hpp>
#include <irrKlang.h>
using namespace irrklang;

// Assets


// GLM
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Libraries
#include "Shader.hpp"
#include "Mesh3D.hpp"
#include "Camera.hpp"
#include "App.hpp"
#include "MeshData.hpp"
#include "Scene.hpp"

// Application Instance
App app;

// Meshes
Mesh3D object;

// VAO
//GLuint vertexArrayObject = 0;
//// VBO
//GLuint vertexBufferObject = 0;      // position
//// Program Objects (for shaders)
//GLuint elementBufferObject = 0;

// Audio
ISoundEngine* SoundEngine = createIrrKlangDevice();
//sf::Music rainBG;

GLuint graphicsPipelineShaderProgram = 0;
Scene scene(graphicsPipelineShaderProgram);

// Uniform values
float uOffset = -2.0f;
float uRotate = 0.0f;
float uScale = 1.0f;

// Delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
    // glDetachShader / delete shader
    glDetachShader(programObject, vertexShader | fragmentShader);
    glDeleteShader(vertexShader | fragmentShader);

    return programObject;
}

void CreateGraphicsPipeline() {
    std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");

    graphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
    scene.SetShaderProgram(graphicsPipelineShaderProgram);
}

void GetOpenGLVersionInfo() {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void InitializeProgram() {
    app.Create(640, 480, "myTamagotchi.exe");
    app.Initialize();

    // Enable Resize window
    SDL_SetWindowResizable(app.getWindow(), SDL_TRUE);

    // Initialize the Glad Library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Could not initialize glad.\n" << std::endl;
        exit(1);
    }

    /*
    if (!rainBG.openFromFile("./assets/audio/music/light-rain.wav")) {
        std::cout << "Could not load music!" << std::endl;
        exit(1);
    }
    */

    if (!SoundEngine) {
        std::cout << "Failed to create sound engine from irrKlang.\n" << std::endl;
        exit(1);
    } else {
        ISound* rain = SoundEngine->play3D("./assets/audio/music/light-rain.wav", vec3df(0.0f, 0.0f, 0.0f), true, false, true);
        rain->setMinDistance(1.0f);
        rain->setMaxDistance(3.0f);

        SoundEngine->setListenerPosition(vec3df(0, 0, 0), vec3df(0, 0, 0));
    }

    GetOpenGLVersionInfo();
}

// SDL Input Handling
void Input() {
    static int mouseX = app.getWidth() / 2;
    static int mouseY = app.getHeight() / 2;
    
    SDL_Event e;
    // Retrieve keyboard state
    const Uint8* state = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            std::cout << "goodbye!!" << std::endl;
            app.setActive(false);
        }
        else if (e.type == SDL_WINDOWEVENT) {
            if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            
                int newWidth = e.window.data1;
                int newHeight = e.window.data2;

                // Update OpenGL viewport
                glViewport(0, 0, newWidth, newHeight);

                // Update camera's aspect ratio
                float aspect = static_cast<float>(newWidth) / static_cast<float>(newHeight);
                camera.UpdateAspectRatio(aspect);

                app.setWidth(newWidth);
                app.setHeight(newHeight);
            
            }
        }
        else if (e.type == SDL_MOUSEMOTION) {
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            camera.MouseLook(mouseX, mouseY);
        } 
    }
     
    // Screen Logic
    Uint32 windowFlags = SDL_GetWindowFlags(app.getWindow());
    if (state[SDL_SCANCODE_F]) {
       SDL_SetWindowFullscreen(app.getWindow(), SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    // Audio Test
    if (state[SDL_SCANCODE_0]) {
    }

    // Move Logic
    // Speed
    float multiplier = 1.0; 
    const float base_fov = 60.0f;
    const float max_fov = 100.0f;
    const float fov_tween = 2.0f;
    
    if (state[SDL_SCANCODE_LSHIFT]) {
        multiplier = 3.0;

        float fov_delta = (max_fov - base_fov) * (log(multiplier) / fov_tween);

        camera.SetFovy(base_fov + fov_delta);
    }
    else {
        camera.SetFovy(base_fov);
    }

    float speed = 0.001f * deltaTime * multiplier;
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

    if (state[SDL_SCANCODE_ESCAPE]) {
        if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
        else {
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
    }
}

void InitializeObjects() {
    // Construct house
    Mesh3D* frontWall = scene.CreateObject(MeshData::CreateWall(1.0f, 0.05f, 2.0f));
    frontWall->SetPosition(glm::vec3(0.0f, 0.0f, -2.0f));

    /*for (float i = -20.0f; i < 20.0f; i += 1.0f) {
        for (float j = -20.0f; j < 20.0f; j += 1.0f) {
            Mesh3D* cube = scene.CreateObject(MeshData::CreateCube(0.1f));
            cube->SetPosition(glm::vec3(i, 0.0f, j));
        }
    }*/
}


void Draw() {
    scene.PrepareDraw(app.getWidth(), app.getHeight());
    scene.DrawAll(camera.GetViewMatrix(), camera.GetProjectionMatrix());
}

void MainLoop() {
    vec3df cameraPos = vec3df(camera.GetEye().x, camera.GetEye().y, camera.GetEye().z);
    vec3df cameraLook = vec3df(camera.GetLookDir().x, camera.GetEye().y, camera.GetEye().z);

    SoundEngine->setListenerPosition(cameraPos, cameraLook);

    // Set mouse in middle of window
    SDL_WarpMouseInWindow(app.getWindow(), app.getWidth() / 2, app.getHeight() / 2);

    // Set mouse to move relatively within application
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (app.isActive()) {
        // Create delta time
        float currentFrame = 1.0 * SDL_GetTicks();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Projection Matrix
        camera.SetProjectionMatrix(glm::radians(60.0f), (float)app.getWidth() / (float)app.getHeight(), 0.1f, 50.0f);

        Input();

        Draw();

        // Update the screen
        SDL_GL_SwapWindow(app.getWindow());
    }
}

void CleanUp() {
    // Clean up objects
    scene.CleanUpAll();

    // Delete pipeline
    glDeleteProgram(graphicsPipelineShaderProgram);

    app.Terminate();

    SoundEngine->drop();
}

#undef main // bug fix: potential overlap of main declaration in SDL??
int main()
{
    InitializeProgram();

    CreateGraphicsPipeline();

    InitializeObjects();

    MainLoop();

    CleanUp();

    return 0;
}