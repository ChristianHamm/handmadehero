/* ****************************************************************************
 *
 * Adaption of Casey's Handmade Hero in C with SDL
 *
 * Author: Christian Hamm
 * Date: 25.02.2015
 *
 *************************************************************************** */

#include "hero.h"

// Dynamic loading of functions
#ifdef HERO_DYNLOAD
typedef void *(*_Hero_PlayTestSound)(Hero_AudioDef audio_def);

typedef void *(*_Hero_DebugDrawRunningPixel)(SDL_Renderer *renderer,
                                             Hero_GameState *game_state);
#endif

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

    // Just output the viewport
    SDL_Rect *viewport = SDL_malloc(sizeof(SDL_Rect));
    SDL_RenderGetViewport(renderer, viewport);
    SDL_free(viewport);

    log_debug("got viewport: x: %d, y: %d, h: %d, w: %d", viewport->x,
              viewport->y, viewport->h, viewport->w);

    // Initially clear the screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Initially set the pixel buffer dimensions
    int window_w, window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    g_current_screen_width = (Uint32) window_w;
    g_current_screen_height = (Uint32) window_h;
    //Hero_ResizeTexture(renderer, (Uint32) window_w, (Uint32) window_h);

    // Loop things
    int sound_is_playing = 0;
    Uint32 frame_step = 0;
    int running = 1;

    SDL_SetCursor(Hero_InitSystemCursor(arrow));
    SDL_ShowCursor(SDL_ENABLE);

#ifdef HERO_DYNLOAD
    g_logic_lib = SDL_LoadObject("libherologic.so");
    _Hero_PlayTestSound Hero_PlayTestSound = SDL_LoadFunction(g_logic_lib,
                                                              "Hero_PlayTestSound");
    _Hero_DebugDrawRunningPixel Hero_DebugDrawRunningPixel = SDL_LoadFunction(
            g_logic_lib,
            "Hero_DebugDrawRunningPixel");
#endif

    Hero_GameInput *game_input = malloc(sizeof(Hero_GameInput));
    SDL_zerop(game_input);

    Hero_GameState *game_state = malloc(sizeof(Hero_GameState));
    SDL_zerop(game_state);

    while (running) {
        // Performance
        Uint64 perf_freq = SDL_GetPerformanceFrequency();
        Uint64 perf_counter_start = SDL_GetPerformanceCounter();

#ifdef HERO_DYNLOAD
        // Load our library every n frames
        if ((frame_step % 120) == 0) {
            SDL_UnloadObject(g_logic_lib);
            log_debug("reloading symbols");
            g_logic_lib = SDL_LoadObject("libherologic.so");
            Hero_PlayTestSound = SDL_LoadFunction(g_logic_lib,
                                                  "Hero_PlayTestSound");
            Hero_DebugDrawRunningPixel = SDL_LoadFunction(
                    g_logic_lib,
                    "Hero_DebugDrawRunningPixel");
        }
#endif

        // Actual game stuff
        running = Hero_HandleEvents(game_input);
        SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
        //Hero_DebugDrawGradient(frame_step);
        //Hero_DebugDrawWeirdGradient();

        Hero_UpdateGameState(game_state, game_input);
        Hero_UpdateGraphics(renderer);
        Hero_DebugDrawRunningPixel(renderer, game_state);

        SDL_RenderPresent(renderer);

        // Playing test sound
        //Hero_PlayTestSound(audio_def);

        if (!sound_is_playing) {
            SDL_PauseAudioDevice(g_audio_device, 1);
            sound_is_playing = 1;
        }

        // Performance
        Uint64 perf_counter_end = SDL_GetPerformanceCounter();
        Uint64 perf_counter_elapsed = perf_counter_end - perf_counter_start;
        double perf_per_frame = ((
                (1000.0f * (double) perf_counter_elapsed) /
                (double) perf_freq));

        // Ensure we are at a constant framerate
        double fps_padding_time = k_display_msmax - perf_per_frame;

//        if (fps_padding_time > 0)
//            SDL_Delay((Uint32) fps_padding_time);

        if ((frame_step % 120) == 0)
            log_debug("Frame time %d: %f ms, max %f ms, padding %f ms",
                      frame_step,
                      perf_per_frame, k_display_msmax, fps_padding_time);

        frame_step++;
    }

    log_debug("Shutting down...");

    if (g_game_controller != NULL)
        SDL_GameControllerClose(g_game_controller);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

