/*
** Agartha-Software, 2023
** Cevy
** File description:
** Line
*/

#pragma once

#include "Vector.hpp"

namespace cevy::engine {
class Line {
  public:
  Vector start;
  Vector end;

  Line(Vector start, Vector end) : start(start), end(end) {}
  operator Ray() { return Ray{.position = start, .direction = end}; }
};
} // namespace cevy::engine
