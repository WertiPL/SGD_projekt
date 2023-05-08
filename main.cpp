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
bool checkIfWin(player_c &player,playerTwo_C *player2,int players2num)
{
    if(players2num >0)
    {
        for(int i=0;i<=players2num;i++)
        {
            //  std::cout<<"Player1:"<<round(player.colissionMap[0][1])<<":"<<round(player.colissionMap[0][1])<<std::endl;
            //  std::cout<<"Player2:"<<round(player2[i].colissionMap[0][1])<<":"<<round(player2[i].colissionMap[0][1])<<std::endl;
            for (int j=0;j<90;j++)
            {
                for(int k=0;k<90;k++)
                {

                    player_c tempPlayer = player;
                    if(round(player.colissionMap[k][0]) == round(player2[i].colissionMap[j][0]) &&
                       round(player.colissionMap[k][1]) == round(player2[i].colissionMap[j][1]))
                    {
                        return true;
                    }
                }

            }
        }
    }

    return false;
}

void play_the_game(SDL_Renderer *renderer) {
    auto street_texture = load_texture(renderer, "street.bmp");
    auto player_texture = load_texture(renderer, "car.bmp");

    SDL_Rect player_rect = get_texture_rect(player_texture);
    SDL_Rect street_rect = get_texture_rect(street_texture);
    auto player2_texture1 = load_texture(renderer, "bot.bmp");

    int limitOfbots=4;

    SDL_Rect *two_rect = new SDL_Rect[limitOfbots];


    player_c player = {M_PI/2, {123.0, 200.0}};
    player.changePosition();
    double playerTwoDirection = M_PI;
    playerTwo_C *player2 = new playerTwo_C[8];
    vec2d standard_resp[3] = {{520.0,200.0},{420.0, 240.0},{320.0, 350.0}};
    int countOfbots= 0;
    int lastTypeOfbots= 0;

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

                    if (e.key.keysym.sym == SDLK_s)
                    {

                        //std::cout<<"Pressed"<<std::endl;
                        if(countOfbots <= limitOfbots)
                        {
                            //create new car in Game
                            if(lastTypeOfbots == 2)
                            {
                                two_rect[countOfbots] = get_texture_rect(player2_texture1);
                                player2[countOfbots] = {playerTwoDirection,standard_resp[lastTypeOfbots]};
                                //player2[countOfbots].changePosition();
                            }
                            else if (lastTypeOfbots == 1) {
                                two_rect[countOfbots] = get_texture_rect(player2_texture1);
                                player2[countOfbots] = {playerTwoDirection,standard_resp[lastTypeOfbots]};
                                //player2[countOfbots].changePosition();

                            }
                            else if (lastTypeOfbots == 0) {
                                two_rect[countOfbots] = get_texture_rect(player2_texture1);
                                player2[countOfbots] = {playerTwoDirection,standard_resp[lastTypeOfbots]};
                                //player2[countOfbots].changePosition();
                            }
                            lastTypeOfbots++;
                            if(lastTypeOfbots==3)
                            {
                                lastTypeOfbots=0;
                            }


                            countOfbots++;
                            if(countOfbots==limitOfbots)
                            {
                                countOfbots=limitOfbots;
                            }
                        }
                        else
                        {
                            for(int i=0;i<countOfbots;i++ )
                            {
                                if(player2[i].finished == true)
                                {
                                    if(lastTypeOfbots == 2)
                                    {
                                        two_rect[i] = get_texture_rect(player2_texture1);
                                        player2[i] = {playerTwoDirection,standard_resp[lastTypeOfbots]};
                                    }
                                    else if (lastTypeOfbots == 1) {
                                        two_rect[i] = get_texture_rect(player2_texture1);
                                        player2[i] = {playerTwoDirection,standard_resp[lastTypeOfbots]};

                                    }
                                    else if (lastTypeOfbots == 0) {
                                        two_rect[i] = get_texture_rect(player2_texture1);
                                        player2[i] = {playerTwoDirection,standard_resp[lastTypeOfbots]};

                                    }
                                    lastTypeOfbots++;
                                    if(lastTypeOfbots==3)
                                    {
                                        lastTypeOfbots=0;
                                    }
                                }
                            }
                            std::cout<<"Limit number of Bots achieved"<<std::endl;

                        }
                    }
                    break;
            }
        }




        player.acceleration = acceleration_vector_from_keyboard_and_player(player);
        player = player.next_state(TICK_TIME);
        player = player.changePosition();


        {
            auto rect = player_rect;


            rect.x = player.position[0] - rect.w / 2;
            rect.y = player.position[1] - rect.h / 2;

            SDL_RenderCopy(renderer, street_texture.get(), nullptr, nullptr);

            SDL_RenderCopyEx(renderer, player_texture.get(),
                             nullptr, &rect,  player.angle,
                             nullptr, SDL_FLIP_NONE);


        }

        //changing position of Player 2
        for(int i=0;i<countOfbots;i++)
        {
            player2[i].acceleration = acceleration_vector_from_keyboard_and_player2(player2[i]);
            player2[i]=player2[i].next_state(TICK_TIME);
            player2[i]=player2[i].changePosition();

        }

        //rendering position of Player 2
        {


            SDL_Rect *copyTwo_rect = new SDL_Rect[limitOfbots];
            for(int i=0;i<countOfbots;i++)
            {
                copyTwo_rect[i]= two_rect[i];

                copyTwo_rect[i].x = player2[i].position[0] - copyTwo_rect[i].w / 3;
                copyTwo_rect[i].y = player2[i].position[1] - copyTwo_rect[i].h / 3;
                if(!player2[i].checkState())
                {
                    SDL_RenderCopyEx(renderer, player2_texture1.get(),
                                     nullptr, &copyTwo_rect[i], player2[i].angle,
                                     nullptr, SDL_FLIP_NONE);

                }else
                {
                    player2[i].finished = true;
                }

            }
            if(checkIfWin(player,player2,countOfbots))
            {
                std::cout<<"Player2 Win"<<std::endl;
            }
            else
            {
                //std::cout<<"Player1 Win"<<std::endl;
            }
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