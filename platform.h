//
// Created by Christian Hamm on 26.04.15.
//

#pragma once


Hero_AudioDef Hero_InitAudio();

SDL_Cursor *Hero_InitSystemCursor(const char **image);


void Hero_ResizeAndUpdateWindow(SDL_Window *window, SDL_Surface *surface,
                                SDL_bool forceStretch);

void Hero_InitControllers();

SDL_bool Hero_HandleEvents(Hero_GameInput *game_input);

void Hero_PrintSDLVersion();

