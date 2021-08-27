/**
 *
 * SDL 2.0 OpenGL ES Test Application
 *
 * Copyright (C) 2013 Jolla Ltd.
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

