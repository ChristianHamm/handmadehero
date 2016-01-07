//
// Created by Christian Hamm on 08.01.16.
//

#ifndef HERO_TILE_H
#define HERO_TILE_H

#include "globals.h"

typedef struct _Hero_TileChunk {
    Uint32 *tiles;
} Hero_TileChunk;

typedef struct _Hero_Map {
    Uint32 chunk_shift;
    Uint32 chunk_mask;
    Uint32 chunk_dim;

    float tile_side_in_meters;
    float meters_to_pixels;
    Sint32 tile_side_in_pixels;

    // TODO(casey): Beginner's sparseness
    Sint32 tile_chunk_count_x;
    Sint32 tile_chunk_count_y;

    Hero_TileChunk *tile_chunks;
} Hero_Map;

typedef struct _Hero_TileChunkPosition {
    Uint32 tile_chunk_x;
    Uint32 tile_chunk_y;

    Uint32 rel_tile_x;
    Uint32 rel_tile_y;
} Hero_TileChunkPosition;

// canonical_position from Casey
typedef struct _Hero_TilemapPosition {
    // These are fixed point tile locations.
    // The ghigh bits are the tile chunk index,
    // and the low bits are the tile index in the chunk
    Uint32 abs_tile_x;
    Uint32 abs_tile_y;
    float tile_rel_x;
    float tile_rel_y;
} Hero_TilemapPosition;


void Hero_RecanonicalizeCoord(Hero_Map *world,
                              Uint32 *tile,
                              float *tile_rel);

Hero_TilemapPosition Hero_RecanonicalizePosition(Hero_Map *world,
                                                 Hero_TilemapPosition pos);

SDL_bool Hero_IsWorldPointEmpty(Hero_Map *world,
                                Hero_TilemapPosition test_pos);


Uint32 Hero_GetTileValueUnchecked(Hero_Map *world,
                                  Hero_TileChunk *tile_chunk,
                                  Uint32 tile_x,
                                  Uint32 tile_y);

Uint32 Hero_GetTileChunkValue(Hero_Map *world,
                              Uint32 abs_tile_x,
                              Uint32 abs_tile_y);

Hero_TileChunk *Hero_GetTileChunk(Hero_Map *world, Sint32 tilemap_x,
                                  Sint32 tilemap_y);

Uint32 Hero_GetTileValue(Hero_Map *world,
                         Hero_TileChunk *tile_chunk,
                         Uint32 test_tile_x,
                         Uint32 test_tile_y);

Hero_TileChunkPosition Hero_GetChunkPositionFor(Hero_Map *world,
                                                Uint32 abs_tile_x,
                                                Uint32 abs_tile_y);


#endif //HERO_TILE_H
