#include <iostream>
#include <memory>
#include <array>
#include <cmath>
#include <map>

#define SDL_MAIN_HANDLED

#include <SDL.h>

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
class car_c {
public:
    vec2d position;
    vec2d  acceleration
    vec2d  speed;

    car_c next_state(double dt_ms) {
        double dt = dt_ms / 1000.0;
        car_c next = *this;
        const double C = 0.1;
        vec2d friction = {0.0,0.0};
        if (len(velocity) > 0) {
            friction = velocity*len(velocity)*C;
        }
        auto a = acceleration - friction;
        next.position = position + velocity * dt + (a * dt * dt) / 2;
        next.velocity = velocity + a * dt;
        next.acceleration = a;
    }
};
class player_c {
public:
    double angle;
    //double speed;
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
class carBot_c{
public:
    carBot_c() {

    }

    int positionX;
    int positionY;
    int weight=64;
    int height=64;

    carBot_c(int positionY){
        this->positionY=positionY;
        this->positionX = 600;
    }

    carBot_c increaseSpeed(){
        carBot_c carbot = *this;
        carbot.positionX-=1;
       return carbot;
    }


};
class street_c{
public:
    int numberOfBots;
    static const int maxNumBots=10;
    int lastAddedBot;
    int acceleration;
    carBot_c *bots = new carBot_c;

    street_c addBot1(int num,street_c street_object){
        carBot_c bot1(60);
        street_object.bots[num] = bot1;
        street_object.numberOfBots++;
        street_object.lastAddedBot=1;
        return street_object;
    }
    street_c addBot2(int num,street_c street_object){
        carBot_c bot(120);
        street_object.bots[num] = bot;
        street_object.numberOfBots++;
        street_object.lastAddedBot=2;
        return street_object;
    }
    street_c addBot3(int num,street_c street_object){
        carBot_c bot(180);
        street_object.bots[numberOfBots] = bot;
        street_object.numberOfBots++;
        street_object.lastAddedBot=3;
        return street_object;
    }
      street_c deletebot(int num,street_c street_object){
        if(street_object.bots[num].positionX == 10)
        {
            if(street_object.lastAddedBot==1) addBot1(num,street_object);
            if(street_object.lastAddedBot==2) addBot2(num,street_object);
            if(street_object.lastAddedBot==3) addBot3(num,street_object);
        }
    }


};

int increaseSpeedOfStreet(const street_c &street) {
    auto *keyboard_state = SDL_GetKeyboardState(nullptr);
    int acc = street.acceleration;
   // if (keyboard_state[SDL_SCANCODE_UP]) acc -= 1;
   // if (keyboard_state[ SDL_SCANCODE_DOWN]) acc += 1;
    return acc;
}
//to delete
vec2d angle_to_vector(double angle) {
    return {std::cos(angle), std::sin(angle)};
}

vec2d changePosition(const car_c &car) {
    auto *keyboard_state = SDL_GetKeyboardState(nullptr);
    vec2d forward_vec = angle_to_vector(car.angle);
    vec2d acceleration = 0;
    vec2d car_speed = car.speed;
    if (keyboard_state[SDL_SCANCODE_DOWN]) {
        acceleration = acceleration + car_speed;
    }
    if (keyboard_state[SDL_SCANCODE_UP]) {
        acceleration = acceleration - car_speed;
    }
    return acceleration*200.0;
}


vec2d acceleration_vector_from_keyboard_and_player(const player_c &player) {
    auto *keyboard_state = SDL_GetKeyboardState(nullptr);
    vec2d forward_vec = angle_to_vector(player.angle);
    vec2d acceleration = {0, 0};
    if (keyboard_state[SDL_SCANCODE_LEFT]) {
        acceleration = acceleration + forward_vec;
    }
    if (keyboard_state[SDL_SCANCODE_RIGHT]) {
        acceleration = acceleration - forward_vec;
    }
    return acceleration*200.0;
}

double angle_from_keyboard_and_player(const player_c &player) {
    auto *keyboard_state = SDL_GetKeyboardState(nullptr);
    double angle = player.angle;
    if (keyboard_state[SDL_SCANCODE_LEFT]) angle -= angle;
    if (keyboard_state[SDL_SCANCODE_RIGHT]) angle += angle;
    return angle;
}

void play_the_game(SDL_Renderer *renderer) {
    auto street_texture = load_texture(renderer, "street.bmp");
    auto clouds_texture = load_texture(renderer, "clouds.bmp");
    //auto player_texture = load_texture(renderer, "car.bmp");
    auto car_texture = load_texture(renderer, "car.bmp");
    auto bot_texture = load_texture(renderer, "bot.bmp");

    //SDL_Rect player_rect = get_texture_rect(player_texture);
    SDL_Rect street_rect = get_texture_rect(street_texture);
    SDL_Rect car_rect = get_texture_rect(street_texture);
    SDL_Rect clouds_rect = get_texture_rect(clouds_texture);
    SDL_Rect bot_rect = get_texture_rect(bot_texture);

    //player_c player = {0, {320.0, 200.0}};
    street_c streetOfbot;

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

        /*player.acceleration = acceleration_vector_from_keyboard_and_player(player);
        player.angle = angle_from_keyboard_and_player(player);
        player = player.next_state(TICK_TIME);
*/
// Solid background Color
//        SDL_SetRenderDrawColor(renderer, 1, 40, 128, 255);
//        SDL_RenderClear(renderer);




        {
            auto rect = car_rect;


            rect.x = car. - rect.w / 2;
            rect.y = player.position[1] - rect.h / 2;

            SDL_RenderCopy(renderer, street_texture.get(), nullptr, nullptr);
            SDL_RenderCopy(renderer, car_texture.get(), nullptr, &rect);
          //  SDL_RenderCopyEx(renderer, player_texture.get(),
            //                 nullptr, &rect, 180.0 * player.angle / M_PI,
            //                 nullptr, SDL_FLIP_NONE);
            //{
               // streetOfbot.addBot1(0,streetOfbot);
                //SDL_RenderCopy(renderer, street_texture.get(), nullptr, {rect.x = });

                //miejsce generowania botow SDL_RenderCopy

            //}
           // clouds_rect = {200,200};
           // SDL_RenderCopy(renderer, clouds_texture.get(), nullptr, &clouds_rect);
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