//
// Created by Christian Hamm on 15.10.15.
//

#include "hero.h"

void Hero_UpdateGameState(Hero_GameState *game_state,
                          Hero_GameInput *game_input,
                          SDL_Surface *buffer) {
    if ((game_input->right && game_input->down)
        || (game_input->right && game_input->up)
        || (game_input->left && game_input->down)
        || (game_input->left && game_input->up)) {
        game_state->player_x +=
                0 +
                game_input->right * 6 -
                game_input->left * 6;

        game_state->player_y +=
                0 +
                game_input->down * 6 -
                game_input->up * 6;

    } else {
        game_state->player_x +=
                0 +
                game_input->right * 8 -
                game_input->left * 8;

        game_state->player_y +=
                0 +
                game_input->down * 8 -
                game_input->up * 8;
    }

    game_state->gradient_xoffset -= 1 * game_input->left;
    game_state->gradient_xoffset += 1 * game_input->right;
    game_state->gradient_yoffset -= 1 * game_input->up;
    game_state->gradient_yoffset += 1 * game_input->down;


    Hero_DebugDrawRectangle(buffer, 0.0f, 0.0f, (float) buffer->h,
                            (float) buffer->w, 0x0);
    Hero_DebugDrawRectangle(buffer,
                            40.0f + (float)game_state->gradient_yoffset,
                            40.0f + (float)game_state->gradient_yoffset,
                            50.0f,
                            50.0f,
                            0x000000FF);
    Hero_DebugDrawRectangle(buffer,
                            40.0f,
                            50.0f,
                            60.0f + (float)game_state->gradient_yoffset,
                            50.0f + (float)game_state->gradient_yoffset,
                            0x00FF0000);
    Hero_DebugDrawRectangle(buffer,
                            60.0f - (float)game_state->gradient_yoffset,
                            60.0f + (float)game_state->gradient_yoffset,
                            100.0f,
                            100.0f,
                            0x0000FF00);
    //Hero_DebugDrawWeirdGradient(buffer, game_state->gradient_xoffset,
    //                            game_state->gradient_yoffset);

}

Sint32 Hero_RoundFloatToInt32(float x) {
    return (Sint32) (x + 0.5f);
}

