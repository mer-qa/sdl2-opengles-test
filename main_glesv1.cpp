
#include <GLES/gl.h>

#include "common.h"

class SDL2TestApplicationGLESv1 : public SDL2TestApplication {
    public:
        SDL2TestApplicationGLESv1();

        virtual void initGL();
        virtual void resizeGL(int width, int height);
        virtual void renderGL();
};

SDL2TestApplicationGLESv1::SDL2TestApplicationGLESv1()
    : SDL2TestApplication(1, 1)
{
}

void
SDL2TestApplicationGLESv1::initGL()
{
    glClearColor(1.0, 0.0, 0.0, 1.0);
}

void
SDL2TestApplicationGLESv1::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, width, height, 0, -1, +1);
}

void
SDL2TestApplicationGLESv1::renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor4f(0.0, 1.0, 1.0, 1.0);

    float vertices[] = {
        100, 100,
        200, 100,
        100, 200,
    };
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
}

int
main(int argc, char *argv[])
{
    SDL2TestApplicationGLESv1 testapp_glesv1;
    return testapp_glesv1.run();
}

