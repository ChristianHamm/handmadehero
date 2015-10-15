//
// Created by Christian Hamm on 26.04.15.
//

#ifndef HERO_DEBUG_STUFF_H
#define HERO_DEBUG_STUFF_H

void Hero_DebugDrawWeirdGradient(SDL_Surface *surface, int xoffset,
                                 int yoffset);

void Hero_DebugPlayTestSound(Hero_AudioDef audio_def);

void Hero_RenderPlayer(SDL_Surface *buffer, int playerx, int playery);

#endif //HERO_DEBUG_STUFF_H
