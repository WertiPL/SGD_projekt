//
// Created by wikto on 7/05/2023.
//

#ifndef SGD_PROJEKT_PLAYERTWO_C_H
#define SGD_PROJEKT_PLAYERTWO_C_H


#include "../engine/vec2d.h"

class playerTwo_C
{
public:
    double angle;
    vec2d position;
    vec2d velocity;
    vec2d acceleration;
    bool finished;
    playerTwo_C next_state(double dt_ms);
    bool checkState();
    vec2d colissionMap[193];
    playerTwo_C changePosition();

};


#endif //SGD_PROJEKT_PLAYERTWO_C_H
