//
// Created by wikto on 7/05/2023.
//
#include <memory>
#include <array>
#include <cmath>
#include "player_c.h"




player_c player_c::next_state(double dt_ms) {
        double dt = dt_ms / 1000.0;
        player_c next = *this;
        const double C = 0.1;
        vec2d friction = {0.0,0.0};
        if (len(velocity) > 0) {
            friction = velocity*len(velocity)*C;
        }
        auto a = acceleration - friction;
        next.position = position + velocity * dt + (a * dt * dt) / 2;
        if(next.position[1]<=120)
        {
            next.position[1]=120;
        }
        else if(next.position[1]>=400)
        {
            next.position[1]=400;
        }
        next.velocity = velocity + a * dt;
        next.acceleration = a;
        return next;
    }
player_c player_c::changePosition() {
    player_c next = *this;
    double haft_width=32;
    double haftHeight= 26 / 2;
        // 15 left -6 + 6
        // 15 right -5 + 5
        vec2d temp;
        int i=0;
    double j = -haftHeight;
//right
    int maks = i + (haftHeight*2);
    for(i;i<maks;i++)
        {
            temp = {haft_width,j};
            next.colissionMap[i][0] = next.position[0]+temp[0];
            next.colissionMap[i][1] = next.position[1]+temp[1];

            j++;
        }
    //left

    maks = i +(haftHeight*2);
    j = -haftHeight;

    for( i;i<maks;i++)
    {
        temp = {-haft_width,j};
        next.colissionMap[i][0] = next.position[0]+temp[0];
        next.colissionMap[i][1] = next.position[1]+temp[1];

        j++;
    }


    maks = i +(haft_width*2);
    j = -haft_width;

    for( i;i<maks;i++)
    {
        temp = {j,-haftHeight};
        next.colissionMap[i][0] = next.position[0]+temp[0];
        next.colissionMap[i][1] = next.position[1]+temp[1];
        j++;
    }
    maks = i +(haft_width*2);
    j = -haft_width;

    for( i;i<maks;i++)
    {
        temp = {j, haftHeight};
        next.colissionMap[i][0] = next.position[0]+temp[0];
        next.colissionMap[i][1] = next.position[1]+temp[1];
        j++;
    }
    return next;
}
