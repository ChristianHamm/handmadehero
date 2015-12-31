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

void Hero_RecanonicalizeCoord(Hero_World *world,
                              Uint32 *tile,
                              float *tile_rel);

Hero_WorldPosition Hero_RecanonicalizePosition(Hero_World *world,
                                               Hero_WorldPosition pos);

SDL_bool Hero_IsWorldPointEmpty(Hero_World *world,
                                Hero_WorldPosition test_pos);


Uint32 Hero_GetTileValueUnchecked(Hero_World *world,
                                  Hero_TileChunk *tile_chunk,
                                  Uint32 tile_x,
                                  Uint32 tile_y);

Uint32 Hero_GetTileChunkValue(Hero_World *world,
                              Uint32 abs_tile_x,
                              Uint32 abs_tile_y);

Hero_TileChunk *Hero_GetTileChunk(Hero_World *world, Sint32 tilemap_x,
                                  Sint32 tilemap_y);

Uint32 Hero_GetTileValue(Hero_World *world,
                         Hero_TileChunk *tile_chunk,
                         Uint32 test_tile_x,
                         Uint32 test_tile_y);

Hero_TileChunkPosition Hero_GetChunkPositionFor(Hero_World *world,
                                                Uint32 abs_tile_x,
                                                Uint32 abs_tile_y);

#endif //HERO_LOGIC_H
