/**
 *
 * SDL 2.0 OpenGL ES Test Application
 *
 * Copyright (C) 2013-2021 Jolla Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **/


#include "common.h"

#include <stdlib.h>

SDL2TestApplication::SDL2TestApplication(int major, int minor)
    : m_major(major)
    , m_minor(minor)
    , m_window(NULL)
    , m_touches()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}

SDL2TestApplication::~SDL2TestApplication()
{
    std::list<TouchPoint*>::iterator it;
    for (it=m_touches.begin(); it != m_touches.end(); ++it) {
        delete *it;
    }
}

void
SDL2TestApplication::for_each_touch(touch_point_func f, void *user_data)
{
    std::list<TouchPoint*>::iterator it;
    for (it=m_touches.begin(); it != m_touches.end(); ++it) {
        f(*it, user_data);
    }
}

int
SDL2TestApplication::run()
{
    m_window = SDL_CreateWindow("SDL2TestApplication",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, 
            0, 0,
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

    if (m_major != 0 && m_minor != 0) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, m_major);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, m_minor);
    }

    m_gl_context = SDL_GL_CreateContext(m_window);

    initGL();
    resizeGL(w, h);

    std::list<TouchPoint*>::iterator it;
    TouchPoint *touch;

    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_DISPLAYEVENT:
                    printf("Display: %u event: %d (%d)\n", event.display.display,
                            event.window.event, event.display.data1);
                    break;
                case SDL_WINDOWEVENT:
                    printf("Window event: %d (%d, %d)\n", event.window.event,
                            event.window.data1, event.window.data2);
                    break;
                case SDL_FINGERDOWN:
                    touch = new TouchPoint(event.tfinger.fingerId,
#if SDL_VERSION_ATLEAST(2, 0, 16)
                            event.tfinger.x * w, event.tfinger.y * h);
#else
                            event.tfinger.x, event.tfinger.y);
#endif
                    m_touches.push_back(touch);
                    onPressed(touch);
                    printf("Finger down: (%.2f, %.2f)\n", touch->x, touch->y);
                    break;
                case SDL_FINGERUP:
                case SDL_FINGERMOTION:
                    for (it=m_touches.begin(); it != m_touches.end(); ++it) {
                        touch = *it;
                        if (touch->id == event.tfinger.fingerId) {
                            if (event.type == SDL_FINGERMOTION) {
#if SDL_VERSION_ATLEAST(2, 0, 16)
                                touch->x = event.tfinger.x * w;
                                touch->y = event.tfinger.y * h;
#else
                                touch->x = event.tfinger.x;
                                touch->y = event.tfinger.y;
#endif
                                printf("finger move: (%.2f, %.2f)\n", touch->x, touch->y);
                            } else {
                                printf("Finger up: (%.2f, %.2f)\n", touch->x, touch->y);
                                m_touches.erase(it);
                            }
                            break;
                        }
                    }
                    break;
                default:
                    printf("SDL Event: %d\n", event.type);
                    break;
            }
        }

        if (quit) {
            /* Application has to quit - don't render anymore */
            break;
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

