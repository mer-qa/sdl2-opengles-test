#ifndef SAILFISH_SDL_WAYLAND_OPENGL_TEST_H
#define SAILFISH_SDL_WAYLAND_OPENGL_TEST_H

#include <SDL.h>
#include <stdio.h>

class SDL2TestApplication {
    public:
        SDL2TestApplication(int major, int minor);
        ~SDL2TestApplication();

        int run();

        virtual void initGL() = 0;
        virtual void resizeGL(int width, int height) = 0;
        virtual void renderGL() = 0;

    private:
        int m_major;
        int m_minor;
        SDL_Window *m_window;
        SDL_GLContext m_gl_context;
};

#endif /* SAILFISH_SDL_WAYLAND_OPENGL_TEST_H */
