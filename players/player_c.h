//
// Created by wikto on 7/05/2023.
//

#ifndef SGD_PROJEKT_PLAYER_C_H
#define SGD_PROJEKT_PLAYER_C_H

#include "../engine/vec2d.h"

class player_c
{
public:
double angle;
vec2d position;
vec2d velocity;
vec2d acceleration;
vec2d colissionMap[181];
player_c changePosition();
player_c next_state(double dt_ms);
};
#endif //SGD_PROJEKT_PLAYER_C_H
