//
// Created by Christian Hamm on 26.04.15.
//

#include "globals.h"

void *g_logic_lib;

const Uint32 k_window_width = 960;
const Uint32 k_window_height = 540;
const double k_display_msmax = 1000.0 / 60;    // 30 FPS

const Uint16 k_audio_freq = 48000;
const Uint16 k_audio_rate = 60;
const Uint16 k_audio_channels = 2;

const Sint16 k_audio_bytes_per_sample = sizeof(Sint16) * 2;

Uint32 g_current_screen_height;
Uint32 g_current_screen_width;
const Uint32 k_bytes_per_pixel = (Uint32) sizeof(Uint32);

SDL_AudioSpec g_desired_audio_spec, g_audio_spec;
SDL_AudioDeviceID g_audio_device;
char *g_audio_device_name;

SDL_GameController *g_game_controller = NULL;
int g_num_game_controllers = 0;

SDL_Point g_mouse_position;
SDL_Surface *g_backbuffer = NULL;

const char *arrow[] = {
        /* width height num_colors chars_per_pixel */
        "    32    32        3            1",
        /* colors */
        "X c #000000",
        ". c #ffffff",
        "  c None",
        /* pixels */
        "                X               ",
        "                X.              ",
        "                X.              ",
        "                X.              ",
        "                X.              ",
        "                X.              ",
        "                 .              ",
        "         XXXXXX   XXXXXX        ",
        "          ......   ......       ",
        "                X.              ",
        "                X.              ",
        "                X.              ",
        "                X.              ",
        "                X.              ",
        "                X.              ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "0,0"
};
