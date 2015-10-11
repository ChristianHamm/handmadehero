//
// Created by Christian Hamm on 26.04.15.
//

#include "hero.h"

Hero_AudioDef Hero_InitAudio() {
    SDL_zero(g_desired_audio_spec);

    Hero_AudioDef audio_def;
    SDL_zero(audio_def);

    g_desired_audio_spec.freq = k_audio_freq;
    g_desired_audio_spec.format = AUDIO_S16LSB;
    g_desired_audio_spec.channels = (Uint8) k_audio_channels;
    //g_desired_audio_spec.samples = 4096;
    g_desired_audio_spec.samples = (k_audio_freq *
                                    k_audio_bytes_per_sample /
                                    k_audio_rate);
    g_audio_device = SDL_OpenAudioDevice(g_audio_device_name, 0,
                                         &g_desired_audio_spec,
                                         &g_audio_spec,
                                         SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (g_audio_device == 0) {
        log_debug("Failed to open audio: %s\n", SDL_GetError());
    } else {
        log_debug("Opened audio device %s", g_audio_device_name);

        if (g_audio_spec.format != g_desired_audio_spec.format)
            log_debug("Could not get desired audio format.\n");

        audio_def.audio_device = g_audio_device;
        audio_def.audio_freq = g_audio_spec.freq;
        audio_def.audio_rate = k_audio_rate;
        audio_def.audio_channels = g_audio_spec.channels;
        audio_def.audio_bytes_per_sample = k_audio_bytes_per_sample;
        audio_def.sdl_audioSpec = g_audio_spec;
    }

    return audio_def;
}

SDL_Cursor *Hero_InitSystemCursor(const char **image) {
    int i, row, col;
    Uint8 data[4 * 32];
    Uint8 mask[4 * 32];
    int hot_x, hot_y;

    i = -1;
    for (row = 0; row < 32; ++row) {
        for (col = 0; col < 32; ++col) {
            if (col % 8) {
                data[i] <<= 1;
                mask[i] <<= 1;
            } else {
                ++i;
                data[i] = mask[i] = 0;
            }
            switch (image[4 + row][col]) {
                case 'X':
                    data[i] |= 0x01;
                    mask[i] |= 0x01;
                    break;
                case '.':
                    mask[i] |= 0x01;
                    break;
                case ' ':
                    break;
            }
        }
    }
    sscanf(image[4 + row], "%d,%d", &hot_x, &hot_y);
    return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}

void Hero_UpdateGameState(Hero_GameState *game_state,
                          Hero_GameInput *game_input) {
    if ((game_input->right && game_input->down)
        || (game_input->right && game_input->up)
        || (game_input->left && game_input->down)
        || (game_input->left && game_input->up)) {
        game_state->player_x +=
                0 +
                game_input->right * 6 -
                game_input->left * 6;

        game_state->player_y +=
                0 +
                game_input->down * 6 -
                game_input->up * 6;

    } else {
        game_state->player_x +=
                0 +
                game_input->right * 8 -
                game_input->left * 8;

        game_state->player_y +=
                0 +
                game_input->down * 8 -
                game_input->up * 8;
    }
}

void Hero_InitControllers() {
    log_debug("Init controllers...");

    if (g_game_controller)
        SDL_GameControllerClose(g_game_controller);

    g_num_game_controllers = SDL_NumJoysticks();
    log_debug("Found %d controller(s)", g_num_game_controllers);
    SDL_GameControllerEventState(SDL_IGNORE);

    for (int i = 0; i < g_num_game_controllers; ++i) {
        if (SDL_IsGameController(i)) {
            g_game_controller = SDL_GameControllerOpen(i);
            if (g_game_controller) {
                log_debug("Controller #%d name %s", i,
                          SDL_GameControllerNameForIndex(i));
                SDL_GameControllerEventState(SDL_ENABLE);
                break;
            } else {
                fprintf(stderr, "Could not open gamecontroller %i: %s\n", i,
                        SDL_GetError());
            }
        }
    }
}

SDL_bool Hero_HandleEvents(Hero_GameInput *game_input) {
    SDL_bool running = SDL_TRUE;
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = SDL_FALSE;
        }

        // Controller plugged in or out
        if (event.type == SDL_CONTROLLERDEVICEADDED ||
            event.type == SDL_CONTROLLERDEVICEREMOVED)
            Hero_InitControllers();

        // Controller DPad stuff
        if (event.type == SDL_CONTROLLERBUTTONDOWN &&
            event.cbutton.state == SDL_PRESSED) {
            switch (event.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    game_input->right = 1;
                    game_input->left = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    game_input->right = 0;
                    game_input->left = 1;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    game_input->up = 1;
                    game_input->down = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    game_input->up = 0;
                    game_input->down = 1;
                    break;
                case SDL_CONTROLLER_BUTTON_BACK:
                    running = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }

        // Controller DPad stuff
        if (event.type == SDL_CONTROLLERBUTTONUP &&
            event.cbutton.state == SDL_RELEASED) {
            switch (event.cbutton.button) {
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    game_input->right = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    game_input->left = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    game_input->up = 0;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    game_input->down = 0;
                    break;
                default:
                    break;
            }
        }

        // Controller Axis stuff
        /*
        if (event.type == SDL_CONTROLLERAXISMOTION) {
            switch (event.caxis.axis) {
                case SDL_CONTROLLER_AXIS_LEFTX:
                    log_debug("left axis: %d", event.caxis.value);
                    if (event.caxis.value > 15500) {
                        game_input->right = 1;
                    }
                    else if (event.caxis.value < -15500) {
                        game_input->left = 1;
                    } else {
                        game_input->left = 0;
                        game_input->right = 0;
                    }
                    break;
                case SDL_CONTROLLER_AXIS_LEFTY:
                    //log_debug("left axis: %d", event.caxis.value);
                    if (event.caxis.value > 15500) {
                        game_input->down = 1;
                    }
                    else if (event.caxis.value < -15500) {
                        game_input->up = 1;
                    } else {
                        game_input->up = 0;
                        game_input->down = 0;
                    }
                    break;
                default:
                    break;
            }
        }
         */

        if (event.type == SDL_KEYDOWN && event.key.state == SDL_PRESSED) {
            log_debug("Key down %d - %s", event.key.keysym.sym,
                      SDL_GetKeyName(event.key.keysym.sym));

            switch (event.key.keysym.sym) {
                case SDLK_d:
                case SDLK_RIGHT:
                    game_input->right = 1;
                    game_input->left = 0;
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    game_input->right = 0;
                    game_input->left = 1;
                    break;
                case SDLK_w:
                case SDLK_UP:
                    game_input->up = 1;
                    game_input->down = 0;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    game_input->up = 0;
                    game_input->down = 1;
                    break;
                default:
                    break;
            }
        }

        if (event.type == SDL_KEYUP && event.key.state == SDL_RELEASED) {
            switch (event.key.keysym.sym) {
                case SDLK_d:
                case SDLK_RIGHT:
                    game_input->right = 0;
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    game_input->left = 0;
                    break;
                case SDLK_w:
                case SDLK_UP:
                    game_input->up = 0;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    game_input->down = 0;
                default:
                    break;
            }
        }

        if (event.type == SDL_WINDOWEVENT) {
            // Always update the graphics when we get an window event
            SDL_Window *window = SDL_GetWindowFromID(event.window.windowID);
            Hero_ResizeAndUpdateWindow(window, g_backbuffer);

            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    SDL_Log("Window %d resized to %dx%d",
                            event.window.windowID, event.window.data1,
                            event.window.data2);
                    // Stretch the window
                    break;
                default:
                    SDL_Log("Window %d got unknown event %d",
                            event.window.windowID, event.window.event);
                    break;
            }
        }
    }

    return running;
}

void Hero_ResizeAndUpdateWindow(SDL_Window *window, SDL_Surface *surface) {
   SDL_Surface *current_surface = SDL_GetWindowSurface(window);

   SDL_SoftStretch(surface, NULL, current_surface, NULL);
   SDL_UpdateWindowSurface(window);
}

void Hero_PrintSDLVersion() {
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

    log_debug("sizeof(Uint32) = %ld", sizeof(Uint32));
    log_debug("sizeof(Uint8) = %ld", sizeof(Uint8));
}
