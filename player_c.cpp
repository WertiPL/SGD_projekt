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
