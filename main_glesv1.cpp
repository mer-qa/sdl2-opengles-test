
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
    glClearColor(0.3, 0.3, 0.6, 1.0);
}

void
SDL2TestApplicationGLESv1::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, width, height, 0, -1, +1);
}

static void
draw_touch_point(TouchPoint *touch, void *user_data)
{
    float d = 30;

    float vertices[] = {
        touch->x - d, touch->y - d,
        touch->x + d, touch->y - d,
        touch->x - d, touch->y + d,
        touch->x + d, touch->y + d,
    };

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void
SDL2TestApplicationGLESv1::renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor4f(0.9, 0.4, 0.3, 1.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    for_each_touch(draw_touch_point, NULL);
    glDisableClientState(GL_VERTEX_ARRAY);

}

int
main(int argc, char *argv[])
{
    SDL2TestApplicationGLESv1 testapp_glesv1;
    return testapp_glesv1.run();
}

