#include <iostream>
#include <memory>
#include <array>
#include <cmath>

#define SDL_MAIN_HANDLED

#include <SDL.h>

const int TICK_TIME = 33;

std::shared_ptr<SDL_Texture> load_texture(SDL_Renderer *renderer, std::string fname) {
    SDL_Surface *bmp = SDL_LoadBMP(("assets/" + fname).c_str());
    if (!bmp) {
        throw std::invalid_argument("Could not load bmp");
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

using vec2d = std::array<double, 2>;

vec2d operator+(vec2d a, vec2d b) {
    return {a[0] + b[0], a[1] + b[1]};
}

vec2d operator-(vec2d a, vec2d b) {
    return {a[0] - b[0], a[1] - b[1]};
}

vec2d operator*(vec2d a, double b) {
    return {a[0] * b, a[1] * b};
}

vec2d operator/(vec2d a, double b) {
    return a * (1.0 / b);
}
double len(vec2d v) {
    return std::sqrt(v[0]*v[0] + v[1]*v[1]);
}
class player_c {
public:
    double angle;
    vec2d position;
    vec2d velocity;
    vec2d acceleration;

    player_c next_state(double dt_ms) {
        double dt = dt_ms / 1000.0;
        player_c next = *this;
        const double C = 0.1;
        vec2d friction = {0.0,0.0};
        if (len(velocity) > 0) {
            friction = velocity*len(velocity)*C;
        }
        auto a = acceleration - friction;
        next.position = position + velocity * dt + (a * dt * dt) / 2;
        next.velocity = velocity + a * dt;
        next.acceleration = a;




        return next;
    }
};

vec2d angle_to_vector(double angle) {
    return {std::cos(angle), std::sin(angle)};
}

vec2d acceleration_vector_from_keyboard_and_player(const player_c &player) {
    auto *keyboard_state = SDL_GetKeyboardState(nullptr);
    vec2d forward_vec = angle_to_vector(player.angle);
    vec2d acceleration = {0, 0};
    if (keyboard_state[SDL_SCANCODE_UP]) {
        acceleration = acceleration + forward_vec;
    }
    if (keyboard_state[SDL_SCANCODE_DOWN]) {
        acceleration = acceleration - forward_vec;
    }
    return acceleration*200.0;
}

double angle_from_keyboard_and_player(const player_c &player) {
    auto *keyboard_state = SDL_GetKeyboardState(nullptr);
    double angle = player.angle;
    if (keyboard_state[SDL_SCANCODE_LEFT]) angle = angle - M_PI / 10.0;
    if (keyboard_state[SDL_SCANCODE_RIGHT]) angle = angle + M_PI / 10.0;
    return angle;
}

void play_the_game(SDL_Renderer *renderer) {
    auto player_texture = load_texture(renderer, "player1.bmp");
    SDL_Rect player_rect = get_texture_rect(player_texture);
    player_c player = {0, {320.0, 200.0}};
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
        player.angle = angle_from_keyboard_and_player(player);
        player = player.next_state(TICK_TIME);


        SDL_SetRenderDrawColor(renderer, 1, 40, 128, 255);
        SDL_RenderClear(renderer);

        //SDL_RenderCopy(renderer, player_texture.get(), nullptr, &player_rect);
        {
            auto rect = player_rect;

            rect.x = player.position[0] - rect.w / 2;
            rect.y = player.position[1] - rect.h / 2;
            SDL_RenderCopyEx(renderer, player_texture.get(),
                             nullptr, &rect, 180.0 * player.angle / M_PI,
                             nullptr, SDL_FLIP_NONE);
        }
        SDL_RenderPresent(renderer);
        auto current_tick = SDL_GetTicks();
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