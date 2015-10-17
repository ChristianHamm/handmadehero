//
// Created by Christian Hamm on 26.04.15.
//

#ifndef HERO_DEBUG_STUFF_H
#define HERO_DEBUG_STUFF_H

void Hero_DebugDrawWeirdGradient(SDL_Surface *surface, int xoffset,
                                 int yoffset);

void Hero_DebugPlayTestSound(Hero_AudioDef audio_def);

void Hero_DebugRenderPlayer(SDL_Surface *buffer, int playerx, int playery);

void Hero_DebugDrawRectangle(SDL_Surface *buffer,
                             float fmin_x, float fmin_y,
                             float fmax_x, float fmax_y,
                             float red,
                             float green,
                             float blue);

#endif //HERO_DEBUG_STUFF_H
