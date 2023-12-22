/*
** Agartha-Software, 2023
** Cevy
** File description:
** rotation
*/

#include <iostream>
#include <math.h>
#include <vector>

#pragma once

namespace cevy::engine {
class Rotation : public Vector {
  private:
  int direction = 0;

  public:
  Rotation(double x = 0, double y = 0, double z = 0) : Vector(x, y, z){};
  ~Rotation(){};
  void rotate(bool right) {
    if (right) {
      direction += 1;
    } else {
      direction -= 1;
    }
    if (direction < 0) {
      direction = 3;
    }
    direction %= 4;
  };
  Rotation fowards() {
    if (direction == 0) {
      return (Rotation(0.0, 1.0, 0.0));
    }
    if (direction == 1) {
      return (Rotation(0.0, 1.0, 0.0));
    }
    if (direction == 2) {
      return (Rotation(0.0, 1.0, 0.0));
    }
    if (direction == 3) {
      return (Rotation(0.0, 1.0, 0.0));
    }
    return (Rotation(1.0, 0.0, 0.0));
  };
  Rotation up() {
    if (direction == 0) {
      return (Rotation(0.0, 0.0, 1.0));
    }
    if (direction == 1) {
      return (Rotation(1.0, 0.0, 0.0));
    }
    if (direction == 2) {
      return (Rotation(0.0, 0.0, -1.0));
    }
    if (direction == 3) {
      return (Rotation(-1.0, 0.0, 0.0));
    }
    return (Rotation(1.0, 0.0, 0.0));
  }
  Rotation to_rad() {
    return Rotation(x * (M_PI / 180.0), y * (M_PI / 180.0), z * (M_PI / 180.0));
  };
};
} // namespace cevy::engine
