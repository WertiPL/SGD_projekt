//
// Created by wikto on 13/05/2023.
//

#include "physics.h"
#include "SDL.h"

vec2d physics::angle_to_vector(double angle) {
    return {std::cos(angle), std::sin(angle)};
}



vec2d physics::acceleration_vector_from_keyboard_and_player2(const playerTwo_C &player) {
    auto *keyboard_state = SDL_GetKeyboardState(nullptr);
    vec2d forward_vec = angle_to_vector(player.angle);
    vec2d acceleration = {0, 0};

    if (keyboard_state[SDL_SCANCODE_A]) {
        acceleration = acceleration + forward_vec;
        acceleration[1] *= 1.0;

    }
    if (keyboard_state[SDL_SCANCODE_D]) {
        acceleration[1] *= 400.0;
    }
    return acceleration* 1200.0;

}

vec2d physics::acceleration_vector_from_keyboard_and_player(const player_c &player) {
    auto *keyboard_state = SDL_GetKeyboardState(nullptr);
    vec2d forward_vec = angle_to_vector(player.angle);
    vec2d acceleration = {0, 0};
    if (keyboard_state[SDL_SCANCODE_DOWN]) {
        acceleration = acceleration + forward_vec;
    }
    if (keyboard_state[SDL_SCANCODE_UP]) {
        acceleration = acceleration - forward_vec;
    }

    return acceleration* 1200.0;
}
