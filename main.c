
#include <stdio.h>

#include <GLES2/gl2.h>

#include <SDL.h>

int
main(int argc, char *argv[])
{
    SDL_Window *window;

    setenv("SDL_VIDEODRIVER", "wayland", 1);
    setenv("WAYLAND_DISPLAY", "wayland-0", 1);

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Test",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, 
            800, 480,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    printf("Window size after create: (%d, %d)\n", w, h);

#if 0
    SDL_SetWindowFullscreen(window, SDL_FALSE);
    SDL_GetWindowSize(window, &w, &h);
    printf("Window size after fullscreen off: (%d, %d)\n", w, h);

    SDL_SetWindowSize(window, 200, 200);
    SDL_GetWindowSize(window, &w, &h);
    printf("Window size after resize: (%d, %d)\n", w, h);

    SDL_SetWindowFullscreen(window, SDL_TRUE);
    SDL_GetWindowSize(window, &w, &h);
    printf("Window size after fullscreen on: (%d, %d)\n", w, h);
#endif

    // Request an OpenGL ES 2.0 context for rendering
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    SDL_Event event;

    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    quit = 1;
                    break;
                case SDL_WINDOWEVENT:
                    printf("Window event: %d (%d, %d)\n", event.window.event,
                            event.window.data1, event.window.data2);
                    break;
                default:
                    printf("SDL Event: %d\n", event.type);
                    break;
            }
        }

        glClearColor(1.0, 0.0, 0.0, 1.0);

        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    SDL_GL_DeleteContext(glcontext);

    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

