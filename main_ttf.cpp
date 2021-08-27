/**
 *
 * SDL 2.0 TTF Test Application
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

#include <SDL_ttf.h>

struct Size {
    Size() : w(0), h(0) {}

    int w;
    int h;
};

class SDL2TestApplicationTTF : public SDL2TestApplication {
    public:
        SDL2TestApplicationTTF();

        virtual void initGL();
        virtual void resizeGL(int width, int height);
        virtual void renderGL();

    private:
        GLuint texture;
        TTF_Font *font;
};

SDL2TestApplicationTTF::SDL2TestApplicationTTF()
    : SDL2TestApplication(1, 1)
    , texture(0)
    , font(NULL)
{
}

static void
renderText(TTF_Font *font, struct Size *textsize, const char *text)
{
    // Assumption: NPOT Texture Support

    SDL_Color white = {0xff, 0xff, 0xff, 0xff};
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, white);

    if (surface == NULL) {
        printf("TTF_RenderUTF8_Blended: %s\n", TTF_GetError());
        exit(0);
    }

    SDL_PixelFormat fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.format = SDL_PIXELFORMAT_RGBA8888;
    fmt.BitsPerPixel = 32;
    fmt.BytesPerPixel = 4;
    fmt.Rmask = 0x000000ff;
    fmt.Gmask = 0x0000ff00;
    fmt.Bmask = 0x00ff0000;
    fmt.Amask = 0xff000000;

    SDL_Surface *tmp = SDL_ConvertSurface(surface, &fmt, 0);
    SDL_FreeSurface(surface);

    if (tmp == NULL) {
        printf("SDL_ConvertSurface: %s\n", SDL_GetError());
        exit(0);
    }

    textsize->w = tmp->w;
    textsize->h = tmp->h;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    SDL_LockSurface(tmp);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp->w, tmp->h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, tmp->pixels);
    SDL_UnlockSurface(tmp);

    SDL_FreeSurface(tmp);
}

void
SDL2TestApplicationTTF::initGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        return;
    }

    glGenTextures(1, &texture);

    int size = 30;
    font = TTF_OpenFont(DATADIR "SourceSansPro-Regular.ttf", size);
}

void
SDL2TestApplicationTTF::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, width, height, 0, -1, +1);
}

static void
draw_touch_point(TouchPoint *touch, void *user_data)
{
    TTF_Font *font = (TTF_Font *)user_data;

    struct Size textsize;
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "Touch %d: %.2f/%.2f", touch->id, touch->x, touch->y);

    renderText(font, &textsize, tmp);

    float vtxcoords[] = {
        touch->x, touch->y,
        touch->x + textsize.w, touch->y,
        touch->x, touch->y + textsize.h,
        touch->x + textsize.w, touch->y + textsize.h,
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
SDL2TestApplicationTTF::renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, texture);
    for_each_touch(draw_touch_point, font);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

int
main(int argc, char *argv[])
{
    SDL2TestApplicationTTF testapp_ttf;
    return testapp_ttf.run();
}

