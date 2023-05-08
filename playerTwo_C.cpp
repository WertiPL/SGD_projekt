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

    next.changePosition();
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

playerTwo_C playerTwo_C::changePosition() {
    playerTwo_C next = *this;

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