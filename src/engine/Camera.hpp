#pragma once

#include "raylib.h"

namespace Cevy {
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
