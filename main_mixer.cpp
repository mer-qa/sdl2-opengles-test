/**
 *
 * SDL 2.0 Mixer Test Application
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

#include <SDL_mixer.h>

#include <audioresource.h>
#include <glib.h>

class SDL2TestApplicationMixer : public SDL2TestApplication {
    public:
        SDL2TestApplicationMixer();

        virtual void initGL();
        virtual void resizeGL(int width, int height);
        virtual void renderGL();

        virtual void onPressed(TouchPoint *touch);

    private:
        Mix_Chunk *sample;
        audioresource_t *audio_resource;

    public:
        bool mix_opened;
};


void
on_audio_resource_acquired(audioresource_t *audio_resource, bool acquired, void *user_data)
{
    /**
     * This function will be called from libaudioresource every time the
     * acquired status of a given audio resource changes. In particular, it
     * will be called with acquired=true when the application can start playing
     * audio, and will be called with acquired=false when the application must
     * stop playing audio (for example, when a higher priority audio stream such
     * as a different media player or phone call comes in).
     **/

    SDL2TestApplicationMixer *app = static_cast<SDL2TestApplicationMixer *>(user_data);

    if (acquired && !app->mix_opened) {
        fprintf(stderr, "Audio resource acquired.\n");
        int result = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
        if (result == -1) {
            printf("Mix_OpenAudio: %s\n", Mix_GetError());
            exit(1);
        }

        // Remember that we have opened the mixer, so we don't open it two times
        app->mix_opened = true;
    } else if (!acquired && app->mix_opened) {
        fprintf(stderr, "Audio resource lost.\n");
        // Must stop playing back audio here, otherwise writes to the audio device
        // will block. As long as we have requested the resource, the callback will
        // be called again once the audio resource is again ready for our use (e.g.
        // when the higher-priority audio has finished).
        Mix_CloseAudio();
        app->mix_opened = false;
    }
}


SDL2TestApplicationMixer::SDL2TestApplicationMixer()
    : SDL2TestApplication(1, 1)
    , sample(NULL)
    , audio_resource(NULL)
    , mix_opened(false)
{
    // Here, we declare a new audio resource that we want to later acquire - this is
    // a game, and every time the acquired value changes, on_audio_resource_acquired
    // will be called with "this" as user_data pointer.
    audio_resource = audioresource_init(AUDIO_RESOURCE_GAME,
            on_audio_resource_acquired, this);
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

    /* Acquire audio resource */
    audioresource_acquire(audio_resource);

    // Wait for the audio resource to be acquired; we need to run a GLib
    // mainloop iteration here, as this is what libaudioresource uses to
    // communicate with the resource framework.
    while (!mix_opened) {
        fprintf(stderr, "Waiting for audio resource to be acquired...\n");
        g_main_context_iteration(NULL, true);
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

    // Feed the glib main loop; this will make sure that any changes in the
    // resource policy (e.g. higher-priority audio makes us lose our
    // already-acquired audio resource) are handled, and audio is stoppped
    // in this case (and also later restarted)
    g_main_context_iteration(NULL, false);
}

int
main(int argc, char *argv[])
{
    SDL2TestApplicationMixer testapp_mixer;
    return testapp_mixer.run();
}

