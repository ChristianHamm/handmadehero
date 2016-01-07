//
// Created by Christian Hamm on 08.01.16.
//

#include "globals.h"
#include "logic.h"

inline Hero_TileChunkPosition
Hero_GetChunkPositionFor(Hero_Map *world,
                         Uint32 abs_tile_x,
                         Uint32 abs_tile_y) {
    Hero_TileChunkPosition result;

    result.tile_chunk_x = abs_tile_x >> world->chunk_shift;
    result.tile_chunk_y = abs_tile_y >> world->chunk_shift;
    result.rel_tile_x = abs_tile_x & world->chunk_mask;
    result.rel_tile_y = abs_tile_y & world->chunk_mask;

    return result;
}

inline Uint32
Hero_GetTileChunkValue(Hero_Map *world,
                       Uint32 abs_tile_x,
                       Uint32 abs_tile_y) {
    Hero_TileChunkPosition chunk_pos = Hero_GetChunkPositionFor(world,
                                                                abs_tile_x,
                                                                abs_tile_y);
    Hero_TileChunk *tilemap = Hero_GetTileChunk(world,
                                                chunk_pos.tile_chunk_x,
                                                chunk_pos.tile_chunk_y);
    Uint32 tile_chunk_value = Hero_GetTileValue(world,
                                                tilemap,
                                                chunk_pos.rel_tile_x,
                                                chunk_pos.rel_tile_y);

    return tile_chunk_value;
}

inline SDL_bool Hero_IsWorldPointEmpty(Hero_Map *world,
                                       Hero_TilemapPosition can_pos) {
    SDL_bool empty = SDL_FALSE;
    Uint32 TileChunkValue = Hero_GetTileChunkValue(world,
                                                   can_pos.abs_tile_x,
                                                   can_pos.abs_tile_y);
    if (TileChunkValue == 0)
        empty = SDL_TRUE;

    return empty;
}

inline void Hero_RecanonicalizeCoord(Hero_Map *world,
                                     Uint32 *tile,
                                     float *tile_rel) {

    Sint32 offset = (Sint32) Hero_RoundFloatToUint32(
            (*tile_rel) / world->tile_side_in_meters);
    *tile += offset;
    *tile_rel -= offset * world->tile_side_in_meters;

    SDL_assert(*tile_rel >= 0);
    SDL_assert(*tile_rel < world->tile_side_in_meters);
}

inline Hero_TilemapPosition Hero_RecanonicalizePosition(Hero_Map *world,
                                                        const Hero_TilemapPosition
                                                        pos) {
    Hero_TilemapPosition result = pos;

    Hero_RecanonicalizeCoord(world,
                             &result.abs_tile_x,
                             &result.tile_rel_x);
    Hero_RecanonicalizeCoord(world,
                             &result.abs_tile_y,
                             &result.tile_rel_y);

    return result;
}

inline Uint32 Hero_GetTileValue(Hero_Map *world,
                                Hero_TileChunk *tile_chunk,
                                Uint32 test_tile_x,
                                Uint32 test_tile_y) {
    Uint32 res = 0;

    if (tile_chunk)
        res = Hero_GetTileValueUnchecked(world,
                                         tile_chunk,
                                         test_tile_x,
                                         test_tile_y);

    return res;
}

inline Uint32 Hero_GetTileValueUnchecked(Hero_Map *world,
                                         Hero_TileChunk *tile_chunk,
                                         Uint32 tile_x,
                                         Uint32 tile_y) {
    SDL_assert(tile_chunk);
    SDL_assert(tile_x < world->chunk_dim);
    SDL_assert(tile_y < world->chunk_dim);
    Uint32 res = tile_chunk->tiles[tile_y * world->chunk_dim + tile_x];
    return res;
}

inline Hero_TileChunk *Hero_GetTileChunk(Hero_Map *world,
                                         Sint32 tile_chunk_x,
                                         Sint32 tile_chunk_y) {
    Hero_TileChunk *tile_chunk = 0;

    if ((tile_chunk_x >= 0) && (tile_chunk_x < world->tile_chunk_count_x) &&
        (tile_chunk_y >= 0) && (tile_chunk_y < world->tile_chunk_count_y)) {
        tile_chunk = &world->tile_chunks[
                tile_chunk_y * world->tile_chunk_count_x +
                tile_chunk_x];
    }

    return tile_chunk;
}