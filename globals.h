//
// Created by Christian Hamm on 26.04.15.
//

#ifndef HERO_GLOBALS_H
#define HERO_GLOBALS_H

#include <SDL.h>

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

    float frame_dt;
} Hero_GameInput;

// TODO(cha) I don't know if we need this
typedef struct _Hero_GameMemory {
    Uint64 perm_store_size;
    void *perm_store;

} Hero_GameMemory;

typedef struct _Hero_GameClocks {

} Hero_GameClocks;

typedef struct _Hero_RecordedInput {
    int input_count;
    Hero_GameInput *input_stream;
} Hero_RecordedInput;

typedef struct _Hero_TileMap {
    Uint32 *tiles;
} Hero_TileMap;

typedef struct _Hero_World {
    float tile_side_in_meters;
    float meters_to_pixels;
    Sint32 tile_side_in_pixels;
    Sint32 tilemap_count_x;
    Sint32 tilemap_count_y;
    Sint32 count_x;
    Sint32 count_y;
    float upper_left_x;
    float upper_left_y;
    Hero_TileMap *tilemaps;
} Hero_World;

// canonical_position from Casey
typedef struct _Hero_WorldPosition {
#if 1
    Sint32 tilemap_x;
    Sint32 tilemap_y;
    Sint32 tile_x;
    Sint32 tile_y;
#else
    Uint32 _tile_x;
    Uint32 _tile_y;
#endif
    float tile_rel_x;  // TileRelX
    float tile_rel_y;  // TileRelY
} Hero_WorldPosition;

typedef struct _Hero_GameState {
    Hero_WorldPosition player_position;
} Hero_GameState;

typedef struct _Hero_Color {
    float r;
    float g;
    float b;
} Hero_Color;

typedef struct _Hero_Dimensions {
    float min_x;
    float min_y;
    float max_x;
    float max_y;
} Hero_Dimensions;

extern SDL_Surface *g_backbuffer;

extern SDL_Point g_mouse_position;

extern const char *arrow[];

#endif //HERO_GLOBALS_H
