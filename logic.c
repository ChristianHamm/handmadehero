//
// Created by Christian Hamm on 15.10.15.
//

#include "hero.h"
#include <smmintrin.h>

#define TILE_CHUNK_COUNT_X 256
#define TILE_CHUNK_COUNT_Y 256

void Hero_UpdateGameState(Hero_GameState *game_state,
                          Hero_GameInput *game_input,
                          SDL_Surface *buffer) {

    // Create the tile maps
    Uint32 temp_tiles[TILE_CHUNK_COUNT_Y][TILE_CHUNK_COUNT_X] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Hero_TileChunk tile_chunk = {.tiles = (Uint32 *) temp_tiles};

    // NOTE(casey): This is set to using 256x256 tile chunks.
    Hero_World world = {
            .chunk_shift = 8,
            .chunk_mask = (1u << world.chunk_shift) - 1u,
            .chunk_dim = 256,
            .tile_chunk_count_x = 1,
            .tile_chunk_count_y = 1,
            .tile_side_in_meters = 1.4f,
            .tile_side_in_pixels = 60,
            .meters_to_pixels = (float) world.tile_side_in_pixels
                                / world.tile_side_in_meters,
            .tile_chunks = &tile_chunk
    };

    float player_height = world.tile_side_in_meters;
    float player_width = 0.75f * player_height;

    // Handle movement
    float dt = game_input->frame_dt * player_width * 0.1f;

    // Calculate the new player position from the old state
    Hero_WorldPosition new_player_pos = game_state->player_position;
    float new_player_x = new_player_pos.tile_rel_x;
    float new_player_y = new_player_pos.tile_rel_y;

    if ((game_input->right && game_input->down)
        || (game_input->right && game_input->up)
        || (game_input->left && game_input->down)
        || (game_input->left && game_input->up)) {
        new_player_x += 0
                        + dt * game_input->right * 6
                        - dt * game_input->left * 6;

        new_player_y += 0
                        + dt * game_input->down * 6
                        - dt * game_input->up * 6;

    } else {
        new_player_x += 0
                        + dt * game_input->right * 8
                        - dt * game_input->left * 8;

        new_player_y += 0
                        + dt * game_input->down * 8
                        - dt * game_input->up * 8;
    }

    // Update player position
    new_player_pos.tile_rel_x = new_player_x;
    new_player_pos.tile_rel_y = new_player_y;
    new_player_pos = Hero_RecanonicalizePosition(&world, new_player_pos);

    Hero_WorldPosition player_left = new_player_pos;
    Hero_WorldPosition player_right = new_player_pos;
    player_left.tile_rel_x -= 0.5f * player_width;
    player_right.tile_rel_x += 0.5f * player_width;

    player_left = Hero_RecanonicalizePosition(&world, player_left);
    player_right = Hero_RecanonicalizePosition(&world, player_right);

    if (Hero_IsWorldPointEmpty(&world, new_player_pos)
        && Hero_IsWorldPointEmpty(&world, player_left)
        && Hero_IsWorldPointEmpty(&world, player_right)) {
        game_state->player_position = new_player_pos;
    }

    // Clear the screen
    Hero_Dimensions clear_screen_dims = {
            .min_x = 0.0f,
            .min_y = 0.0f,
            .max_x = (float) buffer->w,
            .max_y = (float) buffer->h
    };

    Hero_Color clear_screen_color = {.r = 1.0f, .g = 0.0f, .b = 0.0f};
    Hero_DebugDrawRectangle(buffer, clear_screen_dims, clear_screen_color);

    // Draw the tile map
    float center_x = 0.5f * (float) buffer->w;
    float center_y = 0.5f * (float) buffer->h;

    for (int rel_row = -10; rel_row < 10; ++rel_row) {
        for (int rel_col = -20; rel_col < 20; ++rel_col) {
            float gray = 0.5f;
            Uint32 column = game_state->player_position.abs_tile_x + rel_col;
            Uint32 row = game_state->player_position.abs_tile_y + rel_row;
            Uint32 tile_id = Hero_GetTileChunkValue(&world, column, row);

            if (tile_id == 1)
                gray = 1.0f;

            if ((column == game_state->player_position.abs_tile_x) &&
                (row == game_state->player_position.abs_tile_y))
                gray = 0.0f;

            float min_x =
                    center_x
                    - world.meters_to_pixels
                      * game_state->player_position.tile_rel_x
                    + ((float) rel_col) * world.tile_side_in_pixels;
            float min_y =
                    center_y
                    + world.meters_to_pixels
                      * game_state->player_position.tile_rel_y
                    - ((float) rel_row) * world.tile_side_in_pixels;
            float max_x = min_x + world.tile_side_in_pixels;
            float max_y = min_y - world.tile_side_in_pixels;

            Hero_Dimensions tile_dims = {
                    .min_x = min_x,
                    .min_y = max_y,
                    .max_x = max_x,
                    .max_y = min_y,
            };

            Hero_Color tile_color = {.r = gray, .g = gray, .b = gray};
            Hero_DebugDrawRectangle(buffer, tile_dims, tile_color);
        }
    }

    // Draw the player
    float draw_p_left = center_x

                        - 0.5f * world.meters_to_pixels * player_width;
    float draw_p_top = center_y

                       - world.meters_to_pixels * player_height;

    Hero_Dimensions player_dims = {
            .min_x = draw_p_left,
            .max_y = draw_p_top + player_height * world.meters_to_pixels,
            .max_x = draw_p_left + player_width * world.meters_to_pixels,
            .min_y = draw_p_top
    };

    Hero_Color player_colors = {.r = 1.0f, .g = 1.0f, .b = 0.0f};
    Hero_DebugDrawRectangle(buffer, player_dims, player_colors);
}

