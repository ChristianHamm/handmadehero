//
// Created by Christian Hamm on 15.10.15.
//

#ifndef HERO_LOGIC_H
#define HERO_LOGIC_H

void Hero_UpdateGameState(Hero_GameState *game_state,
                          Hero_GameInput *game_input,
                          SDL_Surface *buffer);

Uint32 Hero_RoundFloatToUint32(const float x);

Hero_Dimensions Hero_RoundDimensions(Hero_Dimensions dims);

Uint32 Hero_GetRGBColorForFloat(const float red, const float green, const
float blue, const float alpha);

SDL_bool Hero_IsTileMapPointEmpty(Hero_TileMap *tile_map,
                                  float test_x, float test_y);

Uint32 Hero_GetTileValueUnchecked(Hero_TileMap *tile_map, Sint32 tile_x,
                                  Sint32 tile_y);

#endif //HERO_LOGIC_H
