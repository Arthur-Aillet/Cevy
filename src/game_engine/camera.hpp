#pragma once

#include "raylib.h"

class CevyCamera
{
private:
    /* data */
public:
    Camera3D camera;
    operator Camera3D&();
    operator Camera3D*();
    operator Camera3D() const;
    CevyCamera(/* args */);
    ~CevyCamera();
};
