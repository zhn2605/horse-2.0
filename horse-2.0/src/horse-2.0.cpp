// C++ STL
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Third Party Libraries
#include <SDL.h>
#include <glad/glad.h>
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

GLuint graphicsPipelineShaderProgram = 0;

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
}

void GetOpenGLVersionInfo() {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void InitializeProgram() {
    app.Create(640, 480, "test");
    app.Initialize();

    // Enable Resize window
    SDL_SetWindowResizable(app.getWindow(), SDL_TRUE);

    // Initialize the Glad Library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Could not initialize glad.\n" << std::endl;
        exit(1);
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

    // Move Logic
    // Speed
    float multiplier = state[SDL_SCANCODE_LSHIFT] ? 3.0f : 1.0f;
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

void VertexSpecification() {
    const std::vector<GLfloat> vertices{  // Create dynamic array
        // Diamond
        // Front face
        -0.25f, 0.25f, 0.25f,     // top-left
        0.0f, 0.25f, 0.5f,     // color

        0.25f, 0.25f, 0.25f,       // top-right
        0.0f, 0.5f, 0.25f,      // color

        0.25f, -0.25f, 0.25f,      // bot-right
        0.0f, 0.55f, 0.3f,     // color

        -0.25f, -0.25f, 0.25f,     // bot-left
        0.0f, 0.15f, 0.15f,     // color

       // Left face
        -0.25f, 0.25f, -0.25f,     // top-left
        0.0f, 0.25f, 0.5f,     // color

        0.25f, 0.25f, -0.25f,       // top-right
        0.0f, 0.5f, 0.25f,      // color

        0.25f, -0.25f, -0.25f,      // bot-right
        0.0f, 0.55f, 0.3f,     // color

        -0.25f, -0.25f, -0.25f,     // bot-left
        0.0f, 0.15f, 0.15f,     // color
    };


    const std::vector<GLuint> vertexIndicies{
        // Front
        0, 1, 2,                // top triangle
        0, 3, 2,                // bot triangle

        // Back
        4, 5, 6,
        4, 7, 6,

        // Left
        0, 4, 7,
        0, 3, 7,

        // Right
        1, 5, 6,
        1, 2, 6,

        // Top
        0, 4, 5,
        0, 1, 5,

        // Bot
        3, 7, 6,
        3, 2, 6
    };

    object.SpecifyVertices(vertices, vertexIndicies);
    object.Initialize();
}

// Open GL Drawing
void PreDraw() {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, app.getWidth(), app.getHeight());        // creates a viewport starting left corner (0,0) 
    //glClearColor(0.2f, 0.3f, .3f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(graphicsPipelineShaderProgram);        // modifying shaders in program object will not affect curr executables

    // Important: ORDER MATTERS (T, R, S for rotation around object. R, T, S for simulated rotation around us
    // Rotation test
    uRotate -= .025f;
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
    //glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)app.getWidth() / (float)app.getHeight(), 0.1f, 10.0f);
    glm::mat4 perspective = camera.GetProjectionMatrix();
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
    glBindVertexArray(object.getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, object.getVBO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.getIBO());

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void MainLoop() {
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
        camera.SetProjectionMatrix(glm::radians(45.0f), (float)app.getWidth() / (float)app.getHeight(), 0.1f, 10.0f);

        Input();

        PreDraw();

        Draw();

        // Update the screen
        SDL_GL_SwapWindow(app.getWindow());
    }
}

void CleanUp() {
    // Clean up object
    object.CleanUp();

    // Delete pipeline
    glDeleteProgram(graphicsPipelineShaderProgram);

    app.Terminate();
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