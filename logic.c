//
// Created by Christian Hamm on 15.10.15.
//

#include <stdbool.h>
#include "hero.h"

#define TILE_MAP_HEIGHT 9
#define TILE_MAP_WIDTH 17

void Hero_UpdateGameState(Hero_GameState *game_state,
                          Hero_GameInput *game_input,
                          SDL_Surface *buffer) {
    float dt = game_input->frame_dt;

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

    Uint32 tiles00[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {
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

    Uint32 tiles01[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {
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

    Uint32 tiles10[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {
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

    Uint32 tiles11[TILE_MAP_HEIGHT][TILE_MAP_WIDTH] = {
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

    Hero_TileMap tile_maps[2][2];

    tile_maps[0][0].tiles = (Uint32 *) tiles00;

    tile_maps[0][0].upper_left_x = -30;
    tile_maps[0][0].upper_left_y = -30;
    tile_maps[0][0].tile_width = 60;
    tile_maps[0][0].tile_height = 60;
    tile_maps[0][0].count_x = TILE_MAP_WIDTH;
    tile_maps[0][0].count_y = TILE_MAP_HEIGHT;

    tile_maps[0][1] = tile_maps[0][0];
    tile_maps[0][1].tiles = (Uint32 *) tiles01;

    tile_maps[1][0] = tile_maps[0][0];
    tile_maps[1][0].tiles = (Uint32 *) tiles10;

    tile_maps[1][1] = tile_maps[0][0];
    tile_maps[1][1].tiles = (Uint32 *) tiles11;

    Hero_TileMap *tile_map = &tile_maps[0][0];

    float player_red = 1.0f;
    float player_green = 1.0f;
    float player_blue = 0.0f;
    float player_width = 0.75f * tile_map->tile_width;
    float player_height = tile_map->tile_height;

    if (Hero_IsTileMapPointEmpty(tile_map, new_player_x, new_player_y)
        && Hero_IsTileMapPointEmpty(tile_map,
                                    new_player_x - 0.5f * player_width,
                                    new_player_y)
        && Hero_IsTileMapPointEmpty(tile_map,
                                    new_player_x + 0.5f * player_width,
                                    new_player_y)
            ) {
        game_state->player_x = new_player_x;
        game_state->player_y = new_player_y;
    }

    float player_left = game_state->player_x - 0.5f * player_width;
    float player_top = game_state->player_y - player_height;

    // Clear the screen
    Hero_DebugDrawRectangle(buffer, 0.0f, 0.0f, (float) buffer->w,
                            (float) buffer->h, 1.0, 0.0, 0.0);

    // Draw the tile map
    for (int row = 0; row < TILE_MAP_HEIGHT; ++row) {
        for (int column = 0; column < TILE_MAP_WIDTH; ++column) {
            Uint32 tile_id = Hero_GetTileValueUnchecked(tile_map, column, row);
            float gray = 0.5f;

            if (tile_id == 1)
                gray = 1.0f;

            float min_x = tile_map->upper_left_x
                          + ((float) column) * tile_map->tile_width;
            float min_y = tile_map->upper_left_y +
                          ((float) row) * tile_map->tile_height;
            float max_x = min_x + tile_map->tile_width;
            float max_y = min_y + tile_map->tile_height;

            Hero_DebugDrawRectangle(buffer, min_x, min_y, max_x, max_y, gray,
                                    gray, gray);
        }
    }

    // Draw the player
    Hero_DebugDrawRectangle(buffer,
                            player_left,
                            player_top,
                            player_left + player_width,
                            player_top + player_height,
                            player_red, player_green, player_blue
    );


}

Sint32 Hero_RoundFloatToSint32(float x) {
    return (Sint32) (x + 0.5f);
}

Uint32 Hero_RoundFloatToUint32(float x) {
    return (Uint32) (x + 0.5f);
}

SDL_bool Hero_IsTileMapPointEmpty(Hero_TileMap *tile_map,
                                  float test_x, float test_y) {
    SDL_bool valid = SDL_FALSE;
    Sint32 player_tile_x = (Sint32) ((test_x - tile_map->upper_left_x) /
                                     tile_map->tile_width);
    Sint32 player_tile_y = (Sint32) ((test_y - tile_map->upper_left_y) /
                                     tile_map->tile_height);

    if ((player_tile_x >= 0) && (player_tile_x < tile_map->count_x)
        && (player_tile_y >= 0) && (player_tile_y < tile_map->count_y)) {
        Uint32 tile_map_value = Hero_GetTileValueUnchecked(tile_map,
                                                           player_tile_x,
                                                           player_tile_y);
        if (tile_map_value == 0)
            valid = SDL_TRUE;
    }

    return valid;
}

Uint32 Hero_GetTileValueUnchecked(Hero_TileMap *tile_map, Sint32 tile_x,
                                  Sint32 tile_y) {
    return tile_map->tiles[tile_y * tile_map->count_x + tile_x];
}

Hero_TileMap *Hero_GetTileMap(Hero_World *world, Sint32 tile_map_x, Sint32
tile_map_y) {
    Hero_TileMap *tile_map = 0;

    if ((tile_map_x >= 0) && (tile_map_x < world->count_x) &&
        (tile_map_y >= 0) && (tile_map_y < world->count_y)) {
        tile_map = &world->tile_maps[tile_map_y * world->count_x +
                                     tile_map_x];
    }

    return tile_map;
}