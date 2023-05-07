//
// Created by wikto on 7/05/2023.
//

#include "playerTwo_C.h"


playerTwo_C playerTwo_C::next_state(double dt_ms) {
    double dt = dt_ms / 1000.0;
    playerTwo_C next = *this;
    const double C = 0.1;
    vec2d friction = {0.0,0.0};
    if (len(velocity) > 0) {
        friction = velocity*len(velocity)*C;
    }
    auto a = acceleration - friction;
    next.position = position + velocity * dt + (a * dt * dt) / 2;
/*    if(next.position[0]<=80)
    {
        next.position[0]=520;
    }*/
    next.velocity = velocity + a * dt;
    next.acceleration = a;
    return next;
}
bool playerTwo_C::checkstate() {

    playerTwo_C next = *this;

    if(next.position[0]<=80)
    {
        return true;
    }

    return false;
}