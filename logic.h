//
// Created by Christian Hamm on 15.10.15.
//

#pragma once

void Hero_UpdateGameState(Hero_GameState *game_state,
                          Hero_GameInput *game_input,
                          SDL_Surface *buffer,
                          Hero_AudioDef audio_def);

Uint32 Hero_RoundFloatToUint32(const float x);

Hero_Dimensions Hero_RoundDimensions(Hero_Dimensions dims);

Uint32 Hero_GetRGBColorForFloat(const float red, const float green, const
float blue, const float alpha);


