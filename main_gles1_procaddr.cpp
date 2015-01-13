/**
 *
 * SDL 2.0 OpenGL ES Test Application
 *
 * Copyright (C) 2013, 2015 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jolla.com>
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
#include "procaddr_helper.h"

#include "common.h"

class SDL2TestApplicationGLESv1ProcAddr : public SDL2TestApplication {
    public:
        SDL2TestApplicationGLESv1ProcAddr();

        virtual void initGL();
        virtual void resizeGL(int width, int height);
        virtual void renderGL();
};

SDL2TestApplicationGLESv1ProcAddr::SDL2TestApplicationGLESv1ProcAddr()
    : SDL2TestApplication(1, 1)
{
}

void
SDL2TestApplicationGLESv1ProcAddr::initGL()
{
    testProcAddr("libGLESv1.so");
}

void
SDL2TestApplicationGLESv1ProcAddr::resizeGL(int width, int height)
{
}

void
SDL2TestApplicationGLESv1ProcAddr::renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

int
main(int argc, char *argv[])
{
    SDL2TestApplicationGLESv1ProcAddr testapp_glesv1_procaddr;
    return testapp_glesv1_procaddr.run();
}
