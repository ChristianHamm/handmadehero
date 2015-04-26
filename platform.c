//
// Created by Christian Hamm on 26.04.15.
//

#include "hero.h"

void Hero_InitAudio() {
    SDL_zero(g_desired_audio_spec);
    g_desired_audio_spec.freq = k_audio_freq;
    g_desired_audio_spec.format = AUDIO_S16LSB;
    g_desired_audio_spec.channels = k_audio_channels;
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
    }
}

void Hero_PutPixel(void *pixel_buffer, int pitch,
                   const Uint32 x, const Uint32 y, const Uint32 color) {
    Uint8 *pixel = (Uint8 *) pixel_buffer;
    pixel += (y * pitch) + (x * sizeof(Uint32));
    *((Uint32 *) pixel) = color;
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
