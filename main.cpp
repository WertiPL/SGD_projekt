#include <iostream>
#include <memory>
#include <array>
#include <cmath>

#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "players/player_c.h"
#include "players/playerTwo_C.h"
#include "engine/physics.h"

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

bool checkIfWin(player_c &player,playerTwo_C *player2,int players2num)
{
    if(players2num >0)
    {
        for(int i=0;i<=players2num;i++)
        {
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
    auto player_texture_win1 = load_texture(renderer, "win1.bmp");
    auto player_texture_win2 = load_texture(renderer, "win2.bmp");

    SDL_Rect player_rect = get_texture_rect(player_texture);
    SDL_Rect street_rect = get_texture_rect(street_texture);
    SDL_Rect win1 = get_texture_rect(player_texture_win1);
    SDL_Rect win2 = get_texture_rect(player_texture_win2);
    win1.x=0;
    win1.y=0;
    win2.x=0;
    win2.y=0;

    auto player2_texture1 = load_texture(renderer, "bot.bmp");

    int limitOfbots=4;
    int time=0;
    int win=60000;

    SDL_Rect *two_rect = new SDL_Rect[limitOfbots+1];
    physics physics;

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
                        if(countOfbots <= limitOfbots)
                        {
                            //create new car in Game
                            if(lastTypeOfbots == 2)
                            {
                                two_rect[countOfbots] = get_texture_rect(player2_texture1);
                                player2[countOfbots] = {playerTwoDirection,standard_resp[lastTypeOfbots]};
                            }
                            else if (lastTypeOfbots == 1) {
                                two_rect[countOfbots] = get_texture_rect(player2_texture1);
                                player2[countOfbots] = {playerTwoDirection,standard_resp[lastTypeOfbots]};
                            }
                            else if (lastTypeOfbots == 0) {
                                two_rect[countOfbots] = get_texture_rect(player2_texture1);
                                player2[countOfbots] = {playerTwoDirection,standard_resp[lastTypeOfbots]};
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




        player.acceleration = physics.acceleration_vector_from_keyboard_and_player(player);
        player = player.next_state(TICK_TIME);
        player = player.changePosition();


        {
            auto rect = player_rect;
            rect.x = player.position[0] - rect.w / 2;
            rect.y = player.position[1] - rect.h / 2;
            SDL_RenderCopy(renderer, street_texture.get(), nullptr, nullptr);
            SDL_RenderCopyEx(renderer, player_texture.get(),nullptr, &rect,  player.angle,nullptr, SDL_FLIP_NONE);
        }

        //changing position of Player 2
        for(int i=0;i<countOfbots;i++)
        {
            player2[i].acceleration = physics.acceleration_vector_from_keyboard_and_player2(player2[i]);
            player2[i]=player2[i].next_state(TICK_TIME);
            player2[i]=player2[i].changePosition();

        }
        bool winGame = false;

        //rendering position of Player 2
        {


            SDL_Rect *copyTwo_rect = new SDL_Rect[limitOfbots+1];
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
                winGame = true;
                    if(winGame == true)
                    {
                        SDL_RenderCopy(renderer,player_texture_win2.get(), nullptr, &win2);

                    }
            }
            else
            {
                if(time > win)
                {
                    std::cout<<"Player1 Win"<<std::endl;
                    winGame = true;
                    if(winGame == true)
                    {
                        SDL_RenderCopy(renderer,player_texture_win1.get(), nullptr, &win1);
                    }
                }
                else
                {
                    time+=33;
                }
            }
        }
        SDL_RenderPresent(renderer);
        int current_tick = SDL_GetTicks();
        SDL_Delay(TICK_TIME - (current_tick - prev_tick));
        prev_tick += TICK_TIME;

        if(winGame == true)
        {
            SDL_Delay(1000);
            gaming = false;
            return;
        }
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