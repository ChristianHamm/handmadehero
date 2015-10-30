//
// Created by Christian Hamm on 15.10.15.
//

#include "hero.h"
#include <smmintrin.h>

#define TILE_MAP_COUNT_X 17
#define TILE_MAP_COUNT_Y 9

void Hero_UpdateGameState(Hero_GameState *game_state,
                          Hero_GameInput *game_input,
                          SDL_Surface *buffer) {
    float dt = game_input->frame_dt * 1.7f;

    float new_player_x = game_state->player_x;
    float new_player_y = game_state->player_y;

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

    Uint32 tiles00[TILE_MAP_COUNT_Y][TILE_MAP_COUNT_X] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
            {1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Uint32 tiles01[TILE_MAP_COUNT_Y][TILE_MAP_COUNT_X] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Uint32 tiles10[TILE_MAP_COUNT_Y][TILE_MAP_COUNT_X] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Uint32 tiles11[TILE_MAP_COUNT_Y][TILE_MAP_COUNT_X] = {
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
    world.tilemap_count_x = 2;
    world.tilemap_count_y = 2;
    world.upper_left_x = -30;
    world.upper_left_y = 0;
    world.tile_width = 60;
    world.tile_height = 60;
    world.count_x = TILE_MAP_COUNT_X;
    world.count_y = TILE_MAP_COUNT_Y;

    Hero_TileMap tile_maps[2][2];
    tile_maps[0][0].tiles = (Uint32 *) tiles00;
    tile_maps[0][1].tiles = (Uint32 *) tiles10;
    tile_maps[1][0].tiles = (Uint32 *) tiles01;
    tile_maps[1][1].tiles = (Uint32 *) tiles11;

    world.tile_maps = (Hero_TileMap *) tile_maps;

    Hero_Color player_colors = {.r = 1.0f, .g = 1.0f, .b = 0.0f};

    float player_width = 0.75f * world.tile_width;
    float player_height = world.tile_height;

    Hero_TileMap *tile_map = Hero_GetTileMap(&world,
                                             game_state->tile_map_count_x,
                                             game_state->tile_map_count_y);
    SDL_assert(tile_map);

    Hero_WorldPosition player_pos = {
            .tilemap_x = game_state->tile_map_count_x,
            .tilemap_y = game_state->tile_map_count_y,
            .tile_x = 0,
            .tile_y =0,
            .x = new_player_x,
            .y = new_player_y
    };

    Hero_WorldPosition player_left = player_pos;
    Hero_WorldPosition player_right = player_pos;
    player_left.x -= 0.5f * player_width;
    player_right.x += 0.5f * player_width;

    if (Hero_IsWorldPointEmpty(&world, player_pos)
        && Hero_IsWorldPointEmpty(&world, player_left)
        && Hero_IsWorldPointEmpty(&world, player_right)) {
        Hero_WorldPosition can_pos = Hero_GetCannonicalLocation(&world,
                                                                player_pos);

        game_state->tile_map_count_x = can_pos.tilemap_x;
        game_state->tile_map_count_y = can_pos.tilemap_y;

        game_state->player_x = world.upper_left_x
                               + world.tile_width * can_pos.tile_x
                               + can_pos.x;
        game_state->player_y = world.upper_left_y
                               + world.tile_height * can_pos.tile_y
                               + can_pos.y;
    }

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
    for (int row = 0; row < TILE_MAP_COUNT_Y; ++row) {
        for (int column = 0; column < TILE_MAP_COUNT_X; ++column) {
            float gray = 0.5f;

            Uint32 tile_id = Hero_GetTileValueUnchecked(&world, tile_map,
                                                        column, row);
            if (tile_id == 1)
                gray = 1.0f;

            Hero_Dimensions tile_dims = {
                    .min_x = world.upper_left_x +
                             ((float) column) * world.tile_width,
                    .min_y = world.upper_left_y +
                             ((float) row) * world.tile_height,
                    .max_x = tile_dims.min_x + world.tile_width,
                    .max_y = tile_dims.min_y + world.tile_height
            };

            Hero_Color tile_color = {.r = gray, .g = gray, .b = gray};
            Hero_DebugDrawRectangle(buffer, tile_dims, tile_color);
        }
    }

    // Draw the player
    Hero_Dimensions player_dims = {
            .min_x = game_state->player_x - 0.5f * player_width,
            .min_y = game_state->player_y - player_height,
            .max_x = game_state->player_x - 0.5f * player_width + player_width,
            .max_y = game_state->player_y - player_height + player_height
    };

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
                                       Hero_WorldPosition test_pos) {
    SDL_bool valid;
    Hero_WorldPosition can_pos = Hero_GetCannonicalLocation(world, test_pos);

    Hero_TileMap *tile_map = Hero_GetTileMap(world,
                                             can_pos.tilemap_x,
                                             can_pos.tilemap_y);

    valid = Hero_IsTileMapPointEmpty(world, tile_map,
                                     can_pos.tile_x,
                                     can_pos.tile_y);
    return valid;
}

inline Hero_WorldPosition Hero_GetCannonicalLocation(Hero_World *world,
                                                     Hero_WorldPosition test_pos) {
    Hero_WorldPosition result;
    result.tilemap_x = test_pos.tilemap_x;
    result.tilemap_y = test_pos.tilemap_y;

    float x = test_pos.x - world->upper_left_x;
    float y = test_pos.y - world->upper_left_y;

    result.tile_x = (Sint32) Hero_RoundFloatToUint32(x / world->tile_width);
    result.tile_y = (Sint32) Hero_RoundFloatToUint32(y / world->tile_height);

    result.x = x - result.tile_x * world->tile_width;
    result.y = y - result.tile_y * world->tile_height;

    SDL_assert(result.x >= 0);
    SDL_assert(result.y >= 0);
    SDL_assert(result.x < world->tile_width);
    SDL_assert(result.y < world->tile_height);

    if (result.tile_x < 0) {
        result.tile_x += world->count_x;
        --result.tilemap_x;
    }

    if (result.tile_y < 0) {
        result.tile_y += world->count_y;
        --result.tilemap_y;
    }

    if (result.tile_x >= world->count_x) {
        result.tile_x -= world->count_x;
        ++result.tilemap_x;
    }

    if (result.tile_y >= world->count_y) {
        result.tile_y -= world->count_y;
        ++result.tilemap_y;
    }

    return result;
}

inline SDL_bool Hero_IsTileMapPointEmpty(Hero_World *world,
                                         Hero_TileMap *tile_map,
                                         Sint32 test_tile_x,
                                         Sint32 test_tile_y) {
    SDL_bool valid = SDL_FALSE;

    if (tile_map) {
        if ((test_tile_x >= 0) && (test_tile_x < world->count_x)
            && (test_tile_y >= 0) && (test_tile_y < world->count_y)) {
            Uint32 tile_map_value = Hero_GetTileValueUnchecked(world,
                                                               tile_map,
                                                               test_tile_x,
                                                               test_tile_y);
            if (tile_map_value == 0)
                valid = SDL_TRUE;
        }
    }

    return valid;
}

inline Uint32 Hero_GetTileValueUnchecked(Hero_World *world,
                                         Hero_TileMap *tile_map,
                                         Sint32 tile_x,
                                         Sint32 tile_y) {
    SDL_assert(tile_map);
    SDL_assert((tile_x >= 0) && (tile_x < world->count_x) &&
               (tile_y >= 0) && (tile_y < world->count_y));
    return tile_map->tiles[tile_y * world->count_x + tile_x];
}

inline Hero_TileMap *Hero_GetTileMap(Hero_World *world,
                                     Sint32 tile_map_x,
                                     Sint32 tile_map_y) {
    Hero_TileMap *tile_map = 0;

    if ((tile_map_x >= 0) && (tile_map_x < world->tilemap_count_x) &&
        (tile_map_y >= 0) && (tile_map_y < world->tilemap_count_y)) {
        tile_map = &world->tile_maps[tile_map_y * world->tilemap_count_x +
                                     tile_map_x];
    }

    return tile_map;
}