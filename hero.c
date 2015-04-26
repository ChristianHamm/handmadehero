/* ****************************************************************************
 *
 * Adaption of Casey's Handmade Hero in C with SDL
 *
 * Author: Christian Hamm
 * Date: 25.02.2015
 *
 *************************************************************************** */

#include "hero.h"
#include <dlfcn.h>

typedef void *(*_Hero_PlayTestSound)(Hero_AudioDef audio_def);

int main(int argc, char **argv) {
    // Init stuff
    srand((unsigned int) time(NULL));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
    Hero_PrintSDLVersion();

    // Audio stuff
    Hero_AudioDef audio_def = Hero_InitAudio();

    // Create the window and renderer
    SDL_Window *window = SDL_CreateWindow(
            "Handmade Hero",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            k_window_width, k_window_height,
            SDL_WINDOW_RESIZABLE);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Initially set the pixel buffer dimensions
    int window_w, window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    g_pixel_buffer_width = (Uint32) window_w;
    g_pixel_buffer_height = (Uint32) window_h;
    //Hero_ResizeTexture(renderer, (Uint32) window_w, (Uint32) window_h);

    // Loop things
    int sound_is_playing = 0;
    Uint32 frame_step = 0;
    int running = 1;

    SDL_SetCursor(Hero_InitSystemCursor(arrow));
    SDL_ShowCursor(SDL_ENABLE);


    while (running) {
        // Performance
        Uint64 perf_freq = SDL_GetPerformanceFrequency();
        Uint64 perf_counter_start = SDL_GetPerformanceCounter();

        // Load our library
        void *logic_lib = dlopen("libherologic.so", RTLD_NOW);
        _Hero_PlayTestSound Hero_PlayTestSound = dlsym(logic_lib,
                                                       "Hero_PlayTestSound");

        // Actual game stuff
        running = Hero_HandleEvents();
        SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
        //Hero_DebugDrawGradient(frame_step);
        //Hero_DebugDrawWeirdGradient();


        Hero_UpdateGraphics(renderer);

        // Playing test sound
        Hero_PlayTestSound(audio_def);

        if (!sound_is_playing) {
            SDL_PauseAudioDevice(g_audio_device, 0);
            sound_is_playing = 1;
        }

        dlclose(logic_lib);

        // Performance
        Uint64 perf_counter_end = SDL_GetPerformanceCounter();
        Uint64 perf_counter_elapsed = perf_counter_end - perf_counter_start;
        double perf_per_frame = ((
                (1000.0f * (double) perf_counter_elapsed) /
                (double) perf_freq));

        // Ensure we are at a constant framerate
        double fps_padding_time = k_display_msmax - perf_per_frame;

        if (fps_padding_time > 0)
            SDL_Delay((Uint32) fps_padding_time);

        if ((frame_step % 100) == 0)
            log_debug("Frame time %d: %f, maxms %f, padding %f", frame_step,
                      perf_per_frame, k_display_msmax, fps_padding_time);

        frame_step++;
    }

    log_debug("Shutting down...");

    if (g_game_controller != NULL)
        SDL_GameControllerClose(g_game_controller);

    /*
    if (g_pixel_buffer)
        munmap(g_pixel_buffer,
               g_pixel_buffer_width * g_pixel_buffer_height *
               k_bytes_per_pixel);
               */

    if (g_texture)
        SDL_DestroyTexture(g_texture);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

