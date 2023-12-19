/*
** Agartha-Software, 2023
** Cevy
** File description:
** position
*/

#pragma once
#include "Vector.hpp"

namespace cevy {
    class Position : public Vector {
        public:
            Position(double x = 0, double y = 0, double z = 0) : Vector(x, y, z) {};
            ~Position() {};
    };
}
