//
// Created by Christian Hamm on 26.04.15.
//

#ifndef HERO_PLATFORM_H
#define HERO_PLATFORM_H

Hero_AudioDef Hero_InitAudio();

SDL_Cursor *Hero_InitSystemCursor(const char **image);

void Hero_UpdateGameState(Hero_GameState *game_state,
                          Hero_GameInput *game_input);

void Hero_UpdateGraphics(SDL_Renderer *renderer);

void Hero_InitControllers();

int Hero_HandleEvents(Hero_GameInput *game_input);

void Hero_PrintSDLVersion();

#endif //HERO_PLATFORM_H
