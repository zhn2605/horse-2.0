#include <SDL.h>
#include <iostream>

// Globals
int screenHeight = 480, screenWidth = 640;
SDL_Window* window = nullptr;
SDL_GLContext context = nullptr;

bool quit = false;

//void GetOpenGLVersionInfo() {
//    printf("Vendor: %s\nRenderer: %s\nVersion: %s\nShading Language: %s\n", glGetString)
//}

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
        screenHeight,           // height
        screenWidth,            // width
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

// Open GL
void PreDraw() {

}

void Draw() {

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
    MainLoop();
    CleanUp();
    return 0;
}
