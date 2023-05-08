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
        next.changePosition();
        next.velocity = velocity + a * dt;
        next.acceleration = a;
        return next;
    }
player_c player_c::changePosition() {
    player_c next = *this;

        // 15 left -6 + 6
        // 15 right -5 + 5
        vec2d temp;
        int i=0;
    int maks = i +15;
    for(i;i<=15;i++)
        {
            double j = -6;
            temp = {15.0,j};
            next.colissionMap[i] = next.position+temp;
            j++;
        }
    maks = i +15;
    for( i;i<=maks;i++)
    {
        double j = -6;
        temp = {-15.0,j};
        next.colissionMap[i] = next.position+temp;
        j++;
    }
    maks = i +30;
    for( i;i<=maks;i++)
    {
        double j = -15;
        temp = {-j,-6};
        next.colissionMap[i] = next.position+temp;
        j++;
    }
    maks = i +30;
    for( i;i<=maks;i++)
    {
        double j = -15;
        temp = {-j,6};
        next.colissionMap[i] = next.position+temp;
        j++;
    }
    return next;
}
