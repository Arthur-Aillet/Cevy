/*
** Agartha-Software, 2023
** C++evy
** File description:
** Camera
*/

#pragma once

#include "Position.hpp"
#include "Query.hpp"
#include "Rotation.hpp"
#include "cevy.hpp"
#include "raylib.hpp"
#include <optional>

namespace cevy::engine {
class Camera {
  public:
  Camera3D camera;
  operator Camera3D &();
  operator Camera3D *();
  operator Camera3D() const;
  Camera();
  ~Camera();
};
} // namespace cevy::engine

void update_camera(cevy::ecs::Query<cevy::engine::Camera, option<cevy::engine::Position>,
                                    option<cevy::engine::Rotation>>
                       cams);
