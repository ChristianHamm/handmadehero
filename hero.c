/* ****************************************************************************
 *
 * Adaption of Casey's Handmade Hero in C with SDL
 *
 * Author: Christian Hamm
 * Date: 25.02.2015
 *
 *************************************************************************** */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#define log_debug(...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

int main(int argc, char ** argv) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_version compiled;
    SDL_version linked;

    Uint32 subsystem_video_mask = SDL_INIT_VIDEO;
    Uint32 subsystem_audio_mask = SDL_INIT_AUDIO;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
    log_debug("Init");
    log_debug("SDL Revision %s", SDL_GetRevision());

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    log_debug("Compiled against SDL version %d.%d.%d\n",
       compiled.major, compiled.minor, compiled.patch);
    log_debug("Linking against SDL version %d.%d.%d\n",
       linked.major, linked.minor, linked.patch);

    /* Check for two subsystems */
    if (SDL_WasInit(subsystem_video_mask) == subsystem_video_mask) {
        log_debug("Video initialized.");
    } else {
        log_debug("Video not initialized.");
    }

    if (SDL_WasInit(subsystem_audio_mask) == subsystem_audio_mask) {
        log_debug("Audio initialized.");
    } else {
        log_debug("Audio not initialized.");
    }

    SDL_Window* window = SDL_CreateWindow(
        "Handmade Hero",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        0);

    SDL_Surface *surface;
    SDL_Surface *screenSurface;
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurface(0, 640, 480, 32,
                                   rmask, gmask, bmask, amask);
    screenSurface = SDL_GetWindowSurface(window);

    if(surface == NULL) {
        log_debug("CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }




    while(1) {
        SDL_Event e;

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }

        //SDL_Delay(10);
        SDL_LockSurface(surface);
        SDL_UnlockSurface(surface);
        SDL_BlitSurface(surface, NULL, screenSurface, NULL);
        SDL_UpdateWindowSurface(window);
    }

    SDL_FreeSurface(surface);
    SDL_FreeSurface(screenSurface);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

