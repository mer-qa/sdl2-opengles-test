
#include "common.h"

#include <stdlib.h>

SDL2TestApplication::SDL2TestApplication(int major, int minor)
    : m_major(major)
    , m_minor(minor)
    , m_window(NULL)
{
    setenv("SDL_VIDEODRIVER", "wayland", 1);
    setenv("WAYLAND_DISPLAY", "wayland-0", 1);
    SDL_Init(SDL_INIT_VIDEO);
}

SDL2TestApplication::~SDL2TestApplication()
{
}

int
SDL2TestApplication::run()
{
    m_window = SDL_CreateWindow("SDL2TestApplication",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, 
            800, 480,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);

    if (m_window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);
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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_minor);

    m_gl_context = SDL_GL_CreateContext(m_window);

    initGL();
    resizeGL(w, h);

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
                case SDL_FINGERDOWN:
                case SDL_FINGERUP:
                case SDL_FINGERMOTION:
                    printf("Finger %s: %d (%.2f / %.2f)\n",
                            (event.type == SDL_FINGERDOWN) ? "down" :
                             ((event.type == SDL_FINGERUP) ? "up" : "moved"),
                            event.tfinger.fingerId,
                            event.tfinger.x, event.tfinger.y);
                    break;
                default:
                    printf("SDL Event: %d\n", event.type);
                    break;
            }
        }

        renderGL();

        SDL_GL_SwapWindow(m_window);
        SDL_Delay(10);
    }

    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    return 0;
}

