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

void Hero_PutPixel(SDL_Surface *surface,
    const Uint32 x, const Uint32 y, const Uint32 color) {

    Uint8* pixel = (Uint8 *) surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint32));
    /*log_debug("x,y: %d,%d; pixel=%p", x, y, pixel);*/
    *((Uint32*) pixel) = color;
}

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

    SDL_Surface *screenSurface;
    screenSurface = SDL_GetWindowSurface(window);

    Uint32 frame_step = 0;

    while(1) {
        Uint64 perf_freq = SDL_GetPerformanceFrequency();
        Uint64 perf_counter_start = SDL_GetPerformanceCounter();


        SDL_Event e;

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }

        // Direct access to surface->pixels
        SDL_LockSurface(screenSurface);


        for (int y = 0; y < HEIGHT ; y++) {
            for (int x = 0; x < WIDTH ; x++) {
                Uint32 number = y + frame_step;
                //Uint32 number = rand();
                Uint8 R = (x) % 256;
                Uint8 G = (x) % 256;
                Uint8 B = (number) % 256;
                Uint32 color = (0x06 << 24)
                                | ((B & (0xff)) << 16)
                                | ((G & (0xff)) << 8)
                                | ((R & 0xff));
                Hero_PutPixel(screenSurface, (x - frame_step) % WIDTH, y, color);
            }
                //log_debug("color = 0x%x", color);
        }

        frame_step++;

        // End direct access to pixels
        SDL_UnlockSurface(screenSurface);
        SDL_UpdateWindowSurface(window);
        Uint64 perf_counter_end = SDL_GetPerformanceCounter();
        Uint64 perf_counter_elapsed = perf_counter_end - perf_counter_start;
        double perf_per_frame = (((1000.0f * (double)perf_counter_elapsed) / (double)perf_freq));

        if((frame_step % 100) == 0)
            log_debug("Frame time %d: %f", frame_step, perf_per_frame);
        //SDL_Delay(1);
    }

    SDL_FreeSurface(screenSurface);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

