//
// Created by Christian Hamm on 26.04.15.
//

#ifndef HERO_DEBUG_STUFF_H
#define HERO_DEBUG_STUFF_H

void Hero_DebugDrawWeirdGradient(SDL_Surface *surface, int xoffset,
                                 int yoffset);

void Hero_DebugPlayTestSound(Hero_AudioDef audio_def);

void Hero_DebugDrawRectangle(SDL_Surface *buffer,
                             Hero_Dimensions dims,
                             Hero_Color colors);

#endif //HERO_DEBUG_STUFF_H
