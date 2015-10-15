//
// Created by Christian Hamm on 15.10.15.
//

#ifndef HERO_LOGIC_H
#define HERO_LOGIC_H

void Hero_UpdateGameState(Hero_GameState *game_state,
                          Hero_GameInput *game_input,
                          SDL_Surface *buffer);

void Hero_DrawRectangle(SDL_Surface *buffer, float minx, float miny, float maxy,
                        float maxx);

#endif //HERO_LOGIC_H