inline Uint32 Hero_RoundFloatToUint32(const float x) {
    float r;
    __m128 src = _mm_set_ss(x);
    __m128 dst = _mm_floor_ps(src);
    _mm_store_ss(&r, dst);

    return (Uint32) r;
}

inline Hero_Dimensions Hero_RoundDimensions(Hero_Dimensions dims) {
    float r[4];

    __m128 src = _mm_setr_ps(dims.min_x, dims.min_y, dims.max_x, dims.max_y);
    __m128 dst = _mm_floor_ps(src);
    _mm_store_ps(r, dst);

    Hero_Dimensions rdims = {
            .min_x = r[0],
            .min_y = r[1],
            .max_x = r[2],
            .max_y = r[3],
    };

    return rdims;
}

inline Uint32 Hero_GetRGBColorForFloat(const float red, const float green,
                                       const float blue, const float alpha) {
    // multiply each float with 255 then round down
    __m128 floorSrc = _mm_setr_ps(red, green, blue, alpha);
    __m128 mulSrc = _mm_set_ps1(255.0f);
    __m128 mulDst = _mm_mul_ps(floorSrc, mulSrc);
    __m128 floorDst = _mm_floor_ps(mulDst);

    float r[4];
    _mm_store_ps(r, floorDst);

    return (Uint32) r[0] << 16 | (Uint32) r[1] << 8 | (Uint32) r[2];
}

inline Hero_TileChunkPosition
Hero_GetChunkPositionFor(Hero_World *world,
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
Hero_GetTileChunkValue(Hero_World *world,
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

inline SDL_bool Hero_IsWorldPointEmpty(Hero_World *world,
                                       Hero_WorldPosition can_pos) {
    SDL_bool empty = SDL_FALSE;
    Uint32 TileChunkValue = Hero_GetTileChunkValue(world,
                                                   can_pos.abs_tile_x,
                                                   can_pos.abs_tile_y);
    if (TileChunkValue == 0)
        empty = SDL_TRUE;

    return empty;
}

inline void Hero_RecanonicalizeCoord(Hero_World *world,
                                     Uint32 *tile,
                                     float *tile_rel) {

    Sint32 offset = (Sint32) Hero_RoundFloatToUint32(
            (*tile_rel) / world->tile_side_in_meters);
    *tile += offset;
    *tile_rel -= offset * world->tile_side_in_meters;

    SDL_assert(*tile_rel >= 0);
    SDL_assert(*tile_rel < world->tile_side_in_meters);
}

inline Hero_WorldPosition Hero_RecanonicalizePosition(Hero_World *world,
                                                      const Hero_WorldPosition
                                                      pos) {
    Hero_WorldPosition result = pos;

    Hero_RecanonicalizeCoord(world,
                             &result.abs_tile_x,
                             &result.tile_rel_x);
    Hero_RecanonicalizeCoord(world,
                             &result.abs_tile_y,
                             &result.tile_rel_y);

    return result;
}

inline Uint32 Hero_GetTileValue(Hero_World *world,
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

inline Uint32 Hero_GetTileValueUnchecked(Hero_World *world,
                                         Hero_TileChunk *tile_chunk,
                                         Uint32 tile_x,
                                         Uint32 tile_y) {
    SDL_assert(tile_chunk);
    SDL_assert(tile_x < world->chunk_dim);
    SDL_assert(tile_y < world->chunk_dim);
    Uint32 res = tile_chunk->tiles[tile_y * world->chunk_dim + tile_x];
    return res;
}

inline Hero_TileChunk *Hero_GetTileChunk(Hero_World *world,
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