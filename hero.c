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
//typedef void *(*_Hero_PlayTestSound)(Hero_AudioDef audio_def);

typedef void *(*_Hero_UpdateGameState)(Hero_GameState *game_state,
                                       Hero_GameInput *game_input,
                                       SDL_Surface *buffer);

#endif

int main(int argc, char **argv) {
    // Init stuff
    //srand((unsigned int) time(NULL));
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

    // Get the window surface, make a copy of it and update the window
    SDL_Surface *source = SDL_GetWindowSurface(window);
    g_backbuffer = SDL_ConvertSurfaceFormat(source, source->format->format, 0);
    Hero_ResizeAndUpdateWindow(window, g_backbuffer, SDL_TRUE);
    SDL_FreeSurface(source);

    // Loop things
    SDL_bool sound_is_playing = SDL_FALSE;
    Uint32 frame_step = 0;
    SDL_bool running = SDL_TRUE;

    //SDL_SetCursor(Hero_InitSystemCursor(arrow));
    //SDL_ShowCursor(SDL_ENABLE);

#ifdef HERO_DYNLOAD
    g_logic_lib = SDL_LoadObject("libherologic.so");
    _Hero_UpdateGameState Hero_UpdateGameState = SDL_LoadFunction(
            g_logic_lib,
            "Hero_UpdateGameState");
#endif

    // Game Setup
    Hero_GameInput *game_input = SDL_malloc(sizeof(Hero_GameInput));
    SDL_zerop(game_input);

    Hero_GameState *game_state = SDL_malloc(sizeof(Hero_GameState));
    SDL_zerop(game_state);

    game_state->player_position.abs_tile_x = 3;
    game_state->player_position.abs_tile_y = 3;
    game_state->player_position.tile_rel_x = 5.0f;
    game_state->player_position.tile_rel_y = 5.0f;

    while (running) {
        // Performance
        Uint64 perf_freq = SDL_GetPerformanceFrequency();
        Uint64 perf_counter_start = SDL_GetPerformanceCounter();

#ifdef HERO_DYNLOAD
        // Load our library every n frames
        if ((frame_step % 30) == 0) {
            SDL_UnloadObject(g_logic_lib);
            //log_debug("reloading symbols");
            g_logic_lib = SDL_LoadObject("libherologic.so");
            Hero_UpdateGameState = SDL_LoadFunction(g_logic_lib,
                                                    "Hero_UpdateGameState");
        }
#endif

        // Actual game stuff
        running = Hero_HandleEvents(game_input);
        //SDL_GetMouseState(&g_mouse_position.x, &g_mouse_position.y);
        Hero_UpdateGameState(game_state, game_input, g_backbuffer);
        Hero_ResizeAndUpdateWindow(window, g_backbuffer, SDL_FALSE);
        Hero_DebugPlayTestSound(audio_def);

        if (!sound_is_playing) {
            SDL_PauseAudioDevice(g_audio_device, 1);
            sound_is_playing = SDL_TRUE;
        }

        // Performance
        Uint64 perf_counter_end = SDL_GetPerformanceCounter();
        Uint64 perf_counter_elapsed = perf_counter_end - perf_counter_start;
        double perf_per_frame = ((
                (1000.0f * (double) perf_counter_elapsed) /
                (double) perf_freq));

        // Ensure we are at a constant framerate
        double fps_padding_time = k_display_msmax - perf_per_frame;
        // Save the value for movement speed adjustment
        game_input->frame_dt = (float) fps_padding_time * 0.01f;

        if (fps_padding_time > 0)
            SDL_Delay((Uint32) fps_padding_time);

        if ((frame_step % 30) == 0)
            log_debug("Frame stats: %f ms, max %f ms, padding %f ms",
                      perf_per_frame, k_display_msmax, fps_padding_time);

        frame_step++;
    }

    log_debug("Shutting down...");

    if (g_game_controller != NULL)
        SDL_GameControllerClose(g_game_controller);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

