#include "camera.hpp"
#include <iostream>

CevyCamera::CevyCamera()
{
    this->camera = { 0 };
    this->camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    this->camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    this->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    this->camera.fovy = 45.0f;
    this->camera.projection = CAMERA_PERSPECTIVE;
}

CevyCamera::~CevyCamera()
{
}

CevyCamera::operator Camera3D&() {
    return this->camera;
}

CevyCamera::operator Camera3D*() {
    return &this->camera;
}

CevyCamera::operator Camera3D() const {
    return this->camera;
}
