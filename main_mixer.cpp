/**
 *
 * SDL 2.0 Mixer Test Application
 *
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jollamobile.com>
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

#include <SDL_mixer.h>

class SDL2TestApplicationMixer : public SDL2TestApplication {
    public:
        SDL2TestApplicationMixer();

        virtual void initGL();
        virtual void resizeGL(int width, int height);
        virtual void renderGL();

        virtual void onPressed(TouchPoint *touch);

    private:
        Mix_Chunk *sample;
};

SDL2TestApplicationMixer::SDL2TestApplicationMixer()
    : SDL2TestApplication(1, 1)
    , sample(NULL)
{
}

void
SDL2TestApplicationMixer::initGL()
{
    glClearColor(0.9, 0.3, 0.0, 1.0);

    int flags = MIX_INIT_OGG;

    int result = Mix_Init(flags);
    if ((result & flags) != flags) {
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    Mix_AllocateChannels(16);

    result = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 128);
    if (result == -1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        exit(1);
    }

    sample = Mix_LoadWAV(DATADIR "95328__ramas26__c.ogg");
    if (sample == NULL) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
        exit(1);
    }
}

void
SDL2TestApplicationMixer::resizeGL(int width, int height)
{
}

void
SDL2TestApplicationMixer::onPressed(TouchPoint *touch)
{
    // Play sample
    Mix_PlayChannel(-1, sample, 0);
}

void
SDL2TestApplicationMixer::renderGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

int
main(int argc, char *argv[])
{
    SDL2TestApplicationMixer testapp_mixer;
    return testapp_mixer.run();
}

