/*
** Agartha-Software, 2023
** Cevy
** File description:
** position
*/

#pragma once
#include <array>

namespace cevy {
    class Vector {
        public:
            Vector(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {};
            ~Vector() {};
            double x = 0;
            double y = 0;
            double z = 0;
            operator Vector3 () const {return((Vector3) {(float)x, (float)y, (float)z});}
            Vector operator -(Vector other) {return (Vector(x - other.x, y - other.y, z - other.z));};
            Vector operator +(Vector other) {return (Vector(x + other.x, y + other.y, z + other.z));};
            Vector operator *(Vector other) {return (Vector(x * other.x, y * other.y, z * other.z));};
            Vector operator /(Vector other) {return (Vector(x / other.x, y / other.y, z / other.z));};
            void operator =(Vector other) {x = other.x; y = other.y; z = other.z;};
            Vector cross(Vector other) {
                return Vector (
                    other.y * z - other.z * y,
                    other.z * x - other.x * z,
                    other.x * y - other.y * x
                );
            }
    };
}
