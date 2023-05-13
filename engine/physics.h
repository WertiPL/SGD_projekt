//
// Created by wikto on 13/05/2023.
//

#ifndef SGD_PROJEKT_PHYSICS_H
#define SGD_PROJEKT_PHYSICS_H


#include "vec2d.h"
#include "../players/player_c.h"
#include "../players/playerTwo_C.h"

class physics {
public:
    vec2d angle_to_vector(double angle);



    vec2d acceleration_vector_from_keyboard_and_player2(const playerTwo_C &player);


    vec2d acceleration_vector_from_keyboard_and_player(const player_c &player);
};


#endif //SGD_PROJEKT_PHYSICS_H
