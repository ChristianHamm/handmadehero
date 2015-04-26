//
// Created by Christian Hamm on 26.04.15.
//

#ifndef HERO_GLOBALS_H
#define HERO_GLOBALS_H

#include <SDL2/SDL.h>

extern const Uint32 k_window_width;
extern const Uint32 k_window_height;
extern const double k_display_msmax;

extern const Uint16 k_audio_freq;
extern const Uint16 k_audio_rate;
extern const Uint16 k_audio_channels;

extern const Sint16 k_audio_bytes_per_sample;

extern SDL_Texture *g_texture;
extern void *g_pixel_buffer;
extern Uint32 g_pixel_buffer_height;
extern Uint32 g_pixel_buffer_width;
extern const Uint32 k_bytes_per_pixel;

extern SDL_AudioSpec g_desired_audio_spec, g_audio_spec;
extern SDL_AudioDeviceID g_audio_device;
extern char *g_audio_device_name;

extern SDL_GameController *g_game_controller;
extern int g_num_game_controllers;
extern int EVT_RIGHT;
extern int EVT_LEFT;
extern int EVT_UP;
extern int EVT_DOWN;
extern int EVT_BTNA;
extern int EVT_BTNB;
extern int EVT_BTNX;
extern int EVT_BTNY;
extern SDL_Point mouse_position;

extern const char *arrow[];

#endif //HERO_GLOBALS_H
