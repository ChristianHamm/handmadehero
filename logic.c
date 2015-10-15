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


}

void Hero_DrawRectangle(SDL_Surface *buffer, float minx, float miny, float maxy,
                        float maxx) {
    /* Handmade Hero Day 027 - 51:33
     * https://youtu.be/Mi98zVBb6Wk?t=51m29s
    int top = playery;
    int bottom = playery + 10;
    Uint32 color = 0xFAFFFFFF;
    Uint8 *end_of_buffer = (Uint8 *) buffer->pixels +
                           buffer->format->BytesPerPixel * buffer->w +
                           buffer->pitch *
                           buffer->h;

    for (int x = playerx; x < playerx + 10; ++x) {
        Uint8 *pixel = ((Uint8 *) buffer->pixels
                        + x * buffer->format->BytesPerPixel
                        + top * buffer->pitch);

        for (int y = top; y < bottom; ++y) {
            if (pixel >= buffer->pixels && pixel < end_of_buffer) {
                *(Uint32 *) pixel = color;
                pixel += buffer->pitch;
            }
        }
    }*/
}