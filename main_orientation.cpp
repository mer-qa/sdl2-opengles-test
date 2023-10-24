/**
 *
 * SDL 2.0 QtWayland Orientation Hint Test Application
 *
 * Copyright (C) 2015 Jolla Ltd.
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

#include "common.h"

#include <SDL_ttf.h>
#include <SDL_hints.h>

struct Size {
    Size() : w(0), h(0) {}

    int w;
    int h;
};

class SDL2TestApplicationOrientation : public SDL2TestApplication {
    public:
        SDL2TestApplicationOrientation();

        virtual void initGL();
        virtual void resizeGL(int width, int height);
        virtual void renderGL();
        virtual void onReleased(TouchPoint *touch);

    private:
        GLuint texture;
        TTF_Font *font;

        enum Mode {
            MODE_DEFAULT = 0,
            MODE_PORTRAIT,
            MODE_LANDSCAPE,
            MODE_REVERSE_PORTRAIT,
            MODE_REVERSE_LANDSCAPE,
            MODE_PORTRAIT_SWIPE_LOCK,
            MODE_LANDSCAPE_SWIPE_LOCK,
            MODE_LAST,
        } mode;
};

SDL2TestApplicationOrientation::SDL2TestApplicationOrientation()
    : SDL2TestApplication(1, 1)
    , texture(0)
    , font(NULL)
    , mode(MODE_DEFAULT)
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
SDL2TestApplicationOrientation::onReleased(TouchPoint *touch)
{
    mode = Mode(((int(mode) + 1) % MODE_LAST));

    const char *content_orientation_hint = "";
    const char *window_flags_hint = "";

    switch (mode) {
        case MODE_PORTRAIT:
            content_orientation_hint = "portrait";
            break;
        case MODE_LANDSCAPE:
            content_orientation_hint = "landscape";
            break;
        case MODE_REVERSE_PORTRAIT:
            content_orientation_hint = "inverted-portrait";
            break;
        case MODE_REVERSE_LANDSCAPE:
            content_orientation_hint = "inverted-landscape";
            break;
        case MODE_PORTRAIT_SWIPE_LOCK:
            content_orientation_hint = "portrait";
            window_flags_hint = "OverridesSystemGestures";
            break;
        case MODE_LANDSCAPE_SWIPE_LOCK:
            content_orientation_hint = "landscape";
            window_flags_hint = "OverridesSystemGestures";
            break;
        default:
            break;
    }

    SDL_SetHint(SDL_HINT_QTWAYLAND_CONTENT_ORIENTATION, content_orientation_hint);
    SDL_SetHint(SDL_HINT_QTWAYLAND_WINDOW_FLAGS, window_flags_hint);
}

void
SDL2TestApplicationOrientation::initGL()
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
SDL2TestApplicationOrientation::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, width, height, 0, -1, +1);
}

static struct Size
drawTextAt(TTF_Font *font, const char *text, float x, float y)
{
    struct Size textsize;

    if (text == NULL || strlen(text) == 0) {
        return textsize;
    }

    renderText(font, &textsize, text);

    float vtxcoords[] = {
        x, y,
        x + textsize.w, y,
        x, y + textsize.h,
        x + textsize.w, y + textsize.h,
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

    return textsize;
}

void
SDL2TestApplicationOrientation::renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, texture);
    {
        float x = 0.f;
        float y = 40.f; // Shift down a bit, so it's not hidden by the volume overlay
        struct Size size;

        size = drawTextAt(font, "Content orientation: ", x, y);
        x += size.w;

        const char *text = SDL_GetHint(SDL_HINT_QTWAYLAND_CONTENT_ORIENTATION);
        if (text != NULL && strlen(text) == 0) {
            text = NULL;
        }
        size = drawTextAt(font, text ?: "(default)", x, y);

        x = 0.f;
        y += 40.f;

        size = drawTextAt(font, "Window flags: ", x, y);
        x += size.w;

        text = SDL_GetHint(SDL_HINT_QTWAYLAND_WINDOW_FLAGS);
        if (text != NULL && strlen(text) == 0) {
            text = NULL;
        }
        size = drawTextAt(font, text ?: "(empty)", x, y);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

int
main(int argc, char *argv[])
{
    SDL2TestApplicationOrientation testapp_orientation;
    return testapp_orientation.run();
}

