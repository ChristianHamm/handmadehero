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
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

const Uint32 WIDTH = 640;
const Uint32 HEIGHT = 480;
const Uint32 DEPTH = 32;

#define log_debug(...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

int main(int argc, char ** argv) {
    srand(time(NULL));
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
        WIDTH, HEIGHT,
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

    surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, DEPTH,
                                   rmask, gmask, bmask, amask);
    screenSurface = SDL_GetWindowSurface(window);

    if(surface == NULL) {
        log_debug("CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }

    log_debug("surface->pitch: %d", surface->pitch);

    while(1) {
        SDL_Event e;

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }

        // Direct access to surface->pixels
        SDL_LockSurface(surface);

        for (int y = 0; y < HEIGHT ; y++) {
                Uint8 R = (rand()) % 0xff;
                Uint8 G = (rand()) % 0xff;
                Uint8 B = (rand()) % 0xff;
                Uint32 color = (0xff << 24)
                                | ((B & (0xff)) << 16)
                                | ((G & (0xff)) << 8)
                                | ((R & 0xff));
            for (int x = 0; x < WIDTH ; x++) {

                Uint8* pixel = (Uint8 *) surface->pixels;

                pixel += (y * surface->pitch) + (x * sizeof(Uint32));
                /*log_debug("x,y: %d,%d; color=%x, pixel=%p", x, y, color, pixel);*/
                *((Uint32*) pixel) = color;
            }
                log_debug("color = 0x%x", color);
        }

        SDL_Delay(50);
        // End direct access to pixels
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

void Hero_PutPixel(SDL_Surface *surface,
    const Uint32 x, const Uint32 y, const Uint32 color) {

    Uint8* pixel = (Uint8 *) surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint32));
    /*log_debug("x,y: %d,%d; pixel=%p", x, y, pixel);*/
    *((Uint32*) pixel) = color;
}

