//
// Created by Christian Hamm on 26.04.15.
//

#ifndef HERO_GLOBALS_H
#define HERO_GLOBALS_H

#include <SDL2/SDL.h>

extern void *g_logic_lib;

extern const Uint32 k_window_width;
extern const Uint32 k_window_height;
extern const double k_display_msmax;

extern const Uint16 k_audio_freq;
extern const Uint16 k_audio_rate;
extern const Uint16 k_audio_channels;

extern const Sint16 k_audio_bytes_per_sample;

typedef struct _Hero_AudioDef {
    SDL_AudioDeviceID audio_device;
    int audio_freq;
    int audio_rate;
    int audio_channels;
    int audio_bytes_per_sample;
    SDL_AudioSpec sdl_audioSpec;
} Hero_AudioDef;

extern Uint32 g_current_screen_height;
extern Uint32 g_current_screen_width;
extern const Uint32 k_bytes_per_pixel;

extern SDL_AudioSpec g_desired_audio_spec, g_audio_spec;
extern SDL_AudioDeviceID g_audio_device;
extern char *g_audio_device_name;

extern SDL_GameController *g_game_controller;
extern int g_num_game_controllers;

typedef struct _Hero_GameInput {
    int right;
    int left;
    int up;
    int down;
    int btna;
    int btnb;
    int btnx;
    int btny;
} Hero_GameInput;

typedef struct _Hero_GameState {
    int player_x;
    int player_y;
} Hero_GameState;


extern SDL_Point mouse_position;

extern const char *arrow[];

#endif //HERO_GLOBALS_H
