/*
** Agartha-Software, 2023
** Cevy
** File description:
** target
*/

#pragma once

#include "Vector.hpp"

namespace cevy::engine {
class Target : public cevy::engine::Vector {
  using cevy::engine::Vector::Vector;

  public:
  void operator=(const Vector &v) {
    x = v.x;
    y = v.y;
    z = v.z;
  }
};
} // namespace cevy::engine
