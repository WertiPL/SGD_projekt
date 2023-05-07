#include <iostream>
#include <memory>
#include <array>
#include <cmath>

#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "player_c.h"
#include "playerTwo_C.h"

const int TICK_TIME = 33;

std::shared_ptr<SDL_Texture> load_texture(SDL_Renderer *renderer, std::string fname) {
    SDL_Surface *bmp = SDL_LoadBMP(("data/" + fname).c_str());
    if (!bmp) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Couldn't create surface from image: %s",SDL_GetError());
        throw std::invalid_argument(SDL_GetError());
    }

    SDL_SetColorKey(bmp, SDL_TRUE, 0x0ff00ff);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bmp);

    if (!texture) {
        throw std::invalid_argument("Could not create texture");
    }
    SDL_FreeSurface(bmp);
    return std::shared_ptr<SDL_Texture>(texture, [](SDL_Texture *tex) {
        std::cout << "Texture destroyed!" << std::endl;
        SDL_DestroyTexture(tex);
    });
}

std::pair<int, int> get_texture_w_h(std::shared_ptr<SDL_Texture> texture) {
    int w, h;
    SDL_QueryTexture(texture.get(), nullptr, nullptr, &w, &h);
    return {w, h};
}

SDL_Rect get_texture_rect(std::shared_ptr<SDL_Texture> texture) {
    auto [w, h] = get_texture_w_h(texture);
    return {0, 0, w, h};
}



//to delete
vec2d angle_to_vector(double angle) {
    return {std::cos(angle), std::sin(angle)};
}

vec2d acceleration_vector_from_keyboard_and_player(const player_c &player) {
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

vec2d acceleration_vector_from_keyboard_and_player2(const playerTwo_C &player) {
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

void play_the_game(SDL_Renderer *renderer) {
    auto street_texture = load_texture(renderer, "street.bmp");
    auto player_texture = load_texture(renderer, "car.bmp");

    SDL_Rect player_rect = get_texture_rect(player_texture);
    SDL_Rect street_rect = get_texture_rect(street_texture);
    auto player2_texture1 = load_texture(renderer, "bot.bmp");
    auto player2_texture2 = load_texture(renderer, "bot.bmp");
    auto player2_texture3 = load_texture(renderer, "bot.bmp");

    SDL_Rect two1_rect = get_texture_rect(player2_texture1);
    SDL_Rect two2_rect = get_texture_rect(player2_texture2);
    SDL_Rect two3_rect = get_texture_rect(player2_texture3);

    player_c player = {M_PI/2, {120.0, 200.0}};
    playerTwo_C two1 = {M_PI , {520.0, 180.0}};
    playerTwo_C two2 = {M_PI , {420.0, 240.0}};
    playerTwo_C two3 = {M_PI , {320.0, 350.0}};
    int gaming = true;
    auto prev_tick = SDL_GetTicks();
    while (gaming) {
        SDL_Event e;

        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    gaming = false;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_q) gaming = false;
                    break;
            }
        }




        player.acceleration = acceleration_vector_from_keyboard_and_player(player);
        player = player.next_state(TICK_TIME);

        {
            auto rect = player_rect;


            rect.x = player.position[0] - rect.w / 2;
            rect.y = player.position[1] - rect.h / 2;

            SDL_RenderCopy(renderer, street_texture.get(), nullptr, nullptr);

            SDL_RenderCopyEx(renderer, player_texture.get(),
                             nullptr, &rect,  player.angle,
                             nullptr, SDL_FLIP_NONE);


        }


        two1.acceleration = acceleration_vector_from_keyboard_and_player2(two1);
        two1 = two1.next_state(TICK_TIME);
        two2.acceleration = acceleration_vector_from_keyboard_and_player2(two2);
        two2 = two2.next_state(TICK_TIME);
        two3.acceleration = acceleration_vector_from_keyboard_and_player2(two3);
        two3 = two3.next_state(TICK_TIME);


        {
            auto two1Rect = two1_rect;
            two1Rect.x = two1.position[0] - two1Rect.w / 3;
            two1Rect.y = two1.position[1] - two1Rect.h / 3;
            SDL_RenderCopyEx(renderer, player2_texture1.get(),
                             nullptr, &two1Rect, two1.angle,
                             nullptr, SDL_FLIP_NONE);
        }
        {
            auto two2Rect = two2_rect;
            two2Rect.x = two2.position[0] - two2Rect.w / 3;
            two2Rect.y = two2.position[1] - two2Rect.h / 3;
            SDL_RenderCopyEx(renderer, player2_texture2.get(),
                             nullptr, &two2Rect, two2.angle,
                             nullptr, SDL_FLIP_NONE);
        }
        {
            auto two3Rect = two3_rect;
            two3Rect.x = two3.position[0] - two3Rect.w / 3;
            two3Rect.y = two3.position[1] - two3Rect.h / 3;
            SDL_RenderCopyEx(renderer, player2_texture3.get(),
                             nullptr, &two3Rect, two3.angle,
                             nullptr, SDL_FLIP_NONE);
        }
        SDL_RenderPresent(renderer);
        int current_tick = SDL_GetTicks();
        SDL_Delay(TICK_TIME - (current_tick - prev_tick));
        prev_tick += TICK_TIME;
    }
}

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_CreateWindowAndRenderer(640, 480,
                                SDL_WINDOW_SHOWN,
                                &window, &renderer);

    play_the_game(renderer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}