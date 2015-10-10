//
// Created by Christian Hamm on 26.04.15.
//

#include "hero.h"

void Hero_DebugDrawWeirdGradient(SDL_Surface *surface, int xoffset, int yoffset) {
    SDL_LockSurface(surface);
    Uint8 *row = (Uint8 *) surface->pixels;

    for (int y = 0; y < surface->h; ++y) {
        Uint32 *pixel = (Uint32 *) row;
        for (int x = 0; x < surface->w; ++x) {
            /*
            +0 +1 +2 +3
            Pixel in memory: 00 00 00 00
            BB GG RR xx
            */

            Uint8 blue = (x + xoffset);
            Uint8 green = (y + yoffset);

            *pixel++ = ((green << 8) | blue);
        }
        row += surface->pitch;
    }

    SDL_UnlockSurface(surface);
}

void Hero_PlayTestSound(Hero_AudioDef audio_def) {
    // Retain the position where we are in the audio test loop
    static Uint32 audio_step = 0;
    Uint32 tone_hz = 220;
    Sint16 tone_volume = 3000;
    Uint32 square_wave_period = audio_def.audio_freq / tone_hz;
    Uint32 half_square_wave_period = square_wave_period / 2;

    Uint32 target_queue_bytes =
            (Uint32) (audio_def.audio_freq *
                      audio_def.audio_bytes_per_sample);
    Uint32 bytes_to_write =
            target_queue_bytes -
            SDL_GetQueuedAudioSize(audio_def.audio_device);

    if (bytes_to_write) {
        void *sound_buffer = malloc(bytes_to_write);
        Sint16 *sample_out = (Sint16 *) sound_buffer;
        Uint32 sample_count =
                bytes_to_write / audio_def.audio_bytes_per_sample;

        for (Uint32 i = 0; i < sample_count; ++i) {
            Sint16 sample_value = ((audio_step++ /
                                    half_square_wave_period) % 2)
                                  ? tone_volume : -tone_volume;
            *sample_out++ = sample_value;
            *sample_out++ = sample_value;
        }
        SDL_QueueAudio(audio_def.audio_device, sound_buffer,
                       bytes_to_write);
        free(sound_buffer);
    }
}


