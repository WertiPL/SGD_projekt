//
// Created by wikto on 7/05/2023.
//

#include "vec2d.h"
using vec2d = std::array<double, 2>;

vec2d operator+(vec2d a, vec2d b) {
    return {a[0] + b[0], a[1] + b[1]};
}

vec2d operator-(vec2d a, vec2d b) {
    return {a[0] - b[0], a[1] - b[1]};
}

vec2d operator*(vec2d a, double b) {
    return {a[0] * b, a[1] * b};
}

vec2d operator/(vec2d a, double b) {
    return a * (1.0 / b);
}
double len(vec2d v) {
    return std::sqrt(v[0]*v[0] + v[1]*v[1]);
}