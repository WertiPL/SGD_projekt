//
// Created by wikto on 7/05/2023.
//
#include <memory>
#include <array>
#include <cmath>
#ifndef SGD_PROJEKT_VEC2D_H
#define SGD_PROJEKT_VEC2D_H


using vec2d = std::array<double, 2>;

vec2d operator+(vec2d a, vec2d b);

vec2d operator-(vec2d a, vec2d b);

vec2d operator*(vec2d a, double b);

vec2d operator/(vec2d a, double b);
double len(vec2d v);


#endif //SGD_PROJEKT_VEC2D_H
