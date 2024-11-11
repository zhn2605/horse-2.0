// horse-2.0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SDL.h>
#include <iostream>

#undef main
int main()
{
    SDL_Window* window;
    SDL_Init(SDL_INIT_VIDEO); // Initialize subsystems

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Creating window
    window = SDL_CreateWindow(
        "horse-2.0",            // title name
        SDL_WINDOWPOS_CENTERED, // x pos
        SDL_WINDOWPOS_CENTERED, // y pos
        640,                    // height
        480,                    // width
        SDL_WINDOW_OPENGL       // Unit32 flags
    );

    // Window error check
    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        printf("Could not create context: %s\n", SDL_GetError());
        return 1;
    }


    SDL_Delay(3000);
    SDL_FlashOperation(SDL_FLASH_UNTIL_FOCUSED);
    SDL_Delay(2000);

    SDL_DestroyWindow(window);
    SDL_Quit;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
