//
// Created by Christian Hamm on 15.10.15.
//

#include "hero.h"
#include <smmintrin.h>

#define TILEMAP_COUNT_X 17
#define TILEMAP_COUNT_Y 9

void Hero_UpdateGameState(Hero_GameState *game_state,
                          Hero_GameInput *game_input,
                          SDL_Surface *buffer) {
    // Handle movement
    float dt = game_input->frame_dt * 0.075f;

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

    new_player_pos.tile_rel_x = new_player_x;
    new_player_pos.tile_rel_y = new_player_y;

    // Create the tile maps
    Uint32 tiles00[TILEMAP_COUNT_Y][TILEMAP_COUNT_X] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
            {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Uint32 tiles01[TILEMAP_COUNT_Y][TILEMAP_COUNT_X] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
            {1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
            {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Uint32 tiles10[TILEMAP_COUNT_Y][TILEMAP_COUNT_X] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
            {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Uint32 tiles11[TILEMAP_COUNT_Y][TILEMAP_COUNT_X] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
            {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Hero_World world;
    world.tile_side_in_meters = 1.4f;
    world.tile_side_in_pixels = 60;
    world.meters_to_pixels = (float) world.tile_side_in_pixels
                             / world.tile_side_in_meters;
    world.tilemap_count_x = 2;
    world.tilemap_count_y = 2;
    world.upper_left_x = -30;
    world.upper_left_y = 0;
    world.count_x = TILEMAP_COUNT_X;
    world.count_y = TILEMAP_COUNT_Y;

    Hero_TileMap tilemaps[2][2];
    tilemaps[0][0].tiles = (Uint32 *) tiles00;
    tilemaps[0][1].tiles = (Uint32 *) tiles10;
    tilemaps[1][0].tiles = (Uint32 *) tiles01;
    tilemaps[1][1].tiles = (Uint32 *) tiles11;

    world.tilemaps = (Hero_TileMap *) tilemaps;

    float player_width = 0.75f * world.tile_side_in_meters;
    float player_height = world.tile_side_in_meters;

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

    Hero_TileMap *tilemap = Hero_GetTileMap(&world,
                                            game_state->player_position.tilemap_x,
                                            game_state->player_position.tilemap_y);
    SDL_assert(tilemap);

    // Clear the screen
    /*
    Hero_Dimensions clear_screen_dims = {
            .min_x = 0.0f,
            .min_y = 0.0f,
            .max_x = (float) buffer->w,
            .max_y = (float) buffer->h
    };

    Hero_Color clear_screen_color = {.r = 1.0f, .g = 0.0f, .b = 0.0f};
    Hero_DebugDrawRectangle(buffer, clear_screen_dims, clear_screen_color);
    */

    // Draw the tile map
    for (int row = 0; row < TILEMAP_COUNT_Y; ++row) {
        for (int column = 0; column < TILEMAP_COUNT_X; ++column) {
            float gray = 0.5f;

            Uint32 tile_id = Hero_GetTileValueUnchecked(&world, tilemap,
                                                        column, row);
            if (tile_id == 1)
                gray = 1.0f;

            if ((column == game_state->player_position.tile_x) &&
                (row == game_state->player_position.tile_y))
                gray = 0.0f;

            Hero_Dimensions tile_dims = {
                    .min_x = world.upper_left_x +
                             ((float) column) * world.tile_side_in_pixels,
                    .min_y = world.upper_left_y +
                             ((float) row) * world.tile_side_in_pixels,
                    .max_x = tile_dims.min_x + world.tile_side_in_pixels,
                    .max_y = tile_dims.min_y + world.tile_side_in_pixels
            };

            Hero_Color tile_color = {.r = gray, .g = gray, .b = gray};
            Hero_DebugDrawRectangle(buffer, tile_dims, tile_color);
        }
    }

    // Draw the player
    float draw_p_left = world.upper_left_x
                        + world.tile_side_in_pixels
                          * game_state->player_position.tile_x
                        + world.meters_to_pixels
                          * game_state->player_position.tile_rel_x
                        - 0.5f * world.meters_to_pixels * player_width;
    float draw_p_top = world.upper_left_y
                       + world.tile_side_in_pixels
                         * game_state->player_position.tile_y
                       + world.meters_to_pixels
                         * game_state->player_position.tile_rel_y
                       - world.meters_to_pixels * player_height;

    Hero_Dimensions player_dims = {
            .min_x = draw_p_left,
            .min_y = draw_p_top,
            .max_x = draw_p_left + player_width * world.meters_to_pixels,
            .max_y = draw_p_top + player_height * world.meters_to_pixels
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

inline SDL_bool Hero_IsWorldPointEmpty(Hero_World *world,
                                       Hero_WorldPosition can_pos) {
    SDL_bool valid;

    Hero_TileMap *tilemap = Hero_GetTileMap(world,
                                            can_pos.tilemap_x,
                                            can_pos.tilemap_y);

    valid = Hero_IsTileMapPointEmpty(world, tilemap,
                                     can_pos.tile_x,
                                     can_pos.tile_y);
    return valid;
}

inline void Hero_CanonicalizeCoord(Hero_World *world,
                                   Sint32 tile_count,
                                   Sint32 *tilemap,
                                   Sint32 *tile,
                                   float *tile_rel) {

    Sint32 offset = (Sint32) Hero_RoundFloatToUint32(
            (*tile_rel) / world->tile_side_in_meters);
    *tile += offset;
    *tile_rel -= offset * world->tile_side_in_meters;

    SDL_assert(*tile_rel >= 0);
    SDL_assert(*tile_rel < world->tile_side_in_meters);

    if (*tile < 0) {
        *tile = tile_count + *tile;
        --*tilemap;
    }

    if (*tile >= tile_count) {
        *tile = *tile - tile_count;
        ++*tilemap;
    }
}

inline Hero_WorldPosition Hero_RecanonicalizePosition(Hero_World *world,
                                                      const Hero_WorldPosition
                                                      pos) {
    Hero_WorldPosition result = pos;

    Hero_CanonicalizeCoord(world, world->count_x,
                           &result.tilemap_x,
                           &result.tile_x,
                           &result.tile_rel_x);
    Hero_CanonicalizeCoord(world, world->count_y,
                           &result.tilemap_y,
                           &result.tile_y,
                           &result.tile_rel_y);

    return result;
}

inline SDL_bool Hero_IsTileMapPointEmpty(Hero_World *world,
                                         Hero_TileMap *tilemap,
                                         Sint32 test_tile_x,
                                         Sint32 test_tile_y) {
    SDL_bool valid = SDL_FALSE;

    if (tilemap) {
        if ((test_tile_x >= 0) && (test_tile_x < world->count_x)
            && (test_tile_y >= 0) && (test_tile_y < world->count_y)) {
            Uint32 tilemap_value = Hero_GetTileValueUnchecked(world,
                                                              tilemap,
                                                              test_tile_x,
                                                              test_tile_y);
            if (tilemap_value == 0)
                valid = SDL_TRUE;
        }
    }

    return valid;
}

inline Uint32 Hero_GetTileValueUnchecked(Hero_World *world,
                                         Hero_TileMap *tilemap,
                                         Sint32 tile_x,
                                         Sint32 tile_y) {
    SDL_assert(tilemap);
    SDL_assert((tile_x >= 0) && (tile_x < world->count_x) &&
               (tile_y >= 0) && (tile_y < world->count_y));
    return tilemap->tiles[tile_y * world->count_x + tile_x];
}

inline Hero_TileMap *Hero_GetTileMap(Hero_World *world,
                                     Sint32 tilemap_x,
                                     Sint32 tilemap_y) {
    Hero_TileMap *tilemap = 0;

    if ((tilemap_x >= 0) && (tilemap_x < world->tilemap_count_x) &&
        (tilemap_y >= 0) && (tilemap_y < world->tilemap_count_y)) {
        tilemap = &world->tilemaps[tilemap_y * world->tilemap_count_x +
                                   tilemap_x];
    }

    return tilemap;
}