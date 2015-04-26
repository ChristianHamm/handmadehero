//
// Created by Christian Hamm on 26.04.15.
//

#ifndef HERO_PLATFORM_H
#define HERO_PLATFORM_H

void Hero_InitAudio();

void Hero_PutPixel(void *pixel_buffer, int pitch,
                   const Uint32 x, const Uint32 y, const Uint32 color);

SDL_Cursor *Hero_InitSystemCursor(const char **image);

#endif //HERO_PLATFORM_H
