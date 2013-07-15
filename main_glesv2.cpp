
#include <GLES2/gl2.h>

#include "common.h"

class SDL2TestApplicationGLESv2 : public SDL2TestApplication {
    public:
        SDL2TestApplicationGLESv2();

        virtual void initGL();
        virtual void resizeGL(int width, int height);
        virtual void renderGL();
};

SDL2TestApplicationGLESv2::SDL2TestApplicationGLESv2()
    : SDL2TestApplication(2, 0)
{
}

void
SDL2TestApplicationGLESv2::initGL()
{
    glClearColor(0.0, 1.0, 0.0, 1.0);
}

void
SDL2TestApplicationGLESv2::resizeGL(int width, int height)
{
}

void
SDL2TestApplicationGLESv2::renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

int
main(int argc, char *argv[])
{
    SDL2TestApplicationGLESv2 testapp_glesv2;
    return testapp_glesv2.run();
}

