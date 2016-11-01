//
// Created by Christian Hamm on 26.04.15.
//

#pragma once

#define HERO_DYNLOAD

void Hero_DebugDrawWeirdGradient(SDL_Surface *surface, int xoffset,
                                 int yoffset);

void Hero_DebugPlayTestSquareWave(Hero_AudioDef audio_def, const Uint32 tone_hz,
                                  const Sint16 tone_volume);


void Hero_DebugPlayTestSineWave(Hero_AudioDef audio_def, const Uint32 tone_hz,
                                const Sint16 tone_volume);

void Hero_DebugDrawRectangle(SDL_Surface *buffer,
                             Hero_Dimensions dims,
                             Hero_Color colors);

