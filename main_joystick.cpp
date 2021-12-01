/**
 *
 * SDL 2.0 Joystick Test Application
 *
 * Copyright (C) 2014 Jolla Ltd.
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
#include <SDL_joystick.h>

struct Size {
    Size() : w(0), h(0) {}

    int w;
    int h;
};

class SDL2TestApplicationJoystick : public SDL2TestApplication {
    public:
        SDL2TestApplicationJoystick();

        virtual void initGL();
        virtual void resizeGL(int width, int height);
        virtual void renderGL();

        float renderText(float x, float y, const char *fmt, ...);

    private:
        GLuint texture;
        TTF_Font *font;
        SDL_Joystick *joy;
};

SDL2TestApplicationJoystick::SDL2TestApplicationJoystick()
    : SDL2TestApplication(1, 1)
    , texture(0)
    , font(NULL)
    , joy(NULL)
{
}

static void
ttf_render_text(TTF_Font *font, struct Size *textsize, const char *text)
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
SDL2TestApplicationJoystick::initGL()
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

    // Initialize the joystick subsystem
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    if (SDL_NumJoysticks() > 0) {
        // Open joystick
        joy = SDL_JoystickOpen(0);

        if (joy) {
            printf("Joystick name: %s\n", SDL_JoystickNameForIndex(0));
        } else {
            printf("Couldn't open Joystick 0\n");
        }
    } else {
        printf("No joysticks connected\n");
    }
}

void
SDL2TestApplicationJoystick::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, width, height, 0, -1, +1);
}

float
SDL2TestApplicationJoystick::renderText(float x, float y, const char *fmt, ...)
{
    struct Size textsize;

    va_list args;
    va_start(args, fmt);

    char *text;
    if (vasprintf(&text, fmt, args) < 0) {
        return 0;
    }

    ttf_render_text(font, &textsize, text);
    free(text);

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

    return textsize.h;
}

void
SDL2TestApplicationJoystick::renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, texture);

#define TEXTOUT(...) y += renderText(0.f, y, __VA_ARGS__)

    float y = 0.f;
    if (joy) {
        TEXTOUT("Name: %s", SDL_JoystickNameForIndex(0));
        TEXTOUT("Axes: %d, Buttons: %d, Balls: %d, Hats: %d",
                SDL_JoystickNumAxes(joy),
                SDL_JoystickNumButtons(joy),
                SDL_JoystickNumBalls(joy),
                SDL_JoystickNumHats(joy));

        TEXTOUT("---");
        for (int i=0; i<SDL_JoystickNumAxes(joy); i++) {
            TEXTOUT("Axis %d: %d", i, SDL_JoystickGetAxis(joy, i));
        }

        TEXTOUT("---");
        bool anyPressed = false;
        for (int i=0; i<SDL_JoystickNumButtons(joy); i++) {
            if (SDL_JoystickGetButton(joy, i)) {
                TEXTOUT("Button %d pressed", i);
                anyPressed = true;
            }
        }
        if (!anyPressed) {
            TEXTOUT("No button pressed");
        }

        TEXTOUT("---");
        for (int i=0; i<SDL_JoystickNumBalls(joy); i++) {
            int dx = 0, dy = 0;
            SDL_JoystickGetBall(joy, i, &dx, &dy);
            TEXTOUT("Axis %d: (%d,%d)", i, dx, dy);
        }

        TEXTOUT("---");
        for (int i=0; i<SDL_JoystickNumHats(joy); i++) {
            const char *hatState;

#define CASE_HAT_STATE(x) case x: hatState = #x; break

            switch (SDL_JoystickGetHat(joy, i)) {
                CASE_HAT_STATE(SDL_HAT_CENTERED);
                CASE_HAT_STATE(SDL_HAT_UP);
                CASE_HAT_STATE(SDL_HAT_RIGHT);
                CASE_HAT_STATE(SDL_HAT_DOWN);
                CASE_HAT_STATE(SDL_HAT_LEFT);
                CASE_HAT_STATE(SDL_HAT_RIGHTUP);
                CASE_HAT_STATE(SDL_HAT_RIGHTDOWN);
                CASE_HAT_STATE(SDL_HAT_LEFTUP);
                CASE_HAT_STATE(SDL_HAT_LEFTDOWN);
                default:
                    hatState = "<unknown>";
                    break;
            }

#undef CASE_HAT_STATE

            TEXTOUT("Hat %d: %s (%d)", i, hatState,
                    SDL_JoystickGetHat(joy, i));
        }

    } else {
        TEXTOUT("No joystick connected");
    }

#undef TEXTOUT

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

int
main(int argc, char *argv[])
{
    SDL2TestApplicationJoystick testapp_joystick;
    return testapp_joystick.run();
}

