/*
** Agartha-Software, 2023
** Cevy
** File description:
** Camera
*/

#pragma once

#include "raylib.h"

namespace cevy::engine {
    class Camera {
        private:
            /* data */
        public:
            Camera3D camera;
            operator Camera3D&();
            operator Camera3D*();
            operator Camera3D() const;
            Camera();
            ~Camera();
    };
}
