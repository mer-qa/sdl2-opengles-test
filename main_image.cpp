/**
 *
 * SDL 2.0 Image Test Application
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

#include <SDL_image.h>

class SDL2TestApplicationImage : public SDL2TestApplication {
    public:
        SDL2TestApplicationImage();

        virtual void initGL();
        virtual void resizeGL(int width, int height);
        virtual void renderGL();

    private:
        GLuint texture_from_png;
        GLuint texture_from_jpg;
        int width;
        int height;
};

SDL2TestApplicationImage::SDL2TestApplicationImage()
    : SDL2TestApplication(1, 1)
    , texture_from_png(0)
    , texture_from_jpg(0)
    , width(0)
    , height(0)
{
}

namespace TextureImage {
    enum Format {
        RGB,
        RGBA,
    };
};

static GLuint
textureFromImage(const char *filename, enum TextureImage::Format format)
{
    // Assumption: NPOT Texture Support

    GLuint id;

    SDL_Surface *surface = IMG_Load(filename);
    if (surface == NULL) {
        printf("IMG_Load: %s\n", IMG_GetError());
        exit(0);
    }

    SDL_PixelFormat fmt;
    memset(&fmt, 0, sizeof(fmt));
    if (format == TextureImage::RGBA) {
        fmt.format = SDL_PIXELFORMAT_RGBA8888;
        fmt.BitsPerPixel = 32;
        fmt.BytesPerPixel = 4;
        fmt.Rmask = 0x000000ff;
        fmt.Gmask = 0x0000ff00;
        fmt.Bmask = 0x00ff0000;
        fmt.Amask = 0xff000000;
    } else {
        fmt.format = SDL_PIXELFORMAT_RGB888;
        fmt.BitsPerPixel = 24;
        fmt.BytesPerPixel = 3;
        fmt.Rmask = 0x0000ff;
        fmt.Gmask = 0x00ff00;
        fmt.Bmask = 0xff0000;
        fmt.Amask = 0x000000;
    }

    SDL_Surface *tmp = SDL_ConvertSurface(surface, &fmt, 0);
    SDL_FreeSurface(surface);

    if (tmp == NULL) {
        printf("SDL_ConvertSurface: %s\n", SDL_GetError());
        exit(0);
    }

    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SDL_LockSurface(tmp);
    GLenum gl_format = ((format == TextureImage::RGBA) ? GL_RGBA : GL_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, gl_format, tmp->w, tmp->h, 0,
            gl_format, GL_UNSIGNED_BYTE, tmp->pixels);
    SDL_UnlockSurface(tmp);

    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(tmp);

    return id;
}

void
SDL2TestApplicationImage::initGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;

    int result = IMG_Init(flags);
    if (result != flags) {
        printf("IMG_Init: %s\n", IMG_GetError());
        return;
    }

    texture_from_png = textureFromImage(DATADIR "SDL_logo.png", TextureImage::RGBA);
    texture_from_jpg = textureFromImage(DATADIR "sailfish-site-bg_small.jpg", TextureImage::RGB);
}

void
SDL2TestApplicationImage::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, width, height, 0, -1, +1);

    this->width = width;
    this->height = height;
}

static void
draw_touch_point(TouchPoint *touch, void *user_data)
{
    float d = 160;

    float vtxcoords[] = {
        touch->x - d, touch->y - d,
        touch->x + d, touch->y - d,
        touch->x - d, touch->y + d,
        touch->x + d, touch->y + d,
    };

    float texcoords[] = {
        0, 0,
        1, 0,
        0, 1,
        1, 1,
    };

    glVertexPointer(2, GL_FLOAT, 0, vtxcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void
SDL2TestApplicationImage::renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, texture_from_jpg);
    float vtxcoords[] = {
        0, 0,
        width, 0,
        0, height,
        width, height,
    };
    float texcoords[] = { 0, 0, 1, 0, 0, 1, 1, 1 };

    glVertexPointer(2, GL_FLOAT, 0, vtxcoords);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, texture_from_png);
    for_each_touch(draw_touch_point, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

}

int
main(int argc, char *argv[])
{
    SDL2TestApplicationImage testapp_image;
    return testapp_image.run();
}

