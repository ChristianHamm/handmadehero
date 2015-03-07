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
#include <sys/mman.h>
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

static SDL_Texture* texture;
static void* pixels;

void Hero_PutPixel(void *pixels, int pitch,
    const Uint32 x, const Uint32 y, const Uint32 color) {

    Uint8* pixel = (Uint8 *) pixels;
    pixel += (y * pitch) + (x * sizeof(Uint32));
    *((Uint32*) pixel) = color;
}

void Hero_ResizeTexture(SDL_Renderer* renderer, int width, int height) {
    if(pixels)
        pixels = realloc(pixels, width * height * 4);
    else
        pixels = malloc(width * height * 4);

    memset(pixels, 0xff, width * height * 4);

    if(texture)
        SDL_DestroyTexture(texture);

    texture = SDL_CreateTexture(renderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         width,
                                         height);
}

void Hero_DrawGradient(int window_w, int window_h, int frame_step) {
        for (int y = 0; y < window_h ; y++) {
            for (int x = 0; x < window_w ; x++) {
                Uint32 number = y + frame_step;
                //Uint32 number = rand();
                Uint8 R = (x) % 256;
                Uint8 G = (x) % 256;
                Uint8 B = (number) % 256;
                Uint32 color = (0x06 << 24)
                                | ((B & (0xff)) << 16)
                                | ((G & (0xff)) << 8)
                                | ((R & 0xff));
                Hero_PutPixel(pixels, window_w * 4,
                    (x - frame_step) % window_w, y, color);
            }
        }
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
        SDL_WINDOW_RESIZABLE);


    int window_w, window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        //SDL_RENDERER_PRESENTVSYNC);
         SDL_RENDERER_SOFTWARE);

    Uint32 frame_step = 0;
    int running = 1;

    Hero_ResizeTexture(renderer, window_w, window_h);

    while(running) {
        Uint64 perf_freq = SDL_GetPerformanceFrequency();
        Uint64 perf_counter_start = SDL_GetPerformanceCounter();
        SDL_GetWindowSize(window, &window_w, &window_h);

        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if (event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                case SDL_WINDOWEVENT_SHOWN:
                    SDL_Log("Window %d shown", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_HIDDEN:
                    SDL_Log("Window %d hidden", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_EXPOSED:
                    SDL_Log("Window %d exposed", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_MOVED:
                    SDL_Log("Window %d moved to %d,%d",
                            event.window.windowID, event.window.data1,
                            event.window.data2);
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    SDL_Log("Window %d resized to %dx%d",
                            event.window.windowID, event.window.data1,
                            event.window.data2);
                    Hero_ResizeTexture(renderer, window_w, window_h);
                    break;
                case SDL_WINDOWEVENT_MINIMIZED:
                    SDL_Log("Window %d minimized", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_MAXIMIZED:
                    SDL_Log("Window %d maximized", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_RESTORED:
                    SDL_Log("Window %d restored", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_ENTER:
                    SDL_Log("Mouse entered window %d",
                            event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_LEAVE:
                    SDL_Log("Mouse left window %d", event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    SDL_Log("Window %d gained keyboard focus",
                            event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    SDL_Log("Window %d lost keyboard focus",
                            event.window.windowID);
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    SDL_Log("Window %d closed", event.window.windowID);
                    break;
                default:
                    SDL_Log("Window %d got unknown event %d",
                            event.window.windowID, event.window.event);
                    break;
                }
            }
        }

        //Hero_DrawGradient(window_w, window_h, frame_step);

        /*
        for (int y = 0; y < window_h ; y++) {
            for (int x = 0; x < window_w ; x++) {
                Uint32 number = y + frame_step;
                //Uint32 number = rand();
                Uint8 R = (x) % 256;
                Uint8 G = (x) % 256;
                Uint8 B = (number) % 256;
                Uint32 color = (0x06 << 24)
                                | ((B & (0xff)) << 16)
                                | ((G & (0xff)) << 8)
                                | ((R & 0xff));
                Hero_PutPixel(pixels, window_w * 4,
                    (x - frame_step) % window_w, y, color);
            }
        }
        */

        SDL_RenderClear(renderer);
        if (SDL_UpdateTexture(texture, 0, pixels, window_w * 4)) {
            log_debug("Could not update texture!");
            running = 0;
        }

        // End direct access to pixels
        SDL_RenderCopy(renderer, texture, 0, 0);
        SDL_RenderPresent(renderer);

        // Performance
        Uint64 perf_counter_end = SDL_GetPerformanceCounter();
        Uint64 perf_counter_elapsed = perf_counter_end - perf_counter_start;
        double perf_per_frame = (((1000.0f * (double)perf_counter_elapsed) / (double)perf_freq));

        if((frame_step % 100) == 0)
            log_debug("Frame time %d: %f", frame_step, perf_per_frame);

        frame_step++;
    }

    if(pixels)
        free(pixels);

    if(texture)
        SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

