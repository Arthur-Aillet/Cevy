/*
** Agartha-Software, 2023
** Cevy
** File description:
** rotation
*/

#pragma once

#include "Vector.hpp"

namespace cevy::engine {
class Rotation : public cevy::engine::Vector {
  using cevy::engine::Vector::Vector;

  public:
  float &yaw() const { return const_cast<float &>(x); };
  float &pitch() const { return const_cast<float &>(y); };
  float &roll() const { return const_cast<float &>(z); };
  float &yaw() { return x; };
  float &pitch() { return y; };
  float &roll() { return z; };
};
} // namespace cevy::engine
