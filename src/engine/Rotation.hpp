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
  protected:
  Vector fwd;
  Vector tan;
  Vector cotan;

  public:
  float &yaw() const { return const_cast<float &>(x); };
  float &pitch() const { return const_cast<float &>(y); };
  float &roll() const { return const_cast<float &>(z); };
  float &yaw() { return x; };
  float &pitch() { return y; };
  float &roll() { return z; };

  Rotation& rotate(const Vector& v)
  {
    Vector::rotate(fwd * v.z);
    fwd = Vector(0, 0, 1);
    fwd.rotate(*this);
    Vector::rotate(tan * v.y);
    tan = Vector(0, 1, 0);
    tan.rotate(*this);
    Vector::rotate(cotan * v.x);
    cotan = Vector(1, 0, 0);
    cotan.rotate(*this);
    return *this;
  }

  void operator=(Vector &v) {
    x = v.x;
    y = v.y;
    z = v.z;
  }
};
} // namespace cevy::engine
