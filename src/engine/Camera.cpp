/*
** Agartha-Software, 2023
** Cevy
** File description:
** Camera
*/

#include "Camera.hpp"

cevy::engine::Camera::Camera() {
  this->camera = {
      (Vector3){10.0f, 10.0f, 10.0f},
      (Vector3){11.0f, 10.0f, 10.0f},
      (Vector3){0.0f, 1.0f, 0.0f},
      45.0f,
      CAMERA_PERSPECTIVE,
  };
}

cevy::engine::Camera::~Camera() {}

cevy::engine::Camera::operator Camera3D &() { return this->camera; }

cevy::engine::Camera::operator Camera3D *() { return &this->camera; }

cevy::engine::Camera::operator Camera3D() const { return this->camera; }

void update_camera(cevy::ecs::Query<cevy::engine::Camera, option<cevy::engine::Position>,
                                    option<cevy::engine::Rotation>>
                       cams) {
  for (auto [cam, opt_pos, opt_rot] : cams) {
    const auto &pos = opt_pos.value_or(cevy::engine::Position(0., 0., 0.));
    const auto &rot = opt_rot.value_or(cevy::engine::Rotation(0., 0., 0.));

    cam.camera.position = pos;

    float pitch = rot.pitch() * DEG2RAD;
    float yaw = rot.yaw() * DEG2RAD;

    Vector3 direction;
    direction.x = cos(yaw) * cos(pitch);
    direction.y = sin(pitch);
    direction.z = sin(yaw) * cos(pitch);
    Vector3 cameraFront = Vector3Normalize(direction);
    Vector3 cameraRight = Vector3Normalize(Vector3CrossProduct(cam.camera.up, cameraFront));
    Vector3 cameraUp = Vector3CrossProduct(direction, cameraRight);
    cam.camera.target = Vector3Add(cam.camera.position, cameraFront);
    cam.camera.up =
        Vector3RotateByAxisAngle(cevy::engine::Position(0, 1, 0), cameraFront, rot.z * DEG2RAD);
  }
}
