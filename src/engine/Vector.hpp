/*
** Agartha-Software, 2023
** Cevy
** File description:
** position
*/

#pragma once

namespace cevy {
    class Vector {
        public:
            Vector(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {};
            ~Vector() {};
            double x = 0;
            double y = 0;
            double z = 0;
            operator Vector3 () const {return((Vector3) {x, y, z});}
    };
}
