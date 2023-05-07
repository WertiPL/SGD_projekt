//
// Created by wikto on 7/05/2023.
//

#ifndef SGD_PROJEKT_PLAYERTWO_C_H
#define SGD_PROJEKT_PLAYERTWO_C_H


#include "vec2d.h"

class playerTwo_C
{
public:
    double angle;
    vec2d position;
    vec2d velocity;
    vec2d acceleration;
    vec2d *colissionMap = new vec2d[1800];
    bool finished;
    playerTwo_C next_state(double dt_ms);
    bool checkstate();
};


#endif //SGD_PROJEKT_PLAYERTWO_C_H
