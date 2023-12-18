/*
** Agartha-Software, 2023
** Cevy
** File description:
** rotation
*/

#include <math.h>
#include <vector>
#include <array>
#include <iostream>
#include "raymath.h"

#pragma once

namespace cevy {
    class Rotation {
        private:
        public:
            Rotation(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {};
            ~Rotation() {};
            double x = 0;
            double y = 0;
            double z = 0;
            Rotation fowards() {
                return(Rotation(0.0, 1.0, 0.0));
            };
            Rotation to_rad() {return Rotation(x * (M_PI / 180.0), y * (M_PI / 180.0), z * (M_PI / 180.0));};
            operator Vector3 () const {return((Vector3) {x, y, z});}
    };
}
